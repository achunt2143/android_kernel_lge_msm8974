<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
<<<<<<< HEAD

/*
 * The 'big tty mutex'
 *
 * This mutex is taken and released by tty_lock() and tty_unlock(),
 * replacing the older big kernel lock.
 * It can no longer be taken recursively, and does not get
 * released implicitly while sleeping.
 *
 * Don't use in new code.
 */
static DEFINE_MUTEX(big_tty_mutex);
=======
#include "tty.h"

/* Legacy tty mutex glue */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Getting the big tty mutex.
 */
<<<<<<< HEAD
void __lockfunc tty_lock(void)
{
	mutex_lock(&big_tty_mutex);
}
EXPORT_SYMBOL(tty_lock);

void __lockfunc tty_unlock(void)
{
	mutex_unlock(&big_tty_mutex);
}
EXPORT_SYMBOL(tty_unlock);
=======

void tty_lock(struct tty_struct *tty)
{
	tty_kref_get(tty);
	mutex_lock(&tty->legacy_mutex);
}
EXPORT_SYMBOL(tty_lock);

int tty_lock_interruptible(struct tty_struct *tty)
{
	int ret;

	tty_kref_get(tty);
	ret = mutex_lock_interruptible(&tty->legacy_mutex);
	if (ret)
		tty_kref_put(tty);
	return ret;
}

void tty_unlock(struct tty_struct *tty)
{
	mutex_unlock(&tty->legacy_mutex);
	tty_kref_put(tty);
}
EXPORT_SYMBOL(tty_unlock);

void tty_lock_slave(struct tty_struct *tty)
{
	if (tty && tty != tty->link)
		tty_lock(tty);
}

void tty_unlock_slave(struct tty_struct *tty)
{
	if (tty && tty != tty->link)
		tty_unlock(tty);
}

void tty_set_lock_subclass(struct tty_struct *tty)
{
	lockdep_set_subclass(&tty->legacy_mutex, TTY_LOCK_SLAVE);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
