/*
 * Copyright (C) 2009 Red Hat <bskeggs@redhat.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * Authors:
 *  Ben Skeggs <bskeggs@redhat.com>
 */

#include <linux/debugfs.h>
<<<<<<< HEAD

#include "drmP.h"
#include "nouveau_drv.h"

#include <ttm/ttm_page_alloc.h>

static int
nouveau_debugfs_channel_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct nouveau_channel *chan = node->info_ent->data;

	seq_printf(m, "channel id    : %d\n", chan->id);

	seq_printf(m, "cpu fifo state:\n");
	seq_printf(m, "          base: 0x%10llx\n", chan->pushbuf_base);
	seq_printf(m, "           max: 0x%08x\n", chan->dma.max << 2);
	seq_printf(m, "           cur: 0x%08x\n", chan->dma.cur << 2);
	seq_printf(m, "           put: 0x%08x\n", chan->dma.put << 2);
	seq_printf(m, "          free: 0x%08x\n", chan->dma.free << 2);
	if (chan->dma.ib_max) {
		seq_printf(m, "        ib max: 0x%08x\n", chan->dma.ib_max);
		seq_printf(m, "        ib put: 0x%08x\n", chan->dma.ib_put);
		seq_printf(m, "       ib free: 0x%08x\n", chan->dma.ib_free);
	}

	seq_printf(m, "gpu fifo state:\n");
	seq_printf(m, "           get: 0x%08x\n",
					nvchan_rd32(chan, chan->user_get));
	seq_printf(m, "           put: 0x%08x\n",
					nvchan_rd32(chan, chan->user_put));
	if (chan->dma.ib_max) {
		seq_printf(m, "        ib get: 0x%08x\n",
			   nvchan_rd32(chan, 0x88));
		seq_printf(m, "        ib put: 0x%08x\n",
			   nvchan_rd32(chan, 0x8c));
	}

	seq_printf(m, "last fence    : %d\n", chan->fence.sequence);
	seq_printf(m, "last signalled: %d\n", chan->fence.sequence_ack);
	return 0;
}

int
nouveau_debugfs_channel_init(struct nouveau_channel *chan)
{
	struct drm_nouveau_private *dev_priv = chan->dev->dev_private;
	struct drm_minor *minor = chan->dev->primary;
	int ret;

	if (!dev_priv->debugfs.channel_root) {
		dev_priv->debugfs.channel_root =
			debugfs_create_dir("channel", minor->debugfs_root);
		if (!dev_priv->debugfs.channel_root)
			return -ENOENT;
	}

	snprintf(chan->debugfs.name, 32, "%d", chan->id);
	chan->debugfs.info.name = chan->debugfs.name;
	chan->debugfs.info.show = nouveau_debugfs_channel_info;
	chan->debugfs.info.driver_features = 0;
	chan->debugfs.info.data = chan;

	ret = drm_debugfs_create_files(&chan->debugfs.info, 1,
				       dev_priv->debugfs.channel_root,
				       chan->dev->primary);
	if (ret == 0)
		chan->debugfs.active = true;
	return ret;
}

void
nouveau_debugfs_channel_fini(struct nouveau_channel *chan)
{
	struct drm_nouveau_private *dev_priv = chan->dev->dev_private;

	if (!chan->debugfs.active)
		return;

	drm_debugfs_remove_files(&chan->debugfs.info, 1, chan->dev->primary);
	chan->debugfs.active = false;

	if (chan == dev_priv->channel) {
		debugfs_remove(dev_priv->debugfs.channel_root);
		dev_priv->debugfs.channel_root = NULL;
	}
}

static int
nouveau_debugfs_chipset_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_minor *minor = node->minor;
	struct drm_device *dev = minor->dev;
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	uint32_t ppci_0;

	ppci_0 = nv_rd32(dev, dev_priv->chipset >= 0x40 ? 0x88000 : 0x1800);

	seq_printf(m, "PMC_BOOT_0: 0x%08x\n", nv_rd32(dev, NV03_PMC_BOOT_0));
	seq_printf(m, "PCI ID    : 0x%04x:0x%04x\n",
		   ppci_0 & 0xffff, ppci_0 >> 16);
	return 0;
}

