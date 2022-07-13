/*
 * An implementation of key value pair (KVP) functionality for Linux.
 *
 *
 * Copyright (C) 2010, Novell, Inc.
 * Author : K. Y. Srinivasan <ksrinivasan@novell.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/net.h>
#include <linux/nls.h>
#include <linux/connector.h>
#include <linux/workqueue.h>
#include <linux/hyperv.h>
<<<<<<< HEAD



/*
 * Global state maintained for transaction that is being processed.
 * Note that only one transaction can be active at any point in time.
 *
 * This state is set when we receive a request from the host; we
 * cleanup this state when the transaction is completed - when we respond
 * to the host with the key value.
 */

static struct {
	bool active; /* transaction status - active or not */
=======
#include <asm/hyperv-tlfs.h>

#include "hyperv_vmbus.h"
#include "hv_utils_transport.h"

/*
 * Pre win8 version numbers used in ws2008 and ws 2008 r2 (win7)
 */
#define WS2008_SRV_MAJOR	1
#define WS2008_SRV_MINOR	0
#define WS2008_SRV_VERSION     (WS2008_SRV_MAJOR << 16 | WS2008_SRV_MINOR)

#define WIN7_SRV_MAJOR   3
#define WIN7_SRV_MINOR   0
#define WIN7_SRV_VERSION     (WIN7_SRV_MAJOR << 16 | WIN7_SRV_MINOR)

#define WIN8_SRV_MAJOR   4
#define WIN8_SRV_MINOR   0
#define WIN8_SRV_VERSION     (WIN8_SRV_MAJOR << 16 | WIN8_SRV_MINOR)

#define KVP_VER_COUNT 3
static const int kvp_versions[] = {
	WIN8_SRV_VERSION,
	WIN7_SRV_VERSION,
	WS2008_SRV_VERSION
};

#define FW_VER_COUNT 2
static const int fw_versions[] = {
	UTIL_FW_VERSION,
	UTIL_WS2K8_FW_VERSION
};

/*
 * Global state maintained for transaction that is being processed. For a class
 * of integration services, including the "KVP service", the specified protocol
 * is a "request/response" protocol which means that there can only be single
 * outstanding transaction from the host at any given point in time. We use
 * this to simplify memory management in this driver - we cache and process
 * only one message at a time.
 *
 * While the request/response protocol is guaranteed by the host, we further
 * ensure this by serializing packet processing in this driver - we do not
 * read additional packets from the VMBUS until the current packet is fully
 * handled.
 */

static struct {
	int state;   /* hvutil_device_state */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int recv_len; /* number of bytes received. */
	struct hv_kvp_msg  *kvp_msg; /* current message */
	struct vmbus_channel *recv_channel; /* chn we got the request */
	u64 recv_req_id; /* request ID. */
<<<<<<< HEAD
	void *kvp_context; /* for the channel callback */
} kvp_transaction;

static void kvp_send_key(struct work_struct *dummy);

#define TIMEOUT_FIRED 1

static void kvp_respond_to_host(char *key, char *value, int error);
static void kvp_work_func(struct work_struct *dummy);
static void kvp_register(void);

static DECLARE_DELAYED_WORK(kvp_work, kvp_work_func);
static DECLARE_WORK(kvp_sendkey_work, kvp_send_key);

static struct cb_id kvp_id = { CN_KVP_IDX, CN_KVP_VAL };
static const char kvp_name[] = "kvp_kernel_module";
static u8 *recv_buffer;
/*
 * Register the kernel component with the user-level daemon.
 * As part of this registration, pass the LIC version number.
 */

static void
kvp_register(void)
{

	struct cn_msg *msg;
	struct hv_kvp_msg *kvp_msg;
	char *version;

	msg = kzalloc(sizeof(*msg) + sizeof(struct hv_kvp_msg), GFP_ATOMIC);

	if (msg) {
		kvp_msg = (struct hv_kvp_msg *)msg->data;
		version = kvp_msg->body.kvp_register.version;
		msg->id.idx =  CN_KVP_IDX;
		msg->id.val = CN_KVP_VAL;

		kvp_msg->kvp_hdr.operation = KVP_OP_REGISTER;
		strcpy(version, HV_DRV_VERSION);
		msg->len = sizeof(struct hv_kvp_msg);
		cn_netlink_send(msg, 0, GFP_ATOMIC);
		kfree(msg);
	}
}
static void
kvp_work_func(struct work_struct *dummy)
=======
} kvp_transaction;

