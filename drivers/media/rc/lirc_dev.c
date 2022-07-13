<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * LIRC base driver
 *
 * by Artur Lipowski <alipowski@interia.pl>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/completion.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/unistd.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <media/lirc.h>
#include <media/lirc_dev.h>

static bool debug;

#define IRCTL_DEV_NAME	"BaseRemoteCtl"
#define NOPLUG		-1
#define LOGHEAD		"lirc_dev (%s[%d]): "

static dev_t lirc_base_dev;

struct irctl {
	struct lirc_driver d;
	int attached;
	int open;

	struct mutex irctl_lock;
	struct lirc_buffer *buf;
	unsigned int chunk_size;

	struct cdev *cdev;

	struct task_struct *task;
	long jiffies_to_wait;
};

static DEFINE_MUTEX(lirc_dev_lock);

static struct irctl *irctls[MAX_IRCTL_DEVICES];
=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/file.h>
#include <linux/idr.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "rc-core-priv.h"
#include <uapi/linux/lirc.h>

#define LIRCBUF_SIZE	1024

static dev_t lirc_base_dev;

/* Used to keep track of allocated lirc devices */
static DEFINE_IDA(lirc_ida);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Only used for sysfs but defined to void otherwise */
static struct class *lirc_class;

<<<<<<< HEAD
/*  helper function
 *  initializes the irctl structure
 */
static void lirc_irctl_init(struct irctl *ir)
{
	mutex_init(&ir->irctl_lock);
	ir->d.minor = NOPLUG;
}

static void lirc_irctl_cleanup(struct irctl *ir)
{
	dev_dbg(ir->d.dev, LOGHEAD "cleaning up\n", ir->d.name, ir->d.minor);

	device_destroy(lirc_class, MKDEV(MAJOR(lirc_base_dev), ir->d.minor));

	if (ir->buf != ir->d.rbuf) {
		lirc_buffer_free(ir->buf);
		kfree(ir->buf);
	}
	ir->buf = NULL;
}

/*  helper function
 *  reads key codes from driver and puts them into buffer
 *  returns 0 on success
 */
static int lirc_add_to_buf(struct irctl *ir)
{
	if (ir->d.add_to_buf) {
		int res = -ENODATA;
		int got_data = 0;

		/*
		 * service the device as long as it is returning
		 * data and we have space
		 */
get_data:
		res = ir->d.add_to_buf(ir->d.data, ir->buf);
		if (res == 0) {
			got_data++;
			goto get_data;
		}

		if (res == -ENODEV)
			kthread_stop(ir->task);

		return got_data ? 0 : res;
	}

	return 0;
}

/* main function of the polling thread
 */
static int lirc_thread(void *irctl)
{
	struct irctl *ir = irctl;

	dev_dbg(ir->d.dev, LOGHEAD "poll thread started\n",
		ir->d.name, ir->d.minor);

	do {
		if (ir->open) {
			if (ir->jiffies_to_wait) {
				set_current_state(TASK_INTERRUPTIBLE);
				schedule_timeout(ir->jiffies_to_wait);
			}
			if (kthread_should_stop())
				break;
			if (!lirc_add_to_buf(ir))
				wake_up_interruptible(&ir->buf->wait_poll);
		} else {
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
		}
	} while (!kthread_should_stop());

	dev_dbg(ir->d.dev, LOGHEAD "poll thread ended\n",
		ir->d.name, ir->d.minor);

	return 0;
}


static struct file_operations lirc_dev_fops = {
	.owner		= THIS_MODULE,
	.read		= lirc_dev_fop_read,
	.write		= lirc_dev_fop_write,
	.poll		= lirc_dev_fop_poll,
	.unlocked_ioctl	= lirc_dev_fop_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= lirc_dev_fop_ioctl,
#endif
	.open		= lirc_dev_fop_open,
	.release	= lirc_dev_fop_close,
	.llseek		= noop_llseek,
};

static int lirc_cdev_add(struct irctl *ir)
{
	int retval = -ENOMEM;
	struct lirc_driver *d = &ir->d;
	struct cdev *cdev;

	cdev = kzalloc(sizeof(*cdev), GFP_KERNEL);
	if (!cdev)
		goto err_out;

	if (d->fops) {
		cdev_init(cdev, d->fops);
		cdev->owner = d->owner;
	} else {
		cdev_init(cdev, &lirc_dev_fops);
		cdev->owner = THIS_MODULE;
	}
	retval = kobject_set_name(&cdev->kobj, "lirc%d", d->minor);
	if (retval)
		goto err_out;

	retval = cdev_add(cdev, MKDEV(MAJOR(lirc_base_dev), d->minor), 1);
	if (retval) {
		kobject_put(&cdev->kobj);
		goto err_out;
	}

	ir->cdev = cdev;

	return 0;

err_out:
	kfree(cdev);
	return retval;
}

