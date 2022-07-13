<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
 * Filename:  target_core_device.c (based on iscsi_target_device.c)
 *
 * This file contains the TCM Virtual Device and Disk Transport
 * agnostic related functions.
 *
<<<<<<< HEAD
 * Copyright (c) 2003, 2004, 2005 PyX Technologies, Inc.
 * Copyright (c) 2005-2006 SBE, Inc.  All Rights Reserved.
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
 * (c) Copyright 2003-2013 Datera, Inc.
 *
 * Nicholas A. Bellinger <nab@kernel.org>
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ******************************************************************************/

#include <linux/net.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <net/sock.h>
#include <net/tcp.h>
#include <scsi/scsi.h>
#include <scsi/scsi_device.h>
=======
#include <linux/t10-pi.h>
#include <asm/unaligned.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <scsi/scsi_common.h>
#include <scsi/scsi_proto.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <target/target_core_base.h>
#include <target/target_core_backend.h>
#include <target/target_core_fabric.h>

#include "target_core_internal.h"
#include "target_core_alua.h"
#include "target_core_pr.h"
#include "target_core_ua.h"

<<<<<<< HEAD
static void se_dev_start(struct se_device *dev);
static void se_dev_stop(struct se_device *dev);

static struct se_hba *lun0_hba;
static struct se_subsystem_dev *lun0_su_dev;
/* not static, needed by tpg.c */
struct se_device *g_lun0_dev;

int transport_lookup_cmd_lun(struct se_cmd *se_cmd, u32 unpacked_lun)
{
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = se_cmd->se_sess;
	struct se_device *dev;
	unsigned long flags;

	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}

	spin_lock_irqsave(&se_sess->se_node_acl->device_list_lock, flags);
	se_cmd->se_deve = se_sess->se_node_acl->device_list[unpacked_lun];
	if (se_cmd->se_deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		struct se_dev_entry *deve = se_cmd->se_deve;

		deve->total_cmds++;
		deve->total_bytes += se_cmd->data_length;

		if ((se_cmd->data_direction == DMA_TO_DEVICE) &&
		    (deve->lun_flags & TRANSPORT_LUNFLAGS_READ_ONLY)) {
			se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			pr_err("TARGET_CORE[%s]: Detected WRITE_PROTECTED LUN"
				" Access for 0x%08x\n",
				se_cmd->se_tfo->get_fabric_name(),
				unpacked_lun);
			spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);
			return -EACCES;
		}

		if (se_cmd->data_direction == DMA_TO_DEVICE)
			deve->write_bytes += se_cmd->data_length;
		else if (se_cmd->data_direction == DMA_FROM_DEVICE)
			deve->read_bytes += se_cmd->data_length;

		deve->deve_cmds++;

		se_lun = deve->se_lun;
		se_cmd->se_lun = deve->se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
	}
	spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);
=======
static DEFINE_MUTEX(device_mutex);
static LIST_HEAD(device_list);
static DEFINE_IDR(devices_idr);

static struct se_hba *lun0_hba;
/* not static, needed by tpg.c */
struct se_device *g_lun0_dev;

sense_reason_t
transport_lookup_cmd_lun(struct se_cmd *se_cmd)
{
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = se_cmd->se_sess;
	struct se_node_acl *nacl = se_sess->se_node_acl;
	struct se_dev_entry *deve;
	sense_reason_t ret = TCM_NO_SENSE;

	rcu_read_lock();
	deve = target_nacl_find_deve(nacl, se_cmd->orig_fe_lun);
	if (deve) {
		atomic_long_inc(&deve->total_cmds);

		if (se_cmd->data_direction == DMA_TO_DEVICE)
			atomic_long_add(se_cmd->data_length,
					&deve->write_bytes);
		else if (se_cmd->data_direction == DMA_FROM_DEVICE)
			atomic_long_add(se_cmd->data_length,
					&deve->read_bytes);

		if ((se_cmd->data_direction == DMA_TO_DEVICE) &&
		    deve->lun_access_ro) {
			pr_err("TARGET_CORE[%s]: Detected WRITE_PROTECTED LUN"
				" Access for 0x%08llx\n",
				se_cmd->se_tfo->fabric_name,
				se_cmd->orig_fe_lun);
			rcu_read_unlock();
			return TCM_WRITE_PROTECTED;
		}

		se_lun = deve->se_lun;

		if (!percpu_ref_tryget_live(&se_lun->lun_ref)) {
			se_lun = NULL;
			goto out_unlock;
		}

		se_cmd->se_lun = se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
		se_cmd->lun_ref_active = true;
	}
out_unlock:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!se_lun) {
		/*
		 * Use the se_portal_group->tpg_virt_lun0 to allow for
		 * REPORT_LUNS, et al to be returned when no active
		 * MappedLUN=0 exists for this Initiator Port.
		 */
<<<<<<< HEAD
		if (unpacked_lun != 0) {
			se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			pr_err("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
				" Access for 0x%08x\n",
				se_cmd->se_tfo->get_fabric_name(),
				unpacked_lun);
			return -ENODEV;
		}
=======
		if (se_cmd->orig_fe_lun != 0) {
			pr_err("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
				" Access for 0x%08llx from %s\n",
				se_cmd->se_tfo->fabric_name,
				se_cmd->orig_fe_lun,
				nacl->initiatorname);
			return TCM_NON_EXISTENT_LUN;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Force WRITE PROTECT for virtual LUN 0
		 */
		if ((se_cmd->data_direction != DMA_FROM_DEVICE) &&
<<<<<<< HEAD
		    (se_cmd->data_direction != DMA_NONE)) {
			se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			return -EACCES;
		}

		se_lun = &se_sess->se_tpg->tpg_virt_lun0;
		se_cmd->se_lun = &se_sess->se_tpg->tpg_virt_lun0;
		se_cmd->orig_fe_lun = 0;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
	}
	/*
	 * Determine if the struct se_lun is online.
	 * FIXME: Check for LUN_RESET + UNIT Attention
	 */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}

	/* Directly associate cmd with se_dev */
	se_cmd->se_dev = se_lun->lun_se_dev;

	/* TODO: get rid of this and use atomics for stats */
	dev = se_lun->lun_se_dev;
	spin_lock_irqsave(&dev->stats_lock, flags);
	dev->num_cmds++;
	if (se_cmd->data_direction == DMA_TO_DEVICE)
		dev->write_bytes += se_cmd->data_length;
	else if (se_cmd->data_direction == DMA_FROM_DEVICE)
		dev->read_bytes += se_cmd->data_length;
	spin_unlock_irqrestore(&dev->stats_lock, flags);

	spin_lock_irqsave(&se_lun->lun_cmd_lock, flags);
	list_add_tail(&se_cmd->se_lun_node, &se_lun->lun_cmd_list);
	spin_unlock_irqrestore(&se_lun->lun_cmd_lock, flags);

	return 0;
}
EXPORT_SYMBOL(transport_lookup_cmd_lun);

int transport_lookup_tmr_lun(struct se_cmd *se_cmd, u32 unpacked_lun)
=======
		    (se_cmd->data_direction != DMA_NONE))
			return TCM_WRITE_PROTECTED;

		se_lun = se_sess->se_tpg->tpg_virt_lun0;
		if (!percpu_ref_tryget_live(&se_lun->lun_ref))
			return TCM_NON_EXISTENT_LUN;

		se_cmd->se_lun = se_sess->se_tpg->tpg_virt_lun0;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
		se_cmd->lun_ref_active = true;
	}
	/*
	 * RCU reference protected by percpu se_lun->lun_ref taken above that
	 * must drop to zero (including initial reference) before this se_lun
	 * pointer can be kfree_rcu() by the final se_lun->lun_group put via
	 * target_core_fabric_configfs.c:target_fabric_port_release
	 */
	se_cmd->se_dev = rcu_dereference_raw(se_lun->lun_se_dev);
	atomic_long_inc(&se_cmd->se_dev->num_cmds);

	if (se_cmd->data_direction == DMA_TO_DEVICE)
		atomic_long_add(se_cmd->data_length,
				&se_cmd->se_dev->write_bytes);
	else if (se_cmd->data_direction == DMA_FROM_DEVICE)
		atomic_long_add(se_cmd->data_length,
				&se_cmd->se_dev->read_bytes);

	return ret;
}
EXPORT_SYMBOL(transport_lookup_cmd_lun);

int transport_lookup_tmr_lun(struct se_cmd *se_cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct se_dev_entry *deve;
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = se_cmd->se_sess;
<<<<<<< HEAD
	struct se_tmr_req *se_tmr = se_cmd->se_tmr_req;
	unsigned long flags;

	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}

	spin_lock_irqsave(&se_sess->se_node_acl->device_list_lock, flags);
	se_cmd->se_deve = se_sess->se_node_acl->device_list[unpacked_lun];
	deve = se_cmd->se_deve;

	if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		se_tmr->tmr_lun = deve->se_lun;
		se_cmd->se_lun = deve->se_lun;
		se_lun = deve->se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
	}
	spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);

	if (!se_lun) {
		pr_debug("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
			" Access for 0x%08x\n",
			se_cmd->se_tfo->get_fabric_name(),
			unpacked_lun);
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}
	/*
	 * Determine if the struct se_lun is online.
	 * FIXME: Check for LUN_RESET + UNIT Attention
	 */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}

	/* Directly associate cmd with se_dev */
	se_cmd->se_dev = se_lun->lun_se_dev;
	se_tmr->tmr_dev = se_lun->lun_se_dev;

	spin_lock_irqsave(&se_tmr->tmr_dev->se_tmr_lock, flags);
	list_add_tail(&se_tmr->tmr_list, &se_tmr->tmr_dev->dev_tmr_list);
	spin_unlock_irqrestore(&se_tmr->tmr_dev->se_tmr_lock, flags);
=======
	struct se_node_acl *nacl = se_sess->se_node_acl;
	struct se_tmr_req *se_tmr = se_cmd->se_tmr_req;

	rcu_read_lock();
	deve = target_nacl_find_deve(nacl, se_cmd->orig_fe_lun);
	if (deve) {
		se_lun = deve->se_lun;

		if (!percpu_ref_tryget_live(&se_lun->lun_ref)) {
			se_lun = NULL;
			goto out_unlock;
		}

		se_cmd->se_lun = se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
		se_cmd->lun_ref_active = true;
	}
