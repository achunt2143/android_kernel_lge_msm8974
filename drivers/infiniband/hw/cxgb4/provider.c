/*
 * Copyright (c) 2009-2010 Chelsio, Inc. All rights reserved.
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
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/inetdevice.h>
<<<<<<< HEAD
=======
#include <net/addrconf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/io.h>

#include <asm/irq.h>
#include <asm/byteorder.h>

#include <rdma/iw_cm.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_smi.h>
#include <rdma/ib_umem.h>
#include <rdma/ib_user_verbs.h>

#include "iw_cxgb4.h"

static int fastreg_support = 1;
module_param(fastreg_support, int, 0644);
MODULE_PARM_DESC(fastreg_support, "Advertise fastreg support (default=1)");

<<<<<<< HEAD
static struct ib_ah *c4iw_ah_create(struct ib_pd *pd,
				    struct ib_ah_attr *ah_attr)
{
	return ERR_PTR(-ENOSYS);
}

static int c4iw_ah_destroy(struct ib_ah *ah)
{
	return -ENOSYS;
}

static int c4iw_multicast_attach(struct ib_qp *ibqp, union ib_gid *gid, u16 lid)
{
	return -ENOSYS;
}

static int c4iw_multicast_detach(struct ib_qp *ibqp, union ib_gid *gid, u16 lid)
{
	return -ENOSYS;
}

static int c4iw_process_mad(struct ib_device *ibdev, int mad_flags,
			    u8 port_num, struct ib_wc *in_wc,
			    struct ib_grh *in_grh, struct ib_mad *in_mad,
			    struct ib_mad *out_mad)
{
	return -ENOSYS;
}

static int c4iw_dealloc_ucontext(struct ib_ucontext *context)
{
	struct c4iw_dev *rhp = to_c4iw_dev(context->device);
	struct c4iw_ucontext *ucontext = to_c4iw_ucontext(context);
	struct c4iw_mm_entry *mm, *tmp;

	PDBG("%s context %p\n", __func__, context);
	list_for_each_entry_safe(mm, tmp, &ucontext->mmaps, entry)
		kfree(mm);
	c4iw_release_dev_ucontext(&rhp->rdev, &ucontext->uctx);
	kfree(ucontext);
	return 0;
}

static struct ib_ucontext *c4iw_alloc_ucontext(struct ib_device *ibdev,
					       struct ib_udata *udata)
{
	struct c4iw_ucontext *context;
	struct c4iw_dev *rhp = to_c4iw_dev(ibdev);

	PDBG("%s ibdev %p\n", __func__, ibdev);
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return ERR_PTR(-ENOMEM);
	c4iw_init_dev_ucontext(&rhp->rdev, &context->uctx);
	INIT_LIST_HEAD(&context->mmaps);
	spin_lock_init(&context->mmap_lock);
	return &context->ibucontext;
=======
static void c4iw_dealloc_ucontext(struct ib_ucontext *context)
{
	struct c4iw_ucontext *ucontext = to_c4iw_ucontext(context);
	struct c4iw_dev *rhp;
	struct c4iw_mm_entry *mm, *tmp;

	pr_debug("context %p\n", context);
	rhp = to_c4iw_dev(ucontext->ibucontext.device);

	list_for_each_entry_safe(mm, tmp, &ucontext->mmaps, entry)
		kfree(mm);
	c4iw_release_dev_ucontext(&rhp->rdev, &ucontext->uctx);
}

static int c4iw_alloc_ucontext(struct ib_ucontext *ucontext,
			       struct ib_udata *udata)
{
	struct ib_device *ibdev = ucontext->device;
	struct c4iw_ucontext *context = to_c4iw_ucontext(ucontext);
	struct c4iw_dev *rhp = to_c4iw_dev(ibdev);
	struct c4iw_alloc_ucontext_resp uresp;
	int ret = 0;
	struct c4iw_mm_entry *mm = NULL;

	pr_debug("ibdev %p\n", ibdev);
	c4iw_init_dev_ucontext(&rhp->rdev, &context->uctx);
	INIT_LIST_HEAD(&context->mmaps);
	spin_lock_init(&context->mmap_lock);

	if (udata->outlen < sizeof(uresp) - sizeof(uresp.reserved)) {
		pr_err_once("Warning - downlevel libcxgb4 (non-fatal), device status page disabled\n");
		rhp->rdev.flags |= T4_STATUS_PAGE_DISABLED;
	} else {
		mm = kmalloc(sizeof(*mm), GFP_KERNEL);
		if (!mm) {
			ret = -ENOMEM;
			goto err;
		}

		uresp.status_page_size = PAGE_SIZE;

		spin_lock(&context->mmap_lock);
		uresp.status_page_key = context->key;
		context->key += PAGE_SIZE;
		spin_unlock(&context->mmap_lock);

		ret = ib_copy_to_udata(udata, &uresp,
				       sizeof(uresp) - sizeof(uresp.reserved));
		if (ret)
			goto err_mm;

		mm->key = uresp.status_page_key;
		mm->addr = virt_to_phys(rhp->rdev.status_page);
		mm->len = PAGE_SIZE;
		insert_mmap(context, mm);
	}
	return 0;
err_mm:
	kfree(mm);
err:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int c4iw_mmap(struct ib_ucontext *context, struct vm_area_struct *vma)
{
	int len = vma->vm_end - vma->vm_start;
	u32 key = vma->vm_pgoff << PAGE_SHIFT;
	struct c4iw_rdev *rdev;
	int ret = 0;
	struct c4iw_mm_entry *mm;
	struct c4iw_ucontext *ucontext;
	u64 addr;

<<<<<<< HEAD
	PDBG("%s pgoff 0x%lx key 0x%x len %d\n", __func__, vma->vm_pgoff,
	     key, len);
=======
	pr_debug("pgoff 0x%lx key 0x%x len %d\n", vma->vm_pgoff,
		 key, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (vma->vm_start & (PAGE_SIZE-1))
		return -EINVAL;

	rdev = &(to_c4iw_dev(context->device)->rdev);
	ucontext = to_c4iw_ucontext(context);

	mm = remove_mmap(ucontext, key, len);
	if (!mm)
		return -EINVAL;
	addr = mm->addr;
	kfree(mm);

	if ((addr >= pci_resource_start(rdev->lldi.pdev, 0)) &&
	    (addr < (pci_resource_start(rdev->lldi.pdev, 0) +
		    pci_resource_len(rdev->lldi.pdev, 0)))) {

		/*
		 * MA_SYNC register...
		 */
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		ret = io_remap_pfn_range(vma, vma->vm_start,
					 addr >> PAGE_SHIFT,
					 len, vma->vm_page_prot);
	} else if ((addr >= pci_resource_start(rdev->lldi.pdev, 2)) &&
		   (addr < (pci_resource_start(rdev->lldi.pdev, 2) +
		    pci_resource_len(rdev->lldi.pdev, 2)))) {

		/*
		 * Map user DB or OCQP memory...
		 */
		if (addr >= rdev->oc_mw_pa)
			vma->vm_page_prot = t4_pgprot_wc(vma->vm_page_prot);
<<<<<<< HEAD
		else
			vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
=======
		else {
			if (!is_t4(rdev->lldi.adapter_type))
				vma->vm_page_prot =
					t4_pgprot_wc(vma->vm_page_prot);
			else
				vma->vm_page_prot =
					pgprot_noncached(vma->vm_page_prot);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = io_remap_pfn_range(vma, vma->vm_start,
					 addr >> PAGE_SHIFT,
					 len, vma->vm_page_prot);
	} else {

		/*
		 * Map WQ or CQ contig dma memory...
		 */
		ret = remap_pfn_range(vma, vma->vm_start,
				      addr >> PAGE_SHIFT,
				      len, vma->vm_page_prot);
	}

	return ret;
}

