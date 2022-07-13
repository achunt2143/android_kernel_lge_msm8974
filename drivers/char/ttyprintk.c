<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/char/ttyprintk.c
 *
 *  Copyright (C) 2010  Samo Pogacnik
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the smems of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * This pseudo device allows user to make printk messages. It is possible
 * to store "console" messages inline with kernel messages for better analyses
 * of the boot process, for example.
 */

<<<<<<< HEAD
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/export.h>

struct ttyprintk_port {
	struct tty_port port;
	struct mutex port_write_mutex;
=======
#include <linux/console.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/spinlock.h>

struct ttyprintk_port {
	struct tty_port port;
	spinlock_t spinlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct ttyprintk_port tpk_port;

/*
 * Our simple preformatting supports transparent output of (time-stamped)
 * printk messages (also suitable for logging service):
 * - any cr is replaced by nl
 * - adds a ttyprintk source tag in front of each line
<<<<<<< HEAD
 * - too long message is fragmeted, with '\'nl between fragments
 * - TPK_STR_SIZE isn't really the write_room limiting factor, bcause
=======
 * - too long message is fragmented, with '\'nl between fragments
 * - TPK_STR_SIZE isn't really the write_room limiting factor, because
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   it is emptied on the fly during preformatting.
 */
#define TPK_STR_SIZE 508 /* should be bigger then max expected line length */
#define TPK_MAX_ROOM 4096 /* we could assume 4K for instance */
<<<<<<< HEAD
static const char *tpk_tag = "[U] "; /* U for User */
static int tpk_curr;

static int tpk_printk(const unsigned char *buf, int count)
{
	static char tmp[TPK_STR_SIZE + 4];
	int i = tpk_curr;

	if (buf == NULL) {
		/* flush tmp[] */
		if (tpk_curr > 0) {
			/* non nl or cr terminated message - add nl */
			tmp[tpk_curr + 0] = '\n';
			tmp[tpk_curr + 1] = '\0';
			printk(KERN_INFO "%s%s", tpk_tag, tmp);
			tpk_curr = 0;
		}
		return i;
	}

	for (i = 0; i < count; i++) {
		tmp[tpk_curr] = buf[i];
		if (tpk_curr < TPK_STR_SIZE) {
			switch (buf[i]) {
			case '\r':
				/* replace cr with nl */
				tmp[tpk_curr + 0] = '\n';
				tmp[tpk_curr + 1] = '\0';
				printk(KERN_INFO "%s%s", tpk_tag, tmp);
				tpk_curr = 0;
				if ((i + 1) < count && buf[i + 1] == '\n')
					i++;
				break;
			case '\n':
				tmp[tpk_curr + 1] = '\0';
				printk(KERN_INFO "%s%s", tpk_tag, tmp);
				tpk_curr = 0;
				break;
			default:
				tpk_curr++;
			}
		} else {
			/* end of tmp buffer reached: cut the message in two */
			tmp[tpk_curr + 1] = '\\';
			tmp[tpk_curr + 2] = '\n';
			tmp[tpk_curr + 3] = '\0';
			printk(KERN_INFO "%s%s", tpk_tag, tmp);
			tpk_curr = 0;
=======
#define TPK_PREFIX KERN_SOH __stringify(CONFIG_TTY_PRINTK_LEVEL)

static int tpk_curr;

static u8 tpk_buffer[TPK_STR_SIZE + 4];

static void tpk_flush(void)
{
	if (tpk_curr > 0) {
		tpk_buffer[tpk_curr] = '\0';
		printk(TPK_PREFIX "[U] %s\n", tpk_buffer);
		tpk_curr = 0;
	}
}

static int tpk_printk(const u8 *buf, size_t count)
{
	size_t i;

	for (i = 0; i < count; i++) {
		if (tpk_curr >= TPK_STR_SIZE) {
			/* end of tmp buffer reached: cut the message in two */
			tpk_buffer[tpk_curr++] = '\\';
			tpk_flush();
		}

		switch (buf[i]) {
		case '\r':
			tpk_flush();
			if ((i + 1) < count && buf[i + 1] == '\n')
				i++;
			break;
		case '\n':
			tpk_flush();
			break;
		default:
			tpk_buffer[tpk_curr++] = buf[i];
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return count;
}

/*
 * TTY operations open function.
 */
static int tpk_open(struct tty_struct *tty, struct file *filp)
{
	tty->driver_data = &tpk_port;

	return tty_port_open(&tpk_port.port, tty, filp);
}

/*
 * TTY operations close function.
 */
static void tpk_close(struct tty_struct *tty, struct file *filp)
{
	struct ttyprintk_port *tpkp = tty->driver_data;

<<<<<<< HEAD
	mutex_lock(&tpkp->port_write_mutex);
	/* flush tpk_printk buffer */
	tpk_printk(NULL, 0);
	mutex_unlock(&tpkp->port_write_mutex);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_port_close(&tpkp->port, tty, filp);
}

/*
 * TTY operations write function.
 */
<<<<<<< HEAD
static int tpk_write(struct tty_struct *tty,
		const unsigned char *buf, int count)
{
	struct ttyprintk_port *tpkp = tty->driver_data;
	int ret;


	/* exclusive use of tpk_printk within this tty */
	mutex_lock(&tpkp->port_write_mutex);
	ret = tpk_printk(buf, count);
	mutex_unlock(&tpkp->port_write_mutex);
=======
static ssize_t tpk_write(struct tty_struct *tty, const u8 *buf, size_t count)
{
	struct ttyprintk_port *tpkp = tty->driver_data;
	unsigned long flags;
	int ret;

	/* exclusive use of tpk_printk within this tty */
	spin_lock_irqsave(&tpkp->spinlock, flags);
	ret = tpk_printk(buf, count);
	spin_unlock_irqrestore(&tpkp->spinlock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/*
 * TTY operations write_room function.
 */
<<<<<<< HEAD
static int tpk_write_room(struct tty_struct *tty)
=======
static unsigned int tpk_write_room(struct tty_struct *tty)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return TPK_MAX_ROOM;
}

/*
<<<<<<< HEAD
 * TTY operations ioctl function.
 */
static int tpk_ioctl(struct tty_struct *tty,
			unsigned int cmd, unsigned long arg)
{
	struct ttyprintk_port *tpkp = tty->driver_data;

	if (!tpkp)
		return -EINVAL;

	switch (cmd) {
	/* Stop TIOCCONS */
	case TIOCCONS:
		return -EOPNOTSUPP;
	default:
		return -ENOIOCTLCMD;
	}
	return 0;
=======
 * TTY operations hangup function.
 */
static void tpk_hangup(struct tty_struct *tty)
{
	struct ttyprintk_port *tpkp = tty->driver_data;

	tty_port_hangup(&tpkp->port);
}

/*
 * TTY port operations shutdown function.
 */
static void tpk_port_shutdown(struct tty_port *tport)
{
	struct ttyprintk_port *tpkp =
		container_of(tport, struct ttyprintk_port, port);
	unsigned long flags;

	spin_lock_irqsave(&tpkp->spinlock, flags);
	tpk_flush();
	spin_unlock_irqrestore(&tpkp->spinlock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct tty_operations ttyprintk_ops = {
	.open = tpk_open,
	.close = tpk_close,
	.write = tpk_write,
	.write_room = tpk_write_room,
<<<<<<< HEAD
	.ioctl = tpk_ioctl,
};

static struct tty_port_operations null_ops = { };

static struct tty_driver *ttyprintk_driver;

static int __init ttyprintk_init(void)
{
	int ret = -ENOMEM;
	void *rp;

	ttyprintk_driver = alloc_tty_driver(1);
	if (!ttyprintk_driver)
		return ret;
=======
	.hangup = tpk_hangup,
};

static const struct tty_port_operations tpk_port_ops = {
	.shutdown = tpk_port_shutdown,
};

static struct tty_driver *ttyprintk_driver;

static struct tty_driver *ttyprintk_console_device(struct console *c,
						   int *index)
{
	*index = 0;
	return ttyprintk_driver;
}

static struct console ttyprintk_console = {
	.name = "ttyprintk",
	.device = ttyprintk_console_device,
};

static int __init ttyprintk_init(void)
{
	int ret;

	spin_lock_init(&tpk_port.spinlock);

	ttyprintk_driver = tty_alloc_driver(1,
			TTY_DRIVER_RESET_TERMIOS |
			TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_UNNUMBERED_NODE);
	if (IS_ERR(ttyprintk_driver))
		return PTR_ERR(ttyprintk_driver);

	tty_port_init(&tpk_port.port);
	tpk_port.port.ops = &tpk_port_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ttyprintk_driver->driver_name = "ttyprintk";
	ttyprintk_driver->name = "ttyprintk";
	ttyprintk_driver->major = TTYAUX_MAJOR;
	ttyprintk_driver->minor_start = 3;
	ttyprintk_driver->type = TTY_DRIVER_TYPE_CONSOLE;
	ttyprintk_driver->init_termios = tty_std_termios;
	ttyprintk_driver->init_termios.c_oflag = OPOST | OCRNL | ONOCR | ONLRET;
<<<<<<< HEAD
	ttyprintk_driver->flags = TTY_DRIVER_RESET_TERMIOS |
		TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
	tty_set_operations(ttyprintk_driver, &ttyprintk_ops);
=======
	tty_set_operations(ttyprintk_driver, &ttyprintk_ops);
	tty_port_link_device(&tpk_port.port, ttyprintk_driver, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = tty_register_driver(ttyprintk_driver);
	if (ret < 0) {
		printk(KERN_ERR "Couldn't register ttyprintk driver\n");
		goto error;
	}

<<<<<<< HEAD
	/* create our unnumbered device */
	rp = device_create(tty_class, NULL, MKDEV(TTYAUX_MAJOR, 3), NULL,
				ttyprintk_driver->name);
	if (IS_ERR(rp)) {
		printk(KERN_ERR "Couldn't create ttyprintk device\n");
		ret = PTR_ERR(rp);
		goto error;
	}

	tty_port_init(&tpk_port.port);
	tpk_port.port.ops = &null_ops;
	mutex_init(&tpk_port.port_write_mutex);
=======
	register_console(&ttyprintk_console);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

error:
<<<<<<< HEAD
	put_tty_driver(ttyprintk_driver);
	ttyprintk_driver = NULL;
	return ret;
}
module_init(ttyprintk_init);
=======
	tty_driver_kref_put(ttyprintk_driver);
	tty_port_destroy(&tpk_port.port);
	return ret;
}

static void __exit ttyprintk_exit(void)
{
	unregister_console(&ttyprintk_console);
	tty_unregister_driver(ttyprintk_driver);
	tty_driver_kref_put(ttyprintk_driver);
	tty_port_destroy(&tpk_port.port);
}

device_initcall(ttyprintk_init);
module_exit(ttyprintk_exit);

MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
