<<<<<<< HEAD
/*
 *  linux/drivers/s390/crypto/zcrypt_api.h
 *
 *  zcrypt 2.1.0
 *
 *  Copyright (C)  2001, 2006 IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *  Copyright IBM Corp. 2001, 2019
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Author(s): Robert Burroughs
 *	       Eric Rossman (edrossma@us.ibm.com)
 *	       Cornelia Huck <cornelia.huck@de.ibm.com>
 *
 *  Hotplug & misc device support: Jochen Roehrig (roehrig@de.ibm.com)
 *  Major cleanup & driver split: Martin Schwidefsky <schwidefsky@de.ibm.com>
 *				  Ralph Wuerthner <rwuerthn@de.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
 *  MSGTYPE restruct:		  Holger Dengler <hd@linux.vnet.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ZCRYPT_API_H_
#define _ZCRYPT_API_H_

<<<<<<< HEAD
#include "ap_bus.h"
#include <asm/zcrypt.h>

/* deprecated status calls */
#define ICAZ90STATUS		_IOR(ZCRYPT_IOCTL_MAGIC, 0x10, struct ica_z90_status)
#define Z90STAT_PCIXCCCOUNT	_IOR(ZCRYPT_IOCTL_MAGIC, 0x43, int)

/**
 * This structure is deprecated and the corresponding ioctl() has been
 * replaced with individual ioctl()s for each piece of data!
 */
struct ica_z90_status {
	int totalcount;
	int leedslitecount; // PCICA
	int leeds2count;    // PCICC
	// int PCIXCCCount; is not in struct for backward compatibility
	int requestqWaitCount;
	int pendingqWaitCount;
	int totalOpenCount;
	int cryptoDomain;
	// status: 0=not there, 1=PCICA, 2=PCICC, 3=PCIXCC_MCL2, 4=PCIXCC_MCL3,
	//	   5=CEX2C
	unsigned char status[64];
	// qdepth: # work elements waiting for each device
	unsigned char qdepth[64];
};

/**
 * device type for an actual device is either PCICA, PCICC, PCIXCC_MCL2,
 * PCIXCC_MCL3, CEX2C, or CEX2A
 *
 * NOTE: PCIXCC_MCL3 refers to a PCIXCC with May 2004 version of Licensed
 *	 Internal Code (LIC) (EC J12220 level 29).
 *	 PCIXCC_MCL2 refers to any LIC before this level.
 */
#define ZCRYPT_PCICA		1
#define ZCRYPT_PCICC		2
#define ZCRYPT_PCIXCC_MCL2	3
#define ZCRYPT_PCIXCC_MCL3	4
=======
#include <linux/atomic.h>
#include <asm/debug.h>
#include <asm/zcrypt.h>
#include "ap_bus.h"

/**
 * Supported device types
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ZCRYPT_CEX2C		5
#define ZCRYPT_CEX2A		6
#define ZCRYPT_CEX3C		7
#define ZCRYPT_CEX3A		8
<<<<<<< HEAD
=======
#define ZCRYPT_CEX4	       10
#define ZCRYPT_CEX5	       11
#define ZCRYPT_CEX6	       12
#define ZCRYPT_CEX7	       13
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Large random numbers are pulled in 4096 byte chunks from the crypto cards
 * and stored in a page. Be careful when increasing this buffer due to size
 * limitations for AP requests.
 */
#define ZCRYPT_RNG_BUFFER_SIZE	4096

<<<<<<< HEAD
struct zcrypt_device;

struct zcrypt_ops {
	long (*rsa_modexpo)(struct zcrypt_device *, struct ica_rsa_modexpo *);
	long (*rsa_modexpo_crt)(struct zcrypt_device *,
				struct ica_rsa_modexpo_crt *);
	long (*send_cprb)(struct zcrypt_device *, struct ica_xcRB *);
	long (*rng)(struct zcrypt_device *, char *);
};

struct zcrypt_device {
	struct list_head list;		/* Device list. */
	spinlock_t lock;		/* Per device lock. */
	struct kref refcount;		/* device refcounting */
	struct ap_device *ap_dev;	/* The "real" ap device. */
	struct zcrypt_ops *ops;		/* Crypto operations. */
=======
/**
 * The zcrypt_wait_api_operational() function waits this
 * amount in milliseconds for ap_wait_aqpn_bindings_complete().
 * Also on a cprb send failure with ENODEV the send functions
 * trigger an ap bus rescan and wait this time in milliseconds
 * for ap_wait_aqpn_bindings_complete() before resending.
 */
#define ZCRYPT_WAIT_BINDINGS_COMPLETE_MS 30000

/*
 * Identifier for Crypto Request Performance Index
 */
enum crypto_ops {
	MEX_1K,
	MEX_2K,
	MEX_4K,
	CRT_1K,
	CRT_2K,
	CRT_4K,
	HWRNG,
	SECKEY,
	NUM_OPS
};

struct zcrypt_queue;

/* struct to hold tracking information for a userspace request/response */
struct zcrypt_track {
	int again_counter;		/* retry attempts counter */
	int last_qid;			/* last qid used */
	int last_rc;			/* last return code */
};

/* defines related to message tracking */
#define TRACK_AGAIN_MAX 10
#define TRACK_AGAIN_CARD_WEIGHT_PENALTY  1000
#define TRACK_AGAIN_QUEUE_WEIGHT_PENALTY 10000

struct zcrypt_ops {
	long (*rsa_modexpo)(struct zcrypt_queue *, struct ica_rsa_modexpo *,
			    struct ap_message *);
	long (*rsa_modexpo_crt)(struct zcrypt_queue *,
				struct ica_rsa_modexpo_crt *,
				struct ap_message *);
	long (*send_cprb)(bool userspace, struct zcrypt_queue *, struct ica_xcRB *,
			  struct ap_message *);
	long (*send_ep11_cprb)(bool userspace, struct zcrypt_queue *, struct ep11_urb *,
			       struct ap_message *);
	long (*rng)(struct zcrypt_queue *, char *, struct ap_message *);
	struct list_head list;		/* zcrypt ops list. */
	struct module *owner;
	int variant;
	char name[128];
};

struct zcrypt_card {
	struct list_head list;		/* Device list. */
	struct list_head zqueues;	/* List of zcrypt queues */
	struct kref refcount;		/* device refcounting */
	struct ap_card *card;		/* The "real" ap card device. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int online;			/* User online/offline */

