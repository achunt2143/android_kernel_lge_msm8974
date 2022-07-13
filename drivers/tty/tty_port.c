<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Tty port functions
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/init.h>
=======
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/wait.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/module.h>
<<<<<<< HEAD

void tty_port_init(struct tty_port *port)
{
	memset(port, 0, sizeof(*port));
	init_waitqueue_head(&port->open_wait);
	init_waitqueue_head(&port->close_wait);
=======
#include <linux/serdev.h>
#include "tty.h"

static size_t tty_port_default_receive_buf(struct tty_port *port, const u8 *p,
					   const u8 *f, size_t count)
{
	struct tty_struct *tty;
	struct tty_ldisc *ld;

	tty = READ_ONCE(port->itty);
	if (!tty)
		return 0;

	ld = tty_ldisc_ref(tty);
	if (!ld)
		return 0;

	count = tty_ldisc_receive_buf(ld, p, f, count);

	tty_ldisc_deref(ld);

	return count;
}

static void tty_port_default_lookahead_buf(struct tty_port *port, const u8 *p,
					   const u8 *f, size_t count)
{
	struct tty_struct *tty;
	struct tty_ldisc *ld;

	tty = READ_ONCE(port->itty);
	if (!tty)
		return;

	ld = tty_ldisc_ref(tty);
	if (!ld)
		return;

	if (ld->ops->lookahead_buf)
		ld->ops->lookahead_buf(ld->tty, p, f, count);

	tty_ldisc_deref(ld);
}

static void tty_port_default_wakeup(struct tty_port *port)
{
	struct tty_struct *tty = tty_port_tty_get(port);

	if (tty) {
		tty_wakeup(tty);
		tty_kref_put(tty);
	}
}

const struct tty_port_client_operations tty_port_default_client_ops = {
	.receive_buf = tty_port_default_receive_buf,
	.lookahead_buf = tty_port_default_lookahead_buf,
	.write_wakeup = tty_port_default_wakeup,
};
EXPORT_SYMBOL_GPL(tty_port_default_client_ops);

/**
 * tty_port_init - initialize tty_port
 * @port: tty_port to initialize
 *
 * Initializes the state of struct tty_port. When a port was initialized using
 * this function, one has to destroy the port by tty_port_destroy(). Either
 * indirectly by using &tty_port refcounting (tty_port_put()) or directly if
 * refcounting is not used.
 */
void tty_port_init(struct tty_port *port)
{
	memset(port, 0, sizeof(*port));
	tty_buffer_init(port);
	init_waitqueue_head(&port->open_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	init_waitqueue_head(&port->delta_msr_wait);
	mutex_init(&port->mutex);
	mutex_init(&port->buf_mutex);
	spin_lock_init(&port->lock);
	port->close_delay = (50 * HZ) / 100;
	port->closing_wait = (3000 * HZ) / 100;
<<<<<<< HEAD
=======
	port->client_ops = &tty_port_default_client_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kref_init(&port->kref);
}
EXPORT_SYMBOL(tty_port_init);

<<<<<<< HEAD
=======
/**
 * tty_port_link_device - link tty and tty_port
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 *
 * Provide the tty layer with a link from a tty (specified by @index) to a
 * tty_port (@port). Use this only if neither tty_port_register_device() nor
 * tty_port_install() is used in the driver. If used, this has to be called
 * before tty_register_driver().
 */
void tty_port_link_device(struct tty_port *port,
		struct tty_driver *driver, unsigned index)
{
	if (WARN_ON(index >= driver->num))
		return;
	driver->ports[index] = port;
}
EXPORT_SYMBOL_GPL(tty_port_link_device);

/**
 * tty_port_register_device - register tty device
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 * @device: parent if exists, otherwise NULL
 *
 * It is the same as tty_register_device() except the provided @port is linked
 * to a concrete tty specified by @index. Use this or tty_port_install() (or
 * both). Call tty_port_link_device() as a last resort.
 */
struct device *tty_port_register_device(struct tty_port *port,
		struct tty_driver *driver, unsigned index,
		struct device *device)
{
	return tty_port_register_device_attr(port, driver, index, device, NULL, NULL);
}
EXPORT_SYMBOL_GPL(tty_port_register_device);

/**
 * tty_port_register_device_attr - register tty device
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 * @device: parent if exists, otherwise NULL
 * @drvdata: Driver data to be set to device.
 * @attr_grp: Attribute group to be set on device.
 *
 * It is the same as tty_register_device_attr() except the provided @port is
 * linked to a concrete tty specified by @index. Use this or tty_port_install()
 * (or both). Call tty_port_link_device() as a last resort.
 */
struct device *tty_port_register_device_attr(struct tty_port *port,
		struct tty_driver *driver, unsigned index,
		struct device *device, void *drvdata,
		const struct attribute_group **attr_grp)
{
	tty_port_link_device(port, driver, index);
	return tty_register_device_attr(driver, index, device, drvdata,
			attr_grp);
}
EXPORT_SYMBOL_GPL(tty_port_register_device_attr);

/**
 * tty_port_register_device_attr_serdev - register tty or serdev device
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 * @host: serial port hardware device
 * @parent: parent if exists, otherwise NULL
 * @drvdata: driver data for the device
 * @attr_grp: attribute group for the device
 *
 * Register a serdev or tty device depending on if the parent device has any
 * defined serdev clients or not.
 */
struct device *tty_port_register_device_attr_serdev(struct tty_port *port,
		struct tty_driver *driver, unsigned index,
		struct device *host, struct device *parent, void *drvdata,
		const struct attribute_group **attr_grp)
{
	struct device *dev;

	tty_port_link_device(port, driver, index);

	dev = serdev_tty_port_register(port, host, parent, driver, index);
	if (PTR_ERR(dev) != -ENODEV) {
		/* Skip creating cdev if we registered a serdev device */
		return dev;
	}

	return tty_register_device_attr(driver, index, parent, drvdata,
			attr_grp);
}
EXPORT_SYMBOL_GPL(tty_port_register_device_attr_serdev);

/**
 * tty_port_register_device_serdev - register tty or serdev device
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 * @host: serial port hardware controller device
 * @parent: parent if exists, otherwise NULL
 *
 * Register a serdev or tty device depending on if the parent device has any
 * defined serdev clients or not.
 */
struct device *tty_port_register_device_serdev(struct tty_port *port,
		struct tty_driver *driver, unsigned index,
		struct device *host, struct device *parent)
{
	return tty_port_register_device_attr_serdev(port, driver, index,
			host, parent, NULL, NULL);
}
EXPORT_SYMBOL_GPL(tty_port_register_device_serdev);

/**
 * tty_port_unregister_device - deregister a tty or serdev device
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @index: index of the tty
 *
 * If a tty or serdev device is registered with a call to
 * tty_port_register_device_serdev() then this function must be called when
 * the device is gone.
 */
void tty_port_unregister_device(struct tty_port *port,
		struct tty_driver *driver, unsigned index)
{
	int ret;

	ret = serdev_tty_port_unregister(port);
	if (ret == 0)
		return;

	tty_unregister_device(driver, index);
}
EXPORT_SYMBOL_GPL(tty_port_unregister_device);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int tty_port_alloc_xmit_buf(struct tty_port *port)
{
	/* We may sleep in get_zeroed_page() */
	mutex_lock(&port->buf_mutex);
<<<<<<< HEAD
	if (port->xmit_buf == NULL)
		port->xmit_buf = (unsigned char *)get_zeroed_page(GFP_KERNEL);
=======
	if (port->xmit_buf == NULL) {
		port->xmit_buf = (u8 *)get_zeroed_page(GFP_KERNEL);
		if (port->xmit_buf)
			kfifo_init(&port->xmit_fifo, port->xmit_buf, PAGE_SIZE);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->buf_mutex);
	if (port->xmit_buf == NULL)
		return -ENOMEM;
	return 0;
}
EXPORT_SYMBOL(tty_port_alloc_xmit_buf);

void tty_port_free_xmit_buf(struct tty_port *port)
{
	mutex_lock(&port->buf_mutex);
<<<<<<< HEAD
	if (port->xmit_buf != NULL) {
		free_page((unsigned long)port->xmit_buf);
		port->xmit_buf = NULL;
	}
=======
	free_page((unsigned long)port->xmit_buf);
	port->xmit_buf = NULL;
	INIT_KFIFO(port->xmit_fifo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->buf_mutex);
}
EXPORT_SYMBOL(tty_port_free_xmit_buf);

<<<<<<< HEAD
static void tty_port_destructor(struct kref *kref)
{
	struct tty_port *port = container_of(kref, struct tty_port, kref);
	if (port->xmit_buf)
		free_page((unsigned long)port->xmit_buf);
	if (port->ops->destruct)
=======
/**
 * tty_port_destroy - destroy inited port
 * @port: tty port to be destroyed
 *
 * When a port was initialized using tty_port_init(), one has to destroy the
 * port by this function. Either indirectly by using &tty_port refcounting
 * (tty_port_put()) or directly if refcounting is not used.
 */
void tty_port_destroy(struct tty_port *port)
{
	tty_buffer_cancel_work(port);
	tty_buffer_free_all(port);
}
EXPORT_SYMBOL(tty_port_destroy);

static void tty_port_destructor(struct kref *kref)
{
	struct tty_port *port = container_of(kref, struct tty_port, kref);

	/* check if last port ref was dropped before tty release */
	if (WARN_ON(port->itty))
		return;
	free_page((unsigned long)port->xmit_buf);
	tty_port_destroy(port);
	if (port->ops && port->ops->destruct)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->ops->destruct(port);
	else
		kfree(port);
}

<<<<<<< HEAD
=======
/**
 * tty_port_put - drop a reference to tty_port
 * @port: port to drop a reference of (can be NULL)
 *
 * The final put will destroy and free up the @port using
 * @port->ops->destruct() hook, or using kfree() if not provided.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tty_port_put(struct tty_port *port)
{
	if (port)
		kref_put(&port->kref, tty_port_destructor);
}
EXPORT_SYMBOL(tty_port_put);

/**
<<<<<<< HEAD
 *	tty_port_tty_get	-	get a tty reference
 *	@port: tty port
 *
 *	Return a refcount protected tty instance or NULL if the port is not
 *	associated with a tty (eg due to close or hangup)
 */

=======
 * tty_port_tty_get	-	get a tty reference
 * @port: tty port
 *
 * Return a refcount protected tty instance or %NULL if the port is not
 * associated with a tty (eg due to close or hangup).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct tty_struct *tty_port_tty_get(struct tty_port *port)
{
	unsigned long flags;
	struct tty_struct *tty;

	spin_lock_irqsave(&port->lock, flags);
	tty = tty_kref_get(port->tty);
	spin_unlock_irqrestore(&port->lock, flags);
	return tty;
}
EXPORT_SYMBOL(tty_port_tty_get);

/**
<<<<<<< HEAD
 *	tty_port_tty_set	-	set the tty of a port
 *	@port: tty port
 *	@tty: the tty
 *
 *	Associate the port and tty pair. Manages any internal refcounts.
 *	Pass NULL to deassociate a port
 */

=======
 * tty_port_tty_set	-	set the tty of a port
 * @port: tty port
 * @tty: the tty
 *
 * Associate the port and tty pair. Manages any internal refcounts. Pass %NULL
 * to deassociate a port.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tty_port_tty_set(struct tty_port *port, struct tty_struct *tty)
{
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
<<<<<<< HEAD
	if (port->tty)
		tty_kref_put(port->tty);
=======
	tty_kref_put(port->tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port->tty = tty_kref_get(tty);
	spin_unlock_irqrestore(&port->lock, flags);
}
EXPORT_SYMBOL(tty_port_tty_set);

<<<<<<< HEAD
static void tty_port_shutdown(struct tty_port *port)
{
	mutex_lock(&port->mutex);
	if (port->ops->shutdown && !port->console &&
		test_and_clear_bit(ASYNCB_INITIALIZED, &port->flags))
			port->ops->shutdown(port);
=======
/**
 * tty_port_shutdown - internal helper to shutdown the device
 * @port: tty port to be shut down
 * @tty: the associated tty
 *
 * It is used by tty_port_hangup() and tty_port_close(). Its task is to
 * shutdown the device if it was initialized (note consoles remain
 * functioning). It lowers DTR/RTS (if @tty has HUPCL set) and invokes
 * @port->ops->shutdown().
 */
static void tty_port_shutdown(struct tty_port *port, struct tty_struct *tty)
{
	mutex_lock(&port->mutex);
	if (port->console)
		goto out;

	if (tty_port_initialized(port)) {
		tty_port_set_initialized(port, false);
		/*
		 * Drop DTR/RTS if HUPCL is set. This causes any attached
		 * modem to hang up the line.
		 */
		if (tty && C_HUPCL(tty))
			tty_port_lower_dtr_rts(port);

		if (port->ops->shutdown)
			port->ops->shutdown(port);
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->mutex);
}

/**
<<<<<<< HEAD
 *	tty_port_hangup		-	hangup helper
 *	@port: tty port
 *
 *	Perform port level tty hangup flag and count changes. Drop the tty
 *	reference.
 */

void tty_port_hangup(struct tty_port *port)
{
=======
 * tty_port_hangup		-	hangup helper
 * @port: tty port
 *
 * Perform port level tty hangup flag and count changes. Drop the tty
 * reference.
 *
 * Caller holds tty lock.
 */
void tty_port_hangup(struct tty_port *port)
{
	struct tty_struct *tty;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	port->count = 0;
<<<<<<< HEAD
	port->flags &= ~ASYNC_NORMAL_ACTIVE;
	if (port->tty) {
		set_bit(TTY_IO_ERROR, &port->tty->flags);
		tty_kref_put(port->tty);
	}
	port->tty = NULL;
	spin_unlock_irqrestore(&port->lock, flags);
	wake_up_interruptible(&port->open_wait);
	wake_up_interruptible(&port->delta_msr_wait);
	tty_port_shutdown(port);
=======
	tty = port->tty;
	if (tty)
		set_bit(TTY_IO_ERROR, &tty->flags);
	port->tty = NULL;
	spin_unlock_irqrestore(&port->lock, flags);
	tty_port_set_active(port, false);
	tty_port_shutdown(port, tty);
	tty_kref_put(tty);
	wake_up_interruptible(&port->open_wait);
	wake_up_interruptible(&port->delta_msr_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(tty_port_hangup);

/**
<<<<<<< HEAD
 *	tty_port_carrier_raised	-	carrier raised check
 *	@port: tty port
 *
 *	Wrapper for the carrier detect logic. For the moment this is used
 *	to hide some internal details. This will eventually become entirely
 *	internal to the tty port.
 */

int tty_port_carrier_raised(struct tty_port *port)
{
	if (port->ops->carrier_raised == NULL)
		return 1;
=======
 * tty_port_tty_hangup - helper to hang up a tty
 * @port: tty port
 * @check_clocal: hang only ttys with %CLOCAL unset?
 */
void tty_port_tty_hangup(struct tty_port *port, bool check_clocal)
{
	struct tty_struct *tty = tty_port_tty_get(port);

	if (tty && (!check_clocal || !C_CLOCAL(tty)))
		tty_hangup(tty);
	tty_kref_put(tty);
}
EXPORT_SYMBOL_GPL(tty_port_tty_hangup);

/**
 * tty_port_tty_wakeup - helper to wake up a tty
 * @port: tty port
 */
void tty_port_tty_wakeup(struct tty_port *port)
{
	port->client_ops->write_wakeup(port);
}
EXPORT_SYMBOL_GPL(tty_port_tty_wakeup);

/**
 * tty_port_carrier_raised	-	carrier raised check
 * @port: tty port
 *
 * Wrapper for the carrier detect logic. For the moment this is used
 * to hide some internal details. This will eventually become entirely
 * internal to the tty port.
 */
bool tty_port_carrier_raised(struct tty_port *port)
{
	if (port->ops->carrier_raised == NULL)
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return port->ops->carrier_raised(port);
}
EXPORT_SYMBOL(tty_port_carrier_raised);

/**
<<<<<<< HEAD
 *	tty_port_raise_dtr_rts	-	Raise DTR/RTS
 *	@port: tty port
 *
 *	Wrapper for the DTR/RTS raise logic. For the moment this is used
 *	to hide some internal details. This will eventually become entirely
 *	internal to the tty port.
 */

void tty_port_raise_dtr_rts(struct tty_port *port)
{
	if (port->ops->dtr_rts)
		port->ops->dtr_rts(port, 1);
=======
 * tty_port_raise_dtr_rts	-	Raise DTR/RTS
 * @port: tty port
 *
 * Wrapper for the DTR/RTS raise logic. For the moment this is used to hide
 * some internal details. This will eventually become entirely internal to the
 * tty port.
 */
void tty_port_raise_dtr_rts(struct tty_port *port)
{
	if (port->ops->dtr_rts)
		port->ops->dtr_rts(port, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(tty_port_raise_dtr_rts);

/**
<<<<<<< HEAD
 *	tty_port_lower_dtr_rts	-	Lower DTR/RTS
 *	@port: tty port
 *
 *	Wrapper for the DTR/RTS raise logic. For the moment this is used
 *	to hide some internal details. This will eventually become entirely
 *	internal to the tty port.
 */

void tty_port_lower_dtr_rts(struct tty_port *port)
{
	if (port->ops->dtr_rts)
		port->ops->dtr_rts(port, 0);
=======
 * tty_port_lower_dtr_rts	-	Lower DTR/RTS
 * @port: tty port
 *
 * Wrapper for the DTR/RTS raise logic. For the moment this is used to hide
 * some internal details. This will eventually become entirely internal to the
 * tty port.
 */
void tty_port_lower_dtr_rts(struct tty_port *port)
{
	if (port->ops->dtr_rts)
		port->ops->dtr_rts(port, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(tty_port_lower_dtr_rts);

/**
<<<<<<< HEAD
 *	tty_port_block_til_ready	-	Waiting logic for tty open
 *	@port: the tty port being opened
 *	@tty: the tty device being bound
 *	@filp: the file pointer of the opener
 *
 *	Implement the core POSIX/SuS tty behaviour when opening a tty device.
 *	Handles:
 *		- hangup (both before and during)
 *		- non blocking open
 *		- rts/dtr/dcd
 *		- signals
 *		- port flags and counts
 *
 *	The passed tty_port must implement the carrier_raised method if it can
 *	do carrier detect and the dtr_rts method if it supports software
 *	management of these lines. Note that the dtr/rts raise is done each
 *	iteration as a hangup may have previously dropped them while we wait.
 */

=======
 * tty_port_block_til_ready	-	Waiting logic for tty open
 * @port: the tty port being opened
 * @tty: the tty device being bound
 * @filp: the file pointer of the opener or %NULL
 *
 * Implement the core POSIX/SuS tty behaviour when opening a tty device.
 * Handles:
 *
 *	- hangup (both before and during)
 *	- non blocking open
 *	- rts/dtr/dcd
 *	- signals
 *	- port flags and counts
 *
 * The passed @port must implement the @port->ops->carrier_raised method if it
 * can do carrier detect and the @port->ops->dtr_rts method if it supports
 * software management of these lines. Note that the dtr/rts raise is done each
 * iteration as a hangup may have previously dropped them while we wait.
 *
 * Caller holds tty lock.
 *
 * Note: May drop and reacquire tty lock when blocking, so @tty and @port may
 * have changed state (eg., may have been hung up).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int tty_port_block_til_ready(struct tty_port *port,
				struct tty_struct *tty, struct file *filp)
{
	int do_clocal = 0, retval;
	unsigned long flags;
	DEFINE_WAIT(wait);

<<<<<<< HEAD
	/* block if port is in the process of being closed */
	if (tty_hung_up_p(filp) || port->flags & ASYNC_CLOSING) {
		wait_event_interruptible_tty(port->close_wait,
				!(port->flags & ASYNC_CLOSING));
		if (port->flags & ASYNC_HUP_NOTIFY)
			return -EAGAIN;
		else
			return -ERESTARTSYS;
	}

	/* if non-blocking mode is set we can pass directly to open unless
	   the port has just hung up or is in another error state */
	if (tty->flags & (1 << TTY_IO_ERROR)) {
		port->flags |= ASYNC_NORMAL_ACTIVE;
		return 0;
	}
	if (filp->f_flags & O_NONBLOCK) {
		/* Indicate we are open */
		if (tty->termios->c_cflag & CBAUD)
			tty_port_raise_dtr_rts(port);
		port->flags |= ASYNC_NORMAL_ACTIVE;
=======
	/* if non-blocking mode is set we can pass directly to open unless
	 * the port has just hung up or is in another error state.
	 */
	if (tty_io_error(tty)) {
		tty_port_set_active(port, true);
		return 0;
	}
	if (filp == NULL || (filp->f_flags & O_NONBLOCK)) {
		/* Indicate we are open */
		if (C_BAUD(tty))
			tty_port_raise_dtr_rts(port);
		tty_port_set_active(port, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	if (C_CLOCAL(tty))
		do_clocal = 1;

	/* Block waiting until we can proceed. We may need to wait for the
<<<<<<< HEAD
	   carrier, but we must also wait for any close that is in progress
	   before the next open may complete */
=======
	 * carrier, but we must also wait for any close that is in progress
	 * before the next open may complete.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = 0;

	/* The port lock protects the port counts */
	spin_lock_irqsave(&port->lock, flags);
<<<<<<< HEAD
	if (!tty_hung_up_p(filp))
		port->count--;
=======
	port->count--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port->blocked_open++;
	spin_unlock_irqrestore(&port->lock, flags);

	while (1) {
		/* Indicate we are open */
<<<<<<< HEAD
		if (tty->termios->c_cflag & CBAUD)
=======
		if (C_BAUD(tty) && tty_port_initialized(port))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			tty_port_raise_dtr_rts(port);

		prepare_to_wait(&port->open_wait, &wait, TASK_INTERRUPTIBLE);
		/* Check for a hangup or uninitialised port.
<<<<<<< HEAD
							Return accordingly */
		if (tty_hung_up_p(filp) || !(port->flags & ASYNC_INITIALIZED)) {
=======
		 * Return accordingly.
		 */
		if (tty_hung_up_p(filp) || !tty_port_initialized(port)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (port->flags & ASYNC_HUP_NOTIFY)
				retval = -EAGAIN;
			else
				retval = -ERESTARTSYS;
			break;
		}
		/*
		 * Probe the carrier. For devices with no carrier detect
		 * tty_port_carrier_raised will always return true.
		 * Never ask drivers if CLOCAL is set, this causes troubles
		 * on some hardware.
		 */
<<<<<<< HEAD
		if (!(port->flags & ASYNC_CLOSING) &&
				(do_clocal || tty_port_carrier_raised(port)))
=======
		if (do_clocal || tty_port_carrier_raised(port))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			break;
		}
<<<<<<< HEAD
		tty_unlock();
		schedule();
		tty_lock();
=======
		tty_unlock(tty);
		schedule();
		tty_lock(tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	finish_wait(&port->open_wait, &wait);

	/* Update counts. A parallel hangup will have set count to zero and
<<<<<<< HEAD
	   we must not mess that up further */
=======
	 * we must not mess that up further.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock_irqsave(&port->lock, flags);
	if (!tty_hung_up_p(filp))
		port->count++;
	port->blocked_open--;
<<<<<<< HEAD
	if (retval == 0)
		port->flags |= ASYNC_NORMAL_ACTIVE;
	spin_unlock_irqrestore(&port->lock, flags);
=======
	spin_unlock_irqrestore(&port->lock, flags);
	if (retval == 0)
		tty_port_set_active(port, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}
EXPORT_SYMBOL(tty_port_block_til_ready);

<<<<<<< HEAD
=======
static void tty_port_drain_delay(struct tty_port *port, struct tty_struct *tty)
{
	unsigned int bps = tty_get_baud_rate(tty);
	long timeout;

	if (bps > 1200) {
		timeout = (HZ * 10 * port->drain_delay) / bps;
		timeout = max_t(long, timeout, HZ / 10);
	} else {
		timeout = 2 * HZ;
	}
	schedule_timeout_interruptible(timeout);
}

/**
 * tty_port_close_start - helper for tty->ops->close, part 1/2
 * @port: tty_port of the device
 * @tty: tty being closed
 * @filp: passed file pointer
 *
 * Decrements and checks open count. Flushes the port if this is the last
 * close. That means, dropping the data from the outpu buffer on the device and
 * waiting for sending logic to finish. The rest of close handling is performed
 * in tty_port_close_end().
 *
 * Locking: Caller holds tty lock.
 *
 * Return: 1 if this is the last close, otherwise 0
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int tty_port_close_start(struct tty_port *port,
				struct tty_struct *tty, struct file *filp)
{
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);
	if (tty_hung_up_p(filp)) {
		spin_unlock_irqrestore(&port->lock, flags);
		return 0;
	}

	if (tty->count == 1 && port->count != 1) {
		printk(KERN_WARNING
		    "tty_port_close_start: tty->count = 1 port count = %d.\n",
								port->count);
		port->count = 1;
	}
	if (--port->count < 0) {
		printk(KERN_WARNING "tty_port_close_start: count = %d\n",
								port->count);
=======
	if (tty_hung_up_p(filp))
		return 0;

	spin_lock_irqsave(&port->lock, flags);
	if (tty->count == 1 && port->count != 1) {
		tty_warn(tty, "%s: tty->count = 1 port count = %d\n", __func__,
			 port->count);
		port->count = 1;
	}
	if (--port->count < 0) {
		tty_warn(tty, "%s: bad port count (%d)\n", __func__,
			 port->count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->count = 0;
	}

	if (port->count) {
		spin_unlock_irqrestore(&port->lock, flags);
<<<<<<< HEAD
		if (port->ops->drop)
			port->ops->drop(port);
		return 0;
	}
	set_bit(ASYNCB_CLOSING, &port->flags);
	tty->closing = 1;
	spin_unlock_irqrestore(&port->lock, flags);
	/* Don't block on a stalled port, just pull the chain */
	if (tty->flow_stopped)
		tty_driver_flush_buffer(tty);
	if (test_bit(ASYNCB_INITIALIZED, &port->flags) &&
			port->closing_wait != ASYNC_CLOSING_WAIT_NONE)
		tty_wait_until_sent_from_close(tty, port->closing_wait);
	if (port->drain_delay) {
		unsigned int bps = tty_get_baud_rate(tty);
		long timeout;

		if (bps > 1200)
			timeout = max_t(long,
				(HZ * 10 * port->drain_delay) / bps, HZ / 10);
		else
			timeout = 2 * HZ;
		schedule_timeout_interruptible(timeout);
=======
		return 0;
	}
	spin_unlock_irqrestore(&port->lock, flags);

	tty->closing = 1;

	if (tty_port_initialized(port)) {
		/* Don't block on a stalled port, just pull the chain */
		if (tty->flow.tco_stopped)
			tty_driver_flush_buffer(tty);
		if (port->closing_wait != ASYNC_CLOSING_WAIT_NONE)
			tty_wait_until_sent(tty, port->closing_wait);
		if (port->drain_delay)
			tty_port_drain_delay(port, tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* Flush the ldisc buffering */
	tty_ldisc_flush(tty);

<<<<<<< HEAD
	/* Drop DTR/RTS if HUPCL is set. This causes any attached modem to
	   hang up the line */
	if (tty->termios->c_cflag & HUPCL)
		tty_port_lower_dtr_rts(port);

	/* Don't call port->drop for the last reference. Callers will want
	   to drop the last active reference in ->shutdown() or the tty
	   shutdown path */
=======
	/* Report to caller this is the last port reference */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}
EXPORT_SYMBOL(tty_port_close_start);

<<<<<<< HEAD
=======
/**
 * tty_port_close_end - helper for tty->ops->close, part 2/2
 * @port: tty_port of the device
 * @tty: tty being closed
 *
 * This is a continuation of the first part: tty_port_close_start(). This
 * should be called after turning off the device. It flushes the data from the
 * line discipline and delays the close by @port->close_delay.
 *
 * Locking: Caller holds tty lock.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tty_port_close_end(struct tty_port *port, struct tty_struct *tty)
{
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);
	tty->closing = 0;

	if (port->blocked_open) {
		spin_unlock_irqrestore(&port->lock, flags);
		if (port->close_delay) {
			msleep_interruptible(
				jiffies_to_msecs(port->close_delay));
		}
		spin_lock_irqsave(&port->lock, flags);
		wake_up_interruptible(&port->open_wait);
	}
	port->flags &= ~(ASYNC_NORMAL_ACTIVE | ASYNC_CLOSING);
	wake_up_interruptible(&port->close_wait);
	spin_unlock_irqrestore(&port->lock, flags);
}
EXPORT_SYMBOL(tty_port_close_end);

=======
	tty_ldisc_flush(tty);
	tty->closing = 0;

	spin_lock_irqsave(&port->lock, flags);

	if (port->blocked_open) {
		spin_unlock_irqrestore(&port->lock, flags);
		if (port->close_delay)
			msleep_interruptible(jiffies_to_msecs(port->close_delay));
		spin_lock_irqsave(&port->lock, flags);
		wake_up_interruptible(&port->open_wait);
	}
	spin_unlock_irqrestore(&port->lock, flags);
	tty_port_set_active(port, false);
}
EXPORT_SYMBOL(tty_port_close_end);

/**
 * tty_port_close - generic tty->ops->close handler
 * @port: tty_port of the device
 * @tty: tty being closed
 * @filp: passed file pointer
 *
 * It is a generic helper to be used in driver's @tty->ops->close. It wraps a
 * sequence of tty_port_close_start(), tty_port_shutdown(), and
 * tty_port_close_end(). The latter two are called only if this is the last
 * close. See the respective functions for the details.
 *
 * Locking: Caller holds tty lock
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tty_port_close(struct tty_port *port, struct tty_struct *tty,
							struct file *filp)
{
	if (tty_port_close_start(port, tty, filp) == 0)
		return;
<<<<<<< HEAD
	tty_port_shutdown(port);
	set_bit(TTY_IO_ERROR, &tty->flags);
=======
	tty_port_shutdown(port, tty);
	if (!port->console)
		set_bit(TTY_IO_ERROR, &tty->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_port_close_end(port, tty);
	tty_port_tty_set(port, NULL);
}
EXPORT_SYMBOL(tty_port_close);

<<<<<<< HEAD
=======
/**
 * tty_port_install - generic tty->ops->install handler
 * @port: tty_port of the device
 * @driver: tty_driver for this device
 * @tty: tty to be installed
 *
 * It is the same as tty_standard_install() except the provided @port is linked
 * to a concrete tty specified by @tty. Use this or tty_port_register_device()
 * (or both). Call tty_port_link_device() as a last resort.
 */
int tty_port_install(struct tty_port *port, struct tty_driver *driver,
		struct tty_struct *tty)
{
	tty->port = port;
	return tty_standard_install(driver, tty);
}
EXPORT_SYMBOL_GPL(tty_port_install);

/**
 * tty_port_open - generic tty->ops->open handler
 * @port: tty_port of the device
 * @tty: tty to be opened
 * @filp: passed file pointer
 *
 * It is a generic helper to be used in driver's @tty->ops->open. It activates
 * the devices using @port->ops->activate if not active already. And waits for
 * the device to be ready using tty_port_block_til_ready() (e.g.  raises
 * DTR/CTS and waits for carrier).
 *
 * Note that @port->ops->shutdown is not called when @port->ops->activate
 * returns an error (on the contrary, @tty->ops->close is).
 *
 * Locking: Caller holds tty lock.
 *
 * Note: may drop and reacquire tty lock (in tty_port_block_til_ready()) so
 * @tty and @port may have changed state (eg., may be hung up now).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int tty_port_open(struct tty_port *port, struct tty_struct *tty,
							struct file *filp)
{
	spin_lock_irq(&port->lock);
<<<<<<< HEAD
	if (!tty_hung_up_p(filp))
		++port->count;
=======
	++port->count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irq(&port->lock);
	tty_port_tty_set(port, tty);

	/*
	 * Do the device-specific open only if the hardware isn't
	 * already initialized. Serialize open and shutdown using the
	 * port mutex.
	 */

	mutex_lock(&port->mutex);

<<<<<<< HEAD
	if (!test_bit(ASYNCB_INITIALIZED, &port->flags)) {
		clear_bit(TTY_IO_ERROR, &tty->flags);
		if (port->ops->activate) {
			int retval = port->ops->activate(port, tty);
=======
	if (!tty_port_initialized(port)) {
		clear_bit(TTY_IO_ERROR, &tty->flags);
		if (port->ops->activate) {
			int retval = port->ops->activate(port, tty);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (retval) {
				mutex_unlock(&port->mutex);
				return retval;
			}
		}
<<<<<<< HEAD
		set_bit(ASYNCB_INITIALIZED, &port->flags);
=======
		tty_port_set_initialized(port, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&port->mutex);
	return tty_port_block_til_ready(port, tty, filp);
}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(tty_port_open);
