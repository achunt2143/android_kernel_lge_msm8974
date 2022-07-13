<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
 * Filename:  target_core_transport.c
 *
 * This file contains the Generic Target Engine Core.
 *
<<<<<<< HEAD
 * Copyright (c) 2002, 2003, 2004, 2005 PyX Technologies, Inc.
 * Copyright (c) 2005, 2006, 2007 SBE, Inc.
 * Copyright (c) 2007-2010 Rising Tide Systems
 * Copyright (c) 2008-2010 Linux-iSCSI.org
 *
 * Nicholas A. Bellinger <nab@kernel.org>
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
 *
=======
 * (c) Copyright 2002-2013 Datera, Inc.
 *
 * Nicholas A. Bellinger <nab@kernel.org>
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ******************************************************************************/

#include <linux/net.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/cdrom.h>
#include <linux/module.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
#include <asm/unaligned.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_tcq.h>
=======
#include <linux/vmalloc.h>
#include <asm/unaligned.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <scsi/scsi_proto.h>
#include <scsi/scsi_common.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <target/target_core_base.h>
#include <target/target_core_backend.h>
#include <target/target_core_fabric.h>
<<<<<<< HEAD
#include <target/target_core_configfs.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "target_core_internal.h"
#include "target_core_alua.h"
#include "target_core_pr.h"
#include "target_core_ua.h"

<<<<<<< HEAD
static int sub_api_initialized;

static struct workqueue_struct *target_completion_wq;
=======
#define CREATE_TRACE_POINTS
#include <trace/events/target.h>

static struct workqueue_struct *target_completion_wq;
static struct workqueue_struct *target_submission_wq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct kmem_cache *se_sess_cache;
struct kmem_cache *se_ua_cache;
struct kmem_cache *t10_pr_reg_cache;
struct kmem_cache *t10_alua_lu_gp_cache;
struct kmem_cache *t10_alua_lu_gp_mem_cache;
struct kmem_cache *t10_alua_tg_pt_gp_cache;
<<<<<<< HEAD
struct kmem_cache *t10_alua_tg_pt_gp_mem_cache;

static int transport_generic_write_pending(struct se_cmd *);
static int transport_processing_thread(void *param);
static int __transport_execute_tasks(struct se_device *dev, struct se_cmd *);
static void transport_complete_task_attr(struct se_cmd *cmd);
static void transport_handle_queue_full(struct se_cmd *cmd,
		struct se_device *dev);
static void transport_free_dev_tasks(struct se_cmd *cmd);
static int transport_generic_get_mem(struct se_cmd *cmd);
static void transport_put_cmd(struct se_cmd *cmd);
static void transport_remove_cmd_from_queue(struct se_cmd *cmd);
static int transport_set_sense_codes(struct se_cmd *cmd, u8 asc, u8 ascq);
=======
struct kmem_cache *t10_alua_lba_map_cache;
struct kmem_cache *t10_alua_lba_map_mem_cache;

static void transport_complete_task_attr(struct se_cmd *cmd);
static void translate_sense_reason(struct se_cmd *cmd, sense_reason_t reason);
static void transport_handle_queue_full(struct se_cmd *cmd,
		struct se_device *dev, int err, bool write_pending);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void target_complete_ok_work(struct work_struct *work);