<<<<<<< HEAD
static int c4iw_deallocate_pd(struct ib_pd *pd)
=======
static int c4iw_deallocate_pd(struct ib_pd *pd, struct ib_udata *udata)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct c4iw_dev *rhp;
	struct c4iw_pd *php;

	php = to_c4iw_pd(pd);
	rhp = php->rhp;
<<<<<<< HEAD
	PDBG("%s ibpd %p pdid 0x%x\n", __func__, pd, php->pdid);
	c4iw_put_resource(&rhp->rdev.resource.pdid_fifo, php->pdid,
			  &rhp->rdev.resource.pdid_fifo_lock);
	kfree(php);
	return 0;
}

static struct ib_pd *c4iw_allocate_pd(struct ib_device *ibdev,
				      struct ib_ucontext *context,
				      struct ib_udata *udata)
{
	struct c4iw_pd *php;
	u32 pdid;
	struct c4iw_dev *rhp;

	PDBG("%s ibdev %p\n", __func__, ibdev);
	rhp = (struct c4iw_dev *) ibdev;
	pdid =  c4iw_get_resource(&rhp->rdev.resource.pdid_fifo,
				  &rhp->rdev.resource.pdid_fifo_lock);
	if (!pdid)
		return ERR_PTR(-EINVAL);
	php = kzalloc(sizeof(*php), GFP_KERNEL);
	if (!php) {
		c4iw_put_resource(&rhp->rdev.resource.pdid_fifo, pdid,
				  &rhp->rdev.resource.pdid_fifo_lock);
		return ERR_PTR(-ENOMEM);
	}
	php->pdid = pdid;
	php->rhp = rhp;
	if (context) {
		if (ib_copy_to_udata(udata, &php->pdid, sizeof(u32))) {
			c4iw_deallocate_pd(&php->ibpd);
			return ERR_PTR(-EFAULT);
		}
	}
	PDBG("%s pdid 0x%0x ptr 0x%p\n", __func__, pdid, php);
	return &php->ibpd;
}

static int c4iw_query_pkey(struct ib_device *ibdev, u8 port, u16 index,
			   u16 *pkey)
{
	PDBG("%s ibdev %p\n", __func__, ibdev);
	*pkey = 0;
	return 0;
}

