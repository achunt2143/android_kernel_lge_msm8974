<<<<<<< HEAD
/* Copyright (c) 2007 Coraid, Inc.  See COPYING for GPL terms. */
=======
/* Copyright (c) 2013 Coraid, Inc.  See COPYING for GPL terms. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * aoeblk.c
 * block device routines
 */

#include <linux/kernel.h>
#include <linux/hdreg.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
=======
#include <linux/blk-mq.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/backing-dev.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
#include <linux/genhd.h>
#include <linux/netdevice.h>
#include <linux/mutex.h>
#include <linux/export.h>
=======
#include <linux/netdevice.h>
#include <linux/mutex.h>
#include <linux/export.h>
#include <linux/moduleparam.h>
#include <linux/debugfs.h>
#include <scsi/sg.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "aoe.h"

static DEFINE_MUTEX(aoeblk_mutex);
static struct kmem_cache *buf_pool_cache;
<<<<<<< HEAD
=======
static struct dentry *aoe_debugfs_dir;

/* random default picked from the historic block max_sectors cap */
static int aoe_maxsectors = 2560;
module_param(aoe_maxsectors, int, 0644);
MODULE_PARM_DESC(aoe_maxsectors,
	"When nonzero, set the maximum number of sectors per I/O request");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t aoedisk_show_state(struct device *dev,
				  struct device_attribute *attr, char *page)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct aoedev *d = disk->private_data;

<<<<<<< HEAD
	return snprintf(page, PAGE_SIZE,
			"%s%s\n",
=======
	return sysfs_emit(page, "%s%s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			(d->flags & DEVFL_UP) ? "up" : "down",
			(d->flags & DEVFL_KICKME) ? ",kickme" :
			(d->nopen && !(d->flags & DEVFL_UP)) ? ",closewait" : "");
	/* I'd rather see nopen exported so we can ditch closewait */
}
static ssize_t aoedisk_show_mac(struct device *dev,
				struct device_attribute *attr, char *page)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct aoedev *d = disk->private_data;
	struct aoetgt *t = d->targets[0];

	if (t == NULL)
<<<<<<< HEAD
		return snprintf(page, PAGE_SIZE, "none\n");
	return snprintf(page, PAGE_SIZE, "%pm\n", t->addr);
=======
		return sysfs_emit(page, "none\n");
	return sysfs_emit(page, "%pm\n", t->addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t aoedisk_show_netif(struct device *dev,
				  struct device_attribute *attr, char *page)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct aoedev *d = disk->private_data;
	struct net_device *nds[8], **nd, **nnd, **ne;
	struct aoetgt **t, **te;
	struct aoeif *ifp, *e;
	char *p;

	memset(nds, 0, sizeof nds);
	nd = nds;
	ne = nd + ARRAY_SIZE(nds);
	t = d->targets;
<<<<<<< HEAD
	te = t + NTARGETS;
=======
	te = t + d->ntargets;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (; t < te && *t; t++) {
		ifp = (*t)->ifs;
		e = ifp + NAOEIFS;
		for (; ifp < e && ifp->nd; ifp++) {
			for (nnd = nds; nnd < nd; nnd++)
				if (*nnd == ifp->nd)
					break;
			if (nnd == nd && nd != ne)
				*nd++ = ifp->nd;
		}
	}

	ne = nd;
	nd = nds;
	if (*nd == NULL)
<<<<<<< HEAD
		return snprintf(page, PAGE_SIZE, "none\n");
	for (p = page; nd < ne; nd++)
		p += snprintf(p, PAGE_SIZE - (p-page), "%s%s",
			p == page ? "" : ",", (*nd)->name);
	p += snprintf(p, PAGE_SIZE - (p-page), "\n");
=======
		return sysfs_emit(page, "none\n");
	for (p = page; nd < ne; nd++)
		p += scnprintf(p, PAGE_SIZE - (p-page), "%s%s",
			p == page ? "" : ",", (*nd)->name);
	p += scnprintf(p, PAGE_SIZE - (p-page), "\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return p-page;
}
/* firmware version */
static ssize_t aoedisk_show_fwver(struct device *dev,
				  struct device_attribute *attr, char *page)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct aoedev *d = disk->private_data;

<<<<<<< HEAD
	return snprintf(page, PAGE_SIZE, "0x%04x\n", (unsigned int) d->fw_ver);
}

