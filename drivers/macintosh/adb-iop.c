<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * I/O Processor (IOP) ADB Driver
 * Written and (C) 1999 by Joshua M. Thompson (funaho@jurai.org)
 * Based on via-cuda.c by Paul Mackerras.
 *
 * 1999-07-01 (jmt) - First implementation for new driver architecture.
 *
 * 1999-07-31 (jmt) - First working version.
<<<<<<< HEAD
 *
 * TODO:
 *
 * o Implement SRQ handling.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>

#include <asm/macintosh.h> 
#include <asm/macints.h> 
#include <asm/mac_iop.h>
#include <asm/mac_oss.h>
#include <asm/adb_iop.h>

#include <linux/adb.h> 

/*#define DEBUG_ADB_IOP*/

extern void iop_ism_irq(int, void *);

static struct adb_request *current_req;
static struct adb_request *last_req;
#if 0
static unsigned char reply_buff[16];
static unsigned char *reply_ptr;
#endif

static enum adb_iop_state {
    idle,
    sending,
    awaiting_reply
=======

#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_iop.h>
#include <asm/adb_iop.h>
#include <asm/unaligned.h>

#include <linux/adb.h>

static struct adb_request *current_req;
static struct adb_request *last_req;
static unsigned int autopoll_devs;
static u8 autopoll_addr;

static enum adb_iop_state {
	idle,
	sending,
	awaiting_reply
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} adb_iop_state;

static void adb_iop_start(void);
static int adb_iop_probe(void);
static int adb_iop_init(void);
static int adb_iop_send_request(struct adb_request *, int);
static int adb_iop_write(struct adb_request *);
static int adb_iop_autopoll(int);
static void adb_iop_poll(void);
static int adb_iop_reset_bus(void);

<<<<<<< HEAD
struct adb_driver adb_iop_driver = {
	"ISM IOP",
	adb_iop_probe,
	adb_iop_init,
	adb_iop_send_request,
	adb_iop_autopoll,
	adb_iop_poll,
	adb_iop_reset_bus
};

static void adb_iop_end_req(struct adb_request *req, int state)
{
	req->complete = 1;
	current_req = req->next;
	if (req->done) (*req->done)(req);
	adb_iop_state = state;
=======
/* ADB command byte structure */
#define ADDR_MASK       0xF0
#define OP_MASK         0x0C
#define TALK            0x0C

struct adb_driver adb_iop_driver = {
	.name         = "ISM IOP",
	.probe        = adb_iop_probe,
	.init         = adb_iop_init,
	.send_request = adb_iop_send_request,
	.autopoll     = adb_iop_autopoll,
	.poll         = adb_iop_poll,
	.reset_bus    = adb_iop_reset_bus
};

static void adb_iop_done(void)
{
	struct adb_request *req = current_req;

	adb_iop_state = idle;

	req->complete = 1;
	current_req = req->next;
	if (req->done)
		(*req->done)(req);

	if (adb_iop_state == idle)
		adb_iop_start();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Completion routine for ADB commands sent to the IOP.
 *
 * This will be called when a packet has been successfully sent.
 */

static void adb_iop_complete(struct iop_msg *msg)
{
<<<<<<< HEAD
	struct adb_request *req;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;

	local_irq_save(flags);

<<<<<<< HEAD
	req = current_req;
	if ((adb_iop_state == sending) && req && req->reply_expected) {
		adb_iop_state = awaiting_reply;
	}
=======
	adb_iop_state = awaiting_reply;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_restore(flags);
}

/*
 * Listen for ADB messages from the IOP.
 *
<<<<<<< HEAD
 * This will be called when unsolicited messages (usually replies to TALK
 * commands or autopoll packets) are received.
=======
 * This will be called when unsolicited IOP messages are received.
 * These IOP messages can carry ADB autopoll responses and also occur
 * after explicit ADB commands.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

static void adb_iop_listen(struct iop_msg *msg)
{
<<<<<<< HEAD
	struct adb_iopmsg *amsg = (struct adb_iopmsg *) msg->message;
	struct adb_request *req;
	unsigned long flags;
#ifdef DEBUG_ADB_IOP
	int i;
#endif

	local_irq_save(flags);

	req = current_req;

#ifdef DEBUG_ADB_IOP
	printk("adb_iop_listen %p: rcvd packet, %d bytes: %02X %02X", req,
		(uint) amsg->count + 2, (uint) amsg->flags, (uint) amsg->cmd);
	for (i = 0; i < amsg->count; i++)
		printk(" %02X", (uint) amsg->data[i]);
	printk("\n");
#endif

	/* Handle a timeout. Timeout packets seem to occur even after */
	/* we've gotten a valid reply to a TALK, so I'm assuming that */
	/* a "timeout" is actually more like an "end-of-data" signal. */
	/* We need to send back a timeout packet to the IOP to shut   */
	/* it up, plus complete the current request, if any.          */

	if (amsg->flags & ADB_IOP_TIMEOUT) {
		msg->reply[0] = ADB_IOP_TIMEOUT | ADB_IOP_AUTOPOLL;
		msg->reply[1] = 0;
		msg->reply[2] = 0;
		if (req && (adb_iop_state != idle)) {
			adb_iop_end_req(req, idle);
		}
	} else {
		/* TODO: is it possible for more than one chunk of data  */
		/*       to arrive before the timeout? If so we need to */
		/*       use reply_ptr here like the other drivers do.  */
		if ((adb_iop_state == awaiting_reply) &&
		    (amsg->flags & ADB_IOP_EXPLICIT)) {
			req->reply_len = amsg->count + 1;
			memcpy(req->reply, &amsg->cmd, req->reply_len);
		} else {
			adb_input(&amsg->cmd, amsg->count + 1,
				  amsg->flags & ADB_IOP_AUTOPOLL);
		}
		memcpy(msg->reply, msg->message, IOP_MSG_LEN);
	}
	iop_complete_message(msg);
=======
	struct adb_iopmsg *amsg = (struct adb_iopmsg *)msg->message;
	u8 addr = (amsg->cmd & ADDR_MASK) >> 4;
	u8 op = amsg->cmd & OP_MASK;
	unsigned long flags;
	bool req_done = false;

	local_irq_save(flags);

	/* Responses to Talk commands may be unsolicited as they are
	 * produced when the IOP polls devices. They are mostly timeouts.
	 */
	if (op == TALK && ((1 << addr) & autopoll_devs))
		autopoll_addr = addr;

	switch (amsg->flags & (ADB_IOP_EXPLICIT |
			       ADB_IOP_AUTOPOLL |
			       ADB_IOP_TIMEOUT)) {
	case ADB_IOP_EXPLICIT:
	case ADB_IOP_EXPLICIT | ADB_IOP_TIMEOUT:
		if (adb_iop_state == awaiting_reply) {
			struct adb_request *req = current_req;

			if (req->reply_expected) {
				req->reply_len = amsg->count + 1;
				memcpy(req->reply, &amsg->cmd, req->reply_len);
			}

			req_done = true;
		}
		break;
	case ADB_IOP_AUTOPOLL:
		if (((1 << addr) & autopoll_devs) &&
		    amsg->cmd == ADB_READREG(addr, 0))
			adb_input(&amsg->cmd, amsg->count + 1, 1);
		break;
	}
	msg->reply[0] = autopoll_addr ? ADB_IOP_AUTOPOLL : 0;
	msg->reply[1] = 0;
	msg->reply[2] = autopoll_addr ? ADB_READREG(autopoll_addr, 0) : 0;
	iop_complete_message(msg);

	if (req_done)
		adb_iop_done();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	local_irq_restore(flags);
}

/*
 * Start sending an ADB packet, IOP style
 *
 * There isn't much to do other than hand the packet over to the IOP
 * after encapsulating it in an adb_iopmsg.
 */

static void adb_iop_start(void)
{
<<<<<<< HEAD
	unsigned long flags;
	struct adb_request *req;
	struct adb_iopmsg amsg;
#ifdef DEBUG_ADB_IOP
	int i;
#endif

	/* get the packet to send */
	req = current_req;
	if (!req) return;

	local_irq_save(flags);

#ifdef DEBUG_ADB_IOP
	printk("adb_iop_start %p: sending packet, %d bytes:", req, req->nbytes);
	for (i = 0 ; i < req->nbytes ; i++)
		printk(" %02X", (uint) req->data[i]);
	printk("\n");
#endif

	/* The IOP takes MacII-style packets, so */
	/* strip the initial ADB_PACKET byte.    */

	amsg.flags = ADB_IOP_EXPLICIT;
	amsg.count = req->nbytes - 2;

	/* amsg.data immediately follows amsg.cmd, effectively making */
	/* amsg.cmd a pointer to the beginning of a full ADB packet.  */
=======
	struct adb_request *req;
	struct adb_iopmsg amsg;

	/* get the packet to send */
	req = current_req;
	if (!req)
		return;

	/* The IOP takes MacII-style packets, so strip the initial
	 * ADB_PACKET byte.
	 */
	amsg.flags = ADB_IOP_EXPLICIT;
	amsg.count = req->nbytes - 2;

	/* amsg.data immediately follows amsg.cmd, effectively making
	 * &amsg.cmd a pointer to the beginning of a full ADB packet.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(&amsg.cmd, req->data + 1, req->nbytes - 1);

	req->sent = 1;
	adb_iop_state = sending;
<<<<<<< HEAD
	local_irq_restore(flags);

	/* Now send it. The IOP manager will call adb_iop_complete */
	/* when the packet has been sent.                          */

	iop_send_message(ADB_IOP, ADB_CHAN, req,
			 sizeof(amsg), (__u8 *) &amsg, adb_iop_complete);
}

int adb_iop_probe(void)
{
	if (!iop_ism_present) return -ENODEV;
	return 0;
}

int adb_iop_init(void)
{
	printk("adb: IOP ISM driver v0.4 for Unified ADB.\n");
=======

	/* Now send it. The IOP manager will call adb_iop_complete
	 * when the message has been sent.
	 */
	iop_send_message(ADB_IOP, ADB_CHAN, req, sizeof(amsg), (__u8 *)&amsg,
			 adb_iop_complete);
}

static int adb_iop_probe(void)
{
	if (!iop_ism_present)
		return -ENODEV;
	return 0;
}

static int adb_iop_init(void)
{
	pr_info("adb: IOP ISM driver v0.4 for Unified ADB\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iop_listen(ADB_IOP, ADB_CHAN, adb_iop_listen, "ADB");
	return 0;
}

<<<<<<< HEAD
int adb_iop_send_request(struct adb_request *req, int sync)
=======
static int adb_iop_send_request(struct adb_request *req, int sync)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;

	err = adb_iop_write(req);
<<<<<<< HEAD
	if (err) return err;

	if (sync) {
		while (!req->complete) adb_iop_poll();
=======
	if (err)
		return err;

	if (sync) {
		while (!req->complete)
			adb_iop_poll();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

static int adb_iop_write(struct adb_request *req)
{
	unsigned long flags;

	if ((req->nbytes < 2) || (req->data[0] != ADB_PACKET)) {
		req->complete = 1;
		return -EINVAL;
	}

<<<<<<< HEAD
	local_irq_save(flags);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	req->next = NULL;
	req->sent = 0;
	req->complete = 0;
	req->reply_len = 0;

<<<<<<< HEAD
	if (current_req != 0) {
=======
	local_irq_save(flags);

	if (current_req) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		last_req->next = req;
		last_req = req;
	} else {
		current_req = req;
		last_req = req;
	}

<<<<<<< HEAD
	local_irq_restore(flags);
	if (adb_iop_state == idle) adb_iop_start();
	return 0;
}

int adb_iop_autopoll(int devs)
{
	/* TODO: how do we enable/disable autopoll? */
	return 0;
}

void adb_iop_poll(void)
{
	if (adb_iop_state == idle) adb_iop_start();
	iop_ism_irq(0, (void *) ADB_IOP);
}

int adb_iop_reset_bus(void)
{
	struct adb_request req = {
		.reply_expected = 0,
		.nbytes = 2,
		.data = { ADB_PACKET, 0 },
	};

	adb_iop_write(&req);
	while (!req.complete) {
		adb_iop_poll();
		schedule();
	}
=======
	if (adb_iop_state == idle)
		adb_iop_start();

	local_irq_restore(flags);

	return 0;
}

static void adb_iop_set_ap_complete(struct iop_msg *msg)
{
	struct adb_iopmsg *amsg = (struct adb_iopmsg *)msg->message;

	autopoll_devs = get_unaligned_be16(amsg->data);
	if (autopoll_devs & (1 << autopoll_addr))
		return;
	autopoll_addr = autopoll_devs ? (ffs(autopoll_devs) - 1) : 0;
}

static int adb_iop_autopoll(int devs)
{
	struct adb_iopmsg amsg;
	unsigned long flags;
	unsigned int mask = (unsigned int)devs & 0xFFFE;

	local_irq_save(flags);

	amsg.flags = ADB_IOP_SET_AUTOPOLL | (mask ? ADB_IOP_AUTOPOLL : 0);
	amsg.count = 2;
	amsg.cmd = 0;
	put_unaligned_be16(mask, amsg.data);

	iop_send_message(ADB_IOP, ADB_CHAN, NULL, sizeof(amsg), (__u8 *)&amsg,
			 adb_iop_set_ap_complete);

	local_irq_restore(flags);

	return 0;
}

static void adb_iop_poll(void)
{
	iop_ism_irq_poll(ADB_IOP);
}

static int adb_iop_reset_bus(void)
{
	struct adb_request req;

	/* Command = 0, Address = ignored */
	adb_request(&req, NULL, ADBREQ_NOSEND, 1, ADB_BUSRESET);
	adb_iop_send_request(&req, 1);

	/* Don't want any more requests during the Global Reset low time. */
	mdelay(3);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
