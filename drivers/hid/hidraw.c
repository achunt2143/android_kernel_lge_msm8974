<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * HID raw devices, giving access to raw HID events.
 *
 * In comparison to hiddev, this device does not process the
 * hid events at all (no parsing, no lookups). This lets applications
 * to work on raw hid events as they want to, and avoids a need to
 * use a transport-specific userspace libhid/libusb libraries.
 *
<<<<<<< HEAD
 *  Copyright (c) 2007 Jiri Kosina
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */
=======
 *  Copyright (c) 2007-2014 Jiri Kosina
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/hid.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/signal.h>
#include <linux/string.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/hidraw.h>

static int hidraw_major;
static struct cdev hidraw_cdev;
<<<<<<< HEAD
static struct class *hidraw_class;
static struct hidraw *hidraw_table[HIDRAW_MAX_DEVICES];
static DEFINE_MUTEX(minors_lock);
=======
static const struct class hidraw_class = {
	.name = "hidraw",
};
static struct hidraw *hidraw_table[HIDRAW_MAX_DEVICES];
static DECLARE_RWSEM(minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t hidraw_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	struct hidraw_list *list = file->private_data;
	int ret = 0, len;
	DECLARE_WAITQUEUE(wait, current);

	mutex_lock(&list->read_mutex);

	while (ret == 0) {
		if (list->head == list->tail) {
			add_wait_queue(&list->hidraw->wait, &wait);
			set_current_state(TASK_INTERRUPTIBLE);

			while (list->head == list->tail) {
<<<<<<< HEAD
				if (file->f_flags & O_NONBLOCK) {
					ret = -EAGAIN;
					break;
				}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (signal_pending(current)) {
					ret = -ERESTARTSYS;
					break;
				}
				if (!list->hidraw->exist) {
					ret = -EIO;
					break;
				}
<<<<<<< HEAD
=======
				if (file->f_flags & O_NONBLOCK) {
					ret = -EAGAIN;
					break;
				}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

				/* allow O_NONBLOCK to work well from other threads */
				mutex_unlock(&list->read_mutex);
				schedule();
				mutex_lock(&list->read_mutex);
				set_current_state(TASK_INTERRUPTIBLE);
			}

			set_current_state(TASK_RUNNING);
			remove_wait_queue(&list->hidraw->wait, &wait);
		}

		if (ret)
			goto out;

		len = list->buffer[list->tail].len > count ?
			count : list->buffer[list->tail].len;

		if (list->buffer[list->tail].value) {
			if (copy_to_user(buffer, list->buffer[list->tail].value, len)) {
				ret = -EFAULT;
				goto out;
			}
			ret = len;
		}

		kfree(list->buffer[list->tail].value);
		list->buffer[list->tail].value = NULL;
		list->tail = (list->tail + 1) & (HIDRAW_BUFFER_SIZE - 1);
	}
out:
	mutex_unlock(&list->read_mutex);
	return ret;
}

<<<<<<< HEAD
/* The first byte is expected to be a report number.
 * This function is to be called with the minors_lock mutex held */