out_unlock:
	rcu_read_unlock();

	if (!se_lun) {
		pr_debug("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
			" Access for 0x%08llx for %s\n",
			se_cmd->se_tfo->fabric_name,
			se_cmd->orig_fe_lun,
			nacl->initiatorname);
		return -ENODEV;
	}
	se_cmd->se_dev = rcu_dereference_raw(se_lun->lun_se_dev);
	se_tmr->tmr_dev = rcu_dereference_raw(se_lun->lun_se_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL(transport_lookup_tmr_lun);

<<<<<<< HEAD
/*
 * This function is called from core_scsi3_emulate_pro_register_and_move()
 * and core_scsi3_decode_spec_i_port(), and will increment &deve->pr_ref_count
=======
bool target_lun_is_rdonly(struct se_cmd *cmd)
{
	struct se_session *se_sess = cmd->se_sess;
	struct se_dev_entry *deve;
	bool ret;

	rcu_read_lock();
	deve = target_nacl_find_deve(se_sess->se_node_acl, cmd->orig_fe_lun);
	ret = deve && deve->lun_access_ro;
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL(target_lun_is_rdonly);

/*
 * This function is called from core_scsi3_emulate_pro_register_and_move()
 * and core_scsi3_decode_spec_i_port(), and will increment &deve->pr_kref
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * when a matching rtpi is found.
 */
struct se_dev_entry *core_get_se_deve_from_rtpi(
	struct se_node_acl *nacl,
	u16 rtpi)
{
	struct se_dev_entry *deve;
	struct se_lun *lun;
<<<<<<< HEAD
	struct se_port *port;
	struct se_portal_group *tpg = nacl->se_tpg;
	u32 i;

	spin_lock_irq(&nacl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
		deve = nacl->device_list[i];

		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;

=======
	struct se_portal_group *tpg = nacl->se_tpg;

	rcu_read_lock();
	hlist_for_each_entry_rcu(deve, &nacl->lun_entry_hlist, link) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lun = deve->se_lun;
		if (!lun) {
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
<<<<<<< HEAD
				tpg->se_tpg_tfo->get_fabric_name());
			continue;
		}
		port = lun->lun_sep;
		if (!port) {
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
			continue;
		}
		if (port->sep_rtpi != rtpi)
			continue;

		atomic_inc(&deve->pr_ref_count);
		smp_mb__after_atomic_inc();
		spin_unlock_irq(&nacl->device_list_lock);

		return deve;
	}
	spin_unlock_irq(&nacl->device_list_lock);
=======
				tpg->se_tpg_tfo->fabric_name);
			continue;
		}
		if (lun->lun_tpg->tpg_rtpi != rtpi)
			continue;

		kref_get(&deve->pr_kref);
		rcu_read_unlock();

		return deve;
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NULL;
}