int lirc_register_driver(struct lirc_driver *d)
{
	struct irctl *ir;
	int minor;
	int bytes_in_key;
	unsigned int chunk_size;
	unsigned int buffer_size;
	int err;

	if (!d) {
		printk(KERN_ERR "lirc_dev: lirc_register_driver: "
		       "driver pointer must be not NULL!\n");
		err = -EBADRQC;
		goto out;
	}

	if (!d->dev) {
		printk(KERN_ERR "%s: dev pointer not filled in!\n", __func__);
		err = -EINVAL;
		goto out;
	}

	if (MAX_IRCTL_DEVICES <= d->minor) {
		dev_err(d->dev, "lirc_dev: lirc_register_driver: "
			"\"minor\" must be between 0 and %d (%d)!\n",
			MAX_IRCTL_DEVICES - 1, d->minor);
		err = -EBADRQC;
		goto out;
	}

	if (1 > d->code_length || (BUFLEN * 8) < d->code_length) {
		dev_err(d->dev, "lirc_dev: lirc_register_driver: "
			"code length in bits for minor (%d) "
			"must be less than %d!\n",
			d->minor, BUFLEN * 8);
		err = -EBADRQC;
		goto out;
	}

	dev_dbg(d->dev, "lirc_dev: lirc_register_driver: sample_rate: %d\n",
		d->sample_rate);
	if (d->sample_rate) {
		if (2 > d->sample_rate || HZ < d->sample_rate) {
			dev_err(d->dev, "lirc_dev: lirc_register_driver: "
				"sample_rate must be between 2 and %d!\n", HZ);
			err = -EBADRQC;
			goto out;
		}
		if (!d->add_to_buf) {
			dev_err(d->dev, "lirc_dev: lirc_register_driver: "
				"add_to_buf cannot be NULL when "
				"sample_rate is set\n");
			err = -EBADRQC;
			goto out;
		}
	} else if (!(d->fops && d->fops->read) && !d->rbuf) {
		dev_err(d->dev, "lirc_dev: lirc_register_driver: "
			"fops->read and rbuf cannot all be NULL!\n");
		err = -EBADRQC;
		goto out;
	} else if (!d->rbuf) {
		if (!(d->fops && d->fops->read && d->fops->poll &&
		      d->fops->unlocked_ioctl)) {
			dev_err(d->dev, "lirc_dev: lirc_register_driver: "
				"neither read, poll nor unlocked_ioctl can be NULL!\n");
			err = -EBADRQC;
			goto out;
		}
	}

	mutex_lock(&lirc_dev_lock);

	minor = d->minor;

	if (minor < 0) {
		/* find first free slot for driver */
		for (minor = 0; minor < MAX_IRCTL_DEVICES; minor++)
			if (!irctls[minor])
				break;
		if (MAX_IRCTL_DEVICES == minor) {
			dev_err(d->dev, "lirc_dev: lirc_register_driver: "
				"no free slots for drivers!\n");
			err = -ENOMEM;
			goto out_lock;
		}
	} else if (irctls[minor]) {
		dev_err(d->dev, "lirc_dev: lirc_register_driver: "
			"minor (%d) just registered!\n", minor);
		err = -EBUSY;
		goto out_lock;
	}

	ir = kzalloc(sizeof(struct irctl), GFP_KERNEL);
	if (!ir) {
		err = -ENOMEM;
		goto out_lock;
	}
	lirc_irctl_init(ir);
	irctls[minor] = ir;
	d->minor = minor;

	if (d->sample_rate) {
		ir->jiffies_to_wait = HZ / d->sample_rate;
	} else {
		/* it means - wait for external event in task queue */
		ir->jiffies_to_wait = 0;
	}

	/* some safety check 8-) */
	d->name[sizeof(d->name)-1] = '\0';

	bytes_in_key = BITS_TO_LONGS(d->code_length) +
			(d->code_length % 8 ? 1 : 0);
	buffer_size = d->buffer_size ? d->buffer_size : BUFLEN / bytes_in_key;
	chunk_size  = d->chunk_size  ? d->chunk_size  : bytes_in_key;

	if (d->rbuf) {
		ir->buf = d->rbuf;
	} else {
		ir->buf = kmalloc(sizeof(struct lirc_buffer), GFP_KERNEL);
		if (!ir->buf) {
			err = -ENOMEM;
			goto out_lock;
		}
		err = lirc_buffer_init(ir->buf, chunk_size, buffer_size);
		if (err) {
			kfree(ir->buf);
			goto out_lock;
		}
	}
	ir->chunk_size = ir->buf->chunk_size;

	if (d->features == 0)
		d->features = LIRC_CAN_REC_LIRCCODE;

	ir->d = *d;

	device_create(lirc_class, ir->d.dev,
		      MKDEV(MAJOR(lirc_base_dev), ir->d.minor), NULL,
		      "lirc%u", ir->d.minor);

	if (d->sample_rate) {
		/* try to fire up polling thread */
		ir->task = kthread_run(lirc_thread, (void *)ir, "lirc_dev");
		if (IS_ERR(ir->task)) {
			dev_err(d->dev, "lirc_dev: lirc_register_driver: "
				"cannot run poll thread for minor = %d\n",
				d->minor);
			err = -ECHILD;
			goto out_sysfs;
		}
	}

	err = lirc_cdev_add(ir);
	if (err)
		goto out_sysfs;

	ir->attached = 1;
	mutex_unlock(&lirc_dev_lock);

	dev_info(ir->d.dev, "lirc_dev: driver %s registered at minor = %d\n",
		 ir->d.name, ir->d.minor);
	return minor;

out_sysfs:
	device_destroy(lirc_class, MKDEV(MAJOR(lirc_base_dev), ir->d.minor));
out_lock:
	mutex_unlock(&lirc_dev_lock);
out:
	return err;
}
EXPORT_SYMBOL(lirc_register_driver);

int lirc_unregister_driver(int minor)
{
	struct irctl *ir;
	struct cdev *cdev;

	if (minor < 0 || minor >= MAX_IRCTL_DEVICES) {
		printk(KERN_ERR "lirc_dev: %s: minor (%d) must be between "
		       "0 and %d!\n", __func__, minor, MAX_IRCTL_DEVICES - 1);
		return -EBADRQC;
	}

	ir = irctls[minor];
	if (!ir) {
		printk(KERN_ERR "lirc_dev: %s: failed to get irctl struct "
		       "for minor %d!\n", __func__, minor);
		return -ENOENT;
	}

	cdev = ir->cdev;

	mutex_lock(&lirc_dev_lock);

	if (ir->d.minor != minor) {
		printk(KERN_ERR "lirc_dev: %s: minor (%d) device not "
		       "registered!\n", __func__, minor);
		mutex_unlock(&lirc_dev_lock);
		return -ENOENT;
	}

	/* end up polling thread */
	if (ir->task)
		kthread_stop(ir->task);

	dev_dbg(ir->d.dev, "lirc_dev: driver %s unregistered from minor = %d\n",
		ir->d.name, ir->d.minor);

	ir->attached = 0;
	if (ir->open) {
		dev_dbg(ir->d.dev, LOGHEAD "releasing opened driver\n",
			ir->d.name, ir->d.minor);
		wake_up_interruptible(&ir->buf->wait_poll);
		mutex_lock(&ir->irctl_lock);
		ir->d.set_use_dec(ir->d.data);
		module_put(cdev->owner);
		mutex_unlock(&ir->irctl_lock);
	} else {
		lirc_irctl_cleanup(ir);
		cdev_del(cdev);
		kfree(cdev);
		kfree(ir);
		irctls[minor] = NULL;
	}

	mutex_unlock(&lirc_dev_lock);

	return 0;
}
EXPORT_SYMBOL(lirc_unregister_driver);

