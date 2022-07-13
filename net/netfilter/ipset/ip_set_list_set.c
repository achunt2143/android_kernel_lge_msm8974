<<<<<<< HEAD
/* Copyright (C) 2008-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (C) 2008-2013 Jozsef Kadlecsik <kadlec@netfilter.org> */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Kernel module implementing an IP set type: the list:set type */

#include <linux/module.h>
#include <linux/ip.h>
<<<<<<< HEAD
=======
#include <linux/rculist.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/skbuff.h>
#include <linux/errno.h>

#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("list:set type of IP sets");
MODULE_ALIAS("ip_set_list:set");

/* Member elements without and with timeout */
struct set_elem {
	ip_set_id_t id;
};

struct set_telem {
	ip_set_id_t id;
	unsigned long timeout;
=======
#include <linux/netfilter/ipset/ip_set_list.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    Counters support added */
/*				2    Comments support added */
#define IPSET_TYPE_REV_MAX	3 /* skbinfo support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@netfilter.org>");
IP_SET_MODULE_DESC("list:set", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_list:set");

/* Member elements  */
struct set_elem {
	struct rcu_head rcu;
	struct list_head list;
	struct ip_set *set;	/* Sigh, in order to cleanup reference */
	ip_set_id_t id;
} __aligned(__alignof__(u64));

struct set_adt_elem {
	ip_set_id_t id;
	ip_set_id_t refid;
	int before;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Type structure */
struct list_set {
<<<<<<< HEAD
	size_t dsize;		/* element size */
	u32 size;		/* size of set list array */
	u32 timeout;		/* timeout value */
	struct timer_list gc;	/* garbage collection */
	struct set_elem members[0]; /* the set members */
};

static inline struct set_elem *
list_set_elem(const struct list_set *map, u32 id)
{
	return (struct set_elem *)((void *)map->members + id * map->dsize);
}

static inline struct set_telem *
list_set_telem(const struct list_set *map, u32 id)
{
	return (struct set_telem *)((void *)map->members + id * map->dsize);
}

static inline bool
list_set_timeout(const struct list_set *map, u32 id)
{
	const struct set_telem *elem = list_set_telem(map, id);

	return ip_set_timeout_test(elem->timeout);
}

static inline bool
list_set_expired(const struct list_set *map, u32 id)
{
	const struct set_telem *elem = list_set_telem(map, id);

	return ip_set_timeout_expired(elem->timeout);
}

/* Set list without and with timeout */

static int
list_set_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      enum ipset_adt adt, const struct ip_set_adt_opt *opt)
{
	struct list_set *map = set->data;
	struct set_elem *elem;
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		elem = list_set_elem(map, i);
		if (elem->id == IPSET_INVALID_ID)
			return 0;
		if (with_timeout(map->timeout) && list_set_expired(map, i))
			continue;
		switch (adt) {
		case IPSET_TEST:
			ret = ip_set_test(elem->id, skb, par, opt);
			if (ret > 0)
				return ret;
			break;
		case IPSET_ADD:
			ret = ip_set_add(elem->id, skb, par, opt);
			if (ret == 0)
				return ret;
			break;
		case IPSET_DEL:
			ret = ip_set_del(elem->id, skb, par, opt);
			if (ret == 0)
				return ret;
			break;
		default:
			break;
		}
	}
	return -EINVAL;
}

static bool
id_eq(const struct list_set *map, u32 i, ip_set_id_t id)
{
	const struct set_elem *elem;

	if (i < map->size) {
		elem = list_set_elem(map, i);
		return elem->id == id;
	}

	return 0;
}

static bool
id_eq_timeout(const struct list_set *map, u32 i, ip_set_id_t id)
{
	const struct set_elem *elem;

	if (i < map->size) {
		elem = list_set_elem(map, i);
		return !!(elem->id == id &&
			  !(with_timeout(map->timeout) &&
			    list_set_expired(map, i)));
	}

=======
	u32 size;		/* size of set list array */
	struct timer_list gc;	/* garbage collection */
	struct ip_set *set;	/* attached to this ip_set */
	struct net *net;	/* namespace */
	struct list_head members; /* the set members */
};

static int
list_set_ktest(struct ip_set *set, const struct sk_buff *skb,
	       const struct xt_action_param *par,
	       struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct ip_set_ext *mext = &opt->ext;
	struct set_elem *e;
	u32 flags = opt->cmdflags;
	int ret;

	/* Don't lookup sub-counters at all */
	opt->cmdflags &= ~IPSET_FLAG_MATCH_COUNTERS;
	if (opt->cmdflags & IPSET_FLAG_SKIP_SUBCOUNTER_UPDATE)
		opt->cmdflags |= IPSET_FLAG_SKIP_COUNTER_UPDATE;
	list_for_each_entry_rcu(e, &map->members, list) {
		ret = ip_set_test(e->id, skb, par, opt);
		if (ret <= 0)
			continue;
		if (ip_set_match_extensions(set, ext, mext, flags, e))
			return 1;
	}
	return 0;
}

static int
list_set_kadd(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		ret = ip_set_add(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kdel(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		ret = ip_set_del(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	int ret = -EINVAL;

	rcu_read_lock();
	switch (adt) {
	case IPSET_TEST:
		ret = list_set_ktest(set, skb, par, opt, &ext);
		break;
	case IPSET_ADD:
		ret = list_set_kadd(set, skb, par, opt, &ext);
		break;
	case IPSET_DEL:
		ret = list_set_kdel(set, skb, par, opt, &ext);
		break;
	default:
		break;
	}
	rcu_read_unlock();

	return ret;
}

/* Userspace interfaces: we are protected by the nfnl mutex */

static void
__list_set_del_rcu(struct rcu_head * rcu)
{
	struct set_elem *e = container_of(rcu, struct set_elem, rcu);
	struct ip_set *set = e->set;

	ip_set_ext_destroy(set, e);
	kfree(e);
}

static void
list_set_del(struct ip_set *set, struct set_elem *e)
{
	struct list_set *map = set->data;

	set->elements--;
	list_del_rcu(&e->list);
	ip_set_put_byindex(map->net, e->id);
	call_rcu(&e->rcu, __list_set_del_rcu);
}

static void
list_set_replace(struct ip_set *set, struct set_elem *e, struct set_elem *old)
{
	struct list_set *map = set->data;

	list_replace_rcu(&old->list, &e->list);
	ip_set_put_byindex(map->net, old->id);
	call_rcu(&old->rcu, __list_set_del_rcu);
}

static void
set_cleanup_entries(struct ip_set *set)
{
	struct list_set *map = set->data;
	struct set_elem *e, *n;

	list_for_each_entry_safe(e, n, &map->members, list)
		if (ip_set_timeout_expired(ext_timeout(e, set)))
			list_set_del(set, e);
}

static int
list_set_utest(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	       struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e, *next, *prev = NULL;
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id) {
			prev = e;
			continue;
		}

		if (d->before == 0) {
			ret = 1;
		} else if (d->before > 0) {
			next = list_next_entry(e, list);
			ret = !list_is_last(&e->list, &map->members) &&
			      next->id == d->refid;
		} else {
			ret = prev && prev->id == d->refid;
		}
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void
<<<<<<< HEAD
list_elem_add(struct list_set *map, u32 i, ip_set_id_t id)
{
	struct set_elem *e;

	for (; i < map->size; i++) {
		e = list_set_elem(map, i);
		swap(e->id, id);
		if (e->id == IPSET_INVALID_ID)
			break;
	}
}

static void
list_elem_tadd(struct list_set *map, u32 i, ip_set_id_t id,
	       unsigned long timeout)
{
	struct set_telem *e;

	for (; i < map->size; i++) {
		e = list_set_telem(map, i);
		swap(e->id, id);
		swap(e->timeout, timeout);
		if (e->id == IPSET_INVALID_ID)
			break;
	}
}

static int
list_set_add(struct list_set *map, u32 i, ip_set_id_t id,
	     unsigned long timeout)
{
	const struct set_elem *e = list_set_elem(map, i);

	if (i == map->size - 1 && e->id != IPSET_INVALID_ID)
		/* Last element replaced: e.g. add new,before,last */
		ip_set_put_byindex(e->id);
	if (with_timeout(map->timeout))
		list_elem_tadd(map, i, id, ip_set_timeout_set(timeout));
	else
		list_elem_add(map, i, id);
=======
list_set_init_extensions(struct ip_set *set, const struct ip_set_ext *ext,
			 struct set_elem *e)
{
	if (SET_WITH_COUNTER(set))
		ip_set_init_counter(ext_counter(e, set), ext);
	if (SET_WITH_COMMENT(set))
		ip_set_init_comment(set, ext_comment(e, set), ext);
	if (SET_WITH_SKBINFO(set))
		ip_set_init_skbinfo(ext_skbinfo(e, set), ext);
	/* Update timeout last */
	if (SET_WITH_TIMEOUT(set))
		ip_set_timeout_set(ext_timeout(e, set), ext->timeout);
}

static int
list_set_uadd(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e, *n, *prev, *next;
	bool flag_exist = flags & IPSET_FLAG_EXIST;

	/* Find where to add the new entry */
	n = prev = next = NULL;
	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (d->id == e->id)
			n = e;
		else if (d->before == 0 || e->id != d->refid)
			continue;
		else if (d->before > 0)
			next = e;
		else
			prev = e;
	}

	/* If before/after is used on an empty set */
	if ((d->before > 0 && !next) ||
	    (d->before < 0 && !prev))
		return -IPSET_ERR_REF_EXIST;

	/* Re-add already existing element */
	if (n) {
		if (!flag_exist)
			return -IPSET_ERR_EXIST;
		/* Update extensions */
		ip_set_ext_destroy(set, n);
		list_set_init_extensions(set, ext, n);

		/* Set is already added to the list */
		ip_set_put_byindex(map->net, d->id);
		return 0;
	}
	/* Add new entry */
	if (d->before == 0) {
		/* Append  */
		n = list_empty(&map->members) ? NULL :
		    list_last_entry(&map->members, struct set_elem, list);
	} else if (d->before > 0) {
		/* Insert after next element */
		if (!list_is_last(&next->list, &map->members))
			n = list_next_entry(next, list);
	} else {
		/* Insert before prev element */
		if (prev->list.prev != &map->members)
			n = list_prev_entry(prev, list);
	}
	/* Can we replace a timed out entry? */
	if (n &&
	    !(SET_WITH_TIMEOUT(set) &&
	      ip_set_timeout_expired(ext_timeout(n, set))))
		n = NULL;

	e = kzalloc(set->dsize, GFP_ATOMIC);
	if (!e)
		return -ENOMEM;
	e->id = d->id;
	e->set = set;
	INIT_LIST_HEAD(&e->list);
	list_set_init_extensions(set, ext, e);
	if (n)
		list_set_replace(set, e, n);
	else if (next)
		list_add_tail_rcu(&e->list, &next->list);
	else if (prev)
		list_add_rcu(&e->list, &prev->list);
	else
		list_add_tail_rcu(&e->list, &map->members);
	set->elements++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int
<<<<<<< HEAD
list_set_del(struct list_set *map, u32 i)
{
	struct set_elem *a = list_set_elem(map, i), *b;

	ip_set_put_byindex(a->id);

	for (; i < map->size - 1; i++) {
		b = list_set_elem(map, i + 1);
		a->id = b->id;
		if (with_timeout(map->timeout))
			((struct set_telem *)a)->timeout =
				((struct set_telem *)b)->timeout;
		a = b;
		if (a->id == IPSET_INVALID_ID)
			break;
	}
	/* Last element */
	a->id = IPSET_INVALID_ID;
	return 0;
}

static void
cleanup_entries(struct list_set *map)
{
	struct set_telem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_telem(map, i);
		if (e->id != IPSET_INVALID_ID && list_set_expired(map, i))
			list_set_del(map, i);
	}
=======
list_set_udel(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e, *next, *prev = NULL;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id) {
			prev = e;
			continue;
		}

		if (d->before > 0) {
			next = list_next_entry(e, list);
			if (list_is_last(&e->list, &map->members) ||
			    next->id != d->refid)
				return -IPSET_ERR_REF_EXIST;
		} else if (d->before < 0) {
			if (!prev || prev->id != d->refid)
				return -IPSET_ERR_REF_EXIST;
		}
		list_set_del(set, e);
		return 0;
	}
	return d->before != 0 ? -IPSET_ERR_REF_EXIST : -IPSET_ERR_EXIST;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
list_set_uadt(struct ip_set *set, struct nlattr *tb[],
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	bool with_timeout = with_timeout(map->timeout);
	bool flag_exist = flags & IPSET_FLAG_EXIST;
	int before = 0;
	u32 timeout = map->timeout;
	ip_set_id_t id, refid = IPSET_INVALID_ID;
	const struct set_elem *elem;
	struct ip_set *s;
	u32 i;
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_NAME] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	id = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAME]), &s);
	if (id == IPSET_INVALID_ID)
=======
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct set_adt_elem e = { .refid = IPSET_INVALID_ID };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	struct ip_set *s;
	int ret = 0;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	if (unlikely(!tb[IPSET_ATTR_NAME] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
		return -IPSET_ERR_PROTOCOL;

	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;
	e.id = ip_set_get_byname(map->net, nla_data(tb[IPSET_ATTR_NAME]), &s);
	if (e.id == IPSET_INVALID_ID)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -IPSET_ERR_NAME;
	/* "Loop detection" */
	if (s->type->features & IPSET_TYPE_NAME) {
		ret = -IPSET_ERR_LOOP;
		goto finish;
	}

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 f = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
<<<<<<< HEAD
		before = f & IPSET_FLAG_BEFORE;
	}

	if (before && !tb[IPSET_ATTR_NAMEREF]) {
=======

		e.before = f & IPSET_FLAG_BEFORE;
	}

	if (e.before && !tb[IPSET_ATTR_NAMEREF]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -IPSET_ERR_BEFORE;
		goto finish;
	}

	if (tb[IPSET_ATTR_NAMEREF]) {
<<<<<<< HEAD
		refid = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAMEREF]),
					  &s);
		if (refid == IPSET_INVALID_ID) {
			ret = -IPSET_ERR_NAMEREF;
			goto finish;
		}
		if (!before)
			before = -1;
	}
	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout) {
			ret = -IPSET_ERR_TIMEOUT;
			goto finish;
		}
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}
	if (with_timeout && adt != IPSET_TEST)
		cleanup_entries(map);

	switch (adt) {
	case IPSET_TEST:
		for (i = 0; i < map->size && !ret; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID ||
			    (before != 0 && i + 1 >= map->size))
				break;
			else if (with_timeout && list_set_expired(map, i))
				continue;
			else if (before > 0 && elem->id == id)
				ret = id_eq_timeout(map, i + 1, refid);
			else if (before < 0 && elem->id == refid)
				ret = id_eq_timeout(map, i + 1, id);
			else if (before == 0 && elem->id == id)
				ret = 1;
		}
		break;
	case IPSET_ADD:
		for (i = 0; i < map->size; i++) {
			elem = list_set_elem(map, i);
			if (elem->id != id)
				continue;
			if (!(with_timeout && flag_exist)) {
				ret = -IPSET_ERR_EXIST;
				goto finish;
			} else {
				struct set_telem *e = list_set_telem(map, i);

				if ((before > 1 &&
				     !id_eq(map, i + 1, refid)) ||
				    (before < 0 &&
				     (i == 0 || !id_eq(map, i - 1, refid)))) {
					ret = -IPSET_ERR_EXIST;
					goto finish;
				}
				e->timeout = ip_set_timeout_set(timeout);
				ip_set_put_byindex(id);
				ret = 0;
				goto finish;
			}
		}
		ret = -IPSET_ERR_LIST_FULL;
		for (i = 0; i < map->size && ret == -IPSET_ERR_LIST_FULL; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID)
				ret = before != 0 ? -IPSET_ERR_REF_EXIST
					: list_set_add(map, i, id, timeout);
			else if (elem->id != refid)
				continue;
			else if (before > 0)
				ret = list_set_add(map, i, id, timeout);
			else if (i + 1 < map->size)
				ret = list_set_add(map, i + 1, id, timeout);
		}
		break;
	case IPSET_DEL:
		ret = -IPSET_ERR_EXIST;
		for (i = 0; i < map->size && ret == -IPSET_ERR_EXIST; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID) {
				ret = before != 0 ? -IPSET_ERR_REF_EXIST
						  : -IPSET_ERR_EXIST;
				break;
			} else if (elem->id == id &&
				   (before == 0 ||
				    (before > 0 && id_eq(map, i + 1, refid))))
				ret = list_set_del(map, i);
			else if (elem->id == refid &&
				 before < 0 && id_eq(map, i + 1, id))
				ret = list_set_del(map, i + 1);
		}
		break;
	default:
		break;
	}