<<<<<<< HEAD
int core_free_device_list_for_node(
=======
void core_free_device_list_for_node(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct se_node_acl *nacl,
	struct se_portal_group *tpg)
{
	struct se_dev_entry *deve;
<<<<<<< HEAD
	struct se_lun *lun;
	u32 i;

	if (!nacl->device_list)
		return 0;

	spin_lock_irq(&nacl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
		deve = nacl->device_list[i];

		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;

		if (!deve->se_lun) {
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
			continue;
		}
		lun = deve->se_lun;

		spin_unlock_irq(&nacl->device_list_lock);
		core_update_device_list_for_node(lun, NULL, deve->mapped_lun,
			TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg, 0);
		spin_lock_irq(&nacl->device_list_lock);
	}
	spin_unlock_irq(&nacl->device_list_lock);

	array_free(nacl->device_list, TRANSPORT_MAX_LUNS_PER_TPG);
	nacl->device_list = NULL;

	return 0;
}

void core_dec_lacl_count(struct se_node_acl *se_nacl, struct se_cmd *se_cmd)
{
	struct se_dev_entry *deve;
	unsigned long flags;

	spin_lock_irqsave(&se_nacl->device_list_lock, flags);
	deve = se_nacl->device_list[se_cmd->orig_fe_lun];
	deve->deve_cmds--;
	spin_unlock_irqrestore(&se_nacl->device_list_lock, flags);
}

void core_update_device_list_access(
	u32 mapped_lun,
	u32 lun_access,
=======

	mutex_lock(&nacl->lun_entry_mutex);
	hlist_for_each_entry_rcu(deve, &nacl->lun_entry_hlist, link)
		core_disable_device_list_for_node(deve->se_lun, deve, nacl, tpg);
	mutex_unlock(&nacl->lun_entry_mutex);
}

void core_update_device_list_access(
	u64 mapped_lun,
	bool lun_access_ro,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct se_node_acl *nacl)
{
	struct se_dev_entry *deve;

<<<<<<< HEAD
	spin_lock_irq(&nacl->device_list_lock);
	deve = nacl->device_list[mapped_lun];
	if (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_ONLY;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_WRITE;
	} else {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_WRITE;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_ONLY;
	}
	spin_unlock_irq(&nacl->device_list_lock);
}

/*      core_update_device_list_for_node():
 *
 *
 */
int core_update_device_list_for_node(
	struct se_lun *lun,
	struct se_lun_acl *lun_acl,
	u32 mapped_lun,
	u32 lun_access,
	struct se_node_acl *nacl,
	struct se_portal_group *tpg,
	int enable)
{
	struct se_port *port = lun->lun_sep;
	struct se_dev_entry *deve = nacl->device_list[mapped_lun];
	int trans = 0;
	/*
	 * If the MappedLUN entry is being disabled, the entry in
	 * port->sep_alua_list must be removed now before clearing the
	 * struct se_dev_entry pointers below as logic in
	 * core_alua_do_transition_tg_pt() depends on these being present.
	 */
	if (!enable) {
		/*
		 * deve->se_lun_acl will be NULL for demo-mode created LUNs
		 * that have not been explicitly concerted to MappedLUNs ->
		 * struct se_lun_acl, but we remove deve->alua_port_list from
		 * port->sep_alua_list. This also means that active UAs and
		 * NodeACL context specific PR metadata for demo-mode
		 * MappedLUN *deve will be released below..
		 */
		spin_lock_bh(&port->sep_alua_lock);
		list_del(&deve->alua_port_list);
		spin_unlock_bh(&port->sep_alua_lock);
	}

	spin_lock_irq(&nacl->device_list_lock);
	if (enable) {
		/*
		 * Check if the call is handling demo mode -> explict LUN ACL
		 * transition.  This transition must be for the same struct se_lun
		 * + mapped_lun that was setup in demo mode..
		 */
		if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
			if (deve->se_lun_acl != NULL) {
				pr_err("struct se_dev_entry->se_lun_acl"
					" already set for demo mode -> explict"
					" LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -EINVAL;
			}
			if (deve->se_lun != lun) {
				pr_err("struct se_dev_entry->se_lun does"
					" match passed struct se_lun for demo mode"
					" -> explict LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -EINVAL;
			}
			deve->se_lun_acl = lun_acl;
			trans = 1;
		} else {
			deve->se_lun = lun;
			deve->se_lun_acl = lun_acl;
			deve->mapped_lun = mapped_lun;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_INITIATOR_ACCESS;
		}

		if (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) {
			deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_ONLY;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_WRITE;
		} else {
			deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_WRITE;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_ONLY;
		}

		if (trans) {
			spin_unlock_irq(&nacl->device_list_lock);
			return 0;
		}
		deve->creation_time = get_jiffies_64();
		deve->attach_count++;
		spin_unlock_irq(&nacl->device_list_lock);

		spin_lock_bh(&port->sep_alua_lock);
		list_add_tail(&deve->alua_port_list, &port->sep_alua_list);
		spin_unlock_bh(&port->sep_alua_lock);

		return 0;
	}
	/*
	 * Wait for any in process SPEC_I_PT=1 or REGISTER_AND_MOVE
	 * PR operation to complete.
	 */
	spin_unlock_irq(&nacl->device_list_lock);
	while (atomic_read(&deve->pr_ref_count) != 0)
		cpu_relax();
	spin_lock_irq(&nacl->device_list_lock);
	/*
	 * Disable struct se_dev_entry LUN ACL mapping
	 */
	core_scsi3_ua_release_all(deve);
	deve->se_lun = NULL;
	deve->se_lun_acl = NULL;
	deve->lun_flags = 0;
	deve->creation_time = 0;
	deve->attach_count--;
	spin_unlock_irq(&nacl->device_list_lock);

	core_scsi3_free_pr_reg_from_nacl(lun->lun_se_dev, nacl);
	return 0;
=======
	mutex_lock(&nacl->lun_entry_mutex);
	deve = target_nacl_find_deve(nacl, mapped_lun);
	if (deve)
		deve->lun_access_ro = lun_access_ro;
	mutex_unlock(&nacl->lun_entry_mutex);
}

/*
 * Called with rcu_read_lock or nacl->device_list_lock held.
 */
struct se_dev_entry *target_nacl_find_deve(struct se_node_acl *nacl, u64 mapped_lun)
{
	struct se_dev_entry *deve;

	hlist_for_each_entry_rcu(deve, &nacl->lun_entry_hlist, link)
		if (deve->mapped_lun == mapped_lun)
			return deve;

	return NULL;
}
EXPORT_SYMBOL(target_nacl_find_deve);

void target_pr_kref_release(struct kref *kref)
{
	struct se_dev_entry *deve = container_of(kref, struct se_dev_entry,
						 pr_kref);
	complete(&deve->pr_comp);
}

/*
 * Establish UA condition on SCSI device - all LUNs
 */
void target_dev_ua_allocate(struct se_device *dev, u8 asc, u8 ascq)
{
	struct se_dev_entry *se_deve;
	struct se_lun *lun;

	spin_lock(&dev->se_port_lock);
	list_for_each_entry(lun, &dev->dev_sep_list, lun_dev_link) {

		spin_lock(&lun->lun_deve_lock);
		list_for_each_entry(se_deve, &lun->lun_deve_list, lun_link)
			core_scsi3_ua_allocate(se_deve, asc, ascq);
		spin_unlock(&lun->lun_deve_lock);
	}
	spin_unlock(&dev->se_port_lock);
}

static void
target_luns_data_has_changed(struct se_node_acl *nacl, struct se_dev_entry *new,
			     bool skip_new)
{
	struct se_dev_entry *tmp;

	rcu_read_lock();
	hlist_for_each_entry_rcu(tmp, &nacl->lun_entry_hlist, link) {
		if (skip_new && tmp == new)
			continue;
		core_scsi3_ua_allocate(tmp, 0x3F,
				       ASCQ_3FH_REPORTED_LUNS_DATA_HAS_CHANGED);
	}
	rcu_read_unlock();
}

int core_enable_device_list_for_node(
	struct se_lun *lun,
	struct se_lun_acl *lun_acl,
	u64 mapped_lun,
	bool lun_access_ro,
	struct se_node_acl *nacl,
	struct se_portal_group *tpg)
{
	struct se_dev_entry *orig, *new;

	new = kzalloc(sizeof(*new), GFP_KERNEL);
	if (!new) {
		pr_err("Unable to allocate se_dev_entry memory\n");
		return -ENOMEM;
	}

	spin_lock_init(&new->ua_lock);
	INIT_LIST_HEAD(&new->ua_list);
	INIT_LIST_HEAD(&new->lun_link);

	new->mapped_lun = mapped_lun;
	kref_init(&new->pr_kref);
	init_completion(&new->pr_comp);

	new->lun_access_ro = lun_access_ro;
	new->creation_time = get_jiffies_64();
	new->attach_count++;

	mutex_lock(&nacl->lun_entry_mutex);
	orig = target_nacl_find_deve(nacl, mapped_lun);
	if (orig && orig->se_lun) {
		struct se_lun *orig_lun = orig->se_lun;

		if (orig_lun != lun) {
			pr_err("Existing orig->se_lun doesn't match new lun"
			       " for dynamic -> explicit NodeACL conversion:"
				" %s\n", nacl->initiatorname);
			mutex_unlock(&nacl->lun_entry_mutex);
			kfree(new);
			return -EINVAL;
		}
		if (orig->se_lun_acl != NULL) {
			pr_warn_ratelimited("Detected existing explicit"
				" se_lun_acl->se_lun_group reference for %s"
				" mapped_lun: %llu, failing\n",
				 nacl->initiatorname, mapped_lun);
			mutex_unlock(&nacl->lun_entry_mutex);
			kfree(new);
			return -EINVAL;
		}

		new->se_lun = lun;
		new->se_lun_acl = lun_acl;
		hlist_del_rcu(&orig->link);
		hlist_add_head_rcu(&new->link, &nacl->lun_entry_hlist);
		mutex_unlock(&nacl->lun_entry_mutex);

		spin_lock(&lun->lun_deve_lock);
		list_del(&orig->lun_link);
		list_add_tail(&new->lun_link, &lun->lun_deve_list);
		spin_unlock(&lun->lun_deve_lock);

		kref_put(&orig->pr_kref, target_pr_kref_release);
		wait_for_completion(&orig->pr_comp);

		target_luns_data_has_changed(nacl, new, true);
		kfree_rcu(orig, rcu_head);
		return 0;
	}

	new->se_lun = lun;
	new->se_lun_acl = lun_acl;
	hlist_add_head_rcu(&new->link, &nacl->lun_entry_hlist);
	mutex_unlock(&nacl->lun_entry_mutex);

	spin_lock(&lun->lun_deve_lock);
	list_add_tail(&new->lun_link, &lun->lun_deve_list);
	spin_unlock(&lun->lun_deve_lock);

	target_luns_data_has_changed(nacl, new, true);
	return 0;
}

void core_disable_device_list_for_node(
	struct se_lun *lun,
	struct se_dev_entry *orig,
	struct se_node_acl *nacl,
	struct se_portal_group *tpg)
{
	/*
	 * rcu_dereference_raw protected by se_lun->lun_group symlink
	 * reference to se_device->dev_group.
	 */
	struct se_device *dev = rcu_dereference_raw(lun->lun_se_dev);

	lockdep_assert_held(&nacl->lun_entry_mutex);

	/*
	 * If the MappedLUN entry is being disabled, the entry in
	 * lun->lun_deve_list must be removed now before clearing the
	 * struct se_dev_entry pointers below as logic in
	 * core_alua_do_transition_tg_pt() depends on these being present.
	 *
	 * deve->se_lun_acl will be NULL for demo-mode created LUNs
	 * that have not been explicitly converted to MappedLUNs ->
	 * struct se_lun_acl, but we remove deve->lun_link from
	 * lun->lun_deve_list. This also means that active UAs and
	 * NodeACL context specific PR metadata for demo-mode
	 * MappedLUN *deve will be released below..
	 */
	spin_lock(&lun->lun_deve_lock);
	list_del(&orig->lun_link);
	spin_unlock(&lun->lun_deve_lock);
	/*
	 * Disable struct se_dev_entry LUN ACL mapping
	 */
	core_scsi3_ua_release_all(orig);

	hlist_del_rcu(&orig->link);
	clear_bit(DEF_PR_REG_ACTIVE, &orig->deve_flags);
	orig->lun_access_ro = false;
	orig->creation_time = 0;
	orig->attach_count--;
	/*
	 * Before firing off RCU callback, wait for any in process SPEC_I_PT=1
	 * or REGISTER_AND_MOVE PR operation to complete.
	 */
	kref_put(&orig->pr_kref, target_pr_kref_release);
	wait_for_completion(&orig->pr_comp);

	kfree_rcu(orig, rcu_head);

	core_scsi3_free_pr_reg_from_nacl(dev, nacl);
	target_luns_data_has_changed(nacl, NULL, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*      core_clear_lun_from_tpg():
 *
 *
 */
void core_clear_lun_from_tpg(struct se_lun *lun, struct se_portal_group *tpg)
{
	struct se_node_acl *nacl;
	struct se_dev_entry *deve;
<<<<<<< HEAD
	u32 i;

	spin_lock_irq(&tpg->acl_node_lock);
	list_for_each_entry(nacl, &tpg->acl_node_list, acl_list) {
		spin_unlock_irq(&tpg->acl_node_lock);

		spin_lock_irq(&nacl->device_list_lock);
		for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
			deve = nacl->device_list[i];
			if (lun != deve->se_lun)
				continue;
			spin_unlock_irq(&nacl->device_list_lock);

			core_update_device_list_for_node(lun, NULL,
				deve->mapped_lun, TRANSPORT_LUNFLAGS_NO_ACCESS,
				nacl, tpg, 0);

			spin_lock_irq(&nacl->device_list_lock);
		}
		spin_unlock_irq(&nacl->device_list_lock);

		spin_lock_irq(&tpg->acl_node_lock);
	}
	spin_unlock_irq(&tpg->acl_node_lock);
}

static struct se_port *core_alloc_port(struct se_device *dev)
{
	struct se_port *port, *port_tmp;

	port = kzalloc(sizeof(struct se_port), GFP_KERNEL);
	if (!port) {
		pr_err("Unable to allocate struct se_port\n");
		return ERR_PTR(-ENOMEM);
	}
	INIT_LIST_HEAD(&port->sep_alua_list);
	INIT_LIST_HEAD(&port->sep_list);
	atomic_set(&port->sep_tg_pt_secondary_offline, 0);
	spin_lock_init(&port->sep_alua_lock);
	mutex_init(&port->sep_tg_pt_md_mutex);

	spin_lock(&dev->se_port_lock);
	if (dev->dev_port_count == 0x0000ffff) {
		pr_warn("Reached dev->dev_port_count =="
				" 0x0000ffff\n");
		spin_unlock(&dev->se_port_lock);
		return ERR_PTR(-ENOSPC);
	}
again:
	/*
	 * Allocate the next RELATIVE TARGET PORT IDENTIFER for this struct se_device
	 * Here is the table from spc4r17 section 7.7.3.8.
	 *
	 *    Table 473 -- RELATIVE TARGET PORT IDENTIFIER field
	 *
	 * Code      Description
	 * 0h        Reserved
	 * 1h        Relative port 1, historically known as port A
	 * 2h        Relative port 2, historically known as port B
	 * 3h to FFFFh    Relative port 3 through 65 535
	 */
	port->sep_rtpi = dev->dev_rpti_counter++;
	if (!port->sep_rtpi)
		goto again;

	list_for_each_entry(port_tmp, &dev->dev_sep_list, sep_list) {
		/*
		 * Make sure RELATIVE TARGET PORT IDENTIFER is unique
		 * for 16-bit wrap..
		 */
		if (port->sep_rtpi == port_tmp->sep_rtpi)
			goto again;
	}
	spin_unlock(&dev->se_port_lock);

	return port;
}

static void core_export_port(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_port *port,
	struct se_lun *lun)
{
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem = NULL;

	spin_lock(&dev->se_port_lock);
	spin_lock(&lun->lun_sep_lock);
	port->sep_tpg = tpg;
	port->sep_lun = lun;
	lun->lun_sep = port;
	spin_unlock(&lun->lun_sep_lock);

	list_add_tail(&port->sep_list, &dev->dev_sep_list);
	spin_unlock(&dev->se_port_lock);

	if (su_dev->t10_alua.alua_type == SPC3_ALUA_EMULATED) {
		tg_pt_gp_mem = core_alua_allocate_tg_pt_gp_mem(port);
		if (IS_ERR(tg_pt_gp_mem) || !tg_pt_gp_mem) {
			pr_err("Unable to allocate t10_alua_tg_pt"
					"_gp_member_t\n");
			return;
		}
		spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
			su_dev->t10_alua.default_tg_pt_gp);
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		pr_debug("%s/%s: Adding to default ALUA Target Port"
			" Group: alua/default_tg_pt_gp\n",
			dev->transport->name, tpg->se_tpg_tfo->get_fabric_name());
	}

	dev->dev_port_count++;
	port->sep_index = port->sep_rtpi; /* RELATIVE TARGET PORT IDENTIFER */
}

/*
 *	Called with struct se_device->se_port_lock spinlock held.
 */
static void core_release_port(struct se_device *dev, struct se_port *port)
	__releases(&dev->se_port_lock) __acquires(&dev->se_port_lock)
{
	/*
	 * Wait for any port reference for PR ALL_TG_PT=1 operation
	 * to complete in __core_scsi3_alloc_registration()
	 */
	spin_unlock(&dev->se_port_lock);
	if (atomic_read(&port->sep_tg_pt_ref_cnt))
		cpu_relax();
	spin_lock(&dev->se_port_lock);

	core_alua_free_tg_pt_gp_mem(port);

	list_del(&port->sep_list);
	dev->dev_port_count--;
	kfree(port);
}

int core_dev_export(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_lun *lun)
{
	struct se_port *port;

	port = core_alloc_port(dev);
	if (IS_ERR(port))
		return PTR_ERR(port);

	lun->lun_se_dev = dev;
	se_dev_start(dev);

	atomic_inc(&dev->dev_export_obj.obj_access_count);
	core_export_port(dev, tpg, port, lun);
	return 0;
}

void core_dev_unexport(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_lun *lun)
{
	struct se_port *port = lun->lun_sep;

	spin_lock(&lun->lun_sep_lock);
	if (lun->lun_se_dev == NULL) {
		spin_unlock(&lun->lun_sep_lock);
		return;
	}
	spin_unlock(&lun->lun_sep_lock);

	spin_lock(&dev->se_port_lock);
	atomic_dec(&dev->dev_export_obj.obj_access_count);
	core_release_port(dev, port);
	spin_unlock(&dev->se_port_lock);

	se_dev_stop(dev);
	lun->lun_se_dev = NULL;
}

int target_report_luns(struct se_task *se_task)
{
	struct se_cmd *se_cmd = se_task->task_se_cmd;
	struct se_dev_entry *deve;
	struct se_session *se_sess = se_cmd->se_sess;
	unsigned char *buf;
	u32 lun_count = 0, offset = 8, i;

	buf = transport_kmap_data_sg(se_cmd);
	if (!buf)
		return -ENOMEM;

	/*
	 * If no struct se_session pointer is present, this struct se_cmd is
	 * coming via a target_core_mod PASSTHROUGH op, and not through
	 * a $FABRIC_MOD.  In that case, report LUN=0 only.
	 */
	if (!se_sess) {
		int_to_scsilun(0, (struct scsi_lun *)&buf[offset]);
		lun_count = 1;
		goto done;
	}

	spin_lock_irq(&se_sess->se_node_acl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
		deve = se_sess->se_node_acl->device_list[i];
		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;
		/*
		 * We determine the correct LUN LIST LENGTH even once we
		 * have reached the initial allocation length.
		 * See SPC2-R20 7.19.
		 */
		lun_count++;
		if ((offset + 8) > se_cmd->data_length)
			continue;

		int_to_scsilun(deve->mapped_lun, (struct scsi_lun *)&buf[offset]);
		offset += 8;
	}
	spin_unlock_irq(&se_sess->se_node_acl->device_list_lock);

	/*
	 * See SPC3 r07, page 159.
	 */
done:
	lun_count *= 8;
	buf[0] = ((lun_count >> 24) & 0xff);
	buf[1] = ((lun_count >> 16) & 0xff);
	buf[2] = ((lun_count >> 8) & 0xff);
	buf[3] = (lun_count & 0xff);
	transport_kunmap_data_sg(se_cmd);

	se_task->task_scsi_status = GOOD;
	transport_complete_task(se_task, 1);
	return 0;
}

/*	se_release_device_for_hba():
 *
 *
 */
void se_release_device_for_hba(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	if ((dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_SHUTDOWN) ||
	    (dev->dev_status & TRANSPORT_DEVICE_OFFLINE_ACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_OFFLINE_DEACTIVATED))
		se_dev_stop(dev);

	if (dev->dev_ptr) {
		kthread_stop(dev->process_thread);
		if (dev->transport->free_device)
			dev->transport->free_device(dev->dev_ptr);
	}

	spin_lock(&hba->device_lock);
	list_del(&dev->dev_list);
	hba->dev_count--;
	spin_unlock(&hba->device_lock);

	core_scsi3_free_all_registrations(dev);
	se_release_vpd_for_dev(dev);

	kfree(dev);
}

void se_release_vpd_for_dev(struct se_device *dev)
{
	struct t10_vpd *vpd, *vpd_tmp;

	spin_lock(&dev->se_sub_dev->t10_wwn.t10_vpd_lock);
	list_for_each_entry_safe(vpd, vpd_tmp,
			&dev->se_sub_dev->t10_wwn.t10_vpd_list, vpd_list) {
		list_del(&vpd->vpd_list);
		kfree(vpd);
	}
	spin_unlock(&dev->se_sub_dev->t10_wwn.t10_vpd_lock);
}

/*	se_free_virtual_device():
 *
 *	Used for IBLOCK, RAMDISK, and FILEIO Transport Drivers.
 */
int se_free_virtual_device(struct se_device *dev, struct se_hba *hba)
{
	if (!list_empty(&dev->dev_sep_list))
		dump_stack();

	core_alua_free_lu_gp_mem(dev);
	se_release_device_for_hba(dev);

	return 0;
}

static void se_dev_start(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	spin_lock(&hba->device_lock);
	atomic_inc(&dev->dev_obj.obj_access_count);
	if (atomic_read(&dev->dev_obj.obj_access_count) == 1) {
		if (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_DEACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_ACTIVATED;
		} else if (dev->dev_status &
			   TRANSPORT_DEVICE_OFFLINE_DEACTIVATED) {
			dev->dev_status &=
				~TRANSPORT_DEVICE_OFFLINE_DEACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_OFFLINE_ACTIVATED;
		}
	}
	spin_unlock(&hba->device_lock);
}

static void se_dev_stop(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	spin_lock(&hba->device_lock);
	atomic_dec(&dev->dev_obj.obj_access_count);
	if (atomic_read(&dev->dev_obj.obj_access_count) == 0) {
		if (dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_ACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_DEACTIVATED;
		} else if (dev->dev_status &
			   TRANSPORT_DEVICE_OFFLINE_ACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_OFFLINE_ACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_OFFLINE_DEACTIVATED;
		}
	}
	spin_unlock(&hba->device_lock);
}

int se_dev_check_online(struct se_device *dev)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&dev->dev_status_lock, flags);
	ret = ((dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) ||
	       (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED)) ? 0 : 1;
	spin_unlock_irqrestore(&dev->dev_status_lock, flags);

	return ret;
}