int lirc_dev_fop_open(struct inode *inode, struct file *file)
{
	struct irctl *ir;
	struct cdev *cdev;
	int retval = 0;

	if (iminor(inode) >= MAX_IRCTL_DEVICES) {
		printk(KERN_WARNING "lirc_dev [%d]: open result = -ENODEV\n",
		       iminor(inode));
		return -ENODEV;
	}

	if (mutex_lock_interruptible(&lirc_dev_lock))
		return -ERESTARTSYS;

	ir = irctls[iminor(inode)];
	if (!ir) {
		retval = -ENODEV;
		goto error;
	}

	dev_dbg(ir->d.dev, LOGHEAD "open called\n", ir->d.name, ir->d.minor);

	if (ir->d.minor == NOPLUG) {
		retval = -ENODEV;
		goto error;
	}

	if (ir->open) {
		retval = -EBUSY;
		goto error;
	}

	cdev = ir->cdev;
	if (try_module_get(cdev->owner)) {
		ir->open++;
		retval = ir->d.set_use_inc(ir->d.data);

		if (retval) {
			module_put(cdev->owner);
			ir->open--;
		} else {
			lirc_buffer_clear(ir->buf);
		}
		if (ir->task)
			wake_up_process(ir->task);
	}

error:
	if (ir)
		dev_dbg(ir->d.dev, LOGHEAD "open result = %d\n",
			ir->d.name, ir->d.minor, retval);

	mutex_unlock(&lirc_dev_lock);

	nonseekable_open(inode, file);

	return retval;
}
EXPORT_SYMBOL(lirc_dev_fop_open);

int lirc_dev_fop_close(struct inode *inode, struct file *file)
{
	struct irctl *ir = irctls[iminor(inode)];
	struct cdev *cdev;

	if (!ir) {
		printk(KERN_ERR "%s: called with invalid irctl\n", __func__);
		return -EINVAL;
	}

	cdev = ir->cdev;

	dev_dbg(ir->d.dev, LOGHEAD "close called\n", ir->d.name, ir->d.minor);

	WARN_ON(mutex_lock_killable(&lirc_dev_lock));

	ir->open--;
	if (ir->attached) {
		ir->d.set_use_dec(ir->d.data);
		module_put(cdev->owner);
	} else {
		lirc_irctl_cleanup(ir);
		cdev_del(cdev);
		irctls[ir->d.minor] = NULL;
		kfree(cdev);
		kfree(ir);
	}

	mutex_unlock(&lirc_dev_lock);

	return 0;
}
EXPORT_SYMBOL(lirc_dev_fop_close);

unsigned int lirc_dev_fop_poll(struct file *file, poll_table *wait)
{
	struct irctl *ir = irctls[iminor(file->f_dentry->d_inode)];
	unsigned int ret;

	if (!ir) {
		printk(KERN_ERR "%s: called with invalid irctl\n", __func__);
		return POLLERR;
	}

	dev_dbg(ir->d.dev, LOGHEAD "poll called\n", ir->d.name, ir->d.minor);

	if (!ir->attached)
		return POLLERR;

	poll_wait(file, &ir->buf->wait_poll, wait);

	if (ir->buf)
		if (lirc_buffer_empty(ir->buf))
			ret = 0;
		else
			ret = POLLIN | POLLRDNORM;
	else
		ret = POLLERR;

	dev_dbg(ir->d.dev, LOGHEAD "poll result = %d\n",
		ir->d.name, ir->d.minor, ret);

	return ret;
}
EXPORT_SYMBOL(lirc_dev_fop_poll);

long lirc_dev_fop_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	__u32 mode;
	int result = 0;
	struct irctl *ir = irctls[iminor(file->f_dentry->d_inode)];

	if (!ir) {
		printk(KERN_ERR "lirc_dev: %s: no irctl found!\n", __func__);
		return -ENODEV;
	}

	dev_dbg(ir->d.dev, LOGHEAD "ioctl called (0x%x)\n",
		ir->d.name, ir->d.minor, cmd);

	if (ir->d.minor == NOPLUG || !ir->attached) {
		dev_dbg(ir->d.dev, LOGHEAD "ioctl result = -ENODEV\n",
			ir->d.name, ir->d.minor);
		return -ENODEV;
	}

	mutex_lock(&ir->irctl_lock);

	switch (cmd) {
	case LIRC_GET_FEATURES:
		result = put_user(ir->d.features, (__u32 *)arg);
		break;
	case LIRC_GET_REC_MODE:
		if (!(ir->d.features & LIRC_CAN_REC_MASK)) {
			result = -ENOSYS;
			break;
		}

		result = put_user(LIRC_REC2MODE
				  (ir->d.features & LIRC_CAN_REC_MASK),
				  (__u32 *)arg);
		break;
	case LIRC_SET_REC_MODE:
		if (!(ir->d.features & LIRC_CAN_REC_MASK)) {
			result = -ENOSYS;
			break;
		}

		result = get_user(mode, (__u32 *)arg);
		if (!result && !(LIRC_MODE2REC(mode) & ir->d.features))
			result = -EINVAL;
		/*
		 * FIXME: We should actually set the mode somehow but
		 * for now, lirc_serial doesn't support mode changing either
		 */
		break;
	case LIRC_GET_LENGTH:
		result = put_user(ir->d.code_length, (__u32 *)arg);
		break;
	case LIRC_GET_MIN_TIMEOUT:
		if (!(ir->d.features & LIRC_CAN_SET_REC_TIMEOUT) ||
		    ir->d.min_timeout == 0) {
			result = -ENOSYS;
			break;
		}

		result = put_user(ir->d.min_timeout, (__u32 *)arg);
		break;
	case LIRC_GET_MAX_TIMEOUT:
		if (!(ir->d.features & LIRC_CAN_SET_REC_TIMEOUT) ||
		    ir->d.max_timeout == 0) {
			result = -ENOSYS;
			break;
		}

		result = put_user(ir->d.max_timeout, (__u32 *)arg);
		break;
	default:
		result = -EINVAL;
	}

	dev_dbg(ir->d.dev, LOGHEAD "ioctl result = %d\n",
		ir->d.name, ir->d.minor, result);

	mutex_unlock(&ir->irctl_lock);

	return result;
}
EXPORT_SYMBOL(lirc_dev_fop_ioctl);