/*
 * This state maintains the version number registered by the daemon.
 */
static int dm_reg_value;

static void kvp_send_key(struct work_struct *dummy);


static void kvp_respond_to_host(struct hv_kvp_msg *msg, int error);
static void kvp_timeout_func(struct work_struct *dummy);
static void kvp_host_handshake_func(struct work_struct *dummy);
static void kvp_register(int);

static DECLARE_DELAYED_WORK(kvp_timeout_work, kvp_timeout_func);
static DECLARE_DELAYED_WORK(kvp_host_handshake_work, kvp_host_handshake_func);
static DECLARE_WORK(kvp_sendkey_work, kvp_send_key);

static const char kvp_devname[] = "vmbus/hv_kvp";
static u8 *recv_buffer;
static struct hvutil_transport *hvt;
/*
 * Register the kernel component with the user-level daemon.
 * As part of this registration, pass the LIC version number.
 * This number has no meaning, it satisfies the registration protocol.
 */
#define HV_DRV_VERSION           "3.1"

static void kvp_poll_wrapper(void *channel)
{
	/* Transaction is finished, reset the state here to avoid races. */
	kvp_transaction.state = HVUTIL_READY;
	tasklet_schedule(&((struct vmbus_channel *)channel)->callback_event);
}

static void kvp_register_done(void)
{
	/*
	 * If we're still negotiating with the host cancel the timeout
	 * work to not poll the channel twice.
	 */
	pr_debug("KVP: userspace daemon registered\n");
	cancel_delayed_work_sync(&kvp_host_handshake_work);
	hv_poll_channel(kvp_transaction.recv_channel, kvp_poll_wrapper);
}

static void
kvp_register(int reg_value)
{

	struct hv_kvp_msg *kvp_msg;
	char *version;

	kvp_msg = kzalloc(sizeof(*kvp_msg), GFP_KERNEL);

	if (kvp_msg) {
		version = kvp_msg->body.kvp_register.version;
		kvp_msg->kvp_hdr.operation = reg_value;
		strcpy(version, HV_DRV_VERSION);

		hvutil_transport_send(hvt, kvp_msg, sizeof(*kvp_msg),
				      kvp_register_done);
		kfree(kvp_msg);
	}
}

static void kvp_timeout_func(struct work_struct *dummy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * If the timer fires, the user-mode component has not responded;
	 * process the pending transaction.
	 */
<<<<<<< HEAD
	kvp_respond_to_host("Unknown key", "Guest timed out", TIMEOUT_FIRED);
}

=======
	kvp_respond_to_host(NULL, HV_E_FAIL);

	hv_poll_channel(kvp_transaction.recv_channel, kvp_poll_wrapper);
}

static void kvp_host_handshake_func(struct work_struct *dummy)
{
	tasklet_schedule(&kvp_transaction.recv_channel->callback_event);
}

