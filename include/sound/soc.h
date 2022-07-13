<<<<<<< HEAD
/*
 * linux/sound/soc.h -- ALSA SoC Layer
 *
 * Author:		Liam Girdwood
 * Created:		Aug 11th 2005
 * Copyright:	Wolfson Microelectronics. PLC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0
 *
 * linux/sound/soc.h -- ALSA SoC Layer
 *
 * Author:	Liam Girdwood
 * Created:	Aug 11th 2005
 * Copyright:	Wolfson Microelectronics. PLC.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_SND_SOC_H
#define __LINUX_SND_SOC_H

<<<<<<< HEAD
=======
#include <linux/args.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/regmap.h>
<<<<<<< HEAD
=======
#include <linux/log2.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/compress_driver.h>
#include <sound/control.h>
#include <sound/ac97_codec.h>

/*
 * Convenience kcontrol builders
 */
<<<<<<< HEAD
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, .platform_max = xmax, \
	.invert = xinvert})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert)
#define SOC_SINGLE_VALUE_EXT(xreg, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .max = xmax, .platform_max = xmax, .invert = xinvert})
#define SOC_DOUBLE_R_VALUE(xlreg, xrreg, xshift, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .platform_max = xmax, .invert = xinvert})
#ifdef CONFIG_SND_SOC_WM5110
#define SOC_DOUBLE_R_RANGE_VALUE(xlreg, xrreg, xshift, xmin, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.min = xmin, .max = xmax, .platform_max = xmax, .invert = xinvert})
#endif
=======
#define SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, xmax, xinvert, xautodisable) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .max = xmax, \
	.invert = xinvert, .autodisable = xautodisable})
#define SOC_DOUBLE_S_VALUE(xreg, shift_left, shift_right, xmin, xmax, xsign_bit, xinvert, xautodisable) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg, .shift = shift_left, \
	.rshift = shift_right, .min = xmin, .max = xmax, \
	.sign_bit = xsign_bit, .invert = xinvert, .autodisable = xautodisable})
#define SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, xautodisable) \
	SOC_DOUBLE_VALUE(xreg, xshift, xshift, xmax, xinvert, xautodisable)
#define SOC_SINGLE_VALUE_EXT(xreg, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .max = xmax, .invert = xinvert})
#define SOC_DOUBLE_R_VALUE(xlreg, xrreg, xshift, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .invert = xinvert})
#define SOC_DOUBLE_R_S_VALUE(xlreg, xrreg, xshift, xmin, xmax, xsign_bit, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.max = xmax, .min = xmin, .sign_bit = xsign_bit, \
	.invert = xinvert})
#define SOC_DOUBLE_R_RANGE_VALUE(xlreg, xrreg, xshift, xmin, xmax, xinvert) \
	((unsigned long)&(struct soc_mixer_control) \
	{.reg = xlreg, .rreg = xrreg, .shift = xshift, .rshift = xshift, \
	.min = xmin, .max = xmax, .invert = xinvert})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE(xname, reg, shift, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
#ifdef CONFIG_SND_SOC_WM5110
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_RANGE(xname, xreg, xshift, xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, .get = snd_soc_get_volsw_range, \
	.put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
<<<<<<< HEAD
		{.reg = xreg, .shift = xshift, .min = xmin,\
		 .max = xmax, .platform_max = xmax, .invert = xinvert} }
#endif
=======
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift,  .min = xmin, .max = xmax, \
		 .invert = xinvert} }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_TLV(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
<<<<<<< HEAD
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }
#define SOC_SINGLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info   = snd_soc_info_volsw_s8, .get = snd_soc_get_volsw_s8, \
	.put    = snd_soc_put_volsw_s8, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .min = xmin, .max = xmax, \
		 .platform_max = xmax} }
#ifdef CONFIG_SND_SOC_WM5110
=======
	.private_value = SOC_SINGLE_VALUE(reg, shift, max, invert, 0) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_SX_TLV(xname, xreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array),\
<<<<<<< HEAD
	.info = snd_soc_info_volsw, \
=======
	.info = snd_soc_info_volsw_sx, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get = snd_soc_get_volsw_sx,\
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
#define SOC_SINGLE_RANGE_TLV(xname, xreg, xshift, xmin, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
<<<<<<< HEAD
		{.reg = xreg, .shift = xshift, .min = xmin,\
		 .max = xmax, .platform_max = xmax, .invert = xinvert} }
#endif
=======
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift, .min = xmin, .max = xmax, \
		 .invert = xinvert} }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE(xname, reg, shift_left, shift_right, max, invert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
<<<<<<< HEAD
					  max, invert) }
=======
					  max, invert, 0) }
#define SOC_DOUBLE_STS(xname, reg, shift_left, shift_right, max, invert) \
{									\
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),		\
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,		\
	.access = SNDRV_CTL_ELEM_ACCESS_READ |				\
		SNDRV_CTL_ELEM_ACCESS_VOLATILE,				\
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right,	\
					  max, invert, 0) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_R(xname, reg_left, reg_right, xshift, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_WM5110
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_R_RANGE(xname, reg_left, reg_right, xshift, xmin, \
			   xmax, xinvert)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_TLV(xname, reg, shift_left, shift_right, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw, \
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_VALUE(reg, shift_left, shift_right, \
<<<<<<< HEAD
					  max, invert) }
=======
					  max, invert, 0) }
#define SOC_DOUBLE_SX_TLV(xname, xreg, shift_left, shift_right, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info = snd_soc_info_volsw_sx, \
	.get = snd_soc_get_volsw_sx, \
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, \
		.shift = shift_left, .rshift = shift_right, \
		.max = xmax, .min = xmin} }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_R_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_WM5110
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_R_RANGE_TLV(xname, reg_left, reg_right, xshift, xmin, \
			       xmax, xinvert, tlv_array)		\
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = snd_soc_get_volsw_range, .put = snd_soc_put_volsw_range, \
	.private_value = SOC_DOUBLE_R_RANGE_VALUE(reg_left, reg_right, \
					    xshift, xmin, xmax, xinvert) }
#define SOC_DOUBLE_R_SX_TLV(xname, xreg, xrreg, xshift, xmin, xmax, tlv_array) \
{       .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
	SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
<<<<<<< HEAD
	.info = snd_soc_info_volsw, \
=======
	.info = snd_soc_info_volsw_sx, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get = snd_soc_get_volsw_sx, \
	.put = snd_soc_put_volsw_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xrreg, \
		.shift = xshift, .rshift = xshift, \
		.max = xmax, .min = xmin} }
<<<<<<< HEAD
#endif
=======
#define SOC_DOUBLE_R_S_TLV(xname, reg_left, reg_right, xshift, xmin, xmax, xsign_bit, xinvert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = snd_soc_get_volsw, .put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_R_S_VALUE(reg_left, reg_right, xshift, \
					    xmin, xmax, xsign_bit, xinvert) }
#define SOC_SINGLE_S_TLV(xname, xreg, xshift, xmin, xmax, xsign_bit, xinvert, tlv_array) \
	SOC_DOUBLE_R_S_TLV(xname, xreg, xreg, xshift, xmin, xmax, xsign_bit, xinvert, tlv_array)
#define SOC_SINGLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
	{.reg = xreg, .rreg = xreg,  \
	 .min = xmin, .max = xmax, \
	.sign_bit = 7,} }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_S8_TLV(xname, xreg, xmin, xmax, tlv_array) \
{	.iface  = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p  = (tlv_array), \
<<<<<<< HEAD
	.info   = snd_soc_info_volsw_s8, .get = snd_soc_get_volsw_s8, \
	.put    = snd_soc_put_volsw_s8, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .min = xmin, .max = xmax, \
		 .platform_max = xmax} }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmax, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.max = xmax, .texts = xtexts }
#define SOC_ENUM_SINGLE(xreg, xshift, xmax, xtexts) \
	SOC_ENUM_DOUBLE(xreg, xshift, xshift, xmax, xtexts)
#define SOC_ENUM_SINGLE_EXT(xmax, xtexts) \
{	.max = xmax, .texts = xtexts }
#define SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, xmax, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.mask = xmask, .max = xmax, .texts = xtexts, .values = xvalues}
#define SOC_VALUE_ENUM_SINGLE(xreg, xshift, xmask, xmax, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE(xreg, xshift, xshift, xmask, xmax, xtexts, xvalues)
=======
	.info = snd_soc_info_volsw, .get = snd_soc_get_volsw,\
	.put = snd_soc_put_volsw, \
	.private_value = SOC_DOUBLE_S_VALUE(xreg, 0, 8, xmin, xmax, 7, 0, 0) }
#define SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xitems, xtexts) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.items = xitems, .texts = xtexts, \
	.mask = xitems ? roundup_pow_of_two(xitems) - 1 : 0}
#define SOC_ENUM_SINGLE(xreg, xshift, xitems, xtexts) \
	SOC_ENUM_DOUBLE(xreg, xshift, xshift, xitems, xtexts)
#define SOC_ENUM_SINGLE_EXT(xitems, xtexts) \
{	.items = xitems, .texts = xtexts }
#define SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, xitems, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift_l, .shift_r = xshift_r, \
	.mask = xmask, .items = xitems, .texts = xtexts, .values = xvalues}
#define SOC_VALUE_ENUM_SINGLE(xreg, xshift, xmask, xitems, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE(xreg, xshift, xshift, xmask, xitems, xtexts, xvalues)
#define SOC_VALUE_ENUM_SINGLE_AUTODISABLE(xreg, xshift, xmask, xitems, xtexts, xvalues) \
{	.reg = xreg, .shift_l = xshift, .shift_r = xshift, \
	.mask = xmask, .items = xitems, .texts = xtexts, \
	.values = xvalues, .autodisable = 1}
#define SOC_ENUM_SINGLE_VIRT(xitems, xtexts) \
	SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, xitems, xtexts)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_enum_double, .put = snd_soc_put_enum_double, \
	.private_value = (unsigned long)&xenum }
<<<<<<< HEAD
#define SOC_VALUE_ENUM(xname, xenum) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,\
	.info = snd_soc_info_enum_double, \
	.get = snd_soc_get_value_enum_double, \
	.put = snd_soc_put_value_enum_double, \
	.private_value = (unsigned long)&xenum }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_EXT(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_EXT(xname, reg, shift_left, shift_right, max, invert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = \
<<<<<<< HEAD
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert) }
 #define SOC_SINGLE_MULTI_EXT(xname, xreg, xshift, xmax, xinvert, xcount,\
	xhandler_get, xhandler_put) \
{      .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_multi_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_multi_mixer_control) \
		{.reg = xreg, .shift = xshift, .rshift = xshift, .max = xmax, \
		.count = xcount, .platform_max = xmax, .invert = xinvert} }