static DEVICE_ATTR(state, S_IRUGO, aoedisk_show_state, NULL);
static DEVICE_ATTR(mac, S_IRUGO, aoedisk_show_mac, NULL);
static DEVICE_ATTR(netif, S_IRUGO, aoedisk_show_netif, NULL);
static struct device_attribute dev_attr_firmware_version = {
	.attr = { .name = "firmware-version", .mode = S_IRUGO },
	.show = aoedisk_show_fwver,
};
=======
	return sysfs_emit(page, "0x%04x\n", (unsigned int) d->fw_ver);
}
static ssize_t aoedisk_show_payload(struct device *dev,
				    struct device_attribute *attr, char *page)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct aoedev *d = disk->private_data;

	return sysfs_emit(page, "%lu\n", d->maxbcnt);
}

static int aoe_debugfs_show(struct seq_file *s, void *ignored)
{
	struct aoedev *d;
	struct aoetgt **t, **te;
	struct aoeif *ifp, *ife;
	unsigned long flags;
	char c;

	d = s->private;
	seq_printf(s, "rttavg: %d rttdev: %d\n",
		d->rttavg >> RTTSCALE,
		d->rttdev >> RTTDSCALE);
	seq_printf(s, "nskbpool: %d\n", skb_queue_len(&d->skbpool));
	seq_printf(s, "kicked: %ld\n", d->kicked);
	seq_printf(s, "maxbcnt: %ld\n", d->maxbcnt);
	seq_printf(s, "ref: %ld\n", d->ref);

	spin_lock_irqsave(&d->lock, flags);
	t = d->targets;
	te = t + d->ntargets;
	for (; t < te && *t; t++) {
		c = '\t';
		seq_printf(s, "falloc: %ld\n", (*t)->falloc);
		seq_printf(s, "ffree: %p\n",
			list_empty(&(*t)->ffree) ? NULL : (*t)->ffree.next);
		seq_printf(s, "%pm:%d:%d:%d\n", (*t)->addr, (*t)->nout,
			(*t)->maxout, (*t)->nframes);
		seq_printf(s, "\tssthresh:%d\n", (*t)->ssthresh);
		seq_printf(s, "\ttaint:%d\n", (*t)->taint);
		seq_printf(s, "\tr:%d\n", (*t)->rpkts);
		seq_printf(s, "\tw:%d\n", (*t)->wpkts);
		ifp = (*t)->ifs;
		ife = ifp + ARRAY_SIZE((*t)->ifs);
		for (; ifp->nd && ifp < ife; ifp++) {
			seq_printf(s, "%c%s", c, ifp->nd->name);
			c = ',';
		}
		seq_puts(s, "\n");
	}
	spin_unlock_irqrestore(&d->lock, flags);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(aoe_debugfs);

static DEVICE_ATTR(state, 0444, aoedisk_show_state, NULL);
static DEVICE_ATTR(mac, 0444, aoedisk_show_mac, NULL);
static DEVICE_ATTR(netif, 0444, aoedisk_show_netif, NULL);
static struct device_attribute dev_attr_firmware_version = {
	.attr = { .name = "firmware-version", .mode = 0444 },
	.show = aoedisk_show_fwver,
};
static DEVICE_ATTR(payload, 0444, aoedisk_show_payload, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *aoe_attrs[] = {
	&dev_attr_state.attr,
	&dev_attr_mac.attr,
	&dev_attr_netif.attr,
	&dev_attr_firmware_version.attr,
<<<<<<< HEAD
	NULL,
};

static const struct attribute_group attr_group = {
	.attrs = aoe_attrs,
};

static int
aoedisk_add_sysfs(struct aoedev *d)
{
	return sysfs_create_group(&disk_to_dev(d->gd)->kobj, &attr_group);
}
void
aoedisk_rm_sysfs(struct aoedev *d)
{
	sysfs_remove_group(&disk_to_dev(d->gd)->kobj, &attr_group);
}

static int
aoeblk_open(struct block_device *bdev, fmode_t mode)
{
	struct aoedev *d = bdev->bd_disk->private_data;
	ulong flags;

	mutex_lock(&aoeblk_mutex);
	spin_lock_irqsave(&d->lock, flags);
	if (d->flags & DEVFL_UP) {
=======
	&dev_attr_payload.attr,
	NULL,
};

static const struct attribute_group aoe_attr_group = {
	.attrs = aoe_attrs,
};

static const struct attribute_group *aoe_attr_groups[] = {
	&aoe_attr_group,
	NULL,
};

static void
aoedisk_add_debugfs(struct aoedev *d)
{
	char *p;

	if (aoe_debugfs_dir == NULL)
		return;
	p = strchr(d->gd->disk_name, '/');
	if (p == NULL)
		p = d->gd->disk_name;
	else
		p++;
	BUG_ON(*p == '\0');
	d->debugfs = debugfs_create_file(p, 0444, aoe_debugfs_dir, d,
					 &aoe_debugfs_fops);
}
void
aoedisk_rm_debugfs(struct aoedev *d)
{
	debugfs_remove(d->debugfs);
	d->debugfs = NULL;
}

static int
aoeblk_open(struct gendisk *disk, blk_mode_t mode)
{
	struct aoedev *d = disk->private_data;
	ulong flags;

	if (!virt_addr_valid(d)) {
		pr_crit("aoe: invalid device pointer in %s\n",
			__func__);
		WARN_ON(1);
		return -ENODEV;
	}
	if (!(d->flags & DEVFL_UP) || d->flags & DEVFL_TKILL)
		return -ENODEV;

	mutex_lock(&aoeblk_mutex);
	spin_lock_irqsave(&d->lock, flags);
	if (d->flags & DEVFL_UP && !(d->flags & DEVFL_TKILL)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		d->nopen++;
		spin_unlock_irqrestore(&d->lock, flags);
		mutex_unlock(&aoeblk_mutex);
		return 0;
	}
	spin_unlock_irqrestore(&d->lock, flags);
	mutex_unlock(&aoeblk_mutex);
	return -ENODEV;
}

<<<<<<< HEAD
static int
aoeblk_release(struct gendisk *disk, fmode_t mode)
=======
static void
aoeblk_release(struct gendisk *disk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct aoedev *d = disk->private_data;
	ulong flags;

	spin_lock_irqsave(&d->lock, flags);

	if (--d->nopen == 0) {
		spin_unlock_irqrestore(&d->lock, flags);
		aoecmd_cfg(d->aoemajor, d->aoeminor);
<<<<<<< HEAD
		return 0;
	}
	spin_unlock_irqrestore(&d->lock, flags);

	return 0;
}

static void
aoeblk_make_request(struct request_queue *q, struct bio *bio)
{
	struct sk_buff_head queue;
	struct aoedev *d;
	struct buf *buf;
	ulong flags;

	blk_queue_bounce(q, &bio);

	if (bio == NULL) {
		printk(KERN_ERR "aoe: bio is NULL\n");
		BUG();
		return;
	}
	d = bio->bi_bdev->bd_disk->private_data;
	if (d == NULL) {
		printk(KERN_ERR "aoe: bd_disk->private_data is NULL\n");
		BUG();
		bio_endio(bio, -ENXIO);
		return;
	} else if (bio->bi_io_vec == NULL) {
		printk(KERN_ERR "aoe: bi_io_vec is NULL\n");
		BUG();
		bio_endio(bio, -ENXIO);
		return;
	}
	buf = mempool_alloc(d->bufpool, GFP_NOIO);
	if (buf == NULL) {
		printk(KERN_INFO "aoe: buf allocation failure\n");
		bio_endio(bio, -ENOMEM);
		return;
	}
	memset(buf, 0, sizeof(*buf));
	INIT_LIST_HEAD(&buf->bufs);
	buf->stime = jiffies;
	buf->bio = bio;
	buf->resid = bio->bi_size;
	buf->sector = bio->bi_sector;
	buf->bv = &bio->bi_io_vec[bio->bi_idx];
	buf->bv_resid = buf->bv->bv_len;
	WARN_ON(buf->bv_resid == 0);
	buf->bv_off = buf->bv->bv_offset;

	spin_lock_irqsave(&d->lock, flags);
=======
		return;
	}
	spin_unlock_irqrestore(&d->lock, flags);
}

static blk_status_t aoeblk_queue_rq(struct blk_mq_hw_ctx *hctx,
				    const struct blk_mq_queue_data *bd)
{
	struct aoedev *d = hctx->queue->queuedata;

	spin_lock_irq(&d->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((d->flags & DEVFL_UP) == 0) {
		pr_info_ratelimited("aoe: device %ld.%d is not up\n",
			d->aoemajor, d->aoeminor);
<<<<<<< HEAD
		spin_unlock_irqrestore(&d->lock, flags);
		mempool_free(buf, d->bufpool);
		bio_endio(bio, -ENXIO);
		return;
	}

	list_add_tail(&buf->bufs, &d->bufq);

	aoecmd_work(d);
	__skb_queue_head_init(&queue);
	skb_queue_splice_init(&d->sendq, &queue);

	spin_unlock_irqrestore(&d->lock, flags);
	aoenet_xmit(&queue);
=======
		spin_unlock_irq(&d->lock);
		blk_mq_start_request(bd->rq);
		return BLK_STS_IOERR;
	}

	list_add_tail(&bd->rq->queuelist, &d->rq_list);
	aoecmd_work(d);
	spin_unlock_irq(&d->lock);
	return BLK_STS_OK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
aoeblk_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	struct aoedev *d = bdev->bd_disk->private_data;

	if ((d->flags & DEVFL_UP) == 0) {
		printk(KERN_ERR "aoe: disk not up\n");
		return -ENODEV;
	}

	geo->cylinders = d->geo.cylinders;
	geo->heads = d->geo.heads;
	geo->sectors = d->geo.sectors;
	return 0;
}

<<<<<<< HEAD
static const struct block_device_operations aoe_bdops = {
	.open = aoeblk_open,
	.release = aoeblk_release,
=======
static int
aoeblk_ioctl(struct block_device *bdev, blk_mode_t mode, uint cmd, ulong arg)
{
	struct aoedev *d;

	if (!arg)
		return -EINVAL;

	d = bdev->bd_disk->private_data;
	if ((d->flags & DEVFL_UP) == 0) {
		pr_err("aoe: disk not up\n");
		return -ENODEV;
	}

	if (cmd == HDIO_GET_IDENTITY) {
		if (!copy_to_user((void __user *) arg, &d->ident,
			sizeof(d->ident)))
			return 0;
		return -EFAULT;
	}

	/* udev calls scsi_id, which uses SG_IO, resulting in noise */
	if (cmd != SG_IO)
		pr_info("aoe: unknown ioctl 0x%x\n", cmd);

	return -ENOTTY;
}

static const struct block_device_operations aoe_bdops = {
	.open = aoeblk_open,
	.release = aoeblk_release,
	.ioctl = aoeblk_ioctl,
	.compat_ioctl = blkdev_compat_ptr_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.getgeo = aoeblk_getgeo,
	.owner = THIS_MODULE,
};

<<<<<<< HEAD
/* alloc_disk and add_disk can sleep */
=======
static const struct blk_mq_ops aoeblk_mq_ops = {
	.queue_rq	= aoeblk_queue_rq,
};

/* blk_mq_alloc_disk and add_disk can sleep */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void
aoeblk_gdalloc(void *vp)
{
	struct aoedev *d = vp;
	struct gendisk *gd;
<<<<<<< HEAD
	ulong flags;

	gd = alloc_disk(AOE_PARTITIONS);
	if (gd == NULL) {
		printk(KERN_ERR
			"aoe: cannot allocate disk structure for %ld.%d\n",
=======
	mempool_t *mp;
	struct blk_mq_tag_set *set;
	sector_t ssize;
	struct queue_limits lim = {
		.max_hw_sectors		= aoe_maxsectors,
		.io_opt			= SZ_2M,
	};
	ulong flags;
	int late = 0;
	int err;

	spin_lock_irqsave(&d->lock, flags);
	if (d->flags & DEVFL_GDALLOC
	&& !(d->flags & DEVFL_TKILL)
	&& !(d->flags & DEVFL_GD_NOW))
		d->flags |= DEVFL_GD_NOW;
	else
		late = 1;
	spin_unlock_irqrestore(&d->lock, flags);
	if (late)
		return;

	mp = mempool_create(MIN_BUFS, mempool_alloc_slab, mempool_free_slab,
		buf_pool_cache);
	if (mp == NULL) {
		printk(KERN_ERR "aoe: cannot allocate bufpool for %ld.%d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			d->aoemajor, d->aoeminor);
		goto err;
	}

<<<<<<< HEAD
	d->bufpool = mempool_create_slab_pool(MIN_BUFS, buf_pool_cache);
	if (d->bufpool == NULL) {
		printk(KERN_ERR "aoe: cannot allocate bufpool for %ld.%d\n",
			d->aoemajor, d->aoeminor);
		goto err_disk;
	}

	d->blkq = blk_alloc_queue(GFP_KERNEL);
	if (!d->blkq)
		goto err_mempool;
	blk_queue_make_request(d->blkq, aoeblk_make_request);
	d->blkq->backing_dev_info.name = "aoe";
	spin_lock_irqsave(&d->lock, flags);
	gd->major = AOE_MAJOR;
	gd->first_minor = d->sysminor * AOE_PARTITIONS;
	gd->fops = &aoe_bdops;
	gd->private_data = d;
	set_capacity(gd, d->ssize);
	snprintf(gd->disk_name, sizeof gd->disk_name, "etherd/e%ld.%d",
		d->aoemajor, d->aoeminor);

	gd->queue = d->blkq;
	d->gd = gd;
=======
	set = &d->tag_set;
	set->ops = &aoeblk_mq_ops;
	set->cmd_size = sizeof(struct aoe_req);
	set->nr_hw_queues = 1;
	set->queue_depth = 128;
	set->numa_node = NUMA_NO_NODE;
	set->flags = BLK_MQ_F_SHOULD_MERGE;
	err = blk_mq_alloc_tag_set(set);
	if (err) {
		pr_err("aoe: cannot allocate tag set for %ld.%d\n",
			d->aoemajor, d->aoeminor);
		goto err_mempool;
	}

	gd = blk_mq_alloc_disk(set, &lim, d);
	if (IS_ERR(gd)) {
		pr_err("aoe: cannot allocate block queue for %ld.%d\n",
			d->aoemajor, d->aoeminor);
		goto err_tagset;
	}

	spin_lock_irqsave(&d->lock, flags);
	WARN_ON(!(d->flags & DEVFL_GD_NOW));
	WARN_ON(!(d->flags & DEVFL_GDALLOC));
	WARN_ON(d->flags & DEVFL_TKILL);
	WARN_ON(d->gd);
	WARN_ON(d->flags & DEVFL_UP);
	d->bufpool = mp;
	d->blkq = gd->queue;
	d->gd = gd;
	gd->major = AOE_MAJOR;
	gd->first_minor = d->sysminor;
	gd->minors = AOE_PARTITIONS;
	gd->fops = &aoe_bdops;
	gd->private_data = d;
	ssize = d->ssize;
	snprintf(gd->disk_name, sizeof gd->disk_name, "etherd/e%ld.%d",
		d->aoemajor, d->aoeminor);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	d->flags &= ~DEVFL_GDALLOC;
	d->flags |= DEVFL_UP;

	spin_unlock_irqrestore(&d->lock, flags);

<<<<<<< HEAD
	add_disk(gd);
	aoedisk_add_sysfs(d);
	return;

err_mempool:
	mempool_destroy(d->bufpool);
err_disk:
	put_disk(gd);
err:
	spin_lock_irqsave(&d->lock, flags);
	d->flags &= ~DEVFL_GDALLOC;
=======
	set_capacity(gd, ssize);

	err = device_add_disk(NULL, gd, aoe_attr_groups);
	if (err)
		goto out_disk_cleanup;
	aoedisk_add_debugfs(d);

	spin_lock_irqsave(&d->lock, flags);
	WARN_ON(!(d->flags & DEVFL_GD_NOW));
	d->flags &= ~DEVFL_GD_NOW;
	spin_unlock_irqrestore(&d->lock, flags);
	return;

out_disk_cleanup:
	put_disk(gd);
err_tagset:
	blk_mq_free_tag_set(set);
err_mempool:
	mempool_destroy(mp);
err:
	spin_lock_irqsave(&d->lock, flags);
	d->flags &= ~DEVFL_GD_NOW;
	queue_work(aoe_wq, &d->work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&d->lock, flags);
}

void
aoeblk_exit(void)
{
<<<<<<< HEAD
=======
	debugfs_remove_recursive(aoe_debugfs_dir);
	aoe_debugfs_dir = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kmem_cache_destroy(buf_pool_cache);
}

int __init
aoeblk_init(void)
{
	buf_pool_cache = kmem_cache_create("aoe_bufs",
					   sizeof(struct buf),
					   0, 0, NULL);
	if (buf_pool_cache == NULL)
		return -ENOMEM;
<<<<<<< HEAD

=======
	aoe_debugfs_dir = debugfs_create_dir("aoe", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