ssize_t lirc_dev_fop_read(struct file *file,
			  char __user *buffer,
			  size_t length,
			  loff_t *ppos)
{
	struct irctl *ir = irctls[iminor(file->f_dentry->d_inode)];
	unsigned char *buf;
	int ret = 0, written = 0;
	DECLARE_WAITQUEUE(wait, current);

	if (!ir) {
		printk(KERN_ERR "%s: called with invalid irctl\n", __func__);
		return -ENODEV;
	}

	dev_dbg(ir->d.dev, LOGHEAD "read called\n", ir->d.name, ir->d.minor);

	buf = kzalloc(ir->chunk_size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (mutex_lock_interruptible(&ir->irctl_lock)) {
		ret = -ERESTARTSYS;
		goto out_unlocked;
	}
	if (!ir->attached) {
		ret = -ENODEV;
		goto out_locked;
	}

	if (length % ir->chunk_size) {
		ret = -EINVAL;
		goto out_locked;
	}

	/*
	 * we add ourselves to the task queue before buffer check
	 * to avoid losing scan code (in case when queue is awaken somewhere
	 * between while condition checking and scheduling)
	 */
	add_wait_queue(&ir->buf->wait_poll, &wait);
	set_current_state(TASK_INTERRUPTIBLE);

	/*
	 * while we didn't provide 'length' bytes, device is opened in blocking
	 * mode and 'copy_to_user' is happy, wait for data.
	 */
	while (written < length && ret == 0) {
		if (lirc_buffer_empty(ir->buf)) {
			/* According to the read(2) man page, 'written' can be
			 * returned as less than 'length', instead of blocking
			 * again, returning -EWOULDBLOCK, or returning
			 * -ERESTARTSYS */
			if (written)
				break;
			if (file->f_flags & O_NONBLOCK) {
				ret = -EWOULDBLOCK;
				break;
			}
			if (signal_pending(current)) {
				ret = -ERESTARTSYS;
				break;
			}

			mutex_unlock(&ir->irctl_lock);
			schedule();
			set_current_state(TASK_INTERRUPTIBLE);

			if (mutex_lock_interruptible(&ir->irctl_lock)) {
				ret = -ERESTARTSYS;
				remove_wait_queue(&ir->buf->wait_poll, &wait);
				set_current_state(TASK_RUNNING);
				goto out_unlocked;
			}

			if (!ir->attached) {
				ret = -ENODEV;
				break;
			}
		} else {
			lirc_buffer_read(ir->buf, buf);
			ret = copy_to_user((void *)buffer+written, buf,
					   ir->buf->chunk_size);
			if (!ret)
				written += ir->buf->chunk_size;
			else
				ret = -EFAULT;
		}
	}

	remove_wait_queue(&ir->buf->wait_poll, &wait);
	set_current_state(TASK_RUNNING);

out_locked:
	mutex_unlock(&ir->irctl_lock);

out_unlocked:
	kfree(buf);
	dev_dbg(ir->d.dev, LOGHEAD "read result = %s (%d)\n",
		ir->d.name, ir->d.minor, ret ? "<fail>" : "<ok>", ret);

	return ret ? ret : written;
}
EXPORT_SYMBOL(lirc_dev_fop_read);

void *lirc_get_pdata(struct file *file)
{
	void *data = NULL;

	if (file && file->f_dentry && file->f_dentry->d_inode &&
	    file->f_dentry->d_inode->i_rdev) {
		struct irctl *ir;
		ir = irctls[iminor(file->f_dentry->d_inode)];
		data = ir->d.data;
	}

	return data;
}
EXPORT_SYMBOL(lirc_get_pdata);


ssize_t lirc_dev_fop_write(struct file *file, const char __user *buffer,
			   size_t length, loff_t *ppos)
{
	struct irctl *ir = irctls[iminor(file->f_dentry->d_inode)];

	if (!ir) {
		printk(KERN_ERR "%s: called with invalid irctl\n", __func__);
		return -ENODEV;
	}

	dev_dbg(ir->d.dev, LOGHEAD "write called\n", ir->d.name, ir->d.minor);

	if (!ir->attached)
		return -ENODEV;

	return -EINVAL;
}
EXPORT_SYMBOL(lirc_dev_fop_write);


static int __init lirc_dev_init(void)
{
	int retval;

	lirc_class = class_create(THIS_MODULE, "lirc");
	if (IS_ERR(lirc_class)) {
		retval = PTR_ERR(lirc_class);
		printk(KERN_ERR "lirc_dev: class_create failed\n");
		goto error;
	}

	retval = alloc_chrdev_region(&lirc_base_dev, 0, MAX_IRCTL_DEVICES,
				     IRCTL_DEV_NAME);
	if (retval) {
		class_destroy(lirc_class);
		printk(KERN_ERR "lirc_dev: alloc_chrdev_region failed\n");
		goto error;
	}


	printk(KERN_INFO "lirc_dev: IR Remote Control driver registered, "
	       "major %d \n", MAJOR(lirc_base_dev));

error:
	return retval;
}



static void __exit lirc_dev_exit(void)
{
	class_destroy(lirc_class);
	unregister_chrdev_region(lirc_base_dev, MAX_IRCTL_DEVICES);
	printk(KERN_INFO "lirc_dev: module unloaded\n");
}

module_init(lirc_dev_init);
module_exit(lirc_dev_exit);

MODULE_DESCRIPTION("LIRC base driver module");
MODULE_AUTHOR("Artur Lipowski");
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Enable debugging messages");
=======
/**
 * lirc_raw_event() - Send raw IR data to lirc to be relayed to userspace
 *
 * @dev:	the struct rc_dev descriptor of the device
 * @ev:		the struct ir_raw_event descriptor of the pulse/space
 */
void lirc_raw_event(struct rc_dev *dev, struct ir_raw_event ev)
{
	unsigned long flags;
	struct lirc_fh *fh;
	int sample;

	/* Receiver overflow, data missing */
	if (ev.overflow) {
		/*
		 * Send lirc overflow message. This message is unknown to
		 * lircd, but it will interpret this as a long space as
		 * long as the value is set to high value. This resets its
		 * decoder state.
		 */
		sample = LIRC_OVERFLOW(LIRC_VALUE_MASK);
		dev_dbg(&dev->dev, "delivering overflow to lirc_dev\n");

	/* Carrier reports */
	} else if (ev.carrier_report) {
		sample = LIRC_FREQUENCY(ev.carrier);
		dev_dbg(&dev->dev, "carrier report (freq: %d)\n", sample);

	/* Packet end */
	} else if (ev.timeout) {
		dev->gap_start = ktime_get();

		sample = LIRC_TIMEOUT(ev.duration);
		dev_dbg(&dev->dev, "timeout report (duration: %d)\n", sample);

	/* Normal sample */
	} else {
		if (dev->gap_start) {
			u64 duration = ktime_us_delta(ktime_get(),
						      dev->gap_start);

			/* Cap by LIRC_VALUE_MASK */
			duration = min_t(u64, duration, LIRC_VALUE_MASK);

			spin_lock_irqsave(&dev->lirc_fh_lock, flags);
			list_for_each_entry(fh, &dev->lirc_fh, list)
				kfifo_put(&fh->rawir, LIRC_SPACE(duration));
			spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);
			dev->gap_start = 0;
		}

		sample = ev.pulse ? LIRC_PULSE(ev.duration) :
					LIRC_SPACE(ev.duration);
		dev_dbg(&dev->dev, "delivering %uus %s to lirc_dev\n",
			ev.duration, TO_STR(ev.pulse));
	}

	/*
	 * bpf does not care about the gap generated above; that exists
	 * for backwards compatibility
	 */
	lirc_bpf_run(dev, sample);

	spin_lock_irqsave(&dev->lirc_fh_lock, flags);
	list_for_each_entry(fh, &dev->lirc_fh, list) {
		if (kfifo_put(&fh->rawir, sample))
			wake_up_poll(&fh->wait_poll, EPOLLIN | EPOLLRDNORM);
	}
	spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);
}

