<<<<<<< HEAD
/* Copyright (c) 2007 Coraid, Inc.  See COPYING for GPL terms. */
=======
/* Copyright (c) 2012 Coraid, Inc.  See COPYING for GPL terms. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * aoemain.c
 * Module initialization routines, discover timer
 */

#include <linux/hdreg.h>
#include <linux/blkdev.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include "aoe.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sam Hopkins <sah@coraid.com>");
MODULE_DESCRIPTION("AoE block/char driver for 2.6.2 and newer 2.6 kernels");
MODULE_VERSION(VERSION);

<<<<<<< HEAD
enum { TINIT, TRUN, TKILL };

static void
discover_timer(ulong vp)
{
	static struct timer_list t;
	static volatile ulong die;
	static spinlock_t lock;
	ulong flags;
	enum { DTIMERTICK = HZ * 60 }; /* one minute */

	switch (vp) {
	case TINIT:
		init_timer(&t);
		spin_lock_init(&lock);
		t.data = TRUN;
		t.function = discover_timer;
		die = 0;
	case TRUN:
		spin_lock_irqsave(&lock, flags);
		if (!die) {
			t.expires = jiffies + DTIMERTICK;
			add_timer(&t);
		}
		spin_unlock_irqrestore(&lock, flags);

		aoecmd_cfg(0xffff, 0xff);
		return;
	case TKILL:
		spin_lock_irqsave(&lock, flags);
		die = 1;
		spin_unlock_irqrestore(&lock, flags);

		del_timer_sync(&t);
	default:
		return;
	}
}

static void
aoe_exit(void)
{
	discover_timer(TKILL);

	aoenet_exit();
	unregister_blkdev(AOE_MAJOR, DEVICE_NAME);
	aoechr_exit();
	aoedev_exit();
	aoeblk_exit();		/* free cache after de-allocating bufs */
=======
static struct timer_list timer;
struct workqueue_struct *aoe_wq;

static void discover_timer(struct timer_list *t)
{
	mod_timer(t, jiffies + HZ * 60); /* one minute */

	aoecmd_cfg(0xffff, 0xff);
}

static void __exit
aoe_exit(void)
{
	del_timer_sync(&timer);

	aoenet_exit();
	unregister_blkdev(AOE_MAJOR, DEVICE_NAME);
	aoecmd_exit();
	aoechr_exit();
	aoedev_exit();
	aoeblk_exit();		/* free cache after de-allocating bufs */
	destroy_workqueue(aoe_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init
aoe_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = aoedev_init();
	if (ret)
		return ret;
=======
	aoe_wq = alloc_workqueue("aoe_wq", 0, 0);
	if (!aoe_wq)
		return -ENOMEM;

	ret = aoedev_init();
	if (ret)
		goto dev_fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = aoechr_init();
	if (ret)
		goto chr_fail;
	ret = aoeblk_init();
	if (ret)
		goto blk_fail;
	ret = aoenet_init();
	if (ret)
		goto net_fail;
<<<<<<< HEAD
=======
	ret = aoecmd_init();
	if (ret)
		goto cmd_fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = register_blkdev(AOE_MAJOR, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ERR "aoe: can't register major\n");
		goto blkreg_fail;
	}
<<<<<<< HEAD

	printk(KERN_INFO "aoe: AoE v%s initialised.\n", VERSION);
	discover_timer(TINIT);
	return 0;

 blkreg_fail:
=======
	printk(KERN_INFO "aoe: AoE v%s initialised.\n", VERSION);

	timer_setup(&timer, discover_timer, 0);
	discover_timer(&timer);
	return 0;
 blkreg_fail:
	aoecmd_exit();
 cmd_fail:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	aoenet_exit();
 net_fail:
	aoeblk_exit();
 blk_fail:
	aoechr_exit();
 chr_fail:
	aoedev_exit();
<<<<<<< HEAD
	
=======
 dev_fail:
	destroy_workqueue(aoe_wq);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk(KERN_INFO "aoe: initialisation failure.\n");
	return ret;
}

module_init(aoe_init);
module_exit(aoe_exit);