int init_se_kmem_caches(void)
{
	se_sess_cache = kmem_cache_create("se_sess_cache",
			sizeof(struct se_session), __alignof__(struct se_session),
			0, NULL);
	if (!se_sess_cache) {
		pr_err("kmem_cache_create() for struct se_session"
				" failed\n");
		goto out;
	}
	se_ua_cache = kmem_cache_create("se_ua_cache",
			sizeof(struct se_ua), __alignof__(struct se_ua),
			0, NULL);
	if (!se_ua_cache) {
		pr_err("kmem_cache_create() for struct se_ua failed\n");
		goto out_free_sess_cache;
	}
	t10_pr_reg_cache = kmem_cache_create("t10_pr_reg_cache",
			sizeof(struct t10_pr_registration),
			__alignof__(struct t10_pr_registration), 0, NULL);
	if (!t10_pr_reg_cache) {
		pr_err("kmem_cache_create() for struct t10_pr_registration"
				" failed\n");
		goto out_free_ua_cache;
	}
	t10_alua_lu_gp_cache = kmem_cache_create("t10_alua_lu_gp_cache",
			sizeof(struct t10_alua_lu_gp), __alignof__(struct t10_alua_lu_gp),
			0, NULL);
	if (!t10_alua_lu_gp_cache) {
		pr_err("kmem_cache_create() for t10_alua_lu_gp_cache"
				" failed\n");
		goto out_free_pr_reg_cache;
	}
	t10_alua_lu_gp_mem_cache = kmem_cache_create("t10_alua_lu_gp_mem_cache",
			sizeof(struct t10_alua_lu_gp_member),
			__alignof__(struct t10_alua_lu_gp_member), 0, NULL);
	if (!t10_alua_lu_gp_mem_cache) {
		pr_err("kmem_cache_create() for t10_alua_lu_gp_mem_"
				"cache failed\n");
		goto out_free_lu_gp_cache;
	}
	t10_alua_tg_pt_gp_cache = kmem_cache_create("t10_alua_tg_pt_gp_cache",
			sizeof(struct t10_alua_tg_pt_gp),
			__alignof__(struct t10_alua_tg_pt_gp), 0, NULL);
	if (!t10_alua_tg_pt_gp_cache) {
		pr_err("kmem_cache_create() for t10_alua_tg_pt_gp_"
				"cache failed\n");
		goto out_free_lu_gp_mem_cache;
	}
<<<<<<< HEAD
	t10_alua_tg_pt_gp_mem_cache = kmem_cache_create(
			"t10_alua_tg_pt_gp_mem_cache",
			sizeof(struct t10_alua_tg_pt_gp_member),
			__alignof__(struct t10_alua_tg_pt_gp_member),
			0, NULL);
	if (!t10_alua_tg_pt_gp_mem_cache) {
		pr_err("kmem_cache_create() for t10_alua_tg_pt_gp_"
				"mem_t failed\n");
		goto out_free_tg_pt_gp_cache;
	}
=======
	t10_alua_lba_map_cache = kmem_cache_create(
			"t10_alua_lba_map_cache",
			sizeof(struct t10_alua_lba_map),
			__alignof__(struct t10_alua_lba_map), 0, NULL);
	if (!t10_alua_lba_map_cache) {
		pr_err("kmem_cache_create() for t10_alua_lba_map_"
				"cache failed\n");
		goto out_free_tg_pt_gp_cache;
	}
	t10_alua_lba_map_mem_cache = kmem_cache_create(
			"t10_alua_lba_map_mem_cache",
			sizeof(struct t10_alua_lba_map_member),
			__alignof__(struct t10_alua_lba_map_member), 0, NULL);
	if (!t10_alua_lba_map_mem_cache) {
		pr_err("kmem_cache_create() for t10_alua_lba_map_mem_"
				"cache failed\n");
		goto out_free_lba_map_cache;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	target_completion_wq = alloc_workqueue("target_completion",
					       WQ_MEM_RECLAIM, 0);
	if (!target_completion_wq)
<<<<<<< HEAD
		goto out_free_tg_pt_gp_mem_cache;

	return 0;

out_free_tg_pt_gp_mem_cache:
	kmem_cache_destroy(t10_alua_tg_pt_gp_mem_cache);
=======
		goto out_free_lba_map_mem_cache;

	target_submission_wq = alloc_workqueue("target_submission",
					       WQ_MEM_RECLAIM, 0);
	if (!target_submission_wq)
		goto out_free_completion_wq;

	return 0;

out_free_completion_wq:
	destroy_workqueue(target_completion_wq);
out_free_lba_map_mem_cache:
	kmem_cache_destroy(t10_alua_lba_map_mem_cache);
out_free_lba_map_cache:
	kmem_cache_destroy(t10_alua_lba_map_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_free_tg_pt_gp_cache:
	kmem_cache_destroy(t10_alua_tg_pt_gp_cache);
out_free_lu_gp_mem_cache:
	kmem_cache_destroy(t10_alua_lu_gp_mem_cache);
out_free_lu_gp_cache:
	kmem_cache_destroy(t10_alua_lu_gp_cache);
out_free_pr_reg_cache:
	kmem_cache_destroy(t10_pr_reg_cache);
out_free_ua_cache:
	kmem_cache_destroy(se_ua_cache);
out_free_sess_cache:
	kmem_cache_destroy(se_sess_cache);
out:
	return -ENOMEM;
}

void release_se_kmem_caches(void)
{
<<<<<<< HEAD
=======
	destroy_workqueue(target_submission_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destroy_workqueue(target_completion_wq);
	kmem_cache_destroy(se_sess_cache);
	kmem_cache_destroy(se_ua_cache);
	kmem_cache_destroy(t10_pr_reg_cache);
	kmem_cache_destroy(t10_alua_lu_gp_cache);
	kmem_cache_destroy(t10_alua_lu_gp_mem_cache);
	kmem_cache_destroy(t10_alua_tg_pt_gp_cache);
<<<<<<< HEAD
	kmem_cache_destroy(t10_alua_tg_pt_gp_mem_cache);
=======
	kmem_cache_destroy(t10_alua_lba_map_cache);
	kmem_cache_destroy(t10_alua_lba_map_mem_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This code ensures unique mib indexes are handed out. */
static DEFINE_SPINLOCK(scsi_mib_index_lock);
static u32 scsi_mib_index[SCSI_INDEX_TYPE_MAX];

/*
 * Allocate a new row index for the entry type specified
 */
u32 scsi_get_new_index(scsi_index_t type)
{
	u32 new_index;

	BUG_ON((type < 0) || (type >= SCSI_INDEX_TYPE_MAX));

	spin_lock(&scsi_mib_index_lock);
	new_index = ++scsi_mib_index[type];
	spin_unlock(&scsi_mib_index_lock);

	return new_index;
}

<<<<<<< HEAD
static void transport_init_queue_obj(struct se_queue_obj *qobj)
{
	atomic_set(&qobj->queue_cnt, 0);
	INIT_LIST_HEAD(&qobj->qobj_list);
	init_waitqueue_head(&qobj->thread_wq);
	spin_lock_init(&qobj->cmd_queue_lock);
}

void transport_subsystem_check_init(void)
{
	int ret;
=======
void transport_subsystem_check_init(void)
{
	int ret;
	static int sub_api_initialized;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sub_api_initialized)
		return;

<<<<<<< HEAD
	ret = request_module("target_core_iblock");
	if (ret != 0)
		pr_err("Unable to load target_core_iblock\n");

	ret = request_module("target_core_file");
	if (ret != 0)
		pr_err("Unable to load target_core_file\n");

	ret = request_module("target_core_pscsi");
	if (ret != 0)
		pr_err("Unable to load target_core_pscsi\n");

	ret = request_module("target_core_stgt");
	if (ret != 0)
		pr_err("Unable to load target_core_stgt\n");

	sub_api_initialized = 1;
	return;
}

struct se_session *transport_init_session(void)
=======
	ret = IS_ENABLED(CONFIG_TCM_IBLOCK) && request_module("target_core_iblock");
	if (ret != 0)
		pr_err("Unable to load target_core_iblock\n");

	ret = IS_ENABLED(CONFIG_TCM_FILEIO) && request_module("target_core_file");
	if (ret != 0)
		pr_err("Unable to load target_core_file\n");

	ret = IS_ENABLED(CONFIG_TCM_PSCSI) && request_module("target_core_pscsi");
	if (ret != 0)
		pr_err("Unable to load target_core_pscsi\n");

	ret = IS_ENABLED(CONFIG_TCM_USER2) && request_module("target_core_user");
	if (ret != 0)
		pr_err("Unable to load target_core_user\n");

	sub_api_initialized = 1;
}

static void target_release_cmd_refcnt(struct percpu_ref *ref)
{
	struct target_cmd_counter *cmd_cnt  = container_of(ref,
							   typeof(*cmd_cnt),
							   refcnt);
	wake_up(&cmd_cnt->refcnt_wq);
}

struct target_cmd_counter *target_alloc_cmd_counter(void)
{
	struct target_cmd_counter *cmd_cnt;
	int rc;

	cmd_cnt = kzalloc(sizeof(*cmd_cnt), GFP_KERNEL);
	if (!cmd_cnt)
		return NULL;

	init_completion(&cmd_cnt->stop_done);
	init_waitqueue_head(&cmd_cnt->refcnt_wq);
	atomic_set(&cmd_cnt->stopped, 0);

	rc = percpu_ref_init(&cmd_cnt->refcnt, target_release_cmd_refcnt, 0,
			     GFP_KERNEL);
	if (rc)
		goto free_cmd_cnt;

	return cmd_cnt;

free_cmd_cnt:
	kfree(cmd_cnt);
	return NULL;
}
EXPORT_SYMBOL_GPL(target_alloc_cmd_counter);

void target_free_cmd_counter(struct target_cmd_counter *cmd_cnt)
{
	/*
	 * Drivers like loop do not call target_stop_session during session
	 * shutdown so we have to drop the ref taken at init time here.
	 */
	if (!atomic_read(&cmd_cnt->stopped))
		percpu_ref_put(&cmd_cnt->refcnt);

	percpu_ref_exit(&cmd_cnt->refcnt);
	kfree(cmd_cnt);
}
EXPORT_SYMBOL_GPL(target_free_cmd_counter);

/**
 * transport_init_session - initialize a session object
 * @se_sess: Session object pointer.
 *
 * The caller must have zero-initialized @se_sess before calling this function.
 */
void transport_init_session(struct se_session *se_sess)
{
	INIT_LIST_HEAD(&se_sess->sess_list);
	INIT_LIST_HEAD(&se_sess->sess_acl_list);
	spin_lock_init(&se_sess->sess_cmd_lock);
}
EXPORT_SYMBOL(transport_init_session);

/**
 * transport_alloc_session - allocate a session object and initialize it
 * @sup_prot_ops: bitmask that defines which T10-PI modes are supported.
 */
struct se_session *transport_alloc_session(enum target_prot_op sup_prot_ops)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct se_session *se_sess;

	se_sess = kmem_cache_zalloc(se_sess_cache, GFP_KERNEL);
	if (!se_sess) {
		pr_err("Unable to allocate struct se_session from"
				" se_sess_cache\n");
		return ERR_PTR(-ENOMEM);
	}
<<<<<<< HEAD
	INIT_LIST_HEAD(&se_sess->sess_list);
	INIT_LIST_HEAD(&se_sess->sess_acl_list);
	INIT_LIST_HEAD(&se_sess->sess_cmd_list);
	INIT_LIST_HEAD(&se_sess->sess_wait_list);
	spin_lock_init(&se_sess->sess_cmd_lock);
	kref_init(&se_sess->sess_kref);

	return se_sess;
}
EXPORT_SYMBOL(transport_init_session);
=======
	transport_init_session(se_sess);
	se_sess->sup_prot_ops = sup_prot_ops;

	return se_sess;
}
EXPORT_SYMBOL(transport_alloc_session);

/**
 * transport_alloc_session_tags - allocate target driver private data
 * @se_sess:  Session pointer.
 * @tag_num:  Maximum number of in-flight commands between initiator and target.
 * @tag_size: Size in bytes of the private data a target driver associates with
 *	      each command.
 */
int transport_alloc_session_tags(struct se_session *se_sess,
			         unsigned int tag_num, unsigned int tag_size)
{
	int rc;

	se_sess->sess_cmd_map = kvcalloc(tag_size, tag_num,
					 GFP_KERNEL | __GFP_RETRY_MAYFAIL);
	if (!se_sess->sess_cmd_map) {
		pr_err("Unable to allocate se_sess->sess_cmd_map\n");
		return -ENOMEM;
	}

	rc = sbitmap_queue_init_node(&se_sess->sess_tag_pool, tag_num, -1,
			false, GFP_KERNEL, NUMA_NO_NODE);
	if (rc < 0) {
		pr_err("Unable to init se_sess->sess_tag_pool,"
			" tag_num: %u\n", tag_num);
		kvfree(se_sess->sess_cmd_map);
		se_sess->sess_cmd_map = NULL;
		return -ENOMEM;
	}

	return 0;
}
EXPORT_SYMBOL(transport_alloc_session_tags);

/**
 * transport_init_session_tags - allocate a session and target driver private data
 * @tag_num:  Maximum number of in-flight commands between initiator and target.
 * @tag_size: Size in bytes of the private data a target driver associates with
 *	      each command.
 * @sup_prot_ops: bitmask that defines which T10-PI modes are supported.
 */
static struct se_session *
transport_init_session_tags(unsigned int tag_num, unsigned int tag_size,
			    enum target_prot_op sup_prot_ops)
{
	struct se_session *se_sess;
	int rc;

	if (tag_num != 0 && !tag_size) {
		pr_err("init_session_tags called with percpu-ida tag_num:"
		       " %u, but zero tag_size\n", tag_num);
		return ERR_PTR(-EINVAL);
	}
	if (!tag_num && tag_size) {
		pr_err("init_session_tags called with percpu-ida tag_size:"
		       " %u, but zero tag_num\n", tag_size);
		return ERR_PTR(-EINVAL);
	}

	se_sess = transport_alloc_session(sup_prot_ops);
	if (IS_ERR(se_sess))
		return se_sess;

	rc = transport_alloc_session_tags(se_sess, tag_num, tag_size);
	if (rc < 0) {
		transport_free_session(se_sess);
		return ERR_PTR(-ENOMEM);
	}

	return se_sess;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Called with spin_lock_irqsave(&struct se_portal_group->session_lock called.
 */
void __transport_register_session(
	struct se_portal_group *se_tpg,
	struct se_node_acl *se_nacl,
	struct se_session *se_sess,
	void *fabric_sess_ptr)
{
<<<<<<< HEAD
	unsigned char buf[PR_REG_ISID_LEN];
=======
	const struct target_core_fabric_ops *tfo = se_tpg->se_tpg_tfo;
	unsigned char buf[PR_REG_ISID_LEN];
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	se_sess->se_tpg = se_tpg;
	se_sess->fabric_sess_ptr = fabric_sess_ptr;
	/*
	 * Used by struct se_node_acl's under ConfigFS to locate active se_session-t
	 *
	 * Only set for struct se_session's that will actually be moving I/O.
	 * eg: *NOT* discovery sessions.
	 */
	if (se_nacl) {
		/*
<<<<<<< HEAD
=======
		 *
		 * Determine if fabric allows for T10-PI feature bits exposed to
		 * initiators for device backends with !dev->dev_attrib.pi_prot_type.
		 *
		 * If so, then always save prot_type on a per se_node_acl node
		 * basis and re-instate the previous sess_prot_type to avoid
		 * disabling PI from below any previously initiator side
		 * registered LUNs.
		 */
		if (se_nacl->saved_prot_type)
			se_sess->sess_prot_type = se_nacl->saved_prot_type;
		else if (tfo->tpg_check_prot_fabric_only)
			se_sess->sess_prot_type = se_nacl->saved_prot_type =
					tfo->tpg_check_prot_fabric_only(se_tpg);
		/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * If the fabric module supports an ISID based TransportID,
		 * save this value in binary from the fabric I_T Nexus now.
		 */
		if (se_tpg->se_tpg_tfo->sess_get_initiator_sid != NULL) {
			memset(&buf[0], 0, PR_REG_ISID_LEN);
			se_tpg->se_tpg_tfo->sess_get_initiator_sid(se_sess,
					&buf[0], PR_REG_ISID_LEN);
			se_sess->sess_bin_isid = get_unaligned_be64(&buf[0]);
		}
<<<<<<< HEAD
		kref_get(&se_nacl->acl_kref);

		spin_lock_irq(&se_nacl->nacl_sess_lock);
=======

		spin_lock_irqsave(&se_nacl->nacl_sess_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * The se_nacl->nacl_sess pointer will be set to the
		 * last active I_T Nexus for each struct se_node_acl.
		 */
		se_nacl->nacl_sess = se_sess;

		list_add_tail(&se_sess->sess_acl_list,
			      &se_nacl->acl_sess_list);
<<<<<<< HEAD
		spin_unlock_irq(&se_nacl->nacl_sess_lock);
=======
		spin_unlock_irqrestore(&se_nacl->nacl_sess_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	list_add_tail(&se_sess->sess_list, &se_tpg->tpg_sess_list);

	pr_debug("TARGET_CORE[%s]: Registered fabric_sess_ptr: %p\n",
<<<<<<< HEAD
		se_tpg->se_tpg_tfo->get_fabric_name(), se_sess->fabric_sess_ptr);
=======
		se_tpg->se_tpg_tfo->fabric_name, se_sess->fabric_sess_ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(__transport_register_session);

void transport_register_session(
	struct se_portal_group *se_tpg,
	struct se_node_acl *se_nacl,
	struct se_session *se_sess,
	void *fabric_sess_ptr)
{
	unsigned long flags;

	spin_lock_irqsave(&se_tpg->session_lock, flags);
	__transport_register_session(se_tpg, se_nacl, se_sess, fabric_sess_ptr);
	spin_unlock_irqrestore(&se_tpg->session_lock, flags);
}
EXPORT_SYMBOL(transport_register_session);

<<<<<<< HEAD
static void target_release_session(struct kref *kref)
{
	struct se_session *se_sess = container_of(kref,
			struct se_session, sess_kref);
	struct se_portal_group *se_tpg = se_sess->se_tpg;

	se_tpg->se_tpg_tfo->close_session(se_sess);
}

void target_get_session(struct se_session *se_sess)
{
	kref_get(&se_sess->sess_kref);
}
EXPORT_SYMBOL(target_get_session);

int target_put_session(struct se_session *se_sess)
{
	return kref_put(&se_sess->sess_kref, target_release_session);
}
EXPORT_SYMBOL(target_put_session);
=======
struct se_session *
target_setup_session(struct se_portal_group *tpg,
		     unsigned int tag_num, unsigned int tag_size,
		     enum target_prot_op prot_op,
		     const char *initiatorname, void *private,
		     int (*callback)(struct se_portal_group *,
				     struct se_session *, void *))
{
	struct target_cmd_counter *cmd_cnt;
	struct se_session *sess;
	int rc;

	cmd_cnt = target_alloc_cmd_counter();
	if (!cmd_cnt)
		return ERR_PTR(-ENOMEM);
	/*
	 * If the fabric driver is using percpu-ida based pre allocation
	 * of I/O descriptor tags, go ahead and perform that setup now..
	 */
	if (tag_num != 0)
		sess = transport_init_session_tags(tag_num, tag_size, prot_op);
	else
		sess = transport_alloc_session(prot_op);

	if (IS_ERR(sess)) {
		rc = PTR_ERR(sess);
		goto free_cnt;
	}
	sess->cmd_cnt = cmd_cnt;

	sess->se_node_acl = core_tpg_check_initiator_node_acl(tpg,
					(unsigned char *)initiatorname);
	if (!sess->se_node_acl) {
		rc = -EACCES;
		goto free_sess;
	}
	/*
	 * Go ahead and perform any remaining fabric setup that is
	 * required before transport_register_session().
	 */
	if (callback != NULL) {
		rc = callback(tpg, sess, private);
		if (rc)
			goto free_sess;
	}

	transport_register_session(tpg, sess->se_node_acl, sess, private);
	return sess;

free_sess:
	transport_free_session(sess);
	return ERR_PTR(rc);

free_cnt:
	target_free_cmd_counter(cmd_cnt);
	return ERR_PTR(rc);
}
EXPORT_SYMBOL(target_setup_session);

ssize_t target_show_dynamic_sessions(struct se_portal_group *se_tpg, char *page)
{
	struct se_session *se_sess;
	ssize_t len = 0;

	spin_lock_bh(&se_tpg->session_lock);
	list_for_each_entry(se_sess, &se_tpg->tpg_sess_list, sess_list) {
		if (!se_sess->se_node_acl)
			continue;
		if (!se_sess->se_node_acl->dynamic_node_acl)
			continue;
		if (strlen(se_sess->se_node_acl->initiatorname) + 1 + len > PAGE_SIZE)
			break;

		len += snprintf(page + len, PAGE_SIZE - len, "%s\n",
				se_sess->se_node_acl->initiatorname);
		len += 1; /* Include NULL terminator */
	}
	spin_unlock_bh(&se_tpg->session_lock);

	return len;
}
EXPORT_SYMBOL(target_show_dynamic_sessions);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void target_complete_nacl(struct kref *kref)
{
	struct se_node_acl *nacl = container_of(kref,
				struct se_node_acl, acl_kref);
<<<<<<< HEAD

	complete(&nacl->acl_free_comp);
=======
	struct se_portal_group *se_tpg = nacl->se_tpg;

	if (!nacl->dynamic_stop) {
		complete(&nacl->acl_free_comp);
		return;
	}

	mutex_lock(&se_tpg->acl_node_mutex);
	list_del_init(&nacl->acl_list);
	mutex_unlock(&se_tpg->acl_node_mutex);

	core_tpg_wait_for_nacl_pr_ref(nacl);
	core_free_device_list_for_node(nacl, se_tpg);
	kfree(nacl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void target_put_nacl(struct se_node_acl *nacl)
{
	kref_put(&nacl->acl_kref, target_complete_nacl);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(target_put_nacl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void transport_deregister_session_configfs(struct se_session *se_sess)
{
	struct se_node_acl *se_nacl;
	unsigned long flags;
	/*
	 * Used by struct se_node_acl's under ConfigFS to locate active struct se_session
	 */
	se_nacl = se_sess->se_node_acl;
	if (se_nacl) {
		spin_lock_irqsave(&se_nacl->nacl_sess_lock, flags);
<<<<<<< HEAD
		if (se_nacl->acl_stop == 0)
			list_del(&se_sess->sess_acl_list);
=======
		if (!list_empty(&se_sess->sess_acl_list))
			list_del_init(&se_sess->sess_acl_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * If the session list is empty, then clear the pointer.
		 * Otherwise, set the struct se_session pointer from the tail
		 * element of the per struct se_node_acl active session list.
		 */
		if (list_empty(&se_nacl->acl_sess_list))
			se_nacl->nacl_sess = NULL;
		else {
			se_nacl->nacl_sess = container_of(
					se_nacl->acl_sess_list.prev,
					struct se_session, sess_acl_list);
		}
		spin_unlock_irqrestore(&se_nacl->nacl_sess_lock, flags);
	}
}
EXPORT_SYMBOL(transport_deregister_session_configfs);

void transport_free_session(struct se_session *se_sess)
{
<<<<<<< HEAD
=======
	struct se_node_acl *se_nacl = se_sess->se_node_acl;

	/*
	 * Drop the se_node_acl->nacl_kref obtained from within
	 * core_tpg_get_initiator_node_acl().
	 */
	if (se_nacl) {
		struct se_portal_group *se_tpg = se_nacl->se_tpg;
		const struct target_core_fabric_ops *se_tfo = se_tpg->se_tpg_tfo;
		unsigned long flags;

		se_sess->se_node_acl = NULL;

		/*
		 * Also determine if we need to drop the extra ->cmd_kref if
		 * it had been previously dynamically generated, and
		 * the endpoint is not caching dynamic ACLs.
		 */
		mutex_lock(&se_tpg->acl_node_mutex);
		if (se_nacl->dynamic_node_acl &&
		    !se_tfo->tpg_check_demo_mode_cache(se_tpg)) {
			spin_lock_irqsave(&se_nacl->nacl_sess_lock, flags);
			if (list_empty(&se_nacl->acl_sess_list))
				se_nacl->dynamic_stop = true;
			spin_unlock_irqrestore(&se_nacl->nacl_sess_lock, flags);

			if (se_nacl->dynamic_stop)
				list_del_init(&se_nacl->acl_list);
		}
		mutex_unlock(&se_tpg->acl_node_mutex);

		if (se_nacl->dynamic_stop)
			target_put_nacl(se_nacl);

		target_put_nacl(se_nacl);
	}
	if (se_sess->sess_cmd_map) {
		sbitmap_queue_free(&se_sess->sess_tag_pool);
		kvfree(se_sess->sess_cmd_map);
	}
	if (se_sess->cmd_cnt)
		target_free_cmd_counter(se_sess->cmd_cnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kmem_cache_free(se_sess_cache, se_sess);
}
EXPORT_SYMBOL(transport_free_session);

<<<<<<< HEAD
void transport_deregister_session(struct se_session *se_sess)
{
	struct se_portal_group *se_tpg = se_sess->se_tpg;
	struct target_core_fabric_ops *se_tfo;
	struct se_node_acl *se_nacl;
	unsigned long flags;
	bool comp_nacl = true;
=======
static int target_release_res(struct se_device *dev, void *data)
{
	struct se_session *sess = data;

	if (dev->reservation_holder == sess)
		target_release_reservation(dev);
	return 0;
}

void transport_deregister_session(struct se_session *se_sess)
{
	struct se_portal_group *se_tpg = se_sess->se_tpg;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!se_tpg) {
		transport_free_session(se_sess);
		return;
	}
<<<<<<< HEAD
	se_tfo = se_tpg->se_tpg_tfo;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&se_tpg->session_lock, flags);
	list_del(&se_sess->sess_list);
	se_sess->se_tpg = NULL;
	se_sess->fabric_sess_ptr = NULL;
	spin_unlock_irqrestore(&se_tpg->session_lock, flags);

	/*
<<<<<<< HEAD
	 * Determine if we need to do extra work for this initiator node's
	 * struct se_node_acl if it had been previously dynamically generated.
	 */
	se_nacl = se_sess->se_node_acl;

	spin_lock_irqsave(&se_tpg->acl_node_lock, flags);
	if (se_nacl && se_nacl->dynamic_node_acl) {
		if (!se_tfo->tpg_check_demo_mode_cache(se_tpg)) {
			list_del(&se_nacl->acl_list);
			se_tpg->num_node_acls--;
			spin_unlock_irqrestore(&se_tpg->acl_node_lock, flags);
			core_tpg_wait_for_nacl_pr_ref(se_nacl);
			core_free_device_list_for_node(se_nacl, se_tpg);
			se_tfo->tpg_release_fabric_acl(se_tpg, se_nacl);

			comp_nacl = false;
			spin_lock_irqsave(&se_tpg->acl_node_lock, flags);
		}
	}
	spin_unlock_irqrestore(&se_tpg->acl_node_lock, flags);

	pr_debug("TARGET_CORE[%s]: Deregistered fabric_sess\n",
		se_tpg->se_tpg_tfo->get_fabric_name());
	/*
	 * If last kref is dropping now for an explict NodeACL, awake sleeping
	 * ->acl_free_comp caller to wakeup configfs se_node_acl->acl_group
	 * removal context.
	 */
	if (se_nacl && comp_nacl == true)
		target_put_nacl(se_nacl);
=======
	 * Since the session is being removed, release SPC-2
	 * reservations held by the session that is disappearing.
	 */
	target_for_each_device(target_release_res, se_sess);

	pr_debug("TARGET_CORE[%s]: Deregistered fabric_sess\n",
		se_tpg->se_tpg_tfo->fabric_name);
	/*
	 * If last kref is dropping now for an explicit NodeACL, awake sleeping
	 * ->acl_free_comp caller to wakeup configfs se_node_acl->acl_group
	 * removal context from within transport_free_session() code.
	 *
	 * For dynamic ACL, target_put_nacl() uses target_complete_nacl()
	 * to release all remaining generate_node_acl=1 created ACL resources.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	transport_free_session(se_sess);
}
EXPORT_SYMBOL(transport_deregister_session);

<<<<<<< HEAD
/*
 * Called with cmd->t_state_lock held.
 */
static void transport_all_task_dev_remove_state(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	struct se_task *task;
=======
void target_remove_session(struct se_session *se_sess)
{
	transport_deregister_session_configfs(se_sess);
	transport_deregister_session(se_sess);
}
EXPORT_SYMBOL(target_remove_session);

static void target_remove_from_state_list(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	if (!dev)
		return;

<<<<<<< HEAD
	list_for_each_entry(task, &cmd->t_task_list, t_list) {
		if (task->task_flags & TF_ACTIVE)
			continue;

		spin_lock_irqsave(&dev->execute_task_lock, flags);
		if (task->t_state_active) {
			pr_debug("Removed ITT: 0x%08x dev: %p task[%p]\n",
				cmd->se_tfo->get_task_tag(cmd), dev, task);

			list_del(&task->t_state_list);
			atomic_dec(&cmd->t_task_cdbs_ex_left);
			task->t_state_active = false;
		}
		spin_unlock_irqrestore(&dev->execute_task_lock, flags);
	}

}

/*	transport_cmd_check_stop():
 *
 *	'transport_off = 1' determines if CMD_T_ACTIVE should be cleared.
 *	'transport_off = 2' determines if task_dev_state should be removed.
 *
 *	A non-zero u8 t_state sets cmd->t_state.
 *	Returns 1 when command is stopped, else 0.
 */
static int transport_cmd_check_stop(
	struct se_cmd *cmd,
	int transport_off,
	u8 t_state)
=======
	spin_lock_irqsave(&dev->queues[cmd->cpuid].lock, flags);
	if (cmd->state_active) {
		list_del(&cmd->state_list);
		cmd->state_active = false;
	}
	spin_unlock_irqrestore(&dev->queues[cmd->cpuid].lock, flags);
}

static void target_remove_from_tmr_list(struct se_cmd *cmd)
{
	struct se_device *dev = NULL;
	unsigned long flags;

	if (cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)
		dev = cmd->se_tmr_req->tmr_dev;

	if (dev) {
		spin_lock_irqsave(&dev->se_tmr_lock, flags);
		if (cmd->se_tmr_req->tmr_dev)
			list_del_init(&cmd->se_tmr_req->tmr_list);
		spin_unlock_irqrestore(&dev->se_tmr_lock, flags);
	}
}
/*
 * This function is called by the target core after the target core has
 * finished processing a SCSI command or SCSI TMF. Both the regular command
 * processing code and the code for aborting commands can call this
 * function. CMD_T_STOP is set if and only if another thread is waiting
 * inside transport_wait_for_tasks() for t_transport_stop_comp.
 */
static int transport_cmd_check_stop_to_fabric(struct se_cmd *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	/*
<<<<<<< HEAD
	 * Determine if IOCTL context caller in requesting the stopping of this
	 * command for LUN shutdown purposes.
	 */
	if (cmd->transport_state & CMD_T_LUN_STOP) {
		pr_debug("%s:%d CMD_T_LUN_STOP for ITT: 0x%08x\n",
			__func__, __LINE__, cmd->se_tfo->get_task_tag(cmd));

		cmd->transport_state &= ~CMD_T_ACTIVE;
		if (transport_off == 2)
			transport_all_task_dev_remove_state(cmd);
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		complete(&cmd->transport_lun_stop_comp);
		return 1;
	}
	/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Determine if frontend context caller is requesting the stopping of
	 * this command for frontend exceptions.
	 */
	if (cmd->transport_state & CMD_T_STOP) {
<<<<<<< HEAD
		pr_debug("%s:%d CMD_T_STOP for ITT: 0x%08x\n",
			__func__, __LINE__,
			cmd->se_tfo->get_task_tag(cmd));

		if (transport_off == 2)
			transport_all_task_dev_remove_state(cmd);

		/*
		 * Clear struct se_cmd->se_lun before the transport_off == 2 handoff
		 * to FE.
		 */
		if (transport_off == 2)
			cmd->se_lun = NULL;
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		complete(&cmd->t_transport_stop_comp);
		return 1;
	}
	if (transport_off) {
		cmd->transport_state &= ~CMD_T_ACTIVE;
		if (transport_off == 2) {
			transport_all_task_dev_remove_state(cmd);
			/*
			 * Clear struct se_cmd->se_lun before the transport_off == 2
			 * handoff to fabric module.
			 */
			cmd->se_lun = NULL;
			/*
			 * Some fabric modules like tcm_loop can release
			 * their internally allocated I/O reference now and
			 * struct se_cmd now.
			 *
			 * Fabric modules are expected to return '1' here if the
			 * se_cmd being passed is released at this point,
			 * or zero if not being released.
			 */
			if (cmd->se_tfo->check_stop_free != NULL) {
				spin_unlock_irqrestore(
					&cmd->t_state_lock, flags);

				return cmd->se_tfo->check_stop_free(cmd);
			}
		}
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		return 0;
	} else if (t_state)
		cmd->t_state = t_state;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	return 0;
}

static int transport_cmd_check_stop_to_fabric(struct se_cmd *cmd)
{
	return transport_cmd_check_stop(cmd, 2, 0);
=======
		pr_debug("%s:%d CMD_T_STOP for ITT: 0x%08llx\n",
			__func__, __LINE__, cmd->tag);

		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		complete_all(&cmd->t_transport_stop_comp);
		return 1;
	}
	cmd->transport_state &= ~CMD_T_ACTIVE;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	/*
	 * Some fabric modules like tcm_loop can release their internally
	 * allocated I/O reference and struct se_cmd now.
	 *
	 * Fabric modules are expected to return '1' here if the se_cmd being
	 * passed is released at this point, or zero if not being released.
	 */
	return cmd->se_tfo->check_stop_free(cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void transport_lun_remove_cmd(struct se_cmd *cmd)
{
	struct se_lun *lun = cmd->se_lun;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!lun)
		return;

<<<<<<< HEAD
	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->transport_state & CMD_T_DEV_ACTIVE) {
		cmd->transport_state &= ~CMD_T_DEV_ACTIVE;
		transport_all_task_dev_remove_state(cmd);
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	spin_lock_irqsave(&lun->lun_cmd_lock, flags);
	if (!list_empty(&cmd->se_lun_node))
		list_del_init(&cmd->se_lun_node);
	spin_unlock_irqrestore(&lun->lun_cmd_lock, flags);
}

void transport_cmd_finish_abort(struct se_cmd *cmd, int remove)
{
	if (!(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB))
		transport_lun_remove_cmd(cmd);

	if (transport_cmd_check_stop_to_fabric(cmd))
		return;
	if (remove) {
		transport_remove_cmd_from_queue(cmd);
		transport_put_cmd(cmd);
	}
}

static void transport_add_cmd_to_queue(struct se_cmd *cmd, int t_state,
		bool at_head)
{
	struct se_device *dev = cmd->se_dev;
	struct se_queue_obj *qobj = &dev->dev_queue_obj;
	unsigned long flags;

	if (t_state) {
		spin_lock_irqsave(&cmd->t_state_lock, flags);
		cmd->t_state = t_state;
		cmd->transport_state |= CMD_T_ACTIVE;
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
	}

	spin_lock_irqsave(&qobj->cmd_queue_lock, flags);

	/* If the cmd is already on the list, remove it before we add it */
	if (!list_empty(&cmd->se_queue_node))
		list_del(&cmd->se_queue_node);
	else
		atomic_inc(&qobj->queue_cnt);

	if (at_head)
		list_add(&cmd->se_queue_node, &qobj->qobj_list);
	else
		list_add_tail(&cmd->se_queue_node, &qobj->qobj_list);
	cmd->transport_state |= CMD_T_QUEUED;
	spin_unlock_irqrestore(&qobj->cmd_queue_lock, flags);

	wake_up_interruptible(&qobj->thread_wq);
}

static struct se_cmd *
transport_get_cmd_from_queue(struct se_queue_obj *qobj)
{
	struct se_cmd *cmd;
	unsigned long flags;

	spin_lock_irqsave(&qobj->cmd_queue_lock, flags);
	if (list_empty(&qobj->qobj_list)) {
		spin_unlock_irqrestore(&qobj->cmd_queue_lock, flags);
		return NULL;
	}
	cmd = list_first_entry(&qobj->qobj_list, struct se_cmd, se_queue_node);

	cmd->transport_state &= ~CMD_T_QUEUED;
	list_del_init(&cmd->se_queue_node);
	atomic_dec(&qobj->queue_cnt);
	spin_unlock_irqrestore(&qobj->cmd_queue_lock, flags);

	return cmd;
}

static void transport_remove_cmd_from_queue(struct se_cmd *cmd)
{
	struct se_queue_obj *qobj = &cmd->se_dev->dev_queue_obj;
	unsigned long flags;

	spin_lock_irqsave(&qobj->cmd_queue_lock, flags);
	if (!(cmd->transport_state & CMD_T_QUEUED)) {
		spin_unlock_irqrestore(&qobj->cmd_queue_lock, flags);
		return;
	}
	cmd->transport_state &= ~CMD_T_QUEUED;
	atomic_dec(&qobj->queue_cnt);
	list_del_init(&cmd->se_queue_node);
	spin_unlock_irqrestore(&qobj->cmd_queue_lock, flags);
}

/*
 * Completion function used by TCM subsystem plugins (such as FILEIO)
 * for queueing up response from struct se_subsystem_api->do_task()
 */
void transport_complete_sync_cache(struct se_cmd *cmd, int good)
{
	struct se_task *task = list_entry(cmd->t_task_list.next,
				struct se_task, t_list);

	if (good) {
		cmd->scsi_status = SAM_STAT_GOOD;
		task->task_scsi_status = GOOD;
	} else {
		task->task_scsi_status = SAM_STAT_CHECK_CONDITION;
		task->task_se_cmd->scsi_sense_reason =
				TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	}

	transport_complete_task(task, good);
}
EXPORT_SYMBOL(transport_complete_sync_cache);

=======
	target_remove_from_state_list(cmd);
	target_remove_from_tmr_list(cmd);

	if (cmpxchg(&cmd->lun_ref_active, true, false))
		percpu_ref_put(&lun->lun_ref);

	/*
	 * Clear struct se_cmd->se_lun before the handoff to FE.
	 */
	cmd->se_lun = NULL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void target_complete_failure_work(struct work_struct *work)
{
	struct se_cmd *cmd = container_of(work, struct se_cmd, work);

<<<<<<< HEAD
	transport_generic_request_failure(cmd);
}

/*	transport_complete_task():
 *
 *	Called from interrupt and non interrupt context depending
 *	on the transport plugin.
 */
void transport_complete_task(struct se_task *task, int success)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	task->task_flags &= ~TF_ACTIVE;

	/*
	 * See if any sense data exists, if so set the TASK_SENSE flag.
	 * Also check for any other post completion work that needs to be
	 * done by the plugins.
	 */
	if (dev && dev->transport->transport_complete) {
		if (dev->transport->transport_complete(task) != 0) {
			cmd->se_cmd_flags |= SCF_TRANSPORT_TASK_SENSE;
			task->task_flags |= TF_HAS_SENSE;
			success = 1;
		}
	}

	/*
	 * See if we are waiting for outstanding struct se_task
	 * to complete for an exception condition
	 */
	if (task->task_flags & TF_REQUEST_STOP) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		complete(&task->task_stop_comp);
		return;
	}

	if (!success)
		cmd->transport_state |= CMD_T_FAILED;

	/*
	 * Decrement the outstanding t_task_cdbs_left count.  The last
	 * struct se_task from struct se_cmd will complete itself into the
	 * device queue depending upon int success.
	 */
	if (!atomic_dec_and_test(&cmd->t_task_cdbs_left)) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return;
	}
	/*
	 * Check for case where an explict ABORT_TASK has been received
	 * and transport_wait_for_tasks() will be waiting for completion..
	 */
	if (cmd->transport_state & CMD_T_ABORTED &&
	    cmd->transport_state & CMD_T_STOP) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		complete(&cmd->t_transport_stop_comp);
		return;
	} else if (cmd->transport_state & CMD_T_FAILED) {
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		INIT_WORK(&cmd->work, target_complete_failure_work);
	} else {
		INIT_WORK(&cmd->work, target_complete_ok_work);
=======
	transport_generic_request_failure(cmd, cmd->sense_reason);
}

/*
 * Used when asking transport to copy Sense Data from the underlying
 * Linux/SCSI struct scsi_cmnd
 */
static unsigned char *transport_get_sense_buffer(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;

	WARN_ON(!cmd->se_lun);

	if (!dev)
		return NULL;

	if (cmd->se_cmd_flags & SCF_SENT_CHECK_CONDITION)
		return NULL;

	cmd->scsi_sense_length = TRANSPORT_SENSE_BUFFER;

	pr_debug("HBA_[%u]_PLUG[%s]: Requesting sense for SAM STATUS: 0x%02x\n",
		dev->se_hba->hba_id, dev->transport->name, cmd->scsi_status);
	return cmd->sense_buffer;
}

void transport_copy_sense_to_cmd(struct se_cmd *cmd, unsigned char *sense)
{
	unsigned char *cmd_sense_buf;
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	cmd_sense_buf = transport_get_sense_buffer(cmd);
	if (!cmd_sense_buf) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return;
	}

	cmd->se_cmd_flags |= SCF_TRANSPORT_TASK_SENSE;
	memcpy(cmd_sense_buf, sense, cmd->scsi_sense_length);
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);
}
EXPORT_SYMBOL(transport_copy_sense_to_cmd);

static void target_handle_abort(struct se_cmd *cmd)
{
	bool tas = cmd->transport_state & CMD_T_TAS;
	bool ack_kref = cmd->se_cmd_flags & SCF_ACK_KREF;
	int ret;

	pr_debug("tag %#llx: send_abort_response = %d\n", cmd->tag, tas);

	if (tas) {
		if (!(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)) {
			cmd->scsi_status = SAM_STAT_TASK_ABORTED;
			pr_debug("Setting SAM_STAT_TASK_ABORTED status for CDB: 0x%02x, ITT: 0x%08llx\n",
				 cmd->t_task_cdb[0], cmd->tag);
			trace_target_cmd_complete(cmd);
			ret = cmd->se_tfo->queue_status(cmd);
			if (ret) {
				transport_handle_queue_full(cmd, cmd->se_dev,
							    ret, false);
				return;
			}
		} else {
			cmd->se_tmr_req->response = TMR_FUNCTION_REJECTED;
			cmd->se_tfo->queue_tm_rsp(cmd);
		}
	} else {
		/*
		 * Allow the fabric driver to unmap any resources before
		 * releasing the descriptor via TFO->release_cmd().
		 */
		cmd->se_tfo->aborted_task(cmd);
		if (ack_kref)
			WARN_ON_ONCE(target_put_sess_cmd(cmd) != 0);
		/*
		 * To do: establish a unit attention condition on the I_T
		 * nexus associated with cmd. See also the paragraph "Aborting
		 * commands" in SAM.
		 */
	}

	WARN_ON_ONCE(kref_read(&cmd->cmd_kref) == 0);

	transport_lun_remove_cmd(cmd);

	transport_cmd_check_stop_to_fabric(cmd);
}

static void target_abort_work(struct work_struct *work)
{
	struct se_cmd *cmd = container_of(work, struct se_cmd, work);

	target_handle_abort(cmd);
}

static bool target_cmd_interrupted(struct se_cmd *cmd)
{
	int post_ret;

	if (cmd->transport_state & CMD_T_ABORTED) {
		if (cmd->transport_complete_callback)
			cmd->transport_complete_callback(cmd, false, &post_ret);
		INIT_WORK(&cmd->work, target_abort_work);
		queue_work(target_completion_wq, &cmd->work);
		return true;
	} else if (cmd->transport_state & CMD_T_STOP) {
		if (cmd->transport_complete_callback)
			cmd->transport_complete_callback(cmd, false, &post_ret);
		complete_all(&cmd->t_transport_stop_comp);
		return true;
	}

	return false;
}

/* May be called from interrupt context so must not sleep. */
void target_complete_cmd_with_sense(struct se_cmd *cmd, u8 scsi_status,
				    sense_reason_t sense_reason)
{
	struct se_wwn *wwn = cmd->se_sess->se_tpg->se_tpg_wwn;
	int success, cpu;
	unsigned long flags;

	if (target_cmd_interrupted(cmd))
		return;

	cmd->scsi_status = scsi_status;
	cmd->sense_reason = sense_reason;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	switch (cmd->scsi_status) {
	case SAM_STAT_CHECK_CONDITION:
		if (cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE)
			success = 1;
		else
			success = 0;
		break;
	default:
		success = 1;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	cmd->t_state = TRANSPORT_COMPLETE;
	cmd->transport_state |= (CMD_T_COMPLETE | CMD_T_ACTIVE);
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

<<<<<<< HEAD
	queue_work(target_completion_wq, &cmd->work);
}
EXPORT_SYMBOL(transport_complete_task);

/*
 * Called by transport_add_tasks_from_cmd() once a struct se_cmd's
 * struct se_task list are ready to be added to the active execution list
 * struct se_device

 * Called with se_dev_t->execute_task_lock called.
 */
static inline int transport_add_task_check_sam_attr(
	struct se_task *task,
	struct se_task *task_prev,
	struct se_device *dev)
{
	/*
	 * No SAM Task attribute emulation enabled, add to tail of
	 * execution queue
	 */
	if (dev->dev_task_attr_type != SAM_TASK_ATTR_EMULATED) {
		list_add_tail(&task->t_execute_list, &dev->execute_task_list);
		return 0;
	}
	/*
	 * HEAD_OF_QUEUE attribute for received CDB, which means
	 * the first task that is associated with a struct se_cmd goes to
	 * head of the struct se_device->execute_task_list, and task_prev
	 * after that for each subsequent task
	 */
	if (task->task_se_cmd->sam_task_attr == MSG_HEAD_TAG) {
		list_add(&task->t_execute_list,
				(task_prev != NULL) ?
				&task_prev->t_execute_list :
				&dev->execute_task_list);

		pr_debug("Set HEAD_OF_QUEUE for task CDB: 0x%02x"
				" in execution queue\n",
				task->task_se_cmd->t_task_cdb[0]);
		return 1;
	}
	/*
	 * For ORDERED, SIMPLE or UNTAGGED attribute tasks once they have been
	 * transitioned from Dermant -> Active state, and are added to the end
	 * of the struct se_device->execute_task_list
	 */
	list_add_tail(&task->t_execute_list, &dev->execute_task_list);
	return 0;
}

/*	__transport_add_task_to_execute_queue():
 *
 *	Called with se_dev_t->execute_task_lock called.
 */
static void __transport_add_task_to_execute_queue(
	struct se_task *task,
	struct se_task *task_prev,
	struct se_device *dev)
{
	int head_of_queue;

	head_of_queue = transport_add_task_check_sam_attr(task, task_prev, dev);
	atomic_inc(&dev->execute_tasks);

	if (task->t_state_active)
		return;
	/*
	 * Determine if this task needs to go to HEAD_OF_QUEUE for the
	 * state list as well.  Running with SAM Task Attribute emulation
	 * will always return head_of_queue == 0 here
	 */
	if (head_of_queue)
		list_add(&task->t_state_list, (task_prev) ?
				&task_prev->t_state_list :
				&dev->state_task_list);
	else
		list_add_tail(&task->t_state_list, &dev->state_task_list);

	task->t_state_active = true;

	pr_debug("Added ITT: 0x%08x task[%p] to dev: %p\n",
		task->task_se_cmd->se_tfo->get_task_tag(task->task_se_cmd),
		task, dev);
}

static void transport_add_tasks_to_state_queue(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	struct se_task *task;
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	list_for_each_entry(task, &cmd->t_task_list, t_list) {
		spin_lock(&dev->execute_task_lock);
		if (!task->t_state_active) {
			list_add_tail(&task->t_state_list,
				      &dev->state_task_list);
			task->t_state_active = true;

			pr_debug("Added ITT: 0x%08x task[%p] to dev: %p\n",
				task->task_se_cmd->se_tfo->get_task_tag(
				task->task_se_cmd), task, dev);
		}
		spin_unlock(&dev->execute_task_lock);
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);
}

static void __transport_add_tasks_from_cmd(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	struct se_task *task, *task_prev = NULL;

	list_for_each_entry(task, &cmd->t_task_list, t_list) {
		if (!list_empty(&task->t_execute_list))
			continue;
		/*
		 * __transport_add_task_to_execute_queue() handles the
		 * SAM Task Attribute emulation if enabled
		 */
		__transport_add_task_to_execute_queue(task, task_prev, dev);
		task_prev = task;
	}
}

static void transport_add_tasks_from_cmd(struct se_cmd *cmd)
{
	unsigned long flags;
	struct se_device *dev = cmd->se_dev;

	spin_lock_irqsave(&dev->execute_task_lock, flags);
	__transport_add_tasks_from_cmd(cmd);
	spin_unlock_irqrestore(&dev->execute_task_lock, flags);
}

void __transport_remove_task_from_execute_queue(struct se_task *task,
		struct se_device *dev)
{
	list_del_init(&task->t_execute_list);
	atomic_dec(&dev->execute_tasks);
}

static void transport_remove_task_from_execute_queue(
	struct se_task *task,
	struct se_device *dev)
{
	unsigned long flags;

	if (WARN_ON(list_empty(&task->t_execute_list)))
		return;

	spin_lock_irqsave(&dev->execute_task_lock, flags);
	__transport_remove_task_from_execute_queue(task, dev);
	spin_unlock_irqrestore(&dev->execute_task_lock, flags);
=======
	INIT_WORK(&cmd->work, success ? target_complete_ok_work :
		  target_complete_failure_work);

	if (!wwn || wwn->cmd_compl_affinity == SE_COMPL_AFFINITY_CPUID)
		cpu = cmd->cpuid;
	else
		cpu = wwn->cmd_compl_affinity;

	queue_work_on(cpu, target_completion_wq, &cmd->work);
}
EXPORT_SYMBOL(target_complete_cmd_with_sense);

void target_complete_cmd(struct se_cmd *cmd, u8 scsi_status)
{
	target_complete_cmd_with_sense(cmd, scsi_status, scsi_status ?
			      TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE :
			      TCM_NO_SENSE);
}
EXPORT_SYMBOL(target_complete_cmd);

void target_set_cmd_data_length(struct se_cmd *cmd, int length)
{
	if (length < cmd->data_length) {
		if (cmd->se_cmd_flags & SCF_UNDERFLOW_BIT) {
			cmd->residual_count += cmd->data_length - length;
		} else {
			cmd->se_cmd_flags |= SCF_UNDERFLOW_BIT;
			cmd->residual_count = cmd->data_length - length;
		}

		cmd->data_length = length;
	}
}
EXPORT_SYMBOL(target_set_cmd_data_length);

void target_complete_cmd_with_length(struct se_cmd *cmd, u8 scsi_status, int length)
{
	if (scsi_status == SAM_STAT_GOOD ||
	    cmd->se_cmd_flags & SCF_TREAT_READ_AS_NORMAL) {
		target_set_cmd_data_length(cmd, length);
	}

	target_complete_cmd(cmd, scsi_status);
}
EXPORT_SYMBOL(target_complete_cmd_with_length);

static void target_add_to_state_list(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->queues[cmd->cpuid].lock, flags);
	if (!cmd->state_active) {
		list_add_tail(&cmd->state_list,
			      &dev->queues[cmd->cpuid].state_list);
		cmd->state_active = true;
	}
	spin_unlock_irqrestore(&dev->queues[cmd->cpuid].lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Handle QUEUE_FULL / -EAGAIN and -ENOMEM status
 */
<<<<<<< HEAD

static void target_qf_do_work(struct work_struct *work)
=======
static void transport_write_pending_qf(struct se_cmd *cmd);
static void transport_complete_qf(struct se_cmd *cmd);

void target_qf_do_work(struct work_struct *work)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct se_device *dev = container_of(work, struct se_device,
					qf_work_queue);
	LIST_HEAD(qf_cmd_list);
	struct se_cmd *cmd, *cmd_tmp;

	spin_lock_irq(&dev->qf_cmd_lock);
	list_splice_init(&dev->qf_cmd_list, &qf_cmd_list);
	spin_unlock_irq(&dev->qf_cmd_lock);

	list_for_each_entry_safe(cmd, cmd_tmp, &qf_cmd_list, se_qf_node) {
		list_del(&cmd->se_qf_node);
<<<<<<< HEAD
		atomic_dec(&dev->dev_qf_count);
		smp_mb__after_atomic_dec();

		pr_debug("Processing %s cmd: %p QUEUE_FULL in work queue"
			" context: %s\n", cmd->se_tfo->get_fabric_name(), cmd,
=======
		atomic_dec_mb(&dev->dev_qf_count);

		pr_debug("Processing %s cmd: %p QUEUE_FULL in work queue"
			" context: %s\n", cmd->se_tfo->fabric_name, cmd,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			(cmd->t_state == TRANSPORT_COMPLETE_QF_OK) ? "COMPLETE_OK" :
			(cmd->t_state == TRANSPORT_COMPLETE_QF_WP) ? "WRITE_PENDING"
			: "UNKNOWN");

<<<<<<< HEAD
		transport_add_cmd_to_queue(cmd, cmd->t_state, true);
=======
		if (cmd->t_state == TRANSPORT_COMPLETE_QF_WP)
			transport_write_pending_qf(cmd);
		else if (cmd->t_state == TRANSPORT_COMPLETE_QF_OK ||
			 cmd->t_state == TRANSPORT_COMPLETE_QF_ERR)
			transport_complete_qf(cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

unsigned char *transport_dump_cmd_direction(struct se_cmd *cmd)
{
	switch (cmd->data_direction) {
	case DMA_NONE:
		return "NONE";
	case DMA_FROM_DEVICE:
		return "READ";
	case DMA_TO_DEVICE:
		return "WRITE";
	case DMA_BIDIRECTIONAL:
		return "BIDI";
	default:
		break;
	}

	return "UNKNOWN";
}

void transport_dump_dev_state(
	struct se_device *dev,
	char *b,
	int *bl)
{
	*bl += sprintf(b + *bl, "Status: ");
<<<<<<< HEAD
	switch (dev->dev_status) {
	case TRANSPORT_DEVICE_ACTIVATED:
		*bl += sprintf(b + *bl, "ACTIVATED");
		break;
	case TRANSPORT_DEVICE_DEACTIVATED:
		*bl += sprintf(b + *bl, "DEACTIVATED");
		break;
	case TRANSPORT_DEVICE_SHUTDOWN:
		*bl += sprintf(b + *bl, "SHUTDOWN");
		break;
	case TRANSPORT_DEVICE_OFFLINE_ACTIVATED:
	case TRANSPORT_DEVICE_OFFLINE_DEACTIVATED:
		*bl += sprintf(b + *bl, "OFFLINE");
		break;
	default:
		*bl += sprintf(b + *bl, "UNKNOWN=%d", dev->dev_status);
		break;
	}

	*bl += sprintf(b + *bl, "  Execute/Max Queue Depth: %d/%d",
		atomic_read(&dev->execute_tasks), dev->queue_depth);
	*bl += sprintf(b + *bl, "  SectorSize: %u  MaxSectors: %u\n",
		dev->se_sub_dev->se_dev_attrib.block_size, dev->se_sub_dev->se_dev_attrib.max_sectors);
=======
	if (dev->export_count)
		*bl += sprintf(b + *bl, "ACTIVATED");
	else
		*bl += sprintf(b + *bl, "DEACTIVATED");

	*bl += sprintf(b + *bl, "  Max Queue Depth: %d", dev->queue_depth);
	*bl += sprintf(b + *bl, "  SectorSize: %u  HwMaxSectors: %u\n",
		dev->dev_attrib.block_size,
		dev->dev_attrib.hw_max_sectors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*bl += sprintf(b + *bl, "        ");
}

void transport_dump_vpd_proto_id(
	struct t10_vpd *vpd,
	unsigned char *p_buf,
	int p_buf_len)
{
	unsigned char buf[VPD_TMP_BUF_SIZE];
	int len;

	memset(buf, 0, VPD_TMP_BUF_SIZE);
	len = sprintf(buf, "T10 VPD Protocol Identifier: ");

	switch (vpd->protocol_identifier) {
	case 0x00:
		sprintf(buf+len, "Fibre Channel\n");
		break;
	case 0x10:
		sprintf(buf+len, "Parallel SCSI\n");
		break;
	case 0x20:
		sprintf(buf+len, "SSA\n");
		break;
	case 0x30:
		sprintf(buf+len, "IEEE 1394\n");
		break;
	case 0x40:
		sprintf(buf+len, "SCSI Remote Direct Memory Access"
				" Protocol\n");
		break;
	case 0x50:
		sprintf(buf+len, "Internet SCSI (iSCSI)\n");
		break;
	case 0x60:
		sprintf(buf+len, "SAS Serial SCSI Protocol\n");
		break;
	case 0x70:
		sprintf(buf+len, "Automation/Drive Interface Transport"
				" Protocol\n");
		break;
	case 0x80:
		sprintf(buf+len, "AT Attachment Interface ATA/ATAPI\n");
		break;
	default:
		sprintf(buf+len, "Unknown 0x%02x\n",
				vpd->protocol_identifier);
		break;
	}

	if (p_buf)
		strncpy(p_buf, buf, p_buf_len);
	else
		pr_debug("%s", buf);
}

void
transport_set_vpd_proto_id(struct t10_vpd *vpd, unsigned char *page_83)
{
	/*
	 * Check if the Protocol Identifier Valid (PIV) bit is set..
	 *
	 * from spc3r23.pdf section 7.5.1
	 */
	 if (page_83[1] & 0x80) {
		vpd->protocol_identifier = (page_83[0] & 0xf0);
		vpd->protocol_identifier_set = 1;
		transport_dump_vpd_proto_id(vpd, NULL, 0);
	}
}
EXPORT_SYMBOL(transport_set_vpd_proto_id);

int transport_dump_vpd_assoc(
	struct t10_vpd *vpd,
	unsigned char *p_buf,
	int p_buf_len)
{
	unsigned char buf[VPD_TMP_BUF_SIZE];
	int ret = 0;
	int len;

	memset(buf, 0, VPD_TMP_BUF_SIZE);
	len = sprintf(buf, "T10 VPD Identifier Association: ");

	switch (vpd->association) {
	case 0x00:
		sprintf(buf+len, "addressed logical unit\n");
		break;
	case 0x10:
		sprintf(buf+len, "target port\n");
		break;
	case 0x20:
		sprintf(buf+len, "SCSI target device\n");
		break;
	default:
		sprintf(buf+len, "Unknown 0x%02x\n", vpd->association);
		ret = -EINVAL;
		break;
	}

	if (p_buf)
		strncpy(p_buf, buf, p_buf_len);
	else
		pr_debug("%s", buf);

	return ret;
}

int transport_set_vpd_assoc(struct t10_vpd *vpd, unsigned char *page_83)
{
	/*
	 * The VPD identification association..
	 *
	 * from spc3r23.pdf Section 7.6.3.1 Table 297
	 */
	vpd->association = (page_83[1] & 0x30);
	return transport_dump_vpd_assoc(vpd, NULL, 0);
}
EXPORT_SYMBOL(transport_set_vpd_assoc);

int transport_dump_vpd_ident_type(
	struct t10_vpd *vpd,
	unsigned char *p_buf,
	int p_buf_len)
{
	unsigned char buf[VPD_TMP_BUF_SIZE];
	int ret = 0;
	int len;

	memset(buf, 0, VPD_TMP_BUF_SIZE);
	len = sprintf(buf, "T10 VPD Identifier Type: ");

	switch (vpd->device_identifier_type) {
	case 0x00:
		sprintf(buf+len, "Vendor specific\n");
		break;
	case 0x01:
		sprintf(buf+len, "T10 Vendor ID based\n");
		break;
	case 0x02:
		sprintf(buf+len, "EUI-64 based\n");
		break;
	case 0x03:
		sprintf(buf+len, "NAA\n");
		break;
	case 0x04:
		sprintf(buf+len, "Relative target port identifier\n");
		break;
	case 0x08:
		sprintf(buf+len, "SCSI name string\n");
		break;
	default:
		sprintf(buf+len, "Unsupported: 0x%02x\n",
				vpd->device_identifier_type);
		ret = -EINVAL;
		break;
	}

	if (p_buf) {
		if (p_buf_len < strlen(buf)+1)
			return -EINVAL;
		strncpy(p_buf, buf, p_buf_len);
	} else {
		pr_debug("%s", buf);
	}

	return ret;
}

int transport_set_vpd_ident_type(struct t10_vpd *vpd, unsigned char *page_83)
{
	/*
	 * The VPD identifier type..
	 *
	 * from spc3r23.pdf Section 7.6.3.1 Table 298
	 */
	vpd->device_identifier_type = (page_83[1] & 0x0f);
	return transport_dump_vpd_ident_type(vpd, NULL, 0);
}
EXPORT_SYMBOL(transport_set_vpd_ident_type);

int transport_dump_vpd_ident(
	struct t10_vpd *vpd,
	unsigned char *p_buf,
	int p_buf_len)
{
	unsigned char buf[VPD_TMP_BUF_SIZE];
	int ret = 0;

	memset(buf, 0, VPD_TMP_BUF_SIZE);

	switch (vpd->device_identifier_code_set) {
	case 0x01: /* Binary */
<<<<<<< HEAD
		sprintf(buf, "T10 VPD Binary Device Identifier: %s\n",
			&vpd->device_identifier[0]);
		break;
	case 0x02: /* ASCII */
		sprintf(buf, "T10 VPD ASCII Device Identifier: %s\n",
			&vpd->device_identifier[0]);
		break;
	case 0x03: /* UTF-8 */
		sprintf(buf, "T10 VPD UTF-8 Device Identifier: %s\n",
=======
		snprintf(buf, sizeof(buf),
			"T10 VPD Binary Device Identifier: %s\n",
			&vpd->device_identifier[0]);
		break;
	case 0x02: /* ASCII */
		snprintf(buf, sizeof(buf),
			"T10 VPD ASCII Device Identifier: %s\n",
			&vpd->device_identifier[0]);
		break;
	case 0x03: /* UTF-8 */
		snprintf(buf, sizeof(buf),
			"T10 VPD UTF-8 Device Identifier: %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			&vpd->device_identifier[0]);
		break;
	default:
		sprintf(buf, "T10 VPD Device Identifier encoding unsupported:"
			" 0x%02x", vpd->device_identifier_code_set);
		ret = -EINVAL;
		break;
	}

	if (p_buf)
		strncpy(p_buf, buf, p_buf_len);
	else
		pr_debug("%s", buf);

	return ret;
}

int
transport_set_vpd_ident(struct t10_vpd *vpd, unsigned char *page_83)
{
	static const char hex_str[] = "0123456789abcdef";
<<<<<<< HEAD
	int j = 0, i = 4; /* offset to start of the identifer */
=======
	int j = 0, i = 4; /* offset to start of the identifier */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The VPD Code Set (encoding)
	 *
	 * from spc3r23.pdf Section 7.6.3.1 Table 296
	 */
	vpd->device_identifier_code_set = (page_83[0] & 0x0f);
	switch (vpd->device_identifier_code_set) {
	case 0x01: /* Binary */
		vpd->device_identifier[j++] =
				hex_str[vpd->device_identifier_type];
		while (i < (4 + page_83[3])) {
			vpd->device_identifier[j++] =
				hex_str[(page_83[i] & 0xf0) >> 4];
			vpd->device_identifier[j++] =
				hex_str[page_83[i] & 0x0f];
			i++;
		}
		break;
	case 0x02: /* ASCII */
	case 0x03: /* UTF-8 */
		while (i < (4 + page_83[3]))
			vpd->device_identifier[j++] = page_83[i++];
		break;
	default:
		break;
	}

	return transport_dump_vpd_ident(vpd, NULL, 0);
}
EXPORT_SYMBOL(transport_set_vpd_ident);

<<<<<<< HEAD
static void core_setup_task_attr_emulation(struct se_device *dev)
{
	/*
	 * If this device is from Target_Core_Mod/pSCSI, disable the
	 * SAM Task Attribute emulation.
	 *
	 * This is currently not available in upsream Linux/SCSI Target
	 * mode code, and is assumed to be disabled while using TCM/pSCSI.
	 */
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		dev->dev_task_attr_type = SAM_TASK_ATTR_PASSTHROUGH;
		return;
	}

	dev->dev_task_attr_type = SAM_TASK_ATTR_EMULATED;
	pr_debug("%s: Using SAM_TASK_ATTR_EMULATED for SPC: 0x%02x"
		" device\n", dev->transport->name,
		dev->transport->get_device_rev(dev));
}

static void scsi_dump_inquiry(struct se_device *dev)
{
	struct t10_wwn *wwn = &dev->se_sub_dev->t10_wwn;
	char buf[17];
	int i, device_type;
	/*
	 * Print Linux/SCSI style INQUIRY formatting to the kernel ring buffer
	 */
	for (i = 0; i < 8; i++)
		if (wwn->vendor[i] >= 0x20)
			buf[i] = wwn->vendor[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Vendor: %s\n", buf);

	for (i = 0; i < 16; i++)
		if (wwn->model[i] >= 0x20)
			buf[i] = wwn->model[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Model: %s\n", buf);

	for (i = 0; i < 4; i++)
		if (wwn->revision[i] >= 0x20)
			buf[i] = wwn->revision[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Revision: %s\n", buf);

	device_type = dev->transport->get_device_type(dev);
	pr_debug("  Type:   %s ", scsi_device_type(device_type));
	pr_debug("                 ANSI SCSI revision: %02x\n",
				dev->transport->get_device_rev(dev));
}

struct se_device *transport_add_device_to_core_hba(
	struct se_hba *hba,
	struct se_subsystem_api *transport,
	struct se_subsystem_dev *se_dev,
	u32 device_flags,
	void *transport_dev,
	struct se_dev_limits *dev_limits,
	const char *inquiry_prod,
	const char *inquiry_rev)
{
	int force_pt;
	struct se_device  *dev;

	dev = kzalloc(sizeof(struct se_device), GFP_KERNEL);
	if (!dev) {
		pr_err("Unable to allocate memory for se_dev_t\n");
		return NULL;
	}

	transport_init_queue_obj(&dev->dev_queue_obj);
	dev->dev_flags		= device_flags;
	dev->dev_status		|= TRANSPORT_DEVICE_DEACTIVATED;
	dev->dev_ptr		= transport_dev;
	dev->se_hba		= hba;
	dev->se_sub_dev		= se_dev;
	dev->transport		= transport;
	dev->dev_link_magic	= SE_DEV_LINK_MAGIC;
	INIT_LIST_HEAD(&dev->dev_list);
	INIT_LIST_HEAD(&dev->dev_sep_list);
	INIT_LIST_HEAD(&dev->dev_tmr_list);
	INIT_LIST_HEAD(&dev->execute_task_list);
	INIT_LIST_HEAD(&dev->delayed_cmd_list);
	INIT_LIST_HEAD(&dev->state_task_list);
	INIT_LIST_HEAD(&dev->qf_cmd_list);
	spin_lock_init(&dev->execute_task_lock);
	spin_lock_init(&dev->delayed_cmd_lock);
	spin_lock_init(&dev->dev_reservation_lock);
	spin_lock_init(&dev->dev_status_lock);
	spin_lock_init(&dev->se_port_lock);
	spin_lock_init(&dev->se_tmr_lock);
	spin_lock_init(&dev->qf_cmd_lock);
	atomic_set(&dev->dev_ordered_id, 0);

	se_dev_set_default_attribs(dev, dev_limits);

	dev->dev_index = scsi_get_new_index(SCSI_DEVICE_INDEX);
	dev->creation_time = get_jiffies_64();
	spin_lock_init(&dev->stats_lock);

	spin_lock(&hba->device_lock);
	list_add_tail(&dev->dev_list, &hba->hba_dev_list);
	hba->dev_count++;
	spin_unlock(&hba->device_lock);
	/*
	 * Setup the SAM Task Attribute emulation for struct se_device
	 */
	core_setup_task_attr_emulation(dev);
	/*
	 * Force PR and ALUA passthrough emulation with internal object use.
	 */
	force_pt = (hba->hba_flags & HBA_FLAGS_INTERNAL_USE);
	/*
	 * Setup the Reservations infrastructure for struct se_device
	 */
	core_setup_reservations(dev, force_pt);
	/*
	 * Setup the Asymmetric Logical Unit Assignment for struct se_device
	 */
	if (core_setup_alua(dev, force_pt) < 0)
		goto out;

	/*
	 * Startup the struct se_device processing thread
	 */
	dev->process_thread = kthread_run(transport_processing_thread, dev,
					  "LIO_%s", dev->transport->name);
	if (IS_ERR(dev->process_thread)) {
		pr_err("Unable to create kthread: LIO_%s\n",
			dev->transport->name);
		goto out;
	}
	/*
	 * Setup work_queue for QUEUE_FULL
	 */
	INIT_WORK(&dev->qf_work_queue, target_qf_do_work);
	/*
	 * Preload the initial INQUIRY const values if we are doing
	 * anything virtual (IBLOCK, FILEIO, RAMDISK), but not for TCM/pSCSI
	 * passthrough because this is being provided by the backend LLD.
	 * This is required so that transport_get_inquiry() copies these
	 * originals once back into DEV_T10_WWN(dev) for the virtual device
	 * setup.
	 */
	if (dev->transport->transport_type != TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (!inquiry_prod || !inquiry_rev) {
			pr_err("All non TCM/pSCSI plugins require"
				" INQUIRY consts\n");
			goto out;
		}

		strncpy(&dev->se_sub_dev->t10_wwn.vendor[0], "LIO-ORG", 8);
		strncpy(&dev->se_sub_dev->t10_wwn.model[0], inquiry_prod, 16);
		strncpy(&dev->se_sub_dev->t10_wwn.revision[0], inquiry_rev, 4);
	}
	scsi_dump_inquiry(dev);

	return dev;
out:
	kthread_stop(dev->process_thread);

	spin_lock(&hba->device_lock);
	list_del(&dev->dev_list);
	hba->dev_count--;
	spin_unlock(&hba->device_lock);

	se_release_vpd_for_dev(dev);

	kfree(dev);

	return NULL;
}
EXPORT_SYMBOL(transport_add_device_to_core_hba);

/*	transport_generic_prepare_cdb():
 *
 *	Since the Initiator sees iSCSI devices as LUNs,  the SCSI CDB will
 *	contain the iSCSI LUN in bits 7-5 of byte 1 as per SAM-2.
 *	The point of this is since we are mapping iSCSI LUNs to
 *	SCSI Target IDs having a non-zero LUN in the CDB will throw the
 *	devices and HBAs for a loop.
 */
static inline void transport_generic_prepare_cdb(
	unsigned char *cdb)
{
	switch (cdb[0]) {
	case READ_10: /* SBC - RDProtect */
	case READ_12: /* SBC - RDProtect */
	case READ_16: /* SBC - RDProtect */
	case SEND_DIAGNOSTIC: /* SPC - SELF-TEST Code */
	case VERIFY: /* SBC - VRProtect */
	case VERIFY_16: /* SBC - VRProtect */
	case WRITE_VERIFY: /* SBC - VRProtect */
	case WRITE_VERIFY_12: /* SBC - VRProtect */
	case MAINTENANCE_IN: /* SPC - Parameter Data Format for SA RTPG */
		break;
	default:
		cdb[1] &= 0x1f; /* clear logical unit number */
		break;
	}
}

static struct se_task *
transport_generic_get_task(struct se_cmd *cmd,
		enum dma_data_direction data_direction)
{
	struct se_task *task;
	struct se_device *dev = cmd->se_dev;

	task = dev->transport->alloc_task(cmd->t_task_cdb);
	if (!task) {
		pr_err("Unable to allocate struct se_task\n");
		return NULL;
	}

	INIT_LIST_HEAD(&task->t_list);
	INIT_LIST_HEAD(&task->t_execute_list);
	INIT_LIST_HEAD(&task->t_state_list);
	init_completion(&task->task_stop_comp);
	task->task_se_cmd = cmd;
	task->task_data_direction = data_direction;

	return task;
}

static int transport_generic_cmd_sequencer(struct se_cmd *, unsigned char *);

/*
 * Used by fabric modules containing a local struct se_cmd within their
 * fabric dependent per I/O descriptor.
 */
void transport_init_se_cmd(
	struct se_cmd *cmd,
	struct target_core_fabric_ops *tfo,
	struct se_session *se_sess,
	u32 data_length,
	int data_direction,
	int task_attr,
	unsigned char *sense_buffer)
{
	INIT_LIST_HEAD(&cmd->se_lun_node);
	INIT_LIST_HEAD(&cmd->se_delayed_node);
	INIT_LIST_HEAD(&cmd->se_qf_node);
	INIT_LIST_HEAD(&cmd->se_queue_node);
	INIT_LIST_HEAD(&cmd->se_cmd_list);
	INIT_LIST_HEAD(&cmd->t_task_list);
	init_completion(&cmd->transport_lun_fe_stop_comp);
	init_completion(&cmd->transport_lun_stop_comp);
	init_completion(&cmd->t_transport_stop_comp);
	init_completion(&cmd->cmd_wait_comp);
	spin_lock_init(&cmd->t_state_lock);
	cmd->transport_state = CMD_T_DEV_ACTIVE;

=======
static sense_reason_t
target_check_max_data_sg_nents(struct se_cmd *cmd, struct se_device *dev,
			       unsigned int size)
{
	u32 mtl;

	if (!cmd->se_tfo->max_data_sg_nents)
		return TCM_NO_SENSE;
	/*
	 * Check if fabric enforced maximum SGL entries per I/O descriptor
	 * exceeds se_cmd->data_length.  If true, set SCF_UNDERFLOW_BIT +
	 * residual_count and reduce original cmd->data_length to maximum
	 * length based on single PAGE_SIZE entry scatter-lists.
	 */
	mtl = (cmd->se_tfo->max_data_sg_nents * PAGE_SIZE);
	if (cmd->data_length > mtl) {
		/*
		 * If an existing CDB overflow is present, calculate new residual
		 * based on CDB size minus fabric maximum transfer length.
		 *
		 * If an existing CDB underflow is present, calculate new residual
		 * based on original cmd->data_length minus fabric maximum transfer
		 * length.
		 *
		 * Otherwise, set the underflow residual based on cmd->data_length
		 * minus fabric maximum transfer length.
		 */
		if (cmd->se_cmd_flags & SCF_OVERFLOW_BIT) {
			cmd->residual_count = (size - mtl);
		} else if (cmd->se_cmd_flags & SCF_UNDERFLOW_BIT) {
			u32 orig_dl = size + cmd->residual_count;
			cmd->residual_count = (orig_dl - mtl);
		} else {
			cmd->se_cmd_flags |= SCF_UNDERFLOW_BIT;
			cmd->residual_count = (cmd->data_length - mtl);
		}
		cmd->data_length = mtl;
		/*
		 * Reset sbc_check_prot() calculated protection payload
		 * length based upon the new smaller MTL.
		 */
		if (cmd->prot_length) {
			u32 sectors = (mtl / dev->dev_attrib.block_size);
			cmd->prot_length = dev->prot_length * sectors;
		}
	}
	return TCM_NO_SENSE;
}

/**
 * target_cmd_size_check - Check whether there will be a residual.
 * @cmd: SCSI command.
 * @size: Data buffer size derived from CDB. The data buffer size provided by
 *   the SCSI transport driver is available in @cmd->data_length.
 *
 * Compare the data buffer size from the CDB with the data buffer limit from the transport
 * header. Set @cmd->residual_count and SCF_OVERFLOW_BIT or SCF_UNDERFLOW_BIT if necessary.
 *
 * Note: target drivers set @cmd->data_length by calling __target_init_cmd().
 *
 * Return: TCM_NO_SENSE
 */
sense_reason_t
target_cmd_size_check(struct se_cmd *cmd, unsigned int size)
{
	struct se_device *dev = cmd->se_dev;

	if (cmd->unknown_data_length) {
		cmd->data_length = size;
	} else if (size != cmd->data_length) {
		pr_warn_ratelimited("TARGET_CORE[%s]: Expected Transfer Length:"
			" %u does not match SCSI CDB Length: %u for SAM Opcode:"
			" 0x%02x\n", cmd->se_tfo->fabric_name,
				cmd->data_length, size, cmd->t_task_cdb[0]);
		/*
		 * For READ command for the overflow case keep the existing
		 * fabric provided ->data_length. Otherwise for the underflow
		 * case, reset ->data_length to the smaller SCSI expected data
		 * transfer length.
		 */
		if (size > cmd->data_length) {
			cmd->se_cmd_flags |= SCF_OVERFLOW_BIT;
			cmd->residual_count = (size - cmd->data_length);
		} else {
			cmd->se_cmd_flags |= SCF_UNDERFLOW_BIT;
			cmd->residual_count = (cmd->data_length - size);
			/*
			 * Do not truncate ->data_length for WRITE command to
			 * dump all payload
			 */
			if (cmd->data_direction == DMA_FROM_DEVICE) {
				cmd->data_length = size;
			}
		}

		if (cmd->data_direction == DMA_TO_DEVICE) {
			if (cmd->se_cmd_flags & SCF_SCSI_DATA_CDB) {
				pr_err_ratelimited("Rejecting underflow/overflow"
						   " for WRITE data CDB\n");
				return TCM_INVALID_FIELD_IN_COMMAND_IU;
			}
			/*
			 * Some fabric drivers like iscsi-target still expect to
			 * always reject overflow writes.  Reject this case until
			 * full fabric driver level support for overflow writes
			 * is introduced tree-wide.
			 */
			if (size > cmd->data_length) {
				pr_err_ratelimited("Rejecting overflow for"
						   " WRITE control CDB\n");
				return TCM_INVALID_CDB_FIELD;
			}
		}
	}

	return target_check_max_data_sg_nents(cmd, dev, size);

}

/*
 * Used by fabric modules containing a local struct se_cmd within their
 * fabric dependent per I/O descriptor.
 *
 * Preserves the value of @cmd->tag.
 */
void __target_init_cmd(struct se_cmd *cmd,
		       const struct target_core_fabric_ops *tfo,
		       struct se_session *se_sess, u32 data_length,
		       int data_direction, int task_attr,
		       unsigned char *sense_buffer, u64 unpacked_lun,
		       struct target_cmd_counter *cmd_cnt)
{
	INIT_LIST_HEAD(&cmd->se_delayed_node);
	INIT_LIST_HEAD(&cmd->se_qf_node);
	INIT_LIST_HEAD(&cmd->state_list);
	init_completion(&cmd->t_transport_stop_comp);
	cmd->free_compl = NULL;
	cmd->abrt_compl = NULL;
	spin_lock_init(&cmd->t_state_lock);
	INIT_WORK(&cmd->work, NULL);
	kref_init(&cmd->cmd_kref);

	cmd->t_task_cdb = &cmd->__t_task_cdb[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmd->se_tfo = tfo;
	cmd->se_sess = se_sess;
	cmd->data_length = data_length;
	cmd->data_direction = data_direction;
	cmd->sam_task_attr = task_attr;
	cmd->sense_buffer = sense_buffer;
<<<<<<< HEAD
}
EXPORT_SYMBOL(transport_init_se_cmd);

static int transport_check_alloc_task_attr(struct se_cmd *cmd)
{
=======
	cmd->orig_fe_lun = unpacked_lun;
	cmd->cmd_cnt = cmd_cnt;

	if (!(cmd->se_cmd_flags & SCF_USE_CPUID))
		cmd->cpuid = raw_smp_processor_id();

	cmd->state_active = false;
}
EXPORT_SYMBOL(__target_init_cmd);

static sense_reason_t
transport_check_alloc_task_attr(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check if SAM Task Attribute emulation is enabled for this
	 * struct se_device storage object
	 */
<<<<<<< HEAD
	if (cmd->se_dev->dev_task_attr_type != SAM_TASK_ATTR_EMULATED)
		return 0;

	if (cmd->sam_task_attr == MSG_ACA_TAG) {
		pr_debug("SAM Task Attribute ACA"
			" emulation is not supported\n");
		return -EINVAL;
	}
	/*
	 * Used to determine when ORDERED commands should go from
	 * Dormant to Active status.
	 */
	cmd->se_ordered_id = atomic_inc_return(&cmd->se_dev->dev_ordered_id);
	smp_mb__after_atomic_inc();
	pr_debug("Allocated se_ordered_id: %u for Task Attr: 0x%02x on %s\n",
			cmd->se_ordered_id, cmd->sam_task_attr,
			cmd->se_dev->transport->name);
	return 0;
}

/*	target_setup_cmd_from_cdb():
 *
 *	Called from fabric RX Thread.
 */
int target_setup_cmd_from_cdb(
	struct se_cmd *cmd,
	unsigned char *cdb)
{
	int ret;

	transport_generic_prepare_cdb(cdb);
=======
	if (dev->transport_flags & TRANSPORT_FLAG_PASSTHROUGH)
		return 0;

	if (cmd->sam_task_attr == TCM_ACA_TAG) {
		pr_debug("SAM Task Attribute ACA"
			" emulation is not supported\n");
		return TCM_INVALID_CDB_FIELD;
	}

	return 0;
}

sense_reason_t
target_cmd_init_cdb(struct se_cmd *cmd, unsigned char *cdb, gfp_t gfp)
{
	sense_reason_t ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Ensure that the received CDB is less than the max (252 + 8) bytes
	 * for VARIABLE_LENGTH_CMD
	 */
	if (scsi_command_size(cdb) > SCSI_MAX_VARLEN_CDB_SIZE) {
		pr_err("Received SCSI CDB with command_size: %d that"
			" exceeds SCSI_MAX_VARLEN_CDB_SIZE: %d\n",
			scsi_command_size(cdb), SCSI_MAX_VARLEN_CDB_SIZE);
<<<<<<< HEAD
		cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		cmd->scsi_sense_reason = TCM_INVALID_CDB_FIELD;
		return -EINVAL;
=======
		ret = TCM_INVALID_CDB_FIELD;
		goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/*
	 * If the received CDB is larger than TCM_MAX_COMMAND_SIZE,
	 * allocate the additional extended CDB buffer now..  Otherwise
	 * setup the pointer from __t_task_cdb to t_task_cdb.
	 */
	if (scsi_command_size(cdb) > sizeof(cmd->__t_task_cdb)) {
<<<<<<< HEAD
		cmd->t_task_cdb = kzalloc(scsi_command_size(cdb),
						GFP_KERNEL);
=======
		cmd->t_task_cdb = kzalloc(scsi_command_size(cdb), gfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!cmd->t_task_cdb) {
			pr_err("Unable to allocate cmd->t_task_cdb"
				" %u > sizeof(cmd->__t_task_cdb): %lu ops\n",
				scsi_command_size(cdb),
				(unsigned long)sizeof(cmd->__t_task_cdb));
<<<<<<< HEAD
			cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			cmd->scsi_sense_reason =
					TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
			return -ENOMEM;
		}
	} else
		cmd->t_task_cdb = &cmd->__t_task_cdb[0];
=======
			ret = TCM_OUT_OF_RESOURCES;
			goto err;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Copy the original CDB into cmd->
	 */
	memcpy(cmd->t_task_cdb, cdb, scsi_command_size(cdb));
<<<<<<< HEAD
	/*
	 * Setup the received CDB based on SCSI defined opcodes and
	 * perform unit attention, persistent reservations and ALUA
	 * checks for virtual device backends.  The cmd->t_task_cdb
	 * pointer is expected to be setup before we reach this point.
	 */
	ret = transport_generic_cmd_sequencer(cmd, cdb);
	if (ret < 0)
		return ret;
	/*
	 * Check for SAM Task Attribute Emulation
	 */
	if (transport_check_alloc_task_attr(cmd) < 0) {
		cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		cmd->scsi_sense_reason = TCM_INVALID_CDB_FIELD;
		return -EINVAL;
	}
	spin_lock(&cmd->se_lun->lun_sep_lock);
	if (cmd->se_lun->lun_sep)
		cmd->se_lun->lun_sep->sep_stats.cmd_pdus++;
	spin_unlock(&cmd->se_lun->lun_sep_lock);
	return 0;
}
EXPORT_SYMBOL(target_setup_cmd_from_cdb);

/*
 * Used by fabric module frontends to queue tasks directly.
 * Many only be used from process context only
 */
int transport_handle_cdb_direct(
	struct se_cmd *cmd)
{
	int ret;
=======

	trace_target_sequencer_start(cmd);
	return 0;

err:
	/*
	 * Copy the CDB here to allow trace_target_cmd_complete() to
	 * print the cdb to the trace buffers.
	 */
	memcpy(cmd->t_task_cdb, cdb, min(scsi_command_size(cdb),
					 (unsigned int)TCM_MAX_COMMAND_SIZE));
	return ret;
}
EXPORT_SYMBOL(target_cmd_init_cdb);

sense_reason_t
target_cmd_parse_cdb(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	sense_reason_t ret;

	ret = dev->transport->parse_cdb(cmd);
	if (ret == TCM_UNSUPPORTED_SCSI_OPCODE)
		pr_debug_ratelimited("%s/%s: Unsupported SCSI Opcode 0x%02x, sending CHECK_CONDITION.\n",
				     cmd->se_tfo->fabric_name,
				     cmd->se_sess->se_node_acl->initiatorname,
				     cmd->t_task_cdb[0]);
	if (ret)
		return ret;

	ret = transport_check_alloc_task_attr(cmd);
	if (ret)
		return ret;

	cmd->se_cmd_flags |= SCF_SUPPORTED_SAM_OPCODE;
	atomic_long_inc(&cmd->se_lun->lun_stats.cmd_pdus);
	return 0;
}
EXPORT_SYMBOL(target_cmd_parse_cdb);

static int __target_submit(struct se_cmd *cmd)
{
	sense_reason_t ret;

	might_sleep();

	/*
	 * Check if we need to delay processing because of ALUA
	 * Active/NonOptimized primary access state..
	 */
	core_alua_check_nonop_delay(cmd);

	if (cmd->t_data_nents != 0) {
		/*
		 * This is primarily a hack for udev and tcm loop which sends
		 * INQUIRYs with a single page and expects the data to be
		 * cleared.
		 */
		if (!(cmd->se_cmd_flags & SCF_SCSI_DATA_CDB) &&
		    cmd->data_direction == DMA_FROM_DEVICE) {
			struct scatterlist *sgl = cmd->t_data_sg;
			unsigned char *buf = NULL;

			BUG_ON(!sgl);

			buf = kmap_local_page(sg_page(sgl));
			if (buf) {
				memset(buf + sgl->offset, 0, sgl->length);
				kunmap_local(buf);
			}
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!cmd->se_lun) {
		dump_stack();
		pr_err("cmd->se_lun is NULL\n");
		return -EINVAL;
	}
<<<<<<< HEAD
	if (in_interrupt()) {
		dump_stack();
		pr_err("transport_generic_handle_cdb cannot be called"
				" from interrupt context\n");
		return -EINVAL;
	}
	/*
	 * Set TRANSPORT_NEW_CMD state and CMD_T_ACTIVE following
	 * transport_generic_handle_cdb*() -> transport_add_cmd_to_queue()
	 * in existing usage to ensure that outstanding descriptors are handled
	 * correctly during shutdown via transport_wait_for_tasks()
=======

	/*
	 * Set TRANSPORT_NEW_CMD state and CMD_T_ACTIVE to ensure that
	 * outstanding descriptors are handled correctly during shutdown via
	 * transport_wait_for_tasks()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * Also, we don't take cmd->t_state_lock here as we only expect
	 * this to be called for initial descriptor submission.
	 */
	cmd->t_state = TRANSPORT_NEW_CMD;
	cmd->transport_state |= CMD_T_ACTIVE;

	/*
	 * transport_generic_new_cmd() is already handling QUEUE_FULL,
	 * so follow TRANSPORT_NEW_CMD processing thread context usage
	 * and call transport_generic_request_failure() if necessary..
	 */
	ret = transport_generic_new_cmd(cmd);
<<<<<<< HEAD
	if (ret < 0)
		transport_generic_request_failure(cmd);

	return 0;
}
EXPORT_SYMBOL(transport_handle_cdb_direct);
=======
	if (ret)
		transport_generic_request_failure(cmd, ret);
	return 0;
}

sense_reason_t
transport_generic_map_mem_to_cmd(struct se_cmd *cmd, struct scatterlist *sgl,
		u32 sgl_count, struct scatterlist *sgl_bidi, u32 sgl_bidi_count)
{
	if (!sgl || !sgl_count)
		return 0;

	/*
	 * Reject SCSI data overflow with map_mem_to_cmd() as incoming
	 * scatterlists already have been set to follow what the fabric
	 * passes for the original expected data transfer length.
	 */
	if (cmd->se_cmd_flags & SCF_OVERFLOW_BIT) {
		pr_warn("Rejecting SCSI DATA overflow for fabric using"
			" SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC\n");
		return TCM_INVALID_CDB_FIELD;
	}

	cmd->t_data_sg = sgl;
	cmd->t_data_nents = sgl_count;
	cmd->t_bidi_data_sg = sgl_bidi;
	cmd->t_bidi_data_nents = sgl_bidi_count;

	cmd->se_cmd_flags |= SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC;
	return 0;
}

/**
 * target_init_cmd - initialize se_cmd
 * @se_cmd: command descriptor to init
 * @se_sess: associated se_sess for endpoint
 * @sense: pointer to SCSI sense buffer
 * @unpacked_lun: unpacked LUN to reference for struct se_lun
 * @data_length: fabric expected data transfer length
 * @task_attr: SAM task attribute
 * @data_dir: DMA data direction
 * @flags: flags for command submission from target_sc_flags_tables
 *
 * Task tags are supported if the caller has set @se_cmd->tag.
 *
 * Returns:
 *	- less than zero to signal active I/O shutdown failure.
 *	- zero on success.
 *
 * If the fabric driver calls target_stop_session, then it must check the
 * return code and handle failures. This will never fail for other drivers,
 * and the return code can be ignored.
 */
int target_init_cmd(struct se_cmd *se_cmd, struct se_session *se_sess,
		    unsigned char *sense, u64 unpacked_lun,
		    u32 data_length, int task_attr, int data_dir, int flags)
{
	struct se_portal_group *se_tpg;

	se_tpg = se_sess->se_tpg;
	BUG_ON(!se_tpg);
	BUG_ON(se_cmd->se_tfo || se_cmd->se_sess);

	if (flags & TARGET_SCF_USE_CPUID)
		se_cmd->se_cmd_flags |= SCF_USE_CPUID;
	/*
	 * Signal bidirectional data payloads to target-core
	 */
	if (flags & TARGET_SCF_BIDI_OP)
		se_cmd->se_cmd_flags |= SCF_BIDI;

	if (flags & TARGET_SCF_UNKNOWN_SIZE)
		se_cmd->unknown_data_length = 1;
	/*
	 * Initialize se_cmd for target operation.  From this point
	 * exceptions are handled by sending exception status via
	 * target_core_fabric_ops->queue_status() callback
	 */
	__target_init_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess, data_length,
			  data_dir, task_attr, sense, unpacked_lun,
			  se_sess->cmd_cnt);

	/*
	 * Obtain struct se_cmd->cmd_kref reference. A second kref_get here is
	 * necessary for fabrics using TARGET_SCF_ACK_KREF that expect a second
	 * kref_put() to happen during fabric packet acknowledgement.
	 */
	return target_get_sess_cmd(se_cmd, flags & TARGET_SCF_ACK_KREF);
}
EXPORT_SYMBOL_GPL(target_init_cmd);

/**
 * target_submit_prep - prepare cmd for submission
 * @se_cmd: command descriptor to prep
 * @cdb: pointer to SCSI CDB
 * @sgl: struct scatterlist memory for unidirectional mapping
 * @sgl_count: scatterlist count for unidirectional mapping
 * @sgl_bidi: struct scatterlist memory for bidirectional READ mapping
 * @sgl_bidi_count: scatterlist count for bidirectional READ mapping
 * @sgl_prot: struct scatterlist memory protection information
 * @sgl_prot_count: scatterlist count for protection information
 * @gfp: gfp allocation type
 *
 * Returns:
 *	- less than zero to signal failure.
 *	- zero on success.
 *
 * If failure is returned, lio will the callers queue_status to complete
 * the cmd.
 */
int target_submit_prep(struct se_cmd *se_cmd, unsigned char *cdb,
		       struct scatterlist *sgl, u32 sgl_count,
		       struct scatterlist *sgl_bidi, u32 sgl_bidi_count,
		       struct scatterlist *sgl_prot, u32 sgl_prot_count,
		       gfp_t gfp)
{
	sense_reason_t rc;

	rc = target_cmd_init_cdb(se_cmd, cdb, gfp);
	if (rc)
		goto send_cc_direct;

	/*
	 * Locate se_lun pointer and attach it to struct se_cmd
	 */
	rc = transport_lookup_cmd_lun(se_cmd);
	if (rc)
		goto send_cc_direct;

	rc = target_cmd_parse_cdb(se_cmd);
	if (rc != 0)
		goto generic_fail;

	/*
	 * Save pointers for SGLs containing protection information,
	 * if present.
	 */
	if (sgl_prot_count) {
		se_cmd->t_prot_sg = sgl_prot;
		se_cmd->t_prot_nents = sgl_prot_count;
		se_cmd->se_cmd_flags |= SCF_PASSTHROUGH_PROT_SG_TO_MEM_NOALLOC;
	}

	/*
	 * When a non zero sgl_count has been passed perform SGL passthrough
	 * mapping for pre-allocated fabric memory instead of having target
	 * core perform an internal SGL allocation..
	 */
	if (sgl_count != 0) {
		BUG_ON(!sgl);

		rc = transport_generic_map_mem_to_cmd(se_cmd, sgl, sgl_count,
				sgl_bidi, sgl_bidi_count);
		if (rc != 0)
			goto generic_fail;
	}

	return 0;

send_cc_direct:
	transport_send_check_condition_and_sense(se_cmd, rc, 0);
	target_put_sess_cmd(se_cmd);
	return -EIO;

generic_fail:
	transport_generic_request_failure(se_cmd, rc);
	return -EIO;
}
EXPORT_SYMBOL_GPL(target_submit_prep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * target_submit_cmd - lookup unpacked lun and submit uninitialized se_cmd
 *
 * @se_cmd: command descriptor to submit
 * @se_sess: associated se_sess for endpoint
 * @cdb: pointer to SCSI CDB
 * @sense: pointer to SCSI sense buffer
 * @unpacked_lun: unpacked LUN to reference for struct se_lun
 * @data_length: fabric expected data transfer length
<<<<<<< HEAD
 * @task_addr: SAM task attribute
 * @data_dir: DMA data direction
 * @flags: flags for command submission from target_sc_flags_tables
 *
 * This may only be called from process context, and also currently
 * assumes internal allocation of fabric payload buffer by target-core.
 **/
void target_submit_cmd(struct se_cmd *se_cmd, struct se_session *se_sess,
		unsigned char *cdb, unsigned char *sense, u32 unpacked_lun,
		u32 data_length, int task_attr, int data_dir, int flags)
{
	struct se_portal_group *se_tpg;
	int rc;

	se_tpg = se_sess->se_tpg;
	BUG_ON(!se_tpg);
	BUG_ON(se_cmd->se_tfo || se_cmd->se_sess);
	BUG_ON(in_interrupt());
	/*
	 * Initialize se_cmd for target operation.  From this point
	 * exceptions are handled by sending exception status via
	 * target_core_fabric_ops->queue_status() callback
	 */
	transport_init_se_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess,
				data_length, data_dir, task_attr, sense);
	if (flags & TARGET_SCF_UNKNOWN_SIZE)
		se_cmd->unknown_data_length = 1;
	/*
	 * Obtain struct se_cmd->cmd_kref reference and add new cmd to
	 * se_sess->sess_cmd_list.  A second kref_get here is necessary
	 * for fabrics using TARGET_SCF_ACK_KREF that expect a second
	 * kref_put() to happen during fabric packet acknowledgement.
	 */
	target_get_sess_cmd(se_sess, se_cmd, (flags & TARGET_SCF_ACK_KREF));
	/*
	 * Signal bidirectional data payloads to target-core
	 */
	if (flags & TARGET_SCF_BIDI_OP)
		se_cmd->se_cmd_flags |= SCF_BIDI;
	/*
	 * Locate se_lun pointer and attach it to struct se_cmd
	 */
	if (transport_lookup_cmd_lun(se_cmd, unpacked_lun) < 0) {
		transport_send_check_condition_and_sense(se_cmd,
				se_cmd->scsi_sense_reason, 0);
		target_put_sess_cmd(se_sess, se_cmd);
		return;
	}
	/*
	 * Sanitize CDBs via transport_generic_cmd_sequencer() and
	 * allocate the necessary tasks to complete the received CDB+data
	 */
	rc = target_setup_cmd_from_cdb(se_cmd, cdb);
	if (rc != 0) {
		transport_generic_request_failure(se_cmd);
		return;
	}
	/*
	 * Dispatch se_cmd descriptor to se_lun->lun_se_dev backend
	 * for immediate execution of READs, otherwise wait for
	 * transport_generic_handle_data() to be called for WRITEs
	 * when fabric has filled the incoming buffer.
	 */
	transport_handle_cdb_direct(se_cmd);
	return;
}
EXPORT_SYMBOL(target_submit_cmd);

=======
 * @task_attr: SAM task attribute
 * @data_dir: DMA data direction
 * @flags: flags for command submission from target_sc_flags_tables
 *
 * Task tags are supported if the caller has set @se_cmd->tag.
 *
 * This may only be called from process context, and also currently
 * assumes internal allocation of fabric payload buffer by target-core.
 *
 * It also assumes interal target core SGL memory allocation.
 *
 * This function must only be used by drivers that do their own
 * sync during shutdown and does not use target_stop_session. If there
 * is a failure this function will call into the fabric driver's
 * queue_status with a CHECK_CONDITION.
 */
void target_submit_cmd(struct se_cmd *se_cmd, struct se_session *se_sess,
		unsigned char *cdb, unsigned char *sense, u64 unpacked_lun,
		u32 data_length, int task_attr, int data_dir, int flags)
{
	int rc;

	rc = target_init_cmd(se_cmd, se_sess, sense, unpacked_lun, data_length,
			     task_attr, data_dir, flags);
	WARN(rc, "Invalid target_submit_cmd use. Driver must not use target_stop_session or call target_init_cmd directly.\n");
	if (rc)
		return;

	if (target_submit_prep(se_cmd, cdb, NULL, 0, NULL, 0, NULL, 0,
			       GFP_KERNEL))
		return;

	target_submit(se_cmd);
}
EXPORT_SYMBOL(target_submit_cmd);


static struct se_dev_plug *target_plug_device(struct se_device *se_dev)
{
	struct se_dev_plug *se_plug;

	if (!se_dev->transport->plug_device)
		return NULL;

	se_plug = se_dev->transport->plug_device(se_dev);
	if (!se_plug)
		return NULL;

	se_plug->se_dev = se_dev;
	/*
	 * We have a ref to the lun at this point, but the cmds could
	 * complete before we unplug, so grab a ref to the se_device so we
	 * can call back into the backend.
	 */
	config_group_get(&se_dev->dev_group);
	return se_plug;
}

static void target_unplug_device(struct se_dev_plug *se_plug)
{
	struct se_device *se_dev = se_plug->se_dev;

	se_dev->transport->unplug_device(se_plug);
	config_group_put(&se_dev->dev_group);
}

void target_queued_submit_work(struct work_struct *work)
{
	struct se_cmd_queue *sq = container_of(work, struct se_cmd_queue, work);
	struct se_cmd *se_cmd, *next_cmd;
	struct se_dev_plug *se_plug = NULL;
	struct se_device *se_dev = NULL;
	struct llist_node *cmd_list;

	cmd_list = llist_del_all(&sq->cmd_list);
	if (!cmd_list)
		/* Previous call took what we were queued to submit */
		return;

	cmd_list = llist_reverse_order(cmd_list);
	llist_for_each_entry_safe(se_cmd, next_cmd, cmd_list, se_cmd_list) {
		if (!se_dev) {
			se_dev = se_cmd->se_dev;
			se_plug = target_plug_device(se_dev);
		}

		__target_submit(se_cmd);
	}

	if (se_plug)
		target_unplug_device(se_plug);
}

/**
 * target_queue_submission - queue the cmd to run on the LIO workqueue
 * @se_cmd: command descriptor to submit
 */
static void target_queue_submission(struct se_cmd *se_cmd)
{
	struct se_device *se_dev = se_cmd->se_dev;
	int cpu = se_cmd->cpuid;
	struct se_cmd_queue *sq;

	sq = &se_dev->queues[cpu].sq;
	llist_add(&se_cmd->se_cmd_list, &sq->cmd_list);
	queue_work_on(cpu, target_submission_wq, &sq->work);
}

/**
 * target_submit - perform final initialization and submit cmd to LIO core
 * @se_cmd: command descriptor to submit
 *
 * target_submit_prep or something similar must have been called on the cmd,
 * and this must be called from process context.
 */
int target_submit(struct se_cmd *se_cmd)
{
	const struct target_core_fabric_ops *tfo = se_cmd->se_sess->se_tpg->se_tpg_tfo;
	struct se_dev_attrib *da = &se_cmd->se_dev->dev_attrib;
	u8 submit_type;

	if (da->submit_type == TARGET_FABRIC_DEFAULT_SUBMIT)
		submit_type = tfo->default_submit_type;
	else if (da->submit_type == TARGET_DIRECT_SUBMIT &&
		 tfo->direct_submit_supp)
		submit_type = TARGET_DIRECT_SUBMIT;
	else
		submit_type = TARGET_QUEUE_SUBMIT;

	if (submit_type == TARGET_DIRECT_SUBMIT)
		return __target_submit(se_cmd);

	target_queue_submission(se_cmd);
	return 0;
}
EXPORT_SYMBOL_GPL(target_submit);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void target_complete_tmr_failure(struct work_struct *work)
{
	struct se_cmd *se_cmd = container_of(work, struct se_cmd, work);

	se_cmd->se_tmr_req->response = TMR_LUN_DOES_NOT_EXIST;
	se_cmd->se_tfo->queue_tm_rsp(se_cmd);

<<<<<<< HEAD
=======
	transport_lun_remove_cmd(se_cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	transport_cmd_check_stop_to_fabric(se_cmd);
}

/**
 * target_submit_tmr - lookup unpacked lun and submit uninitialized se_cmd
 *                     for TMR CDBs
 *
 * @se_cmd: command descriptor to submit
 * @se_sess: associated se_sess for endpoint
 * @sense: pointer to SCSI sense buffer
 * @unpacked_lun: unpacked LUN to reference for struct se_lun
<<<<<<< HEAD
 * @fabric_context: fabric context for TMR req
=======
 * @fabric_tmr_ptr: fabric context for TMR req
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @tm_type: Type of TM request
 * @gfp: gfp type for caller
 * @tag: referenced task tag for TMR_ABORT_TASK
 * @flags: submit cmd flags
 *
 * Callable from all contexts.
 **/

int target_submit_tmr(struct se_cmd *se_cmd, struct se_session *se_sess,
<<<<<<< HEAD
		unsigned char *sense, u32 unpacked_lun,
		void *fabric_tmr_ptr, unsigned char tm_type,
		gfp_t gfp, unsigned int tag, int flags)
=======
		unsigned char *sense, u64 unpacked_lun,
		void *fabric_tmr_ptr, unsigned char tm_type,
		gfp_t gfp, u64 tag, int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct se_portal_group *se_tpg;
	int ret;

	se_tpg = se_sess->se_tpg;
	BUG_ON(!se_tpg);

<<<<<<< HEAD
	transport_init_se_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess,
			      0, DMA_NONE, MSG_SIMPLE_TAG, sense);
=======
	__target_init_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess,
			  0, DMA_NONE, TCM_SIMPLE_TAG, sense, unpacked_lun,
			  se_sess->cmd_cnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * FIXME: Currently expect caller to handle se_cmd->se_tmr_req
	 * allocation failure.
	 */
	ret = core_tmr_alloc_req(se_cmd, fabric_tmr_ptr, tm_type, gfp);
	if (ret < 0)
		return -ENOMEM;

	if (tm_type == TMR_ABORT_TASK)
		se_cmd->se_tmr_req->ref_task_tag = tag;

	/* See target_submit_cmd for commentary */
<<<<<<< HEAD
	target_get_sess_cmd(se_sess, se_cmd, (flags & TARGET_SCF_ACK_KREF));

	ret = transport_lookup_tmr_lun(se_cmd, unpacked_lun);
	if (ret) {
		/*
		 * For callback during failure handling, push this work off
		 * to process context with TMR_LUN_DOES_NOT_EXIST status.
		 */
		INIT_WORK(&se_cmd->work, target_complete_tmr_failure);
		schedule_work(&se_cmd->work);
		return 0;
	}
	transport_generic_handle_tmr(se_cmd);
	return 0;
=======
	ret = target_get_sess_cmd(se_cmd, flags & TARGET_SCF_ACK_KREF);
	if (ret) {
		core_tmr_release_req(se_cmd->se_tmr_req);
		return ret;
	}

	ret = transport_lookup_tmr_lun(se_cmd);
	if (ret)
		goto failure;

	transport_generic_handle_tmr(se_cmd);
	return 0;

	/*
	 * For callback during failure handling, push this work off
	 * to process context with TMR_LUN_DOES_NOT_EXIST status.
	 */
failure:
	INIT_WORK(&se_cmd->work, target_complete_tmr_failure);
	schedule_work(&se_cmd->work);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(target_submit_tmr);

/*
<<<<<<< HEAD
 * Used by fabric module frontends defining a TFO->new_cmd_map() caller
 * to  queue up a newly setup se_cmd w/ TRANSPORT_NEW_CMD_MAP in order to
 * complete setup in TCM process context w/ TFO->new_cmd_map().
 */
int transport_generic_handle_cdb_map(
	struct se_cmd *cmd)
{
	if (!cmd->se_lun) {
		dump_stack();
		pr_err("cmd->se_lun is NULL\n");
		return -EINVAL;
	}

	transport_add_cmd_to_queue(cmd, TRANSPORT_NEW_CMD_MAP, false);
	return 0;
}
EXPORT_SYMBOL(transport_generic_handle_cdb_map);

/*	transport_generic_handle_data():
 *
 *
 */
int transport_generic_handle_data(
	struct se_cmd *cmd)
{
	/*
	 * For the software fabric case, then we assume the nexus is being
	 * failed/shutdown when signals are pending from the kthread context
	 * caller, so we return a failure.  For the HW target mode case running
	 * in interrupt code, the signal_pending() check is skipped.
	 */
	if (!in_interrupt() && signal_pending(current))
		return -EPERM;
	/*
	 * If the received CDB has aleady been ABORTED by the generic
	 * target engine, we now call transport_check_aborted_status()
	 * to queue any delated TASK_ABORTED status for the received CDB to the
	 * fabric module as we are expecting no further incoming DATA OUT
	 * sequences at this point.
	 */
	if (transport_check_aborted_status(cmd, 1) != 0)
		return 0;

	transport_add_cmd_to_queue(cmd, TRANSPORT_PROCESS_WRITE, false);
	return 0;
}
EXPORT_SYMBOL(transport_generic_handle_data);

/*	transport_generic_handle_tmr():
 *
 *
 */
int transport_generic_handle_tmr(
	struct se_cmd *cmd)
{
	transport_add_cmd_to_queue(cmd, TRANSPORT_PROCESS_TMR, false);
	return 0;
}
EXPORT_SYMBOL(transport_generic_handle_tmr);

/*
 * If the task is active, request it to be stopped and sleep until it
 * has completed.
 */
bool target_stop_task(struct se_task *task, unsigned long *flags)
{
	struct se_cmd *cmd = task->task_se_cmd;
	bool was_active = false;

	if (task->task_flags & TF_ACTIVE) {
		task->task_flags |= TF_REQUEST_STOP;
		spin_unlock_irqrestore(&cmd->t_state_lock, *flags);

		pr_debug("Task %p waiting to complete\n", task);
		wait_for_completion(&task->task_stop_comp);
		pr_debug("Task %p stopped successfully\n", task);

		spin_lock_irqsave(&cmd->t_state_lock, *flags);
		atomic_dec(&cmd->t_task_cdbs_left);
		task->task_flags &= ~(TF_ACTIVE | TF_REQUEST_STOP);
		was_active = true;
	}

	return was_active;
}

static int transport_stop_tasks_for_cmd(struct se_cmd *cmd)
{
	struct se_task *task, *task_tmp;
	unsigned long flags;
	int ret = 0;

	pr_debug("ITT[0x%08x] - Stopping tasks\n",
		cmd->se_tfo->get_task_tag(cmd));

	/*
	 * No tasks remain in the execution queue
	 */
	spin_lock_irqsave(&cmd->t_state_lock, flags);
	list_for_each_entry_safe(task, task_tmp,
				&cmd->t_task_list, t_list) {
		pr_debug("Processing task %p\n", task);
		/*
		 * If the struct se_task has not been sent and is not active,
		 * remove the struct se_task from the execution queue.
		 */
		if (!(task->task_flags & (TF_ACTIVE | TF_SENT))) {
			spin_unlock_irqrestore(&cmd->t_state_lock,
					flags);
			transport_remove_task_from_execute_queue(task,
					cmd->se_dev);

			pr_debug("Task %p removed from execute queue\n", task);
			spin_lock_irqsave(&cmd->t_state_lock, flags);
			continue;
		}

		if (!target_stop_task(task, &flags)) {
			pr_debug("Task %p - did nothing\n", task);
			ret++;
		}
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	return ret;
}

/*
 * Handle SAM-esque emulation for generic transport request failures.
 */
void transport_generic_request_failure(struct se_cmd *cmd)
{
	int ret = 0;

	pr_debug("-----[ Storage Engine Exception for cmd: %p ITT: 0x%08x"
		" CDB: 0x%02x\n", cmd, cmd->se_tfo->get_task_tag(cmd),
		cmd->t_task_cdb[0]);
	pr_debug("-----[ i_state: %d t_state: %d scsi_sense_reason: %d\n",
		cmd->se_tfo->get_cmd_state(cmd),
		cmd->t_state, cmd->scsi_sense_reason);
	pr_debug("-----[ t_tasks: %d t_task_cdbs_left: %d"
		" t_task_cdbs_sent: %d t_task_cdbs_ex_left: %d --"
		" CMD_T_ACTIVE: %d CMD_T_STOP: %d CMD_T_SENT: %d\n",
		cmd->t_task_list_num,
		atomic_read(&cmd->t_task_cdbs_left),
		atomic_read(&cmd->t_task_cdbs_sent),
		atomic_read(&cmd->t_task_cdbs_ex_left),
		(cmd->transport_state & CMD_T_ACTIVE) != 0,
		(cmd->transport_state & CMD_T_STOP) != 0,
		(cmd->transport_state & CMD_T_SENT) != 0);
=======
 * Handle SAM-esque emulation for generic transport request failures.
 */
void transport_generic_request_failure(struct se_cmd *cmd,
		sense_reason_t sense_reason)
{
	int ret = 0, post_ret;

	pr_debug("-----[ Storage Engine Exception; sense_reason %d\n",
		 sense_reason);
	target_show_cmd("-----[ ", cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For SAM Task Attribute emulation for failed struct se_cmd
	 */
<<<<<<< HEAD
	if (cmd->se_dev->dev_task_attr_type == SAM_TASK_ATTR_EMULATED)
		transport_complete_task_attr(cmd);

	switch (cmd->scsi_sense_reason) {
=======
	transport_complete_task_attr(cmd);

	if (cmd->transport_complete_callback)
		cmd->transport_complete_callback(cmd, false, &post_ret);

	if (cmd->transport_state & CMD_T_ABORTED) {
		INIT_WORK(&cmd->work, target_abort_work);
		queue_work(target_completion_wq, &cmd->work);
		return;
	}

	switch (sense_reason) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCM_NON_EXISTENT_LUN:
	case TCM_UNSUPPORTED_SCSI_OPCODE:
	case TCM_INVALID_CDB_FIELD:
	case TCM_INVALID_PARAMETER_LIST:
<<<<<<< HEAD
=======
	case TCM_PARAMETER_LIST_LENGTH_ERROR:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE:
	case TCM_UNKNOWN_MODE_PAGE:
	case TCM_WRITE_PROTECTED:
	case TCM_ADDRESS_OUT_OF_RANGE:
	case TCM_CHECK_CONDITION_ABORT_CMD:
	case TCM_CHECK_CONDITION_UNIT_ATTENTION:
<<<<<<< HEAD
	case TCM_CHECK_CONDITION_NOT_READY:
		break;
=======
	case TCM_LOGICAL_BLOCK_GUARD_CHECK_FAILED:
	case TCM_LOGICAL_BLOCK_APP_TAG_CHECK_FAILED:
	case TCM_LOGICAL_BLOCK_REF_TAG_CHECK_FAILED:
	case TCM_COPY_TARGET_DEVICE_NOT_REACHABLE:
	case TCM_TOO_MANY_TARGET_DESCS:
	case TCM_UNSUPPORTED_TARGET_DESC_TYPE_CODE:
	case TCM_TOO_MANY_SEGMENT_DESCS:
	case TCM_UNSUPPORTED_SEGMENT_DESC_TYPE_CODE:
	case TCM_INVALID_FIELD_IN_COMMAND_IU:
	case TCM_ALUA_TG_PT_STANDBY:
	case TCM_ALUA_TG_PT_UNAVAILABLE:
	case TCM_ALUA_STATE_TRANSITION:
	case TCM_ALUA_OFFLINE:
		break;
	case TCM_OUT_OF_RESOURCES:
		cmd->scsi_status = SAM_STAT_TASK_SET_FULL;
		goto queue_status;
	case TCM_LUN_BUSY:
		cmd->scsi_status = SAM_STAT_BUSY;
		goto queue_status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCM_RESERVATION_CONFLICT:
		/*
		 * No SENSE Data payload for this case, set SCSI Status
		 * and queue the response to $FABRIC_MOD.
		 *
		 * Uses linux/include/scsi/scsi.h SAM status codes defs
		 */
		cmd->scsi_status = SAM_STAT_RESERVATION_CONFLICT;
		/*
		 * For UA Interlock Code 11b, a RESERVATION CONFLICT will
		 * establish a UNIT ATTENTION with PREVIOUS RESERVATION
		 * CONFLICT STATUS.
		 *
		 * See spc4r17, section 7.4.6 Control Mode Page, Table 349
		 */
		if (cmd->se_sess &&
<<<<<<< HEAD
		    cmd->se_dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl == 2)
			core_scsi3_ua_allocate(cmd->se_sess->se_node_acl,
				cmd->orig_fe_lun, 0x2C,
				ASCQ_2CH_PREVIOUS_RESERVATION_CONFLICT_STATUS);

		ret = cmd->se_tfo->queue_status(cmd);
		if (ret == -EAGAIN || ret == -ENOMEM)
			goto queue_full;
		goto check_stop;
	default:
		pr_err("Unknown transport error for CDB 0x%02x: %d\n",
			cmd->t_task_cdb[0], cmd->scsi_sense_reason);
		cmd->scsi_sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
		break;
	}
	/*
	 * If a fabric does not define a cmd->se_tfo->new_cmd_map caller,
	 * make the call to transport_send_check_condition_and_sense()
	 * directly.  Otherwise expect the fabric to make the call to
	 * transport_send_check_condition_and_sense() after handling
	 * possible unsoliticied write data payloads.
	 */
	ret = transport_send_check_condition_and_sense(cmd,
			cmd->scsi_sense_reason, 0);
	if (ret == -EAGAIN || ret == -ENOMEM)
=======
		    cmd->se_dev->dev_attrib.emulate_ua_intlck_ctrl
					== TARGET_UA_INTLCK_CTRL_ESTABLISH_UA) {
			target_ua_allocate_lun(cmd->se_sess->se_node_acl,
					       cmd->orig_fe_lun, 0x2C,
					ASCQ_2CH_PREVIOUS_RESERVATION_CONFLICT_STATUS);
		}

		goto queue_status;
	default:
		pr_err("Unknown transport error for CDB 0x%02x: %d\n",
			cmd->t_task_cdb[0], sense_reason);
		sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
		break;
	}

	ret = transport_send_check_condition_and_sense(cmd, sense_reason, 0);
	if (ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto queue_full;

check_stop:
	transport_lun_remove_cmd(cmd);
<<<<<<< HEAD
	if (!transport_cmd_check_stop_to_fabric(cmd))
		;
	return;

queue_full:
	cmd->t_state = TRANSPORT_COMPLETE_QF_OK;
	transport_handle_queue_full(cmd, cmd->se_dev);
}
EXPORT_SYMBOL(transport_generic_request_failure);

static inline u32 transport_lba_21(unsigned char *cdb)
{
	return ((cdb[1] & 0x1f) << 16) | (cdb[2] << 8) | cdb[3];
}

static inline u32 transport_lba_32(unsigned char *cdb)
{
	return (cdb[2] << 24) | (cdb[3] << 16) | (cdb[4] << 8) | cdb[5];
}

static inline unsigned long long transport_lba_64(unsigned char *cdb)
{
	unsigned int __v1, __v2;

	__v1 = (cdb[2] << 24) | (cdb[3] << 16) | (cdb[4] << 8) | cdb[5];
	__v2 = (cdb[6] << 24) | (cdb[7] << 16) | (cdb[8] << 8) | cdb[9];

	return ((unsigned long long)__v2) | (unsigned long long)__v1 << 32;
}

/*
 * For VARIABLE_LENGTH_CDB w/ 32 byte extended CDBs
 */
static inline unsigned long long transport_lba_64_ext(unsigned char *cdb)
{
	unsigned int __v1, __v2;

	__v1 = (cdb[12] << 24) | (cdb[13] << 16) | (cdb[14] << 8) | cdb[15];
	__v2 = (cdb[16] << 24) | (cdb[17] << 16) | (cdb[18] << 8) | cdb[19];

	return ((unsigned long long)__v2) | (unsigned long long)__v1 << 32;
}

static void transport_set_supported_SAM_opcode(struct se_cmd *se_cmd)
{
	unsigned long flags;

	spin_lock_irqsave(&se_cmd->t_state_lock, flags);
	se_cmd->se_cmd_flags |= SCF_SUPPORTED_SAM_OPCODE;
	spin_unlock_irqrestore(&se_cmd->t_state_lock, flags);
}

/*
 * Called from Fabric Module context from transport_execute_tasks()
 *
 * The return of this function determins if the tasks from struct se_cmd
 * get added to the execution queue in transport_execute_tasks(),
 * or are added to the delayed or ordered lists here.
 */
static inline int transport_execute_task_attr(struct se_cmd *cmd)
{
	if (cmd->se_dev->dev_task_attr_type != SAM_TASK_ATTR_EMULATED)
		return 1;
=======
	transport_cmd_check_stop_to_fabric(cmd);
	return;

queue_status:
	trace_target_cmd_complete(cmd);
	ret = cmd->se_tfo->queue_status(cmd);
	if (!ret)
		goto check_stop;
queue_full:
	transport_handle_queue_full(cmd, cmd->se_dev, ret, false);
}
EXPORT_SYMBOL(transport_generic_request_failure);

void __target_execute_cmd(struct se_cmd *cmd, bool do_checks)
{
	sense_reason_t ret;

	if (!cmd->execute_cmd) {
		ret = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		goto err;
	}
	if (do_checks) {
		/*
		 * Check for an existing UNIT ATTENTION condition after
		 * target_handle_task_attr() has done SAM task attr
		 * checking, and possibly have already defered execution
		 * out to target_restart_delayed_cmds() context.
		 */
		ret = target_scsi3_ua_check(cmd);
		if (ret)
			goto err;

		ret = target_alua_state_check(cmd);
		if (ret)
			goto err;

		ret = target_check_reservation(cmd);
		if (ret) {
			cmd->scsi_status = SAM_STAT_RESERVATION_CONFLICT;
			goto err;
		}
	}

	ret = cmd->execute_cmd(cmd);
	if (!ret)
		return;
err:
	spin_lock_irq(&cmd->t_state_lock);
	cmd->transport_state &= ~CMD_T_SENT;
	spin_unlock_irq(&cmd->t_state_lock);

	transport_generic_request_failure(cmd, ret);
}

static int target_write_prot_action(struct se_cmd *cmd)
{
	u32 sectors;
	/*
	 * Perform WRITE_INSERT of PI using software emulation when backend
	 * device has PI enabled, if the transport has not already generated
	 * PI using hardware WRITE_INSERT offload.
	 */
	switch (cmd->prot_op) {
	case TARGET_PROT_DOUT_INSERT:
		if (!(cmd->se_sess->sup_prot_ops & TARGET_PROT_DOUT_INSERT))
			sbc_dif_generate(cmd);
		break;
	case TARGET_PROT_DOUT_STRIP:
		if (cmd->se_sess->sup_prot_ops & TARGET_PROT_DOUT_STRIP)
			break;

		sectors = cmd->data_length >> ilog2(cmd->se_dev->dev_attrib.block_size);
		cmd->pi_err = sbc_dif_verify(cmd, cmd->t_task_lba,
					     sectors, 0, cmd->t_prot_sg, 0);
		if (unlikely(cmd->pi_err)) {
			spin_lock_irq(&cmd->t_state_lock);
			cmd->transport_state &= ~CMD_T_SENT;
			spin_unlock_irq(&cmd->t_state_lock);
			transport_generic_request_failure(cmd, cmd->pi_err);
			return -1;
		}
		break;
	default:
		break;
	}

	return 0;
}

static bool target_handle_task_attr(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;

	if (dev->transport_flags & TRANSPORT_FLAG_PASSTHROUGH)
		return false;

	cmd->se_cmd_flags |= SCF_TASK_ATTR_SET;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check for the existence of HEAD_OF_QUEUE, and if true return 1
	 * to allow the passed struct se_cmd list of tasks to the front of the list.
	 */
<<<<<<< HEAD
	 if (cmd->sam_task_attr == MSG_HEAD_TAG) {
		pr_debug("Added HEAD_OF_QUEUE for CDB:"
			" 0x%02x, se_ordered_id: %u\n",
			cmd->t_task_cdb[0],
			cmd->se_ordered_id);
		return 1;
	} else if (cmd->sam_task_attr == MSG_ORDERED_TAG) {
		atomic_inc(&cmd->se_dev->dev_ordered_sync);
		smp_mb__after_atomic_inc();

		pr_debug("Added ORDERED for CDB: 0x%02x to ordered"
				" list, se_ordered_id: %u\n",
				cmd->t_task_cdb[0],
				cmd->se_ordered_id);
		/*
		 * Add ORDERED command to tail of execution queue if
		 * no other older commands exist that need to be
		 * completed first.
		 */
		if (!atomic_read(&cmd->se_dev->simple_cmds))
			return 1;
	} else {
		/*
		 * For SIMPLE and UNTAGGED Task Attribute commands
		 */
		atomic_inc(&cmd->se_dev->simple_cmds);
		smp_mb__after_atomic_inc();
	}
	/*
	 * Otherwise if one or more outstanding ORDERED task attribute exist,
	 * add the dormant task(s) built for the passed struct se_cmd to the
	 * execution queue and become in Active state for this struct se_device.
	 */
	if (atomic_read(&cmd->se_dev->dev_ordered_sync) != 0) {
		/*
		 * Otherwise, add cmd w/ tasks to delayed cmd queue that
		 * will be drained upon completion of HEAD_OF_QUEUE task.
		 */
		spin_lock(&cmd->se_dev->delayed_cmd_lock);
		cmd->se_cmd_flags |= SCF_DELAYED_CMD_FROM_SAM_ATTR;
		list_add_tail(&cmd->se_delayed_node,
				&cmd->se_dev->delayed_cmd_list);
		spin_unlock(&cmd->se_dev->delayed_cmd_lock);

		pr_debug("Added CDB: 0x%02x Task Attr: 0x%02x to"
			" delayed CMD list, se_ordered_id: %u\n",
			cmd->t_task_cdb[0], cmd->sam_task_attr,
			cmd->se_ordered_id);
		/*
		 * Return zero to let transport_execute_tasks() know
		 * not to add the delayed tasks to the execution list.
		 */
		return 0;
	}
	/*
	 * Otherwise, no ORDERED task attributes exist..
	 */
	return 1;
}

/*
 * Called from fabric module context in transport_generic_new_cmd() and
 * transport_generic_process_write()
 */
static int transport_execute_tasks(struct se_cmd *cmd)
{
	int add_tasks;
	struct se_device *se_dev = cmd->se_dev;
	/*
	 * Call transport_cmd_check_stop() to see if a fabric exception
	 * has occurred that prevents execution.
	 */
	if (!transport_cmd_check_stop(cmd, 0, TRANSPORT_PROCESSING)) {
		/*
		 * Check for SAM Task Attribute emulation and HEAD_OF_QUEUE
		 * attribute for the tasks of the received struct se_cmd CDB
		 */
		add_tasks = transport_execute_task_attr(cmd);
		if (!add_tasks)
			goto execute_tasks;
		/*
		 * __transport_execute_tasks() -> __transport_add_tasks_from_cmd()
		 * adds associated se_tasks while holding dev->execute_task_lock
		 * before I/O dispath to avoid a double spinlock access.
		 */
		__transport_execute_tasks(se_dev, cmd);
		return 0;
	}

execute_tasks:
	__transport_execute_tasks(se_dev, NULL);
	return 0;
}

/*
 * Called to check struct se_device tcq depth window, and once open pull struct se_task
 * from struct se_device->execute_task_list and
 *
 * Called from transport_processing_thread()
 */
static int __transport_execute_tasks(struct se_device *dev, struct se_cmd *new_cmd)
{
	int error;
	struct se_cmd *cmd = NULL;
	struct se_task *task = NULL;
	unsigned long flags;

check_depth:
	spin_lock_irq(&dev->execute_task_lock);
	if (new_cmd != NULL)
		__transport_add_tasks_from_cmd(new_cmd);

	if (list_empty(&dev->execute_task_list)) {
		spin_unlock_irq(&dev->execute_task_lock);
		return 0;
	}
	task = list_first_entry(&dev->execute_task_list,
				struct se_task, t_execute_list);
	__transport_remove_task_from_execute_queue(task, dev);
	spin_unlock_irq(&dev->execute_task_lock);

	cmd = task->task_se_cmd;
	spin_lock_irqsave(&cmd->t_state_lock, flags);
	task->task_flags |= (TF_ACTIVE | TF_SENT);
	atomic_inc(&cmd->t_task_cdbs_sent);

	if (atomic_read(&cmd->t_task_cdbs_sent) ==
	    cmd->t_task_list_num)
		cmd->transport_state |= CMD_T_SENT;

	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	if (cmd->execute_task)
		error = cmd->execute_task(task);
	else
		error = dev->transport->do_task(task);
	if (error != 0) {
		spin_lock_irqsave(&cmd->t_state_lock, flags);
		task->task_flags &= ~TF_ACTIVE;
		cmd->transport_state &= ~CMD_T_SENT;
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		transport_stop_tasks_for_cmd(cmd);
		transport_generic_request_failure(cmd);
	}

	new_cmd = NULL;
	goto check_depth;

	return 0;
}

static inline u32 transport_get_sectors_6(
	unsigned char *cdb,
	struct se_cmd *cmd,
	int *ret)
{
	struct se_device *dev = cmd->se_dev;

	/*
	 * Assume TYPE_DISK for non struct se_device objects.
	 * Use 8-bit sector value.
	 */
	if (!dev)
		goto type_disk;

	/*
	 * Use 24-bit allocation length for TYPE_TAPE.
	 */
	if (dev->transport->get_device_type(dev) == TYPE_TAPE)
		return (u32)(cdb[2] << 16) + (cdb[3] << 8) + cdb[4];

	/*
	 * Everything else assume TYPE_DISK Sector CDB location.
	 * Use 8-bit sector value.  SBC-3 says:
	 *
	 *   A TRANSFER LENGTH field set to zero specifies that 256
	 *   logical blocks shall be written.  Any other value
	 *   specifies the number of logical blocks that shall be
	 *   written.
	 */
type_disk:
	return cdb[4] ? : 256;
}

static inline u32 transport_get_sectors_10(
	unsigned char *cdb,
	struct se_cmd *cmd,
	int *ret)
{
	struct se_device *dev = cmd->se_dev;

	/*
	 * Assume TYPE_DISK for non struct se_device objects.
	 * Use 16-bit sector value.
	 */
	if (!dev)
		goto type_disk;

	/*
	 * XXX_10 is not defined in SSC, throw an exception
	 */
	if (dev->transport->get_device_type(dev) == TYPE_TAPE) {
		*ret = -EINVAL;
		return 0;
	}

	/*
	 * Everything else assume TYPE_DISK Sector CDB location.
	 * Use 16-bit sector value.
	 */
type_disk:
	return (u32)(cdb[7] << 8) + cdb[8];
}

static inline u32 transport_get_sectors_12(
	unsigned char *cdb,
	struct se_cmd *cmd,
	int *ret)
{
	struct se_device *dev = cmd->se_dev;

	/*
	 * Assume TYPE_DISK for non struct se_device objects.
	 * Use 32-bit sector value.
	 */
	if (!dev)
		goto type_disk;

	/*
	 * XXX_12 is not defined in SSC, throw an exception
	 */
	if (dev->transport->get_device_type(dev) == TYPE_TAPE) {
		*ret = -EINVAL;
		return 0;
	}

	/*
	 * Everything else assume TYPE_DISK Sector CDB location.
	 * Use 32-bit sector value.
	 */
type_disk:
	return (u32)(cdb[6] << 24) + (cdb[7] << 16) + (cdb[8] << 8) + cdb[9];
}

static inline u32 transport_get_sectors_16(
	unsigned char *cdb,
	struct se_cmd *cmd,
	int *ret)
{
	struct se_device *dev = cmd->se_dev;

	/*
	 * Assume TYPE_DISK for non struct se_device objects.
	 * Use 32-bit sector value.
	 */
	if (!dev)
		goto type_disk;

	/*
	 * Use 24-bit allocation length for TYPE_TAPE.
	 */
	if (dev->transport->get_device_type(dev) == TYPE_TAPE)
		return (u32)(cdb[12] << 16) + (cdb[13] << 8) + cdb[14];

type_disk:
	return (u32)(cdb[10] << 24) + (cdb[11] << 16) +
		    (cdb[12] << 8) + cdb[13];
}

/*
 * Used for VARIABLE_LENGTH_CDB WRITE_32 and READ_32 variants
 */
static inline u32 transport_get_sectors_32(
	unsigned char *cdb,
	struct se_cmd *cmd,
	int *ret)
{
	/*
	 * Assume TYPE_DISK for non struct se_device objects.
	 * Use 32-bit sector value.
	 */
	return (u32)(cdb[28] << 24) + (cdb[29] << 16) +
		    (cdb[30] << 8) + cdb[31];

}

static inline u32 transport_get_size(
	u32 sectors,
	unsigned char *cdb,
	struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;

	if (dev->transport->get_device_type(dev) == TYPE_TAPE) {
		if (cdb[1] & 1) { /* sectors */
			return dev->se_sub_dev->se_dev_attrib.block_size * sectors;
		} else /* bytes */
			return sectors;
	}
#if 0
	pr_debug("Returning block_size: %u, sectors: %u == %u for"
			" %s object\n", dev->se_sub_dev->se_dev_attrib.block_size, sectors,
			dev->se_sub_dev->se_dev_attrib.block_size * sectors,
			dev->transport->name);
#endif
	return dev->se_sub_dev->se_dev_attrib.block_size * sectors;
}

static void transport_xor_callback(struct se_cmd *cmd)
{
	unsigned char *buf, *addr;
	struct scatterlist *sg;
	unsigned int offset;
	int i;
	int count;
	/*
	 * From sbc3r22.pdf section 5.48 XDWRITEREAD (10) command
	 *
	 * 1) read the specified logical block(s);
	 * 2) transfer logical blocks from the data-out buffer;
	 * 3) XOR the logical blocks transferred from the data-out buffer with
	 *    the logical blocks read, storing the resulting XOR data in a buffer;
	 * 4) if the DISABLE WRITE bit is set to zero, then write the logical
	 *    blocks transferred from the data-out buffer; and
	 * 5) transfer the resulting XOR data to the data-in buffer.
	 */
	buf = kmalloc(cmd->data_length, GFP_KERNEL);
	if (!buf) {
		pr_err("Unable to allocate xor_callback buf\n");
		return;
	}
	/*
	 * Copy the scatterlist WRITE buffer located at cmd->t_data_sg
	 * into the locally allocated *buf
	 */
	sg_copy_to_buffer(cmd->t_data_sg,
			  cmd->t_data_nents,
			  buf,
			  cmd->data_length);

	/*
	 * Now perform the XOR against the BIDI read memory located at
	 * cmd->t_mem_bidi_list
	 */

	offset = 0;
	for_each_sg(cmd->t_bidi_data_sg, sg, cmd->t_bidi_data_nents, count) {
		addr = kmap_atomic(sg_page(sg));
		if (!addr)
			goto out;

		for (i = 0; i < sg->length; i++)
			*(addr + sg->offset + i) ^= *(buf + offset + i);

		offset += sg->length;
		kunmap_atomic(addr);
	}

out:
	kfree(buf);
}

/*
 * Used to obtain Sense Data from underlying Linux/SCSI struct scsi_cmnd
 */
static int transport_get_sense_data(struct se_cmd *cmd)
{
	unsigned char *buffer = cmd->sense_buffer, *sense_buffer = NULL;
	struct se_device *dev = cmd->se_dev;
	struct se_task *task = NULL, *task_tmp;
	unsigned long flags;
	u32 offset = 0;

	WARN_ON(!cmd->se_lun);

	if (!dev)
		return 0;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->se_cmd_flags & SCF_SENT_CHECK_CONDITION) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return 0;
	}

	list_for_each_entry_safe(task, task_tmp,
				&cmd->t_task_list, t_list) {
		if (!(task->task_flags & TF_HAS_SENSE))
			continue;

		if (!dev->transport->get_sense_buffer) {
			pr_err("dev->transport->get_sense_buffer"
					" is NULL\n");
			continue;
		}

		sense_buffer = dev->transport->get_sense_buffer(task);
		if (!sense_buffer) {
			pr_err("ITT[0x%08x]_TASK[%p]: Unable to locate"
				" sense buffer for task with sense\n",
				cmd->se_tfo->get_task_tag(cmd), task);
			continue;
		}
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		offset = cmd->se_tfo->set_fabric_sense_len(cmd,
				TRANSPORT_SENSE_BUFFER);

		memcpy(&buffer[offset], sense_buffer,
				TRANSPORT_SENSE_BUFFER);
		cmd->scsi_status = task->task_scsi_status;
		/* Automatically padded */
		cmd->scsi_sense_length =
				(TRANSPORT_SENSE_BUFFER + offset);

		pr_debug("HBA_[%u]_PLUG[%s]: Set SAM STATUS: 0x%02x"
				" and sense\n",
			dev->se_hba->hba_id, dev->transport->name,
				cmd->scsi_status);
		return 0;
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	return -1;
}

static inline long long transport_dev_end_lba(struct se_device *dev)
{
	return dev->transport->get_blocks(dev) + 1;
}

static int transport_cmd_get_valid_sectors(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	u32 sectors;

	if (dev->transport->get_device_type(dev) != TYPE_DISK)
		return 0;

	sectors = (cmd->data_length / dev->se_sub_dev->se_dev_attrib.block_size);

	if ((cmd->t_task_lba + sectors) > transport_dev_end_lba(dev)) {
		pr_err("LBA: %llu Sectors: %u exceeds"
			" transport_dev_end_lba(): %llu\n",
			cmd->t_task_lba, sectors,
			transport_dev_end_lba(dev));
		return -EINVAL;
	}

	return 0;
}

static int target_check_write_same_discard(unsigned char *flags, struct se_device *dev)
{
	/*
	 * Determine if the received WRITE_SAME is used to for direct
	 * passthrough into Linux/SCSI with struct request via TCM/pSCSI
	 * or we are signaling the use of internal WRITE_SAME + UNMAP=1
	 * emulation for -> Linux/BLOCK disbard with TCM/IBLOCK code.
	 */
	int passthrough = (dev->transport->transport_type ==
				TRANSPORT_PLUGIN_PHBA_PDEV);

	if (!passthrough) {
		if ((flags[0] & 0x04) || (flags[0] & 0x02)) {
			pr_err("WRITE_SAME PBDATA and LBDATA"
				" bits not supported for Block Discard"
				" Emulation\n");
			return -ENOSYS;
		}
		/*
		 * Currently for the emulated case we only accept
		 * tpws with the UNMAP=1 bit set.
		 */
		if (!(flags[0] & 0x08)) {
			pr_err("WRITE_SAME w/o UNMAP bit not"
				" supported for Block Discard Emulation\n");
			return -ENOSYS;
		}
	}

	return 0;
}

/*	transport_generic_cmd_sequencer():
 *
 *	Generic Command Sequencer that should work for most DAS transport
 *	drivers.
 *
 *	Called from target_setup_cmd_from_cdb() in the $FABRIC_MOD
 *	RX Thread.
 *
 *	FIXME: Need to support other SCSI OPCODES where as well.
 */
static int transport_generic_cmd_sequencer(
	struct se_cmd *cmd,
	unsigned char *cdb)
{
	struct se_device *dev = cmd->se_dev;
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
	int ret = 0, sector_ret = 0, passthrough;
	u32 sectors = 0, size = 0, pr_reg_type = 0;
	u16 service_action;
	u8 alua_ascq = 0;
	/*
	 * Check for an existing UNIT ATTENTION condition
	 */
	if (core_scsi3_ua_check(cmd, cdb) < 0) {
		cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		cmd->scsi_sense_reason = TCM_CHECK_CONDITION_UNIT_ATTENTION;
		return -EINVAL;
	}
	/*
	 * Check status of Asymmetric Logical Unit Assignment port
	 */
	ret = su_dev->t10_alua.alua_state_check(cmd, cdb, &alua_ascq);
	if (ret != 0) {
		/*
		 * Set SCSI additional sense code (ASC) to 'LUN Not Accessible';
		 * The ALUA additional sense code qualifier (ASCQ) is determined
		 * by the ALUA primary or secondary access state..
		 */
		if (ret > 0) {
#if 0
			pr_debug("[%s]: ALUA TG Port not available,"
				" SenseKey: NOT_READY, ASC/ASCQ: 0x04/0x%02x\n",
				cmd->se_tfo->get_fabric_name(), alua_ascq);
#endif
			transport_set_sense_codes(cmd, 0x04, alua_ascq);
			cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			cmd->scsi_sense_reason = TCM_CHECK_CONDITION_NOT_READY;
			return -EINVAL;
		}
		goto out_invalid_cdb_field;
	}
	/*
	 * Check status for SPC-3 Persistent Reservations
	 */
	if (su_dev->t10_pr.pr_ops.t10_reservation_check(cmd, &pr_reg_type) != 0) {
		if (su_dev->t10_pr.pr_ops.t10_seq_non_holder(
					cmd, cdb, pr_reg_type) != 0) {
			cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			cmd->se_cmd_flags |= SCF_SCSI_RESERVATION_CONFLICT;
			cmd->scsi_status = SAM_STAT_RESERVATION_CONFLICT;
			cmd->scsi_sense_reason = TCM_RESERVATION_CONFLICT;
			return -EBUSY;
		}
		/*
		 * This means the CDB is allowed for the SCSI Initiator port
		 * when said port is *NOT* holding the legacy SPC-2 or
		 * SPC-3 Persistent Reservation.
		 */
	}

	/*
	 * If we operate in passthrough mode we skip most CDB emulation and
	 * instead hand the commands down to the physical SCSI device.
	 */
	passthrough =
		(dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV);

	switch (cdb[0]) {
	case READ_6:
		sectors = transport_get_sectors_6(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_21(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case READ_10:
		sectors = transport_get_sectors_10(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_32(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case READ_12:
		sectors = transport_get_sectors_12(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_32(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case READ_16:
		sectors = transport_get_sectors_16(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_64(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case WRITE_6:
		sectors = transport_get_sectors_6(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_21(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case WRITE_10:
		sectors = transport_get_sectors_10(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_32(cdb);
		if (cdb[1] & 0x8)
			cmd->se_cmd_flags |= SCF_FUA;
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case WRITE_12:
		sectors = transport_get_sectors_12(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_32(cdb);
		if (cdb[1] & 0x8)
			cmd->se_cmd_flags |= SCF_FUA;
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case WRITE_16:
		sectors = transport_get_sectors_16(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_64(cdb);
		if (cdb[1] & 0x8)
			cmd->se_cmd_flags |= SCF_FUA;
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;
		break;
	case XDWRITEREAD_10:
		if ((cmd->data_direction != DMA_TO_DEVICE) ||
		    !(cmd->se_cmd_flags & SCF_BIDI))
			goto out_invalid_cdb_field;
		sectors = transport_get_sectors_10(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;
		size = transport_get_size(sectors, cdb, cmd);
		cmd->t_task_lba = transport_lba_32(cdb);
		cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;

		/*
		 * Do now allow BIDI commands for passthrough mode.
		 */
		if (passthrough)
			goto out_unsupported_cdb;

		/*
		 * Setup BIDI XOR callback to be run after I/O completion.
		 */
		cmd->transport_complete_callback = &transport_xor_callback;
		if (cdb[1] & 0x8)
			cmd->se_cmd_flags |= SCF_FUA;
		break;
	case VARIABLE_LENGTH_CMD:
		service_action = get_unaligned_be16(&cdb[8]);
		switch (service_action) {
		case XDWRITEREAD_32:
			sectors = transport_get_sectors_32(cdb, cmd, &sector_ret);
			if (sector_ret)
				goto out_unsupported_cdb;
			size = transport_get_size(sectors, cdb, cmd);
			/*
			 * Use WRITE_32 and READ_32 opcodes for the emulated
			 * XDWRITE_READ_32 logic.
			 */
			cmd->t_task_lba = transport_lba_64_ext(cdb);
			cmd->se_cmd_flags |= SCF_SCSI_DATA_SG_IO_CDB;

			/*
			 * Do now allow BIDI commands for passthrough mode.
			 */
			if (passthrough)
				goto out_unsupported_cdb;

			/*
			 * Setup BIDI XOR callback to be run during after I/O
			 * completion.
			 */
			cmd->transport_complete_callback = &transport_xor_callback;
			if (cdb[1] & 0x8)
				cmd->se_cmd_flags |= SCF_FUA;
			break;
		case WRITE_SAME_32:
			sectors = transport_get_sectors_32(cdb, cmd, &sector_ret);
			if (sector_ret)
				goto out_unsupported_cdb;

			if (sectors)
				size = transport_get_size(1, cdb, cmd);
			else {
				pr_err("WSNZ=1, WRITE_SAME w/sectors=0 not"
				       " supported\n");
				goto out_invalid_cdb_field;
			}

			cmd->t_task_lba = get_unaligned_be64(&cdb[12]);
			cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;

			if (target_check_write_same_discard(&cdb[10], dev) < 0)
				goto out_unsupported_cdb;
			if (!passthrough)
				cmd->execute_task = target_emulate_write_same;
			break;
		default:
			pr_err("VARIABLE_LENGTH_CMD service action"
				" 0x%04x not supported\n", service_action);
			goto out_unsupported_cdb;
		}
		break;
	case MAINTENANCE_IN:
		if (dev->transport->get_device_type(dev) != TYPE_ROM) {
			/* MAINTENANCE_IN from SCC-2 */
			/*
			 * Check for emulated MI_REPORT_TARGET_PGS.
			 */
			if ((cdb[1] & 0x1f) == MI_REPORT_TARGET_PGS &&
			    su_dev->t10_alua.alua_type == SPC3_ALUA_EMULATED) {
				cmd->execute_task =
					target_emulate_report_target_port_groups;
			}
			size = (cdb[6] << 24) | (cdb[7] << 16) |
			       (cdb[8] << 8) | cdb[9];
		} else {
			/* GPCMD_SEND_KEY from multi media commands */
			size = (cdb[8] << 8) + cdb[9];
		}
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case MODE_SELECT:
		size = cdb[4];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case MODE_SELECT_10:
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case MODE_SENSE:
		size = cdb[4];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_modesense;
		break;
	case MODE_SENSE_10:
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_modesense;
		break;
	case GPCMD_READ_BUFFER_CAPACITY:
	case GPCMD_SEND_OPC:
	case LOG_SELECT:
	case LOG_SENSE:
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case READ_BLOCK_LIMITS:
		size = READ_BLOCK_LEN;
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case GPCMD_GET_CONFIGURATION:
	case GPCMD_READ_FORMAT_CAPACITIES:
	case GPCMD_READ_DISC_INFO:
	case GPCMD_READ_TRACK_RZONE_INFO:
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case PERSISTENT_RESERVE_IN:
		if (su_dev->t10_pr.res_type == SPC3_PERSISTENT_RESERVATIONS)
			cmd->execute_task = target_scsi3_emulate_pr_in;
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case PERSISTENT_RESERVE_OUT:
		if (su_dev->t10_pr.res_type == SPC3_PERSISTENT_RESERVATIONS)
			cmd->execute_task = target_scsi3_emulate_pr_out;
		size = (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case GPCMD_MECHANISM_STATUS:
	case GPCMD_READ_DVD_STRUCTURE:
		size = (cdb[8] << 8) + cdb[9];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case READ_POSITION:
		size = READ_POSITION_LEN;
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case MAINTENANCE_OUT:
		if (dev->transport->get_device_type(dev) != TYPE_ROM) {
			/* MAINTENANCE_OUT from SCC-2
			 *
			 * Check for emulated MO_SET_TARGET_PGS.
			 */
			if (cdb[1] == MO_SET_TARGET_PGS &&
			    su_dev->t10_alua.alua_type == SPC3_ALUA_EMULATED) {
				cmd->execute_task =
					target_emulate_set_target_port_groups;
			}

			size = (cdb[6] << 24) | (cdb[7] << 16) |
			       (cdb[8] << 8) | cdb[9];
		} else  {
			/* GPCMD_REPORT_KEY from multi media commands */
			size = (cdb[8] << 8) + cdb[9];
		}
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case INQUIRY:
		size = (cdb[3] << 8) + cdb[4];
		/*
		 * Do implict HEAD_OF_QUEUE processing for INQUIRY.
		 * See spc4r17 section 5.3
		 */
		if (cmd->se_dev->dev_task_attr_type == SAM_TASK_ATTR_EMULATED)
			cmd->sam_task_attr = MSG_HEAD_TAG;
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_inquiry;
		break;
	case READ_BUFFER:
		size = (cdb[6] << 16) + (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case READ_CAPACITY:
		size = READ_CAP_LEN;
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_readcapacity;
		break;
	case READ_MEDIA_SERIAL_NUMBER:
	case SECURITY_PROTOCOL_IN:
	case SECURITY_PROTOCOL_OUT:
		size = (cdb[6] << 24) | (cdb[7] << 16) | (cdb[8] << 8) | cdb[9];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case SERVICE_ACTION_IN:
		switch (cmd->t_task_cdb[1] & 0x1f) {
		case SAI_READ_CAPACITY_16:
			if (!passthrough)
				cmd->execute_task =
					target_emulate_readcapacity_16;
			break;
		default:
			if (passthrough)
				break;

			pr_err("Unsupported SA: 0x%02x\n",
				cmd->t_task_cdb[1] & 0x1f);
			goto out_invalid_cdb_field;
		}
		/*FALLTHROUGH*/
	case ACCESS_CONTROL_IN:
	case ACCESS_CONTROL_OUT:
	case EXTENDED_COPY:
	case READ_ATTRIBUTE:
	case RECEIVE_COPY_RESULTS:
	case WRITE_ATTRIBUTE:
		size = (cdb[10] << 24) | (cdb[11] << 16) |
		       (cdb[12] << 8) | cdb[13];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case RECEIVE_DIAGNOSTIC:
	case SEND_DIAGNOSTIC:
		size = (cdb[3] << 8) | cdb[4];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
/* #warning FIXME: Figure out correct GPCMD_READ_CD blocksize. */
#if 0
	case GPCMD_READ_CD:
		sectors = (cdb[6] << 16) + (cdb[7] << 8) + cdb[8];
		size = (2336 * sectors);
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
#endif
	case READ_TOC:
		size = cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case REQUEST_SENSE:
		size = cdb[4];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_request_sense;
		break;
	case READ_ELEMENT_STATUS:
		size = 65536 * cdb[7] + 256 * cdb[8] + cdb[9];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case WRITE_BUFFER:
		size = (cdb[6] << 16) + (cdb[7] << 8) + cdb[8];
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	case RESERVE:
	case RESERVE_10:
		/*
		 * The SPC-2 RESERVE does not contain a size in the SCSI CDB.
		 * Assume the passthrough or $FABRIC_MOD will tell us about it.
		 */
		if (cdb[0] == RESERVE_10)
			size = (cdb[7] << 8) | cdb[8];
		else
			size = cmd->data_length;

		/*
		 * Setup the legacy emulated handler for SPC-2 and
		 * >= SPC-3 compatible reservation handling (CRH=1)
		 * Otherwise, we assume the underlying SCSI logic is
		 * is running in SPC_PASSTHROUGH, and wants reservations
		 * emulation disabled.
		 */
		if (su_dev->t10_pr.res_type != SPC_PASSTHROUGH)
			cmd->execute_task = target_scsi2_reservation_reserve;
		cmd->se_cmd_flags |= SCF_SCSI_NON_DATA_CDB;
		break;
	case RELEASE:
	case RELEASE_10:
		/*
		 * The SPC-2 RELEASE does not contain a size in the SCSI CDB.
		 * Assume the passthrough or $FABRIC_MOD will tell us about it.
		*/
		if (cdb[0] == RELEASE_10)
			size = (cdb[7] << 8) | cdb[8];
		else
			size = cmd->data_length;

		if (su_dev->t10_pr.res_type != SPC_PASSTHROUGH)
			cmd->execute_task = target_scsi2_reservation_release;
		cmd->se_cmd_flags |= SCF_SCSI_NON_DATA_CDB;
		break;
	case SYNCHRONIZE_CACHE:
	case SYNCHRONIZE_CACHE_16:
		/*
		 * Extract LBA and range to be flushed for emulated SYNCHRONIZE_CACHE
		 */
		if (cdb[0] == SYNCHRONIZE_CACHE) {
			sectors = transport_get_sectors_10(cdb, cmd, &sector_ret);
			cmd->t_task_lba = transport_lba_32(cdb);
		} else {
			sectors = transport_get_sectors_16(cdb, cmd, &sector_ret);
			cmd->t_task_lba = transport_lba_64(cdb);
		}
		if (sector_ret)
			goto out_unsupported_cdb;

		size = transport_get_size(sectors, cdb, cmd);
		cmd->se_cmd_flags |= SCF_SCSI_NON_DATA_CDB;

		if (passthrough)
			break;

		/*
		 * Check to ensure that LBA + Range does not exceed past end of
		 * device for IBLOCK and FILEIO ->do_sync_cache() backend calls
		 */
		if ((cmd->t_task_lba != 0) || (sectors != 0)) {
			if (transport_cmd_get_valid_sectors(cmd) < 0)
				goto out_invalid_cdb_field;
		}
		cmd->execute_task = target_emulate_synchronize_cache;
		break;
	case UNMAP:
		size = get_unaligned_be16(&cdb[7]);
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_unmap;
		break;
	case WRITE_SAME_16:
		sectors = transport_get_sectors_16(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;

		if (sectors)
			size = transport_get_size(1, cdb, cmd);
		else {
			pr_err("WSNZ=1, WRITE_SAME w/sectors=0 not supported\n");
			goto out_invalid_cdb_field;
		}

		cmd->t_task_lba = get_unaligned_be64(&cdb[2]);
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;

		if (target_check_write_same_discard(&cdb[1], dev) < 0)
			goto out_unsupported_cdb;
		if (!passthrough)
			cmd->execute_task = target_emulate_write_same;
		break;
	case WRITE_SAME:
		sectors = transport_get_sectors_10(cdb, cmd, &sector_ret);
		if (sector_ret)
			goto out_unsupported_cdb;

		if (sectors)
			size = transport_get_size(1, cdb, cmd);
		else {
			pr_err("WSNZ=1, WRITE_SAME w/sectors=0 not supported\n");
			goto out_invalid_cdb_field;
		}

		cmd->t_task_lba = get_unaligned_be32(&cdb[2]);
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		/*
		 * Follow sbcr26 with WRITE_SAME (10) and check for the existence
		 * of byte 1 bit 3 UNMAP instead of original reserved field
		 */
		if (target_check_write_same_discard(&cdb[1], dev) < 0)
			goto out_unsupported_cdb;
		if (!passthrough)
			cmd->execute_task = target_emulate_write_same;
		break;
	case ALLOW_MEDIUM_REMOVAL:
	case ERASE:
	case REZERO_UNIT:
	case SEEK_10:
	case SPACE:
	case START_STOP:
	case TEST_UNIT_READY:
	case VERIFY:
	case WRITE_FILEMARKS:
		cmd->se_cmd_flags |= SCF_SCSI_NON_DATA_CDB;
		if (!passthrough)
			cmd->execute_task = target_emulate_noop;
		break;
	case GPCMD_CLOSE_TRACK:
	case INITIALIZE_ELEMENT_STATUS:
	case GPCMD_LOAD_UNLOAD:
	case GPCMD_SET_SPEED:
	case MOVE_MEDIUM:
		cmd->se_cmd_flags |= SCF_SCSI_NON_DATA_CDB;
		break;
	case REPORT_LUNS:
		cmd->execute_task = target_report_luns;
		size = (cdb[6] << 24) | (cdb[7] << 16) | (cdb[8] << 8) | cdb[9];
		/*
		 * Do implict HEAD_OF_QUEUE processing for REPORT_LUNS
		 * See spc4r17 section 5.3
		 */
		if (cmd->se_dev->dev_task_attr_type == SAM_TASK_ATTR_EMULATED)
			cmd->sam_task_attr = MSG_HEAD_TAG;
		cmd->se_cmd_flags |= SCF_SCSI_CONTROL_SG_IO_CDB;
		break;
	default:
		pr_warn("TARGET_CORE[%s]: Unsupported SCSI Opcode"
			" 0x%02x, sending CHECK_CONDITION.\n",
			cmd->se_tfo->get_fabric_name(), cdb[0]);
		goto out_unsupported_cdb;
	}

	if (cmd->unknown_data_length)
		cmd->data_length = size;

	if (size != cmd->data_length) {
		pr_warn("TARGET_CORE[%s]: Expected Transfer Length:"
			" %u does not match SCSI CDB Length: %u for SAM Opcode:"
			" 0x%02x\n", cmd->se_tfo->get_fabric_name(),
				cmd->data_length, size, cdb[0]);

		cmd->cmd_spdtl = size;

		if (cmd->data_direction == DMA_TO_DEVICE) {
			pr_err("Rejecting underflow/overflow"
					" WRITE data\n");
			goto out_invalid_cdb_field;
		}
		/*
		 * Reject READ_* or WRITE_* with overflow/underflow for
		 * type SCF_SCSI_DATA_SG_IO_CDB.
		 */
		if (!ret && (dev->se_sub_dev->se_dev_attrib.block_size != 512))  {
			pr_err("Failing OVERFLOW/UNDERFLOW for LBA op"
				" CDB on non 512-byte sector setup subsystem"
				" plugin: %s\n", dev->transport->name);
			/* Returns CHECK_CONDITION + INVALID_CDB_FIELD */
			goto out_invalid_cdb_field;
		}
		/*
		 * For the overflow case keep the existing fabric provided
		 * ->data_length.  Otherwise for the underflow case, reset
		 * ->data_length to the smaller SCSI expected data transfer
		 * length.
		 */
		if (size > cmd->data_length) {
			cmd->se_cmd_flags |= SCF_OVERFLOW_BIT;
			cmd->residual_count = (size - cmd->data_length);
		} else {
			cmd->se_cmd_flags |= SCF_UNDERFLOW_BIT;
			cmd->residual_count = (cmd->data_length - size);
			cmd->data_length = size;
		}
	}

	if (cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB &&
	    sectors > dev->se_sub_dev->se_dev_attrib.fabric_max_sectors) {
		printk_ratelimited(KERN_ERR "SCSI OP %02xh with too big sectors %u\n",
				   cdb[0], sectors);
		goto out_invalid_cdb_field;
	}

	/* reject any command that we don't have a handler for */
	if (!(passthrough || cmd->execute_task ||
	     (cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB)))
		goto out_unsupported_cdb;

	transport_set_supported_SAM_opcode(cmd);
	return ret;

out_unsupported_cdb:
	cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
	cmd->scsi_sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
	return -EINVAL;
out_invalid_cdb_field:
	cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
	cmd->scsi_sense_reason = TCM_INVALID_CDB_FIELD;
	return -EINVAL;
=======
	switch (cmd->sam_task_attr) {
	case TCM_HEAD_TAG:
		atomic_inc_mb(&dev->non_ordered);
		pr_debug("Added HEAD_OF_QUEUE for CDB: 0x%02x\n",
			 cmd->t_task_cdb[0]);
		return false;
	case TCM_ORDERED_TAG:
		atomic_inc_mb(&dev->delayed_cmd_count);

		pr_debug("Added ORDERED for CDB: 0x%02x to ordered list\n",
			 cmd->t_task_cdb[0]);
		break;
	default:
		/*
		 * For SIMPLE and UNTAGGED Task Attribute commands
		 */
		atomic_inc_mb(&dev->non_ordered);

		if (atomic_read(&dev->delayed_cmd_count) == 0)
			return false;
		break;
	}

	if (cmd->sam_task_attr != TCM_ORDERED_TAG) {
		atomic_inc_mb(&dev->delayed_cmd_count);
		/*
		 * We will account for this when we dequeue from the delayed
		 * list.
		 */
		atomic_dec_mb(&dev->non_ordered);
	}

	spin_lock_irq(&cmd->t_state_lock);
	cmd->transport_state &= ~CMD_T_SENT;
	spin_unlock_irq(&cmd->t_state_lock);

	spin_lock(&dev->delayed_cmd_lock);
	list_add_tail(&cmd->se_delayed_node, &dev->delayed_cmd_list);
	spin_unlock(&dev->delayed_cmd_lock);

	pr_debug("Added CDB: 0x%02x Task Attr: 0x%02x to delayed CMD listn",
		cmd->t_task_cdb[0], cmd->sam_task_attr);
	/*
	 * We may have no non ordered cmds when this function started or we
	 * could have raced with the last simple/head cmd completing, so kick
	 * the delayed handler here.
	 */
	schedule_work(&dev->delayed_cmd_work);
	return true;
}

void target_execute_cmd(struct se_cmd *cmd)
{
	/*
	 * Determine if frontend context caller is requesting the stopping of
	 * this command for frontend exceptions.
	 *
	 * If the received CDB has already been aborted stop processing it here.
	 */
	if (target_cmd_interrupted(cmd))
		return;

	spin_lock_irq(&cmd->t_state_lock);
	cmd->t_state = TRANSPORT_PROCESSING;
	cmd->transport_state |= CMD_T_ACTIVE | CMD_T_SENT;
	spin_unlock_irq(&cmd->t_state_lock);

	if (target_write_prot_action(cmd))
		return;

	if (target_handle_task_attr(cmd))
		return;

	__target_execute_cmd(cmd, true);
}
EXPORT_SYMBOL(target_execute_cmd);

/*
 * Process all commands up to the last received ORDERED task attribute which
 * requires another blocking boundary
 */
void target_do_delayed_work(struct work_struct *work)
{
	struct se_device *dev = container_of(work, struct se_device,
					     delayed_cmd_work);

	spin_lock(&dev->delayed_cmd_lock);
	while (!dev->ordered_sync_in_progress) {
		struct se_cmd *cmd;

		if (list_empty(&dev->delayed_cmd_list))
			break;

		cmd = list_entry(dev->delayed_cmd_list.next,
				 struct se_cmd, se_delayed_node);

		if (cmd->sam_task_attr == TCM_ORDERED_TAG) {
			/*
			 * Check if we started with:
			 * [ordered] [simple] [ordered]
			 * and we are now at the last ordered so we have to wait
			 * for the simple cmd.
			 */
			if (atomic_read(&dev->non_ordered) > 0)
				break;

			dev->ordered_sync_in_progress = true;
		}

		list_del(&cmd->se_delayed_node);
		atomic_dec_mb(&dev->delayed_cmd_count);
		spin_unlock(&dev->delayed_cmd_lock);

		if (cmd->sam_task_attr != TCM_ORDERED_TAG)
			atomic_inc_mb(&dev->non_ordered);

		cmd->transport_state |= CMD_T_SENT;

		__target_execute_cmd(cmd, true);

		spin_lock(&dev->delayed_cmd_lock);
	}
	spin_unlock(&dev->delayed_cmd_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Called from I/O completion to determine which dormant/delayed
 * and ordered cmds need to have their tasks added to the execution queue.
 */
static void transport_complete_task_attr(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
<<<<<<< HEAD
	struct se_cmd *cmd_p, *cmd_tmp;
	int new_active_tasks = 0;

	if (cmd->sam_task_attr == MSG_SIMPLE_TAG) {
		atomic_dec(&dev->simple_cmds);
		smp_mb__after_atomic_dec();
		dev->dev_cur_ordered_id++;
		pr_debug("Incremented dev->dev_cur_ordered_id: %u for"
			" SIMPLE: %u\n", dev->dev_cur_ordered_id,
			cmd->se_ordered_id);
	} else if (cmd->sam_task_attr == MSG_HEAD_TAG) {
		dev->dev_cur_ordered_id++;
		pr_debug("Incremented dev_cur_ordered_id: %u for"
			" HEAD_OF_QUEUE: %u\n", dev->dev_cur_ordered_id,
			cmd->se_ordered_id);
	} else if (cmd->sam_task_attr == MSG_ORDERED_TAG) {
		atomic_dec(&dev->dev_ordered_sync);
		smp_mb__after_atomic_dec();

		dev->dev_cur_ordered_id++;
		pr_debug("Incremented dev_cur_ordered_id: %u for ORDERED:"
			" %u\n", dev->dev_cur_ordered_id, cmd->se_ordered_id);
	}
	/*
	 * Process all commands up to the last received
	 * ORDERED task attribute which requires another blocking
	 * boundary
	 */
	spin_lock(&dev->delayed_cmd_lock);
	list_for_each_entry_safe(cmd_p, cmd_tmp,
			&dev->delayed_cmd_list, se_delayed_node) {

		list_del(&cmd_p->se_delayed_node);
		spin_unlock(&dev->delayed_cmd_lock);

		pr_debug("Calling add_tasks() for"
			" cmd_p: 0x%02x Task Attr: 0x%02x"
			" Dormant -> Active, se_ordered_id: %u\n",
			cmd_p->t_task_cdb[0],
			cmd_p->sam_task_attr, cmd_p->se_ordered_id);

		transport_add_tasks_from_cmd(cmd_p);
		new_active_tasks++;

		spin_lock(&dev->delayed_cmd_lock);
		if (cmd_p->sam_task_attr == MSG_ORDERED_TAG)
			break;
	}
	spin_unlock(&dev->delayed_cmd_lock);
	/*
	 * If new tasks have become active, wake up the transport thread
	 * to do the processing of the Active tasks.
	 */
	if (new_active_tasks != 0)
		wake_up_interruptible(&dev->dev_queue_obj.thread_wq);
=======

	if (dev->transport_flags & TRANSPORT_FLAG_PASSTHROUGH)
		return;

	if (!(cmd->se_cmd_flags & SCF_TASK_ATTR_SET))
		goto restart;

	if (cmd->sam_task_attr == TCM_SIMPLE_TAG) {
		atomic_dec_mb(&dev->non_ordered);
		dev->dev_cur_ordered_id++;
	} else if (cmd->sam_task_attr == TCM_HEAD_TAG) {
		atomic_dec_mb(&dev->non_ordered);
		dev->dev_cur_ordered_id++;
		pr_debug("Incremented dev_cur_ordered_id: %u for HEAD_OF_QUEUE\n",
			 dev->dev_cur_ordered_id);
	} else if (cmd->sam_task_attr == TCM_ORDERED_TAG) {
		spin_lock(&dev->delayed_cmd_lock);
		dev->ordered_sync_in_progress = false;
		spin_unlock(&dev->delayed_cmd_lock);

		dev->dev_cur_ordered_id++;
		pr_debug("Incremented dev_cur_ordered_id: %u for ORDERED\n",
			 dev->dev_cur_ordered_id);
	}
	cmd->se_cmd_flags &= ~SCF_TASK_ATTR_SET;

restart:
	if (atomic_read(&dev->delayed_cmd_count) > 0)
		schedule_work(&dev->delayed_cmd_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void transport_complete_qf(struct se_cmd *cmd)
{
	int ret = 0;

<<<<<<< HEAD
	if (cmd->se_dev->dev_task_attr_type == SAM_TASK_ATTR_EMULATED)
		transport_complete_task_attr(cmd);

	if (cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE) {
		ret = cmd->se_tfo->queue_status(cmd);
		goto out;
	}

	switch (cmd->data_direction) {
	case DMA_FROM_DEVICE:
		ret = cmd->se_tfo->queue_data_in(cmd);
		break;
	case DMA_TO_DEVICE:
		if (cmd->t_bidi_data_sg) {
			ret = cmd->se_tfo->queue_data_in(cmd);
			if (ret < 0)
				break;
		}
		/* Fall through for DMA_TO_DEVICE */
	case DMA_NONE:
=======
	transport_complete_task_attr(cmd);
	/*
	 * If a fabric driver ->write_pending() or ->queue_data_in() callback
	 * has returned neither -ENOMEM or -EAGAIN, assume it's fatal and
	 * the same callbacks should not be retried.  Return CHECK_CONDITION
	 * if a scsi_status is not already set.
	 *
	 * If a fabric driver ->queue_status() has returned non zero, always
	 * keep retrying no matter what..
	 */
	if (cmd->t_state == TRANSPORT_COMPLETE_QF_ERR) {
		if (cmd->scsi_status)
			goto queue_status;

		translate_sense_reason(cmd, TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE);
		goto queue_status;
	}

	/*
	 * Check if we need to send a sense buffer from
	 * the struct se_cmd in question. We do NOT want
	 * to take this path of the IO has been marked as
	 * needing to be treated like a "normal read". This
	 * is the case if it's a tape read, and either the
	 * FM, EOM, or ILI bits are set, but there is no
	 * sense data.
	 */
	if (!(cmd->se_cmd_flags & SCF_TREAT_READ_AS_NORMAL) &&
	    cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE)
		goto queue_status;

	switch (cmd->data_direction) {
	case DMA_FROM_DEVICE:
		/* queue status if not treating this as a normal read */
		if (cmd->scsi_status &&
		    !(cmd->se_cmd_flags & SCF_TREAT_READ_AS_NORMAL))
			goto queue_status;

		trace_target_cmd_complete(cmd);
		ret = cmd->se_tfo->queue_data_in(cmd);
		break;
	case DMA_TO_DEVICE:
		if (cmd->se_cmd_flags & SCF_BIDI) {
			ret = cmd->se_tfo->queue_data_in(cmd);
			break;
		}
		fallthrough;
	case DMA_NONE:
queue_status:
		trace_target_cmd_complete(cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = cmd->se_tfo->queue_status(cmd);
		break;
	default:
		break;
	}

<<<<<<< HEAD
out:
	if (ret < 0) {
		transport_handle_queue_full(cmd, cmd->se_dev);
=======
	if (ret < 0) {
		transport_handle_queue_full(cmd, cmd->se_dev, ret, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}
	transport_lun_remove_cmd(cmd);
	transport_cmd_check_stop_to_fabric(cmd);
}

<<<<<<< HEAD
static void transport_handle_queue_full(
	struct se_cmd *cmd,
	struct se_device *dev)
{
	spin_lock_irq(&dev->qf_cmd_lock);
	list_add_tail(&cmd->se_qf_node, &cmd->se_dev->qf_cmd_list);
	atomic_inc(&dev->dev_qf_count);
	smp_mb__after_atomic_inc();
=======
static void transport_handle_queue_full(struct se_cmd *cmd, struct se_device *dev,
					int err, bool write_pending)
{
	/*
	 * -EAGAIN or -ENOMEM signals retry of ->write_pending() and/or
	 * ->queue_data_in() callbacks from new process context.
	 *
	 * Otherwise for other errors, transport_complete_qf() will send
	 * CHECK_CONDITION via ->queue_status() instead of attempting to
	 * retry associated fabric driver data-transfer callbacks.
	 */
	if (err == -EAGAIN || err == -ENOMEM) {
		cmd->t_state = (write_pending) ? TRANSPORT_COMPLETE_QF_WP :
						 TRANSPORT_COMPLETE_QF_OK;
	} else {
		pr_warn_ratelimited("Got unknown fabric queue status: %d\n", err);
		cmd->t_state = TRANSPORT_COMPLETE_QF_ERR;
	}

	spin_lock_irq(&dev->qf_cmd_lock);
	list_add_tail(&cmd->se_qf_node, &cmd->se_dev->qf_cmd_list);
	atomic_inc_mb(&dev->dev_qf_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irq(&cmd->se_dev->qf_cmd_lock);

	schedule_work(&cmd->se_dev->qf_work_queue);
}

<<<<<<< HEAD
static void target_complete_ok_work(struct work_struct *work)
{
	struct se_cmd *cmd = container_of(work, struct se_cmd, work);
	int reason = 0, ret;
=======
static bool target_read_prot_action(struct se_cmd *cmd)
{
	switch (cmd->prot_op) {
	case TARGET_PROT_DIN_STRIP:
		if (!(cmd->se_sess->sup_prot_ops & TARGET_PROT_DIN_STRIP)) {
			u32 sectors = cmd->data_length >>
				  ilog2(cmd->se_dev->dev_attrib.block_size);

			cmd->pi_err = sbc_dif_verify(cmd, cmd->t_task_lba,
						     sectors, 0, cmd->t_prot_sg,
						     0);
			if (cmd->pi_err)
				return true;
		}
		break;
	case TARGET_PROT_DIN_INSERT:
		if (cmd->se_sess->sup_prot_ops & TARGET_PROT_DIN_INSERT)
			break;

		sbc_dif_generate(cmd);
		break;
	default:
		break;
	}

	return false;
}

static void target_complete_ok_work(struct work_struct *work)
{
	struct se_cmd *cmd = container_of(work, struct se_cmd, work);
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check if we need to move delayed/dormant tasks from cmds on the
	 * delayed execution list after a HEAD_OF_QUEUE or ORDERED Task
	 * Attribute.
	 */
<<<<<<< HEAD
	if (cmd->se_dev->dev_task_attr_type == SAM_TASK_ATTR_EMULATED)
		transport_complete_task_attr(cmd);
=======
	transport_complete_task_attr(cmd);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check to schedule QUEUE_FULL work, or execute an existing
	 * cmd->transport_qf_callback()
	 */
	if (atomic_read(&cmd->se_dev->dev_qf_count) != 0)
		schedule_work(&cmd->se_dev->qf_work_queue);

	/*
<<<<<<< HEAD
	 * Check if we need to retrieve a sense buffer from
	 * the struct se_cmd in question.
	 */
	if (cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE) {
		if (transport_get_sense_data(cmd) < 0)
			reason = TCM_NON_EXISTENT_LUN;

		/*
		 * Only set when an struct se_task->task_scsi_status returned
		 * a non GOOD status.
		 */
		if (cmd->scsi_status) {
			ret = transport_send_check_condition_and_sense(
					cmd, reason, 1);
			if (ret == -EAGAIN || ret == -ENOMEM)
=======
	 * Check if we need to send a sense buffer from
	 * the struct se_cmd in question. We do NOT want
	 * to take this path of the IO has been marked as
	 * needing to be treated like a "normal read". This
	 * is the case if it's a tape read, and either the
	 * FM, EOM, or ILI bits are set, but there is no
	 * sense data.
	 */
	if (!(cmd->se_cmd_flags & SCF_TREAT_READ_AS_NORMAL) &&
	    cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE) {
		WARN_ON(!cmd->scsi_status);
		ret = transport_send_check_condition_and_sense(
					cmd, 0, 1);
		if (ret)
			goto queue_full;

		transport_lun_remove_cmd(cmd);
		transport_cmd_check_stop_to_fabric(cmd);
		return;
	}
	/*
	 * Check for a callback, used by amongst other things
	 * XDWRITE_READ_10 and COMPARE_AND_WRITE emulation.
	 */
	if (cmd->transport_complete_callback) {
		sense_reason_t rc;
		bool caw = (cmd->se_cmd_flags & SCF_COMPARE_AND_WRITE);
		bool zero_dl = !(cmd->data_length);
		int post_ret = 0;

		rc = cmd->transport_complete_callback(cmd, true, &post_ret);
		if (!rc && !post_ret) {
			if (caw && zero_dl)
				goto queue_rsp;

			return;
		} else if (rc) {
			ret = transport_send_check_condition_and_sense(cmd,
						rc, 0);
			if (ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto queue_full;

			transport_lun_remove_cmd(cmd);
			transport_cmd_check_stop_to_fabric(cmd);
			return;
		}
	}
<<<<<<< HEAD
	/*
	 * Check for a callback, used by amongst other things
	 * XDWRITE_READ_10 emulation.
	 */
	if (cmd->transport_complete_callback)
		cmd->transport_complete_callback(cmd);

	switch (cmd->data_direction) {
	case DMA_FROM_DEVICE:
		spin_lock(&cmd->se_lun->lun_sep_lock);
		if (cmd->se_lun->lun_sep) {
			cmd->se_lun->lun_sep->sep_stats.tx_data_octets +=
					cmd->data_length;
		}
		spin_unlock(&cmd->se_lun->lun_sep_lock);

		ret = cmd->se_tfo->queue_data_in(cmd);
		if (ret == -EAGAIN || ret == -ENOMEM)
			goto queue_full;
		break;
	case DMA_TO_DEVICE:
		spin_lock(&cmd->se_lun->lun_sep_lock);
		if (cmd->se_lun->lun_sep) {
			cmd->se_lun->lun_sep->sep_stats.rx_data_octets +=
				cmd->data_length;
		}
		spin_unlock(&cmd->se_lun->lun_sep_lock);
		/*
		 * Check if we need to send READ payload for BIDI-COMMAND
		 */
		if (cmd->t_bidi_data_sg) {
			spin_lock(&cmd->se_lun->lun_sep_lock);
			if (cmd->se_lun->lun_sep) {
				cmd->se_lun->lun_sep->sep_stats.tx_data_octets +=
					cmd->data_length;
			}
			spin_unlock(&cmd->se_lun->lun_sep_lock);
			ret = cmd->se_tfo->queue_data_in(cmd);
			if (ret == -EAGAIN || ret == -ENOMEM)
				goto queue_full;
			break;
		}
		/* Fall through for DMA_TO_DEVICE */
	case DMA_NONE:
		ret = cmd->se_tfo->queue_status(cmd);
		if (ret == -EAGAIN || ret == -ENOMEM)
=======

queue_rsp:
	switch (cmd->data_direction) {
	case DMA_FROM_DEVICE:
		/*
		 * if this is a READ-type IO, but SCSI status
		 * is set, then skip returning data and just
		 * return the status -- unless this IO is marked
		 * as needing to be treated as a normal read,
		 * in which case we want to go ahead and return
		 * the data. This happens, for example, for tape
		 * reads with the FM, EOM, or ILI bits set, with
		 * no sense data.
		 */
		if (cmd->scsi_status &&
		    !(cmd->se_cmd_flags & SCF_TREAT_READ_AS_NORMAL))
			goto queue_status;

		atomic_long_add(cmd->data_length,
				&cmd->se_lun->lun_stats.tx_data_octets);
		/*
		 * Perform READ_STRIP of PI using software emulation when
		 * backend had PI enabled, if the transport will not be
		 * performing hardware READ_STRIP offload.
		 */
		if (target_read_prot_action(cmd)) {
			ret = transport_send_check_condition_and_sense(cmd,
						cmd->pi_err, 0);
			if (ret)
				goto queue_full;

			transport_lun_remove_cmd(cmd);
			transport_cmd_check_stop_to_fabric(cmd);
			return;
		}

		trace_target_cmd_complete(cmd);
		ret = cmd->se_tfo->queue_data_in(cmd);
		if (ret)
			goto queue_full;
		break;
	case DMA_TO_DEVICE:
		atomic_long_add(cmd->data_length,
				&cmd->se_lun->lun_stats.rx_data_octets);
		/*
		 * Check if we need to send READ payload for BIDI-COMMAND
		 */
		if (cmd->se_cmd_flags & SCF_BIDI) {
			atomic_long_add(cmd->data_length,
					&cmd->se_lun->lun_stats.tx_data_octets);
			ret = cmd->se_tfo->queue_data_in(cmd);
			if (ret)
				goto queue_full;
			break;
		}
		fallthrough;
	case DMA_NONE:
queue_status:
		trace_target_cmd_complete(cmd);
		ret = cmd->se_tfo->queue_status(cmd);
		if (ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto queue_full;
		break;
	default:
		break;
	}

	transport_lun_remove_cmd(cmd);
	transport_cmd_check_stop_to_fabric(cmd);
	return;

queue_full:
	pr_debug("Handling complete_ok QUEUE_FULL: se_cmd: %p,"
		" data_direction: %d\n", cmd, cmd->data_direction);
<<<<<<< HEAD
	cmd->t_state = TRANSPORT_COMPLETE_QF_OK;
	transport_handle_queue_full(cmd, cmd->se_dev);
}

static void transport_free_dev_tasks(struct se_cmd *cmd)
{
	struct se_task *task, *task_tmp;
	unsigned long flags;
	LIST_HEAD(dispose_list);

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	list_for_each_entry_safe(task, task_tmp,
				&cmd->t_task_list, t_list) {
		if (!(task->task_flags & TF_ACTIVE))
			list_move_tail(&task->t_list, &dispose_list);
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	while (!list_empty(&dispose_list)) {
		task = list_first_entry(&dispose_list, struct se_task, t_list);

		if (task->task_sg != cmd->t_data_sg &&
		    task->task_sg != cmd->t_bidi_data_sg)
			kfree(task->task_sg);

		list_del(&task->t_list);

		cmd->se_dev->transport->free_task(task);
	}
}

static inline void transport_free_sgl(struct scatterlist *sgl, int nents)
{
	struct scatterlist *sg;
	int count;

	for_each_sg(sgl, sg, nents, count)
		__free_page(sg_page(sg));

	kfree(sgl);
=======

	transport_handle_queue_full(cmd, cmd->se_dev, ret, false);
}

void target_free_sgl(struct scatterlist *sgl, int nents)
{
	sgl_free_n_order(sgl, nents, 0);
}
EXPORT_SYMBOL(target_free_sgl);

static inline void transport_reset_sgl_orig(struct se_cmd *cmd)
{
	/*
	 * Check for saved t_data_sg that may be used for COMPARE_AND_WRITE
	 * emulation, and free + reset pointers if necessary..
	 */
	if (!cmd->t_data_sg_orig)
		return;

	kfree(cmd->t_data_sg);
	cmd->t_data_sg = cmd->t_data_sg_orig;
	cmd->t_data_sg_orig = NULL;
	cmd->t_data_nents = cmd->t_data_nents_orig;
	cmd->t_data_nents_orig = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void transport_free_pages(struct se_cmd *cmd)
{
<<<<<<< HEAD
	if (cmd->se_cmd_flags & SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC)
		return;

	transport_free_sgl(cmd->t_data_sg, cmd->t_data_nents);
	cmd->t_data_sg = NULL;
	cmd->t_data_nents = 0;

	transport_free_sgl(cmd->t_bidi_data_sg, cmd->t_bidi_data_nents);
=======
	if (!(cmd->se_cmd_flags & SCF_PASSTHROUGH_PROT_SG_TO_MEM_NOALLOC)) {
		target_free_sgl(cmd->t_prot_sg, cmd->t_prot_nents);
		cmd->t_prot_sg = NULL;
		cmd->t_prot_nents = 0;
	}

	if (cmd->se_cmd_flags & SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC) {
		/*
		 * Release special case READ buffer payload required for
		 * SG_TO_MEM_NOALLOC to function with COMPARE_AND_WRITE
		 */
		if (cmd->se_cmd_flags & SCF_COMPARE_AND_WRITE) {
			target_free_sgl(cmd->t_bidi_data_sg,
					   cmd->t_bidi_data_nents);
			cmd->t_bidi_data_sg = NULL;
			cmd->t_bidi_data_nents = 0;
		}
		transport_reset_sgl_orig(cmd);
		return;
	}
	transport_reset_sgl_orig(cmd);

	target_free_sgl(cmd->t_data_sg, cmd->t_data_nents);
	cmd->t_data_sg = NULL;
	cmd->t_data_nents = 0;

	target_free_sgl(cmd->t_bidi_data_sg, cmd->t_bidi_data_nents);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmd->t_bidi_data_sg = NULL;
	cmd->t_bidi_data_nents = 0;
}

<<<<<<< HEAD
/**
 * transport_release_cmd - free a command
 * @cmd:       command to free
 *
 * This routine unconditionally frees a command, and reference counting
 * or list removal must be done in the caller.
 */
static void transport_release_cmd(struct se_cmd *cmd)
{
	BUG_ON(!cmd->se_tfo);

	if (cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)
		core_tmr_release_req(cmd->se_tmr_req);
	if (cmd->t_task_cdb != cmd->__t_task_cdb)
		kfree(cmd->t_task_cdb);
	/*
	 * If this cmd has been setup with target_get_sess_cmd(), drop
	 * the kref and call ->release_cmd() in kref callback.
	 */
	 if (cmd->check_release != 0) {
		target_put_sess_cmd(cmd->se_sess, cmd);
		return;
	}
	cmd->se_tfo->release_cmd(cmd);
}

/**
 * transport_put_cmd - release a reference to a command
 * @cmd:       command to release
 *
 * This routine releases our reference to the command and frees it if possible.
 */
static void transport_put_cmd(struct se_cmd *cmd)
{
	unsigned long flags;
	int free_tasks = 0;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (atomic_read(&cmd->t_fe_count)) {
		if (!atomic_dec_and_test(&cmd->t_fe_count))
			goto out_busy;
	}

	if (atomic_read(&cmd->t_se_count)) {
		if (!atomic_dec_and_test(&cmd->t_se_count))
			goto out_busy;
	}

	if (cmd->transport_state & CMD_T_DEV_ACTIVE) {
		cmd->transport_state &= ~CMD_T_DEV_ACTIVE;
		transport_all_task_dev_remove_state(cmd);
		free_tasks = 1;
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	if (free_tasks != 0)
		transport_free_dev_tasks(cmd);

	transport_free_pages(cmd);
	transport_release_cmd(cmd);
	return;
out_busy:
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);
}

/*
 * transport_generic_map_mem_to_cmd - Use fabric-alloced pages instead of
 * allocating in the core.
 * @cmd:  Associated se_cmd descriptor
 * @mem:  SGL style memory for TCM WRITE / READ
 * @sg_mem_num: Number of SGL elements
 * @mem_bidi_in: SGL style memory for TCM BIDI READ
 * @sg_mem_bidi_num: Number of BIDI READ SGL elements
 *
 * Return: nonzero return cmd was rejected for -ENOMEM or inproper usage
 * of parameters.
 */
int transport_generic_map_mem_to_cmd(
	struct se_cmd *cmd,
	struct scatterlist *sgl,
	u32 sgl_count,
	struct scatterlist *sgl_bidi,
	u32 sgl_bidi_count)
{
	if (!sgl || !sgl_count)
		return 0;

	if ((cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB) ||
	    (cmd->se_cmd_flags & SCF_SCSI_CONTROL_SG_IO_CDB)) {
		/*
		 * Reject SCSI data overflow with map_mem_to_cmd() as incoming
		 * scatterlists already have been set to follow what the fabric
		 * passes for the original expected data transfer length.
		 */
		if (cmd->se_cmd_flags & SCF_OVERFLOW_BIT) {
			pr_warn("Rejecting SCSI DATA overflow for fabric using"
				" SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC\n");
			cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			cmd->scsi_sense_reason = TCM_INVALID_CDB_FIELD;
			return -EINVAL;
		}

		cmd->t_data_sg = sgl;
		cmd->t_data_nents = sgl_count;

		if (sgl_bidi && sgl_bidi_count) {
			cmd->t_bidi_data_sg = sgl_bidi;
			cmd->t_bidi_data_nents = sgl_bidi_count;
		}
		cmd->se_cmd_flags |= SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC;
	}

	return 0;
}
EXPORT_SYMBOL(transport_generic_map_mem_to_cmd);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void *transport_kmap_data_sg(struct se_cmd *cmd)
{
	struct scatterlist *sg = cmd->t_data_sg;
	struct page **pages;
	int i;

<<<<<<< HEAD
	BUG_ON(!sg);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We need to take into account a possible offset here for fabrics like
	 * tcm_loop who may be using a contig buffer from the SCSI midlayer for
	 * control CDBs passed as SGLs via transport_generic_map_mem_to_cmd()
	 */
	if (!cmd->t_data_nents)
		return NULL;
<<<<<<< HEAD
	else if (cmd->t_data_nents == 1)
		return kmap(sg_page(sg)) + sg->offset;

	/* >1 page. use vmap */
	pages = kmalloc(sizeof(*pages) * cmd->t_data_nents, GFP_KERNEL);
=======

	BUG_ON(!sg);
	if (cmd->t_data_nents == 1)
		return kmap(sg_page(sg)) + sg->offset;

	/* >1 page. use vmap */
	pages = kmalloc_array(cmd->t_data_nents, sizeof(*pages), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pages)
		return NULL;

	/* convert sg[] to pages[] */
	for_each_sg(cmd->t_data_sg, sg, cmd->t_data_nents, i) {
		pages[i] = sg_page(sg);
	}

	cmd->t_data_vmap = vmap(pages, cmd->t_data_nents,  VM_MAP, PAGE_KERNEL);
	kfree(pages);
	if (!cmd->t_data_vmap)
		return NULL;

	return cmd->t_data_vmap + cmd->t_data_sg[0].offset;
}
EXPORT_SYMBOL(transport_kmap_data_sg);

void transport_kunmap_data_sg(struct se_cmd *cmd)
{
	if (!cmd->t_data_nents) {
		return;
	} else if (cmd->t_data_nents == 1) {
		kunmap(sg_page(cmd->t_data_sg));
		return;
	}

	vunmap(cmd->t_data_vmap);
	cmd->t_data_vmap = NULL;
}
EXPORT_SYMBOL(transport_kunmap_data_sg);

<<<<<<< HEAD
static int
transport_generic_get_mem(struct se_cmd *cmd)
{
	u32 length = cmd->data_length;
	unsigned int nents;
	struct page *page;
	gfp_t zero_flag;
	int i = 0;

	nents = DIV_ROUND_UP(length, PAGE_SIZE);
	cmd->t_data_sg = kmalloc(sizeof(struct scatterlist) * nents, GFP_KERNEL);
	if (!cmd->t_data_sg)
		return -ENOMEM;

	cmd->t_data_nents = nents;
	sg_init_table(cmd->t_data_sg, nents);

	zero_flag = cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB ? 0 : __GFP_ZERO;

	while (length) {
		u32 page_len = min_t(u32, length, PAGE_SIZE);
		page = alloc_page(GFP_KERNEL | zero_flag);
		if (!page)
			goto out;

		sg_set_page(&cmd->t_data_sg[i], page, page_len, 0);
		length -= page_len;
		i++;
	}
	return 0;

out:
	while (i > 0) {
		i--;
		__free_page(sg_page(&cmd->t_data_sg[i]));
	}
	kfree(cmd->t_data_sg);
	cmd->t_data_sg = NULL;
	return -ENOMEM;
}

/* Reduce sectors if they are too long for the device */
static inline sector_t transport_limit_task_sectors(
	struct se_device *dev,
	unsigned long long lba,
	sector_t sectors)
{
	sectors = min_t(sector_t, sectors, dev->se_sub_dev->se_dev_attrib.max_sectors);

	if (dev->transport->get_device_type(dev) == TYPE_DISK)
		if ((lba + sectors) > transport_dev_end_lba(dev))
			sectors = ((transport_dev_end_lba(dev) - lba) + 1);

	return sectors;
}


/*
 * This function can be used by HW target mode drivers to create a linked
 * scatterlist from all contiguously allocated struct se_task->task_sg[].
 * This is intended to be called during the completion path by TCM Core
 * when struct target_core_fabric_ops->check_task_sg_chaining is enabled.
 */
void transport_do_task_sg_chain(struct se_cmd *cmd)
{
	struct scatterlist *sg_first = NULL;
	struct scatterlist *sg_prev = NULL;
	int sg_prev_nents = 0;
	struct scatterlist *sg;
	struct se_task *task;
	u32 chained_nents = 0;
	int i;

	BUG_ON(!cmd->se_tfo->task_sg_chaining);

	/*
	 * Walk the struct se_task list and setup scatterlist chains
	 * for each contiguously allocated struct se_task->task_sg[].
	 */
	list_for_each_entry(task, &cmd->t_task_list, t_list) {
		if (!task->task_sg)
			continue;

		if (!sg_first) {
			sg_first = task->task_sg;
			chained_nents = task->task_sg_nents;
		} else {
			sg_chain(sg_prev, sg_prev_nents, task->task_sg);
			chained_nents += task->task_sg_nents;
		}
		/*
		 * For the padded tasks, use the extra SGL vector allocated
		 * in transport_allocate_data_tasks() for the sg_prev_nents
		 * offset into sg_chain() above.
		 *
		 * We do not need the padding for the last task (or a single
		 * task), but in that case we will never use the sg_prev_nents
		 * value below which would be incorrect.
		 */
		sg_prev_nents = (task->task_sg_nents + 1);
		sg_prev = task->task_sg;
	}
	/*
	 * Setup the starting pointer and total t_tasks_sg_linked_no including
	 * padding SGs for linking and to mark the end.
	 */
	cmd->t_tasks_sg_chained = sg_first;
	cmd->t_tasks_sg_chained_no = chained_nents;

	pr_debug("Setup cmd: %p cmd->t_tasks_sg_chained: %p and"
		" t_tasks_sg_chained_no: %u\n", cmd, cmd->t_tasks_sg_chained,
		cmd->t_tasks_sg_chained_no);

	for_each_sg(cmd->t_tasks_sg_chained, sg,
			cmd->t_tasks_sg_chained_no, i) {

		pr_debug("SG[%d]: %p page: %p length: %d offset: %d\n",
			i, sg, sg_page(sg), sg->length, sg->offset);
		if (sg_is_chain(sg))
			pr_debug("SG: %p sg_is_chain=1\n", sg);
		if (sg_is_last(sg))
			pr_debug("SG: %p sg_is_last=1\n", sg);
	}
}
EXPORT_SYMBOL(transport_do_task_sg_chain);

/*
 * Break up cmd into chunks transport can handle
 */
static int
transport_allocate_data_tasks(struct se_cmd *cmd,
	enum dma_data_direction data_direction,
	struct scatterlist *cmd_sg, unsigned int sgl_nents)
{
	struct se_device *dev = cmd->se_dev;
	int task_count, i;
	unsigned long long lba;
	sector_t sectors, dev_max_sectors;
	u32 sector_size;

	if (transport_cmd_get_valid_sectors(cmd) < 0)
		return -EINVAL;

	dev_max_sectors = dev->se_sub_dev->se_dev_attrib.max_sectors;
	sector_size = dev->se_sub_dev->se_dev_attrib.block_size;

	WARN_ON(cmd->data_length % sector_size);

	lba = cmd->t_task_lba;
	sectors = DIV_ROUND_UP(cmd->data_length, sector_size);
	task_count = DIV_ROUND_UP_SECTOR_T(sectors, dev_max_sectors);

	/*
	 * If we need just a single task reuse the SG list in the command
	 * and avoid a lot of work.
	 */
	if (task_count == 1) {
		struct se_task *task;
		unsigned long flags;

		task = transport_generic_get_task(cmd, data_direction);
		if (!task)
			return -ENOMEM;

		task->task_sg = cmd_sg;
		task->task_sg_nents = sgl_nents;

		task->task_lba = lba;
		task->task_sectors = sectors;
		task->task_size = task->task_sectors * sector_size;

		spin_lock_irqsave(&cmd->t_state_lock, flags);
		list_add_tail(&task->t_list, &cmd->t_task_list);
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		return task_count;
	}

	for (i = 0; i < task_count; i++) {
		struct se_task *task;
		unsigned int task_size, task_sg_nents_padded;
		struct scatterlist *sg;
		unsigned long flags;
		int count;

		task = transport_generic_get_task(cmd, data_direction);
		if (!task)
			return -ENOMEM;

		task->task_lba = lba;
		task->task_sectors = min(sectors, dev_max_sectors);
		task->task_size = task->task_sectors * sector_size;

		/*
		 * This now assumes that passed sg_ents are in PAGE_SIZE chunks
		 * in order to calculate the number per task SGL entries
		 */
		task->task_sg_nents = DIV_ROUND_UP(task->task_size, PAGE_SIZE);
		/*
		 * Check if the fabric module driver is requesting that all
		 * struct se_task->task_sg[] be chained together..  If so,
		 * then allocate an extra padding SG entry for linking and
		 * marking the end of the chained SGL for every task except
		 * the last one for (task_count > 1) operation, or skipping
		 * the extra padding for the (task_count == 1) case.
		 */
		if (cmd->se_tfo->task_sg_chaining && (i < (task_count - 1))) {
			task_sg_nents_padded = (task->task_sg_nents + 1);
		} else
			task_sg_nents_padded = task->task_sg_nents;

		task->task_sg = kmalloc(sizeof(struct scatterlist) *
					task_sg_nents_padded, GFP_KERNEL);
		if (!task->task_sg) {
			cmd->se_dev->transport->free_task(task);
			return -ENOMEM;
		}

		sg_init_table(task->task_sg, task_sg_nents_padded);

		task_size = task->task_size;

		/* Build new sgl, only up to task_size */
		for_each_sg(task->task_sg, sg, task->task_sg_nents, count) {
			if (cmd_sg->length > task_size)
				break;

			*sg = *cmd_sg;
			task_size -= cmd_sg->length;
			cmd_sg = sg_next(cmd_sg);
		}

		lba += task->task_sectors;
		sectors -= task->task_sectors;

		spin_lock_irqsave(&cmd->t_state_lock, flags);
		list_add_tail(&task->t_list, &cmd->t_task_list);
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
	}

	return task_count;
}

static int
transport_allocate_control_task(struct se_cmd *cmd)
{
	struct se_task *task;
	unsigned long flags;

	/* Workaround for handling zero-length control CDBs */
	if ((cmd->se_cmd_flags & SCF_SCSI_CONTROL_SG_IO_CDB) &&
	    !cmd->data_length)
		return 0;

	task = transport_generic_get_task(cmd, cmd->data_direction);
	if (!task)
		return -ENOMEM;

	task->task_sg = cmd->t_data_sg;
	task->task_size = cmd->data_length;
	task->task_sg_nents = cmd->t_data_nents;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	list_add_tail(&task->t_list, &cmd->t_task_list);
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	/* Success! Return number of tasks allocated */
	return 1;
}

/*
 * Allocate any required ressources to execute the command, and either place
 * it on the execution queue if possible.  For writes we might not have the
 * payload yet, thus notify the fabric via a call to ->write_pending instead.
 */
int transport_generic_new_cmd(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	int task_cdbs, task_cdbs_bidi = 0;
	int set_counts = 1;
	int ret = 0;

	/*
	 * Determine is the TCM fabric module has already allocated physical
=======
int
target_alloc_sgl(struct scatterlist **sgl, unsigned int *nents, u32 length,
		 bool zero_page, bool chainable)
{
	gfp_t gfp = GFP_KERNEL | (zero_page ? __GFP_ZERO : 0);

	*sgl = sgl_alloc_order(length, 0, chainable, gfp, nents);
	return *sgl ? 0 : -ENOMEM;
}
EXPORT_SYMBOL(target_alloc_sgl);

/*
 * Allocate any required resources to execute the command.  For writes we
 * might not have the payload yet, so notify the fabric via a call to
 * ->write_pending instead. Otherwise place it on the execution queue.
 */
sense_reason_t
transport_generic_new_cmd(struct se_cmd *cmd)
{
	unsigned long flags;
	int ret = 0;
	bool zero_flag = !(cmd->se_cmd_flags & SCF_SCSI_DATA_CDB);

	if (cmd->prot_op != TARGET_PROT_NORMAL &&
	    !(cmd->se_cmd_flags & SCF_PASSTHROUGH_PROT_SG_TO_MEM_NOALLOC)) {
		ret = target_alloc_sgl(&cmd->t_prot_sg, &cmd->t_prot_nents,
				       cmd->prot_length, true, false);
		if (ret < 0)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	/*
	 * Determine if the TCM fabric module has already allocated physical
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * memory, and is directly calling transport_generic_map_mem_to_cmd()
	 * beforehand.
	 */
	if (!(cmd->se_cmd_flags & SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC) &&
	    cmd->data_length) {
<<<<<<< HEAD
		ret = transport_generic_get_mem(cmd);
		if (ret < 0)
			goto out_fail;
	}

	/*
	 * For BIDI command set up the read tasks first.
	 */
	if (cmd->t_bidi_data_sg &&
	    dev->transport->transport_type != TRANSPORT_PLUGIN_PHBA_PDEV) {
		BUG_ON(!(cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB));

		task_cdbs_bidi = transport_allocate_data_tasks(cmd,
				DMA_FROM_DEVICE, cmd->t_bidi_data_sg,
				cmd->t_bidi_data_nents);
		if (task_cdbs_bidi <= 0)
			goto out_fail;

		atomic_inc(&cmd->t_fe_count);
		atomic_inc(&cmd->t_se_count);
		set_counts = 0;
	}

	if (cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB) {
		task_cdbs = transport_allocate_data_tasks(cmd,
					cmd->data_direction, cmd->t_data_sg,
					cmd->t_data_nents);
	} else {
		task_cdbs = transport_allocate_control_task(cmd);
	}

	if (task_cdbs < 0)
		goto out_fail;
	else if (!task_cdbs && (cmd->se_cmd_flags & SCF_SCSI_DATA_SG_IO_CDB)) {
		spin_lock_irq(&cmd->t_state_lock);
		cmd->t_state = TRANSPORT_COMPLETE;
		cmd->transport_state |= CMD_T_ACTIVE;
		spin_unlock_irq(&cmd->t_state_lock);

		if (cmd->t_task_cdb[0] == REQUEST_SENSE) {
			u8 ua_asc = 0, ua_ascq = 0;

			core_scsi3_ua_clear_for_request_sense(cmd,
					&ua_asc, &ua_ascq);
		}

		INIT_WORK(&cmd->work, target_complete_ok_work);
		queue_work(target_completion_wq, &cmd->work);
		return 0;
	}

	if (set_counts) {
		atomic_inc(&cmd->t_fe_count);
		atomic_inc(&cmd->t_se_count);
	}

	cmd->t_task_list_num = (task_cdbs + task_cdbs_bidi);
	atomic_set(&cmd->t_task_cdbs_left, cmd->t_task_list_num);
	atomic_set(&cmd->t_task_cdbs_ex_left, cmd->t_task_list_num);

	/*
	 * For WRITEs, let the fabric know its buffer is ready..
	 * This WRITE struct se_cmd (and all of its associated struct se_task's)
	 * will be added to the struct se_device execution queue after its WRITE
	 * data has arrived. (ie: It gets handled by the transport processing
	 * thread a second time)
	 */
	if (cmd->data_direction == DMA_TO_DEVICE) {
		transport_add_tasks_to_state_queue(cmd);
		return transport_generic_write_pending(cmd);
	}
	/*
	 * Everything else but a WRITE, add the struct se_cmd's struct se_task's
	 * to the execution queue.
	 */
	transport_execute_tasks(cmd);
	return 0;

out_fail:
	cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
	cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	return -EINVAL;
}
EXPORT_SYMBOL(transport_generic_new_cmd);

/*	transport_generic_process_write():
 *
 *
 */
void transport_generic_process_write(struct se_cmd *cmd)
{
	transport_execute_tasks(cmd);
}
EXPORT_SYMBOL(transport_generic_process_write);

static void transport_write_pending_qf(struct se_cmd *cmd)
{
	int ret;

	ret = cmd->se_tfo->write_pending(cmd);
	if (ret == -EAGAIN || ret == -ENOMEM) {
		pr_debug("Handling write_pending QUEUE__FULL: se_cmd: %p\n",
			 cmd);
		transport_handle_queue_full(cmd, cmd->se_dev);
	}
}

static int transport_generic_write_pending(struct se_cmd *cmd)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	cmd->t_state = TRANSPORT_WRITE_PENDING;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	/*
	 * Clear the se_cmd for WRITE_PENDING status in order to set
	 * CMD_T_ACTIVE so that transport_generic_handle_data can be called
	 * from HW target mode interrupt code.  This is safe to be called
	 * with transport_off=1 before the cmd->se_tfo->write_pending
	 * because the se_cmd->se_lun pointer is not being cleared.
	 */
	transport_cmd_check_stop(cmd, 1, 0);

	/*
	 * Call the fabric write_pending function here to let the
	 * frontend know that WRITE buffers are ready.
	 */
	ret = cmd->se_tfo->write_pending(cmd);
	if (ret == -EAGAIN || ret == -ENOMEM)
		goto queue_full;
	else if (ret < 0)
		return ret;

	return 1;

queue_full:
	pr_debug("Handling write_pending QUEUE__FULL: se_cmd: %p\n", cmd);
	cmd->t_state = TRANSPORT_COMPLETE_QF_WP;
	transport_handle_queue_full(cmd, cmd->se_dev);
	return 0;
}

void transport_generic_free_cmd(struct se_cmd *cmd, int wait_for_tasks)
{
	if (!(cmd->se_cmd_flags & SCF_SE_LUN_CMD)) {
		if (wait_for_tasks && (cmd->se_cmd_flags & SCF_SCSI_TMR_CDB))
			 transport_wait_for_tasks(cmd);

		transport_release_cmd(cmd);
	} else {
		if (wait_for_tasks)
			transport_wait_for_tasks(cmd);

		core_dec_lacl_count(cmd->se_sess->se_node_acl, cmd);

		if (cmd->se_lun)
			transport_lun_remove_cmd(cmd);

		transport_free_dev_tasks(cmd);

		transport_put_cmd(cmd);
	}
}
EXPORT_SYMBOL(transport_generic_free_cmd);

/* target_get_sess_cmd - Add command to active ->sess_cmd_list
 * @se_sess:	session to reference
 * @se_cmd:	command descriptor to add
 * @ack_kref:	Signal that fabric will perform an ack target_put_sess_cmd()
 */
void target_get_sess_cmd(struct se_session *se_sess, struct se_cmd *se_cmd,
			bool ack_kref)
{
	unsigned long flags;

	kref_init(&se_cmd->cmd_kref);
=======

		if ((cmd->se_cmd_flags & SCF_BIDI) ||
		    (cmd->se_cmd_flags & SCF_COMPARE_AND_WRITE)) {
			u32 bidi_length;

			if (cmd->se_cmd_flags & SCF_COMPARE_AND_WRITE)
				bidi_length = cmd->t_task_nolb *
					      cmd->se_dev->dev_attrib.block_size;
			else
				bidi_length = cmd->data_length;

			ret = target_alloc_sgl(&cmd->t_bidi_data_sg,
					       &cmd->t_bidi_data_nents,
					       bidi_length, zero_flag, false);
			if (ret < 0)
				return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}

		ret = target_alloc_sgl(&cmd->t_data_sg, &cmd->t_data_nents,
				       cmd->data_length, zero_flag, false);
		if (ret < 0)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	} else if ((cmd->se_cmd_flags & SCF_COMPARE_AND_WRITE) &&
		    cmd->data_length) {
		/*
		 * Special case for COMPARE_AND_WRITE with fabrics
		 * using SCF_PASSTHROUGH_SG_TO_MEM_NOALLOC.
		 */
		u32 caw_length = cmd->t_task_nolb *
				 cmd->se_dev->dev_attrib.block_size;

		ret = target_alloc_sgl(&cmd->t_bidi_data_sg,
				       &cmd->t_bidi_data_nents,
				       caw_length, zero_flag, false);
		if (ret < 0)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}
	/*
	 * If this command is not a write we can execute it right here,
	 * for write buffers we need to notify the fabric driver first
	 * and let it call back once the write buffers are ready.
	 */
	target_add_to_state_list(cmd);
	if (cmd->data_direction != DMA_TO_DEVICE || cmd->data_length == 0) {
		target_execute_cmd(cmd);
		return 0;
	}

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	cmd->t_state = TRANSPORT_WRITE_PENDING;
	/*
	 * Determine if frontend context caller is requesting the stopping of
	 * this command for frontend exceptions.
	 */
	if (cmd->transport_state & CMD_T_STOP &&
	    !cmd->se_tfo->write_pending_must_be_called) {
		pr_debug("%s:%d CMD_T_STOP for ITT: 0x%08llx\n",
			 __func__, __LINE__, cmd->tag);

		spin_unlock_irqrestore(&cmd->t_state_lock, flags);

		complete_all(&cmd->t_transport_stop_comp);
		return 0;
	}
	cmd->transport_state &= ~CMD_T_ACTIVE;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	ret = cmd->se_tfo->write_pending(cmd);
	if (ret)
		goto queue_full;

	return 0;

queue_full:
	pr_debug("Handling write_pending QUEUE__FULL: se_cmd: %p\n", cmd);
	transport_handle_queue_full(cmd, cmd->se_dev, ret, true);
	return 0;
}
EXPORT_SYMBOL(transport_generic_new_cmd);

static void transport_write_pending_qf(struct se_cmd *cmd)
{
	unsigned long flags;
	int ret;
	bool stop;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	stop = (cmd->transport_state & (CMD_T_STOP | CMD_T_ABORTED));
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	if (stop) {
		pr_debug("%s:%d CMD_T_STOP|CMD_T_ABORTED for ITT: 0x%08llx\n",
			__func__, __LINE__, cmd->tag);
		complete_all(&cmd->t_transport_stop_comp);
		return;
	}

	ret = cmd->se_tfo->write_pending(cmd);
	if (ret) {
		pr_debug("Handling write_pending QUEUE__FULL: se_cmd: %p\n",
			 cmd);
		transport_handle_queue_full(cmd, cmd->se_dev, ret, true);
	}
}

static bool
__transport_wait_for_tasks(struct se_cmd *, bool, bool *, bool *,
			   unsigned long *flags);

static void target_wait_free_cmd(struct se_cmd *cmd, bool *aborted, bool *tas)
{
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	__transport_wait_for_tasks(cmd, true, aborted, tas, &flags);
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);
}

/*
 * Call target_put_sess_cmd() and wait until target_release_cmd_kref(@cmd) has
 * finished.
 */
void target_put_cmd_and_wait(struct se_cmd *cmd)
{
	DECLARE_COMPLETION_ONSTACK(compl);

	WARN_ON_ONCE(cmd->abrt_compl);
	cmd->abrt_compl = &compl;
	target_put_sess_cmd(cmd);
	wait_for_completion(&compl);
}

/*
 * This function is called by frontend drivers after processing of a command
 * has finished.
 *
 * The protocol for ensuring that either the regular frontend command
 * processing flow or target_handle_abort() code drops one reference is as
 * follows:
 * - Calling .queue_data_in(), .queue_status() or queue_tm_rsp() will cause
 *   the frontend driver to call this function synchronously or asynchronously.
 *   That will cause one reference to be dropped.
 * - During regular command processing the target core sets CMD_T_COMPLETE
 *   before invoking one of the .queue_*() functions.
 * - The code that aborts commands skips commands and TMFs for which
 *   CMD_T_COMPLETE has been set.
 * - CMD_T_ABORTED is set atomically after the CMD_T_COMPLETE check for
 *   commands that will be aborted.
 * - If the CMD_T_ABORTED flag is set but CMD_T_TAS has not been set
 *   transport_generic_free_cmd() skips its call to target_put_sess_cmd().
 * - For aborted commands for which CMD_T_TAS has been set .queue_status() will
 *   be called and will drop a reference.
 * - For aborted commands for which CMD_T_TAS has not been set .aborted_task()
 *   will be called. target_handle_abort() will drop the final reference.
 */
int transport_generic_free_cmd(struct se_cmd *cmd, int wait_for_tasks)
{
	DECLARE_COMPLETION_ONSTACK(compl);
	int ret = 0;
	bool aborted = false, tas = false;

	if (wait_for_tasks)
		target_wait_free_cmd(cmd, &aborted, &tas);

	if (cmd->se_cmd_flags & SCF_SE_LUN_CMD) {
		/*
		 * Handle WRITE failure case where transport_generic_new_cmd()
		 * has already added se_cmd to state_list, but fabric has
		 * failed command before I/O submission.
		 */
		if (cmd->state_active)
			target_remove_from_state_list(cmd);

		if (cmd->se_lun)
			transport_lun_remove_cmd(cmd);
	}
	if (aborted)
		cmd->free_compl = &compl;
	ret = target_put_sess_cmd(cmd);
	if (aborted) {
		pr_debug("Detected CMD_T_ABORTED for ITT: %llu\n", cmd->tag);
		wait_for_completion(&compl);
		ret = 1;
	}
	return ret;
}
EXPORT_SYMBOL(transport_generic_free_cmd);

/**
 * target_get_sess_cmd - Verify the session is accepting cmds and take ref
 * @se_cmd:	command descriptor to add
 * @ack_kref:	Signal that fabric will perform an ack target_put_sess_cmd()
 */
int target_get_sess_cmd(struct se_cmd *se_cmd, bool ack_kref)
{
	int ret = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Add a second kref if the fabric caller is expecting to handle
	 * fabric acknowledgement that requires two target_put_sess_cmd()
	 * invocations before se_cmd descriptor release.
	 */
<<<<<<< HEAD
	if (ack_kref == true) {
=======
	if (ack_kref) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kref_get(&se_cmd->cmd_kref);
		se_cmd->se_cmd_flags |= SCF_ACK_KREF;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&se_sess->sess_cmd_lock, flags);
	list_add_tail(&se_cmd->se_cmd_list, &se_sess->sess_cmd_list);
	se_cmd->check_release = 1;
	spin_unlock_irqrestore(&se_sess->sess_cmd_lock, flags);
}
EXPORT_SYMBOL(target_get_sess_cmd);

static void target_release_cmd_kref(struct kref *kref)
{
	struct se_cmd *se_cmd = container_of(kref, struct se_cmd, cmd_kref);
	struct se_session *se_sess = se_cmd->se_sess;
	unsigned long flags;

	spin_lock_irqsave(&se_sess->sess_cmd_lock, flags);
	if (list_empty(&se_cmd->se_cmd_list)) {
		spin_unlock_irqrestore(&se_sess->sess_cmd_lock, flags);
		se_cmd->se_tfo->release_cmd(se_cmd);
		return;
	}
	if (se_sess->sess_tearing_down && se_cmd->cmd_wait_set) {
		spin_unlock_irqrestore(&se_sess->sess_cmd_lock, flags);
		complete(&se_cmd->cmd_wait_comp);
		return;
	}
	list_del(&se_cmd->se_cmd_list);
	spin_unlock_irqrestore(&se_sess->sess_cmd_lock, flags);

	se_cmd->se_tfo->release_cmd(se_cmd);
}

/* target_put_sess_cmd - Check for active I/O shutdown via kref_put
 * @se_sess:	session to reference
 * @se_cmd:	command descriptor to drop
 */
int target_put_sess_cmd(struct se_session *se_sess, struct se_cmd *se_cmd)
=======
	/*
	 * Users like xcopy do not use counters since they never do a stop
	 * and wait.
	 */
	if (se_cmd->cmd_cnt) {
		if (!percpu_ref_tryget_live(&se_cmd->cmd_cnt->refcnt))
			ret = -ESHUTDOWN;
	}
	if (ret && ack_kref)
		target_put_sess_cmd(se_cmd);

	return ret;
}
EXPORT_SYMBOL(target_get_sess_cmd);

static void target_free_cmd_mem(struct se_cmd *cmd)
{
	transport_free_pages(cmd);

	if (cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)
		core_tmr_release_req(cmd->se_tmr_req);
	if (cmd->t_task_cdb != cmd->__t_task_cdb)
		kfree(cmd->t_task_cdb);
}

static void target_release_cmd_kref(struct kref *kref)
{
	struct se_cmd *se_cmd = container_of(kref, struct se_cmd, cmd_kref);
	struct target_cmd_counter *cmd_cnt = se_cmd->cmd_cnt;
	struct completion *free_compl = se_cmd->free_compl;
	struct completion *abrt_compl = se_cmd->abrt_compl;

	target_free_cmd_mem(se_cmd);
	se_cmd->se_tfo->release_cmd(se_cmd);
	if (free_compl)
		complete(free_compl);
	if (abrt_compl)
		complete(abrt_compl);

	if (cmd_cnt)
		percpu_ref_put(&cmd_cnt->refcnt);
}

/**
 * target_put_sess_cmd - decrease the command reference count
 * @se_cmd:	command to drop a reference from
 *
 * Returns 1 if and only if this target_put_sess_cmd() call caused the
 * refcount to drop to zero. Returns zero otherwise.
 */
int target_put_sess_cmd(struct se_cmd *se_cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return kref_put(&se_cmd->cmd_kref, target_release_cmd_kref);
}
EXPORT_SYMBOL(target_put_sess_cmd);

<<<<<<< HEAD
/* target_splice_sess_cmd_list - Split active cmds into sess_wait_list
 * @se_sess:	session to split
 */
void target_splice_sess_cmd_list(struct se_session *se_sess)
{
	struct se_cmd *se_cmd;
	unsigned long flags;

	WARN_ON(!list_empty(&se_sess->sess_wait_list));
	INIT_LIST_HEAD(&se_sess->sess_wait_list);

	spin_lock_irqsave(&se_sess->sess_cmd_lock, flags);
	se_sess->sess_tearing_down = 1;

	list_splice_init(&se_sess->sess_cmd_list, &se_sess->sess_wait_list);

	list_for_each_entry(se_cmd, &se_sess->sess_wait_list, se_cmd_list)
		se_cmd->cmd_wait_set = 1;

	spin_unlock_irqrestore(&se_sess->sess_cmd_lock, flags);
}
EXPORT_SYMBOL(target_splice_sess_cmd_list);

/* target_wait_for_sess_cmds - Wait for outstanding descriptors
 * @se_sess:    session to wait for active I/O
 * @wait_for_tasks:	Make extra transport_wait_for_tasks call
 */
void target_wait_for_sess_cmds(
	struct se_session *se_sess,
	int wait_for_tasks)
{
	struct se_cmd *se_cmd, *tmp_cmd;
	bool rc = false;

	list_for_each_entry_safe(se_cmd, tmp_cmd,
				&se_sess->sess_wait_list, se_cmd_list) {
		list_del(&se_cmd->se_cmd_list);

		pr_debug("Waiting for se_cmd: %p t_state: %d, fabric state:"
			" %d\n", se_cmd, se_cmd->t_state,
			se_cmd->se_tfo->get_cmd_state(se_cmd));

		if (wait_for_tasks) {
			pr_debug("Calling transport_wait_for_tasks se_cmd: %p t_state: %d,"
				" fabric state: %d\n", se_cmd, se_cmd->t_state,
				se_cmd->se_tfo->get_cmd_state(se_cmd));

			rc = transport_wait_for_tasks(se_cmd);

			pr_debug("After transport_wait_for_tasks se_cmd: %p t_state: %d,"
				" fabric state: %d\n", se_cmd, se_cmd->t_state,
				se_cmd->se_tfo->get_cmd_state(se_cmd));
		}

		if (!rc) {
			wait_for_completion(&se_cmd->cmd_wait_comp);
			pr_debug("After cmd_wait_comp: se_cmd: %p t_state: %d"
				" fabric state: %d\n", se_cmd, se_cmd->t_state,
				se_cmd->se_tfo->get_cmd_state(se_cmd));
		}

		se_cmd->se_tfo->release_cmd(se_cmd);
	}
}
EXPORT_SYMBOL(target_wait_for_sess_cmds);

/*	transport_lun_wait_for_tasks():
 *
 *	Called from ConfigFS context to stop the passed struct se_cmd to allow
 *	an struct se_lun to be successfully shutdown.
 */
static int transport_lun_wait_for_tasks(struct se_cmd *cmd, struct se_lun *lun)
{
	unsigned long flags;
	int ret;
	/*
	 * If the frontend has already requested this struct se_cmd to
	 * be stopped, we can safely ignore this struct se_cmd.
	 */
	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->transport_state & CMD_T_STOP) {
		cmd->transport_state &= ~CMD_T_LUN_STOP;

		pr_debug("ConfigFS ITT[0x%08x] - CMD_T_STOP, skipping\n",
			 cmd->se_tfo->get_task_tag(cmd));
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		transport_cmd_check_stop(cmd, 1, 0);
		return -EPERM;
	}
	cmd->transport_state |= CMD_T_LUN_FE_STOP;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	wake_up_interruptible(&cmd->se_dev->dev_queue_obj.thread_wq);

	ret = transport_stop_tasks_for_cmd(cmd);

	pr_debug("ConfigFS: cmd: %p t_tasks: %d stop tasks ret:"
			" %d\n", cmd, cmd->t_task_list_num, ret);
	if (!ret) {
		pr_debug("ConfigFS: ITT[0x%08x] - stopping cmd....\n",
				cmd->se_tfo->get_task_tag(cmd));
		wait_for_completion(&cmd->transport_lun_stop_comp);
		pr_debug("ConfigFS: ITT[0x%08x] - stopped cmd....\n",
				cmd->se_tfo->get_task_tag(cmd));
	}
	transport_remove_cmd_from_queue(cmd);

	return 0;
}

static void __transport_clear_lun_from_sessions(struct se_lun *lun)
{
	struct se_cmd *cmd = NULL;
	unsigned long lun_flags, cmd_flags;
	/*
	 * Do exception processing and return CHECK_CONDITION status to the
	 * Initiator Port.
	 */
	spin_lock_irqsave(&lun->lun_cmd_lock, lun_flags);
	while (!list_empty(&lun->lun_cmd_list)) {
		cmd = list_first_entry(&lun->lun_cmd_list,
		       struct se_cmd, se_lun_node);
		list_del_init(&cmd->se_lun_node);

		/*
		 * This will notify iscsi_target_transport.c:
		 * transport_cmd_check_stop() that a LUN shutdown is in
		 * progress for the iscsi_cmd_t.
		 */
		spin_lock(&cmd->t_state_lock);
		pr_debug("SE_LUN[%d] - Setting cmd->transport"
			"_lun_stop for  ITT: 0x%08x\n",
			cmd->se_lun->unpacked_lun,
			cmd->se_tfo->get_task_tag(cmd));
		cmd->transport_state |= CMD_T_LUN_STOP;
		spin_unlock(&cmd->t_state_lock);

		spin_unlock_irqrestore(&lun->lun_cmd_lock, lun_flags);

		if (!cmd->se_lun) {
			pr_err("ITT: 0x%08x, [i,t]_state: %u/%u\n",
				cmd->se_tfo->get_task_tag(cmd),
				cmd->se_tfo->get_cmd_state(cmd), cmd->t_state);
			BUG();
		}
		/*
		 * If the Storage engine still owns the iscsi_cmd_t, determine
		 * and/or stop its context.
		 */
		pr_debug("SE_LUN[%d] - ITT: 0x%08x before transport"
			"_lun_wait_for_tasks()\n", cmd->se_lun->unpacked_lun,
			cmd->se_tfo->get_task_tag(cmd));

		if (transport_lun_wait_for_tasks(cmd, cmd->se_lun) < 0) {
			spin_lock_irqsave(&lun->lun_cmd_lock, lun_flags);
			continue;
		}

		pr_debug("SE_LUN[%d] - ITT: 0x%08x after transport_lun"
			"_wait_for_tasks(): SUCCESS\n",
			cmd->se_lun->unpacked_lun,
			cmd->se_tfo->get_task_tag(cmd));

		spin_lock_irqsave(&cmd->t_state_lock, cmd_flags);
		if (!(cmd->transport_state & CMD_T_DEV_ACTIVE)) {
			spin_unlock_irqrestore(&cmd->t_state_lock, cmd_flags);
			goto check_cond;
		}
		cmd->transport_state &= ~CMD_T_DEV_ACTIVE;
		transport_all_task_dev_remove_state(cmd);
		spin_unlock_irqrestore(&cmd->t_state_lock, cmd_flags);

		transport_free_dev_tasks(cmd);
		/*
		 * The Storage engine stopped this struct se_cmd before it was
		 * send to the fabric frontend for delivery back to the
		 * Initiator Node.  Return this SCSI CDB back with an
		 * CHECK_CONDITION status.
		 */
check_cond:
		transport_send_check_condition_and_sense(cmd,
				TCM_NON_EXISTENT_LUN, 0);
		/*
		 *  If the fabric frontend is waiting for this iscsi_cmd_t to
		 * be released, notify the waiting thread now that LU has
		 * finished accessing it.
		 */
		spin_lock_irqsave(&cmd->t_state_lock, cmd_flags);
		if (cmd->transport_state & CMD_T_LUN_FE_STOP) {
			pr_debug("SE_LUN[%d] - Detected FE stop for"
				" struct se_cmd: %p ITT: 0x%08x\n",
				lun->unpacked_lun,
				cmd, cmd->se_tfo->get_task_tag(cmd));

			spin_unlock_irqrestore(&cmd->t_state_lock,
					cmd_flags);
			transport_cmd_check_stop(cmd, 1, 0);
			complete(&cmd->transport_lun_fe_stop_comp);
			spin_lock_irqsave(&lun->lun_cmd_lock, lun_flags);
			continue;
		}
		pr_debug("SE_LUN[%d] - ITT: 0x%08x finished processing\n",
			lun->unpacked_lun, cmd->se_tfo->get_task_tag(cmd));

		spin_unlock_irqrestore(&cmd->t_state_lock, cmd_flags);
		spin_lock_irqsave(&lun->lun_cmd_lock, lun_flags);
	}
	spin_unlock_irqrestore(&lun->lun_cmd_lock, lun_flags);
}

static int transport_clear_lun_thread(void *p)
{
	struct se_lun *lun = p;

	__transport_clear_lun_from_sessions(lun);
	complete(&lun->lun_shutdown_comp);

	return 0;
}

int transport_clear_lun_from_sessions(struct se_lun *lun)
{
	struct task_struct *kt;

	kt = kthread_run(transport_clear_lun_thread, lun,
			"tcm_cl_%u", lun->unpacked_lun);
	if (IS_ERR(kt)) {
		pr_err("Unable to start clear_lun thread\n");
		return PTR_ERR(kt);
	}
	wait_for_completion(&lun->lun_shutdown_comp);

	return 0;
}

/**
 * transport_wait_for_tasks - wait for completion to occur
 * @cmd:	command to wait
 *
 * Called from frontend fabric context to wait for storage engine
 * to pause and/or release frontend generated struct se_cmd.
=======
static const char *data_dir_name(enum dma_data_direction d)
{
	switch (d) {
	case DMA_BIDIRECTIONAL:	return "BIDI";
	case DMA_TO_DEVICE:	return "WRITE";
	case DMA_FROM_DEVICE:	return "READ";
	case DMA_NONE:		return "NONE";
	}

	return "(?)";
}

static const char *cmd_state_name(enum transport_state_table t)
{
	switch (t) {
	case TRANSPORT_NO_STATE:	return "NO_STATE";
	case TRANSPORT_NEW_CMD:		return "NEW_CMD";
	case TRANSPORT_WRITE_PENDING:	return "WRITE_PENDING";
	case TRANSPORT_PROCESSING:	return "PROCESSING";
	case TRANSPORT_COMPLETE:	return "COMPLETE";
	case TRANSPORT_ISTATE_PROCESSING:
					return "ISTATE_PROCESSING";
	case TRANSPORT_COMPLETE_QF_WP:	return "COMPLETE_QF_WP";
	case TRANSPORT_COMPLETE_QF_OK:	return "COMPLETE_QF_OK";
	case TRANSPORT_COMPLETE_QF_ERR:	return "COMPLETE_QF_ERR";
	}

	return "(?)";
}

static void target_append_str(char **str, const char *txt)
{
	char *prev = *str;

	*str = *str ? kasprintf(GFP_ATOMIC, "%s,%s", *str, txt) :
		kstrdup(txt, GFP_ATOMIC);
	kfree(prev);
}

/*
 * Convert a transport state bitmask into a string. The caller is
 * responsible for freeing the returned pointer.
 */
static char *target_ts_to_str(u32 ts)
{
	char *str = NULL;

	if (ts & CMD_T_ABORTED)
		target_append_str(&str, "aborted");
	if (ts & CMD_T_ACTIVE)
		target_append_str(&str, "active");
	if (ts & CMD_T_COMPLETE)
		target_append_str(&str, "complete");
	if (ts & CMD_T_SENT)
		target_append_str(&str, "sent");
	if (ts & CMD_T_STOP)
		target_append_str(&str, "stop");
	if (ts & CMD_T_FABRIC_STOP)
		target_append_str(&str, "fabric_stop");

	return str;
}

static const char *target_tmf_name(enum tcm_tmreq_table tmf)
{
	switch (tmf) {
	case TMR_ABORT_TASK:		return "ABORT_TASK";
	case TMR_ABORT_TASK_SET:	return "ABORT_TASK_SET";
	case TMR_CLEAR_ACA:		return "CLEAR_ACA";
	case TMR_CLEAR_TASK_SET:	return "CLEAR_TASK_SET";
	case TMR_LUN_RESET:		return "LUN_RESET";
	case TMR_TARGET_WARM_RESET:	return "TARGET_WARM_RESET";
	case TMR_TARGET_COLD_RESET:	return "TARGET_COLD_RESET";
	case TMR_LUN_RESET_PRO:		return "LUN_RESET_PRO";
	case TMR_UNKNOWN:		break;
	}
	return "(?)";
}

void target_show_cmd(const char *pfx, struct se_cmd *cmd)
{
	char *ts_str = target_ts_to_str(cmd->transport_state);
	const u8 *cdb = cmd->t_task_cdb;
	struct se_tmr_req *tmf = cmd->se_tmr_req;

	if (!(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)) {
		pr_debug("%scmd %#02x:%#02x with tag %#llx dir %s i_state %d t_state %s len %d refcnt %d transport_state %s\n",
			 pfx, cdb[0], cdb[1], cmd->tag,
			 data_dir_name(cmd->data_direction),
			 cmd->se_tfo->get_cmd_state(cmd),
			 cmd_state_name(cmd->t_state), cmd->data_length,
			 kref_read(&cmd->cmd_kref), ts_str);
	} else {
		pr_debug("%stmf %s with tag %#llx ref_task_tag %#llx i_state %d t_state %s refcnt %d transport_state %s\n",
			 pfx, target_tmf_name(tmf->function), cmd->tag,
			 tmf->ref_task_tag, cmd->se_tfo->get_cmd_state(cmd),
			 cmd_state_name(cmd->t_state),
			 kref_read(&cmd->cmd_kref), ts_str);
	}
	kfree(ts_str);
}
EXPORT_SYMBOL(target_show_cmd);

static void target_stop_cmd_counter_confirm(struct percpu_ref *ref)
{
	struct target_cmd_counter *cmd_cnt = container_of(ref,
						struct target_cmd_counter,
						refcnt);
	complete_all(&cmd_cnt->stop_done);
}

/**
 * target_stop_cmd_counter - Stop new IO from being added to the counter.
 * @cmd_cnt: counter to stop
 */
void target_stop_cmd_counter(struct target_cmd_counter *cmd_cnt)
{
	pr_debug("Stopping command counter.\n");
	if (!atomic_cmpxchg(&cmd_cnt->stopped, 0, 1))
		percpu_ref_kill_and_confirm(&cmd_cnt->refcnt,
					    target_stop_cmd_counter_confirm);
}
EXPORT_SYMBOL_GPL(target_stop_cmd_counter);

/**
 * target_stop_session - Stop new IO from being queued on the session.
 * @se_sess: session to stop
 */
void target_stop_session(struct se_session *se_sess)
{
	target_stop_cmd_counter(se_sess->cmd_cnt);
}
EXPORT_SYMBOL(target_stop_session);

/**
 * target_wait_for_cmds - Wait for outstanding cmds.
 * @cmd_cnt: counter to wait for active I/O for.
 */
void target_wait_for_cmds(struct target_cmd_counter *cmd_cnt)
{
	int ret;

	WARN_ON_ONCE(!atomic_read(&cmd_cnt->stopped));

	do {
		pr_debug("Waiting for running cmds to complete.\n");
		ret = wait_event_timeout(cmd_cnt->refcnt_wq,
					 percpu_ref_is_zero(&cmd_cnt->refcnt),
					 180 * HZ);
	} while (ret <= 0);

	wait_for_completion(&cmd_cnt->stop_done);
	pr_debug("Waiting for cmds done.\n");
}
EXPORT_SYMBOL_GPL(target_wait_for_cmds);

/**
 * target_wait_for_sess_cmds - Wait for outstanding commands
 * @se_sess: session to wait for active I/O
 */
void target_wait_for_sess_cmds(struct se_session *se_sess)
{
	target_wait_for_cmds(se_sess->cmd_cnt);
}
EXPORT_SYMBOL(target_wait_for_sess_cmds);

/*
 * Prevent that new percpu_ref_tryget_live() calls succeed and wait until
 * all references to the LUN have been released. Called during LUN shutdown.
 */
void transport_clear_lun_ref(struct se_lun *lun)
{
	percpu_ref_kill(&lun->lun_ref);
	wait_for_completion(&lun->lun_shutdown_comp);
}

static bool
__transport_wait_for_tasks(struct se_cmd *cmd, bool fabric_stop,
			   bool *aborted, bool *tas, unsigned long *flags)
	__releases(&cmd->t_state_lock)
	__acquires(&cmd->t_state_lock)
{
	lockdep_assert_held(&cmd->t_state_lock);

	if (fabric_stop)
		cmd->transport_state |= CMD_T_FABRIC_STOP;

	if (cmd->transport_state & CMD_T_ABORTED)
		*aborted = true;

	if (cmd->transport_state & CMD_T_TAS)
		*tas = true;

	if (!(cmd->se_cmd_flags & SCF_SE_LUN_CMD) &&
	    !(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB))
		return false;

	if (!(cmd->se_cmd_flags & SCF_SUPPORTED_SAM_OPCODE) &&
	    !(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB))
		return false;

	if (!(cmd->transport_state & CMD_T_ACTIVE))
		return false;

	if (fabric_stop && *aborted)
		return false;

	cmd->transport_state |= CMD_T_STOP;

	target_show_cmd("wait_for_tasks: Stopping ", cmd);

	spin_unlock_irqrestore(&cmd->t_state_lock, *flags);

	while (!wait_for_completion_timeout(&cmd->t_transport_stop_comp,
					    180 * HZ))
		target_show_cmd("wait for tasks: ", cmd);

	spin_lock_irqsave(&cmd->t_state_lock, *flags);
	cmd->transport_state &= ~(CMD_T_ACTIVE | CMD_T_STOP);

	pr_debug("wait_for_tasks: Stopped wait_for_completion(&cmd->"
		 "t_transport_stop_comp) for ITT: 0x%08llx\n", cmd->tag);

	return true;
}

/**
 * transport_wait_for_tasks - set CMD_T_STOP and wait for t_transport_stop_comp
 * @cmd: command to wait on
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
bool transport_wait_for_tasks(struct se_cmd *cmd)
{
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (!(cmd->se_cmd_flags & SCF_SE_LUN_CMD) &&
	    !(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return false;
	}
	/*
	 * Only perform a possible wait_for_tasks if SCF_SUPPORTED_SAM_OPCODE
	 * has been set in transport_set_supported_SAM_opcode().
	 */
	if (!(cmd->se_cmd_flags & SCF_SUPPORTED_SAM_OPCODE) &&
	    !(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return false;
	}
	/*
	 * If we are already stopped due to an external event (ie: LUN shutdown)
	 * sleep until the connection can have the passed struct se_cmd back.
	 * The cmd->transport_lun_stopped_sem will be upped by
	 * transport_clear_lun_from_sessions() once the ConfigFS context caller
	 * has completed its operation on the struct se_cmd.
	 */
	if (cmd->transport_state & CMD_T_LUN_STOP) {
		pr_debug("wait_for_tasks: Stopping"
			" wait_for_completion(&cmd->t_tasktransport_lun_fe"
			"_stop_comp); for ITT: 0x%08x\n",
			cmd->se_tfo->get_task_tag(cmd));
		/*
		 * There is a special case for WRITES where a FE exception +
		 * LUN shutdown means ConfigFS context is still sleeping on
		 * transport_lun_stop_comp in transport_lun_wait_for_tasks().
		 * We go ahead and up transport_lun_stop_comp just to be sure
		 * here.
		 */
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		complete(&cmd->transport_lun_stop_comp);
		wait_for_completion(&cmd->transport_lun_fe_stop_comp);
		spin_lock_irqsave(&cmd->t_state_lock, flags);

		transport_all_task_dev_remove_state(cmd);
		/*
		 * At this point, the frontend who was the originator of this
		 * struct se_cmd, now owns the structure and can be released through
		 * normal means below.
		 */
		pr_debug("wait_for_tasks: Stopped"
			" wait_for_completion(&cmd->t_tasktransport_lun_fe_"
			"stop_comp); for ITT: 0x%08x\n",
			cmd->se_tfo->get_task_tag(cmd));

		cmd->transport_state &= ~CMD_T_LUN_STOP;
	}

	if (!(cmd->transport_state & CMD_T_ACTIVE)) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return false;
	}

	cmd->transport_state |= CMD_T_STOP;

	pr_debug("wait_for_tasks: Stopping %p ITT: 0x%08x"
		" i_state: %d, t_state: %d, CMD_T_STOP\n",
		cmd, cmd->se_tfo->get_task_tag(cmd),
		cmd->se_tfo->get_cmd_state(cmd), cmd->t_state);

	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	wake_up_interruptible(&cmd->se_dev->dev_queue_obj.thread_wq);

	wait_for_completion(&cmd->t_transport_stop_comp);

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	cmd->transport_state &= ~(CMD_T_ACTIVE | CMD_T_STOP);

	pr_debug("wait_for_tasks: Stopped wait_for_compltion("
		"&cmd->t_transport_stop_comp) for ITT: 0x%08x\n",
		cmd->se_tfo->get_task_tag(cmd));

	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	return true;
}
EXPORT_SYMBOL(transport_wait_for_tasks);

static int transport_get_sense_codes(
	struct se_cmd *cmd,
	u8 *asc,
	u8 *ascq)
{
	*asc = cmd->scsi_asc;
	*ascq = cmd->scsi_ascq;

	return 0;
}

static int transport_set_sense_codes(
	struct se_cmd *cmd,
	u8 asc,
	u8 ascq)
{
	cmd->scsi_asc = asc;
	cmd->scsi_ascq = ascq;

	return 0;
}

int transport_send_check_condition_and_sense(
	struct se_cmd *cmd,
	u8 reason,
	int from_transport)
{
	unsigned char *buffer = cmd->sense_buffer;
	unsigned long flags;
	int offset;
	u8 asc = 0, ascq = 0;
=======
	bool ret, aborted = false, tas = false;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	ret = __transport_wait_for_tasks(cmd, false, &aborted, &tas, &flags);
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	return ret;
}
EXPORT_SYMBOL(transport_wait_for_tasks);

struct sense_detail {
	u8 key;
	u8 asc;
	u8 ascq;
	bool add_sense_info;
};

static const struct sense_detail sense_detail_table[] = {
	[TCM_NO_SENSE] = {
		.key = NOT_READY
	},
	[TCM_NON_EXISTENT_LUN] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x25 /* LOGICAL UNIT NOT SUPPORTED */
	},
	[TCM_UNSUPPORTED_SCSI_OPCODE] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x20, /* INVALID COMMAND OPERATION CODE */
	},
	[TCM_SECTOR_COUNT_TOO_MANY] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x20, /* INVALID COMMAND OPERATION CODE */
	},
	[TCM_UNKNOWN_MODE_PAGE] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x24, /* INVALID FIELD IN CDB */
	},
	[TCM_CHECK_CONDITION_ABORT_CMD] = {
		.key = ABORTED_COMMAND,
		.asc = 0x29, /* BUS DEVICE RESET FUNCTION OCCURRED */
		.ascq = 0x03,
	},
	[TCM_INCORRECT_AMOUNT_OF_DATA] = {
		.key = ABORTED_COMMAND,
		.asc = 0x0c, /* WRITE ERROR */
		.ascq = 0x0d, /* NOT ENOUGH UNSOLICITED DATA */
	},
	[TCM_INVALID_CDB_FIELD] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x24, /* INVALID FIELD IN CDB */
	},
	[TCM_INVALID_PARAMETER_LIST] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x26, /* INVALID FIELD IN PARAMETER LIST */
	},
	[TCM_TOO_MANY_TARGET_DESCS] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x26,
		.ascq = 0x06, /* TOO MANY TARGET DESCRIPTORS */
	},
	[TCM_UNSUPPORTED_TARGET_DESC_TYPE_CODE] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x26,
		.ascq = 0x07, /* UNSUPPORTED TARGET DESCRIPTOR TYPE CODE */
	},
	[TCM_TOO_MANY_SEGMENT_DESCS] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x26,
		.ascq = 0x08, /* TOO MANY SEGMENT DESCRIPTORS */
	},
	[TCM_UNSUPPORTED_SEGMENT_DESC_TYPE_CODE] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x26,
		.ascq = 0x09, /* UNSUPPORTED SEGMENT DESCRIPTOR TYPE CODE */
	},
	[TCM_PARAMETER_LIST_LENGTH_ERROR] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x1a, /* PARAMETER LIST LENGTH ERROR */
	},
	[TCM_UNEXPECTED_UNSOLICITED_DATA] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x0c, /* WRITE ERROR */
		.ascq = 0x0c, /* UNEXPECTED_UNSOLICITED_DATA */
	},
	[TCM_SERVICE_CRC_ERROR] = {
		.key = ABORTED_COMMAND,
		.asc = 0x47, /* PROTOCOL SERVICE CRC ERROR */
		.ascq = 0x05, /* N/A */
	},
	[TCM_SNACK_REJECTED] = {
		.key = ABORTED_COMMAND,
		.asc = 0x11, /* READ ERROR */
		.ascq = 0x13, /* FAILED RETRANSMISSION REQUEST */
	},
	[TCM_WRITE_PROTECTED] = {
		.key = DATA_PROTECT,
		.asc = 0x27, /* WRITE PROTECTED */
	},
	[TCM_ADDRESS_OUT_OF_RANGE] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x21, /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
	},
	[TCM_CHECK_CONDITION_UNIT_ATTENTION] = {
		.key = UNIT_ATTENTION,
	},
	[TCM_MISCOMPARE_VERIFY] = {
		.key = MISCOMPARE,
		.asc = 0x1d, /* MISCOMPARE DURING VERIFY OPERATION */
		.ascq = 0x00,
		.add_sense_info = true,
	},
	[TCM_LOGICAL_BLOCK_GUARD_CHECK_FAILED] = {
		.key = ABORTED_COMMAND,
		.asc = 0x10,
		.ascq = 0x01, /* LOGICAL BLOCK GUARD CHECK FAILED */
		.add_sense_info = true,
	},
	[TCM_LOGICAL_BLOCK_APP_TAG_CHECK_FAILED] = {
		.key = ABORTED_COMMAND,
		.asc = 0x10,
		.ascq = 0x02, /* LOGICAL BLOCK APPLICATION TAG CHECK FAILED */
		.add_sense_info = true,
	},
	[TCM_LOGICAL_BLOCK_REF_TAG_CHECK_FAILED] = {
		.key = ABORTED_COMMAND,
		.asc = 0x10,
		.ascq = 0x03, /* LOGICAL BLOCK REFERENCE TAG CHECK FAILED */
		.add_sense_info = true,
	},
	[TCM_COPY_TARGET_DEVICE_NOT_REACHABLE] = {
		.key = COPY_ABORTED,
		.asc = 0x0d,
		.ascq = 0x02, /* COPY TARGET DEVICE NOT REACHABLE */

	},
	[TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE] = {
		/*
		 * Returning ILLEGAL REQUEST would cause immediate IO errors on
		 * Solaris initiators.  Returning NOT READY instead means the
		 * operations will be retried a finite number of times and we
		 * can survive intermittent errors.
		 */
		.key = NOT_READY,
		.asc = 0x08, /* LOGICAL UNIT COMMUNICATION FAILURE */
	},
	[TCM_INSUFFICIENT_REGISTRATION_RESOURCES] = {
		/*
		 * From spc4r22 section5.7.7,5.7.8
		 * If a PERSISTENT RESERVE OUT command with a REGISTER service action
		 * or a REGISTER AND IGNORE EXISTING KEY service action or
		 * REGISTER AND MOVE service actionis attempted,
		 * but there are insufficient device server resources to complete the
		 * operation, then the command shall be terminated with CHECK CONDITION
		 * status, with the sense key set to ILLEGAL REQUEST,and the additonal
		 * sense code set to INSUFFICIENT REGISTRATION RESOURCES.
		 */
		.key = ILLEGAL_REQUEST,
		.asc = 0x55,
		.ascq = 0x04, /* INSUFFICIENT REGISTRATION RESOURCES */
	},
	[TCM_INVALID_FIELD_IN_COMMAND_IU] = {
		.key = ILLEGAL_REQUEST,
		.asc = 0x0e,
		.ascq = 0x03, /* INVALID FIELD IN COMMAND INFORMATION UNIT */
	},
	[TCM_ALUA_TG_PT_STANDBY] = {
		.key = NOT_READY,
		.asc = 0x04,
		.ascq = ASCQ_04H_ALUA_TG_PT_STANDBY,
	},
	[TCM_ALUA_TG_PT_UNAVAILABLE] = {
		.key = NOT_READY,
		.asc = 0x04,
		.ascq = ASCQ_04H_ALUA_TG_PT_UNAVAILABLE,
	},
	[TCM_ALUA_STATE_TRANSITION] = {
		.key = NOT_READY,
		.asc = 0x04,
		.ascq = ASCQ_04H_ALUA_STATE_TRANSITION,
	},
	[TCM_ALUA_OFFLINE] = {
		.key = NOT_READY,
		.asc = 0x04,
		.ascq = ASCQ_04H_ALUA_OFFLINE,
	},
};

/**
 * translate_sense_reason - translate a sense reason into T10 key, asc and ascq
 * @cmd: SCSI command in which the resulting sense buffer or SCSI status will
 *   be stored.
 * @reason: LIO sense reason code. If this argument has the value
 *   TCM_CHECK_CONDITION_UNIT_ATTENTION, try to dequeue a unit attention. If
 *   dequeuing a unit attention fails due to multiple commands being processed
 *   concurrently, set the command status to BUSY.
 *
 * Return: 0 upon success or -EINVAL if the sense buffer is too small.
 */
static void translate_sense_reason(struct se_cmd *cmd, sense_reason_t reason)
{
	const struct sense_detail *sd;
	u8 *buffer = cmd->sense_buffer;
	int r = (__force int)reason;
	u8 key, asc, ascq;
	bool desc_format = target_sense_desc_format(cmd->se_dev);

	if (r < ARRAY_SIZE(sense_detail_table) && sense_detail_table[r].key)
		sd = &sense_detail_table[r];
	else
		sd = &sense_detail_table[(__force int)
				       TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE];

	key = sd->key;
	if (reason == TCM_CHECK_CONDITION_UNIT_ATTENTION) {
		if (!core_scsi3_ua_for_check_condition(cmd, &key, &asc,
						       &ascq)) {
			cmd->scsi_status = SAM_STAT_BUSY;
			return;
		}
	} else {
		WARN_ON_ONCE(sd->asc == 0);
		asc = sd->asc;
		ascq = sd->ascq;
	}

	cmd->se_cmd_flags |= SCF_EMULATED_TASK_SENSE;
	cmd->scsi_status = SAM_STAT_CHECK_CONDITION;
	cmd->scsi_sense_length  = TRANSPORT_SENSE_BUFFER;
	scsi_build_sense_buffer(desc_format, buffer, key, asc, ascq);
	if (sd->add_sense_info)
		WARN_ON_ONCE(scsi_set_sense_information(buffer,
							cmd->scsi_sense_length,
							cmd->sense_info) < 0);
}

int
transport_send_check_condition_and_sense(struct se_cmd *cmd,
		sense_reason_t reason, int from_transport)
{
	unsigned long flags;

	WARN_ON_ONCE(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->se_cmd_flags & SCF_SENT_CHECK_CONDITION) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return 0;
	}
	cmd->se_cmd_flags |= SCF_SENT_CHECK_CONDITION;
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

<<<<<<< HEAD
	if (!reason && from_transport)
		goto after_reason;

	if (!from_transport)
		cmd->se_cmd_flags |= SCF_EMULATED_TASK_SENSE;
	/*
	 * Data Segment and SenseLength of the fabric response PDU.
	 *
	 * TRANSPORT_SENSE_BUFFER is now set to SCSI_SENSE_BUFFERSIZE
	 * from include/scsi/scsi_cmnd.h
	 */
	offset = cmd->se_tfo->set_fabric_sense_len(cmd,
				TRANSPORT_SENSE_BUFFER);
	/*
	 * Actual SENSE DATA, see SPC-3 7.23.2  SPC_SENSE_KEY_OFFSET uses
	 * SENSE KEY values from include/scsi/scsi.h
	 */
	switch (reason) {
	case TCM_NON_EXISTENT_LUN:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* LOGICAL UNIT NOT SUPPORTED */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x25;
		break;
	case TCM_UNSUPPORTED_SCSI_OPCODE:
	case TCM_SECTOR_COUNT_TOO_MANY:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* INVALID COMMAND OPERATION CODE */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x20;
		break;
	case TCM_UNKNOWN_MODE_PAGE:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* INVALID FIELD IN CDB */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x24;
		break;
	case TCM_CHECK_CONDITION_ABORT_CMD:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ABORTED COMMAND */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ABORTED_COMMAND;
		/* BUS DEVICE RESET FUNCTION OCCURRED */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x29;
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = 0x03;
		break;
	case TCM_INCORRECT_AMOUNT_OF_DATA:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ABORTED COMMAND */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ABORTED_COMMAND;
		/* WRITE ERROR */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x0c;
		/* NOT ENOUGH UNSOLICITED DATA */
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = 0x0d;
		break;
	case TCM_INVALID_CDB_FIELD:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* INVALID FIELD IN CDB */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x24;
		break;
	case TCM_INVALID_PARAMETER_LIST:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* INVALID FIELD IN PARAMETER LIST */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x26;
		break;
	case TCM_UNEXPECTED_UNSOLICITED_DATA:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ABORTED COMMAND */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ABORTED_COMMAND;
		/* WRITE ERROR */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x0c;
		/* UNEXPECTED_UNSOLICITED_DATA */
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = 0x0c;
		break;
	case TCM_SERVICE_CRC_ERROR:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ABORTED COMMAND */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ABORTED_COMMAND;
		/* PROTOCOL SERVICE CRC ERROR */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x47;
		/* N/A */
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = 0x05;
		break;
	case TCM_SNACK_REJECTED:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ABORTED COMMAND */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ABORTED_COMMAND;
		/* READ ERROR */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x11;
		/* FAILED RETRANSMISSION REQUEST */
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = 0x13;
		break;
	case TCM_WRITE_PROTECTED:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* DATA PROTECT */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = DATA_PROTECT;
		/* WRITE PROTECTED */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x27;
		break;
	case TCM_ADDRESS_OUT_OF_RANGE:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* LOGICAL BLOCK ADDRESS OUT OF RANGE */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x21;
		break;
	case TCM_CHECK_CONDITION_UNIT_ATTENTION:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* UNIT ATTENTION */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = UNIT_ATTENTION;
		core_scsi3_ua_for_check_condition(cmd, &asc, &ascq);
		buffer[offset+SPC_ASC_KEY_OFFSET] = asc;
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = ascq;
		break;
	case TCM_CHECK_CONDITION_NOT_READY:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* Not Ready */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = NOT_READY;
		transport_get_sense_codes(cmd, &asc, &ascq);
		buffer[offset+SPC_ASC_KEY_OFFSET] = asc;
		buffer[offset+SPC_ASCQ_KEY_OFFSET] = ascq;
		break;
	case TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE:
	default:
		/* CURRENT ERROR */
		buffer[offset] = 0x70;
		buffer[offset+SPC_ADD_SENSE_LEN_OFFSET] = 10;
		/* ILLEGAL REQUEST */
		buffer[offset+SPC_SENSE_KEY_OFFSET] = ILLEGAL_REQUEST;
		/* LOGICAL UNIT COMMUNICATION FAILURE */
		buffer[offset+SPC_ASC_KEY_OFFSET] = 0x08;
		break;
	}
	/*
	 * This code uses linux/include/scsi/scsi.h SAM status codes!
	 */
	cmd->scsi_status = SAM_STAT_CHECK_CONDITION;
	/*
	 * Automatically padded, this value is encoded in the fabric's
	 * data_length response PDU containing the SCSI defined sense data.
	 */
	cmd->scsi_sense_length  = TRANSPORT_SENSE_BUFFER + offset;

after_reason:
=======
	if (!from_transport)
		translate_sense_reason(cmd, reason);

	trace_target_cmd_complete(cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return cmd->se_tfo->queue_status(cmd);
}
EXPORT_SYMBOL(transport_send_check_condition_and_sense);

<<<<<<< HEAD
int transport_check_aborted_status(struct se_cmd *cmd, int send_status)
{
	int ret = 0;

	if (cmd->transport_state & CMD_T_ABORTED) {
		if (!send_status ||
		     (cmd->se_cmd_flags & SCF_SENT_DELAYED_TAS))
			return 1;
#if 0
		pr_debug("Sending delayed SAM_STAT_TASK_ABORTED"
			" status for CDB: 0x%02x ITT: 0x%08x\n",
			cmd->t_task_cdb[0],
			cmd->se_tfo->get_task_tag(cmd));
#endif
		cmd->se_cmd_flags |= SCF_SENT_DELAYED_TAS;
		cmd->se_tfo->queue_status(cmd);
		ret = 1;
	}
	return ret;
}
EXPORT_SYMBOL(transport_check_aborted_status);

void transport_send_task_abort(struct se_cmd *cmd)
{
	unsigned long flags;

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->se_cmd_flags & SCF_SENT_CHECK_CONDITION) {
		spin_unlock_irqrestore(&cmd->t_state_lock, flags);
		return;
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	/*
	 * If there are still expected incoming fabric WRITEs, we wait
	 * until until they have completed before sending a TASK_ABORTED
	 * response.  This response with TASK_ABORTED status will be
	 * queued back to fabric module by transport_check_aborted_status().
	 */
	if (cmd->data_direction == DMA_TO_DEVICE) {
		if (cmd->se_tfo->write_pending_status(cmd) != 0) {
			cmd->transport_state |= CMD_T_ABORTED;
			smp_mb__after_atomic_inc();
		}
	}
	cmd->scsi_status = SAM_STAT_TASK_ABORTED;
#if 0
	pr_debug("Setting SAM_STAT_TASK_ABORTED status for CDB: 0x%02x,"
		" ITT: 0x%08x\n", cmd->t_task_cdb[0],
		cmd->se_tfo->get_task_tag(cmd));
#endif
	cmd->se_tfo->queue_status(cmd);
}

static int transport_generic_do_tmr(struct se_cmd *cmd)
{
=======
/**
 * target_send_busy - Send SCSI BUSY status back to the initiator
 * @cmd: SCSI command for which to send a BUSY reply.
 *
 * Note: Only call this function if target_submit_cmd*() failed.
 */
int target_send_busy(struct se_cmd *cmd)
{
	WARN_ON_ONCE(cmd->se_cmd_flags & SCF_SCSI_TMR_CDB);

	cmd->scsi_status = SAM_STAT_BUSY;
	trace_target_cmd_complete(cmd);
	return cmd->se_tfo->queue_status(cmd);
}
EXPORT_SYMBOL(target_send_busy);

static void target_tmr_work(struct work_struct *work)
{
	struct se_cmd *cmd = container_of(work, struct se_cmd, work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct se_device *dev = cmd->se_dev;
	struct se_tmr_req *tmr = cmd->se_tmr_req;
	int ret;

<<<<<<< HEAD
=======
	if (cmd->transport_state & CMD_T_ABORTED)
		goto aborted;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (tmr->function) {
	case TMR_ABORT_TASK:
		core_tmr_abort_task(dev, tmr, cmd->se_sess);
		break;
	case TMR_ABORT_TASK_SET:
	case TMR_CLEAR_ACA:
	case TMR_CLEAR_TASK_SET:
		tmr->response = TMR_TASK_MGMT_FUNCTION_NOT_SUPPORTED;
		break;
	case TMR_LUN_RESET:
		ret = core_tmr_lun_reset(dev, tmr, NULL, NULL);
		tmr->response = (!ret) ? TMR_FUNCTION_COMPLETE :
					 TMR_FUNCTION_REJECTED;
<<<<<<< HEAD
=======
		if (tmr->response == TMR_FUNCTION_COMPLETE) {
			target_dev_ua_allocate(dev, 0x29,
					       ASCQ_29H_BUS_DEVICE_RESET_FUNCTION_OCCURRED);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case TMR_TARGET_WARM_RESET:
		tmr->response = TMR_FUNCTION_REJECTED;
		break;
	case TMR_TARGET_COLD_RESET:
		tmr->response = TMR_FUNCTION_REJECTED;
		break;
	default:
<<<<<<< HEAD
		pr_err("Uknown TMR function: 0x%02x.\n",
=======
		pr_err("Unknown TMR function: 0x%02x.\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				tmr->function);
		tmr->response = TMR_FUNCTION_REJECTED;
		break;
	}

<<<<<<< HEAD
	cmd->t_state = TRANSPORT_ISTATE_PROCESSING;
	cmd->se_tfo->queue_tm_rsp(cmd);

	transport_cmd_check_stop_to_fabric(cmd);
	return 0;
}

/*	transport_processing_thread():
 *
 *
 */
static int transport_processing_thread(void *param)
{
	int ret;
	struct se_cmd *cmd;
	struct se_device *dev = param;

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(dev->dev_queue_obj.thread_wq,
				atomic_read(&dev->dev_queue_obj.queue_cnt) ||
				kthread_should_stop());
		if (ret < 0)
			goto out;

get_cmd:
		cmd = transport_get_cmd_from_queue(&dev->dev_queue_obj);
		if (!cmd)
			continue;

		switch (cmd->t_state) {
		case TRANSPORT_NEW_CMD:
			BUG();
			break;
		case TRANSPORT_NEW_CMD_MAP:
			if (!cmd->se_tfo->new_cmd_map) {
				pr_err("cmd->se_tfo->new_cmd_map is"
					" NULL for TRANSPORT_NEW_CMD_MAP\n");
				BUG();
			}
			ret = cmd->se_tfo->new_cmd_map(cmd);
			if (ret < 0) {
				transport_generic_request_failure(cmd);
				break;
			}
			ret = transport_generic_new_cmd(cmd);
			if (ret < 0) {
				transport_generic_request_failure(cmd);
				break;
			}
			break;
		case TRANSPORT_PROCESS_WRITE:
			transport_generic_process_write(cmd);
			break;
		case TRANSPORT_PROCESS_TMR:
			transport_generic_do_tmr(cmd);
			break;
		case TRANSPORT_COMPLETE_QF_WP:
			transport_write_pending_qf(cmd);
			break;
		case TRANSPORT_COMPLETE_QF_OK:
			transport_complete_qf(cmd);
			break;
		default:
			pr_err("Unknown t_state: %d  for ITT: 0x%08x "
				"i_state: %d on SE LUN: %u\n",
				cmd->t_state,
				cmd->se_tfo->get_task_tag(cmd),
				cmd->se_tfo->get_cmd_state(cmd),
				cmd->se_lun->unpacked_lun);
			BUG();
		}

		goto get_cmd;
	}

out:
	WARN_ON(!list_empty(&dev->state_task_list));
	WARN_ON(!list_empty(&dev->dev_queue_obj.qobj_list));
	dev->process_thread = NULL;
	return 0;
}
=======
	if (cmd->transport_state & CMD_T_ABORTED)
		goto aborted;

	cmd->se_tfo->queue_tm_rsp(cmd);

	transport_lun_remove_cmd(cmd);
	transport_cmd_check_stop_to_fabric(cmd);
	return;

aborted:
	target_handle_abort(cmd);
}

int transport_generic_handle_tmr(
	struct se_cmd *cmd)
{
	unsigned long flags;
	bool aborted = false;

	spin_lock_irqsave(&cmd->se_dev->se_tmr_lock, flags);
	list_add_tail(&cmd->se_tmr_req->tmr_list, &cmd->se_dev->dev_tmr_list);
	spin_unlock_irqrestore(&cmd->se_dev->se_tmr_lock, flags);

	spin_lock_irqsave(&cmd->t_state_lock, flags);
	if (cmd->transport_state & CMD_T_ABORTED) {
		aborted = true;
	} else {
		cmd->t_state = TRANSPORT_ISTATE_PROCESSING;
		cmd->transport_state |= CMD_T_ACTIVE;
	}
	spin_unlock_irqrestore(&cmd->t_state_lock, flags);

	if (aborted) {
		pr_warn_ratelimited("handle_tmr caught CMD_T_ABORTED TMR %d ref_tag: %llu tag: %llu\n",
				    cmd->se_tmr_req->function,
				    cmd->se_tmr_req->ref_task_tag, cmd->tag);
		target_handle_abort(cmd);
		return 0;
	}

	INIT_WORK(&cmd->work, target_tmr_work);
	schedule_work(&cmd->work);
	return 0;
}
EXPORT_SYMBOL(transport_generic_handle_tmr);

bool
target_check_wce(struct se_device *dev)
{
	bool wce = false;

	if (dev->transport->get_write_cache)
		wce = dev->transport->get_write_cache(dev);
	else if (dev->dev_attrib.emulate_write_cache > 0)
		wce = true;

	return wce;
}

bool
target_check_fua(struct se_device *dev)
{
	return target_check_wce(dev) && dev->dev_attrib.emulate_fua_write > 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