int se_dev_check_shutdown(struct se_device *dev)
{
	int ret;

	spin_lock_irq(&dev->dev_status_lock);
	ret = (dev->dev_status & TRANSPORT_DEVICE_SHUTDOWN);
	spin_unlock_irq(&dev->dev_status_lock);

	return ret;
}

u32 se_dev_align_max_sectors(u32 max_sectors, u32 block_size)
=======

	mutex_lock(&tpg->acl_node_mutex);
	list_for_each_entry(nacl, &tpg->acl_node_list, acl_list) {

		mutex_lock(&nacl->lun_entry_mutex);
		hlist_for_each_entry_rcu(deve, &nacl->lun_entry_hlist, link) {
			if (lun != deve->se_lun)
				continue;

			core_disable_device_list_for_node(lun, deve, nacl, tpg);
		}
		mutex_unlock(&nacl->lun_entry_mutex);
	}
	mutex_unlock(&tpg->acl_node_mutex);
}

static void se_release_vpd_for_dev(struct se_device *dev)
{
	struct t10_vpd *vpd, *vpd_tmp;

	spin_lock(&dev->t10_wwn.t10_vpd_lock);
	list_for_each_entry_safe(vpd, vpd_tmp,
			&dev->t10_wwn.t10_vpd_list, vpd_list) {
		list_del(&vpd->vpd_list);
		kfree(vpd);
	}
	spin_unlock(&dev->t10_wwn.t10_vpd_lock);
}

static u32 se_dev_align_max_sectors(u32 max_sectors, u32 block_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 aligned_max_sectors;
	u32 alignment;
	/*
	 * Limit max_sectors to a PAGE_SIZE aligned value for modern
	 * transport_allocate_data_tasks() operation.
	 */
	alignment = max(1ul, PAGE_SIZE / block_size);
	aligned_max_sectors = rounddown(max_sectors, alignment);

	if (max_sectors != aligned_max_sectors)
		pr_info("Rounding down aligned max_sectors from %u to %u\n",
			max_sectors, aligned_max_sectors);

	return aligned_max_sectors;
}

<<<<<<< HEAD
void se_dev_set_default_attribs(
	struct se_device *dev,
	struct se_dev_limits *dev_limits)
{
	struct queue_limits *limits = &dev_limits->limits;

	dev->se_sub_dev->se_dev_attrib.emulate_dpo = DA_EMULATE_DPO;
	dev->se_sub_dev->se_dev_attrib.emulate_fua_write = DA_EMULATE_FUA_WRITE;
	dev->se_sub_dev->se_dev_attrib.emulate_fua_read = DA_EMULATE_FUA_READ;
	dev->se_sub_dev->se_dev_attrib.emulate_write_cache = DA_EMULATE_WRITE_CACHE;
	dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl = DA_EMULATE_UA_INTLLCK_CTRL;
	dev->se_sub_dev->se_dev_attrib.emulate_tas = DA_EMULATE_TAS;
	dev->se_sub_dev->se_dev_attrib.emulate_tpu = DA_EMULATE_TPU;
	dev->se_sub_dev->se_dev_attrib.emulate_tpws = DA_EMULATE_TPWS;
	dev->se_sub_dev->se_dev_attrib.emulate_reservations = DA_EMULATE_RESERVATIONS;
	dev->se_sub_dev->se_dev_attrib.emulate_alua = DA_EMULATE_ALUA;
	dev->se_sub_dev->se_dev_attrib.enforce_pr_isids = DA_ENFORCE_PR_ISIDS;
	dev->se_sub_dev->se_dev_attrib.is_nonrot = DA_IS_NONROT;
	dev->se_sub_dev->se_dev_attrib.emulate_rest_reord = DA_EMULATE_REST_REORD;
	/*
	 * The TPU=1 and TPWS=1 settings will be set in TCM/IBLOCK
	 * iblock_create_virtdevice() from struct queue_limits values
	 * if blk_queue_discard()==1
	 */
	dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count = DA_MAX_UNMAP_LBA_COUNT;
	dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count =
		DA_MAX_UNMAP_BLOCK_DESC_COUNT;
	dev->se_sub_dev->se_dev_attrib.unmap_granularity = DA_UNMAP_GRANULARITY_DEFAULT;
	dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment =
				DA_UNMAP_GRANULARITY_ALIGNMENT_DEFAULT;
	/*
	 * block_size is based on subsystem plugin dependent requirements.
	 */
	dev->se_sub_dev->se_dev_attrib.hw_block_size = limits->logical_block_size;
	dev->se_sub_dev->se_dev_attrib.block_size = limits->logical_block_size;
	/*
	 * max_sectors is based on subsystem plugin dependent requirements.
	 */
	dev->se_sub_dev->se_dev_attrib.hw_max_sectors = limits->max_hw_sectors;
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	limits->max_sectors = se_dev_align_max_sectors(limits->max_sectors,
						limits->logical_block_size);
	dev->se_sub_dev->se_dev_attrib.max_sectors = limits->max_sectors;
	/*
	 * Set fabric_max_sectors, which is reported in block limits
	 * VPD page (B0h).
	 */
	dev->se_sub_dev->se_dev_attrib.fabric_max_sectors = DA_FABRIC_MAX_SECTORS;
	/*
	 * Set optimal_sectors from fabric_max_sectors, which can be
	 * lowered via configfs.
	 */
	dev->se_sub_dev->se_dev_attrib.optimal_sectors = DA_FABRIC_MAX_SECTORS;
	/*
	 * queue_depth is based on subsystem plugin dependent requirements.
	 */
	dev->se_sub_dev->se_dev_attrib.hw_queue_depth = dev_limits->hw_queue_depth;
	dev->se_sub_dev->se_dev_attrib.queue_depth = dev_limits->queue_depth;
}

