<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Linux VM pressure
 *
 * Copyright 2012 Linaro Ltd.
 *		  Anton Vorontsov <anton.vorontsov@linaro.org>
 *
 * Based on ideas from Andrew Morton, David Rientjes, KOSAKI Motohiro,
 * Leonid Moiseichuk, Mel Gorman, Minchan Kim and Pekka Enberg.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/cgroup.h>
#include <linux/fs.h>
#include <linux/log2.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/eventfd.h>
<<<<<<< HEAD
#include <linux/swap.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/notifier.h>
#include <linux/init.h>
=======
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/printk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/vmpressure.h>

/*
 * The window size (vmpressure_win) is the number of scanned pages before
 * we try to analyze scanned/reclaimed ratio. So the window is used as a
 * rate-limit tunable for the "low" level notification, and also for
 * averaging the ratio for medium/critical levels. Using small window
 * sizes can cause lot of false positives, but too big window size will
 * delay the notifications.
 *
 * As the vmscan reclaimer logic works with chunks which are multiple of
 * SWAP_CLUSTER_MAX, it makes sense to use it for the window size as well.
 *
 * TODO: Make the window size depend on machine size, as we do for vmstat
 * thresholds. Currently we set it to 512 pages (2MB for 4KB pages).
 */
static const unsigned long vmpressure_win = SWAP_CLUSTER_MAX * 16;

/*
 * These thresholds are used when we account memory pressure through
 * scanned/reclaimed ratio. The current values were chosen empirically. In
 * essence, they are percents: the higher the value, the more number
 * unsuccessful reclaims there were.
 */
static const unsigned int vmpressure_level_med = 60;
static const unsigned int vmpressure_level_critical = 95;

<<<<<<< HEAD
static struct vmpressure global_vmpressure;
BLOCKING_NOTIFIER_HEAD(vmpressure_notifier);

int vmpressure_notifier_register(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&vmpressure_notifier, nb);
}

int vmpressure_notifier_unregister(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&vmpressure_notifier, nb);
}

