<<<<<<< HEAD
/*
=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2015-2018 Oracle.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2005-2006 Network Appliance, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of the Network Appliance, Inc. nor the names of
 *      its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Tom Tucker <tom@opengridcomputing.com>
 */
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sysctl.h>
#include <linux/workqueue.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/svc_rdma.h>
<<<<<<< HEAD
#include "xprt_rdma.h"
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

/* RPC/RDMA parameters */
<<<<<<< HEAD
unsigned int svcrdma_ord = RPCRDMA_ORD;
static unsigned int min_ord = 1;
static unsigned int max_ord = 4096;
unsigned int svcrdma_max_requests = RPCRDMA_MAX_REQUESTS;
static unsigned int min_max_requests = 4;
static unsigned int max_max_requests = 16384;
unsigned int svcrdma_max_req_size = RPCRDMA_MAX_REQ_SIZE;
static unsigned int min_max_inline = 4096;
static unsigned int max_max_inline = 65536;

atomic_t rdma_stat_recv;
atomic_t rdma_stat_read;
atomic_t rdma_stat_write;
atomic_t rdma_stat_sq_starve;
atomic_t rdma_stat_rq_starve;
atomic_t rdma_stat_rq_poll;
atomic_t rdma_stat_rq_prod;
atomic_t rdma_stat_sq_poll;
atomic_t rdma_stat_sq_prod;

/* Temporary NFS request map and context caches */
struct kmem_cache *svc_rdma_map_cachep;
struct kmem_cache *svc_rdma_ctxt_cachep;

struct workqueue_struct *svc_rdma_wq;

/*
 * This function implements reading and resetting an atomic_t stat
 * variable through read/write to a proc file. Any write to the file
 * resets the associated statistic to zero. Any read returns it's
 * current value.
 */
