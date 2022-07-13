<<<<<<< HEAD
/*
 * Copyright (C) 2008 Mathieu Desnoyers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2008-2014 Mathieu Desnoyers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/jhash.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/tracepoint.h>
#include <linux/err.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/static_key.h>

extern struct tracepoint * const __start___tracepoints_ptrs[];
extern struct tracepoint * const __stop___tracepoints_ptrs[];
=======
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/static_key.h>

enum tp_func_state {
	TP_FUNC_0,
	TP_FUNC_1,
	TP_FUNC_2,
	TP_FUNC_N,
};

extern tracepoint_ptr_t __start___tracepoints_ptrs[];
extern tracepoint_ptr_t __stop___tracepoints_ptrs[];

DEFINE_SRCU(tracepoint_srcu);
EXPORT_SYMBOL_GPL(tracepoint_srcu);

enum tp_transition_sync {
	TP_TRANSITION_SYNC_1_0_1,
	TP_TRANSITION_SYNC_N_2_1,

	_NR_TP_TRANSITION_SYNC,
};

struct tp_transition_snapshot {
	unsigned long rcu;
	unsigned long srcu;
	bool ongoing;
};

/* Protected by tracepoints_mutex */
static struct tp_transition_snapshot tp_transition_snapshot[_NR_TP_TRANSITION_SYNC];

static void tp_rcu_get_state(enum tp_transition_sync sync)
{
	struct tp_transition_snapshot *snapshot = &tp_transition_snapshot[sync];

	/* Keep the latest get_state snapshot. */
	snapshot->rcu = get_state_synchronize_rcu();
	snapshot->srcu = start_poll_synchronize_srcu(&tracepoint_srcu);
	snapshot->ongoing = true;
}