=======
		SOC_DOUBLE_VALUE(reg, shift_left, shift_right, max, invert, 0) }
#define SOC_DOUBLE_R_EXT(xname, reg_left, reg_right, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_EXT_TLV(xname, xreg, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
<<<<<<< HEAD
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert) }
=======
	.private_value = SOC_SINGLE_VALUE(xreg, xshift, xmax, xinvert, 0) }
#define SOC_SINGLE_RANGE_EXT_TLV(xname, xreg, xshift, xmin, xmax, xinvert, \
				 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname),\
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_range, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg, .rreg = xreg, .shift = xshift, \
		 .rshift = xshift, .min = xmin, .max = xmax, \
		 .invert = xinvert} }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_EXT_TLV(xname, xreg, shift_left, shift_right, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_VALUE(xreg, shift_left, shift_right, \
<<<<<<< HEAD
					  xmax, xinvert) }
=======
					  xmax, xinvert, 0) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_DOUBLE_R_EXT_TLV(xname, reg_left, reg_right, xshift, xmax, xinvert,\
	 xhandler_get, xhandler_put, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		 SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_VALUE(reg_left, reg_right, xshift, \
					    xmax, xinvert) }
<<<<<<< HEAD
=======
#define SOC_DOUBLE_R_S_EXT_TLV(xname, reg_left, reg_right, xshift, xmin, xmax, \
			       xsign_bit, xinvert, xhandler_get, xhandler_put, \
			       tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = SOC_DOUBLE_R_S_VALUE(reg_left, reg_right, xshift, \
					      xmin, xmax, xsign_bit, xinvert) }
#define SOC_SINGLE_S_EXT_TLV(xname, xreg, xshift, xmin, xmax, \
			     xsign_bit, xinvert, xhandler_get, xhandler_put, \
			     tlv_array) \
	SOC_DOUBLE_R_S_EXT_TLV(xname, xreg, xreg, xshift, xmin, xmax, \
			       xsign_bit, xinvert, xhandler_get, xhandler_put, \
			       tlv_array)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SOC_SINGLE_BOOL_EXT(xname, xdata, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_info_bool_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = xdata }
#define SOC_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
<<<<<<< HEAD
	.info = snd_soc_info_enum_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }
#ifdef CONFIG_SND_SOC_WM5110
=======
	.info = snd_soc_info_enum_double, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&xenum }
#define SOC_VALUE_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put) \
	SOC_ENUM_EXT(xname, xenum, xhandler_get, xhandler_put)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SND_SOC_BYTES(xname, xbase, xregs)		      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs }) }
<<<<<<< HEAD
=======
#define SND_SOC_BYTES_E(xname, xbase, xregs, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_bytes_info, .get = xhandler_get, \
	.put = xhandler_put, .private_value = \
		((unsigned long)&(struct soc_bytes) \
		{.base = xbase, .num_regs = xregs }) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SND_SOC_BYTES_MASK(xname, xbase, xregs, xmask)	      \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname,   \
	.info = snd_soc_bytes_info, .get = snd_soc_bytes_get, \
	.put = snd_soc_bytes_put, .private_value =	      \
		((unsigned long)&(struct soc_bytes)           \
		{.base = xbase, .num_regs = xregs,	      \
		 .mask = xmask }) }
<<<<<<< HEAD
#endif

#ifndef CONFIG_SND_SOC_WM5110
#define SOC_DOUBLE_R_SX_TLV(xname, xreg_left, xreg_right, xshift,\
		xmin, xmax, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
		  SNDRV_CTL_ELEM_ACCESS_READWRITE, \
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw_2r_sx, \
	.get = snd_soc_get_volsw_2r_sx, \
	.put = snd_soc_put_volsw_2r_sx, \
	.private_value = (unsigned long)&(struct soc_mixer_control) \
		{.reg = xreg_left, \
		 .rreg = xreg_right, .shift = xshift, \
		 .min = xmin, .max = xmax} }
#endif
=======

/*
 * SND_SOC_BYTES_EXT is deprecated, please USE SND_SOC_BYTES_TLV instead
 */
#define SND_SOC_BYTES_EXT(xname, xcount, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.info = snd_soc_bytes_info_ext, \
	.get = xhandler_get, .put = xhandler_put, \
	.private_value = (unsigned long)&(struct soc_bytes_ext) \
		{.max = xcount} }
#define SND_SOC_BYTES_TLV(xname, xcount, xhandler_get, xhandler_put) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE | \
		  SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK, \
	.tlv.c = (snd_soc_bytes_tlv_callback), \
	.info = snd_soc_bytes_info_ext, \
	.private_value = (unsigned long)&(struct soc_bytes_ext) \
		{.max = xcount, .get = xhandler_get, .put = xhandler_put, } }
#define SOC_SINGLE_XR_SX(xname, xregbase, xregcount, xnbits, \
		xmin, xmax, xinvert) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	.info = snd_soc_info_xr_sx, .get = snd_soc_get_xr_sx, \
	.put = snd_soc_put_xr_sx, \
	.private_value = (unsigned long)&(struct soc_mreg_control) \
		{.regbase = xregbase, .regcount = xregcount, .nbits = xnbits, \
		.invert = xinvert, .min = xmin, .max = xmax} }

#define SOC_SINGLE_STROBE(xname, xreg, xshift, xinvert) \
	SOC_SINGLE_EXT(xname, xreg, xshift, 1, xinvert, \
		snd_soc_get_strobe, snd_soc_put_strobe)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Simplified versions of above macros, declaring a struct and calculating
 * ARRAY_SIZE internally
 */
#define SOC_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xtexts) \
<<<<<<< HEAD
	struct soc_enum name = SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, \
=======
	const struct soc_enum name = SOC_ENUM_DOUBLE(xreg, xshift_l, xshift_r, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						ARRAY_SIZE(xtexts), xtexts)
#define SOC_ENUM_SINGLE_DECL(name, xreg, xshift, xtexts) \
	SOC_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xtexts)
#define SOC_ENUM_SINGLE_EXT_DECL(name, xtexts) \
<<<<<<< HEAD
	struct soc_enum name = SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(xtexts), xtexts)
#define SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xmask, xtexts, xvalues) \
	struct soc_enum name = SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, \
=======
	const struct soc_enum name = SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(xtexts), xtexts)
#define SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift_l, xshift_r, xmask, xtexts, xvalues) \
	const struct soc_enum name = SOC_VALUE_ENUM_DOUBLE(xreg, xshift_l, xshift_r, xmask, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							ARRAY_SIZE(xtexts), xtexts, xvalues)
#define SOC_VALUE_ENUM_SINGLE_DECL(name, xreg, xshift, xmask, xtexts, xvalues) \
	SOC_VALUE_ENUM_DOUBLE_DECL(name, xreg, xshift, xshift, xmask, xtexts, xvalues)

<<<<<<< HEAD

/* DAI Link Host Mode Support */
#define SND_SOC_DAI_LINK_NO_HOST		0x1
#define SND_SOC_DAI_LINK_OPT_HOST		0x2

#define snd_soc_get_enum_text(soc_enum, idx) \
	(soc_enum->texts ? soc_enum->texts[idx] : soc_enum->dtexts[idx])


/*
 * Component probe and remove ordering levels for components with runtime
 * dependencies.
 */
#define SND_SOC_COMP_ORDER_FIRST		-2
#define SND_SOC_COMP_ORDER_EARLY		-1
#define SND_SOC_COMP_ORDER_NORMAL		0
#define SND_SOC_COMP_ORDER_LATE		1
#define SND_SOC_COMP_ORDER_LAST		2

/*
 * Bias levels
 *
 * @ON:      Bias is fully on for audio playback and capture operations.
 * @PREPARE: Prepare for audio operations. Called before DAPM switching for
 *           stream start and stop operations.
 * @STANDBY: Low power standby state when no playback/capture operations are
 *           in progress. NOTE: The transition time between STANDBY and ON
 *           should be as fast as possible and no longer than 10ms.
 * @OFF:     Power Off. No restrictions on transition times.
 */
enum snd_soc_bias_level {
	SND_SOC_BIAS_OFF = 0,
	SND_SOC_BIAS_STANDBY = 1,
	SND_SOC_BIAS_PREPARE = 2,
	SND_SOC_BIAS_ON = 3,
};
=======
#define SOC_VALUE_ENUM_SINGLE_AUTODISABLE_DECL(name, xreg, xshift, xmask, xtexts, xvalues) \
	const struct soc_enum name = SOC_VALUE_ENUM_SINGLE_AUTODISABLE(xreg, \
		xshift, xmask, ARRAY_SIZE(xtexts), xtexts, xvalues)

#define SOC_ENUM_SINGLE_VIRT_DECL(name, xtexts) \
	const struct soc_enum name = SOC_ENUM_SINGLE_VIRT(ARRAY_SIZE(xtexts), xtexts)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct device_node;
struct snd_jack;
struct snd_soc_card;
struct snd_soc_pcm_stream;
struct snd_soc_ops;
struct snd_soc_pcm_runtime;
struct snd_soc_dai;
struct snd_soc_dai_driver;
<<<<<<< HEAD
struct snd_soc_platform;
struct snd_soc_dai_link;
struct snd_soc_platform_driver;
struct snd_soc_codec;
struct snd_soc_codec_driver;
=======
struct snd_soc_dai_link;
struct snd_soc_component;
struct snd_soc_component_driver;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct soc_enum;
struct snd_soc_jack;
struct snd_soc_jack_zone;
struct snd_soc_jack_pin;
<<<<<<< HEAD
struct snd_soc_cache_ops;
struct snd_soc_dpcm_link;
#include <sound/soc-dapm.h>

#ifdef CONFIG_GPIOLIB
struct snd_soc_jack_gpio;
#endif

typedef int (*hw_write_t)(void *,const char* ,int);

extern struct snd_ac97_bus_ops soc_ac97_ops;

enum snd_soc_control_type {
	SND_SOC_I2C = 1,
	SND_SOC_SPI,
	SND_SOC_REGMAP,
};

