<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ipmi_smi.h
 *
 * MontaVista IPMI system management interface
 *
 * Author: MontaVista Software, Inc.
 *         Corey Minyard <minyard@mvista.com>
 *         source@mvista.com
 *
 * Copyright 2002 MontaVista Software Inc.
 *
<<<<<<< HEAD
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_IPMI_SMI_H
#define __LINUX_IPMI_SMI_H

#include <linux/ipmi_msgdefs.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/ipmi.h>

struct device;

<<<<<<< HEAD
/* This files describes the interface for IPMI system management interface
   drivers to bind into the IPMI message handler. */

/* Structure for the low-level drivers. */
typedef struct ipmi_smi *ipmi_smi_t;
=======
/*
 * This files describes the interface for IPMI system management interface
 * drivers to bind into the IPMI message handler.
 */

/* Structure for the low-level drivers. */
struct ipmi_smi;

/*
 * Flags for set_check_watch() below.  Tells if the SMI should be
 * waiting for watchdog timeouts, commands and/or messages.
 */
#define IPMI_WATCH_MASK_CHECK_MESSAGES	(1 << 0)
#define IPMI_WATCH_MASK_CHECK_WATCHDOG	(1 << 1)
#define IPMI_WATCH_MASK_CHECK_COMMANDS	(1 << 2)

/*
 * SMI messages
 *
 * When communicating with an SMI, messages come in two formats:
 *
 * * Normal (to a BMC over a BMC interface)
 *
 * * IPMB (over a IPMB to another MC)
 *
 * When normal, commands are sent using the format defined by a
 * standard message over KCS (NetFn must be even):
 *
 *   +-----------+-----+------+
 *   | NetFn/LUN | Cmd | Data |
 *   +-----------+-----+------+
 *
 * And responses, similarly, with an completion code added (NetFn must
 * be odd):
 *
 *   +-----------+-----+------+------+
 *   | NetFn/LUN | Cmd | CC   | Data |
 *   +-----------+-----+------+------+
 *
 * With normal messages, only commands are sent and only responses are
 * received.
 *
 * In IPMB mode, we are acting as an IPMB device. Commands will be in
 * the following format (NetFn must be even):
 *
 *   +-------------+------+-------------+-----+------+
 *   | NetFn/rsLUN | Addr | rqSeq/rqLUN | Cmd | Data |
 *   +-------------+------+-------------+-----+------+
 *
 * Responses will using the following format:
 *
 *   +-------------+------+-------------+-----+------+------+
 *   | NetFn/rqLUN | Addr | rqSeq/rsLUN | Cmd | CC   | Data |
 *   +-------------+------+-------------+-----+------+------+
 *
 * This is similar to the format defined in the IPMB manual section
 * 2.11.1 with the checksums and the first address removed.  Also, the
 * address is always the remote address.
 *
 * IPMB messages can be commands and responses in both directions.
 * Received commands are handled as received commands from the message
 * queue.
 */

enum ipmi_smi_msg_type {
	IPMI_SMI_MSG_TYPE_NORMAL = 0,
	IPMI_SMI_MSG_TYPE_IPMB_DIRECT
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Messages to/from the lower layer.  The smi interface will take one
 * of these to send. After the send has occurred and a response has
 * been received, it will report this same data structure back up to
 * the upper layer.  If an error occurs, it should fill in the
 * response with an error code in the completion code location. When
 * asynchronous data is received, one of these is allocated, the
 * data_size is set to zero and the response holds the data from the
 * get message or get event command that the interface initiated.
 * Note that it is the interfaces responsibility to detect
 * asynchronous data and messages and request them from the
 * interface.
 */
struct ipmi_smi_msg {
	struct list_head link;

<<<<<<< HEAD
=======
	enum ipmi_smi_msg_type type;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	long    msgid;
	void    *user_data;

	int           data_size;
	unsigned char data[IPMI_MAX_MSG_LENGTH];

	int           rsp_size;
	unsigned char rsp[IPMI_MAX_MSG_LENGTH];

<<<<<<< HEAD
	/* Will be called when the system is done with the message
	   (presumably to free it). */
	void (*done)(struct ipmi_smi_msg *msg);
};

struct ipmi_smi_handlers {
	struct module *owner;

	/* The low-level interface cannot start sending messages to
	   the upper layer until this function is called.  This may
	   not be NULL, the lower layer must take the interface from
	   this call. */
	int (*start_processing)(void       *send_info,
				ipmi_smi_t new_intf);
=======
	/*
	 * Will be called when the system is done with the message
	 * (presumably to free it).
	 */
	void (*done)(struct ipmi_smi_msg *msg);
};

#define INIT_IPMI_SMI_MSG(done_handler) \
{						\
	.done = done_handler,			\
	.type = IPMI_SMI_MSG_TYPE_NORMAL	\
}

struct ipmi_smi_handlers {
	struct module *owner;

	/* Capabilities of the SMI. */
#define IPMI_SMI_CAN_HANDLE_IPMB_DIRECT		(1 << 0)
	unsigned int flags;

