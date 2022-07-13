<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) 2005-2009 Red Hat, Inc.  All rights reserved.
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

#include <linux/pagemap.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/slab.h>

#include "dlm_internal.h"
<<<<<<< HEAD
#include "lock.h"
=======
#include "midcomms.h"
#include "lock.h"
#include "ast.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DLM_DEBUG_BUF_LEN 4096
static char debug_buf[DLM_DEBUG_BUF_LEN];
static struct mutex debug_buf_lock;

static struct dentry *dlm_root;
<<<<<<< HEAD
=======
static struct dentry *dlm_comms;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static char *print_lockmode(int mode)
{
	switch (mode) {
	case DLM_LOCK_IV:
		return "--";
	case DLM_LOCK_NL:
		return "NL";
	case DLM_LOCK_CR:
		return "CR";
	case DLM_LOCK_CW:
		return "CW";
	case DLM_LOCK_PR:
		return "PR";
	case DLM_LOCK_PW:
		return "PW";
	case DLM_LOCK_EX:
		return "EX";
	default:
		return "??";
	}
}

<<<<<<< HEAD
static int print_format1_lock(struct seq_file *s, struct dlm_lkb *lkb,
			      struct dlm_rsb *res)
=======
static void print_format1_lock(struct seq_file *s, struct dlm_lkb *lkb,
			       struct dlm_rsb *res)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	seq_printf(s, "%08x %s", lkb->lkb_id, print_lockmode(lkb->lkb_grmode));

	if (lkb->lkb_status == DLM_LKSTS_CONVERT ||
	    lkb->lkb_status == DLM_LKSTS_WAITING)
		seq_printf(s, " (%s)", print_lockmode(lkb->lkb_rqmode));

	if (lkb->lkb_nodeid) {
		if (lkb->lkb_nodeid != res->res_nodeid)
			seq_printf(s, " Remote: %3d %08x", lkb->lkb_nodeid,
				   lkb->lkb_remid);
		else
			seq_printf(s, " Master:     %08x", lkb->lkb_remid);
	}

	if (lkb->lkb_wait_type)
		seq_printf(s, " wait_type: %d", lkb->lkb_wait_type);

<<<<<<< HEAD
	return seq_printf(s, "\n");
}

static int print_format1(struct dlm_rsb *res, struct seq_file *s)
{
	struct dlm_lkb *lkb;
	int i, lvblen = res->res_ls->ls_lvblen, recover_list, root_list;
	int rv;

	lock_rsb(res);

	rv = seq_printf(s, "\nResource %p Name (len=%d) \"",
			res, res->res_length);
	if (rv)
		goto out;
=======
	seq_putc(s, '\n');
}