static int c4iw_query_gid(struct ib_device *ibdev, u8 port, int index,
=======
	pr_debug("ibpd %p pdid 0x%x\n", pd, php->pdid);
	c4iw_put_resource(&rhp->rdev.resource.pdid_table, php->pdid);
	mutex_lock(&rhp->rdev.stats.lock);
	rhp->rdev.stats.pd.cur--;
	mutex_unlock(&rhp->rdev.stats.lock);
	return 0;
}

static int c4iw_allocate_pd(struct ib_pd *pd, struct ib_udata *udata)
{
	struct c4iw_pd *php = to_c4iw_pd(pd);
	struct ib_device *ibdev = pd->device;
	u32 pdid;
	struct c4iw_dev *rhp;

	pr_debug("ibdev %p\n", ibdev);
	rhp = (struct c4iw_dev *) ibdev;
	pdid =  c4iw_get_resource(&rhp->rdev.resource.pdid_table);
	if (!pdid)
		return -EINVAL;

	php->pdid = pdid;
	php->rhp = rhp;
	if (udata) {
		struct c4iw_alloc_pd_resp uresp = {.pdid = php->pdid};

		if (ib_copy_to_udata(udata, &uresp, sizeof(uresp))) {
			c4iw_deallocate_pd(&php->ibpd, udata);
			return -EFAULT;
		}
	}
	mutex_lock(&rhp->rdev.stats.lock);
	rhp->rdev.stats.pd.cur++;
	if (rhp->rdev.stats.pd.cur > rhp->rdev.stats.pd.max)
		rhp->rdev.stats.pd.max = rhp->rdev.stats.pd.cur;
	mutex_unlock(&rhp->rdev.stats.lock);
	pr_debug("pdid 0x%0x ptr 0x%p\n", pdid, php);
	return 0;
}

