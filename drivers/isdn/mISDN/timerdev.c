<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 * general timer device for using in ISDN stacks
 *
 * Author	Karsten Keil <kkeil@novell.com>
 *
 * Copyright 2008  by Karsten Keil <kkeil@novell.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/poll.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mISDNif.h>
#include <linux/mutex.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "core.h"

static DEFINE_MUTEX(mISDN_mutex);
static u_int	*debug;


struct mISDNtimerdev {
	int			next_id;
	struct list_head	pending;
	struct list_head	expired;
	wait_queue_head_t	wait;
	u_int			work;
	spinlock_t		lock; /* protect lists */
};

struct mISDNtimer {
	struct list_head	list;
	struct  mISDNtimerdev	*dev;
	struct timer_list	tl;
	int			id;
};

static int
mISDN_open(struct inode *ino, struct file *filep)
{
	struct mISDNtimerdev	*dev;

	if (*debug & DEBUG_TIMER)
		printk(KERN_DEBUG "%s(%p,%p)\n", __func__, ino, filep);
	dev = kmalloc(sizeof(struct mISDNtimerdev) , GFP_KERNEL);
	if (!dev)
		return -ENOMEM;
	dev->next_id = 1;
	INIT_LIST_HEAD(&dev->pending);
	INIT_LIST_HEAD(&dev->expired);
	spin_lock_init(&dev->lock);
	dev->work = 0;
	init_waitqueue_head(&dev->wait);
	filep->private_data = dev;
<<<<<<< HEAD
	__module_get(THIS_MODULE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return nonseekable_open(ino, filep);
}

static int
mISDN_close(struct inode *ino, struct file *filep)
{
	struct mISDNtimerdev	*dev = filep->private_data;
<<<<<<< HEAD
=======
	struct list_head	*list = &dev->pending;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mISDNtimer	*timer, *next;

	if (*debug & DEBUG_TIMER)
		printk(KERN_DEBUG "%s(%p,%p)\n", __func__, ino, filep);
<<<<<<< HEAD
	list_for_each_entry_safe(timer, next, &dev->pending, list) {
		del_timer(&timer->tl);
		kfree(timer);
	}
=======

	spin_lock_irq(&dev->lock);
	while (!list_empty(list)) {
		timer = list_first_entry(list, struct mISDNtimer, list);
		spin_unlock_irq(&dev->lock);
		timer_shutdown_sync(&timer->tl);
		spin_lock_irq(&dev->lock);
		/* it might have been moved to ->expired */
		list_del(&timer->list);
		kfree(timer);
	}
	spin_unlock_irq(&dev->lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry_safe(timer, next, &dev->expired, list) {
		kfree(timer);
	}
	kfree(dev);
<<<<<<< HEAD
	module_put(THIS_MODULE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static ssize_t
mISDN_read(struct file *filep, char __user *buf, size_t count, loff_t *off)
{
	struct mISDNtimerdev	*dev = filep->private_data;
<<<<<<< HEAD
	struct mISDNtimer	*timer;
	u_long	flags;
=======
	struct list_head *list = &dev->expired;
	struct mISDNtimer	*timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	ret = 0;

	if (*debug & DEBUG_TIMER)
		printk(KERN_DEBUG "%s(%p, %p, %d, %p)\n", __func__,
		       filep, buf, (int)count, off);

<<<<<<< HEAD
	if (list_empty(&dev->expired) && (dev->work == 0)) {
		if (filep->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(dev->wait, (dev->work ||
						     !list_empty(&dev->expired)));
		if (signal_pending(current))
			return -ERESTARTSYS;
	}
	if (count < sizeof(int))
		return -ENOSPC;
	if (dev->work)
		dev->work = 0;
	if (!list_empty(&dev->expired)) {
		spin_lock_irqsave(&dev->lock, flags);
		timer = (struct mISDNtimer *)dev->expired.next;
		list_del(&timer->list);
		spin_unlock_irqrestore(&dev->lock, flags);
=======
	if (count < sizeof(int))
		return -ENOSPC;

	spin_lock_irq(&dev->lock);
	while (list_empty(list) && (dev->work == 0)) {
		spin_unlock_irq(&dev->lock);
		if (filep->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(dev->wait, (dev->work ||
						     !list_empty(list)));
		if (signal_pending(current))
			return -ERESTARTSYS;
		spin_lock_irq(&dev->lock);
	}
	if (dev->work)
		dev->work = 0;
	if (!list_empty(list)) {
		timer = list_first_entry(list, struct mISDNtimer, list);
		list_del(&timer->list);
		spin_unlock_irq(&dev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (put_user(timer->id, (int __user *)buf))
			ret = -EFAULT;
		else
			ret = sizeof(int);
		kfree(timer);
<<<<<<< HEAD
=======
	} else {
		spin_unlock_irq(&dev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ret;
}

<<<<<<< HEAD
static unsigned int
mISDN_poll(struct file *filep, poll_table *wait)
{
	struct mISDNtimerdev	*dev = filep->private_data;
	unsigned int		mask = POLLERR;
=======
static __poll_t
mISDN_poll(struct file *filep, poll_table *wait)
{
	struct mISDNtimerdev	*dev = filep->private_data;
	__poll_t		mask = EPOLLERR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (*debug & DEBUG_TIMER)
		printk(KERN_DEBUG "%s(%p, %p)\n", __func__, filep, wait);
	if (dev) {
		poll_wait(filep, &dev->wait, wait);
		mask = 0;
		if (dev->work || !list_empty(&dev->expired))
<<<<<<< HEAD
			mask |= (POLLIN | POLLRDNORM);
=======
			mask |= (EPOLLIN | EPOLLRDNORM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (*debug & DEBUG_TIMER)
			printk(KERN_DEBUG "%s work(%d) empty(%d)\n", __func__,
			       dev->work, list_empty(&dev->expired));
	}
	return mask;
}

static void
<<<<<<< HEAD
dev_expire_timer(unsigned long data)
{
	struct mISDNtimer *timer = (void *)data;
	u_long			flags;

	spin_lock_irqsave(&timer->dev->lock, flags);
	list_move_tail(&timer->list, &timer->dev->expired);
	spin_unlock_irqrestore(&timer->dev->lock, flags);
	wake_up_interruptible(&timer->dev->wait);
=======
dev_expire_timer(struct timer_list *t)
{
	struct mISDNtimer *timer = from_timer(timer, t, tl);
	u_long			flags;

	spin_lock_irqsave(&timer->dev->lock, flags);
	if (timer->id >= 0)
		list_move_tail(&timer->list, &timer->dev->expired);
	wake_up_interruptible(&timer->dev->wait);
	spin_unlock_irqrestore(&timer->dev->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
misdn_add_timer(struct mISDNtimerdev *dev, int timeout)
{
	int			id;
<<<<<<< HEAD
	u_long			flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mISDNtimer	*timer;

	if (!timeout) {
		dev->work = 1;
		wake_up_interruptible(&dev->wait);
		id = 0;
	} else {
		timer = kzalloc(sizeof(struct mISDNtimer), GFP_KERNEL);
		if (!timer)
			return -ENOMEM;
<<<<<<< HEAD
		spin_lock_irqsave(&dev->lock, flags);
		timer->id = dev->next_id++;
		if (dev->next_id < 0)
			dev->next_id = 1;
		list_add_tail(&timer->list, &dev->pending);
		spin_unlock_irqrestore(&dev->lock, flags);
		timer->dev = dev;
		timer->tl.data = (long)timer;
		timer->tl.function = dev_expire_timer;
		init_timer(&timer->tl);
		timer->tl.expires = jiffies + ((HZ * (u_long)timeout) / 1000);
		add_timer(&timer->tl);
		id = timer->id;
=======
		timer->dev = dev;
		timer_setup(&timer->tl, dev_expire_timer, 0);
		spin_lock_irq(&dev->lock);
		id = timer->id = dev->next_id++;
		if (dev->next_id < 0)
			dev->next_id = 1;
		list_add_tail(&timer->list, &dev->pending);
		timer->tl.expires = jiffies + ((HZ * (u_long)timeout) / 1000);
		add_timer(&timer->tl);
		spin_unlock_irq(&dev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return id;
}

static int
misdn_del_timer(struct mISDNtimerdev *dev, int id)
{
<<<<<<< HEAD
	u_long			flags;
	struct mISDNtimer	*timer;
	int			ret = 0;

	spin_lock_irqsave(&dev->lock, flags);
	list_for_each_entry(timer, &dev->pending, list) {
		if (timer->id == id) {
			list_del_init(&timer->list);
			/* RED-PEN AK: race -- timer can be still running on
			 * other CPU. Needs reference count I think
			 */
			del_timer(&timer->tl);
			ret = timer->id;
			kfree(timer);
			goto unlock;
		}
	}
unlock:
	spin_unlock_irqrestore(&dev->lock, flags);
	return ret;
=======
	struct mISDNtimer	*timer;

	spin_lock_irq(&dev->lock);
	list_for_each_entry(timer, &dev->pending, list) {
		if (timer->id == id) {
			list_del_init(&timer->list);
			timer->id = -1;
			spin_unlock_irq(&dev->lock);
			timer_shutdown_sync(&timer->tl);
			kfree(timer);
			return id;
		}
	}
	spin_unlock_irq(&dev->lock);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static long
mISDN_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	struct mISDNtimerdev	*dev = filep->private_data;
	int			id, tout, ret = 0;


	if (*debug & DEBUG_TIMER)
		printk(KERN_DEBUG "%s(%p, %x, %lx)\n", __func__,
		       filep, cmd, arg);
	mutex_lock(&mISDN_mutex);
	switch (cmd) {
	case IMADDTIMER:
		if (get_user(tout, (int __user *)arg)) {
			ret = -EFAULT;
			break;
		}
		id = misdn_add_timer(dev, tout);
		if (*debug & DEBUG_TIMER)
			printk(KERN_DEBUG "%s add %d id %d\n", __func__,
			       tout, id);
		if (id < 0) {
			ret = id;
			break;
		}
		if (put_user(id, (int __user *)arg))
			ret = -EFAULT;
		break;
	case IMDELTIMER:
		if (get_user(id, (int __user *)arg)) {
			ret = -EFAULT;
			break;
		}
		if (*debug & DEBUG_TIMER)
			printk(KERN_DEBUG "%s del id %d\n", __func__, id);
		id = misdn_del_timer(dev, id);
		if (put_user(id, (int __user *)arg))
			ret = -EFAULT;
		break;
	default:
		ret = -EINVAL;
	}
	mutex_unlock(&mISDN_mutex);
	return ret;
}

static const struct file_operations mISDN_fops = {
<<<<<<< HEAD
=======
	.owner		= THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read		= mISDN_read,
	.poll		= mISDN_poll,
	.unlocked_ioctl	= mISDN_ioctl,
	.open		= mISDN_open,
	.release	= mISDN_close,
	.llseek		= no_llseek,
};

static struct miscdevice mISDNtimer = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "mISDNtimer",
	.fops	= &mISDN_fops,
};

int
mISDN_inittimer(u_int *deb)
{
	int	err;

	debug = deb;
	err = misc_register(&mISDNtimer);
	if (err)
		printk(KERN_WARNING "mISDN: Could not register timer device\n");
	return err;
}

void mISDN_timer_cleanup(void)
{
	misc_deregister(&mISDNtimer);
}
