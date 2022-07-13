/*
 * Copyright (c) 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2005, 2006, 2007 Cisco Systems.  All rights reserved.
 * Copyright (c) 2005 PathScale, Inc.  All rights reserved.
 * Copyright (c) 2006 Mellanox Technologies.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/file.h>
#include <linux/fs.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include <asm/uaccess.h>

#include "uverbs.h"

static struct lock_class_key pd_lock_key;
static struct lock_class_key mr_lock_key;
static struct lock_class_key cq_lock_key;
static struct lock_class_key qp_lock_key;
static struct lock_class_key ah_lock_key;
static struct lock_class_key srq_lock_key;
static struct lock_class_key xrcd_lock_key;

#define INIT_UDATA(udata, ibuf, obuf, ilen, olen)			\
	do {								\
		(udata)->inbuf  = (void __user *) (ibuf);		\
		(udata)->outbuf = (void __user *) (obuf);		\
		(udata)->inlen  = (ilen);				\
		(udata)->outlen = (olen);				\
	} while (0)

/*
 * The ib_uobject locking scheme is as follows:
 *
 * - ib_uverbs_idr_lock protects the uverbs idrs themselves, so it
 *   needs to be held during all idr operations.  When an object is
 *   looked up, a reference must be taken on the object's kref before
 *   dropping this lock.
 *
 * - Each object also has an rwsem.  This rwsem must be held for
 *   reading while an operation that uses the object is performed.
 *   For example, while registering an MR, the associated PD's
 *   uobject.mutex must be held for reading.  The rwsem must be held
 *   for writing while initializing or destroying an object.
 *
 * - In addition, each object has a "live" flag.  If this flag is not
 *   set, then lookups of the object will fail even if it is found in
 *   the idr.  This handles a reader that blocks and does not acquire
 *   the rwsem until after the object is destroyed.  The destroy
 *   operation will set the live flag to 0 and then drop the rwsem;
 *   this will allow the reader to acquire the rwsem, see that the
 *   live flag is 0, and then drop the rwsem and its reference to
 *   object.  The underlying storage will not be freed until the last
 *   reference to the object is dropped.
 */

static void init_uobj(struct ib_uobject *uobj, u64 user_handle,
		      struct ib_ucontext *context, struct lock_class_key *key)
{
	uobj->user_handle = user_handle;
	uobj->context     = context;
	kref_init(&uobj->ref);
	init_rwsem(&uobj->mutex);
	lockdep_set_class(&uobj->mutex, key);
	uobj->live        = 0;
}

static void release_uobj(struct kref *kref)
{
	kfree(container_of(kref, struct ib_uobject, ref));
}

static void put_uobj(struct ib_uobject *uobj)
{
	kref_put(&uobj->ref, release_uobj);
}

static void put_uobj_read(struct ib_uobject *uobj)
{
	up_read(&uobj->mutex);
	put_uobj(uobj);
}

static void put_uobj_write(struct ib_uobject *uobj)
{
	up_write(&uobj->mutex);
	put_uobj(uobj);
}

static int idr_add_uobj(struct idr *idr, struct ib_uobject *uobj)
{
	int ret;

retry:
	if (!idr_pre_get(idr, GFP_KERNEL))
		return -ENOMEM;

	spin_lock(&ib_uverbs_idr_lock);
	ret = idr_get_new(idr, uobj, &uobj->id);
	spin_unlock(&ib_uverbs_idr_lock);

	if (ret == -EAGAIN)
		goto retry;

	return ret;
}

void idr_remove_uobj(struct idr *idr, struct ib_uobject *uobj)
{
	spin_lock(&ib_uverbs_idr_lock);
	idr_remove(idr, uobj->id);
	spin_unlock(&ib_uverbs_idr_lock);
}

static struct ib_uobject *__idr_get_uobj(struct idr *idr, int id,
					 struct ib_ucontext *context)
{
	struct ib_uobject *uobj;

	spin_lock(&ib_uverbs_idr_lock);
	uobj = idr_find(idr, id);
	if (uobj) {
		if (uobj->context == context)
			kref_get(&uobj->ref);
		else
			uobj = NULL;
	}
	spin_unlock(&ib_uverbs_idr_lock);

	return uobj;
}

static struct ib_uobject *idr_read_uobj(struct idr *idr, int id,
					struct ib_ucontext *context, int nested)
{
	struct ib_uobject *uobj;

	uobj = __idr_get_uobj(idr, id, context);
	if (!uobj)
		return NULL;

	if (nested)
		down_read_nested(&uobj->mutex, SINGLE_DEPTH_NESTING);
	else
		down_read(&uobj->mutex);
	if (!uobj->live) {
		put_uobj_read(uobj);
		return NULL;
	}

	return uobj;
}

static struct ib_uobject *idr_write_uobj(struct idr *idr, int id,
					 struct ib_ucontext *context)
{
	struct ib_uobject *uobj;

	uobj = __idr_get_uobj(idr, id, context);
	if (!uobj)
		return NULL;

	down_write(&uobj->mutex);
	if (!uobj->live) {
		put_uobj_write(uobj);
		return NULL;
	}

	return uobj;
}

static void *idr_read_obj(struct idr *idr, int id, struct ib_ucontext *context,
			  int nested)
{
	struct ib_uobject *uobj;

	uobj = idr_read_uobj(idr, id, context, nested);
	return uobj ? uobj->object : NULL;
}

static struct ib_pd *idr_read_pd(int pd_handle, struct ib_ucontext *context)
{
	return idr_read_obj(&ib_uverbs_pd_idr, pd_handle, context, 0);
}

static void put_pd_read(struct ib_pd *pd)
{
	put_uobj_read(pd->uobject);
}

static struct ib_cq *idr_read_cq(int cq_handle, struct ib_ucontext *context, int nested)
{
	return idr_read_obj(&ib_uverbs_cq_idr, cq_handle, context, nested);
}

static void put_cq_read(struct ib_cq *cq)
{
	put_uobj_read(cq->uobject);
}

static struct ib_ah *idr_read_ah(int ah_handle, struct ib_ucontext *context)
{
	return idr_read_obj(&ib_uverbs_ah_idr, ah_handle, context, 0);
}

static void put_ah_read(struct ib_ah *ah)
{
	put_uobj_read(ah->uobject);
}

static struct ib_qp *idr_read_qp(int qp_handle, struct ib_ucontext *context)
{
	return idr_read_obj(&ib_uverbs_qp_idr, qp_handle, context, 0);
}

static struct ib_qp *idr_write_qp(int qp_handle, struct ib_ucontext *context)
{
	struct ib_uobject *uobj;

	uobj = idr_write_uobj(&ib_uverbs_qp_idr, qp_handle, context);
	return uobj ? uobj->object : NULL;
}

static void put_qp_read(struct ib_qp *qp)
{
	put_uobj_read(qp->uobject);
}

static void put_qp_write(struct ib_qp *qp)
{
	put_uobj_write(qp->uobject);
}

static struct ib_srq *idr_read_srq(int srq_handle, struct ib_ucontext *context)
{
	return idr_read_obj(&ib_uverbs_srq_idr, srq_handle, context, 0);
}

static void put_srq_read(struct ib_srq *srq)
{
	put_uobj_read(srq->uobject);
}

static struct ib_xrcd *idr_read_xrcd(int xrcd_handle, struct ib_ucontext *context,
				     struct ib_uobject **uobj)
{
	*uobj = idr_read_uobj(&ib_uverbs_xrcd_idr, xrcd_handle, context, 0);
	return *uobj ? (*uobj)->object : NULL;
}

static void put_xrcd_read(struct ib_uobject *uobj)
{
	put_uobj_read(uobj);
}

