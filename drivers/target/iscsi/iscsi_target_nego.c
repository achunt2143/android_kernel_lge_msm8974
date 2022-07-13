<<<<<<< HEAD
/*******************************************************************************
 * This file contains main functions related to iSCSI Parameter negotiation.
 *
 * \u00a9 Copyright 2007-2011 RisingTide Systems LLC.
 *
 * Licensed to the Linux Foundation under the General Public License (GPL) version 2.
 *
 * Author: Nicholas A. Bellinger <nab@linux-iscsi.org>
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
 ******************************************************************************/

#include <linux/ctype.h>
#include <scsi/iscsi_proto.h>
#include <target/target_core_base.h>
#include <target/target_core_fabric.h>

#include "iscsi_target_core.h"
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*******************************************************************************
 * This file contains main functions related to iSCSI Parameter negotiation.
 *
 * (c) Copyright 2007-2013 Datera, Inc.
 *
 * Author: Nicholas A. Bellinger <nab@linux-iscsi.org>
 *
 ******************************************************************************/

#include <linux/ctype.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
#include <net/sock.h>
#include <trace/events/sock.h>
#include <scsi/iscsi_proto.h>
#include <target/target_core_base.h>
#include <target/target_core_fabric.h>
#include <target/iscsi/iscsi_transport.h>

#include <target/iscsi/iscsi_target_core.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "iscsi_target_parameters.h"
#include "iscsi_target_login.h"
#include "iscsi_target_nego.h"
#include "iscsi_target_tpg.h"
#include "iscsi_target_util.h"
#include "iscsi_target.h"
#include "iscsi_target_auth.h"

#define MAX_LOGIN_PDUS  7
<<<<<<< HEAD
#define TEXT_LEN	4096
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void convert_null_to_semi(char *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
		if (buf[i] == '\0')
			buf[i] = ';';
}

<<<<<<< HEAD
int strlen_semi(char *buf)
=======
static int strlen_semi(char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i = 0;

	while (buf[i] != '\0') {
		if (buf[i] == ';')
			return i;
		i++;
	}

	return -1;
}