	int user_space_type;		/* User space device id. */
	char *type_string;		/* User space device name. */
	int min_mod_size;		/* Min number of bits. */
	int max_mod_size;		/* Max number of bits. */
<<<<<<< HEAD
	int short_crt;			/* Card has crt length restriction. */
	int speed_rating;		/* Speed of the crypto device. */
=======
	int max_exp_bit_length;
	const int *speed_rating;	/* Speed idx of crypto ops. */
	atomic_t load;			/* Utilization of the crypto device */

	int request_count;		/* # current requests. */
};

struct zcrypt_queue {
	struct list_head list;		/* Device list. */
	struct kref refcount;		/* device refcounting */
	struct zcrypt_card *zcard;
	struct zcrypt_ops *ops;		/* Crypto operations. */
	struct ap_queue *queue;		/* The "real" ap queue device. */
	int online;			/* User online/offline */

	atomic_t load;			/* Utilization of the crypto device */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int request_count;		/* # current requests. */

	struct ap_message reply;	/* Per-device reply structure. */
<<<<<<< HEAD
	int max_exp_bit_length;
};

struct zcrypt_device *zcrypt_device_alloc(size_t);
void zcrypt_device_free(struct zcrypt_device *);
void zcrypt_device_get(struct zcrypt_device *);
int zcrypt_device_put(struct zcrypt_device *);
int zcrypt_device_register(struct zcrypt_device *);
void zcrypt_device_unregister(struct zcrypt_device *);
int zcrypt_api_init(void);
void zcrypt_api_exit(void);
=======
};

/* transport layer rescanning */
extern atomic_t zcrypt_rescan_req;

extern spinlock_t zcrypt_list_lock;
extern struct list_head zcrypt_card_list;

#define for_each_zcrypt_card(_zc) \
	list_for_each_entry(_zc, &zcrypt_card_list, list)

#define for_each_zcrypt_queue(_zq, _zc) \
	list_for_each_entry(_zq, &(_zc)->zqueues, list)

struct zcrypt_card *zcrypt_card_alloc(void);
void zcrypt_card_free(struct zcrypt_card *);
void zcrypt_card_get(struct zcrypt_card *);
int zcrypt_card_put(struct zcrypt_card *);
int zcrypt_card_register(struct zcrypt_card *);
void zcrypt_card_unregister(struct zcrypt_card *);

struct zcrypt_queue *zcrypt_queue_alloc(size_t);
void zcrypt_queue_free(struct zcrypt_queue *);
void zcrypt_queue_get(struct zcrypt_queue *);
int zcrypt_queue_put(struct zcrypt_queue *);
int zcrypt_queue_register(struct zcrypt_queue *);
void zcrypt_queue_unregister(struct zcrypt_queue *);
bool zcrypt_queue_force_online(struct zcrypt_queue *zq, int online);

int zcrypt_rng_device_add(void);
void zcrypt_rng_device_remove(void);

void zcrypt_msgtype_register(struct zcrypt_ops *);
void zcrypt_msgtype_unregister(struct zcrypt_ops *);
struct zcrypt_ops *zcrypt_msgtype(unsigned char *, int);
int zcrypt_api_init(void);
void zcrypt_api_exit(void);
long zcrypt_send_cprb(struct ica_xcRB *xcRB);
long zcrypt_send_ep11_cprb(struct ep11_urb *urb);
void zcrypt_device_status_mask_ext(struct zcrypt_device_status_ext *devstatus);
int zcrypt_device_status_ext(int card, int queue,
			     struct zcrypt_device_status_ext *devstatus);

int zcrypt_wait_api_operational(void);

static inline unsigned long z_copy_from_user(bool userspace,
					     void *to,
					     const void __user *from,
					     unsigned long n)
{
	if (likely(userspace))
		return copy_from_user(to, from, n);
	memcpy(to, (void __force *)from, n);
	return 0;
}

static inline unsigned long z_copy_to_user(bool userspace,
					   void __user *to,
					   const void *from,
					   unsigned long n)
{
	if (likely(userspace))
		return copy_to_user(to, from, n);
	memcpy((void __force *)to, from, n);
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ZCRYPT_API_H_ */
