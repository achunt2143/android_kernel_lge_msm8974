<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_TIMERQUEUE_H
#define _LINUX_TIMERQUEUE_H

#include <linux/rbtree.h>
<<<<<<< HEAD
#include <linux/ktime.h>


struct timerqueue_node {
	struct rb_node node;
	ktime_t expires;
};

struct timerqueue_head {
	struct rb_root head;
	struct timerqueue_node *next;
};


extern void timerqueue_add(struct timerqueue_head *head,
				struct timerqueue_node *node);
extern void timerqueue_del(struct timerqueue_head *head,
				struct timerqueue_node *node);
=======
#include <linux/timerqueue_types.h>

extern bool timerqueue_add(struct timerqueue_head *head,
			   struct timerqueue_node *node);
extern bool timerqueue_del(struct timerqueue_head *head,
			   struct timerqueue_node *node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct timerqueue_node *timerqueue_iterate_next(
						struct timerqueue_node *node);

/**
 * timerqueue_getnext - Returns the timer with the earliest expiration time
 *
 * @head: head of timerqueue
 *
<<<<<<< HEAD
 * Returns a pointer to the timer node that has the
 * earliest expiration time.
=======
 * Returns a pointer to the timer node that has the earliest expiration time.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static inline
struct timerqueue_node *timerqueue_getnext(struct timerqueue_head *head)
{
<<<<<<< HEAD
	return head->next;
=======
	struct rb_node *leftmost = rb_first_cached(&head->rb_root);

	return rb_entry_safe(leftmost, struct timerqueue_node, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void timerqueue_init(struct timerqueue_node *node)
{
<<<<<<< HEAD
	rb_init_node(&node->node);
=======
	RB_CLEAR_NODE(&node->node);
}

static inline bool timerqueue_node_queued(struct timerqueue_node *node)
{
	return !RB_EMPTY_NODE(&node->node);
}

static inline bool timerqueue_node_expires(struct timerqueue_node *node)
{
	return node->expires;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void timerqueue_init_head(struct timerqueue_head *head)
{
<<<<<<< HEAD
	head->head = RB_ROOT;
	head->next = NULL;
=======
	head->rb_root = RB_ROOT_CACHED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* _LINUX_TIMERQUEUE_H */