	/*
	 * The low-level interface cannot start sending messages to
	 * the upper layer until this function is called.  This may
	 * not be NULL, the lower layer must take the interface from
	 * this call.
	 */
	int (*start_processing)(void            *send_info,
				struct ipmi_smi *new_intf);

	/*
	 * When called, the low-level interface should disable all
	 * processing, it should be complete shut down when it returns.
	 */
	void (*shutdown)(void *send_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Get the detailed private info of the low level interface and store
	 * it into the structure of ipmi_smi_data. For example: the
	 * ACPI device handle will be returned for the pnp_acpi IPMI device.
	 */
	int (*get_smi_info)(void *send_info, struct ipmi_smi_info *data);

<<<<<<< HEAD
	/* Called to enqueue an SMI message to be sent.  This
	   operation is not allowed to fail.  If an error occurs, it
	   should report back the error in a received message.  It may
	   do this in the current call context, since no write locks
	   are held when this is run.  If the priority is > 0, the
	   message will go into a high-priority queue and be sent
	   first.  Otherwise, it goes into a normal-priority queue. */
	void (*sender)(void                *send_info,
		       struct ipmi_smi_msg *msg,
		       int                 priority);

	/* Called by the upper layer to request that we try to get
	   events from the BMC we are attached to. */
	void (*request_events)(void *send_info);

	/* Called when the interface should go into "run to
	   completion" mode.  If this call sets the value to true, the
	   interface should make sure that all messages are flushed
	   out and that none are pending, and any new requests are run
	   to completion immediately. */
	void (*set_run_to_completion)(void *send_info, int run_to_completion);

	/* Called to poll for work to do.  This is so upper layers can
	   poll for operations during things like crash dumps. */
	void (*poll)(void *send_info);

	/* Enable/disable firmware maintenance mode.  Note that this
	   is *not* the modes defined, this is simply an on/off
	   setting.  The message handler does the mode handling.  Note
	   that this is called from interrupt context, so it cannot
	   block. */
	void (*set_maintenance_mode)(void *send_info, int enable);

	/* Tell the handler that we are using it/not using it.  The
	   message handler get the modules that this handler belongs
	   to; this function lets the SMI claim any modules that it
	   uses.  These may be NULL if this is not required. */
	int (*inc_usecount)(void *send_info);
	void (*dec_usecount)(void *send_info);
=======
	/*
	 * Called to enqueue an SMI message to be sent.  This
	 * operation is not allowed to fail.  If an error occurs, it
	 * should report back the error in a received message.  It may
	 * do this in the current call context, since no write locks
	 * are held when this is run.  Message are delivered one at
	 * a time by the message handler, a new message will not be
	 * delivered until the previous message is returned.
	 */
	void (*sender)(void                *send_info,
		       struct ipmi_smi_msg *msg);

	/*
	 * Called by the upper layer to request that we try to get
	 * events from the BMC we are attached to.
	 */
	void (*request_events)(void *send_info);

	/*
	 * Called by the upper layer when some user requires that the
	 * interface watch for received messages and watchdog
	 * pretimeouts (basically do a "Get Flags", or not.  Used by
	 * the SMI to know if it should watch for these.  This may be
	 * NULL if the SMI does not implement it.  watch_mask is from
	 * IPMI_WATCH_MASK_xxx above.  The interface should run slower
	 * timeouts for just watchdog checking or faster timeouts when
	 * waiting for the message queue.
	 */
	void (*set_need_watch)(void *send_info, unsigned int watch_mask);

	/*
	 * Called when flushing all pending messages.
	 */
	void (*flush_messages)(void *send_info);

	/*
	 * Called when the interface should go into "run to
	 * completion" mode.  If this call sets the value to true, the
	 * interface should make sure that all messages are flushed
	 * out and that none are pending, and any new requests are run
	 * to completion immediately.
	 */
	void (*set_run_to_completion)(void *send_info, bool run_to_completion);

	/*
	 * Called to poll for work to do.  This is so upper layers can
	 * poll for operations during things like crash dumps.
	 */
	void (*poll)(void *send_info);

	/*
	 * Enable/disable firmware maintenance mode.  Note that this
	 * is *not* the modes defined, this is simply an on/off
	 * setting.  The message handler does the mode handling.  Note
	 * that this is called from interrupt context, so it cannot
	 * block.
	 */
	void (*set_maintenance_mode)(void *send_info, bool enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ipmi_device_id {
	unsigned char device_id;
	unsigned char device_revision;
	unsigned char firmware_revision_1;
	unsigned char firmware_revision_2;
	unsigned char ipmi_version;
	unsigned char additional_device_support;
	unsigned int  manufacturer_id;
	unsigned int  product_id;
	unsigned char aux_firmware_revision[4];
	unsigned int  aux_firmware_revision_set : 1;
};

#define ipmi_version_major(v) ((v)->ipmi_version & 0xf)
#define ipmi_version_minor(v) ((v)->ipmi_version >> 4)

<<<<<<< HEAD
/* Take a pointer to a raw data buffer and a length and extract device
   id information from it.  The first byte of data must point to the
   netfn << 2, the data should be of the format:
      netfn << 2, cmd, completion code, data
   as normally comes from a device interface. */
static inline int ipmi_demangle_device_id(const unsigned char *data,
					  unsigned int data_len,
					  struct ipmi_device_id *id)
{
	if (data_len < 9)
		return -EINVAL;
	if (data[0] != IPMI_NETFN_APP_RESPONSE << 2 ||
	    data[1] != IPMI_GET_DEVICE_ID_CMD)
		/* Strange, didn't get the response we expected. */
		return -EINVAL;
	if (data[2] != 0)
		/* That's odd, it shouldn't be able to fail. */
		return -EINVAL;

	data += 3;
	data_len -= 3;
=======
/*
 * Take a pointer to an IPMI response and extract device id information from
 * it. @netfn is in the IPMI_NETFN_ format, so may need to be shifted from
 * a SI response.
 */
static inline int ipmi_demangle_device_id(uint8_t netfn, uint8_t cmd,
					  const unsigned char *data,
					  unsigned int data_len,
					  struct ipmi_device_id *id)
{
	if (data_len < 7)
		return -EINVAL;
	if (netfn != IPMI_NETFN_APP_RESPONSE || cmd != IPMI_GET_DEVICE_ID_CMD)
		/* Strange, didn't get the response we expected. */
		return -EINVAL;
	if (data[0] != 0)
		/* That's odd, it shouldn't be able to fail. */
		return -EINVAL;

	data++;
	data_len--;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	id->device_id = data[0];
	id->device_revision = data[1];
	id->firmware_revision_1 = data[2];
	id->firmware_revision_2 = data[3];
	id->ipmi_version = data[4];
	id->additional_device_support = data[5];
	if (data_len >= 11) {
		id->manufacturer_id = (data[6] | (data[7] << 8) |
				       (data[8] << 16));
		id->product_id = data[9] | (data[10] << 8);
	} else {
		id->manufacturer_id = 0;
		id->product_id = 0;
	}
	if (data_len >= 15) {
		memcpy(id->aux_firmware_revision, data+11, 4);
		id->aux_firmware_revision_set = 1;
	} else
		id->aux_firmware_revision_set = 0;

	return 0;
}

<<<<<<< HEAD
/* Add a low-level interface to the IPMI driver.  Note that if the
   interface doesn't know its slave address, it should pass in zero.
   The low-level interface should not deliver any messages to the
   upper layer until the start_processing() function in the handlers
   is called, and the lower layer must get the interface from that
   call. */
int ipmi_register_smi(struct ipmi_smi_handlers *handlers,
		      void                     *send_info,
		      struct ipmi_device_id    *device_id,
		      struct device            *dev,
		      const char               *sysfs_name,
		      unsigned char            slave_addr);
=======
/*
 * Add a low-level interface to the IPMI driver.  Note that if the
 * interface doesn't know its slave address, it should pass in zero.
 * The low-level interface should not deliver any messages to the
 * upper layer until the start_processing() function in the handlers
 * is called, and the lower layer must get the interface from that
 * call.
 */
int ipmi_add_smi(struct module            *owner,
		 const struct ipmi_smi_handlers *handlers,
		 void                     *send_info,
		 struct device            *dev,
		 unsigned char            slave_addr);

#define ipmi_register_smi(handlers, send_info, dev, slave_addr) \
	ipmi_add_smi(THIS_MODULE, handlers, send_info, dev, slave_addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Remove a low-level interface from the IPMI driver.  This will
 * return an error if the interface is still in use by a user.
 */
<<<<<<< HEAD
int ipmi_unregister_smi(ipmi_smi_t intf);

/*
 * The lower layer reports received messages through this interface.
 * The data_size should be zero if this is an asyncronous message.  If
 * the lower layer gets an error sending a message, it should format
 * an error response in the message response.
 */
void ipmi_smi_msg_received(ipmi_smi_t          intf,
			   struct ipmi_smi_msg *msg);

/* The lower layer received a watchdog pre-timeout on interface. */
void ipmi_smi_watchdog_pretimeout(ipmi_smi_t intf);
=======
void ipmi_unregister_smi(struct ipmi_smi *intf);

/*
 * The lower layer reports received messages through this interface.
 * The data_size should be zero if this is an asynchronous message.  If
 * the lower layer gets an error sending a message, it should format
 * an error response in the message response.
 */
void ipmi_smi_msg_received(struct ipmi_smi     *intf,
			   struct ipmi_smi_msg *msg);

/* The lower layer received a watchdog pre-timeout on interface. */
void ipmi_smi_watchdog_pretimeout(struct ipmi_smi *intf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct ipmi_smi_msg *ipmi_alloc_smi_msg(void);
static inline void ipmi_free_smi_msg(struct ipmi_smi_msg *msg)
{
	msg->done(msg);
}

<<<<<<< HEAD
/* Allow the lower layer to add things to the proc filesystem
   directory for this interface.  Note that the entry will
   automatically be dstroyed when the interface is destroyed. */
int ipmi_smi_add_proc_entry(ipmi_smi_t smi, char *name,
			    const struct file_operations *proc_ops,
			    void *data);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_IPMI_SMI_H */