static int kvp_handle_handshake(struct hv_kvp_msg *msg)
{
	switch (msg->kvp_hdr.operation) {
	case KVP_OP_REGISTER:
		dm_reg_value = KVP_OP_REGISTER;
		pr_info("KVP: IP injection functionality not available\n");
		pr_info("KVP: Upgrade the KVP daemon\n");
		break;
	case KVP_OP_REGISTER1:
		dm_reg_value = KVP_OP_REGISTER1;
		break;
	default:
		pr_info("KVP: incompatible daemon\n");
		pr_info("KVP: KVP version: %d, Daemon version: %d\n",
			KVP_OP_REGISTER1, msg->kvp_hdr.operation);
		return -EINVAL;
	}

	/*
	 * We have a compatible daemon; complete the handshake.
	 */
	pr_debug("KVP: userspace daemon ver. %d connected\n",
		 msg->kvp_hdr.operation);
	kvp_register(dm_reg_value);

	return 0;
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Callback when data is received from user mode.
 */

<<<<<<< HEAD
static void
kvp_cn_callback(struct cn_msg *msg, struct netlink_skb_parms *nsp)
{
	struct hv_kvp_msg *message;
	struct hv_kvp_msg_enumerate *data;

	message = (struct hv_kvp_msg *)msg->data;
	switch (message->kvp_hdr.operation) {
	case KVP_OP_REGISTER:
		pr_info("KVP: user-mode registering done.\n");
		kvp_register();
		kvp_transaction.active = false;
		hv_kvp_onchannelcallback(kvp_transaction.kvp_context);
		break;

	default:
		data = &message->body.kvp_enum_data;
		/*
		 * Complete the transaction by forwarding the key value
		 * to the host. But first, cancel the timeout.
		 */
		if (cancel_delayed_work_sync(&kvp_work))
			kvp_respond_to_host(data->data.key,
					 data->data.value,
					!strlen(data->data.key));
	}
}

static void
kvp_send_key(struct work_struct *dummy)
{
	struct cn_msg *msg;
=======
static int kvp_on_msg(void *msg, int len)
{
	struct hv_kvp_msg *message = (struct hv_kvp_msg *)msg;
	struct hv_kvp_msg_enumerate *data;
	int	error = 0;

	if (len < sizeof(*message))
		return -EINVAL;

	/*
	 * If we are negotiating the version information
	 * with the daemon; handle that first.
	 */

	if (kvp_transaction.state < HVUTIL_READY) {
		return kvp_handle_handshake(message);
	}

	/* We didn't send anything to userspace so the reply is spurious */
	if (kvp_transaction.state < HVUTIL_USERSPACE_REQ)
		return -EINVAL;

	kvp_transaction.state = HVUTIL_USERSPACE_RECV;

	/*
	 * Based on the version of the daemon, we propagate errors from the
	 * daemon differently.
	 */

	data = &message->body.kvp_enum_data;

	switch (dm_reg_value) {
	case KVP_OP_REGISTER:
		/*
		 * Null string is used to pass back error condition.
		 */
		if (data->data.key[0] == 0)
			error = HV_S_CONT;
		break;

	case KVP_OP_REGISTER1:
		/*
		 * We use the message header information from
		 * the user level daemon to transmit errors.
		 */
		error = message->error;
		break;
	}

	/*
	 * Complete the transaction by forwarding the key value
	 * to the host. But first, cancel the timeout.
	 */
	if (cancel_delayed_work_sync(&kvp_timeout_work)) {
		kvp_respond_to_host(message, error);
		hv_poll_channel(kvp_transaction.recv_channel, kvp_poll_wrapper);
	}

	return 0;
}


static int process_ob_ipinfo(void *in_msg, void *out_msg, int op)
{
	struct hv_kvp_msg *in = in_msg;
	struct hv_kvp_ip_msg *out = out_msg;
	int len;

	switch (op) {
	case KVP_OP_GET_IP_INFO:
		/*
		 * Transform all parameters into utf16 encoding.
		 */
		len = utf8s_to_utf16s((char *)in->body.kvp_ip_val.ip_addr,
				strlen((char *)in->body.kvp_ip_val.ip_addr),
				UTF16_HOST_ENDIAN,
				(wchar_t *)out->kvp_ip_val.ip_addr,
				MAX_IP_ADDR_SIZE);
		if (len < 0)
			return len;

		len = utf8s_to_utf16s((char *)in->body.kvp_ip_val.sub_net,
				strlen((char *)in->body.kvp_ip_val.sub_net),
				UTF16_HOST_ENDIAN,
				(wchar_t *)out->kvp_ip_val.sub_net,
				MAX_IP_ADDR_SIZE);
		if (len < 0)
			return len;

		len = utf8s_to_utf16s((char *)in->body.kvp_ip_val.gate_way,
				strlen((char *)in->body.kvp_ip_val.gate_way),
				UTF16_HOST_ENDIAN,
				(wchar_t *)out->kvp_ip_val.gate_way,
				MAX_GATEWAY_SIZE);
		if (len < 0)
			return len;

		len = utf8s_to_utf16s((char *)in->body.kvp_ip_val.dns_addr,
				strlen((char *)in->body.kvp_ip_val.dns_addr),
				UTF16_HOST_ENDIAN,
				(wchar_t *)out->kvp_ip_val.dns_addr,
				MAX_IP_ADDR_SIZE);
		if (len < 0)
			return len;

		len = utf8s_to_utf16s((char *)in->body.kvp_ip_val.adapter_id,
				strlen((char *)in->body.kvp_ip_val.adapter_id),
				UTF16_HOST_ENDIAN,
				(wchar_t *)out->kvp_ip_val.adapter_id,
				MAX_ADAPTER_ID_SIZE);
		if (len < 0)
			return len;

		out->kvp_ip_val.dhcp_enabled =
			in->body.kvp_ip_val.dhcp_enabled;
		out->kvp_ip_val.addr_family =
			in->body.kvp_ip_val.addr_family;
	}

	return 0;
}

static void process_ib_ipinfo(void *in_msg, void *out_msg, int op)
{
	struct hv_kvp_ip_msg *in = in_msg;
	struct hv_kvp_msg *out = out_msg;

	switch (op) {
	case KVP_OP_SET_IP_INFO:
		/*
		 * Transform all parameters into utf8 encoding.
		 */
		utf16s_to_utf8s((wchar_t *)in->kvp_ip_val.ip_addr,
				MAX_IP_ADDR_SIZE,
				UTF16_LITTLE_ENDIAN,
				(__u8 *)out->body.kvp_ip_val.ip_addr,
				MAX_IP_ADDR_SIZE);

		utf16s_to_utf8s((wchar_t *)in->kvp_ip_val.sub_net,
				MAX_IP_ADDR_SIZE,
				UTF16_LITTLE_ENDIAN,
				(__u8 *)out->body.kvp_ip_val.sub_net,
				MAX_IP_ADDR_SIZE);

		utf16s_to_utf8s((wchar_t *)in->kvp_ip_val.gate_way,
				MAX_GATEWAY_SIZE,
				UTF16_LITTLE_ENDIAN,
				(__u8 *)out->body.kvp_ip_val.gate_way,
				MAX_GATEWAY_SIZE);

		utf16s_to_utf8s((wchar_t *)in->kvp_ip_val.dns_addr,
				MAX_IP_ADDR_SIZE,
				UTF16_LITTLE_ENDIAN,
				(__u8 *)out->body.kvp_ip_val.dns_addr,
				MAX_IP_ADDR_SIZE);

		out->body.kvp_ip_val.dhcp_enabled = in->kvp_ip_val.dhcp_enabled;

		fallthrough;

	case KVP_OP_GET_IP_INFO:
		utf16s_to_utf8s((wchar_t *)in->kvp_ip_val.adapter_id,
				MAX_ADAPTER_ID_SIZE,
				UTF16_LITTLE_ENDIAN,
				(__u8 *)out->body.kvp_ip_val.adapter_id,
				MAX_ADAPTER_ID_SIZE);

		out->body.kvp_ip_val.addr_family = in->kvp_ip_val.addr_family;
	}
}




static void
kvp_send_key(struct work_struct *dummy)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hv_kvp_msg *message;
	struct hv_kvp_msg *in_msg;
	__u8 operation = kvp_transaction.kvp_msg->kvp_hdr.operation;
	__u8 pool = kvp_transaction.kvp_msg->kvp_hdr.pool;
	__u32 val32;
	__u64 val64;
<<<<<<< HEAD

	msg = kzalloc(sizeof(*msg) + sizeof(struct hv_kvp_msg) , GFP_ATOMIC);
	if (!msg)
		return;

	msg->id.idx =  CN_KVP_IDX;
	msg->id.val = CN_KVP_VAL;

	message = (struct hv_kvp_msg *)msg->data;
=======
	int rc;

	/* The transaction state is wrong. */
	if (kvp_transaction.state != HVUTIL_HOSTMSG_RECEIVED)
		return;

	message = kzalloc(sizeof(*message), GFP_KERNEL);
	if (!message)
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	message->kvp_hdr.operation = operation;
	message->kvp_hdr.pool = pool;
	in_msg = kvp_transaction.kvp_msg;

	/*
<<<<<<< HEAD
	 * The key/value strings sent from the host are encoded in
=======
	 * The key/value strings sent from the host are encoded
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * in utf16; convert it to utf8 strings.
	 * The host assures us that the utf16 strings will not exceed
	 * the max lengths specified. We will however, reserve room
	 * for the string terminating character - in the utf16s_utf8s()
	 * function we limit the size of the buffer where the converted
<<<<<<< HEAD
	 * string is placed to HV_KVP_EXCHANGE_MAX_*_SIZE -1 to gaurantee
=======
	 * string is placed to HV_KVP_EXCHANGE_MAX_*_SIZE -1 to guarantee
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * that the strings can be properly terminated!
	 */

	switch (message->kvp_hdr.operation) {
<<<<<<< HEAD
=======
	case KVP_OP_SET_IP_INFO:
		process_ib_ipinfo(in_msg, message, KVP_OP_SET_IP_INFO);
		break;
	case KVP_OP_GET_IP_INFO:
		/*
		 * We only need to pass on the info of operation, adapter_id
		 * and addr_family to the userland kvp daemon.
		 */
		process_ib_ipinfo(in_msg, message, KVP_OP_GET_IP_INFO);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case KVP_OP_SET:
		switch (in_msg->body.kvp_set.data.value_type) {
		case REG_SZ:
			/*
			 * The value is a string - utf16 encoding.
			 */
			message->body.kvp_set.data.value_size =
				utf16s_to_utf8s(
				(wchar_t *)in_msg->body.kvp_set.data.value,
				in_msg->body.kvp_set.data.value_size,
				UTF16_LITTLE_ENDIAN,
				message->body.kvp_set.data.value,
				HV_KVP_EXCHANGE_MAX_VALUE_SIZE - 1) + 1;
<<<<<<< HEAD
				break;
=======
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		case REG_U32:
			/*
			 * The value is a 32 bit scalar.
			 * We save this as a utf8 string.
			 */
			val32 = in_msg->body.kvp_set.data.value_u32;
			message->body.kvp_set.data.value_size =
				sprintf(message->body.kvp_set.data.value,
<<<<<<< HEAD
					"%d", val32) + 1;
=======
					"%u", val32) + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		case REG_U64:
			/*
			 * The value is a 64 bit scalar.
			 * We save this as a utf8 string.
			 */
			val64 = in_msg->body.kvp_set.data.value_u64;
			message->body.kvp_set.data.value_size =
				sprintf(message->body.kvp_set.data.value,
					"%llu", val64) + 1;
			break;

		}
<<<<<<< HEAD
	case KVP_OP_GET:
=======

		/*
		 * The key is always a string - utf16 encoding.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		message->body.kvp_set.data.key_size =
			utf16s_to_utf8s(
			(wchar_t *)in_msg->body.kvp_set.data.key,
			in_msg->body.kvp_set.data.key_size,
			UTF16_LITTLE_ENDIAN,
			message->body.kvp_set.data.key,
			HV_KVP_EXCHANGE_MAX_KEY_SIZE - 1) + 1;
<<<<<<< HEAD
			break;
=======

		break;

	case KVP_OP_GET:
		message->body.kvp_get.data.key_size =
			utf16s_to_utf8s(
			(wchar_t *)in_msg->body.kvp_get.data.key,
			in_msg->body.kvp_get.data.key_size,
			UTF16_LITTLE_ENDIAN,
			message->body.kvp_get.data.key,
			HV_KVP_EXCHANGE_MAX_KEY_SIZE - 1) + 1;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case KVP_OP_DELETE:
		message->body.kvp_delete.key_size =
			utf16s_to_utf8s(
			(wchar_t *)in_msg->body.kvp_delete.key,
			in_msg->body.kvp_delete.key_size,
			UTF16_LITTLE_ENDIAN,
			message->body.kvp_delete.key,
			HV_KVP_EXCHANGE_MAX_KEY_SIZE - 1) + 1;
<<<<<<< HEAD
			break;
=======
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case KVP_OP_ENUMERATE:
		message->body.kvp_enum_data.index =
			in_msg->body.kvp_enum_data.index;
<<<<<<< HEAD
			break;
	}

	msg->len = sizeof(struct hv_kvp_msg);
	cn_netlink_send(msg, 0, GFP_ATOMIC);
	kfree(msg);

	return;
=======
		break;
	}

	kvp_transaction.state = HVUTIL_USERSPACE_REQ;
	rc = hvutil_transport_send(hvt, message, sizeof(*message), NULL);
	if (rc) {
		pr_debug("KVP: failed to communicate to the daemon: %d\n", rc);
		if (cancel_delayed_work_sync(&kvp_timeout_work)) {
			kvp_respond_to_host(message, HV_E_FAIL);
			kvp_transaction.state = HVUTIL_READY;
		}
	}

	kfree(message);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Send a response back to the host.
 */

static void
<<<<<<< HEAD
kvp_respond_to_host(char *key, char *value, int error)
=======
kvp_respond_to_host(struct hv_kvp_msg *msg_to_host, int error)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hv_kvp_msg  *kvp_msg;
	struct hv_kvp_exchg_msg_value  *kvp_data;
	char	*key_name;
<<<<<<< HEAD
=======
	char	*value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct icmsg_hdr *icmsghdrp;
	int	keylen = 0;
	int	valuelen = 0;
	u32	buf_len;
	struct vmbus_channel *channel;
	u64	req_id;
<<<<<<< HEAD

	/*
	 * If a transaction is not active; log and return.
	 */

	if (!kvp_transaction.active) {
		/*
		 * This is a spurious call!
		 */
		pr_warn("KVP: Transaction not active\n");
		return;
	}
	/*
=======
	int ret;

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Copy the global state for completing the transaction. Note that
	 * only one transaction can be active at a time.
	 */

	buf_len = kvp_transaction.recv_len;
	channel = kvp_transaction.recv_channel;
	req_id = kvp_transaction.recv_req_id;

<<<<<<< HEAD
	kvp_transaction.active = false;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	icmsghdrp = (struct icmsg_hdr *)
			&recv_buffer[sizeof(struct vmbuspipe_hdr)];

	if (channel->onchannel_callback == NULL)
		/*
		 * We have raced with util driver being unloaded;
		 * silently return.
		 */
		return;

<<<<<<< HEAD
=======
	icmsghdrp->status = error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If the error parameter is set, terminate the host's enumeration
	 * on this pool.
	 */
	if (error) {
		/*
<<<<<<< HEAD
		 * Something failed or the we have timedout;
		 * terminate the current  host-side iteration.
		 */
		icmsghdrp->status = HV_S_CONT;
		goto response_done;
	}

	icmsghdrp->status = HV_S_OK;

=======
		 * Something failed or we have timed out;
		 * terminate the current host-side iteration.
		 */
		goto response_done;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kvp_msg = (struct hv_kvp_msg *)
			&recv_buffer[sizeof(struct vmbuspipe_hdr) +
			sizeof(struct icmsg_hdr)];

	switch (kvp_transaction.kvp_msg->kvp_hdr.operation) {
<<<<<<< HEAD
=======
	case KVP_OP_GET_IP_INFO:
		ret = process_ob_ipinfo(msg_to_host,
				 (struct hv_kvp_ip_msg *)kvp_msg,
				 KVP_OP_GET_IP_INFO);
		if (ret < 0)
			icmsghdrp->status = HV_E_FAIL;

		goto response_done;
	case KVP_OP_SET_IP_INFO:
		goto response_done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case KVP_OP_GET:
		kvp_data = &kvp_msg->body.kvp_get.data;
		goto copy_value;

	case KVP_OP_SET:
	case KVP_OP_DELETE:
		goto response_done;

	default:
		break;
	}

	kvp_data = &kvp_msg->body.kvp_enum_data.data;
<<<<<<< HEAD
	key_name = key;
=======
	key_name = msg_to_host->body.kvp_enum_data.data.key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The windows host expects the key/value pair to be encoded
	 * in utf16. Ensure that the key/value size reported to the host
	 * will be less than or equal to the MAX size (including the
	 * terminating character).
	 */
	keylen = utf8s_to_utf16s(key_name, strlen(key_name), UTF16_HOST_ENDIAN,
				(wchar_t *) kvp_data->key,
				(HV_KVP_EXCHANGE_MAX_KEY_SIZE / 2) - 2);
	kvp_data->key_size = 2*(keylen + 1); /* utf16 encoding */

copy_value:
<<<<<<< HEAD
=======
	value = msg_to_host->body.kvp_enum_data.data.value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	valuelen = utf8s_to_utf16s(value, strlen(value), UTF16_HOST_ENDIAN,
				(wchar_t *) kvp_data->value,
				(HV_KVP_EXCHANGE_MAX_VALUE_SIZE / 2) - 2);
	kvp_data->value_size = 2*(valuelen + 1); /* utf16 encoding */

	/*
	 * If the utf8s to utf16s conversion failed; notify host
	 * of the error.
	 */
	if ((keylen < 0) || (valuelen < 0))
		icmsghdrp->status = HV_E_FAIL;

	kvp_data->value_type = REG_SZ; /* all our values are strings */

response_done:
	icmsghdrp->icflags = ICMSGHDRFLAG_TRANSACTION | ICMSGHDRFLAG_RESPONSE;

	vmbus_sendpacket(channel, recv_buffer, buf_len, req_id,
				VM_PKT_DATA_INBAND, 0);
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This callback is invoked when we get a KVP message from the host.
 * The host ensures that only one KVP transaction can be active at a time.
 * KVP implementation in Linux needs to forward the key to a user-mde
<<<<<<< HEAD
 * component to retrive the corresponding value. Consequently, we cannot
 * respond to the host in the conext of this callback. Since the host
=======
 * component to retrieve the corresponding value. Consequently, we cannot
 * respond to the host in the context of this callback. Since the host
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * guarantees that at most only one transaction can be active at a time,
 * we stash away the transaction state in a set of global variables.
 */

void hv_kvp_onchannelcallback(void *context)
{
	struct vmbus_channel *channel = context;
	u32 recvlen;
	u64 requestid;

	struct hv_kvp_msg *kvp_msg;

	struct icmsg_hdr *icmsghdrp;
<<<<<<< HEAD
	struct icmsg_negotiate *negop = NULL;

	if (kvp_transaction.active) {
		/*
		 * We will defer processing this callback once
		 * the current transaction is complete.
		 */
		kvp_transaction.kvp_context = context;
		return;
	}

	vmbus_recvpacket(channel, recv_buffer, PAGE_SIZE, &recvlen, &requestid);

	if (recvlen > 0) {
		icmsghdrp = (struct icmsg_hdr *)&recv_buffer[
			sizeof(struct vmbuspipe_hdr)];

		if (icmsghdrp->icmsgtype == ICMSGTYPE_NEGOTIATE) {
			vmbus_prep_negotiate_resp(icmsghdrp, negop, recv_buffer);
		} else {
			kvp_msg = (struct hv_kvp_msg *)&recv_buffer[
				sizeof(struct vmbuspipe_hdr) +
				sizeof(struct icmsg_hdr)];

			/*
			 * Stash away this global state for completing the
			 * transaction; note transactions are serialized.
			 */

			kvp_transaction.recv_len = recvlen;
			kvp_transaction.recv_channel = channel;
			kvp_transaction.recv_req_id = requestid;
			kvp_transaction.active = true;
			kvp_transaction.kvp_msg = kvp_msg;

			/*
			 * Get the information from the
			 * user-mode component.
			 * component. This transaction will be
			 * completed when we get the value from
			 * the user-mode component.
			 * Set a timeout to deal with
			 * user-mode not responding.
			 */
			schedule_work(&kvp_sendkey_work);
			schedule_delayed_work(&kvp_work, 5*HZ);

			return;

		}

		icmsghdrp->icflags = ICMSGHDRFLAG_TRANSACTION
			| ICMSGHDRFLAG_RESPONSE;

		vmbus_sendpacket(channel, recv_buffer,
				       recvlen, requestid,
				       VM_PKT_DATA_INBAND, 0);
	}

=======
	int kvp_srv_version;
	static enum {NEGO_NOT_STARTED,
		     NEGO_IN_PROGRESS,
		     NEGO_FINISHED} host_negotiatied = NEGO_NOT_STARTED;

	if (kvp_transaction.state < HVUTIL_READY) {
		/*
		 * If userspace daemon is not connected and host is asking
		 * us to negotiate we need to delay to not lose messages.
		 * This is important for Failover IP setting.
		 */
		if (host_negotiatied == NEGO_NOT_STARTED) {
			host_negotiatied = NEGO_IN_PROGRESS;
			schedule_delayed_work(&kvp_host_handshake_work,
				      HV_UTIL_NEGO_TIMEOUT * HZ);
		}
		return;
	}
	if (kvp_transaction.state > HVUTIL_READY)
		return;

	if (vmbus_recvpacket(channel, recv_buffer, HV_HYP_PAGE_SIZE * 4, &recvlen, &requestid)) {
		pr_err_ratelimited("KVP request received. Could not read into recv buf\n");
		return;
	}

	if (!recvlen)
		return;

	/* Ensure recvlen is big enough to read header data */
	if (recvlen < ICMSG_HDR) {
		pr_err_ratelimited("KVP request received. Packet length too small: %d\n",
				   recvlen);
		return;
	}

	icmsghdrp = (struct icmsg_hdr *)&recv_buffer[sizeof(struct vmbuspipe_hdr)];

	if (icmsghdrp->icmsgtype == ICMSGTYPE_NEGOTIATE) {
		if (vmbus_prep_negotiate_resp(icmsghdrp,
				recv_buffer, recvlen,
				fw_versions, FW_VER_COUNT,
				kvp_versions, KVP_VER_COUNT,
				NULL, &kvp_srv_version)) {
			pr_info("KVP IC version %d.%d\n",
				kvp_srv_version >> 16,
				kvp_srv_version & 0xFFFF);
		}
	} else if (icmsghdrp->icmsgtype == ICMSGTYPE_KVPEXCHANGE) {
		/*
		 * recvlen is not checked against sizeof(struct kvp_msg) because kvp_msg contains
		 * a union of structs and the msg type received is not known. Code using this
		 * struct should provide validation when accessing its fields.
		 */
		kvp_msg = (struct hv_kvp_msg *)&recv_buffer[ICMSG_HDR];

		/*
		 * Stash away this global state for completing the
		 * transaction; note transactions are serialized.
		 */

		kvp_transaction.recv_len = recvlen;
		kvp_transaction.recv_req_id = requestid;
		kvp_transaction.kvp_msg = kvp_msg;

		if (kvp_transaction.state < HVUTIL_READY) {
			/* Userspace is not registered yet */
			kvp_respond_to_host(NULL, HV_E_FAIL);
			return;
		}
		kvp_transaction.state = HVUTIL_HOSTMSG_RECEIVED;

		/*
		 * Get the information from the
		 * user-mode component.
		 * component. This transaction will be
		 * completed when we get the value from
		 * the user-mode component.
		 * Set a timeout to deal with
		 * user-mode not responding.
		 */
		schedule_work(&kvp_sendkey_work);
		schedule_delayed_work(&kvp_timeout_work,
					HV_UTIL_TIMEOUT * HZ);

		return;

	} else {
		pr_err_ratelimited("KVP request received. Invalid msg type: %d\n",
				   icmsghdrp->icmsgtype);
		return;
	}

	icmsghdrp->icflags = ICMSGHDRFLAG_TRANSACTION
		| ICMSGHDRFLAG_RESPONSE;

	vmbus_sendpacket(channel, recv_buffer,
			 recvlen, requestid,
			 VM_PKT_DATA_INBAND, 0);

	host_negotiatied = NEGO_FINISHED;
	hv_poll_channel(kvp_transaction.recv_channel, kvp_poll_wrapper);
}

static void kvp_on_reset(void)
{
	if (cancel_delayed_work_sync(&kvp_timeout_work))
		kvp_respond_to_host(NULL, HV_E_FAIL);
	kvp_transaction.state = HVUTIL_DEVICE_INIT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
hv_kvp_init(struct hv_util_service *srv)
{
<<<<<<< HEAD
	int err;

	err = cn_add_callback(&kvp_id, kvp_name, kvp_cn_callback);
	if (err)
		return err;
	recv_buffer = srv->recv_buffer;
=======
	recv_buffer = srv->recv_buffer;
	kvp_transaction.recv_channel = srv->channel;
	kvp_transaction.recv_channel->max_pkt_size = HV_HYP_PAGE_SIZE * 4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * When this driver loads, the user level daemon that
	 * processes the host requests may not yet be running.
	 * Defer processing channel callbacks until the daemon
	 * has registered.
	 */
<<<<<<< HEAD
	kvp_transaction.active = true;
=======
	kvp_transaction.state = HVUTIL_DEVICE_INIT;

	hvt = hvutil_transport_init(kvp_devname, CN_KVP_IDX, CN_KVP_VAL,
				    kvp_on_msg, kvp_on_reset);
	if (!hvt)
		return -EFAULT;

	return 0;
}

static void hv_kvp_cancel_work(void)
{
	cancel_delayed_work_sync(&kvp_host_handshake_work);
	cancel_delayed_work_sync(&kvp_timeout_work);
	cancel_work_sync(&kvp_sendkey_work);
}

int hv_kvp_pre_suspend(void)
{
	struct vmbus_channel *channel = kvp_transaction.recv_channel;

	tasklet_disable(&channel->callback_event);

	/*
	 * If there is a pending transtion, it's unnecessary to tell the host
	 * that the transaction will fail, because that is implied when
	 * util_suspend() calls vmbus_close() later.
	 */
	hv_kvp_cancel_work();

	/*
	 * Forece the state to READY to handle the ICMSGTYPE_NEGOTIATE message
	 * later. The user space daemon may go out of order and its write()
	 * may fail with EINVAL: this doesn't matter since the daemon will
	 * reset the device by closing and re-opening it.
	 */
	kvp_transaction.state = HVUTIL_READY;
	return 0;
}

int hv_kvp_pre_resume(void)
{
	struct vmbus_channel *channel = kvp_transaction.recv_channel;

	tasklet_enable(&channel->callback_event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

void hv_kvp_deinit(void)
{
<<<<<<< HEAD
	cn_del_callback(&kvp_id);
	cancel_delayed_work_sync(&kvp_work);
	cancel_work_sync(&kvp_sendkey_work);
=======
	kvp_transaction.state = HVUTIL_DEVICE_DYING;

	hv_kvp_cancel_work();

	hvutil_transport_destroy(hvt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