finish:
	if (refid != IPSET_INVALID_ID)
		ip_set_put_byindex(refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(id);
=======
		e.refid = ip_set_get_byname(map->net,
					    nla_data(tb[IPSET_ATTR_NAMEREF]),
					    &s);
		if (e.refid == IPSET_INVALID_ID) {
			ret = -IPSET_ERR_NAMEREF;
			goto finish;
		}
		if (!e.before)
			e.before = -1;
	}
	if (adt != IPSET_TEST && SET_WITH_TIMEOUT(set))
		set_cleanup_entries(set);

	ret = adtfn(set, &e, &ext, &ext, flags);

finish:
	if (e.refid != IPSET_INVALID_ID)
		ip_set_put_byindex(map->net, e.refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(map->net, e.id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

static void
list_set_flush(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	struct set_elem *elem;
	u32 i;

	for (i = 0; i < map->size; i++) {
		elem = list_set_elem(map, i);
		if (elem->id != IPSET_INVALID_ID) {
			ip_set_put_byindex(elem->id);
			elem->id = IPSET_INVALID_ID;
		}
	}
=======
	struct set_elem *e, *n;

	list_for_each_entry_safe(e, n, &map->members, list)
		list_set_del(set, e);
	set->elements = 0;
	set->ext_size = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
list_set_destroy(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD

	if (with_timeout(map->timeout))
		del_timer_sync(&map->gc);
	list_set_flush(set);
=======
	struct set_elem *e, *n;

	list_for_each_entry_safe(e, n, &map->members, list) {
		list_del(&e->list);
		ip_set_put_byindex(map->net, e->id);
		ip_set_ext_destroy(set, e);
		kfree(e);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(map);

	set->data = NULL;
}

<<<<<<< HEAD
=======
/* Calculate the actual memory size of the set data */
static size_t
list_set_memsize(const struct list_set *map, size_t dsize)
{
	struct set_elem *e;
	u32 n = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(e, &map->members, list)
		n++;
	rcu_read_unlock();

	return (sizeof(*map) + n * dsize);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int
list_set_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct list_set *map = set->data;
	struct nlattr *nested;
<<<<<<< HEAD

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	NLA_PUT_NET32(skb, IPSET_ATTR_SIZE, htonl(map->size));
	if (with_timeout(map->timeout))
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT, htonl(map->timeout));
	NLA_PUT_NET32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1));
	NLA_PUT_NET32(skb, IPSET_ATTR_MEMSIZE,
		      htonl(sizeof(*map) + map->size * map->dsize));
	ipset_nest_end(skb, nested);
=======
	size_t memsize = list_set_memsize(map, set->dsize) + set->ext_size;

	nested = nla_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	if (nla_put_net32(skb, IPSET_ATTR_SIZE, htonl(map->size)) ||
	    nla_put_net32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref)) ||
	    nla_put_net32(skb, IPSET_ATTR_MEMSIZE, htonl(memsize)) ||
	    nla_put_net32(skb, IPSET_ATTR_ELEMENTS, htonl(set->elements)))
		goto nla_put_failure;
	if (unlikely(ip_set_put_flags(skb, set)))
		goto nla_put_failure;
	nla_nest_end(skb, nested);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