int se_dev_set_max_unmap_lba_count(
	struct se_device *dev,
	u32 max_unmap_lba_count)
{
	dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count = max_unmap_lba_count;
	pr_debug("dev[%p]: Set max_unmap_lba_count: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count);
	return 0;
}

int se_dev_set_max_unmap_block_desc_count(
	struct se_device *dev,
	u32 max_unmap_block_desc_count)
{
	dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count =
		max_unmap_block_desc_count;
	pr_debug("dev[%p]: Set max_unmap_block_desc_count: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count);
	return 0;
}

int se_dev_set_unmap_granularity(
	struct se_device *dev,
	u32 unmap_granularity)
{
	dev->se_sub_dev->se_dev_attrib.unmap_granularity = unmap_granularity;
	pr_debug("dev[%p]: Set unmap_granularity: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.unmap_granularity);
	return 0;
}

int se_dev_set_unmap_granularity_alignment(
	struct se_device *dev,
	u32 unmap_granularity_alignment)
{
	dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment = unmap_granularity_alignment;
	pr_debug("dev[%p]: Set unmap_granularity_alignment: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment);
	return 0;
}

int se_dev_set_emulate_dpo(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag) {
		pr_err("dpo_emulated not supported\n");
		return -EINVAL;
	}

	return 0;
}

int se_dev_set_emulate_fua_write(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag && dev->transport->fua_write_emulated == 0) {
		pr_err("fua_write_emulated not supported\n");
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_fua_write = flag;
	pr_debug("dev[%p]: SE Device Forced Unit Access WRITEs: %d\n",
			dev, dev->se_sub_dev->se_dev_attrib.emulate_fua_write);
	return 0;
}

int se_dev_set_emulate_fua_read(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag) {
		pr_err("ua read emulated not supported\n");
		return -EINVAL;
	}

	return 0;
}

int se_dev_set_emulate_write_cache(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	if (flag && dev->transport->write_cache_emulated == 0) {
		pr_err("write_cache_emulated not supported\n");
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_write_cache = flag;
	pr_debug("dev[%p]: SE Device WRITE_CACHE_EMULATION flag: %d\n",
			dev, dev->se_sub_dev->se_dev_attrib.emulate_write_cache);
	return 0;
}

int se_dev_set_emulate_ua_intlck_ctrl(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1) && (flag != 2)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" UA_INTRLCK_CTRL while dev_export_obj: %d count"
			" exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl = flag;
	pr_debug("dev[%p]: SE Device UA_INTRLCK_CTRL flag: %d\n",
		dev, dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl);

	return 0;
}

int se_dev_set_emulate_tas(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device TAS while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_tas = flag;
	pr_debug("dev[%p]: SE Device TASK_ABORTED status bit: %s\n",
		dev, (dev->se_sub_dev->se_dev_attrib.emulate_tas) ? "Enabled" : "Disabled");

	return 0;
}

int se_dev_set_emulate_tpu(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	/*
	 * We expect this value to be non-zero when generic Block Layer
	 * Discard supported is detected iblock_create_virtdevice().
	 */
	if (flag && !dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count) {
		pr_err("Generic Block Discard not supported\n");
		return -ENOSYS;
	}

	dev->se_sub_dev->se_dev_attrib.emulate_tpu = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning UNMAP bit: %d\n",
				dev, flag);
	return 0;
}

int se_dev_set_emulate_tpws(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	/*
	 * We expect this value to be non-zero when generic Block Layer
	 * Discard supported is detected iblock_create_virtdevice().
	 */
	if (flag && !dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count) {
		pr_err("Generic Block Discard not supported\n");
		return -ENOSYS;
	}

	dev->se_sub_dev->se_dev_attrib.emulate_tpws = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning WRITE_SAME: %d\n",
				dev, flag);
	return 0;
}

int se_dev_set_enforce_pr_isids(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.enforce_pr_isids = flag;
	pr_debug("dev[%p]: SE Device enforce_pr_isids bit: %s\n", dev,
		(dev->se_sub_dev->se_dev_attrib.enforce_pr_isids) ? "Enabled" : "Disabled");
	return 0;
}

int se_dev_set_is_nonrot(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.is_nonrot = flag;
	pr_debug("dev[%p]: SE Device is_nonrot bit: %d\n",
	       dev, flag);
	return 0;
}

int se_dev_set_emulate_rest_reord(struct se_device *dev, int flag)
{
	if (flag != 0) {
		printk(KERN_ERR "dev[%p]: SE Device emulatation of restricted"
			" reordering not implemented\n", dev);
		return -ENOSYS;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_rest_reord = flag;
	pr_debug("dev[%p]: SE Device emulate_rest_reord: %d\n", dev, flag);
	return 0;
}

/*
 * Note, this can only be called on unexported SE Device Object.
 */
int se_dev_set_queue_depth(struct se_device *dev, u32 queue_depth)
{
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device TCQ while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	if (!queue_depth) {
		pr_err("dev[%p]: Illegal ZERO value for queue"
			"_depth\n", dev);
		return -EINVAL;
	}

	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (queue_depth > dev->se_sub_dev->se_dev_attrib.hw_queue_depth) {
			pr_err("dev[%p]: Passed queue_depth: %u"
				" exceeds TCM/SE_Device TCQ: %u\n",
				dev, queue_depth,
				dev->se_sub_dev->se_dev_attrib.hw_queue_depth);
			return -EINVAL;
		}
	} else {
		if (queue_depth > dev->se_sub_dev->se_dev_attrib.queue_depth) {
			if (queue_depth > dev->se_sub_dev->se_dev_attrib.hw_queue_depth) {
				pr_err("dev[%p]: Passed queue_depth:"
					" %u exceeds TCM/SE_Device MAX"
					" TCQ: %u\n", dev, queue_depth,
					dev->se_sub_dev->se_dev_attrib.hw_queue_depth);
				return -EINVAL;
			}
		}
	}

	dev->se_sub_dev->se_dev_attrib.queue_depth = dev->queue_depth = queue_depth;
	pr_debug("dev[%p]: SE Device TCQ Depth changed to: %u\n",
			dev, queue_depth);
	return 0;
}

int se_dev_set_max_sectors(struct se_device *dev, u32 max_sectors)
{
	int force = 0; /* Force setting for VDEVS */

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" max_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	if (!max_sectors) {
		pr_err("dev[%p]: Illegal ZERO value for"
			" max_sectors\n", dev);
		return -EINVAL;
	}
	if (max_sectors < DA_STATUS_MAX_SECTORS_MIN) {
		pr_err("dev[%p]: Passed max_sectors: %u less than"
			" DA_STATUS_MAX_SECTORS_MIN: %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MIN);
		return -EINVAL;
	}
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (max_sectors > dev->se_sub_dev->se_dev_attrib.hw_max_sectors) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, max_sectors,
				dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
			 return -EINVAL;
		}
	} else {
		if (!force && (max_sectors >
				 dev->se_sub_dev->se_dev_attrib.hw_max_sectors)) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors"
				": %u, use force=1 to override.\n", dev,
				max_sectors, dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
			return -EINVAL;
		}
		if (max_sectors > DA_STATUS_MAX_SECTORS_MAX) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than DA_STATUS_MAX_SECTORS_MAX:"
				" %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MAX);
			return -EINVAL;
		}
	}
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	max_sectors = se_dev_align_max_sectors(max_sectors,
				dev->se_sub_dev->se_dev_attrib.block_size);

	dev->se_sub_dev->se_dev_attrib.max_sectors = max_sectors;
	pr_debug("dev[%p]: SE Device max_sectors changed to %u\n",
			dev, max_sectors);
	return 0;
}

int se_dev_set_fabric_max_sectors(struct se_device *dev, u32 fabric_max_sectors)
{
	int block_size = dev->se_sub_dev->se_dev_attrib.block_size;

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" fabric_max_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	if (!fabric_max_sectors) {
		pr_err("dev[%p]: Illegal ZERO value for"
			" fabric_max_sectors\n", dev);
		return -EINVAL;
	}
	if (fabric_max_sectors < DA_STATUS_MAX_SECTORS_MIN) {
		pr_err("dev[%p]: Passed fabric_max_sectors: %u less than"
			" DA_STATUS_MAX_SECTORS_MIN: %u\n", dev, fabric_max_sectors,
				DA_STATUS_MAX_SECTORS_MIN);
		return -EINVAL;
	}
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (fabric_max_sectors > dev->se_sub_dev->se_dev_attrib.hw_max_sectors) {
			pr_err("dev[%p]: Passed fabric_max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, fabric_max_sectors,
				dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
			 return -EINVAL;
		}
	} else {
		if (fabric_max_sectors > DA_STATUS_MAX_SECTORS_MAX) {
			pr_err("dev[%p]: Passed fabric_max_sectors: %u"
				" greater than DA_STATUS_MAX_SECTORS_MAX:"
				" %u\n", dev, fabric_max_sectors,
				DA_STATUS_MAX_SECTORS_MAX);
			return -EINVAL;
		}
	}
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	if (!block_size) {
		block_size = 512;
		pr_warn("Defaulting to 512 for zero block_size\n");
	}
	fabric_max_sectors = se_dev_align_max_sectors(fabric_max_sectors,
						      block_size);

	dev->se_sub_dev->se_dev_attrib.fabric_max_sectors = fabric_max_sectors;
	pr_debug("dev[%p]: SE Device max_sectors changed to %u\n",
			dev, fabric_max_sectors);
	return 0;
}

int se_dev_set_optimal_sectors(struct se_device *dev, u32 optimal_sectors)
{
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" optimal_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("dev[%p]: Passed optimal_sectors cannot be"
				" changed for TCM/pSCSI\n", dev);
		return -EINVAL;
	}
	if (optimal_sectors > dev->se_sub_dev->se_dev_attrib.fabric_max_sectors) {
		pr_err("dev[%p]: Passed optimal_sectors %u cannot be"
			" greater than fabric_max_sectors: %u\n", dev,
			optimal_sectors, dev->se_sub_dev->se_dev_attrib.fabric_max_sectors);
		return -EINVAL;
	}

	dev->se_sub_dev->se_dev_attrib.optimal_sectors = optimal_sectors;
	pr_debug("dev[%p]: SE Device optimal_sectors changed to %u\n",
			dev, optimal_sectors);
	return 0;
}

int se_dev_set_block_size(struct se_device *dev, u32 block_size)
{
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device block_size"
			" while dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}

	if ((block_size != 512) &&
	    (block_size != 1024) &&
	    (block_size != 2048) &&
	    (block_size != 4096)) {
		pr_err("dev[%p]: Illegal value for block_device: %u"
			" for SE device, must be 512, 1024, 2048 or 4096\n",
			dev, block_size);
		return -EINVAL;
	}

	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("dev[%p]: Not allowed to change block_size for"
			" Physical Device, use for Linux/SCSI to change"
			" block_size for underlying hardware\n", dev);
		return -EINVAL;
	}

	dev->se_sub_dev->se_dev_attrib.block_size = block_size;
	pr_debug("dev[%p]: SE Device block_size changed to %u\n",
			dev, block_size);
	return 0;
}