enum snd_soc_compress_type {
	SND_SOC_FLAT_COMPRESSION = 1,
};
=======
#include <sound/soc-dapm.h>
#include <sound/soc-dpcm.h>
#include <sound/soc-topology.h>

struct snd_soc_jack_gpio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum snd_soc_pcm_subclass {
	SND_SOC_PCM_CLASS_PCM	= 0,
	SND_SOC_PCM_CLASS_BE	= 1,
};

<<<<<<< HEAD
/*
 * Dynamic PCM DAI link states.
 */
enum snd_soc_dpcm_state {
	SND_SOC_DPCM_STATE_NEW	= 0,
	SND_SOC_DPCM_STATE_OPEN,
	SND_SOC_DPCM_STATE_HW_PARAMS,
	SND_SOC_DPCM_STATE_PREPARE,
	SND_SOC_DPCM_STATE_START,
	SND_SOC_DPCM_STATE_STOP,
	SND_SOC_DPCM_STATE_PAUSED,
	SND_SOC_DPCM_STATE_SUSPEND,
	SND_SOC_DPCM_STATE_HW_FREE,
	SND_SOC_DPCM_STATE_CLOSE,
};

/*
 * Dynamic PCM trigger ordering. Triggering flexibility is required as some
 * DSPs require triggering before/after their clients/hosts.
 *
 * i.e. some clients may want to manually order this call in their PCM
 * trigger() whilst others will just use the regular core ordering.
 */
enum snd_soc_dpcm_trigger {
	SND_SOC_DPCM_TRIGGER_PRE		= 0,
	SND_SOC_DPCM_TRIGGER_POST,
	SND_SOC_DPCM_TRIGGER_BESPOKE,
};

int snd_soc_codec_set_sysclk(struct snd_soc_codec *codec, int clk_id,
			     int source, unsigned int freq, int dir);
int snd_soc_codec_set_pll(struct snd_soc_codec *codec, int pll_id, int source,
			  unsigned int freq_in, unsigned int freq_out);

int snd_soc_register_card(struct snd_soc_card *card);
int snd_soc_unregister_card(struct snd_soc_card *card);
int snd_soc_suspend(struct device *dev);
int snd_soc_resume(struct device *dev);
int snd_soc_poweroff(struct device *dev);
int snd_soc_register_platform(struct device *dev,
		struct snd_soc_platform_driver *platform_drv);
void snd_soc_unregister_platform(struct device *dev);
int snd_soc_register_codec(struct device *dev,
		const struct snd_soc_codec_driver *codec_drv,
		struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_codec(struct device *dev);
int snd_soc_codec_volatile_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_readable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_writable_register(struct snd_soc_codec *codec,
				    unsigned int reg);
int snd_soc_codec_set_cache_io(struct snd_soc_codec *codec,
			       int addr_bits, int data_bits,
			       enum snd_soc_control_type control);
int snd_soc_cache_sync(struct snd_soc_codec *codec);
int snd_soc_cache_init(struct snd_soc_codec *codec);
int snd_soc_cache_exit(struct snd_soc_codec *codec);
int snd_soc_cache_write(struct snd_soc_codec *codec,
			unsigned int reg, unsigned int value);
int snd_soc_cache_read(struct snd_soc_codec *codec,
		       unsigned int reg, unsigned int *value);
int snd_soc_default_volatile_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_readable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_default_writable_register(struct snd_soc_codec *codec,
				      unsigned int reg);
int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg);
int snd_soc_platform_write(struct snd_soc_platform *platform,
					unsigned int reg, unsigned int val);
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num);
int soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num);

struct snd_pcm_substream *snd_soc_get_dai_substream(struct snd_soc_card *card,
		const char *dai_link, int stream);
struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
		const char *dai_link);
=======
int snd_soc_register_card(struct snd_soc_card *card);
void snd_soc_unregister_card(struct snd_soc_card *card);
int devm_snd_soc_register_card(struct device *dev, struct snd_soc_card *card);
#ifdef CONFIG_PM_SLEEP
int snd_soc_suspend(struct device *dev);
int snd_soc_resume(struct device *dev);
#else
static inline int snd_soc_suspend(struct device *dev)
{
	return 0;
}

static inline int snd_soc_resume(struct device *dev)
{
	return 0;
}
#endif
int snd_soc_poweroff(struct device *dev);
int snd_soc_component_initialize(struct snd_soc_component *component,
				 const struct snd_soc_component_driver *driver,
				 struct device *dev);
int snd_soc_add_component(struct snd_soc_component *component,
			  struct snd_soc_dai_driver *dai_drv,
			  int num_dai);
int snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *component_driver,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
int devm_snd_soc_register_component(struct device *dev,
			 const struct snd_soc_component_driver *component_driver,
			 struct snd_soc_dai_driver *dai_drv, int num_dai);
void snd_soc_unregister_component(struct device *dev);
void snd_soc_unregister_component_by_driver(struct device *dev,
			 const struct snd_soc_component_driver *component_driver);
struct snd_soc_component *snd_soc_lookup_component_nolocked(struct device *dev,
							    const char *driver_name);
struct snd_soc_component *snd_soc_lookup_component(struct device *dev,
						   const char *driver_name);

int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num);
#ifdef CONFIG_SND_SOC_COMPRESS
int snd_soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num);
#else
static inline int snd_soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num)
{
	return 0;
}
#endif

void snd_soc_disconnect_sync(struct device *dev);

struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
				struct snd_soc_dai_link *dai_link);

bool snd_soc_runtime_ignore_pmdown_time(struct snd_soc_pcm_runtime *rtd);

void snd_soc_runtime_action(struct snd_soc_pcm_runtime *rtd,
			    int stream, int action);
static inline void snd_soc_runtime_activate(struct snd_soc_pcm_runtime *rtd,
				     int stream)
{
	snd_soc_runtime_action(rtd, stream, 1);
}
static inline void snd_soc_runtime_deactivate(struct snd_soc_pcm_runtime *rtd,
				       int stream)
{
	snd_soc_runtime_action(rtd, stream, -1);
}

int snd_soc_runtime_calc_hw(struct snd_soc_pcm_runtime *rtd,
			    struct snd_pcm_hardware *hw, int stream);

int snd_soc_runtime_set_dai_fmt(struct snd_soc_pcm_runtime *rtd,
	unsigned int dai_fmt);

#ifdef CONFIG_DMI
int snd_soc_set_dmi_name(struct snd_soc_card *card, const char *flavour);
#else
static inline int snd_soc_set_dmi_name(struct snd_soc_card *card,
				       const char *flavour)
{
	return 0;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Utility functions to get clock rates from various things */
int snd_soc_calc_frame_size(int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_frame_size(struct snd_pcm_hw_params *params);
int snd_soc_calc_bclk(int fs, int sample_size, int channels, int tdm_slots);
int snd_soc_params_to_bclk(struct snd_pcm_hw_params *parms);
<<<<<<< HEAD
=======
int snd_soc_tdm_params_to_bclk(struct snd_pcm_hw_params *params,
			       int tdm_width, int tdm_slots, int slot_multiple);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* set runtime hw params */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw);

<<<<<<< HEAD
/* Jack reporting */
int snd_soc_jack_new(struct snd_soc_codec *codec, const char *id, int type,
		     struct snd_soc_jack *jack);
void snd_soc_jack_report(struct snd_soc_jack *jack, int status, int mask);
void snd_soc_jack_report_no_dapm(struct snd_soc_jack *jack, int status,
				 int mask);
int snd_soc_jack_add_pins(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_pin *pins);
void snd_soc_jack_notifier_register(struct snd_soc_jack *jack,
				    struct notifier_block *nb);
void snd_soc_jack_notifier_unregister(struct snd_soc_jack *jack,
				      struct notifier_block *nb);
int snd_soc_jack_add_zones(struct snd_soc_jack *jack, int count,
			  struct snd_soc_jack_zone *zones);
int snd_soc_jack_get_type(struct snd_soc_jack *jack, int micbias_voltage);
#ifdef CONFIG_GPIOLIB
int snd_soc_jack_add_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
void snd_soc_jack_free_gpios(struct snd_soc_jack *jack, int count,
			struct snd_soc_jack_gpio *gpios);
#endif

/* codec register bit access */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);
int snd_soc_update_bits_locked(struct snd_soc_codec *codec,
			       unsigned short reg, unsigned int mask,
			       unsigned int value);
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value);

int snd_soc_new_ac97_codec(struct snd_soc_codec *codec,
	struct snd_ac97_bus_ops *ops, int num);
void snd_soc_free_ac97_codec(struct snd_soc_codec *codec);

void snd_soc_card_change_online_state(struct snd_soc_card *soc_card,
				      int online);

=======
struct snd_ac97 *snd_soc_alloc_ac97_component(struct snd_soc_component *component);
struct snd_ac97 *snd_soc_new_ac97_component(struct snd_soc_component *component,
	unsigned int id, unsigned int id_mask);
void snd_soc_free_ac97_component(struct snd_ac97 *ac97);

#ifdef CONFIG_SND_SOC_AC97_BUS
int snd_soc_set_ac97_ops(struct snd_ac97_bus_ops *ops);
int snd_soc_set_ac97_ops_of_reset(struct snd_ac97_bus_ops *ops,
		struct platform_device *pdev);

extern struct snd_ac97_bus_ops *soc_ac97_ops;
#else
static inline int snd_soc_set_ac97_ops_of_reset(struct snd_ac97_bus_ops *ops,
	struct platform_device *pdev)
{
	return 0;
}

static inline int snd_soc_set_ac97_ops(struct snd_ac97_bus_ops *ops)
{
	return 0;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *Controls
 */
struct snd_kcontrol *snd_soc_cnew(const struct snd_kcontrol_new *_template,
<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_WM5110
				  void *data, const char *long_name,
#else
				  void *data, char *long_name,
#endif
				  const char *prefix);
int snd_soc_add_codec_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, int num_controls);
=======
				  void *data, const char *long_name,
				  const char *prefix);