void vmpressure_notify(unsigned long pressure)
{
	blocking_notifier_call_chain(&vmpressure_notifier, pressure, NULL);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * When there are too little pages left to scan, vmpressure() may miss the
 * critical pressure as number of pages will be less than "window size".
 * However, in that case the vmscan priority will raise fast as the
 * reclaimer will try to scan LRUs more deeply.
 *
 * The vmscan logic considers these special priorities:
 *
 * prio == DEF_PRIORITY (12): reclaimer starts with that value
 * prio <= DEF_PRIORITY - 2 : kswapd becomes somewhat overwhelmed
 * prio == 0                : close to OOM, kernel scans every page in an lru
 *
 * Any value in this range is acceptable for this tunable (i.e. from 12 to
 * 0). Current value for the vmpressure_level_critical_prio is chosen
 * empirically, but the number, in essence, means that we consider
 * critical level when scanning depth is ~10% of the lru size (vmscan
 * scans 'lru_size >> prio' pages, so it is actually 12.5%, or one
 * eights).
 */
static const unsigned int vmpressure_level_critical_prio = ilog2(100 / 10);

static struct vmpressure *work_to_vmpressure(struct work_struct *work)
{
	return container_of(work, struct vmpressure, work);
}

<<<<<<< HEAD
#ifdef CONFIG_MEMCG
static struct vmpressure *cg_to_vmpressure(struct cgroup *cg)
{
	return css_to_vmpressure(cgroup_subsys_state(cg, mem_cgroup_subsys_id));
}

static struct vmpressure *vmpressure_parent(struct vmpressure *vmpr)
{
	struct cgroup *cg = vmpressure_to_css(vmpr)->cgroup;
	struct mem_cgroup *memcg = mem_cgroup_from_cont(cg);
=======
static struct vmpressure *vmpressure_parent(struct vmpressure *vmpr)
{
	struct mem_cgroup *memcg = vmpressure_to_memcg(vmpr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcg = parent_mem_cgroup(memcg);
	if (!memcg)
		return NULL;
	return memcg_to_vmpressure(memcg);
}
<<<<<<< HEAD
#else
static struct vmpressure *cg_to_vmpressure(struct cgroup *cg)
{
	return NULL;
}

static struct vmpressure *vmpressure_parent(struct vmpressure *vmpr)
{
	return NULL;
}
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum vmpressure_levels {
	VMPRESSURE_LOW = 0,
	VMPRESSURE_MEDIUM,
	VMPRESSURE_CRITICAL,
	VMPRESSURE_NUM_LEVELS,
};

<<<<<<< HEAD
=======
enum vmpressure_modes {
	VMPRESSURE_NO_PASSTHROUGH = 0,
	VMPRESSURE_HIERARCHY,
	VMPRESSURE_LOCAL,
	VMPRESSURE_NUM_MODES,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const char * const vmpressure_str_levels[] = {
	[VMPRESSURE_LOW] = "low",
	[VMPRESSURE_MEDIUM] = "medium",
	[VMPRESSURE_CRITICAL] = "critical",
};

<<<<<<< HEAD
=======
static const char * const vmpressure_str_modes[] = {
	[VMPRESSURE_NO_PASSTHROUGH] = "default",
	[VMPRESSURE_HIERARCHY] = "hierarchy",
	[VMPRESSURE_LOCAL] = "local",
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static enum vmpressure_levels vmpressure_level(unsigned long pressure)
{
	if (pressure >= vmpressure_level_critical)
		return VMPRESSURE_CRITICAL;
	else if (pressure >= vmpressure_level_med)
		return VMPRESSURE_MEDIUM;
	return VMPRESSURE_LOW;
}

<<<<<<< HEAD
static unsigned long vmpressure_calc_pressure(unsigned long scanned,
						    unsigned long reclaimed)
{
	unsigned long scale = scanned + reclaimed;
	unsigned long pressure;

	/*
=======
static enum vmpressure_levels vmpressure_calc_level(unsigned long scanned,
						    unsigned long reclaimed)
{
	unsigned long scale = scanned + reclaimed;
	unsigned long pressure = 0;

	/*
	 * reclaimed can be greater than scanned for things such as reclaimed
	 * slab pages. shrink_node() just adds reclaimed pages without a
	 * related increment to scanned pages.
	 */
	if (reclaimed >= scanned)
		goto out;
	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * We calculate the ratio (in percents) of how many pages were
	 * scanned vs. reclaimed in a given time frame (window). Note that
	 * time is in VM reclaimer's "ticks", i.e. number of pages
	 * scanned. This makes it possible to set desired reaction time
	 * and serves as a ratelimit.
	 */
	pressure = scale - (reclaimed * scale / scanned);
	pressure = pressure * 100 / scale;

<<<<<<< HEAD
	pr_debug("%s: %3lu  (s: %lu  r: %lu)\n", __func__, pressure,
		 scanned, reclaimed);

	return pressure;
=======
out:
	pr_debug("%s: %3lu  (s: %lu  r: %lu)\n", __func__, pressure,
		 scanned, reclaimed);

	return vmpressure_level(pressure);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct vmpressure_event {
	struct eventfd_ctx *efd;
	enum vmpressure_levels level;
<<<<<<< HEAD
=======
	enum vmpressure_modes mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head node;
};

static bool vmpressure_event(struct vmpressure *vmpr,
<<<<<<< HEAD
			     unsigned long scanned, unsigned long reclaimed)
{
	struct vmpressure_event *ev;
	enum vmpressure_levels level;
	unsigned long pressure;
	bool signalled = false;

	pressure = vmpressure_calc_pressure(scanned, reclaimed);
	level = vmpressure_level(pressure);

	mutex_lock(&vmpr->events_lock);

	list_for_each_entry(ev, &vmpr->events, node) {
		if (level >= ev->level) {
			eventfd_signal(ev->efd, 1);
			signalled = true;
		}
	}

	mutex_unlock(&vmpr->events_lock);

	return signalled;
=======
			     const enum vmpressure_levels level,
			     bool ancestor, bool signalled)
{
	struct vmpressure_event *ev;
	bool ret = false;

	mutex_lock(&vmpr->events_lock);
	list_for_each_entry(ev, &vmpr->events, node) {
		if (ancestor && ev->mode == VMPRESSURE_LOCAL)
			continue;
		if (signalled && ev->mode == VMPRESSURE_NO_PASSTHROUGH)
			continue;
		if (level < ev->level)
			continue;
		eventfd_signal(ev->efd);
		ret = true;
	}
	mutex_unlock(&vmpr->events_lock);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void vmpressure_work_fn(struct work_struct *work)
{
	struct vmpressure *vmpr = work_to_vmpressure(work);
	unsigned long scanned;
	unsigned long reclaimed;
<<<<<<< HEAD

=======
	enum vmpressure_levels level;
	bool ancestor = false;
	bool signalled = false;

	spin_lock(&vmpr->sr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Several contexts might be calling vmpressure(), so it is
	 * possible that the work was rescheduled again before the old
	 * work context cleared the counters. In that case we will run
	 * just after the old work returns, but then scanned might be zero
	 * here. No need for any locks here since we don't care if
	 * vmpr->reclaimed is in sync.
	 */
<<<<<<< HEAD
	if (!vmpr->scanned)
		return;

	mutex_lock(&vmpr->sr_lock);
	scanned = vmpr->scanned;
	reclaimed = vmpr->reclaimed;
	vmpr->scanned = 0;
	vmpr->reclaimed = 0;
	mutex_unlock(&vmpr->sr_lock);

	do {
		if (vmpressure_event(vmpr, scanned, reclaimed))
			break;
		/*
		 * If not handled, propagate the event upward into the
		 * hierarchy.
		 */
	} while ((vmpr = vmpressure_parent(vmpr)));
}

void vmpressure_memcg(gfp_t gfp, struct mem_cgroup *memcg,
		unsigned long scanned, unsigned long reclaimed)
{
	struct vmpressure *vmpr = memcg_to_vmpressure(memcg);

	BUG_ON(!vmpr);
=======
	scanned = vmpr->tree_scanned;
	if (!scanned) {
		spin_unlock(&vmpr->sr_lock);
		return;
	}

	reclaimed = vmpr->tree_reclaimed;
	vmpr->tree_scanned = 0;
	vmpr->tree_reclaimed = 0;
	spin_unlock(&vmpr->sr_lock);

	level = vmpressure_calc_level(scanned, reclaimed);

	do {
		if (vmpressure_event(vmpr, level, ancestor, signalled))
			signalled = true;
		ancestor = true;
	} while ((vmpr = vmpressure_parent(vmpr)));
}

/**
 * vmpressure() - Account memory pressure through scanned/reclaimed ratio
 * @gfp:	reclaimer's gfp mask
 * @memcg:	cgroup memory controller handle
 * @tree:	legacy subtree mode
 * @scanned:	number of pages scanned
 * @reclaimed:	number of pages reclaimed
 *
 * This function should be called from the vmscan reclaim path to account
 * "instantaneous" memory pressure (scanned/reclaimed ratio). The raw
 * pressure index is then further refined and averaged over time.
 *
 * If @tree is set, vmpressure is in traditional userspace reporting
 * mode: @memcg is considered the pressure root and userspace is
 * notified of the entire subtree's reclaim efficiency.
 *
 * If @tree is not set, reclaim efficiency is recorded for @memcg, and
 * only in-kernel users are notified.
 *
 * This function does not return any value.
 */
void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
		unsigned long scanned, unsigned long reclaimed)
{
	struct vmpressure *vmpr;

	if (mem_cgroup_disabled())
		return;

	/*
	 * The in-kernel users only care about the reclaim efficiency
	 * for this @memcg rather than the whole subtree, and there
	 * isn't and won't be any in-kernel user in a legacy cgroup.
	 */
	if (!cgroup_subsys_on_dfl(memory_cgrp_subsys) && !tree)
		return;

	vmpr = memcg_to_vmpressure(memcg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Here we only want to account pressure that userland is able to
	 * help us with. For example, suppose that DMA zone is under
	 * pressure; if we notify userland about that kind of pressure,
	 * then it will be mostly a waste as it will trigger unnecessary
	 * freeing of memory by userland (since userland is more likely to
	 * have HIGHMEM/MOVABLE pages instead of the DMA fallback). That
	 * is why we include only movable, highmem and FS/IO pages.
	 * Indirect reclaim (kswapd) sets sc->gfp_mask to GFP_KERNEL, so
	 * we account it too.
	 */
	if (!(gfp & (__GFP_HIGHMEM | __GFP_MOVABLE | __GFP_IO | __GFP_FS)))
		return;

	/*
	 * If we got here with no pages scanned, then that is an indicator
	 * that reclaimer was unable to find any shrinkable LRUs at the
	 * current scanning depth. But it does not mean that we should
	 * report the critical pressure, yet. If the scanning priority
	 * (scanning depth) goes too high (deep), we will be notified
	 * through vmpressure_prio(). But so far, keep calm.
	 */
	if (!scanned)
		return;

<<<<<<< HEAD
	mutex_lock(&vmpr->sr_lock);
	vmpr->scanned += scanned;
	vmpr->reclaimed += reclaimed;
	scanned = vmpr->scanned;
	mutex_unlock(&vmpr->sr_lock);

	if (scanned < vmpressure_win || work_pending(&vmpr->work))
		return;
	schedule_work(&vmpr->work);
}

void vmpressure_global(gfp_t gfp, unsigned long scanned,
		unsigned long reclaimed)
{
	struct vmpressure *vmpr = &global_vmpressure;
	unsigned long pressure;

	if (!(gfp & (__GFP_HIGHMEM | __GFP_MOVABLE | __GFP_IO | __GFP_FS)))
		return;

	if (!scanned)
		return;

	mutex_lock(&vmpr->sr_lock);
	vmpr->scanned += scanned;
	vmpr->reclaimed += reclaimed;
	scanned = vmpr->scanned;
	reclaimed = vmpr->reclaimed;
	mutex_unlock(&vmpr->sr_lock);

	if (scanned < vmpressure_win)
		return;

	mutex_lock(&vmpr->sr_lock);
	vmpr->scanned = 0;
	vmpr->reclaimed = 0;
	mutex_unlock(&vmpr->sr_lock);

	pressure = vmpressure_calc_pressure(scanned, reclaimed);
	vmpressure_notify(pressure);
}

/**
 * vmpressure() - Account memory pressure through scanned/reclaimed ratio
 * @gfp:	reclaimer's gfp mask
 * @memcg:	cgroup memory controller handle
 * @scanned:	number of pages scanned
 * @reclaimed:	number of pages reclaimed
 *
 * This function should be called from the vmscan reclaim path to account
 * "instantaneous" memory pressure (scanned/reclaimed ratio). The raw
 * pressure index is then further refined and averaged over time.
 *
 * This function does not return any value.
 */
void vmpressure(gfp_t gfp, struct mem_cgroup *memcg,
		unsigned long scanned, unsigned long reclaimed)
{
	if (!memcg)
		vmpressure_global(gfp, scanned, reclaimed);

	if (IS_ENABLED(CONFIG_MEMCG))
		vmpressure_memcg(gfp, memcg, scanned, reclaimed);
=======
	if (tree) {
		spin_lock(&vmpr->sr_lock);
		scanned = vmpr->tree_scanned += scanned;
		vmpr->tree_reclaimed += reclaimed;
		spin_unlock(&vmpr->sr_lock);

		if (scanned < vmpressure_win)
			return;
		schedule_work(&vmpr->work);
	} else {
		enum vmpressure_levels level;

		/* For now, no users for root-level efficiency */
		if (!memcg || mem_cgroup_is_root(memcg))
			return;

		spin_lock(&vmpr->sr_lock);
		scanned = vmpr->scanned += scanned;
		reclaimed = vmpr->reclaimed += reclaimed;
		if (scanned < vmpressure_win) {
			spin_unlock(&vmpr->sr_lock);
			return;
		}
		vmpr->scanned = vmpr->reclaimed = 0;
		spin_unlock(&vmpr->sr_lock);

		level = vmpressure_calc_level(scanned, reclaimed);

		if (level > VMPRESSURE_LOW) {
			/*
			 * Let the socket buffer allocator know that
			 * we are having trouble reclaiming LRU pages.
			 *
			 * For hysteresis keep the pressure state
			 * asserted for a second in which subsequent
			 * pressure events can occur.
			 */
			WRITE_ONCE(memcg->socket_pressure, jiffies + HZ);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * vmpressure_prio() - Account memory pressure through reclaimer priority level
 * @gfp:	reclaimer's gfp mask
 * @memcg:	cgroup memory controller handle
 * @prio:	reclaimer's priority
 *
 * This function should be called from the reclaim path every time when
 * the vmscan's reclaiming priority (scanning depth) changes.
 *
 * This function does not return any value.
 */
void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg, int prio)
{
	/*
	 * We only use prio for accounting critical level. For more info
	 * see comment for vmpressure_level_critical_prio variable above.
	 */
	if (prio > vmpressure_level_critical_prio)
		return;

	/*
	 * OK, the prio is below the threshold, updating vmpressure
	 * information before shrinker dives into long shrinking of long
	 * range vmscan. Passing scanned = vmpressure_win, reclaimed = 0
	 * to the vmpressure() basically means that we signal 'critical'
	 * level.
	 */
<<<<<<< HEAD
	vmpressure(gfp, memcg, vmpressure_win, 0);
}

/**
 * vmpressure_register_event() - Bind vmpressure notifications to an eventfd
 * @cg:		cgroup that is interested in vmpressure notifications
 * @cft:	cgroup control files handle
 * @eventfd:	eventfd context to link notifications with
 * @args:	event arguments (used to set up a pressure level threshold)
 *
 * This function associates eventfd context with the vmpressure
 * infrastructure, so that the notifications will be delivered to the
 * @eventfd. The @args parameter is a string that denotes pressure level
 * threshold (one of vmpressure_str_levels, i.e. "low", "medium", or
 * "critical").
 *
 * This function should not be used directly, just pass it to (struct
 * cftype).register_event, and then cgroup core will handle everything by
 * itself.
 */
int vmpressure_register_event(struct cgroup *cg, struct cftype *cft,
			      struct eventfd_ctx *eventfd, const char *args)
{
	struct vmpressure *vmpr = cg_to_vmpressure(cg);
	struct vmpressure_event *ev;
	int level;

	BUG_ON(!vmpr);

	for (level = 0; level < VMPRESSURE_NUM_LEVELS; level++) {
		if (!strcmp(vmpressure_str_levels[level], args))
			break;
	}

	if (level >= VMPRESSURE_NUM_LEVELS)
		return -EINVAL;

	ev = kzalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev)
		return -ENOMEM;

	ev->efd = eventfd;
	ev->level = level;
=======
	vmpressure(gfp, memcg, true, vmpressure_win, 0);
}

#define MAX_VMPRESSURE_ARGS_LEN	(strlen("critical") + strlen("hierarchy") + 2)

/**
 * vmpressure_register_event() - Bind vmpressure notifications to an eventfd
 * @memcg:	memcg that is interested in vmpressure notifications
 * @eventfd:	eventfd context to link notifications with
 * @args:	event arguments (pressure level threshold, optional mode)
 *
 * This function associates eventfd context with the vmpressure
 * infrastructure, so that the notifications will be delivered to the
 * @eventfd. The @args parameter is a comma-delimited string that denotes a
 * pressure level threshold (one of vmpressure_str_levels, i.e. "low", "medium",
 * or "critical") and an optional mode (one of vmpressure_str_modes, i.e.
 * "hierarchy" or "local").
 *
 * To be used as memcg event method.
 *
 * Return: 0 on success, -ENOMEM on memory failure or -EINVAL if @args could
 * not be parsed.
 */
int vmpressure_register_event(struct mem_cgroup *memcg,
			      struct eventfd_ctx *eventfd, const char *args)
{
	struct vmpressure *vmpr = memcg_to_vmpressure(memcg);
	struct vmpressure_event *ev;
	enum vmpressure_modes mode = VMPRESSURE_NO_PASSTHROUGH;
	enum vmpressure_levels level;
	char *spec, *spec_orig;
	char *token;
	int ret = 0;

	spec_orig = spec = kstrndup(args, MAX_VMPRESSURE_ARGS_LEN, GFP_KERNEL);
	if (!spec)
		return -ENOMEM;

	/* Find required level */
	token = strsep(&spec, ",");
	ret = match_string(vmpressure_str_levels, VMPRESSURE_NUM_LEVELS, token);
	if (ret < 0)
		goto out;
	level = ret;

	/* Find optional mode */
	token = strsep(&spec, ",");
	if (token) {
		ret = match_string(vmpressure_str_modes, VMPRESSURE_NUM_MODES, token);
		if (ret < 0)
			goto out;
		mode = ret;
	}

	ev = kzalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev) {
		ret = -ENOMEM;
		goto out;
	}

	ev->efd = eventfd;
	ev->level = level;
	ev->mode = mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&vmpr->events_lock);
	list_add(&ev->node, &vmpr->events);
	mutex_unlock(&vmpr->events_lock);
<<<<<<< HEAD

	return 0;
=======
	ret = 0;
out:
	kfree(spec_orig);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * vmpressure_unregister_event() - Unbind eventfd from vmpressure
<<<<<<< HEAD
 * @cg:		cgroup handle
 * @cft:	cgroup control files handle
=======
 * @memcg:	memcg handle
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @eventfd:	eventfd context that was used to link vmpressure with the @cg
 *
 * This function does internal manipulations to detach the @eventfd from
 * the vmpressure notifications, and then frees internal resources
 * associated with the @eventfd (but the @eventfd itself is not freed).
 *
<<<<<<< HEAD
 * This function should not be used directly, just pass it to (struct
 * cftype).unregister_event, and then cgroup core will handle everything
 * by itself.
 */
void vmpressure_unregister_event(struct cgroup *cg, struct cftype *cft,
				 struct eventfd_ctx *eventfd)
{
	struct vmpressure *vmpr = cg_to_vmpressure(cg);
	struct vmpressure_event *ev;

	if (!vmpr)
		BUG();

=======
 * To be used as memcg event method.
 */
void vmpressure_unregister_event(struct mem_cgroup *memcg,
				 struct eventfd_ctx *eventfd)
{
	struct vmpressure *vmpr = memcg_to_vmpressure(memcg);
	struct vmpressure_event *ev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&vmpr->events_lock);
	list_for_each_entry(ev, &vmpr->events, node) {
		if (ev->efd != eventfd)
			continue;
		list_del(&ev->node);
		kfree(ev);
		break;
	}
	mutex_unlock(&vmpr->events_lock);
}

/**
 * vmpressure_init() - Initialize vmpressure control structure
 * @vmpr:	Structure to be initialized
 *
 * This function should be called on every allocated vmpressure structure
 * before any usage.
 */
void vmpressure_init(struct vmpressure *vmpr)
{
<<<<<<< HEAD
	mutex_init(&vmpr->sr_lock);
=======
	spin_lock_init(&vmpr->sr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_init(&vmpr->events_lock);
	INIT_LIST_HEAD(&vmpr->events);
	INIT_WORK(&vmpr->work, vmpressure_work_fn);
}

<<<<<<< HEAD
int vmpressure_global_init(void)
{
	vmpressure_init(&global_vmpressure);
	return 0;
}
late_initcall(vmpressure_global_init);
=======
/**
 * vmpressure_cleanup() - shuts down vmpressure control structure
 * @vmpr:	Structure to be cleaned up
 *
 * This function should be called before the structure in which it is
 * embedded is cleaned up.
 */
void vmpressure_cleanup(struct vmpressure *vmpr)
{
	/*
	 * Make sure there is no pending work before eventfd infrastructure
	 * goes away.
	 */
	flush_work(&vmpr->work);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