/**
 * lirc_scancode_event() - Send scancode data to lirc to be relayed to
 *		userspace. This can be called in atomic context.
 * @dev:	the struct rc_dev descriptor of the device
 * @lsc:	the struct lirc_scancode describing the decoded scancode
 */
void lirc_scancode_event(struct rc_dev *dev, struct lirc_scancode *lsc)
{
	unsigned long flags;
	struct lirc_fh *fh;

	lsc->timestamp = ktime_get_ns();

	spin_lock_irqsave(&dev->lirc_fh_lock, flags);
	list_for_each_entry(fh, &dev->lirc_fh, list) {
		if (kfifo_put(&fh->scancodes, *lsc))
			wake_up_poll(&fh->wait_poll, EPOLLIN | EPOLLRDNORM);
	}
	spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);
}
EXPORT_SYMBOL_GPL(lirc_scancode_event);

static int lirc_open(struct inode *inode, struct file *file)
{
	struct rc_dev *dev = container_of(inode->i_cdev, struct rc_dev,
					  lirc_cdev);
	struct lirc_fh *fh = kzalloc(sizeof(*fh), GFP_KERNEL);
	unsigned long flags;
	int retval;

	if (!fh)
		return -ENOMEM;

	get_device(&dev->dev);

	if (!dev->registered) {
		retval = -ENODEV;
		goto out_fh;
	}

	if (dev->driver_type == RC_DRIVER_IR_RAW) {
		if (kfifo_alloc(&fh->rawir, MAX_IR_EVENT_SIZE, GFP_KERNEL)) {
			retval = -ENOMEM;
			goto out_fh;
		}
	}

	if (dev->driver_type != RC_DRIVER_IR_RAW_TX) {
		if (kfifo_alloc(&fh->scancodes, 32, GFP_KERNEL)) {
			retval = -ENOMEM;
			goto out_rawir;
		}
	}

	fh->send_mode = LIRC_MODE_PULSE;
	fh->rc = dev;

	if (dev->driver_type == RC_DRIVER_SCANCODE)
		fh->rec_mode = LIRC_MODE_SCANCODE;
	else
		fh->rec_mode = LIRC_MODE_MODE2;

	retval = rc_open(dev);
	if (retval)
		goto out_kfifo;

	init_waitqueue_head(&fh->wait_poll);

	file->private_data = fh;
	spin_lock_irqsave(&dev->lirc_fh_lock, flags);
	list_add(&fh->list, &dev->lirc_fh);
	spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);

	stream_open(inode, file);

	return 0;
out_kfifo:
	if (dev->driver_type != RC_DRIVER_IR_RAW_TX)
		kfifo_free(&fh->scancodes);
out_rawir:
	if (dev->driver_type == RC_DRIVER_IR_RAW)
		kfifo_free(&fh->rawir);
out_fh:
	kfree(fh);
	put_device(&dev->dev);

	return retval;
}

static int lirc_close(struct inode *inode, struct file *file)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *dev = fh->rc;
	unsigned long flags;

	spin_lock_irqsave(&dev->lirc_fh_lock, flags);
	list_del(&fh->list);
	spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);

	if (dev->driver_type == RC_DRIVER_IR_RAW)
		kfifo_free(&fh->rawir);
	if (dev->driver_type != RC_DRIVER_IR_RAW_TX)
		kfifo_free(&fh->scancodes);
	kfree(fh);

	rc_close(dev);
	put_device(&dev->dev);

	return 0;
}

