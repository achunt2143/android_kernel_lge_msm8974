<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/jbd2/checkpoint.c
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>, 1999
 *
 * Copyright 1999 Red Hat Software --- All Rights Reserved
 *
<<<<<<< HEAD
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Checkpoint routines for the generic filesystem journaling code.
 * Part of the ext2fs journaling system.
 *
 * Checkpointing is the process of ensuring that a section of the log is
 * committed fully to disk, so that that portion of the log can be
 * reused.
 */

#include <linux/time.h>
#include <linux/fs.h>
#include <linux/jbd2.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/blkdev.h>
#include <trace/events/jbd2.h>

/*
 * Unlink a buffer from a transaction checkpoint list.
 *
 * Called with j_list_lock held.
 */
<<<<<<< HEAD
static inline void __buffer_unlink_first(struct journal_head *jh)
=======
static inline void __buffer_unlink(struct journal_head *jh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	transaction_t *transaction = jh->b_cp_transaction;

	jh->b_cpnext->b_cpprev = jh->b_cpprev;
	jh->b_cpprev->b_cpnext = jh->b_cpnext;
	if (transaction->t_checkpoint_list == jh) {
		transaction->t_checkpoint_list = jh->b_cpnext;
		if (transaction->t_checkpoint_list == jh)
			transaction->t_checkpoint_list = NULL;
	}
}

/*
<<<<<<< HEAD
 * Unlink a buffer from a transaction checkpoint(io) list.
 *
 * Called with j_list_lock held.
 */
static inline void __buffer_unlink(struct journal_head *jh)
{
	transaction_t *transaction = jh->b_cp_transaction;

	__buffer_unlink_first(jh);
	if (transaction->t_checkpoint_io_list == jh) {
		transaction->t_checkpoint_io_list = jh->b_cpnext;
		if (transaction->t_checkpoint_io_list == jh)
			transaction->t_checkpoint_io_list = NULL;
	}
}

/*
 * Move a buffer from the checkpoint list to the checkpoint io list
 *
 * Called with j_list_lock held
 */
static inline void __buffer_relink_io(struct journal_head *jh)
{
	transaction_t *transaction = jh->b_cp_transaction;

	__buffer_unlink_first(jh);

	if (!transaction->t_checkpoint_io_list) {
		jh->b_cpnext = jh->b_cpprev = jh;
	} else {
		jh->b_cpnext = transaction->t_checkpoint_io_list;
		jh->b_cpprev = transaction->t_checkpoint_io_list->b_cpprev;
		jh->b_cpprev->b_cpnext = jh;
		jh->b_cpnext->b_cpprev = jh;
	}
	transaction->t_checkpoint_io_list = jh;
}

/*
 * Try to release a checkpointed buffer from its transaction.
 * Returns 1 if we released it and 2 if we also released the
 * whole transaction.
 *
 * Requires j_list_lock
 */
