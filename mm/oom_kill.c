<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/mm/oom_kill.c
 * 
 *  Copyright (C)  1998,2000  Rik van Riel
 *	Thanks go out to Claus Fischer for some serious inspiration and
 *	for goading me into coding this file...
 *  Copyright (C)  2010  Google, Inc.
 *	Rewritten by David Rientjes
 *
 *  The routines in this file are used to kill a process when
 *  we're seriously out of memory. This gets called from __alloc_pages()
 *  in mm/page_alloc.c when we really run out of memory.
 *
 *  Since we won't call these routines often (on a well-configured
 *  machine) this file will double as a 'coding guide' and a signpost
 *  for newbie kernel hackers. It features several pointers to major
 *  kernel subsystems and hints as to where to find out what things do.
 */

#include <linux/oom.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/gfp.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/swap.h>
=======
#include <linux/sched/mm.h>
#include <linux/sched/coredump.h>
#include <linux/sched/task.h>
#include <linux/sched/debug.h>
#include <linux/swap.h>
#include <linux/syscalls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/timex.h>
#include <linux/jiffies.h>
#include <linux/cpuset.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include <linux/memcontrol.h>
#include <linux/mempolicy.h>
#include <linux/security.h>
#include <linux/ptrace.h>
#include <linux/freezer.h>
#include <linux/ftrace.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
=======
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/mmu_notifier.h>
#include <linux/cred.h>

#include <asm/tlb.h>
#include "internal.h"
#include "slab.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CREATE_TRACE_POINTS
#include <trace/events/oom.h>

<<<<<<< HEAD
int sysctl_panic_on_oom;
int sysctl_oom_kill_allocating_task;
int sysctl_oom_dump_tasks = 1;
static DEFINE_SPINLOCK(zone_scan_lock);

/*
 * compare_swap_oom_score_adj() - compare and swap current's oom_score_adj
 * @old_val: old oom_score_adj for compare
 * @new_val: new oom_score_adj for swap
 *
 * Sets the oom_score_adj value for current to @new_val iff its present value is
 * @old_val.  Usually used to reinstate a previous value to prevent racing with
 * userspacing tuning the value in the interim.
 */
void compare_swap_oom_score_adj(int old_val, int new_val)
{
	struct sighand_struct *sighand = current->sighand;

	spin_lock_irq(&sighand->siglock);
	if (current->signal->oom_score_adj == old_val)
		current->signal->oom_score_adj = new_val;
	trace_oom_score_adj_update(current);
	spin_unlock_irq(&sighand->siglock);
}

/**
 * test_set_oom_score_adj() - set current's oom_score_adj and return old value
 * @new_val: new oom_score_adj value
 *
 * Sets the oom_score_adj value for current to @new_val with proper
 * synchronization and returns the old value.  Usually used to temporarily
 * set a value, save the old value in the caller, and then reinstate it later.
 */
int test_set_oom_score_adj(int new_val)
{
	struct sighand_struct *sighand = current->sighand;
	int old_val;

	spin_lock_irq(&sighand->siglock);
	old_val = current->signal->oom_score_adj;
	current->signal->oom_score_adj = new_val;
	trace_oom_score_adj_update(current);
	spin_unlock_irq(&sighand->siglock);

	return old_val;
=======
static int sysctl_panic_on_oom;
static int sysctl_oom_kill_allocating_task;
static int sysctl_oom_dump_tasks = 1;

/*
 * Serializes oom killer invocations (out_of_memory()) from all contexts to
 * prevent from over eager oom killing (e.g. when the oom killer is invoked
 * from different domains).
 *
 * oom_killer_disable() relies on this lock to stabilize oom_killer_disabled
 * and mark_oom_victim
 */
DEFINE_MUTEX(oom_lock);
/* Serializes oom_score_adj and oom_score_adj_min updates */
DEFINE_MUTEX(oom_adj_mutex);

static inline bool is_memcg_oom(struct oom_control *oc)
{
	return oc->memcg != NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_NUMA
/**
<<<<<<< HEAD
 * has_intersects_mems_allowed() - check task eligiblity for kill
 * @tsk: task struct of which task to consider
 * @mask: nodemask passed to page allocator for mempolicy ooms
=======
 * oom_cpuset_eligible() - check task eligibility for kill
 * @start: task struct of which task to consider
 * @oc: pointer to struct oom_control
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Task eligibility is determined by whether or not a candidate task, @tsk,
 * shares the same mempolicy nodes as current if it is bound by such a policy
 * and whether or not it has the same set of allowed cpuset nodes.
<<<<<<< HEAD
 */
static bool has_intersects_mems_allowed(struct task_struct *tsk,
					const nodemask_t *mask)
{
	struct task_struct *start = tsk;

	do {
=======
 *
 * This function is assuming oom-killer context and 'current' has triggered
 * the oom-killer.
 */
static bool oom_cpuset_eligible(struct task_struct *start,
				struct oom_control *oc)
{
	struct task_struct *tsk;
	bool ret = false;
	const nodemask_t *mask = oc->nodemask;

	rcu_read_lock();
	for_each_thread(start, tsk) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (mask) {
			/*
			 * If this is a mempolicy constrained oom, tsk's
			 * cpuset is irrelevant.  Only return true if its
			 * mempolicy intersects current, otherwise it may be
			 * needlessly killed.
			 */
<<<<<<< HEAD
			if (mempolicy_nodemask_intersects(tsk, mask))
				return true;
=======
			ret = mempolicy_in_oom_domain(tsk, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			/*
			 * This is not a mempolicy constrained oom, so only
			 * check the mems of tsk's cpuset.
			 */
<<<<<<< HEAD
			if (cpuset_mems_allowed_intersects(current, tsk))
				return true;
		}
	} while_each_thread(start, tsk);

	return false;
}
#else
static bool has_intersects_mems_allowed(struct task_struct *tsk,
					const nodemask_t *mask)
=======
			ret = cpuset_mems_allowed_intersects(current, tsk);
		}
		if (ret)
			break;
	}
	rcu_read_unlock();

	return ret;
}
#else
static bool oom_cpuset_eligible(struct task_struct *tsk, struct oom_control *oc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return true;
}
#endif /* CONFIG_NUMA */

/*
 * The process p may have detached its own ->mm while exiting or through
<<<<<<< HEAD
 * use_mm(), but one or more of its subthreads may still have a valid
=======
 * kthread_use_mm(), but one or more of its subthreads may still have a valid
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * pointer.  Return p, or any of its subthreads with a valid ->mm, with
 * task_lock() held.
 */
struct task_struct *find_lock_task_mm(struct task_struct *p)
{
<<<<<<< HEAD
	struct task_struct *t = p;

	do {
		task_lock(t);
		if (likely(t->mm))
			return t;
		task_unlock(t);
	} while_each_thread(p, t);

	return NULL;
}

/* return true if the task is not adequate as candidate victim task. */
static bool oom_unkillable_task(struct task_struct *p,
		const struct mem_cgroup *memcg, const nodemask_t *nodemask)
=======
	struct task_struct *t;

	rcu_read_lock();

	for_each_thread(p, t) {
		task_lock(t);
		if (likely(t->mm))
			goto found;
		task_unlock(t);
	}
	t = NULL;
found:
	rcu_read_unlock();

	return t;
}

/*
 * order == -1 means the oom kill is required by sysrq, otherwise only
 * for display purposes.
 */
static inline bool is_sysrq_oom(struct oom_control *oc)
{
	return oc->order == -1;
}

/* return true if the task is not adequate as candidate victim task. */
static bool oom_unkillable_task(struct task_struct *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (is_global_init(p))
		return true;
	if (p->flags & PF_KTHREAD)
		return true;
<<<<<<< HEAD

	/* When mem_cgroup_out_of_memory() and p is not member of the group */
	if (memcg && !task_in_mem_cgroup(p, memcg))
		return true;

	/* p may not have freeable memory in nodemask */
	if (!has_intersects_mems_allowed(p, nodemask))
		return true;

	return false;
}