static ssize_t lirc_transmit(struct file *file, const char __user *buf,
			     size_t n, loff_t *ppos)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *dev = fh->rc;
	unsigned int *txbuf;
	struct ir_raw_event *raw = NULL;
	ssize_t ret;
	size_t count;
	ktime_t start;
	s64 towait;
	unsigned int duration = 0; /* signal duration in us */
	int i;

	ret = mutex_lock_interruptible(&dev->lock);
	if (ret)
		return ret;

	if (!dev->registered) {
		ret = -ENODEV;
		goto out_unlock;
	}

	if (!dev->tx_ir) {
		ret = -EINVAL;
		goto out_unlock;
	}

	if (fh->send_mode == LIRC_MODE_SCANCODE) {
		struct lirc_scancode scan;

		if (n != sizeof(scan)) {
			ret = -EINVAL;
			goto out_unlock;
		}

		if (copy_from_user(&scan, buf, sizeof(scan))) {
			ret = -EFAULT;
			goto out_unlock;
		}

		if (scan.flags || scan.keycode || scan.timestamp ||
		    scan.rc_proto > RC_PROTO_MAX) {
			ret = -EINVAL;
			goto out_unlock;
		}

		/* We only have encoders for 32-bit protocols. */
		if (scan.scancode > U32_MAX ||
		    !rc_validate_scancode(scan.rc_proto, scan.scancode)) {
			ret = -EINVAL;
			goto out_unlock;
		}

		raw = kmalloc_array(LIRCBUF_SIZE, sizeof(*raw), GFP_KERNEL);
		if (!raw) {
			ret = -ENOMEM;
			goto out_unlock;
		}

		ret = ir_raw_encode_scancode(scan.rc_proto, scan.scancode,
					     raw, LIRCBUF_SIZE);
		if (ret < 0)
			goto out_kfree_raw;

		/* drop trailing space */
		if (!(ret % 2))
			count = ret - 1;
		else
			count = ret;

		txbuf = kmalloc_array(count, sizeof(unsigned int), GFP_KERNEL);
		if (!txbuf) {
			ret = -ENOMEM;
			goto out_kfree_raw;
		}

		for (i = 0; i < count; i++)
			txbuf[i] = raw[i].duration;

		if (dev->s_tx_carrier) {
			int carrier = ir_raw_encode_carrier(scan.rc_proto);

			if (carrier > 0)
				dev->s_tx_carrier(dev, carrier);
		}
	} else {
		if (n < sizeof(unsigned int) || n % sizeof(unsigned int)) {
			ret = -EINVAL;
			goto out_unlock;
		}

		count = n / sizeof(unsigned int);
		if (count > LIRCBUF_SIZE || count % 2 == 0) {
			ret = -EINVAL;
			goto out_unlock;
		}

		txbuf = memdup_user(buf, n);
		if (IS_ERR(txbuf)) {
			ret = PTR_ERR(txbuf);
			goto out_unlock;
		}
	}

	for (i = 0; i < count; i++) {
		if (txbuf[i] > IR_MAX_DURATION - duration || !txbuf[i]) {
			ret = -EINVAL;
			goto out_kfree;
		}

		duration += txbuf[i];
	}

	start = ktime_get();

	ret = dev->tx_ir(dev, txbuf, count);
	if (ret < 0)
		goto out_kfree;

	kfree(txbuf);
	kfree(raw);
	mutex_unlock(&dev->lock);

	/*
	 * The lircd gap calculation expects the write function to
	 * wait for the actual IR signal to be transmitted before
	 * returning.
	 */
	towait = ktime_us_delta(ktime_add_us(start, duration),
				ktime_get());
	if (towait > 0) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(usecs_to_jiffies(towait));
	}

	return n;
out_kfree:
	kfree(txbuf);
out_kfree_raw:
	kfree(raw);
out_unlock:
	mutex_unlock(&dev->lock);
	return ret;
}