struct se_lun *core_dev_add_lun(
	struct se_portal_group *tpg,
	struct se_hba *hba,
	struct se_device *dev,
	u32 lun)
{
	struct se_lun *lun_p;
	u32 lun_access = 0;
	int rc;

	if (atomic_read(&dev->dev_access_obj.obj_access_count) != 0) {
		pr_err("Unable to export struct se_device while dev_access_obj: %d\n",
			atomic_read(&dev->dev_access_obj.obj_access_count));
		return ERR_PTR(-EACCES);
	}

	lun_p = core_tpg_pre_addlun(tpg, lun);
	if (IS_ERR(lun_p))
		return lun_p;

	if (dev->dev_flags & DF_READ_ONLY)
		lun_access = TRANSPORT_LUNFLAGS_READ_ONLY;
	else
		lun_access = TRANSPORT_LUNFLAGS_READ_WRITE;

	rc = core_tpg_post_addlun(tpg, lun_p, lun_access, dev);
	if (rc < 0)
		return ERR_PTR(rc);

	pr_debug("%s_TPG[%u]_LUN[%u] - Activated %s Logical Unit from"
		" CORE HBA: %u\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun_p->unpacked_lun,
		tpg->se_tpg_tfo->get_fabric_name(), hba->hba_id);
=======
int core_dev_add_lun(
	struct se_portal_group *tpg,
	struct se_device *dev,
	struct se_lun *lun)
{
	int rc;

	rc = core_tpg_add_lun(tpg, lun, false, dev);
	if (rc < 0)
		return rc;

	pr_debug("%s_TPG[%u]_LUN[%llu] - Activated %s Logical Unit from"
		" CORE HBA: %u\n", tpg->se_tpg_tfo->fabric_name,
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun,
		tpg->se_tpg_tfo->fabric_name, dev->se_hba->hba_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Update LUN maps for dynamically added initiators when
	 * generate_node_acl is enabled.
	 */
	if (tpg->se_tpg_tfo->tpg_check_demo_mode(tpg)) {
		struct se_node_acl *acl;
<<<<<<< HEAD
		spin_lock_irq(&tpg->acl_node_lock);
=======

		mutex_lock(&tpg->acl_node_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		list_for_each_entry(acl, &tpg->acl_node_list, acl_list) {
			if (acl->dynamic_node_acl &&
			    (!tpg->se_tpg_tfo->tpg_check_demo_mode_login_only ||
			     !tpg->se_tpg_tfo->tpg_check_demo_mode_login_only(tpg))) {
<<<<<<< HEAD
				spin_unlock_irq(&tpg->acl_node_lock);
				core_tpg_add_node_to_devs(acl, tpg);
				spin_lock_irq(&tpg->acl_node_lock);
			}
		}
		spin_unlock_irq(&tpg->acl_node_lock);
	}

	return lun_p;
=======
				core_tpg_add_node_to_devs(acl, tpg, lun);
			}
		}
		mutex_unlock(&tpg->acl_node_mutex);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*      core_dev_del_lun():
 *
 *
 */
<<<<<<< HEAD
int core_dev_del_lun(
	struct se_portal_group *tpg,
	u32 unpacked_lun)
{
	struct se_lun *lun;

	lun = core_tpg_pre_dellun(tpg, unpacked_lun);
	if (IS_ERR(lun))
		return PTR_ERR(lun);

	core_tpg_post_dellun(tpg, lun);

	pr_debug("%s_TPG[%u]_LUN[%u] - Deactivated %s Logical Unit from"
		" device object\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), unpacked_lun,
		tpg->se_tpg_tfo->get_fabric_name());

	return 0;
}

struct se_lun *core_get_lun_from_tpg(struct se_portal_group *tpg, u32 unpacked_lun)
{
	struct se_lun *lun;

	spin_lock(&tpg->tpg_lun_lock);
	if (unpacked_lun > (TRANSPORT_MAX_LUNS_PER_TPG-1)) {
		pr_err("%s LUN: %u exceeds TRANSPORT_MAX_LUNS"
			"_PER_TPG-1: %u for Target Portal Group: %hu\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_FREE) {
		pr_err("%s Logical Unit Number: %u is not free on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	spin_unlock(&tpg->tpg_lun_lock);

	return lun;
}

/*      core_dev_get_lun():
 *
 *
 */
static struct se_lun *core_dev_get_lun(struct se_portal_group *tpg, u32 unpacked_lun)
{
	struct se_lun *lun;

	spin_lock(&tpg->tpg_lun_lock);
	if (unpacked_lun > (TRANSPORT_MAX_LUNS_PER_TPG-1)) {
		pr_err("%s LUN: %u exceeds TRANSPORT_MAX_LUNS_PER"
			"_TPG-1: %u for Target Portal Group: %hu\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_ACTIVE) {
		pr_err("%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	spin_unlock(&tpg->tpg_lun_lock);

	return lun;
=======
void core_dev_del_lun(
	struct se_portal_group *tpg,
	struct se_lun *lun)
{
	pr_debug("%s_TPG[%u]_LUN[%llu] - Deactivating %s Logical Unit from"
		" device object\n", tpg->se_tpg_tfo->fabric_name,
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun,
		tpg->se_tpg_tfo->fabric_name);

	core_tpg_remove_lun(tpg, lun);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct se_lun_acl *core_dev_init_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_node_acl *nacl,
<<<<<<< HEAD
	u32 mapped_lun,
=======
	u64 mapped_lun,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int *ret)
{
	struct se_lun_acl *lacl;

	if (strlen(nacl->initiatorname) >= TRANSPORT_IQN_LEN) {
		pr_err("%s InitiatorName exceeds maximum size.\n",
<<<<<<< HEAD
			tpg->se_tpg_tfo->get_fabric_name());
=======
			tpg->se_tpg_tfo->fabric_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*ret = -EOVERFLOW;
		return NULL;
	}
	lacl = kzalloc(sizeof(struct se_lun_acl), GFP_KERNEL);
	if (!lacl) {
		pr_err("Unable to allocate memory for struct se_lun_acl.\n");
		*ret = -ENOMEM;
		return NULL;
	}

<<<<<<< HEAD
	INIT_LIST_HEAD(&lacl->lacl_list);
	lacl->mapped_lun = mapped_lun;
	lacl->se_lun_nacl = nacl;
	snprintf(lacl->initiatorname, TRANSPORT_IQN_LEN, "%s",
		 nacl->initiatorname);
=======
	lacl->mapped_lun = mapped_lun;
	lacl->se_lun_nacl = nacl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return lacl;
}

int core_dev_add_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun_acl *lacl,
<<<<<<< HEAD
	u32 unpacked_lun,
	u32 lun_access)
{
	struct se_lun *lun;
	struct se_node_acl *nacl;

	lun = core_dev_get_lun(tpg, unpacked_lun);
	if (!lun) {
		pr_err("%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		return -EINVAL;
	}

	nacl = lacl->se_lun_nacl;
	if (!nacl)
		return -EINVAL;

	if ((lun->lun_access & TRANSPORT_LUNFLAGS_READ_ONLY) &&
	    (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE))
		lun_access = TRANSPORT_LUNFLAGS_READ_ONLY;

	lacl->se_lun = lun;

	if (core_update_device_list_for_node(lun, lacl, lacl->mapped_lun,
			lun_access, nacl, tpg, 1) < 0)
		return -EINVAL;

	spin_lock(&lun->lun_acl_lock);
	list_add_tail(&lacl->lacl_list, &lun->lun_acl_list);
	atomic_inc(&lun->lun_acl_count);
	smp_mb__after_atomic_inc();
	spin_unlock(&lun->lun_acl_lock);

	pr_debug("%s_TPG[%hu]_LUN[%u->%u] - Added %s ACL for "
		" InitiatorNode: %s\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), unpacked_lun, lacl->mapped_lun,
		(lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) ? "RW" : "RO",
		lacl->initiatorname);
=======
	struct se_lun *lun,
	bool lun_access_ro)
{
	struct se_node_acl *nacl = lacl->se_lun_nacl;
	/*
	 * rcu_dereference_raw protected by se_lun->lun_group symlink
	 * reference to se_device->dev_group.
	 */
	struct se_device *dev = rcu_dereference_raw(lun->lun_se_dev);

	if (!nacl)
		return -EINVAL;

	if (lun->lun_access_ro)
		lun_access_ro = true;

	lacl->se_lun = lun;

	if (core_enable_device_list_for_node(lun, lacl, lacl->mapped_lun,
			lun_access_ro, nacl, tpg) < 0)
		return -EINVAL;

	pr_debug("%s_TPG[%hu]_LUN[%llu->%llu] - Added %s ACL for "
		" InitiatorNode: %s\n", tpg->se_tpg_tfo->fabric_name,
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun, lacl->mapped_lun,
		lun_access_ro ? "RO" : "RW",
		nacl->initiatorname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check to see if there are any existing persistent reservation APTPL
	 * pre-registrations that need to be enabled for this LUN ACL..
	 */
<<<<<<< HEAD
	core_scsi3_check_aptpl_registration(lun->lun_se_dev, tpg, lun, lacl);
	return 0;
}

/*      core_dev_del_initiator_node_lun_acl():
 *
 *
 */
int core_dev_del_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun *lun,
	struct se_lun_acl *lacl)
{
	struct se_node_acl *nacl;
=======
	core_scsi3_check_aptpl_registration(dev, tpg, lun, nacl,
					    lacl->mapped_lun);
	return 0;
}

int core_dev_del_initiator_node_lun_acl(
	struct se_lun *lun,
	struct se_lun_acl *lacl)
{
	struct se_portal_group *tpg = lun->lun_tpg;
	struct se_node_acl *nacl;
	struct se_dev_entry *deve;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nacl = lacl->se_lun_nacl;
	if (!nacl)
		return -EINVAL;

<<<<<<< HEAD
	spin_lock(&lun->lun_acl_lock);
	list_del(&lacl->lacl_list);
	atomic_dec(&lun->lun_acl_count);
	smp_mb__after_atomic_dec();
	spin_unlock(&lun->lun_acl_lock);

	core_update_device_list_for_node(lun, NULL, lacl->mapped_lun,
		TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg, 0);

	lacl->se_lun = NULL;

	pr_debug("%s_TPG[%hu]_LUN[%u] - Removed ACL for"
		" InitiatorNode: %s Mapped LUN: %u\n",
		tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun,
		lacl->initiatorname, lacl->mapped_lun);
=======
	mutex_lock(&nacl->lun_entry_mutex);
	deve = target_nacl_find_deve(nacl, lacl->mapped_lun);
	if (deve)
		core_disable_device_list_for_node(lun, deve, nacl, tpg);
	mutex_unlock(&nacl->lun_entry_mutex);

	pr_debug("%s_TPG[%hu]_LUN[%llu] - Removed ACL for"
		" InitiatorNode: %s Mapped LUN: %llu\n",
		tpg->se_tpg_tfo->fabric_name,
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun,
		nacl->initiatorname, lacl->mapped_lun);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

void core_dev_free_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun_acl *lacl)
{
	pr_debug("%s_TPG[%hu] - Freeing ACL for %s InitiatorNode: %s"
<<<<<<< HEAD
		" Mapped LUN: %u\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg),
		tpg->se_tpg_tfo->get_fabric_name(),
		lacl->initiatorname, lacl->mapped_lun);
=======
		" Mapped LUN: %llu\n", tpg->se_tpg_tfo->fabric_name,
		tpg->se_tpg_tfo->tpg_get_tag(tpg),
		tpg->se_tpg_tfo->fabric_name,
		lacl->se_lun_nacl->initiatorname, lacl->mapped_lun);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(lacl);
}

<<<<<<< HEAD
=======
static void scsi_dump_inquiry(struct se_device *dev)
{
	struct t10_wwn *wwn = &dev->t10_wwn;
	int device_type = dev->transport->get_device_type(dev);

	/*
	 * Print Linux/SCSI style INQUIRY formatting to the kernel ring buffer
	 */
	pr_debug("  Vendor: %-" __stringify(INQUIRY_VENDOR_LEN) "s\n",
		wwn->vendor);
	pr_debug("  Model: %-" __stringify(INQUIRY_MODEL_LEN) "s\n",
		wwn->model);
	pr_debug("  Revision: %-" __stringify(INQUIRY_REVISION_LEN) "s\n",
		wwn->revision);
	pr_debug("  Type:   %s ", scsi_device_type(device_type));
}

struct se_device *target_alloc_device(struct se_hba *hba, const char *name)
{
	struct se_device *dev;
	struct se_lun *xcopy_lun;
	int i;

	dev = hba->backend->ops->alloc_device(hba, name);
	if (!dev)
		return NULL;

	dev->queues = kcalloc(nr_cpu_ids, sizeof(*dev->queues), GFP_KERNEL);
	if (!dev->queues) {
		dev->transport->free_device(dev);
		return NULL;
	}

	dev->queue_cnt = nr_cpu_ids;
	for (i = 0; i < dev->queue_cnt; i++) {
		struct se_device_queue *q;

		q = &dev->queues[i];
		INIT_LIST_HEAD(&q->state_list);
		spin_lock_init(&q->lock);

		init_llist_head(&q->sq.cmd_list);
		INIT_WORK(&q->sq.work, target_queued_submit_work);
	}

	dev->se_hba = hba;
	dev->transport = hba->backend->ops;
	dev->transport_flags = dev->transport->transport_flags_default;
	dev->prot_length = sizeof(struct t10_pi_tuple);
	dev->hba_index = hba->hba_index;

	INIT_LIST_HEAD(&dev->dev_sep_list);
	INIT_LIST_HEAD(&dev->dev_tmr_list);
	INIT_LIST_HEAD(&dev->delayed_cmd_list);
	INIT_LIST_HEAD(&dev->qf_cmd_list);
	spin_lock_init(&dev->delayed_cmd_lock);
	spin_lock_init(&dev->dev_reservation_lock);
	spin_lock_init(&dev->se_port_lock);
	spin_lock_init(&dev->se_tmr_lock);
	spin_lock_init(&dev->qf_cmd_lock);
	sema_init(&dev->caw_sem, 1);
	INIT_LIST_HEAD(&dev->t10_wwn.t10_vpd_list);
	spin_lock_init(&dev->t10_wwn.t10_vpd_lock);
	INIT_LIST_HEAD(&dev->t10_pr.registration_list);
	INIT_LIST_HEAD(&dev->t10_pr.aptpl_reg_list);
	spin_lock_init(&dev->t10_pr.registration_lock);
	spin_lock_init(&dev->t10_pr.aptpl_reg_lock);
	INIT_LIST_HEAD(&dev->t10_alua.tg_pt_gps_list);
	spin_lock_init(&dev->t10_alua.tg_pt_gps_lock);
	INIT_LIST_HEAD(&dev->t10_alua.lba_map_list);
	spin_lock_init(&dev->t10_alua.lba_map_lock);

	INIT_WORK(&dev->delayed_cmd_work, target_do_delayed_work);
	mutex_init(&dev->lun_reset_mutex);

	dev->t10_wwn.t10_dev = dev;
	/*
	 * Use OpenFabrics IEEE Company ID: 00 14 05
	 */
	dev->t10_wwn.company_id = 0x001405;

	dev->t10_alua.t10_dev = dev;

	dev->dev_attrib.da_dev = dev;
	dev->dev_attrib.emulate_model_alias = DA_EMULATE_MODEL_ALIAS;
	dev->dev_attrib.emulate_dpo = 1;
	dev->dev_attrib.emulate_fua_write = 1;
	dev->dev_attrib.emulate_fua_read = 1;
	dev->dev_attrib.emulate_write_cache = DA_EMULATE_WRITE_CACHE;
	dev->dev_attrib.emulate_ua_intlck_ctrl = TARGET_UA_INTLCK_CTRL_CLEAR;
	dev->dev_attrib.emulate_tas = DA_EMULATE_TAS;
	dev->dev_attrib.emulate_tpu = DA_EMULATE_TPU;
	dev->dev_attrib.emulate_tpws = DA_EMULATE_TPWS;
	dev->dev_attrib.emulate_caw = DA_EMULATE_CAW;
	dev->dev_attrib.emulate_3pc = DA_EMULATE_3PC;
	dev->dev_attrib.emulate_pr = DA_EMULATE_PR;
	dev->dev_attrib.emulate_rsoc = DA_EMULATE_RSOC;
	dev->dev_attrib.pi_prot_type = TARGET_DIF_TYPE0_PROT;
	dev->dev_attrib.enforce_pr_isids = DA_ENFORCE_PR_ISIDS;
	dev->dev_attrib.force_pr_aptpl = DA_FORCE_PR_APTPL;
	dev->dev_attrib.is_nonrot = DA_IS_NONROT;
	dev->dev_attrib.emulate_rest_reord = DA_EMULATE_REST_REORD;
	dev->dev_attrib.max_unmap_lba_count = DA_MAX_UNMAP_LBA_COUNT;
	dev->dev_attrib.max_unmap_block_desc_count =
		DA_MAX_UNMAP_BLOCK_DESC_COUNT;
	dev->dev_attrib.unmap_granularity = DA_UNMAP_GRANULARITY_DEFAULT;
	dev->dev_attrib.unmap_granularity_alignment =
				DA_UNMAP_GRANULARITY_ALIGNMENT_DEFAULT;
	dev->dev_attrib.unmap_zeroes_data =
				DA_UNMAP_ZEROES_DATA_DEFAULT;
	dev->dev_attrib.max_write_same_len = DA_MAX_WRITE_SAME_LEN;
	dev->dev_attrib.submit_type = TARGET_FABRIC_DEFAULT_SUBMIT;

	xcopy_lun = &dev->xcopy_lun;
	rcu_assign_pointer(xcopy_lun->lun_se_dev, dev);
	init_completion(&xcopy_lun->lun_shutdown_comp);
	INIT_LIST_HEAD(&xcopy_lun->lun_deve_list);
	INIT_LIST_HEAD(&xcopy_lun->lun_dev_link);
	mutex_init(&xcopy_lun->lun_tg_pt_md_mutex);
	xcopy_lun->lun_tpg = &xcopy_pt_tpg;

	/* Preload the default INQUIRY const values */
	strscpy(dev->t10_wwn.vendor, "LIO-ORG", sizeof(dev->t10_wwn.vendor));
	strscpy(dev->t10_wwn.model, dev->transport->inquiry_prod,
		sizeof(dev->t10_wwn.model));
	strscpy(dev->t10_wwn.revision, dev->transport->inquiry_rev,
		sizeof(dev->t10_wwn.revision));

	return dev;
}

/*
 * Check if the underlying struct block_device supports discard and if yes
 * configure the UNMAP parameters.
 */
bool target_configure_unmap_from_queue(struct se_dev_attrib *attrib,
				       struct block_device *bdev)
{
	int block_size = bdev_logical_block_size(bdev);

	if (!bdev_max_discard_sectors(bdev))
		return false;

	attrib->max_unmap_lba_count =
		bdev_max_discard_sectors(bdev) >> (ilog2(block_size) - 9);
	/*
	 * Currently hardcoded to 1 in Linux/SCSI code..
	 */
	attrib->max_unmap_block_desc_count = 1;
	attrib->unmap_granularity = bdev_discard_granularity(bdev) / block_size;
	attrib->unmap_granularity_alignment =
		bdev_discard_alignment(bdev) / block_size;
	return true;
}
EXPORT_SYMBOL(target_configure_unmap_from_queue);

/*
 * Convert from blocksize advertised to the initiator to the 512 byte
 * units unconditionally used by the Linux block layer.
 */
sector_t target_to_linux_sector(struct se_device *dev, sector_t lb)
{
	switch (dev->dev_attrib.block_size) {
	case 4096:
		return lb << 3;
	case 2048:
		return lb << 2;
	case 1024:
		return lb << 1;
	default:
		return lb;
	}
}
EXPORT_SYMBOL(target_to_linux_sector);

struct devices_idr_iter {
	int (*fn)(struct se_device *dev, void *data);
	void *data;
};

static int target_devices_idr_iter(int id, void *p, void *data)
	 __must_hold(&device_mutex)
{
	struct devices_idr_iter *iter = data;
	struct se_device *dev = p;
	struct config_item *item;
	int ret;

	/*
	 * We add the device early to the idr, so it can be used
	 * by backend modules during configuration. We do not want
	 * to allow other callers to access partially setup devices,
	 * so we skip them here.
	 */
	if (!target_dev_configured(dev))
		return 0;

	item = config_item_get_unless_zero(&dev->dev_group.cg_item);
	if (!item)
		return 0;
	mutex_unlock(&device_mutex);

	ret = iter->fn(dev, iter->data);
	config_item_put(item);

	mutex_lock(&device_mutex);
	return ret;
}

/**
 * target_for_each_device - iterate over configured devices
 * @fn: iterator function
 * @data: pointer to data that will be passed to fn
 *
 * fn must return 0 to continue looping over devices. non-zero will break
 * from the loop and return that value to the caller.
 */
int target_for_each_device(int (*fn)(struct se_device *dev, void *data),
			   void *data)
{
	struct devices_idr_iter iter = { .fn = fn, .data = data };
	int ret;

	mutex_lock(&device_mutex);
	ret = idr_for_each(&devices_idr, target_devices_idr_iter, &iter);
	mutex_unlock(&device_mutex);
	return ret;
}

int target_configure_device(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;
	int ret, id;

	if (target_dev_configured(dev)) {
		pr_err("se_dev->se_dev_ptr already set for storage"
				" object\n");
		return -EEXIST;
	}

	/*
	 * Add early so modules like tcmu can use during its
	 * configuration.
	 */
	mutex_lock(&device_mutex);
	/*
	 * Use cyclic to try and avoid collisions with devices
	 * that were recently removed.
	 */
	id = idr_alloc_cyclic(&devices_idr, dev, 0, INT_MAX, GFP_KERNEL);
	mutex_unlock(&device_mutex);
	if (id < 0) {
		ret = -ENOMEM;
		goto out;
	}
	dev->dev_index = id;

	ret = dev->transport->configure_device(dev);
	if (ret)
		goto out_free_index;

	if (dev->transport->configure_unmap &&
	    dev->transport->configure_unmap(dev)) {
		pr_debug("Discard support available, but disabled by default.\n");
	}

	/*
	 * XXX: there is not much point to have two different values here..
	 */
	dev->dev_attrib.block_size = dev->dev_attrib.hw_block_size;
	dev->dev_attrib.queue_depth = dev->dev_attrib.hw_queue_depth;

	/*
	 * Align max_hw_sectors down to PAGE_SIZE I/O transfers
	 */
	dev->dev_attrib.hw_max_sectors =
		se_dev_align_max_sectors(dev->dev_attrib.hw_max_sectors,
					 dev->dev_attrib.hw_block_size);
	dev->dev_attrib.optimal_sectors = dev->dev_attrib.hw_max_sectors;

	dev->creation_time = get_jiffies_64();

	ret = core_setup_alua(dev);
	if (ret)
		goto out_destroy_device;

	/*
	 * Setup work_queue for QUEUE_FULL
	 */
	INIT_WORK(&dev->qf_work_queue, target_qf_do_work);

	scsi_dump_inquiry(dev);

	spin_lock(&hba->device_lock);
	hba->dev_count++;
	spin_unlock(&hba->device_lock);

	dev->dev_flags |= DF_CONFIGURED;

	return 0;

out_destroy_device:
	dev->transport->destroy_device(dev);
out_free_index:
	mutex_lock(&device_mutex);
	idr_remove(&devices_idr, dev->dev_index);
	mutex_unlock(&device_mutex);
out:
	se_release_vpd_for_dev(dev);
	return ret;
}

void target_free_device(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	WARN_ON(!list_empty(&dev->dev_sep_list));

	if (target_dev_configured(dev)) {
		dev->transport->destroy_device(dev);

		mutex_lock(&device_mutex);
		idr_remove(&devices_idr, dev->dev_index);
		mutex_unlock(&device_mutex);

		spin_lock(&hba->device_lock);
		hba->dev_count--;
		spin_unlock(&hba->device_lock);
	}

	core_alua_free_lu_gp_mem(dev);
	core_alua_set_lba_map(dev, NULL, 0, 0);
	core_scsi3_free_all_registrations(dev);
	se_release_vpd_for_dev(dev);

	if (dev->transport->free_prot)
		dev->transport->free_prot(dev);

	kfree(dev->queues);
	dev->transport->free_device(dev);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int core_dev_setup_virtual_lun0(void)
{
	struct se_hba *hba;
	struct se_device *dev;
<<<<<<< HEAD
	struct se_subsystem_dev *se_dev = NULL;
	struct se_subsystem_api *t;
	char buf[16];
=======
	char buf[] = "rd_pages=8,rd_nullio=1,rd_dummy=1";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	hba = core_alloc_hba("rd_mcp", 0, HBA_FLAGS_INTERNAL_USE);
	if (IS_ERR(hba))
		return PTR_ERR(hba);

<<<<<<< HEAD
	lun0_hba = hba;
	t = hba->transport;

	se_dev = kzalloc(sizeof(struct se_subsystem_dev), GFP_KERNEL);
	if (!se_dev) {
		pr_err("Unable to allocate memory for"
				" struct se_subsystem_dev\n");
		ret = -ENOMEM;
		goto out;
	}
	INIT_LIST_HEAD(&se_dev->t10_wwn.t10_vpd_list);
	spin_lock_init(&se_dev->t10_wwn.t10_vpd_lock);
	INIT_LIST_HEAD(&se_dev->t10_pr.registration_list);
	INIT_LIST_HEAD(&se_dev->t10_pr.aptpl_reg_list);
	spin_lock_init(&se_dev->t10_pr.registration_lock);
	spin_lock_init(&se_dev->t10_pr.aptpl_reg_lock);
	INIT_LIST_HEAD(&se_dev->t10_alua.tg_pt_gps_list);
	spin_lock_init(&se_dev->t10_alua.tg_pt_gps_lock);
	spin_lock_init(&se_dev->se_dev_lock);
	se_dev->t10_pr.pr_aptpl_buf_len = PR_APTPL_BUF_LEN;
	se_dev->t10_wwn.t10_sub_dev = se_dev;
	se_dev->t10_alua.t10_sub_dev = se_dev;
	se_dev->se_dev_attrib.da_sub_dev = se_dev;
	se_dev->se_dev_hba = hba;

	se_dev->se_dev_su_ptr = t->allocate_virtdevice(hba, "virt_lun0");
	if (!se_dev->se_dev_su_ptr) {
		pr_err("Unable to locate subsystem dependent pointer"
			" from allocate_virtdevice()\n");
		ret = -ENOMEM;
		goto out;
	}
	lun0_su_dev = se_dev;

	memset(buf, 0, 16);
	sprintf(buf, "rd_pages=8");
	t->set_configfs_dev_params(hba, se_dev, buf, sizeof(buf));

	dev = t->create_virtdevice(hba, se_dev, se_dev->se_dev_su_ptr);
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto out;
	}
	dev->dev_link_magic = SE_DEV_LINK_MAGIC;
	se_dev->se_dev_ptr = dev;
	g_lun0_dev = dev;

	return 0;
out:
	lun0_su_dev = NULL;
	kfree(se_dev);
	if (lun0_hba) {
		core_delete_hba(lun0_hba);
		lun0_hba = NULL;
	}
=======
	dev = target_alloc_device(hba, "virt_lun0");
	if (!dev) {
		ret = -ENOMEM;
		goto out_free_hba;
	}

	hba->backend->ops->set_configfs_dev_params(dev, buf, sizeof(buf));

	ret = target_configure_device(dev);
	if (ret)
		goto out_free_se_dev;

	lun0_hba = hba;
	g_lun0_dev = dev;
	return 0;

out_free_se_dev:
	target_free_device(dev);
out_free_hba:
	core_delete_hba(hba);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}


void core_dev_release_virtual_lun0(void)
{
	struct se_hba *hba = lun0_hba;
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = lun0_su_dev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!hba)
		return;

	if (g_lun0_dev)
<<<<<<< HEAD
		se_free_virtual_device(g_lun0_dev, hba);

	kfree(su_dev);
	core_delete_hba(hba);
}
=======
		target_free_device(g_lun0_dev);
	core_delete_hba(hba);
}

/*
 * Common CDB parsing for kernel and user passthrough.
 */
sense_reason_t
passthrough_parse_cdb(struct se_cmd *cmd,
	sense_reason_t (*exec_cmd)(struct se_cmd *cmd))
{
	unsigned char *cdb = cmd->t_task_cdb;
	struct se_device *dev = cmd->se_dev;
	unsigned int size;

	/*
	 * For REPORT LUNS we always need to emulate the response, for everything
	 * else, pass it up.
	 */
	if (cdb[0] == REPORT_LUNS) {
		cmd->execute_cmd = spc_emulate_report_luns;
		return TCM_NO_SENSE;
	}

	/*
	 * With emulate_pr disabled, all reservation requests should fail,
	 * regardless of whether or not TRANSPORT_FLAG_PASSTHROUGH_PGR is set.
	 */
	if (!dev->dev_attrib.emulate_pr &&
	    ((cdb[0] == PERSISTENT_RESERVE_IN) ||
	     (cdb[0] == PERSISTENT_RESERVE_OUT) ||
	     (cdb[0] == RELEASE || cdb[0] == RELEASE_10) ||
	     (cdb[0] == RESERVE || cdb[0] == RESERVE_10))) {
		return TCM_UNSUPPORTED_SCSI_OPCODE;
	}

	/*
	 * For PERSISTENT RESERVE IN/OUT, RELEASE, and RESERVE we need to
	 * emulate the response, since tcmu does not have the information
	 * required to process these commands.
	 */
	if (!(dev->transport_flags &
	      TRANSPORT_FLAG_PASSTHROUGH_PGR)) {
		if (cdb[0] == PERSISTENT_RESERVE_IN) {
			cmd->execute_cmd = target_scsi3_emulate_pr_in;
			size = get_unaligned_be16(&cdb[7]);
			return target_cmd_size_check(cmd, size);
		}
		if (cdb[0] == PERSISTENT_RESERVE_OUT) {
			cmd->execute_cmd = target_scsi3_emulate_pr_out;
			size = get_unaligned_be32(&cdb[5]);
			return target_cmd_size_check(cmd, size);
		}

		if (cdb[0] == RELEASE || cdb[0] == RELEASE_10) {
			cmd->execute_cmd = target_scsi2_reservation_release;
			if (cdb[0] == RELEASE_10)
				size = get_unaligned_be16(&cdb[7]);
			else
				size = cmd->data_length;
			return target_cmd_size_check(cmd, size);
		}
		if (cdb[0] == RESERVE || cdb[0] == RESERVE_10) {
			cmd->execute_cmd = target_scsi2_reservation_reserve;
			if (cdb[0] == RESERVE_10)
				size = get_unaligned_be16(&cdb[7]);
			else
				size = cmd->data_length;
			return target_cmd_size_check(cmd, size);
		}
	}

	/* Set DATA_CDB flag for ops that should have it */
	switch (cdb[0]) {
	case READ_6:
	case READ_10:
	case READ_12:
	case READ_16:
	case WRITE_6:
	case WRITE_10:
	case WRITE_12:
	case WRITE_16:
	case WRITE_VERIFY:
	case WRITE_VERIFY_12:
	case WRITE_VERIFY_16:
	case COMPARE_AND_WRITE:
	case XDWRITEREAD_10:
		cmd->se_cmd_flags |= SCF_SCSI_DATA_CDB;
		break;
	case VARIABLE_LENGTH_CMD:
		switch (get_unaligned_be16(&cdb[8])) {
		case READ_32:
		case WRITE_32:
		case WRITE_VERIFY_32:
		case XDWRITEREAD_32:
			cmd->se_cmd_flags |= SCF_SCSI_DATA_CDB;
			break;
		}
	}

	cmd->execute_cmd = exec_cmd;

	return TCM_NO_SENSE;
}
EXPORT_SYMBOL(passthrough_parse_cdb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