int snd_soc_add_component_controls(struct snd_soc_component *component,
	const struct snd_kcontrol_new *controls, unsigned int num_controls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_add_card_controls(struct snd_soc_card *soc_card,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_add_dai_controls(struct snd_soc_dai *dai,
	const struct snd_kcontrol_new *controls, int num_controls);
int snd_soc_info_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
<<<<<<< HEAD
int snd_soc_info_enum_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_get_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
int snd_soc_get_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_volsw_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
=======
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_info_volsw_sx(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_info *uinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define snd_soc_info_bool_ext		snd_ctl_boolean_mono_info
int snd_soc_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
#define snd_soc_get_volsw_2r snd_soc_get_volsw
#define snd_soc_put_volsw_2r snd_soc_put_volsw
<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_WM5110
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_get_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
#endif
int snd_soc_info_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
#ifdef CONFIG_SND_SOC_WM5110
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_info_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_put_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
#endif
int snd_soc_limit_volume(struct snd_soc_codec *codec,
	const char *name, int max);
int snd_soc_info_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_volsw_2r_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
#ifdef CONFIG_SND_SOC_WM5110
=======
int snd_soc_limit_volume(struct snd_soc_card *card,
	const char *name, int max);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_bytes_info(struct snd_kcontrol *kcontrol,
		       struct snd_ctl_elem_info *uinfo);
int snd_soc_bytes_get(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
int snd_soc_bytes_put(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol);
<<<<<<< HEAD
#endif

/**
 * struct snd_soc_reg_access - Describes whether a given register is
 * readable, writable or volatile.
 *
 * @reg: the register number
 * @read: whether this register is readable
 * @write: whether this register is writable
 * @vol: whether this register is volatile
 */
struct snd_soc_reg_access {
	u16 reg;
	u16 read;
	u16 write;
	u16 vol;
};

/**
 * struct snd_soc_jack_pin - Describes a pin to update based on jack detection
 *
 * @pin:    name of the pin to update
 * @mask:   bits to check for in reported jack status
 * @invert: if non-zero then pin is enabled when status is not reported
 */
struct snd_soc_jack_pin {
	struct list_head list;
	const char *pin;
	int mask;
	bool invert;
};

/**
 * struct snd_soc_jack_zone - Describes voltage zones of jack detection
 *
 * @min_mv: start voltage in mv
 * @max_mv: end voltage in mv
 * @jack_type: type of jack that is expected for this voltage
 * @debounce_time: debounce_time for jack, codec driver should wait for this
 *		duration before reading the adc for voltages
 * @:list: list container
 */
struct snd_soc_jack_zone {
	unsigned int min_mv;
	unsigned int max_mv;
	unsigned int jack_type;
	unsigned int debounce_time;
	struct list_head list;
};

/**
 * struct snd_soc_jack_gpio - Describes a gpio pin for jack detection
 *
 * @gpio:         gpio number
 * @name:         gpio name
 * @report:       value to report when jack detected
 * @invert:       report presence in low state
 * @debouce_time: debouce time in ms
 * @wake:	  enable as wake source
 * @jack_status_check: callback function which overrides the detection
 *		       to provide more complex checks (eg, reading an
 *		       ADC).
 */
#ifdef CONFIG_GPIOLIB
struct snd_soc_jack_gpio {
	unsigned int gpio;
	const char *name;
	int report;
	int invert;
	int debounce_time;
	bool wake;

	struct snd_soc_jack *jack;
	struct delayed_work work;

	int (*jack_status_check)(void);
};
#endif

struct snd_soc_jack {
	struct mutex mutex;
	struct snd_jack *jack;
	struct snd_soc_codec *codec;
	struct list_head pins;
	int status;
	struct blocking_notifier_head notifier;
	struct list_head jack_zones;
=======
int snd_soc_bytes_info_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *ucontrol);
int snd_soc_bytes_tlv_callback(struct snd_kcontrol *kcontrol, int op_flag,
	unsigned int size, unsigned int __user *tlv);
int snd_soc_info_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo);
int snd_soc_get_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_xr_sx(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_get_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);
int snd_soc_put_strobe(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol);

enum snd_soc_trigger_order {
						/* start			stop		     */
	SND_SOC_TRIGGER_ORDER_DEFAULT	= 0,	/* Link->Component->DAI		DAI->Component->Link */
	SND_SOC_TRIGGER_ORDER_LDC,		/* Link->DAI->Component		Component->DAI->Link */

	SND_SOC_TRIGGER_ORDER_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* SoC PCM stream information */
struct snd_soc_pcm_stream {
	const char *stream_name;
<<<<<<< HEAD
	const char *aif_name;	/* DAPM AIF widget name */
	u64 formats;			/* SNDRV_PCM_FMTBIT_* */
=======
	u64 formats;			/* SNDRV_PCM_FMTBIT_* */
	u32 subformats;			/* for S32_LE format, SNDRV_PCM_SUBFMTBIT_* */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int rates;		/* SNDRV_PCM_RATE_* */
	unsigned int rate_min;		/* min rate */
	unsigned int rate_max;		/* max rate */
	unsigned int channels_min;	/* min channels */
	unsigned int channels_max;	/* max channels */
	unsigned int sig_bits;		/* number of bits of content */
};

/* SoC audio ops */
struct snd_soc_ops {
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*hw_params)(struct snd_pcm_substream *, struct snd_pcm_hw_params *);
	int (*hw_free)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
	int (*trigger)(struct snd_pcm_substream *, int);
};

struct snd_soc_compr_ops {
	int (*startup)(struct snd_compr_stream *);
	void (*shutdown)(struct snd_compr_stream *);
	int (*set_params)(struct snd_compr_stream *);
<<<<<<< HEAD
	int (*trigger)(struct snd_compr_stream *);
};

/* SoC cache ops */
struct snd_soc_cache_ops {
	const char *name;
	enum snd_soc_compress_type id;
	int (*init)(struct snd_soc_codec *codec);
	int (*exit)(struct snd_soc_codec *codec);
	int (*read)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int *value);
	int (*write)(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int value);
	int (*sync)(struct snd_soc_codec *codec);
};

/* SoC Audio Codec device */
struct snd_soc_codec {
	const char *name;
	const char *name_prefix;
	int id;
	struct device *dev;
	const struct snd_soc_codec_driver *driver;

	struct mutex mutex;
	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;
	int num_dai;
	enum snd_soc_compress_type compress_type;
	size_t reg_size;	/* reg_cache_size * reg_word_size */
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);

	/* runtime */
	struct snd_ac97 *ac97;  /* for ad-hoc ac97 devices */
	unsigned int active;
	unsigned int cache_bypass:1; /* Suppress access to the cache */
	unsigned int suspended:1; /* Codec is in suspend PM state */
	unsigned int probed:1; /* Codec has been probed */
	unsigned int ac97_registered:1; /* Codec has been AC97 registered */
	unsigned int ac97_created:1; /* Codec has been created by SoC */
	unsigned int sysfs_registered:1; /* codec has been sysfs registered */
	unsigned int cache_init:1; /* codec cache has been initialized */
	unsigned int using_regmap:1; /* using regmap access */
	u32 cache_only;  /* Suppress writes to hardware */
	u32 cache_sync; /* Cache needs to be synced to hardware */

	/* codec IO */
	void *control_data; /* codec control (i2c/3wire) data */
	enum snd_soc_control_type control_type;
	hw_write_t hw_write;
	unsigned int (*hw_read)(struct snd_soc_codec *, unsigned int);
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
	int (*bulk_write_raw)(struct snd_soc_codec *, unsigned int, const void *, size_t);
	void *reg_cache;
	const void *reg_def_copy;
	const struct snd_soc_cache_ops *cache_ops;
	struct mutex cache_rw_mutex;
	int val_bytes;

	/* dapm */
	struct snd_soc_dapm_context dapm;
	unsigned int ignore_pmdown_time:1; /* pmdown_time is ignored at stop */

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_codec_root;
	struct dentry *debugfs_reg;
	struct dentry *debugfs_dapm;
#endif
};

/* codec driver */
struct snd_soc_codec_driver {

	/* driver ops */
	int (*probe)(struct snd_soc_codec *);
	int (*remove)(struct snd_soc_codec *);
	int (*suspend)(struct snd_soc_codec *);
	int (*resume)(struct snd_soc_codec *);

	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;

	/* codec wide operations */
	int (*set_sysclk)(struct snd_soc_codec *codec,
			  int clk_id, int source, unsigned int freq, int dir);
	int (*set_pll)(struct snd_soc_codec *codec, int pll_id, int source,
		unsigned int freq_in, unsigned int freq_out);

	/* codec IO */
	unsigned int (*read)(struct snd_soc_codec *, unsigned int);
	int (*write)(struct snd_soc_codec *, unsigned int, unsigned int);
	int (*display_register)(struct snd_soc_codec *, char *,
				size_t, unsigned int);
	int (*volatile_register)(struct snd_soc_codec *, unsigned int);
	int (*readable_register)(struct snd_soc_codec *, unsigned int);
	int (*writable_register)(struct snd_soc_codec *, unsigned int);
	unsigned int reg_cache_size;
	short reg_cache_step;
	short reg_word_size;
	const void *reg_cache_default;
	short reg_access_size;
	const struct snd_soc_reg_access *reg_access_default;
	enum snd_soc_compress_type compress_type;

	/* codec bias level */
	int (*set_bias_level)(struct snd_soc_codec *,
			      enum snd_soc_bias_level level);
	bool idle_bias_off;

	void (*seq_notifier)(struct snd_soc_dapm_context *,
			     enum snd_soc_dapm_type, int);

	/* codec stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;
};

/* SoC platform interface */
struct snd_soc_platform_driver {

	int (*probe)(struct snd_soc_platform *);
	int (*remove)(struct snd_soc_platform *);
	int (*suspend)(struct snd_soc_dai *dai);
	int (*resume)(struct snd_soc_dai *dai);

	/* pcm creation and destruction */
	int (*pcm_new)(struct snd_soc_pcm_runtime *);
	void (*pcm_free)(struct snd_pcm *);

	/* Default control and setup, added after probe() is run */
	const struct snd_kcontrol_new *controls;
	int num_controls;
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;

	/*
	 * For platform caused delay reporting.
	 * Optional.
	 */
	snd_pcm_sframes_t (*delay)(struct snd_pcm_substream *,
		struct snd_soc_dai *);

	/* platform stream pcm ops */
	struct snd_pcm_ops *ops;

	/* platform stream compress ops */
	struct snd_compr_ops *compr_ops;

	/* platform stream completion event */
	int (*stream_event)(struct snd_soc_dapm_context *dapm, int event);

	/* probe ordering - for components with runtime dependencies */
	int probe_order;
	int remove_order;

	/* platform IO - used for platform DAPM */
	unsigned int (*read)(struct snd_soc_platform *, unsigned int);
	int (*write)(struct snd_soc_platform *, unsigned int, unsigned int);

	int (*bespoke_trigger)(struct snd_pcm_substream *, int);
};

struct snd_soc_platform {
	const char *name;
	int id;
	struct device *dev;
	struct snd_soc_platform_driver *driver;

	unsigned int suspended:1; /* platform is suspended */
	unsigned int probed:1;

	struct snd_soc_card *card;
	struct list_head list;
	struct list_head card_list;

	struct snd_soc_dapm_context dapm;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_platform_root;
	struct dentry *debugfs_dapm;
#endif
=======
};

struct snd_soc_component*
snd_soc_rtdcom_lookup(struct snd_soc_pcm_runtime *rtd,
		       const char *driver_name);

struct snd_soc_dai_link_component {
	const char *name;
	struct device_node *of_node;
	const char *dai_name;
	struct of_phandle_args *dai_args;
};

/*
 * [dai_link->ch_maps Image sample]
 *
 *-------------------------
 * CPU0 <---> Codec0
 *
 * ch-map[0].cpu = 0	ch-map[0].codec = 0
 *
 *-------------------------
 * CPU0 <---> Codec0
 * CPU1 <---> Codec1
 * CPU2 <---> Codec2
 *
 * ch-map[0].cpu = 0	ch-map[0].codec = 0
 * ch-map[1].cpu = 1	ch-map[1].codec = 1
 * ch-map[2].cpu = 2	ch-map[2].codec = 2
 *
 *-------------------------
 * CPU0 <---> Codec0
 * CPU1 <-+-> Codec1
 * CPU2 <-/
 *
 * ch-map[0].cpu = 0	ch-map[0].codec = 0
 * ch-map[1].cpu = 1	ch-map[1].codec = 1
 * ch-map[2].cpu = 2	ch-map[2].codec = 1
 *
 *-------------------------
 * CPU0 <---> Codec0
 * CPU1 <-+-> Codec1
 *	  \-> Codec2
 *
 * ch-map[0].cpu = 0	ch-map[0].codec = 0
 * ch-map[1].cpu = 1	ch-map[1].codec = 1
 * ch-map[2].cpu = 1	ch-map[2].codec = 2
 *
 */
struct snd_soc_dai_link_ch_map {
	unsigned int cpu;
	unsigned int codec;
	unsigned int ch_mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct snd_soc_dai_link {
	/* config - must be set by machine driver */
	const char *name;			/* Codec name */
	const char *stream_name;		/* Stream name */
<<<<<<< HEAD
	const char *codec_name;		/* for multi-codec */
	const struct device_node *codec_of_node;
	const char *platform_name;	/* for multi-platform */
	const struct device_node *platform_of_node;
	const char *cpu_dai_name;
	const struct device_node *cpu_dai_of_node;
	const char *codec_dai_name;
=======

	/*
	 * You MAY specify the link's CPU-side device, either by device name,
	 * or by DT/OF node, but not both. If this information is omitted,
	 * the CPU-side DAI is matched using .cpu_dai_name only, which hence
	 * must be globally unique. These fields are currently typically used
	 * only for codec to codec links, or systems using device tree.
	 */
	/*
	 * You MAY specify the DAI name of the CPU DAI. If this information is
	 * omitted, the CPU-side DAI is matched using .cpu_name/.cpu_of_node
	 * only, which only works well when that device exposes a single DAI.
	 */
	struct snd_soc_dai_link_component *cpus;
	unsigned int num_cpus;

	/*
	 * You MUST specify the link's codec, either by device name, or by
	 * DT/OF node, but not both.
	 */
	/* You MUST specify the DAI name within the codec */
	struct snd_soc_dai_link_component *codecs;
	unsigned int num_codecs;

	/* num_ch_maps = max(num_cpu, num_codecs) */
	struct snd_soc_dai_link_ch_map *ch_maps;

	/*
	 * You MAY specify the link's platform/PCM/DMA driver, either by
	 * device name, or by DT/OF node, but not both. Some forms of link
	 * do not need a platform. In such case, platforms are not mandatory.
	 */
	struct snd_soc_dai_link_component *platforms;
	unsigned int num_platforms;

	int id;	/* optional ID for machine driver link identification */

	/*
	 * for Codec2Codec
	 */
	const struct snd_soc_pcm_stream *c2c_params;
	unsigned int num_c2c_params;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned int dai_fmt;           /* format to set on init */

	enum snd_soc_dpcm_trigger trigger[2]; /* trigger type for DPCM */

<<<<<<< HEAD
	/* Keep DAI active over suspend */
	unsigned int ignore_suspend:1;

	/* Symmetry requirements */
	unsigned int symmetric_rates:1;
	/* No PCM created for this DAI link */
	unsigned int no_pcm:1;
	/* This DAI link can change CODEC and platform at runtime*/
	unsigned int dynamic:1;
	/* This DAI has a Backend ID */
	unsigned int be_id;
	/* This DAI can support no host IO (no pcm data is copied to from host) */
	unsigned int no_host_mode:2;

	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

	/* hw_params re-writing for BE and FE sync */
=======
	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_pcm_runtime *rtd);

	/* codec/machine specific exit - dual of init() */
	void (*exit)(struct snd_soc_pcm_runtime *rtd);

	/* optional hw_params re-writing for BE and FE sync */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*be_hw_params_fixup)(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params);

	/* machine stream operations */
<<<<<<< HEAD
	struct snd_soc_ops *ops;
	struct snd_soc_compr_ops *compr_ops;
};

struct snd_soc_codec_conf {
	const char *dev_name;
=======
	const struct snd_soc_ops *ops;
	const struct snd_soc_compr_ops *compr_ops;

	/*
	 * soc_pcm_trigger() start/stop sequence.
	 * see also
	 *	snd_soc_component_driver
	 *	soc_pcm_trigger()
	 */
	enum snd_soc_trigger_order trigger_start;
	enum snd_soc_trigger_order trigger_stop;

	/* Mark this pcm with non atomic ops */
	unsigned int nonatomic:1;

	/* For unidirectional dai links */
	unsigned int playback_only:1;
	unsigned int capture_only:1;

	/* Keep DAI active over suspend */
	unsigned int ignore_suspend:1;

	/* Symmetry requirements */
	unsigned int symmetric_rate:1;
	unsigned int symmetric_channels:1;
	unsigned int symmetric_sample_bits:1;

	/* Do not create a PCM for this DAI link (Backend link) */
	unsigned int no_pcm:1;

	/* This DAI link can route to other DAI links at runtime (Frontend)*/
	unsigned int dynamic:1;

	/* DPCM capture and Playback support */
	unsigned int dpcm_capture:1;
	unsigned int dpcm_playback:1;

	/* DPCM used FE & BE merged format */
	unsigned int dpcm_merged_format:1;
	/* DPCM used FE & BE merged channel */
	unsigned int dpcm_merged_chan:1;
	/* DPCM used FE & BE merged rate */
	unsigned int dpcm_merged_rate:1;

	/* pmdown_time is ignored at stop */
	unsigned int ignore_pmdown_time:1;

	/* Do not create a PCM for this DAI link (Backend link) */
	unsigned int ignore:1;

#ifdef CONFIG_SND_SOC_TOPOLOGY
	struct snd_soc_dobj dobj; /* For topology */
#endif
};

static inline int snd_soc_link_num_ch_map(struct snd_soc_dai_link *link) {
	return max(link->num_cpus, link->num_codecs);
}

static inline struct snd_soc_dai_link_component*
snd_soc_link_to_cpu(struct snd_soc_dai_link *link, int n) {
	return &(link)->cpus[n];
}

static inline struct snd_soc_dai_link_component*
snd_soc_link_to_codec(struct snd_soc_dai_link *link, int n) {
	return &(link)->codecs[n];
}

static inline struct snd_soc_dai_link_component*
snd_soc_link_to_platform(struct snd_soc_dai_link *link, int n) {
	return &(link)->platforms[n];
}

#define for_each_link_codecs(link, i, codec)				\
	for ((i) = 0;							\
	     ((i) < link->num_codecs) &&				\
		     ((codec) = snd_soc_link_to_codec(link, i));		\
	     (i)++)

#define for_each_link_platforms(link, i, platform)			\
	for ((i) = 0;							\
	     ((i) < link->num_platforms) &&				\
		     ((platform) = snd_soc_link_to_platform(link, i));	\
	     (i)++)

#define for_each_link_cpus(link, i, cpu)				\
	for ((i) = 0;							\
	     ((i) < link->num_cpus) &&					\
		     ((cpu) = snd_soc_link_to_cpu(link, i));		\
	     (i)++)

#define for_each_link_ch_maps(link, i, ch_map)			\
	for ((i) = 0;						\
	     ((i) < snd_soc_link_num_ch_map(link) &&		\
		      ((ch_map) = link->ch_maps + i));		\
	     (i)++)

/*
 * Sample 1 : Single CPU/Codec/Platform
 *
 * SND_SOC_DAILINK_DEFS(test,
 *	DAILINK_COMP_ARRAY(COMP_CPU("cpu_dai")),
 *	DAILINK_COMP_ARRAY(COMP_CODEC("codec", "codec_dai")),
 *	DAILINK_COMP_ARRAY(COMP_PLATFORM("platform")));
 *
 * struct snd_soc_dai_link link = {
 *	...
 *	SND_SOC_DAILINK_REG(test),
 * };
 *
 * Sample 2 : Multi CPU/Codec, no Platform
 *
 * SND_SOC_DAILINK_DEFS(test,
 *	DAILINK_COMP_ARRAY(COMP_CPU("cpu_dai1"),
 *			   COMP_CPU("cpu_dai2")),
 *	DAILINK_COMP_ARRAY(COMP_CODEC("codec1", "codec_dai1"),
 *			   COMP_CODEC("codec2", "codec_dai2")));
 *
 * struct snd_soc_dai_link link = {
 *	...
 *	SND_SOC_DAILINK_REG(test),
 * };
 *
 * Sample 3 : Define each CPU/Codec/Platform manually
 *
 * SND_SOC_DAILINK_DEF(test_cpu,
 *		DAILINK_COMP_ARRAY(COMP_CPU("cpu_dai1"),
 *				   COMP_CPU("cpu_dai2")));
 * SND_SOC_DAILINK_DEF(test_codec,
 *		DAILINK_COMP_ARRAY(COMP_CODEC("codec1", "codec_dai1"),
 *				   COMP_CODEC("codec2", "codec_dai2")));
 * SND_SOC_DAILINK_DEF(test_platform,
 *		DAILINK_COMP_ARRAY(COMP_PLATFORM("platform")));
 *
 * struct snd_soc_dai_link link = {
 *	...
 *	SND_SOC_DAILINK_REG(test_cpu,
 *			    test_codec,
 *			    test_platform),
 * };
 *
 * Sample 4 : Sample3 without platform
 *
 * struct snd_soc_dai_link link = {
 *	...
 *	SND_SOC_DAILINK_REG(test_cpu,
 *			    test_codec);
 * };
 */

#define SND_SOC_DAILINK_REG1(name)	 SND_SOC_DAILINK_REG3(name##_cpus, name##_codecs, name##_platforms)
#define SND_SOC_DAILINK_REG2(cpu, codec) SND_SOC_DAILINK_REG3(cpu, codec, null_dailink_component)
#define SND_SOC_DAILINK_REG3(cpu, codec, platform)	\
	.cpus		= cpu,				\
	.num_cpus	= ARRAY_SIZE(cpu),		\
	.codecs		= codec,			\
	.num_codecs	= ARRAY_SIZE(codec),		\
	.platforms	= platform,			\
	.num_platforms	= ARRAY_SIZE(platform)

#define SND_SOC_DAILINK_REG(...) \
	CONCATENATE(SND_SOC_DAILINK_REG, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define SND_SOC_DAILINK_DEF(name, def...)		\
	static struct snd_soc_dai_link_component name[]	= { def }

#define SND_SOC_DAILINK_DEFS(name, cpu, codec, platform...)	\
	SND_SOC_DAILINK_DEF(name##_cpus, cpu);			\
	SND_SOC_DAILINK_DEF(name##_codecs, codec);		\
	SND_SOC_DAILINK_DEF(name##_platforms, platform)

#define DAILINK_COMP_ARRAY(param...)	param
#define COMP_EMPTY()			{ }
#define COMP_CPU(_dai)			{ .dai_name = _dai, }
#define COMP_CODEC(_name, _dai)		{ .name = _name, .dai_name = _dai, }
#define COMP_PLATFORM(_name)		{ .name = _name }
#define COMP_AUX(_name)			{ .name = _name }
#define COMP_CODEC_CONF(_name)		{ .name = _name }
#define COMP_DUMMY()			/* see snd_soc_fill_dummy_dai() */

extern struct snd_soc_dai_link_component null_dailink_component[0];
extern struct snd_soc_dai_link_component snd_soc_dummy_dlc;


struct snd_soc_codec_conf {
	/*
	 * specify device either by device name, or by
	 * DT/OF node, but not both.
	 */
	struct snd_soc_dai_link_component dlc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * optional map of kcontrol, widget and path name prefixes that are
	 * associated per device
	 */
	const char *name_prefix;
<<<<<<< HEAD

	/*
	 * set this to the desired compression type if you want to
	 * override the one supplied in codec->driver->compress_type
	 */
	enum snd_soc_compress_type compress_type;
};

struct snd_soc_aux_dev {
	const char *name;		/* Codec name */
	const char *codec_name;		/* for multi-codec */

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_dapm_context *dapm);
=======
};

struct snd_soc_aux_dev {
	/*
	 * specify multi-codec either by device name, or by
	 * DT/OF node, but not both.
	 */
	struct snd_soc_dai_link_component dlc;

	/* codec/machine specific init - e.g. add machine controls */
	int (*init)(struct snd_soc_component *component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* SoC card */
struct snd_soc_card {
	const char *name;
	const char *long_name;
	const char *driver_name;
<<<<<<< HEAD
=======
	const char *components;
#ifdef CONFIG_DMI
	char dmi_longname[80];
#endif /* CONFIG_DMI */

#ifdef CONFIG_PCI
	/*
	 * PCI does not define 0 as invalid, so pci_subsystem_set indicates
	 * whether a value has been written to these fields.
	 */
	unsigned short pci_subsystem_vendor;
	unsigned short pci_subsystem_device;
	bool pci_subsystem_set;
#endif /* CONFIG_PCI */

	char topology_shortname[32];

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device *dev;
	struct snd_card *snd_card;
	struct module *owner;

<<<<<<< HEAD
	struct list_head list;
	struct mutex mutex;
	struct mutex dpcm_mutex;

	struct mutex dapm_mutex;
	struct mutex dapm_power_mutex;
	struct mutex dsp_mutex;
	spinlock_t dsp_spinlock;

	bool instantiated;

	int (*probe)(struct snd_soc_card *card);
	int (*late_probe)(struct snd_soc_card *card);
=======
	struct mutex mutex;
	struct mutex dapm_mutex;

	/* Mutex for PCM operations */
	struct mutex pcm_mutex;
	enum snd_soc_pcm_subclass pcm_subclass;

	int (*probe)(struct snd_soc_card *card);
	int (*late_probe)(struct snd_soc_card *card);
	void (*fixup_controls)(struct snd_soc_card *card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*remove)(struct snd_soc_card *card);

	/* the pre and post PM functions are used to do any PM work before and
	 * after the codec and DAI's do any PM work. */
	int (*suspend_pre)(struct snd_soc_card *card);
	int (*suspend_post)(struct snd_soc_card *card);
	int (*resume_pre)(struct snd_soc_card *card);
	int (*resume_post)(struct snd_soc_card *card);

	/* callbacks */
	int (*set_bias_level)(struct snd_soc_card *,
			      struct snd_soc_dapm_context *dapm,
			      enum snd_soc_bias_level level);
	int (*set_bias_level_post)(struct snd_soc_card *,
				   struct snd_soc_dapm_context *dapm,
				   enum snd_soc_bias_level level);

<<<<<<< HEAD
	long pmdown_time;

	/* CPU <--> Codec DAI links  */
	struct snd_soc_dai_link *dai_link;
	int num_links;
	struct snd_soc_pcm_runtime *rtd;
	int num_rtd;
	int num_playback_channels;
	int num_capture_channels;
=======
	int (*add_dai_link)(struct snd_soc_card *,
			    struct snd_soc_dai_link *link);
	void (*remove_dai_link)(struct snd_soc_card *,
			    struct snd_soc_dai_link *link);

	long pmdown_time;

	/* CPU <--> Codec DAI links  */
	struct snd_soc_dai_link *dai_link;  /* predefined links only */
	int num_links;  /* predefined links only */

	struct list_head rtd_list;
	int num_rtd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* optional codec specific configuration */
	struct snd_soc_codec_conf *codec_conf;
	int num_configs;

	/*
	 * optional auxiliary devices such as amplifiers or codecs with DAI
	 * link unused
	 */
	struct snd_soc_aux_dev *aux_dev;
	int num_aux_devs;
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd_aux;
	int num_aux_rtd;
=======
	struct list_head aux_comp_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	const struct snd_kcontrol_new *controls;
	int num_controls;

	/*
	 * Card-specific routes and widgets.
<<<<<<< HEAD
=======
	 * Note: of_dapm_xxx for Device Tree; Otherwise for driver build-in.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	const struct snd_soc_dapm_widget *dapm_widgets;
	int num_dapm_widgets;
	const struct snd_soc_dapm_route *dapm_routes;
	int num_dapm_routes;
<<<<<<< HEAD
	bool fully_routed;

	struct work_struct deferred_resume_work;

	/* lists of probed devices belonging to this card */
	struct list_head codec_dev_list;
	struct list_head platform_dev_list;
	struct list_head dai_dev_list;
=======
	const struct snd_soc_dapm_widget *of_dapm_widgets;
	int num_of_dapm_widgets;
	const struct snd_soc_dapm_route *of_dapm_routes;
	int num_of_dapm_routes;

	/* lists of probed devices belonging to this card */
	struct list_head component_dev_list;
	struct list_head list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct list_head widgets;
	struct list_head paths;
	struct list_head dapm_list;
	struct list_head dapm_dirty;

<<<<<<< HEAD
	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
	struct snd_soc_dapm_stats dapm_stats;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_card_root;
	struct dentry *debugfs_pop_time;
#endif
	u32 pop_time;

	void *drvdata;
};

/* DSP runtime data */
struct snd_soc_dpcm_runtime {
	struct list_head be_clients;
	struct list_head fe_clients;
	int users;
	struct snd_pcm_runtime *runtime;
	struct snd_pcm_hw_params hw_params;
	int runtime_update;
	enum snd_soc_dpcm_state state;
};
=======
	/* attached dynamic objects */
	struct list_head dobj_list;

	/* Generic DAPM context for the card */
	struct snd_soc_dapm_context dapm;
	struct snd_soc_dapm_stats dapm_stats;
	struct snd_soc_dapm_update *update;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_card_root;
#endif
#ifdef CONFIG_PM_SLEEP
	struct work_struct deferred_resume_work;
#endif
	u32 pop_time;

	/* bit field */
	unsigned int instantiated:1;
	unsigned int topology_shortname_created:1;
	unsigned int fully_routed:1;
	unsigned int disable_route_checks:1;
	unsigned int probed:1;
	unsigned int component_chaining:1;

	void *drvdata;
};
#define for_each_card_prelinks(card, i, link)				\
	for ((i) = 0;							\
	     ((i) < (card)->num_links) && ((link) = &(card)->dai_link[i]); \
	     (i)++)
#define for_each_card_pre_auxs(card, i, aux)				\
	for ((i) = 0;							\
	     ((i) < (card)->num_aux_devs) && ((aux) = &(card)->aux_dev[i]); \
	     (i)++)

#define for_each_card_rtds(card, rtd)			\
	list_for_each_entry(rtd, &(card)->rtd_list, list)
#define for_each_card_rtds_safe(card, rtd, _rtd)	\
	list_for_each_entry_safe(rtd, _rtd, &(card)->rtd_list, list)

#define for_each_card_auxs(card, component)			\
	list_for_each_entry(component, &card->aux_comp_list, card_aux_list)
#define for_each_card_auxs_safe(card, component, _comp)	\
	list_for_each_entry_safe(component, _comp,	\
				 &card->aux_comp_list, card_aux_list)

#define for_each_card_components(card, component)			\
	list_for_each_entry(component, &(card)->component_dev_list, card_list)

#define for_each_card_dapms(card, dapm)					\
	list_for_each_entry(dapm, &card->dapm_list, list)

#define for_each_card_widgets(card, w)\
	list_for_each_entry(w, &card->widgets, list)
#define for_each_card_widgets_safe(card, w, _w)	\
	list_for_each_entry_safe(w, _w, &card->widgets, list)


static inline int snd_soc_card_is_instantiated(struct snd_soc_card *card)
{
	return card && card->instantiated;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* SoC machine DAI configuration, glues a codec and cpu DAI together */
struct snd_soc_pcm_runtime {
	struct device *dev;
	struct snd_soc_card *card;
	struct snd_soc_dai_link *dai_link;
<<<<<<< HEAD
	struct mutex pcm_mutex;
	enum snd_soc_pcm_subclass pcm_subclass;
	struct snd_pcm_ops ops;

	unsigned int dev_registered:1;

	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[2];
	int fe_compr;
=======
	struct snd_pcm_ops ops;

	unsigned int c2c_params_select; /* currently selected c2c_param for dai link */

	/* Dynamic PCM BE runtime data */
	struct snd_soc_dpcm_runtime dpcm[SNDRV_PCM_STREAM_LAST + 1];
	struct snd_soc_dapm_widget *c2c_widget[SNDRV_PCM_STREAM_LAST + 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	long pmdown_time;

	/* runtime devices */
	struct snd_pcm *pcm;
	struct snd_compr *compr;
<<<<<<< HEAD
	struct snd_soc_codec *codec;
	struct snd_soc_platform *platform;
	struct snd_soc_dai *codec_dai;
	struct snd_soc_dai *cpu_dai;

	struct delayed_work delayed_work;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dpcm_root;
	struct dentry *debugfs_dpcm_state;
#endif
};

/* mixer control */
struct soc_mixer_control {
	int min, max, platform_max;
	unsigned int reg, rreg, shift, rshift, invert;
};
struct soc_multi_mixer_control {
	int min, max, platform_max, count;
	unsigned int reg, rreg, shift, rshift, invert;
};

#ifdef CONFIG_SND_SOC_WM5110
/* LGE_CHANGE, yeri.lee@lge.com, 2013-10-31, wm5110 Bring up */
=======

	/*
	 * dais = cpu_dai + codec_dai
	 * see
	 *	soc_new_pcm_runtime()
	 *	snd_soc_rtd_to_cpu()
	 *	snd_soc_rtd_to_codec()
	 */
	struct snd_soc_dai **dais;

	struct delayed_work delayed_work;
	void (*close_delayed_work_func)(struct snd_soc_pcm_runtime *rtd);
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_dpcm_root;
#endif

	unsigned int num; /* 0-based and monotonic increasing */
	struct list_head list; /* rtd list of the soc card */

	/* function mark */
	struct snd_pcm_substream *mark_startup;
	struct snd_pcm_substream *mark_hw_params;
	struct snd_pcm_substream *mark_trigger;
	struct snd_compr_stream  *mark_compr_startup;

	/* bit field */
	unsigned int pop_wait:1;
	unsigned int fe_compr:1; /* for Dynamic PCM */

	bool initialized;

	int num_components;
	struct snd_soc_component *components[]; /* CPU/Codec/Platform */
};

/* see soc_new_pcm_runtime()  */
#define snd_soc_rtd_to_cpu(rtd, n)   (rtd)->dais[n]
#define snd_soc_rtd_to_codec(rtd, n) (rtd)->dais[n + (rtd)->dai_link->num_cpus]
#define snd_soc_substream_to_rtd(substream) \
	(struct snd_soc_pcm_runtime *)snd_pcm_substream_chip(substream)

#define for_each_rtd_components(rtd, i, component)			\
	for ((i) = 0, component = NULL;					\
	     ((i) < rtd->num_components) && ((component) = rtd->components[i]);\
	     (i)++)
#define for_each_rtd_cpu_dais(rtd, i, dai)				\
	for ((i) = 0;							\
	     ((i) < rtd->dai_link->num_cpus) && ((dai) = snd_soc_rtd_to_cpu(rtd, i)); \
	     (i)++)
#define for_each_rtd_codec_dais(rtd, i, dai)				\
	for ((i) = 0;							\
	     ((i) < rtd->dai_link->num_codecs) && ((dai) = snd_soc_rtd_to_codec(rtd, i)); \
	     (i)++)
#define for_each_rtd_dais(rtd, i, dai)					\
	for ((i) = 0;							\
	     ((i) < (rtd)->dai_link->num_cpus + (rtd)->dai_link->num_codecs) &&	\
		     ((dai) = (rtd)->dais[i]);				\
	     (i)++)
#define for_each_rtd_ch_maps(rtd, i, ch_maps) for_each_link_ch_maps(rtd->dai_link, i, ch_maps)

void snd_soc_close_delayed_work(struct snd_soc_pcm_runtime *rtd);

/* mixer control */
struct soc_mixer_control {
	int min, max, platform_max;
	int reg, rreg;
	unsigned int shift, rshift;
	unsigned int sign_bit;
	unsigned int invert:1;
	unsigned int autodisable:1;
#ifdef CONFIG_SND_SOC_TOPOLOGY
	struct snd_soc_dobj dobj;
#endif
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct soc_bytes {
	int base;
	int num_regs;
	u32 mask;
};
<<<<<<< HEAD
#endif

/* enumerated kcontrol */
struct soc_enum {
	unsigned short reg;
	unsigned short reg2;
	unsigned char shift_l;
	unsigned char shift_r;
	unsigned int max;
	unsigned int mask;
	const char * const *texts;
	char **dtexts;
	const unsigned int *values;
	void *dapm;
};

/* codec IO */
unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg);
unsigned int snd_soc_write(struct snd_soc_codec *codec,
			   unsigned int reg, unsigned int val);
unsigned int snd_soc_bulk_write_raw(struct snd_soc_codec *codec,
				    unsigned int reg, const void *data, size_t len);

/* device driver data */

static inline void snd_soc_card_set_drvdata(struct snd_soc_card *card,
		void *data)
{
	card->drvdata = data;
}

static inline void *snd_soc_card_get_drvdata(struct snd_soc_card *card)
{
	return card->drvdata;
}

static inline void snd_soc_codec_set_drvdata(struct snd_soc_codec *codec,
		void *data)
{
	dev_set_drvdata(codec->dev, data);
}

static inline void *snd_soc_codec_get_drvdata(struct snd_soc_codec *codec)
{
	return dev_get_drvdata(codec->dev);
}

static inline void snd_soc_platform_set_drvdata(struct snd_soc_platform *platform,
		void *data)
{
	dev_set_drvdata(platform->dev, data);
}

static inline void *snd_soc_platform_get_drvdata(struct snd_soc_platform *platform)
{
	return dev_get_drvdata(platform->dev);
}

static inline void snd_soc_pcm_set_drvdata(struct snd_soc_pcm_runtime *rtd,
		void *data)
{
	dev_set_drvdata(rtd->dev, data);
}

static inline void *snd_soc_pcm_get_drvdata(struct snd_soc_pcm_runtime *rtd)
{
	return dev_get_drvdata(rtd->dev);
}

static inline void snd_soc_initialize_card_lists(struct snd_soc_card *card)
{
	INIT_LIST_HEAD(&card->dai_dev_list);
	INIT_LIST_HEAD(&card->codec_dev_list);
	INIT_LIST_HEAD(&card->platform_dev_list);
	INIT_LIST_HEAD(&card->widgets);
	INIT_LIST_HEAD(&card->paths);
	INIT_LIST_HEAD(&card->dapm_list);
}

static inline bool snd_soc_volsw_is_stereo(struct soc_mixer_control *mc)
{
	if (mc->reg == mc->rreg && mc->shift == mc->rshift)
		return 0;
=======

struct soc_bytes_ext {
	int max;
#ifdef CONFIG_SND_SOC_TOPOLOGY
	struct snd_soc_dobj dobj;
#endif
	/* used for TLV byte control */
	int (*get)(struct snd_kcontrol *kcontrol, unsigned int __user *bytes,
			unsigned int size);
	int (*put)(struct snd_kcontrol *kcontrol, const unsigned int __user *bytes,
			unsigned int size);
};

/* multi register control */
struct soc_mreg_control {
	long min, max;
	unsigned int regbase, regcount, nbits, invert;
};

/* enumerated kcontrol */
struct soc_enum {
	int reg;
	unsigned char shift_l;
	unsigned char shift_r;
	unsigned int items;
	unsigned int mask;
	const char * const *texts;
	const unsigned int *values;
	unsigned int autodisable:1;
#ifdef CONFIG_SND_SOC_TOPOLOGY
	struct snd_soc_dobj dobj;
#endif
};

static inline bool snd_soc_volsw_is_stereo(struct soc_mixer_control *mc)
{
	if (mc->reg == mc->rreg && mc->shift == mc->rshift)
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * mc->reg == mc->rreg && mc->shift != mc->rshift, or
	 * mc->reg != mc->rreg means that the control is
	 * stereo (bits in one register or in two registers)
	 */
<<<<<<< HEAD
	return 1;
=======
	return true;
}

static inline unsigned int snd_soc_enum_val_to_item(struct soc_enum *e,
	unsigned int val)
{
	unsigned int i;

	if (!e->values)
		return val;

	for (i = 0; i < e->items; i++)
		if (val == e->values[i])
			return i;

	return 0;
}

static inline unsigned int snd_soc_enum_item_to_val(struct soc_enum *e,
	unsigned int item)
{
	if (!e->values)
		return item;

	return e->values[item];
}

/**
 * snd_soc_kcontrol_component() - Returns the component that registered the
 *  control
 * @kcontrol: The control for which to get the component
 *
 * Note: This function will work correctly if the control has been registered
 * for a component. With snd_soc_add_codec_controls() or via table based
 * setup for either a CODEC or component driver. Otherwise the behavior is
 * undefined.
 */
static inline struct snd_soc_component *snd_soc_kcontrol_component(
	struct snd_kcontrol *kcontrol)
{
	return snd_kcontrol_chip(kcontrol);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int snd_soc_util_init(void);
void snd_soc_util_exit(void);

int snd_soc_of_parse_card_name(struct snd_soc_card *card,
			       const char *propname);
<<<<<<< HEAD
int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname);

#include <sound/soc-dai.h>

=======
int snd_soc_of_parse_audio_simple_widgets(struct snd_soc_card *card,
					  const char *propname);
int snd_soc_of_parse_pin_switches(struct snd_soc_card *card, const char *prop);
int snd_soc_of_get_slot_mask(struct device_node *np,
			     const char *prop_name,
			     unsigned int *mask);
int snd_soc_of_parse_tdm_slot(struct device_node *np,
			      unsigned int *tx_mask,
			      unsigned int *rx_mask,
			      unsigned int *slots,
			      unsigned int *slot_width);
void snd_soc_of_parse_node_prefix(struct device_node *np,
				   struct snd_soc_codec_conf *codec_conf,
				   struct device_node *of_node,
				   const char *propname);
static inline
void snd_soc_of_parse_audio_prefix(struct snd_soc_card *card,
				   struct snd_soc_codec_conf *codec_conf,
				   struct device_node *of_node,
				   const char *propname)
{
	snd_soc_of_parse_node_prefix(card->dev->of_node,
				     codec_conf, of_node, propname);
}

int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname);
int snd_soc_of_parse_aux_devs(struct snd_soc_card *card, const char *propname);

unsigned int snd_soc_daifmt_clock_provider_flipped(unsigned int dai_fmt);
unsigned int snd_soc_daifmt_clock_provider_from_bitmap(unsigned int bit_frame);

unsigned int snd_soc_daifmt_parse_format(struct device_node *np, const char *prefix);
unsigned int snd_soc_daifmt_parse_clock_provider_raw(struct device_node *np,
						     const char *prefix,
						     struct device_node **bitclkmaster,
						     struct device_node **framemaster);
#define snd_soc_daifmt_parse_clock_provider_as_bitmap(np, prefix)	\
	snd_soc_daifmt_parse_clock_provider_raw(np, prefix, NULL, NULL)
#define snd_soc_daifmt_parse_clock_provider_as_phandle			\
	snd_soc_daifmt_parse_clock_provider_raw
#define snd_soc_daifmt_parse_clock_provider_as_flag(np, prefix)		\
	snd_soc_daifmt_clock_provider_from_bitmap(			\
		snd_soc_daifmt_parse_clock_provider_as_bitmap(np, prefix))

int snd_soc_get_stream_cpu(struct snd_soc_dai_link *dai_link, int stream);
int snd_soc_get_dlc(const struct of_phandle_args *args,
		    struct snd_soc_dai_link_component *dlc);
int snd_soc_of_get_dlc(struct device_node *of_node,
		       struct of_phandle_args *args,
		       struct snd_soc_dai_link_component *dlc,
		       int index);
int snd_soc_get_dai_id(struct device_node *ep);
int snd_soc_get_dai_name(const struct of_phandle_args *args,
			 const char **dai_name);
int snd_soc_of_get_dai_name(struct device_node *of_node,
			    const char **dai_name, int index);
int snd_soc_of_get_dai_link_codecs(struct device *dev,
				   struct device_node *of_node,
				   struct snd_soc_dai_link *dai_link);
void snd_soc_of_put_dai_link_codecs(struct snd_soc_dai_link *dai_link);
int snd_soc_of_get_dai_link_cpus(struct device *dev,
				 struct device_node *of_node,
				 struct snd_soc_dai_link *dai_link);
void snd_soc_of_put_dai_link_cpus(struct snd_soc_dai_link *dai_link);

int snd_soc_add_pcm_runtimes(struct snd_soc_card *card,
			     struct snd_soc_dai_link *dai_link,
			     int num_dai_link);
void snd_soc_remove_pcm_runtime(struct snd_soc_card *card,
				struct snd_soc_pcm_runtime *rtd);

void snd_soc_dlc_use_cpu_as_platform(struct snd_soc_dai_link_component *platforms,
				     struct snd_soc_dai_link_component *cpus);
struct of_phandle_args *snd_soc_copy_dai_args(struct device *dev,
					      const struct of_phandle_args *args);
struct snd_soc_dai *snd_soc_get_dai_via_args(const struct of_phandle_args *dai_args);
struct snd_soc_dai *snd_soc_register_dai(struct snd_soc_component *component,
					 struct snd_soc_dai_driver *dai_drv,
					 bool legacy_dai_naming);
struct snd_soc_dai *devm_snd_soc_register_dai(struct device *dev,
					      struct snd_soc_component *component,
					      struct snd_soc_dai_driver *dai_drv,
					      bool legacy_dai_naming);
void snd_soc_unregister_dai(struct snd_soc_dai *dai);

struct snd_soc_dai *snd_soc_find_dai(
	const struct snd_soc_dai_link_component *dlc);
struct snd_soc_dai *snd_soc_find_dai_with_mutex(
	const struct snd_soc_dai_link_component *dlc);

#include <sound/soc-dai.h>

static inline
int snd_soc_fixup_dai_links_platform_name(struct snd_soc_card *card,
					  const char *platform_name)
{
	struct snd_soc_dai_link *dai_link;
	const char *name;
	int i;

	if (!platform_name) /* nothing to do */
		return 0;

	/* set platform name for each dailink */
	for_each_card_prelinks(card, i, dai_link) {
		/* only single platform is supported for now */
		if (dai_link->num_platforms != 1)
			return -EINVAL;

		if (!dai_link->platforms)
			return -EINVAL;

		name = devm_kstrdup(card->dev, platform_name, GFP_KERNEL);
		if (!name)
			return -ENOMEM;

		/* only single platform is supported for now */
		dai_link->platforms->name = name;
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_DEBUG_FS
extern struct dentry *snd_soc_debugfs_root;
#endif

extern const struct dev_pm_ops snd_soc_pm_ops;

<<<<<<< HEAD
=======
/*
 *	DAPM helper functions
 */
enum snd_soc_dapm_subclass {
	SND_SOC_DAPM_CLASS_ROOT		= 0,
	SND_SOC_DAPM_CLASS_RUNTIME	= 1,
};

static inline void _snd_soc_dapm_mutex_lock_root_c(struct snd_soc_card *card)
{
	mutex_lock_nested(&card->dapm_mutex, SND_SOC_DAPM_CLASS_ROOT);
}

static inline void _snd_soc_dapm_mutex_lock_c(struct snd_soc_card *card)
{
	mutex_lock_nested(&card->dapm_mutex, SND_SOC_DAPM_CLASS_RUNTIME);
}

static inline void _snd_soc_dapm_mutex_unlock_c(struct snd_soc_card *card)
{
	mutex_unlock(&card->dapm_mutex);
}

static inline void _snd_soc_dapm_mutex_assert_held_c(struct snd_soc_card *card)
{
	lockdep_assert_held(&card->dapm_mutex);
}

static inline void _snd_soc_dapm_mutex_lock_root_d(struct snd_soc_dapm_context *dapm)
{
	_snd_soc_dapm_mutex_lock_root_c(dapm->card);
}

static inline void _snd_soc_dapm_mutex_lock_d(struct snd_soc_dapm_context *dapm)
{
	_snd_soc_dapm_mutex_lock_c(dapm->card);
}

static inline void _snd_soc_dapm_mutex_unlock_d(struct snd_soc_dapm_context *dapm)
{
	_snd_soc_dapm_mutex_unlock_c(dapm->card);
}

static inline void _snd_soc_dapm_mutex_assert_held_d(struct snd_soc_dapm_context *dapm)
{
	_snd_soc_dapm_mutex_assert_held_c(dapm->card);
}

#define snd_soc_dapm_mutex_lock_root(x) _Generic((x),			\
	struct snd_soc_card * :		_snd_soc_dapm_mutex_lock_root_c, \
	struct snd_soc_dapm_context * :	_snd_soc_dapm_mutex_lock_root_d)(x)
#define snd_soc_dapm_mutex_lock(x) _Generic((x),			\
	struct snd_soc_card * :		_snd_soc_dapm_mutex_lock_c,	\
	struct snd_soc_dapm_context * :	_snd_soc_dapm_mutex_lock_d)(x)
#define snd_soc_dapm_mutex_unlock(x) _Generic((x),			\
	struct snd_soc_card * :		_snd_soc_dapm_mutex_unlock_c,	\
	struct snd_soc_dapm_context * :	_snd_soc_dapm_mutex_unlock_d)(x)
#define snd_soc_dapm_mutex_assert_held(x) _Generic((x),			\
	struct snd_soc_card * :		_snd_soc_dapm_mutex_assert_held_c, \
	struct snd_soc_dapm_context * :	_snd_soc_dapm_mutex_assert_held_d)(x)

/*
 *	PCM helper functions
 */
static inline void _snd_soc_dpcm_mutex_lock_c(struct snd_soc_card *card)
{
	mutex_lock_nested(&card->pcm_mutex, card->pcm_subclass);
}

static inline void _snd_soc_dpcm_mutex_unlock_c(struct snd_soc_card *card)
{
	mutex_unlock(&card->pcm_mutex);
}

static inline void _snd_soc_dpcm_mutex_assert_held_c(struct snd_soc_card *card)
{
	lockdep_assert_held(&card->pcm_mutex);
}

static inline void _snd_soc_dpcm_mutex_lock_r(struct snd_soc_pcm_runtime *rtd)
{
	_snd_soc_dpcm_mutex_lock_c(rtd->card);
}

static inline void _snd_soc_dpcm_mutex_unlock_r(struct snd_soc_pcm_runtime *rtd)
{
	_snd_soc_dpcm_mutex_unlock_c(rtd->card);
}

static inline void _snd_soc_dpcm_mutex_assert_held_r(struct snd_soc_pcm_runtime *rtd)
{
	_snd_soc_dpcm_mutex_assert_held_c(rtd->card);
}

#define snd_soc_dpcm_mutex_lock(x) _Generic((x),			\
	 struct snd_soc_card * :	_snd_soc_dpcm_mutex_lock_c,	\
	 struct snd_soc_pcm_runtime * :	_snd_soc_dpcm_mutex_lock_r)(x)

#define snd_soc_dpcm_mutex_unlock(x) _Generic((x),			\
	 struct snd_soc_card * :	_snd_soc_dpcm_mutex_unlock_c,	\
	 struct snd_soc_pcm_runtime * :	_snd_soc_dpcm_mutex_unlock_r)(x)

#define snd_soc_dpcm_mutex_assert_held(x) _Generic((x),		\
	struct snd_soc_card * :		_snd_soc_dpcm_mutex_assert_held_c, \
	struct snd_soc_pcm_runtime * :	_snd_soc_dpcm_mutex_assert_held_r)(x)

#include <sound/soc-component.h>
#include <sound/soc-card.h>
#include <sound/soc-jack.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