static long lirc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *dev = fh->rc;
	u32 __user *argp = (u32 __user *)(arg);
	u32 val = 0;
	int ret;

	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		ret = get_user(val, argp);
		if (ret)
			return ret;
	}

	ret = mutex_lock_interruptible(&dev->lock);
	if (ret)
		return ret;

	if (!dev->registered) {
		ret = -ENODEV;
		goto out;
	}

	switch (cmd) {
	case LIRC_GET_FEATURES:
		if (dev->driver_type == RC_DRIVER_SCANCODE)
			val |= LIRC_CAN_REC_SCANCODE;

		if (dev->driver_type == RC_DRIVER_IR_RAW) {
			val |= LIRC_CAN_REC_MODE2;
			if (dev->rx_resolution)
				val |= LIRC_CAN_GET_REC_RESOLUTION;
		}

		if (dev->tx_ir) {
			val |= LIRC_CAN_SEND_PULSE;
			if (dev->s_tx_mask)
				val |= LIRC_CAN_SET_TRANSMITTER_MASK;
			if (dev->s_tx_carrier)
				val |= LIRC_CAN_SET_SEND_CARRIER;
			if (dev->s_tx_duty_cycle)
				val |= LIRC_CAN_SET_SEND_DUTY_CYCLE;
		}

		if (dev->s_rx_carrier_range)
			val |= LIRC_CAN_SET_REC_CARRIER |
				LIRC_CAN_SET_REC_CARRIER_RANGE;

		if (dev->s_wideband_receiver)
			val |= LIRC_CAN_USE_WIDEBAND_RECEIVER;

		if (dev->s_carrier_report)
			val |= LIRC_CAN_MEASURE_CARRIER;

		if (dev->max_timeout)
			val |= LIRC_CAN_SET_REC_TIMEOUT;

		break;

	/* mode support */
	case LIRC_GET_REC_MODE:
		if (dev->driver_type == RC_DRIVER_IR_RAW_TX)
			ret = -ENOTTY;
		else
			val = fh->rec_mode;
		break;

	case LIRC_SET_REC_MODE:
		switch (dev->driver_type) {
		case RC_DRIVER_IR_RAW_TX:
			ret = -ENOTTY;
			break;
		case RC_DRIVER_SCANCODE:
			if (val != LIRC_MODE_SCANCODE)
				ret = -EINVAL;
			break;
		case RC_DRIVER_IR_RAW:
			if (!(val == LIRC_MODE_MODE2 ||
			      val == LIRC_MODE_SCANCODE))
				ret = -EINVAL;
			break;
		}

		if (!ret)
			fh->rec_mode = val;
		break;

	case LIRC_GET_SEND_MODE:
		if (!dev->tx_ir)
			ret = -ENOTTY;
		else
			val = fh->send_mode;
		break;

	case LIRC_SET_SEND_MODE:
		if (!dev->tx_ir)
			ret = -ENOTTY;
		else if (!(val == LIRC_MODE_PULSE || val == LIRC_MODE_SCANCODE))
			ret = -EINVAL;
		else
			fh->send_mode = val;
		break;

	/* TX settings */
	case LIRC_SET_TRANSMITTER_MASK:
		if (!dev->s_tx_mask)
			ret = -ENOTTY;
		else
			ret = dev->s_tx_mask(dev, val);
		break;

	case LIRC_SET_SEND_CARRIER:
		if (!dev->s_tx_carrier)
			ret = -ENOTTY;
		else
			ret = dev->s_tx_carrier(dev, val);
		break;

	case LIRC_SET_SEND_DUTY_CYCLE:
		if (!dev->s_tx_duty_cycle)
			ret = -ENOTTY;
		else if (val <= 0 || val >= 100)
			ret = -EINVAL;
		else
			ret = dev->s_tx_duty_cycle(dev, val);
		break;

	/* RX settings */
	case LIRC_SET_REC_CARRIER:
		if (!dev->s_rx_carrier_range)
			ret = -ENOTTY;
		else if (val <= 0)
			ret = -EINVAL;
		else
			ret = dev->s_rx_carrier_range(dev, fh->carrier_low,
						      val);
		break;

	case LIRC_SET_REC_CARRIER_RANGE:
		if (!dev->s_rx_carrier_range)
			ret = -ENOTTY;
		else if (val <= 0)
			ret = -EINVAL;
		else
			fh->carrier_low = val;
		break;

	case LIRC_GET_REC_RESOLUTION:
		if (!dev->rx_resolution)
			ret = -ENOTTY;
		else
			val = dev->rx_resolution;
		break;

	case LIRC_SET_WIDEBAND_RECEIVER:
		if (!dev->s_wideband_receiver)
			ret = -ENOTTY;
		else
			ret = dev->s_wideband_receiver(dev, !!val);
		break;

	case LIRC_SET_MEASURE_CARRIER_MODE:
		if (!dev->s_carrier_report)
			ret = -ENOTTY;
		else
			ret = dev->s_carrier_report(dev, !!val);
		break;

	/* Generic timeout support */
	case LIRC_GET_MIN_TIMEOUT:
		if (!dev->max_timeout)
			ret = -ENOTTY;
		else
			val = dev->min_timeout;
		break;

	case LIRC_GET_MAX_TIMEOUT:
		if (!dev->max_timeout)
			ret = -ENOTTY;
		else
			val = dev->max_timeout;
		break;

	case LIRC_SET_REC_TIMEOUT:
		if (!dev->max_timeout) {
			ret = -ENOTTY;
		} else {
			if (val < dev->min_timeout || val > dev->max_timeout)
				ret = -EINVAL;
			else if (dev->s_timeout)
				ret = dev->s_timeout(dev, val);
			else
				dev->timeout = val;
		}
		break;

	case LIRC_GET_REC_TIMEOUT:
		if (!dev->timeout)
			ret = -ENOTTY;
		else
			val = dev->timeout;
		break;

	case LIRC_SET_REC_TIMEOUT_REPORTS:
		if (dev->driver_type != RC_DRIVER_IR_RAW)
			ret = -ENOTTY;
		break;

	default:
		ret = -ENOTTY;
	}

	if (!ret && _IOC_DIR(cmd) & _IOC_READ)
		ret = put_user(val, argp);

out:
	mutex_unlock(&dev->lock);
	return ret;
}

static __poll_t lirc_poll(struct file *file, struct poll_table_struct *wait)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *rcdev = fh->rc;
	__poll_t events = 0;

	poll_wait(file, &fh->wait_poll, wait);

	if (!rcdev->registered) {
		events = EPOLLHUP | EPOLLERR;
	} else if (rcdev->driver_type != RC_DRIVER_IR_RAW_TX) {
		if (fh->rec_mode == LIRC_MODE_SCANCODE &&
		    !kfifo_is_empty(&fh->scancodes))
			events = EPOLLIN | EPOLLRDNORM;

		if (fh->rec_mode == LIRC_MODE_MODE2 &&
		    !kfifo_is_empty(&fh->rawir))
			events = EPOLLIN | EPOLLRDNORM;
	}

	return events;
}

static ssize_t lirc_read_mode2(struct file *file, char __user *buffer,
			       size_t length)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *rcdev = fh->rc;
	unsigned int copied;
	int ret;

	if (length < sizeof(unsigned int) || length % sizeof(unsigned int))
		return -EINVAL;

	do {
		if (kfifo_is_empty(&fh->rawir)) {
			if (file->f_flags & O_NONBLOCK)
				return -EAGAIN;

			ret = wait_event_interruptible(fh->wait_poll,
					!kfifo_is_empty(&fh->rawir) ||
					!rcdev->registered);
			if (ret)
				return ret;
		}

		if (!rcdev->registered)
			return -ENODEV;

		ret = mutex_lock_interruptible(&rcdev->lock);
		if (ret)
			return ret;
		ret = kfifo_to_user(&fh->rawir, buffer, length, &copied);
		mutex_unlock(&rcdev->lock);
		if (ret)
			return ret;
	} while (copied == 0);

	return copied;
}

