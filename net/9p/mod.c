<<<<<<< HEAD
/*
 *  net/9p/9p.c
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  9P entry point
 *
 *  Copyright (C) 2007 by Latchesar Ionkov <lucho@ionkov.net>
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/kmod.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <net/9p/9p.h>
#include <linux/fs.h>
#include <linux/parser.h>
#include <net/9p/client.h>
#include <net/9p/transport.h>
#include <linux/list.h>
#include <linux/spinlock.h>

#ifdef CONFIG_NET_9P_DEBUG
<<<<<<< HEAD
unsigned int p9_debug_level = 0;	/* feature-rific global debug level  */
=======
unsigned int p9_debug_level;	/* feature-rific global debug level  */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(p9_debug_level);
module_param_named(debug, p9_debug_level, uint, 0);
MODULE_PARM_DESC(debug, "9P debugging level");

void _p9_debug(enum p9_debug_flags level, const char *func,
<<<<<<< HEAD
		const char *fmt, ...)
=======
	       const char *fmt, ...)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct va_format vaf;
	va_list args;

	if ((p9_debug_level & level) != level)
		return;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	if (level == P9_DEBUG_9P)
		pr_notice("(%8.8d) %pV", task_pid_nr(current), &vaf);
	else
		pr_notice("-- %s (%d): %pV", func, task_pid_nr(current), &vaf);

	va_end(args);
}
EXPORT_SYMBOL(_p9_debug);
#endif

<<<<<<< HEAD
/*
 * Dynamic Transport Registration Routines
 *
 */
=======
/* Dynamic Transport Registration Routines */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(v9fs_trans_lock);
static LIST_HEAD(v9fs_trans_list);

/**
 * v9fs_register_trans - register a new transport with 9p
 * @m: structure describing the transport module and entry points
 *
 */
void v9fs_register_trans(struct p9_trans_module *m)
{
	spin_lock(&v9fs_trans_lock);
	list_add_tail(&m->list, &v9fs_trans_list);
	spin_unlock(&v9fs_trans_lock);
}
EXPORT_SYMBOL(v9fs_register_trans);

/**
 * v9fs_unregister_trans - unregister a 9p transport
 * @m: the transport to remove
 *
 */
void v9fs_unregister_trans(struct p9_trans_module *m)
{
	spin_lock(&v9fs_trans_lock);
	list_del_init(&m->list);
	spin_unlock(&v9fs_trans_lock);
}
EXPORT_SYMBOL(v9fs_unregister_trans);

<<<<<<< HEAD
/**
 * v9fs_get_trans_by_name - get transport with the matching name
 * @name: string identifying transport
 *
 */
struct p9_trans_module *v9fs_get_trans_by_name(char *s)
=======
static struct p9_trans_module *_p9_get_trans_by_name(const char *s)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct p9_trans_module *t, *found = NULL;

	spin_lock(&v9fs_trans_lock);

	list_for_each_entry(t, &v9fs_trans_list, list)
		if (strcmp(t->name, s) == 0 &&
		    try_module_get(t->owner)) {
			found = t;
			break;
		}

	spin_unlock(&v9fs_trans_lock);
<<<<<<< HEAD
=======

	return found;
}

/**
 * v9fs_get_trans_by_name - get transport with the matching name
 * @s: string identifying transport
 *
 */
struct p9_trans_module *v9fs_get_trans_by_name(const char *s)
{
	struct p9_trans_module *found = NULL;

	found = _p9_get_trans_by_name(s);

#ifdef CONFIG_MODULES
	if (!found) {
		request_module("9p-%s", s);
		found = _p9_get_trans_by_name(s);
	}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return found;
}
EXPORT_SYMBOL(v9fs_get_trans_by_name);

<<<<<<< HEAD
=======
static const char * const v9fs_default_transports[] = {
	"virtio", "tcp", "fd", "unix", "xen", "rdma",
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * v9fs_get_default_trans - get the default transport
 *
 */

struct p9_trans_module *v9fs_get_default_trans(void)
{
	struct p9_trans_module *t, *found = NULL;
<<<<<<< HEAD
=======
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&v9fs_trans_lock);

	list_for_each_entry(t, &v9fs_trans_list, list)
		if (t->def && try_module_get(t->owner)) {
			found = t;
			break;
		}

	if (!found)
		list_for_each_entry(t, &v9fs_trans_list, list)
			if (try_module_get(t->owner)) {
				found = t;
				break;
			}

	spin_unlock(&v9fs_trans_lock);
<<<<<<< HEAD
=======

	for (i = 0; !found && i < ARRAY_SIZE(v9fs_default_transports); i++)
		found = v9fs_get_trans_by_name(v9fs_default_transports[i]);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return found;
}
EXPORT_SYMBOL(v9fs_get_default_trans);

/**
 * v9fs_put_trans - put trans
 * @m: transport to put
 *
 */
void v9fs_put_trans(struct p9_trans_module *m)
{
	if (m)
		module_put(m->owner);
}

/**
 * init_p9 - Initialize module
 *
 */
static int __init init_p9(void)
{
<<<<<<< HEAD
	int ret = 0;

	p9_error_init();
	pr_info("Installing 9P2000 support\n");
	p9_trans_fd_init();
=======
	int ret;

	ret = p9_client_init();
	if (ret)
		return ret;

	p9_error_init();
	pr_info("Installing 9P2000 support\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/**
 * exit_p9 - shutdown module
 *
 */

static void __exit exit_p9(void)
{
	pr_info("Unloading 9P2000 support\n");

<<<<<<< HEAD
	p9_trans_fd_exit();
=======
	p9_client_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_p9)
module_exit(exit_p9)

MODULE_AUTHOR("Latchesar Ionkov <lucho@ionkov.net>");
MODULE_AUTHOR("Eric Van Hensbergen <ericvh@gmail.com>");
MODULE_AUTHOR("Ron Minnich <rminnich@lanl.gov>");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Plan 9 Resource Sharing Support (9P2000)");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
