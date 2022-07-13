<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * include/linux/journal-head.h
 *
 * buffer_head fields for JBD
 *
 * 27 May 2001 Andrew Morton
 *	Created - pulled out of fs.h
 */

#ifndef JOURNAL_HEAD_H_INCLUDED
#define JOURNAL_HEAD_H_INCLUDED

<<<<<<< HEAD
=======
#include <linux/spinlock.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef unsigned int		tid_t;		/* Unique transaction ID */
typedef struct transaction_s	transaction_t;	/* Compound transaction type */


struct buffer_head;

struct journal_head {
	/*
	 * Points back to our buffer_head. [jbd_lock_bh_journal_head()]
	 */
	struct buffer_head *b_bh;

	/*
<<<<<<< HEAD
=======
	 * Protect the buffer head state
	 */
	spinlock_t b_state_lock;

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Reference count - see description in journal.c
	 * [jbd_lock_bh_journal_head()]
	 */
	int b_jcount;

	/*
<<<<<<< HEAD
	 * Journalling list for this buffer [jbd_lock_bh_state()]
=======
	 * Journalling list for this buffer [b_state_lock]
	 * NOTE: We *cannot* combine this with b_modified into a bitfield
	 * as gcc would then (which the C standard allows but which is
	 * very unuseful) make 64-bit accesses to the bitfield and clobber
	 * b_jcount if its update races with bitfield modification.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	unsigned b_jlist;

	/*
	 * This flag signals the buffer has been modified by
	 * the currently running transaction
<<<<<<< HEAD
	 * [jbd_lock_bh_state()]
=======
	 * [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	unsigned b_modified;

	/*
<<<<<<< HEAD
	 * This feild tracks the last transaction id in which this buffer
	 * has been cowed
	 * [jbd_lock_bh_state()]
	 */
	tid_t b_cow_tid;

	/*
	 * Copy of the buffer data frozen for writing to the log.
	 * [jbd_lock_bh_state()]
=======
	 * Copy of the buffer data frozen for writing to the log.
	 * [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	char *b_frozen_data;

	/*
	 * Pointer to a saved copy of the buffer containing no uncommitted
	 * deallocation references, so that allocations can avoid overwriting
<<<<<<< HEAD
	 * uncommitted deletes. [jbd_lock_bh_state()]
=======
	 * uncommitted deletes. [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	char *b_committed_data;

	/*
	 * Pointer to the compound transaction which owns this buffer's
	 * metadata: either the running transaction or the committing
	 * transaction (if there is one).  Only applies to buffers on a
	 * transaction's data or metadata journaling list.
<<<<<<< HEAD
	 * [j_list_lock] [jbd_lock_bh_state()]
=======
	 * [j_list_lock] [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Either of these locks is enough for reading, both are needed for
	 * changes.
	 */
	transaction_t *b_transaction;

	/*
	 * Pointer to the running compound transaction which is currently
	 * modifying the buffer's metadata, if there was already a transaction
	 * committing it when the new transaction touched it.
<<<<<<< HEAD
	 * [t_list_lock] [jbd_lock_bh_state()]
=======
	 * [t_list_lock] [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	transaction_t *b_next_transaction;

	/*
	 * Doubly-linked list of buffers on a transaction's data, metadata or
<<<<<<< HEAD
	 * forget queue. [t_list_lock] [jbd_lock_bh_state()]
=======
	 * forget queue. [t_list_lock] [b_state_lock]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	struct journal_head *b_tnext, *b_tprev;

	/*
	 * Pointer to the compound transaction against which this buffer
	 * is checkpointed.  Only dirty buffers can be checkpointed.
	 * [j_list_lock]
	 */
	transaction_t *b_cp_transaction;

	/*
	 * Doubly-linked list of buffers still remaining to be flushed
	 * before an old transaction can be checkpointed.
	 * [j_list_lock]
	 */
	struct journal_head *b_cpnext, *b_cpprev;

	/* Trigger type */
	struct jbd2_buffer_trigger_type *b_triggers;

	/* Trigger type for the committing transaction's frozen data */
	struct jbd2_buffer_trigger_type *b_frozen_triggers;
};

#endif		/* JOURNAL_HEAD_H_INCLUDED */