static ssize_t lirc_read_scancode(struct file *file, char __user *buffer,
				  size_t length)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *rcdev = fh->rc;
	unsigned int copied;
	int ret;

	if (length < sizeof(struct lirc_scancode) ||
	    length % sizeof(struct lirc_scancode))
		return -EINVAL;

	do {
		if (kfifo_is_empty(&fh->scancodes)) {
			if (file->f_flags & O_NONBLOCK)
				return -EAGAIN;

			ret = wait_event_interruptible(fh->wait_poll,
					!kfifo_is_empty(&fh->scancodes) ||
					!rcdev->registered);
			if (ret)
				return ret;
		}

		if (!rcdev->registered)
			return -ENODEV;

		ret = mutex_lock_interruptible(&rcdev->lock);
		if (ret)
			return ret;
		ret = kfifo_to_user(&fh->scancodes, buffer, length, &copied);
		mutex_unlock(&rcdev->lock);
		if (ret)
			return ret;
	} while (copied == 0);

	return copied;
}

static ssize_t lirc_read(struct file *file, char __user *buffer, size_t length,
			 loff_t *ppos)
{
	struct lirc_fh *fh = file->private_data;
	struct rc_dev *rcdev = fh->rc;

	if (rcdev->driver_type == RC_DRIVER_IR_RAW_TX)
		return -EINVAL;

	if (!rcdev->registered)
		return -ENODEV;

	if (fh->rec_mode == LIRC_MODE_MODE2)
		return lirc_read_mode2(file, buffer, length);
	else /* LIRC_MODE_SCANCODE */
		return lirc_read_scancode(file, buffer, length);
}

static const struct file_operations lirc_fops = {
	.owner		= THIS_MODULE,
	.write		= lirc_transmit,
	.unlocked_ioctl	= lirc_ioctl,
	.compat_ioctl	= compat_ptr_ioctl,
	.read		= lirc_read,
	.poll		= lirc_poll,
	.open		= lirc_open,
	.release	= lirc_close,
	.llseek		= no_llseek,
};

static void lirc_release_device(struct device *ld)
{
	struct rc_dev *rcdev = container_of(ld, struct rc_dev, lirc_dev);

	put_device(&rcdev->dev);
}

int lirc_register(struct rc_dev *dev)
{
	const char *rx_type, *tx_type;
	int err, minor;

	minor = ida_alloc_max(&lirc_ida, RC_DEV_MAX - 1, GFP_KERNEL);
	if (minor < 0)
		return minor;

	device_initialize(&dev->lirc_dev);
	dev->lirc_dev.class = lirc_class;
	dev->lirc_dev.parent = &dev->dev;
	dev->lirc_dev.release = lirc_release_device;
	dev->lirc_dev.devt = MKDEV(MAJOR(lirc_base_dev), minor);
	dev_set_name(&dev->lirc_dev, "lirc%d", minor);

	INIT_LIST_HEAD(&dev->lirc_fh);
	spin_lock_init(&dev->lirc_fh_lock);

	cdev_init(&dev->lirc_cdev, &lirc_fops);

	err = cdev_device_add(&dev->lirc_cdev, &dev->lirc_dev);
	if (err)
		goto out_ida;

	get_device(&dev->dev);

	switch (dev->driver_type) {
	case RC_DRIVER_SCANCODE:
		rx_type = "scancode";
		break;
	case RC_DRIVER_IR_RAW:
		rx_type = "raw IR";
		break;
	default:
		rx_type = "no";
		break;
	}

	if (dev->tx_ir)
		tx_type = "raw IR";
	else
		tx_type = "no";

	dev_info(&dev->dev, "lirc_dev: driver %s registered at minor = %d, %s receiver, %s transmitter",
		 dev->driver_name, minor, rx_type, tx_type);

	return 0;

out_ida:
	ida_free(&lirc_ida, minor);
	return err;
}

void lirc_unregister(struct rc_dev *dev)
{
	unsigned long flags;
	struct lirc_fh *fh;

	dev_dbg(&dev->dev, "lirc_dev: driver %s unregistered from minor = %d\n",
		dev->driver_name, MINOR(dev->lirc_dev.devt));

	spin_lock_irqsave(&dev->lirc_fh_lock, flags);
	list_for_each_entry(fh, &dev->lirc_fh, list)
		wake_up_poll(&fh->wait_poll, EPOLLHUP | EPOLLERR);
	spin_unlock_irqrestore(&dev->lirc_fh_lock, flags);

	cdev_device_del(&dev->lirc_cdev, &dev->lirc_dev);
	ida_free(&lirc_ida, MINOR(dev->lirc_dev.devt));
}

int __init lirc_dev_init(void)
{
	int retval;

	lirc_class = class_create("lirc");
	if (IS_ERR(lirc_class)) {
		pr_err("class_create failed\n");
		return PTR_ERR(lirc_class);
	}

	retval = alloc_chrdev_region(&lirc_base_dev, 0, RC_DEV_MAX, "lirc");
	if (retval) {
		class_destroy(lirc_class);
		pr_err("alloc_chrdev_region failed\n");
		return retval;
	}

	pr_debug("IR Remote Control driver registered, major %d\n",
		 MAJOR(lirc_base_dev));

	return 0;
}

void __exit lirc_dev_exit(void)
{
	class_destroy(lirc_class);
	unregister_chrdev_region(lirc_base_dev, RC_DEV_MAX);
}

struct rc_dev *rc_dev_get_from_fd(int fd, bool write)
{
	struct fd f = fdget(fd);
	struct lirc_fh *fh;
	struct rc_dev *dev;

	if (!f.file)
		return ERR_PTR(-EBADF);

	if (f.file->f_op != &lirc_fops) {
		fdput(f);
		return ERR_PTR(-EINVAL);
	}

	if (write && !(f.file->f_mode & FMODE_WRITE))
		return ERR_PTR(-EPERM);

	fh = f.file->private_data;
	dev = fh->rc;

	get_device(&dev->dev);
	fdput(f);

	return dev;
}

MODULE_ALIAS("lirc_dev");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
