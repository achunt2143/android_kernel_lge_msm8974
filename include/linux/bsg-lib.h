<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  BSG helper library
 *
 *  Copyright (C) 2008   James Smart, Emulex Corporation
 *  Copyright (C) 2011   Red Hat, Inc.  All rights reserved.
 *  Copyright (C) 2011   Mike Christie
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _BLK_BSG_
#define _BLK_BSG_

#include <linux/blkdev.h>

<<<<<<< HEAD
=======
struct bsg_job;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct request;
struct device;
struct scatterlist;
struct request_queue;

<<<<<<< HEAD
=======
typedef int (bsg_job_fn) (struct bsg_job *);
typedef enum blk_eh_timer_return (bsg_timeout_fn)(struct request *);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct bsg_buffer {
	unsigned int payload_len;
	int sg_cnt;
	struct scatterlist *sg_list;
};

struct bsg_job {
	struct device *dev;
<<<<<<< HEAD
	struct request *req;
=======

	struct kref kref;

	unsigned int timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Transport/driver specific request/reply structs */
	void *request;
	void *reply;

	unsigned int request_len;
	unsigned int reply_len;
	/*
	 * On entry : reply_len indicates the buffer size allocated for
	 * the reply.
	 *
	 * Upon completion : the message handler must set reply_len
	 *  to indicates the size of the reply to be returned to the
	 *  caller.
	 */

	/* DMA payloads for the request/response */
	struct bsg_buffer request_payload;
	struct bsg_buffer reply_payload;

<<<<<<< HEAD
=======
	int result;
	unsigned int reply_payload_rcv_len;

	/* BIDI support */
	struct request *bidi_rq;
	struct bio *bidi_bio;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void *dd_data;		/* Used for driver-specific storage */
};

void bsg_job_done(struct bsg_job *job, int result,
		  unsigned int reply_payload_rcv_len);
<<<<<<< HEAD
int bsg_setup_queue(struct device *dev, struct request_queue *q, char *name,
		    bsg_job_fn *job_fn, int dd_job_size);
void bsg_request_fn(struct request_queue *q);
void bsg_remove_queue(struct request_queue *q);
void bsg_goose_queue(struct request_queue *q);
=======
struct request_queue *bsg_setup_queue(struct device *dev, const char *name,
		bsg_job_fn *job_fn, bsg_timeout_fn *timeout, int dd_job_size);
void bsg_remove_queue(struct request_queue *q);
void bsg_job_put(struct bsg_job *job);
int __must_check bsg_job_get(struct bsg_job *job);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