nla_put_failure:
	return -EMSGSIZE;
}

static int
list_set_list(const struct ip_set *set,
	      struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct list_set *map = set->data;
	struct nlattr *atd, *nested;
<<<<<<< HEAD
	u32 i, first = cb->args[2];
	const struct set_elem *e;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
	for (; cb->args[2] < map->size; cb->args[2]++) {
		i = cb->args[2];
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			goto finish;
		if (with_timeout(map->timeout) && list_set_expired(map, i))
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (i == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		NLA_PUT_STRING(skb, IPSET_ATTR_NAME,
			       ip_set_name_byindex(e->id));
		if (with_timeout(map->timeout)) {
			const struct set_telem *te =
				(const struct set_telem *) e;
			NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
				      htonl(ip_set_timeout_get(te->timeout)));
		}
		ipset_nest_end(skb, nested);
	}
finish:
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	ipset_nest_end(skb, atd);
	if (unlikely(i == first)) {
		cb->args[2] = 0;
		return -EMSGSIZE;
	}
	return 0;
=======
	u32 i = 0, first = cb->args[IPSET_CB_ARG0];
	char name[IPSET_MAXNAMELEN];
	struct set_elem *e;
	int ret = 0;

	atd = nla_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;

	rcu_read_lock();
	list_for_each_entry_rcu(e, &map->members, list) {
		if (i < first ||
		    (SET_WITH_TIMEOUT(set) &&
		     ip_set_timeout_expired(ext_timeout(e, set)))) {
			i++;
			continue;
		}
		nested = nla_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested)
			goto nla_put_failure;
		ip_set_name_byindex(map->net, e->id, name);
		if (nla_put_string(skb, IPSET_ATTR_NAME, name))
			goto nla_put_failure;
		if (ip_set_put_extensions(skb, set, e, true))
			goto nla_put_failure;
		nla_nest_end(skb, nested);
		i++;
	}

	nla_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[IPSET_CB_ARG0] = 0;
	goto out;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	if (unlikely(i == first)) {
		nla_nest_cancel(skb, atd);
		cb->args[IPSET_CB_ARG0] = 0;
		ret = -EMSGSIZE;
	} else {
		cb->args[IPSET_CB_ARG0] = i;
		nla_nest_end(skb, atd);
	}