static int read_reset_stat(ctl_table *table, int write,
			   void __user *buffer, size_t *lenp,
			   loff_t *ppos)
{
	atomic_t *stat = (atomic_t *)table->data;

	if (!stat)
		return -EINVAL;

	if (write)
		atomic_set(stat, 0);
	else {
		char str_buf[32];
		char *data;
		int len = snprintf(str_buf, 32, "%d\n", atomic_read(stat));
		if (len >= 32)
			return -EFAULT;
		len = strlen(str_buf);
		if (*ppos > len) {
			*lenp = 0;
			return 0;
		}
		data = &str_buf[*ppos];
		len -= *ppos;
		if (len > *lenp)
			len = *lenp;
		if (len && copy_to_user(buffer, str_buf, len))
			return -EFAULT;
		*lenp = len;
		*ppos += len;
	}
=======
unsigned int svcrdma_ord = 16;	/* historical default */
static unsigned int min_ord = 1;
static unsigned int max_ord = 255;
unsigned int svcrdma_max_requests = RPCRDMA_MAX_REQUESTS;
unsigned int svcrdma_max_bc_requests = RPCRDMA_MAX_BC_REQUESTS;
static unsigned int min_max_requests = 4;
static unsigned int max_max_requests = 16384;
unsigned int svcrdma_max_req_size = RPCRDMA_DEF_INLINE_THRESH;
static unsigned int min_max_inline = RPCRDMA_DEF_INLINE_THRESH;
static unsigned int max_max_inline = RPCRDMA_MAX_INLINE_THRESH;
static unsigned int svcrdma_stat_unused;
static unsigned int zero;

struct percpu_counter svcrdma_stat_read;
struct percpu_counter svcrdma_stat_recv;
struct percpu_counter svcrdma_stat_sq_starve;
struct percpu_counter svcrdma_stat_write;

enum {
	SVCRDMA_COUNTER_BUFSIZ	= sizeof(unsigned long long),
};

static int svcrdma_counter_handler(struct ctl_table *table, int write,
				   void *buffer, size_t *lenp, loff_t *ppos)
{
	struct percpu_counter *stat = (struct percpu_counter *)table->data;
	char tmp[SVCRDMA_COUNTER_BUFSIZ + 1];
	int len;

	if (write) {
		percpu_counter_set(stat, 0);
		return 0;
	}

	len = snprintf(tmp, SVCRDMA_COUNTER_BUFSIZ, "%lld\n",
		       percpu_counter_sum_positive(stat));
	if (len >= SVCRDMA_COUNTER_BUFSIZ)
		return -EFAULT;
	len = strlen(tmp);
	if (*ppos > len) {
		*lenp = 0;
		return 0;
	}
	len -= *ppos;
	if (len > *lenp)
		len = *lenp;
	if (len)
		memcpy(buffer, tmp, len);
	*lenp = len;
	*ppos += len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static struct ctl_table_header *svcrdma_table_header;
<<<<<<< HEAD
static ctl_table svcrdma_parm_table[] = {
=======
static struct ctl_table svcrdma_parm_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "max_requests",
		.data		= &svcrdma_max_requests,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_max_requests,
		.extra2		= &max_max_requests
	},
	{
		.procname	= "max_req_size",
		.data		= &svcrdma_max_req_size,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_max_inline,
		.extra2		= &max_max_inline
	},
	{
		.procname	= "max_outbound_read_requests",
		.data		= &svcrdma_ord,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_ord,
		.extra2		= &max_ord,
	},

	{
		.procname	= "rdma_stat_read",
<<<<<<< HEAD
		.data		= &rdma_stat_read,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_recv",
		.data		= &rdma_stat_recv,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_write",
		.data		= &rdma_stat_write,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_sq_starve",
		.data		= &rdma_stat_sq_starve,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_rq_starve",
		.data		= &rdma_stat_rq_starve,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_rq_poll",
		.data		= &rdma_stat_rq_poll,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_rq_prod",
		.data		= &rdma_stat_rq_prod,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_sq_poll",
		.data		= &rdma_stat_sq_poll,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
	},
	{
		.procname	= "rdma_stat_sq_prod",
		.data		= &rdma_stat_sq_prod,
		.maxlen		= sizeof(atomic_t),
		.mode		= 0644,
		.proc_handler	= read_reset_stat,
=======
		.data		= &svcrdma_stat_read,
		.maxlen		= SVCRDMA_COUNTER_BUFSIZ,
		.mode		= 0644,
		.proc_handler	= svcrdma_counter_handler,
	},
	{
		.procname	= "rdma_stat_recv",
		.data		= &svcrdma_stat_recv,
		.maxlen		= SVCRDMA_COUNTER_BUFSIZ,
		.mode		= 0644,
		.proc_handler	= svcrdma_counter_handler,
	},
	{
		.procname	= "rdma_stat_write",
		.data		= &svcrdma_stat_write,
		.maxlen		= SVCRDMA_COUNTER_BUFSIZ,
		.mode		= 0644,
		.proc_handler	= svcrdma_counter_handler,
	},
	{
		.procname	= "rdma_stat_sq_starve",
		.data		= &svcrdma_stat_sq_starve,
		.maxlen		= SVCRDMA_COUNTER_BUFSIZ,
		.mode		= 0644,
		.proc_handler	= svcrdma_counter_handler,
	},
	{
		.procname	= "rdma_stat_rq_starve",
		.data		= &svcrdma_stat_unused,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &zero,
	},
	{
		.procname	= "rdma_stat_rq_poll",
		.data		= &svcrdma_stat_unused,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &zero,
	},
	{
		.procname	= "rdma_stat_rq_prod",
		.data		= &svcrdma_stat_unused,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &zero,
	},
	{
		.procname	= "rdma_stat_sq_poll",
		.data		= &svcrdma_stat_unused,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &zero,
	},
	{
		.procname	= "rdma_stat_sq_prod",
		.data		= &svcrdma_stat_unused,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &zero,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{ },
};

<<<<<<< HEAD
static ctl_table svcrdma_table[] = {
	{
		.procname	= "svc_rdma",
		.mode		= 0555,
		.child		= svcrdma_parm_table
	},
	{ },
};

static ctl_table svcrdma_root_table[] = {
	{
		.procname	= "sunrpc",
		.mode		= 0555,
		.child		= svcrdma_table
	},
	{ },
};

void svc_rdma_cleanup(void)
{
	dprintk("SVCRDMA Module Removed, deregister RPC RDMA transport\n");
	destroy_workqueue(svc_rdma_wq);
	if (svcrdma_table_header) {
		unregister_sysctl_table(svcrdma_table_header);
		svcrdma_table_header = NULL;
	}
	svc_unreg_xprt_class(&svc_rdma_class);
	kmem_cache_destroy(svc_rdma_map_cachep);
	kmem_cache_destroy(svc_rdma_ctxt_cachep);
=======
static void svc_rdma_proc_cleanup(void)
{
	if (!svcrdma_table_header)
		return;
	unregister_sysctl_table(svcrdma_table_header);
	svcrdma_table_header = NULL;

	percpu_counter_destroy(&svcrdma_stat_write);
	percpu_counter_destroy(&svcrdma_stat_sq_starve);
	percpu_counter_destroy(&svcrdma_stat_recv);
	percpu_counter_destroy(&svcrdma_stat_read);
}

static int svc_rdma_proc_init(void)
{
	int rc;

	if (svcrdma_table_header)
		return 0;

	rc = percpu_counter_init(&svcrdma_stat_read, 0, GFP_KERNEL);
	if (rc)
		goto out_err;
	rc = percpu_counter_init(&svcrdma_stat_recv, 0, GFP_KERNEL);
	if (rc)
		goto out_err;
	rc = percpu_counter_init(&svcrdma_stat_sq_starve, 0, GFP_KERNEL);
	if (rc)
		goto out_err;
	rc = percpu_counter_init(&svcrdma_stat_write, 0, GFP_KERNEL);
	if (rc)
		goto out_err;

	svcrdma_table_header = register_sysctl("sunrpc/svc_rdma",
					       svcrdma_parm_table);
	return 0;

out_err:
	percpu_counter_destroy(&svcrdma_stat_sq_starve);
	percpu_counter_destroy(&svcrdma_stat_recv);
	percpu_counter_destroy(&svcrdma_stat_read);
	return rc;
}

struct workqueue_struct *svcrdma_wq;

void svc_rdma_cleanup(void)
{
	svc_unreg_xprt_class(&svc_rdma_class);
	svc_rdma_proc_cleanup();
	if (svcrdma_wq) {
		struct workqueue_struct *wq = svcrdma_wq;

		svcrdma_wq = NULL;
		destroy_workqueue(wq);
	}

	dprintk("SVCRDMA Module Removed, deregister RPC RDMA transport\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int svc_rdma_init(void)
{
<<<<<<< HEAD
	dprintk("SVCRDMA Module Init, register RPC RDMA transport\n");
	dprintk("\tsvcrdma_ord      : %d\n", svcrdma_ord);
	dprintk("\tmax_requests     : %d\n", svcrdma_max_requests);
	dprintk("\tsq_depth         : %d\n",
		svcrdma_max_requests * RPCRDMA_SQ_DEPTH_MULT);
	dprintk("\tmax_inline       : %d\n", svcrdma_max_req_size);

	svc_rdma_wq = alloc_workqueue("svc_rdma", 0, 0);
	if (!svc_rdma_wq)
		return -ENOMEM;

	if (!svcrdma_table_header)
		svcrdma_table_header =
			register_sysctl_table(svcrdma_root_table);

	/* Create the temporary map cache */
	svc_rdma_map_cachep = kmem_cache_create("svc_rdma_map_cache",
						sizeof(struct svc_rdma_req_map),
						0,
						SLAB_HWCACHE_ALIGN,
						NULL);
	if (!svc_rdma_map_cachep) {
		printk(KERN_INFO "Could not allocate map cache.\n");
		goto err0;
	}

	/* Create the temporary context cache */
	svc_rdma_ctxt_cachep =
		kmem_cache_create("svc_rdma_ctxt_cache",
				  sizeof(struct svc_rdma_op_ctxt),
				  0,
				  SLAB_HWCACHE_ALIGN,
				  NULL);
	if (!svc_rdma_ctxt_cachep) {
		printk(KERN_INFO "Could not allocate WR ctxt cache.\n");
		goto err1;
	}

	/* Register RDMA with the SVC transport switch */
	svc_reg_xprt_class(&svc_rdma_class);
	return 0;
 err1:
	kmem_cache_destroy(svc_rdma_map_cachep);
 err0:
	unregister_sysctl_table(svcrdma_table_header);
	destroy_workqueue(svc_rdma_wq);
	return -ENOMEM;
}
MODULE_AUTHOR("Tom Tucker <tom@opengridcomputing.com>");
MODULE_DESCRIPTION("SVC RDMA Transport");
MODULE_LICENSE("Dual BSD/GPL");
module_init(svc_rdma_init);
module_exit(svc_rdma_cleanup);
=======
	struct workqueue_struct *wq;
	int rc;

	wq = alloc_workqueue("svcrdma", WQ_UNBOUND, 0);
	if (!wq)
		return -ENOMEM;

	rc = svc_rdma_proc_init();
	if (rc) {
		destroy_workqueue(wq);
		return rc;
	}

	svcrdma_wq = wq;
	svc_reg_xprt_class(&svc_rdma_class);

	dprintk("SVCRDMA Module Init, register RPC RDMA transport\n");
	dprintk("\tsvcrdma_ord      : %d\n", svcrdma_ord);
	dprintk("\tmax_requests     : %u\n", svcrdma_max_requests);
	dprintk("\tmax_bc_requests  : %u\n", svcrdma_max_bc_requests);
	dprintk("\tmax_inline       : %d\n", svcrdma_max_req_size);
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