static int __try_to_free_cp_buf(struct journal_head *jh)
{
	int ret = 0;
	struct buffer_head *bh = jh2bh(jh);

	if (jh->b_transaction == NULL && !buffer_locked(bh) &&
	    !buffer_dirty(bh) && !buffer_write_io_error(bh)) {
		/*
		 * Get our reference so that bh cannot be freed before
		 * we unlock it
		 */
		get_bh(bh);
		JBUFFER_TRACE(jh, "remove from checkpoint list");
		ret = __jbd2_journal_remove_checkpoint(jh) + 1;
		BUFFER_TRACE(bh, "release");
		__brelse(bh);
	}
	return ret;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * __jbd2_log_wait_for_space: wait until there is space in the journal.
 *
 * Called under j-state_lock *only*.  It will be unlocked if we have to wait
 * for a checkpoint to free up some space in the log.
 */
void __jbd2_log_wait_for_space(journal_t *journal)
<<<<<<< HEAD
=======
__acquires(&journal->j_state_lock)
__releases(&journal->j_state_lock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int nblocks, space_left;
	/* assert_spin_locked(&journal->j_state_lock); */

<<<<<<< HEAD
	nblocks = jbd_space_needed(journal);
	while (__jbd2_log_space_left(journal) < nblocks) {
		if (journal->j_flags & JBD2_ABORT)
			return;
		write_unlock(&journal->j_state_lock);
		mutex_lock(&journal->j_checkpoint_mutex);
=======
	nblocks = journal->j_max_transaction_buffers;
	while (jbd2_log_space_left(journal) < nblocks) {
		write_unlock(&journal->j_state_lock);
		mutex_lock_io(&journal->j_checkpoint_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Test again, another process may have checkpointed while we
		 * were waiting for the checkpoint lock. If there are no
		 * transactions ready to be checkpointed, try to recover
		 * journal space by calling cleanup_journal_tail(), and if
		 * that doesn't work, by waiting for the currently committing
		 * transaction to complete.  If there is absolutely no way
		 * to make progress, this is either a BUG or corrupted
		 * filesystem, so abort the journal and leave a stack
		 * trace for forensic evidence.
		 */
		write_lock(&journal->j_state_lock);
<<<<<<< HEAD
		spin_lock(&journal->j_list_lock);
		nblocks = jbd_space_needed(journal);
		space_left = __jbd2_log_space_left(journal);
=======
		if (journal->j_flags & JBD2_ABORT) {
			mutex_unlock(&journal->j_checkpoint_mutex);
			return;
		}
		spin_lock(&journal->j_list_lock);
		space_left = jbd2_log_space_left(journal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (space_left < nblocks) {
			int chkpt = journal->j_checkpoint_transactions != NULL;
			tid_t tid = 0;

			if (journal->j_committing_transaction)
				tid = journal->j_committing_transaction->t_tid;
			spin_unlock(&journal->j_list_lock);
			write_unlock(&journal->j_state_lock);
			if (chkpt) {
				jbd2_log_do_checkpoint(journal);
			} else if (jbd2_cleanup_journal_tail(journal) == 0) {
				/* We were able to recover space; yay! */
				;
			} else if (tid) {
<<<<<<< HEAD
				jbd2_log_wait_commit(journal, tid);
=======
				/*
				 * jbd2_journal_commit_transaction() may want
				 * to take the checkpoint_mutex if JBD2_FLUSHED
				 * is set.  So we need to temporarily drop it.
				 */
				mutex_unlock(&journal->j_checkpoint_mutex);
				jbd2_log_wait_commit(journal, tid);
				write_lock(&journal->j_state_lock);
				continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else {
				printk(KERN_ERR "%s: needed %d blocks and "
				       "only had %d space available\n",
				       __func__, nblocks, space_left);
				printk(KERN_ERR "%s: no way to get more "
				       "journal space in %s\n", __func__,
				       journal->j_devname);
				WARN_ON(1);
<<<<<<< HEAD
				jbd2_journal_abort(journal, 0);
=======
				jbd2_journal_abort(journal, -EIO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			write_lock(&journal->j_state_lock);
		} else {
			spin_unlock(&journal->j_list_lock);
		}
		mutex_unlock(&journal->j_checkpoint_mutex);
	}
}

<<<<<<< HEAD
/*
 * Clean up transaction's list of buffers submitted for io.
 * We wait for any pending IO to complete and remove any clean
 * buffers. Note that we take the buffers in the opposite ordering
 * from the one in which they were submitted for IO.
 *
 * Return 0 on success, and return <0 if some buffers have failed
 * to be written out.
 *
 * Called with j_list_lock held.
 */
static int __wait_cp_io(journal_t *journal, transaction_t *transaction)
{
	struct journal_head *jh;
	struct buffer_head *bh;
	tid_t this_tid;
	int released = 0;
	int ret = 0;

	this_tid = transaction->t_tid;
restart:
	/* Did somebody clean up the transaction in the meanwhile? */
	if (journal->j_checkpoint_transactions != transaction ||
			transaction->t_tid != this_tid)
		return ret;
	while (!released && transaction->t_checkpoint_io_list) {
		jh = transaction->t_checkpoint_io_list;
		bh = jh2bh(jh);
		get_bh(bh);
		if (buffer_locked(bh)) {
			spin_unlock(&journal->j_list_lock);
			wait_on_buffer(bh);
			/* the journal_head may have gone by now */
			BUFFER_TRACE(bh, "brelse");
			__brelse(bh);
			spin_lock(&journal->j_list_lock);
			goto restart;
		}
		if (unlikely(buffer_write_io_error(bh)))
			ret = -EIO;

		/*
		 * Now in whatever state the buffer currently is, we know that
		 * it has been written out and so we can drop it from the list
		 */
		released = __jbd2_journal_remove_checkpoint(jh);
		__brelse(bh);
	}

	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void
__flush_batch(journal_t *journal, int *batch_count)
{
	int i;
	struct blk_plug plug;

	blk_start_plug(&plug);
	for (i = 0; i < *batch_count; i++)
<<<<<<< HEAD
		write_dirty_buffer(journal->j_chkpt_bhs[i], WRITE_SYNC);
=======
		write_dirty_buffer(journal->j_chkpt_bhs[i], REQ_SYNC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	blk_finish_plug(&plug);

	for (i = 0; i < *batch_count; i++) {
		struct buffer_head *bh = journal->j_chkpt_bhs[i];
		BUFFER_TRACE(bh, "brelse");
		__brelse(bh);
<<<<<<< HEAD
=======
		journal->j_chkpt_bhs[i] = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	*batch_count = 0;
}

/*
<<<<<<< HEAD
 * Try to flush one buffer from the checkpoint list to disk.
 *
 * Return 1 if something happened which requires us to abort the current
 * scan of the checkpoint list.  Return <0 if the buffer has failed to
 * be written out.
 *
 * Called with j_list_lock held and drops it if 1 is returned
 */
static int __process_buffer(journal_t *journal, struct journal_head *jh,
			    int *batch_count, transaction_t *transaction)
{
	struct buffer_head *bh = jh2bh(jh);
	int ret = 0;

	if (buffer_locked(bh)) {
		get_bh(bh);
		spin_unlock(&journal->j_list_lock);
		wait_on_buffer(bh);
		/* the journal_head may have gone by now */
		BUFFER_TRACE(bh, "brelse");
		__brelse(bh);
		ret = 1;
	} else if (jh->b_transaction != NULL) {
		transaction_t *t = jh->b_transaction;
		tid_t tid = t->t_tid;

		transaction->t_chp_stats.cs_forced_to_close++;
		spin_unlock(&journal->j_list_lock);
		if (unlikely(journal->j_flags & JBD2_UNMOUNT))
			/*
			 * The journal thread is dead; so starting and
			 * waiting for a commit to finish will cause
			 * us to wait for a _very_ long time.
			 */
			printk(KERN_ERR "JBD2: %s: "
			       "Waiting for Godot: block %llu\n",
			       journal->j_devname,
			       (unsigned long long) bh->b_blocknr);
		jbd2_log_start_commit(journal, tid);
		jbd2_log_wait_commit(journal, tid);
		ret = 1;
	} else if (!buffer_dirty(bh)) {
		ret = 1;
		if (unlikely(buffer_write_io_error(bh)))
			ret = -EIO;
		get_bh(bh);
		BUFFER_TRACE(bh, "remove from checkpoint");
		__jbd2_journal_remove_checkpoint(jh);
		spin_unlock(&journal->j_list_lock);
		__brelse(bh);
	} else {
		/*
		 * Important: we are about to write the buffer, and
		 * possibly block, while still holding the journal lock.
		 * We cannot afford to let the transaction logic start
		 * messing around with this buffer before we write it to
		 * disk, as that would break recoverability.
		 */
		BUFFER_TRACE(bh, "queue");
		get_bh(bh);
		J_ASSERT_BH(bh, !buffer_jwrite(bh));
		journal->j_chkpt_bhs[*batch_count] = bh;
		__buffer_relink_io(jh);
		transaction->t_chp_stats.cs_written++;
		(*batch_count)++;
		if (*batch_count == JBD2_NR_BATCH) {
			spin_unlock(&journal->j_list_lock);
			__flush_batch(journal, batch_count);
			ret = 1;
		}
	}
	return ret;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Perform an actual checkpoint. We take the first transaction on the
 * list of transactions to be checkpointed and send all its buffers
 * to disk. We submit larger chunks of data at once.
 *
 * The journal should be locked before calling this function.
 * Called with j_checkpoint_mutex held.
 */
int jbd2_log_do_checkpoint(journal_t *journal)
{
<<<<<<< HEAD
	transaction_t *transaction;
	tid_t this_tid;
	int result;

	jbd_debug(1, "Start checkpoint\n");
=======
	struct journal_head	*jh;
	struct buffer_head	*bh;
	transaction_t		*transaction;
	tid_t			this_tid;
	int			result, batch_count = 0;

	jbd2_debug(1, "Start checkpoint\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * First thing: if there are any transactions in the log which
	 * don't need checkpointing, just eliminate them from the
	 * journal straight away.
	 */
	result = jbd2_cleanup_journal_tail(journal);
	trace_jbd2_checkpoint(journal, result);
<<<<<<< HEAD
	jbd_debug(1, "cleanup_journal_tail returned %d\n", result);
=======
	jbd2_debug(1, "cleanup_journal_tail returned %d\n", result);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (result <= 0)
		return result;

	/*
	 * OK, we need to start writing disk blocks.  Take one transaction
	 * and write it.
	 */
<<<<<<< HEAD
	result = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&journal->j_list_lock);
	if (!journal->j_checkpoint_transactions)
		goto out;
	transaction = journal->j_checkpoint_transactions;
	if (transaction->t_chp_stats.cs_chp_time == 0)
		transaction->t_chp_stats.cs_chp_time = jiffies;
	this_tid = transaction->t_tid;
restart:
	/*
	 * If someone cleaned up this transaction while we slept, we're
	 * done (maybe it's a new transaction, but it fell at the same
	 * address).
	 */
<<<<<<< HEAD
	if (journal->j_checkpoint_transactions == transaction &&
			transaction->t_tid == this_tid) {
		int batch_count = 0;
		struct journal_head *jh;
		int retry = 0, err;

		while (!retry && transaction->t_checkpoint_list) {
			jh = transaction->t_checkpoint_list;
			retry = __process_buffer(journal, jh, &batch_count,
						 transaction);
			if (retry < 0 && !result)
				result = retry;
			if (!retry && (need_resched() ||
				spin_needbreak(&journal->j_list_lock))) {
				spin_unlock(&journal->j_list_lock);
				retry = 1;
				break;
			}
		}

		if (batch_count) {
			if (!retry) {
				spin_unlock(&journal->j_list_lock);
				retry = 1;
			}
			__flush_batch(journal, &batch_count);
		}

		if (retry) {
			spin_lock(&journal->j_list_lock);
			goto restart;
		}
		/*
		 * Now we have cleaned up the first transaction's checkpoint
		 * list. Let's clean up the second one
		 */
		err = __wait_cp_io(journal, transaction);
		if (!result)
			result = err;
	}
out:
	spin_unlock(&journal->j_list_lock);
	if (result < 0)
		jbd2_journal_abort(journal, result);
	else
		result = jbd2_cleanup_journal_tail(journal);
=======
	if (journal->j_checkpoint_transactions != transaction ||
	    transaction->t_tid != this_tid)
		goto out;

	/* checkpoint all of the transaction's buffers */
	while (transaction->t_checkpoint_list) {
		jh = transaction->t_checkpoint_list;
		bh = jh2bh(jh);

		if (jh->b_transaction != NULL) {
			transaction_t *t = jh->b_transaction;
			tid_t tid = t->t_tid;

			transaction->t_chp_stats.cs_forced_to_close++;
			spin_unlock(&journal->j_list_lock);
			if (unlikely(journal->j_flags & JBD2_UNMOUNT))
				/*
				 * The journal thread is dead; so
				 * starting and waiting for a commit
				 * to finish will cause us to wait for
				 * a _very_ long time.
				 */
				printk(KERN_ERR
		"JBD2: %s: Waiting for Godot: block %llu\n",
		journal->j_devname, (unsigned long long) bh->b_blocknr);

			if (batch_count)
				__flush_batch(journal, &batch_count);
			jbd2_log_start_commit(journal, tid);
			/*
			 * jbd2_journal_commit_transaction() may want
			 * to take the checkpoint_mutex if JBD2_FLUSHED
			 * is set, jbd2_update_log_tail() called by
			 * jbd2_journal_commit_transaction() may also take
			 * checkpoint_mutex.  So we need to temporarily
			 * drop it.
			 */
			mutex_unlock(&journal->j_checkpoint_mutex);
			jbd2_log_wait_commit(journal, tid);
			mutex_lock_io(&journal->j_checkpoint_mutex);
			spin_lock(&journal->j_list_lock);
			goto restart;
		}
		if (!trylock_buffer(bh)) {
			/*
			 * The buffer is locked, it may be writing back, or
			 * flushing out in the last couple of cycles, or
			 * re-adding into a new transaction, need to check
			 * it again until it's unlocked.
			 */
			get_bh(bh);
			spin_unlock(&journal->j_list_lock);
			wait_on_buffer(bh);
			/* the journal_head may have gone by now */
			BUFFER_TRACE(bh, "brelse");
			__brelse(bh);
			goto retry;
		} else if (!buffer_dirty(bh)) {
			unlock_buffer(bh);
			BUFFER_TRACE(bh, "remove from checkpoint");
			/*
			 * If the transaction was released or the checkpoint
			 * list was empty, we're done.
			 */
			if (__jbd2_journal_remove_checkpoint(jh) ||
			    !transaction->t_checkpoint_list)
				goto out;
		} else {
			unlock_buffer(bh);
			/*
			 * We are about to write the buffer, it could be
			 * raced by some other transaction shrink or buffer
			 * re-log logic once we release the j_list_lock,
			 * leave it on the checkpoint list and check status
			 * again to make sure it's clean.
			 */
			BUFFER_TRACE(bh, "queue");
			get_bh(bh);
			J_ASSERT_BH(bh, !buffer_jwrite(bh));
			journal->j_chkpt_bhs[batch_count++] = bh;
			transaction->t_chp_stats.cs_written++;
			transaction->t_checkpoint_list = jh->b_cpnext;
		}

		if ((batch_count == JBD2_NR_BATCH) ||
		    need_resched() || spin_needbreak(&journal->j_list_lock) ||
		    jh2bh(transaction->t_checkpoint_list) == journal->j_chkpt_bhs[0])
			goto unlock_and_flush;
	}

	if (batch_count) {
		unlock_and_flush:
			spin_unlock(&journal->j_list_lock);
		retry:
			if (batch_count)
				__flush_batch(journal, &batch_count);
			spin_lock(&journal->j_list_lock);
			goto restart;
	}

out:
	spin_unlock(&journal->j_list_lock);
	result = jbd2_cleanup_journal_tail(journal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (result < 0) ? result : 0;
}

/*
 * Check the list of checkpoint transactions for the journal to see if
 * we have already got rid of any since the last update of the log tail
 * in the journal superblock.  If so, we can instantly roll the
 * superblock forward to remove those transactions from the log.
 *
 * Return <0 on error, 0 on success, 1 if there was nothing to clean up.
 *
 * Called with the journal lock held.
 *
 * This is the only part of the journaling code which really needs to be
 * aware of transaction aborts.  Checkpointing involves writing to the
 * main filesystem area rather than to the journal, so it can proceed
 * even in abort state, but we must not update the super block if
 * checkpointing may have failed.  Otherwise, we would lose some metadata
 * buffers which should be written-back to the filesystem.
 */

int jbd2_cleanup_journal_tail(journal_t *journal)
{
	tid_t		first_tid;
	unsigned long	blocknr;

	if (is_journal_aborted(journal))
		return -EIO;

	if (!jbd2_journal_get_log_tail(journal, &first_tid, &blocknr))
		return 1;
	J_ASSERT(blocknr != 0);

	/*
	 * We need to make sure that any blocks that were recently written out
	 * --- perhaps by jbd2_log_do_checkpoint() --- are flushed out before
	 * we drop the transactions from the journal. It's unlikely this will
	 * be necessary, especially with an appropriately sized journal, but we
	 * need this to guarantee correctness.  Fortunately
	 * jbd2_cleanup_journal_tail() doesn't get called all that often.
	 */
	if (journal->j_flags & JBD2_BARRIER)
<<<<<<< HEAD
		blkdev_issue_flush(journal->j_fs_dev, GFP_NOFS, NULL);
=======
		blkdev_issue_flush(journal->j_fs_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return __jbd2_update_log_tail(journal, first_tid, blocknr);
}


/* Checkpoint list management */

<<<<<<< HEAD
/*
 * journal_clean_one_cp_list
 *
 * Find all the written-back checkpoint buffers in the given list and
 * release them. If 'destroy' is set, clean all buffers unconditionally.
 *
 * Called with the journal locked.
 * Called with j_list_lock held.
 * Returns number of buffers reaped (for debug)
 */

static int journal_clean_one_cp_list(struct journal_head *jh, int *released, bool destroy)
{
	struct journal_head *last_jh;
	struct journal_head *next_jh = jh;
	int ret, freed = 0;

	*released = 0;
=======
enum shrink_type {SHRINK_DESTROY, SHRINK_BUSY_STOP, SHRINK_BUSY_SKIP};

/*
 * journal_shrink_one_cp_list
 *
 * Find all the written-back checkpoint buffers in the given list
 * and try to release them. If the whole transaction is released, set
 * the 'released' parameter. Return the number of released checkpointed
 * buffers.
 *
 * Called with j_list_lock held.
 */
static unsigned long journal_shrink_one_cp_list(struct journal_head *jh,
						enum shrink_type type,
						bool *released)
{
	struct journal_head *last_jh;
	struct journal_head *next_jh = jh;
	unsigned long nr_freed = 0;
	int ret;

	*released = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!jh)
		return 0;

	last_jh = jh->b_cpprev;
	do {
		jh = next_jh;
		next_jh = jh->b_cpnext;
<<<<<<< HEAD
		if (!destroy)
			ret = __try_to_free_cp_buf(jh);
		else
			ret = __jbd2_journal_remove_checkpoint(jh) + 1;
		if (ret) {
			freed++;
			if (ret == 2) {
				*released = 1;
				return freed;
			}
		}
		/*
		 * This function only frees up some memory
		 * if possible so we dont have an obligation
		 * to finish processing. Bail out if preemption
		 * requested:
		 */
		if (need_resched())
			return freed;
	} while (jh != last_jh);

	return freed;
=======

		if (type == SHRINK_DESTROY) {
			ret = __jbd2_journal_remove_checkpoint(jh);
		} else {
			ret = jbd2_journal_try_remove_checkpoint(jh);
			if (ret < 0) {
				if (type == SHRINK_BUSY_SKIP)
					continue;
				break;
			}
		}

		nr_freed++;
		if (ret) {
			*released = true;
			break;
		}

		if (need_resched())
			break;
	} while (jh != last_jh);

	return nr_freed;
}

/*
 * jbd2_journal_shrink_checkpoint_list
 *
 * Find 'nr_to_scan' written-back checkpoint buffers in the journal
 * and try to release them. Return the number of released checkpointed
 * buffers.
 *
 * Called with j_list_lock held.
 */
unsigned long jbd2_journal_shrink_checkpoint_list(journal_t *journal,
						  unsigned long *nr_to_scan)
{
	transaction_t *transaction, *last_transaction, *next_transaction;
	bool __maybe_unused released;
	tid_t first_tid = 0, last_tid = 0, next_tid = 0;
	tid_t tid = 0;
	unsigned long nr_freed = 0;
	unsigned long freed;

again:
	spin_lock(&journal->j_list_lock);
	if (!journal->j_checkpoint_transactions) {
		spin_unlock(&journal->j_list_lock);
		goto out;
	}

	/*
	 * Get next shrink transaction, resume previous scan or start
	 * over again. If some others do checkpoint and drop transaction
	 * from the checkpoint list, we ignore saved j_shrink_transaction
	 * and start over unconditionally.
	 */
	if (journal->j_shrink_transaction)
		transaction = journal->j_shrink_transaction;
	else
		transaction = journal->j_checkpoint_transactions;

	if (!first_tid)
		first_tid = transaction->t_tid;
	last_transaction = journal->j_checkpoint_transactions->t_cpprev;
	next_transaction = transaction;
	last_tid = last_transaction->t_tid;
	do {
		transaction = next_transaction;
		next_transaction = transaction->t_cpnext;
		tid = transaction->t_tid;

		freed = journal_shrink_one_cp_list(transaction->t_checkpoint_list,
						   SHRINK_BUSY_SKIP, &released);
		nr_freed += freed;
		(*nr_to_scan) -= min(*nr_to_scan, freed);
		if (*nr_to_scan == 0)
			break;
		if (need_resched() || spin_needbreak(&journal->j_list_lock))
			break;
	} while (transaction != last_transaction);

	if (transaction != last_transaction) {
		journal->j_shrink_transaction = next_transaction;
		next_tid = next_transaction->t_tid;
	} else {
		journal->j_shrink_transaction = NULL;
		next_tid = 0;
	}

	spin_unlock(&journal->j_list_lock);
	cond_resched();

	if (*nr_to_scan && next_tid)
		goto again;
out:
	trace_jbd2_shrink_checkpoint_list(journal, first_tid, tid, last_tid,
					  nr_freed, next_tid);

	return nr_freed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * journal_clean_checkpoint_list
 *
 * Find all the written-back checkpoint buffers in the journal and release them.
<<<<<<< HEAD
 *
 * If 'destroy' is set, release all buffers unconditionally.
 *
 * Called with the journal locked.
 * Called with j_list_lock held.
 * Returns number of buffers reaped (for debug)
 */

int __jbd2_journal_clean_checkpoint_list(journal_t *journal, bool destroy)
{
	transaction_t *transaction, *last_transaction, *next_transaction;
	int ret = 0;
	int released;

	transaction = journal->j_checkpoint_transactions;
	if (!transaction)
		goto out;

=======
 * If 'destroy' is set, release all buffers unconditionally.
 *
 * Called with j_list_lock held.
 */
void __jbd2_journal_clean_checkpoint_list(journal_t *journal, bool destroy)
{
	transaction_t *transaction, *last_transaction, *next_transaction;
	enum shrink_type type;
	bool released;

	transaction = journal->j_checkpoint_transactions;
	if (!transaction)
		return;

	type = destroy ? SHRINK_DESTROY : SHRINK_BUSY_STOP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	last_transaction = transaction->t_cpprev;
	next_transaction = transaction;
	do {
		transaction = next_transaction;
		next_transaction = transaction->t_cpnext;
<<<<<<< HEAD
		ret += journal_clean_one_cp_list(transaction->
				t_checkpoint_list, &released, destroy);
=======
		journal_shrink_one_cp_list(transaction->t_checkpoint_list,
					   type, &released);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * This function only frees up some memory if possible so we
		 * dont have an obligation to finish processing. Bail out if
		 * preemption requested:
		 */
		if (need_resched())
<<<<<<< HEAD
			goto out;
		if (released)
			continue;
		/*
		 * It is essential that we are as careful as in the case of
		 * t_checkpoint_list with removing the buffer from the list as
		 * we can possibly see not yet submitted buffers on io_list
		 */
		ret += journal_clean_one_cp_list(transaction->
				t_checkpoint_io_list, &released, destroy);
		if (need_resched())
			goto out;
	} while (transaction != last_transaction);
out:
	return ret;
=======
			return;
		/*
		 * Stop scanning if we couldn't free the transaction. This
		 * avoids pointless scanning of transactions which still
		 * weren't checkpointed.
		 */
		if (!released)
			return;
	} while (transaction != last_transaction);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Remove buffers from all checkpoint lists as journal is aborted and we just
 * need to free memory
 */
void jbd2_journal_destroy_checkpoint(journal_t *journal)
{
	/*
	 * We loop because __jbd2_journal_clean_checkpoint_list() may abort
	 * early due to a need of rescheduling.
	 */
	while (1) {
		spin_lock(&journal->j_list_lock);
		if (!journal->j_checkpoint_transactions) {
			spin_unlock(&journal->j_list_lock);
			break;
		}
		__jbd2_journal_clean_checkpoint_list(journal, true);
		spin_unlock(&journal->j_list_lock);
		cond_resched();
	}
}

/*
 * journal_remove_checkpoint: called after a buffer has been committed
 * to disk (either by being write-back flushed to disk, or being
 * committed to the log).
 *
 * We cannot safely clean a transaction out of the log until all of the
 * buffer updates committed in that transaction have safely been stored
 * elsewhere on disk.  To achieve this, all of the buffers in a
 * transaction need to be maintained on the transaction's checkpoint
 * lists until they have been rewritten, at which point this function is
 * called to remove the buffer from the existing transaction's
 * checkpoint lists.
 *
 * The function returns 1 if it frees the transaction, 0 otherwise.
 * The function can free jh and bh.
 *
 * This function is called with j_list_lock held.
 */
int __jbd2_journal_remove_checkpoint(struct journal_head *jh)
{
	struct transaction_chp_stats_s *stats;
	transaction_t *transaction;
	journal_t *journal;
<<<<<<< HEAD
	int ret = 0;

	JBUFFER_TRACE(jh, "entry");

	if ((transaction = jh->b_cp_transaction) == NULL) {
		JBUFFER_TRACE(jh, "not on transaction");
		goto out;
=======

	JBUFFER_TRACE(jh, "entry");

	transaction = jh->b_cp_transaction;
	if (!transaction) {
		JBUFFER_TRACE(jh, "not on transaction");
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	journal = transaction->t_journal;

	JBUFFER_TRACE(jh, "removing from transaction");
<<<<<<< HEAD
	__buffer_unlink(jh);
	jh->b_cp_transaction = NULL;
	jbd2_journal_put_journal_head(jh);

	if (transaction->t_checkpoint_list != NULL ||
	    transaction->t_checkpoint_io_list != NULL)
		goto out;
=======

	__buffer_unlink(jh);
	jh->b_cp_transaction = NULL;
	percpu_counter_dec(&journal->j_checkpoint_jh_count);
	jbd2_journal_put_journal_head(jh);

	/* Is this transaction empty? */
	if (transaction->t_checkpoint_list)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * There is one special case to worry about: if we have just pulled the
	 * buffer off a running or committing transaction's checkpoing list,
	 * then even if the checkpoint list is empty, the transaction obviously
	 * cannot be dropped!
	 *
	 * The locking here around t_state is a bit sleazy.
	 * See the comment at the end of jbd2_journal_commit_transaction().
	 */
	if (transaction->t_state != T_FINISHED)
<<<<<<< HEAD
		goto out;

	/* OK, that was the last buffer for the transaction: we can now
	   safely remove this transaction from the log */
=======
		return 0;

	/*
	 * OK, that was the last buffer for the transaction, we can now
	 * safely remove this transaction from the log.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stats = &transaction->t_chp_stats;
	if (stats->cs_chp_time)
		stats->cs_chp_time = jbd2_time_diff(stats->cs_chp_time,
						    jiffies);
	trace_jbd2_checkpoint_stats(journal->j_fs_dev->bd_dev,
				    transaction->t_tid, stats);

	__jbd2_journal_drop_transaction(journal, transaction);
	jbd2_journal_free_transaction(transaction);
<<<<<<< HEAD

	/* Just in case anybody was waiting for more transactions to be
           checkpointed... */
	wake_up(&journal->j_wait_logspace);
	ret = 1;
out:
	return ret;
=======
	return 1;
}

/*
 * Check the checkpoint buffer and try to remove it from the checkpoint
 * list if it's clean. Returns -EBUSY if it is not clean, returns 1 if
 * it frees the transaction, 0 otherwise.
 *
 * This function is called with j_list_lock held.
 */
int jbd2_journal_try_remove_checkpoint(struct journal_head *jh)
{
	struct buffer_head *bh = jh2bh(jh);

	if (jh->b_transaction)
		return -EBUSY;
	if (!trylock_buffer(bh))
		return -EBUSY;
	if (buffer_dirty(bh)) {
		unlock_buffer(bh);
		return -EBUSY;
	}
	unlock_buffer(bh);

	/*
	 * Buffer is clean and the IO has finished (we held the buffer
	 * lock) so the checkpoint is done. We can safely remove the
	 * buffer from this transaction.
	 */
	JBUFFER_TRACE(jh, "remove from checkpoint list");
	return __jbd2_journal_remove_checkpoint(jh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * journal_insert_checkpoint: put a committed buffer onto a checkpoint
 * list so that we know when it is safe to clean the transaction out of
 * the log.
 *
 * Called with the journal locked.
 * Called with j_list_lock held.
 */
void __jbd2_journal_insert_checkpoint(struct journal_head *jh,
			       transaction_t *transaction)
{
	JBUFFER_TRACE(jh, "entry");
	J_ASSERT_JH(jh, buffer_dirty(jh2bh(jh)) || buffer_jbddirty(jh2bh(jh)));
	J_ASSERT_JH(jh, jh->b_cp_transaction == NULL);

	/* Get reference for checkpointing transaction */
	jbd2_journal_grab_journal_head(jh2bh(jh));
	jh->b_cp_transaction = transaction;

	if (!transaction->t_checkpoint_list) {
		jh->b_cpnext = jh->b_cpprev = jh;
	} else {
		jh->b_cpnext = transaction->t_checkpoint_list;
		jh->b_cpprev = transaction->t_checkpoint_list->b_cpprev;
		jh->b_cpprev->b_cpnext = jh;
		jh->b_cpnext->b_cpprev = jh;
	}
	transaction->t_checkpoint_list = jh;
<<<<<<< HEAD
=======
	percpu_counter_inc(&transaction->t_journal->j_checkpoint_jh_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We've finished with this transaction structure: adios...
 *
 * The transaction must have no links except for the checkpoint by this
 * point.
 *
 * Called with the journal locked.
 * Called with j_list_lock held.
 */

void __jbd2_journal_drop_transaction(journal_t *journal, transaction_t *transaction)
{
	assert_spin_locked(&journal->j_list_lock);
<<<<<<< HEAD
=======

	journal->j_shrink_transaction = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (transaction->t_cpnext) {
		transaction->t_cpnext->t_cpprev = transaction->t_cpprev;
		transaction->t_cpprev->t_cpnext = transaction->t_cpnext;
		if (journal->j_checkpoint_transactions == transaction)
			journal->j_checkpoint_transactions =
				transaction->t_cpnext;
		if (journal->j_checkpoint_transactions == transaction)
			journal->j_checkpoint_transactions = NULL;
	}

	J_ASSERT(transaction->t_state == T_FINISHED);
	J_ASSERT(transaction->t_buffers == NULL);
	J_ASSERT(transaction->t_forget == NULL);
<<<<<<< HEAD
	J_ASSERT(transaction->t_iobuf_list == NULL);
	J_ASSERT(transaction->t_shadow_list == NULL);
	J_ASSERT(transaction->t_log_list == NULL);
	J_ASSERT(transaction->t_checkpoint_list == NULL);
	J_ASSERT(transaction->t_checkpoint_io_list == NULL);
=======
	J_ASSERT(transaction->t_shadow_list == NULL);
	J_ASSERT(transaction->t_checkpoint_list == NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	J_ASSERT(atomic_read(&transaction->t_updates) == 0);
	J_ASSERT(journal->j_committing_transaction != transaction);
	J_ASSERT(journal->j_running_transaction != transaction);

	trace_jbd2_drop_transaction(journal, transaction);

<<<<<<< HEAD
	jbd_debug(1, "Dropping transaction %d, all done\n", transaction->t_tid);
=======
	jbd2_debug(1, "Dropping transaction %d, all done\n", transaction->t_tid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