static int
nouveau_debugfs_memory_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_minor *minor = node->minor;
	struct drm_nouveau_private *dev_priv = minor->dev->dev_private;

	seq_printf(m, "VRAM total: %dKiB\n", (int)(dev_priv->vram_size >> 10));
	return 0;
}

=======
#include <nvif/class.h>
#include <nvif/if0001.h>
#include "nouveau_debugfs.h"
#include "nouveau_drv.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int
nouveau_debugfs_vbios_image(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = node->minor->dev->dev_private;
	int i;

	for (i = 0; i < dev_priv->vbios.length; i++)
		seq_printf(m, "%c", dev_priv->vbios.data[i]);
=======
	struct nouveau_drm *drm = nouveau_drm(node->minor->dev);
	int i;

	for (i = 0; i < drm->vbios.length; i++)
		seq_printf(m, "%c", drm->vbios.data[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int
<<<<<<< HEAD
nouveau_debugfs_evict_vram(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_nouveau_private *dev_priv = node->minor->dev->dev_private;
	int ret;

	ret = ttm_bo_evict_mm(&dev_priv->ttm.bdev, TTM_PL_VRAM);
	if (ret)
		seq_printf(m, "failed: %d", ret);
	else
		seq_printf(m, "succeeded\n");
	return 0;
}

static struct drm_info_list nouveau_debugfs_list[] = {
	{ "evict_vram", nouveau_debugfs_evict_vram, 0, NULL },
	{ "chipset", nouveau_debugfs_chipset_info, 0, NULL },
	{ "memory", nouveau_debugfs_memory_info, 0, NULL },
	{ "vbios.rom", nouveau_debugfs_vbios_image, 0, NULL },
	{ "ttm_page_pool", ttm_page_alloc_debugfs, 0, NULL },
	{ "ttm_dma_page_pool", ttm_dma_page_alloc_debugfs, 0, NULL },
};
#define NOUVEAU_DEBUGFS_ENTRIES ARRAY_SIZE(nouveau_debugfs_list)

int
nouveau_debugfs_init(struct drm_minor *minor)
{
	drm_debugfs_create_files(nouveau_debugfs_list, NOUVEAU_DEBUGFS_ENTRIES,
				 minor->debugfs_root, minor);
	return 0;
}

void
nouveau_debugfs_takedown(struct drm_minor *minor)
{
	drm_debugfs_remove_files(nouveau_debugfs_list, NOUVEAU_DEBUGFS_ENTRIES,
				 minor);
=======
nouveau_debugfs_strap_peek(struct seq_file *m, void *data)
{
	struct drm_info_node *node = m->private;
	struct nouveau_drm *drm = nouveau_drm(node->minor->dev);
	int ret;

	ret = pm_runtime_get_sync(drm->dev->dev);
	if (ret < 0 && ret != -EACCES) {
		pm_runtime_put_autosuspend(drm->dev->dev);
		return ret;
	}

	seq_printf(m, "0x%08x\n",
		   nvif_rd32(&drm->client.device.object, 0x101000));

	pm_runtime_mark_last_busy(drm->dev->dev);
	pm_runtime_put_autosuspend(drm->dev->dev);

	return 0;
}

static int
nouveau_debugfs_pstate_get(struct seq_file *m, void *data)
{
	struct drm_device *drm = m->private;
	struct nouveau_debugfs *debugfs = nouveau_debugfs(drm);
	struct nvif_object *ctrl;
	struct nvif_control_pstate_info_v0 info = {};
	int ret, i;

	if (!debugfs)
		return -ENODEV;

	ctrl = &debugfs->ctrl;
	ret = nvif_mthd(ctrl, NVIF_CONTROL_PSTATE_INFO, &info, sizeof(info));
	if (ret)
		return ret;

	for (i = 0; i < info.count + 1; i++) {
		const s32 state = i < info.count ? i :
			NVIF_CONTROL_PSTATE_ATTR_V0_STATE_CURRENT;
		struct nvif_control_pstate_attr_v0 attr = {
			.state = state,
			.index = 0,
		};

		ret = nvif_mthd(ctrl, NVIF_CONTROL_PSTATE_ATTR,
				&attr, sizeof(attr));
		if (ret)
			return ret;

		if (i < info.count)
			seq_printf(m, "%02x:", attr.state);
		else
			seq_printf(m, "%s:", info.pwrsrc == 0 ? "DC" :
					     info.pwrsrc == 1 ? "AC" : "--");

		attr.index = 0;
		do {
			attr.state = state;
			ret = nvif_mthd(ctrl, NVIF_CONTROL_PSTATE_ATTR,
					&attr, sizeof(attr));
			if (ret)
				return ret;

			seq_printf(m, " %s %d", attr.name, attr.min);
			if (attr.min != attr.max)
				seq_printf(m, "-%d", attr.max);
			seq_printf(m, " %s", attr.unit);
		} while (attr.index);

		if (state >= 0) {
			if (info.ustate_ac == state)
				seq_puts(m, " AC");
			if (info.ustate_dc == state)
				seq_puts(m, " DC");
			if (info.pstate == state)
				seq_puts(m, " *");
		} else {
			if (info.ustate_ac < -1)
				seq_puts(m, " AC");
			if (info.ustate_dc < -1)
				seq_puts(m, " DC");
		}

		seq_putc(m, '\n');
	}

	return 0;
}

static ssize_t
nouveau_debugfs_pstate_set(struct file *file, const char __user *ubuf,
			   size_t len, loff_t *offp)
{
	struct seq_file *m = file->private_data;
	struct drm_device *drm = m->private;
	struct nouveau_debugfs *debugfs = nouveau_debugfs(drm);
	struct nvif_control_pstate_user_v0 args = { .pwrsrc = -EINVAL };
	char buf[32] = {}, *tmp, *cur = buf;
	long value, ret;

	if (!debugfs)
		return -ENODEV;

	if (len >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, len))
		return -EFAULT;

	if ((tmp = strchr(buf, '\n')))
		*tmp = '\0';

	if (!strncasecmp(cur, "dc:", 3)) {
		args.pwrsrc = 0;
		cur += 3;
	} else
	if (!strncasecmp(cur, "ac:", 3)) {
		args.pwrsrc = 1;
		cur += 3;
	}

	if (!strcasecmp(cur, "none"))
		args.ustate = NVIF_CONTROL_PSTATE_USER_V0_STATE_UNKNOWN;
	else
	if (!strcasecmp(cur, "auto"))
		args.ustate = NVIF_CONTROL_PSTATE_USER_V0_STATE_PERFMON;
	else {
		ret = kstrtol(cur, 16, &value);
		if (ret)
			return ret;
		args.ustate = value;
	}

	ret = pm_runtime_get_sync(drm->dev);
	if (ret < 0 && ret != -EACCES) {
		pm_runtime_put_autosuspend(drm->dev);
		return ret;
	}

	ret = nvif_mthd(&debugfs->ctrl, NVIF_CONTROL_PSTATE_USER,
			&args, sizeof(args));
	pm_runtime_put_autosuspend(drm->dev);
	if (ret < 0)
		return ret;

	return len;
}

static int
nouveau_debugfs_pstate_open(struct inode *inode, struct file *file)
{
	return single_open(file, nouveau_debugfs_pstate_get, inode->i_private);
}

static void
nouveau_debugfs_gpuva_regions(struct seq_file *m, struct nouveau_uvmm *uvmm)
{
	MA_STATE(mas, &uvmm->region_mt, 0, 0);
	struct nouveau_uvma_region *reg;

	seq_puts  (m, " VA regions  | start              | range              | end                \n");
	seq_puts  (m, "----------------------------------------------------------------------------\n");
	mas_for_each(&mas, reg, ULONG_MAX)
		seq_printf(m, "             | 0x%016llx | 0x%016llx | 0x%016llx\n",
			   reg->va.addr, reg->va.range, reg->va.addr + reg->va.range);
}

static int
nouveau_debugfs_gpuva(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct nouveau_drm *drm = nouveau_drm(node->minor->dev);
	struct nouveau_cli *cli;

	mutex_lock(&drm->clients_lock);
	list_for_each_entry(cli, &drm->clients, head) {
		struct nouveau_uvmm *uvmm = nouveau_cli_uvmm(cli);

		if (!uvmm)
			continue;

		nouveau_uvmm_lock(uvmm);
		drm_debugfs_gpuva_info(m, &uvmm->base);
		seq_puts(m, "\n");
		nouveau_debugfs_gpuva_regions(m, uvmm);
		nouveau_uvmm_unlock(uvmm);
	}
	mutex_unlock(&drm->clients_lock);

	return 0;
}

static const struct file_operations nouveau_pstate_fops = {
	.owner = THIS_MODULE,
	.open = nouveau_debugfs_pstate_open,
	.read = seq_read,
	.write = nouveau_debugfs_pstate_set,
	.release = single_release,
};

static struct drm_info_list nouveau_debugfs_list[] = {
	{ "vbios.rom",  nouveau_debugfs_vbios_image, 0, NULL },
	{ "strap_peek", nouveau_debugfs_strap_peek, 0, NULL },
	DRM_DEBUGFS_GPUVA_INFO(nouveau_debugfs_gpuva, NULL),
};
#define NOUVEAU_DEBUGFS_ENTRIES ARRAY_SIZE(nouveau_debugfs_list)

static const struct nouveau_debugfs_files {
	const char *name;
	const struct file_operations *fops;
} nouveau_debugfs_files[] = {
	{"pstate", &nouveau_pstate_fops},
};

void
nouveau_drm_debugfs_init(struct drm_minor *minor)
{
	struct nouveau_drm *drm = nouveau_drm(minor->dev);
	struct dentry *dentry;
	int i;

	for (i = 0; i < ARRAY_SIZE(nouveau_debugfs_files); i++) {
		debugfs_create_file(nouveau_debugfs_files[i].name,
				    S_IRUGO | S_IWUSR,
				    minor->debugfs_root, minor->dev,
				    nouveau_debugfs_files[i].fops);
	}

	drm_debugfs_create_files(nouveau_debugfs_list,
				 NOUVEAU_DEBUGFS_ENTRIES,
				 minor->debugfs_root, minor);

	/* Set the size of the vbios since we know it, and it's confusing to
	 * userspace if it wants to seek() but the file has a length of 0
	 */
	dentry = debugfs_lookup("vbios.rom", minor->debugfs_root);
	if (!dentry)
		return;

	d_inode(dentry)->i_size = drm->vbios.length;
	dput(dentry);
}

int
nouveau_debugfs_init(struct nouveau_drm *drm)
{
	drm->debugfs = kzalloc(sizeof(*drm->debugfs), GFP_KERNEL);
	if (!drm->debugfs)
		return -ENOMEM;

	return nvif_object_ctor(&drm->client.device.object, "debugfsCtrl", 0,
				NVIF_CLASS_CONTROL, NULL, 0,
				&drm->debugfs->ctrl);
}

void
nouveau_debugfs_fini(struct nouveau_drm *drm)
{
	if (drm->debugfs && drm->debugfs->ctrl.priv)
		nvif_object_dtor(&drm->debugfs->ctrl);

	kfree(drm->debugfs);
	drm->debugfs = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