ssize_t ib_uverbs_get_context(struct ib_uverbs_file *file,
			      const char __user *buf,
			      int in_len, int out_len)
{
	struct ib_uverbs_get_context      cmd;
	struct ib_uverbs_get_context_resp resp;
	struct ib_udata                   udata;
	struct ib_device                 *ibdev = file->device->ib_dev;
	struct ib_ucontext		 *ucontext;
	struct file			 *filp;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	mutex_lock(&file->mutex);

=======
#include <linux/sched.h>

#include <linux/uaccess.h>

#include <rdma/uverbs_types.h>
#include <rdma/uverbs_std_types.h>
#include "rdma_core.h"

#include "uverbs.h"
#include "core_priv.h"

/*
 * Copy a response to userspace. If the provided 'resp' is larger than the
 * user buffer it is silently truncated. If the user provided a larger buffer
 * then the trailing portion is zero filled.
 *
 * These semantics are intended to support future extension of the output
 * structures.
 */
static int uverbs_response(struct uverbs_attr_bundle *attrs, const void *resp,
			   size_t resp_len)
{
	int ret;

	if (uverbs_attr_is_valid(attrs, UVERBS_ATTR_CORE_OUT))
		return uverbs_copy_to_struct_or_zero(
			attrs, UVERBS_ATTR_CORE_OUT, resp, resp_len);

	if (copy_to_user(attrs->ucore.outbuf, resp,
			 min(attrs->ucore.outlen, resp_len)))
		return -EFAULT;

	if (resp_len < attrs->ucore.outlen) {
		/*
		 * Zero fill any extra memory that user
		 * space might have provided.
		 */
		ret = clear_user(attrs->ucore.outbuf + resp_len,
				 attrs->ucore.outlen - resp_len);
		if (ret)
			return -EFAULT;
	}

	return 0;
}

/*
 * Copy a request from userspace. If the provided 'req' is larger than the
 * user buffer then the user buffer is zero extended into the 'req'. If 'req'
 * is smaller than the user buffer then the uncopied bytes in the user buffer
 * must be zero.
 */
static int uverbs_request(struct uverbs_attr_bundle *attrs, void *req,
			  size_t req_len)
{
	if (copy_from_user(req, attrs->ucore.inbuf,
			   min(attrs->ucore.inlen, req_len)))
		return -EFAULT;

	if (attrs->ucore.inlen < req_len) {
		memset(req + attrs->ucore.inlen, 0,
		       req_len - attrs->ucore.inlen);
	} else if (attrs->ucore.inlen > req_len) {
		if (!ib_is_buffer_cleared(attrs->ucore.inbuf + req_len,
					  attrs->ucore.inlen - req_len))
			return -EOPNOTSUPP;
	}
	return 0;
}

/*
 * Generate the value for the 'response_length' protocol used by write_ex.
 * This is the number of bytes the kernel actually wrote. Userspace can use
 * this to detect what structure members in the response the kernel
 * understood.
 */
static u32 uverbs_response_length(struct uverbs_attr_bundle *attrs,
				  size_t resp_len)
{
	return min_t(size_t, attrs->ucore.outlen, resp_len);
}

/*
 * The iterator version of the request interface is for handlers that need to
 * step over a flex array at the end of a command header.
 */
struct uverbs_req_iter {
	const void __user *cur;
	const void __user *end;
};

static int uverbs_request_start(struct uverbs_attr_bundle *attrs,
				struct uverbs_req_iter *iter,
				void *req,
				size_t req_len)
{
	if (attrs->ucore.inlen < req_len)
		return -ENOSPC;

	if (copy_from_user(req, attrs->ucore.inbuf, req_len))
		return -EFAULT;

	iter->cur = attrs->ucore.inbuf + req_len;
	iter->end = attrs->ucore.inbuf + attrs->ucore.inlen;
	return 0;
}

static int uverbs_request_next(struct uverbs_req_iter *iter, void *val,
			       size_t len)
{
	if (iter->cur + len > iter->end)
		return -ENOSPC;

	if (copy_from_user(val, iter->cur, len))
		return -EFAULT;

	iter->cur += len;
	return 0;
}

static const void __user *uverbs_request_next_ptr(struct uverbs_req_iter *iter,
						  size_t len)
{
	const void __user *res = iter->cur;

	if (iter->cur + len > iter->end)
		return (void __force __user *)ERR_PTR(-ENOSPC);
	iter->cur += len;
	return res;
}

static int uverbs_request_finish(struct uverbs_req_iter *iter)
{
	if (!ib_is_buffer_cleared(iter->cur, iter->end - iter->cur))
		return -EOPNOTSUPP;
	return 0;
}

/*
 * When calling a destroy function during an error unwind we need to pass in
 * the udata that is sanitized of all user arguments. Ie from the driver
 * perspective it looks like no udata was passed.
 */
struct ib_udata *uverbs_get_cleared_udata(struct uverbs_attr_bundle *attrs)
{
	attrs->driver_udata = (struct ib_udata){};
	return &attrs->driver_udata;
}

static struct ib_uverbs_completion_event_file *
_ib_uverbs_lookup_comp_file(s32 fd, struct uverbs_attr_bundle *attrs)
{
	struct ib_uobject *uobj = ufd_get_read(UVERBS_OBJECT_COMP_CHANNEL,
					       fd, attrs);

	if (IS_ERR(uobj))
		return (void *)uobj;

	uverbs_uobject_get(uobj);
	uobj_put_read(uobj);

	return container_of(uobj, struct ib_uverbs_completion_event_file,
			    uobj);
}
#define ib_uverbs_lookup_comp_file(_fd, _ufile)                                \
	_ib_uverbs_lookup_comp_file((_fd)*typecheck(s32, _fd), _ufile)

int ib_alloc_ucontext(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_file *ufile = attrs->ufile;
	struct ib_ucontext *ucontext;
	struct ib_device *ib_dev;

	ib_dev = srcu_dereference(ufile->device->ib_dev,
				  &ufile->device->disassociate_srcu);
	if (!ib_dev)
		return -EIO;

	ucontext = rdma_zalloc_drv_obj(ib_dev, ib_ucontext);
	if (!ucontext)
		return -ENOMEM;

	ucontext->device = ib_dev;
	ucontext->ufile = ufile;
	xa_init_flags(&ucontext->mmap_xa, XA_FLAGS_ALLOC);

	rdma_restrack_new(&ucontext->res, RDMA_RESTRACK_CTX);
	rdma_restrack_set_name(&ucontext->res, NULL);
	attrs->context = ucontext;
	return 0;
}

int ib_init_ucontext(struct uverbs_attr_bundle *attrs)
{
	struct ib_ucontext *ucontext = attrs->context;
	struct ib_uverbs_file *file = attrs->ufile;
	int ret;

	if (!down_read_trylock(&file->hw_destroy_rwsem))
		return -EIO;
	mutex_lock(&file->ucontext_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (file->ucontext) {
		ret = -EINVAL;
		goto err;
	}

<<<<<<< HEAD
	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	ucontext = ibdev->alloc_ucontext(ibdev, &udata);
	if (IS_ERR(ucontext)) {
		ret = PTR_ERR(ucontext);
		goto err;
	}

	ucontext->device = ibdev;
	INIT_LIST_HEAD(&ucontext->pd_list);
	INIT_LIST_HEAD(&ucontext->mr_list);
	INIT_LIST_HEAD(&ucontext->mw_list);
	INIT_LIST_HEAD(&ucontext->cq_list);
	INIT_LIST_HEAD(&ucontext->qp_list);
	INIT_LIST_HEAD(&ucontext->srq_list);
	INIT_LIST_HEAD(&ucontext->ah_list);
	INIT_LIST_HEAD(&ucontext->xrcd_list);
	ucontext->closing = 0;

	resp.num_comp_vectors = file->device->num_comp_vectors;

	ret = get_unused_fd();
	if (ret < 0)
		goto err_free;
	resp.async_fd = ret;

	filp = ib_uverbs_alloc_event_file(file, 1);
	if (IS_ERR(filp)) {
		ret = PTR_ERR(filp);
		goto err_fd;
	}

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_file;
	}

	file->async_file = filp->private_data;

	INIT_IB_EVENT_HANDLER(&file->event_handler, file->device->ib_dev,
			      ib_uverbs_event_handler);
	ret = ib_register_event_handler(&file->event_handler);
	if (ret)
		goto err_file;

	kref_get(&file->async_file->ref);
	kref_get(&file->ref);
	file->ucontext = ucontext;

	fd_install(resp.async_fd, filp);

	mutex_unlock(&file->mutex);

	return in_len;

err_file:
	fput(filp);

err_fd:
	put_unused_fd(resp.async_fd);

err_free:
	ibdev->dealloc_ucontext(ucontext);

err:
	mutex_unlock(&file->mutex);
	return ret;
}

ssize_t ib_uverbs_query_device(struct ib_uverbs_file *file,
			       const char __user *buf,
			       int in_len, int out_len)
{
	struct ib_uverbs_query_device      cmd;
	struct ib_uverbs_query_device_resp resp;
	struct ib_device_attr              attr;
	int                                ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	ret = ib_query_device(file->device->ib_dev, &attr);
=======
	ret = ib_rdmacg_try_charge(&ucontext->cg_obj, ucontext->device,
				   RDMACG_RESOURCE_HCA_HANDLE);
	if (ret)
		goto err;

	ret = ucontext->device->ops.alloc_ucontext(ucontext,
						   &attrs->driver_udata);
	if (ret)
		goto err_uncharge;

	rdma_restrack_add(&ucontext->res);

	/*
	 * Make sure that ib_uverbs_get_ucontext() sees the pointer update
	 * only after all writes to setup the ucontext have completed
	 */
	smp_store_release(&file->ucontext, ucontext);

	mutex_unlock(&file->ucontext_lock);
	up_read(&file->hw_destroy_rwsem);
	return 0;

err_uncharge:
	ib_rdmacg_uncharge(&ucontext->cg_obj, ucontext->device,
			   RDMACG_RESOURCE_HCA_HANDLE);
err:
	mutex_unlock(&file->ucontext_lock);
	up_read(&file->hw_destroy_rwsem);
	return ret;
}

static int ib_uverbs_get_context(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_get_context_resp resp;
	struct ib_uverbs_get_context cmd;
	struct ib_device *ib_dev;
	struct ib_uobject *uobj;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	ret = ib_alloc_ucontext(attrs);
	if (ret)
		return ret;

	uobj = uobj_alloc(UVERBS_OBJECT_ASYNC_EVENT, attrs, &ib_dev);
	if (IS_ERR(uobj)) {
		ret = PTR_ERR(uobj);
		goto err_ucontext;
	}

	resp = (struct ib_uverbs_get_context_resp){
		.num_comp_vectors = attrs->ufile->device->num_comp_vectors,
		.async_fd = uobj->id,
	};
	ret = uverbs_response(attrs, &resp, sizeof(resp));
	if (ret)
		goto err_uobj;

	ret = ib_init_ucontext(attrs);
	if (ret)
		goto err_uobj;

	ib_uverbs_init_async_event_file(
		container_of(uobj, struct ib_uverbs_async_event_file, uobj));
	rdma_alloc_commit_uobject(uobj, attrs);
	return 0;

err_uobj:
	rdma_alloc_abort_uobject(uobj, attrs, false);
err_ucontext:
	rdma_restrack_put(&attrs->context->res);
	kfree(attrs->context);
	attrs->context = NULL;
	return ret;
}

static void copy_query_dev_fields(struct ib_ucontext *ucontext,
				  struct ib_uverbs_query_device_resp *resp,
				  struct ib_device_attr *attr)
{
	struct ib_device *ib_dev = ucontext->device;

	resp->fw_ver		= attr->fw_ver;
	resp->node_guid		= ib_dev->node_guid;
	resp->sys_image_guid	= attr->sys_image_guid;
	resp->max_mr_size	= attr->max_mr_size;
	resp->page_size_cap	= attr->page_size_cap;
	resp->vendor_id		= attr->vendor_id;
	resp->vendor_part_id	= attr->vendor_part_id;
	resp->hw_ver		= attr->hw_ver;
	resp->max_qp		= attr->max_qp;
	resp->max_qp_wr		= attr->max_qp_wr;
	resp->device_cap_flags  = lower_32_bits(attr->device_cap_flags);
	resp->max_sge		= min(attr->max_send_sge, attr->max_recv_sge);
	resp->max_sge_rd	= attr->max_sge_rd;
	resp->max_cq		= attr->max_cq;
	resp->max_cqe		= attr->max_cqe;
	resp->max_mr		= attr->max_mr;
	resp->max_pd		= attr->max_pd;
	resp->max_qp_rd_atom	= attr->max_qp_rd_atom;
	resp->max_ee_rd_atom	= attr->max_ee_rd_atom;
	resp->max_res_rd_atom	= attr->max_res_rd_atom;
	resp->max_qp_init_rd_atom	= attr->max_qp_init_rd_atom;
	resp->max_ee_init_rd_atom	= attr->max_ee_init_rd_atom;
	resp->atomic_cap		= attr->atomic_cap;
	resp->max_ee			= attr->max_ee;
	resp->max_rdd			= attr->max_rdd;
	resp->max_mw			= attr->max_mw;
	resp->max_raw_ipv6_qp		= attr->max_raw_ipv6_qp;
	resp->max_raw_ethy_qp		= attr->max_raw_ethy_qp;
	resp->max_mcast_grp		= attr->max_mcast_grp;
	resp->max_mcast_qp_attach	= attr->max_mcast_qp_attach;
	resp->max_total_mcast_qp_attach	= attr->max_total_mcast_qp_attach;
	resp->max_ah			= attr->max_ah;
	resp->max_srq			= attr->max_srq;
	resp->max_srq_wr		= attr->max_srq_wr;
	resp->max_srq_sge		= attr->max_srq_sge;
	resp->max_pkeys			= attr->max_pkeys;
	resp->local_ca_ack_delay	= attr->local_ca_ack_delay;
	resp->phys_port_cnt = min_t(u32, ib_dev->phys_port_cnt, U8_MAX);
}

static int ib_uverbs_query_device(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_query_device      cmd;
	struct ib_uverbs_query_device_resp resp;
	struct ib_ucontext *ucontext;
	int ret;

	ucontext = ib_uverbs_get_ucontext(attrs);
	if (IS_ERR(ucontext))
		return PTR_ERR(ucontext);

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	memset(&resp, 0, sizeof resp);
<<<<<<< HEAD

	resp.fw_ver 		       = attr.fw_ver;
	resp.node_guid 		       = file->device->ib_dev->node_guid;
	resp.sys_image_guid 	       = attr.sys_image_guid;
	resp.max_mr_size 	       = attr.max_mr_size;
	resp.page_size_cap 	       = attr.page_size_cap;
	resp.vendor_id 		       = attr.vendor_id;
	resp.vendor_part_id 	       = attr.vendor_part_id;
	resp.hw_ver 		       = attr.hw_ver;
	resp.max_qp 		       = attr.max_qp;
	resp.max_qp_wr 		       = attr.max_qp_wr;
	resp.device_cap_flags 	       = attr.device_cap_flags;
	resp.max_sge 		       = attr.max_sge;
	resp.max_sge_rd 	       = attr.max_sge_rd;
	resp.max_cq 		       = attr.max_cq;
	resp.max_cqe 		       = attr.max_cqe;
	resp.max_mr 		       = attr.max_mr;
	resp.max_pd 		       = attr.max_pd;
	resp.max_qp_rd_atom 	       = attr.max_qp_rd_atom;
	resp.max_ee_rd_atom 	       = attr.max_ee_rd_atom;
	resp.max_res_rd_atom 	       = attr.max_res_rd_atom;
	resp.max_qp_init_rd_atom       = attr.max_qp_init_rd_atom;
	resp.max_ee_init_rd_atom       = attr.max_ee_init_rd_atom;
	resp.atomic_cap 	       = attr.atomic_cap;
	resp.max_ee 		       = attr.max_ee;
	resp.max_rdd 		       = attr.max_rdd;
	resp.max_mw 		       = attr.max_mw;
	resp.max_raw_ipv6_qp 	       = attr.max_raw_ipv6_qp;
	resp.max_raw_ethy_qp 	       = attr.max_raw_ethy_qp;
	resp.max_mcast_grp 	       = attr.max_mcast_grp;
	resp.max_mcast_qp_attach       = attr.max_mcast_qp_attach;
	resp.max_total_mcast_qp_attach = attr.max_total_mcast_qp_attach;
	resp.max_ah 		       = attr.max_ah;
	resp.max_fmr 		       = attr.max_fmr;
	resp.max_map_per_fmr 	       = attr.max_map_per_fmr;
	resp.max_srq 		       = attr.max_srq;
	resp.max_srq_wr 	       = attr.max_srq_wr;
	resp.max_srq_sge 	       = attr.max_srq_sge;
	resp.max_pkeys 		       = attr.max_pkeys;
	resp.local_ca_ack_delay        = attr.local_ca_ack_delay;
	resp.phys_port_cnt	       = file->device->ib_dev->phys_port_cnt;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		return -EFAULT;

	return in_len;
}

ssize_t ib_uverbs_query_port(struct ib_uverbs_file *file,
			     const char __user *buf,
			     int in_len, int out_len)
=======
	copy_query_dev_fields(ucontext, &resp, &ucontext->device->attrs);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_query_port(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_query_port      cmd;
	struct ib_uverbs_query_port_resp resp;
	struct ib_port_attr              attr;
	int                              ret;
<<<<<<< HEAD

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	ret = ib_query_port(file->device->ib_dev, cmd.port_num, &attr);
=======
	struct ib_ucontext *ucontext;
	struct ib_device *ib_dev;

	ucontext = ib_uverbs_get_ucontext(attrs);
	if (IS_ERR(ucontext))
		return PTR_ERR(ucontext);
	ib_dev = ucontext->device;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	ret = ib_query_port(ib_dev, cmd.port_num, &attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	memset(&resp, 0, sizeof resp);
<<<<<<< HEAD

	resp.state 	     = attr.state;
	resp.max_mtu 	     = attr.max_mtu;
	resp.active_mtu      = attr.active_mtu;
	resp.gid_tbl_len     = attr.gid_tbl_len;
	resp.port_cap_flags  = attr.port_cap_flags;
	resp.max_msg_sz      = attr.max_msg_sz;
	resp.bad_pkey_cntr   = attr.bad_pkey_cntr;
	resp.qkey_viol_cntr  = attr.qkey_viol_cntr;
	resp.pkey_tbl_len    = attr.pkey_tbl_len;
	resp.lid 	     = attr.lid;
	resp.sm_lid 	     = attr.sm_lid;
	resp.lmc 	     = attr.lmc;
	resp.max_vl_num      = attr.max_vl_num;
	resp.sm_sl 	     = attr.sm_sl;
	resp.subnet_timeout  = attr.subnet_timeout;
	resp.init_type_reply = attr.init_type_reply;
	resp.active_width    = attr.active_width;
	resp.active_speed    = attr.active_speed;
	resp.phys_state      = attr.phys_state;
	resp.link_layer      = rdma_port_get_link_layer(file->device->ib_dev,
							cmd.port_num);

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		return -EFAULT;

	return in_len;
}

ssize_t ib_uverbs_alloc_pd(struct ib_uverbs_file *file,
			   const char __user *buf,
			   int in_len, int out_len)
{
	struct ib_uverbs_alloc_pd      cmd;
	struct ib_uverbs_alloc_pd_resp resp;
	struct ib_udata                udata;
	struct ib_uobject             *uobj;
	struct ib_pd                  *pd;
	int                            ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	uobj = kmalloc(sizeof *uobj, GFP_KERNEL);
	if (!uobj)
		return -ENOMEM;

	init_uobj(uobj, 0, file->ucontext, &pd_lock_key);
	down_write(&uobj->mutex);

	pd = file->device->ib_dev->alloc_pd(file->device->ib_dev,
					    file->ucontext, &udata);
	if (IS_ERR(pd)) {
		ret = PTR_ERR(pd);
		goto err;
	}

	pd->device  = file->device->ib_dev;
	pd->uobject = uobj;
	atomic_set(&pd->usecnt, 0);

	uobj->object = pd;
	ret = idr_add_uobj(&ib_uverbs_pd_idr, uobj);
	if (ret)
		goto err_idr;

	memset(&resp, 0, sizeof resp);
	resp.pd_handle = uobj->id;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	mutex_lock(&file->mutex);
	list_add_tail(&uobj->list, &file->ucontext->pd_list);
	mutex_unlock(&file->mutex);

	uobj->live = 1;

	up_write(&uobj->mutex);

	return in_len;

err_copy:
	idr_remove_uobj(&ib_uverbs_pd_idr, uobj);

err_idr:
	ib_dealloc_pd(pd);

err:
	put_uobj_write(uobj);
	return ret;
}

ssize_t ib_uverbs_dealloc_pd(struct ib_uverbs_file *file,
			     const char __user *buf,
			     int in_len, int out_len)
{
	struct ib_uverbs_dealloc_pd cmd;
	struct ib_uobject          *uobj;
	int                         ret;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = idr_write_uobj(&ib_uverbs_pd_idr, cmd.pd_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;

	ret = ib_dealloc_pd(uobj->object);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_pd_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	put_uobj(uobj);

	return in_len;
=======
	copy_port_attr_to_resp(&attr, &resp, ib_dev, cmd.port_num);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_alloc_pd(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_alloc_pd_resp resp = {};
	struct ib_uverbs_alloc_pd      cmd;
	struct ib_uobject             *uobj;
	struct ib_pd                  *pd;
	int                            ret;
	struct ib_device *ib_dev;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_alloc(UVERBS_OBJECT_PD, attrs, &ib_dev);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	pd = rdma_zalloc_drv_obj(ib_dev, ib_pd);
	if (!pd) {
		ret = -ENOMEM;
		goto err;
	}

	pd->device  = ib_dev;
	pd->uobject = uobj;
	atomic_set(&pd->usecnt, 0);

	rdma_restrack_new(&pd->res, RDMA_RESTRACK_PD);
	rdma_restrack_set_name(&pd->res, NULL);

	ret = ib_dev->ops.alloc_pd(pd, &attrs->driver_udata);
	if (ret)
		goto err_alloc;
	rdma_restrack_add(&pd->res);

	uobj->object = pd;
	uobj_finalize_uobj_create(uobj, attrs);

	resp.pd_handle = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_alloc:
	rdma_restrack_put(&pd->res);
	kfree(pd);
err:
	uobj_alloc_abort(uobj, attrs);
	return ret;
}

static int ib_uverbs_dealloc_pd(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_dealloc_pd cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return uobj_perform_destroy(UVERBS_OBJECT_PD, cmd.pd_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct xrcd_table_entry {
	struct rb_node  node;
	struct ib_xrcd *xrcd;
	struct inode   *inode;
};

static int xrcd_table_insert(struct ib_uverbs_device *dev,
			    struct inode *inode,
			    struct ib_xrcd *xrcd)
{
	struct xrcd_table_entry *entry, *scan;
	struct rb_node **p = &dev->xrcd_tree.rb_node;
	struct rb_node *parent = NULL;

	entry = kmalloc(sizeof *entry, GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->xrcd  = xrcd;
	entry->inode = inode;

	while (*p) {
		parent = *p;
		scan = rb_entry(parent, struct xrcd_table_entry, node);

		if (inode < scan->inode) {
			p = &(*p)->rb_left;
		} else if (inode > scan->inode) {
			p = &(*p)->rb_right;
		} else {
			kfree(entry);
			return -EEXIST;
		}
	}

	rb_link_node(&entry->node, parent, p);
	rb_insert_color(&entry->node, &dev->xrcd_tree);
	igrab(inode);
	return 0;
}

static struct xrcd_table_entry *xrcd_table_search(struct ib_uverbs_device *dev,
						  struct inode *inode)
{
	struct xrcd_table_entry *entry;
	struct rb_node *p = dev->xrcd_tree.rb_node;

	while (p) {
		entry = rb_entry(p, struct xrcd_table_entry, node);

		if (inode < entry->inode)
			p = p->rb_left;
		else if (inode > entry->inode)
			p = p->rb_right;
		else
			return entry;
	}

	return NULL;
}

static struct ib_xrcd *find_xrcd(struct ib_uverbs_device *dev, struct inode *inode)
{
	struct xrcd_table_entry *entry;

	entry = xrcd_table_search(dev, inode);
	if (!entry)
		return NULL;

	return entry->xrcd;
}

static void xrcd_table_delete(struct ib_uverbs_device *dev,
			      struct inode *inode)
{
	struct xrcd_table_entry *entry;

	entry = xrcd_table_search(dev, inode);
	if (entry) {
		iput(inode);
		rb_erase(&entry->node, &dev->xrcd_tree);
		kfree(entry);
	}
}

<<<<<<< HEAD
ssize_t ib_uverbs_open_xrcd(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_open_xrcd	cmd;
	struct ib_uverbs_open_xrcd_resp	resp;
	struct ib_udata			udata;
	struct ib_uxrcd_object         *obj;
	struct ib_xrcd                 *xrcd = NULL;
	struct file                    *f = NULL;
	struct inode                   *inode = NULL;
	int				ret = 0;
	int				new_xrcd = 0;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof  resp);

	mutex_lock(&file->device->xrcd_tree_mutex);

	if (cmd.fd != -1) {
		/* search for file descriptor */
		f = fget(cmd.fd);
		if (!f) {
=======
static int ib_uverbs_open_xrcd(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_device *ibudev = attrs->ufile->device;
	struct ib_uverbs_open_xrcd_resp	resp = {};
	struct ib_uverbs_open_xrcd	cmd;
	struct ib_uxrcd_object         *obj;
	struct ib_xrcd                 *xrcd = NULL;
	struct inode                   *inode = NULL;
	int				new_xrcd = 0;
	struct ib_device *ib_dev;
	struct fd f = {};
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	mutex_lock(&ibudev->xrcd_tree_mutex);

	if (cmd.fd != -1) {
		/* search for file descriptor */
		f = fdget(cmd.fd);
		if (!f.file) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EBADF;
			goto err_tree_mutex_unlock;
		}

<<<<<<< HEAD
		inode = f->f_dentry->d_inode;
		if (!inode) {
			ret = -EBADF;
			goto err_tree_mutex_unlock;
		}

		xrcd = find_xrcd(file->device, inode);
=======
		inode = file_inode(f.file);
		xrcd = find_xrcd(ibudev, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!xrcd && !(cmd.oflags & O_CREAT)) {
			/* no file descriptor. Need CREATE flag */
			ret = -EAGAIN;
			goto err_tree_mutex_unlock;
		}

		if (xrcd && cmd.oflags & O_EXCL) {
			ret = -EINVAL;
			goto err_tree_mutex_unlock;
		}
	}

<<<<<<< HEAD
	obj = kmalloc(sizeof *obj, GFP_KERNEL);
	if (!obj) {
		ret = -ENOMEM;
		goto err_tree_mutex_unlock;
	}

	init_uobj(&obj->uobject, 0, file->ucontext, &xrcd_lock_key);

	down_write(&obj->uobject.mutex);

	if (!xrcd) {
		xrcd = file->device->ib_dev->alloc_xrcd(file->device->ib_dev,
							file->ucontext, &udata);
=======
	obj = (struct ib_uxrcd_object *)uobj_alloc(UVERBS_OBJECT_XRCD, attrs,
						   &ib_dev);
	if (IS_ERR(obj)) {
		ret = PTR_ERR(obj);
		goto err_tree_mutex_unlock;
	}

	if (!xrcd) {
		xrcd = ib_alloc_xrcd_user(ib_dev, inode, &attrs->driver_udata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(xrcd)) {
			ret = PTR_ERR(xrcd);
			goto err;
		}
<<<<<<< HEAD

		xrcd->inode   = inode;
		xrcd->device  = file->device->ib_dev;
		atomic_set(&xrcd->usecnt, 0);
		mutex_init(&xrcd->tgt_qp_mutex);
		INIT_LIST_HEAD(&xrcd->tgt_qp_list);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		new_xrcd = 1;
	}

	atomic_set(&obj->refcnt, 0);
	obj->uobject.object = xrcd;
<<<<<<< HEAD
	ret = idr_add_uobj(&ib_uverbs_xrcd_idr, &obj->uobject);
	if (ret)
		goto err_idr;

	memset(&resp, 0, sizeof resp);
	resp.xrcd_handle = obj->uobject.id;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (inode) {
		if (new_xrcd) {
			/* create new inode/xrcd table entry */
<<<<<<< HEAD
			ret = xrcd_table_insert(file->device, inode, xrcd);
			if (ret)
				goto err_insert_xrcd;
=======
			ret = xrcd_table_insert(ibudev, inode, xrcd);
			if (ret)
				goto err_dealloc_xrcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		atomic_inc(&xrcd->usecnt);
	}

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	if (f)
		fput(f);

	mutex_lock(&file->mutex);
	list_add_tail(&obj->uobject.list, &file->ucontext->xrcd_list);
	mutex_unlock(&file->mutex);

	obj->uobject.live = 1;
	up_write(&obj->uobject.mutex);

	mutex_unlock(&file->device->xrcd_tree_mutex);
	return in_len;

err_copy:
	if (inode) {
		if (new_xrcd)
			xrcd_table_delete(file->device, inode);
		atomic_dec(&xrcd->usecnt);
	}

err_insert_xrcd:
	idr_remove_uobj(&ib_uverbs_xrcd_idr, &obj->uobject);

err_idr:
	ib_dealloc_xrcd(xrcd);

err:
	put_uobj_write(&obj->uobject);

err_tree_mutex_unlock:
	if (f)
		fput(f);

	mutex_unlock(&file->device->xrcd_tree_mutex);
=======
	if (f.file)
		fdput(f);

	mutex_unlock(&ibudev->xrcd_tree_mutex);
	uobj_finalize_uobj_create(&obj->uobject, attrs);

	resp.xrcd_handle = obj->uobject.id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_dealloc_xrcd:
	ib_dealloc_xrcd_user(xrcd, uverbs_get_cleared_udata(attrs));

err:
	uobj_alloc_abort(&obj->uobject, attrs);

err_tree_mutex_unlock:
	if (f.file)
		fdput(f);

	mutex_unlock(&ibudev->xrcd_tree_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
ssize_t ib_uverbs_close_xrcd(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len,
			     int out_len)
{
	struct ib_uverbs_close_xrcd cmd;
	struct ib_uobject           *uobj;
	struct ib_xrcd              *xrcd = NULL;
	struct inode                *inode = NULL;
	struct ib_uxrcd_object      *obj;
	int                         live;
	int                         ret = 0;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	mutex_lock(&file->device->xrcd_tree_mutex);
	uobj = idr_write_uobj(&ib_uverbs_xrcd_idr, cmd.xrcd_handle, file->ucontext);
	if (!uobj) {
		ret = -EINVAL;
		goto out;
	}

	xrcd  = uobj->object;
	inode = xrcd->inode;
	obj   = container_of(uobj, struct ib_uxrcd_object, uobject);
	if (atomic_read(&obj->refcnt)) {
		put_uobj_write(uobj);
		ret = -EBUSY;
		goto out;
	}

	if (!inode || atomic_dec_and_test(&xrcd->usecnt)) {
		ret = ib_dealloc_xrcd(uobj->object);
		if (!ret)
			uobj->live = 0;
	}

	live = uobj->live;
	if (inode && ret)
		atomic_inc(&xrcd->usecnt);

	put_uobj_write(uobj);

	if (ret)
		goto out;

	if (inode && !live)
		xrcd_table_delete(file->device, inode);

	idr_remove_uobj(&ib_uverbs_xrcd_idr, uobj);
	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	put_uobj(uobj);
	ret = in_len;

out:
	mutex_unlock(&file->device->xrcd_tree_mutex);
	return ret;
}

void ib_uverbs_dealloc_xrcd(struct ib_uverbs_device *dev,
			    struct ib_xrcd *xrcd)
{
	struct inode *inode;

	inode = xrcd->inode;
	if (inode && !atomic_dec_and_test(&xrcd->usecnt))
		return;

	ib_dealloc_xrcd(xrcd);

	if (inode)
		xrcd_table_delete(dev, inode);
}

ssize_t ib_uverbs_reg_mr(struct ib_uverbs_file *file,
			 const char __user *buf, int in_len,
			 int out_len)
{
	struct ib_uverbs_reg_mr      cmd;
	struct ib_uverbs_reg_mr_resp resp;
	struct ib_udata              udata;
=======
static int ib_uverbs_close_xrcd(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_close_xrcd cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return uobj_perform_destroy(UVERBS_OBJECT_XRCD, cmd.xrcd_handle, attrs);
}

int ib_uverbs_dealloc_xrcd(struct ib_uobject *uobject, struct ib_xrcd *xrcd,
			   enum rdma_remove_reason why,
			   struct uverbs_attr_bundle *attrs)
{
	struct inode *inode;
	int ret;
	struct ib_uverbs_device *dev = attrs->ufile->device;

	inode = xrcd->inode;
	if (inode && !atomic_dec_and_test(&xrcd->usecnt))
		return 0;

	ret = ib_dealloc_xrcd_user(xrcd, &attrs->driver_udata);
	if (ret) {
		atomic_inc(&xrcd->usecnt);
		return ret;
	}

	if (inode)
		xrcd_table_delete(dev, inode);

	return 0;
}

static int ib_uverbs_reg_mr(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_reg_mr_resp resp = {};
	struct ib_uverbs_reg_mr      cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ib_uobject           *uobj;
	struct ib_pd                *pd;
	struct ib_mr                *mr;
	int                          ret;
<<<<<<< HEAD

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);
=======
	struct ib_device *ib_dev;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((cmd.start & ~PAGE_MASK) != (cmd.hca_va & ~PAGE_MASK))
		return -EINVAL;

<<<<<<< HEAD
	/*
	 * Local write permission is required if remote write or
	 * remote atomic permission is also requested.
	 */
	if (cmd.access_flags & (IB_ACCESS_REMOTE_ATOMIC | IB_ACCESS_REMOTE_WRITE) &&
	    !(cmd.access_flags & IB_ACCESS_LOCAL_WRITE))
		return -EINVAL;

	uobj = kmalloc(sizeof *uobj, GFP_KERNEL);
	if (!uobj)
		return -ENOMEM;

	init_uobj(uobj, 0, file->ucontext, &mr_lock_key);
	down_write(&uobj->mutex);

	pd = idr_read_pd(cmd.pd_handle, file->ucontext);
=======
	uobj = uobj_alloc(UVERBS_OBJECT_MR, attrs, &ib_dev);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	ret = ib_check_mr_access(ib_dev, cmd.access_flags);
	if (ret)
		goto err_free;

	pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd.pd_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pd) {
		ret = -EINVAL;
		goto err_free;
	}

<<<<<<< HEAD
	mr = pd->device->reg_user_mr(pd, cmd.start, cmd.length, cmd.hca_va,
				     cmd.access_flags, &udata);
=======
	mr = pd->device->ops.reg_user_mr(pd, cmd.start, cmd.length, cmd.hca_va,
					 cmd.access_flags,
					 &attrs->driver_udata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(mr)) {
		ret = PTR_ERR(mr);
		goto err_put;
	}

	mr->device  = pd->device;
	mr->pd      = pd;
<<<<<<< HEAD
	mr->uobject = uobj;
	atomic_inc(&pd->usecnt);
	atomic_set(&mr->usecnt, 0);

	uobj->object = mr;
	ret = idr_add_uobj(&ib_uverbs_mr_idr, uobj);
	if (ret)
		goto err_unreg;

	memset(&resp, 0, sizeof resp);
	resp.lkey      = mr->lkey;
	resp.rkey      = mr->rkey;
	resp.mr_handle = uobj->id;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	put_pd_read(pd);

	mutex_lock(&file->mutex);
	list_add_tail(&uobj->list, &file->ucontext->mr_list);
	mutex_unlock(&file->mutex);

	uobj->live = 1;

	up_write(&uobj->mutex);

	return in_len;

err_copy:
	idr_remove_uobj(&ib_uverbs_mr_idr, uobj);

err_unreg:
	ib_dereg_mr(mr);

err_put:
	put_pd_read(pd);

err_free:
	put_uobj_write(uobj);
	return ret;
}

ssize_t ib_uverbs_dereg_mr(struct ib_uverbs_file *file,
			   const char __user *buf, int in_len,
			   int out_len)
{
	struct ib_uverbs_dereg_mr cmd;
	struct ib_mr             *mr;
	struct ib_uobject	 *uobj;
	int                       ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = idr_write_uobj(&ib_uverbs_mr_idr, cmd.mr_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;

	mr = uobj->object;

	ret = ib_dereg_mr(mr);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_mr_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	put_uobj(uobj);

	return in_len;
}

ssize_t ib_uverbs_create_comp_channel(struct ib_uverbs_file *file,
				      const char __user *buf, int in_len,
				      int out_len)
{
	struct ib_uverbs_create_comp_channel	   cmd;
	struct ib_uverbs_create_comp_channel_resp  resp;
	struct file				  *filp;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	ret = get_unused_fd();
	if (ret < 0)
		return ret;
	resp.fd = ret;

	filp = ib_uverbs_alloc_event_file(file, 0);
	if (IS_ERR(filp)) {
		put_unused_fd(resp.fd);
		return PTR_ERR(filp);
	}

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		put_unused_fd(resp.fd);
		fput(filp);
		return -EFAULT;
	}

	fd_install(resp.fd, filp);
	return in_len;
}

ssize_t ib_uverbs_create_cq(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_create_cq      cmd;
	struct ib_uverbs_create_cq_resp resp;
	struct ib_udata                 udata;
	struct ib_ucq_object           *obj;
	struct ib_uverbs_event_file    *ev_file = NULL;
	struct ib_cq                   *cq;
	int                             ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	if (cmd.comp_vector >= file->device->num_comp_vectors)
		return -EINVAL;

	obj = kmalloc(sizeof *obj, GFP_KERNEL);
	if (!obj)
		return -ENOMEM;

	init_uobj(&obj->uobject, cmd.user_handle, file->ucontext, &cq_lock_key);
	down_write(&obj->uobject.mutex);

	if (cmd.comp_channel >= 0) {
		ev_file = ib_uverbs_lookup_comp_file(cmd.comp_channel);
		if (!ev_file) {
			ret = -EINVAL;
=======
	mr->type    = IB_MR_TYPE_USER;
	mr->dm	    = NULL;
	mr->sig_attrs = NULL;
	mr->uobject = uobj;
	atomic_inc(&pd->usecnt);
	mr->iova = cmd.hca_va;
	mr->length = cmd.length;

	rdma_restrack_new(&mr->res, RDMA_RESTRACK_MR);
	rdma_restrack_set_name(&mr->res, NULL);
	rdma_restrack_add(&mr->res);

	uobj->object = mr;
	uobj_put_obj_read(pd);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.lkey = mr->lkey;
	resp.rkey = mr->rkey;
	resp.mr_handle = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_put:
	uobj_put_obj_read(pd);
err_free:
	uobj_alloc_abort(uobj, attrs);
	return ret;
}

static int ib_uverbs_rereg_mr(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_rereg_mr      cmd;
	struct ib_uverbs_rereg_mr_resp resp;
	struct ib_mr                *mr;
	int                          ret;
	struct ib_uobject	    *uobj;
	struct ib_uobject *new_uobj;
	struct ib_device *ib_dev;
	struct ib_pd *orig_pd;
	struct ib_pd *new_pd;
	struct ib_mr *new_mr;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (!cmd.flags)
		return -EINVAL;

	if (cmd.flags & ~IB_MR_REREG_SUPPORTED)
		return -EOPNOTSUPP;

	if ((cmd.flags & IB_MR_REREG_TRANS) &&
	    (cmd.start & ~PAGE_MASK) != (cmd.hca_va & ~PAGE_MASK))
		return -EINVAL;

	uobj = uobj_get_write(UVERBS_OBJECT_MR, cmd.mr_handle, attrs);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	mr = uobj->object;

	if (mr->dm) {
		ret = -EINVAL;
		goto put_uobjs;
	}

	if (cmd.flags & IB_MR_REREG_ACCESS) {
		ret = ib_check_mr_access(mr->device, cmd.access_flags);
		if (ret)
			goto put_uobjs;
	}

	orig_pd = mr->pd;
	if (cmd.flags & IB_MR_REREG_PD) {
		new_pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd.pd_handle,
					   attrs);
		if (!new_pd) {
			ret = -EINVAL;
			goto put_uobjs;
		}
	} else {
		new_pd = mr->pd;
	}

	/*
	 * The driver might create a new HW object as part of the rereg, we need
	 * to have a uobject ready to hold it.
	 */
	new_uobj = uobj_alloc(UVERBS_OBJECT_MR, attrs, &ib_dev);
	if (IS_ERR(new_uobj)) {
		ret = PTR_ERR(new_uobj);
		goto put_uobj_pd;
	}

	new_mr = ib_dev->ops.rereg_user_mr(mr, cmd.flags, cmd.start, cmd.length,
					   cmd.hca_va, cmd.access_flags, new_pd,
					   &attrs->driver_udata);
	if (IS_ERR(new_mr)) {
		ret = PTR_ERR(new_mr);
		goto put_new_uobj;
	}
	if (new_mr) {
		new_mr->device = new_pd->device;
		new_mr->pd = new_pd;
		new_mr->type = IB_MR_TYPE_USER;
		new_mr->uobject = uobj;
		atomic_inc(&new_pd->usecnt);
		new_uobj->object = new_mr;

		rdma_restrack_new(&new_mr->res, RDMA_RESTRACK_MR);
		rdma_restrack_set_name(&new_mr->res, NULL);
		rdma_restrack_add(&new_mr->res);

		/*
		 * The new uobj for the new HW object is put into the same spot
		 * in the IDR and the old uobj & HW object is deleted.
		 */
		rdma_assign_uobject(uobj, new_uobj, attrs);
		rdma_alloc_commit_uobject(new_uobj, attrs);
		uobj_put_destroy(uobj);
		new_uobj = NULL;
		uobj = NULL;
		mr = new_mr;
	} else {
		if (cmd.flags & IB_MR_REREG_PD) {
			atomic_dec(&orig_pd->usecnt);
			mr->pd = new_pd;
			atomic_inc(&new_pd->usecnt);
		}
		if (cmd.flags & IB_MR_REREG_TRANS) {
			mr->iova = cmd.hca_va;
			mr->length = cmd.length;
		}
	}

	memset(&resp, 0, sizeof(resp));
	resp.lkey      = mr->lkey;
	resp.rkey      = mr->rkey;

	ret = uverbs_response(attrs, &resp, sizeof(resp));

put_new_uobj:
	if (new_uobj)
		uobj_alloc_abort(new_uobj, attrs);
put_uobj_pd:
	if (cmd.flags & IB_MR_REREG_PD)
		uobj_put_obj_read(new_pd);

put_uobjs:
	if (uobj)
		uobj_put_write(uobj);

	return ret;
}

static int ib_uverbs_dereg_mr(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_dereg_mr cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return uobj_perform_destroy(UVERBS_OBJECT_MR, cmd.mr_handle, attrs);
}

static int ib_uverbs_alloc_mw(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_alloc_mw      cmd;
	struct ib_uverbs_alloc_mw_resp resp = {};
	struct ib_uobject             *uobj;
	struct ib_pd                  *pd;
	struct ib_mw                  *mw;
	int                            ret;
	struct ib_device *ib_dev;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_alloc(UVERBS_OBJECT_MW, attrs, &ib_dev);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd.pd_handle, attrs);
	if (!pd) {
		ret = -EINVAL;
		goto err_free;
	}

	if (cmd.mw_type != IB_MW_TYPE_1 && cmd.mw_type != IB_MW_TYPE_2) {
		ret = -EINVAL;
		goto err_put;
	}

	mw = rdma_zalloc_drv_obj(ib_dev, ib_mw);
	if (!mw) {
		ret = -ENOMEM;
		goto err_put;
	}

	mw->device = ib_dev;
	mw->pd = pd;
	mw->uobject = uobj;
	mw->type = cmd.mw_type;

	ret = pd->device->ops.alloc_mw(mw, &attrs->driver_udata);
	if (ret)
		goto err_alloc;

	atomic_inc(&pd->usecnt);

	uobj->object = mw;
	uobj_put_obj_read(pd);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.rkey = mw->rkey;
	resp.mw_handle = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_alloc:
	kfree(mw);
err_put:
	uobj_put_obj_read(pd);
err_free:
	uobj_alloc_abort(uobj, attrs);
	return ret;
}

static int ib_uverbs_dealloc_mw(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_dealloc_mw cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return uobj_perform_destroy(UVERBS_OBJECT_MW, cmd.mw_handle, attrs);
}

static int ib_uverbs_create_comp_channel(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_comp_channel	   cmd;
	struct ib_uverbs_create_comp_channel_resp  resp;
	struct ib_uobject			  *uobj;
	struct ib_uverbs_completion_event_file	  *ev_file;
	struct ib_device *ib_dev;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_alloc(UVERBS_OBJECT_COMP_CHANNEL, attrs, &ib_dev);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	ev_file = container_of(uobj, struct ib_uverbs_completion_event_file,
			       uobj);
	ib_uverbs_init_event_queue(&ev_file->ev_queue);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.fd = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int create_cq(struct uverbs_attr_bundle *attrs,
		     struct ib_uverbs_ex_create_cq *cmd)
{
	struct ib_ucq_object           *obj;
	struct ib_uverbs_completion_event_file    *ev_file = NULL;
	struct ib_cq                   *cq;
	int                             ret;
	struct ib_uverbs_ex_create_cq_resp resp = {};
	struct ib_cq_init_attr attr = {};
	struct ib_device *ib_dev;

	if (cmd->comp_vector >= attrs->ufile->device->num_comp_vectors)
		return -EINVAL;

	obj = (struct ib_ucq_object *)uobj_alloc(UVERBS_OBJECT_CQ, attrs,
						 &ib_dev);
	if (IS_ERR(obj))
		return PTR_ERR(obj);

	if (cmd->comp_channel >= 0) {
		ev_file = ib_uverbs_lookup_comp_file(cmd->comp_channel, attrs);
		if (IS_ERR(ev_file)) {
			ret = PTR_ERR(ev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto err;
		}
	}

<<<<<<< HEAD
	obj->uverbs_file	   = file;
	obj->comp_events_reported  = 0;
	obj->async_events_reported = 0;
	INIT_LIST_HEAD(&obj->comp_list);
	INIT_LIST_HEAD(&obj->async_list);

	cq = file->device->ib_dev->create_cq(file->device->ib_dev, cmd.cqe,
					     cmd.comp_vector,
					     file->ucontext, &udata);
	if (IS_ERR(cq)) {
		ret = PTR_ERR(cq);
		goto err_file;
	}

	cq->device        = file->device->ib_dev;
	cq->uobject       = &obj->uobject;
	cq->comp_handler  = ib_uverbs_comp_handler;
	cq->event_handler = ib_uverbs_cq_event_handler;
	cq->cq_context    = ev_file;
	atomic_set(&cq->usecnt, 0);

	obj->uobject.object = cq;
	ret = idr_add_uobj(&ib_uverbs_cq_idr, &obj->uobject);
	if (ret)
		goto err_free;

	memset(&resp, 0, sizeof resp);
	resp.cq_handle = obj->uobject.id;
	resp.cqe       = cq->cqe;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	mutex_lock(&file->mutex);
	list_add_tail(&obj->uobject.list, &file->ucontext->cq_list);
	mutex_unlock(&file->mutex);

	obj->uobject.live = 1;

	up_write(&obj->uobject.mutex);

	return in_len;

err_copy:
	idr_remove_uobj(&ib_uverbs_cq_idr, &obj->uobject);

err_free:
	ib_destroy_cq(cq);

err_file:
	if (ev_file)
		ib_uverbs_release_ucq(file, ev_file, obj);

err:
	put_uobj_write(&obj->uobject);
	return ret;
}

ssize_t ib_uverbs_resize_cq(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_resize_cq	cmd;
	struct ib_uverbs_resize_cq_resp	resp;
	struct ib_udata                 udata;
	struct ib_cq			*cq;
	int				ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	cq = idr_read_cq(cmd.cq_handle, file->ucontext, 0);
	if (!cq)
		return -EINVAL;

	ret = cq->device->resize_cq(cq, cmd.cqe, &udata);
=======
	obj->uevent.uobject.user_handle = cmd->user_handle;
	INIT_LIST_HEAD(&obj->comp_list);
	INIT_LIST_HEAD(&obj->uevent.event_list);

	attr.cqe = cmd->cqe;
	attr.comp_vector = cmd->comp_vector;
	attr.flags = cmd->flags;

	cq = rdma_zalloc_drv_obj(ib_dev, ib_cq);
	if (!cq) {
		ret = -ENOMEM;
		goto err_file;
	}
	cq->device        = ib_dev;
	cq->uobject       = obj;
	cq->comp_handler  = ib_uverbs_comp_handler;
	cq->event_handler = ib_uverbs_cq_event_handler;
	cq->cq_context    = ev_file ? &ev_file->ev_queue : NULL;
	atomic_set(&cq->usecnt, 0);

	rdma_restrack_new(&cq->res, RDMA_RESTRACK_CQ);
	rdma_restrack_set_name(&cq->res, NULL);

	ret = ib_dev->ops.create_cq(cq, &attr, &attrs->driver_udata);
	if (ret)
		goto err_free;
	rdma_restrack_add(&cq->res);

	obj->uevent.uobject.object = cq;
	obj->uevent.event_file = READ_ONCE(attrs->ufile->default_async_file);
	if (obj->uevent.event_file)
		uverbs_uobject_get(&obj->uevent.event_file->uobj);
	uobj_finalize_uobj_create(&obj->uevent.uobject, attrs);

	resp.base.cq_handle = obj->uevent.uobject.id;
	resp.base.cqe = cq->cqe;
	resp.response_length = uverbs_response_length(attrs, sizeof(resp));
	return uverbs_response(attrs, &resp, sizeof(resp));

err_free:
	rdma_restrack_put(&cq->res);
	kfree(cq);
err_file:
	if (ev_file)
		ib_uverbs_release_ucq(ev_file, obj);
err:
	uobj_alloc_abort(&obj->uevent.uobject, attrs);
	return ret;
}

static int ib_uverbs_create_cq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_cq      cmd;
	struct ib_uverbs_ex_create_cq	cmd_ex;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	memset(&cmd_ex, 0, sizeof(cmd_ex));
	cmd_ex.user_handle = cmd.user_handle;
	cmd_ex.cqe = cmd.cqe;
	cmd_ex.comp_vector = cmd.comp_vector;
	cmd_ex.comp_channel = cmd.comp_channel;

	return create_cq(attrs, &cmd_ex);
}

static int ib_uverbs_ex_create_cq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_create_cq  cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (cmd.comp_mask)
		return -EINVAL;

	if (cmd.reserved)
		return -EINVAL;

	return create_cq(attrs, &cmd);
}

static int ib_uverbs_resize_cq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_resize_cq	cmd;
	struct ib_uverbs_resize_cq_resp	resp = {};
	struct ib_cq			*cq;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
	if (!cq)
		return -EINVAL;

	ret = cq->device->ops.resize_cq(cq, cmd.cqe, &attrs->driver_udata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out;

	resp.cqe = cq->cqe;

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp.cqe))
		ret = -EFAULT;

out:
	put_cq_read(cq);

	return ret ? ret : in_len;
}

static int copy_wc_to_user(void __user *dest, struct ib_wc *wc)
=======
	ret = uverbs_response(attrs, &resp, sizeof(resp));
out:
	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);

	return ret;
}

static int copy_wc_to_user(struct ib_device *ib_dev, void __user *dest,
			   struct ib_wc *wc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_wc tmp;

	tmp.wr_id		= wc->wr_id;
	tmp.status		= wc->status;
	tmp.opcode		= wc->opcode;
	tmp.vendor_err		= wc->vendor_err;
	tmp.byte_len		= wc->byte_len;
<<<<<<< HEAD
	tmp.ex.imm_data		= (__u32 __force) wc->ex.imm_data;
=======
	tmp.ex.imm_data		= wc->ex.imm_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tmp.qp_num		= wc->qp->qp_num;
	tmp.src_qp		= wc->src_qp;
	tmp.wc_flags		= wc->wc_flags;
	tmp.pkey_index		= wc->pkey_index;
<<<<<<< HEAD
	tmp.slid		= wc->slid;
=======
	if (rdma_cap_opa_ah(ib_dev, wc->port_num))
		tmp.slid	= OPA_TO_IB_UCAST_LID(wc->slid);
	else
		tmp.slid	= ib_lid_cpu16(wc->slid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tmp.sl			= wc->sl;
	tmp.dlid_path_bits	= wc->dlid_path_bits;
	tmp.port_num		= wc->port_num;
	tmp.reserved		= 0;

	if (copy_to_user(dest, &tmp, sizeof tmp))
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
ssize_t ib_uverbs_poll_cq(struct ib_uverbs_file *file,
			  const char __user *buf, int in_len,
			  int out_len)
=======
static int ib_uverbs_poll_cq(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_poll_cq       cmd;
	struct ib_uverbs_poll_cq_resp  resp;
	u8 __user                     *header_ptr;
	u8 __user                     *data_ptr;
	struct ib_cq                  *cq;
	struct ib_wc                   wc;
	int                            ret;

<<<<<<< HEAD
	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	cq = idr_read_cq(cmd.cq_handle, file->ucontext, 0);
=======
	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!cq)
		return -EINVAL;

	/* we copy a struct ib_uverbs_poll_cq_resp to user space */
<<<<<<< HEAD
	header_ptr = (void __user *)(unsigned long) cmd.response;
=======
	header_ptr = attrs->ucore.outbuf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data_ptr = header_ptr + sizeof resp;

	memset(&resp, 0, sizeof resp);
	while (resp.count < cmd.ne) {
		ret = ib_poll_cq(cq, 1, &wc);
		if (ret < 0)
			goto out_put;
		if (!ret)
			break;

<<<<<<< HEAD
		ret = copy_wc_to_user(data_ptr, &wc);
=======
		ret = copy_wc_to_user(cq->device, data_ptr, &wc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			goto out_put;

		data_ptr += sizeof(struct ib_uverbs_wc);
		++resp.count;
	}

	if (copy_to_user(header_ptr, &resp, sizeof resp)) {
		ret = -EFAULT;
		goto out_put;
	}
<<<<<<< HEAD

	ret = in_len;

out_put:
	put_cq_read(cq);
	return ret;
}

ssize_t ib_uverbs_req_notify_cq(struct ib_uverbs_file *file,
				const char __user *buf, int in_len,
				int out_len)
{
	struct ib_uverbs_req_notify_cq cmd;
	struct ib_cq                  *cq;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	cq = idr_read_cq(cmd.cq_handle, file->ucontext, 0);
=======
	ret = 0;

	if (uverbs_attr_is_valid(attrs, UVERBS_ATTR_CORE_OUT))
		ret = uverbs_output_written(attrs, UVERBS_ATTR_CORE_OUT);

out_put:
	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	return ret;
}

static int ib_uverbs_req_notify_cq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_req_notify_cq cmd;
	struct ib_cq                  *cq;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!cq)
		return -EINVAL;

	ib_req_notify_cq(cq, cmd.solicited_only ?
			 IB_CQ_SOLICITED : IB_CQ_NEXT_COMP);

<<<<<<< HEAD
	put_cq_read(cq);

	return in_len;
}

ssize_t ib_uverbs_destroy_cq(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len,
			     int out_len)
=======
	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	return 0;
}

static int ib_uverbs_destroy_cq(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_destroy_cq      cmd;
	struct ib_uverbs_destroy_cq_resp resp;
	struct ib_uobject		*uobj;
<<<<<<< HEAD
	struct ib_cq               	*cq;
	struct ib_ucq_object        	*obj;
	struct ib_uverbs_event_file	*ev_file;
	int                        	 ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = idr_write_uobj(&ib_uverbs_cq_idr, cmd.cq_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;
	cq      = uobj->object;
	ev_file = cq->cq_context;
	obj     = container_of(cq->uobject, struct ib_ucq_object, uobject);

	ret = ib_destroy_cq(cq);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_cq_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	ib_uverbs_release_ucq(file, ev_file, obj);

	memset(&resp, 0, sizeof resp);
	resp.comp_events_reported  = obj->comp_events_reported;
	resp.async_events_reported = obj->async_events_reported;

	put_uobj(uobj);

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		return -EFAULT;

	return in_len;
}

ssize_t ib_uverbs_create_qp(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_create_qp      cmd;
	struct ib_uverbs_create_qp_resp resp;
	struct ib_udata                 udata;
	struct ib_uqp_object           *obj;
	struct ib_device	       *device;
	struct ib_pd                   *pd = NULL;
	struct ib_xrcd		       *xrcd = NULL;
	struct ib_uobject	       *uninitialized_var(xrcd_uobj);
	struct ib_cq                   *scq = NULL, *rcq = NULL;
	struct ib_srq                  *srq = NULL;
	struct ib_qp                   *qp;
	struct ib_qp_init_attr          attr;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	obj = kmalloc(sizeof *obj, GFP_KERNEL);
	if (!obj)
		return -ENOMEM;

	init_uobj(&obj->uevent.uobject, cmd.user_handle, file->ucontext, &qp_lock_key);
	down_write(&obj->uevent.uobject.mutex);

	if (cmd.qp_type == IB_QPT_XRC_TGT) {
		xrcd = idr_read_xrcd(cmd.pd_handle, file->ucontext, &xrcd_uobj);
=======
	struct ib_ucq_object        	*obj;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_get_destroy(UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	obj = container_of(uobj, struct ib_ucq_object, uevent.uobject);
	memset(&resp, 0, sizeof(resp));
	resp.comp_events_reported  = obj->comp_events_reported;
	resp.async_events_reported = obj->uevent.events_reported;

	uobj_put_destroy(uobj);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int create_qp(struct uverbs_attr_bundle *attrs,
		     struct ib_uverbs_ex_create_qp *cmd)
{
	struct ib_uqp_object		*obj;
	struct ib_device		*device;
	struct ib_pd			*pd = NULL;
	struct ib_xrcd			*xrcd = NULL;
	struct ib_uobject		*xrcd_uobj = ERR_PTR(-ENOENT);
	struct ib_cq			*scq = NULL, *rcq = NULL;
	struct ib_srq			*srq = NULL;
	struct ib_qp			*qp;
	struct ib_qp_init_attr		attr = {};
	struct ib_uverbs_ex_create_qp_resp resp = {};
	int				ret;
	struct ib_rwq_ind_table *ind_tbl = NULL;
	bool has_sq = true;
	struct ib_device *ib_dev;

	switch (cmd->qp_type) {
	case IB_QPT_RAW_PACKET:
		if (!capable(CAP_NET_RAW))
			return -EPERM;
		break;
	case IB_QPT_RC:
	case IB_QPT_UC:
	case IB_QPT_UD:
	case IB_QPT_XRC_INI:
	case IB_QPT_XRC_TGT:
	case IB_QPT_DRIVER:
		break;
	default:
		return -EINVAL;
	}

	obj = (struct ib_uqp_object *)uobj_alloc(UVERBS_OBJECT_QP, attrs,
						 &ib_dev);
	if (IS_ERR(obj))
		return PTR_ERR(obj);
	obj->uxrcd = NULL;
	obj->uevent.uobject.user_handle = cmd->user_handle;
	mutex_init(&obj->mcast_lock);

	if (cmd->comp_mask & IB_UVERBS_CREATE_QP_MASK_IND_TABLE) {
		ind_tbl = uobj_get_obj_read(rwq_ind_table,
					    UVERBS_OBJECT_RWQ_IND_TBL,
					    cmd->rwq_ind_tbl_handle, attrs);
		if (!ind_tbl) {
			ret = -EINVAL;
			goto err_put;
		}

		attr.rwq_ind_tbl = ind_tbl;
	}

	if (ind_tbl && (cmd->max_recv_wr || cmd->max_recv_sge || cmd->is_srq)) {
		ret = -EINVAL;
		goto err_put;
	}

	if (ind_tbl && !cmd->max_send_wr)
		has_sq = false;

	if (cmd->qp_type == IB_QPT_XRC_TGT) {
		xrcd_uobj = uobj_get_read(UVERBS_OBJECT_XRCD, cmd->pd_handle,
					  attrs);

		if (IS_ERR(xrcd_uobj)) {
			ret = -EINVAL;
			goto err_put;
		}

		xrcd = (struct ib_xrcd *)xrcd_uobj->object;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!xrcd) {
			ret = -EINVAL;
			goto err_put;
		}
		device = xrcd->device;
	} else {
<<<<<<< HEAD
		pd  = idr_read_pd(cmd.pd_handle, file->ucontext);
		scq = idr_read_cq(cmd.send_cq_handle, file->ucontext, 0);
		if (!pd || !scq) {
			ret = -EINVAL;
			goto err_put;
		}

		if (cmd.qp_type == IB_QPT_XRC_INI) {
			cmd.max_recv_wr = cmd.max_recv_sge = 0;
		} else {
			if (cmd.is_srq) {
				srq = idr_read_srq(cmd.srq_handle, file->ucontext);
				if (!srq || srq->srq_type != IB_SRQT_BASIC) {
=======
		if (cmd->qp_type == IB_QPT_XRC_INI) {
			cmd->max_recv_wr = 0;
			cmd->max_recv_sge = 0;
		} else {
			if (cmd->is_srq) {
				srq = uobj_get_obj_read(srq, UVERBS_OBJECT_SRQ,
							cmd->srq_handle, attrs);
				if (!srq || srq->srq_type == IB_SRQT_XRC) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					ret = -EINVAL;
					goto err_put;
				}
			}
<<<<<<< HEAD
			rcq = (cmd.recv_cq_handle == cmd.send_cq_handle) ?
			       scq : idr_read_cq(cmd.recv_cq_handle, file->ucontext, 1);
			if (!rcq) {
				ret = -EINVAL;
				goto err_put;
			}
		}
=======

			if (!ind_tbl) {
				if (cmd->recv_cq_handle != cmd->send_cq_handle) {
					rcq = uobj_get_obj_read(
						cq, UVERBS_OBJECT_CQ,
						cmd->recv_cq_handle, attrs);
					if (!rcq) {
						ret = -EINVAL;
						goto err_put;
					}
				}
			}
		}

		if (has_sq)
			scq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ,
						cmd->send_cq_handle, attrs);
		if (!ind_tbl && cmd->qp_type != IB_QPT_XRC_INI)
			rcq = rcq ?: scq;
		pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd->pd_handle,
				       attrs);
		if (!pd || (!scq && has_sq)) {
			ret = -EINVAL;
			goto err_put;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		device = pd->device;
	}

	attr.event_handler = ib_uverbs_qp_event_handler;
<<<<<<< HEAD
	attr.qp_context    = file;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	attr.send_cq       = scq;
	attr.recv_cq       = rcq;
	attr.srq           = srq;
	attr.xrcd	   = xrcd;
<<<<<<< HEAD
	attr.sq_sig_type   = cmd.sq_sig_all ? IB_SIGNAL_ALL_WR : IB_SIGNAL_REQ_WR;
	attr.qp_type       = cmd.qp_type;
	attr.create_flags  = 0;

	attr.cap.max_send_wr     = cmd.max_send_wr;
	attr.cap.max_recv_wr     = cmd.max_recv_wr;
	attr.cap.max_send_sge    = cmd.max_send_sge;
	attr.cap.max_recv_sge    = cmd.max_recv_sge;
	attr.cap.max_inline_data = cmd.max_inline_data;

	obj->uevent.events_reported     = 0;
	INIT_LIST_HEAD(&obj->uevent.event_list);
	INIT_LIST_HEAD(&obj->mcast_list);

	if (cmd.qp_type == IB_QPT_XRC_TGT)
		qp = ib_create_qp(pd, &attr);
	else
		qp = device->create_qp(pd, &attr, &udata);

	if (IS_ERR(qp)) {
		ret = PTR_ERR(qp);
		goto err_put;
	}

	if (cmd.qp_type != IB_QPT_XRC_TGT) {
		qp->real_qp	  = qp;
		qp->device	  = device;
		qp->pd		  = pd;
		qp->send_cq	  = attr.send_cq;
		qp->recv_cq	  = attr.recv_cq;
		qp->srq		  = attr.srq;
		qp->event_handler = attr.event_handler;
		qp->qp_context	  = attr.qp_context;
		qp->qp_type	  = attr.qp_type;
		atomic_set(&qp->usecnt, 0);
		atomic_inc(&pd->usecnt);
		atomic_inc(&attr.send_cq->usecnt);
		if (attr.recv_cq)
			atomic_inc(&attr.recv_cq->usecnt);
		if (attr.srq)
			atomic_inc(&attr.srq->usecnt);
	}
	qp->uobject = &obj->uevent.uobject;

	obj->uevent.uobject.object = qp;
	ret = idr_add_uobj(&ib_uverbs_qp_idr, &obj->uevent.uobject);
	if (ret)
		goto err_destroy;

	memset(&resp, 0, sizeof resp);
	resp.qpn             = qp->qp_num;
	resp.qp_handle       = obj->uevent.uobject.id;
	resp.max_recv_sge    = attr.cap.max_recv_sge;
	resp.max_send_sge    = attr.cap.max_send_sge;
	resp.max_recv_wr     = attr.cap.max_recv_wr;
	resp.max_send_wr     = attr.cap.max_send_wr;
	resp.max_inline_data = attr.cap.max_inline_data;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	if (xrcd)
		put_xrcd_read(xrcd_uobj);
	if (pd)
		put_pd_read(pd);
	if (scq)
		put_cq_read(scq);
	if (rcq && rcq != scq)
		put_cq_read(rcq);
	if (srq)
		put_srq_read(srq);

	mutex_lock(&file->mutex);
	list_add_tail(&obj->uevent.uobject.list, &file->ucontext->qp_list);
	mutex_unlock(&file->mutex);

	obj->uevent.uobject.live = 1;

	up_write(&obj->uevent.uobject.mutex);

	return in_len;

err_copy:
	idr_remove_uobj(&ib_uverbs_qp_idr, &obj->uevent.uobject);

err_destroy:
	ib_destroy_qp(qp);

err_put:
	if (xrcd)
		put_xrcd_read(xrcd_uobj);
	if (pd)
		put_pd_read(pd);
	if (scq)
		put_cq_read(scq);
	if (rcq && rcq != scq)
		put_cq_read(rcq);
	if (srq)
		put_srq_read(srq);

	put_uobj_write(&obj->uevent.uobject);
	return ret;
}

ssize_t ib_uverbs_open_qp(struct ib_uverbs_file *file,
			  const char __user *buf, int in_len, int out_len)
{
	struct ib_uverbs_open_qp        cmd;
	struct ib_uverbs_create_qp_resp resp;
	struct ib_udata                 udata;
	struct ib_uqp_object           *obj;
	struct ib_xrcd		       *xrcd;
	struct ib_uobject	       *uninitialized_var(xrcd_uobj);
	struct ib_qp                   *qp;
	struct ib_qp_open_attr          attr;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	obj = kmalloc(sizeof *obj, GFP_KERNEL);
	if (!obj)
		return -ENOMEM;

	init_uobj(&obj->uevent.uobject, cmd.user_handle, file->ucontext, &qp_lock_key);
	down_write(&obj->uevent.uobject.mutex);

	xrcd = idr_read_xrcd(cmd.pd_handle, file->ucontext, &xrcd_uobj);
	if (!xrcd) {
=======
	attr.sq_sig_type   = cmd->sq_sig_all ? IB_SIGNAL_ALL_WR :
					      IB_SIGNAL_REQ_WR;
	attr.qp_type       = cmd->qp_type;

	attr.cap.max_send_wr     = cmd->max_send_wr;
	attr.cap.max_recv_wr     = cmd->max_recv_wr;
	attr.cap.max_send_sge    = cmd->max_send_sge;
	attr.cap.max_recv_sge    = cmd->max_recv_sge;
	attr.cap.max_inline_data = cmd->max_inline_data;

	INIT_LIST_HEAD(&obj->uevent.event_list);
	INIT_LIST_HEAD(&obj->mcast_list);

	attr.create_flags = cmd->create_flags;
	if (attr.create_flags & ~(IB_QP_CREATE_BLOCK_MULTICAST_LOOPBACK |
				IB_QP_CREATE_CROSS_CHANNEL |
				IB_QP_CREATE_MANAGED_SEND |
				IB_QP_CREATE_MANAGED_RECV |
				IB_QP_CREATE_SCATTER_FCS |
				IB_QP_CREATE_CVLAN_STRIPPING |
				IB_QP_CREATE_SOURCE_QPN |
				IB_QP_CREATE_PCI_WRITE_END_PADDING)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto err_put;
	}

<<<<<<< HEAD
	attr.event_handler = ib_uverbs_qp_event_handler;
	attr.qp_context    = file;
	attr.qp_num        = cmd.qpn;
	attr.qp_type       = cmd.qp_type;

	obj->uevent.events_reported = 0;
=======
	if (attr.create_flags & IB_QP_CREATE_SOURCE_QPN) {
		if (!capable(CAP_NET_RAW)) {
			ret = -EPERM;
			goto err_put;
		}

		attr.source_qpn = cmd->source_qpn;
	}

	qp = ib_create_qp_user(device, pd, &attr, &attrs->driver_udata, obj,
			       KBUILD_MODNAME);
	if (IS_ERR(qp)) {
		ret = PTR_ERR(qp);
		goto err_put;
	}
	ib_qp_usecnt_inc(qp);

	obj->uevent.uobject.object = qp;
	obj->uevent.event_file = READ_ONCE(attrs->ufile->default_async_file);
	if (obj->uevent.event_file)
		uverbs_uobject_get(&obj->uevent.event_file->uobj);

	if (xrcd) {
		obj->uxrcd = container_of(xrcd_uobj, struct ib_uxrcd_object,
					  uobject);
		atomic_inc(&obj->uxrcd->refcnt);
		uobj_put_read(xrcd_uobj);
	}

	if (pd)
		uobj_put_obj_read(pd);
	if (scq)
		rdma_lookup_put_uobject(&scq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (rcq && rcq != scq)
		rdma_lookup_put_uobject(&rcq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (srq)
		rdma_lookup_put_uobject(&srq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (ind_tbl)
		uobj_put_obj_read(ind_tbl);
	uobj_finalize_uobj_create(&obj->uevent.uobject, attrs);

	resp.base.qpn             = qp->qp_num;
	resp.base.qp_handle       = obj->uevent.uobject.id;
	resp.base.max_recv_sge    = attr.cap.max_recv_sge;
	resp.base.max_send_sge    = attr.cap.max_send_sge;
	resp.base.max_recv_wr     = attr.cap.max_recv_wr;
	resp.base.max_send_wr     = attr.cap.max_send_wr;
	resp.base.max_inline_data = attr.cap.max_inline_data;
	resp.response_length = uverbs_response_length(attrs, sizeof(resp));
	return uverbs_response(attrs, &resp, sizeof(resp));

err_put:
	if (!IS_ERR(xrcd_uobj))
		uobj_put_read(xrcd_uobj);
	if (pd)
		uobj_put_obj_read(pd);
	if (scq)
		rdma_lookup_put_uobject(&scq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (rcq && rcq != scq)
		rdma_lookup_put_uobject(&rcq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (srq)
		rdma_lookup_put_uobject(&srq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	if (ind_tbl)
		uobj_put_obj_read(ind_tbl);

	uobj_alloc_abort(&obj->uevent.uobject, attrs);
	return ret;
}

static int ib_uverbs_create_qp(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_qp      cmd;
	struct ib_uverbs_ex_create_qp	cmd_ex;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	memset(&cmd_ex, 0, sizeof(cmd_ex));
	cmd_ex.user_handle = cmd.user_handle;
	cmd_ex.pd_handle = cmd.pd_handle;
	cmd_ex.send_cq_handle = cmd.send_cq_handle;
	cmd_ex.recv_cq_handle = cmd.recv_cq_handle;
	cmd_ex.srq_handle = cmd.srq_handle;
	cmd_ex.max_send_wr = cmd.max_send_wr;
	cmd_ex.max_recv_wr = cmd.max_recv_wr;
	cmd_ex.max_send_sge = cmd.max_send_sge;
	cmd_ex.max_recv_sge = cmd.max_recv_sge;
	cmd_ex.max_inline_data = cmd.max_inline_data;
	cmd_ex.sq_sig_all = cmd.sq_sig_all;
	cmd_ex.qp_type = cmd.qp_type;
	cmd_ex.is_srq = cmd.is_srq;

	return create_qp(attrs, &cmd_ex);
}

static int ib_uverbs_ex_create_qp(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_create_qp cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (cmd.comp_mask & ~IB_UVERBS_CREATE_QP_SUP_COMP_MASK)
		return -EINVAL;

	if (cmd.reserved)
		return -EINVAL;

	return create_qp(attrs, &cmd);
}

static int ib_uverbs_open_qp(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_qp_resp resp = {};
	struct ib_uverbs_open_qp        cmd;
	struct ib_uqp_object           *obj;
	struct ib_xrcd		       *xrcd;
	struct ib_qp                   *qp;
	struct ib_qp_open_attr          attr = {};
	int ret;
	struct ib_uobject *xrcd_uobj;
	struct ib_device *ib_dev;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	obj = (struct ib_uqp_object *)uobj_alloc(UVERBS_OBJECT_QP, attrs,
						 &ib_dev);
	if (IS_ERR(obj))
		return PTR_ERR(obj);

	xrcd_uobj = uobj_get_read(UVERBS_OBJECT_XRCD, cmd.pd_handle, attrs);
	if (IS_ERR(xrcd_uobj)) {
		ret = -EINVAL;
		goto err_put;
	}

	xrcd = (struct ib_xrcd *)xrcd_uobj->object;
	if (!xrcd) {
		ret = -EINVAL;
		goto err_xrcd;
	}

	attr.event_handler = ib_uverbs_qp_event_handler;
	attr.qp_num        = cmd.qpn;
	attr.qp_type       = cmd.qp_type;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&obj->uevent.event_list);
	INIT_LIST_HEAD(&obj->mcast_list);

	qp = ib_open_qp(xrcd, &attr);
	if (IS_ERR(qp)) {
		ret = PTR_ERR(qp);
<<<<<<< HEAD
		goto err_put;
	}

	qp->uobject = &obj->uevent.uobject;

	obj->uevent.uobject.object = qp;
	ret = idr_add_uobj(&ib_uverbs_qp_idr, &obj->uevent.uobject);
	if (ret)
		goto err_destroy;

	memset(&resp, 0, sizeof resp);
	resp.qpn       = qp->qp_num;
	resp.qp_handle = obj->uevent.uobject.id;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_remove;
	}

	put_xrcd_read(xrcd_uobj);

	mutex_lock(&file->mutex);
	list_add_tail(&obj->uevent.uobject.list, &file->ucontext->qp_list);
	mutex_unlock(&file->mutex);

	obj->uevent.uobject.live = 1;

	up_write(&obj->uevent.uobject.mutex);

	return in_len;

err_remove:
	idr_remove_uobj(&ib_uverbs_qp_idr, &obj->uevent.uobject);

err_destroy:
	ib_destroy_qp(qp);

err_put:
	put_xrcd_read(xrcd_uobj);
	put_uobj_write(&obj->uevent.uobject);
	return ret;
}

ssize_t ib_uverbs_query_qp(struct ib_uverbs_file *file,
			   const char __user *buf, int in_len,
			   int out_len)
=======
		goto err_xrcd;
	}

	obj->uevent.uobject.object = qp;
	obj->uevent.uobject.user_handle = cmd.user_handle;

	obj->uxrcd = container_of(xrcd_uobj, struct ib_uxrcd_object, uobject);
	atomic_inc(&obj->uxrcd->refcnt);
	qp->uobject = obj;
	uobj_put_read(xrcd_uobj);
	uobj_finalize_uobj_create(&obj->uevent.uobject, attrs);

	resp.qpn = qp->qp_num;
	resp.qp_handle = obj->uevent.uobject.id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_xrcd:
	uobj_put_read(xrcd_uobj);
err_put:
	uobj_alloc_abort(&obj->uevent.uobject, attrs);
	return ret;
}

static void copy_ah_attr_to_uverbs(struct ib_uverbs_qp_dest *uverb_attr,
				   struct rdma_ah_attr *rdma_attr)
{
	const struct ib_global_route   *grh;

	uverb_attr->dlid              = rdma_ah_get_dlid(rdma_attr);
	uverb_attr->sl                = rdma_ah_get_sl(rdma_attr);
	uverb_attr->src_path_bits     = rdma_ah_get_path_bits(rdma_attr);
	uverb_attr->static_rate       = rdma_ah_get_static_rate(rdma_attr);
	uverb_attr->is_global         = !!(rdma_ah_get_ah_flags(rdma_attr) &
					 IB_AH_GRH);
	if (uverb_attr->is_global) {
		grh = rdma_ah_read_grh(rdma_attr);
		memcpy(uverb_attr->dgid, grh->dgid.raw, 16);
		uverb_attr->flow_label        = grh->flow_label;
		uverb_attr->sgid_index        = grh->sgid_index;
		uverb_attr->hop_limit         = grh->hop_limit;
		uverb_attr->traffic_class     = grh->traffic_class;
	}
	uverb_attr->port_num          = rdma_ah_get_port_num(rdma_attr);
}

static int ib_uverbs_query_qp(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_query_qp      cmd;
	struct ib_uverbs_query_qp_resp resp;
	struct ib_qp                   *qp;
	struct ib_qp_attr              *attr;
	struct ib_qp_init_attr         *init_attr;
	int                            ret;

<<<<<<< HEAD
	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;
=======
	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	attr      = kmalloc(sizeof *attr, GFP_KERNEL);
	init_attr = kmalloc(sizeof *init_attr, GFP_KERNEL);
	if (!attr || !init_attr) {
		ret = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	qp = idr_read_qp(cmd.qp_handle, file->ucontext);
=======
	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!qp) {
		ret = -EINVAL;
		goto out;
	}

	ret = ib_query_qp(qp, attr, cmd.attr_mask, init_attr);

<<<<<<< HEAD
	put_qp_read(qp);
=======
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret)
		goto out;

	memset(&resp, 0, sizeof resp);

	resp.qp_state               = attr->qp_state;
	resp.cur_qp_state           = attr->cur_qp_state;
	resp.path_mtu               = attr->path_mtu;
	resp.path_mig_state         = attr->path_mig_state;
	resp.qkey                   = attr->qkey;
	resp.rq_psn                 = attr->rq_psn;
	resp.sq_psn                 = attr->sq_psn;
	resp.dest_qp_num            = attr->dest_qp_num;
	resp.qp_access_flags        = attr->qp_access_flags;
	resp.pkey_index             = attr->pkey_index;
	resp.alt_pkey_index         = attr->alt_pkey_index;
	resp.sq_draining            = attr->sq_draining;
	resp.max_rd_atomic          = attr->max_rd_atomic;
	resp.max_dest_rd_atomic     = attr->max_dest_rd_atomic;
	resp.min_rnr_timer          = attr->min_rnr_timer;
	resp.port_num               = attr->port_num;
	resp.timeout                = attr->timeout;
	resp.retry_cnt              = attr->retry_cnt;
	resp.rnr_retry              = attr->rnr_retry;
	resp.alt_port_num           = attr->alt_port_num;
	resp.alt_timeout            = attr->alt_timeout;

<<<<<<< HEAD
	memcpy(resp.dest.dgid, attr->ah_attr.grh.dgid.raw, 16);
	resp.dest.flow_label        = attr->ah_attr.grh.flow_label;
	resp.dest.sgid_index        = attr->ah_attr.grh.sgid_index;
	resp.dest.hop_limit         = attr->ah_attr.grh.hop_limit;
	resp.dest.traffic_class     = attr->ah_attr.grh.traffic_class;
	resp.dest.dlid              = attr->ah_attr.dlid;
	resp.dest.sl                = attr->ah_attr.sl;
	resp.dest.src_path_bits     = attr->ah_attr.src_path_bits;
	resp.dest.static_rate       = attr->ah_attr.static_rate;
	resp.dest.is_global         = !!(attr->ah_attr.ah_flags & IB_AH_GRH);
	resp.dest.port_num          = attr->ah_attr.port_num;

	memcpy(resp.alt_dest.dgid, attr->alt_ah_attr.grh.dgid.raw, 16);
	resp.alt_dest.flow_label    = attr->alt_ah_attr.grh.flow_label;
	resp.alt_dest.sgid_index    = attr->alt_ah_attr.grh.sgid_index;
	resp.alt_dest.hop_limit     = attr->alt_ah_attr.grh.hop_limit;
	resp.alt_dest.traffic_class = attr->alt_ah_attr.grh.traffic_class;
	resp.alt_dest.dlid          = attr->alt_ah_attr.dlid;
	resp.alt_dest.sl            = attr->alt_ah_attr.sl;
	resp.alt_dest.src_path_bits = attr->alt_ah_attr.src_path_bits;
	resp.alt_dest.static_rate   = attr->alt_ah_attr.static_rate;
	resp.alt_dest.is_global     = !!(attr->alt_ah_attr.ah_flags & IB_AH_GRH);
	resp.alt_dest.port_num      = attr->alt_ah_attr.port_num;
=======
	copy_ah_attr_to_uverbs(&resp.dest, &attr->ah_attr);
	copy_ah_attr_to_uverbs(&resp.alt_dest, &attr->alt_ah_attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	resp.max_send_wr            = init_attr->cap.max_send_wr;
	resp.max_recv_wr            = init_attr->cap.max_recv_wr;
	resp.max_send_sge           = init_attr->cap.max_send_sge;
	resp.max_recv_sge           = init_attr->cap.max_recv_sge;
	resp.max_inline_data        = init_attr->cap.max_inline_data;
	resp.sq_sig_all             = init_attr->sq_sig_type == IB_SIGNAL_ALL_WR;

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		ret = -EFAULT;
=======
	ret = uverbs_response(attrs, &resp, sizeof(resp));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	kfree(attr);
	kfree(init_attr);

<<<<<<< HEAD
	return ret ? ret : in_len;
=======
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Remove ignored fields set in the attribute mask */
static int modify_qp_mask(enum ib_qp_type qp_type, int mask)
{
	switch (qp_type) {
	case IB_QPT_XRC_INI:
		return mask & ~(IB_QP_MAX_DEST_RD_ATOMIC | IB_QP_MIN_RNR_TIMER);
	case IB_QPT_XRC_TGT:
		return mask & ~(IB_QP_MAX_QP_RD_ATOMIC | IB_QP_RETRY_CNT |
				IB_QP_RNR_RETRY);
	default:
		return mask;
	}
}

<<<<<<< HEAD
ssize_t ib_uverbs_modify_qp(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_modify_qp cmd;
	struct ib_udata            udata;
	struct ib_qp              *qp;
	struct ib_qp_attr         *attr;
	int                        ret;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd, NULL, in_len - sizeof cmd,
		   out_len);

	attr = kmalloc(sizeof *attr, GFP_KERNEL);
	if (!attr)
		return -ENOMEM;

	qp = idr_read_qp(cmd.qp_handle, file->ucontext);
=======
static void copy_ah_attr_from_uverbs(struct ib_device *dev,
				     struct rdma_ah_attr *rdma_attr,
				     struct ib_uverbs_qp_dest *uverb_attr)
{
	rdma_attr->type = rdma_ah_find_type(dev, uverb_attr->port_num);
	if (uverb_attr->is_global) {
		rdma_ah_set_grh(rdma_attr, NULL,
				uverb_attr->flow_label,
				uverb_attr->sgid_index,
				uverb_attr->hop_limit,
				uverb_attr->traffic_class);
		rdma_ah_set_dgid_raw(rdma_attr, uverb_attr->dgid);
	} else {
		rdma_ah_set_ah_flags(rdma_attr, 0);
	}
	rdma_ah_set_dlid(rdma_attr, uverb_attr->dlid);
	rdma_ah_set_sl(rdma_attr, uverb_attr->sl);
	rdma_ah_set_path_bits(rdma_attr, uverb_attr->src_path_bits);
	rdma_ah_set_static_rate(rdma_attr, uverb_attr->static_rate);
	rdma_ah_set_port_num(rdma_attr, uverb_attr->port_num);
	rdma_ah_set_make_grd(rdma_attr, false);
}

static int modify_qp(struct uverbs_attr_bundle *attrs,
		     struct ib_uverbs_ex_modify_qp *cmd)
{
	struct ib_qp_attr *attr;
	struct ib_qp *qp;
	int ret;

	attr = kzalloc(sizeof(*attr), GFP_KERNEL);
	if (!attr)
		return -ENOMEM;

	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd->base.qp_handle,
			       attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!qp) {
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	attr->qp_state 		  = cmd.qp_state;
	attr->cur_qp_state 	  = cmd.cur_qp_state;
	attr->path_mtu 		  = cmd.path_mtu;
	attr->path_mig_state 	  = cmd.path_mig_state;
	attr->qkey 		  = cmd.qkey;
	attr->rq_psn 		  = cmd.rq_psn;
	attr->sq_psn 		  = cmd.sq_psn;
	attr->dest_qp_num 	  = cmd.dest_qp_num;
	attr->qp_access_flags 	  = cmd.qp_access_flags;
	attr->pkey_index 	  = cmd.pkey_index;
	attr->alt_pkey_index 	  = cmd.alt_pkey_index;
	attr->en_sqd_async_notify = cmd.en_sqd_async_notify;
	attr->max_rd_atomic 	  = cmd.max_rd_atomic;
	attr->max_dest_rd_atomic  = cmd.max_dest_rd_atomic;
	attr->min_rnr_timer 	  = cmd.min_rnr_timer;
	attr->port_num 		  = cmd.port_num;
	attr->timeout 		  = cmd.timeout;
	attr->retry_cnt 	  = cmd.retry_cnt;
	attr->rnr_retry 	  = cmd.rnr_retry;
	attr->alt_port_num 	  = cmd.alt_port_num;
	attr->alt_timeout 	  = cmd.alt_timeout;

	memcpy(attr->ah_attr.grh.dgid.raw, cmd.dest.dgid, 16);
	attr->ah_attr.grh.flow_label        = cmd.dest.flow_label;
	attr->ah_attr.grh.sgid_index        = cmd.dest.sgid_index;
	attr->ah_attr.grh.hop_limit         = cmd.dest.hop_limit;
	attr->ah_attr.grh.traffic_class     = cmd.dest.traffic_class;
	attr->ah_attr.dlid 	    	    = cmd.dest.dlid;
	attr->ah_attr.sl   	    	    = cmd.dest.sl;
	attr->ah_attr.src_path_bits 	    = cmd.dest.src_path_bits;
	attr->ah_attr.static_rate   	    = cmd.dest.static_rate;
	attr->ah_attr.ah_flags 	    	    = cmd.dest.is_global ? IB_AH_GRH : 0;
	attr->ah_attr.port_num 	    	    = cmd.dest.port_num;

	memcpy(attr->alt_ah_attr.grh.dgid.raw, cmd.alt_dest.dgid, 16);
	attr->alt_ah_attr.grh.flow_label    = cmd.alt_dest.flow_label;
	attr->alt_ah_attr.grh.sgid_index    = cmd.alt_dest.sgid_index;
	attr->alt_ah_attr.grh.hop_limit     = cmd.alt_dest.hop_limit;
	attr->alt_ah_attr.grh.traffic_class = cmd.alt_dest.traffic_class;
	attr->alt_ah_attr.dlid 	    	    = cmd.alt_dest.dlid;
	attr->alt_ah_attr.sl   	    	    = cmd.alt_dest.sl;
	attr->alt_ah_attr.src_path_bits     = cmd.alt_dest.src_path_bits;
	attr->alt_ah_attr.static_rate       = cmd.alt_dest.static_rate;
	attr->alt_ah_attr.ah_flags 	    = cmd.alt_dest.is_global ? IB_AH_GRH : 0;
	attr->alt_ah_attr.port_num 	    = cmd.alt_dest.port_num;

	if (qp->real_qp == qp) {
		ret = qp->device->modify_qp(qp, attr,
			modify_qp_mask(qp->qp_type, cmd.attr_mask), &udata);
	} else {
		ret = ib_modify_qp(qp, attr, modify_qp_mask(qp->qp_type, cmd.attr_mask));
	}

	put_qp_read(qp);

	if (ret)
		goto out;

	ret = in_len;

=======
	if ((cmd->base.attr_mask & IB_QP_PORT) &&
	    !rdma_is_port_valid(qp->device, cmd->base.port_num)) {
		ret = -EINVAL;
		goto release_qp;
	}

	if ((cmd->base.attr_mask & IB_QP_AV)) {
		if (!rdma_is_port_valid(qp->device, cmd->base.dest.port_num)) {
			ret = -EINVAL;
			goto release_qp;
		}

		if (cmd->base.attr_mask & IB_QP_STATE &&
		    cmd->base.qp_state == IB_QPS_RTR) {
		/* We are in INIT->RTR TRANSITION (if we are not,
		 * this transition will be rejected in subsequent checks).
		 * In the INIT->RTR transition, we cannot have IB_QP_PORT set,
		 * but the IB_QP_STATE flag is required.
		 *
		 * Since kernel 3.14 (commit dbf727de7440), the uverbs driver,
		 * when IB_QP_AV is set, has required inclusion of a valid
		 * port number in the primary AV. (AVs are created and handled
		 * differently for infiniband and ethernet (RoCE) ports).
		 *
		 * Check the port number included in the primary AV against
		 * the port number in the qp struct, which was set (and saved)
		 * in the RST->INIT transition.
		 */
			if (cmd->base.dest.port_num != qp->real_qp->port) {
				ret = -EINVAL;
				goto release_qp;
			}
		} else {
		/* We are in SQD->SQD. (If we are not, this transition will
		 * be rejected later in the verbs layer checks).
		 * Check for both IB_QP_PORT and IB_QP_AV, these can be set
		 * together in the SQD->SQD transition.
		 *
		 * If only IP_QP_AV was set, add in IB_QP_PORT as well (the
		 * verbs layer driver does not track primary port changes
		 * resulting from path migration. Thus, in SQD, if the primary
		 * AV is modified, the primary port should also be modified).
		 *
		 * Note that in this transition, the IB_QP_STATE flag
		 * is not allowed.
		 */
			if (((cmd->base.attr_mask & (IB_QP_AV | IB_QP_PORT))
			     == (IB_QP_AV | IB_QP_PORT)) &&
			    cmd->base.port_num != cmd->base.dest.port_num) {
				ret = -EINVAL;
				goto release_qp;
			}
			if ((cmd->base.attr_mask & (IB_QP_AV | IB_QP_PORT))
			    == IB_QP_AV) {
				cmd->base.attr_mask |= IB_QP_PORT;
				cmd->base.port_num = cmd->base.dest.port_num;
			}
		}
	}

	if ((cmd->base.attr_mask & IB_QP_ALT_PATH) &&
	    (!rdma_is_port_valid(qp->device, cmd->base.alt_port_num) ||
	    !rdma_is_port_valid(qp->device, cmd->base.alt_dest.port_num) ||
	    cmd->base.alt_port_num != cmd->base.alt_dest.port_num)) {
		ret = -EINVAL;
		goto release_qp;
	}

	if ((cmd->base.attr_mask & IB_QP_CUR_STATE &&
	    cmd->base.cur_qp_state > IB_QPS_ERR) ||
	    (cmd->base.attr_mask & IB_QP_STATE &&
	    cmd->base.qp_state > IB_QPS_ERR)) {
		ret = -EINVAL;
		goto release_qp;
	}

	if (cmd->base.attr_mask & IB_QP_STATE)
		attr->qp_state = cmd->base.qp_state;
	if (cmd->base.attr_mask & IB_QP_CUR_STATE)
		attr->cur_qp_state = cmd->base.cur_qp_state;
	if (cmd->base.attr_mask & IB_QP_PATH_MTU)
		attr->path_mtu = cmd->base.path_mtu;
	if (cmd->base.attr_mask & IB_QP_PATH_MIG_STATE)
		attr->path_mig_state = cmd->base.path_mig_state;
	if (cmd->base.attr_mask & IB_QP_QKEY) {
		if (cmd->base.qkey & IB_QP_SET_QKEY &&
		    !rdma_nl_get_privileged_qkey()) {
			ret = -EPERM;
			goto release_qp;
		}
		attr->qkey = cmd->base.qkey;
	}
	if (cmd->base.attr_mask & IB_QP_RQ_PSN)
		attr->rq_psn = cmd->base.rq_psn;
	if (cmd->base.attr_mask & IB_QP_SQ_PSN)
		attr->sq_psn = cmd->base.sq_psn;
	if (cmd->base.attr_mask & IB_QP_DEST_QPN)
		attr->dest_qp_num = cmd->base.dest_qp_num;
	if (cmd->base.attr_mask & IB_QP_ACCESS_FLAGS)
		attr->qp_access_flags = cmd->base.qp_access_flags;
	if (cmd->base.attr_mask & IB_QP_PKEY_INDEX)
		attr->pkey_index = cmd->base.pkey_index;
	if (cmd->base.attr_mask & IB_QP_EN_SQD_ASYNC_NOTIFY)
		attr->en_sqd_async_notify = cmd->base.en_sqd_async_notify;
	if (cmd->base.attr_mask & IB_QP_MAX_QP_RD_ATOMIC)
		attr->max_rd_atomic = cmd->base.max_rd_atomic;
	if (cmd->base.attr_mask & IB_QP_MAX_DEST_RD_ATOMIC)
		attr->max_dest_rd_atomic = cmd->base.max_dest_rd_atomic;
	if (cmd->base.attr_mask & IB_QP_MIN_RNR_TIMER)
		attr->min_rnr_timer = cmd->base.min_rnr_timer;
	if (cmd->base.attr_mask & IB_QP_PORT)
		attr->port_num = cmd->base.port_num;
	if (cmd->base.attr_mask & IB_QP_TIMEOUT)
		attr->timeout = cmd->base.timeout;
	if (cmd->base.attr_mask & IB_QP_RETRY_CNT)
		attr->retry_cnt = cmd->base.retry_cnt;
	if (cmd->base.attr_mask & IB_QP_RNR_RETRY)
		attr->rnr_retry = cmd->base.rnr_retry;
	if (cmd->base.attr_mask & IB_QP_ALT_PATH) {
		attr->alt_port_num = cmd->base.alt_port_num;
		attr->alt_timeout = cmd->base.alt_timeout;
		attr->alt_pkey_index = cmd->base.alt_pkey_index;
	}
	if (cmd->base.attr_mask & IB_QP_RATE_LIMIT)
		attr->rate_limit = cmd->rate_limit;

	if (cmd->base.attr_mask & IB_QP_AV)
		copy_ah_attr_from_uverbs(qp->device, &attr->ah_attr,
					 &cmd->base.dest);

	if (cmd->base.attr_mask & IB_QP_ALT_PATH)
		copy_ah_attr_from_uverbs(qp->device, &attr->alt_ah_attr,
					 &cmd->base.alt_dest);

	ret = ib_modify_qp_with_udata(qp, attr,
				      modify_qp_mask(qp->qp_type,
						     cmd->base.attr_mask),
				      &attrs->driver_udata);

release_qp:
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(attr);

	return ret;
}

<<<<<<< HEAD
ssize_t ib_uverbs_destroy_qp(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len,
			     int out_len)
=======
static int ib_uverbs_modify_qp(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_modify_qp cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd.base, sizeof(cmd.base));
	if (ret)
		return ret;

	if (cmd.base.attr_mask & ~IB_QP_ATTR_STANDARD_BITS)
		return -EOPNOTSUPP;

	return modify_qp(attrs, &cmd);
}

static int ib_uverbs_ex_modify_qp(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_modify_qp cmd;
	struct ib_uverbs_ex_modify_qp_resp resp = {
		.response_length = uverbs_response_length(attrs, sizeof(resp))
	};
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	/*
	 * Last bit is reserved for extending the attr_mask by
	 * using another field.
	 */
	if (cmd.base.attr_mask & ~(IB_QP_ATTR_STANDARD_BITS | IB_QP_RATE_LIMIT))
		return -EOPNOTSUPP;

	ret = modify_qp(attrs, &cmd);
	if (ret)
		return ret;

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_destroy_qp(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_destroy_qp      cmd;
	struct ib_uverbs_destroy_qp_resp resp;
	struct ib_uobject		*uobj;
<<<<<<< HEAD
	struct ib_qp               	*qp;
	struct ib_uqp_object        	*obj;
	int                        	 ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	memset(&resp, 0, sizeof resp);

	uobj = idr_write_uobj(&ib_uverbs_qp_idr, cmd.qp_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;
	qp  = uobj->object;
	obj = container_of(uobj, struct ib_uqp_object, uevent.uobject);

	if (!list_empty(&obj->mcast_list)) {
		put_uobj_write(uobj);
		return -EBUSY;
	}

	ret = ib_destroy_qp(qp);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_qp_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	ib_uverbs_release_uevent(file, &obj->uevent);

	resp.events_reported = obj->uevent.events_reported;

	put_uobj(uobj);

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		return -EFAULT;

	return in_len;
}

ssize_t ib_uverbs_post_send(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
=======
	struct ib_uqp_object        	*obj;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_get_destroy(UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	obj = container_of(uobj, struct ib_uqp_object, uevent.uobject);
	memset(&resp, 0, sizeof(resp));
	resp.events_reported = obj->uevent.events_reported;

	uobj_put_destroy(uobj);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static void *alloc_wr(size_t wr_size, __u32 num_sge)
{
	if (num_sge >= (U32_MAX - ALIGN(wr_size, sizeof(struct ib_sge))) /
			       sizeof(struct ib_sge))
		return NULL;

	return kmalloc(ALIGN(wr_size, sizeof(struct ib_sge)) +
			       num_sge * sizeof(struct ib_sge),
		       GFP_KERNEL);
}

static int ib_uverbs_post_send(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_post_send      cmd;
	struct ib_uverbs_post_send_resp resp;
	struct ib_uverbs_send_wr       *user_wr;
<<<<<<< HEAD
	struct ib_send_wr              *wr = NULL, *last, *next, *bad_wr;
	struct ib_qp                   *qp;
	int                             i, sg_ind;
	int				is_ud;
	ssize_t                         ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	if (in_len < sizeof cmd + cmd.wqe_size * cmd.wr_count +
	    cmd.sge_count * sizeof (struct ib_uverbs_sge))
		return -EINVAL;

	if (cmd.wqe_size < sizeof (struct ib_uverbs_send_wr))
		return -EINVAL;
=======
	struct ib_send_wr              *wr = NULL, *last, *next;
	const struct ib_send_wr	       *bad_wr;
	struct ib_qp                   *qp;
	int                             i, sg_ind;
	int				is_ud;
	int ret, ret2;
	size_t                          next_size;
	const struct ib_sge __user *sgls;
	const void __user *wqes;
	struct uverbs_req_iter iter;

	ret = uverbs_request_start(attrs, &iter, &cmd, sizeof(cmd));
	if (ret)
		return ret;
	wqes = uverbs_request_next_ptr(&iter, cmd.wqe_size * cmd.wr_count);
	if (IS_ERR(wqes))
		return PTR_ERR(wqes);
	sgls = uverbs_request_next_ptr(
		&iter, cmd.sge_count * sizeof(struct ib_uverbs_sge));
	if (IS_ERR(sgls))
		return PTR_ERR(sgls);
	ret = uverbs_request_finish(&iter);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	user_wr = kmalloc(cmd.wqe_size, GFP_KERNEL);
	if (!user_wr)
		return -ENOMEM;

<<<<<<< HEAD
	qp = idr_read_qp(cmd.qp_handle, file->ucontext);
	if (!qp)
		goto out;
=======
	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (!qp) {
		ret = -EINVAL;
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	is_ud = qp->qp_type == IB_QPT_UD;
	sg_ind = 0;
	last = NULL;
	for (i = 0; i < cmd.wr_count; ++i) {
<<<<<<< HEAD
		if (copy_from_user(user_wr,
				   buf + sizeof cmd + i * cmd.wqe_size,
=======
		if (copy_from_user(user_wr, wqes + i * cmd.wqe_size,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   cmd.wqe_size)) {
			ret = -EFAULT;
			goto out_put;
		}

		if (user_wr->num_sge + sg_ind > cmd.sge_count) {
			ret = -EINVAL;
			goto out_put;
		}

<<<<<<< HEAD
		next = kmalloc(ALIGN(sizeof *next, sizeof (struct ib_sge)) +
			       user_wr->num_sge * sizeof (struct ib_sge),
			       GFP_KERNEL);
		if (!next) {
			ret = -ENOMEM;
			goto out_put;
		}

=======
		if (is_ud) {
			struct ib_ud_wr *ud;

			if (user_wr->opcode != IB_WR_SEND &&
			    user_wr->opcode != IB_WR_SEND_WITH_IMM) {
				ret = -EINVAL;
				goto out_put;
			}

			next_size = sizeof(*ud);
			ud = alloc_wr(next_size, user_wr->num_sge);
			if (!ud) {
				ret = -ENOMEM;
				goto out_put;
			}

			ud->ah = uobj_get_obj_read(ah, UVERBS_OBJECT_AH,
						   user_wr->wr.ud.ah, attrs);
			if (!ud->ah) {
				kfree(ud);
				ret = -EINVAL;
				goto out_put;
			}
			ud->remote_qpn = user_wr->wr.ud.remote_qpn;
			ud->remote_qkey = user_wr->wr.ud.remote_qkey;

			next = &ud->wr;
		} else if (user_wr->opcode == IB_WR_RDMA_WRITE_WITH_IMM ||
			   user_wr->opcode == IB_WR_RDMA_WRITE ||
			   user_wr->opcode == IB_WR_RDMA_READ) {
			struct ib_rdma_wr *rdma;

			next_size = sizeof(*rdma);
			rdma = alloc_wr(next_size, user_wr->num_sge);
			if (!rdma) {
				ret = -ENOMEM;
				goto out_put;
			}

			rdma->remote_addr = user_wr->wr.rdma.remote_addr;
			rdma->rkey = user_wr->wr.rdma.rkey;

			next = &rdma->wr;
		} else if (user_wr->opcode == IB_WR_ATOMIC_CMP_AND_SWP ||
			   user_wr->opcode == IB_WR_ATOMIC_FETCH_AND_ADD) {
			struct ib_atomic_wr *atomic;

			next_size = sizeof(*atomic);
			atomic = alloc_wr(next_size, user_wr->num_sge);
			if (!atomic) {
				ret = -ENOMEM;
				goto out_put;
			}

			atomic->remote_addr = user_wr->wr.atomic.remote_addr;
			atomic->compare_add = user_wr->wr.atomic.compare_add;
			atomic->swap = user_wr->wr.atomic.swap;
			atomic->rkey = user_wr->wr.atomic.rkey;

			next = &atomic->wr;
		} else if (user_wr->opcode == IB_WR_SEND ||
			   user_wr->opcode == IB_WR_SEND_WITH_IMM ||
			   user_wr->opcode == IB_WR_SEND_WITH_INV) {
			next_size = sizeof(*next);
			next = alloc_wr(next_size, user_wr->num_sge);
			if (!next) {
				ret = -ENOMEM;
				goto out_put;
			}
		} else {
			ret = -EINVAL;
			goto out_put;
		}

		if (user_wr->opcode == IB_WR_SEND_WITH_IMM ||
		    user_wr->opcode == IB_WR_RDMA_WRITE_WITH_IMM) {
			next->ex.imm_data =
					(__be32 __force) user_wr->ex.imm_data;
		} else if (user_wr->opcode == IB_WR_SEND_WITH_INV) {
			next->ex.invalidate_rkey = user_wr->ex.invalidate_rkey;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!last)
			wr = next;
		else
			last->next = next;
		last = next;

		next->next       = NULL;
		next->wr_id      = user_wr->wr_id;
		next->num_sge    = user_wr->num_sge;
		next->opcode     = user_wr->opcode;
		next->send_flags = user_wr->send_flags;

<<<<<<< HEAD
		if (is_ud) {
			if (next->opcode != IB_WR_SEND &&
			    next->opcode != IB_WR_SEND_WITH_IMM) {
				ret = -EINVAL;
				goto out_put;
			}

			next->wr.ud.ah = idr_read_ah(user_wr->wr.ud.ah,
						     file->ucontext);
			if (!next->wr.ud.ah) {
				ret = -EINVAL;
				goto out_put;
			}
			next->wr.ud.remote_qpn  = user_wr->wr.ud.remote_qpn;
			next->wr.ud.remote_qkey = user_wr->wr.ud.remote_qkey;
		} else {
			switch (next->opcode) {
			case IB_WR_RDMA_WRITE_WITH_IMM:
				next->ex.imm_data =
					(__be32 __force) user_wr->ex.imm_data;
			case IB_WR_RDMA_WRITE:
			case IB_WR_RDMA_READ:
				next->wr.rdma.remote_addr =
					user_wr->wr.rdma.remote_addr;
				next->wr.rdma.rkey        =
					user_wr->wr.rdma.rkey;
				break;
			case IB_WR_SEND_WITH_IMM:
				next->ex.imm_data =
					(__be32 __force) user_wr->ex.imm_data;
				break;
			case IB_WR_SEND_WITH_INV:
				next->ex.invalidate_rkey =
					user_wr->ex.invalidate_rkey;
				break;
			case IB_WR_ATOMIC_CMP_AND_SWP:
			case IB_WR_ATOMIC_FETCH_AND_ADD:
				next->wr.atomic.remote_addr =
					user_wr->wr.atomic.remote_addr;
				next->wr.atomic.compare_add =
					user_wr->wr.atomic.compare_add;
				next->wr.atomic.swap = user_wr->wr.atomic.swap;
				next->wr.atomic.rkey = user_wr->wr.atomic.rkey;
			case IB_WR_SEND:
				break;
			default:
				ret = -EINVAL;
				goto out_put;
			}
		}

		if (next->num_sge) {
			next->sg_list = (void *) next +
				ALIGN(sizeof *next, sizeof (struct ib_sge));
			if (copy_from_user(next->sg_list,
					   buf + sizeof cmd +
					   cmd.wr_count * cmd.wqe_size +
					   sg_ind * sizeof (struct ib_sge),
					   next->num_sge * sizeof (struct ib_sge))) {
=======
		if (next->num_sge) {
			next->sg_list = (void *) next +
				ALIGN(next_size, sizeof(struct ib_sge));
			if (copy_from_user(next->sg_list, sgls + sg_ind,
					   next->num_sge *
						   sizeof(struct ib_sge))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ret = -EFAULT;
				goto out_put;
			}
			sg_ind += next->num_sge;
		} else
			next->sg_list = NULL;
	}

	resp.bad_wr = 0;
<<<<<<< HEAD
	ret = qp->device->post_send(qp->real_qp, wr, &bad_wr);
=======
	ret = qp->device->ops.post_send(qp->real_qp, wr, &bad_wr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		for (next = wr; next; next = next->next) {
			++resp.bad_wr;
			if (next == bad_wr)
				break;
		}

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		ret = -EFAULT;

out_put:
	put_qp_read(qp);

	while (wr) {
		if (is_ud && wr->wr.ud.ah)
			put_ah_read(wr->wr.ud.ah);
=======
	ret2 = uverbs_response(attrs, &resp, sizeof(resp));
	if (ret2)
		ret = ret2;

out_put:
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);

	while (wr) {
		if (is_ud && ud_wr(wr)->ah)
			uobj_put_obj_read(ud_wr(wr)->ah);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		next = wr->next;
		kfree(wr);
		wr = next;
	}

out:
	kfree(user_wr);

<<<<<<< HEAD
	return ret ? ret : in_len;
}

static struct ib_recv_wr *ib_uverbs_unmarshall_recv(const char __user *buf,
						    int in_len,
						    u32 wr_count,
						    u32 sge_count,
						    u32 wqe_size)
=======
	return ret;
}

static struct ib_recv_wr *
ib_uverbs_unmarshall_recv(struct uverbs_req_iter *iter, u32 wr_count,
			  u32 wqe_size, u32 sge_count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_recv_wr *user_wr;
	struct ib_recv_wr        *wr = NULL, *last, *next;
	int                       sg_ind;
	int                       i;
	int                       ret;
<<<<<<< HEAD

	if (in_len < wqe_size * wr_count +
	    sge_count * sizeof (struct ib_uverbs_sge))
		return ERR_PTR(-EINVAL);

	if (wqe_size < sizeof (struct ib_uverbs_recv_wr))
		return ERR_PTR(-EINVAL);
=======
	const struct ib_sge __user *sgls;
	const void __user *wqes;

	if (wqe_size < sizeof(struct ib_uverbs_recv_wr))
		return ERR_PTR(-EINVAL);

	wqes = uverbs_request_next_ptr(iter, wqe_size * wr_count);
	if (IS_ERR(wqes))
		return ERR_CAST(wqes);
	sgls = uverbs_request_next_ptr(
		iter, sge_count * sizeof(struct ib_uverbs_sge));
	if (IS_ERR(sgls))
		return ERR_CAST(sgls);
	ret = uverbs_request_finish(iter);
	if (ret)
		return ERR_PTR(ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	user_wr = kmalloc(wqe_size, GFP_KERNEL);
	if (!user_wr)
		return ERR_PTR(-ENOMEM);

	sg_ind = 0;
	last = NULL;
	for (i = 0; i < wr_count; ++i) {
<<<<<<< HEAD
		if (copy_from_user(user_wr, buf + i * wqe_size,
=======
		if (copy_from_user(user_wr, wqes + i * wqe_size,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   wqe_size)) {
			ret = -EFAULT;
			goto err;
		}

		if (user_wr->num_sge + sg_ind > sge_count) {
			ret = -EINVAL;
			goto err;
		}

<<<<<<< HEAD
		next = kmalloc(ALIGN(sizeof *next, sizeof (struct ib_sge)) +
			       user_wr->num_sge * sizeof (struct ib_sge),
=======
		if (user_wr->num_sge >=
		    (U32_MAX - ALIGN(sizeof(*next), sizeof(struct ib_sge))) /
			    sizeof(struct ib_sge)) {
			ret = -EINVAL;
			goto err;
		}

		next = kmalloc(ALIGN(sizeof(*next), sizeof(struct ib_sge)) +
				       user_wr->num_sge * sizeof(struct ib_sge),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       GFP_KERNEL);
		if (!next) {
			ret = -ENOMEM;
			goto err;
		}

		if (!last)
			wr = next;
		else
			last->next = next;
		last = next;

		next->next       = NULL;
		next->wr_id      = user_wr->wr_id;
		next->num_sge    = user_wr->num_sge;

		if (next->num_sge) {
<<<<<<< HEAD
			next->sg_list = (void *) next +
				ALIGN(sizeof *next, sizeof (struct ib_sge));
			if (copy_from_user(next->sg_list,
					   buf + wr_count * wqe_size +
					   sg_ind * sizeof (struct ib_sge),
					   next->num_sge * sizeof (struct ib_sge))) {
=======
			next->sg_list = (void *)next +
				ALIGN(sizeof(*next), sizeof(struct ib_sge));
			if (copy_from_user(next->sg_list, sgls + sg_ind,
					   next->num_sge *
						   sizeof(struct ib_sge))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ret = -EFAULT;
				goto err;
			}
			sg_ind += next->num_sge;
		} else
			next->sg_list = NULL;
	}

	kfree(user_wr);
	return wr;

err:
	kfree(user_wr);

	while (wr) {
		next = wr->next;
		kfree(wr);
		wr = next;
	}

	return ERR_PTR(ret);
}

<<<<<<< HEAD
ssize_t ib_uverbs_post_recv(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
{
	struct ib_uverbs_post_recv      cmd;
	struct ib_uverbs_post_recv_resp resp;
	struct ib_recv_wr              *wr, *next, *bad_wr;
	struct ib_qp                   *qp;
	ssize_t                         ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	wr = ib_uverbs_unmarshall_recv(buf + sizeof cmd,
				       in_len - sizeof cmd, cmd.wr_count,
				       cmd.sge_count, cmd.wqe_size);
	if (IS_ERR(wr))
		return PTR_ERR(wr);

	qp = idr_read_qp(cmd.qp_handle, file->ucontext);
	if (!qp)
		goto out;

	resp.bad_wr = 0;
	ret = qp->device->post_recv(qp->real_qp, wr, &bad_wr);

	put_qp_read(qp);

	if (ret)
=======
static int ib_uverbs_post_recv(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_post_recv      cmd;
	struct ib_uverbs_post_recv_resp resp;
	struct ib_recv_wr              *wr, *next;
	const struct ib_recv_wr	       *bad_wr;
	struct ib_qp                   *qp;
	int ret, ret2;
	struct uverbs_req_iter iter;

	ret = uverbs_request_start(attrs, &iter, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	wr = ib_uverbs_unmarshall_recv(&iter, cmd.wr_count, cmd.wqe_size,
				       cmd.sge_count);
	if (IS_ERR(wr))
		return PTR_ERR(wr);

	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (!qp) {
		ret = -EINVAL;
		goto out;
	}

	resp.bad_wr = 0;
	ret = qp->device->ops.post_recv(qp->real_qp, wr, &bad_wr);

	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	if (ret) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (next = wr; next; next = next->next) {
			++resp.bad_wr;
			if (next == bad_wr)
				break;
		}
<<<<<<< HEAD

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		ret = -EFAULT;

=======
	}

	ret2 = uverbs_response(attrs, &resp, sizeof(resp));
	if (ret2)
		ret = ret2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	while (wr) {
		next = wr->next;
		kfree(wr);
		wr = next;
	}

<<<<<<< HEAD
	return ret ? ret : in_len;
}

ssize_t ib_uverbs_post_srq_recv(struct ib_uverbs_file *file,
				const char __user *buf, int in_len,
				int out_len)
{
	struct ib_uverbs_post_srq_recv      cmd;
	struct ib_uverbs_post_srq_recv_resp resp;
	struct ib_recv_wr                  *wr, *next, *bad_wr;
	struct ib_srq                      *srq;
	ssize_t                             ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	wr = ib_uverbs_unmarshall_recv(buf + sizeof cmd,
				       in_len - sizeof cmd, cmd.wr_count,
				       cmd.sge_count, cmd.wqe_size);
	if (IS_ERR(wr))
		return PTR_ERR(wr);

	srq = idr_read_srq(cmd.srq_handle, file->ucontext);
	if (!srq)
		goto out;

	resp.bad_wr = 0;
	ret = srq->device->post_srq_recv(srq, wr, &bad_wr);

	put_srq_read(srq);
=======
	return ret;
}

static int ib_uverbs_post_srq_recv(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_post_srq_recv      cmd;
	struct ib_uverbs_post_srq_recv_resp resp;
	struct ib_recv_wr                  *wr, *next;
	const struct ib_recv_wr		   *bad_wr;
	struct ib_srq                      *srq;
	int ret, ret2;
	struct uverbs_req_iter iter;

	ret = uverbs_request_start(attrs, &iter, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	wr = ib_uverbs_unmarshall_recv(&iter, cmd.wr_count, cmd.wqe_size,
				       cmd.sge_count);
	if (IS_ERR(wr))
		return PTR_ERR(wr);

	srq = uobj_get_obj_read(srq, UVERBS_OBJECT_SRQ, cmd.srq_handle, attrs);
	if (!srq) {
		ret = -EINVAL;
		goto out;
	}

	resp.bad_wr = 0;
	ret = srq->device->ops.post_srq_recv(srq, wr, &bad_wr);

	rdma_lookup_put_uobject(&srq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret)
		for (next = wr; next; next = next->next) {
			++resp.bad_wr;
			if (next == bad_wr)
				break;
		}

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		ret = -EFAULT;
=======
	ret2 = uverbs_response(attrs, &resp, sizeof(resp));
	if (ret2)
		ret = ret2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	while (wr) {
		next = wr->next;
		kfree(wr);
		wr = next;
	}

<<<<<<< HEAD
	return ret ? ret : in_len;
}

ssize_t ib_uverbs_create_ah(struct ib_uverbs_file *file,
			    const char __user *buf, int in_len,
			    int out_len)
=======
	return ret;
}

static int ib_uverbs_create_ah(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_create_ah	 cmd;
	struct ib_uverbs_create_ah_resp	 resp;
	struct ib_uobject		*uobj;
	struct ib_pd			*pd;
	struct ib_ah			*ah;
<<<<<<< HEAD
	struct ib_ah_attr		attr;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = kmalloc(sizeof *uobj, GFP_KERNEL);
	if (!uobj)
		return -ENOMEM;

	init_uobj(uobj, cmd.user_handle, file->ucontext, &ah_lock_key);
	down_write(&uobj->mutex);

	pd = idr_read_pd(cmd.pd_handle, file->ucontext);
=======
	struct rdma_ah_attr		attr = {};
	int ret;
	struct ib_device *ib_dev;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_alloc(UVERBS_OBJECT_AH, attrs, &ib_dev);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	if (!rdma_is_port_valid(ib_dev, cmd.attr.port_num)) {
		ret = -EINVAL;
		goto err;
	}

	pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd.pd_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pd) {
		ret = -EINVAL;
		goto err;
	}

<<<<<<< HEAD
	attr.dlid 	       = cmd.attr.dlid;
	attr.sl 	       = cmd.attr.sl;
	attr.src_path_bits     = cmd.attr.src_path_bits;
	attr.static_rate       = cmd.attr.static_rate;
	attr.ah_flags          = cmd.attr.is_global ? IB_AH_GRH : 0;
	attr.port_num 	       = cmd.attr.port_num;
	attr.grh.flow_label    = cmd.attr.grh.flow_label;
	attr.grh.sgid_index    = cmd.attr.grh.sgid_index;
	attr.grh.hop_limit     = cmd.attr.grh.hop_limit;
	attr.grh.traffic_class = cmd.attr.grh.traffic_class;
	memcpy(attr.grh.dgid.raw, cmd.attr.grh.dgid, 16);

	ah = ib_create_ah(pd, &attr);
=======
	attr.type = rdma_ah_find_type(ib_dev, cmd.attr.port_num);
	rdma_ah_set_make_grd(&attr, false);
	rdma_ah_set_dlid(&attr, cmd.attr.dlid);
	rdma_ah_set_sl(&attr, cmd.attr.sl);
	rdma_ah_set_path_bits(&attr, cmd.attr.src_path_bits);
	rdma_ah_set_static_rate(&attr, cmd.attr.static_rate);
	rdma_ah_set_port_num(&attr, cmd.attr.port_num);

	if (cmd.attr.is_global) {
		rdma_ah_set_grh(&attr, NULL, cmd.attr.grh.flow_label,
				cmd.attr.grh.sgid_index,
				cmd.attr.grh.hop_limit,
				cmd.attr.grh.traffic_class);
		rdma_ah_set_dgid_raw(&attr, cmd.attr.grh.dgid);
	} else {
		rdma_ah_set_ah_flags(&attr, 0);
	}

	ah = rdma_create_user_ah(pd, &attr, &attrs->driver_udata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ah)) {
		ret = PTR_ERR(ah);
		goto err_put;
	}

	ah->uobject  = uobj;
<<<<<<< HEAD
	uobj->object = ah;

	ret = idr_add_uobj(&ib_uverbs_ah_idr, uobj);
	if (ret)
		goto err_destroy;

	resp.ah_handle = uobj->id;

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	put_pd_read(pd);

	mutex_lock(&file->mutex);
	list_add_tail(&uobj->list, &file->ucontext->ah_list);
	mutex_unlock(&file->mutex);

	uobj->live = 1;

	up_write(&uobj->mutex);

	return in_len;

err_copy:
	idr_remove_uobj(&ib_uverbs_ah_idr, uobj);

err_destroy:
	ib_destroy_ah(ah);

err_put:
	put_pd_read(pd);

err:
	put_uobj_write(uobj);
	return ret;
}

ssize_t ib_uverbs_destroy_ah(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len, int out_len)
{
	struct ib_uverbs_destroy_ah cmd;
	struct ib_ah		   *ah;
	struct ib_uobject	   *uobj;
	int			    ret;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = idr_write_uobj(&ib_uverbs_ah_idr, cmd.ah_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;
	ah = uobj->object;

	ret = ib_destroy_ah(ah);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_ah_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	put_uobj(uobj);

	return in_len;
}

ssize_t ib_uverbs_attach_mcast(struct ib_uverbs_file *file,
			       const char __user *buf, int in_len,
			       int out_len)
=======
	uobj->user_handle = cmd.user_handle;
	uobj->object = ah;
	uobj_put_obj_read(pd);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.ah_handle = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_put:
	uobj_put_obj_read(pd);
err:
	uobj_alloc_abort(uobj, attrs);
	return ret;
}

static int ib_uverbs_destroy_ah(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_destroy_ah cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return uobj_perform_destroy(UVERBS_OBJECT_AH, cmd.ah_handle, attrs);
}

static int ib_uverbs_attach_mcast(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_attach_mcast cmd;
	struct ib_qp                 *qp;
	struct ib_uqp_object         *obj;
	struct ib_uverbs_mcast_entry *mcast;
	int                           ret;

<<<<<<< HEAD
	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	qp = idr_write_qp(cmd.qp_handle, file->ucontext);
	if (!qp)
		return -EINVAL;

	obj = container_of(qp->uobject, struct ib_uqp_object, uevent.uobject);

=======
	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (!qp)
		return -EINVAL;

	obj = qp->uobject;

	mutex_lock(&obj->mcast_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(mcast, &obj->mcast_list, list)
		if (cmd.mlid == mcast->lid &&
		    !memcmp(cmd.gid, mcast->gid.raw, sizeof mcast->gid.raw)) {
			ret = 0;
			goto out_put;
		}

	mcast = kmalloc(sizeof *mcast, GFP_KERNEL);
	if (!mcast) {
		ret = -ENOMEM;
		goto out_put;
	}

	mcast->lid = cmd.mlid;
	memcpy(mcast->gid.raw, cmd.gid, sizeof mcast->gid.raw);

	ret = ib_attach_mcast(qp, &mcast->gid, cmd.mlid);
	if (!ret)
		list_add_tail(&mcast->list, &obj->mcast_list);
	else
		kfree(mcast);

out_put:
<<<<<<< HEAD
	put_qp_write(qp);

	return ret ? ret : in_len;
}

ssize_t ib_uverbs_detach_mcast(struct ib_uverbs_file *file,
			       const char __user *buf, int in_len,
			       int out_len)
=======
	mutex_unlock(&obj->mcast_lock);
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);

	return ret;
}

static int ib_uverbs_detach_mcast(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_detach_mcast cmd;
	struct ib_uqp_object         *obj;
	struct ib_qp                 *qp;
	struct ib_uverbs_mcast_entry *mcast;
<<<<<<< HEAD
	int                           ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	qp = idr_write_qp(cmd.qp_handle, file->ucontext);
	if (!qp)
		return -EINVAL;

	ret = ib_detach_mcast(qp, (union ib_gid *) cmd.gid, cmd.mlid);
	if (ret)
		goto out_put;

	obj = container_of(qp->uobject, struct ib_uqp_object, uevent.uobject);
=======
	int                           ret;
	bool                          found = false;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (!qp)
		return -EINVAL;

	obj = qp->uobject;
	mutex_lock(&obj->mcast_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_for_each_entry(mcast, &obj->mcast_list, list)
		if (cmd.mlid == mcast->lid &&
		    !memcmp(cmd.gid, mcast->gid.raw, sizeof mcast->gid.raw)) {
			list_del(&mcast->list);
			kfree(mcast);
<<<<<<< HEAD
			break;
		}

out_put:
	put_qp_write(qp);

	return ret ? ret : in_len;
}

static int __uverbs_create_xsrq(struct ib_uverbs_file *file,
				struct ib_uverbs_create_xsrq *cmd,
				struct ib_udata *udata)
{
	struct ib_uverbs_create_srq_resp resp;
	struct ib_usrq_object           *obj;
	struct ib_pd                    *pd;
	struct ib_srq                   *srq;
	struct ib_uobject               *uninitialized_var(xrcd_uobj);
	struct ib_srq_init_attr          attr;
	int ret;

	obj = kmalloc(sizeof *obj, GFP_KERNEL);
	if (!obj)
		return -ENOMEM;

	init_uobj(&obj->uevent.uobject, cmd->user_handle, file->ucontext, &srq_lock_key);
	down_write(&obj->uevent.uobject.mutex);

	pd  = idr_read_pd(cmd->pd_handle, file->ucontext);
	if (!pd) {
		ret = -EINVAL;
		goto err;
	}

	if (cmd->srq_type == IB_SRQT_XRC) {
		attr.ext.xrc.cq  = idr_read_cq(cmd->cq_handle, file->ucontext, 0);
		if (!attr.ext.xrc.cq) {
			ret = -EINVAL;
			goto err_put_pd;
		}

		attr.ext.xrc.xrcd  = idr_read_xrcd(cmd->xrcd_handle, file->ucontext, &xrcd_uobj);
		if (!attr.ext.xrc.xrcd) {
			ret = -EINVAL;
			goto err_put_cq;
=======
			found = true;
			break;
		}

	if (!found) {
		ret = -EINVAL;
		goto out_put;
	}

	ret = ib_detach_mcast(qp, (union ib_gid *)cmd.gid, cmd.mlid);

out_put:
	mutex_unlock(&obj->mcast_lock);
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	return ret;
}

struct ib_uflow_resources *flow_resources_alloc(size_t num_specs)
{
	struct ib_uflow_resources *resources;

	resources = kzalloc(sizeof(*resources), GFP_KERNEL);

	if (!resources)
		return NULL;

	if (!num_specs)
		goto out;

	resources->counters =
		kcalloc(num_specs, sizeof(*resources->counters), GFP_KERNEL);
	resources->collection =
		kcalloc(num_specs, sizeof(*resources->collection), GFP_KERNEL);

	if (!resources->counters || !resources->collection)
		goto err;

out:
	resources->max = num_specs;
	return resources;

err:
	kfree(resources->counters);
	kfree(resources);

	return NULL;
}
EXPORT_SYMBOL(flow_resources_alloc);

void ib_uverbs_flow_resources_free(struct ib_uflow_resources *uflow_res)
{
	unsigned int i;

	if (!uflow_res)
		return;

	for (i = 0; i < uflow_res->collection_num; i++)
		atomic_dec(&uflow_res->collection[i]->usecnt);

	for (i = 0; i < uflow_res->counters_num; i++)
		atomic_dec(&uflow_res->counters[i]->usecnt);

	kfree(uflow_res->collection);
	kfree(uflow_res->counters);
	kfree(uflow_res);
}
EXPORT_SYMBOL(ib_uverbs_flow_resources_free);

void flow_resources_add(struct ib_uflow_resources *uflow_res,
			enum ib_flow_spec_type type,
			void *ibobj)
{
	WARN_ON(uflow_res->num >= uflow_res->max);

	switch (type) {
	case IB_FLOW_SPEC_ACTION_HANDLE:
		atomic_inc(&((struct ib_flow_action *)ibobj)->usecnt);
		uflow_res->collection[uflow_res->collection_num++] =
			(struct ib_flow_action *)ibobj;
		break;
	case IB_FLOW_SPEC_ACTION_COUNT:
		atomic_inc(&((struct ib_counters *)ibobj)->usecnt);
		uflow_res->counters[uflow_res->counters_num++] =
			(struct ib_counters *)ibobj;
		break;
	default:
		WARN_ON(1);
	}

	uflow_res->num++;
}
EXPORT_SYMBOL(flow_resources_add);

static int kern_spec_to_ib_spec_action(struct uverbs_attr_bundle *attrs,
				       struct ib_uverbs_flow_spec *kern_spec,
				       union ib_flow_spec *ib_spec,
				       struct ib_uflow_resources *uflow_res)
{
	ib_spec->type = kern_spec->type;
	switch (ib_spec->type) {
	case IB_FLOW_SPEC_ACTION_TAG:
		if (kern_spec->flow_tag.size !=
		    sizeof(struct ib_uverbs_flow_spec_action_tag))
			return -EINVAL;

		ib_spec->flow_tag.size = sizeof(struct ib_flow_spec_action_tag);
		ib_spec->flow_tag.tag_id = kern_spec->flow_tag.tag_id;
		break;
	case IB_FLOW_SPEC_ACTION_DROP:
		if (kern_spec->drop.size !=
		    sizeof(struct ib_uverbs_flow_spec_action_drop))
			return -EINVAL;

		ib_spec->drop.size = sizeof(struct ib_flow_spec_action_drop);
		break;
	case IB_FLOW_SPEC_ACTION_HANDLE:
		if (kern_spec->action.size !=
		    sizeof(struct ib_uverbs_flow_spec_action_handle))
			return -EOPNOTSUPP;
		ib_spec->action.act = uobj_get_obj_read(flow_action,
							UVERBS_OBJECT_FLOW_ACTION,
							kern_spec->action.handle,
							attrs);
		if (!ib_spec->action.act)
			return -EINVAL;
		ib_spec->action.size =
			sizeof(struct ib_flow_spec_action_handle);
		flow_resources_add(uflow_res,
				   IB_FLOW_SPEC_ACTION_HANDLE,
				   ib_spec->action.act);
		uobj_put_obj_read(ib_spec->action.act);
		break;
	case IB_FLOW_SPEC_ACTION_COUNT:
		if (kern_spec->flow_count.size !=
			sizeof(struct ib_uverbs_flow_spec_action_count))
			return -EINVAL;
		ib_spec->flow_count.counters =
			uobj_get_obj_read(counters,
					  UVERBS_OBJECT_COUNTERS,
					  kern_spec->flow_count.handle,
					  attrs);
		if (!ib_spec->flow_count.counters)
			return -EINVAL;
		ib_spec->flow_count.size =
				sizeof(struct ib_flow_spec_action_count);
		flow_resources_add(uflow_res,
				   IB_FLOW_SPEC_ACTION_COUNT,
				   ib_spec->flow_count.counters);
		uobj_put_obj_read(ib_spec->flow_count.counters);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static ssize_t spec_filter_size(const void *kern_spec_filter, u16 kern_filter_size,
				u16 ib_real_filter_sz)
{
	/*
	 * User space filter structures must be 64 bit aligned, otherwise this
	 * may pass, but we won't handle additional new attributes.
	 */

	if (kern_filter_size > ib_real_filter_sz) {
		if (memchr_inv(kern_spec_filter +
			       ib_real_filter_sz, 0,
			       kern_filter_size - ib_real_filter_sz))
			return -EINVAL;
		return ib_real_filter_sz;
	}
	return kern_filter_size;
}

int ib_uverbs_kern_spec_to_ib_spec_filter(enum ib_flow_spec_type type,
					  const void *kern_spec_mask,
					  const void *kern_spec_val,
					  size_t kern_filter_sz,
					  union ib_flow_spec *ib_spec)
{
	ssize_t actual_filter_sz;
	ssize_t ib_filter_sz;

	/* User flow spec size must be aligned to 4 bytes */
	if (kern_filter_sz != ALIGN(kern_filter_sz, 4))
		return -EINVAL;

	ib_spec->type = type;

	if (ib_spec->type == (IB_FLOW_SPEC_INNER | IB_FLOW_SPEC_VXLAN_TUNNEL))
		return -EINVAL;

	switch (ib_spec->type & ~IB_FLOW_SPEC_INNER) {
	case IB_FLOW_SPEC_ETH:
		ib_filter_sz = sizeof(struct ib_flow_eth_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->size = sizeof(struct ib_flow_spec_eth);
		memcpy(&ib_spec->eth.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->eth.mask, kern_spec_mask, actual_filter_sz);
		break;
	case IB_FLOW_SPEC_IPV4:
		ib_filter_sz = sizeof(struct ib_flow_ipv4_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->size = sizeof(struct ib_flow_spec_ipv4);
		memcpy(&ib_spec->ipv4.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->ipv4.mask, kern_spec_mask, actual_filter_sz);
		break;
	case IB_FLOW_SPEC_IPV6:
		ib_filter_sz = sizeof(struct ib_flow_ipv6_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->size = sizeof(struct ib_flow_spec_ipv6);
		memcpy(&ib_spec->ipv6.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->ipv6.mask, kern_spec_mask, actual_filter_sz);

		if ((ntohl(ib_spec->ipv6.mask.flow_label)) >= BIT(20) ||
		    (ntohl(ib_spec->ipv6.val.flow_label)) >= BIT(20))
			return -EINVAL;
		break;
	case IB_FLOW_SPEC_TCP:
	case IB_FLOW_SPEC_UDP:
		ib_filter_sz = sizeof(struct ib_flow_tcp_udp_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->size = sizeof(struct ib_flow_spec_tcp_udp);
		memcpy(&ib_spec->tcp_udp.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->tcp_udp.mask, kern_spec_mask, actual_filter_sz);
		break;
	case IB_FLOW_SPEC_VXLAN_TUNNEL:
		ib_filter_sz = sizeof(struct ib_flow_tunnel_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->tunnel.size = sizeof(struct ib_flow_spec_tunnel);
		memcpy(&ib_spec->tunnel.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->tunnel.mask, kern_spec_mask, actual_filter_sz);

		if ((ntohl(ib_spec->tunnel.mask.tunnel_id)) >= BIT(24) ||
		    (ntohl(ib_spec->tunnel.val.tunnel_id)) >= BIT(24))
			return -EINVAL;
		break;
	case IB_FLOW_SPEC_ESP:
		ib_filter_sz = sizeof(struct ib_flow_esp_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->esp.size = sizeof(struct ib_flow_spec_esp);
		memcpy(&ib_spec->esp.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->esp.mask, kern_spec_mask, actual_filter_sz);
		break;
	case IB_FLOW_SPEC_GRE:
		ib_filter_sz = sizeof(struct ib_flow_gre_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->gre.size = sizeof(struct ib_flow_spec_gre);
		memcpy(&ib_spec->gre.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->gre.mask, kern_spec_mask, actual_filter_sz);
		break;
	case IB_FLOW_SPEC_MPLS:
		ib_filter_sz = sizeof(struct ib_flow_mpls_filter);
		actual_filter_sz = spec_filter_size(kern_spec_mask,
						    kern_filter_sz,
						    ib_filter_sz);
		if (actual_filter_sz <= 0)
			return -EINVAL;
		ib_spec->mpls.size = sizeof(struct ib_flow_spec_mpls);
		memcpy(&ib_spec->mpls.val, kern_spec_val, actual_filter_sz);
		memcpy(&ib_spec->mpls.mask, kern_spec_mask, actual_filter_sz);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int kern_spec_to_ib_spec_filter(struct ib_uverbs_flow_spec *kern_spec,
				       union ib_flow_spec *ib_spec)
{
	size_t kern_filter_sz;
	void *kern_spec_mask;
	void *kern_spec_val;

	if (check_sub_overflow((size_t)kern_spec->hdr.size,
			       sizeof(struct ib_uverbs_flow_spec_hdr),
			       &kern_filter_sz))
		return -EINVAL;

	kern_filter_sz /= 2;

	kern_spec_val = (void *)kern_spec +
		sizeof(struct ib_uverbs_flow_spec_hdr);
	kern_spec_mask = kern_spec_val + kern_filter_sz;

	return ib_uverbs_kern_spec_to_ib_spec_filter(kern_spec->type,
						     kern_spec_mask,
						     kern_spec_val,
						     kern_filter_sz, ib_spec);
}

static int kern_spec_to_ib_spec(struct uverbs_attr_bundle *attrs,
				struct ib_uverbs_flow_spec *kern_spec,
				union ib_flow_spec *ib_spec,
				struct ib_uflow_resources *uflow_res)
{
	if (kern_spec->reserved)
		return -EINVAL;

	if (kern_spec->type >= IB_FLOW_SPEC_ACTION_TAG)
		return kern_spec_to_ib_spec_action(attrs, kern_spec, ib_spec,
						   uflow_res);
	else
		return kern_spec_to_ib_spec_filter(kern_spec, ib_spec);
}

static int ib_uverbs_ex_create_wq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_create_wq cmd;
	struct ib_uverbs_ex_create_wq_resp resp = {};
	struct ib_uwq_object           *obj;
	int err = 0;
	struct ib_cq *cq;
	struct ib_pd *pd;
	struct ib_wq *wq;
	struct ib_wq_init_attr wq_init_attr = {};
	struct ib_device *ib_dev;

	err = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (err)
		return err;

	if (cmd.comp_mask)
		return -EOPNOTSUPP;

	obj = (struct ib_uwq_object *)uobj_alloc(UVERBS_OBJECT_WQ, attrs,
						 &ib_dev);
	if (IS_ERR(obj))
		return PTR_ERR(obj);

	pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd.pd_handle, attrs);
	if (!pd) {
		err = -EINVAL;
		goto err_uobj;
	}

	cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
	if (!cq) {
		err = -EINVAL;
		goto err_put_pd;
	}

	wq_init_attr.cq = cq;
	wq_init_attr.max_sge = cmd.max_sge;
	wq_init_attr.max_wr = cmd.max_wr;
	wq_init_attr.wq_type = cmd.wq_type;
	wq_init_attr.event_handler = ib_uverbs_wq_event_handler;
	wq_init_attr.create_flags = cmd.create_flags;
	INIT_LIST_HEAD(&obj->uevent.event_list);
	obj->uevent.uobject.user_handle = cmd.user_handle;

	wq = pd->device->ops.create_wq(pd, &wq_init_attr, &attrs->driver_udata);
	if (IS_ERR(wq)) {
		err = PTR_ERR(wq);
		goto err_put_cq;
	}

	wq->uobject = obj;
	obj->uevent.uobject.object = wq;
	wq->wq_type = wq_init_attr.wq_type;
	wq->cq = cq;
	wq->pd = pd;
	wq->device = pd->device;
	atomic_set(&wq->usecnt, 0);
	atomic_inc(&pd->usecnt);
	atomic_inc(&cq->usecnt);
	obj->uevent.event_file = READ_ONCE(attrs->ufile->default_async_file);
	if (obj->uevent.event_file)
		uverbs_uobject_get(&obj->uevent.event_file->uobj);

	uobj_put_obj_read(pd);
	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	uobj_finalize_uobj_create(&obj->uevent.uobject, attrs);

	resp.wq_handle = obj->uevent.uobject.id;
	resp.max_sge = wq_init_attr.max_sge;
	resp.max_wr = wq_init_attr.max_wr;
	resp.wqn = wq->wq_num;
	resp.response_length = uverbs_response_length(attrs, sizeof(resp));
	return uverbs_response(attrs, &resp, sizeof(resp));

err_put_cq:
	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
err_put_pd:
	uobj_put_obj_read(pd);
err_uobj:
	uobj_alloc_abort(&obj->uevent.uobject, attrs);

	return err;
}

static int ib_uverbs_ex_destroy_wq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_destroy_wq	cmd;
	struct ib_uverbs_ex_destroy_wq_resp	resp = {};
	struct ib_uobject		*uobj;
	struct ib_uwq_object		*obj;
	int				ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (cmd.comp_mask)
		return -EOPNOTSUPP;

	resp.response_length = uverbs_response_length(attrs, sizeof(resp));
	uobj = uobj_get_destroy(UVERBS_OBJECT_WQ, cmd.wq_handle, attrs);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	obj = container_of(uobj, struct ib_uwq_object, uevent.uobject);
	resp.events_reported = obj->uevent.events_reported;

	uobj_put_destroy(uobj);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_ex_modify_wq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_modify_wq cmd;
	struct ib_wq *wq;
	struct ib_wq_attr wq_attr = {};
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (!cmd.attr_mask)
		return -EINVAL;

	if (cmd.attr_mask > (IB_WQ_STATE | IB_WQ_CUR_STATE | IB_WQ_FLAGS))
		return -EINVAL;

	wq = uobj_get_obj_read(wq, UVERBS_OBJECT_WQ, cmd.wq_handle, attrs);
	if (!wq)
		return -EINVAL;

	if (cmd.attr_mask & IB_WQ_FLAGS) {
		wq_attr.flags = cmd.flags;
		wq_attr.flags_mask = cmd.flags_mask;
	}

	if (cmd.attr_mask & IB_WQ_CUR_STATE) {
		if (cmd.curr_wq_state > IB_WQS_ERR)
			return -EINVAL;

		wq_attr.curr_wq_state = cmd.curr_wq_state;
	} else {
		wq_attr.curr_wq_state = wq->state;
	}

	if (cmd.attr_mask & IB_WQ_STATE) {
		if (cmd.wq_state > IB_WQS_ERR)
			return -EINVAL;

		wq_attr.wq_state = cmd.wq_state;
	} else {
		wq_attr.wq_state = wq_attr.curr_wq_state;
	}

	ret = wq->device->ops.modify_wq(wq, &wq_attr, cmd.attr_mask,
					&attrs->driver_udata);
	rdma_lookup_put_uobject(&wq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	return ret;
}

static int ib_uverbs_ex_create_rwq_ind_table(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_create_rwq_ind_table cmd;
	struct ib_uverbs_ex_create_rwq_ind_table_resp  resp = {};
	struct ib_uobject *uobj;
	int err;
	struct ib_rwq_ind_table_init_attr init_attr = {};
	struct ib_rwq_ind_table *rwq_ind_tbl;
	struct ib_wq **wqs = NULL;
	u32 *wqs_handles = NULL;
	struct ib_wq	*wq = NULL;
	int i, num_read_wqs;
	u32 num_wq_handles;
	struct uverbs_req_iter iter;
	struct ib_device *ib_dev;

	err = uverbs_request_start(attrs, &iter, &cmd, sizeof(cmd));
	if (err)
		return err;

	if (cmd.comp_mask)
		return -EOPNOTSUPP;

	if (cmd.log_ind_tbl_size > IB_USER_VERBS_MAX_LOG_IND_TBL_SIZE)
		return -EINVAL;

	num_wq_handles = 1 << cmd.log_ind_tbl_size;
	wqs_handles = kcalloc(num_wq_handles, sizeof(*wqs_handles),
			      GFP_KERNEL);
	if (!wqs_handles)
		return -ENOMEM;

	err = uverbs_request_next(&iter, wqs_handles,
				  num_wq_handles * sizeof(__u32));
	if (err)
		goto err_free;

	err = uverbs_request_finish(&iter);
	if (err)
		goto err_free;

	wqs = kcalloc(num_wq_handles, sizeof(*wqs), GFP_KERNEL);
	if (!wqs) {
		err = -ENOMEM;
		goto  err_free;
	}

	for (num_read_wqs = 0; num_read_wqs < num_wq_handles;
			num_read_wqs++) {
		wq = uobj_get_obj_read(wq, UVERBS_OBJECT_WQ,
				       wqs_handles[num_read_wqs], attrs);
		if (!wq) {
			err = -EINVAL;
			goto put_wqs;
		}

		wqs[num_read_wqs] = wq;
		atomic_inc(&wqs[num_read_wqs]->usecnt);
	}

	uobj = uobj_alloc(UVERBS_OBJECT_RWQ_IND_TBL, attrs, &ib_dev);
	if (IS_ERR(uobj)) {
		err = PTR_ERR(uobj);
		goto put_wqs;
	}

	rwq_ind_tbl = rdma_zalloc_drv_obj(ib_dev, ib_rwq_ind_table);
	if (!rwq_ind_tbl) {
		err = -ENOMEM;
		goto err_uobj;
	}

	init_attr.log_ind_tbl_size = cmd.log_ind_tbl_size;
	init_attr.ind_tbl = wqs;

	rwq_ind_tbl->ind_tbl = wqs;
	rwq_ind_tbl->log_ind_tbl_size = init_attr.log_ind_tbl_size;
	rwq_ind_tbl->uobject = uobj;
	uobj->object = rwq_ind_tbl;
	rwq_ind_tbl->device = ib_dev;
	atomic_set(&rwq_ind_tbl->usecnt, 0);

	err = ib_dev->ops.create_rwq_ind_table(rwq_ind_tbl, &init_attr,
					       &attrs->driver_udata);
	if (err)
		goto err_create;

	for (i = 0; i < num_wq_handles; i++)
		rdma_lookup_put_uobject(&wqs[i]->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
	kfree(wqs_handles);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.ind_tbl_handle = uobj->id;
	resp.ind_tbl_num = rwq_ind_tbl->ind_tbl_num;
	resp.response_length = uverbs_response_length(attrs, sizeof(resp));
	return uverbs_response(attrs, &resp, sizeof(resp));

err_create:
	kfree(rwq_ind_tbl);
err_uobj:
	uobj_alloc_abort(uobj, attrs);
put_wqs:
	for (i = 0; i < num_read_wqs; i++) {
		rdma_lookup_put_uobject(&wqs[i]->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);
		atomic_dec(&wqs[i]->usecnt);
	}
err_free:
	kfree(wqs_handles);
	kfree(wqs);
	return err;
}

static int ib_uverbs_ex_destroy_rwq_ind_table(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_destroy_rwq_ind_table cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (cmd.comp_mask)
		return -EOPNOTSUPP;

	return uobj_perform_destroy(UVERBS_OBJECT_RWQ_IND_TBL,
				    cmd.ind_tbl_handle, attrs);
}

static int ib_uverbs_ex_create_flow(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_flow	  cmd;
	struct ib_uverbs_create_flow_resp resp = {};
	struct ib_uobject		  *uobj;
	struct ib_flow			  *flow_id;
	struct ib_uverbs_flow_attr	  *kern_flow_attr;
	struct ib_flow_attr		  *flow_attr;
	struct ib_qp			  *qp;
	struct ib_uflow_resources	  *uflow_res;
	struct ib_uverbs_flow_spec_hdr	  *kern_spec;
	struct uverbs_req_iter iter;
	int err;
	void *ib_spec;
	int i;
	struct ib_device *ib_dev;

	err = uverbs_request_start(attrs, &iter, &cmd, sizeof(cmd));
	if (err)
		return err;

	if (cmd.comp_mask)
		return -EINVAL;

	if (!capable(CAP_NET_RAW))
		return -EPERM;

	if (cmd.flow_attr.flags >= IB_FLOW_ATTR_FLAGS_RESERVED)
		return -EINVAL;

	if ((cmd.flow_attr.flags & IB_FLOW_ATTR_FLAGS_DONT_TRAP) &&
	    ((cmd.flow_attr.type == IB_FLOW_ATTR_ALL_DEFAULT) ||
	     (cmd.flow_attr.type == IB_FLOW_ATTR_MC_DEFAULT)))
		return -EINVAL;

	if (cmd.flow_attr.num_of_specs > IB_FLOW_SPEC_SUPPORT_LAYERS)
		return -EINVAL;

	if (cmd.flow_attr.size >
	    (cmd.flow_attr.num_of_specs * sizeof(struct ib_uverbs_flow_spec)))
		return -EINVAL;

	if (cmd.flow_attr.reserved[0] ||
	    cmd.flow_attr.reserved[1])
		return -EINVAL;

	if (cmd.flow_attr.num_of_specs) {
		kern_flow_attr = kmalloc(sizeof(*kern_flow_attr) + cmd.flow_attr.size,
					 GFP_KERNEL);
		if (!kern_flow_attr)
			return -ENOMEM;

		*kern_flow_attr = cmd.flow_attr;
		err = uverbs_request_next(&iter, &kern_flow_attr->flow_specs,
					  cmd.flow_attr.size);
		if (err)
			goto err_free_attr;
	} else {
		kern_flow_attr = &cmd.flow_attr;
	}

	err = uverbs_request_finish(&iter);
	if (err)
		goto err_free_attr;

	uobj = uobj_alloc(UVERBS_OBJECT_FLOW, attrs, &ib_dev);
	if (IS_ERR(uobj)) {
		err = PTR_ERR(uobj);
		goto err_free_attr;
	}

	if (!rdma_is_port_valid(uobj->context->device, cmd.flow_attr.port)) {
		err = -EINVAL;
		goto err_uobj;
	}

	qp = uobj_get_obj_read(qp, UVERBS_OBJECT_QP, cmd.qp_handle, attrs);
	if (!qp) {
		err = -EINVAL;
		goto err_uobj;
	}

	if (qp->qp_type != IB_QPT_UD && qp->qp_type != IB_QPT_RAW_PACKET) {
		err = -EINVAL;
		goto err_put;
	}

	flow_attr = kzalloc(struct_size(flow_attr, flows,
				cmd.flow_attr.num_of_specs), GFP_KERNEL);
	if (!flow_attr) {
		err = -ENOMEM;
		goto err_put;
	}
	uflow_res = flow_resources_alloc(cmd.flow_attr.num_of_specs);
	if (!uflow_res) {
		err = -ENOMEM;
		goto err_free_flow_attr;
	}

	flow_attr->type = kern_flow_attr->type;
	flow_attr->priority = kern_flow_attr->priority;
	flow_attr->num_of_specs = kern_flow_attr->num_of_specs;
	flow_attr->port = kern_flow_attr->port;
	flow_attr->flags = kern_flow_attr->flags;
	flow_attr->size = sizeof(*flow_attr);

	kern_spec = kern_flow_attr->flow_specs;
	ib_spec = flow_attr + 1;
	for (i = 0; i < flow_attr->num_of_specs &&
			cmd.flow_attr.size >= sizeof(*kern_spec) &&
			cmd.flow_attr.size >= kern_spec->size;
	     i++) {
		err = kern_spec_to_ib_spec(
				attrs, (struct ib_uverbs_flow_spec *)kern_spec,
				ib_spec, uflow_res);
		if (err)
			goto err_free;

		flow_attr->size +=
			((union ib_flow_spec *) ib_spec)->size;
		cmd.flow_attr.size -= kern_spec->size;
		kern_spec = ((void *)kern_spec) + kern_spec->size;
		ib_spec += ((union ib_flow_spec *) ib_spec)->size;
	}
	if (cmd.flow_attr.size || (i != flow_attr->num_of_specs)) {
		pr_warn("create flow failed, flow %d: %u bytes left from uverb cmd\n",
			i, cmd.flow_attr.size);
		err = -EINVAL;
		goto err_free;
	}

	flow_id = qp->device->ops.create_flow(qp, flow_attr,
					      &attrs->driver_udata);

	if (IS_ERR(flow_id)) {
		err = PTR_ERR(flow_id);
		goto err_free;
	}

	ib_set_flow(uobj, flow_id, qp, qp->device, uflow_res);

	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	kfree(flow_attr);

	if (cmd.flow_attr.num_of_specs)
		kfree(kern_flow_attr);
	uobj_finalize_uobj_create(uobj, attrs);

	resp.flow_handle = uobj->id;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_free:
	ib_uverbs_flow_resources_free(uflow_res);
err_free_flow_attr:
	kfree(flow_attr);
err_put:
	rdma_lookup_put_uobject(&qp->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
err_uobj:
	uobj_alloc_abort(uobj, attrs);
err_free_attr:
	if (cmd.flow_attr.num_of_specs)
		kfree(kern_flow_attr);
	return err;
}

static int ib_uverbs_ex_destroy_flow(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_destroy_flow	cmd;
	int				ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (cmd.comp_mask)
		return -EINVAL;

	return uobj_perform_destroy(UVERBS_OBJECT_FLOW, cmd.flow_handle, attrs);
}

static int __uverbs_create_xsrq(struct uverbs_attr_bundle *attrs,
				struct ib_uverbs_create_xsrq *cmd,
				struct ib_udata *udata)
{
	struct ib_uverbs_create_srq_resp resp = {};
	struct ib_usrq_object           *obj;
	struct ib_pd                    *pd;
	struct ib_srq                   *srq;
	struct ib_srq_init_attr          attr;
	int ret;
	struct ib_uobject *xrcd_uobj;
	struct ib_device *ib_dev;

	obj = (struct ib_usrq_object *)uobj_alloc(UVERBS_OBJECT_SRQ, attrs,
						  &ib_dev);
	if (IS_ERR(obj))
		return PTR_ERR(obj);

	if (cmd->srq_type == IB_SRQT_TM)
		attr.ext.tag_matching.max_num_tags = cmd->max_num_tags;

	if (cmd->srq_type == IB_SRQT_XRC) {
		xrcd_uobj = uobj_get_read(UVERBS_OBJECT_XRCD, cmd->xrcd_handle,
					  attrs);
		if (IS_ERR(xrcd_uobj)) {
			ret = -EINVAL;
			goto err;
		}

		attr.ext.xrc.xrcd = (struct ib_xrcd *)xrcd_uobj->object;
		if (!attr.ext.xrc.xrcd) {
			ret = -EINVAL;
			goto err_put_xrcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		obj->uxrcd = container_of(xrcd_uobj, struct ib_uxrcd_object, uobject);
		atomic_inc(&obj->uxrcd->refcnt);
	}

<<<<<<< HEAD
	attr.event_handler  = ib_uverbs_srq_event_handler;
	attr.srq_context    = file;
=======
	if (ib_srq_has_cq(cmd->srq_type)) {
		attr.ext.cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ,
						cmd->cq_handle, attrs);
		if (!attr.ext.cq) {
			ret = -EINVAL;
			goto err_put_xrcd;
		}
	}

	pd = uobj_get_obj_read(pd, UVERBS_OBJECT_PD, cmd->pd_handle, attrs);
	if (!pd) {
		ret = -EINVAL;
		goto err_put_cq;
	}

	attr.event_handler  = ib_uverbs_srq_event_handler;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	attr.srq_type       = cmd->srq_type;
	attr.attr.max_wr    = cmd->max_wr;
	attr.attr.max_sge   = cmd->max_sge;
	attr.attr.srq_limit = cmd->srq_limit;

<<<<<<< HEAD
	obj->uevent.events_reported = 0;
	INIT_LIST_HEAD(&obj->uevent.event_list);

	srq = pd->device->create_srq(pd, &attr, udata);
	if (IS_ERR(srq)) {
		ret = PTR_ERR(srq);
		goto err_put;
	}

	srq->device        = pd->device;
	srq->pd            = pd;
	srq->srq_type	   = cmd->srq_type;
	srq->uobject       = &obj->uevent.uobject;
	srq->event_handler = attr.event_handler;
	srq->srq_context   = attr.srq_context;

	if (cmd->srq_type == IB_SRQT_XRC) {
		srq->ext.xrc.cq   = attr.ext.xrc.cq;
		srq->ext.xrc.xrcd = attr.ext.xrc.xrcd;
		atomic_inc(&attr.ext.xrc.cq->usecnt);
		atomic_inc(&attr.ext.xrc.xrcd->usecnt);
	}

	atomic_inc(&pd->usecnt);
	atomic_set(&srq->usecnt, 0);

	obj->uevent.uobject.object = srq;
	ret = idr_add_uobj(&ib_uverbs_srq_idr, &obj->uevent.uobject);
	if (ret)
		goto err_destroy;

	memset(&resp, 0, sizeof resp);
	resp.srq_handle = obj->uevent.uobject.id;
	resp.max_wr     = attr.attr.max_wr;
	resp.max_sge    = attr.attr.max_sge;
	if (cmd->srq_type == IB_SRQT_XRC)
		resp.srqn = srq->ext.xrc.srq_num;

	if (copy_to_user((void __user *) (unsigned long) cmd->response,
			 &resp, sizeof resp)) {
		ret = -EFAULT;
		goto err_copy;
	}

	if (cmd->srq_type == IB_SRQT_XRC) {
		put_uobj_read(xrcd_uobj);
		put_cq_read(attr.ext.xrc.cq);
	}
	put_pd_read(pd);

	mutex_lock(&file->mutex);
	list_add_tail(&obj->uevent.uobject.list, &file->ucontext->srq_list);
	mutex_unlock(&file->mutex);

	obj->uevent.uobject.live = 1;

	up_write(&obj->uevent.uobject.mutex);

	return 0;

err_copy:
	idr_remove_uobj(&ib_uverbs_srq_idr, &obj->uevent.uobject);

err_destroy:
	ib_destroy_srq(srq);

err_put:
	if (cmd->srq_type == IB_SRQT_XRC) {
		atomic_dec(&obj->uxrcd->refcnt);
		put_uobj_read(xrcd_uobj);
	}

err_put_cq:
	if (cmd->srq_type == IB_SRQT_XRC)
		put_cq_read(attr.ext.xrc.cq);

err_put_pd:
	put_pd_read(pd);

err:
	put_uobj_write(&obj->uevent.uobject);
	return ret;
}

ssize_t ib_uverbs_create_srq(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len,
			     int out_len)
{
	struct ib_uverbs_create_srq      cmd;
	struct ib_uverbs_create_xsrq     xcmd;
	struct ib_uverbs_create_srq_resp resp;
	struct ib_udata                  udata;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

=======
	INIT_LIST_HEAD(&obj->uevent.event_list);
	obj->uevent.uobject.user_handle = cmd->user_handle;

	srq = ib_create_srq_user(pd, &attr, obj, udata);
	if (IS_ERR(srq)) {
		ret = PTR_ERR(srq);
		goto err_put_pd;
	}

	obj->uevent.uobject.object = srq;
	obj->uevent.uobject.user_handle = cmd->user_handle;
	obj->uevent.event_file = READ_ONCE(attrs->ufile->default_async_file);
	if (obj->uevent.event_file)
		uverbs_uobject_get(&obj->uevent.event_file->uobj);

	if (cmd->srq_type == IB_SRQT_XRC)
		resp.srqn = srq->ext.xrc.srq_num;

	if (cmd->srq_type == IB_SRQT_XRC)
		uobj_put_read(xrcd_uobj);

	if (ib_srq_has_cq(cmd->srq_type))
		rdma_lookup_put_uobject(&attr.ext.cq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);

	uobj_put_obj_read(pd);
	uobj_finalize_uobj_create(&obj->uevent.uobject, attrs);

	resp.srq_handle = obj->uevent.uobject.id;
	resp.max_wr = attr.attr.max_wr;
	resp.max_sge = attr.attr.max_sge;
	return uverbs_response(attrs, &resp, sizeof(resp));

err_put_pd:
	uobj_put_obj_read(pd);
err_put_cq:
	if (ib_srq_has_cq(cmd->srq_type))
		rdma_lookup_put_uobject(&attr.ext.cq->uobject->uevent.uobject,
					UVERBS_LOOKUP_READ);

err_put_xrcd:
	if (cmd->srq_type == IB_SRQT_XRC) {
		atomic_dec(&obj->uxrcd->refcnt);
		uobj_put_read(xrcd_uobj);
	}

err:
	uobj_alloc_abort(&obj->uevent.uobject, attrs);
	return ret;
}

static int ib_uverbs_create_srq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_srq      cmd;
	struct ib_uverbs_create_xsrq     xcmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	memset(&xcmd, 0, sizeof(xcmd));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xcmd.response	 = cmd.response;
	xcmd.user_handle = cmd.user_handle;
	xcmd.srq_type	 = IB_SRQT_BASIC;
	xcmd.pd_handle	 = cmd.pd_handle;
	xcmd.max_wr	 = cmd.max_wr;
	xcmd.max_sge	 = cmd.max_sge;
	xcmd.srq_limit	 = cmd.srq_limit;

<<<<<<< HEAD
	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	ret = __uverbs_create_xsrq(file, &xcmd, &udata);
	if (ret)
		return ret;

	return in_len;
}

ssize_t ib_uverbs_create_xsrq(struct ib_uverbs_file *file,
			      const char __user *buf, int in_len, int out_len)
{
	struct ib_uverbs_create_xsrq     cmd;
	struct ib_uverbs_create_srq_resp resp;
	struct ib_udata                  udata;
	int ret;

	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd,
		   (unsigned long) cmd.response + sizeof resp,
		   in_len - sizeof cmd, out_len - sizeof resp);

	ret = __uverbs_create_xsrq(file, &cmd, &udata);
	if (ret)
		return ret;

	return in_len;
}

ssize_t ib_uverbs_modify_srq(struct ib_uverbs_file *file,
			     const char __user *buf, int in_len,
			     int out_len)
{
	struct ib_uverbs_modify_srq cmd;
	struct ib_udata             udata;
=======
	return __uverbs_create_xsrq(attrs, &xcmd, &attrs->driver_udata);
}

static int ib_uverbs_create_xsrq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_create_xsrq     cmd;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	return __uverbs_create_xsrq(attrs, &cmd, &attrs->driver_udata);
}

static int ib_uverbs_modify_srq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_modify_srq cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ib_srq              *srq;
	struct ib_srq_attr          attr;
	int                         ret;

<<<<<<< HEAD
	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	INIT_UDATA(&udata, buf + sizeof cmd, NULL, in_len - sizeof cmd,
		   out_len);

	srq = idr_read_srq(cmd.srq_handle, file->ucontext);
=======
	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	srq = uobj_get_obj_read(srq, UVERBS_OBJECT_SRQ, cmd.srq_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!srq)
		return -EINVAL;

	attr.max_wr    = cmd.max_wr;
	attr.srq_limit = cmd.srq_limit;

<<<<<<< HEAD
	ret = srq->device->modify_srq(srq, &attr, cmd.attr_mask, &udata);

	put_srq_read(srq);

	return ret ? ret : in_len;
}

ssize_t ib_uverbs_query_srq(struct ib_uverbs_file *file,
			    const char __user *buf,
			    int in_len, int out_len)
=======
	ret = srq->device->ops.modify_srq(srq, &attr, cmd.attr_mask,
					  &attrs->driver_udata);

	rdma_lookup_put_uobject(&srq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);

	return ret;
}

static int ib_uverbs_query_srq(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_query_srq      cmd;
	struct ib_uverbs_query_srq_resp resp;
	struct ib_srq_attr              attr;
	struct ib_srq                   *srq;
	int                             ret;

<<<<<<< HEAD
	if (out_len < sizeof resp)
		return -ENOSPC;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	srq = idr_read_srq(cmd.srq_handle, file->ucontext);
=======
	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	srq = uobj_get_obj_read(srq, UVERBS_OBJECT_SRQ, cmd.srq_handle, attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!srq)
		return -EINVAL;

	ret = ib_query_srq(srq, &attr);

<<<<<<< HEAD
	put_srq_read(srq);
=======
	rdma_lookup_put_uobject(&srq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret)
		return ret;

	memset(&resp, 0, sizeof resp);

	resp.max_wr    = attr.max_wr;
	resp.max_sge   = attr.max_sge;
	resp.srq_limit = attr.srq_limit;

<<<<<<< HEAD
	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		return -EFAULT;

	return in_len;
}

ssize_t ib_uverbs_destroy_srq(struct ib_uverbs_file *file,
			      const char __user *buf, int in_len,
			      int out_len)
=======
	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_destroy_srq(struct uverbs_attr_bundle *attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ib_uverbs_destroy_srq      cmd;
	struct ib_uverbs_destroy_srq_resp resp;
	struct ib_uobject		 *uobj;
<<<<<<< HEAD
	struct ib_srq               	 *srq;
	struct ib_uevent_object        	 *obj;
	int                         	  ret = -EINVAL;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	uobj = idr_write_uobj(&ib_uverbs_srq_idr, cmd.srq_handle, file->ucontext);
	if (!uobj)
		return -EINVAL;
	srq = uobj->object;
	obj = container_of(uobj, struct ib_uevent_object, uobject);

	ret = ib_destroy_srq(srq);
	if (!ret)
		uobj->live = 0;

	put_uobj_write(uobj);

	if (ret)
		return ret;

	idr_remove_uobj(&ib_uverbs_srq_idr, uobj);

	mutex_lock(&file->mutex);
	list_del(&uobj->list);
	mutex_unlock(&file->mutex);

	ib_uverbs_release_uevent(file, obj);

	memset(&resp, 0, sizeof resp);
	resp.events_reported = obj->events_reported;

	put_uobj(uobj);

	if (copy_to_user((void __user *) (unsigned long) cmd.response,
			 &resp, sizeof resp))
		ret = -EFAULT;

	return ret ? ret : in_len;
}
=======
	struct ib_uevent_object        	 *obj;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	uobj = uobj_get_destroy(UVERBS_OBJECT_SRQ, cmd.srq_handle, attrs);
	if (IS_ERR(uobj))
		return PTR_ERR(uobj);

	obj = container_of(uobj, struct ib_uevent_object, uobject);
	memset(&resp, 0, sizeof(resp));
	resp.events_reported = obj->events_reported;

	uobj_put_destroy(uobj);

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_ex_query_device(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_query_device_resp resp = {};
	struct ib_uverbs_ex_query_device  cmd;
	struct ib_device_attr attr = {0};
	struct ib_ucontext *ucontext;
	struct ib_device *ib_dev;
	int err;

	ucontext = ib_uverbs_get_ucontext(attrs);
	if (IS_ERR(ucontext))
		return PTR_ERR(ucontext);
	ib_dev = ucontext->device;

	err = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (err)
		return err;

	if (cmd.comp_mask)
		return -EINVAL;

	if (cmd.reserved)
		return -EINVAL;

	err = ib_dev->ops.query_device(ib_dev, &attr, &attrs->driver_udata);
	if (err)
		return err;

	copy_query_dev_fields(ucontext, &resp.base, &attr);

	resp.odp_caps.general_caps = attr.odp_caps.general_caps;
	resp.odp_caps.per_transport_caps.rc_odp_caps =
		attr.odp_caps.per_transport_caps.rc_odp_caps;
	resp.odp_caps.per_transport_caps.uc_odp_caps =
		attr.odp_caps.per_transport_caps.uc_odp_caps;
	resp.odp_caps.per_transport_caps.ud_odp_caps =
		attr.odp_caps.per_transport_caps.ud_odp_caps;
	resp.xrc_odp_caps = attr.odp_caps.per_transport_caps.xrc_odp_caps;

	resp.timestamp_mask = attr.timestamp_mask;
	resp.hca_core_clock = attr.hca_core_clock;
	resp.device_cap_flags_ex = attr.device_cap_flags;
	resp.rss_caps.supported_qpts = attr.rss_caps.supported_qpts;
	resp.rss_caps.max_rwq_indirection_tables =
		attr.rss_caps.max_rwq_indirection_tables;
	resp.rss_caps.max_rwq_indirection_table_size =
		attr.rss_caps.max_rwq_indirection_table_size;
	resp.max_wq_type_rq = attr.max_wq_type_rq;
	resp.raw_packet_caps = attr.raw_packet_caps;
	resp.tm_caps.max_rndv_hdr_size	= attr.tm_caps.max_rndv_hdr_size;
	resp.tm_caps.max_num_tags	= attr.tm_caps.max_num_tags;
	resp.tm_caps.max_ops		= attr.tm_caps.max_ops;
	resp.tm_caps.max_sge		= attr.tm_caps.max_sge;
	resp.tm_caps.flags		= attr.tm_caps.flags;
	resp.cq_moderation_caps.max_cq_moderation_count  =
		attr.cq_caps.max_cq_moderation_count;
	resp.cq_moderation_caps.max_cq_moderation_period =
		attr.cq_caps.max_cq_moderation_period;
	resp.max_dm_size = attr.max_dm_size;
	resp.response_length = uverbs_response_length(attrs, sizeof(resp));

	return uverbs_response(attrs, &resp, sizeof(resp));
}

static int ib_uverbs_ex_modify_cq(struct uverbs_attr_bundle *attrs)
{
	struct ib_uverbs_ex_modify_cq cmd;
	struct ib_cq *cq;
	int ret;

	ret = uverbs_request(attrs, &cmd, sizeof(cmd));
	if (ret)
		return ret;

	if (!cmd.attr_mask || cmd.reserved)
		return -EINVAL;

	if (cmd.attr_mask > IB_CQ_MODERATE)
		return -EOPNOTSUPP;

	cq = uobj_get_obj_read(cq, UVERBS_OBJECT_CQ, cmd.cq_handle, attrs);
	if (!cq)
		return -EINVAL;

	ret = rdma_set_cq_moderation(cq, cmd.attr.cq_count, cmd.attr.cq_period);

	rdma_lookup_put_uobject(&cq->uobject->uevent.uobject,
				UVERBS_LOOKUP_READ);
	return ret;
}

/*
 * Describe the input structs for write(). Some write methods have an input
 * only struct, most have an input and output. If the struct has an output then
 * the 'response' u64 must be the first field in the request structure.
 *
 * If udata is present then both the request and response structs have a
 * trailing driver_data flex array. In this case the size of the base struct
 * cannot be changed.
 */
#define UAPI_DEF_WRITE_IO(req, resp)                                           \
	.write.has_resp = 1 +                                                  \
			  BUILD_BUG_ON_ZERO(offsetof(req, response) != 0) +    \
			  BUILD_BUG_ON_ZERO(sizeof_field(req, response) !=    \
					    sizeof(u64)),                      \
	.write.req_size = sizeof(req), .write.resp_size = sizeof(resp)

#define UAPI_DEF_WRITE_I(req) .write.req_size = sizeof(req)

#define UAPI_DEF_WRITE_UDATA_IO(req, resp)                                     \
	UAPI_DEF_WRITE_IO(req, resp),                                          \
		.write.has_udata =                                             \
			1 +                                                    \
			BUILD_BUG_ON_ZERO(offsetof(req, driver_data) !=        \
					  sizeof(req)) +                       \
			BUILD_BUG_ON_ZERO(offsetof(resp, driver_data) !=       \
					  sizeof(resp))

#define UAPI_DEF_WRITE_UDATA_I(req)                                            \
	UAPI_DEF_WRITE_I(req),                                                 \
		.write.has_udata =                                             \
			1 + BUILD_BUG_ON_ZERO(offsetof(req, driver_data) !=    \
					      sizeof(req))

/*
 * The _EX versions are for use with WRITE_EX and allow the last struct member
 * to be specified. Buffers that do not include that member will be rejected.
 */
#define UAPI_DEF_WRITE_IO_EX(req, req_last_member, resp, resp_last_member)     \
	.write.has_resp = 1,                                                   \
	.write.req_size = offsetofend(req, req_last_member),                   \
	.write.resp_size = offsetofend(resp, resp_last_member)

#define UAPI_DEF_WRITE_I_EX(req, req_last_member)                              \
	.write.req_size = offsetofend(req, req_last_member)

const struct uapi_definition uverbs_def_write_intf[] = {
	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_AH,
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_CREATE_AH,
				     ib_uverbs_create_ah,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_create_ah,
					     struct ib_uverbs_create_ah_resp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DESTROY_AH,
			ib_uverbs_destroy_ah,
			UAPI_DEF_WRITE_I(struct ib_uverbs_destroy_ah)),
		UAPI_DEF_OBJ_NEEDS_FN(create_user_ah),
		UAPI_DEF_OBJ_NEEDS_FN(destroy_ah)),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_COMP_CHANNEL,
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL,
			ib_uverbs_create_comp_channel,
			UAPI_DEF_WRITE_IO(
				struct ib_uverbs_create_comp_channel,
				struct ib_uverbs_create_comp_channel_resp))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_CQ,
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_CREATE_CQ,
				     ib_uverbs_create_cq,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_create_cq,
					     struct ib_uverbs_create_cq_resp),
				     UAPI_DEF_METHOD_NEEDS_FN(create_cq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DESTROY_CQ,
			ib_uverbs_destroy_cq,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_destroy_cq,
					  struct ib_uverbs_destroy_cq_resp),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_cq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_POLL_CQ,
			ib_uverbs_poll_cq,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_poll_cq,
					  struct ib_uverbs_poll_cq_resp),
			UAPI_DEF_METHOD_NEEDS_FN(poll_cq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_REQ_NOTIFY_CQ,
			ib_uverbs_req_notify_cq,
			UAPI_DEF_WRITE_I(struct ib_uverbs_req_notify_cq),
			UAPI_DEF_METHOD_NEEDS_FN(req_notify_cq)),
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_RESIZE_CQ,
				     ib_uverbs_resize_cq,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_resize_cq,
					     struct ib_uverbs_resize_cq_resp),
				     UAPI_DEF_METHOD_NEEDS_FN(resize_cq)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_CREATE_CQ,
			ib_uverbs_ex_create_cq,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_ex_create_cq,
					     reserved,
					     struct ib_uverbs_ex_create_cq_resp,
					     response_length),
			UAPI_DEF_METHOD_NEEDS_FN(create_cq)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_MODIFY_CQ,
			ib_uverbs_ex_modify_cq,
			UAPI_DEF_WRITE_I(struct ib_uverbs_ex_modify_cq),
			UAPI_DEF_METHOD_NEEDS_FN(modify_cq))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_DEVICE,
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_GET_CONTEXT,
				     ib_uverbs_get_context,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_get_context,
					     struct ib_uverbs_get_context_resp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_QUERY_DEVICE,
			ib_uverbs_query_device,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_query_device,
					  struct ib_uverbs_query_device_resp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_QUERY_PORT,
			ib_uverbs_query_port,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_query_port,
					  struct ib_uverbs_query_port_resp),
			UAPI_DEF_METHOD_NEEDS_FN(query_port)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_QUERY_DEVICE,
			ib_uverbs_ex_query_device,
			UAPI_DEF_WRITE_IO_EX(
				struct ib_uverbs_ex_query_device,
				reserved,
				struct ib_uverbs_ex_query_device_resp,
				response_length),
			UAPI_DEF_METHOD_NEEDS_FN(query_device)),
		UAPI_DEF_OBJ_NEEDS_FN(alloc_ucontext),
		UAPI_DEF_OBJ_NEEDS_FN(dealloc_ucontext)),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_FLOW,
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_CREATE_FLOW,
			ib_uverbs_ex_create_flow,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_create_flow,
					     flow_attr,
					     struct ib_uverbs_create_flow_resp,
					     flow_handle),
			UAPI_DEF_METHOD_NEEDS_FN(create_flow)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_DESTROY_FLOW,
			ib_uverbs_ex_destroy_flow,
			UAPI_DEF_WRITE_I(struct ib_uverbs_destroy_flow),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_flow))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_MR,
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_DEREG_MR,
				     ib_uverbs_dereg_mr,
				     UAPI_DEF_WRITE_I(struct ib_uverbs_dereg_mr),
				     UAPI_DEF_METHOD_NEEDS_FN(dereg_mr)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_REG_MR,
			ib_uverbs_reg_mr,
			UAPI_DEF_WRITE_UDATA_IO(struct ib_uverbs_reg_mr,
						struct ib_uverbs_reg_mr_resp),
			UAPI_DEF_METHOD_NEEDS_FN(reg_user_mr)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_REREG_MR,
			ib_uverbs_rereg_mr,
			UAPI_DEF_WRITE_UDATA_IO(struct ib_uverbs_rereg_mr,
						struct ib_uverbs_rereg_mr_resp),
			UAPI_DEF_METHOD_NEEDS_FN(rereg_user_mr))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_MW,
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_ALLOC_MW,
			ib_uverbs_alloc_mw,
			UAPI_DEF_WRITE_UDATA_IO(struct ib_uverbs_alloc_mw,
						struct ib_uverbs_alloc_mw_resp),
			UAPI_DEF_METHOD_NEEDS_FN(alloc_mw)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DEALLOC_MW,
			ib_uverbs_dealloc_mw,
			UAPI_DEF_WRITE_I(struct ib_uverbs_dealloc_mw),
			UAPI_DEF_METHOD_NEEDS_FN(dealloc_mw))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_PD,
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_ALLOC_PD,
			ib_uverbs_alloc_pd,
			UAPI_DEF_WRITE_UDATA_IO(struct ib_uverbs_alloc_pd,
						struct ib_uverbs_alloc_pd_resp),
			UAPI_DEF_METHOD_NEEDS_FN(alloc_pd)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DEALLOC_PD,
			ib_uverbs_dealloc_pd,
			UAPI_DEF_WRITE_I(struct ib_uverbs_dealloc_pd),
			UAPI_DEF_METHOD_NEEDS_FN(dealloc_pd))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_QP,
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_ATTACH_MCAST,
			ib_uverbs_attach_mcast,
			UAPI_DEF_WRITE_I(struct ib_uverbs_attach_mcast),
			UAPI_DEF_METHOD_NEEDS_FN(attach_mcast),
			UAPI_DEF_METHOD_NEEDS_FN(detach_mcast)),
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_CREATE_QP,
				     ib_uverbs_create_qp,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_create_qp,
					     struct ib_uverbs_create_qp_resp),
				     UAPI_DEF_METHOD_NEEDS_FN(create_qp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DESTROY_QP,
			ib_uverbs_destroy_qp,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_destroy_qp,
					  struct ib_uverbs_destroy_qp_resp),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_qp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DETACH_MCAST,
			ib_uverbs_detach_mcast,
			UAPI_DEF_WRITE_I(struct ib_uverbs_detach_mcast),
			UAPI_DEF_METHOD_NEEDS_FN(detach_mcast)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_MODIFY_QP,
			ib_uverbs_modify_qp,
			UAPI_DEF_WRITE_I(struct ib_uverbs_modify_qp),
			UAPI_DEF_METHOD_NEEDS_FN(modify_qp)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_POST_RECV,
			ib_uverbs_post_recv,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_post_recv,
					  struct ib_uverbs_post_recv_resp),
			UAPI_DEF_METHOD_NEEDS_FN(post_recv)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_POST_SEND,
			ib_uverbs_post_send,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_post_send,
					  struct ib_uverbs_post_send_resp),
			UAPI_DEF_METHOD_NEEDS_FN(post_send)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_QUERY_QP,
			ib_uverbs_query_qp,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_query_qp,
					  struct ib_uverbs_query_qp_resp),
			UAPI_DEF_METHOD_NEEDS_FN(query_qp)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_CREATE_QP,
			ib_uverbs_ex_create_qp,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_ex_create_qp,
					     comp_mask,
					     struct ib_uverbs_ex_create_qp_resp,
					     response_length),
			UAPI_DEF_METHOD_NEEDS_FN(create_qp)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_MODIFY_QP,
			ib_uverbs_ex_modify_qp,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_ex_modify_qp,
					     base,
					     struct ib_uverbs_ex_modify_qp_resp,
					     response_length),
			UAPI_DEF_METHOD_NEEDS_FN(modify_qp))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_RWQ_IND_TBL,
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_CREATE_RWQ_IND_TBL,
			ib_uverbs_ex_create_rwq_ind_table,
			UAPI_DEF_WRITE_IO_EX(
				struct ib_uverbs_ex_create_rwq_ind_table,
				log_ind_tbl_size,
				struct ib_uverbs_ex_create_rwq_ind_table_resp,
				ind_tbl_num),
			UAPI_DEF_METHOD_NEEDS_FN(create_rwq_ind_table)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_DESTROY_RWQ_IND_TBL,
			ib_uverbs_ex_destroy_rwq_ind_table,
			UAPI_DEF_WRITE_I(
				struct ib_uverbs_ex_destroy_rwq_ind_table),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_rwq_ind_table))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_WQ,
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_CREATE_WQ,
			ib_uverbs_ex_create_wq,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_ex_create_wq,
					     max_sge,
					     struct ib_uverbs_ex_create_wq_resp,
					     wqn),
			UAPI_DEF_METHOD_NEEDS_FN(create_wq)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_DESTROY_WQ,
			ib_uverbs_ex_destroy_wq,
			UAPI_DEF_WRITE_IO_EX(struct ib_uverbs_ex_destroy_wq,
					     wq_handle,
					     struct ib_uverbs_ex_destroy_wq_resp,
					     reserved),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_wq)),
		DECLARE_UVERBS_WRITE_EX(
			IB_USER_VERBS_EX_CMD_MODIFY_WQ,
			ib_uverbs_ex_modify_wq,
			UAPI_DEF_WRITE_I_EX(struct ib_uverbs_ex_modify_wq,
					    curr_wq_state),
			UAPI_DEF_METHOD_NEEDS_FN(modify_wq))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_SRQ,
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_CREATE_SRQ,
				     ib_uverbs_create_srq,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_create_srq,
					     struct ib_uverbs_create_srq_resp),
				     UAPI_DEF_METHOD_NEEDS_FN(create_srq)),
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_CREATE_XSRQ,
				     ib_uverbs_create_xsrq,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_create_xsrq,
					     struct ib_uverbs_create_srq_resp),
				     UAPI_DEF_METHOD_NEEDS_FN(create_srq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_DESTROY_SRQ,
			ib_uverbs_destroy_srq,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_destroy_srq,
					  struct ib_uverbs_destroy_srq_resp),
			UAPI_DEF_METHOD_NEEDS_FN(destroy_srq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_MODIFY_SRQ,
			ib_uverbs_modify_srq,
			UAPI_DEF_WRITE_UDATA_I(struct ib_uverbs_modify_srq),
			UAPI_DEF_METHOD_NEEDS_FN(modify_srq)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_POST_SRQ_RECV,
			ib_uverbs_post_srq_recv,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_post_srq_recv,
					  struct ib_uverbs_post_srq_recv_resp),
			UAPI_DEF_METHOD_NEEDS_FN(post_srq_recv)),
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_QUERY_SRQ,
			ib_uverbs_query_srq,
			UAPI_DEF_WRITE_IO(struct ib_uverbs_query_srq,
					  struct ib_uverbs_query_srq_resp),
			UAPI_DEF_METHOD_NEEDS_FN(query_srq))),

	DECLARE_UVERBS_OBJECT(
		UVERBS_OBJECT_XRCD,
		DECLARE_UVERBS_WRITE(
			IB_USER_VERBS_CMD_CLOSE_XRCD,
			ib_uverbs_close_xrcd,
			UAPI_DEF_WRITE_I(struct ib_uverbs_close_xrcd)),
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_OPEN_QP,
				     ib_uverbs_open_qp,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_open_qp,
					     struct ib_uverbs_create_qp_resp)),
		DECLARE_UVERBS_WRITE(IB_USER_VERBS_CMD_OPEN_XRCD,
				     ib_uverbs_open_xrcd,
				     UAPI_DEF_WRITE_UDATA_IO(
					     struct ib_uverbs_open_xrcd,
					     struct ib_uverbs_open_xrcd_resp)),
		UAPI_DEF_OBJ_NEEDS_FN(alloc_xrcd),
		UAPI_DEF_OBJ_NEEDS_FN(dealloc_xrcd)),

	{},
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
