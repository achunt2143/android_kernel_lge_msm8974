<<<<<<< HEAD
/*
 * Copyright (c) International Business Machines Corp., 2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;  either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) International Business Machines Corp., 2006
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: Artem Bityutskiy (Битюцкий Артём)
 */

/*
 * This file contains implementation of volume creation, deletion, updating and
 * resizing.
 */

#include <linux/err.h>
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/export.h>
#include "ubi.h"

<<<<<<< HEAD
#ifdef CONFIG_MTD_UBI_DEBUG
static int paranoid_check_volumes(struct ubi_device *ubi);
#else
#define paranoid_check_volumes(ubi) 0
#endif
=======
static int self_check_volumes(struct ubi_device *ubi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t vol_attribute_show(struct device *dev,
				  struct device_attribute *attr, char *buf);

/* Device attributes corresponding to files in '/<sysfs>/class/ubi/ubiX_Y' */
static struct device_attribute attr_vol_reserved_ebs =
	__ATTR(reserved_ebs, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_type =
	__ATTR(type, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_name =
	__ATTR(name, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_corrupted =
	__ATTR(corrupted, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_alignment =
	__ATTR(alignment, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_usable_eb_size =
	__ATTR(usable_eb_size, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_data_bytes =
	__ATTR(data_bytes, S_IRUGO, vol_attribute_show, NULL);
static struct device_attribute attr_vol_upd_marker =
	__ATTR(upd_marker, S_IRUGO, vol_attribute_show, NULL);

/*
 * "Show" method for files in '/<sysfs>/class/ubi/ubiX_Y/'.
 *
 * Consider a situation:
 * A. process 1 opens a sysfs file related to volume Y, say
 *    /<sysfs>/class/ubi/ubiX_Y/reserved_ebs;
 * B. process 2 removes volume Y;
 * C. process 1 starts reading the /<sysfs>/class/ubi/ubiX_Y/reserved_ebs file;
 *
 * In this situation, this function will return %-ENODEV because it will find
 * out that the volume was removed from the @ubi->volumes array.
 */
static ssize_t vol_attribute_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	int ret;
	struct ubi_volume *vol = container_of(dev, struct ubi_volume, dev);
<<<<<<< HEAD
	struct ubi_device *ubi;

	ubi = ubi_get_device(vol->ubi->ubi_num);
	if (!ubi)
		return -ENODEV;

	spin_lock(&ubi->volumes_lock);
	if (!ubi->volumes[vol->vol_id]) {
		spin_unlock(&ubi->volumes_lock);
		ubi_put_device(ubi);
=======
	struct ubi_device *ubi = vol->ubi;

	spin_lock(&ubi->volumes_lock);
	if (!ubi->volumes[vol->vol_id] || ubi->volumes[vol->vol_id]->is_dead) {
		spin_unlock(&ubi->volumes_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	}
	/* Take a reference to prevent volume removal */
	vol->ref_count += 1;
	spin_unlock(&ubi->volumes_lock);

	if (attr == &attr_vol_reserved_ebs)
		ret = sprintf(buf, "%d\n", vol->reserved_pebs);
	else if (attr == &attr_vol_type) {
		const char *tp;

		if (vol->vol_type == UBI_DYNAMIC_VOLUME)
			tp = "dynamic";
		else
			tp = "static";
		ret = sprintf(buf, "%s\n", tp);
	} else if (attr == &attr_vol_name)
		ret = sprintf(buf, "%s\n", vol->name);
	else if (attr == &attr_vol_corrupted)
		ret = sprintf(buf, "%d\n", vol->corrupted);
	else if (attr == &attr_vol_alignment)
		ret = sprintf(buf, "%d\n", vol->alignment);
	else if (attr == &attr_vol_usable_eb_size)
		ret = sprintf(buf, "%d\n", vol->usable_leb_size);
	else if (attr == &attr_vol_data_bytes)
		ret = sprintf(buf, "%lld\n", vol->used_bytes);
	else if (attr == &attr_vol_upd_marker)
		ret = sprintf(buf, "%d\n", vol->upd_marker);
	else
		/* This must be a bug */
		ret = -EINVAL;

	/* We've done the operation, drop volume and UBI device references */
	spin_lock(&ubi->volumes_lock);
	vol->ref_count -= 1;
	ubi_assert(vol->ref_count >= 0);
	spin_unlock(&ubi->volumes_lock);
<<<<<<< HEAD
	ubi_put_device(ubi);
	return ret;
}

=======
	return ret;
}

static struct attribute *volume_dev_attrs[] = {
	&attr_vol_reserved_ebs.attr,
	&attr_vol_type.attr,
	&attr_vol_name.attr,
	&attr_vol_corrupted.attr,
	&attr_vol_alignment.attr,
	&attr_vol_usable_eb_size.attr,
	&attr_vol_data_bytes.attr,
	&attr_vol_upd_marker.attr,
	NULL
};
ATTRIBUTE_GROUPS(volume_dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Release method for volume devices */
static void vol_release(struct device *dev)
{
	struct ubi_volume *vol = container_of(dev, struct ubi_volume, dev);

<<<<<<< HEAD
	kfree(vol->eba_tbl);
	kfree(vol);
}

/**
 * volume_sysfs_init - initialize sysfs for new volume.
 * @ubi: UBI device description object
 * @vol: volume description object
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 *
 * Note, this function does not free allocated resources in case of failure -
 * the caller does it. This is because this would cause release() here and the
 * caller would oops.
 */
static int volume_sysfs_init(struct ubi_device *ubi, struct ubi_volume *vol)
{
	int err;

	err = device_create_file(&vol->dev, &attr_vol_reserved_ebs);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_type);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_name);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_corrupted);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_alignment);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_usable_eb_size);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_data_bytes);
	if (err)
		return err;
	err = device_create_file(&vol->dev, &attr_vol_upd_marker);
	return err;
}

/**
 * volume_sysfs_close - close sysfs for a volume.
 * @vol: volume description object
 */
static void volume_sysfs_close(struct ubi_volume *vol)
{
	device_remove_file(&vol->dev, &attr_vol_upd_marker);
	device_remove_file(&vol->dev, &attr_vol_data_bytes);
	device_remove_file(&vol->dev, &attr_vol_usable_eb_size);
	device_remove_file(&vol->dev, &attr_vol_alignment);
	device_remove_file(&vol->dev, &attr_vol_corrupted);
	device_remove_file(&vol->dev, &attr_vol_name);
	device_remove_file(&vol->dev, &attr_vol_type);
	device_remove_file(&vol->dev, &attr_vol_reserved_ebs);
	device_unregister(&vol->dev);
=======
	ubi_eba_replace_table(vol, NULL);
	ubi_fastmap_destroy_checkmap(vol);
	kfree(vol);
}

static struct fwnode_handle *find_volume_fwnode(struct ubi_volume *vol)
{
	struct fwnode_handle *fw_vols, *fw_vol;
	const char *volname;
	u32 volid;

	fw_vols = device_get_named_child_node(vol->dev.parent->parent, "volumes");
	if (!fw_vols)
		return NULL;

	fwnode_for_each_child_node(fw_vols, fw_vol) {
		if (!fwnode_property_read_string(fw_vol, "volname", &volname) &&
		    strncmp(volname, vol->name, vol->name_len))
			continue;

		if (!fwnode_property_read_u32(fw_vol, "volid", &volid) &&
		    vol->vol_id != volid)
			continue;

		return fw_vol;
	}

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ubi_create_volume - create volume.
 * @ubi: UBI device description object
 * @req: volume creation request
 *
 * This function creates volume described by @req. If @req->vol_id id
 * %UBI_VOL_NUM_AUTO, this function automatically assign ID to the new volume
 * and saves it in @req->vol_id. Returns zero in case of success and a negative
 * error code in case of failure. Note, the caller has to have the
 * @ubi->device_mutex locked.
 */
int ubi_create_volume(struct ubi_device *ubi, struct ubi_mkvol_req *req)
{
<<<<<<< HEAD
	int i, err, vol_id = req->vol_id, do_free = 1;
	struct ubi_volume *vol;
	struct ubi_vtbl_record vtbl_rec;
	dev_t dev;
=======
	int i, err, vol_id = req->vol_id;
	struct ubi_volume *vol;
	struct ubi_vtbl_record vtbl_rec;
	struct ubi_eba_table *eba_tbl = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ubi->ro_mode)
		return -EROFS;

	vol = kzalloc(sizeof(struct ubi_volume), GFP_KERNEL);
	if (!vol)
		return -ENOMEM;

<<<<<<< HEAD
=======
	device_initialize(&vol->dev);
	vol->dev.release = vol_release;
	vol->dev.parent = &ubi->dev;
	vol->dev.class = &ubi_class;
	vol->dev.groups = volume_dev_groups;

	if (req->flags & UBI_VOL_SKIP_CRC_CHECK_FLG)
		vol->skip_check = 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&ubi->volumes_lock);
	if (vol_id == UBI_VOL_NUM_AUTO) {
		/* Find unused volume ID */
		dbg_gen("search for vacant volume ID");
		for (i = 0; i < ubi->vtbl_slots; i++)
			if (!ubi->volumes[i]) {
				vol_id = i;
				break;
			}

		if (vol_id == UBI_VOL_NUM_AUTO) {
<<<<<<< HEAD
			dbg_err("out of volume IDs");
=======
			ubi_err(ubi, "out of volume IDs");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENFILE;
			goto out_unlock;
		}
		req->vol_id = vol_id;
	}

	dbg_gen("create device %d, volume %d, %llu bytes, type %d, name %s",
		ubi->ubi_num, vol_id, (unsigned long long)req->bytes,
		(int)req->vol_type, req->name);

	/* Ensure that this volume does not exist */
	err = -EEXIST;
	if (ubi->volumes[vol_id]) {
<<<<<<< HEAD
		dbg_err("volume %d already exists", vol_id);
=======
		ubi_err(ubi, "volume %d already exists", vol_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_unlock;
	}

	/* Ensure that the name is unique */
	for (i = 0; i < ubi->vtbl_slots; i++)
<<<<<<< HEAD
		if (ubi->volumes[i] &&
		    ubi->volumes[i]->name_len == req->name_len &&
		    !strcmp(ubi->volumes[i]->name, req->name)) {
			dbg_err("volume \"%s\" exists (ID %d)", req->name, i);
=======
		if (ubi->volumes[i] && !ubi->volumes[i]->is_dead &&
		    ubi->volumes[i]->name_len == req->name_len &&
		    !strcmp(ubi->volumes[i]->name, req->name)) {
			ubi_err(ubi, "volume \"%s\" exists (ID %d)",
				req->name, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_unlock;
		}

	/* Calculate how many eraseblocks are requested */
	vol->usable_leb_size = ubi->leb_size - ubi->leb_size % req->alignment;
<<<<<<< HEAD
	vol->reserved_pebs += div_u64(req->bytes + vol->usable_leb_size - 1,
				      vol->usable_leb_size);

	/* Reserve physical eraseblocks */
	if (vol->reserved_pebs > ubi->avail_pebs) {
		dbg_err("not enough PEBs, only %d available", ubi->avail_pebs);
		if (ubi->corr_peb_count)
			dbg_err("%d PEBs are corrupted and not used",
=======
	vol->reserved_pebs = div_u64(req->bytes + vol->usable_leb_size - 1,
				     vol->usable_leb_size);

	/* Reserve physical eraseblocks */
	if (vol->reserved_pebs > ubi->avail_pebs) {
		ubi_err(ubi, "not enough PEBs, only %d available",
			ubi->avail_pebs);
		if (ubi->corr_peb_count)
			ubi_err(ubi, "%d PEBs are corrupted and not used",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				ubi->corr_peb_count);
		err = -ENOSPC;
		goto out_unlock;
	}
	ubi->avail_pebs -= vol->reserved_pebs;
	ubi->rsvd_pebs += vol->reserved_pebs;
	spin_unlock(&ubi->volumes_lock);

	vol->vol_id    = vol_id;
	vol->alignment = req->alignment;
	vol->data_pad  = ubi->leb_size % vol->alignment;
	vol->vol_type  = req->vol_type;
	vol->name_len  = req->name_len;
	memcpy(vol->name, req->name, vol->name_len);
	vol->ubi = ubi;
<<<<<<< HEAD
=======
	device_set_node(&vol->dev, find_volume_fwnode(vol));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Finish all pending erases because there may be some LEBs belonging
	 * to the same volume ID.
	 */
<<<<<<< HEAD
	err = ubi_wl_flush(ubi);
	if (err)
		goto out_acc;

	vol->eba_tbl = kmalloc(vol->reserved_pebs * sizeof(int), GFP_KERNEL);
	if (!vol->eba_tbl) {
		err = -ENOMEM;
		goto out_acc;
	}

	for (i = 0; i < vol->reserved_pebs; i++)
		vol->eba_tbl[i] = UBI_LEB_UNMAPPED;
=======
	err = ubi_wl_flush(ubi, vol_id, UBI_ALL);
	if (err)
		goto out_acc;

	eba_tbl = ubi_eba_create_table(vol, vol->reserved_pebs);
	if (IS_ERR(eba_tbl)) {
		err = PTR_ERR(eba_tbl);
		goto out_acc;
	}

	ubi_eba_replace_table(vol, eba_tbl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (vol->vol_type == UBI_DYNAMIC_VOLUME) {
		vol->used_ebs = vol->reserved_pebs;
		vol->last_eb_bytes = vol->usable_leb_size;
		vol->used_bytes =
			(long long)vol->used_ebs * vol->usable_leb_size;
	} else {
		vol->used_ebs = div_u64_rem(vol->used_bytes,
					    vol->usable_leb_size,
					    &vol->last_eb_bytes);
		if (vol->last_eb_bytes != 0)
			vol->used_ebs += 1;
		else
			vol->last_eb_bytes = vol->usable_leb_size;
	}

<<<<<<< HEAD
	/* Register character device for the volume */
	cdev_init(&vol->cdev, &ubi_vol_cdev_operations);
	vol->cdev.owner = THIS_MODULE;
	dev = MKDEV(MAJOR(ubi->cdev.dev), vol_id + 1);
	err = cdev_add(&vol->cdev, dev, 1);
	if (err) {
		ubi_err("cannot add character device");
		goto out_mapping;
	}

	vol->dev.release = vol_release;
	vol->dev.parent = &ubi->dev;
	vol->dev.devt = dev;
	vol->dev.class = ubi_class;

	dev_set_name(&vol->dev, "%s_%d", ubi->ubi_name, vol->vol_id);
	err = device_register(&vol->dev);
	if (err) {
		ubi_err("cannot register device");
		goto out_cdev;
	}

	err = volume_sysfs_init(ubi, vol);
	if (err)
		goto out_sysfs;

=======
	/* Make volume "available" before it becomes accessible via sysfs */
	spin_lock(&ubi->volumes_lock);
	ubi->volumes[vol_id] = vol;
	ubi->vol_count += 1;
	spin_unlock(&ubi->volumes_lock);

	/* Register character device for the volume */
	cdev_init(&vol->cdev, &ubi_vol_cdev_operations);
	vol->cdev.owner = THIS_MODULE;

	vol->dev.devt = MKDEV(MAJOR(ubi->cdev.dev), vol_id + 1);
	dev_set_name(&vol->dev, "%s_%d", ubi->ubi_name, vol->vol_id);
	err = cdev_device_add(&vol->cdev, &vol->dev);
	if (err) {
		ubi_err(ubi, "cannot add device");
		goto out_mapping;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Fill volume table record */
	memset(&vtbl_rec, 0, sizeof(struct ubi_vtbl_record));
	vtbl_rec.reserved_pebs = cpu_to_be32(vol->reserved_pebs);
	vtbl_rec.alignment     = cpu_to_be32(vol->alignment);
	vtbl_rec.data_pad      = cpu_to_be32(vol->data_pad);
	vtbl_rec.name_len      = cpu_to_be16(vol->name_len);
	if (vol->vol_type == UBI_DYNAMIC_VOLUME)
		vtbl_rec.vol_type = UBI_VID_DYNAMIC;
	else
		vtbl_rec.vol_type = UBI_VID_STATIC;
<<<<<<< HEAD
=======

	if (vol->skip_check)
		vtbl_rec.flags |= UBI_VTBL_SKIP_CRC_CHECK_FLG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(vtbl_rec.name, vol->name, vol->name_len);

	err = ubi_change_vtbl_record(ubi, vol_id, &vtbl_rec);
	if (err)
		goto out_sysfs;

<<<<<<< HEAD
	spin_lock(&ubi->volumes_lock);
	ubi->volumes[vol_id] = vol;
	ubi->vol_count += 1;
	spin_unlock(&ubi->volumes_lock);

	ubi_volume_notify(ubi, vol, UBI_VOLUME_ADDED);
	if (paranoid_check_volumes(ubi))
		dbg_err("check failed while creating volume %d", vol_id);
=======
	ubi_volume_notify(ubi, vol, UBI_VOLUME_ADDED);
	self_check_volumes(ubi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;

out_sysfs:
	/*
	 * We have registered our device, we should not free the volume
	 * description object in this function in case of an error - it is
	 * freed by the release function.
<<<<<<< HEAD
	 *
	 * Get device reference to prevent the release function from being
	 * called just after sysfs has been closed.
	 */
	do_free = 0;
	get_device(&vol->dev);
	volume_sysfs_close(vol);
out_cdev:
	cdev_del(&vol->cdev);
out_mapping:
	if (do_free)
		kfree(vol->eba_tbl);
=======
	 */
	cdev_device_del(&vol->cdev, &vol->dev);
out_mapping:
	spin_lock(&ubi->volumes_lock);
	ubi->volumes[vol_id] = NULL;
	ubi->vol_count -= 1;
	spin_unlock(&ubi->volumes_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_acc:
	spin_lock(&ubi->volumes_lock);
	ubi->rsvd_pebs -= vol->reserved_pebs;
	ubi->avail_pebs += vol->reserved_pebs;
out_unlock:
	spin_unlock(&ubi->volumes_lock);
<<<<<<< HEAD
	if (do_free)
		kfree(vol);
	else
		put_device(&vol->dev);
	ubi_err("cannot create volume %d, error %d", vol_id, err);
=======
	put_device(&vol->dev);
	ubi_err(ubi, "cannot create volume %d, error %d", vol_id, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ubi_remove_volume - remove volume.
 * @desc: volume descriptor
 * @no_vtbl: do not change volume table if not zero
 *
 * This function removes volume described by @desc. The volume has to be opened
 * in "exclusive" mode. Returns zero in case of success and a negative error
 * code in case of failure. The caller has to have the @ubi->device_mutex
 * locked.
 */
int ubi_remove_volume(struct ubi_volume_desc *desc, int no_vtbl)
{
	struct ubi_volume *vol = desc->vol;
	struct ubi_device *ubi = vol->ubi;
	int i, err, vol_id = vol->vol_id, reserved_pebs = vol->reserved_pebs;

	dbg_gen("remove device %d, volume %d", ubi->ubi_num, vol_id);
	ubi_assert(desc->mode == UBI_EXCLUSIVE);
	ubi_assert(vol == ubi->volumes[vol_id]);

	if (ubi->ro_mode)
		return -EROFS;

	spin_lock(&ubi->volumes_lock);
	if (vol->ref_count > 1) {
		/*
		 * The volume is busy, probably someone is reading one of its
		 * sysfs files.
		 */
		err = -EBUSY;
		goto out_unlock;
	}
<<<<<<< HEAD
=======

	/*
	 * Mark volume as dead at this point to prevent that anyone
	 * can take a reference to the volume from now on.
	 * This is necessary as we have to release the spinlock before
	 * calling ubi_volume_notify.
	 */
	vol->is_dead = true;
	spin_unlock(&ubi->volumes_lock);

	ubi_volume_notify(ubi, vol, UBI_VOLUME_SHUTDOWN);

	spin_lock(&ubi->volumes_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi->volumes[vol_id] = NULL;
	spin_unlock(&ubi->volumes_lock);

	if (!no_vtbl) {
		err = ubi_change_vtbl_record(ubi, vol_id, NULL);
		if (err)
			goto out_err;
	}

	for (i = 0; i < vol->reserved_pebs; i++) {
		err = ubi_eba_unmap_leb(ubi, vol, i);
		if (err)
			goto out_err;
	}

<<<<<<< HEAD
	cdev_del(&vol->cdev);
	volume_sysfs_close(vol);
=======
	cdev_device_del(&vol->cdev, &vol->dev);
	put_device(&vol->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&ubi->volumes_lock);
	ubi->rsvd_pebs -= reserved_pebs;
	ubi->avail_pebs += reserved_pebs;
<<<<<<< HEAD
	i = ubi->beb_rsvd_level - ubi->beb_rsvd_pebs;
	if (i > 0) {
		i = ubi->avail_pebs >= i ? i : ubi->avail_pebs;
		ubi->avail_pebs -= i;
		ubi->rsvd_pebs += i;
		ubi->beb_rsvd_pebs += i;
		if (i > 0)
			ubi_msg("reserve more %d PEBs", i);
	}
=======
	ubi_update_reserved(ubi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubi->vol_count -= 1;
	spin_unlock(&ubi->volumes_lock);

	ubi_volume_notify(ubi, vol, UBI_VOLUME_REMOVED);
<<<<<<< HEAD
	if (!no_vtbl && paranoid_check_volumes(ubi))
		dbg_err("check failed while removing volume %d", vol_id);

	return err;

out_err:
	ubi_err("cannot remove volume %d, error %d", vol_id, err);
=======
	if (!no_vtbl)
		self_check_volumes(ubi);

	return 0;

out_err:
	ubi_err(ubi, "cannot remove volume %d, error %d", vol_id, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&ubi->volumes_lock);
	ubi->volumes[vol_id] = vol;
out_unlock:
	spin_unlock(&ubi->volumes_lock);
	return err;
}

/**
 * ubi_resize_volume - re-size volume.
 * @desc: volume descriptor
 * @reserved_pebs: new size in physical eraseblocks
 *
 * This function re-sizes the volume and returns zero in case of success, and a
 * negative error code in case of failure. The caller has to have the
 * @ubi->device_mutex locked.
 */
int ubi_resize_volume(struct ubi_volume_desc *desc, int reserved_pebs)
{
<<<<<<< HEAD
	int i, err, pebs, *new_mapping;
	struct ubi_volume *vol = desc->vol;
	struct ubi_device *ubi = vol->ubi;
	struct ubi_vtbl_record vtbl_rec;
=======
	int i, err, pebs;
	struct ubi_volume *vol = desc->vol;
	struct ubi_device *ubi = vol->ubi;
	struct ubi_vtbl_record vtbl_rec;
	struct ubi_eba_table *new_eba_tbl = NULL;
	struct ubi_eba_table *old_eba_tbl = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int vol_id = vol->vol_id;

	if (ubi->ro_mode)
		return -EROFS;

	dbg_gen("re-size device %d, volume %d to from %d to %d PEBs",
		ubi->ubi_num, vol_id, vol->reserved_pebs, reserved_pebs);

	if (vol->vol_type == UBI_STATIC_VOLUME &&
	    reserved_pebs < vol->used_ebs) {
<<<<<<< HEAD
		dbg_err("too small size %d, %d LEBs contain data",
=======
		ubi_err(ubi, "too small size %d, %d LEBs contain data",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			reserved_pebs, vol->used_ebs);
		return -EINVAL;
	}

	/* If the size is the same, we have nothing to do */
	if (reserved_pebs == vol->reserved_pebs)
		return 0;

<<<<<<< HEAD
	new_mapping = kmalloc(reserved_pebs * sizeof(int), GFP_KERNEL);
	if (!new_mapping)
		return -ENOMEM;

	for (i = 0; i < reserved_pebs; i++)
		new_mapping[i] = UBI_LEB_UNMAPPED;
=======
	new_eba_tbl = ubi_eba_create_table(vol, reserved_pebs);
	if (IS_ERR(new_eba_tbl))
		return PTR_ERR(new_eba_tbl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&ubi->volumes_lock);
	if (vol->ref_count > 1) {
		spin_unlock(&ubi->volumes_lock);
		err = -EBUSY;
		goto out_free;
	}
	spin_unlock(&ubi->volumes_lock);

	/* Reserve physical eraseblocks */
	pebs = reserved_pebs - vol->reserved_pebs;
	if (pebs > 0) {
		spin_lock(&ubi->volumes_lock);
		if (pebs > ubi->avail_pebs) {
<<<<<<< HEAD
			dbg_err("not enough PEBs: requested %d, available %d",
				pebs, ubi->avail_pebs);
			if (ubi->corr_peb_count)
				dbg_err("%d PEBs are corrupted and not used",
=======
			ubi_err(ubi, "not enough PEBs: requested %d, available %d",
				pebs, ubi->avail_pebs);
			if (ubi->corr_peb_count)
				ubi_err(ubi, "%d PEBs are corrupted and not used",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					ubi->corr_peb_count);
			spin_unlock(&ubi->volumes_lock);
			err = -ENOSPC;
			goto out_free;
		}
<<<<<<< HEAD
		ubi->avail_pebs -= pebs;
		ubi->rsvd_pebs += pebs;
		for (i = 0; i < vol->reserved_pebs; i++)
			new_mapping[i] = vol->eba_tbl[i];
		kfree(vol->eba_tbl);
		vol->eba_tbl = new_mapping;
		spin_unlock(&ubi->volumes_lock);
	}

	/* Change volume table record */
	memcpy(&vtbl_rec, &ubi->vtbl[vol_id], sizeof(struct ubi_vtbl_record));
	vtbl_rec.reserved_pebs = cpu_to_be32(reserved_pebs);
	err = ubi_change_vtbl_record(ubi, vol_id, &vtbl_rec);
	if (err)
		goto out_acc;

=======

		ubi->avail_pebs -= pebs;
		ubi->rsvd_pebs += pebs;
		ubi_eba_copy_table(vol, new_eba_tbl, vol->reserved_pebs);
		old_eba_tbl = vol->eba_tbl;
		vol->eba_tbl = new_eba_tbl;
		vol->reserved_pebs = reserved_pebs;
		spin_unlock(&ubi->volumes_lock);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pebs < 0) {
		for (i = 0; i < -pebs; i++) {
			err = ubi_eba_unmap_leb(ubi, vol, reserved_pebs + i);
			if (err)
<<<<<<< HEAD
				goto out_acc;
=======
				goto out_free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		spin_lock(&ubi->volumes_lock);
		ubi->rsvd_pebs += pebs;
		ubi->avail_pebs -= pebs;
<<<<<<< HEAD
		pebs = ubi->beb_rsvd_level - ubi->beb_rsvd_pebs;
		if (pebs > 0) {
			pebs = ubi->avail_pebs >= pebs ? pebs : ubi->avail_pebs;
			ubi->avail_pebs -= pebs;
			ubi->rsvd_pebs += pebs;
			ubi->beb_rsvd_pebs += pebs;
			if (pebs > 0)
				ubi_msg("reserve more %d PEBs", pebs);
		}
		for (i = 0; i < reserved_pebs; i++)
			new_mapping[i] = vol->eba_tbl[i];
		kfree(vol->eba_tbl);
		vol->eba_tbl = new_mapping;
		spin_unlock(&ubi->volumes_lock);
	}

	vol->reserved_pebs = reserved_pebs;
=======
		ubi_update_reserved(ubi);
		ubi_eba_copy_table(vol, new_eba_tbl, reserved_pebs);
		old_eba_tbl = vol->eba_tbl;
		vol->eba_tbl = new_eba_tbl;
		vol->reserved_pebs = reserved_pebs;
		spin_unlock(&ubi->volumes_lock);
	}

	/*
	 * When we shrink a volume we have to flush all pending (erase) work.
	 * Otherwise it can happen that upon next attach UBI finds a LEB with
	 * lnum > highest_lnum and refuses to attach.
	 */
	if (pebs < 0) {
		err = ubi_wl_flush(ubi, vol_id, UBI_ALL);
		if (err)
			goto out_acc;
	}

	/* Change volume table record */
	vtbl_rec = ubi->vtbl[vol_id];
	vtbl_rec.reserved_pebs = cpu_to_be32(reserved_pebs);
	err = ubi_change_vtbl_record(ubi, vol_id, &vtbl_rec);
	if (err)
		goto out_acc;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (vol->vol_type == UBI_DYNAMIC_VOLUME) {
		vol->used_ebs = reserved_pebs;
		vol->last_eb_bytes = vol->usable_leb_size;
		vol->used_bytes =
			(long long)vol->used_ebs * vol->usable_leb_size;
	}

<<<<<<< HEAD
	ubi_volume_notify(ubi, vol, UBI_VOLUME_RESIZED);
	if (paranoid_check_volumes(ubi))
		dbg_err("check failed while re-sizing volume %d", vol_id);
	return err;

out_acc:
	if (pebs > 0) {
		spin_lock(&ubi->volumes_lock);
		ubi->rsvd_pebs -= pebs;
		ubi->avail_pebs += pebs;
		spin_unlock(&ubi->volumes_lock);
	}
out_free:
	kfree(new_mapping);
=======
	/* destroy old table */
	ubi_eba_destroy_table(old_eba_tbl);
	ubi_volume_notify(ubi, vol, UBI_VOLUME_RESIZED);
	self_check_volumes(ubi);
	return err;

out_acc:
	spin_lock(&ubi->volumes_lock);
	vol->reserved_pebs = reserved_pebs - pebs;
	ubi->rsvd_pebs -= pebs;
	ubi->avail_pebs += pebs;
	if (pebs > 0)
		ubi_eba_copy_table(vol, old_eba_tbl, vol->reserved_pebs);
	else
		ubi_eba_copy_table(vol, old_eba_tbl, reserved_pebs);
	vol->eba_tbl = old_eba_tbl;
	spin_unlock(&ubi->volumes_lock);
out_free:
	ubi_eba_destroy_table(new_eba_tbl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ubi_rename_volumes - re-name UBI volumes.
 * @ubi: UBI device description object
 * @rename_list: list of &struct ubi_rename_entry objects
 *
 * This function re-names or removes volumes specified in the re-name list.
 * Returns zero in case of success and a negative error code in case of
 * failure.
 */
int ubi_rename_volumes(struct ubi_device *ubi, struct list_head *rename_list)
{
	int err;
	struct ubi_rename_entry *re;

	err = ubi_vtbl_rename_volumes(ubi, rename_list);
	if (err)
		return err;

	list_for_each_entry(re, rename_list, list) {
		if (re->remove) {
			err = ubi_remove_volume(re->desc, 1);
			if (err)
				break;
		} else {
			struct ubi_volume *vol = re->desc->vol;

			spin_lock(&ubi->volumes_lock);
			vol->name_len = re->new_name_len;
			memcpy(vol->name, re->new_name, re->new_name_len + 1);
			spin_unlock(&ubi->volumes_lock);
			ubi_volume_notify(ubi, vol, UBI_VOLUME_RENAMED);
		}
	}

<<<<<<< HEAD
	if (!err && paranoid_check_volumes(ubi))
		;
=======
	if (!err)
		self_check_volumes(ubi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ubi_add_volume - add volume.
 * @ubi: UBI device description object
 * @vol: volume description object
 *
 * This function adds an existing volume and initializes all its data
 * structures. Returns zero in case of success and a negative error code in
 * case of failure.
 */
int ubi_add_volume(struct ubi_device *ubi, struct ubi_volume *vol)
{
	int err, vol_id = vol->vol_id;
	dev_t dev;

	dbg_gen("add volume %d", vol_id);

	/* Register character device for the volume */
	cdev_init(&vol->cdev, &ubi_vol_cdev_operations);
	vol->cdev.owner = THIS_MODULE;
	dev = MKDEV(MAJOR(ubi->cdev.dev), vol->vol_id + 1);
	err = cdev_add(&vol->cdev, dev, 1);
	if (err) {
<<<<<<< HEAD
		ubi_err("cannot add character device for volume %d, error %d",
			vol_id, err);
=======
		ubi_err(ubi, "cannot add character device for volume %d, error %d",
			vol_id, err);
		vol_release(&vol->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return err;
	}

	vol->dev.release = vol_release;
	vol->dev.parent = &ubi->dev;
	vol->dev.devt = dev;
<<<<<<< HEAD
	vol->dev.class = ubi_class;
	dev_set_name(&vol->dev, "%s_%d", ubi->ubi_name, vol->vol_id);
	err = device_register(&vol->dev);
	if (err)
		goto out_cdev;

	err = volume_sysfs_init(ubi, vol);
	if (err) {
		cdev_del(&vol->cdev);
		volume_sysfs_close(vol);
		return err;
	}

	if (paranoid_check_volumes(ubi))
		dbg_err("check failed while adding volume %d", vol_id);
	return err;

out_cdev:
	cdev_del(&vol->cdev);
=======
	vol->dev.class = &ubi_class;
	vol->dev.groups = volume_dev_groups;
	dev_set_name(&vol->dev, "%s_%d", ubi->ubi_name, vol->vol_id);
	device_set_node(&vol->dev, find_volume_fwnode(vol));
	err = device_register(&vol->dev);
	if (err) {
		cdev_del(&vol->cdev);
		put_device(&vol->dev);
		return err;
	}

	self_check_volumes(ubi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ubi_free_volume - free volume.
 * @ubi: UBI device description object
 * @vol: volume description object
 *
 * This function frees all resources for volume @vol but does not remove it.
 * Used only when the UBI device is detached.
 */
void ubi_free_volume(struct ubi_device *ubi, struct ubi_volume *vol)
{
	dbg_gen("free volume %d", vol->vol_id);

	ubi->volumes[vol->vol_id] = NULL;
	cdev_del(&vol->cdev);
<<<<<<< HEAD
	volume_sysfs_close(vol);
}

#ifdef CONFIG_MTD_UBI_DEBUG

/**
 * paranoid_check_volume - check volume information.
 * @ubi: UBI device description object
 * @vol_id: volume ID
 *
 * Returns zero if volume is all right and a a negative error code if not.
 */
static int paranoid_check_volume(struct ubi_device *ubi, int vol_id)
=======
	device_unregister(&vol->dev);
}

/**
 * self_check_volume - check volume information.
 * @ubi: UBI device description object
 * @vol_id: volume ID
 *
 * Returns zero if volume is all right and a negative error code if not.
 */
static int self_check_volume(struct ubi_device *ubi, int vol_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int idx = vol_id2idx(ubi, vol_id);
	int reserved_pebs, alignment, data_pad, vol_type, name_len, upd_marker;
	const struct ubi_volume *vol;
	long long n;
	const char *name;

	spin_lock(&ubi->volumes_lock);
	reserved_pebs = be32_to_cpu(ubi->vtbl[vol_id].reserved_pebs);
	vol = ubi->volumes[idx];

	if (!vol) {
		if (reserved_pebs) {
<<<<<<< HEAD
			ubi_err("no volume info, but volume exists");
=======
			ubi_err(ubi, "no volume info, but volume exists");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
		spin_unlock(&ubi->volumes_lock);
		return 0;
	}

	if (vol->reserved_pebs < 0 || vol->alignment < 0 || vol->data_pad < 0 ||
	    vol->name_len < 0) {
<<<<<<< HEAD
		ubi_err("negative values");
		goto fail;
	}
	if (vol->alignment > ubi->leb_size || vol->alignment == 0) {
		ubi_err("bad alignment");
=======
		ubi_err(ubi, "negative values");
		goto fail;
	}
	if (vol->alignment > ubi->leb_size || vol->alignment == 0) {
		ubi_err(ubi, "bad alignment");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	n = vol->alignment & (ubi->min_io_size - 1);
	if (vol->alignment != 1 && n) {
<<<<<<< HEAD
		ubi_err("alignment is not multiple of min I/O unit");
=======
		ubi_err(ubi, "alignment is not multiple of min I/O unit");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	n = ubi->leb_size % vol->alignment;
	if (vol->data_pad != n) {
<<<<<<< HEAD
		ubi_err("bad data_pad, has to be %lld", n);
=======
		ubi_err(ubi, "bad data_pad, has to be %lld", n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	if (vol->vol_type != UBI_DYNAMIC_VOLUME &&
	    vol->vol_type != UBI_STATIC_VOLUME) {
<<<<<<< HEAD
		ubi_err("bad vol_type");
=======
		ubi_err(ubi, "bad vol_type");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	if (vol->upd_marker && vol->corrupted) {
<<<<<<< HEAD
		dbg_err("update marker and corrupted simultaneously");
=======
		ubi_err(ubi, "update marker and corrupted simultaneously");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	if (vol->reserved_pebs > ubi->good_peb_count) {
<<<<<<< HEAD
		ubi_err("too large reserved_pebs");
=======
		ubi_err(ubi, "too large reserved_pebs");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	n = ubi->leb_size - vol->data_pad;
	if (vol->usable_leb_size != ubi->leb_size - vol->data_pad) {
<<<<<<< HEAD
		ubi_err("bad usable_leb_size, has to be %lld", n);
=======
		ubi_err(ubi, "bad usable_leb_size, has to be %lld", n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	if (vol->name_len > UBI_VOL_NAME_MAX) {
<<<<<<< HEAD
		ubi_err("too long volume name, max is %d", UBI_VOL_NAME_MAX);
=======
		ubi_err(ubi, "too long volume name, max is %d",
			UBI_VOL_NAME_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	n = strnlen(vol->name, vol->name_len + 1);
	if (n != vol->name_len) {
<<<<<<< HEAD
		ubi_err("bad name_len %lld", n);
=======
		ubi_err(ubi, "bad name_len %lld", n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	n = (long long)vol->used_ebs * vol->usable_leb_size;
	if (vol->vol_type == UBI_DYNAMIC_VOLUME) {
		if (vol->corrupted) {
<<<<<<< HEAD
			ubi_err("corrupted dynamic volume");
			goto fail;
		}
		if (vol->used_ebs != vol->reserved_pebs) {
			ubi_err("bad used_ebs");
			goto fail;
		}
		if (vol->last_eb_bytes != vol->usable_leb_size) {
			ubi_err("bad last_eb_bytes");
			goto fail;
		}
		if (vol->used_bytes != n) {
			ubi_err("bad used_bytes");
=======
			ubi_err(ubi, "corrupted dynamic volume");
			goto fail;
		}
		if (vol->used_ebs != vol->reserved_pebs) {
			ubi_err(ubi, "bad used_ebs");
			goto fail;
		}
		if (vol->last_eb_bytes != vol->usable_leb_size) {
			ubi_err(ubi, "bad last_eb_bytes");
			goto fail;
		}
		if (vol->used_bytes != n) {
			ubi_err(ubi, "bad used_bytes");
			goto fail;
		}

		if (vol->skip_check) {
			ubi_err(ubi, "bad skip_check");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
	} else {
		if (vol->used_ebs < 0 || vol->used_ebs > vol->reserved_pebs) {
<<<<<<< HEAD
			ubi_err("bad used_ebs");
=======
			ubi_err(ubi, "bad used_ebs");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
		if (vol->last_eb_bytes < 0 ||
		    vol->last_eb_bytes > vol->usable_leb_size) {
<<<<<<< HEAD
			ubi_err("bad last_eb_bytes");
=======
			ubi_err(ubi, "bad last_eb_bytes");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
		if (vol->used_bytes < 0 || vol->used_bytes > n ||
		    vol->used_bytes < n - vol->usable_leb_size) {
<<<<<<< HEAD
			ubi_err("bad used_bytes");
=======
			ubi_err(ubi, "bad used_bytes");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fail;
		}
	}

	alignment  = be32_to_cpu(ubi->vtbl[vol_id].alignment);
	data_pad   = be32_to_cpu(ubi->vtbl[vol_id].data_pad);
	name_len   = be16_to_cpu(ubi->vtbl[vol_id].name_len);
	upd_marker = ubi->vtbl[vol_id].upd_marker;
	name       = &ubi->vtbl[vol_id].name[0];
	if (ubi->vtbl[vol_id].vol_type == UBI_VID_DYNAMIC)
		vol_type = UBI_DYNAMIC_VOLUME;
	else
		vol_type = UBI_STATIC_VOLUME;

	if (alignment != vol->alignment || data_pad != vol->data_pad ||
	    upd_marker != vol->upd_marker || vol_type != vol->vol_type ||
	    name_len != vol->name_len || strncmp(name, vol->name, name_len)) {
<<<<<<< HEAD
		ubi_err("volume info is different");
=======
		ubi_err(ubi, "volume info is different");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	}

	spin_unlock(&ubi->volumes_lock);
	return 0;

fail:
<<<<<<< HEAD
	ubi_err("paranoid check failed for volume %d", vol_id);
	if (vol)
		ubi_dbg_dump_vol_info(vol);
	ubi_dbg_dump_vtbl_record(&ubi->vtbl[vol_id], vol_id);
=======
	ubi_err(ubi, "self-check failed for volume %d", vol_id);
	if (vol)
		ubi_dump_vol_info(vol);
	ubi_dump_vtbl_record(&ubi->vtbl[vol_id], vol_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_stack();
	spin_unlock(&ubi->volumes_lock);
	return -EINVAL;
}

/**
<<<<<<< HEAD
 * paranoid_check_volumes - check information about all volumes.
 * @ubi: UBI device description object
 *
 * Returns zero if volumes are all right and a a negative error code if not.
 */
static int paranoid_check_volumes(struct ubi_device *ubi)
{
	int i, err = 0;

	if (!ubi->dbg->chk_gen)
		return 0;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		err = paranoid_check_volume(ubi, i);
=======
 * self_check_volumes - check information about all volumes.
 * @ubi: UBI device description object
 *
 * Returns zero if volumes are all right and a negative error code if not.
 */
static int self_check_volumes(struct ubi_device *ubi)
{
	int i, err = 0;

	if (!ubi_dbg_chk_gen(ubi))
		return 0;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		err = self_check_volume(ubi, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			break;
	}

	return err;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