=======
	return false;
}

/*
 * Check whether unreclaimable slab amount is greater than
 * all user memory(LRU pages).
 * dump_unreclaimable_slab() could help in the case that
 * oom due to too much unreclaimable slab used by kernel.
*/
static bool should_dump_unreclaim_slab(void)
{
	unsigned long nr_lru;

	nr_lru = global_node_page_state(NR_ACTIVE_ANON) +
		 global_node_page_state(NR_INACTIVE_ANON) +
		 global_node_page_state(NR_ACTIVE_FILE) +
		 global_node_page_state(NR_INACTIVE_FILE) +
		 global_node_page_state(NR_ISOLATED_ANON) +
		 global_node_page_state(NR_ISOLATED_FILE) +
		 global_node_page_state(NR_UNEVICTABLE);

	return (global_node_page_state_pages(NR_SLAB_UNRECLAIMABLE_B) > nr_lru);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * oom_badness - heuristic function to determine which candidate task to kill
 * @p: task struct of which task we should calculate
 * @totalpages: total present RAM allowed for page allocation
 *
 * The heuristic for determining which task to kill is made to be as simple and
 * predictable as possible.  The goal is to return the highest value for the
 * task consuming the most memory to avoid subsequent oom failures.
 */
<<<<<<< HEAD
unsigned int oom_badness(struct task_struct *p, struct mem_cgroup *memcg,
		      const nodemask_t *nodemask, unsigned long totalpages)
{
	long points;

	if (oom_unkillable_task(p, memcg, nodemask))
		return 0;

	p = find_lock_task_mm(p);
	if (!p)
		return 0;

	if (p->signal->oom_score_adj == OOM_SCORE_ADJ_MIN) {
		task_unlock(p);
		return 0;
	}

	/*
	 * The memory controller may have a limit of 0 bytes, so avoid a divide
	 * by zero, if necessary.
	 */
	if (!totalpages)
		totalpages = 1;
=======
long oom_badness(struct task_struct *p, unsigned long totalpages)
{
	long points;
	long adj;

	if (oom_unkillable_task(p))
		return LONG_MIN;

	p = find_lock_task_mm(p);
	if (!p)
		return LONG_MIN;

	/*
	 * Do not even consider tasks which are explicitly marked oom
	 * unkillable or have been already oom reaped or the are in
	 * the middle of vfork
	 */
	adj = (long)p->signal->oom_score_adj;
	if (adj == OOM_SCORE_ADJ_MIN ||
			test_bit(MMF_OOM_SKIP, &p->mm->flags) ||
			in_vfork(p)) {
		task_unlock(p);
		return LONG_MIN;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The baseline for the badness score is the proportion of RAM that each
	 * task's rss, pagetable and swap space use.
	 */
<<<<<<< HEAD
	points = get_mm_rss(p->mm) + p->mm->nr_ptes;
	points += get_mm_counter(p->mm, MM_SWAPENTS);

	points *= 1000;
	points /= totalpages;
	task_unlock(p);

	/*
	 * Root processes get 3% bonus, just like the __vm_enough_memory()
	 * implementation used by LSMs.
	 */
	if (has_capability_noaudit(p, CAP_SYS_ADMIN))
		points -= (points * 3) / 100;

	/*
	 * /proc/pid/oom_score_adj ranges from -1000 to +1000 such that it may
	 * either completely disable oom killing or always prefer a certain
	 * task.
	 */
	points += p->signal->oom_score_adj;

	/*
	 * Never return 0 for an eligible task that may be killed since it's
	 * possible that no single user task uses more than 0.1% of memory and
	 * no single admin tasks uses more than 3.0%.
	 */
	if (points <= 0)
		return 1;
	return (points < 1000) ? points : 1000;
}

/*
 * Determine the type of allocation constraint.
 */
#ifdef CONFIG_NUMA
static enum oom_constraint constrained_alloc(struct zonelist *zonelist,
				gfp_t gfp_mask, nodemask_t *nodemask,
				unsigned long *totalpages)
{
	struct zone *zone;
	struct zoneref *z;
	enum zone_type high_zoneidx = gfp_zone(gfp_mask);
	bool cpuset_limited = false;
	int nid;

	/* Default to all available memory */
	*totalpages = totalram_pages + total_swap_pages;

	if (!zonelist)
=======
	points = get_mm_rss(p->mm) + get_mm_counter(p->mm, MM_SWAPENTS) +
		mm_pgtables_bytes(p->mm) / PAGE_SIZE;
	task_unlock(p);

	/* Normalize to oom_score_adj units */
	adj *= totalpages / 1000;
	points += adj;

	return points;
}

static const char * const oom_constraint_text[] = {
	[CONSTRAINT_NONE] = "CONSTRAINT_NONE",
	[CONSTRAINT_CPUSET] = "CONSTRAINT_CPUSET",
	[CONSTRAINT_MEMORY_POLICY] = "CONSTRAINT_MEMORY_POLICY",
	[CONSTRAINT_MEMCG] = "CONSTRAINT_MEMCG",
};

/*
 * Determine the type of allocation constraint.
 */
static enum oom_constraint constrained_alloc(struct oom_control *oc)
{
	struct zone *zone;
	struct zoneref *z;
	enum zone_type highest_zoneidx = gfp_zone(oc->gfp_mask);
	bool cpuset_limited = false;
	int nid;

	if (is_memcg_oom(oc)) {
		oc->totalpages = mem_cgroup_get_max(oc->memcg) ?: 1;
		return CONSTRAINT_MEMCG;
	}

	/* Default to all available memory */
	oc->totalpages = totalram_pages() + total_swap_pages;

	if (!IS_ENABLED(CONFIG_NUMA))
		return CONSTRAINT_NONE;

	if (!oc->zonelist)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return CONSTRAINT_NONE;
	/*
	 * Reach here only when __GFP_NOFAIL is used. So, we should avoid
	 * to kill current.We have to random task kill in this case.
	 * Hopefully, CONSTRAINT_THISNODE...but no way to handle it, now.
	 */
<<<<<<< HEAD
	if (gfp_mask & __GFP_THISNODE)
=======
	if (oc->gfp_mask & __GFP_THISNODE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return CONSTRAINT_NONE;

	/*
	 * This is not a __GFP_THISNODE allocation, so a truncated nodemask in
	 * the page allocator means a mempolicy is in effect.  Cpuset policy
	 * is enforced in get_page_from_freelist().
	 */
<<<<<<< HEAD
	if (nodemask && !nodes_subset(node_states[N_HIGH_MEMORY], *nodemask)) {
		*totalpages = total_swap_pages;
		for_each_node_mask(nid, *nodemask)
			*totalpages += node_spanned_pages(nid);
=======
	if (oc->nodemask &&
	    !nodes_subset(node_states[N_MEMORY], *oc->nodemask)) {
		oc->totalpages = total_swap_pages;
		for_each_node_mask(nid, *oc->nodemask)
			oc->totalpages += node_present_pages(nid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return CONSTRAINT_MEMORY_POLICY;
	}

	/* Check this allocation failure is caused by cpuset's wall function */
<<<<<<< HEAD
	for_each_zone_zonelist_nodemask(zone, z, zonelist,
			high_zoneidx, nodemask)
		if (!cpuset_zone_allowed_softwall(zone, gfp_mask))
			cpuset_limited = true;

	if (cpuset_limited) {
		*totalpages = total_swap_pages;
		for_each_node_mask(nid, cpuset_current_mems_allowed)
			*totalpages += node_spanned_pages(nid);
=======
	for_each_zone_zonelist_nodemask(zone, z, oc->zonelist,
			highest_zoneidx, oc->nodemask)
		if (!cpuset_zone_allowed(zone, oc->gfp_mask))
			cpuset_limited = true;

	if (cpuset_limited) {
		oc->totalpages = total_swap_pages;
		for_each_node_mask(nid, cpuset_current_mems_allowed)
			oc->totalpages += node_present_pages(nid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return CONSTRAINT_CPUSET;
	}
	return CONSTRAINT_NONE;
}
<<<<<<< HEAD
#else
static enum oom_constraint constrained_alloc(struct zonelist *zonelist,
				gfp_t gfp_mask, nodemask_t *nodemask,
				unsigned long *totalpages)
{
	*totalpages = totalram_pages + total_swap_pages;
	return CONSTRAINT_NONE;
}
#endif

/*
 * Simple selection loop. We chose the process with the highest
 * number of 'points'. We expect the caller will lock the tasklist.
 *
 * (not docbooked, we don't want this one cluttering up the manual)
 */
static struct task_struct *select_bad_process(unsigned int *ppoints,
		unsigned long totalpages, struct mem_cgroup *memcg,
		const nodemask_t *nodemask, bool force_kill)
{
	struct task_struct *g, *p;
	struct task_struct *chosen = NULL;
	*ppoints = 0;

	do_each_thread(g, p) {
		unsigned int points;

		if (p->exit_state)
			continue;
		if (oom_unkillable_task(p, memcg, nodemask))
			continue;

		/*
		 * This task already has access to memory reserves and is
		 * being killed. Don't allow any other task access to the
		 * memory reserve.
		 *
		 * Note: this may have a chance of deadlock if it gets
		 * blocked waiting for another task which itself is waiting
		 * for memory. Is there a better alternative?
		 */
		if (test_tsk_thread_flag(p, TIF_MEMDIE)) {
			if (unlikely(frozen(p)))
				__thaw_task(p);
			if (!force_kill)
				return ERR_PTR(-1UL);
		}
		if (!p->mm)
			continue;

		if (p->flags & PF_EXITING) {
			/*
			 * If p is the current task and is in the process of
			 * releasing memory, we allow the "kill" to set
			 * TIF_MEMDIE, which will allow it to gain access to
			 * memory reserves.  Otherwise, it may stall forever.
			 *
			 * The loop isn't broken here, however, in case other
			 * threads are found to have already been oom killed.
			 */
			if (p == current) {
				chosen = p;
				*ppoints = 1000;
			} else if (!force_kill) {
				/*
				 * If this task is not being ptraced on exit,
				 * then wait for it to finish before killing
				 * some other task unnecessarily.
				 */
				if (!(p->group_leader->ptrace & PT_TRACE_EXIT))
					return ERR_PTR(-1UL);
			}
		}

		points = oom_badness(p, memcg, nodemask, totalpages);
		if (points > *ppoints) {
			chosen = p;
			*ppoints = points;
		}
	} while_each_thread(g, p);

	return chosen;
=======

static int oom_evaluate_task(struct task_struct *task, void *arg)
{
	struct oom_control *oc = arg;
	long points;

	if (oom_unkillable_task(task))
		goto next;

	/* p may not have freeable memory in nodemask */
	if (!is_memcg_oom(oc) && !oom_cpuset_eligible(task, oc))
		goto next;

	/*
	 * This task already has access to memory reserves and is being killed.
	 * Don't allow any other task to have access to the reserves unless
	 * the task has MMF_OOM_SKIP because chances that it would release
	 * any memory is quite low.
	 */
	if (!is_sysrq_oom(oc) && tsk_is_oom_victim(task)) {
		if (test_bit(MMF_OOM_SKIP, &task->signal->oom_mm->flags))
			goto next;
		goto abort;
	}

	/*
	 * If task is allocating a lot of memory and has been marked to be
	 * killed first if it triggers an oom, then select it.
	 */
	if (oom_task_origin(task)) {
		points = LONG_MAX;
		goto select;
	}

	points = oom_badness(task, oc->totalpages);
	if (points == LONG_MIN || points < oc->chosen_points)
		goto next;

select:
	if (oc->chosen)
		put_task_struct(oc->chosen);
	get_task_struct(task);
	oc->chosen = task;
	oc->chosen_points = points;
next:
	return 0;
abort:
	if (oc->chosen)
		put_task_struct(oc->chosen);
	oc->chosen = (void *)-1UL;
	return 1;
}

/*
 * Simple selection loop. We choose the process with the highest number of
 * 'points'. In case scan was aborted, oc->chosen is set to -1.
 */
static void select_bad_process(struct oom_control *oc)
{
	oc->chosen_points = LONG_MIN;

	if (is_memcg_oom(oc))
		mem_cgroup_scan_tasks(oc->memcg, oom_evaluate_task, oc);
	else {
		struct task_struct *p;

		rcu_read_lock();
		for_each_process(p)
			if (oom_evaluate_task(p, oc))
				break;
		rcu_read_unlock();
	}
}

static int dump_task(struct task_struct *p, void *arg)
{
	struct oom_control *oc = arg;
	struct task_struct *task;

	if (oom_unkillable_task(p))
		return 0;

	/* p may not have freeable memory in nodemask */
	if (!is_memcg_oom(oc) && !oom_cpuset_eligible(p, oc))
		return 0;

	task = find_lock_task_mm(p);
	if (!task) {
		/*
		 * All of p's threads have already detached their mm's. There's
		 * no need to report them; they can't be oom killed anyway.
		 */
		return 0;
	}

	pr_info("[%7d] %5d %5d %8lu %8lu %8lu %8lu %9lu %8ld %8lu         %5hd %s\n",
		task->pid, from_kuid(&init_user_ns, task_uid(task)),
		task->tgid, task->mm->total_vm, get_mm_rss(task->mm),
		get_mm_counter(task->mm, MM_ANONPAGES), get_mm_counter(task->mm, MM_FILEPAGES),
		get_mm_counter(task->mm, MM_SHMEMPAGES), mm_pgtables_bytes(task->mm),
		get_mm_counter(task->mm, MM_SWAPENTS),
		task->signal->oom_score_adj, task->comm);
	task_unlock(task);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * dump_tasks - dump current memory state of all system tasks
<<<<<<< HEAD
 * @mem: current's memory controller, if constrained
 * @nodemask: nodemask passed to page allocator for mempolicy ooms
=======
 * @oc: pointer to struct oom_control
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Dumps the current memory state of all eligible tasks.  Tasks not in the same
 * memcg, not in the same cpuset, or bound to a disjoint set of mempolicy nodes
 * are not shown.
<<<<<<< HEAD
 * State information includes task's pid, uid, tgid, vm size, rss, cpu, oom_adj
 * value, oom_score_adj value, and name.
 *
 * Call with tasklist_lock read-locked.
 */
void dump_tasks(const struct mem_cgroup *memcg, const nodemask_t *nodemask)
{
	struct task_struct *p;
	struct task_struct *task;

	pr_info("[ pid ]   uid  tgid total_vm      rss cpu oom_adj oom_score_adj name\n");
	for_each_process(p) {
		if (oom_unkillable_task(p, memcg, nodemask))
			continue;

		task = find_lock_task_mm(p);
		if (!task) {
			/*
			 * This is a kthread or all of p's threads have already
			 * detached their mm's.  There's no need to report
			 * them; they can't be oom killed anyway.
			 */
			continue;
		}

		pr_info("[%5d] %5d %5d %8lu %8lu %3u     %3d         %5d %s\n",
			task->pid, task_uid(task), task->tgid,
			task->mm->total_vm, get_mm_rss(task->mm),
			task_cpu(task), task->signal->oom_adj,
			task->signal->oom_score_adj, task->comm);
		task_unlock(task);
	}
}

static void dump_header(struct task_struct *p, gfp_t gfp_mask, int order,
			struct mem_cgroup *memcg, const nodemask_t *nodemask)
{
	task_lock(current);
	pr_warning("%s invoked oom-killer: gfp_mask=0x%x, order=%d, "
		"oom_adj=%d, oom_score_adj=%d\n",
		current->comm, gfp_mask, order, current->signal->oom_adj,
		current->signal->oom_score_adj);
	cpuset_print_task_mems_allowed(current);
	task_unlock(current);
	dump_stack();
	mem_cgroup_print_oom_info(memcg, p);
	show_mem(SHOW_MEM_FILTER_NODES);
	if (sysctl_oom_dump_tasks)
		dump_tasks(memcg, nodemask);
}

/*
 * Number of OOM killer invocations (including memcg OOM killer).
 * Primarily used by PM freezer to check for potential races with
 * OOM killed frozen task.
 */
static atomic_t oom_kills = ATOMIC_INIT(0);

int oom_kills_count(void)
{
	return atomic_read(&oom_kills);
}

void note_oom_kill(void)
{
	atomic_inc(&oom_kills);
}

#define K(x) ((x) << (PAGE_SHIFT-10))
static void oom_kill_process(struct task_struct *p, gfp_t gfp_mask, int order,
			     unsigned int points, unsigned long totalpages,
			     struct mem_cgroup *memcg, nodemask_t *nodemask,
			     const char *message)
{
	struct task_struct *victim = p;
	struct task_struct *child;
	struct task_struct *t = p;
	struct mm_struct *mm;
	unsigned int victim_points = 0;
	static DEFINE_RATELIMIT_STATE(oom_rs, DEFAULT_RATELIMIT_INTERVAL,
					      DEFAULT_RATELIMIT_BURST);

	/*
	 * If the task is already exiting, don't alarm the sysadmin or kill
	 * its children or threads, just set TIF_MEMDIE so it can die quickly
	 */
	if (p->flags & PF_EXITING) {
		set_tsk_thread_flag(p, TIF_MEMDIE);
		return;
	}

	if (__ratelimit(&oom_rs))
		dump_header(p, gfp_mask, order, memcg, nodemask);

	task_lock(p);
	pr_err("%s: Kill process %d (%s) score %d or sacrifice child\n",
		message, task_pid_nr(p), p->comm, points);
	task_unlock(p);

	/*
	 * If any of p's children has a different mm and is eligible for kill,
	 * the one with the highest oom_badness() score is sacrificed for its
	 * parent.  This attempts to lose the minimal amount of work done while
	 * still freeing memory.
	 */
	do {
		list_for_each_entry(child, &t->children, sibling) {
			unsigned int child_points;

			if (child->mm == p->mm)
				continue;
			/*
			 * oom_badness() returns 0 if the thread is unkillable
			 */
			child_points = oom_badness(child, memcg, nodemask,
								totalpages);
			if (child_points > victim_points) {
				victim = child;
				victim_points = child_points;
			}
		}
	} while_each_thread(p, t);

	victim = find_lock_task_mm(victim);
	if (!victim)
		return;

	/* mm cannot safely be dereferenced after task_unlock(victim) */
	mm = victim->mm;
	pr_err("Killed process %d (%s) total-vm:%lukB, anon-rss:%lukB, file-rss:%lukB\n",
		task_pid_nr(victim), victim->comm, K(victim->mm->total_vm),
		K(get_mm_counter(victim->mm, MM_ANONPAGES)),
		K(get_mm_counter(victim->mm, MM_FILEPAGES)));
=======
 * State information includes task's pid, uid, tgid, vm size, rss,
 * pgtables_bytes, swapents, oom_score_adj value, and name.
 */
static void dump_tasks(struct oom_control *oc)
{
	pr_info("Tasks state (memory values in pages):\n");
	pr_info("[  pid  ]   uid  tgid total_vm      rss rss_anon rss_file rss_shmem pgtables_bytes swapents oom_score_adj name\n");

	if (is_memcg_oom(oc))
		mem_cgroup_scan_tasks(oc->memcg, dump_task, oc);
	else {
		struct task_struct *p;

		rcu_read_lock();
		for_each_process(p)
			dump_task(p, oc);
		rcu_read_unlock();
	}
}

static void dump_oom_victim(struct oom_control *oc, struct task_struct *victim)
{
	/* one line summary of the oom killer context. */
	pr_info("oom-kill:constraint=%s,nodemask=%*pbl",
			oom_constraint_text[oc->constraint],
			nodemask_pr_args(oc->nodemask));
	cpuset_print_current_mems_allowed();
	mem_cgroup_print_oom_context(oc->memcg, victim);
	pr_cont(",task=%s,pid=%d,uid=%d\n", victim->comm, victim->pid,
		from_kuid(&init_user_ns, task_uid(victim)));
}

static void dump_header(struct oom_control *oc)
{
	pr_warn("%s invoked oom-killer: gfp_mask=%#x(%pGg), order=%d, oom_score_adj=%hd\n",
		current->comm, oc->gfp_mask, &oc->gfp_mask, oc->order,
			current->signal->oom_score_adj);
	if (!IS_ENABLED(CONFIG_COMPACTION) && oc->order)
		pr_warn("COMPACTION is disabled!!!\n");

	dump_stack();
	if (is_memcg_oom(oc))
		mem_cgroup_print_oom_meminfo(oc->memcg);
	else {
		__show_mem(SHOW_MEM_FILTER_NODES, oc->nodemask, gfp_zone(oc->gfp_mask));
		if (should_dump_unreclaim_slab())
			dump_unreclaimable_slab();
	}
	if (sysctl_oom_dump_tasks)
		dump_tasks(oc);
}

/*
 * Number of OOM victims in flight
 */
static atomic_t oom_victims = ATOMIC_INIT(0);
static DECLARE_WAIT_QUEUE_HEAD(oom_victims_wait);

static bool oom_killer_disabled __read_mostly;

/*
 * task->mm can be NULL if the task is the exited group leader.  So to
 * determine whether the task is using a particular mm, we examine all the
 * task's threads: if one of those is using this mm then this task was also
 * using it.
 */
bool process_shares_mm(struct task_struct *p, struct mm_struct *mm)
{
	struct task_struct *t;

	for_each_thread(p, t) {
		struct mm_struct *t_mm = READ_ONCE(t->mm);
		if (t_mm)
			return t_mm == mm;
	}
	return false;
}

#ifdef CONFIG_MMU
/*
 * OOM Reaper kernel thread which tries to reap the memory used by the OOM
 * victim (if that is possible) to help the OOM killer to move on.
 */
static struct task_struct *oom_reaper_th;
static DECLARE_WAIT_QUEUE_HEAD(oom_reaper_wait);
static struct task_struct *oom_reaper_list;
static DEFINE_SPINLOCK(oom_reaper_lock);

static bool __oom_reap_task_mm(struct mm_struct *mm)
{
	struct vm_area_struct *vma;
	bool ret = true;
	VMA_ITERATOR(vmi, mm, 0);

	/*
	 * Tell all users of get_user/copy_from_user etc... that the content
	 * is no longer stable. No barriers really needed because unmapping
	 * should imply barriers already and the reader would hit a page fault
	 * if it stumbled over a reaped memory.
	 */
	set_bit(MMF_UNSTABLE, &mm->flags);

	for_each_vma(vmi, vma) {
		if (vma->vm_flags & (VM_HUGETLB|VM_PFNMAP))
			continue;

		/*
		 * Only anonymous pages have a good chance to be dropped
		 * without additional steps which we cannot afford as we
		 * are OOM already.
		 *
		 * We do not even care about fs backed pages because all
		 * which are reclaimable have already been reclaimed and
		 * we do not want to block exit_mmap by keeping mm ref
		 * count elevated without a good reason.
		 */
		if (vma_is_anonymous(vma) || !(vma->vm_flags & VM_SHARED)) {
			struct mmu_notifier_range range;
			struct mmu_gather tlb;

			mmu_notifier_range_init(&range, MMU_NOTIFY_UNMAP, 0,
						mm, vma->vm_start,
						vma->vm_end);
			tlb_gather_mmu(&tlb, mm);
			if (mmu_notifier_invalidate_range_start_nonblock(&range)) {
				tlb_finish_mmu(&tlb);
				ret = false;
				continue;
			}
			unmap_page_range(&tlb, vma, range.start, range.end, NULL);
			mmu_notifier_invalidate_range_end(&range);
			tlb_finish_mmu(&tlb);
		}
	}

	return ret;
}

/*
 * Reaps the address space of the give task.
 *
 * Returns true on success and false if none or part of the address space
 * has been reclaimed and the caller should retry later.
 */
static bool oom_reap_task_mm(struct task_struct *tsk, struct mm_struct *mm)
{
	bool ret = true;

	if (!mmap_read_trylock(mm)) {
		trace_skip_task_reaping(tsk->pid);
		return false;
	}

	/*
	 * MMF_OOM_SKIP is set by exit_mmap when the OOM reaper can't
	 * work on the mm anymore. The check for MMF_OOM_SKIP must run
	 * under mmap_lock for reading because it serializes against the
	 * mmap_write_lock();mmap_write_unlock() cycle in exit_mmap().
	 */
	if (test_bit(MMF_OOM_SKIP, &mm->flags)) {
		trace_skip_task_reaping(tsk->pid);
		goto out_unlock;
	}

	trace_start_task_reaping(tsk->pid);

	/* failed to reap part of the address space. Try again later */
	ret = __oom_reap_task_mm(mm);
	if (!ret)
		goto out_finish;

	pr_info("oom_reaper: reaped process %d (%s), now anon-rss:%lukB, file-rss:%lukB, shmem-rss:%lukB\n",
			task_pid_nr(tsk), tsk->comm,
			K(get_mm_counter(mm, MM_ANONPAGES)),
			K(get_mm_counter(mm, MM_FILEPAGES)),
			K(get_mm_counter(mm, MM_SHMEMPAGES)));
out_finish:
	trace_finish_task_reaping(tsk->pid);
out_unlock:
	mmap_read_unlock(mm);

	return ret;
}

#define MAX_OOM_REAP_RETRIES 10
static void oom_reap_task(struct task_struct *tsk)
{
	int attempts = 0;
	struct mm_struct *mm = tsk->signal->oom_mm;

	/* Retry the mmap_read_trylock(mm) a few times */
	while (attempts++ < MAX_OOM_REAP_RETRIES && !oom_reap_task_mm(tsk, mm))
		schedule_timeout_idle(HZ/10);

	if (attempts <= MAX_OOM_REAP_RETRIES ||
	    test_bit(MMF_OOM_SKIP, &mm->flags))
		goto done;

	pr_info("oom_reaper: unable to reap pid:%d (%s)\n",
		task_pid_nr(tsk), tsk->comm);
	sched_show_task(tsk);
	debug_show_all_locks();

done:
	tsk->oom_reaper_list = NULL;

	/*
	 * Hide this mm from OOM killer because it has been either reaped or
	 * somebody can't call mmap_write_unlock(mm).
	 */
	set_bit(MMF_OOM_SKIP, &mm->flags);

	/* Drop a reference taken by queue_oom_reaper */
	put_task_struct(tsk);
}

static int oom_reaper(void *unused)
{
	set_freezable();

	while (true) {
		struct task_struct *tsk = NULL;

		wait_event_freezable(oom_reaper_wait, oom_reaper_list != NULL);
		spin_lock_irq(&oom_reaper_lock);
		if (oom_reaper_list != NULL) {
			tsk = oom_reaper_list;
			oom_reaper_list = tsk->oom_reaper_list;
		}
		spin_unlock_irq(&oom_reaper_lock);

		if (tsk)
			oom_reap_task(tsk);
	}

	return 0;
}

static void wake_oom_reaper(struct timer_list *timer)
{
	struct task_struct *tsk = container_of(timer, struct task_struct,
			oom_reaper_timer);
	struct mm_struct *mm = tsk->signal->oom_mm;
	unsigned long flags;

	/* The victim managed to terminate on its own - see exit_mmap */
	if (test_bit(MMF_OOM_SKIP, &mm->flags)) {
		put_task_struct(tsk);
		return;
	}

	spin_lock_irqsave(&oom_reaper_lock, flags);
	tsk->oom_reaper_list = oom_reaper_list;
	oom_reaper_list = tsk;
	spin_unlock_irqrestore(&oom_reaper_lock, flags);
	trace_wake_reaper(tsk->pid);
	wake_up(&oom_reaper_wait);
}

/*
 * Give the OOM victim time to exit naturally before invoking the oom_reaping.
 * The timers timeout is arbitrary... the longer it is, the longer the worst
 * case scenario for the OOM can take. If it is too small, the oom_reaper can
 * get in the way and release resources needed by the process exit path.
 * e.g. The futex robust list can sit in Anon|Private memory that gets reaped
 * before the exit path is able to wake the futex waiters.
 */
#define OOM_REAPER_DELAY (2*HZ)
static void queue_oom_reaper(struct task_struct *tsk)
{
	/* mm is already queued? */
	if (test_and_set_bit(MMF_OOM_REAP_QUEUED, &tsk->signal->oom_mm->flags))
		return;

	get_task_struct(tsk);
	timer_setup(&tsk->oom_reaper_timer, wake_oom_reaper, 0);
	tsk->oom_reaper_timer.expires = jiffies + OOM_REAPER_DELAY;
	add_timer(&tsk->oom_reaper_timer);
}

#ifdef CONFIG_SYSCTL
static struct ctl_table vm_oom_kill_table[] = {
	{
		.procname	= "panic_on_oom",
		.data		= &sysctl_panic_on_oom,
		.maxlen		= sizeof(sysctl_panic_on_oom),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_TWO,
	},
	{
		.procname	= "oom_kill_allocating_task",
		.data		= &sysctl_oom_kill_allocating_task,
		.maxlen		= sizeof(sysctl_oom_kill_allocating_task),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "oom_dump_tasks",
		.data		= &sysctl_oom_dump_tasks,
		.maxlen		= sizeof(sysctl_oom_dump_tasks),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{}
};
#endif

static int __init oom_init(void)
{
	oom_reaper_th = kthread_run(oom_reaper, NULL, "oom_reaper");
#ifdef CONFIG_SYSCTL
	register_sysctl_init("vm", vm_oom_kill_table);
#endif
	return 0;
}
subsys_initcall(oom_init)
#else
static inline void queue_oom_reaper(struct task_struct *tsk)
{
}
#endif /* CONFIG_MMU */

/**
 * mark_oom_victim - mark the given task as OOM victim
 * @tsk: task to mark
 *
 * Has to be called with oom_lock held and never after
 * oom has been disabled already.
 *
 * tsk->mm has to be non NULL and caller has to guarantee it is stable (either
 * under task_lock or operate on the current).
 */
static void mark_oom_victim(struct task_struct *tsk)
{
	const struct cred *cred;
	struct mm_struct *mm = tsk->mm;

	WARN_ON(oom_killer_disabled);
	/* OOM killer might race with memcg OOM */
	if (test_and_set_tsk_thread_flag(tsk, TIF_MEMDIE))
		return;

	/* oom_mm is bound to the signal struct life time. */
	if (!cmpxchg(&tsk->signal->oom_mm, NULL, mm))
		mmgrab(tsk->signal->oom_mm);

	/*
	 * Make sure that the task is woken up from uninterruptible sleep
	 * if it is frozen because OOM killer wouldn't be able to free
	 * any memory and livelock. freezing_slow_path will tell the freezer
	 * that TIF_MEMDIE tasks should be ignored.
	 */
	__thaw_task(tsk);
	atomic_inc(&oom_victims);
	cred = get_task_cred(tsk);
	trace_mark_victim(tsk, cred->uid.val);
	put_cred(cred);
}

/**
 * exit_oom_victim - note the exit of an OOM victim
 */
void exit_oom_victim(void)
{
	clear_thread_flag(TIF_MEMDIE);

	if (!atomic_dec_return(&oom_victims))
		wake_up_all(&oom_victims_wait);
}

/**
 * oom_killer_enable - enable OOM killer
 */
void oom_killer_enable(void)
{
	oom_killer_disabled = false;
	pr_info("OOM killer enabled.\n");
}

/**
 * oom_killer_disable - disable OOM killer
 * @timeout: maximum timeout to wait for oom victims in jiffies
 *
 * Forces all page allocations to fail rather than trigger OOM killer.
 * Will block and wait until all OOM victims are killed or the given
 * timeout expires.
 *
 * The function cannot be called when there are runnable user tasks because
 * the userspace would see unexpected allocation failures as a result. Any
 * new usage of this function should be consulted with MM people.
 *
 * Returns true if successful and false if the OOM killer cannot be
 * disabled.
 */
bool oom_killer_disable(signed long timeout)
{
	signed long ret;

	/*
	 * Make sure to not race with an ongoing OOM killer. Check that the
	 * current is not killed (possibly due to sharing the victim's memory).
	 */
	if (mutex_lock_killable(&oom_lock))
		return false;
	oom_killer_disabled = true;
	mutex_unlock(&oom_lock);

	ret = wait_event_interruptible_timeout(oom_victims_wait,
			!atomic_read(&oom_victims), timeout);
	if (ret <= 0) {
		oom_killer_enable();
		return false;
	}
	pr_info("OOM killer disabled.\n");

	return true;
}

static inline bool __task_will_free_mem(struct task_struct *task)
{
	struct signal_struct *sig = task->signal;

	/*
	 * A coredumping process may sleep for an extended period in
	 * coredump_task_exit(), so the oom killer cannot assume that
	 * the process will promptly exit and release memory.
	 */
	if (sig->core_state)
		return false;

	if (sig->flags & SIGNAL_GROUP_EXIT)
		return true;

	if (thread_group_empty(task) && (task->flags & PF_EXITING))
		return true;

	return false;
}

/*
 * Checks whether the given task is dying or exiting and likely to
 * release its address space. This means that all threads and processes
 * sharing the same mm have to be killed or exiting.
 * Caller has to make sure that task->mm is stable (hold task_lock or
 * it operates on the current).
 */
static bool task_will_free_mem(struct task_struct *task)
{
	struct mm_struct *mm = task->mm;
	struct task_struct *p;
	bool ret = true;

	/*
	 * Skip tasks without mm because it might have passed its exit_mm and
	 * exit_oom_victim. oom_reaper could have rescued that but do not rely
	 * on that for now. We can consider find_lock_task_mm in future.
	 */
	if (!mm)
		return false;

	if (!__task_will_free_mem(task))
		return false;

	/*
	 * This task has already been drained by the oom reaper so there are
	 * only small chances it will free some more
	 */
	if (test_bit(MMF_OOM_SKIP, &mm->flags))
		return false;

	if (atomic_read(&mm->mm_users) <= 1)
		return true;

	/*
	 * Make sure that all tasks which share the mm with the given tasks
	 * are dying as well to make sure that a) nobody pins its mm and
	 * b) the task is also reapable by the oom reaper.
	 */
	rcu_read_lock();
	for_each_process(p) {
		if (!process_shares_mm(p, mm))
			continue;
		if (same_thread_group(task, p))
			continue;
		ret = __task_will_free_mem(p);
		if (!ret)
			break;
	}
	rcu_read_unlock();

	return ret;
}

static void __oom_kill_process(struct task_struct *victim, const char *message)
{
	struct task_struct *p;
	struct mm_struct *mm;
	bool can_oom_reap = true;

	p = find_lock_task_mm(victim);
	if (!p) {
		pr_info("%s: OOM victim %d (%s) is already exiting. Skip killing the task\n",
			message, task_pid_nr(victim), victim->comm);
		put_task_struct(victim);
		return;
	} else if (victim != p) {
		get_task_struct(p);
		put_task_struct(victim);
		victim = p;
	}

	/* Get a reference to safely compare mm after task_unlock(victim) */
	mm = victim->mm;
	mmgrab(mm);

	/* Raise event before sending signal: task reaper must see this */
	count_vm_event(OOM_KILL);
	memcg_memory_event_mm(mm, MEMCG_OOM_KILL);

	/*
	 * We should send SIGKILL before granting access to memory reserves
	 * in order to prevent the OOM victim from depleting the memory
	 * reserves from the user space under its control.
	 */
	do_send_sig_info(SIGKILL, SEND_SIG_PRIV, victim, PIDTYPE_TGID);
	mark_oom_victim(victim);
	pr_err("%s: Killed process %d (%s) total-vm:%lukB, anon-rss:%lukB, file-rss:%lukB, shmem-rss:%lukB, UID:%u pgtables:%lukB oom_score_adj:%hd\n",
		message, task_pid_nr(victim), victim->comm, K(mm->total_vm),
		K(get_mm_counter(mm, MM_ANONPAGES)),
		K(get_mm_counter(mm, MM_FILEPAGES)),
		K(get_mm_counter(mm, MM_SHMEMPAGES)),
		from_kuid(&init_user_ns, task_uid(victim)),
		mm_pgtables_bytes(mm) >> 10, victim->signal->oom_score_adj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	task_unlock(victim);

	/*
	 * Kill all user processes sharing victim->mm in other thread groups, if
	 * any.  They don't get access to memory reserves, though, to avoid
<<<<<<< HEAD
	 * depletion of all memory.  This prevents mm->mmap_sem livelock when an
=======
	 * depletion of all memory.  This prevents mm->mmap_lock livelock when an
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * oom killed thread cannot exit because it requires the semaphore and
	 * its contended by another thread trying to allocate memory itself.
	 * That thread will now get access to memory reserves since it has a
	 * pending fatal signal.
	 */
<<<<<<< HEAD
	for_each_process(p)
		if (p->mm == mm && !same_thread_group(p, victim) &&
		    !(p->flags & PF_KTHREAD)) {
			if (p->signal->oom_score_adj == OOM_SCORE_ADJ_MIN)
				continue;

			task_lock(p);	/* Protect ->comm from prctl() */
			pr_err("Kill process %d (%s) sharing same memory\n",
				task_pid_nr(p), p->comm);
			task_unlock(p);
			do_send_sig_info(SIGKILL, SEND_SIG_FORCED, p, true);
		}

	set_tsk_thread_flag(victim, TIF_MEMDIE);
	do_send_sig_info(SIGKILL, SEND_SIG_FORCED, victim, true);
}
#undef K
=======
	rcu_read_lock();
	for_each_process(p) {
		if (!process_shares_mm(p, mm))
			continue;
		if (same_thread_group(p, victim))
			continue;
		if (is_global_init(p)) {
			can_oom_reap = false;
			set_bit(MMF_OOM_SKIP, &mm->flags);
			pr_info("oom killer %d (%s) has mm pinned by %d (%s)\n",
					task_pid_nr(victim), victim->comm,
					task_pid_nr(p), p->comm);
			continue;
		}
		/*
		 * No kthread_use_mm() user needs to read from the userspace so
		 * we are ok to reap it.
		 */
		if (unlikely(p->flags & PF_KTHREAD))
			continue;
		do_send_sig_info(SIGKILL, SEND_SIG_PRIV, p, PIDTYPE_TGID);
	}
	rcu_read_unlock();

	if (can_oom_reap)
		queue_oom_reaper(victim);

	mmdrop(mm);
	put_task_struct(victim);
}

/*
 * Kill provided task unless it's secured by setting
 * oom_score_adj to OOM_SCORE_ADJ_MIN.
 */
static int oom_kill_memcg_member(struct task_struct *task, void *message)
{
	if (task->signal->oom_score_adj != OOM_SCORE_ADJ_MIN &&
	    !is_global_init(task)) {
		get_task_struct(task);
		__oom_kill_process(task, message);
	}
	return 0;
}

static void oom_kill_process(struct oom_control *oc, const char *message)
{
	struct task_struct *victim = oc->chosen;
	struct mem_cgroup *oom_group;
	static DEFINE_RATELIMIT_STATE(oom_rs, DEFAULT_RATELIMIT_INTERVAL,
					      DEFAULT_RATELIMIT_BURST);

	/*
	 * If the task is already exiting, don't alarm the sysadmin or kill
	 * its children or threads, just give it access to memory reserves
	 * so it can die quickly
	 */
	task_lock(victim);
	if (task_will_free_mem(victim)) {
		mark_oom_victim(victim);
		queue_oom_reaper(victim);
		task_unlock(victim);
		put_task_struct(victim);
		return;
	}
	task_unlock(victim);

	if (__ratelimit(&oom_rs)) {
		dump_header(oc);
		dump_oom_victim(oc, victim);
	}

	/*
	 * Do we need to kill the entire memory cgroup?
	 * Or even one of the ancestor memory cgroups?
	 * Check this out before killing the victim task.
	 */
	oom_group = mem_cgroup_get_oom_group(victim, oc->memcg);

	__oom_kill_process(victim, message);

	/*
	 * If necessary, kill all tasks in the selected memory cgroup.
	 */
	if (oom_group) {
		memcg_memory_event(oom_group, MEMCG_OOM_GROUP_KILL);
		mem_cgroup_print_oom_group(oom_group);
		mem_cgroup_scan_tasks(oom_group, oom_kill_memcg_member,
				      (void *)message);
		mem_cgroup_put(oom_group);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Determines whether the kernel must panic because of the panic_on_oom sysctl.
 */
<<<<<<< HEAD
static void check_panic_on_oom(enum oom_constraint constraint, gfp_t gfp_mask,
				int order, const nodemask_t *nodemask)
=======
static void check_panic_on_oom(struct oom_control *oc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (likely(!sysctl_panic_on_oom))
		return;
	if (sysctl_panic_on_oom != 2) {
		/*
		 * panic_on_oom == 1 only affects CONSTRAINT_NONE, the kernel
		 * does not panic for cpuset, mempolicy, or memcg allocation
		 * failures.
		 */
<<<<<<< HEAD
		if (constraint != CONSTRAINT_NONE)
			return;
	}
	read_lock(&tasklist_lock);
	dump_header(NULL, gfp_mask, order, NULL, nodemask);
	read_unlock(&tasklist_lock);
=======
		if (oc->constraint != CONSTRAINT_NONE)
			return;
	}
	/* Do not panic for oom kills triggered by sysrq */
	if (is_sysrq_oom(oc))
		return;
	dump_header(oc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	panic("Out of memory: %s panic_on_oom is enabled\n",
		sysctl_panic_on_oom == 2 ? "compulsory" : "system-wide");
}

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_MEM_RES_CTLR
void mem_cgroup_out_of_memory(struct mem_cgroup *memcg, gfp_t gfp_mask,
			      int order)
{
	unsigned long limit;
	unsigned int points = 0;
	struct task_struct *p;

	/*
	 * If current has a pending SIGKILL or is exiting, then automatically
	 * select it.  The goal is to allow it to allocate so that it may
	 * quickly exit and free its memory.
	 */
	if (fatal_signal_pending(current) || current->flags & PF_EXITING) {
		set_thread_flag(TIF_MEMDIE);
		return;
	}

	check_panic_on_oom(CONSTRAINT_MEMCG, gfp_mask, order, NULL);
	limit = mem_cgroup_get_limit(memcg) >> PAGE_SHIFT;
	read_lock(&tasklist_lock);
	p = select_bad_process(&points, limit, memcg, NULL, false);
	if (p && PTR_ERR(p) != -1UL)
		oom_kill_process(p, gfp_mask, order, points, limit, memcg, NULL,
				 "Memory cgroup out of memory");
	read_unlock(&tasklist_lock);
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static BLOCKING_NOTIFIER_HEAD(oom_notify_list);

int register_oom_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&oom_notify_list, nb);
}
EXPORT_SYMBOL_GPL(register_oom_notifier);

int unregister_oom_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&oom_notify_list, nb);
}
EXPORT_SYMBOL_GPL(unregister_oom_notifier);

<<<<<<< HEAD
/*
 * Try to acquire the OOM killer lock for the zones in zonelist.  Returns zero
 * if a parallel OOM killing is already taking place that includes a zone in
 * the zonelist.  Otherwise, locks all zones in the zonelist and returns 1.
 */
int try_set_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_mask)
{
	struct zoneref *z;
	struct zone *zone;
	int ret = 1;

	spin_lock(&zone_scan_lock);
	for_each_zone_zonelist(zone, z, zonelist, gfp_zone(gfp_mask)) {
		if (zone_is_oom_locked(zone)) {
			ret = 0;
			goto out;
		}
	}

	for_each_zone_zonelist(zone, z, zonelist, gfp_zone(gfp_mask)) {
		/*
		 * Lock each zone in the zonelist under zone_scan_lock so a
		 * parallel invocation of try_set_zonelist_oom() doesn't succeed
		 * when it shouldn't.
		 */
		zone_set_flag(zone, ZONE_OOM_LOCKED);
	}

out:
	spin_unlock(&zone_scan_lock);
	return ret;
}

/*
 * Clears the ZONE_OOM_LOCKED flag for all zones in the zonelist so that failed
 * allocation attempts with zonelists containing them may now recall the OOM
 * killer, if necessary.
 */
void clear_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_mask)
{
	struct zoneref *z;
	struct zone *zone;

	spin_lock(&zone_scan_lock);
	for_each_zone_zonelist(zone, z, zonelist, gfp_zone(gfp_mask)) {
		zone_clear_flag(zone, ZONE_OOM_LOCKED);
	}
	spin_unlock(&zone_scan_lock);
}

/*
 * Try to acquire the oom killer lock for all system zones.  Returns zero if a
 * parallel oom killing is taking place, otherwise locks all zones and returns
 * non-zero.
 */
static int try_set_system_oom(void)
{
	struct zone *zone;
	int ret = 1;

	spin_lock(&zone_scan_lock);
	for_each_populated_zone(zone)
		if (zone_is_oom_locked(zone)) {
			ret = 0;
			goto out;
		}
	for_each_populated_zone(zone)
		zone_set_flag(zone, ZONE_OOM_LOCKED);
out:
	spin_unlock(&zone_scan_lock);
	return ret;
}

/*
 * Clears ZONE_OOM_LOCKED for all system zones so that failed allocation
 * attempts or page faults may now recall the oom killer, if necessary.
 */
static void clear_system_oom(void)
{
	struct zone *zone;

	spin_lock(&zone_scan_lock);
	for_each_populated_zone(zone)
		zone_clear_flag(zone, ZONE_OOM_LOCKED);
	spin_unlock(&zone_scan_lock);
}

/**
 * out_of_memory - kill the "best" process when we run out of memory
 * @zonelist: zonelist pointer
 * @gfp_mask: memory allocation flags
 * @order: amount of memory being requested as a power of 2
 * @nodemask: nodemask passed to page allocator
 * @force_kill: true if a task must be killed, even if others are exiting
=======
/**
 * out_of_memory - kill the "best" process when we run out of memory
 * @oc: pointer to struct oom_control
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If we run out of memory, we have the choice between either
 * killing a random task (bad), letting the system crash (worse)
 * OR try to be smart about which process to kill. Note that we
 * don't have to be perfect here, we just have to be good.
 */
<<<<<<< HEAD
void out_of_memory(struct zonelist *zonelist, gfp_t gfp_mask,
		int order, nodemask_t *nodemask, bool force_kill)
{
	const nodemask_t *mpol_mask;
	struct task_struct *p;
	unsigned long totalpages;
	unsigned long freed = 0;
	unsigned int points;
	enum oom_constraint constraint = CONSTRAINT_NONE;
	int killed = 0;

	blocking_notifier_call_chain(&oom_notify_list, 0, &freed);
	if (freed > 0)
		/* Got some memory back in the last second. */
		return;

	/*
	 * If current has a pending SIGKILL, then automatically select it.  The
	 * goal is to allow it to allocate so that it may quickly exit and free
	 * its memory.
	 */
	if (fatal_signal_pending(current)) {
		set_thread_flag(TIF_MEMDIE);
		return;
	}

	/*
	 * Check if there were limitations on the allocation (only relevant for
	 * NUMA) that may require different handling.
	 */
	constraint = constrained_alloc(zonelist, gfp_mask, nodemask,
						&totalpages);
	mpol_mask = (constraint == CONSTRAINT_MEMORY_POLICY) ? nodemask : NULL;
	check_panic_on_oom(constraint, gfp_mask, order, mpol_mask);

	read_lock(&tasklist_lock);
	if (sysctl_oom_kill_allocating_task &&
	    !oom_unkillable_task(current, NULL, nodemask) &&
	    current->mm) {
		oom_kill_process(current, gfp_mask, order, 0, totalpages, NULL,
				 nodemask,
				 "Out of memory (oom_kill_allocating_task)");
		goto out;
	}

	p = select_bad_process(&points, totalpages, NULL, mpol_mask,
			       force_kill);
	/* Found nothing?!?! Either we hang forever, or we panic. */
	if (!p) {
		dump_header(NULL, gfp_mask, order, NULL, mpol_mask);
		read_unlock(&tasklist_lock);
		panic("Out of memory and no killable processes...\n");
	}
	if (PTR_ERR(p) != -1UL) {
		oom_kill_process(p, gfp_mask, order, points, totalpages, NULL,
				 nodemask, "Out of memory");
		killed = 1;
	}
out:
	read_unlock(&tasklist_lock);

	/*
	 * Give "p" a good chance of killing itself before we
	 * retry to allocate memory unless "p" is current
	 */
	if (killed && !test_thread_flag(TIF_MEMDIE))
		schedule_timeout_uninterruptible(1);
}

/*
 * The pagefault handler calls here because it is out of memory, so kill a
 * memory-hogging task.  If a populated zone has ZONE_OOM_LOCKED set, a parallel
 * oom killing is already in progress so do nothing.  If a task is found with
 * TIF_MEMDIE set, it has been killed so do nothing and allow it to exit.
 */
void pagefault_out_of_memory(void)
{
	if (try_set_system_oom()) {
		out_of_memory(NULL, 0, 0, NULL, false);
		clear_system_oom();
	}
	if (!test_thread_flag(TIF_MEMDIE))
		schedule_timeout_uninterruptible(1);
=======
bool out_of_memory(struct oom_control *oc)
{
	unsigned long freed = 0;

	if (oom_killer_disabled)
		return false;

	if (!is_memcg_oom(oc)) {
		blocking_notifier_call_chain(&oom_notify_list, 0, &freed);
		if (freed > 0 && !is_sysrq_oom(oc))
			/* Got some memory back in the last second. */
			return true;
	}

	/*
	 * If current has a pending SIGKILL or is exiting, then automatically
	 * select it.  The goal is to allow it to allocate so that it may
	 * quickly exit and free its memory.
	 */
	if (task_will_free_mem(current)) {
		mark_oom_victim(current);
		queue_oom_reaper(current);
		return true;
	}

	/*
	 * The OOM killer does not compensate for IO-less reclaim.
	 * But mem_cgroup_oom() has to invoke the OOM killer even
	 * if it is a GFP_NOFS allocation.
	 */
	if (!(oc->gfp_mask & __GFP_FS) && !is_memcg_oom(oc))
		return true;

	/*
	 * Check if there were limitations on the allocation (only relevant for
	 * NUMA and memcg) that may require different handling.
	 */
	oc->constraint = constrained_alloc(oc);
	if (oc->constraint != CONSTRAINT_MEMORY_POLICY)
		oc->nodemask = NULL;
	check_panic_on_oom(oc);

	if (!is_memcg_oom(oc) && sysctl_oom_kill_allocating_task &&
	    current->mm && !oom_unkillable_task(current) &&
	    oom_cpuset_eligible(current, oc) &&
	    current->signal->oom_score_adj != OOM_SCORE_ADJ_MIN) {
		get_task_struct(current);
		oc->chosen = current;
		oom_kill_process(oc, "Out of memory (oom_kill_allocating_task)");
		return true;
	}

	select_bad_process(oc);
	/* Found nothing?!?! */
	if (!oc->chosen) {
		dump_header(oc);
		pr_warn("Out of memory and no killable processes...\n");
		/*
		 * If we got here due to an actual allocation at the
		 * system level, we cannot survive this and will enter
		 * an endless loop in the allocator. Bail out now.
		 */
		if (!is_sysrq_oom(oc) && !is_memcg_oom(oc))
			panic("System is deadlocked on memory\n");
	}
	if (oc->chosen && oc->chosen != (void *)-1UL)
		oom_kill_process(oc, !is_memcg_oom(oc) ? "Out of memory" :
				 "Memory cgroup out of memory");
	return !!oc->chosen;
}

/*
 * The pagefault handler calls here because some allocation has failed. We have
 * to take care of the memcg OOM here because this is the only safe context without
 * any locks held but let the oom killer triggered from the allocation context care
 * about the global OOM.
 */
void pagefault_out_of_memory(void)
{
	static DEFINE_RATELIMIT_STATE(pfoom_rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);

	if (mem_cgroup_oom_synchronize(true))
		return;

	if (fatal_signal_pending(current))
		return;

	if (__ratelimit(&pfoom_rs))
		pr_warn("Huh VM_FAULT_OOM leaked out to the #PF handler. Retrying PF\n");
}

SYSCALL_DEFINE2(process_mrelease, int, pidfd, unsigned int, flags)
{
#ifdef CONFIG_MMU
	struct mm_struct *mm = NULL;
	struct task_struct *task;
	struct task_struct *p;
	unsigned int f_flags;
	bool reap = false;
	long ret = 0;

	if (flags)
		return -EINVAL;

	task = pidfd_get_task(pidfd, &f_flags);
	if (IS_ERR(task))
		return PTR_ERR(task);

	/*
	 * Make sure to choose a thread which still has a reference to mm
	 * during the group exit
	 */
	p = find_lock_task_mm(task);
	if (!p) {
		ret = -ESRCH;
		goto put_task;
	}

	mm = p->mm;
	mmgrab(mm);

	if (task_will_free_mem(p))
		reap = true;
	else {
		/* Error only if the work has not been done already */
		if (!test_bit(MMF_OOM_SKIP, &mm->flags))
			ret = -EINVAL;
	}
	task_unlock(p);

	if (!reap)
		goto drop_mm;

	if (mmap_read_lock_killable(mm)) {
		ret = -EINTR;
		goto drop_mm;
	}
	/*
	 * Check MMF_OOM_SKIP again under mmap_read_lock protection to ensure
	 * possible change in exit_mmap is seen
	 */
	if (!test_bit(MMF_OOM_SKIP, &mm->flags) && !__oom_reap_task_mm(mm))
		ret = -EAGAIN;
	mmap_read_unlock(mm);

drop_mm:
	mmdrop(mm);
put_task:
	put_task_struct(task);
	return ret;
#else
	return -ENOSYS;
#endif /* CONFIG_MMU */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
