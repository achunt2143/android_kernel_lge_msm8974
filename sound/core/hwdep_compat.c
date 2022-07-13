<<<<<<< HEAD
/*
 *   32bit -> 64bit ioctl wrapper for hwdep API
 *   Copyright (c) by Takashi Iwai <tiwai@suse.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   32bit -> 64bit ioctl wrapper for hwdep API
 *   Copyright (c) by Takashi Iwai <tiwai@suse.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/* This file is included from hwdep.c */

#include <linux/compat.h>

struct snd_hwdep_dsp_image32 {
	u32 index;
	unsigned char name[64];
	u32 image;	/* pointer */
	u32 length;
	u32 driver_data;
} /* don't set packed attribute here */;

static int snd_hwdep_dsp_load_compat(struct snd_hwdep *hw,
				     struct snd_hwdep_dsp_image32 __user *src)
{
<<<<<<< HEAD
	struct snd_hwdep_dsp_image __user *dst;
	compat_caddr_t ptr;
	u32 val;

	dst = compat_alloc_user_space(sizeof(*dst));

	/* index and name */
	if (copy_in_user(dst, src, 4 + 64))
		return -EFAULT;
	if (get_user(ptr, &src->image) ||
	    put_user(compat_ptr(ptr), &dst->image))
		return -EFAULT;
	if (get_user(val, &src->length) ||
	    put_user(val, &dst->length))
		return -EFAULT;
	if (get_user(val, &src->driver_data) ||
	    put_user(val, &dst->driver_data))
		return -EFAULT;

	return snd_hwdep_dsp_load(hw, dst);
=======
	struct snd_hwdep_dsp_image info = {};
	compat_caddr_t ptr;

	if (copy_from_user(&info, src, 4 + 64) ||
	    get_user(ptr, &src->image) ||
	    get_user(info.length, &src->length) ||
	    get_user(info.driver_data, &src->driver_data))
		return -EFAULT;
	info.image = compat_ptr(ptr);

	return snd_hwdep_dsp_load(hw, &info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

enum {
	SNDRV_HWDEP_IOCTL_DSP_LOAD32   = _IOW('H', 0x03, struct snd_hwdep_dsp_image32)
};

static long snd_hwdep_ioctl_compat(struct file * file, unsigned int cmd,
				   unsigned long arg)
{
	struct snd_hwdep *hw = file->private_data;
	void __user *argp = compat_ptr(arg);
	switch (cmd) {
	case SNDRV_HWDEP_IOCTL_PVERSION:
	case SNDRV_HWDEP_IOCTL_INFO:
	case SNDRV_HWDEP_IOCTL_DSP_STATUS:
		return snd_hwdep_ioctl(file, cmd, (unsigned long)argp);
	case SNDRV_HWDEP_IOCTL_DSP_LOAD32:
		return snd_hwdep_dsp_load_compat(hw, argp);
	}
	if (hw->ops.ioctl_compat)
		return hw->ops.ioctl_compat(hw, file, cmd, arg);
	return -ENOIOCTLCMD;
}
