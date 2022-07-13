<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Lock-less NULL terminated single linked list
 *
 * The basic atomic operation of this list is cmpxchg on long.  On
 * architectures that don't have NMI-safe cmpxchg implementation, the
 * list can NOT be used in NMI handlers.  So code that uses the list in
 * an NMI handler should depend on CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG.
 *
 * Copyright 2010,2011 Intel Corp.
 *   Author: Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/interrupt.h>
=======
 */
#include <linux/kernel.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/llist.h>


/**
 * llist_add_batch - add several linked entries in batch
 * @new_first:	first entry in batch to be added
 * @new_last:	last entry in batch to be added
 * @head:	the head for your lock-less list
 *
 * Return whether list is empty before adding.
 */
bool llist_add_batch(struct llist_node *new_first, struct llist_node *new_last,
		     struct llist_head *head)
{
<<<<<<< HEAD
	struct llist_node *entry, *old_entry;

	entry = head->first;
	for (;;) {
		old_entry = entry;
		new_last->next = entry;
		entry = cmpxchg(&head->first, old_entry, new_first);
		if (entry == old_entry)
			break;
	}

	return old_entry == NULL;
=======
	struct llist_node *first = READ_ONCE(head->first);

	do {
		new_last->next = first;
	} while (!try_cmpxchg(&head->first, &first, new_first));

	return !first;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(llist_add_batch);

/**
 * llist_del_first - delete the first entry of lock-less list
 * @head:	the head for your lock-less list
 *
 * If list is empty, return NULL, otherwise, return the first entry
 * deleted, this is the newest added one.
 *
 * Only one llist_del_first user can be used simultaneously with
 * multiple llist_add users without lock.  Because otherwise
 * llist_del_first, llist_add, llist_add (or llist_del_all, llist_add,
 * llist_add) sequence in another user may change @head->first->next,
 * but keep @head->first.  If multiple consumers are needed, please
 * use llist_del_all or use lock between consumers.
 */
struct llist_node *llist_del_first(struct llist_head *head)
{
<<<<<<< HEAD
	struct llist_node *entry, *old_entry, *next;

	entry = head->first;
	for (;;) {
		if (entry == NULL)
			return NULL;
		old_entry = entry;
		next = entry->next;
		entry = cmpxchg(&head->first, old_entry, next);
		if (entry == old_entry)
			break;
	}
=======
	struct llist_node *entry, *next;

	entry = smp_load_acquire(&head->first);
	do {
		if (entry == NULL)
			return NULL;
		next = READ_ONCE(entry->next);
	} while (!try_cmpxchg(&head->first, &entry, next));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return entry;
}
EXPORT_SYMBOL_GPL(llist_del_first);
<<<<<<< HEAD
=======

/**
 * llist_del_first_this - delete given entry of lock-less list if it is first
 * @head:	the head for your lock-less list
 * @this:	a list entry.
 *
 * If head of the list is given entry, delete and return %true else
 * return %false.
 *
 * Multiple callers can safely call this concurrently with multiple
 * llist_add() callers, providing all the callers offer a different @this.
 */
bool llist_del_first_this(struct llist_head *head,
			  struct llist_node *this)
{
	struct llist_node *entry, *next;

	/* acquire ensures orderig wrt try_cmpxchg() is llist_del_first() */
	entry = smp_load_acquire(&head->first);
	do {
		if (entry != this)
			return false;
		next = READ_ONCE(entry->next);
	} while (!try_cmpxchg(&head->first, &entry, next));

	return true;
}
EXPORT_SYMBOL_GPL(llist_del_first_this);

/**
 * llist_reverse_order - reverse order of a llist chain
 * @head:	first item of the list to be reversed
 *
 * Reverse the order of a chain of llist entries and return the
 * new first entry.
 */
struct llist_node *llist_reverse_order(struct llist_node *head)
{
	struct llist_node *new_head = NULL;

	while (head) {
		struct llist_node *tmp = head;
		head = head->next;
		tmp->next = new_head;
		new_head = tmp;
	}

	return new_head;
}
EXPORT_SYMBOL_GPL(llist_reverse_order);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