static ssize_t hidraw_send_report(struct file *file, const char __user *buffer, size_t count, unsigned char report_type)
{
	unsigned int minor = iminor(file->f_path.dentry->d_inode);
=======
/*
 * The first byte of the report buffer is expected to be a report number.
 */
static ssize_t hidraw_send_report(struct file *file, const char __user *buffer, size_t count, unsigned char report_type)
{
	unsigned int minor = iminor(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hid_device *dev;
	__u8 *buf;
	int ret = 0;

<<<<<<< HEAD
=======
	lockdep_assert_held(&minors_rwsem);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!hidraw_table[minor] || !hidraw_table[minor]->exist) {
		ret = -ENODEV;
		goto out;
	}

	dev = hidraw_table[minor]->hid;

<<<<<<< HEAD
	if (!dev->hid_output_raw_report) {
		ret = -ENODEV;
		goto out;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > HID_MAX_BUFFER_SIZE) {
		hid_warn(dev, "pid %d passed too large report\n",
			 task_pid_nr(current));
		ret = -EINVAL;
		goto out;
	}

	if (count < 2) {
		hid_warn(dev, "pid %d passed too short report\n",
			 task_pid_nr(current));
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	buf = kmalloc(count * sizeof(__u8), GFP_KERNEL);
	if (!buf) {
		ret = -ENOMEM;
		goto out;
	}

	if (copy_from_user(buf, buffer, count)) {
		ret = -EFAULT;
		goto out_free;
	}

	ret = dev->hid_output_raw_report(dev, buf, count, report_type);
=======
	buf = memdup_user(buffer, count);
	if (IS_ERR(buf)) {
		ret = PTR_ERR(buf);
		goto out;
	}

	if ((report_type == HID_OUTPUT_REPORT) &&
	    !(dev->quirks & HID_QUIRK_NO_OUTPUT_REPORTS_ON_INTR_EP)) {
		ret = hid_hw_output_report(dev, buf, count);
		/*
		 * compatibility with old implementation of USB-HID and I2C-HID:
		 * if the device does not support receiving output reports,
		 * on an interrupt endpoint, fallback to SET_REPORT HID command.
		 */
		if (ret != -ENOSYS)
			goto out_free;
	}

	ret = hid_hw_raw_request(dev, buf[0], buf, count, report_type,
				HID_REQ_SET_REPORT);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_free:
	kfree(buf);
out:
	return ret;
}

<<<<<<< HEAD
/* the first byte is expected to be a report number */
static ssize_t hidraw_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
	ssize_t ret;
	mutex_lock(&minors_lock);
	ret = hidraw_send_report(file, buffer, count, HID_OUTPUT_REPORT);
	mutex_unlock(&minors_lock);
=======
static ssize_t hidraw_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
	ssize_t ret;
	down_read(&minors_rwsem);
	ret = hidraw_send_report(file, buffer, count, HID_OUTPUT_REPORT);
	up_read(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}


<<<<<<< HEAD
/* This function performs a Get_Report transfer over the control endpoint
 * per section 7.2.1 of the HID specification, version 1.1.  The first byte
 * of buffer is the report number to request, or 0x0 if the defice does not
 * use numbered reports. The report_type parameter can be HID_FEATURE_REPORT
 * or HID_INPUT_REPORT.  This function is to be called with the minors_lock
 *  mutex held. */
static ssize_t hidraw_get_report(struct file *file, char __user *buffer, size_t count, unsigned char report_type)
{
	unsigned int minor = iminor(file->f_path.dentry->d_inode);
=======
/*
 * This function performs a Get_Report transfer over the control endpoint
 * per section 7.2.1 of the HID specification, version 1.1.  The first byte
 * of buffer is the report number to request, or 0x0 if the device does not
 * use numbered reports. The report_type parameter can be HID_FEATURE_REPORT
 * or HID_INPUT_REPORT.
 */
static ssize_t hidraw_get_report(struct file *file, char __user *buffer, size_t count, unsigned char report_type)
{
	unsigned int minor = iminor(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hid_device *dev;
	__u8 *buf;
	int ret = 0, len;
	unsigned char report_number;

<<<<<<< HEAD
	dev = hidraw_table[minor]->hid;

	if (!dev->hid_get_raw_report) {
=======
	lockdep_assert_held(&minors_rwsem);

	if (!hidraw_table[minor] || !hidraw_table[minor]->exist) {
		ret = -ENODEV;
		goto out;
	}

	dev = hidraw_table[minor]->hid;

	if (!dev->ll_driver->raw_request) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENODEV;
		goto out;
	}

	if (count > HID_MAX_BUFFER_SIZE) {
<<<<<<< HEAD
		printk(KERN_WARNING "hidraw: pid %d passed too large report\n",
				task_pid_nr(current));
=======
		hid_warn(dev, "pid %d passed too large report\n",
			task_pid_nr(current));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

	if (count < 2) {
<<<<<<< HEAD
		printk(KERN_WARNING "hidraw: pid %d passed too short report\n",
				task_pid_nr(current));
=======
		hid_warn(dev, "pid %d passed too short report\n",
			task_pid_nr(current));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	buf = kmalloc(count * sizeof(__u8), GFP_KERNEL);
=======
	buf = kmalloc(count, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!buf) {
		ret = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	/* Read the first byte from the user. This is the report number,
	 * which is passed to dev->hid_get_raw_report(). */
=======
	/*
	 * Read the first byte from the user. This is the report number,
	 * which is passed to hid_hw_raw_request().
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_from_user(&report_number, buffer, 1)) {
		ret = -EFAULT;
		goto out_free;
	}

<<<<<<< HEAD
	ret = dev->hid_get_raw_report(dev, report_number, buf, count, report_type);
=======
	ret = hid_hw_raw_request(dev, report_number, buf, count, report_type,
				 HID_REQ_GET_REPORT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret < 0)
		goto out_free;

	len = (ret < count) ? ret : count;

	if (copy_to_user(buffer, buf, len)) {
		ret = -EFAULT;
		goto out_free;
	}

	ret = len;

out_free:
	kfree(buf);
out:
	return ret;
}

<<<<<<< HEAD
static unsigned int hidraw_poll(struct file *file, poll_table *wait)
{
	struct hidraw_list *list = file->private_data;

	poll_wait(file, &list->hidraw->wait, wait);
	if (list->head != list->tail)
		return POLLIN | POLLRDNORM;
	if (!list->hidraw->exist)
		return POLLERR | POLLHUP;
	return 0;
=======
static __poll_t hidraw_poll(struct file *file, poll_table *wait)
{
	struct hidraw_list *list = file->private_data;
	__poll_t mask = EPOLLOUT | EPOLLWRNORM; /* hidraw is always writable */

	poll_wait(file, &list->hidraw->wait, wait);
	if (list->head != list->tail)
		mask |= EPOLLIN | EPOLLRDNORM;
	if (!list->hidraw->exist)
		mask |= EPOLLERR | EPOLLHUP;
	return mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hidraw_open(struct inode *inode, struct file *file)
{
	unsigned int minor = iminor(inode);
	struct hidraw *dev;
	struct hidraw_list *list;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	if (!(list = kzalloc(sizeof(struct hidraw_list), GFP_KERNEL))) {
		err = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	mutex_lock(&minors_lock);
=======
	/*
	 * Technically not writing to the hidraw_table but a write lock is
	 * required to protect the device refcount. This is symmetrical to
	 * hidraw_release().
	 */
	down_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!hidraw_table[minor] || !hidraw_table[minor]->exist) {
		err = -ENODEV;
		goto out_unlock;
	}

<<<<<<< HEAD
	list->hidraw = hidraw_table[minor];
	mutex_init(&list->read_mutex);
	list_add_tail(&list->node, &hidraw_table[minor]->list);
	file->private_data = list;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev = hidraw_table[minor];
	if (!dev->open++) {
		err = hid_hw_power(dev->hid, PM_HINT_FULLON);
		if (err < 0) {
			dev->open--;
			goto out_unlock;
		}

		err = hid_hw_open(dev->hid);
		if (err < 0) {
			hid_hw_power(dev->hid, PM_HINT_NORMAL);
			dev->open--;
<<<<<<< HEAD
		}
	}

out_unlock:
	mutex_unlock(&minors_lock);
=======
			goto out_unlock;
		}
	}

	list->hidraw = hidraw_table[minor];
	mutex_init(&list->read_mutex);
	spin_lock_irqsave(&hidraw_table[minor]->list_lock, flags);
	list_add_tail(&list->node, &hidraw_table[minor]->list);
	spin_unlock_irqrestore(&hidraw_table[minor]->list_lock, flags);
	file->private_data = list;
out_unlock:
	up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	if (err < 0)
		kfree(list);
	return err;

}

<<<<<<< HEAD
static void drop_ref(struct hidraw *hidraw, int exists_bit)
{
	if (exists_bit) {
		hid_hw_close(hidraw->hid);
		hidraw->exist = 0;
		if (hidraw->open)
			wake_up_interruptible(&hidraw->wait);
	} else {
		--hidraw->open;
	}

	if (!hidraw->open && !hidraw->exist) {
		device_destroy(hidraw_class, MKDEV(hidraw_major, hidraw->minor));
		hidraw_table[hidraw->minor] = NULL;
		kfree(hidraw);
=======
static int hidraw_fasync(int fd, struct file *file, int on)
{
	struct hidraw_list *list = file->private_data;

	return fasync_helper(fd, file, on, &list->fasync);
}

static void drop_ref(struct hidraw *hidraw, int exists_bit)
{
	if (exists_bit) {
		hidraw->exist = 0;
		if (hidraw->open) {
			hid_hw_close(hidraw->hid);
			wake_up_interruptible(&hidraw->wait);
		}
		device_destroy(&hidraw_class,
			       MKDEV(hidraw_major, hidraw->minor));
	} else {
		--hidraw->open;
	}
	if (!hidraw->open) {
		if (!hidraw->exist) {
			hidraw_table[hidraw->minor] = NULL;
			kfree(hidraw);
		} else {
			/* close device for last reader */
			hid_hw_close(hidraw->hid);
			hid_hw_power(hidraw->hid, PM_HINT_NORMAL);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int hidraw_release(struct inode * inode, struct file * file)
{
	unsigned int minor = iminor(inode);
	struct hidraw_list *list = file->private_data;
<<<<<<< HEAD

	mutex_lock(&minors_lock);

	list_del(&list->node);
=======
	unsigned long flags;

	down_write(&minors_rwsem);

	spin_lock_irqsave(&hidraw_table[minor]->list_lock, flags);
	while (list->tail != list->head) {
		kfree(list->buffer[list->tail].value);
		list->buffer[list->tail].value = NULL;
		list->tail = (list->tail + 1) & (HIDRAW_BUFFER_SIZE - 1);
	}
	list_del(&list->node);
	spin_unlock_irqrestore(&hidraw_table[minor]->list_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(list);

	drop_ref(hidraw_table[minor], 0);

<<<<<<< HEAD
	mutex_unlock(&minors_lock);
=======
	up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static long hidraw_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int minor = iminor(inode);
	long ret = 0;
	struct hidraw *dev;
	void __user *user_arg = (void __user*) arg;

<<<<<<< HEAD
	mutex_lock(&minors_lock);
	dev = hidraw_table[minor];
	if (!dev) {
=======
	down_read(&minors_rwsem);
	dev = hidraw_table[minor];
	if (!dev || !dev->exist) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENODEV;
		goto out;
	}

	switch (cmd) {
		case HIDIOCGRDESCSIZE:
			if (put_user(dev->hid->rsize, (int __user *)arg))
				ret = -EFAULT;
			break;

		case HIDIOCGRDESC:
			{
				__u32 len;

				if (get_user(len, (int __user *)arg))
					ret = -EFAULT;
				else if (len > HID_MAX_DESCRIPTOR_SIZE - 1)
					ret = -EINVAL;
				else if (copy_to_user(user_arg + offsetof(
					struct hidraw_report_descriptor,
					value[0]),
					dev->hid->rdesc,
					min(dev->hid->rsize, len)))
					ret = -EFAULT;
				break;
			}
		case HIDIOCGRAWINFO:
			{
				struct hidraw_devinfo dinfo;

				dinfo.bustype = dev->hid->bus;
				dinfo.vendor = dev->hid->vendor;
				dinfo.product = dev->hid->product;
				if (copy_to_user(user_arg, &dinfo, sizeof(dinfo)))
					ret = -EFAULT;
				break;
			}
		default:
			{
				struct hid_device *hid = dev->hid;
				if (_IOC_TYPE(cmd) != 'H') {
					ret = -EINVAL;
					break;
				}

				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCSFEATURE(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_send_report(file, user_arg, len, HID_FEATURE_REPORT);
					break;
				}
				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGFEATURE(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_get_report(file, user_arg, len, HID_FEATURE_REPORT);
					break;
				}

<<<<<<< HEAD
=======
				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCSINPUT(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_send_report(file, user_arg, len, HID_INPUT_REPORT);
					break;
				}
				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGINPUT(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_get_report(file, user_arg, len, HID_INPUT_REPORT);
					break;
				}

				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCSOUTPUT(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_send_report(file, user_arg, len, HID_OUTPUT_REPORT);
					break;
				}
				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGOUTPUT(0))) {
					int len = _IOC_SIZE(cmd);
					ret = hidraw_get_report(file, user_arg, len, HID_OUTPUT_REPORT);
					break;
				}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/* Begin Read-only ioctls. */
				if (_IOC_DIR(cmd) != _IOC_READ) {
					ret = -EINVAL;
					break;
				}

				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGRAWNAME(0))) {
					int len = strlen(hid->name) + 1;
					if (len > _IOC_SIZE(cmd))
						len = _IOC_SIZE(cmd);
					ret = copy_to_user(user_arg, hid->name, len) ?
						-EFAULT : len;
					break;
				}

				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGRAWPHYS(0))) {
					int len = strlen(hid->phys) + 1;
					if (len > _IOC_SIZE(cmd))
						len = _IOC_SIZE(cmd);
					ret = copy_to_user(user_arg, hid->phys, len) ?
						-EFAULT : len;
					break;
				}
<<<<<<< HEAD
=======

				if (_IOC_NR(cmd) == _IOC_NR(HIDIOCGRAWUNIQ(0))) {
					int len = strlen(hid->uniq) + 1;
					if (len > _IOC_SIZE(cmd))
						len = _IOC_SIZE(cmd);
					ret = copy_to_user(user_arg, hid->uniq, len) ?
						-EFAULT : len;
					break;
				}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}

		ret = -ENOTTY;
	}
out:
<<<<<<< HEAD
	mutex_unlock(&minors_lock);
=======
	up_read(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct file_operations hidraw_ops = {
	.owner =        THIS_MODULE,
	.read =         hidraw_read,
	.write =        hidraw_write,
	.poll =         hidraw_poll,
	.open =         hidraw_open,
	.release =      hidraw_release,
	.unlocked_ioctl = hidraw_ioctl,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
	.compat_ioctl   = hidraw_ioctl,
#endif
=======
	.fasync =	hidraw_fasync,
	.compat_ioctl   = compat_ptr_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek =	noop_llseek,
};

int hidraw_report_event(struct hid_device *hid, u8 *data, int len)
{
	struct hidraw *dev = hid->hidraw;
	struct hidraw_list *list;
	int ret = 0;
<<<<<<< HEAD

=======
	unsigned long flags;

	spin_lock_irqsave(&dev->list_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(list, &dev->list, node) {
		int new_head = (list->head + 1) & (HIDRAW_BUFFER_SIZE - 1);

		if (new_head == list->tail)
			continue;

		if (!(list->buffer[list->head].value = kmemdup(data, len, GFP_ATOMIC))) {
			ret = -ENOMEM;
			break;
		}
		list->buffer[list->head].len = len;
		list->head = new_head;
		kill_fasync(&list->fasync, SIGIO, POLL_IN);
	}
<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&dev->list_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wake_up_interruptible(&dev->wait);
	return ret;
}
EXPORT_SYMBOL_GPL(hidraw_report_event);

int hidraw_connect(struct hid_device *hid)
{
	int minor, result;
	struct hidraw *dev;

<<<<<<< HEAD
	/* we accept any HID device, no matter the applications */
=======
	/* we accept any HID device, all applications */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev = kzalloc(sizeof(struct hidraw), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	result = -EINVAL;

<<<<<<< HEAD
	mutex_lock(&minors_lock);
=======
	down_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (minor = 0; minor < HIDRAW_MAX_DEVICES; minor++) {
		if (hidraw_table[minor])
			continue;
		hidraw_table[minor] = dev;
		result = 0;
		break;
	}

	if (result) {
<<<<<<< HEAD
		mutex_unlock(&minors_lock);
=======
		up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(dev);
		goto out;
	}

<<<<<<< HEAD
	dev->dev = device_create(hidraw_class, &hid->dev, MKDEV(hidraw_major, minor),
=======
	dev->dev = device_create(&hidraw_class, &hid->dev, MKDEV(hidraw_major, minor),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 NULL, "%s%d", "hidraw", minor);

	if (IS_ERR(dev->dev)) {
		hidraw_table[minor] = NULL;
<<<<<<< HEAD
		mutex_unlock(&minors_lock);
=======
		up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = PTR_ERR(dev->dev);
		kfree(dev);
		goto out;
	}

<<<<<<< HEAD
	mutex_unlock(&minors_lock);
	init_waitqueue_head(&dev->wait);
=======
	init_waitqueue_head(&dev->wait);
	spin_lock_init(&dev->list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&dev->list);

	dev->hid = hid;
	dev->minor = minor;

	dev->exist = 1;
	hid->hidraw = dev;

<<<<<<< HEAD
=======
	up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return result;

}
EXPORT_SYMBOL_GPL(hidraw_connect);

void hidraw_disconnect(struct hid_device *hid)
{
	struct hidraw *hidraw = hid->hidraw;

<<<<<<< HEAD
	mutex_lock(&minors_lock);

	drop_ref(hidraw, 1);

	mutex_unlock(&minors_lock);
=======
	down_write(&minors_rwsem);

	drop_ref(hidraw, 1);

	up_write(&minors_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(hidraw_disconnect);

int __init hidraw_init(void)
{
	int result;
	dev_t dev_id;

	result = alloc_chrdev_region(&dev_id, HIDRAW_FIRST_MINOR,
			HIDRAW_MAX_DEVICES, "hidraw");
<<<<<<< HEAD

	hidraw_major = MAJOR(dev_id);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (result < 0) {
		pr_warn("can't get major number\n");
		goto out;
	}

<<<<<<< HEAD
	hidraw_class = class_create(THIS_MODULE, "hidraw");
	if (IS_ERR(hidraw_class)) {
		result = PTR_ERR(hidraw_class);
		goto error_cdev;
	}
=======
	hidraw_major = MAJOR(dev_id);

	result = class_register(&hidraw_class);
	if (result)
		goto error_cdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

        cdev_init(&hidraw_cdev, &hidraw_ops);
	result = cdev_add(&hidraw_cdev, dev_id, HIDRAW_MAX_DEVICES);
	if (result < 0)
		goto error_class;

<<<<<<< HEAD
=======
	pr_info("raw HID events driver (C) Jiri Kosina\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return result;

error_class:
<<<<<<< HEAD
	class_destroy(hidraw_class);
=======
	class_unregister(&hidraw_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error_cdev:
	unregister_chrdev_region(dev_id, HIDRAW_MAX_DEVICES);
	goto out;
}

void hidraw_exit(void)
{
	dev_t dev_id = MKDEV(hidraw_major, 0);

	cdev_del(&hidraw_cdev);
<<<<<<< HEAD
	class_destroy(hidraw_class);
=======
	class_unregister(&hidraw_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_chrdev_region(dev_id, HIDRAW_MAX_DEVICES);

}