static void tp_rcu_cond_sync(enum tp_transition_sync sync)
{
	struct tp_transition_snapshot *snapshot = &tp_transition_snapshot[sync];

	if (!snapshot->ongoing)
		return;
	cond_synchronize_rcu(snapshot->rcu);
	if (!poll_state_synchronize_srcu(&tracepoint_srcu, snapshot->srcu))
		synchronize_srcu(&tracepoint_srcu);
	snapshot->ongoing = false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Set to 1 to enable tracepoint debug output */
static const int tracepoint_debug;

<<<<<<< HEAD
/*
 * Tracepoints mutex protects the builtin and module tracepoints and the hash
 * table, as well as the local module list.
 */
static DEFINE_MUTEX(tracepoints_mutex);

#ifdef CONFIG_MODULES
/* Local list of struct module */
=======
#ifdef CONFIG_MODULES
/*
 * Tracepoint module list mutex protects the local module list.
 */
static DEFINE_MUTEX(tracepoint_module_list_mutex);

/* Local list of struct tp_module */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static LIST_HEAD(tracepoint_module_list);
#endif /* CONFIG_MODULES */

/*
<<<<<<< HEAD
 * Tracepoint hash table, containing the active tracepoints.
 * Protected by tracepoints_mutex.
 */
#define TRACEPOINT_HASH_BITS 6
#define TRACEPOINT_TABLE_SIZE (1 << TRACEPOINT_HASH_BITS)
static struct hlist_head tracepoint_table[TRACEPOINT_TABLE_SIZE];
=======
 * tracepoints_mutex protects the builtin and module tracepoints.
 * tracepoints_mutex nests inside tracepoint_module_list_mutex.
 */
static DEFINE_MUTEX(tracepoints_mutex);

static struct rcu_head *early_probes;
static bool ok_to_free_tracepoints;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Note about RCU :
 * It is used to delay the free of multiple probes array until a quiescent
 * state is reached.
<<<<<<< HEAD
 * Tracepoint entries modifications are protected by the tracepoints_mutex.
 */
struct tracepoint_entry {
	struct hlist_node hlist;
	struct tracepoint_func *funcs;
	int refcount;	/* Number of times armed. 0 if disarmed. */
	char name[0];
};

struct tp_probes {
	union {
		struct rcu_head rcu;
		struct list_head list;
	} u;
	struct tracepoint_func probes[0];
};

static inline void *allocate_probes(int count)
{
	struct tp_probes *p  = kmalloc(count * sizeof(struct tracepoint_func)
			+ sizeof(struct tp_probes), GFP_KERNEL);
	return p == NULL ? NULL : p->probes;
}

static void rcu_free_old_probes(struct rcu_head *head)
{
	kfree(container_of(head, struct tp_probes, u.rcu));
}

=======
 */
struct tp_probes {
	struct rcu_head rcu;
	struct tracepoint_func probes[];
};

/* Called in removal of a func but failed to allocate a new tp_funcs */
static void tp_stub_func(void)
{
	return;
}

static inline void *allocate_probes(int count)
{
	struct tp_probes *p  = kmalloc(struct_size(p, probes, count),
				       GFP_KERNEL);
	return p == NULL ? NULL : p->probes;
}

static void srcu_free_old_probes(struct rcu_head *head)
{
	kfree(container_of(head, struct tp_probes, rcu));
}

static void rcu_free_old_probes(struct rcu_head *head)
{
	call_srcu(&tracepoint_srcu, head, srcu_free_old_probes);
}

static __init int release_early_probes(void)
{
	struct rcu_head *tmp;

	ok_to_free_tracepoints = true;

	while (early_probes) {
		tmp = early_probes;
		early_probes = tmp->next;
		call_rcu(tmp, rcu_free_old_probes);
	}

	return 0;
}

/* SRCU is initialized at core_initcall */
postcore_initcall(release_early_probes);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void release_probes(struct tracepoint_func *old)
{
	if (old) {
		struct tp_probes *tp_probes = container_of(old,
			struct tp_probes, probes[0]);
<<<<<<< HEAD
		call_rcu_sched(&tp_probes->u.rcu, rcu_free_old_probes);
	}
}

static void debug_print_probes(struct tracepoint_entry *entry)
{
	int i;

	if (!tracepoint_debug || !entry->funcs)
		return;

	for (i = 0; entry->funcs[i].func; i++)
		printk(KERN_DEBUG "Probe %d : %p\n", i, entry->funcs[i].func);
}

static struct tracepoint_func *
tracepoint_entry_add_probe(struct tracepoint_entry *entry,
			   void *probe, void *data)
{
	int nr_probes = 0;
	struct tracepoint_func *old, *new;

	WARN_ON(!probe);

	debug_print_probes(entry);
	old = entry->funcs;
	if (old) {
		/* (N -> N+1), (N != 0, 1) probes */
		for (nr_probes = 0; old[nr_probes].func; nr_probes++)
			if (old[nr_probes].func == probe &&
			    old[nr_probes].data == data)
				return ERR_PTR(-EEXIST);
=======

		/*
		 * We can't free probes if SRCU is not initialized yet.
		 * Postpone the freeing till after SRCU is initialized.
		 */
		if (unlikely(!ok_to_free_tracepoints)) {
			tp_probes->rcu.next = early_probes;
			early_probes = &tp_probes->rcu;
			return;
		}

		/*
		 * Tracepoint probes are protected by both sched RCU and SRCU,
		 * by calling the SRCU callback in the sched RCU callback we
		 * cover both cases. So let us chain the SRCU and sched RCU
		 * callbacks to wait for both grace periods.
		 */
		call_rcu(&tp_probes->rcu, rcu_free_old_probes);
	}
}

static void debug_print_probes(struct tracepoint_func *funcs)
{
	int i;

	if (!tracepoint_debug || !funcs)
		return;

	for (i = 0; funcs[i].func; i++)
		printk(KERN_DEBUG "Probe %d : %p\n", i, funcs[i].func);
}

static struct tracepoint_func *
func_add(struct tracepoint_func **funcs, struct tracepoint_func *tp_func,
	 int prio)
{
	struct tracepoint_func *old, *new;
	int iter_probes;	/* Iterate over old probe array. */
	int nr_probes = 0;	/* Counter for probes */
	int pos = -1;		/* Insertion position into new array */

	if (WARN_ON(!tp_func->func))
		return ERR_PTR(-EINVAL);

	debug_print_probes(*funcs);
	old = *funcs;
	if (old) {
		/* (N -> N+1), (N != 0, 1) probes */
		for (iter_probes = 0; old[iter_probes].func; iter_probes++) {
			if (old[iter_probes].func == tp_stub_func)
				continue;	/* Skip stub functions. */
			if (old[iter_probes].func == tp_func->func &&
			    old[iter_probes].data == tp_func->data)
				return ERR_PTR(-EEXIST);
			nr_probes++;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* + 2 : one for new probe, one for NULL func */
	new = allocate_probes(nr_probes + 2);
	if (new == NULL)
		return ERR_PTR(-ENOMEM);
<<<<<<< HEAD
	if (old)
		memcpy(new, old, nr_probes * sizeof(struct tracepoint_func));
	new[nr_probes].func = probe;
	new[nr_probes].data = data;
	new[nr_probes + 1].func = NULL;
	entry->refcount = nr_probes + 1;
	entry->funcs = new;
	debug_print_probes(entry);
	return old;
}

static void *
tracepoint_entry_remove_probe(struct tracepoint_entry *entry,
			      void *probe, void *data)
=======
	if (old) {
		nr_probes = 0;
		for (iter_probes = 0; old[iter_probes].func; iter_probes++) {
			if (old[iter_probes].func == tp_stub_func)
				continue;
			/* Insert before probes of lower priority */
			if (pos < 0 && old[iter_probes].prio < prio)
				pos = nr_probes++;
			new[nr_probes++] = old[iter_probes];
		}
		if (pos < 0)
			pos = nr_probes++;
		/* nr_probes now points to the end of the new array */
	} else {
		pos = 0;
		nr_probes = 1; /* must point at end of array */
	}
	new[pos] = *tp_func;
	new[nr_probes].func = NULL;
	*funcs = new;
	debug_print_probes(*funcs);
	return old;
}

static void *func_remove(struct tracepoint_func **funcs,
		struct tracepoint_func *tp_func)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int nr_probes = 0, nr_del = 0, i;
	struct tracepoint_func *old, *new;

<<<<<<< HEAD
	old = entry->funcs;
=======
	old = *funcs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!old)
		return ERR_PTR(-ENOENT);

<<<<<<< HEAD
	debug_print_probes(entry);
	/* (N -> M), (N > 1, M >= 0) probes */
	for (nr_probes = 0; old[nr_probes].func; nr_probes++) {
		if (!probe ||
		    (old[nr_probes].func == probe &&
		     old[nr_probes].data == data))
			nr_del++;
	}

	if (nr_probes - nr_del == 0) {
		/* N -> 0, (N > 1) */
		entry->funcs = NULL;
		entry->refcount = 0;
		debug_print_probes(entry);
=======
	debug_print_probes(*funcs);
	/* (N -> M), (N > 1, M >= 0) probes */
	if (tp_func->func) {
		for (nr_probes = 0; old[nr_probes].func; nr_probes++) {
			if ((old[nr_probes].func == tp_func->func &&
			     old[nr_probes].data == tp_func->data) ||
			    old[nr_probes].func == tp_stub_func)
				nr_del++;
		}
	}

	/*
	 * If probe is NULL, then nr_probes = nr_del = 0, and then the
	 * entire entry will be removed.
	 */
	if (nr_probes - nr_del == 0) {
		/* N -> 0, (N > 1) */
		*funcs = NULL;
		debug_print_probes(*funcs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return old;
	} else {
		int j = 0;
		/* N -> M, (N > 1, M > 0) */
		/* + 1 for NULL */
		new = allocate_probes(nr_probes - nr_del + 1);
<<<<<<< HEAD
		if (new == NULL)
			return ERR_PTR(-ENOMEM);
		for (i = 0; old[i].func; i++)
			if (probe &&
			    (old[i].func != probe || old[i].data != data))
				new[j++] = old[i];
		new[nr_probes - nr_del].func = NULL;
		entry->refcount = nr_probes - nr_del;
		entry->funcs = new;
	}
	debug_print_probes(entry);
=======
		if (new) {
			for (i = 0; old[i].func; i++) {
				if ((old[i].func != tp_func->func ||
				     old[i].data != tp_func->data) &&
				    old[i].func != tp_stub_func)
					new[j++] = old[i];
			}
			new[nr_probes - nr_del].func = NULL;
			*funcs = new;
		} else {
			/*
			 * Failed to allocate, replace the old function
			 * with calls to tp_stub_func.
			 */
			for (i = 0; old[i].func; i++) {
				if (old[i].func == tp_func->func &&
				    old[i].data == tp_func->data)
					WRITE_ONCE(old[i].func, tp_stub_func);
			}
			*funcs = old;
		}
	}
	debug_print_probes(*funcs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return old;
}

/*
<<<<<<< HEAD
 * Get tracepoint if the tracepoint is present in the tracepoint hash table.
 * Must be called with tracepoints_mutex held.
 * Returns NULL if not present.
 */
static struct tracepoint_entry *get_tracepoint(const char *name)
{
	struct hlist_head *head;
	struct hlist_node *node;
	struct tracepoint_entry *e;
	u32 hash = jhash(name, strlen(name), 0);

	head = &tracepoint_table[hash & (TRACEPOINT_TABLE_SIZE - 1)];
	hlist_for_each_entry(e, node, head, hlist) {
		if (!strcmp(name, e->name))
			return e;
	}
	return NULL;
}

/*
 * Add the tracepoint to the tracepoint hash table. Must be called with
 * tracepoints_mutex held.
 */
static struct tracepoint_entry *add_tracepoint(const char *name)
{
	struct hlist_head *head;
	struct hlist_node *node;
	struct tracepoint_entry *e;
	size_t name_len = strlen(name) + 1;
	u32 hash = jhash(name, name_len-1, 0);

	head = &tracepoint_table[hash & (TRACEPOINT_TABLE_SIZE - 1)];
	hlist_for_each_entry(e, node, head, hlist) {
		if (!strcmp(name, e->name)) {
			printk(KERN_NOTICE
				"tracepoint %s busy\n", name);
			return ERR_PTR(-EEXIST);	/* Already there */
		}
	}
	/*
	 * Using kmalloc here to allocate a variable length element. Could
	 * cause some memory fragmentation if overused.
	 */
	e = kmalloc(sizeof(struct tracepoint_entry) + name_len, GFP_KERNEL);
	if (!e)
		return ERR_PTR(-ENOMEM);
	memcpy(&e->name[0], name, name_len);
	e->funcs = NULL;
	e->refcount = 0;
	hlist_add_head(&e->hlist, head);
	return e;
}

/*
 * Remove the tracepoint from the tracepoint hash table. Must be called with
 * mutex_lock held.
 */
static inline void remove_tracepoint(struct tracepoint_entry *e)
{
	hlist_del(&e->hlist);
	kfree(e);
}

/*
 * Sets the probe callback corresponding to one tracepoint.
 */
static void set_tracepoint(struct tracepoint_entry **entry,
	struct tracepoint *elem, int active)
{
	WARN_ON(strcmp((*entry)->name, elem->name) != 0);

	if (elem->regfunc && !static_key_enabled(&elem->key) && active)
		elem->regfunc();
	else if (elem->unregfunc && static_key_enabled(&elem->key) && !active)
		elem->unregfunc();

	/*
	 * rcu_assign_pointer has a smp_wmb() which makes sure that the new
	 * probe callbacks array is consistent before setting a pointer to it.
	 * This array is referenced by __DO_TRACE from
	 * include/linux/tracepoints.h. A matching smp_read_barrier_depends()
	 * is used.
	 */
	rcu_assign_pointer(elem->funcs, (*entry)->funcs);
	if (active && !static_key_enabled(&elem->key))
		static_key_slow_inc(&elem->key);
	else if (!active && static_key_enabled(&elem->key))
		static_key_slow_dec(&elem->key);
}

/*
 * Disable a tracepoint and its probe callback.
=======
 * Count the number of functions (enum tp_func_state) in a tp_funcs array.
 */
static enum tp_func_state nr_func_state(const struct tracepoint_func *tp_funcs)
{
	if (!tp_funcs)
		return TP_FUNC_0;
	if (!tp_funcs[1].func)
		return TP_FUNC_1;
	if (!tp_funcs[2].func)
		return TP_FUNC_2;
	return TP_FUNC_N;	/* 3 or more */
}

static void tracepoint_update_call(struct tracepoint *tp, struct tracepoint_func *tp_funcs)
{
	void *func = tp->iterator;

	/* Synthetic events do not have static call sites */
	if (!tp->static_call_key)
		return;
	if (nr_func_state(tp_funcs) == TP_FUNC_1)
		func = tp_funcs[0].func;
	__static_call_update(tp->static_call_key, tp->static_call_tramp, func);
}

/*
 * Add the probe function to a tracepoint.
 */
static int tracepoint_add_func(struct tracepoint *tp,
			       struct tracepoint_func *func, int prio,
			       bool warn)
{
	struct tracepoint_func *old, *tp_funcs;
	int ret;

	if (tp->regfunc && !static_key_enabled(&tp->key)) {
		ret = tp->regfunc();
		if (ret < 0)
			return ret;
	}

	tp_funcs = rcu_dereference_protected(tp->funcs,
			lockdep_is_held(&tracepoints_mutex));
	old = func_add(&tp_funcs, func, prio);
	if (IS_ERR(old)) {
		WARN_ON_ONCE(warn && PTR_ERR(old) != -ENOMEM);
		return PTR_ERR(old);
	}

	/*
	 * rcu_assign_pointer has as smp_store_release() which makes sure
	 * that the new probe callbacks array is consistent before setting
	 * a pointer to it.  This array is referenced by __DO_TRACE from
	 * include/linux/tracepoint.h using rcu_dereference_sched().
	 */
	switch (nr_func_state(tp_funcs)) {
	case TP_FUNC_1:		/* 0->1 */
		/*
		 * Make sure new static func never uses old data after a
		 * 1->0->1 transition sequence.
		 */
		tp_rcu_cond_sync(TP_TRANSITION_SYNC_1_0_1);
		/* Set static call to first function */
		tracepoint_update_call(tp, tp_funcs);
		/* Both iterator and static call handle NULL tp->funcs */
		rcu_assign_pointer(tp->funcs, tp_funcs);
		static_key_enable(&tp->key);
		break;
	case TP_FUNC_2:		/* 1->2 */
		/* Set iterator static call */
		tracepoint_update_call(tp, tp_funcs);
		/*
		 * Iterator callback installed before updating tp->funcs.
		 * Requires ordering between RCU assign/dereference and
		 * static call update/call.
		 */
		fallthrough;
	case TP_FUNC_N:		/* N->N+1 (N>1) */
		rcu_assign_pointer(tp->funcs, tp_funcs);
		/*
		 * Make sure static func never uses incorrect data after a
		 * N->...->2->1 (N>1) transition sequence.
		 */
		if (tp_funcs[0].data != old[0].data)
			tp_rcu_get_state(TP_TRANSITION_SYNC_N_2_1);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}

	release_probes(old);
	return 0;
}

/*
 * Remove a probe function from a tracepoint.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Note: only waiting an RCU period after setting elem->call to the empty
 * function insures that the original callback is not used anymore. This insured
 * by preempt_disable around the call site.
 */
<<<<<<< HEAD
static void disable_tracepoint(struct tracepoint *elem)
{
	if (elem->unregfunc && static_key_enabled(&elem->key))
		elem->unregfunc();

	if (static_key_enabled(&elem->key))
		static_key_slow_dec(&elem->key);
	rcu_assign_pointer(elem->funcs, NULL);
}

/**
 * tracepoint_update_probe_range - Update a probe range
 * @begin: beginning of the range
 * @end: end of the range
 *
 * Updates the probe callback corresponding to a range of tracepoints.
 * Called with tracepoints_mutex held.
 */
static void tracepoint_update_probe_range(struct tracepoint * const *begin,
					  struct tracepoint * const *end)
{
	struct tracepoint * const *iter;
	struct tracepoint_entry *mark_entry;

	if (!begin)
		return;

	for (iter = begin; iter < end; iter++) {
		mark_entry = get_tracepoint((*iter)->name);
		if (mark_entry) {
			set_tracepoint(&mark_entry, *iter,
					!!mark_entry->refcount);
		} else {
			disable_tracepoint(*iter);
		}
	}
}

#ifdef CONFIG_MODULES
void module_update_tracepoints(void)
{
	struct tp_module *tp_mod;

	list_for_each_entry(tp_mod, &tracepoint_module_list, list)
		tracepoint_update_probe_range(tp_mod->tracepoints_ptrs,
			tp_mod->tracepoints_ptrs + tp_mod->num_tracepoints);
}
#else /* CONFIG_MODULES */
void module_update_tracepoints(void)
{
}
#endif /* CONFIG_MODULES */


/*
 * Update probes, removing the faulty probes.
 * Called with tracepoints_mutex held.
 */
static void tracepoint_update_probes(void)
{
	/* Core kernel tracepoints */
	tracepoint_update_probe_range(__start___tracepoints_ptrs,
		__stop___tracepoints_ptrs);
	/* tracepoints in modules. */
	module_update_tracepoints();
}

static struct tracepoint_func *
tracepoint_add_probe(const char *name, void *probe, void *data)
{
	struct tracepoint_entry *entry;
	struct tracepoint_func *old;

	entry = get_tracepoint(name);
	if (!entry) {
		entry = add_tracepoint(name);
		if (IS_ERR(entry))
			return (struct tracepoint_func *)entry;
	}
	old = tracepoint_entry_add_probe(entry, probe, data);
	if (IS_ERR(old) && !entry->refcount)
		remove_tracepoint(entry);
	return old;
}

/**
 * tracepoint_probe_register -  Connect a probe to a tracepoint
 * @name: tracepoint name
 * @probe: probe handler
 *
 * Returns 0 if ok, error value on error.
 * The probe address must at least be aligned on the architecture pointer size.
 */
int tracepoint_probe_register(const char *name, void *probe, void *data)
{
	struct tracepoint_func *old;

	mutex_lock(&tracepoints_mutex);
	old = tracepoint_add_probe(name, probe, data);
	if (IS_ERR(old)) {
		mutex_unlock(&tracepoints_mutex);
		return PTR_ERR(old);
	}
	tracepoint_update_probes();		/* may update entry */
	mutex_unlock(&tracepoints_mutex);
	release_probes(old);
	return 0;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_register);

static struct tracepoint_func *
tracepoint_remove_probe(const char *name, void *probe, void *data)
{
	struct tracepoint_entry *entry;
	struct tracepoint_func *old;

	entry = get_tracepoint(name);
	if (!entry)
		return ERR_PTR(-ENOENT);
	old = tracepoint_entry_remove_probe(entry, probe, data);
	if (IS_ERR(old))
		return old;
	if (!entry->refcount)
		remove_tracepoint(entry);
	return old;
}

/**
 * tracepoint_probe_unregister -  Disconnect a probe from a tracepoint
 * @name: tracepoint name
 * @probe: probe function pointer
 *
 * We do not need to call a synchronize_sched to make sure the probes have
 * finished running before doing a module unload, because the module unload
 * itself uses stop_machine(), which insures that every preempt disabled section
 * have finished.
 */
int tracepoint_probe_unregister(const char *name, void *probe, void *data)
{
	struct tracepoint_func *old;

	mutex_lock(&tracepoints_mutex);
	old = tracepoint_remove_probe(name, probe, data);
	if (IS_ERR(old)) {
		mutex_unlock(&tracepoints_mutex);
		return PTR_ERR(old);
	}
	tracepoint_update_probes();		/* may update entry */
	mutex_unlock(&tracepoints_mutex);
	release_probes(old);
	return 0;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_unregister);

static LIST_HEAD(old_probes);
static int need_update;

static void tracepoint_add_old_probes(void *old)
{
	need_update = 1;
	if (old) {
		struct tp_probes *tp_probes = container_of(old,
			struct tp_probes, probes[0]);
		list_add(&tp_probes->u.list, &old_probes);
	}
}

/**
 * tracepoint_probe_register_noupdate -  register a probe but not connect
 * @name: tracepoint name
 * @probe: probe handler
 *
 * caller must call tracepoint_probe_update_all()
 */
int tracepoint_probe_register_noupdate(const char *name, void *probe,
				       void *data)
{
	struct tracepoint_func *old;

	mutex_lock(&tracepoints_mutex);
	old = tracepoint_add_probe(name, probe, data);
	if (IS_ERR(old)) {
		mutex_unlock(&tracepoints_mutex);
		return PTR_ERR(old);
	}
	tracepoint_add_old_probes(old);
	mutex_unlock(&tracepoints_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_register_noupdate);

/**
 * tracepoint_probe_unregister_noupdate -  remove a probe but not disconnect
 * @name: tracepoint name
 * @probe: probe function pointer
 *
 * caller must call tracepoint_probe_update_all()
 */
int tracepoint_probe_unregister_noupdate(const char *name, void *probe,
					 void *data)
{
	struct tracepoint_func *old;

	mutex_lock(&tracepoints_mutex);
	old = tracepoint_remove_probe(name, probe, data);
	if (IS_ERR(old)) {
		mutex_unlock(&tracepoints_mutex);
		return PTR_ERR(old);
	}
	tracepoint_add_old_probes(old);
	mutex_unlock(&tracepoints_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_unregister_noupdate);

/**
 * tracepoint_probe_update_all -  update tracepoints
 */
void tracepoint_probe_update_all(void)
{
	LIST_HEAD(release_probes);
	struct tp_probes *pos, *next;

	mutex_lock(&tracepoints_mutex);
	if (!need_update) {
		mutex_unlock(&tracepoints_mutex);
		return;
	}
	if (!list_empty(&old_probes))
		list_replace_init(&old_probes, &release_probes);
	need_update = 0;
	tracepoint_update_probes();
	mutex_unlock(&tracepoints_mutex);
	list_for_each_entry_safe(pos, next, &release_probes, u.list) {
		list_del(&pos->u.list);
		call_rcu_sched(&pos->u.rcu, rcu_free_old_probes);
	}
}
EXPORT_SYMBOL_GPL(tracepoint_probe_update_all);

/**
 * tracepoint_get_iter_range - Get a next tracepoint iterator given a range.
 * @tracepoint: current tracepoints (in), next tracepoint (out)
 * @begin: beginning of the range
 * @end: end of the range
 *
 * Returns whether a next tracepoint has been found (1) or not (0).
 * Will return the first tracepoint in the range if the input tracepoint is
 * NULL.
 */
static int tracepoint_get_iter_range(struct tracepoint * const **tracepoint,
	struct tracepoint * const *begin, struct tracepoint * const *end)
{
	if (!*tracepoint && begin != end) {
		*tracepoint = begin;
		return 1;
	}
	if (*tracepoint >= begin && *tracepoint < end)
		return 1;
	return 0;
}

#ifdef CONFIG_MODULES
static void tracepoint_get_iter(struct tracepoint_iter *iter)
{
	int found = 0;
	struct tp_module *iter_mod;

	/* Core kernel tracepoints */
	if (!iter->module) {
		found = tracepoint_get_iter_range(&iter->tracepoint,
				__start___tracepoints_ptrs,
				__stop___tracepoints_ptrs);
		if (found)
			goto end;
	}
	/* Tracepoints in modules */
	mutex_lock(&tracepoints_mutex);
	list_for_each_entry(iter_mod, &tracepoint_module_list, list) {
		/*
		 * Sorted module list
		 */
		if (iter_mod < iter->module)
			continue;
		else if (iter_mod > iter->module)
			iter->tracepoint = NULL;
		found = tracepoint_get_iter_range(&iter->tracepoint,
			iter_mod->tracepoints_ptrs,
			iter_mod->tracepoints_ptrs
				+ iter_mod->num_tracepoints);
		if (found) {
			iter->module = iter_mod;
			break;
		}
	}
	mutex_unlock(&tracepoints_mutex);
end:
	if (!found)
		tracepoint_iter_reset(iter);
}
#else /* CONFIG_MODULES */
static void tracepoint_get_iter(struct tracepoint_iter *iter)
{
	int found = 0;

	/* Core kernel tracepoints */
	found = tracepoint_get_iter_range(&iter->tracepoint,
			__start___tracepoints_ptrs,
			__stop___tracepoints_ptrs);
	if (!found)
		tracepoint_iter_reset(iter);
}
#endif /* CONFIG_MODULES */

void tracepoint_iter_start(struct tracepoint_iter *iter)
{
	tracepoint_get_iter(iter);
}
EXPORT_SYMBOL_GPL(tracepoint_iter_start);

void tracepoint_iter_next(struct tracepoint_iter *iter)
{
	iter->tracepoint++;
	/*
	 * iter->tracepoint may be invalid because we blindly incremented it.
	 * Make sure it is valid by marshalling on the tracepoints, getting the
	 * tracepoints from following modules if necessary.
	 */
	tracepoint_get_iter(iter);
}
EXPORT_SYMBOL_GPL(tracepoint_iter_next);

void tracepoint_iter_stop(struct tracepoint_iter *iter)
{
}
EXPORT_SYMBOL_GPL(tracepoint_iter_stop);

void tracepoint_iter_reset(struct tracepoint_iter *iter)
{
#ifdef CONFIG_MODULES
	iter->module = NULL;
#endif /* CONFIG_MODULES */
	iter->tracepoint = NULL;
}
EXPORT_SYMBOL_GPL(tracepoint_iter_reset);
=======
static int tracepoint_remove_func(struct tracepoint *tp,
		struct tracepoint_func *func)
{
	struct tracepoint_func *old, *tp_funcs;

	tp_funcs = rcu_dereference_protected(tp->funcs,
			lockdep_is_held(&tracepoints_mutex));
	old = func_remove(&tp_funcs, func);
	if (WARN_ON_ONCE(IS_ERR(old)))
		return PTR_ERR(old);

	if (tp_funcs == old)
		/* Failed allocating new tp_funcs, replaced func with stub */
		return 0;

	switch (nr_func_state(tp_funcs)) {
	case TP_FUNC_0:		/* 1->0 */
		/* Removed last function */
		if (tp->unregfunc && static_key_enabled(&tp->key))
			tp->unregfunc();

		static_key_disable(&tp->key);
		/* Set iterator static call */
		tracepoint_update_call(tp, tp_funcs);
		/* Both iterator and static call handle NULL tp->funcs */
		rcu_assign_pointer(tp->funcs, NULL);
		/*
		 * Make sure new static func never uses old data after a
		 * 1->0->1 transition sequence.
		 */
		tp_rcu_get_state(TP_TRANSITION_SYNC_1_0_1);
		break;
	case TP_FUNC_1:		/* 2->1 */
		rcu_assign_pointer(tp->funcs, tp_funcs);
		/*
		 * Make sure static func never uses incorrect data after a
		 * N->...->2->1 (N>2) transition sequence. If the first
		 * element's data has changed, then force the synchronization
		 * to prevent current readers that have loaded the old data
		 * from calling the new function.
		 */
		if (tp_funcs[0].data != old[0].data)
			tp_rcu_get_state(TP_TRANSITION_SYNC_N_2_1);
		tp_rcu_cond_sync(TP_TRANSITION_SYNC_N_2_1);
		/* Set static call to first function */
		tracepoint_update_call(tp, tp_funcs);
		break;
	case TP_FUNC_2:		/* N->N-1 (N>2) */
		fallthrough;
	case TP_FUNC_N:
		rcu_assign_pointer(tp->funcs, tp_funcs);
		/*
		 * Make sure static func never uses incorrect data after a
		 * N->...->2->1 (N>2) transition sequence.
		 */
		if (tp_funcs[0].data != old[0].data)
			tp_rcu_get_state(TP_TRANSITION_SYNC_N_2_1);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}
	release_probes(old);
	return 0;
}

/**
 * tracepoint_probe_register_prio_may_exist -  Connect a probe to a tracepoint with priority
 * @tp: tracepoint
 * @probe: probe handler
 * @data: tracepoint data
 * @prio: priority of this function over other registered functions
 *
 * Same as tracepoint_probe_register_prio() except that it will not warn
 * if the tracepoint is already registered.
 */
int tracepoint_probe_register_prio_may_exist(struct tracepoint *tp, void *probe,
					     void *data, int prio)
{
	struct tracepoint_func tp_func;
	int ret;

	mutex_lock(&tracepoints_mutex);
	tp_func.func = probe;
	tp_func.data = data;
	tp_func.prio = prio;
	ret = tracepoint_add_func(tp, &tp_func, prio, false);
	mutex_unlock(&tracepoints_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_register_prio_may_exist);

/**
 * tracepoint_probe_register_prio -  Connect a probe to a tracepoint with priority
 * @tp: tracepoint
 * @probe: probe handler
 * @data: tracepoint data
 * @prio: priority of this function over other registered functions
 *
 * Returns 0 if ok, error value on error.
 * Note: if @tp is within a module, the caller is responsible for
 * unregistering the probe before the module is gone. This can be
 * performed either with a tracepoint module going notifier, or from
 * within module exit functions.
 */
int tracepoint_probe_register_prio(struct tracepoint *tp, void *probe,
				   void *data, int prio)
{
	struct tracepoint_func tp_func;
	int ret;

	mutex_lock(&tracepoints_mutex);
	tp_func.func = probe;
	tp_func.data = data;
	tp_func.prio = prio;
	ret = tracepoint_add_func(tp, &tp_func, prio, true);
	mutex_unlock(&tracepoints_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_register_prio);

/**
 * tracepoint_probe_register -  Connect a probe to a tracepoint
 * @tp: tracepoint
 * @probe: probe handler
 * @data: tracepoint data
 *
 * Returns 0 if ok, error value on error.
 * Note: if @tp is within a module, the caller is responsible for
 * unregistering the probe before the module is gone. This can be
 * performed either with a tracepoint module going notifier, or from
 * within module exit functions.
 */
int tracepoint_probe_register(struct tracepoint *tp, void *probe, void *data)
{
	return tracepoint_probe_register_prio(tp, probe, data, TRACEPOINT_DEFAULT_PRIO);
}
EXPORT_SYMBOL_GPL(tracepoint_probe_register);

/**
 * tracepoint_probe_unregister -  Disconnect a probe from a tracepoint
 * @tp: tracepoint
 * @probe: probe function pointer
 * @data: tracepoint data
 *
 * Returns 0 if ok, error value on error.
 */
int tracepoint_probe_unregister(struct tracepoint *tp, void *probe, void *data)
{
	struct tracepoint_func tp_func;
	int ret;

	mutex_lock(&tracepoints_mutex);
	tp_func.func = probe;
	tp_func.data = data;
	ret = tracepoint_remove_func(tp, &tp_func);
	mutex_unlock(&tracepoints_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(tracepoint_probe_unregister);

static void for_each_tracepoint_range(
		tracepoint_ptr_t *begin, tracepoint_ptr_t *end,
		void (*fct)(struct tracepoint *tp, void *priv),
		void *priv)
{
	tracepoint_ptr_t *iter;

	if (!begin)
		return;
	for (iter = begin; iter < end; iter++)
		fct(tracepoint_ptr_deref(iter), priv);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_MODULES
bool trace_module_has_bad_taint(struct module *mod)
{
<<<<<<< HEAD
	return mod->taints & ~((1 << TAINT_OOT_MODULE) | (1 << TAINT_CRAP));
=======
	return mod->taints & ~((1 << TAINT_OOT_MODULE) | (1 << TAINT_CRAP) |
				(1 << TAINT_UNSIGNED_MODULE) | (1 << TAINT_TEST) |
				(1 << TAINT_LIVEPATCH));
}

static BLOCKING_NOTIFIER_HEAD(tracepoint_notify_list);

/**
 * register_tracepoint_module_notifier - register tracepoint coming/going notifier
 * @nb: notifier block
 *
 * Notifiers registered with this function are called on module
 * coming/going with the tracepoint_module_list_mutex held.
 * The notifier block callback should expect a "struct tp_module" data
 * pointer.
 */
int register_tracepoint_module_notifier(struct notifier_block *nb)
{
	struct tp_module *tp_mod;
	int ret;

	mutex_lock(&tracepoint_module_list_mutex);
	ret = blocking_notifier_chain_register(&tracepoint_notify_list, nb);
	if (ret)
		goto end;
	list_for_each_entry(tp_mod, &tracepoint_module_list, list)
		(void) nb->notifier_call(nb, MODULE_STATE_COMING, tp_mod);
end:
	mutex_unlock(&tracepoint_module_list_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(register_tracepoint_module_notifier);

/**
 * unregister_tracepoint_module_notifier - unregister tracepoint coming/going notifier
 * @nb: notifier block
 *
 * The notifier block callback should expect a "struct tp_module" data
 * pointer.
 */
int unregister_tracepoint_module_notifier(struct notifier_block *nb)
{
	struct tp_module *tp_mod;
	int ret;

	mutex_lock(&tracepoint_module_list_mutex);
	ret = blocking_notifier_chain_unregister(&tracepoint_notify_list, nb);
	if (ret)
		goto end;
	list_for_each_entry(tp_mod, &tracepoint_module_list, list)
		(void) nb->notifier_call(nb, MODULE_STATE_GOING, tp_mod);
end:
	mutex_unlock(&tracepoint_module_list_mutex);
	return ret;

}
EXPORT_SYMBOL_GPL(unregister_tracepoint_module_notifier);

/*
 * Ensure the tracer unregistered the module's probes before the module
 * teardown is performed. Prevents leaks of probe and data pointers.
 */
static void tp_module_going_check_quiescent(struct tracepoint *tp, void *priv)
{
	WARN_ON_ONCE(tp->funcs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tracepoint_module_coming(struct module *mod)
{
<<<<<<< HEAD
	struct tp_module *tp_mod, *iter;
	int ret = 0;
=======
	struct tp_module *tp_mod;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!mod->num_tracepoints)
		return 0;

	/*
	 * We skip modules that taint the kernel, especially those with different
	 * module headers (for forced load), to make sure we don't cause a crash.
<<<<<<< HEAD
	 * Staging and out-of-tree GPL modules are fine.
	 */
	if (trace_module_has_bad_taint(mod))
		return 0;
	mutex_lock(&tracepoints_mutex);
	tp_mod = kmalloc(sizeof(struct tp_module), GFP_KERNEL);
	if (!tp_mod) {
		ret = -ENOMEM;
		goto end;
	}
	tp_mod->num_tracepoints = mod->num_tracepoints;
	tp_mod->tracepoints_ptrs = mod->tracepoints_ptrs;

	/*
	 * tracepoint_module_list is kept sorted by struct module pointer
	 * address for iteration on tracepoints from a seq_file that can release
	 * the mutex between calls.
	 */
	list_for_each_entry_reverse(iter, &tracepoint_module_list, list) {
		BUG_ON(iter == tp_mod);	/* Should never be in the list twice */
		if (iter < tp_mod) {
			/* We belong to the location right after iter. */
			list_add(&tp_mod->list, &iter->list);
			goto module_added;
		}
	}
	/* We belong to the beginning of the list */
	list_add(&tp_mod->list, &tracepoint_module_list);
module_added:
	tracepoint_update_probe_range(mod->tracepoints_ptrs,
		mod->tracepoints_ptrs + mod->num_tracepoints);
end:
	mutex_unlock(&tracepoints_mutex);
	return ret;
}

static int tracepoint_module_going(struct module *mod)
{
	struct tp_module *pos;

	if (!mod->num_tracepoints)
		return 0;

	mutex_lock(&tracepoints_mutex);
	tracepoint_update_probe_range(mod->tracepoints_ptrs,
		mod->tracepoints_ptrs + mod->num_tracepoints);
	list_for_each_entry(pos, &tracepoint_module_list, list) {
		if (pos->tracepoints_ptrs == mod->tracepoints_ptrs) {
			list_del(&pos->list);
			kfree(pos);
=======
	 * Staging, out-of-tree, unsigned GPL, and test modules are fine.
	 */
	if (trace_module_has_bad_taint(mod))
		return 0;

	tp_mod = kmalloc(sizeof(struct tp_module), GFP_KERNEL);
	if (!tp_mod)
		return -ENOMEM;
	tp_mod->mod = mod;

	mutex_lock(&tracepoint_module_list_mutex);
	list_add_tail(&tp_mod->list, &tracepoint_module_list);
	blocking_notifier_call_chain(&tracepoint_notify_list,
			MODULE_STATE_COMING, tp_mod);
	mutex_unlock(&tracepoint_module_list_mutex);
	return 0;
}

static void tracepoint_module_going(struct module *mod)
{
	struct tp_module *tp_mod;

	if (!mod->num_tracepoints)
		return;

	mutex_lock(&tracepoint_module_list_mutex);
	list_for_each_entry(tp_mod, &tracepoint_module_list, list) {
		if (tp_mod->mod == mod) {
			blocking_notifier_call_chain(&tracepoint_notify_list,
					MODULE_STATE_GOING, tp_mod);
			list_del(&tp_mod->list);
			kfree(tp_mod);
			/*
			 * Called the going notifier before checking for
			 * quiescence.
			 */
			for_each_tracepoint_range(mod->tracepoints_ptrs,
				mod->tracepoints_ptrs + mod->num_tracepoints,
				tp_module_going_check_quiescent, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	/*
	 * In the case of modules that were tainted at "coming", we'll simply
	 * walk through the list without finding it. We cannot use the "tainted"
	 * flag on "going", in case a module taints the kernel only after being
	 * loaded.
	 */
<<<<<<< HEAD
	mutex_unlock(&tracepoints_mutex);
	return 0;
}

int tracepoint_module_notify(struct notifier_block *self,
			     unsigned long val, void *data)
=======
	mutex_unlock(&tracepoint_module_list_mutex);
}

static int tracepoint_module_notify(struct notifier_block *self,
		unsigned long val, void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct module *mod = data;
	int ret = 0;

	switch (val) {
	case MODULE_STATE_COMING:
		ret = tracepoint_module_coming(mod);
		break;
	case MODULE_STATE_LIVE:
		break;
	case MODULE_STATE_GOING:
<<<<<<< HEAD
		ret = tracepoint_module_going(mod);
		break;
	}
	return ret;
}

struct notifier_block tracepoint_module_nb = {
=======
		tracepoint_module_going(mod);
		break;
	case MODULE_STATE_UNFORMED:
		break;
	}
	return notifier_from_errno(ret);
}

static struct notifier_block tracepoint_module_nb = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.notifier_call = tracepoint_module_notify,
	.priority = 0,
};

<<<<<<< HEAD
static int init_tracepoints(void)
{
	return register_module_notifier(&tracepoint_module_nb);
=======
static __init int init_tracepoints(void)
{
	int ret;

	ret = register_module_notifier(&tracepoint_module_nb);
	if (ret)
		pr_warn("Failed to register tracepoint module enter notifier\n");

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
__initcall(init_tracepoints);
#endif /* CONFIG_MODULES */

<<<<<<< HEAD
=======
/**
 * for_each_kernel_tracepoint - iteration on all kernel tracepoints
 * @fct: callback
 * @priv: private data
 */
void for_each_kernel_tracepoint(void (*fct)(struct tracepoint *tp, void *priv),
		void *priv)
{
	for_each_tracepoint_range(__start___tracepoints_ptrs,
		__stop___tracepoints_ptrs, fct, priv);
}
EXPORT_SYMBOL_GPL(for_each_kernel_tracepoint);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_HAVE_SYSCALL_TRACEPOINTS

/* NB: reg/unreg are called while guarded with the tracepoints_mutex */
static int sys_tracepoint_refcount;

<<<<<<< HEAD
void syscall_regfunc(void)
{
	unsigned long flags;
	struct task_struct *g, *t;

	if (!sys_tracepoint_refcount) {
		read_lock_irqsave(&tasklist_lock, flags);
		do_each_thread(g, t) {
			/* Skip kernel threads. */
			if (t->mm)
				set_tsk_thread_flag(t, TIF_SYSCALL_TRACEPOINT);
		} while_each_thread(g, t);
		read_unlock_irqrestore(&tasklist_lock, flags);
	}
	sys_tracepoint_refcount++;
=======
int syscall_regfunc(void)
{
	struct task_struct *p, *t;

	if (!sys_tracepoint_refcount) {
		read_lock(&tasklist_lock);
		for_each_process_thread(p, t) {
			set_task_syscall_work(t, SYSCALL_TRACEPOINT);
		}
		read_unlock(&tasklist_lock);
	}
	sys_tracepoint_refcount++;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void syscall_unregfunc(void)
{
<<<<<<< HEAD
	unsigned long flags;
	struct task_struct *g, *t;

	sys_tracepoint_refcount--;
	if (!sys_tracepoint_refcount) {
		read_lock_irqsave(&tasklist_lock, flags);
		do_each_thread(g, t) {
			clear_tsk_thread_flag(t, TIF_SYSCALL_TRACEPOINT);
		} while_each_thread(g, t);
		read_unlock_irqrestore(&tasklist_lock, flags);
=======
	struct task_struct *p, *t;

	sys_tracepoint_refcount--;
	if (!sys_tracepoint_refcount) {
		read_lock(&tasklist_lock);
		for_each_process_thread(p, t) {
			clear_task_syscall_work(t, SYSCALL_TRACEPOINT);
		}
		read_unlock(&tasklist_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
#endif