static int c4iw_query_gid(struct ib_device *ibdev, u32 port, int index,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  union ib_gid *gid)
{
	struct c4iw_dev *dev;

<<<<<<< HEAD
	PDBG("%s ibdev %p, port %d, index %d, gid %p\n",
	       __func__, ibdev, port, index, gid);
	dev = to_c4iw_dev(ibdev);
	BUG_ON(port == 0);
=======
	pr_debug("ibdev %p, port %u, index %d, gid %p\n",
		 ibdev, port, index, gid);
	if (!port)
		return -EINVAL;
	dev = to_c4iw_dev(ibdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(&(gid->raw[0]), 0, sizeof(gid->raw));
	memcpy(&(gid->raw[0]), dev->rdev.lldi.ports[port-1]->dev_addr, 6);
	return 0;
}

<<<<<<< HEAD
static int c4iw_query_device(struct ib_device *ibdev,
			     struct ib_device_attr *props)
{

	struct c4iw_dev *dev;
	PDBG("%s ibdev %p\n", __func__, ibdev);

	dev = to_c4iw_dev(ibdev);
	memset(props, 0, sizeof *props);
	memcpy(&props->sys_image_guid, dev->rdev.lldi.ports[0]->dev_addr, 6);
	props->hw_ver = dev->rdev.lldi.adapter_type;
	props->fw_ver = dev->rdev.lldi.fw_vers;
	props->device_cap_flags = dev->device_cap_flags;
=======
static int c4iw_query_device(struct ib_device *ibdev, struct ib_device_attr *props,
			     struct ib_udata *uhw)
{

	struct c4iw_dev *dev;

	pr_debug("ibdev %p\n", ibdev);

	if (uhw->inlen || uhw->outlen)
		return -EINVAL;

	dev = to_c4iw_dev(ibdev);
	addrconf_addr_eui48((u8 *)&props->sys_image_guid,
			    dev->rdev.lldi.ports[0]->dev_addr);
	props->hw_ver = CHELSIO_CHIP_RELEASE(dev->rdev.lldi.adapter_type);
	props->fw_ver = dev->rdev.lldi.fw_vers;
	props->device_cap_flags = IB_DEVICE_MEM_WINDOW;
	props->kernel_cap_flags = IBK_LOCAL_DMA_LKEY;
	if (fastreg_support)
		props->device_cap_flags |= IB_DEVICE_MEM_MGT_EXTENSIONS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	props->page_size_cap = T4_PAGESIZE_MASK;
	props->vendor_id = (u32)dev->rdev.lldi.pdev->vendor;
	props->vendor_part_id = (u32)dev->rdev.lldi.pdev->device;
	props->max_mr_size = T4_MAX_MR_SIZE;
<<<<<<< HEAD
	props->max_qp = T4_MAX_NUM_QP;
	props->max_qp_wr = T4_MAX_QP_DEPTH;
	props->max_sge = T4_MAX_RECV_SGE;
	props->max_sge_rd = 1;
	props->max_qp_rd_atom = c4iw_max_read_depth;
	props->max_qp_init_rd_atom = c4iw_max_read_depth;
	props->max_cq = T4_MAX_NUM_CQ;
	props->max_cqe = T4_MAX_CQ_DEPTH;
	props->max_mr = c4iw_num_stags(&dev->rdev);
	props->max_pd = T4_MAX_NUM_PD;
	props->local_ca_ack_delay = 0;
	props->max_fast_reg_page_list_len = T4_MAX_FR_DEPTH;
=======
	props->max_qp = dev->rdev.lldi.vr->qp.size / 2;
	props->max_srq = dev->rdev.lldi.vr->srq.size;
	props->max_qp_wr = dev->rdev.hw_queue.t4_max_qp_depth;
	props->max_srq_wr = dev->rdev.hw_queue.t4_max_qp_depth;
	props->max_send_sge = min(T4_MAX_SEND_SGE, T4_MAX_WRITE_SGE);
	props->max_recv_sge = T4_MAX_RECV_SGE;
	props->max_srq_sge = T4_MAX_RECV_SGE;
	props->max_sge_rd = 1;
	props->max_res_rd_atom = dev->rdev.lldi.max_ird_adapter;
	props->max_qp_rd_atom = min(dev->rdev.lldi.max_ordird_qp,
				    c4iw_max_read_depth);
	props->max_qp_init_rd_atom = props->max_qp_rd_atom;
	props->max_cq = dev->rdev.lldi.vr->qp.size;
	props->max_cqe = dev->rdev.hw_queue.t4_max_cq_depth;
	props->max_mr = c4iw_num_stags(&dev->rdev);
	props->max_pd = T4_MAX_NUM_PD;
	props->local_ca_ack_delay = 0;
	props->max_fast_reg_page_list_len =
		t4_max_fr_depth(dev->rdev.lldi.ulptx_memwrite_dsgl && use_dsgl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int c4iw_query_port(struct ib_device *ibdev, u8 port,
			   struct ib_port_attr *props)
{
	struct c4iw_dev *dev;
	struct net_device *netdev;
	struct in_device *inetdev;

	PDBG("%s ibdev %p\n", __func__, ibdev);

	dev = to_c4iw_dev(ibdev);
	netdev = dev->rdev.lldi.ports[port-1];

	memset(props, 0, sizeof(struct ib_port_attr));
	props->max_mtu = IB_MTU_4096;
	if (netdev->mtu >= 4096)
		props->active_mtu = IB_MTU_4096;
	else if (netdev->mtu >= 2048)
		props->active_mtu = IB_MTU_2048;
	else if (netdev->mtu >= 1024)
		props->active_mtu = IB_MTU_1024;
	else if (netdev->mtu >= 512)
		props->active_mtu = IB_MTU_512;
	else
		props->active_mtu = IB_MTU_256;

	if (!netif_carrier_ok(netdev))
		props->state = IB_PORT_DOWN;
	else {
		inetdev = in_dev_get(netdev);
		if (inetdev) {
			if (inetdev->ifa_list)
				props->state = IB_PORT_ACTIVE;
			else
				props->state = IB_PORT_INIT;
			in_dev_put(inetdev);
		} else
			props->state = IB_PORT_INIT;
	}
=======
static int c4iw_query_port(struct ib_device *ibdev, u32 port,
			   struct ib_port_attr *props)
{
	int ret = 0;
	pr_debug("ibdev %p\n", ibdev);
	ret = ib_get_eth_speed(ibdev, port, &props->active_speed,
			       &props->active_width);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	props->port_cap_flags =
	    IB_PORT_CM_SUP |
	    IB_PORT_SNMP_TUNNEL_SUP |
	    IB_PORT_REINIT_SUP |
	    IB_PORT_DEVICE_MGMT_SUP |
	    IB_PORT_VENDOR_CLASS_SUP | IB_PORT_BOOT_MGMT_SUP;
	props->gid_tbl_len = 1;
<<<<<<< HEAD
	props->pkey_tbl_len = 1;
	props->active_width = 2;
	props->active_speed = IB_SPEED_DDR;
	props->max_msg_sz = -1;

	return 0;
}

static ssize_t show_rev(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct c4iw_dev *c4iw_dev = container_of(dev, struct c4iw_dev,
						 ibdev.dev);
	PDBG("%s dev 0x%p\n", __func__, dev);
	return sprintf(buf, "%d\n", c4iw_dev->rdev.lldi.adapter_type);
}

static ssize_t show_fw_ver(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct c4iw_dev *c4iw_dev = container_of(dev, struct c4iw_dev,
						 ibdev.dev);
	PDBG("%s dev 0x%p\n", __func__, dev);

	return sprintf(buf, "%u.%u.%u.%u\n",
			FW_HDR_FW_VER_MAJOR_GET(c4iw_dev->rdev.lldi.fw_vers),
			FW_HDR_FW_VER_MINOR_GET(c4iw_dev->rdev.lldi.fw_vers),
			FW_HDR_FW_VER_MICRO_GET(c4iw_dev->rdev.lldi.fw_vers),
			FW_HDR_FW_VER_BUILD_GET(c4iw_dev->rdev.lldi.fw_vers));
}

static ssize_t show_hca(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct c4iw_dev *c4iw_dev = container_of(dev, struct c4iw_dev,
						 ibdev.dev);
	struct ethtool_drvinfo info;
	struct net_device *lldev = c4iw_dev->rdev.lldi.ports[0];

	PDBG("%s dev 0x%p\n", __func__, dev);
	lldev->ethtool_ops->get_drvinfo(lldev, &info);
	return sprintf(buf, "%s\n", info.driver);
}

static ssize_t show_board(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct c4iw_dev *c4iw_dev = container_of(dev, struct c4iw_dev,
						 ibdev.dev);
	PDBG("%s dev 0x%p\n", __func__, dev);
	return sprintf(buf, "%x.%x\n", c4iw_dev->rdev.lldi.pdev->vendor,
		       c4iw_dev->rdev.lldi.pdev->device);
}

static int c4iw_get_mib(struct ib_device *ibdev,
			union rdma_protocol_stats *stats)
=======
	props->max_msg_sz = -1;

	return ret;
}

static ssize_t hw_rev_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct c4iw_dev *c4iw_dev =
			rdma_device_to_drv_device(dev, struct c4iw_dev, ibdev);

	pr_debug("dev 0x%p\n", dev);
	return sysfs_emit(
		buf, "%d\n",
		CHELSIO_CHIP_RELEASE(c4iw_dev->rdev.lldi.adapter_type));
}
static DEVICE_ATTR_RO(hw_rev);

static ssize_t hca_type_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	struct c4iw_dev *c4iw_dev =
			rdma_device_to_drv_device(dev, struct c4iw_dev, ibdev);
	struct ethtool_drvinfo info;
	struct net_device *lldev = c4iw_dev->rdev.lldi.ports[0];

	pr_debug("dev 0x%p\n", dev);
	lldev->ethtool_ops->get_drvinfo(lldev, &info);
	return sysfs_emit(buf, "%s\n", info.driver);
}
static DEVICE_ATTR_RO(hca_type);

static ssize_t board_id_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct c4iw_dev *c4iw_dev =
			rdma_device_to_drv_device(dev, struct c4iw_dev, ibdev);

	pr_debug("dev 0x%p\n", dev);
	return sysfs_emit(buf, "%x.%x\n", c4iw_dev->rdev.lldi.pdev->vendor,
			  c4iw_dev->rdev.lldi.pdev->device);
}
static DEVICE_ATTR_RO(board_id);

enum counters {
	IP4INSEGS,
	IP4OUTSEGS,
	IP4RETRANSSEGS,
	IP4OUTRSTS,
	IP6INSEGS,
	IP6OUTSEGS,
	IP6RETRANSSEGS,
	IP6OUTRSTS,
	NR_COUNTERS
};

static const struct rdma_stat_desc cxgb4_descs[] = {
	[IP4INSEGS].name = "ip4InSegs",
	[IP4OUTSEGS].name = "ip4OutSegs",
	[IP4RETRANSSEGS].name = "ip4RetransSegs",
	[IP4OUTRSTS].name = "ip4OutRsts",
	[IP6INSEGS].name = "ip6InSegs",
	[IP6OUTSEGS].name = "ip6OutSegs",
	[IP6RETRANSSEGS].name = "ip6RetransSegs",
	[IP6OUTRSTS].name = "ip6OutRsts"
};

static struct rdma_hw_stats *c4iw_alloc_device_stats(struct ib_device *ibdev)
{
	BUILD_BUG_ON(ARRAY_SIZE(cxgb4_descs) != NR_COUNTERS);

	/* FIXME: these look like port stats */
	return rdma_alloc_hw_stats_struct(cxgb4_descs, NR_COUNTERS,
					  RDMA_HW_STATS_DEFAULT_LIFESPAN);
}

static int c4iw_get_mib(struct ib_device *ibdev,
			struct rdma_hw_stats *stats,
			u32 port, int index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tp_tcp_stats v4, v6;
	struct c4iw_dev *c4iw_dev = to_c4iw_dev(ibdev);

	cxgb4_get_tcp_stats(c4iw_dev->rdev.lldi.pdev, &v4, &v6);
<<<<<<< HEAD
	memset(stats, 0, sizeof *stats);
	stats->iw.tcpInSegs = v4.tcpInSegs + v6.tcpInSegs;
	stats->iw.tcpOutSegs = v4.tcpOutSegs + v6.tcpOutSegs;
	stats->iw.tcpRetransSegs = v4.tcpRetransSegs + v6.tcpRetransSegs;
	stats->iw.tcpOutRsts = v4.tcpOutRsts + v6.tcpOutSegs;
=======
	stats->value[IP4INSEGS] = v4.tcp_in_segs;
	stats->value[IP4OUTSEGS] = v4.tcp_out_segs;
	stats->value[IP4RETRANSSEGS] = v4.tcp_retrans_segs;
	stats->value[IP4OUTRSTS] = v4.tcp_out_rsts;
	stats->value[IP6INSEGS] = v6.tcp_in_segs;
	stats->value[IP6OUTSEGS] = v6.tcp_out_segs;
	stats->value[IP6RETRANSSEGS] = v6.tcp_retrans_segs;
	stats->value[IP6OUTRSTS] = v6.tcp_out_rsts;

	return stats->num_counters;
}

static struct attribute *c4iw_class_attributes[] = {
	&dev_attr_hw_rev.attr,
	&dev_attr_hca_type.attr,
	&dev_attr_board_id.attr,
	NULL
};

static const struct attribute_group c4iw_attr_group = {
	.attrs = c4iw_class_attributes,
};

static int c4iw_port_immutable(struct ib_device *ibdev, u32 port_num,
			       struct ib_port_immutable *immutable)
{
	struct ib_port_attr attr;
	int err;

	immutable->core_cap_flags = RDMA_CORE_PORT_IWARP;

	err = ib_query_port(ibdev, port_num, &attr);
	if (err)
		return err;

	immutable->gid_tbl_len = attr.gid_tbl_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static DEVICE_ATTR(hw_rev, S_IRUGO, show_rev, NULL);
static DEVICE_ATTR(fw_ver, S_IRUGO, show_fw_ver, NULL);
static DEVICE_ATTR(hca_type, S_IRUGO, show_hca, NULL);
static DEVICE_ATTR(board_id, S_IRUGO, show_board, NULL);

static struct device_attribute *c4iw_class_attributes[] = {
	&dev_attr_hw_rev,
	&dev_attr_fw_ver,
	&dev_attr_hca_type,
	&dev_attr_board_id,
};

int c4iw_register_device(struct c4iw_dev *dev)
=======
static void get_dev_fw_str(struct ib_device *dev, char *str)
{
	struct c4iw_dev *c4iw_dev = container_of(dev, struct c4iw_dev,
						 ibdev);
	pr_debug("dev 0x%p\n", dev);

	snprintf(str, IB_FW_VERSION_NAME_MAX, "%u.%u.%u.%u",
		 FW_HDR_FW_VER_MAJOR_G(c4iw_dev->rdev.lldi.fw_vers),
		 FW_HDR_FW_VER_MINOR_G(c4iw_dev->rdev.lldi.fw_vers),
		 FW_HDR_FW_VER_MICRO_G(c4iw_dev->rdev.lldi.fw_vers),
		 FW_HDR_FW_VER_BUILD_G(c4iw_dev->rdev.lldi.fw_vers));
}

static const struct ib_device_ops c4iw_dev_ops = {
	.owner = THIS_MODULE,
	.driver_id = RDMA_DRIVER_CXGB4,
	.uverbs_abi_ver = C4IW_UVERBS_ABI_VERSION,

	.alloc_hw_device_stats = c4iw_alloc_device_stats,
	.alloc_mr = c4iw_alloc_mr,
	.alloc_pd = c4iw_allocate_pd,
	.alloc_ucontext = c4iw_alloc_ucontext,
	.create_cq = c4iw_create_cq,
	.create_qp = c4iw_create_qp,
	.create_srq = c4iw_create_srq,
	.dealloc_pd = c4iw_deallocate_pd,
	.dealloc_ucontext = c4iw_dealloc_ucontext,
	.dereg_mr = c4iw_dereg_mr,
	.destroy_cq = c4iw_destroy_cq,
	.destroy_qp = c4iw_destroy_qp,
	.destroy_srq = c4iw_destroy_srq,
	.device_group = &c4iw_attr_group,
	.fill_res_cq_entry = c4iw_fill_res_cq_entry,
	.fill_res_cm_id_entry = c4iw_fill_res_cm_id_entry,
	.fill_res_mr_entry = c4iw_fill_res_mr_entry,
	.get_dev_fw_str = get_dev_fw_str,
	.get_dma_mr = c4iw_get_dma_mr,
	.get_hw_stats = c4iw_get_mib,
	.get_port_immutable = c4iw_port_immutable,
	.iw_accept = c4iw_accept_cr,
	.iw_add_ref = c4iw_qp_add_ref,
	.iw_connect = c4iw_connect,
	.iw_create_listen = c4iw_create_listen,
	.iw_destroy_listen = c4iw_destroy_listen,
	.iw_get_qp = c4iw_get_qp,
	.iw_reject = c4iw_reject_cr,
	.iw_rem_ref = c4iw_qp_rem_ref,
	.map_mr_sg = c4iw_map_mr_sg,
	.mmap = c4iw_mmap,
	.modify_qp = c4iw_ib_modify_qp,
	.modify_srq = c4iw_modify_srq,
	.poll_cq = c4iw_poll_cq,
	.post_recv = c4iw_post_receive,
	.post_send = c4iw_post_send,
	.post_srq_recv = c4iw_post_srq_recv,
	.query_device = c4iw_query_device,
	.query_gid = c4iw_query_gid,
	.query_port = c4iw_query_port,
	.query_qp = c4iw_ib_query_qp,
	.reg_user_mr = c4iw_reg_user_mr,
	.req_notify_cq = c4iw_arm_cq,

	INIT_RDMA_OBJ_SIZE(ib_cq, c4iw_cq, ibcq),
	INIT_RDMA_OBJ_SIZE(ib_mw, c4iw_mw, ibmw),
	INIT_RDMA_OBJ_SIZE(ib_pd, c4iw_pd, ibpd),
	INIT_RDMA_OBJ_SIZE(ib_qp, c4iw_qp, ibqp),
	INIT_RDMA_OBJ_SIZE(ib_srq, c4iw_srq, ibsrq),
	INIT_RDMA_OBJ_SIZE(ib_ucontext, c4iw_ucontext, ibucontext),
};

static int set_netdevs(struct ib_device *ib_dev, struct c4iw_rdev *rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	int i;

<<<<<<< HEAD
	PDBG("%s c4iw_dev %p\n", __func__, dev);
	BUG_ON(!dev->rdev.lldi.ports[0]);
	strlcpy(dev->ibdev.name, "cxgb4_%d", IB_DEVICE_NAME_MAX);
	memset(&dev->ibdev.node_guid, 0, sizeof(dev->ibdev.node_guid));
	memcpy(&dev->ibdev.node_guid, dev->rdev.lldi.ports[0]->dev_addr, 6);
	dev->ibdev.owner = THIS_MODULE;
	dev->device_cap_flags = IB_DEVICE_LOCAL_DMA_LKEY | IB_DEVICE_MEM_WINDOW;
	if (fastreg_support)
		dev->device_cap_flags |= IB_DEVICE_MEM_MGT_EXTENSIONS;
	dev->ibdev.local_dma_lkey = 0;
	dev->ibdev.uverbs_cmd_mask =
	    (1ull << IB_USER_VERBS_CMD_GET_CONTEXT) |
	    (1ull << IB_USER_VERBS_CMD_QUERY_DEVICE) |
	    (1ull << IB_USER_VERBS_CMD_QUERY_PORT) |
	    (1ull << IB_USER_VERBS_CMD_ALLOC_PD) |
	    (1ull << IB_USER_VERBS_CMD_DEALLOC_PD) |
	    (1ull << IB_USER_VERBS_CMD_REG_MR) |
	    (1ull << IB_USER_VERBS_CMD_DEREG_MR) |
	    (1ull << IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL) |
	    (1ull << IB_USER_VERBS_CMD_CREATE_CQ) |
	    (1ull << IB_USER_VERBS_CMD_DESTROY_CQ) |
	    (1ull << IB_USER_VERBS_CMD_REQ_NOTIFY_CQ) |
	    (1ull << IB_USER_VERBS_CMD_CREATE_QP) |
	    (1ull << IB_USER_VERBS_CMD_MODIFY_QP) |
	    (1ull << IB_USER_VERBS_CMD_POLL_CQ) |
	    (1ull << IB_USER_VERBS_CMD_DESTROY_QP) |
	    (1ull << IB_USER_VERBS_CMD_POST_SEND) |
	    (1ull << IB_USER_VERBS_CMD_POST_RECV);
	dev->ibdev.node_type = RDMA_NODE_RNIC;
	memcpy(dev->ibdev.node_desc, C4IW_NODE_DESC, sizeof(C4IW_NODE_DESC));
	dev->ibdev.phys_port_cnt = dev->rdev.lldi.nports;
	dev->ibdev.num_comp_vectors = 1;
	dev->ibdev.dma_device = &(dev->rdev.lldi.pdev->dev);
	dev->ibdev.query_device = c4iw_query_device;
	dev->ibdev.query_port = c4iw_query_port;
	dev->ibdev.query_pkey = c4iw_query_pkey;
	dev->ibdev.query_gid = c4iw_query_gid;
	dev->ibdev.alloc_ucontext = c4iw_alloc_ucontext;
	dev->ibdev.dealloc_ucontext = c4iw_dealloc_ucontext;
	dev->ibdev.mmap = c4iw_mmap;
	dev->ibdev.alloc_pd = c4iw_allocate_pd;
	dev->ibdev.dealloc_pd = c4iw_deallocate_pd;
	dev->ibdev.create_ah = c4iw_ah_create;
	dev->ibdev.destroy_ah = c4iw_ah_destroy;
	dev->ibdev.create_qp = c4iw_create_qp;
	dev->ibdev.modify_qp = c4iw_ib_modify_qp;
	dev->ibdev.destroy_qp = c4iw_destroy_qp;
	dev->ibdev.create_cq = c4iw_create_cq;
	dev->ibdev.destroy_cq = c4iw_destroy_cq;
	dev->ibdev.resize_cq = c4iw_resize_cq;
	dev->ibdev.poll_cq = c4iw_poll_cq;
	dev->ibdev.get_dma_mr = c4iw_get_dma_mr;
	dev->ibdev.reg_phys_mr = c4iw_register_phys_mem;
	dev->ibdev.rereg_phys_mr = c4iw_reregister_phys_mem;
	dev->ibdev.reg_user_mr = c4iw_reg_user_mr;
	dev->ibdev.dereg_mr = c4iw_dereg_mr;
	dev->ibdev.alloc_mw = c4iw_alloc_mw;
	dev->ibdev.bind_mw = c4iw_bind_mw;
	dev->ibdev.dealloc_mw = c4iw_dealloc_mw;
	dev->ibdev.alloc_fast_reg_mr = c4iw_alloc_fast_reg_mr;
	dev->ibdev.alloc_fast_reg_page_list = c4iw_alloc_fastreg_pbl;
	dev->ibdev.free_fast_reg_page_list = c4iw_free_fastreg_pbl;
	dev->ibdev.attach_mcast = c4iw_multicast_attach;
	dev->ibdev.detach_mcast = c4iw_multicast_detach;
	dev->ibdev.process_mad = c4iw_process_mad;
	dev->ibdev.req_notify_cq = c4iw_arm_cq;
	dev->ibdev.post_send = c4iw_post_send;
	dev->ibdev.post_recv = c4iw_post_receive;
	dev->ibdev.get_protocol_stats = c4iw_get_mib;
	dev->ibdev.uverbs_abi_ver = C4IW_UVERBS_ABI_VERSION;

	dev->ibdev.iwcm = kmalloc(sizeof(struct iw_cm_verbs), GFP_KERNEL);
	if (!dev->ibdev.iwcm)
		return -ENOMEM;

	dev->ibdev.iwcm->connect = c4iw_connect;
	dev->ibdev.iwcm->accept = c4iw_accept_cr;
	dev->ibdev.iwcm->reject = c4iw_reject_cr;
	dev->ibdev.iwcm->create_listen = c4iw_create_listen;
	dev->ibdev.iwcm->destroy_listen = c4iw_destroy_listen;
	dev->ibdev.iwcm->add_ref = c4iw_qp_add_ref;
	dev->ibdev.iwcm->rem_ref = c4iw_qp_rem_ref;
	dev->ibdev.iwcm->get_qp = c4iw_get_qp;

	ret = ib_register_device(&dev->ibdev, NULL);
	if (ret)
		goto bail1;

	for (i = 0; i < ARRAY_SIZE(c4iw_class_attributes); ++i) {
		ret = device_create_file(&dev->ibdev.dev,
					 c4iw_class_attributes[i]);
		if (ret)
			goto bail2;
	}
	return 0;
bail2:
	ib_unregister_device(&dev->ibdev);
bail1:
	kfree(dev->ibdev.iwcm);
	return ret;
=======
	for (i = 0; i < rdev->lldi.nports; i++) {
		ret = ib_device_set_netdev(ib_dev, rdev->lldi.ports[i],
					   i + 1);
		if (ret)
			return ret;
	}
	return 0;
}

void c4iw_register_device(struct work_struct *work)
{
	int ret;
	struct uld_ctx *ctx = container_of(work, struct uld_ctx, reg_work);
	struct c4iw_dev *dev = ctx->dev;

	pr_debug("c4iw_dev %p\n", dev);
	addrconf_addr_eui48((u8 *)&dev->ibdev.node_guid,
			    dev->rdev.lldi.ports[0]->dev_addr);
	dev->ibdev.local_dma_lkey = 0;
	dev->ibdev.node_type = RDMA_NODE_RNIC;
	BUILD_BUG_ON(sizeof(C4IW_NODE_DESC) > IB_DEVICE_NODE_DESC_MAX);
	memcpy(dev->ibdev.node_desc, C4IW_NODE_DESC, sizeof(C4IW_NODE_DESC));
	dev->ibdev.phys_port_cnt = dev->rdev.lldi.nports;
	dev->ibdev.num_comp_vectors =  dev->rdev.lldi.nciq;
	dev->ibdev.dev.parent = &dev->rdev.lldi.pdev->dev;

	memcpy(dev->ibdev.iw_ifname, dev->rdev.lldi.ports[0]->name,
	       sizeof(dev->ibdev.iw_ifname));

	ib_set_device_ops(&dev->ibdev, &c4iw_dev_ops);
	ret = set_netdevs(&dev->ibdev, &dev->rdev);
	if (ret)
		goto err_dealloc_ctx;
	dma_set_max_seg_size(&dev->rdev.lldi.pdev->dev, UINT_MAX);
	ret = ib_register_device(&dev->ibdev, "cxgb4_%d",
				 &dev->rdev.lldi.pdev->dev);
	if (ret)
		goto err_dealloc_ctx;
	return;

err_dealloc_ctx:
	pr_err("%s - Failed registering iwarp device: %d\n",
	       pci_name(ctx->lldi.pdev), ret);
	c4iw_dealloc(ctx);
	return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void c4iw_unregister_device(struct c4iw_dev *dev)
{
<<<<<<< HEAD
	int i;

	PDBG("%s c4iw_dev %p\n", __func__, dev);
	for (i = 0; i < ARRAY_SIZE(c4iw_class_attributes); ++i)
		device_remove_file(&dev->ibdev.dev,
				   c4iw_class_attributes[i]);
	ib_unregister_device(&dev->ibdev);
	kfree(dev->ibdev.iwcm);
=======
	pr_debug("c4iw_dev %p\n", dev);
	ib_unregister_device(&dev->ibdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
}