static void print_format1(struct dlm_rsb *res, struct seq_file *s)
{
	struct dlm_lkb *lkb;
	int i, lvblen = res->res_ls->ls_lvblen, recover_list, root_list;

	lock_rsb(res);

	seq_printf(s, "\nResource %p Name (len=%d) \"", res, res->res_length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < res->res_length; i++) {
		if (isprint(res->res_name[i]))
			seq_printf(s, "%c", res->res_name[i]);
		else
			seq_printf(s, "%c", '.');
	}

	if (res->res_nodeid > 0)
<<<<<<< HEAD
		rv = seq_printf(s, "\"  \nLocal Copy, Master is node %d\n",
				res->res_nodeid);
	else if (res->res_nodeid == 0)
		rv = seq_printf(s, "\"  \nMaster Copy\n");
	else if (res->res_nodeid == -1)
		rv = seq_printf(s, "\"  \nLooking up master (lkid %x)\n",
			   	res->res_first_lkid);
	else
		rv = seq_printf(s, "\"  \nInvalid master %d\n",
				res->res_nodeid);
	if (rv)
=======
		seq_printf(s, "\"\nLocal Copy, Master is node %d\n",
			   res->res_nodeid);
	else if (res->res_nodeid == 0)
		seq_puts(s, "\"\nMaster Copy\n");
	else if (res->res_nodeid == -1)
		seq_printf(s, "\"\nLooking up master (lkid %x)\n",
			   res->res_first_lkid);
	else
		seq_printf(s, "\"\nInvalid master %d\n", res->res_nodeid);
	if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	/* Print the LVB: */
	if (res->res_lvbptr) {
<<<<<<< HEAD
		seq_printf(s, "LVB: ");
		for (i = 0; i < lvblen; i++) {
			if (i == lvblen / 2)
				seq_printf(s, "\n     ");
=======
		seq_puts(s, "LVB: ");
		for (i = 0; i < lvblen; i++) {
			if (i == lvblen / 2)
				seq_puts(s, "\n     ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			seq_printf(s, "%02x ",
				   (unsigned char) res->res_lvbptr[i]);
		}
		if (rsb_flag(res, RSB_VALNOTVALID))
<<<<<<< HEAD
			seq_printf(s, " (INVALID)");
		rv = seq_printf(s, "\n");
		if (rv)
=======
			seq_puts(s, " (INVALID)");
		seq_putc(s, '\n');
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	root_list = !list_empty(&res->res_root_list);
	recover_list = !list_empty(&res->res_recover_list);

	if (root_list || recover_list) {
<<<<<<< HEAD
		rv = seq_printf(s, "Recovery: root %d recover %d flags %lx "
				"count %d\n", root_list, recover_list,
			   	res->res_flags, res->res_recover_locks_count);
		if (rv)
			goto out;
	}

	/* Print the locks attached to this resource */
	seq_printf(s, "Granted Queue\n");
	list_for_each_entry(lkb, &res->res_grantqueue, lkb_statequeue) {
		rv = print_format1_lock(s, lkb, res);
		if (rv)
			goto out;
	}

	seq_printf(s, "Conversion Queue\n");
	list_for_each_entry(lkb, &res->res_convertqueue, lkb_statequeue) {
		rv = print_format1_lock(s, lkb, res);
		if (rv)
			goto out;
	}

	seq_printf(s, "Waiting Queue\n");
	list_for_each_entry(lkb, &res->res_waitqueue, lkb_statequeue) {
		rv = print_format1_lock(s, lkb, res);
		if (rv)
=======
		seq_printf(s, "Recovery: root %d recover %d flags %lx count %d\n",
			   root_list, recover_list,
			   res->res_flags, res->res_recover_locks_count);
	}

	/* Print the locks attached to this resource */
	seq_puts(s, "Granted Queue\n");
	list_for_each_entry(lkb, &res->res_grantqueue, lkb_statequeue) {
		print_format1_lock(s, lkb, res);
		if (seq_has_overflowed(s))
			goto out;
	}

	seq_puts(s, "Conversion Queue\n");
	list_for_each_entry(lkb, &res->res_convertqueue, lkb_statequeue) {
		print_format1_lock(s, lkb, res);
		if (seq_has_overflowed(s))
			goto out;
	}

	seq_puts(s, "Waiting Queue\n");
	list_for_each_entry(lkb, &res->res_waitqueue, lkb_statequeue) {
		print_format1_lock(s, lkb, res);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	if (list_empty(&res->res_lookup))
		goto out;

<<<<<<< HEAD
	seq_printf(s, "Lookup Queue\n");
	list_for_each_entry(lkb, &res->res_lookup, lkb_rsb_lookup) {
		rv = seq_printf(s, "%08x %s", lkb->lkb_id,
				print_lockmode(lkb->lkb_rqmode));
		if (lkb->lkb_wait_type)
			seq_printf(s, " wait_type: %d", lkb->lkb_wait_type);
		rv = seq_printf(s, "\n");
	}
 out:
	unlock_rsb(res);
	return rv;
}

static int print_format2_lock(struct seq_file *s, struct dlm_lkb *lkb,
			      struct dlm_rsb *r)
{
	u64 xid = 0;
	u64 us;
	int rv;

	if (lkb->lkb_flags & DLM_IFL_USER) {
=======
	seq_puts(s, "Lookup Queue\n");
	list_for_each_entry(lkb, &res->res_lookup, lkb_rsb_lookup) {
		seq_printf(s, "%08x %s",
			   lkb->lkb_id, print_lockmode(lkb->lkb_rqmode));
		if (lkb->lkb_wait_type)
			seq_printf(s, " wait_type: %d", lkb->lkb_wait_type);
		seq_putc(s, '\n');
		if (seq_has_overflowed(s))
			goto out;
	}
 out:
	unlock_rsb(res);
}

static void print_format2_lock(struct seq_file *s, struct dlm_lkb *lkb,
			       struct dlm_rsb *r)
{
	u64 xid = 0;
	u64 us;

	if (test_bit(DLM_DFL_USER_BIT, &lkb->lkb_dflags)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (lkb->lkb_ua)
			xid = lkb->lkb_ua->xid;
	}

	/* microseconds since lkb was added to current queue */
	us = ktime_to_us(ktime_sub(ktime_get(), lkb->lkb_timestamp));

	/* id nodeid remid pid xid exflags flags sts grmode rqmode time_us
	   r_nodeid r_len r_name */

<<<<<<< HEAD
	rv = seq_printf(s, "%x %d %x %u %llu %x %x %d %d %d %llu %u %d \"%s\"\n",
			lkb->lkb_id,
			lkb->lkb_nodeid,
			lkb->lkb_remid,
			lkb->lkb_ownpid,
			(unsigned long long)xid,
			lkb->lkb_exflags,
			lkb->lkb_flags,
			lkb->lkb_status,
			lkb->lkb_grmode,
			lkb->lkb_rqmode,
			(unsigned long long)us,
			r->res_nodeid,
			r->res_length,
			r->res_name);
	return rv;
}

static int print_format2(struct dlm_rsb *r, struct seq_file *s)
{
	struct dlm_lkb *lkb;
	int rv = 0;
=======
	seq_printf(s, "%x %d %x %u %llu %x %x %d %d %d %llu %u %d \"%s\"\n",
		   lkb->lkb_id,
		   lkb->lkb_nodeid,
		   lkb->lkb_remid,
		   lkb->lkb_ownpid,
		   (unsigned long long)xid,
		   lkb->lkb_exflags,
		   dlm_iflags_val(lkb),
		   lkb->lkb_status,
		   lkb->lkb_grmode,
		   lkb->lkb_rqmode,
		   (unsigned long long)us,
		   r->res_nodeid,
		   r->res_length,
		   r->res_name);
}

static void print_format2(struct dlm_rsb *r, struct seq_file *s)
{
	struct dlm_lkb *lkb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	lock_rsb(r);

	list_for_each_entry(lkb, &r->res_grantqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format2_lock(s, lkb, r);
		if (rv)
=======
		print_format2_lock(s, lkb, r);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	list_for_each_entry(lkb, &r->res_convertqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format2_lock(s, lkb, r);
		if (rv)
=======
		print_format2_lock(s, lkb, r);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	list_for_each_entry(lkb, &r->res_waitqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format2_lock(s, lkb, r);
		if (rv)
=======
		print_format2_lock(s, lkb, r);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}
 out:
	unlock_rsb(r);
<<<<<<< HEAD
	return rv;
}

static int print_format3_lock(struct seq_file *s, struct dlm_lkb *lkb,
			      int rsb_lookup)
{
	u64 xid = 0;
	int rv;

	if (lkb->lkb_flags & DLM_IFL_USER) {
=======
}

static void print_format3_lock(struct seq_file *s, struct dlm_lkb *lkb,
			      int rsb_lookup)
{
	u64 xid = 0;

	if (test_bit(DLM_DFL_USER_BIT, &lkb->lkb_dflags)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (lkb->lkb_ua)
			xid = lkb->lkb_ua->xid;
	}

<<<<<<< HEAD
	rv = seq_printf(s, "lkb %x %d %x %u %llu %x %x %d %d %d %d %d %d %u %llu %llu\n",
			lkb->lkb_id,
			lkb->lkb_nodeid,
			lkb->lkb_remid,
			lkb->lkb_ownpid,
			(unsigned long long)xid,
			lkb->lkb_exflags,
			lkb->lkb_flags,
			lkb->lkb_status,
			lkb->lkb_grmode,
			lkb->lkb_rqmode,
			lkb->lkb_last_bast.mode,
			rsb_lookup,
			lkb->lkb_wait_type,
			lkb->lkb_lvbseq,
			(unsigned long long)ktime_to_ns(lkb->lkb_timestamp),
			(unsigned long long)ktime_to_ns(lkb->lkb_last_bast_time));
	return rv;
}

static int print_format3(struct dlm_rsb *r, struct seq_file *s)
=======
	seq_printf(s, "lkb %x %d %x %u %llu %x %x %d %d %d %d %d %d %u %llu %llu\n",
		   lkb->lkb_id,
		   lkb->lkb_nodeid,
		   lkb->lkb_remid,
		   lkb->lkb_ownpid,
		   (unsigned long long)xid,
		   lkb->lkb_exflags,
		   dlm_iflags_val(lkb),
		   lkb->lkb_status,
		   lkb->lkb_grmode,
		   lkb->lkb_rqmode,
		   lkb->lkb_last_bast_mode,
		   rsb_lookup,
		   lkb->lkb_wait_type,
		   lkb->lkb_lvbseq,
		   (unsigned long long)ktime_to_ns(lkb->lkb_timestamp),
		   (unsigned long long)ktime_to_ns(lkb->lkb_last_bast_time));
}

static void print_format3(struct dlm_rsb *r, struct seq_file *s)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dlm_lkb *lkb;
	int i, lvblen = r->res_ls->ls_lvblen;
	int print_name = 1;
<<<<<<< HEAD
	int rv;

	lock_rsb(r);

	rv = seq_printf(s, "rsb %p %d %x %lx %d %d %u %d ",
			r,
			r->res_nodeid,
			r->res_first_lkid,
			r->res_flags,
			!list_empty(&r->res_root_list),
			!list_empty(&r->res_recover_list),
			r->res_recover_locks_count,
			r->res_length);
	if (rv)
=======

	lock_rsb(r);

	seq_printf(s, "rsb %p %d %x %lx %d %d %u %d ",
		   r,
		   r->res_nodeid,
		   r->res_first_lkid,
		   r->res_flags,
		   !list_empty(&r->res_root_list),
		   !list_empty(&r->res_recover_list),
		   r->res_recover_locks_count,
		   r->res_length);
	if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	for (i = 0; i < r->res_length; i++) {
		if (!isascii(r->res_name[i]) || !isprint(r->res_name[i]))
			print_name = 0;
	}

<<<<<<< HEAD
	seq_printf(s, "%s", print_name ? "str " : "hex");
=======
	seq_puts(s, print_name ? "str " : "hex");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < r->res_length; i++) {
		if (print_name)
			seq_printf(s, "%c", r->res_name[i]);
		else
			seq_printf(s, " %02x", (unsigned char)r->res_name[i]);
	}
<<<<<<< HEAD
	rv = seq_printf(s, "\n");
	if (rv)
=======
	seq_putc(s, '\n');
	if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	if (!r->res_lvbptr)
		goto do_locks;

	seq_printf(s, "lvb %u %d", r->res_lvbseq, lvblen);

	for (i = 0; i < lvblen; i++)
		seq_printf(s, " %02x", (unsigned char)r->res_lvbptr[i]);
<<<<<<< HEAD
	rv = seq_printf(s, "\n");
	if (rv)
=======
	seq_putc(s, '\n');
	if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

 do_locks:
	list_for_each_entry(lkb, &r->res_grantqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format3_lock(s, lkb, 0);
		if (rv)
=======
		print_format3_lock(s, lkb, 0);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	list_for_each_entry(lkb, &r->res_convertqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format3_lock(s, lkb, 0);
		if (rv)
=======
		print_format3_lock(s, lkb, 0);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	list_for_each_entry(lkb, &r->res_waitqueue, lkb_statequeue) {
<<<<<<< HEAD
		rv = print_format3_lock(s, lkb, 0);
		if (rv)
=======
		print_format3_lock(s, lkb, 0);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	list_for_each_entry(lkb, &r->res_lookup, lkb_rsb_lookup) {
<<<<<<< HEAD
		rv = print_format3_lock(s, lkb, 1);
		if (rv)
=======
		print_format3_lock(s, lkb, 1);
		if (seq_has_overflowed(s))
			goto out;
	}
 out:
	unlock_rsb(r);
}

static void print_format4(struct dlm_rsb *r, struct seq_file *s)
{
	int our_nodeid = dlm_our_nodeid();
	int print_name = 1;
	int i;

	lock_rsb(r);

	seq_printf(s, "rsb %p %d %d %d %d %lu %lx %d ",
		   r,
		   r->res_nodeid,
		   r->res_master_nodeid,
		   r->res_dir_nodeid,
		   our_nodeid,
		   r->res_toss_time,
		   r->res_flags,
		   r->res_length);

	for (i = 0; i < r->res_length; i++) {
		if (!isascii(r->res_name[i]) || !isprint(r->res_name[i]))
			print_name = 0;
	}

	seq_puts(s, print_name ? "str " : "hex");

	for (i = 0; i < r->res_length; i++) {
		if (print_name)
			seq_printf(s, "%c", r->res_name[i]);
		else
			seq_printf(s, " %02x", (unsigned char)r->res_name[i]);
	}
	seq_putc(s, '\n');
	unlock_rsb(r);
}

static void print_format5_lock(struct seq_file *s, struct dlm_lkb *lkb)
{
	struct dlm_callback *cb;

	/* lkb_id lkb_flags mode flags sb_status sb_flags */

	spin_lock(&lkb->lkb_cb_lock);
	list_for_each_entry(cb, &lkb->lkb_callbacks, list) {
		seq_printf(s, "%x %x %d %x %d %x\n",
			   lkb->lkb_id,
			   dlm_iflags_val(lkb),
			   cb->mode,
			   cb->flags,
			   cb->sb_status,
			   cb->sb_flags);
	}
	spin_unlock(&lkb->lkb_cb_lock);
}

static void print_format5(struct dlm_rsb *r, struct seq_file *s)
{
	struct dlm_lkb *lkb;

	lock_rsb(r);

	list_for_each_entry(lkb, &r->res_grantqueue, lkb_statequeue) {
		print_format5_lock(s, lkb);
		if (seq_has_overflowed(s))
			goto out;
	}

	list_for_each_entry(lkb, &r->res_convertqueue, lkb_statequeue) {
		print_format5_lock(s, lkb);
		if (seq_has_overflowed(s))
			goto out;
	}

	list_for_each_entry(lkb, &r->res_waitqueue, lkb_statequeue) {
		print_format5_lock(s, lkb);
		if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}
 out:
	unlock_rsb(r);
<<<<<<< HEAD
	return rv;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct rsbtbl_iter {
	struct dlm_rsb *rsb;
	unsigned bucket;
	int format;
	int header;
};

<<<<<<< HEAD
/* seq_printf returns -1 if the buffer is full, and 0 otherwise.
   If the buffer is full, seq_printf can be called again, but it
   does nothing and just returns -1.  So, the these printing routines
   periodically check the return value to avoid wasting too much time
   trying to print to a full buffer. */
=======
/*
 * If the buffer is full, seq_printf can be called again, but it
 * does nothing.  So, the these printing routines periodically check
 * seq_has_overflowed to avoid wasting too much time trying to print to
 * a full buffer.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int table_seq_show(struct seq_file *seq, void *iter_ptr)
{
	struct rsbtbl_iter *ri = iter_ptr;
<<<<<<< HEAD
	int rv = 0;

	switch (ri->format) {
	case 1:
		rv = print_format1(ri->rsb, seq);
		break;
	case 2:
		if (ri->header) {
			seq_printf(seq, "id nodeid remid pid xid exflags "
					"flags sts grmode rqmode time_ms "
					"r_nodeid r_len r_name\n");
			ri->header = 0;
		}
		rv = print_format2(ri->rsb, seq);
		break;
	case 3:
		if (ri->header) {
			seq_printf(seq, "version rsb 1.1 lvb 1.1 lkb 1.1\n");
			ri->header = 0;
		}
		rv = print_format3(ri->rsb, seq);
		break;
	}

	return rv;
=======

	switch (ri->format) {
	case 1:
		print_format1(ri->rsb, seq);
		break;
	case 2:
		if (ri->header) {
			seq_puts(seq, "id nodeid remid pid xid exflags flags sts grmode rqmode time_ms r_nodeid r_len r_name\n");
			ri->header = 0;
		}
		print_format2(ri->rsb, seq);
		break;
	case 3:
		if (ri->header) {
			seq_puts(seq, "rsb ptr nodeid first_lkid flags !root_list_empty !recover_list_empty recover_locks_count len\n");
			ri->header = 0;
		}
		print_format3(ri->rsb, seq);
		break;
	case 4:
		if (ri->header) {
			seq_puts(seq, "rsb ptr nodeid master_nodeid dir_nodeid our_nodeid toss_time flags len str|hex name\n");
			ri->header = 0;
		}
		print_format4(ri->rsb, seq);
		break;
	case 5:
		if (ri->header) {
			seq_puts(seq, "lkb_id lkb_flags mode flags sb_status sb_flags\n");
			ri->header = 0;
		}
		print_format5(ri->rsb, seq);
		break;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct seq_operations format1_seq_ops;
static const struct seq_operations format2_seq_ops;
static const struct seq_operations format3_seq_ops;
<<<<<<< HEAD

static void *table_seq_start(struct seq_file *seq, loff_t *pos)
{
=======
static const struct seq_operations format4_seq_ops;
static const struct seq_operations format5_seq_ops;

static void *table_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct rb_root *tree;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rb_node *node;
	struct dlm_ls *ls = seq->private;
	struct rsbtbl_iter *ri;
	struct dlm_rsb *r;
	loff_t n = *pos;
	unsigned bucket, entry;
<<<<<<< HEAD
=======
	int toss = (seq->op == &format4_seq_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bucket = n >> 32;
	entry = n & ((1LL << 32) - 1);

	if (bucket >= ls->ls_rsbtbl_size)
		return NULL;

<<<<<<< HEAD
	ri = kzalloc(sizeof(struct rsbtbl_iter), GFP_NOFS);
=======
	ri = kzalloc(sizeof(*ri), GFP_NOFS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ri)
		return NULL;
	if (n == 0)
		ri->header = 1;
	if (seq->op == &format1_seq_ops)
		ri->format = 1;
	if (seq->op == &format2_seq_ops)
		ri->format = 2;
	if (seq->op == &format3_seq_ops)
		ri->format = 3;
<<<<<<< HEAD

	spin_lock(&ls->ls_rsbtbl[bucket].lock);
	if (!RB_EMPTY_ROOT(&ls->ls_rsbtbl[bucket].keep)) {
		for (node = rb_first(&ls->ls_rsbtbl[bucket].keep); node;
		     node = rb_next(node)) {
=======
	if (seq->op == &format4_seq_ops)
		ri->format = 4;
	if (seq->op == &format5_seq_ops)
		ri->format = 5;

	tree = toss ? &ls->ls_rsbtbl[bucket].toss : &ls->ls_rsbtbl[bucket].keep;

	spin_lock(&ls->ls_rsbtbl[bucket].lock);
	if (!RB_EMPTY_ROOT(tree)) {
		for (node = rb_first(tree); node; node = rb_next(node)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			r = rb_entry(node, struct dlm_rsb, res_hashnode);
			if (!entry--) {
				dlm_hold_rsb(r);
				ri->rsb = r;
				ri->bucket = bucket;
				spin_unlock(&ls->ls_rsbtbl[bucket].lock);
				return ri;
			}
		}
	}
	spin_unlock(&ls->ls_rsbtbl[bucket].lock);

	/*
	 * move to the first rsb in the next non-empty bucket
	 */

	/* zero the entry */
	n &= ~((1LL << 32) - 1);

	while (1) {
		bucket++;
		n += 1LL << 32;

		if (bucket >= ls->ls_rsbtbl_size) {
			kfree(ri);
			return NULL;
		}
<<<<<<< HEAD

		spin_lock(&ls->ls_rsbtbl[bucket].lock);
		if (!RB_EMPTY_ROOT(&ls->ls_rsbtbl[bucket].keep)) {
			node = rb_first(&ls->ls_rsbtbl[bucket].keep);
=======
		tree = toss ? &ls->ls_rsbtbl[bucket].toss : &ls->ls_rsbtbl[bucket].keep;

		spin_lock(&ls->ls_rsbtbl[bucket].lock);
		if (!RB_EMPTY_ROOT(tree)) {
			node = rb_first(tree);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			r = rb_entry(node, struct dlm_rsb, res_hashnode);
			dlm_hold_rsb(r);
			ri->rsb = r;
			ri->bucket = bucket;
			spin_unlock(&ls->ls_rsbtbl[bucket].lock);
			*pos = n;
			return ri;
		}
		spin_unlock(&ls->ls_rsbtbl[bucket].lock);
	}
}

static void *table_seq_next(struct seq_file *seq, void *iter_ptr, loff_t *pos)
{
	struct dlm_ls *ls = seq->private;
	struct rsbtbl_iter *ri = iter_ptr;
<<<<<<< HEAD
=======
	struct rb_root *tree;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rb_node *next;
	struct dlm_rsb *r, *rp;
	loff_t n = *pos;
	unsigned bucket;
<<<<<<< HEAD
=======
	int toss = (seq->op == &format4_seq_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bucket = n >> 32;

	/*
	 * move to the next rsb in the same bucket
	 */

	spin_lock(&ls->ls_rsbtbl[bucket].lock);
	rp = ri->rsb;
	next = rb_next(&rp->res_hashnode);

	if (next) {
		r = rb_entry(next, struct dlm_rsb, res_hashnode);
		dlm_hold_rsb(r);
		ri->rsb = r;
		spin_unlock(&ls->ls_rsbtbl[bucket].lock);
		dlm_put_rsb(rp);
		++*pos;
		return ri;
	}
	spin_unlock(&ls->ls_rsbtbl[bucket].lock);
	dlm_put_rsb(rp);

	/*
	 * move to the first rsb in the next non-empty bucket
	 */

	/* zero the entry */
	n &= ~((1LL << 32) - 1);

	while (1) {
		bucket++;
		n += 1LL << 32;

		if (bucket >= ls->ls_rsbtbl_size) {
			kfree(ri);
<<<<<<< HEAD
			return NULL;
		}

		spin_lock(&ls->ls_rsbtbl[bucket].lock);
		if (!RB_EMPTY_ROOT(&ls->ls_rsbtbl[bucket].keep)) {
			next = rb_first(&ls->ls_rsbtbl[bucket].keep);
=======
			++*pos;
			return NULL;
		}
		tree = toss ? &ls->ls_rsbtbl[bucket].toss : &ls->ls_rsbtbl[bucket].keep;

		spin_lock(&ls->ls_rsbtbl[bucket].lock);
		if (!RB_EMPTY_ROOT(tree)) {
			next = rb_first(tree);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			r = rb_entry(next, struct dlm_rsb, res_hashnode);
			dlm_hold_rsb(r);
			ri->rsb = r;
			ri->bucket = bucket;
			spin_unlock(&ls->ls_rsbtbl[bucket].lock);
			*pos = n;
			return ri;
		}
		spin_unlock(&ls->ls_rsbtbl[bucket].lock);
	}
}

static void table_seq_stop(struct seq_file *seq, void *iter_ptr)
{
	struct rsbtbl_iter *ri = iter_ptr;

	if (ri) {
		dlm_put_rsb(ri->rsb);
		kfree(ri);
	}
}

static const struct seq_operations format1_seq_ops = {
	.start = table_seq_start,
	.next  = table_seq_next,
	.stop  = table_seq_stop,
	.show  = table_seq_show,
};

static const struct seq_operations format2_seq_ops = {
	.start = table_seq_start,
	.next  = table_seq_next,
	.stop  = table_seq_stop,
	.show  = table_seq_show,
};

static const struct seq_operations format3_seq_ops = {
	.start = table_seq_start,
	.next  = table_seq_next,
	.stop  = table_seq_stop,
	.show  = table_seq_show,
};

<<<<<<< HEAD
static const struct file_operations format1_fops;
static const struct file_operations format2_fops;
static const struct file_operations format3_fops;

static int table_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret = -1;

	if (file->f_op == &format1_fops)
		ret = seq_open(file, &format1_seq_ops);
	else if (file->f_op == &format2_fops)
		ret = seq_open(file, &format2_seq_ops);
	else if (file->f_op == &format3_fops)
		ret = seq_open(file, &format3_seq_ops);

=======
static const struct seq_operations format4_seq_ops = {
	.start = table_seq_start,
	.next  = table_seq_next,
	.stop  = table_seq_stop,
	.show  = table_seq_show,
};

static const struct seq_operations format5_seq_ops = {
	.start = table_seq_start,
	.next  = table_seq_next,
	.stop  = table_seq_stop,
	.show  = table_seq_show,
};

static const struct file_operations format1_fops;
static const struct file_operations format2_fops;
static const struct file_operations format3_fops;
static const struct file_operations format4_fops;
static const struct file_operations format5_fops;

static int table_open1(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &format1_seq_ops);
	if (ret)
		return ret;

	seq = file->private_data;
	seq->private = inode->i_private; /* the dlm_ls */
	return 0;
}

static int table_open2(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &format2_seq_ops);
	if (ret)
		return ret;

	seq = file->private_data;
	seq->private = inode->i_private; /* the dlm_ls */
	return 0;
}

static ssize_t table_write2(struct file *file, const char __user *user_buf,
			    size_t count, loff_t *ppos)
{
	struct seq_file *seq = file->private_data;
	int n, len, lkb_nodeid, lkb_status, error;
	char name[DLM_RESNAME_MAXLEN + 1] = {};
	struct dlm_ls *ls = seq->private;
	unsigned int lkb_flags;
	char buf[256] = {};
	uint32_t lkb_id;

	if (copy_from_user(buf, user_buf,
			   min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	n = sscanf(buf, "%x %" __stringify(DLM_RESNAME_MAXLEN) "s %x %d %d",
		   &lkb_id, name, &lkb_flags, &lkb_nodeid, &lkb_status);
	if (n != 5)
		return -EINVAL;

	len = strnlen(name, DLM_RESNAME_MAXLEN);
	error = dlm_debug_add_lkb(ls, lkb_id, name, len, lkb_flags,
				  lkb_nodeid, lkb_status);
	if (error)
		return error;

	return count;
}

static int table_open3(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &format3_seq_ops);
	if (ret)
		return ret;

	seq = file->private_data;
	seq->private = inode->i_private; /* the dlm_ls */
	return 0;
}

static int table_open4(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &format4_seq_ops);
	if (ret)
		return ret;

	seq = file->private_data;
	seq->private = inode->i_private; /* the dlm_ls */
	return 0;
}

static int table_open5(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int ret;

	ret = seq_open(file, &format5_seq_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	seq = file->private_data;
	seq->private = inode->i_private; /* the dlm_ls */
	return 0;
}

static const struct file_operations format1_fops = {
	.owner   = THIS_MODULE,
<<<<<<< HEAD
	.open    = table_open,
=======
	.open    = table_open1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

static const struct file_operations format2_fops = {
	.owner   = THIS_MODULE,
<<<<<<< HEAD
	.open    = table_open,
	.read    = seq_read,
=======
	.open    = table_open2,
	.read    = seq_read,
	.write   = table_write2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek  = seq_lseek,
	.release = seq_release
};

static const struct file_operations format3_fops = {
	.owner   = THIS_MODULE,
<<<<<<< HEAD
	.open    = table_open,
=======
	.open    = table_open3,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

static const struct file_operations format4_fops = {
	.owner   = THIS_MODULE,
	.open    = table_open4,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

static const struct file_operations format5_fops = {
	.owner   = THIS_MODULE,
	.open    = table_open5,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

/*
 * dump lkb's on the ls_waiters list
 */
static ssize_t waiters_read(struct file *file, char __user *userbuf,
			    size_t count, loff_t *ppos)
{
	struct dlm_ls *ls = file->private_data;
	struct dlm_lkb *lkb;
	size_t len = DLM_DEBUG_BUF_LEN, pos = 0, ret, rv;

	mutex_lock(&debug_buf_lock);
	mutex_lock(&ls->ls_waiters_mutex);
	memset(debug_buf, 0, sizeof(debug_buf));

	list_for_each_entry(lkb, &ls->ls_waiters, lkb_wait_reply) {
		ret = snprintf(debug_buf + pos, len - pos, "%x %d %d %s\n",
			       lkb->lkb_id, lkb->lkb_wait_type,
			       lkb->lkb_nodeid, lkb->lkb_resource->res_name);
		if (ret >= len - pos)
			break;
		pos += ret;
	}
	mutex_unlock(&ls->ls_waiters_mutex);

	rv = simple_read_from_buffer(userbuf, count, ppos, debug_buf, pos);
	mutex_unlock(&debug_buf_lock);
	return rv;
}

<<<<<<< HEAD
=======
static ssize_t waiters_write(struct file *file, const char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct dlm_ls *ls = file->private_data;
	int mstype, to_nodeid;
	char buf[128] = {};
	uint32_t lkb_id;
	int n, error;

	if (copy_from_user(buf, user_buf,
			   min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	n = sscanf(buf, "%x %d %d", &lkb_id, &mstype, &to_nodeid);
	if (n != 3)
		return -EINVAL;

	error = dlm_debug_add_lkb_to_waiters(ls, lkb_id, mstype, to_nodeid);
	if (error)
		return error;

	return count;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct file_operations waiters_fops = {
	.owner   = THIS_MODULE,
	.open    = simple_open,
	.read    = waiters_read,
<<<<<<< HEAD
=======
	.write   = waiters_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek  = default_llseek,
};

void dlm_delete_debug_file(struct dlm_ls *ls)
{
<<<<<<< HEAD
	if (ls->ls_debug_rsb_dentry)
		debugfs_remove(ls->ls_debug_rsb_dentry);
	if (ls->ls_debug_waiters_dentry)
		debugfs_remove(ls->ls_debug_waiters_dentry);
	if (ls->ls_debug_locks_dentry)
		debugfs_remove(ls->ls_debug_locks_dentry);
	if (ls->ls_debug_all_dentry)
		debugfs_remove(ls->ls_debug_all_dentry);
}

int dlm_create_debug_file(struct dlm_ls *ls)
{
	char name[DLM_LOCKSPACE_LEN+8];
=======
	debugfs_remove(ls->ls_debug_rsb_dentry);
	debugfs_remove(ls->ls_debug_waiters_dentry);
	debugfs_remove(ls->ls_debug_locks_dentry);
	debugfs_remove(ls->ls_debug_all_dentry);
	debugfs_remove(ls->ls_debug_toss_dentry);
	debugfs_remove(ls->ls_debug_queued_asts_dentry);
}

static int dlm_state_show(struct seq_file *file, void *offset)
{
	seq_printf(file, "%s\n", dlm_midcomms_state(file->private));
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dlm_state);

static int dlm_flags_show(struct seq_file *file, void *offset)
{
	seq_printf(file, "%lu\n", dlm_midcomms_flags(file->private));
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dlm_flags);

static int dlm_send_queue_cnt_show(struct seq_file *file, void *offset)
{
	seq_printf(file, "%d\n", dlm_midcomms_send_queue_cnt(file->private));
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dlm_send_queue_cnt);

static int dlm_version_show(struct seq_file *file, void *offset)
{
	seq_printf(file, "0x%08x\n", dlm_midcomms_version(file->private));
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dlm_version);

static ssize_t dlm_rawmsg_write(struct file *fp, const char __user *user_buf,
				size_t count, loff_t *ppos)
{
	void *buf;
	int ret;

	if (count > PAGE_SIZE || count < sizeof(struct dlm_header))
		return -EINVAL;

	buf = kmalloc(PAGE_SIZE, GFP_NOFS);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, user_buf, count)) {
		ret = -EFAULT;
		goto out;
	}

	ret = dlm_midcomms_rawmsg_send(fp->private_data, buf, count);
	if (ret)
		goto out;

	kfree(buf);
	return count;

out:
	kfree(buf);
	return ret;
}

static const struct file_operations dlm_rawmsg_fops = {
	.open	= simple_open,
	.write	= dlm_rawmsg_write,
	.llseek	= no_llseek,
};

void *dlm_create_debug_comms_file(int nodeid, void *data)
{
	struct dentry *d_node;
	char name[256];

	memset(name, 0, sizeof(name));
	snprintf(name, 256, "%d", nodeid);

	d_node = debugfs_create_dir(name, dlm_comms);
	debugfs_create_file("state", 0444, d_node, data, &dlm_state_fops);
	debugfs_create_file("flags", 0444, d_node, data, &dlm_flags_fops);
	debugfs_create_file("send_queue_count", 0444, d_node, data,
			    &dlm_send_queue_cnt_fops);
	debugfs_create_file("version", 0444, d_node, data, &dlm_version_fops);
	debugfs_create_file("rawmsg", 0200, d_node, data, &dlm_rawmsg_fops);

	return d_node;
}

void dlm_delete_debug_comms_file(void *ctx)
{
	debugfs_remove(ctx);
}

void dlm_create_debug_file(struct dlm_ls *ls)
{
	/* Reserve enough space for the longest file name */
	char name[DLM_LOCKSPACE_LEN + sizeof("_queued_asts")];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* format 1 */

	ls->ls_debug_rsb_dentry = debugfs_create_file(ls->ls_name,
						      S_IFREG | S_IRUGO,
						      dlm_root,
						      ls,
						      &format1_fops);
<<<<<<< HEAD
	if (!ls->ls_debug_rsb_dentry)
		goto fail;

	/* format 2 */

	memset(name, 0, sizeof(name));
	snprintf(name, DLM_LOCKSPACE_LEN+8, "%s_locks", ls->ls_name);

	ls->ls_debug_locks_dentry = debugfs_create_file(name,
							S_IFREG | S_IRUGO,
							dlm_root,
							ls,
							&format2_fops);
	if (!ls->ls_debug_locks_dentry)
		goto fail;

	/* format 3 */

	memset(name, 0, sizeof(name));
	snprintf(name, DLM_LOCKSPACE_LEN+8, "%s_all", ls->ls_name);
=======

	/* format 2 */

	snprintf(name, sizeof(name), "%s_locks", ls->ls_name);

	ls->ls_debug_locks_dentry = debugfs_create_file(name,
							0644,
							dlm_root,
							ls,
							&format2_fops);

	/* format 3 */

	snprintf(name, sizeof(name), "%s_all", ls->ls_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ls->ls_debug_all_dentry = debugfs_create_file(name,
						      S_IFREG | S_IRUGO,
						      dlm_root,
						      ls,
						      &format3_fops);
<<<<<<< HEAD
	if (!ls->ls_debug_all_dentry)
		goto fail;

	memset(name, 0, sizeof(name));
	snprintf(name, DLM_LOCKSPACE_LEN+8, "%s_waiters", ls->ls_name);

	ls->ls_debug_waiters_dentry = debugfs_create_file(name,
							  S_IFREG | S_IRUGO,
							  dlm_root,
							  ls,
							  &waiters_fops);
	if (!ls->ls_debug_waiters_dentry)
		goto fail;

	return 0;

 fail:
	dlm_delete_debug_file(ls);
	return -ENOMEM;
}

int __init dlm_register_debugfs(void)
{
	mutex_init(&debug_buf_lock);
	dlm_root = debugfs_create_dir("dlm", NULL);
	return dlm_root ? 0 : -ENOMEM;
=======

	/* format 4 */

	snprintf(name, sizeof(name), "%s_toss", ls->ls_name);

	ls->ls_debug_toss_dentry = debugfs_create_file(name,
						       S_IFREG | S_IRUGO,
						       dlm_root,
						       ls,
						       &format4_fops);

	snprintf(name, sizeof(name), "%s_waiters", ls->ls_name);

	ls->ls_debug_waiters_dentry = debugfs_create_file(name,
							  0644,
							  dlm_root,
							  ls,
							  &waiters_fops);

	/* format 5 */

	snprintf(name, sizeof(name), "%s_queued_asts", ls->ls_name);

	ls->ls_debug_queued_asts_dentry = debugfs_create_file(name,
							      0644,
							      dlm_root,
							      ls,
							      &format5_fops);
}

void __init dlm_register_debugfs(void)
{
	mutex_init(&debug_buf_lock);
	dlm_root = debugfs_create_dir("dlm", NULL);
	dlm_comms = debugfs_create_dir("comms", dlm_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void dlm_unregister_debugfs(void)
{
	debugfs_remove(dlm_root);
}

