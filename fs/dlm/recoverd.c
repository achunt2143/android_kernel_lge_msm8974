<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2011 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
**
*******************************************************************************
******************************************************************************/

#include "dlm_internal.h"
#include "lockspace.h"
#include "member.h"
#include "dir.h"
#include "ast.h"
#include "recover.h"
#include "lowcomms.h"
#include "lock.h"
#include "requestqueue.h"
#include "recoverd.h"


/* If the start for which we're re-enabling locking (seq) has been superseded
   by a newer stop (ls_recover_seq), we need to leave locking disabled.

   We suspend dlm_recv threads here to avoid the race where dlm_recv a) sees
   locking stopped and b) adds a message to the requestqueue, but dlm_recoverd
   enables locking and clears the requestqueue between a and b. */

static int enable_locking(struct dlm_ls *ls, uint64_t seq)
{
	int error = -EINTR;

	down_write(&ls->ls_recv_active);

	spin_lock(&ls->ls_recover_lock);
	if (ls->ls_recover_seq == seq) {
		set_bit(LSFL_RUNNING, &ls->ls_flags);
		/* unblocks processes waiting to enter the dlm */
		up_write(&ls->ls_in_recovery);
<<<<<<< HEAD
=======
		clear_bit(LSFL_RECOVER_LOCK, &ls->ls_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		error = 0;
	}
	spin_unlock(&ls->ls_recover_lock);

	up_write(&ls->ls_recv_active);
	return error;
}

static int ls_recover(struct dlm_ls *ls, struct dlm_recover *rv)
{
	unsigned long start;
	int error, neg = 0;

<<<<<<< HEAD
	log_debug(ls, "dlm_recover %llx", (unsigned long long)rv->seq);
=======
	log_rinfo(ls, "dlm_recover %llu", (unsigned long long)rv->seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&ls->ls_recoverd_active);

	dlm_callback_suspend(ls);

<<<<<<< HEAD
	/*
	 * Free non-master tossed rsb's.  Master rsb's are kept on toss
	 * list and put on root list to be included in resdir recovery.
	 */

	dlm_clear_toss_list(ls);
=======
	dlm_clear_toss(ls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * This list of root rsb's will be the basis of most of the recovery
	 * routines.
	 */

	dlm_create_root_list(ls);

	/*
	 * Add or remove nodes from the lockspace's ls_nodes list.
<<<<<<< HEAD
=======
	 *
	 * Due to the fact that we must report all membership changes to lsops
	 * or midcomms layer, it is not permitted to abort ls_recover() until
	 * this is done.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */

	error = dlm_recover_members(ls, rv, &neg);
	if (error) {
<<<<<<< HEAD
		log_debug(ls, "dlm_recover_members error %d", error);
		goto fail;
	}

	dlm_set_recover_status(ls, DLM_RS_NODES);

	error = dlm_recover_members_wait(ls);
	if (error) {
		log_debug(ls, "dlm_recover_members_wait error %d", error);
=======
		log_rinfo(ls, "dlm_recover_members error %d", error);
		goto fail;
	}

	dlm_recover_dir_nodeid(ls);

	ls->ls_recover_dir_sent_res = 0;
	ls->ls_recover_dir_sent_msg = 0;
	ls->ls_recover_locks_in = 0;

	dlm_set_recover_status(ls, DLM_RS_NODES);

	error = dlm_recover_members_wait(ls, rv->seq);
	if (error) {
		log_rinfo(ls, "dlm_recover_members_wait error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	start = jiffies;

	/*
	 * Rebuild our own share of the directory by collecting from all other
	 * nodes their master rsb names that hash to us.
	 */

<<<<<<< HEAD
	error = dlm_recover_directory(ls);
	if (error) {
		log_debug(ls, "dlm_recover_directory error %d", error);
=======
	error = dlm_recover_directory(ls, rv->seq);
	if (error) {
		log_rinfo(ls, "dlm_recover_directory error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	dlm_set_recover_status(ls, DLM_RS_DIR);

<<<<<<< HEAD
	error = dlm_recover_directory_wait(ls);
	if (error) {
		log_debug(ls, "dlm_recover_directory_wait error %d", error);
		goto fail;
	}

=======
	error = dlm_recover_directory_wait(ls, rv->seq);
	if (error) {
		log_rinfo(ls, "dlm_recover_directory_wait error %d", error);
		goto fail;
	}

	log_rinfo(ls, "dlm_recover_directory %u out %u messages",
		  ls->ls_recover_dir_sent_res, ls->ls_recover_dir_sent_msg);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We may have outstanding operations that are waiting for a reply from
	 * a failed node.  Mark these to be resent after recovery.  Unlock and
	 * cancel ops can just be completed.
	 */

	dlm_recover_waiters_pre(ls);

<<<<<<< HEAD
	error = dlm_recovery_stopped(ls);
	if (error)
		goto fail;
=======
	if (dlm_recovery_stopped(ls)) {
		error = -EINTR;
		goto fail;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (neg || dlm_no_directory(ls)) {
		/*
		 * Clear lkb's for departed nodes.
		 */

<<<<<<< HEAD
		dlm_purge_locks(ls);
=======
		dlm_recover_purge(ls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Get new master nodeid's for rsb's that were mastered on
		 * departed nodes.
		 */

<<<<<<< HEAD
		error = dlm_recover_masters(ls);
		if (error) {
			log_debug(ls, "dlm_recover_masters error %d", error);
=======
		error = dlm_recover_masters(ls, rv->seq);
		if (error) {
			log_rinfo(ls, "dlm_recover_masters error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}

		/*
		 * Send our locks on remastered rsb's to the new masters.
		 */

<<<<<<< HEAD
		error = dlm_recover_locks(ls);
		if (error) {
			log_debug(ls, "dlm_recover_locks error %d", error);
=======
		error = dlm_recover_locks(ls, rv->seq);
		if (error) {
			log_rinfo(ls, "dlm_recover_locks error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}

		dlm_set_recover_status(ls, DLM_RS_LOCKS);

<<<<<<< HEAD
		error = dlm_recover_locks_wait(ls);
		if (error) {
			log_debug(ls, "dlm_recover_locks_wait error %d", error);
			goto fail;
		}

=======
		error = dlm_recover_locks_wait(ls, rv->seq);
		if (error) {
			log_rinfo(ls, "dlm_recover_locks_wait error %d", error);
			goto fail;
		}

		log_rinfo(ls, "dlm_recover_locks %u in",
			  ls->ls_recover_locks_in);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Finalize state in master rsb's now that all locks can be
		 * checked.  This includes conversion resolution and lvb
		 * settings.
		 */

		dlm_recover_rsbs(ls);
	} else {
		/*
		 * Other lockspace members may be going through the "neg" steps
		 * while also adding us to the lockspace, in which case they'll
		 * be doing the recover_locks (RS_LOCKS) barrier.
		 */
		dlm_set_recover_status(ls, DLM_RS_LOCKS);

<<<<<<< HEAD
		error = dlm_recover_locks_wait(ls);
		if (error) {
			log_debug(ls, "dlm_recover_locks_wait error %d", error);
=======
		error = dlm_recover_locks_wait(ls, rv->seq);
		if (error) {
			log_rinfo(ls, "dlm_recover_locks_wait error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
	}

	dlm_release_root_list(ls);

	/*
	 * Purge directory-related requests that are saved in requestqueue.
	 * All dir requests from before recovery are invalid now due to the dir
	 * rebuild and will be resent by the requesting nodes.
	 */

	dlm_purge_requestqueue(ls);

	dlm_set_recover_status(ls, DLM_RS_DONE);

<<<<<<< HEAD
	error = dlm_recover_done_wait(ls);
	if (error) {
		log_debug(ls, "dlm_recover_done_wait error %d", error);
=======
	error = dlm_recover_done_wait(ls, rv->seq);
	if (error) {
		log_rinfo(ls, "dlm_recover_done_wait error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	dlm_clear_members_gone(ls);

<<<<<<< HEAD
	dlm_adjust_timeouts(ls);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dlm_callback_resume(ls);

	error = enable_locking(ls, rv->seq);
	if (error) {
<<<<<<< HEAD
		log_debug(ls, "enable_locking error %d", error);
=======
		log_rinfo(ls, "enable_locking error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	error = dlm_process_requestqueue(ls);
	if (error) {
<<<<<<< HEAD
		log_debug(ls, "dlm_process_requestqueue error %d", error);
=======
		log_rinfo(ls, "dlm_process_requestqueue error %d", error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	error = dlm_recover_waiters_post(ls);
	if (error) {
<<<<<<< HEAD
		log_debug(ls, "dlm_recover_waiters_post error %d", error);
		goto fail;
	}

	dlm_grant_after_purge(ls);

	log_debug(ls, "dlm_recover %llx generation %u done: %u ms",
=======
		log_rinfo(ls, "dlm_recover_waiters_post error %d", error);
		goto fail;
	}

	dlm_recover_grant(ls);

	log_rinfo(ls, "dlm_recover %llu generation %u done: %u ms",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		  (unsigned long long)rv->seq, ls->ls_generation,
		  jiffies_to_msecs(jiffies - start));
	mutex_unlock(&ls->ls_recoverd_active);

<<<<<<< HEAD
	dlm_lsop_recover_done(ls);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

 fail:
	dlm_release_root_list(ls);
<<<<<<< HEAD
	log_debug(ls, "dlm_recover %llx error %d",
		  (unsigned long long)rv->seq, error);
	mutex_unlock(&ls->ls_recoverd_active);
=======
	mutex_unlock(&ls->ls_recoverd_active);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/* The dlm_ls_start() that created the rv we take here may already have been
   stopped via dlm_ls_stop(); in that case we need to leave the RECOVERY_STOP
   flag set. */

static void do_ls_recovery(struct dlm_ls *ls)
{
	struct dlm_recover *rv = NULL;
<<<<<<< HEAD
=======
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&ls->ls_recover_lock);
	rv = ls->ls_recover_args;
	ls->ls_recover_args = NULL;
	if (rv && ls->ls_recover_seq == rv->seq)
<<<<<<< HEAD
		clear_bit(LSFL_RECOVERY_STOP, &ls->ls_flags);
	spin_unlock(&ls->ls_recover_lock);

	if (rv) {
		ls_recover(ls, rv);
=======
		clear_bit(LSFL_RECOVER_STOP, &ls->ls_flags);
	spin_unlock(&ls->ls_recover_lock);

	if (rv) {
		error = ls_recover(ls, rv);
		switch (error) {
		case 0:
			ls->ls_recovery_result = 0;
			complete(&ls->ls_recovery_done);

			dlm_lsop_recover_done(ls);
			break;
		case -EINTR:
			/* if recovery was interrupted -EINTR we wait for the next
			 * ls_recover() iteration until it hopefully succeeds.
			 */
			log_rinfo(ls, "%s %llu interrupted and should be queued to run again",
				  __func__, (unsigned long long)rv->seq);
			break;
		default:
			log_rinfo(ls, "%s %llu error %d", __func__,
				  (unsigned long long)rv->seq, error);

			/* let new_lockspace() get aware of critical error */
			ls->ls_recovery_result = error;
			complete(&ls->ls_recovery_done);
			break;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(rv->nodes);
		kfree(rv);
	}
}

static int dlm_recoverd(void *arg)
{
	struct dlm_ls *ls;

	ls = dlm_find_lockspace_local(arg);
	if (!ls) {
		log_print("dlm_recoverd: no lockspace %p", arg);
		return -1;
	}

<<<<<<< HEAD
	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (!test_bit(LSFL_WORK, &ls->ls_flags))
			schedule();
		set_current_state(TASK_RUNNING);

		if (test_and_clear_bit(LSFL_WORK, &ls->ls_flags))
			do_ls_recovery(ls);
	}

=======
	down_write(&ls->ls_in_recovery);
	set_bit(LSFL_RECOVER_LOCK, &ls->ls_flags);
	wake_up(&ls->ls_recover_lock_wait);

	while (1) {
		/*
		 * We call kthread_should_stop() after set_current_state().
		 * This is because it works correctly if kthread_stop() is
		 * called just before set_current_state().
		 */
		set_current_state(TASK_INTERRUPTIBLE);
		if (kthread_should_stop()) {
			set_current_state(TASK_RUNNING);
			break;
		}
		if (!test_bit(LSFL_RECOVER_WORK, &ls->ls_flags) &&
		    !test_bit(LSFL_RECOVER_DOWN, &ls->ls_flags)) {
			if (kthread_should_stop())
				break;
			schedule();
		}
		set_current_state(TASK_RUNNING);

		if (test_and_clear_bit(LSFL_RECOVER_DOWN, &ls->ls_flags)) {
			down_write(&ls->ls_in_recovery);
			set_bit(LSFL_RECOVER_LOCK, &ls->ls_flags);
			wake_up(&ls->ls_recover_lock_wait);
		}

		if (test_and_clear_bit(LSFL_RECOVER_WORK, &ls->ls_flags))
			do_ls_recovery(ls);
	}

	if (test_bit(LSFL_RECOVER_LOCK, &ls->ls_flags))
		up_write(&ls->ls_in_recovery);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dlm_put_lockspace(ls);
	return 0;
}

<<<<<<< HEAD
void dlm_recoverd_kick(struct dlm_ls *ls)
{
	set_bit(LSFL_WORK, &ls->ls_flags);
	wake_up_process(ls->ls_recoverd_task);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int dlm_recoverd_start(struct dlm_ls *ls)
{
	struct task_struct *p;
	int error = 0;

	p = kthread_run(dlm_recoverd, ls, "dlm_recoverd");
	if (IS_ERR(p))
		error = PTR_ERR(p);
	else
                ls->ls_recoverd_task = p;
	return error;
}

void dlm_recoverd_stop(struct dlm_ls *ls)
{
	kthread_stop(ls->ls_recoverd_task);
}

void dlm_recoverd_suspend(struct dlm_ls *ls)
{
	wake_up(&ls->ls_wait_general);
	mutex_lock(&ls->ls_recoverd_active);
}

void dlm_recoverd_resume(struct dlm_ls *ls)
{
	mutex_unlock(&ls->ls_recoverd_active);
}