int extract_param(
	const char *in_buf,
	const char *pattern,
	unsigned int max_length,
	char *out_buf,
	unsigned char *type)
{
	char *ptr;
	int len;

	if (!in_buf || !pattern || !out_buf || !type)
<<<<<<< HEAD
		return -1;

	ptr = strstr(in_buf, pattern);
	if (!ptr)
		return -1;

	ptr = strstr(ptr, "=");
	if (!ptr)
		return -1;
=======
		return -EINVAL;

	ptr = strstr(in_buf, pattern);
	if (!ptr)
		return -ENOENT;

	ptr = strstr(ptr, "=");
	if (!ptr)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ptr += 1;
	if (*ptr == '0' && (*(ptr+1) == 'x' || *(ptr+1) == 'X')) {
		ptr += 2; /* skip 0x */
		*type = HEX;
<<<<<<< HEAD
=======
	} else if (*ptr == '0' && (*(ptr+1) == 'b' || *(ptr+1) == 'B')) {
		ptr += 2; /* skip 0b */
		*type = BASE64;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		*type = DECIMAL;

	len = strlen_semi(ptr);
	if (len < 0)
<<<<<<< HEAD
		return -1;
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (len >= max_length) {
		pr_err("Length of input: %d exceeds max_length:"
			" %d\n", len, max_length);
<<<<<<< HEAD
		return -1;
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	memcpy(out_buf, ptr, len);
	out_buf[len] = '\0';

	return 0;
}

<<<<<<< HEAD
static u32 iscsi_handle_authentication(
	struct iscsi_conn *conn,
=======
static struct iscsi_node_auth *iscsi_get_node_auth(struct iscsit_conn *conn)
{
	struct iscsi_portal_group *tpg;
	struct iscsi_node_acl *nacl;
	struct se_node_acl *se_nacl;

	if (conn->sess->sess_ops->SessionType)
		return &iscsit_global->discovery_acl.node_auth;

	se_nacl = conn->sess->se_sess->se_node_acl;
	if (!se_nacl) {
		pr_err("Unable to locate struct se_node_acl for CHAP auth\n");
		return NULL;
	}

	if (se_nacl->dynamic_node_acl) {
		tpg = to_iscsi_tpg(se_nacl->se_tpg);
		return &tpg->tpg_demo_auth;
	}

	nacl = to_iscsi_nacl(se_nacl);

	return &nacl->node_auth;
}

static u32 iscsi_handle_authentication(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *in_buf,
	char *out_buf,
	int in_length,
	int *out_length,
	unsigned char *authtype)
{
<<<<<<< HEAD
	struct iscsi_session *sess = conn->sess;
	struct iscsi_node_auth *auth;
	struct iscsi_node_acl *iscsi_nacl;
	struct se_node_acl *se_nacl;

	if (!sess->sess_ops->SessionType) {
		/*
		 * For SessionType=Normal
		 */
		se_nacl = conn->sess->se_sess->se_node_acl;
		if (!se_nacl) {
			pr_err("Unable to locate struct se_node_acl for"
					" CHAP auth\n");
			return -1;
		}
		iscsi_nacl = container_of(se_nacl, struct iscsi_node_acl,
				se_node_acl);
		if (!iscsi_nacl) {
			pr_err("Unable to locate struct iscsi_node_acl for"
					" CHAP auth\n");
			return -1;
		}

		auth = ISCSI_NODE_AUTH(iscsi_nacl);
	} else {
		/*
		 * For SessionType=Discovery
		 */
		auth = &iscsit_global->discovery_acl.node_auth;
	}
=======
	struct iscsi_node_auth *auth;

	auth = iscsi_get_node_auth(conn);
	if (!auth)
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (strstr("CHAP", authtype))
		strcpy(conn->sess->auth_type, "CHAP");
	else
		strcpy(conn->sess->auth_type, NONE);

	if (strstr("None", authtype))
		return 1;
<<<<<<< HEAD
#ifdef CANSRP
	else if (strstr("SRP", authtype))
		return srp_main_loop(conn, auth, in_buf, out_buf,
				&in_length, out_length);
#endif
	else if (strstr("CHAP", authtype))
		return chap_main_loop(conn, auth, in_buf, out_buf,
				&in_length, out_length);
	else if (strstr("SPKM1", authtype))
		return 2;
	else if (strstr("SPKM2", authtype))
		return 2;
	else if (strstr("KRB5", authtype))
		return 2;
	else
		return 2;
}

static void iscsi_remove_failed_auth_entry(struct iscsi_conn *conn)
=======
	else if (strstr("CHAP", authtype))
		return chap_main_loop(conn, auth, in_buf, out_buf,
				&in_length, out_length);
	/* SRP, SPKM1, SPKM2 and KRB5 are unsupported */
	return 2;
}

static void iscsi_remove_failed_auth_entry(struct iscsit_conn *conn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	kfree(conn->auth_protocol);
}

<<<<<<< HEAD
static int iscsi_target_check_login_request(
	struct iscsi_conn *conn,
=======
int iscsi_target_check_login_request(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	int req_csg, req_nsg;
	u32 payload_length;
	struct iscsi_login_req *login_req;

	login_req = (struct iscsi_login_req *) login->req;
	payload_length = ntoh24(login_req->dlength);

	switch (login_req->opcode & ISCSI_OPCODE_MASK) {
	case ISCSI_OP_LOGIN:
		break;
	default:
		pr_err("Received unknown opcode 0x%02x.\n",
				login_req->opcode & ISCSI_OPCODE_MASK);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if ((login_req->flags & ISCSI_FLAG_LOGIN_CONTINUE) &&
	    (login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT)) {
		pr_err("Login request has both ISCSI_FLAG_LOGIN_CONTINUE"
			" and ISCSI_FLAG_LOGIN_TRANSIT set, protocol error.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

<<<<<<< HEAD
	req_csg = (login_req->flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK) >> 2;
	req_nsg = (login_req->flags & ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK);
=======
	req_csg = ISCSI_LOGIN_CURRENT_STAGE(login_req->flags);
	req_nsg = ISCSI_LOGIN_NEXT_STAGE(login_req->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (req_csg != login->current_stage) {
		pr_err("Initiator unexpectedly changed login stage"
			" from %d to %d, login failed.\n", login->current_stage,
			req_csg);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if ((req_nsg == 2) || (req_csg >= 2) ||
	   ((login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT) &&
	    (req_nsg <= req_csg))) {
		pr_err("Illegal login_req->flags Combination, CSG: %d,"
			" NSG: %d, ISCSI_FLAG_LOGIN_TRANSIT: %d.\n", req_csg,
			req_nsg, (login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT));
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if ((login_req->max_version != login->version_max) ||
	    (login_req->min_version != login->version_min)) {
		pr_err("Login request changed Version Max/Nin"
			" unexpectedly to 0x%02x/0x%02x, protocol error\n",
			login_req->max_version, login_req->min_version);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if (memcmp(login_req->isid, login->isid, 6) != 0) {
		pr_err("Login request changed ISID unexpectedly,"
				" protocol error.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if (login_req->itt != login->init_task_tag) {
		pr_err("Login request changed ITT unexpectedly to"
			" 0x%08x, protocol error.\n", login_req->itt);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if (payload_length > MAX_KEY_VALUE_PAIRS) {
		pr_err("Login request payload exceeds default"
			" MaxRecvDataSegmentLength: %u, protocol error.\n",
				MAX_KEY_VALUE_PAIRS);
		return -1;
	}

	return 0;
}
<<<<<<< HEAD

static int iscsi_target_check_first_request(
	struct iscsi_conn *conn,
=======
EXPORT_SYMBOL(iscsi_target_check_login_request);

static int iscsi_target_check_first_request(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	struct iscsi_param *param = NULL;
	struct se_node_acl *se_nacl;

	login->first_request = 0;

	list_for_each_entry(param, &conn->param_list->param_list, p_list) {
		if (!strncmp(param->name, SESSIONTYPE, 11)) {
			if (!IS_PSTATE_ACCEPTOR(param)) {
				pr_err("SessionType key not received"
					" in first login request.\n");
				iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
					ISCSI_LOGIN_STATUS_MISSING_FIELDS);
				return -1;
			}
			if (!strncmp(param->value, DISCOVERY, 9))
				return 0;
		}

		if (!strncmp(param->name, INITIATORNAME, 13)) {
			if (!IS_PSTATE_ACCEPTOR(param)) {
				if (!login->leading_connection)
					continue;

				pr_err("InitiatorName key not received"
					" in first login request.\n");
				iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
					ISCSI_LOGIN_STATUS_MISSING_FIELDS);
				return -1;
			}

			/*
			 * For non-leading connections, double check that the
			 * received InitiatorName matches the existing session's
			 * struct iscsi_node_acl.
			 */
			if (!login->leading_connection) {
				se_nacl = conn->sess->se_sess->se_node_acl;
				if (!se_nacl) {
					pr_err("Unable to locate"
						" struct se_node_acl\n");
					iscsit_tx_login_rsp(conn,
							ISCSI_STATUS_CLS_INITIATOR_ERR,
							ISCSI_LOGIN_STATUS_TGT_NOT_FOUND);
					return -1;
				}

				if (strcmp(param->value,
						se_nacl->initiatorname)) {
					pr_err("Incorrect"
						" InitiatorName: %s for this"
						" iSCSI Initiator Node.\n",
						param->value);
					iscsit_tx_login_rsp(conn,
							ISCSI_STATUS_CLS_INITIATOR_ERR,
							ISCSI_LOGIN_STATUS_TGT_NOT_FOUND);
					return -1;
				}
			}
		}
	}

	return 0;
}

<<<<<<< HEAD
static int iscsi_target_do_tx_login_io(struct iscsi_conn *conn, struct iscsi_login *login)
{
	u32 padding = 0;
	struct iscsi_session *sess = conn->sess;
=======
static int iscsi_target_do_tx_login_io(struct iscsit_conn *conn, struct iscsi_login *login)
{
	u32 padding = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login_rsp *login_rsp;

	login_rsp = (struct iscsi_login_rsp *) login->rsp;

	login_rsp->opcode		= ISCSI_OP_LOGIN_RSP;
	hton24(login_rsp->dlength, login->rsp_length);
	memcpy(login_rsp->isid, login->isid, 6);
	login_rsp->tsih			= cpu_to_be16(login->tsih);
<<<<<<< HEAD
	login_rsp->itt			= cpu_to_be32(login->init_task_tag);
	login_rsp->statsn		= cpu_to_be32(conn->stat_sn++);
	login_rsp->exp_cmdsn		= cpu_to_be32(conn->sess->exp_cmd_sn);
	login_rsp->max_cmdsn		= cpu_to_be32(conn->sess->max_cmd_sn);

	pr_debug("Sending Login Response, Flags: 0x%02x, ITT: 0x%08x,"
		" ExpCmdSN; 0x%08x, MaxCmdSN: 0x%08x, StatSN: 0x%08x, Length:"
		" %u\n", login_rsp->flags, ntohl(login_rsp->itt),
=======
	login_rsp->itt			= login->init_task_tag;
	login_rsp->statsn		= cpu_to_be32(conn->stat_sn++);
	login_rsp->exp_cmdsn		= cpu_to_be32(conn->sess->exp_cmd_sn);
	login_rsp->max_cmdsn		= cpu_to_be32((u32) atomic_read(&conn->sess->max_cmd_sn));

	pr_debug("Sending Login Response, Flags: 0x%02x, ITT: 0x%08x,"
		" ExpCmdSN; 0x%08x, MaxCmdSN: 0x%08x, StatSN: 0x%08x, Length:"
		" %u\n", login_rsp->flags, (__force u32)login_rsp->itt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ntohl(login_rsp->exp_cmdsn), ntohl(login_rsp->max_cmdsn),
		ntohl(login_rsp->statsn), login->rsp_length);

	padding = ((-login->rsp_length) & 3);
<<<<<<< HEAD

	if (iscsi_login_tx_data(
			conn,
			login->rsp,
			login->rsp_buf,
			login->rsp_length + padding) < 0)
		return -1;

	login->rsp_length		= 0;
	login_rsp->tsih			= be16_to_cpu(login_rsp->tsih);
	login_rsp->itt			= be32_to_cpu(login_rsp->itt);
	login_rsp->statsn		= be32_to_cpu(login_rsp->statsn);
	mutex_lock(&sess->cmdsn_mutex);
	login_rsp->exp_cmdsn		= be32_to_cpu(sess->exp_cmd_sn);
	login_rsp->max_cmdsn		= be32_to_cpu(sess->max_cmd_sn);
	mutex_unlock(&sess->cmdsn_mutex);

	return 0;
}

static int iscsi_target_do_rx_login_io(struct iscsi_conn *conn, struct iscsi_login *login)
{
	u32 padding = 0, payload_length;
	struct iscsi_login_req *login_req;

	if (iscsi_login_rx_data(conn, login->req, ISCSI_HDR_LEN) < 0)
		return -1;

	login_req = (struct iscsi_login_req *) login->req;
	payload_length			= ntoh24(login_req->dlength);
	login_req->tsih			= be16_to_cpu(login_req->tsih);
	login_req->itt			= be32_to_cpu(login_req->itt);
	login_req->cid			= be16_to_cpu(login_req->cid);
	login_req->cmdsn		= be32_to_cpu(login_req->cmdsn);
	login_req->exp_statsn		= be32_to_cpu(login_req->exp_statsn);

	pr_debug("Got Login Command, Flags 0x%02x, ITT: 0x%08x,"
		" CmdSN: 0x%08x, ExpStatSN: 0x%08x, CID: %hu, Length: %u\n",
		 login_req->flags, login_req->itt, login_req->cmdsn,
		 login_req->exp_statsn, login_req->cid, payload_length);

	if (iscsi_target_check_login_request(conn, login) < 0)
		return -1;

	padding = ((-payload_length) & 3);
	memset(login->req_buf, 0, MAX_KEY_VALUE_PAIRS);

	if (iscsi_login_rx_data(
			conn,
			login->req_buf,
			payload_length + padding) < 0)
		return -1;

	return 0;
}

static int iscsi_target_do_login_io(struct iscsi_conn *conn, struct iscsi_login *login)
{
	if (iscsi_target_do_tx_login_io(conn, login) < 0)
		return -1;

	if (iscsi_target_do_rx_login_io(conn, login) < 0)
		return -1;

	return 0;
}

static int iscsi_target_get_initial_payload(
	struct iscsi_conn *conn,
	struct iscsi_login *login)
{
	u32 padding = 0, payload_length;
	struct iscsi_login_req *login_req;

	login_req = (struct iscsi_login_req *) login->req;
	payload_length = ntoh24(login_req->dlength);

	pr_debug("Got Login Command, Flags 0x%02x, ITT: 0x%08x,"
		" CmdSN: 0x%08x, ExpStatSN: 0x%08x, Length: %u\n",
		login_req->flags, login_req->itt, login_req->cmdsn,
		login_req->exp_statsn, payload_length);

	if (iscsi_target_check_login_request(conn, login) < 0)
		return -1;

	padding = ((-payload_length) & 3);

	if (iscsi_login_rx_data(
			conn,
			login->req_buf,
			payload_length + padding) < 0)
		return -1;

	return 0;
=======
	/*
	 * Before sending the last login response containing the transition
	 * bit for full-feature-phase, go ahead and start up TX/RX threads
	 * now to avoid potential resource allocation failures after the
	 * final login response has been sent.
	 */
	if (login->login_complete) {
		int rc = iscsit_start_kthreads(conn);
		if (rc) {
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
					    ISCSI_LOGIN_STATUS_NO_RESOURCES);
			return -1;
		}
	}

	if (conn->conn_transport->iscsit_put_login_tx(conn, login,
					login->rsp_length + padding) < 0)
		goto err;

	login->rsp_length		= 0;

	return 0;

err:
	if (login->login_complete) {
		if (conn->rx_thread && conn->rx_thread_active) {
			send_sig(SIGINT, conn->rx_thread, 1);
			complete(&conn->rx_login_comp);
			kthread_stop(conn->rx_thread);
		}
		if (conn->tx_thread && conn->tx_thread_active) {
			send_sig(SIGINT, conn->tx_thread, 1);
			kthread_stop(conn->tx_thread);
		}
		spin_lock(&iscsit_global->ts_bitmap_lock);
		bitmap_release_region(iscsit_global->ts_bitmap, conn->bitmap_id,
				      get_order(1));
		spin_unlock(&iscsit_global->ts_bitmap_lock);
	}
	return -1;
}

static void iscsi_target_sk_data_ready(struct sock *sk)
{
	struct iscsit_conn *conn = sk->sk_user_data;
	bool rc;

	trace_sk_data_ready(sk);
	pr_debug("Entering iscsi_target_sk_data_ready: conn: %p\n", conn);

	write_lock_bh(&sk->sk_callback_lock);
	if (!sk->sk_user_data) {
		write_unlock_bh(&sk->sk_callback_lock);
		return;
	}
	if (!test_bit(LOGIN_FLAGS_READY, &conn->login_flags)) {
		write_unlock_bh(&sk->sk_callback_lock);
		pr_debug("Got LOGIN_FLAGS_READY=0, conn: %p >>>>\n", conn);
		return;
	}
	if (test_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags)) {
		write_unlock_bh(&sk->sk_callback_lock);
		pr_debug("Got LOGIN_FLAGS_CLOSED=1, conn: %p >>>>\n", conn);
		return;
	}
	if (test_and_set_bit(LOGIN_FLAGS_READ_ACTIVE, &conn->login_flags)) {
		write_unlock_bh(&sk->sk_callback_lock);
		pr_debug("Got LOGIN_FLAGS_READ_ACTIVE=1, conn: %p >>>>\n", conn);
		if (iscsi_target_sk_data_ready == conn->orig_data_ready)
			return;
		conn->orig_data_ready(sk);
		return;
	}

	rc = schedule_delayed_work(&conn->login_work, 0);
	if (!rc) {
		pr_debug("iscsi_target_sk_data_ready, schedule_delayed_work"
			 " got false\n");
	}
	write_unlock_bh(&sk->sk_callback_lock);
}

static void iscsi_target_sk_state_change(struct sock *);

static void iscsi_target_set_sock_callbacks(struct iscsit_conn *conn)
{
	struct sock *sk;

	if (!conn->sock)
		return;

	sk = conn->sock->sk;
	pr_debug("Entering iscsi_target_set_sock_callbacks: conn: %p\n", conn);

	write_lock_bh(&sk->sk_callback_lock);
	sk->sk_user_data = conn;
	conn->orig_data_ready = sk->sk_data_ready;
	conn->orig_state_change = sk->sk_state_change;
	sk->sk_data_ready = iscsi_target_sk_data_ready;
	sk->sk_state_change = iscsi_target_sk_state_change;
	write_unlock_bh(&sk->sk_callback_lock);

	sk->sk_sndtimeo = TA_LOGIN_TIMEOUT * HZ;
	sk->sk_rcvtimeo = TA_LOGIN_TIMEOUT * HZ;
}

static void iscsi_target_restore_sock_callbacks(struct iscsit_conn *conn)
{
	struct sock *sk;

	if (!conn->sock)
		return;

	sk = conn->sock->sk;
	pr_debug("Entering iscsi_target_restore_sock_callbacks: conn: %p\n", conn);

	write_lock_bh(&sk->sk_callback_lock);
	if (!sk->sk_user_data) {
		write_unlock_bh(&sk->sk_callback_lock);
		return;
	}
	sk->sk_user_data = NULL;
	sk->sk_data_ready = conn->orig_data_ready;
	sk->sk_state_change = conn->orig_state_change;
	write_unlock_bh(&sk->sk_callback_lock);

	sk->sk_sndtimeo = MAX_SCHEDULE_TIMEOUT;
	sk->sk_rcvtimeo = MAX_SCHEDULE_TIMEOUT;
}

static int iscsi_target_do_login(struct iscsit_conn *, struct iscsi_login *);

static bool __iscsi_target_sk_check_close(struct sock *sk)
{
	if (sk->sk_state == TCP_CLOSE_WAIT || sk->sk_state == TCP_CLOSE) {
		pr_debug("__iscsi_target_sk_check_close: TCP_CLOSE_WAIT|TCP_CLOSE,"
			"returning TRUE\n");
		return true;
	}
	return false;
}

static bool iscsi_target_sk_check_close(struct iscsit_conn *conn)
{
	bool state = false;

	if (conn->sock) {
		struct sock *sk = conn->sock->sk;

		read_lock_bh(&sk->sk_callback_lock);
		state = (__iscsi_target_sk_check_close(sk) ||
			 test_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags));
		read_unlock_bh(&sk->sk_callback_lock);
	}
	return state;
}

static bool iscsi_target_sk_check_flag(struct iscsit_conn *conn, unsigned int flag)
{
	bool state = false;

	if (conn->sock) {
		struct sock *sk = conn->sock->sk;

		read_lock_bh(&sk->sk_callback_lock);
		state = test_bit(flag, &conn->login_flags);
		read_unlock_bh(&sk->sk_callback_lock);
	}
	return state;
}

static bool iscsi_target_sk_check_and_clear(struct iscsit_conn *conn, unsigned int flag)
{
	bool state = false;

	if (conn->sock) {
		struct sock *sk = conn->sock->sk;

		write_lock_bh(&sk->sk_callback_lock);
		state = (__iscsi_target_sk_check_close(sk) ||
			 test_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags));
		if (!state)
			clear_bit(flag, &conn->login_flags);
		write_unlock_bh(&sk->sk_callback_lock);
	}
	return state;
}

static void iscsi_target_login_drop(struct iscsit_conn *conn, struct iscsi_login *login)
{
	bool zero_tsih = login->zero_tsih;

	iscsi_remove_failed_auth_entry(conn);
	iscsi_target_nego_release(conn);
	iscsi_target_login_sess_out(conn, zero_tsih, true);
}

static void iscsi_target_do_login_rx(struct work_struct *work)
{
	struct iscsit_conn *conn = container_of(work,
				struct iscsit_conn, login_work.work);
	struct iscsi_login *login = conn->login;
	struct iscsi_np *np = login->np;
	struct iscsi_portal_group *tpg = conn->tpg;
	struct iscsi_tpg_np *tpg_np = conn->tpg_np;
	int rc, zero_tsih = login->zero_tsih;
	bool state;

	pr_debug("entering iscsi_target_do_login_rx, conn: %p, %s:%d\n",
			conn, current->comm, current->pid);

	spin_lock(&conn->login_worker_lock);
	set_bit(LOGIN_FLAGS_WORKER_RUNNING, &conn->login_flags);
	spin_unlock(&conn->login_worker_lock);
	/*
	 * If iscsi_target_do_login_rx() has been invoked by ->sk_data_ready()
	 * before initial PDU processing in iscsi_target_start_negotiation()
	 * has completed, go ahead and retry until it's cleared.
	 *
	 * Otherwise if the TCP connection drops while this is occuring,
	 * iscsi_target_start_negotiation() will detect the failure, call
	 * cancel_delayed_work_sync(&conn->login_work), and cleanup the
	 * remaining iscsi connection resources from iscsi_np process context.
	 */
	if (iscsi_target_sk_check_flag(conn, LOGIN_FLAGS_INITIAL_PDU)) {
		schedule_delayed_work(&conn->login_work, msecs_to_jiffies(10));
		return;
	}

	spin_lock(&tpg->tpg_state_lock);
	state = (tpg->tpg_state == TPG_STATE_ACTIVE);
	spin_unlock(&tpg->tpg_state_lock);

	if (!state) {
		pr_debug("iscsi_target_do_login_rx: tpg_state != TPG_STATE_ACTIVE\n");
		goto err;
	}

	if (iscsi_target_sk_check_close(conn)) {
		pr_debug("iscsi_target_do_login_rx, TCP state CLOSE\n");
		goto err;
	}

	allow_signal(SIGINT);
	rc = iscsit_set_login_timer_kworker(conn, current);
	if (rc < 0) {
		/* The login timer has already expired */
		pr_debug("iscsi_target_do_login_rx, login failed\n");
		goto err;
	}

	rc = conn->conn_transport->iscsit_get_login_rx(conn, login);
	flush_signals(current);

	if (rc < 0)
		goto err;

	pr_debug("iscsi_target_do_login_rx after rx_login_io, %p, %s:%d\n",
			conn, current->comm, current->pid);

	/*
	 * LOGIN_FLAGS_READ_ACTIVE is cleared so that sk_data_ready
	 * could be triggered again after this.
	 *
	 * LOGIN_FLAGS_WRITE_ACTIVE is cleared after we successfully
	 * process a login PDU, so that sk_state_chage can do login
	 * cleanup as needed if the socket is closed. If a delayed work is
	 * ongoing (LOGIN_FLAGS_WRITE_ACTIVE or LOGIN_FLAGS_READ_ACTIVE),
	 * sk_state_change will leave the cleanup to the delayed work or
	 * it will schedule a delayed work to do cleanup.
	 */
	if (conn->sock) {
		struct sock *sk = conn->sock->sk;

		write_lock_bh(&sk->sk_callback_lock);
		if (!test_bit(LOGIN_FLAGS_INITIAL_PDU, &conn->login_flags)) {
			clear_bit(LOGIN_FLAGS_READ_ACTIVE, &conn->login_flags);
			set_bit(LOGIN_FLAGS_WRITE_ACTIVE, &conn->login_flags);
		}
		write_unlock_bh(&sk->sk_callback_lock);
	}

	rc = iscsi_target_do_login(conn, login);
	if (rc < 0) {
		goto err;
	} else if (!rc) {
		if (iscsi_target_sk_check_and_clear(conn,
						    LOGIN_FLAGS_WRITE_ACTIVE))
			goto err;

		/*
		 * Set the login timer thread pointer to NULL to prevent the
		 * login process from getting stuck if the initiator
		 * stops sending data.
		 */
		rc = iscsit_set_login_timer_kworker(conn, NULL);
		if (rc < 0)
			goto err;
	} else if (rc == 1) {
		iscsit_stop_login_timer(conn);
		cancel_delayed_work(&conn->login_work);
		iscsi_target_nego_release(conn);
		iscsi_post_login_handler(np, conn, zero_tsih);
		iscsit_deaccess_np(np, tpg, tpg_np);
	}
	return;

err:
	iscsi_target_restore_sock_callbacks(conn);
	iscsit_stop_login_timer(conn);
	cancel_delayed_work(&conn->login_work);
	iscsi_target_login_drop(conn, login);
	iscsit_deaccess_np(np, tpg, tpg_np);
}

static void iscsi_target_sk_state_change(struct sock *sk)
{
	struct iscsit_conn *conn;
	void (*orig_state_change)(struct sock *);
	bool state;

	pr_debug("Entering iscsi_target_sk_state_change\n");

	write_lock_bh(&sk->sk_callback_lock);
	conn = sk->sk_user_data;
	if (!conn) {
		write_unlock_bh(&sk->sk_callback_lock);
		return;
	}
	orig_state_change = conn->orig_state_change;

	if (!test_bit(LOGIN_FLAGS_READY, &conn->login_flags)) {
		pr_debug("Got LOGIN_FLAGS_READY=0 sk_state_change conn: %p\n",
			 conn);
		write_unlock_bh(&sk->sk_callback_lock);
		orig_state_change(sk);
		return;
	}
	state = __iscsi_target_sk_check_close(sk);
	pr_debug("__iscsi_target_sk_close_change: state: %d\n", state);

	if (test_bit(LOGIN_FLAGS_READ_ACTIVE, &conn->login_flags) ||
	    test_bit(LOGIN_FLAGS_WRITE_ACTIVE, &conn->login_flags)) {
		pr_debug("Got LOGIN_FLAGS_{READ|WRITE}_ACTIVE=1"
			 " sk_state_change conn: %p\n", conn);
		if (state)
			set_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags);
		write_unlock_bh(&sk->sk_callback_lock);
		orig_state_change(sk);
		return;
	}
	if (test_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags)) {
		pr_debug("Got LOGIN_FLAGS_CLOSED=1 sk_state_change conn: %p\n",
			 conn);
		write_unlock_bh(&sk->sk_callback_lock);
		orig_state_change(sk);
		return;
	}
	/*
	 * If the TCP connection has dropped, go ahead and set LOGIN_FLAGS_CLOSED,
	 * but only queue conn->login_work -> iscsi_target_do_login_rx()
	 * processing if LOGIN_FLAGS_INITIAL_PDU has already been cleared.
	 *
	 * When iscsi_target_do_login_rx() runs, iscsi_target_sk_check_close()
	 * will detect the dropped TCP connection from delayed workqueue context.
	 *
	 * If LOGIN_FLAGS_INITIAL_PDU is still set, which means the initial
	 * iscsi_target_start_negotiation() is running, iscsi_target_do_login()
	 * via iscsi_target_sk_check_close() or iscsi_target_start_negotiation()
	 * via iscsi_target_sk_check_and_clear() is responsible for detecting the
	 * dropped TCP connection in iscsi_np process context, and cleaning up
	 * the remaining iscsi connection resources.
	 */
	if (state) {
		pr_debug("iscsi_target_sk_state_change got failed state\n");
		set_bit(LOGIN_FLAGS_CLOSED, &conn->login_flags);
		state = test_bit(LOGIN_FLAGS_INITIAL_PDU, &conn->login_flags);
		write_unlock_bh(&sk->sk_callback_lock);

		orig_state_change(sk);

		if (!state)
			schedule_delayed_work(&conn->login_work, 0);
		return;
	}
	write_unlock_bh(&sk->sk_callback_lock);

	orig_state_change(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	NOTE: We check for existing sessions or connections AFTER the initiator
 *	has been successfully authenticated in order to protect against faked
 *	ISID/TSIH combinations.
 */
static int iscsi_target_check_for_existing_instances(
<<<<<<< HEAD
	struct iscsi_conn *conn,
=======
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	if (login->checked_for_existing)
		return 0;

	login->checked_for_existing = 1;

	if (!login->tsih)
		return iscsi_check_for_session_reinstatement(conn);
	else
		return iscsi_login_post_auth_non_zero_tsih(conn, login->cid,
				login->initial_exp_statsn);
}

static int iscsi_target_do_authentication(
<<<<<<< HEAD
	struct iscsi_conn *conn,
=======
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	int authret;
	u32 payload_length;
	struct iscsi_param *param;
	struct iscsi_login_req *login_req;
	struct iscsi_login_rsp *login_rsp;

	login_req = (struct iscsi_login_req *) login->req;
	login_rsp = (struct iscsi_login_rsp *) login->rsp;
	payload_length = ntoh24(login_req->dlength);

	param = iscsi_find_param_from_key(AUTHMETHOD, conn->param_list);
	if (!param)
		return -1;

	authret = iscsi_handle_authentication(
			conn,
			login->req_buf,
			login->rsp_buf,
			payload_length,
			&login->rsp_length,
			param->value);
	switch (authret) {
	case 0:
		pr_debug("Received OK response"
		" from LIO Authentication, continuing.\n");
		break;
	case 1:
		pr_debug("iSCSI security negotiation"
			" completed successfully.\n");
		login->auth_complete = 1;
		if ((login_req->flags & ISCSI_FLAG_LOGIN_NEXT_STAGE1) &&
		    (login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT)) {
			login_rsp->flags |= (ISCSI_FLAG_LOGIN_NEXT_STAGE1 |
					     ISCSI_FLAG_LOGIN_TRANSIT);
			login->current_stage = 1;
		}
		return iscsi_target_check_for_existing_instances(
				conn, login);
	case 2:
		pr_err("Security negotiation"
			" failed.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_AUTH_FAILED);
		return -1;
	default:
		pr_err("Received unknown error %d from LIO"
				" Authentication\n", authret);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_TARGET_ERROR);
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
static int iscsi_target_handle_csg_zero(
	struct iscsi_conn *conn,
=======
bool iscsi_conn_auth_required(struct iscsit_conn *conn)
{
	struct iscsi_node_acl *nacl;
	struct se_node_acl *se_nacl;

	if (conn->sess->sess_ops->SessionType) {
		/*
		 * For SessionType=Discovery
		 */
		return conn->tpg->tpg_attrib.authentication;
	}
	/*
	 * For SessionType=Normal
	 */
	se_nacl = conn->sess->se_sess->se_node_acl;
	if (!se_nacl) {
		pr_debug("Unknown ACL is trying to connect\n");
		return true;
	}

	if (se_nacl->dynamic_node_acl) {
		pr_debug("Dynamic ACL %s is trying to connect\n",
			 se_nacl->initiatorname);
		return conn->tpg->tpg_attrib.authentication;
	}

	pr_debug("Known ACL %s is trying to connect\n",
		 se_nacl->initiatorname);

	nacl = to_iscsi_nacl(se_nacl);
	if (nacl->node_attrib.authentication == NA_AUTHENTICATION_INHERITED)
		return conn->tpg->tpg_attrib.authentication;

	return nacl->node_attrib.authentication;
}

static int iscsi_target_handle_csg_zero(
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	int ret;
	u32 payload_length;
	struct iscsi_param *param;
	struct iscsi_login_req *login_req;
	struct iscsi_login_rsp *login_rsp;

	login_req = (struct iscsi_login_req *) login->req;
	login_rsp = (struct iscsi_login_rsp *) login->rsp;
	payload_length = ntoh24(login_req->dlength);

	param = iscsi_find_param_from_key(AUTHMETHOD, conn->param_list);
	if (!param)
		return -1;

	ret = iscsi_decode_text_input(
			PHASE_SECURITY|PHASE_DECLARATIVE,
			SENDER_INITIATOR|SENDER_RECEIVER,
			login->req_buf,
			payload_length,
<<<<<<< HEAD
			conn->param_list);
=======
			conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return -1;

	if (ret > 0) {
		if (login->auth_complete) {
			pr_err("Initiator has already been"
				" successfully authenticated, but is still"
				" sending %s keys.\n", param->value);
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
					ISCSI_LOGIN_STATUS_INIT_ERR);
			return -1;
		}

		goto do_auth;
<<<<<<< HEAD
=======
	} else if (!payload_length) {
		pr_err("Initiator sent zero length security payload,"
		       " login failed\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				    ISCSI_LOGIN_STATUS_AUTH_FAILED);
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (login->first_request)
		if (iscsi_target_check_first_request(conn, login) < 0)
			return -1;

	ret = iscsi_encode_text_output(
			PHASE_SECURITY|PHASE_DECLARATIVE,
			SENDER_TARGET,
			login->rsp_buf,
			&login->rsp_length,
<<<<<<< HEAD
			conn->param_list);
=======
			conn->param_list,
			conn->tpg->tpg_attrib.login_keys_workaround);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return -1;

	if (!iscsi_check_negotiated_keys(conn->param_list)) {
<<<<<<< HEAD
		if (ISCSI_TPG_ATTRIB(ISCSI_TPG_C(conn))->authentication &&
		    !strncmp(param->value, NONE, 4)) {
			pr_err("Initiator sent AuthMethod=None but"
				" Target is enforcing iSCSI Authentication,"
					" login failed.\n");
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
					ISCSI_LOGIN_STATUS_AUTH_FAILED);
			return -1;
		}

		if (ISCSI_TPG_ATTRIB(ISCSI_TPG_C(conn))->authentication &&
		    !login->auth_complete)
			return 0;

		if (strncmp(param->value, NONE, 4) && !login->auth_complete)
			return 0;

=======
		bool auth_required = iscsi_conn_auth_required(conn);

		if (auth_required) {
			if (!strncmp(param->value, NONE, 4)) {
				pr_err("Initiator sent AuthMethod=None but"
				       " Target is enforcing iSCSI Authentication,"
				       " login failed.\n");
				iscsit_tx_login_rsp(conn,
						ISCSI_STATUS_CLS_INITIATOR_ERR,
						ISCSI_LOGIN_STATUS_AUTH_FAILED);
				return -1;
			}

			if (!login->auth_complete)
				return 0;

			if (strncmp(param->value, NONE, 4) &&
			    !login->auth_complete)
				return 0;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((login_req->flags & ISCSI_FLAG_LOGIN_NEXT_STAGE1) &&
		    (login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT)) {
			login_rsp->flags |= ISCSI_FLAG_LOGIN_NEXT_STAGE1 |
					    ISCSI_FLAG_LOGIN_TRANSIT;
			login->current_stage = 1;
		}
	}

	return 0;
do_auth:
	return iscsi_target_do_authentication(conn, login);
}

<<<<<<< HEAD
static int iscsi_target_handle_csg_one(struct iscsi_conn *conn, struct iscsi_login *login)
=======
static bool iscsi_conn_authenticated(struct iscsit_conn *conn,
				     struct iscsi_login *login)
{
	if (!iscsi_conn_auth_required(conn))
		return true;

	if (login->auth_complete)
		return true;

	return false;
}

static int iscsi_target_handle_csg_one(struct iscsit_conn *conn, struct iscsi_login *login)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	u32 payload_length;
	struct iscsi_login_req *login_req;
	struct iscsi_login_rsp *login_rsp;

	login_req = (struct iscsi_login_req *) login->req;
	login_rsp = (struct iscsi_login_rsp *) login->rsp;
	payload_length = ntoh24(login_req->dlength);

	ret = iscsi_decode_text_input(
			PHASE_OPERATIONAL|PHASE_DECLARATIVE,
			SENDER_INITIATOR|SENDER_RECEIVER,
			login->req_buf,
			payload_length,
<<<<<<< HEAD
			conn->param_list);
=======
			conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0) {
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

	if (login->first_request)
		if (iscsi_target_check_first_request(conn, login) < 0)
			return -1;

	if (iscsi_target_check_for_existing_instances(conn, login) < 0)
		return -1;

	ret = iscsi_encode_text_output(
			PHASE_OPERATIONAL|PHASE_DECLARATIVE,
			SENDER_TARGET,
			login->rsp_buf,
			&login->rsp_length,
<<<<<<< HEAD
			conn->param_list);
=======
			conn->param_list,
			conn->tpg->tpg_attrib.login_keys_workaround);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0) {
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_INIT_ERR);
		return -1;
	}

<<<<<<< HEAD
	if (!login->auth_complete &&
	     ISCSI_TPG_ATTRIB(ISCSI_TPG_C(conn))->authentication) {
		pr_err("Initiator is requesting CSG: 1, has not been"
			 " successfully authenticated, and the Target is"
			" enforcing iSCSI Authentication, login failed.\n");
=======
	if (!iscsi_conn_authenticated(conn, login)) {
		pr_err("Initiator is requesting CSG: 1, has not been"
		       " successfully authenticated, and the Target is"
		       " enforcing iSCSI Authentication, login failed.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_AUTH_FAILED);
		return -1;
	}

	if (!iscsi_check_negotiated_keys(conn->param_list))
		if ((login_req->flags & ISCSI_FLAG_LOGIN_NEXT_STAGE3) &&
		    (login_req->flags & ISCSI_FLAG_LOGIN_TRANSIT))
			login_rsp->flags |= ISCSI_FLAG_LOGIN_NEXT_STAGE3 |
					    ISCSI_FLAG_LOGIN_TRANSIT;

	return 0;
}

<<<<<<< HEAD
static int iscsi_target_do_login(struct iscsi_conn *conn, struct iscsi_login *login)
=======
/*
 * RETURN VALUE:
 *
 *  1 = Login successful
 * -1 = Login failed
 *  0 = More PDU exchanges required
 */
static int iscsi_target_do_login(struct iscsit_conn *conn, struct iscsi_login *login)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int pdu_count = 0;
	struct iscsi_login_req *login_req;
	struct iscsi_login_rsp *login_rsp;

	login_req = (struct iscsi_login_req *) login->req;
	login_rsp = (struct iscsi_login_rsp *) login->rsp;

	while (1) {
		if (++pdu_count > MAX_LOGIN_PDUS) {
			pr_err("MAX_LOGIN_PDUS count reached.\n");
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
					ISCSI_LOGIN_STATUS_TARGET_ERROR);
			return -1;
		}

<<<<<<< HEAD
		switch ((login_req->flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK) >> 2) {
		case 0:
			login_rsp->flags |= (0 & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK);
=======
		switch (ISCSI_LOGIN_CURRENT_STAGE(login_req->flags)) {
		case 0:
			login_rsp->flags &= ~ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (iscsi_target_handle_csg_zero(conn, login) < 0)
				return -1;
			break;
		case 1:
			login_rsp->flags |= ISCSI_FLAG_LOGIN_CURRENT_STAGE1;
			if (iscsi_target_handle_csg_one(conn, login) < 0)
				return -1;
			if (login_rsp->flags & ISCSI_FLAG_LOGIN_TRANSIT) {
<<<<<<< HEAD
				login->tsih = conn->sess->tsih;
				if (iscsi_target_do_tx_login_io(conn,
						login) < 0)
					return -1;
				return 0;
=======
				/*
				 * Check to make sure the TCP connection has not
				 * dropped asynchronously while session reinstatement
				 * was occuring in this kthread context, before
				 * transitioning to full feature phase operation.
				 */
				if (iscsi_target_sk_check_close(conn))
					return -1;

				login->tsih = conn->sess->tsih;
				login->login_complete = 1;
				iscsi_target_restore_sock_callbacks(conn);
				if (iscsi_target_do_tx_login_io(conn,
						login) < 0)
					return -1;
				return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			break;
		default:
			pr_err("Illegal CSG: %d received from"
				" Initiator, protocol error.\n",
<<<<<<< HEAD
				(login_req->flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK)
				>> 2);
			break;
		}

		if (iscsi_target_do_login_io(conn, login) < 0)
=======
				ISCSI_LOGIN_CURRENT_STAGE(login_req->flags));
			break;
		}

		if (iscsi_target_do_tx_login_io(conn, login) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -1;

		if (login_rsp->flags & ISCSI_FLAG_LOGIN_TRANSIT) {
			login_rsp->flags &= ~ISCSI_FLAG_LOGIN_TRANSIT;
			login_rsp->flags &= ~ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK;
		}
<<<<<<< HEAD
=======
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

static void iscsi_initiatorname_tolower(
	char *param_buf)
{
	char *c;
	u32 iqn_size = strlen(param_buf), i;

	for (i = 0; i < iqn_size; i++) {
		c = &param_buf[i];
		if (!isupper(*c))
			continue;

		*c = tolower(*c);
	}
}

/*
 * Processes the first Login Request..
 */
<<<<<<< HEAD
static int iscsi_target_locate_portal(
	struct iscsi_np *np,
	struct iscsi_conn *conn,
=======
int iscsi_target_locate_portal(
	struct iscsi_np *np,
	struct iscsit_conn *conn,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct iscsi_login *login)
{
	char *i_buf = NULL, *s_buf = NULL, *t_buf = NULL;
	char *tmpbuf, *start = NULL, *end = NULL, *key, *value;
<<<<<<< HEAD
	struct iscsi_session *sess = conn->sess;
	struct iscsi_tiqn *tiqn;
	struct iscsi_login_req *login_req;
	u32 payload_length;
	int sessiontype = 0, ret = 0;
=======
	struct iscsit_session *sess = conn->sess;
	struct iscsi_tiqn *tiqn;
	struct iscsi_tpg_np *tpg_np = NULL;
	struct iscsi_login_req *login_req;
	struct se_node_acl *se_nacl;
	u32 payload_length, queue_depth = 0;
	int sessiontype = 0, ret = 0, tag_num, tag_size;

	INIT_DELAYED_WORK(&conn->login_work, iscsi_target_do_login_rx);
	iscsi_target_set_sock_callbacks(conn);

	login->np = np;
	conn->tpg = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	login_req = (struct iscsi_login_req *) login->req;
	payload_length = ntoh24(login_req->dlength);

<<<<<<< HEAD
	login->first_request	= 1;
	login->leading_connection = (!login_req->tsih) ? 1 : 0;
	login->current_stage	=
		(login_req->flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK) >> 2;
	login->version_min	= login_req->min_version;
	login->version_max	= login_req->max_version;
	memcpy(login->isid, login_req->isid, 6);
	login->cmd_sn		= login_req->cmdsn;
	login->init_task_tag	= login_req->itt;
	login->initial_exp_statsn = login_req->exp_statsn;
	login->cid		= login_req->cid;
	login->tsih		= login_req->tsih;

	if (iscsi_target_get_initial_payload(conn, login) < 0)
		return -1;

	tmpbuf = kzalloc(payload_length + 1, GFP_KERNEL);
=======
	tmpbuf = kmemdup_nul(login->req_buf, payload_length, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tmpbuf) {
		pr_err("Unable to allocate memory for tmpbuf.\n");
		return -1;
	}

<<<<<<< HEAD
	memcpy(tmpbuf, login->req_buf, payload_length);
	tmpbuf[payload_length] = '\0';
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start = tmpbuf;
	end = (start + payload_length);

	/*
	 * Locate the initial keys expected from the Initiator node in
	 * the first login request in order to progress with the login phase.
	 */
	while (start < end) {
		if (iscsi_extract_key_value(start, &key, &value) < 0) {
			ret = -1;
			goto out;
		}

		if (!strncmp(key, "InitiatorName", 13))
			i_buf = value;
		else if (!strncmp(key, "SessionType", 11))
			s_buf = value;
		else if (!strncmp(key, "TargetName", 10))
			t_buf = value;

		start += strlen(key) + strlen(value) + 2;
	}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * See 5.3.  Login Phase.
	 */
	if (!i_buf) {
		pr_err("InitiatorName key not received"
			" in first login request.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
			ISCSI_LOGIN_STATUS_MISSING_FIELDS);
		ret = -1;
		goto out;
	}
	/*
	 * Convert the incoming InitiatorName to lowercase following
	 * RFC-3720 3.2.6.1. section c) that says that iSCSI IQNs
	 * are NOT case sensitive.
	 */
	iscsi_initiatorname_tolower(i_buf);

	if (!s_buf) {
		if (!login->leading_connection)
			goto get_target;

		pr_err("SessionType key not received"
			" in first login request.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
			ISCSI_LOGIN_STATUS_MISSING_FIELDS);
		ret = -1;
		goto out;
	}

	/*
	 * Use default portal group for discovery sessions.
	 */
	sessiontype = strncmp(s_buf, DISCOVERY, 9);
	if (!sessiontype) {
<<<<<<< HEAD
		conn->tpg = iscsit_global->discovery_tpg;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!login->leading_connection)
			goto get_target;

		sess->sess_ops->SessionType = 1;
		/*
		 * Setup crc32c modules from libcrypto
		 */
		if (iscsi_login_setup_crypto(conn) < 0) {
			pr_err("iscsi_login_setup_crypto() failed\n");
			ret = -1;
			goto out;
		}
		/*
		 * Serialize access across the discovery struct iscsi_portal_group to
		 * process login attempt.
		 */
<<<<<<< HEAD
		if (iscsit_access_np(np, conn->tpg) < 0) {
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
=======
		conn->tpg = iscsit_global->discovery_tpg;
		if (iscsit_access_np(np, conn->tpg) < 0) {
			iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
			conn->tpg = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -1;
			goto out;
		}
		ret = 0;
<<<<<<< HEAD
		goto out;
=======
		goto alloc_tags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

get_target:
	if (!t_buf) {
		pr_err("TargetName key not received"
			" in first login request while"
			" SessionType=Normal.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
			ISCSI_LOGIN_STATUS_MISSING_FIELDS);
		ret = -1;
		goto out;
	}

	/*
	 * Locate Target IQN from Storage Node.
	 */
	tiqn = iscsit_get_tiqn_for_login(t_buf);
	if (!tiqn) {
		pr_err("Unable to locate Target IQN: %s in"
			" Storage Node\n", t_buf);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
		ret = -1;
		goto out;
	}
	pr_debug("Located Storage Object: %s\n", tiqn->tiqn);

	/*
	 * Locate Target Portal Group from Storage Node.
	 */
<<<<<<< HEAD
	conn->tpg = iscsit_get_tpg_from_np(tiqn, np);
=======
	conn->tpg = iscsit_get_tpg_from_np(tiqn, np, &tpg_np);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!conn->tpg) {
		pr_err("Unable to locate Target Portal Group"
				" on %s\n", tiqn->tiqn);
		iscsit_put_tiqn_for_login(tiqn);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
		ret = -1;
		goto out;
	}
<<<<<<< HEAD
=======
	conn->tpg_np = tpg_np;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_debug("Located Portal Group Object: %hu\n", conn->tpg->tpgt);
	/*
	 * Setup crc32c modules from libcrypto
	 */
	if (iscsi_login_setup_crypto(conn) < 0) {
		pr_err("iscsi_login_setup_crypto() failed\n");
<<<<<<< HEAD
=======
		kref_put(&tpg_np->tpg_np_kref, iscsit_login_kref_put);
		iscsit_put_tiqn_for_login(tiqn);
		conn->tpg = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -1;
		goto out;
	}
	/*
	 * Serialize access across the struct iscsi_portal_group to
	 * process login attempt.
	 */
	if (iscsit_access_np(np, conn->tpg) < 0) {
<<<<<<< HEAD
		iscsit_put_tiqn_for_login(tiqn);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
		ret = -1;
		conn->tpg = NULL;
=======
		kref_put(&tpg_np->tpg_np_kref, iscsit_login_kref_put);
		iscsit_put_tiqn_for_login(tiqn);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE);
		conn->tpg = NULL;
		ret = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/*
	 * conn->sess->node_acl will be set when the referenced
<<<<<<< HEAD
	 * struct iscsi_session is located from received ISID+TSIH in
=======
	 * struct iscsit_session is located from received ISID+TSIH in
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * iscsi_login_non_zero_tsih_s2().
	 */
	if (!login->leading_connection) {
		ret = 0;
		goto out;
	}

	/*
	 * This value is required in iscsi_login_zero_tsih_s2()
	 */
	sess->sess_ops->SessionType = 0;

	/*
	 * Locate incoming Initiator IQN reference from Storage Node.
	 */
	sess->se_sess->se_node_acl = core_tpg_check_initiator_node_acl(
			&conn->tpg->tpg_se_tpg, i_buf);
	if (!sess->se_sess->se_node_acl) {
		pr_err("iSCSI Initiator Node: %s is not authorized to"
			" access iSCSI target portal group: %hu.\n",
				i_buf, conn->tpg->tpgt);
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_INITIATOR_ERR,
				ISCSI_LOGIN_STATUS_TGT_FORBIDDEN);
		ret = -1;
		goto out;
	}
<<<<<<< HEAD

	ret = 0;
=======
	se_nacl = sess->se_sess->se_node_acl;
	queue_depth = se_nacl->queue_depth;
	/*
	 * Setup pre-allocated tags based upon allowed per NodeACL CmdSN
	 * depth for non immediate commands, plus extra tags for immediate
	 * commands.
	 *
	 * Also enforce a ISCSIT_MIN_TAGS to prevent unnecessary contention
	 * in per-cpu-ida tag allocation logic + small queue_depth.
	 */
alloc_tags:
	tag_num = max_t(u32, ISCSIT_MIN_TAGS, queue_depth);
	tag_num = (tag_num * 2) + ISCSIT_EXTRA_TAGS;
	tag_size = sizeof(struct iscsit_cmd) + conn->conn_transport->priv_size;

	ret = transport_alloc_session_tags(sess->se_sess, tag_num, tag_size);
	if (ret < 0) {
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				    ISCSI_LOGIN_STATUS_NO_RESOURCES);
		ret = -1;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(tmpbuf);
	return ret;
}

<<<<<<< HEAD
struct iscsi_login *iscsi_target_init_negotiation(
	struct iscsi_np *np,
	struct iscsi_conn *conn,
	char *login_pdu)
{
	struct iscsi_login *login;

	login = kzalloc(sizeof(struct iscsi_login), GFP_KERNEL);
	if (!login) {
		pr_err("Unable to allocate memory for struct iscsi_login.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_NO_RESOURCES);
		return NULL;
	}

	login->req = kmemdup(login_pdu, ISCSI_HDR_LEN, GFP_KERNEL);
	if (!login->req) {
		pr_err("Unable to allocate memory for Login Request.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_NO_RESOURCES);
		goto out;
	}

	login->req_buf = kzalloc(MAX_KEY_VALUE_PAIRS, GFP_KERNEL);
	if (!login->req_buf) {
		pr_err("Unable to allocate memory for response buffer.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_NO_RESOURCES);
		goto out;
	}
	/*
	 * SessionType: Discovery
	 *
	 *	Locates Default Portal
	 *
	 * SessionType: Normal
	 *
	 *	Locates Target Portal from NP -> Target IQN
	 */
	if (iscsi_target_locate_portal(np, conn, login) < 0) {
		pr_err("iSCSI Login negotiation failed.\n");
		goto out;
	}

	return login;
out:
	kfree(login->req);
	kfree(login->req_buf);
	kfree(login);

	return NULL;
}

int iscsi_target_start_negotiation(
	struct iscsi_login *login,
	struct iscsi_conn *conn)
{
	int ret = -1;

	login->rsp = kzalloc(ISCSI_HDR_LEN, GFP_KERNEL);
	if (!login->rsp) {
		pr_err("Unable to allocate memory for"
				" Login Response.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_NO_RESOURCES);
		ret = -1;
		goto out;
	}

	login->rsp_buf = kzalloc(MAX_KEY_VALUE_PAIRS, GFP_KERNEL);
	if (!login->rsp_buf) {
		pr_err("Unable to allocate memory for"
			" request buffer.\n");
		iscsit_tx_login_rsp(conn, ISCSI_STATUS_CLS_TARGET_ERR,
				ISCSI_LOGIN_STATUS_NO_RESOURCES);
		ret = -1;
		goto out;
	}

	ret = iscsi_target_do_login(conn, login);
out:
	if (ret != 0)
		iscsi_remove_failed_auth_entry(conn);

	iscsi_target_nego_release(login, conn);
	return ret;
}

void iscsi_target_nego_release(
	struct iscsi_login *login,
	struct iscsi_conn *conn)
{
	kfree(login->req);
	kfree(login->rsp);
	kfree(login->req_buf);
	kfree(login->rsp_buf);
	kfree(login);
=======
int iscsi_target_start_negotiation(
	struct iscsi_login *login,
	struct iscsit_conn *conn)
{
	int ret;

	if (conn->sock) {
		struct sock *sk = conn->sock->sk;

		write_lock_bh(&sk->sk_callback_lock);
		set_bit(LOGIN_FLAGS_READY, &conn->login_flags);
		set_bit(LOGIN_FLAGS_INITIAL_PDU, &conn->login_flags);
		write_unlock_bh(&sk->sk_callback_lock);
	}
	/*
	 * If iscsi_target_do_login returns zero to signal more PDU
	 * exchanges are required to complete the login, go ahead and
	 * clear LOGIN_FLAGS_INITIAL_PDU but only if the TCP connection
	 * is still active.
	 *
	 * Otherwise if TCP connection dropped asynchronously, go ahead
	 * and perform connection cleanup now.
	 */
	ret = iscsi_target_do_login(conn, login);
	if (!ret) {
		spin_lock(&conn->login_worker_lock);

		if (iscsi_target_sk_check_and_clear(conn, LOGIN_FLAGS_INITIAL_PDU))
			ret = -1;
		else if (!test_bit(LOGIN_FLAGS_WORKER_RUNNING, &conn->login_flags)) {
			if (iscsit_set_login_timer_kworker(conn, NULL) < 0) {
				/*
				 * The timeout has expired already.
				 * Schedule login_work to perform the cleanup.
				 */
				schedule_delayed_work(&conn->login_work, 0);
			}
		}

		spin_unlock(&conn->login_worker_lock);
	}

	if (ret < 0) {
		iscsi_target_restore_sock_callbacks(conn);
		iscsi_remove_failed_auth_entry(conn);
	}
	if (ret != 0) {
		iscsit_stop_login_timer(conn);
		cancel_delayed_work_sync(&conn->login_work);
		iscsi_target_nego_release(conn);
	}

	return ret;
}

void iscsi_target_nego_release(struct iscsit_conn *conn)
{
	struct iscsi_login *login = conn->conn_login;

	if (!login)
		return;

	kfree(login->req_buf);
	kfree(login->rsp_buf);
	kfree(login);

	conn->conn_login = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
