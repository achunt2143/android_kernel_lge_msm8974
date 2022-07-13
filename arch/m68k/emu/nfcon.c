/*
 * ARAnyM console driver
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/natfeat.h>

static int stderr_id;
<<<<<<< HEAD
static struct tty_driver *nfcon_tty_driver;

static void nfputs(const char *str, unsigned int count)
{
	char buf[68];
=======
static struct tty_port nfcon_tty_port;
static struct tty_driver *nfcon_tty_driver;

static void nfputs(const u8 *str, size_t count)
{
	u8 buf[68];
	unsigned long phys = virt_to_phys(buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf[64] = 0;
	while (count > 64) {
		memcpy(buf, str, 64);
<<<<<<< HEAD
		nf_call(stderr_id, buf);
=======
		nf_call(stderr_id, phys);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		str += 64;
		count -= 64;
	}
	memcpy(buf, str, count);
	buf[count] = 0;
<<<<<<< HEAD
	nf_call(stderr_id, buf);
=======
	nf_call(stderr_id, phys);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void nfcon_write(struct console *con, const char *str,
			unsigned int count)
{
	nfputs(str, count);
}

static struct tty_driver *nfcon_device(struct console *con, int *index)
{
	*index = 0;
<<<<<<< HEAD
	return (con->flags & CON_ENABLED) ? nfcon_tty_driver : NULL;
=======
	return console_is_registered(con) ? nfcon_tty_driver : NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct console nf_console = {
	.name	= "nfcon",
	.write	= nfcon_write,
	.device	= nfcon_device,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
};


static int nfcon_tty_open(struct tty_struct *tty, struct file *filp)
{
	return 0;
}

static void nfcon_tty_close(struct tty_struct *tty, struct file *filp)
{
}

<<<<<<< HEAD
static int nfcon_tty_write(struct tty_struct *tty, const unsigned char *buf,
			   int count)
=======
static ssize_t nfcon_tty_write(struct tty_struct *tty, const u8 *buf,
			       size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	nfputs(buf, count);
	return count;
}

<<<<<<< HEAD
static int nfcon_tty_put_char(struct tty_struct *tty, unsigned char ch)
{
	char temp[2] = { ch, 0 };

	nf_call(stderr_id, temp);
	return 1;
}

static int nfcon_tty_write_room(struct tty_struct *tty)
=======
static int nfcon_tty_put_char(struct tty_struct *tty, u8 ch)
{
	u8 temp[2] = { ch, 0 };

	nf_call(stderr_id, virt_to_phys(temp));
	return 1;
}

static unsigned int nfcon_tty_write_room(struct tty_struct *tty)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 64;
}

static const struct tty_operations nfcon_tty_ops = {
	.open		= nfcon_tty_open,
	.close		= nfcon_tty_close,
	.write		= nfcon_tty_write,
	.put_char	= nfcon_tty_put_char,
	.write_room	= nfcon_tty_write_room,
};

#ifndef MODULE

static int __init nf_debug_setup(char *arg)
{
	if (strcmp(arg, "nfcon"))
		return 0;

	stderr_id = nf_get_id("NF_STDERR");
	if (stderr_id) {
<<<<<<< HEAD
=======
		/*
		 * The console will be enabled when debug=nfcon is specified
		 * as a kernel parameter. Since this is a non-standard way
		 * of enabling consoles, it must be explicitly enabled.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nf_console.flags |= CON_ENABLED;
		register_console(&nf_console);
	}

	return 0;
}

early_param("debug", nf_debug_setup);

#endif /* !MODULE */

static int __init nfcon_init(void)
{
<<<<<<< HEAD
=======
	struct tty_driver *driver;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int res;

	stderr_id = nf_get_id("NF_STDERR");
	if (!stderr_id)
		return -ENODEV;

<<<<<<< HEAD
	nfcon_tty_driver = alloc_tty_driver(1);
	if (!nfcon_tty_driver)
		return -ENOMEM;

	nfcon_tty_driver->driver_name = "nfcon";
	nfcon_tty_driver->name = "nfcon";
	nfcon_tty_driver->type = TTY_DRIVER_TYPE_SYSTEM;
	nfcon_tty_driver->subtype = SYSTEM_TYPE_TTY;
	nfcon_tty_driver->init_termios = tty_std_termios;
	nfcon_tty_driver->flags = TTY_DRIVER_REAL_RAW;

	tty_set_operations(nfcon_tty_driver, &nfcon_tty_ops);
	res = tty_register_driver(nfcon_tty_driver);
	if (res) {
		pr_err("failed to register nfcon tty driver\n");
		put_tty_driver(nfcon_tty_driver);
		return res;
	}

	if (!(nf_console.flags & CON_ENABLED))
=======
	driver = tty_alloc_driver(1, TTY_DRIVER_REAL_RAW);
	if (IS_ERR(driver))
		return PTR_ERR(driver);

	tty_port_init(&nfcon_tty_port);

	driver->driver_name = "nfcon";
	driver->name = "nfcon";
	driver->type = TTY_DRIVER_TYPE_SYSTEM;
	driver->subtype = SYSTEM_TYPE_TTY;
	driver->init_termios = tty_std_termios;

	tty_set_operations(driver, &nfcon_tty_ops);
	tty_port_link_device(&nfcon_tty_port, driver, 0);
	res = tty_register_driver(driver);
	if (res) {
		pr_err("failed to register nfcon tty driver\n");
		tty_driver_kref_put(driver);
		tty_port_destroy(&nfcon_tty_port);
		return res;
	}

	nfcon_tty_driver = driver;

	if (!console_is_registered(&nf_console))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		register_console(&nf_console);

	return 0;
}

static void __exit nfcon_exit(void)
{
	unregister_console(&nf_console);
	tty_unregister_driver(nfcon_tty_driver);
<<<<<<< HEAD
	put_tty_driver(nfcon_tty_driver);
=======
	tty_driver_kref_put(nfcon_tty_driver);
	tty_port_destroy(&nfcon_tty_port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(nfcon_init);
module_exit(nfcon_exit);

MODULE_LICENSE("GPL");