out:
	rcu_read_unlock();
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool
list_set_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct list_set *x = a->data;
	const struct list_set *y = b->data;

	return x->size == y->size &&
<<<<<<< HEAD
	       x->timeout == y->timeout;
}

static const struct ip_set_type_variant list_set = {
	.kadt	= list_set_kadt,
	.uadt	= list_set_uadt,
=======
	       a->timeout == b->timeout &&
	       a->extensions == b->extensions;
}

static void
list_set_cancel_gc(struct ip_set *set)
{
	struct list_set *map = set->data;

	if (SET_WITH_TIMEOUT(set))
		timer_shutdown_sync(&map->gc);
}

static const struct ip_set_type_variant set_variant = {
	.kadt	= list_set_kadt,
	.uadt	= list_set_uadt,
	.adt	= {
		[IPSET_ADD] = list_set_uadd,
		[IPSET_DEL] = list_set_udel,
		[IPSET_TEST] = list_set_utest,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.destroy = list_set_destroy,
	.flush	= list_set_flush,
	.head	= list_set_head,
	.list	= list_set_list,
	.same_set = list_set_same_set,
<<<<<<< HEAD
};

static void
list_set_gc(unsigned long ul_set)
{
	struct ip_set *set = (struct ip_set *) ul_set;
	struct list_set *map = set->data;

	write_lock_bh(&set->lock);
	cleanup_entries(map);
	write_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
=======
	.cancel_gc = list_set_cancel_gc,
};

static void
list_set_gc(struct timer_list *t)
{
	struct list_set *map = from_timer(map, t, gc);
	struct ip_set *set = map->set;

	spin_lock_bh(&set->lock);
	set_cleanup_entries(set);
	spin_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(set->timeout) * HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	add_timer(&map->gc);
}

static void
<<<<<<< HEAD
list_set_gc_init(struct ip_set *set)
{
	struct list_set *map = set->data;

	init_timer(&map->gc);
	map->gc.data = (unsigned long) set;
	map->gc.function = list_set_gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
	add_timer(&map->gc);
=======
list_set_gc_init(struct ip_set *set, void (*gc)(struct timer_list *t))
{
	struct list_set *map = set->data;

	timer_setup(&map->gc, gc, 0);
	mod_timer(&map->gc, jiffies + IPSET_GC_PERIOD(set->timeout) * HZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Create list:set type of sets */

static bool
<<<<<<< HEAD
init_list_set(struct ip_set *set, u32 size, size_t dsize,
	      unsigned long timeout)
{
	struct list_set *map;
	struct set_elem *e;
	u32 i;

	map = kzalloc(sizeof(*map) + size * dsize, GFP_KERNEL);
=======
init_list_set(struct net *net, struct ip_set *set, u32 size)
{
	struct list_set *map;

	map = kzalloc(sizeof(*map), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!map)
		return false;

	map->size = size;
<<<<<<< HEAD
	map->dsize = dsize;
	map->timeout = timeout;
	set->data = map;

	for (i = 0; i < size; i++) {
		e = list_set_elem(map, i);
		e->id = IPSET_INVALID_ID;
	}

=======
	map->net = net;
	map->set = set;
	INIT_LIST_HEAD(&map->members);
	set->data = map;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return true;
}

static int
<<<<<<< HEAD
list_set_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
=======
list_set_create(struct net *net, struct ip_set *set, struct nlattr *tb[],
		u32 flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 size = IP_SET_LIST_DEFAULT_SIZE;

	if (unlikely(!ip_set_optattr_netorder(tb, IPSET_ATTR_SIZE) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_SIZE])
		size = ip_set_get_h32(tb[IPSET_ATTR_SIZE]);
	if (size < IP_SET_LIST_MIN_SIZE)
		size = IP_SET_LIST_MIN_SIZE;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!init_list_set(set, size, sizeof(struct set_telem),
				   ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT])))
			return -ENOMEM;

		list_set_gc_init(set);
	} else {
		if (!init_list_set(set, size, sizeof(struct set_elem),
				   IPSET_NO_TIMEOUT))
			return -ENOMEM;
	}
	set->variant = &list_set;
=======
	set->variant = &set_variant;
	set->dsize = ip_set_elem_len(set, tb, sizeof(struct set_elem),
				     __alignof__(struct set_elem));
	if (!init_list_set(net, set, size))
		return -ENOMEM;
	if (tb[IPSET_ATTR_TIMEOUT]) {
		set->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		list_set_gc_init(set, list_set_gc);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static struct ip_set_type list_set_type __read_mostly = {
	.name		= "list:set",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_NAME | IPSET_DUMP_LAST,
	.dimension	= IPSET_DIM_ONE,
	.family		= NFPROTO_UNSPEC,
<<<<<<< HEAD
	.revision_min	= 0,
	.revision_max	= 0,
=======
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.create		= list_set_create,
	.create_policy	= {
		[IPSET_ATTR_SIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.adt_policy	= {
		[IPSET_ATTR_NAME]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_NAMEREF]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING,
					    .len  = IPSET_MAX_COMMENT_SIZE },
		[IPSET_ATTR_SKBMARK]	= { .type = NLA_U64 },
		[IPSET_ATTR_SKBPRIO]	= { .type = NLA_U32 },
		[IPSET_ATTR_SKBQUEUE]	= { .type = NLA_U16 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.me		= THIS_MODULE,
};

static int __init
list_set_init(void)
{
	return ip_set_type_register(&list_set_type);
}

static void __exit
list_set_fini(void)
{
<<<<<<< HEAD
=======
	rcu_barrier();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ip_set_type_unregister(&list_set_type);
}

module_init(list_set_init);
module_exit(list_set_fini);
