<<<<<<< HEAD
/*
 * soc-core.c  --  ALSA SoC Audio Layer
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Copyright 2005 Openedhand Ltd.
 * Copyright (C) 2010 Slimlogic Ltd.
 * Copyright (C) 2010 Texas Instruments Inc.
 *
 * Author: Liam Girdwood <lrg@slimlogic.co.uk>
 *         with code, comments and ideas from :-
 *         Richard Purdie <richard@openedhand.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  TODO:
 *   o Add hw rules to enforce rates, etc.
 *   o More testing with other codecs/machines.
 *   o Add more codecs and platforms to ensure good API coverage.
 *   o Support TDM on PCM and I2S
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// soc-core.c  --  ALSA SoC Audio Layer
//
// Copyright 2005 Wolfson Microelectronics PLC.
// Copyright 2005 Openedhand Ltd.
// Copyright (C) 2010 Slimlogic Ltd.
// Copyright (C) 2010 Texas Instruments Inc.
//
// Author: Liam Girdwood <lrg@slimlogic.co.uk>
//         with code, comments and ideas from :-
//         Richard Purdie <richard@openedhand.com>
//
//  TODO:
//   o Add hw rules to enforce rates, etc.
//   o More testing with other codecs/machines.
//   o Add more codecs and platforms to ensure good API coverage.
//   o Support TDM on PCM and I2S
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/bitops.h>
#include <linux/debugfs.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <sound/ac97_codec.h>
#include <sound/core.h>
#include <sound/jack.h>
=======
#include <linux/pinctrl/consumer.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_graph.h>
#include <linux/dmi.h>
#include <linux/acpi.h>
#include <sound/core.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dpcm.h>
<<<<<<< HEAD
=======
#include <sound/soc-topology.h>
#include <sound/soc-link.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/initval.h>

#define CREATE_TRACE_POINTS
#include <trace/events/asoc.h>

<<<<<<< HEAD
#define NAME_SIZE	32

static DECLARE_WAIT_QUEUE_HEAD(soc_pm_waitq);

#ifdef CONFIG_DEBUG_FS
struct dentry *snd_soc_debugfs_root;
EXPORT_SYMBOL_GPL(snd_soc_debugfs_root);
#endif

static DEFINE_MUTEX(client_mutex);
static LIST_HEAD(dai_list);
static LIST_HEAD(platform_list);
static LIST_HEAD(codec_list);

int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num);
int soc_dpcm_debugfs_add(struct snd_soc_pcm_runtime *rtd);
int soc_dpcm_be_digital_mute(struct snd_soc_pcm_runtime *fe, int mute);
int soc_dpcm_be_ac97_cpu_dai_suspend(struct snd_soc_pcm_runtime *fe);
int soc_dpcm_be_ac97_cpu_dai_resume(struct snd_soc_pcm_runtime *fe);
int soc_dpcm_be_cpu_dai_resume(struct snd_soc_pcm_runtime *fe);
int soc_dpcm_be_cpu_dai_suspend(struct snd_soc_pcm_runtime *fe);
int soc_dpcm_be_platform_suspend(struct snd_soc_pcm_runtime *fe);
int soc_dpcm_be_platform_resume(struct snd_soc_pcm_runtime *fe);
=======
static DEFINE_MUTEX(client_mutex);
static LIST_HEAD(component_list);
static LIST_HEAD(unbind_card_list);

#define for_each_component(component)			\
	list_for_each_entry(component, &component_list, list)

/*
 * This is used if driver don't need to have CPU/Codec/Platform
 * dai_link. see soc.h
 */
struct snd_soc_dai_link_component null_dailink_component[0];
EXPORT_SYMBOL_GPL(null_dailink_component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is a timeout to do a DAPM powerdown after a stream is closed().
 * It can be used to eliminate pops between different playback streams, e.g.
 * between two audio tracks.
 */
<<<<<<< HEAD
static int pmdown_time;
module_param(pmdown_time, int, 0);
MODULE_PARM_DESC(pmdown_time, "DAPM stream powerdown time (msecs)");

/* returns the minimum number of bytes needed to represent
 * a particular given value */
static int min_bytes_needed(unsigned long val)
{
	int c = 0;
	int i;

	for (i = (sizeof val * 8) - 1; i >= 0; --i, ++c)
		if (val & (1UL << i))
			break;
	c = (sizeof val * 8) - c;
	if (!c || (c % 8))
		c = (c + 8) / 8;
	else
		c /= 8;
	return c;
}

/* fill buf which is 'len' bytes with a formatted
 * string of the form 'reg: value\n' */
static int format_register_str(struct snd_soc_codec *codec,
			       unsigned int reg, char *buf, size_t len)
{
	int wordsize = min_bytes_needed(codec->driver->reg_cache_size) * 2;
	int regsize = codec->driver->reg_word_size * 2;
	int ret;
	char tmpbuf[len + 1];
	char regbuf[regsize + 1];

	/* since tmpbuf is allocated on the stack, warn the callers if they
	 * try to abuse this function */
	WARN_ON(len > 63);

	/* +2 for ': ' and + 1 for '\n' */
	if (wordsize + regsize + 2 + 1 != len)
		return -EINVAL;

	ret = snd_soc_read(codec, reg);
	if (ret < 0) {
		memset(regbuf, 'X', regsize);
		regbuf[regsize] = '\0';
	} else {
		snprintf(regbuf, regsize + 1, "%.*x", regsize, ret);
	}

	/* prepare the buffer */
	snprintf(tmpbuf, len + 1, "%.*x: %s\n", wordsize, reg, regbuf);
	/* copy it back to the caller without the '\0' */
	memcpy(buf, tmpbuf, len);

	return 0;
}

/* codec register dump */
static ssize_t soc_codec_reg_show(struct snd_soc_codec *codec, char *buf,
				  size_t count, loff_t pos)
{
	int i, step = 1;
	int wordsize, regsize;
	int len;
	size_t total = 0;
	loff_t p = 0;

	wordsize = min_bytes_needed(codec->driver->reg_cache_size) * 2;
	regsize = codec->driver->reg_word_size * 2;

	len = wordsize + regsize + 2 + 1;

	if (!codec->driver->reg_cache_size)
		return 0;

	if (codec->driver->reg_cache_step)
		step = codec->driver->reg_cache_step;

	for (i = 0; i < codec->driver->reg_cache_size; i += step) {
		if (!snd_soc_codec_readable_register(codec, i))
			continue;
		if (codec->driver->display_register) {
			count += codec->driver->display_register(codec, buf + count,
							 PAGE_SIZE - count, i);
		} else {
			/* only support larger than PAGE_SIZE bytes debugfs
			 * entries for the default case */
			if (p >= pos) {
				if (total + len >= count - 1)
					break;
				format_register_str(codec, i, buf + total, len);
				total += len;
			}
			p += len;
		}
	}

	total = min(total, count - 1);

	return total;
}

static ssize_t codec_reg_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct snd_soc_pcm_runtime *rtd = dev_get_drvdata(dev);

	return soc_codec_reg_show(rtd->codec, buf, PAGE_SIZE, 0);
}

static DEVICE_ATTR(codec_reg, 0444, codec_reg_show, NULL);

=======
static int pmdown_time = 5000;
module_param(pmdown_time, int, 0);
MODULE_PARM_DESC(pmdown_time, "DAPM stream powerdown time (msecs)");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t pmdown_time_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct snd_soc_pcm_runtime *rtd = dev_get_drvdata(dev);

<<<<<<< HEAD
	return sprintf(buf, "%ld\n", rtd->pmdown_time);
}

static ssize_t pmdown_time_set(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
=======
	return sysfs_emit(buf, "%ld\n", rtd->pmdown_time);
}

static ssize_t pmdown_time_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct snd_soc_pcm_runtime *rtd = dev_get_drvdata(dev);
	int ret;

<<<<<<< HEAD
	ret = strict_strtol(buf, 10, &rtd->pmdown_time);
=======
	ret = kstrtol(buf, 10, &rtd->pmdown_time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	return count;
}

<<<<<<< HEAD
static DEVICE_ATTR(pmdown_time, 0644, pmdown_time_show, pmdown_time_set);

#ifdef CONFIG_DEBUG_FS
static int codec_reg_open_file(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

static ssize_t codec_reg_read_file(struct file *file, char __user *user_buf,
				   size_t count, loff_t *ppos)
{
	ssize_t ret;
	struct snd_soc_codec *codec = file->private_data;
	char *buf;

	if (*ppos < 0 || !count)
		return -EINVAL;

	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	ret = soc_codec_reg_show(codec, buf, count, *ppos);
	if (ret >= 0) {
		if (copy_to_user(user_buf, buf, ret)) {
			kfree(buf);
			return -EFAULT;
		}
		*ppos += ret;
	}

	kfree(buf);
	return ret;
}

static ssize_t codec_reg_write_file(struct file *file,
		const char __user *user_buf, size_t count, loff_t *ppos)
{
	char buf[32];
	size_t buf_size;
	char *start = buf;
	unsigned long reg, value;
	struct snd_soc_codec *codec = file->private_data;

	buf_size = min(count, (sizeof(buf)-1));
	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;
	buf[buf_size] = 0;

	while (*start == ' ')
		start++;
	reg = simple_strtoul(start, &start, 16);
	while (*start == ' ')
		start++;
	if (strict_strtoul(start, 16, &value))
		return -EINVAL;

	/* Userspace has been fiddling around behind the kernel's back */
	add_taint(TAINT_USER);

	snd_soc_write(codec, reg, value);
	return buf_size;
}

static const struct file_operations codec_reg_fops = {
	.open = codec_reg_open_file,
	.read = codec_reg_read_file,
	.write = codec_reg_write_file,
	.llseek = default_llseek,
};

static void soc_init_codec_debugfs(struct snd_soc_codec *codec)
{
	struct dentry *debugfs_card_root = codec->card->debugfs_card_root;

	codec->debugfs_codec_root = debugfs_create_dir(codec->name,
						       debugfs_card_root);
	if (!codec->debugfs_codec_root) {
		printk(KERN_WARNING
		       "ASoC: Failed to create codec debugfs directory\n");
		return;
	}

	debugfs_create_bool("cache_sync", 0444, codec->debugfs_codec_root,
			    &codec->cache_sync);
	debugfs_create_bool("cache_only", 0444, codec->debugfs_codec_root,
			    &codec->cache_only);

	codec->debugfs_reg = debugfs_create_file("codec_reg", 0644,
						 codec->debugfs_codec_root,
						 codec, &codec_reg_fops);
	if (!codec->debugfs_reg)
		printk(KERN_WARNING
		       "ASoC: Failed to create codec register debugfs file\n");

	snd_soc_dapm_debugfs_init(&codec->dapm, codec->debugfs_codec_root);
}

static void soc_cleanup_codec_debugfs(struct snd_soc_codec *codec)
{
	debugfs_remove_recursive(codec->debugfs_codec_root);
}

static void soc_init_platform_debugfs(struct snd_soc_platform *platform)
{
	struct dentry *debugfs_card_root = platform->card->debugfs_card_root;

	platform->debugfs_platform_root = debugfs_create_dir(platform->name,
						       debugfs_card_root);
	if (!platform->debugfs_platform_root) {
		printk(KERN_WARNING
		       "ASoC: Failed to create platform debugfs directory\n");
		return;
	}

	snd_soc_dapm_debugfs_init(&platform->dapm, platform->debugfs_platform_root);
}

static void soc_cleanup_platform_debugfs(struct snd_soc_platform *platform)
{
	debugfs_remove_recursive(platform->debugfs_platform_root);
}

static ssize_t codec_list_read_file(struct file *file, char __user *user_buf,
				    size_t count, loff_t *ppos)
{
	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	ssize_t len, ret = 0;
	struct snd_soc_codec *codec;

	if (!buf)
		return -ENOMEM;

	list_for_each_entry(codec, &codec_list, list) {
		len = snprintf(buf + ret, PAGE_SIZE - ret, "%s\n",
			       codec->name);
		if (len >= 0)
			ret += len;
		if (ret > PAGE_SIZE) {
			ret = PAGE_SIZE;
			break;
		}
	}

	if (ret >= 0)
		ret = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

	kfree(buf);

	return ret;
}

static const struct file_operations codec_list_fops = {
	.read = codec_list_read_file,
	.llseek = default_llseek,/* read accesses f_pos */
};

static ssize_t dai_list_read_file(struct file *file, char __user *user_buf,
				  size_t count, loff_t *ppos)
{
	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	ssize_t len, ret = 0;
	struct snd_soc_dai *dai;

	if (!buf)
		return -ENOMEM;

	list_for_each_entry(dai, &dai_list, list) {
		len = snprintf(buf + ret, PAGE_SIZE - ret, "%s\n", dai->name);
		if (len >= 0)
			ret += len;
		if (ret > PAGE_SIZE) {
			ret = PAGE_SIZE;
			break;
		}
	}

	ret = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

	kfree(buf);

	return ret;
}

static const struct file_operations dai_list_fops = {
	.read = dai_list_read_file,
	.llseek = default_llseek,/* read accesses f_pos */
};

static ssize_t platform_list_read_file(struct file *file,
				       char __user *user_buf,
				       size_t count, loff_t *ppos)
{
	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	ssize_t len, ret = 0;
	struct snd_soc_platform *platform;

	if (!buf)
		return -ENOMEM;

	list_for_each_entry(platform, &platform_list, list) {
		len = snprintf(buf + ret, PAGE_SIZE - ret, "%s\n",
			       platform->name);
		if (len >= 0)
			ret += len;
		if (ret > PAGE_SIZE) {
			ret = PAGE_SIZE;
			break;
		}
	}

	ret = simple_read_from_buffer(user_buf, count, ppos, buf, ret);

	kfree(buf);

	return ret;
}

static const struct file_operations platform_list_fops = {
	.read = platform_list_read_file,
	.llseek = default_llseek,/* read accesses f_pos */
};
=======
static DEVICE_ATTR_RW(pmdown_time);

static struct attribute *soc_dev_attrs[] = {
	&dev_attr_pmdown_time.attr,
	NULL
};

static umode_t soc_dev_attr_is_visible(struct kobject *kobj,
				       struct attribute *attr, int idx)
{
	struct device *dev = kobj_to_dev(kobj);
	struct snd_soc_pcm_runtime *rtd = dev_get_drvdata(dev);

	if (!rtd)
		return 0;

	if (attr == &dev_attr_pmdown_time.attr)
		return attr->mode; /* always visible */
	return rtd->dai_link->num_codecs ? attr->mode : 0; /* enabled only with codec */
}

static const struct attribute_group soc_dapm_dev_group = {
	.attrs = soc_dapm_dev_attrs,
	.is_visible = soc_dev_attr_is_visible,
};

static const struct attribute_group soc_dev_group = {
	.attrs = soc_dev_attrs,
	.is_visible = soc_dev_attr_is_visible,
};

static const struct attribute_group *soc_dev_attr_groups[] = {
	&soc_dapm_dev_group,
	&soc_dev_group,
	NULL
};

#ifdef CONFIG_DEBUG_FS
struct dentry *snd_soc_debugfs_root;
EXPORT_SYMBOL_GPL(snd_soc_debugfs_root);

static void soc_init_component_debugfs(struct snd_soc_component *component)
{
	if (!component->card->debugfs_card_root)
		return;

	if (component->debugfs_prefix) {
		char *name;

		name = kasprintf(GFP_KERNEL, "%s:%s",
			component->debugfs_prefix, component->name);
		if (name) {
			component->debugfs_root = debugfs_create_dir(name,
				component->card->debugfs_card_root);
			kfree(name);
		}
	} else {
		component->debugfs_root = debugfs_create_dir(component->name,
				component->card->debugfs_card_root);
	}

	snd_soc_dapm_debugfs_init(snd_soc_component_get_dapm(component),
		component->debugfs_root);
}

static void soc_cleanup_component_debugfs(struct snd_soc_component *component)
{
	if (!component->debugfs_root)
		return;
	debugfs_remove_recursive(component->debugfs_root);
	component->debugfs_root = NULL;
}

static int dai_list_show(struct seq_file *m, void *v)
{
	struct snd_soc_component *component;
	struct snd_soc_dai *dai;

	mutex_lock(&client_mutex);

	for_each_component(component)
		for_each_component_dais(component, dai)
			seq_printf(m, "%s\n", dai->name);

	mutex_unlock(&client_mutex);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dai_list);

static int component_list_show(struct seq_file *m, void *v)
{
	struct snd_soc_component *component;

	mutex_lock(&client_mutex);

	for_each_component(component)
		seq_printf(m, "%s\n", component->name);

	mutex_unlock(&client_mutex);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(component_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void soc_init_card_debugfs(struct snd_soc_card *card)
{
	card->debugfs_card_root = debugfs_create_dir(card->name,
						     snd_soc_debugfs_root);
<<<<<<< HEAD
	if (!card->debugfs_card_root) {
		dev_warn(card->dev,
			 "ASoC: Failed to create card debugfs directory\n");
		return;
	}

	card->debugfs_pop_time = debugfs_create_u32("dapm_pop_time", 0644,
						    card->debugfs_card_root,
						    &card->pop_time);
	if (!card->debugfs_pop_time)
		dev_warn(card->dev,
		       "Failed to create pop time debugfs file\n");
=======

	debugfs_create_u32("dapm_pop_time", 0644, card->debugfs_card_root,
			   &card->pop_time);

	snd_soc_dapm_debugfs_init(&card->dapm, card->debugfs_card_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void soc_cleanup_card_debugfs(struct snd_soc_card *card)
{
	debugfs_remove_recursive(card->debugfs_card_root);
<<<<<<< HEAD
=======
	card->debugfs_card_root = NULL;
}

static void snd_soc_debugfs_init(void)
{
	snd_soc_debugfs_root = debugfs_create_dir("asoc", NULL);

	debugfs_create_file("dais", 0444, snd_soc_debugfs_root, NULL,
			    &dai_list_fops);

	debugfs_create_file("components", 0444, snd_soc_debugfs_root, NULL,
			    &component_list_fops);
}

static void snd_soc_debugfs_exit(void)
{
	debugfs_remove_recursive(snd_soc_debugfs_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else

<<<<<<< HEAD
static inline void soc_init_codec_debugfs(struct snd_soc_codec *codec)
{
}

static inline void soc_cleanup_codec_debugfs(struct snd_soc_codec *codec)
{
}

static inline void soc_init_platform_debugfs(struct snd_soc_platform *platform)
{
}

static inline void soc_cleanup_platform_debugfs(struct snd_soc_platform *platform)
{
}

static inline void soc_init_card_debugfs(struct snd_soc_card *card)
{
}

static inline void soc_cleanup_card_debugfs(struct snd_soc_card *card)
{
}
#endif

struct snd_pcm_substream *snd_soc_get_dai_substream(struct snd_soc_card *card,
		const char *dai_link, int stream)
{
	int i;

	for (i = 0; i < card->num_links; i++) {
		if (card->rtd[i].dai_link->no_pcm &&
			!strcmp(card->rtd[i].dai_link->name, dai_link))
			return card->rtd[i].pcm->streams[stream].substream;
	}
	dev_dbg(card->dev, "failed to find dai link %s\n", dai_link);
	return NULL;
}
EXPORT_SYMBOL_GPL(snd_soc_get_dai_substream);

struct snd_soc_pcm_runtime *snd_soc_get_pcm_runtime(struct snd_soc_card *card,
		const char *dai_link)
{
	int i;

	for (i = 0; i < card->num_links; i++) {
		if (!strcmp(card->rtd[i].dai_link->name, dai_link))
			return &card->rtd[i];
	}
	dev_dbg(card->dev, "failed to find rtd %s\n", dai_link);
=======
static inline void soc_init_component_debugfs(struct snd_soc_component *component) { }
static inline void soc_cleanup_component_debugfs(struct snd_soc_component *component) { }
static inline void soc_init_card_debugfs(struct snd_soc_card *card) { }
static inline void soc_cleanup_card_debugfs(struct snd_soc_card *card) { }
static inline void snd_soc_debugfs_init(void) { }
static inline void snd_soc_debugfs_exit(void) { }

#endif

static int snd_soc_is_match_dai_args(const struct of_phandle_args *args1,
				     const struct of_phandle_args *args2)
{
	if (!args1 || !args2)
		return 0;

	if (args1->np != args2->np)
		return 0;

	for (int i = 0; i < args1->args_count; i++)
		if (args1->args[i] != args2->args[i])
			return 0;

	return 1;
}

static inline int snd_soc_dlc_component_is_empty(struct snd_soc_dai_link_component *dlc)
{
	return !(dlc->dai_args || dlc->name || dlc->of_node);
}

static inline int snd_soc_dlc_component_is_invalid(struct snd_soc_dai_link_component *dlc)
{
	return (dlc->name && dlc->of_node);
}

static inline int snd_soc_dlc_dai_is_empty(struct snd_soc_dai_link_component *dlc)
{
	return !(dlc->dai_args || dlc->dai_name);
}

static int snd_soc_is_matching_dai(const struct snd_soc_dai_link_component *dlc,
				   struct snd_soc_dai *dai)
{
	if (!dlc)
		return 0;

	if (dlc->dai_args)
		return snd_soc_is_match_dai_args(dai->driver->dai_args, dlc->dai_args);

	if (!dlc->dai_name)
		return 1;

	/* see snd_soc_dai_name_get() */

	if (dai->driver->name &&
	    strcmp(dlc->dai_name, dai->driver->name) == 0)
		return 1;

	if (strcmp(dlc->dai_name, dai->name) == 0)
		return 1;

	if (dai->component->name &&
	    strcmp(dlc->dai_name, dai->component->name) == 0)
		return 1;

	return 0;
}

const char *snd_soc_dai_name_get(struct snd_soc_dai *dai)
{
	/* see snd_soc_is_matching_dai() */
	if (dai->driver->name)
		return dai->driver->name;

	if (dai->name)
		return dai->name;

	if (dai->component->name)
		return dai->component->name;

	return NULL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_name_get);

static int snd_soc_rtd_add_component(struct snd_soc_pcm_runtime *rtd,
				     struct snd_soc_component *component)
{
	struct snd_soc_component *comp;
	int i;

	for_each_rtd_components(rtd, i, comp) {
		/* already connected */
		if (comp == component)
			return 0;
	}

	/* see for_each_rtd_components */
	rtd->components[rtd->num_components] = component;
	rtd->num_components++;

	return 0;
}

struct snd_soc_component *snd_soc_rtdcom_lookup(struct snd_soc_pcm_runtime *rtd,
						const char *driver_name)
{
	struct snd_soc_component *component;
	int i;

	if (!driver_name)
		return NULL;

	/*
	 * NOTE
	 *
	 * snd_soc_rtdcom_lookup() will find component from rtd by using
	 * specified driver name.
	 * But, if many components which have same driver name are connected
	 * to 1 rtd, this function will return 1st found component.
	 */
	for_each_rtd_components(rtd, i, component) {
		const char *component_name = component->driver->name;

		if (!component_name)
			continue;

		if ((component_name == driver_name) ||
		    strcmp(component_name, driver_name) == 0)
			return component;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(snd_soc_rtdcom_lookup);

struct snd_soc_component
*snd_soc_lookup_component_nolocked(struct device *dev, const char *driver_name)
{
	struct snd_soc_component *component;
	struct snd_soc_component *found_component;

	found_component = NULL;
	for_each_component(component) {
		if ((dev == component->dev) &&
		    (!driver_name ||
		     (driver_name == component->driver->name) ||
		     (strcmp(component->driver->name, driver_name) == 0))) {
			found_component = component;
			break;
		}
	}

	return found_component;
}
EXPORT_SYMBOL_GPL(snd_soc_lookup_component_nolocked);

struct snd_soc_component *snd_soc_lookup_component(struct device *dev,
						   const char *driver_name)
{
	struct snd_soc_component *component;

	mutex_lock(&client_mutex);
	component = snd_soc_lookup_component_nolocked(dev, driver_name);
	mutex_unlock(&client_mutex);

	return component;
}
EXPORT_SYMBOL_GPL(snd_soc_lookup_component);

struct snd_soc_pcm_runtime
*snd_soc_get_pcm_runtime(struct snd_soc_card *card,
			 struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_pcm_runtime *rtd;

	for_each_card_rtds(card, rtd) {
		if (rtd->dai_link == dai_link)
			return rtd;
	}
	dev_dbg(card->dev, "ASoC: failed to find rtd %s\n", dai_link->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}
EXPORT_SYMBOL_GPL(snd_soc_get_pcm_runtime);

<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_AC97_BUS
/* unregister ac97 codec */
static int soc_ac97_dev_unregister(struct snd_soc_codec *codec)
{
	if (codec->ac97->dev.bus)
		device_unregister(&codec->ac97->dev);
	return 0;
}

/* stop no dev release warning */
static void soc_ac97_device_release(struct device *dev){}

/* register ac97 codec to bus */
static int soc_ac97_dev_register(struct snd_soc_codec *codec)
{
	int err;

	codec->ac97->dev.bus = &ac97_bus_type;
	codec->ac97->dev.parent = codec->card->dev;
	codec->ac97->dev.release = soc_ac97_device_release;

	dev_set_name(&codec->ac97->dev, "%d-%d:%s",
		     codec->card->snd_card->number, 0, codec->name);
	err = device_register(&codec->ac97->dev);
	if (err < 0) {
		snd_printk(KERN_ERR "Can't register ac97 bus\n");
		codec->ac97->dev.bus = NULL;
		return err;
	}
	return 0;
}
#endif

#ifdef CONFIG_PM_SLEEP
=======
/*
 * Power down the audio subsystem pmdown_time msecs after close is called.
 * This is to ensure there are no pops or clicks in between any music tracks
 * due to DAPM power cycling.
 */
void snd_soc_close_delayed_work(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);
	int playback = SNDRV_PCM_STREAM_PLAYBACK;

	snd_soc_dpcm_mutex_lock(rtd);

	dev_dbg(rtd->dev,
		"ASoC: pop wq checking: %s status: %s waiting: %s\n",
		codec_dai->driver->playback.stream_name,
		snd_soc_dai_stream_active(codec_dai, playback) ?
		"active" : "inactive",
		rtd->pop_wait ? "yes" : "no");

	/* are we waiting on this codec DAI stream */
	if (rtd->pop_wait == 1) {
		rtd->pop_wait = 0;
		snd_soc_dapm_stream_event(rtd, playback,
					  SND_SOC_DAPM_STREAM_STOP);
	}

	snd_soc_dpcm_mutex_unlock(rtd);
}
EXPORT_SYMBOL_GPL(snd_soc_close_delayed_work);

static void soc_release_rtd_dev(struct device *dev)
{
	/* "dev" means "rtd->dev" */
	kfree(dev);
}

static void soc_free_pcm_runtime(struct snd_soc_pcm_runtime *rtd)
{
	if (!rtd)
		return;

	list_del(&rtd->list);

	if (delayed_work_pending(&rtd->delayed_work))
		flush_delayed_work(&rtd->delayed_work);
	snd_soc_pcm_component_free(rtd);

	/*
	 * we don't need to call kfree() for rtd->dev
	 * see
	 *	soc_release_rtd_dev()
	 *
	 * We don't need rtd->dev NULL check, because
	 * it is alloced *before* rtd.
	 * see
	 *	soc_new_pcm_runtime()
	 *
	 * We don't need to mind freeing for rtd,
	 * because it was created from dev (= rtd->dev)
	 * see
	 *	soc_new_pcm_runtime()
	 *
	 *		rtd = devm_kzalloc(dev, ...);
	 *		rtd->dev = dev
	 */
	device_unregister(rtd->dev);
}

static void close_delayed_work(struct work_struct *work) {
	struct snd_soc_pcm_runtime *rtd =
			container_of(work, struct snd_soc_pcm_runtime,
				     delayed_work.work);

	if (rtd->close_delayed_work_func)
		rtd->close_delayed_work_func(rtd);
}

static struct snd_soc_pcm_runtime *soc_new_pcm_runtime(
	struct snd_soc_card *card, struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_component *component;
	struct device *dev;
	int ret;
	int stream;

	/*
	 * for rtd->dev
	 */
	dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	if (!dev)
		return NULL;

	dev->parent	= card->dev;
	dev->release	= soc_release_rtd_dev;

	dev_set_name(dev, "%s", dai_link->name);

	ret = device_register(dev);
	if (ret < 0) {
		put_device(dev); /* soc_release_rtd_dev */
		return NULL;
	}

	/*
	 * for rtd
	 */
	rtd = devm_kzalloc(dev,
			   sizeof(*rtd) +
			   sizeof(component) * (dai_link->num_cpus +
						 dai_link->num_codecs +
						 dai_link->num_platforms),
			   GFP_KERNEL);
	if (!rtd) {
		device_unregister(dev);
		return NULL;
	}

	rtd->dev = dev;
	INIT_LIST_HEAD(&rtd->list);
	for_each_pcm_streams(stream) {
		INIT_LIST_HEAD(&rtd->dpcm[stream].be_clients);
		INIT_LIST_HEAD(&rtd->dpcm[stream].fe_clients);
	}
	dev_set_drvdata(dev, rtd);
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);

	/*
	 * for rtd->dais
	 */
	rtd->dais = devm_kcalloc(dev, dai_link->num_cpus + dai_link->num_codecs,
					sizeof(struct snd_soc_dai *),
					GFP_KERNEL);
	if (!rtd->dais)
		goto free_rtd;

	/*
	 * dais = [][][][][][][][][][][][][][][][][][]
	 *	  ^cpu_dais         ^codec_dais
	 *	  |--- num_cpus ---|--- num_codecs --|
	 * see
	 *	snd_soc_rtd_to_cpu()
	 *	snd_soc_rtd_to_codec()
	 */
	rtd->card	= card;
	rtd->dai_link	= dai_link;
	rtd->num	= card->num_rtd++;
	rtd->pmdown_time = pmdown_time;			/* default power off timeout */

	/* see for_each_card_rtds */
	list_add_tail(&rtd->list, &card->rtd_list);

	ret = device_add_groups(dev, soc_dev_attr_groups);
	if (ret < 0)
		goto free_rtd;

	return rtd;

free_rtd:
	soc_free_pcm_runtime(rtd);
	return NULL;
}

static void snd_soc_fill_dummy_dai(struct snd_soc_card *card)
{
	struct snd_soc_dai_link *dai_link;
	int i;

	/*
	 * COMP_DUMMY() creates size 0 array on dai_link.
	 * Fill it as dummy DAI in case of CPU/Codec here.
	 * Do nothing for Platform.
	 */
	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->num_cpus == 0 && dai_link->cpus) {
			dai_link->num_cpus	= 1;
			dai_link->cpus		= &snd_soc_dummy_dlc;
		}
		if (dai_link->num_codecs == 0 && dai_link->codecs) {
			dai_link->num_codecs	= 1;
			dai_link->codecs	= &snd_soc_dummy_dlc;
		}
	}
}

static void snd_soc_flush_all_delayed_work(struct snd_soc_card *card)
{
	struct snd_soc_pcm_runtime *rtd;

	for_each_card_rtds(card, rtd)
		flush_delayed_work(&rtd->delayed_work);
}

#ifdef CONFIG_PM_SLEEP
static void soc_playback_digital_mute(struct snd_soc_card *card, int mute)
{
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_dai *dai;
	int playback = SNDRV_PCM_STREAM_PLAYBACK;
	int i;

	for_each_card_rtds(card, rtd) {

		if (rtd->dai_link->ignore_suspend)
			continue;

		for_each_rtd_dais(rtd, i, dai) {
			if (snd_soc_dai_stream_active(dai, playback))
				snd_soc_dai_digital_mute(dai, mute, playback);
		}
	}
}

static void soc_dapm_suspend_resume(struct snd_soc_card *card, int event)
{
	struct snd_soc_pcm_runtime *rtd;
	int stream;

	for_each_card_rtds(card, rtd) {

		if (rtd->dai_link->ignore_suspend)
			continue;

		for_each_pcm_streams(stream)
			snd_soc_dapm_stream_event(rtd, stream, event);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* powers down audio subsystem for suspend */
int snd_soc_suspend(struct device *dev)
{
	struct snd_soc_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct snd_soc_codec *codec;
	int i;

	/* If the initialization of this soc device failed, there is no codec
	 * associated with it. Just bail out in this case.
	 */
	if (list_empty(&card->codec_dev_list))
		return 0;

	/* Due to the resume being scheduled into a workqueue we could
	* suspend before that's finished - wait for it to complete.
	 */
	snd_power_lock(card->snd_card);
	snd_power_wait(card->snd_card, SNDRV_CTL_POWER_D0);
	snd_power_unlock(card->snd_card);
=======
	struct snd_soc_component *component;
	struct snd_soc_pcm_runtime *rtd;
	int i;

	/* If the card is not initialized yet there is nothing to do */
	if (!snd_soc_card_is_instantiated(card))
		return 0;

	/*
	 * Due to the resume being scheduled into a workqueue we could
	 * suspend before that's finished - wait for it to complete.
	 */
	snd_power_wait(card->snd_card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* we're going to block userspace touching us until resume completes */
	snd_power_change_state(card->snd_card, SNDRV_CTL_POWER_D3hot);

	/* mute any active DACs */
<<<<<<< HEAD
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *dai = card->rtd[i].codec_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic)
			soc_dpcm_be_digital_mute(&card->rtd[i], 1);
		else {
			if (drv->ops->digital_mute && dai->playback_active)
				drv->ops->digital_mute(dai, 1);
		}
	}

	/* suspend all pcms */
	for (i = 0; i < card->num_rtd; i++) {
		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		snd_pcm_suspend_all(card->rtd[i].pcm);
	}

	if (card->suspend_pre)
		card->suspend_pre(card);

	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *cpu_dai = card->rtd[i].cpu_dai;
		struct snd_soc_platform *platform = card->rtd[i].platform;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic) {
			soc_dpcm_be_cpu_dai_suspend(&card->rtd[i]);
			soc_dpcm_be_platform_suspend(&card->rtd[i]);
		} else {
			if (cpu_dai->driver->suspend && !cpu_dai->driver->ac97_control)
				cpu_dai->driver->suspend(cpu_dai);
			if (platform->driver->suspend && !platform->suspended) {
				platform->driver->suspend(cpu_dai);
				platform->suspended = 1;
			}
		}
	}

	/* close any waiting streams and save state */
	for (i = 0; i < card->num_rtd; i++) {
		flush_delayed_work_sync(&card->rtd[i].delayed_work);
		card->rtd[i].codec->dapm.suspend_bias_level = card->rtd[i].codec->dapm.bias_level;
	}

	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai_driver *driver = card->rtd[i].codec_dai->driver;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (driver->playback.stream_name != NULL)
			snd_soc_dapm_stream_event(&card->rtd[i], driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_SUSPEND);

		if (driver->capture.stream_name != NULL)
			snd_soc_dapm_stream_event(&card->rtd[i], driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_SUSPEND);
	}

	/* suspend all CODECs */
	list_for_each_entry(codec, &card->codec_dev_list, card_list) {
		/* If there are paths active then the CODEC will be held with
		 * bias _ON and should not be suspended. */
		if (!codec->suspended && codec->driver->suspend) {
			switch (codec->dapm.bias_level) {
			case SND_SOC_BIAS_STANDBY:
			case SND_SOC_BIAS_OFF:
				codec->driver->suspend(codec);
				codec->suspended = 1;
				codec->cache_sync = 1;
				break;
			default:
				dev_dbg(codec->dev, "CODEC is on over suspend\n");
=======
	soc_playback_digital_mute(card, 1);

	/* suspend all pcms */
	for_each_card_rtds(card, rtd) {
		if (rtd->dai_link->ignore_suspend)
			continue;

		snd_pcm_suspend_all(rtd->pcm);
	}

	snd_soc_card_suspend_pre(card);

	/* close any waiting streams */
	snd_soc_flush_all_delayed_work(card);

	soc_dapm_suspend_resume(card, SND_SOC_DAPM_STREAM_SUSPEND);

	/* Recheck all endpoints too, their state is affected by suspend */
	dapm_mark_endpoints_dirty(card);
	snd_soc_dapm_sync(&card->dapm);

	/* suspend all COMPONENTs */
	for_each_card_rtds(card, rtd) {

		if (rtd->dai_link->ignore_suspend)
			continue;

		for_each_rtd_components(rtd, i, component) {
			struct snd_soc_dapm_context *dapm =
				snd_soc_component_get_dapm(component);

			/*
			 * ignore if component was already suspended
			 */
			if (snd_soc_component_is_suspended(component))
				continue;

			/*
			 * If there are paths active then the COMPONENT will be
			 * held with bias _ON and should not be suspended.
			 */
			switch (snd_soc_dapm_get_bias_level(dapm)) {
			case SND_SOC_BIAS_STANDBY:
				/*
				 * If the COMPONENT is capable of idle
				 * bias off then being in STANDBY
				 * means it's doing something,
				 * otherwise fall through.
				 */
				if (dapm->idle_bias_off) {
					dev_dbg(component->dev,
						"ASoC: idle_bias_off CODEC on over suspend\n");
					break;
				}
				fallthrough;

			case SND_SOC_BIAS_OFF:
				snd_soc_component_suspend(component);
				if (component->regmap)
					regcache_mark_dirty(component->regmap);
				/* deactivate pins to sleep state */
				pinctrl_pm_select_sleep_state(component->dev);
				break;
			default:
				dev_dbg(component->dev,
					"ASoC: COMPONENT is on over suspend\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			}
		}
	}

<<<<<<< HEAD
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *cpu_dai = card->rtd[i].cpu_dai;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic)
			soc_dpcm_be_ac97_cpu_dai_suspend(&card->rtd[i]);
		else
			if (cpu_dai->driver->suspend && cpu_dai->driver->ac97_control)
				cpu_dai->driver->suspend(cpu_dai);
	}

	if (card->suspend_post)
		card->suspend_post(card);
=======
	snd_soc_card_suspend_post(card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_suspend);

<<<<<<< HEAD
/* deferred resume work, so resume can complete before we finished
=======
/*
 * deferred resume work, so resume can complete before we finished
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * setting our codec back up, which can be very slow on I2C
 */
static void soc_resume_deferred(struct work_struct *work)
{
	struct snd_soc_card *card =
<<<<<<< HEAD
			container_of(work, struct snd_soc_card, deferred_resume_work);
	struct snd_soc_codec *codec;
	int i;

	/* our power state is still SNDRV_CTL_POWER_D3hot from suspend time,
	 * so userspace apps are blocked from touching us
	 */

	dev_dbg(card->dev, "starting resume work\n");
=======
			container_of(work, struct snd_soc_card,
				     deferred_resume_work);
	struct snd_soc_component *component;

	/*
	 * our power state is still SNDRV_CTL_POWER_D3hot from suspend time,
	 * so userspace apps are blocked from touching us
	 */

	dev_dbg(card->dev, "ASoC: starting resume work\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Bring us up into D2 so that DAPM starts enabling things */
	snd_power_change_state(card->snd_card, SNDRV_CTL_POWER_D2);

<<<<<<< HEAD
	if (card->resume_pre)
		card->resume_pre(card);

	/* resume AC97 DAIs */
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *cpu_dai = card->rtd[i].cpu_dai;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic)
			soc_dpcm_be_ac97_cpu_dai_resume(&card->rtd[i]);
		else
			if (cpu_dai->driver->resume && cpu_dai->driver->ac97_control)
				cpu_dai->driver->resume(cpu_dai);
	}

	list_for_each_entry(codec, &card->codec_dev_list, card_list) {
		/* If the CODEC was idle over suspend then it will have been
		 * left with bias OFF or STANDBY and suspended so we must now
		 * resume.  Otherwise the suspend was suppressed.
		 */
		if (codec->driver->resume && codec->suspended) {
			switch (codec->dapm.bias_level) {
			case SND_SOC_BIAS_STANDBY:
			case SND_SOC_BIAS_OFF:
				codec->driver->resume(codec);
				codec->suspended = 0;
				break;
			default:
				dev_dbg(codec->dev, "CODEC was on over suspend\n");
				break;
			}
		}
	}

	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai_driver *driver = card->rtd[i].codec_dai->driver;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (driver->playback.stream_name != NULL)
			snd_soc_dapm_stream_event(&card->rtd[i], driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_RESUME);

		if (driver->capture.stream_name != NULL)
			snd_soc_dapm_stream_event(&card->rtd[i], driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_RESUME);
	}

	/* unmute any active DACs */
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *dai = card->rtd[i].codec_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic)
			soc_dpcm_be_digital_mute(&card->rtd[i], 0);
		else {
			if (drv->ops->digital_mute && dai->playback_active)
				drv->ops->digital_mute(dai, 0);
		}
	}

	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *cpu_dai = card->rtd[i].cpu_dai;
		struct snd_soc_platform *platform = card->rtd[i].platform;

		if (card->rtd[i].dai_link->ignore_suspend ||
				card->rtd[i].dai_link->no_pcm)
			continue;

		if (card->rtd[i].dai_link->dynamic) {
			soc_dpcm_be_cpu_dai_resume(&card->rtd[i]);
			soc_dpcm_be_platform_resume(&card->rtd[i]);
		} else {
			if (cpu_dai->driver->resume && !cpu_dai->driver->ac97_control)
				cpu_dai->driver->resume(cpu_dai);
			if (platform->driver->resume && platform->suspended) {
				platform->driver->resume(cpu_dai);
				platform->suspended = 0;
			}
		}
	}

	if (card->resume_post)
		card->resume_post(card);

	dev_dbg(card->dev, "resume work completed\n");
=======
	snd_soc_card_resume_pre(card);

	for_each_card_components(card, component) {
		if (snd_soc_component_is_suspended(component))
			snd_soc_component_resume(component);
	}

	soc_dapm_suspend_resume(card, SND_SOC_DAPM_STREAM_RESUME);

	/* unmute any active DACs */
	soc_playback_digital_mute(card, 0);

	snd_soc_card_resume_post(card);

	dev_dbg(card->dev, "ASoC: resume work completed\n");

	/* Recheck all endpoints too, their state is affected by suspend */
	dapm_mark_endpoints_dirty(card);
	snd_soc_dapm_sync(&card->dapm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* userspace can access us now we are back as we were before */
	snd_power_change_state(card->snd_card, SNDRV_CTL_POWER_D0);
}

/* powers up audio subsystem after a suspend */
int snd_soc_resume(struct device *dev)
{
	struct snd_soc_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	int i, ac97_control = 0;

	/* If the initialization of this soc device failed, there is no codec
	 * associated with it. Just bail out in this case.
	 */
	if (list_empty(&card->codec_dev_list))
		return 0;

	/* AC97 devices might have other drivers hanging off them so
	 * need to resume immediately.  Other drivers don't have that
	 * problem and may take a substantial amount of time to resume
	 * due to I/O costs and anti-pop so handle them out of line.
	 */
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dai *cpu_dai = card->rtd[i].cpu_dai;
		ac97_control |= cpu_dai->driver->ac97_control;
	}
	if (ac97_control) {
		dev_dbg(dev, "Resuming AC97 immediately\n");
		soc_resume_deferred(&card->deferred_resume_work);
	} else {
		dev_dbg(dev, "Scheduling resume work\n");
		if (!schedule_work(&card->deferred_resume_work))
			dev_err(dev, "resume work item may be lost\n");
	}
=======
	struct snd_soc_component *component;

	/* If the card is not initialized yet there is nothing to do */
	if (!snd_soc_card_is_instantiated(card))
		return 0;

	/* activate pins from sleep state */
	for_each_card_components(card, component)
		if (snd_soc_component_active(component))
			pinctrl_pm_select_default_state(component->dev);

	dev_dbg(dev, "ASoC: Scheduling resume work\n");
	if (!schedule_work(&card->deferred_resume_work))
		dev_err(dev, "ASoC: resume work item may be lost\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_resume);
<<<<<<< HEAD
#else
#define snd_soc_suspend NULL
#define snd_soc_resume NULL
#endif

static const struct snd_soc_dai_ops null_dai_ops = {
};

static int soc_bind_dai_link(struct snd_soc_card *card, int num)
{
	struct snd_soc_dai_link *dai_link = &card->dai_link[num];
	struct snd_soc_pcm_runtime *rtd = &card->rtd[num];
	struct snd_soc_codec *codec;
	struct snd_soc_platform *platform;
	struct snd_soc_dai *codec_dai, *cpu_dai;
	const char *platform_name;

	dev_dbg(card->dev, "binding %s at idx %d\n", dai_link->name, num);

	/* Find CPU DAI from registered DAIs*/
	list_for_each_entry(cpu_dai, &dai_list, list) {
		if (dai_link->cpu_dai_of_node) {
			if (cpu_dai->dev->of_node != dai_link->cpu_dai_of_node)
				continue;
		} else {
			if (strcmp(cpu_dai->name, dai_link->cpu_dai_name))
				continue;
		}

		rtd->cpu_dai = cpu_dai;
	}

	if (!rtd->cpu_dai) {
		dev_dbg(card->dev, "CPU DAI %s not registered\n",
			dai_link->cpu_dai_name);
		return -EPROBE_DEFER;

	}

	/* no, then find CODEC from registered CODECs*/
	list_for_each_entry(codec, &codec_list, list) {
		if (dai_link->codec_of_node) {
			if (codec->dev->of_node != dai_link->codec_of_node)
				continue;
		} else {
			if (strcmp(codec->name, dai_link->codec_name))
				continue;
		}

		rtd->codec = codec;

		/*
		 * CODEC found, so find CODEC DAI from registered DAIs from
		 * this CODEC
		 */
		list_for_each_entry(codec_dai, &dai_list, list) {
			if (codec->dev == codec_dai->dev &&
				!strcmp(codec_dai->name,
					dai_link->codec_dai_name)) {

				rtd->codec_dai = codec_dai;
			}
		}
		if (!rtd->codec_dai) {
			dev_dbg(card->dev, "CODEC DAI %s not registered\n",
				dai_link->codec_dai_name);
			return -EPROBE_DEFER;
		}

	}

	if (!rtd->codec) {
		dev_dbg(card->dev, "CODEC %s not registered\n",
			dai_link->codec_name);
		return -EPROBE_DEFER;
	}

	/* if there's no platform we match on the empty platform */
	platform_name = dai_link->platform_name;
	if (!platform_name && !dai_link->platform_of_node)
		platform_name = "snd-soc-dummy";

	/* no, then find one from the set of registered platforms */
	list_for_each_entry(platform, &platform_list, list) {
		if (dai_link->platform_of_node) {
			if (platform->dev->of_node !=
			    dai_link->platform_of_node)
				continue;
		} else {
			if (strcmp(platform->name, platform_name))
				continue;
		}

		rtd->platform = platform;
	}
	if (!rtd->platform) {
		dev_dbg(card->dev, "platform %s not registered\n",
			dai_link->platform_name);

		return -EPROBE_DEFER;
	}
	card->num_rtd++;

	return 0;

}

static void soc_remove_codec(struct snd_soc_codec *codec)
{
	int err;

	if (codec->driver->remove) {
		err = codec->driver->remove(codec);
		if (err < 0)
			dev_err(codec->dev,
				"asoc: failed to remove %s: %d\n",
				codec->name, err);
	}

	/* Make sure all DAPM widgets are freed */
	snd_soc_dapm_free(&codec->dapm);

	soc_cleanup_codec_debugfs(codec);
	codec->probed = 0;
	list_del(&codec->card_list);
	module_put(codec->dev->driver->owner);
}

static void soc_remove_dai_link(struct snd_soc_card *card, int num, int order)
{
	struct snd_soc_pcm_runtime *rtd = &card->rtd[num];
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai, *cpu_dai = rtd->cpu_dai;
	int err;

	/* unregister the rtd device */
	if (rtd->dev_registered) {
		device_remove_file(rtd->dev, &dev_attr_pmdown_time);
		device_remove_file(rtd->dev, &dev_attr_codec_reg);
		device_unregister(rtd->dev);
		rtd->dev_registered = 0;
	}

	/* remove the CODEC DAI */
	if (codec_dai && codec_dai->probed &&
			codec_dai->driver->remove_order == order) {
		if (codec_dai->driver->remove) {
			err = codec_dai->driver->remove(codec_dai);
			if (err < 0)
				printk(KERN_ERR "asoc: failed to remove %s\n", codec_dai->name);
		}
		codec_dai->probed = 0;
		list_del(&codec_dai->card_list);
	}

	/* remove the platform */
	if (platform && platform->probed &&
			platform->driver->remove_order == order) {
		if (platform->driver->remove) {
			err = platform->driver->remove(platform);
			if (err < 0)
				printk(KERN_ERR "asoc: failed to remove %s\n", platform->name);
		}
		/* Make sure all DAPM widgets are freed */
		snd_soc_dapm_free(&platform->dapm);

		soc_cleanup_platform_debugfs(platform);
		platform->probed = 0;
		list_del(&platform->card_list);
		module_put(platform->dev->driver->owner);
	}

	/* remove the CODEC */
	if (codec && codec->probed &&
			codec->driver->remove_order == order)
		soc_remove_codec(codec);

	/* remove the cpu_dai */
	if (cpu_dai && cpu_dai->probed &&
			cpu_dai->driver->remove_order == order) {
		if (cpu_dai->driver->remove) {
			err = cpu_dai->driver->remove(cpu_dai);
			if (err < 0)
				printk(KERN_ERR "asoc: failed to remove %s\n", cpu_dai->name);
		}
		cpu_dai->probed = 0;
		list_del(&cpu_dai->card_list);
		module_put(cpu_dai->dev->driver->owner);
	}
}

static void soc_remove_dai_links(struct snd_soc_card *card)
{
	int dai, order;

	for (order = SND_SOC_COMP_ORDER_FIRST; order <= SND_SOC_COMP_ORDER_LAST;
			order++) {
		for (dai = 0; dai < card->num_rtd; dai++)
			soc_remove_dai_link(card, dai, order);
	}
	card->num_rtd = 0;
}

static void soc_set_name_prefix(struct snd_soc_card *card,
				struct snd_soc_codec *codec)
{
	int i;

	if (card->codec_conf == NULL)
		return;

	for (i = 0; i < card->num_configs; i++) {
		struct snd_soc_codec_conf *map = &card->codec_conf[i];
		if (map->dev_name && !strcmp(codec->name, map->dev_name)) {
			codec->name_prefix = map->name_prefix;
			break;
		}
	}
}

static int soc_probe_codec(struct snd_soc_card *card,
			   struct snd_soc_codec *codec)
{
	int ret = 0;
	const struct snd_soc_codec_driver *driver = codec->driver;

	codec->card = card;
	codec->dapm.card = card;
	soc_set_name_prefix(card, codec);

	if (!try_module_get(codec->dev->driver->owner))
		return -ENODEV;

	soc_init_codec_debugfs(codec);

	if (driver->dapm_widgets)
		snd_soc_dapm_new_controls(&codec->dapm, driver->dapm_widgets,
					  driver->num_dapm_widgets);

	codec->dapm.idle_bias_off = driver->idle_bias_off;

	if (driver->probe) {
		ret = driver->probe(codec);
		if (ret < 0) {
			dev_err(codec->dev,
				"asoc: failed to probe CODEC %s: %d\n",
				codec->name, ret);
			goto err_probe;
		}
	}

	if (driver->controls)
		snd_soc_add_codec_controls(codec, driver->controls,
				     driver->num_controls);
	if (driver->dapm_routes)
		snd_soc_dapm_add_routes(&codec->dapm, driver->dapm_routes,
					driver->num_dapm_routes);

	/* mark codec as probed and add to card codec list */
	codec->probed = 1;
	list_add(&codec->card_list, &card->codec_dev_list);
	list_add(&codec->dapm.list, &card->dapm_list);

	return 0;

err_probe:
	soc_cleanup_codec_debugfs(codec);
	module_put(codec->dev->driver->owner);

	return ret;
}

static int soc_probe_platform(struct snd_soc_card *card,
			   struct snd_soc_platform *platform)
{
	int ret = 0;
	const struct snd_soc_platform_driver *driver = platform->driver;

	platform->card = card;
	platform->dapm.card = card;

	if (!try_module_get(platform->dev->driver->owner))
		return -ENODEV;

	soc_init_platform_debugfs(platform);

	if (driver->dapm_widgets)
		snd_soc_dapm_new_controls(&platform->dapm,
			driver->dapm_widgets, driver->num_dapm_widgets);

	if (driver->probe) {
		ret = driver->probe(platform);
		if (ret < 0) {
			dev_err(platform->dev,
				"asoc: failed to probe platform %s: %d\n",
				platform->name, ret);
			goto err_probe;
		}
	}

	if (driver->controls)
		snd_soc_add_platform_controls(platform, driver->controls,
				     driver->num_controls);
	if (driver->dapm_routes)
		snd_soc_dapm_add_routes(&platform->dapm, driver->dapm_routes,
					driver->num_dapm_routes);

	/* mark platform as probed and add to card platform list */
	platform->probed = 1;
	list_add(&platform->card_list, &card->platform_dev_list);
	list_add(&platform->dapm.list, &card->dapm_list);

	return 0;

err_probe:
	module_put(platform->dev->driver->owner);

	return ret;
}

static void rtd_release(struct device *dev)
{
	kfree(dev);
}

static int soc_post_component_init(struct snd_soc_card *card,
				   struct snd_soc_codec *codec,
				   int num, int dailess)
{
	struct snd_soc_dai_link *dai_link = NULL;
	struct snd_soc_aux_dev *aux_dev = NULL;
	struct snd_soc_pcm_runtime *rtd;
	const char *temp, *name;
	int ret = 0;

	if (!dailess) {
		dai_link = &card->dai_link[num];
		rtd = &card->rtd[num];
		name = dai_link->name;
	} else {
		aux_dev = &card->aux_dev[num];
		rtd = &card->rtd_aux[num];
		name = aux_dev->name;
	}
	rtd->card = card;

	/* Make sure all DAPM widgets are instantiated */
	snd_soc_dapm_new_widgets(&codec->dapm);

	/* machine controls, routes and widgets are not prefixed */
	temp = codec->name_prefix;
	codec->name_prefix = NULL;

	/* do machine specific initialization */
	if (!dailess && dai_link->init)
		ret = dai_link->init(rtd);
	else if (dailess && aux_dev->init)
		ret = aux_dev->init(&codec->dapm);
	if (ret < 0) {
		dev_err(card->dev, "asoc: failed to init %s: %d\n", name, ret);
		return ret;
	}
	codec->name_prefix = temp;

	/* register the rtd device */
	rtd->codec = codec;

	rtd->dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	if (!rtd->dev)
		return -ENOMEM;
	device_initialize(rtd->dev);
	rtd->dev->parent = card->dev;
	rtd->dev->release = rtd_release;
	rtd->dev->init_name = name;
	dev_set_drvdata(rtd->dev, rtd);
	mutex_init(&rtd->pcm_mutex);
	INIT_LIST_HEAD(&rtd->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients);
	INIT_LIST_HEAD(&rtd->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients);
	INIT_LIST_HEAD(&rtd->dpcm[SNDRV_PCM_STREAM_PLAYBACK].fe_clients);
	INIT_LIST_HEAD(&rtd->dpcm[SNDRV_PCM_STREAM_CAPTURE].fe_clients);
	ret = device_add(rtd->dev);
	if (ret < 0) {
		dev_err(card->dev,
			"asoc: failed to register runtime device: %d\n", ret);
		return ret;
	}
	rtd->dev_registered = 1;

	/* add DAPM sysfs entries for this codec */
	ret = snd_soc_dapm_sys_add(rtd->dev);
	if (ret < 0)
		dev_err(codec->dev,
			"asoc: failed to add codec dapm sysfs entries: %d\n",
			ret);

	/* add codec sysfs entries */
	ret = device_create_file(rtd->dev, &dev_attr_codec_reg);
	if (ret < 0)
		dev_err(codec->dev,
			"asoc: failed to add codec sysfs files: %d\n", ret);

#ifdef CONFIG_DEBUG_FS
	/* add DSP sysfs entries */
	if (!dai_link->dynamic)
		goto out;

	ret = soc_dpcm_debugfs_add(rtd);
	if (ret < 0)
		dev_err(rtd->dev, "asoc: failed to add dpcm sysfs entries: %d\n", ret);

out:
#endif
	return 0;
}

static int soc_probe_dai_link(struct snd_soc_card *card, int num, int order)
{
	struct snd_soc_dai_link *dai_link = &card->dai_link[num];
	struct snd_soc_pcm_runtime *rtd = &card->rtd[num];
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai, *cpu_dai = rtd->cpu_dai;
	int ret;

	dev_dbg(card->dev, "probe %s dai link %d late %d\n",
			card->name, num, order);

	/* config components */
	codec_dai->codec = codec;
	cpu_dai->platform = platform;
	codec_dai->card = card;
	cpu_dai->card = card;

	/* set default power off timeout */
	rtd->pmdown_time = pmdown_time;

	/* probe the cpu_dai */
	if (!cpu_dai->probed &&
			cpu_dai->driver->probe_order == order) {
		if (!try_module_get(cpu_dai->dev->driver->owner))
			return -ENODEV;

		if (cpu_dai->driver->probe) {
			ret = cpu_dai->driver->probe(cpu_dai);
			if (ret < 0) {
				printk(KERN_ERR "asoc: failed to probe CPU DAI %s\n",
						cpu_dai->name);
				module_put(cpu_dai->dev->driver->owner);
				return ret;
			}
		}
		cpu_dai->probed = 1;
		/* mark cpu_dai as probed and add to card dai list */
		list_add(&cpu_dai->card_list, &card->dai_dev_list);
	}

	/* probe the CODEC */
	if (!codec->probed &&
			codec->driver->probe_order == order) {
		ret = soc_probe_codec(card, codec);
		if (ret < 0)
			return ret;
	}

	/* probe the platform */
	if (!platform->probed &&
			platform->driver->probe_order == order) {
		ret = soc_probe_platform(card, platform);
		if (ret < 0)
			return ret;
	}

	/* probe the CODEC DAI */
	if (!codec_dai->probed && codec_dai->driver->probe_order == order) {
		if (codec_dai->driver->probe) {
			ret = codec_dai->driver->probe(codec_dai);
			if (ret < 0) {
				printk(KERN_ERR "asoc: failed to probe CODEC DAI %s\n",
						codec_dai->name);
				return ret;
			}
		}

		/* mark codec_dai as probed and add to card dai list */
		codec_dai->probed = 1;
		list_add(&codec_dai->card_list, &card->dai_dev_list);
	}

	/* complete DAI probe during last probe */
	if (order != SND_SOC_COMP_ORDER_LAST)
		return 0;

	ret = soc_post_component_init(card, codec, num, 0);
	if (ret)
		return ret;

	ret = device_create_file(rtd->dev, &dev_attr_pmdown_time);
	if (ret < 0)
		printk(KERN_WARNING "asoc: failed to add pmdown_time sysfs\n");

	if (cpu_dai->driver->compress_dai) {
		/*create compress_device"*/
		ret = soc_new_compress(rtd, num);
		if (ret < 0) {
		  printk(KERN_ERR "asoc: can't create compress %s\n",
					 dai_link->stream_name);
			return ret;
		}
	} else {
		/* create the pcm */
		ret = soc_new_pcm(rtd, num);
		if (ret < 0) {
		  printk(KERN_ERR "asoc: can't create pcm %s :%d\n",
			       dai_link->stream_name, ret);
			return ret;
		}
	}

	/* add platform data for AC97 devices */
	if (rtd->codec_dai->driver->ac97_control)
		snd_ac97_dev_add_pdata(codec->ac97, rtd->cpu_dai->ac97_pdata);

	return 0;
}

#ifdef CONFIG_SND_SOC_AC97_BUS
static int soc_register_ac97_dai_link(struct snd_soc_pcm_runtime *rtd)
{
	int ret;

	/* Only instantiate AC97 if not already done by the adaptor
	 * for the generic AC97 subsystem.
	 */
	if (rtd->codec_dai->driver->ac97_control && !rtd->codec->ac97_registered) {
		/*
		 * It is possible that the AC97 device is already registered to
		 * the device subsystem. This happens when the device is created
		 * via snd_ac97_mixer(). Currently only SoC codec that does so
		 * is the generic AC97 glue but others migh emerge.
		 *
		 * In those cases we don't try to register the device again.
		 */
		if (!rtd->codec->ac97_created)
			return 0;

		ret = soc_ac97_dev_register(rtd->codec);
		if (ret < 0) {
			printk(KERN_ERR "asoc: AC97 device register failed\n");
			return ret;
		}

		rtd->codec->ac97_registered = 1;
	}
	return 0;
}

static void soc_unregister_ac97_dai_link(struct snd_soc_codec *codec)
{
	if (codec->ac97_registered) {
		soc_ac97_dev_unregister(codec);
		codec->ac97_registered = 0;
	}
}
#endif

static int soc_check_aux_dev(struct snd_soc_card *card, int num)
{
	struct snd_soc_aux_dev *aux_dev = &card->aux_dev[num];
	struct snd_soc_codec *codec;

	/* find CODEC from registered CODECs*/
	list_for_each_entry(codec, &codec_list, list) {
		if (!strcmp(codec->name, aux_dev->codec_name))
			return 0;
	}

	return -EPROBE_DEFER;
}

static int soc_probe_aux_dev(struct snd_soc_card *card, int num)
{
	struct snd_soc_aux_dev *aux_dev = &card->aux_dev[num];
	struct snd_soc_codec *codec;
	int ret = -ENODEV;

	/* find CODEC from registered CODECs*/
	list_for_each_entry(codec, &codec_list, list) {
		if (!strcmp(codec->name, aux_dev->codec_name)) {
			if (codec->probed) {
				dev_err(codec->dev,
					"asoc: codec already probed");
				ret = -EBUSY;
				goto out;
			}
			goto found;
		}
	}
	/* codec not found */
	dev_err(card->dev, "asoc: codec %s not found", aux_dev->codec_name);
	return -EPROBE_DEFER;

found:
	ret = soc_probe_codec(card, codec);
	if (ret < 0)
		return ret;

	ret = soc_post_component_init(card, codec, num, 1);

out:
	return ret;
}

static void soc_remove_aux_dev(struct snd_soc_card *card, int num)
{
	struct snd_soc_pcm_runtime *rtd = &card->rtd_aux[num];
	struct snd_soc_codec *codec = rtd->codec;

	/* unregister the rtd device */
	if (rtd->dev_registered) {
		device_remove_file(rtd->dev, &dev_attr_codec_reg);
		device_del(rtd->dev);
		rtd->dev_registered = 0;
	}

	if (codec && codec->probed)
		soc_remove_codec(codec);
}

static int snd_soc_init_codec_cache(struct snd_soc_codec *codec,
				    enum snd_soc_compress_type compress_type)
{
	int ret;

	if (codec->cache_init)
		return 0;

	/* override the compress_type if necessary */
	if (compress_type && codec->compress_type != compress_type)
		codec->compress_type = compress_type;
	ret = snd_soc_cache_init(codec);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set cache compression type: %d\n",
			ret);
		return ret;
	}
	codec->cache_init = 1;
	return 0;
}

static void soc_init_dai_aif_channel_map(struct snd_soc_card *card,
		struct snd_soc_dai *dai, int stream)
{
	struct snd_soc_dapm_widget *w = NULL;
	const char *aif_name;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		aif_name = dai->driver->playback.aif_name;
	else
		aif_name = dai->driver->capture.aif_name;

	if (dai->codec) {
		struct snd_soc_codec *codec;

		list_for_each_entry(codec, &card->codec_dev_list, card_list) {
			w = snd_soc_get_codec_widget(card, codec, aif_name);
			if (w)
				break;
		}
	} else if (dai->platform) {
		struct snd_soc_platform *platform;

		list_for_each_entry(platform, &card->platform_dev_list, card_list) {
			w = snd_soc_get_platform_widget(card, platform, aif_name);
			if (w)
				break;
		}
	}

	if (w) {
		if (stream == SNDRV_PCM_STREAM_PLAYBACK)
			dai->playback_aif = w;
		else
			dai->capture_aif = w;
	} else
		dev_err(dai->dev, "unable to find %s DAI AIF %s\n",
			stream ? "capture" : "playback", aif_name);

	dai->channel_map_instanciated = 1;
}

static int soc_is_dai_pcm(struct snd_soc_card *card, struct snd_soc_dai *dai)
{
	int i;

	for (i = 0; i < card->num_rtd; i++) {
		if (card->rtd[i].cpu_dai == dai && !card->rtd[i].dai_link->no_pcm)
			return 1;
	}
	return 0;
}

static void soc_init_card_aif_channel_map(struct snd_soc_card *card)
{
	struct snd_soc_dai *dai;

	list_for_each_entry(dai, &card->dai_dev_list, card_list) {

		/* only process DAIs that use the new API until
		 * the old "stream name" API is fully deprecated */
		if (!dai->driver->playback.aif_name && !dai->driver->capture.aif_name)
			continue;

		/* channels are only mapped from PCM DAIs */
		if (!soc_is_dai_pcm(card, dai))
			continue;

		/* skip if already instanciated */
		if (dai->channel_map_instanciated)
			continue;

		/* create unique channels masks for each DAI in the sound card */
		dai->playback_channel_map =
			((1 << dai->driver->playback.channels_max) - 1)
			<< card->num_playback_channels;
		card->num_playback_channels += dai->driver->playback.channels_max;

		dai->capture_channel_map =
			((1 << dai->driver->capture.channels_max) - 1)
			<< card->num_capture_channels;
		card->num_capture_channels += dai->driver->capture.channels_max;

		if (dai->driver->playback.channels_max)
			soc_init_dai_aif_channel_map(card, dai, SNDRV_PCM_STREAM_PLAYBACK);
		if (dai->driver->capture.channels_max)
			soc_init_dai_aif_channel_map(card, dai, SNDRV_PCM_STREAM_CAPTURE);
	}
}


static int snd_soc_instantiate_card(struct snd_soc_card *card)
{
	struct snd_soc_codec *codec;
	struct snd_soc_codec_conf *codec_conf;
	enum snd_soc_compress_type compress_type;
	struct snd_soc_dai_link *dai_link;
	int ret, i, order;

	mutex_lock(&card->mutex);


	/* bind DAIs */
	for (i = 0; i < card->num_links; i++) {
		ret = soc_bind_dai_link(card, i);
		if (ret != 0)
			goto base_error;
	}

	/* check aux_devs too */
	for (i = 0; i < card->num_aux_devs; i++) {
		ret = soc_check_aux_dev(card, i);
		if (ret != 0)
			goto base_error;
	}

	/* initialize the register cache for each available codec */
	list_for_each_entry(codec, &codec_list, list) {
		if (codec->cache_init)
			continue;
		/* by default we don't override the compress_type */
		compress_type = 0;
		/* check to see if we need to override the compress_type */
		for (i = 0; i < card->num_configs; ++i) {
			codec_conf = &card->codec_conf[i];
			if (!strcmp(codec->name, codec_conf->dev_name)) {
				compress_type = codec_conf->compress_type;
				if (compress_type && compress_type
				    != codec->compress_type)
					break;
			}
		}
		ret = snd_soc_init_codec_cache(codec, compress_type);
		if (ret < 0)
			goto base_error;
	}

	/* card bind complete so register a sound card */
	ret = snd_card_create(SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1,
			card->owner, 0, &card->snd_card);
	if (ret < 0) {
		printk(KERN_ERR "asoc: can't create sound card for card %s\n",
			card->name);
		goto base_error;
	}
	card->snd_card->dev = card->dev;

	card->dapm.bias_level = SND_SOC_BIAS_OFF;
	card->dapm.dev = card->dev;
	card->dapm.card = card;
	list_add(&card->dapm.list, &card->dapm_list);

#ifdef CONFIG_DEBUG_FS
	snd_soc_dapm_debugfs_init(&card->dapm, card->debugfs_card_root);
#endif

#ifdef CONFIG_PM_SLEEP
	/* deferred resume work */
	INIT_WORK(&card->deferred_resume_work, soc_resume_deferred);
#endif

	if (card->dapm_widgets)
		snd_soc_dapm_new_controls(&card->dapm, card->dapm_widgets,
					  card->num_dapm_widgets);

	/* initialise the sound card only once */
	if (card->probe) {
		ret = card->probe(card);
		if (ret < 0)
			goto card_probe_error;
	}

	/* early DAI link probe */
	for (order = SND_SOC_COMP_ORDER_FIRST; order <= SND_SOC_COMP_ORDER_LAST;
			order++) {
		for (i = 0; i < card->num_links; i++) {
			ret = soc_probe_dai_link(card, i, order);
			if (ret < 0) {
				pr_err("asoc: failed to instantiate card %s: %d\n",
			       card->name, ret);
				goto probe_dai_err;
			}
		}
	}

	for (i = 0; i < card->num_aux_devs; i++) {
		ret = soc_probe_aux_dev(card, i);
		if (ret < 0) {
			pr_err("asoc: failed to add auxiliary devices %s: %d\n",
			       card->name, ret);
			goto probe_aux_dev_err;
		}
	}

	if (card->controls)
		snd_soc_add_card_controls(card, card->controls, card->num_controls);

	if (card->dapm_routes)
		snd_soc_dapm_add_routes(&card->dapm, card->dapm_routes,
					card->num_dapm_routes);

	snd_soc_dapm_new_widgets(&card->dapm);

	for (i = 0; i < card->num_links; i++) {
		dai_link = &card->dai_link[i];

		if (dai_link->dai_fmt) {
			ret = snd_soc_dai_set_fmt(card->rtd[i].codec_dai,
						  dai_link->dai_fmt);
			if (ret != 0)
				dev_warn(card->rtd[i].codec_dai->dev,
					 "Failed to set DAI format: %d\n",
					 ret);

			ret = snd_soc_dai_set_fmt(card->rtd[i].cpu_dai,
						  dai_link->dai_fmt);
			if (ret != 0)
				dev_warn(card->rtd[i].cpu_dai->dev,
					 "Failed to set DAI format: %d\n",
					 ret);
		}
	}

	snprintf(card->snd_card->shortname, sizeof(card->snd_card->shortname),
		 "%s", card->name);
	snprintf(card->snd_card->longname, sizeof(card->snd_card->longname),
		 "%s", card->long_name ? card->long_name : card->name);
	snprintf(card->snd_card->driver, sizeof(card->snd_card->driver),
		 "%s", card->driver_name ? card->driver_name : card->name);
	for (i = 0; i < ARRAY_SIZE(card->snd_card->driver); i++) {
		switch (card->snd_card->driver[i]) {
=======

static void soc_resume_init(struct snd_soc_card *card)
{
	/* deferred resume work */
	INIT_WORK(&card->deferred_resume_work, soc_resume_deferred);
}
#else
#define snd_soc_suspend NULL
#define snd_soc_resume NULL
static inline void soc_resume_init(struct snd_soc_card *card) { }
#endif

static struct device_node
*soc_component_to_node(struct snd_soc_component *component)
{
	struct device_node *of_node;

	of_node = component->dev->of_node;
	if (!of_node && component->dev->parent)
		of_node = component->dev->parent->of_node;

	return of_node;
}

struct of_phandle_args *snd_soc_copy_dai_args(struct device *dev,
					      const struct of_phandle_args *args)
{
	struct of_phandle_args *ret = devm_kzalloc(dev, sizeof(*ret), GFP_KERNEL);

	if (!ret)
		return NULL;

	*ret = *args;

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_copy_dai_args);

static int snd_soc_is_matching_component(
	const struct snd_soc_dai_link_component *dlc,
	struct snd_soc_component *component)
{
	struct device_node *component_of_node;

	if (!dlc)
		return 0;

	if (dlc->dai_args) {
		struct snd_soc_dai *dai;

		for_each_component_dais(component, dai)
			if (snd_soc_is_matching_dai(dlc, dai))
				return 1;
		return 0;
	}

	component_of_node = soc_component_to_node(component);

	if (dlc->of_node && component_of_node != dlc->of_node)
		return 0;
	if (dlc->name && strcmp(component->name, dlc->name))
		return 0;

	return 1;
}

static struct snd_soc_component *soc_find_component(
	const struct snd_soc_dai_link_component *dlc)
{
	struct snd_soc_component *component;

	lockdep_assert_held(&client_mutex);

	/*
	 * NOTE
	 *
	 * It returns *1st* found component, but some driver
	 * has few components by same of_node/name
	 * ex)
	 *	CPU component and generic DMAEngine component
	 */
	for_each_component(component)
		if (snd_soc_is_matching_component(dlc, component))
			return component;

	return NULL;
}

/**
 * snd_soc_find_dai - Find a registered DAI
 *
 * @dlc: name of the DAI or the DAI driver and optional component info to match
 *
 * This function will search all registered components and their DAIs to
 * find the DAI of the same name. The component's of_node and name
 * should also match if being specified.
 *
 * Return: pointer of DAI, or NULL if not found.
 */
struct snd_soc_dai *snd_soc_find_dai(
	const struct snd_soc_dai_link_component *dlc)
{
	struct snd_soc_component *component;
	struct snd_soc_dai *dai;

	lockdep_assert_held(&client_mutex);

	/* Find CPU DAI from registered DAIs */
	for_each_component(component)
		if (snd_soc_is_matching_component(dlc, component))
			for_each_component_dais(component, dai)
				if (snd_soc_is_matching_dai(dlc, dai))
					return dai;

	return NULL;
}
EXPORT_SYMBOL_GPL(snd_soc_find_dai);

struct snd_soc_dai *snd_soc_find_dai_with_mutex(
	const struct snd_soc_dai_link_component *dlc)
{
	struct snd_soc_dai *dai;

	mutex_lock(&client_mutex);
	dai = snd_soc_find_dai(dlc);
	mutex_unlock(&client_mutex);

	return dai;
}
EXPORT_SYMBOL_GPL(snd_soc_find_dai_with_mutex);

static int soc_dai_link_sanity_check(struct snd_soc_card *card,
				     struct snd_soc_dai_link *link)
{
	int i;
	struct snd_soc_dai_link_component *dlc;

	/* Codec check */
	for_each_link_codecs(link, i, dlc) {
		/*
		 * Codec must be specified by 1 of name or OF node,
		 * not both or neither.
		 */
		if (snd_soc_dlc_component_is_invalid(dlc))
			goto component_invalid;

		if (snd_soc_dlc_component_is_empty(dlc))
			goto component_empty;

		/* Codec DAI name must be specified */
		if (snd_soc_dlc_dai_is_empty(dlc))
			goto dai_empty;

		/*
		 * Defer card registration if codec component is not added to
		 * component list.
		 */
		if (!soc_find_component(dlc))
			goto component_not_found;
	}

	/* Platform check */
	for_each_link_platforms(link, i, dlc) {
		/*
		 * Platform may be specified by either name or OF node, but it
		 * can be left unspecified, then no components will be inserted
		 * in the rtdcom list
		 */
		if (snd_soc_dlc_component_is_invalid(dlc))
			goto component_invalid;

		if (snd_soc_dlc_component_is_empty(dlc))
			goto component_empty;

		/*
		 * Defer card registration if platform component is not added to
		 * component list.
		 */
		if (!soc_find_component(dlc))
			goto component_not_found;
	}

	/* CPU check */
	for_each_link_cpus(link, i, dlc) {
		/*
		 * CPU device may be specified by either name or OF node, but
		 * can be left unspecified, and will be matched based on DAI
		 * name alone..
		 */
		if (snd_soc_dlc_component_is_invalid(dlc))
			goto component_invalid;


		if (snd_soc_dlc_component_is_empty(dlc)) {
			/*
			 * At least one of CPU DAI name or CPU device name/node must be specified
			 */
			if (snd_soc_dlc_dai_is_empty(dlc))
				goto component_dai_empty;
		} else {
			/*
			 * Defer card registration if Component is not added
			 */
			if (!soc_find_component(dlc))
				goto component_not_found;
		}
	}

	return 0;

component_invalid:
	dev_err(card->dev, "ASoC: Both Component name/of_node are set for %s\n", link->name);
	return -EINVAL;

component_empty:
	dev_err(card->dev, "ASoC: Neither Component name/of_node are set for %s\n", link->name);
	return -EINVAL;

component_not_found:
	dev_dbg(card->dev, "ASoC: Component %s not found for link %s\n", dlc->name, link->name);
	return -EPROBE_DEFER;

dai_empty:
	dev_err(card->dev, "ASoC: DAI name is not set for %s\n", link->name);
	return -EINVAL;

component_dai_empty:
	dev_err(card->dev, "ASoC: Neither DAI/Component name/of_node are set for %s\n", link->name);
	return -EINVAL;
}

#define MAX_DEFAULT_CH_MAP_SIZE 8
static struct snd_soc_dai_link_ch_map default_ch_map_sync[MAX_DEFAULT_CH_MAP_SIZE] = {
	{ .cpu = 0, .codec = 0 },
	{ .cpu = 1, .codec = 1 },
	{ .cpu = 2, .codec = 2 },
	{ .cpu = 3, .codec = 3 },
	{ .cpu = 4, .codec = 4 },
	{ .cpu = 5, .codec = 5 },
	{ .cpu = 6, .codec = 6 },
	{ .cpu = 7, .codec = 7 },
};
static struct snd_soc_dai_link_ch_map default_ch_map_1cpu[MAX_DEFAULT_CH_MAP_SIZE] = {
	{ .cpu = 0, .codec = 0 },
	{ .cpu = 0, .codec = 1 },
	{ .cpu = 0, .codec = 2 },
	{ .cpu = 0, .codec = 3 },
	{ .cpu = 0, .codec = 4 },
	{ .cpu = 0, .codec = 5 },
	{ .cpu = 0, .codec = 6 },
	{ .cpu = 0, .codec = 7 },
};
static struct snd_soc_dai_link_ch_map default_ch_map_1codec[MAX_DEFAULT_CH_MAP_SIZE] = {
	{ .cpu = 0, .codec = 0 },
	{ .cpu = 1, .codec = 0 },
	{ .cpu = 2, .codec = 0 },
	{ .cpu = 3, .codec = 0 },
	{ .cpu = 4, .codec = 0 },
	{ .cpu = 5, .codec = 0 },
	{ .cpu = 6, .codec = 0 },
	{ .cpu = 7, .codec = 0 },
};
static int snd_soc_compensate_channel_connection_map(struct snd_soc_card *card,
						     struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_dai_link_ch_map *ch_maps;
	int i;

	/*
	 * dai_link->ch_maps indicates how CPU/Codec are connected.
	 * It will be a map seen from a larger number of DAI.
	 * see
	 *	soc.h :: [dai_link->ch_maps Image sample]
	 */

	/* it should have ch_maps if connection was N:M */
	if (dai_link->num_cpus > 1 && dai_link->num_codecs > 1 &&
	    dai_link->num_cpus != dai_link->num_codecs && !dai_link->ch_maps) {
		dev_err(card->dev, "need to have ch_maps when N:M connection (%s)",
			dai_link->name);
		return -EINVAL;
	}

	/* do nothing if it has own maps */
	if (dai_link->ch_maps)
		goto sanity_check;

	/* check default map size */
	if (dai_link->num_cpus   > MAX_DEFAULT_CH_MAP_SIZE ||
	    dai_link->num_codecs > MAX_DEFAULT_CH_MAP_SIZE) {
		dev_err(card->dev, "soc-core.c needs update default_connection_maps");
		return -EINVAL;
	}

	/* Compensate missing map for ... */
	if (dai_link->num_cpus == dai_link->num_codecs)
		dai_link->ch_maps = default_ch_map_sync;	/* for 1:1 or N:N */
	else if (dai_link->num_cpus <  dai_link->num_codecs)
		dai_link->ch_maps = default_ch_map_1cpu;	/* for 1:N */
	else
		dai_link->ch_maps = default_ch_map_1codec;	/* for N:1 */

sanity_check:
	dev_dbg(card->dev, "dai_link %s\n", dai_link->stream_name);
	for_each_link_ch_maps(dai_link, i, ch_maps) {
		if ((ch_maps->cpu   >= dai_link->num_cpus) ||
		    (ch_maps->codec >= dai_link->num_codecs)) {
			dev_err(card->dev,
				"unexpected dai_link->ch_maps[%d] index (cpu(%d/%d) codec(%d/%d))",
				i,
				ch_maps->cpu,	dai_link->num_cpus,
				ch_maps->codec,	dai_link->num_codecs);
			return -EINVAL;
		}

		dev_dbg(card->dev, "  [%d] cpu%d <-> codec%d\n",
			i, ch_maps->cpu, ch_maps->codec);
	}

	return 0;
}

/**
 * snd_soc_remove_pcm_runtime - Remove a pcm_runtime from card
 * @card: The ASoC card to which the pcm_runtime has
 * @rtd: The pcm_runtime to remove
 *
 * This function removes a pcm_runtime from the ASoC card.
 */
void snd_soc_remove_pcm_runtime(struct snd_soc_card *card,
				struct snd_soc_pcm_runtime *rtd)
{
	lockdep_assert_held(&client_mutex);

	/*
	 * Notify the machine driver for extra destruction
	 */
	snd_soc_card_remove_dai_link(card, rtd->dai_link);

	soc_free_pcm_runtime(rtd);
}
EXPORT_SYMBOL_GPL(snd_soc_remove_pcm_runtime);

/**
 * snd_soc_add_pcm_runtime - Add a pcm_runtime dynamically via dai_link
 * @card: The ASoC card to which the pcm_runtime is added
 * @dai_link: The DAI link to find pcm_runtime
 *
 * This function adds a pcm_runtime ASoC card by using dai_link.
 *
 * Note: Topology can use this API to add pcm_runtime when probing the
 * topology component. And machine drivers can still define static
 * DAI links in dai_link array.
 */
static int snd_soc_add_pcm_runtime(struct snd_soc_card *card,
				   struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_dai_link_component *codec, *platform, *cpu;
	struct snd_soc_component *component;
	int i, ret;

	lockdep_assert_held(&client_mutex);

	/*
	 * Notify the machine driver for extra initialization
	 */
	ret = snd_soc_card_add_dai_link(card, dai_link);
	if (ret < 0)
		return ret;

	if (dai_link->ignore)
		return 0;

	dev_dbg(card->dev, "ASoC: binding %s\n", dai_link->name);

	ret = soc_dai_link_sanity_check(card, dai_link);
	if (ret < 0)
		return ret;

	rtd = soc_new_pcm_runtime(card, dai_link);
	if (!rtd)
		return -ENOMEM;

	for_each_link_cpus(dai_link, i, cpu) {
		snd_soc_rtd_to_cpu(rtd, i) = snd_soc_find_dai(cpu);
		if (!snd_soc_rtd_to_cpu(rtd, i)) {
			dev_info(card->dev, "ASoC: CPU DAI %s not registered\n",
				 cpu->dai_name);
			goto _err_defer;
		}
		snd_soc_rtd_add_component(rtd, snd_soc_rtd_to_cpu(rtd, i)->component);
	}

	/* Find CODEC from registered CODECs */
	for_each_link_codecs(dai_link, i, codec) {
		snd_soc_rtd_to_codec(rtd, i) = snd_soc_find_dai(codec);
		if (!snd_soc_rtd_to_codec(rtd, i)) {
			dev_info(card->dev, "ASoC: CODEC DAI %s not registered\n",
				 codec->dai_name);
			goto _err_defer;
		}

		snd_soc_rtd_add_component(rtd, snd_soc_rtd_to_codec(rtd, i)->component);
	}

	/* Find PLATFORM from registered PLATFORMs */
	for_each_link_platforms(dai_link, i, platform) {
		for_each_component(component) {
			if (!snd_soc_is_matching_component(platform, component))
				continue;

			if (snd_soc_component_is_dummy(component) && component->num_dai)
				continue;

			snd_soc_rtd_add_component(rtd, component);
		}
	}

	return 0;

_err_defer:
	snd_soc_remove_pcm_runtime(card, rtd);
	return -EPROBE_DEFER;
}

int snd_soc_add_pcm_runtimes(struct snd_soc_card *card,
			     struct snd_soc_dai_link *dai_link,
			     int num_dai_link)
{
	for (int i = 0; i < num_dai_link; i++) {
		int ret;

		ret = snd_soc_compensate_channel_connection_map(card, dai_link + i);
		if (ret < 0)
			return ret;

		ret = snd_soc_add_pcm_runtime(card, dai_link + i);
		if (ret < 0)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_add_pcm_runtimes);

static void snd_soc_runtime_get_dai_fmt(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dai_link *dai_link = rtd->dai_link;
	struct snd_soc_dai *dai, *not_used;
	u64 pos, possible_fmt;
	unsigned int mask = 0, dai_fmt = 0;
	int i, j, priority, pri, until;

	/*
	 * Get selectable format from each DAIs.
	 *
	 ****************************
	 *            NOTE
	 * Using .auto_selectable_formats is not mandatory,
	 * we can select format manually from Sound Card.
	 * When use it, driver should list well tested format only.
	 ****************************
	 *
	 * ex)
	 *	auto_selectable_formats (= SND_SOC_POSSIBLE_xxx)
	 *		 (A)	 (B)	 (C)
	 *	DAI0_: { 0x000F, 0x00F0, 0x0F00 };
	 *	DAI1 : { 0xF000, 0x0F00 };
	 *		 (X)	 (Y)
	 *
	 * "until" will be 3 in this case (MAX array size from DAI0 and DAI1)
	 * Here is dev_dbg() message and comments
	 *
	 * priority = 1
	 * DAI0: (pri, fmt) = (1, 000000000000000F) // 1st check (A) DAI1 is not selected
	 * DAI1: (pri, fmt) = (0, 0000000000000000) //               Necessary Waste
	 * DAI0: (pri, fmt) = (1, 000000000000000F) // 2nd check (A)
	 * DAI1: (pri, fmt) = (1, 000000000000F000) //           (X)
	 * priority = 2
	 * DAI0: (pri, fmt) = (2, 00000000000000FF) // 3rd check (A) + (B)
	 * DAI1: (pri, fmt) = (1, 000000000000F000) //           (X)
	 * DAI0: (pri, fmt) = (2, 00000000000000FF) // 4th check (A) + (B)
	 * DAI1: (pri, fmt) = (2, 000000000000FF00) //           (X) + (Y)
	 * priority = 3
	 * DAI0: (pri, fmt) = (3, 0000000000000FFF) // 5th check (A) + (B) + (C)
	 * DAI1: (pri, fmt) = (2, 000000000000FF00) //           (X) + (Y)
	 * found auto selected format: 0000000000000F00
	 */
	until = snd_soc_dai_get_fmt_max_priority(rtd);
	for (priority = 1; priority <= until; priority++) {
		for_each_rtd_dais(rtd, j, not_used) {

			possible_fmt = ULLONG_MAX;
			for_each_rtd_dais(rtd, i, dai) {
				u64 fmt = 0;

				pri = (j >= i) ? priority : priority - 1;
				fmt = snd_soc_dai_get_fmt(dai, pri);
				possible_fmt &= fmt;
			}
			if (possible_fmt)
				goto found;
		}
	}
	/* Not Found */
	return;
found:
	/*
	 * convert POSSIBLE_DAIFMT to DAIFMT
	 *
	 * Some basic/default settings on each is defined as 0.
	 * see
	 *	SND_SOC_DAIFMT_NB_NF
	 *	SND_SOC_DAIFMT_GATED
	 *
	 * SND_SOC_DAIFMT_xxx_MASK can't notice it if Sound Card specify
	 * these value, and will be overwrite to auto selected value.
	 *
	 * To avoid such issue, loop from 63 to 0 here.
	 * Small number of SND_SOC_POSSIBLE_xxx will be Hi priority.
	 * Basic/Default settings of each part and above are defined
	 * as Hi priority (= small number) of SND_SOC_POSSIBLE_xxx.
	 */
	for (i = 63; i >= 0; i--) {
		pos = 1ULL << i;
		switch (possible_fmt & pos) {
		/*
		 * for format
		 */
		case SND_SOC_POSSIBLE_DAIFMT_I2S:
		case SND_SOC_POSSIBLE_DAIFMT_RIGHT_J:
		case SND_SOC_POSSIBLE_DAIFMT_LEFT_J:
		case SND_SOC_POSSIBLE_DAIFMT_DSP_A:
		case SND_SOC_POSSIBLE_DAIFMT_DSP_B:
		case SND_SOC_POSSIBLE_DAIFMT_AC97:
		case SND_SOC_POSSIBLE_DAIFMT_PDM:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_FORMAT_MASK) | i;
			break;
		/*
		 * for clock
		 */
		case SND_SOC_POSSIBLE_DAIFMT_CONT:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_MASK) | SND_SOC_DAIFMT_CONT;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_GATED:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_MASK) | SND_SOC_DAIFMT_GATED;
			break;
		/*
		 * for clock invert
		 */
		case SND_SOC_POSSIBLE_DAIFMT_NB_NF:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_INV_MASK) | SND_SOC_DAIFMT_NB_NF;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_NB_IF:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_INV_MASK) | SND_SOC_DAIFMT_NB_IF;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_IB_NF:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_INV_MASK) | SND_SOC_DAIFMT_IB_NF;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_IB_IF:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_INV_MASK) | SND_SOC_DAIFMT_IB_IF;
			break;
		/*
		 * for clock provider / consumer
		 */
		case SND_SOC_POSSIBLE_DAIFMT_CBP_CFP:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) | SND_SOC_DAIFMT_CBP_CFP;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_CBC_CFP:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) | SND_SOC_DAIFMT_CBC_CFP;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_CBP_CFC:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) | SND_SOC_DAIFMT_CBP_CFC;
			break;
		case SND_SOC_POSSIBLE_DAIFMT_CBC_CFC:
			dai_fmt = (dai_fmt & ~SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) | SND_SOC_DAIFMT_CBC_CFC;
			break;
		}
	}

	/*
	 * Some driver might have very complex limitation.
	 * In such case, user want to auto-select non-limitation part,
	 * and want to manually specify complex part.
	 *
	 * Or for example, if both CPU and Codec can be clock provider,
	 * but because of its quality, user want to specify it manually.
	 *
	 * Use manually specified settings if sound card did.
	 */
	if (!(dai_link->dai_fmt & SND_SOC_DAIFMT_FORMAT_MASK))
		mask |= SND_SOC_DAIFMT_FORMAT_MASK;
	if (!(dai_link->dai_fmt & SND_SOC_DAIFMT_CLOCK_MASK))
		mask |= SND_SOC_DAIFMT_CLOCK_MASK;
	if (!(dai_link->dai_fmt & SND_SOC_DAIFMT_INV_MASK))
		mask |= SND_SOC_DAIFMT_INV_MASK;
	if (!(dai_link->dai_fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK))
		mask |= SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK;

	dai_link->dai_fmt |= (dai_fmt & mask);
}

/**
 * snd_soc_runtime_set_dai_fmt() - Change DAI link format for a ASoC runtime
 * @rtd: The runtime for which the DAI link format should be changed
 * @dai_fmt: The new DAI link format
 *
 * This function updates the DAI link format for all DAIs connected to the DAI
 * link for the specified runtime.
 *
 * Note: For setups with a static format set the dai_fmt field in the
 * corresponding snd_dai_link struct instead of using this function.
 *
 * Returns 0 on success, otherwise a negative error code.
 */
int snd_soc_runtime_set_dai_fmt(struct snd_soc_pcm_runtime *rtd,
				unsigned int dai_fmt)
{
	struct snd_soc_dai *cpu_dai;
	struct snd_soc_dai *codec_dai;
	unsigned int i;
	int ret;

	if (!dai_fmt)
		return 0;

	for_each_rtd_codec_dais(rtd, i, codec_dai) {
		ret = snd_soc_dai_set_fmt(codec_dai, dai_fmt);
		if (ret != 0 && ret != -ENOTSUPP)
			return ret;
	}

	/* Flip the polarity for the "CPU" end of link */
	dai_fmt = snd_soc_daifmt_clock_provider_flipped(dai_fmt);

	for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
		ret = snd_soc_dai_set_fmt(cpu_dai, dai_fmt);
		if (ret != 0 && ret != -ENOTSUPP)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_runtime_set_dai_fmt);

static int soc_init_pcm_runtime(struct snd_soc_card *card,
				struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dai_link *dai_link = rtd->dai_link;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	struct snd_soc_component *component;
	int ret, num, i;

	/* do machine specific initialization */
	ret = snd_soc_link_init(rtd);
	if (ret < 0)
		return ret;

	snd_soc_runtime_get_dai_fmt(rtd);
	ret = snd_soc_runtime_set_dai_fmt(rtd, dai_link->dai_fmt);
	if (ret)
		goto err;

	/* add DPCM sysfs entries */
	soc_dpcm_debugfs_add(rtd);

	num = rtd->num;

	/*
	 * most drivers will register their PCMs using DAI link ordering but
	 * topology based drivers can use the DAI link id field to set PCM
	 * device number and then use rtd + a base offset of the BEs.
	 */
	for_each_rtd_components(rtd, i, component) {
		if (!component->driver->use_dai_pcm_id)
			continue;

		if (rtd->dai_link->no_pcm)
			num += component->driver->be_pcm_base;
		else
			num = rtd->dai_link->id;
	}

	/* create compress_device if possible */
	ret = snd_soc_dai_compress_new(cpu_dai, rtd, num);
	if (ret != -ENOTSUPP)
		goto err;

	/* create the pcm */
	ret = soc_new_pcm(rtd, num);
	if (ret < 0) {
		dev_err(card->dev, "ASoC: can't create pcm %s :%d\n",
			dai_link->stream_name, ret);
		goto err;
	}

	ret = snd_soc_pcm_dai_new(rtd);
	if (ret < 0)
		goto err;

	rtd->initialized = true;

	return 0;
err:
	snd_soc_link_exit(rtd);
	return ret;
}

static void soc_set_name_prefix(struct snd_soc_card *card,
				struct snd_soc_component *component)
{
	struct device_node *of_node = soc_component_to_node(component);
	const char *str;
	int ret, i;

	for (i = 0; i < card->num_configs; i++) {
		struct snd_soc_codec_conf *map = &card->codec_conf[i];

		if (snd_soc_is_matching_component(&map->dlc, component) &&
		    map->name_prefix) {
			component->name_prefix = map->name_prefix;
			return;
		}
	}

	/*
	 * If there is no configuration table or no match in the table,
	 * check if a prefix is provided in the node
	 */
	ret = of_property_read_string(of_node, "sound-name-prefix", &str);
	if (ret < 0)
		return;

	component->name_prefix = str;
}

static void soc_remove_component(struct snd_soc_component *component,
				 int probed)
{

	if (!component->card)
		return;

	if (probed)
		snd_soc_component_remove(component);

	list_del_init(&component->card_list);
	snd_soc_dapm_free(snd_soc_component_get_dapm(component));
	soc_cleanup_component_debugfs(component);
	component->card = NULL;
	snd_soc_component_module_put_when_remove(component);
}

static int soc_probe_component(struct snd_soc_card *card,
			       struct snd_soc_component *component)
{
	struct snd_soc_dapm_context *dapm =
		snd_soc_component_get_dapm(component);
	struct snd_soc_dai *dai;
	int probed = 0;
	int ret;

	if (snd_soc_component_is_dummy(component))
		return 0;

	if (component->card) {
		if (component->card != card) {
			dev_err(component->dev,
				"Trying to bind component \"%s\" to card \"%s\" but is already bound to card \"%s\"\n",
				component->name, card->name, component->card->name);
			return -ENODEV;
		}
		return 0;
	}

	ret = snd_soc_component_module_get_when_probe(component);
	if (ret < 0)
		return ret;

	component->card = card;
	soc_set_name_prefix(card, component);

	soc_init_component_debugfs(component);

	snd_soc_dapm_init(dapm, card, component);

	ret = snd_soc_dapm_new_controls(dapm,
					component->driver->dapm_widgets,
					component->driver->num_dapm_widgets);

	if (ret != 0) {
		dev_err(component->dev,
			"Failed to create new controls %d\n", ret);
		goto err_probe;
	}

	for_each_component_dais(component, dai) {
		ret = snd_soc_dapm_new_dai_widgets(dapm, dai);
		if (ret != 0) {
			dev_err(component->dev,
				"Failed to create DAI widgets %d\n", ret);
			goto err_probe;
		}
	}

	ret = snd_soc_component_probe(component);
	if (ret < 0)
		goto err_probe;

	WARN(dapm->idle_bias_off &&
	     dapm->bias_level != SND_SOC_BIAS_OFF,
	     "codec %s can not start from non-off bias with idle_bias_off==1\n",
	     component->name);
	probed = 1;

	/*
	 * machine specific init
	 * see
	 *	snd_soc_component_set_aux()
	 */
	ret = snd_soc_component_init(component);
	if (ret < 0)
		goto err_probe;

	ret = snd_soc_add_component_controls(component,
					     component->driver->controls,
					     component->driver->num_controls);
	if (ret < 0)
		goto err_probe;

	ret = snd_soc_dapm_add_routes(dapm,
				      component->driver->dapm_routes,
				      component->driver->num_dapm_routes);
	if (ret < 0) {
		if (card->disable_route_checks) {
			dev_info(card->dev,
				 "%s: disable_route_checks set, ignoring errors on add_routes\n",
				 __func__);
		} else {
			dev_err(card->dev,
				"%s: snd_soc_dapm_add_routes failed: %d\n",
				__func__, ret);
			goto err_probe;
		}
	}

	/* see for_each_card_components */
	list_add(&component->card_list, &card->component_dev_list);

err_probe:
	if (ret < 0)
		soc_remove_component(component, probed);

	return ret;
}

static void soc_remove_link_dais(struct snd_soc_card *card)
{
	struct snd_soc_pcm_runtime *rtd;
	int order;

	for_each_comp_order(order) {
		for_each_card_rtds(card, rtd) {
			/* remove all rtd connected DAIs in good order */
			snd_soc_pcm_dai_remove(rtd, order);
		}
	}
}

static int soc_probe_link_dais(struct snd_soc_card *card)
{
	struct snd_soc_pcm_runtime *rtd;
	int order, ret;

	for_each_comp_order(order) {
		for_each_card_rtds(card, rtd) {
			/* probe all rtd connected DAIs in good order */
			ret = snd_soc_pcm_dai_probe(rtd, order);
			if (ret)
				return ret;
		}
	}

	return 0;
}

static void soc_remove_link_components(struct snd_soc_card *card)
{
	struct snd_soc_component *component;
	struct snd_soc_pcm_runtime *rtd;
	int i, order;

	for_each_comp_order(order) {
		for_each_card_rtds(card, rtd) {
			for_each_rtd_components(rtd, i, component) {
				if (component->driver->remove_order != order)
					continue;

				soc_remove_component(component, 1);
			}
		}
	}
}

static int soc_probe_link_components(struct snd_soc_card *card)
{
	struct snd_soc_component *component;
	struct snd_soc_pcm_runtime *rtd;
	int i, ret, order;

	for_each_comp_order(order) {
		for_each_card_rtds(card, rtd) {
			for_each_rtd_components(rtd, i, component) {
				if (component->driver->probe_order != order)
					continue;

				ret = soc_probe_component(card, component);
				if (ret < 0)
					return ret;
			}
		}
	}

	return 0;
}

static void soc_unbind_aux_dev(struct snd_soc_card *card)
{
	struct snd_soc_component *component, *_component;

	for_each_card_auxs_safe(card, component, _component) {
		/* for snd_soc_component_init() */
		snd_soc_component_set_aux(component, NULL);
		list_del(&component->card_aux_list);
	}
}

static int soc_bind_aux_dev(struct snd_soc_card *card)
{
	struct snd_soc_component *component;
	struct snd_soc_aux_dev *aux;
	int i;

	for_each_card_pre_auxs(card, i, aux) {
		/* codecs, usually analog devices */
		component = soc_find_component(&aux->dlc);
		if (!component)
			return -EPROBE_DEFER;

		/* for snd_soc_component_init() */
		snd_soc_component_set_aux(component, aux);
		/* see for_each_card_auxs */
		list_add(&component->card_aux_list, &card->aux_comp_list);
	}
	return 0;
}

static int soc_probe_aux_devices(struct snd_soc_card *card)
{
	struct snd_soc_component *component;
	int order;
	int ret;

	for_each_comp_order(order) {
		for_each_card_auxs(card, component) {
			if (component->driver->probe_order != order)
				continue;

			ret = soc_probe_component(card,	component);
			if (ret < 0)
				return ret;
		}
	}

	return 0;
}

static void soc_remove_aux_devices(struct snd_soc_card *card)
{
	struct snd_soc_component *comp, *_comp;
	int order;

	for_each_comp_order(order) {
		for_each_card_auxs_safe(card, comp, _comp) {
			if (comp->driver->remove_order == order)
				soc_remove_component(comp, 1);
		}
	}
}

#ifdef CONFIG_DMI
/*
 * If a DMI filed contain strings in this blacklist (e.g.
 * "Type2 - Board Manufacturer" or "Type1 - TBD by OEM"), it will be taken
 * as invalid and dropped when setting the card long name from DMI info.
 */
static const char * const dmi_blacklist[] = {
	"To be filled by OEM",
	"TBD by OEM",
	"Default String",
	"Board Manufacturer",
	"Board Vendor Name",
	"Board Product Name",
	NULL,	/* terminator */
};

/*
 * Trim special characters, and replace '-' with '_' since '-' is used to
 * separate different DMI fields in the card long name. Only number and
 * alphabet characters and a few separator characters are kept.
 */
static void cleanup_dmi_name(char *name)
{
	int i, j = 0;

	for (i = 0; name[i]; i++) {
		if (isalnum(name[i]) || (name[i] == '.')
		    || (name[i] == '_'))
			name[j++] = name[i];
		else if (name[i] == '-')
			name[j++] = '_';
	}

	name[j] = '\0';
}

/*
 * Check if a DMI field is valid, i.e. not containing any string
 * in the black list.
 */
static int is_dmi_valid(const char *field)
{
	int i = 0;

	while (dmi_blacklist[i]) {
		if (strstr(field, dmi_blacklist[i]))
			return 0;
		i++;
	}

	return 1;
}

/*
 * Append a string to card->dmi_longname with character cleanups.
 */
static void append_dmi_string(struct snd_soc_card *card, const char *str)
{
	char *dst = card->dmi_longname;
	size_t dst_len = sizeof(card->dmi_longname);
	size_t len;

	len = strlen(dst);
	snprintf(dst + len, dst_len - len, "-%s", str);

	len++;	/* skip the separator "-" */
	if (len < dst_len)
		cleanup_dmi_name(dst + len);
}

/**
 * snd_soc_set_dmi_name() - Register DMI names to card
 * @card: The card to register DMI names
 * @flavour: The flavour "differentiator" for the card amongst its peers.
 *
 * An Intel machine driver may be used by many different devices but are
 * difficult for userspace to differentiate, since machine drivers usually
 * use their own name as the card short name and leave the card long name
 * blank. To differentiate such devices and fix bugs due to lack of
 * device-specific configurations, this function allows DMI info to be used
 * as the sound card long name, in the format of
 * "vendor-product-version-board"
 * (Character '-' is used to separate different DMI fields here).
 * This will help the user space to load the device-specific Use Case Manager
 * (UCM) configurations for the card.
 *
 * Possible card long names may be:
 * DellInc.-XPS139343-01-0310JH
 * ASUSTeKCOMPUTERINC.-T100TA-1.0-T100TA
 * Circuitco-MinnowboardMaxD0PLATFORM-D0-MinnowBoardMAX
 *
 * This function also supports flavoring the card longname to provide
 * the extra differentiation, like "vendor-product-version-board-flavor".
 *
 * We only keep number and alphabet characters and a few separator characters
 * in the card long name since UCM in the user space uses the card long names
 * as card configuration directory names and AudoConf cannot support special
 * characters like SPACE.
 *
 * Returns 0 on success, otherwise a negative error code.
 */
int snd_soc_set_dmi_name(struct snd_soc_card *card, const char *flavour)
{
	const char *vendor, *product, *board;

	if (card->long_name)
		return 0; /* long name already set by driver or from DMI */

	if (!dmi_available)
		return 0;

	/* make up dmi long name as: vendor-product-version-board */
	vendor = dmi_get_system_info(DMI_BOARD_VENDOR);
	if (!vendor || !is_dmi_valid(vendor)) {
		dev_warn(card->dev, "ASoC: no DMI vendor name!\n");
		return 0;
	}

	snprintf(card->dmi_longname, sizeof(card->dmi_longname), "%s", vendor);
	cleanup_dmi_name(card->dmi_longname);

	product = dmi_get_system_info(DMI_PRODUCT_NAME);
	if (product && is_dmi_valid(product)) {
		const char *product_version = dmi_get_system_info(DMI_PRODUCT_VERSION);

		append_dmi_string(card, product);

		/*
		 * some vendors like Lenovo may only put a self-explanatory
		 * name in the product version field
		 */
		if (product_version && is_dmi_valid(product_version))
			append_dmi_string(card, product_version);
	}

	board = dmi_get_system_info(DMI_BOARD_NAME);
	if (board && is_dmi_valid(board)) {
		if (!product || strcasecmp(board, product))
			append_dmi_string(card, board);
	} else if (!product) {
		/* fall back to using legacy name */
		dev_warn(card->dev, "ASoC: no DMI board/product name!\n");
		return 0;
	}

	/* Add flavour to dmi long name */
	if (flavour)
		append_dmi_string(card, flavour);

	/* set the card long name */
	card->long_name = card->dmi_longname;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_set_dmi_name);
#endif /* CONFIG_DMI */

static void soc_check_tplg_fes(struct snd_soc_card *card)
{
	struct snd_soc_component *component;
	const struct snd_soc_component_driver *comp_drv;
	struct snd_soc_dai_link *dai_link;
	int i;

	for_each_component(component) {

		/* does this component override BEs ? */
		if (!component->driver->ignore_machine)
			continue;

		/* for this machine ? */
		if (!strcmp(component->driver->ignore_machine,
			    card->dev->driver->name))
			goto match;
		if (strcmp(component->driver->ignore_machine,
			   dev_name(card->dev)))
			continue;
match:
		/* machine matches, so override the rtd data */
		for_each_card_prelinks(card, i, dai_link) {

			/* ignore this FE */
			if (dai_link->dynamic) {
				dai_link->ignore = true;
				continue;
			}

			dev_dbg(card->dev, "info: override BE DAI link %s\n",
				card->dai_link[i].name);

			/* override platform component */
			if (!dai_link->platforms) {
				dev_err(card->dev, "init platform error");
				continue;
			}

			if (component->dev->of_node)
				dai_link->platforms->of_node = component->dev->of_node;
			else
				dai_link->platforms->name = component->name;

			/* convert non BE into BE */
			if (!dai_link->no_pcm) {
				dai_link->no_pcm = 1;

				if (dai_link->dpcm_playback)
					dev_warn(card->dev,
						 "invalid configuration, dailink %s has flags no_pcm=0 and dpcm_playback=1\n",
						 dai_link->name);
				if (dai_link->dpcm_capture)
					dev_warn(card->dev,
						 "invalid configuration, dailink %s has flags no_pcm=0 and dpcm_capture=1\n",
						 dai_link->name);

				/* convert normal link into DPCM one */
				if (!(dai_link->dpcm_playback ||
				      dai_link->dpcm_capture)) {
					dai_link->dpcm_playback = !dai_link->capture_only;
					dai_link->dpcm_capture = !dai_link->playback_only;
				}
			}

			/*
			 * override any BE fixups
			 * see
			 *	snd_soc_link_be_hw_params_fixup()
			 */
			dai_link->be_hw_params_fixup =
				component->driver->be_hw_params_fixup;

			/*
			 * most BE links don't set stream name, so set it to
			 * dai link name if it's NULL to help bind widgets.
			 */
			if (!dai_link->stream_name)
				dai_link->stream_name = dai_link->name;
		}

		/* Inform userspace we are using alternate topology */
		if (component->driver->topology_name_prefix) {

			/* topology shortname created? */
			if (!card->topology_shortname_created) {
				comp_drv = component->driver;

				snprintf(card->topology_shortname, 32, "%s-%s",
					 comp_drv->topology_name_prefix,
					 card->name);
				card->topology_shortname_created = true;
			}

			/* use topology shortname */
			card->name = card->topology_shortname;
		}
	}
}

#define soc_setup_card_name(card, name, name1, name2) \
	__soc_setup_card_name(card, name, sizeof(name), name1, name2)
static void __soc_setup_card_name(struct snd_soc_card *card,
				  char *name, int len,
				  const char *name1, const char *name2)
{
	const char *src = name1 ? name1 : name2;
	int i;

	snprintf(name, len, "%s", src);

	if (name != card->snd_card->driver)
		return;

	/*
	 * Name normalization (driver field)
	 *
	 * The driver name is somewhat special, as it's used as a key for
	 * searches in the user-space.
	 *
	 * ex)
	 *	"abcd??efg" -> "abcd__efg"
	 */
	for (i = 0; i < len; i++) {
		switch (name[i]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case '_':
		case '-':
		case '\0':
			break;
		default:
<<<<<<< HEAD
			if (!isalnum(card->snd_card->driver[i]))
				card->snd_card->driver[i] = '_';
=======
			if (!isalnum(name[i]))
				name[i] = '_';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}

<<<<<<< HEAD
	if (card->late_probe) {
		ret = card->late_probe(card);
		if (ret < 0) {
			dev_err(card->dev, "%s late_probe() failed: %d\n",
				card->name, ret);
			goto probe_aux_dev_err;
		}
	}

	snd_soc_dapm_new_widgets(&card->dapm);
	soc_init_card_aif_channel_map(card);

	if (card->fully_routed)
		list_for_each_entry(codec, &card->codec_dev_list, card_list)
			snd_soc_dapm_auto_nc_codec_pins(codec);

	ret = snd_card_register(card->snd_card);
	if (ret < 0) {
		printk(KERN_ERR "asoc: failed to register soundcard for %s\n", card->name);
		goto probe_aux_dev_err;
	}

#ifdef CONFIG_SND_SOC_AC97_BUS
	/* register any AC97 codecs */
	for (i = 0; i < card->num_rtd; i++) {
		ret = soc_register_ac97_dai_link(&card->rtd[i]);
		if (ret < 0) {
			printk(KERN_ERR "asoc: failed to register AC97 %s\n", card->name);
			while (--i >= 0)
				soc_unregister_ac97_dai_link(card->rtd[i].codec);
			goto probe_aux_dev_err;
		}
	}
#endif

	card->instantiated = 1;
	snd_soc_dapm_sync(&card->dapm);
	mutex_unlock(&card->mutex);
	return 0;

probe_aux_dev_err:
	for (i = 0; i < card->num_aux_devs; i++)
		soc_remove_aux_dev(card, i);

probe_dai_err:
	soc_remove_dai_links(card);

card_probe_error:
	if (card->remove)
		card->remove(card);

	snd_card_free(card->snd_card);
base_error:
	mutex_unlock(&card->mutex);
=======
	/*
	 * The driver field should contain a valid string from the user view.
	 * The wrapping usually does not work so well here. Set a smaller string
	 * in the specific ASoC driver.
	 */
	if (strlen(src) > len - 1)
		dev_err(card->dev, "ASoC: driver name too long '%s' -> '%s'\n", src, name);
}

static void soc_cleanup_card_resources(struct snd_soc_card *card)
{
	struct snd_soc_pcm_runtime *rtd, *n;

	if (card->snd_card)
		snd_card_disconnect_sync(card->snd_card);

	snd_soc_dapm_shutdown(card);

	/* release machine specific resources */
	for_each_card_rtds(card, rtd)
		if (rtd->initialized)
			snd_soc_link_exit(rtd);
	/* remove and free each DAI */
	soc_remove_link_dais(card);
	soc_remove_link_components(card);

	for_each_card_rtds_safe(card, rtd, n)
		snd_soc_remove_pcm_runtime(card, rtd);

	/* remove auxiliary devices */
	soc_remove_aux_devices(card);
	soc_unbind_aux_dev(card);

	snd_soc_dapm_free(&card->dapm);
	soc_cleanup_card_debugfs(card);

	/* remove the card */
	snd_soc_card_remove(card);

	if (card->snd_card) {
		snd_card_free(card->snd_card);
		card->snd_card = NULL;
	}
}

static void snd_soc_unbind_card(struct snd_soc_card *card, bool unregister)
{
	if (snd_soc_card_is_instantiated(card)) {
		card->instantiated = false;
		snd_soc_flush_all_delayed_work(card);

		soc_cleanup_card_resources(card);
		if (!unregister)
			list_add(&card->list, &unbind_card_list);
	} else {
		if (unregister)
			list_del(&card->list);
	}
}

static int snd_soc_bind_card(struct snd_soc_card *card)
{
	struct snd_soc_pcm_runtime *rtd;
	struct snd_soc_component *component;
	int ret;

	mutex_lock(&client_mutex);
	snd_soc_card_mutex_lock_root(card);

	snd_soc_fill_dummy_dai(card);

	snd_soc_dapm_init(&card->dapm, card, NULL);

	/* check whether any platform is ignore machine FE and using topology */
	soc_check_tplg_fes(card);

	/* bind aux_devs too */
	ret = soc_bind_aux_dev(card);
	if (ret < 0)
		goto probe_end;

	/* add predefined DAI links to the list */
	card->num_rtd = 0;
	ret = snd_soc_add_pcm_runtimes(card, card->dai_link, card->num_links);
	if (ret < 0)
		goto probe_end;

	/* card bind complete so register a sound card */
	ret = snd_card_new(card->dev, SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1,
			card->owner, 0, &card->snd_card);
	if (ret < 0) {
		dev_err(card->dev,
			"ASoC: can't create sound card for card %s: %d\n",
			card->name, ret);
		goto probe_end;
	}

	soc_init_card_debugfs(card);

	soc_resume_init(card);

	ret = snd_soc_dapm_new_controls(&card->dapm, card->dapm_widgets,
					card->num_dapm_widgets);
	if (ret < 0)
		goto probe_end;

	ret = snd_soc_dapm_new_controls(&card->dapm, card->of_dapm_widgets,
					card->num_of_dapm_widgets);
	if (ret < 0)
		goto probe_end;

	/* initialise the sound card only once */
	ret = snd_soc_card_probe(card);
	if (ret < 0)
		goto probe_end;

	/* probe all components used by DAI links on this card */
	ret = soc_probe_link_components(card);
	if (ret < 0) {
		if (ret != -EPROBE_DEFER) {
			dev_err(card->dev,
				"ASoC: failed to instantiate card %d\n", ret);
		}
		goto probe_end;
	}

	/* probe auxiliary components */
	ret = soc_probe_aux_devices(card);
	if (ret < 0) {
		dev_err(card->dev,
			"ASoC: failed to probe aux component %d\n", ret);
		goto probe_end;
	}

	/* probe all DAI links on this card */
	ret = soc_probe_link_dais(card);
	if (ret < 0) {
		dev_err(card->dev,
			"ASoC: failed to instantiate card %d\n", ret);
		goto probe_end;
	}

	for_each_card_rtds(card, rtd) {
		ret = soc_init_pcm_runtime(card, rtd);
		if (ret < 0)
			goto probe_end;
	}

	snd_soc_dapm_link_dai_widgets(card);
	snd_soc_dapm_connect_dai_link_widgets(card);

	ret = snd_soc_add_card_controls(card, card->controls,
					card->num_controls);
	if (ret < 0)
		goto probe_end;

	ret = snd_soc_dapm_add_routes(&card->dapm, card->dapm_routes,
				      card->num_dapm_routes);
	if (ret < 0) {
		if (card->disable_route_checks) {
			dev_info(card->dev,
				 "%s: disable_route_checks set, ignoring errors on add_routes\n",
				 __func__);
		} else {
			dev_err(card->dev,
				 "%s: snd_soc_dapm_add_routes failed: %d\n",
				 __func__, ret);
			goto probe_end;
		}
	}

	ret = snd_soc_dapm_add_routes(&card->dapm, card->of_dapm_routes,
				      card->num_of_dapm_routes);
	if (ret < 0)
		goto probe_end;

	/* try to set some sane longname if DMI is available */
	snd_soc_set_dmi_name(card, NULL);

	soc_setup_card_name(card, card->snd_card->shortname,
			    card->name, NULL);
	soc_setup_card_name(card, card->snd_card->longname,
			    card->long_name, card->name);
	soc_setup_card_name(card, card->snd_card->driver,
			    card->driver_name, card->name);

	if (card->components) {
		/* the current implementation of snd_component_add() accepts */
		/* multiple components in the string separated by space, */
		/* but the string collision (identical string) check might */
		/* not work correctly */
		ret = snd_component_add(card->snd_card, card->components);
		if (ret < 0) {
			dev_err(card->dev, "ASoC: %s snd_component_add() failed: %d\n",
				card->name, ret);
			goto probe_end;
		}
	}

	ret = snd_soc_card_late_probe(card);
	if (ret < 0)
		goto probe_end;

	snd_soc_dapm_new_widgets(card);
	snd_soc_card_fixup_controls(card);

	ret = snd_card_register(card->snd_card);
	if (ret < 0) {
		dev_err(card->dev, "ASoC: failed to register soundcard %d\n",
				ret);
		goto probe_end;
	}

	card->instantiated = 1;
	dapm_mark_endpoints_dirty(card);
	snd_soc_dapm_sync(&card->dapm);

	/* deactivate pins to sleep state */
	for_each_card_components(card, component)
		if (!snd_soc_component_active(component))
			pinctrl_pm_select_sleep_state(component->dev);

probe_end:
	if (ret < 0)
		soc_cleanup_card_resources(card);

	snd_soc_card_mutex_unlock(card);
	mutex_unlock(&client_mutex);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* probes a new socdev */
static int soc_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * no card, so machine driver should be registering card
	 * we should not be here in that case so ret error
	 */
	if (!card)
		return -EINVAL;

<<<<<<< HEAD
	/* Bodge while we unpick instantiation */
	card->dev = &pdev->dev;

	ret = snd_soc_register_card(card);
	if (ret != 0) {
		dev_err(&pdev->dev, "Failed to register card\n");
		return ret;
	}

	return 0;
}

static int soc_cleanup_card_resources(struct snd_soc_card *card)
{
	int i;

	/* make sure any delayed work runs */
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_pcm_runtime *rtd = &card->rtd[i];
		flush_delayed_work_sync(&rtd->delayed_work);
	}

	/* remove auxiliary devices */
	for (i = 0; i < card->num_aux_devs; i++)
		soc_remove_aux_dev(card, i);

	/* remove and free each DAI */
	soc_remove_dai_links(card);

	soc_cleanup_card_debugfs(card);

	/* remove the card */
	if (card->remove)
		card->remove(card);

	snd_soc_dapm_free(&card->dapm);

	kfree(card->rtd);
	snd_card_free(card->snd_card);
	return 0;

}

/* removes a socdev */
static int soc_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	snd_soc_unregister_card(card);
	return 0;
=======
	dev_warn(&pdev->dev,
		 "ASoC: machine %s should use snd_soc_register_card()\n",
		 card->name);

	/* Bodge while we unpick instantiation */
	card->dev = &pdev->dev;

	return devm_snd_soc_register_card(&pdev->dev, card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int snd_soc_poweroff(struct device *dev)
{
	struct snd_soc_card *card = dev_get_drvdata(dev);
<<<<<<< HEAD
	int i;

	if (!card->instantiated)
		return 0;

	/* Flush out pmdown_time work - we actually do want to run it
	 * now, we're shutting down so no imminent restart. */
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_pcm_runtime *rtd = &card->rtd[i];
		flush_delayed_work_sync(&rtd->delayed_work);
	}

	snd_soc_dapm_shutdown(card);

=======
	struct snd_soc_component *component;

	if (!snd_soc_card_is_instantiated(card))
		return 0;

	/*
	 * Flush out pmdown_time work - we actually do want to run it
	 * now, we're shutting down so no imminent restart.
	 */
	snd_soc_flush_all_delayed_work(card);

	snd_soc_dapm_shutdown(card);

	/* deactivate pins to sleep state */
	for_each_card_components(card, component)
		pinctrl_pm_select_sleep_state(component->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_poweroff);

const struct dev_pm_ops snd_soc_pm_ops = {
	.suspend = snd_soc_suspend,
	.resume = snd_soc_resume,
<<<<<<< HEAD
	.poweroff = snd_soc_poweroff,
=======
	.freeze = snd_soc_suspend,
	.thaw = snd_soc_resume,
	.poweroff = snd_soc_poweroff,
	.restore = snd_soc_resume,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
EXPORT_SYMBOL_GPL(snd_soc_pm_ops);

/* ASoC platform driver */
static struct platform_driver soc_driver = {
	.driver		= {
		.name		= "soc-audio",
<<<<<<< HEAD
		.owner		= THIS_MODULE,
		.pm		= &snd_soc_pm_ops,
	},
	.probe		= soc_probe,
	.remove		= soc_remove,
};

/**
 * snd_soc_codec_volatile_register: Report if a register is volatile.
 *
 * @codec: CODEC to query.
 * @reg: Register to query.
 *
 * Boolean function indiciating if a CODEC register is volatile.
 */
int snd_soc_codec_volatile_register(struct snd_soc_codec *codec,
				    unsigned int reg)
{
	if (codec->volatile_register)
		return codec->volatile_register(codec, reg);
	else
		return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_volatile_register);

/**
 * snd_soc_codec_readable_register: Report if a register is readable.
 *
 * @codec: CODEC to query.
 * @reg: Register to query.
 *
 * Boolean function indicating if a CODEC register is readable.
 */
int snd_soc_codec_readable_register(struct snd_soc_codec *codec,
				    unsigned int reg)
{
	if (codec->readable_register)
		return codec->readable_register(codec, reg);
	else
		return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_readable_register);

/**
 * snd_soc_codec_writable_register: Report if a register is writable.
 *
 * @codec: CODEC to query.
 * @reg: Register to query.
 *
 * Boolean function indicating if a CODEC register is writable.
 */
int snd_soc_codec_writable_register(struct snd_soc_codec *codec,
				    unsigned int reg)
{
	if (codec->writable_register)
		return codec->writable_register(codec, reg);
	else
		return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_writable_register);

int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg)
{
	unsigned int ret;

	if (!platform->driver->read) {
		dev_err(platform->dev, "platform has no read back\n");
		return -1;
	}

	ret = platform->driver->read(platform, reg);
	dev_dbg(platform->dev, "read %x => %x\n", reg, ret);
	trace_snd_soc_preg_read(platform, reg, ret);

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_platform_read);

int snd_soc_platform_write(struct snd_soc_platform *platform,
					 unsigned int reg, unsigned int val)
{
	if (!platform->driver->write) {
		dev_err(platform->dev, "platform has no write back\n");
		return -1;
	}

	dev_dbg(platform->dev, "write %x = %x\n", reg, val);
	trace_snd_soc_preg_write(platform, reg, val);
	return platform->driver->write(platform, reg, val);
}
EXPORT_SYMBOL_GPL(snd_soc_platform_write);

/**
 * snd_soc_new_ac97_codec - initailise AC97 device
 * @codec: audio codec
 * @ops: AC97 bus operations
 * @num: AC97 codec number
 *
 * Initialises AC97 codec resources for use by ad-hoc devices only.
 */
int snd_soc_new_ac97_codec(struct snd_soc_codec *codec,
	struct snd_ac97_bus_ops *ops, int num)
{
	mutex_lock(&codec->mutex);

	codec->ac97 = kzalloc(sizeof(struct snd_ac97), GFP_KERNEL);
	if (codec->ac97 == NULL) {
		mutex_unlock(&codec->mutex);
		return -ENOMEM;
	}

	codec->ac97->bus = kzalloc(sizeof(struct snd_ac97_bus), GFP_KERNEL);
	if (codec->ac97->bus == NULL) {
		kfree(codec->ac97);
		codec->ac97 = NULL;
		mutex_unlock(&codec->mutex);
		return -ENOMEM;
	}

	codec->ac97->bus->ops = ops;
	codec->ac97->num = num;

	/*
	 * Mark the AC97 device to be created by us. This way we ensure that the
	 * device will be registered with the device subsystem later on.
	 */
	codec->ac97_created = 1;

	mutex_unlock(&codec->mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_new_ac97_codec);

/**
 * snd_soc_free_ac97_codec - free AC97 codec device
 * @codec: audio codec
 *
 * Frees AC97 codec device resources.
 */
void snd_soc_free_ac97_codec(struct snd_soc_codec *codec)
{
	mutex_lock(&codec->mutex);
#ifdef CONFIG_SND_SOC_AC97_BUS
	soc_unregister_ac97_dai_link(codec);
#endif
	kfree(codec->ac97->bus);
	kfree(codec->ac97);
	codec->ac97 = NULL;
	codec->ac97_created = 0;
	mutex_unlock(&codec->mutex);
}
EXPORT_SYMBOL_GPL(snd_soc_free_ac97_codec);

unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg)
{
	unsigned int ret;

	if (unlikely(!snd_card_is_online_state(codec->card->snd_card))) {
		dev_err(codec->dev, "read 0x%02x while offline\n", reg);
		return -ENODEV;
	}
	ret = codec->read(codec, reg);
	dev_dbg(codec->dev, "read %x => %x\n", reg, ret);
	trace_snd_soc_reg_read(codec, reg, ret);

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_read);

unsigned int snd_soc_write(struct snd_soc_codec *codec,
			   unsigned int reg, unsigned int val)
{
	if (unlikely(!snd_card_is_online_state(codec->card->snd_card))) {
		dev_err(codec->dev, "write 0x%02x while offline\n", reg);
		return -ENODEV;
	}
	dev_dbg(codec->dev, "write %x = %x\n", reg, val);
	trace_snd_soc_reg_write(codec, reg, val);
	return codec->write(codec, reg, val);
}
EXPORT_SYMBOL_GPL(snd_soc_write);

unsigned int snd_soc_bulk_write_raw(struct snd_soc_codec *codec,
				    unsigned int reg, const void *data, size_t len)
{
	return codec->bulk_write_raw(codec, reg, data, len);
}
EXPORT_SYMBOL_GPL(snd_soc_bulk_write_raw);

/**
 * snd_soc_update_bits - update codec register bits
 * @codec: audio codec
 * @reg: codec register
 * @mask: register mask
 * @value: new value
 *
 * Writes new register value.
 *
 * Returns 1 for change, 0 for no change, or negative error code.
 */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value)
{
	bool change;
	unsigned int old, new;
	int ret;

	if (codec->using_regmap) {
		ret = regmap_update_bits_check(codec->control_data, reg,
					       mask, value, &change);
	} else {
		ret = snd_soc_read(codec, reg);
		if (ret < 0)
			return ret;

		old = ret;
		new = (old & ~mask) | (value & mask);
		change = old != new;
		if (change)
			ret = snd_soc_write(codec, reg, new);
	}

	if (ret < 0)
		return ret;

	return change;
}
EXPORT_SYMBOL_GPL(snd_soc_update_bits);

/**
 * snd_soc_update_bits_locked - update codec register bits
 * @codec: audio codec
 * @reg: codec register
 * @mask: register mask
 * @value: new value
 *
 * Writes new register value, and takes the codec mutex.
 *
 * Returns 1 for change else 0.
 */
int snd_soc_update_bits_locked(struct snd_soc_codec *codec,
			       unsigned short reg, unsigned int mask,
			       unsigned int value)
{
	int change;

	mutex_lock(&codec->mutex);
	change = snd_soc_update_bits(codec, reg, mask, value);
	mutex_unlock(&codec->mutex);

	return change;
}
EXPORT_SYMBOL_GPL(snd_soc_update_bits_locked);

/**
 * snd_soc_test_bits - test register for change
 * @codec: audio codec
 * @reg: codec register
 * @mask: register mask
 * @value: new value
 *
 * Tests a register with a new value and checks if the new value is
 * different from the old value.
 *
 * Returns 1 for change else 0.
 */
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned short reg,
				unsigned int mask, unsigned int value)
{
	int change;
	unsigned int old, new;

	old = snd_soc_read(codec, reg);
	new = (old & ~mask) | value;
	change = old != new;

	return change;
}
EXPORT_SYMBOL_GPL(snd_soc_test_bits);

/**
 * snd_soc_set_runtime_hwparams - set the runtime hardware parameters
 * @substream: the pcm substream
 * @hw: the hardware parameters
 *
 * Sets the substream runtime hardware parameters.
 */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	if (!runtime)
		return 0;
	runtime->hw.info = hw->info;
	runtime->hw.formats = hw->formats;
	runtime->hw.period_bytes_min = hw->period_bytes_min;
	runtime->hw.period_bytes_max = hw->period_bytes_max;
	runtime->hw.periods_min = hw->periods_min;
	runtime->hw.periods_max = hw->periods_max;
	runtime->hw.buffer_bytes_max = hw->buffer_bytes_max;
	runtime->hw.fifo_size = hw->fifo_size;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_set_runtime_hwparams);

/**
=======
		.pm		= &snd_soc_pm_ops,
	},
	.probe		= soc_probe,
};

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * snd_soc_cnew - create new control
 * @_template: control template
 * @data: control private data
 * @long_name: control long name
 * @prefix: control name prefix
 *
 * Create a new mixer control from a template control.
 *
 * Returns 0 for success, else error.
 */
struct snd_kcontrol *snd_soc_cnew(const struct snd_kcontrol_new *_template,
<<<<<<< HEAD
#ifdef CONFIG_SND_SOC_WM5110
				  void *data, const char *long_name,
#else
				  void *data, char *long_name,
#endif
=======
				  void *data, const char *long_name,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  const char *prefix)
{
	struct snd_kcontrol_new template;
	struct snd_kcontrol *kcontrol;
	char *name = NULL;
<<<<<<< HEAD
	int name_len;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(&template, _template, sizeof(template));
	template.index = 0;

	if (!long_name)
		long_name = template.name;

	if (prefix) {
<<<<<<< HEAD
		name_len = strlen(long_name) + strlen(prefix) + 2;
		name = kmalloc(name_len, GFP_KERNEL);
		if (!name)
			return NULL;

		snprintf(name, name_len, "%s %s", prefix, long_name);

=======
		name = kasprintf(GFP_KERNEL, "%s %s", prefix, long_name);
		if (!name)
			return NULL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		template.name = name;
	} else {
		template.name = long_name;
	}

	kcontrol = snd_ctl_new1(&template, data);

	kfree(name);

	return kcontrol;
}
EXPORT_SYMBOL_GPL(snd_soc_cnew);

static int snd_soc_add_controls(struct snd_card *card, struct device *dev,
	const struct snd_kcontrol_new *controls, int num_controls,
	const char *prefix, void *data)
{
<<<<<<< HEAD
	int err, i;

	for (i = 0; i < num_controls; i++) {
		const struct snd_kcontrol_new *control = &controls[i];
		err = snd_ctl_add(card, snd_soc_cnew(control, data,
						     control->name, prefix));
		if (err < 0) {
			dev_err(dev, "Failed to add %s: %d\n", control->name, err);
=======
	int i;

	for (i = 0; i < num_controls; i++) {
		const struct snd_kcontrol_new *control = &controls[i];
		int err = snd_ctl_add(card, snd_soc_cnew(control, data,
							 control->name, prefix));
		if (err < 0) {
			dev_err(dev, "ASoC: Failed to add %s: %d\n",
				control->name, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
	}

	return 0;
}

/**
<<<<<<< HEAD
 * snd_soc_add_codec_controls - add an array of controls to a codec.
 * Convenience function to add a list of controls. Many codecs were
 * duplicating this code.
 *
 * @codec: codec to add controls to
 * @controls: array of controls to add
 * @num_controls: number of elements in the array
 *
 * Return 0 for success, else error.
 */
int snd_soc_add_codec_controls(struct snd_soc_codec *codec,
	const struct snd_kcontrol_new *controls, int num_controls)
{
	struct snd_card *card = codec->card->snd_card;

	return snd_soc_add_controls(card, codec->dev, controls, num_controls,
			codec->name_prefix, codec);
}
EXPORT_SYMBOL_GPL(snd_soc_add_codec_controls);

/**
 * snd_soc_add_platform_controls - add an array of controls to a platform.
 * Convenience function to add a list of controls.
 *
 * @platform: platform to add controls to
 * @controls: array of controls to add
 * @num_controls: number of elements in the array
 *
 * Return 0 for success, else error.
 */
int snd_soc_add_platform_controls(struct snd_soc_platform *platform,
	const struct snd_kcontrol_new *controls, int num_controls)
{
	struct snd_card *card = platform->card->snd_card;

	return snd_soc_add_controls(card, platform->dev, controls, num_controls,
			NULL, platform);
}
EXPORT_SYMBOL_GPL(snd_soc_add_platform_controls);
=======
 * snd_soc_add_component_controls - Add an array of controls to a component.
 *
 * @component: Component to add controls to
 * @controls: Array of controls to add
 * @num_controls: Number of elements in the array
 *
 * Return: 0 for success, else error.
 */
int snd_soc_add_component_controls(struct snd_soc_component *component,
	const struct snd_kcontrol_new *controls, unsigned int num_controls)
{
	struct snd_card *card = component->card->snd_card;

	return snd_soc_add_controls(card, component->dev, controls,
			num_controls, component->name_prefix, component);
}
EXPORT_SYMBOL_GPL(snd_soc_add_component_controls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * snd_soc_add_card_controls - add an array of controls to a SoC card.
 * Convenience function to add a list of controls.
 *
 * @soc_card: SoC card to add controls to
 * @controls: array of controls to add
 * @num_controls: number of elements in the array
 *
 * Return 0 for success, else error.
 */
int snd_soc_add_card_controls(struct snd_soc_card *soc_card,
	const struct snd_kcontrol_new *controls, int num_controls)
{
	struct snd_card *card = soc_card->snd_card;

	return snd_soc_add_controls(card, soc_card->dev, controls, num_controls,
			NULL, soc_card);
}
EXPORT_SYMBOL_GPL(snd_soc_add_card_controls);

/**
 * snd_soc_add_dai_controls - add an array of controls to a DAI.
<<<<<<< HEAD
 * Convienience function to add a list of controls.
=======
 * Convenience function to add a list of controls.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * @dai: DAI to add controls to
 * @controls: array of controls to add
 * @num_controls: number of elements in the array
 *
 * Return 0 for success, else error.
 */
int snd_soc_add_dai_controls(struct snd_soc_dai *dai,
	const struct snd_kcontrol_new *controls, int num_controls)
{
<<<<<<< HEAD
	struct snd_card *card = dai->card->snd_card;
=======
	struct snd_card *card = dai->component->card->snd_card;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return snd_soc_add_controls(card, dai->dev, controls, num_controls,
			NULL, dai);
}
EXPORT_SYMBOL_GPL(snd_soc_add_dai_controls);

/**
<<<<<<< HEAD
 * snd_soc_info_enum_double - enumerated double mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about a double enumerated
 * mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_info_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	uinfo->count = e->shift_l == e->shift_r ? 1 : 2;
	uinfo->value.enumerated.items = e->max;

	if (uinfo->value.enumerated.item > e->max - 1)
		uinfo->value.enumerated.item = e->max - 1;
	strcpy(uinfo->value.enumerated.name,
		snd_soc_get_enum_text(e, uinfo->value.enumerated.item));

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_enum_double);

/**
 * snd_soc_get_enum_double - enumerated double mixer get callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value of a double enumerated mixer.
 *
 * Returns 0 for success.
 */
int snd_soc_get_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	unsigned int val, bitmask;

	for (bitmask = 1; bitmask < e->max; bitmask <<= 1)
		;
	val = snd_soc_read(codec, e->reg);
	ucontrol->value.enumerated.item[0]
		= (val >> e->shift_l) & (bitmask - 1);
	if (e->shift_l != e->shift_r)
		ucontrol->value.enumerated.item[1] =
			(val >> e->shift_r) & (bitmask - 1);

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_enum_double);

/**
 * snd_soc_put_enum_double - enumerated double mixer put callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to set the value of a double enumerated mixer.
 *
 * Returns 0 for success.
 */
int snd_soc_put_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	unsigned int val;
	unsigned int mask, bitmask;

	for (bitmask = 1; bitmask < e->max; bitmask <<= 1)
		;
	if (ucontrol->value.enumerated.item[0] > e->max - 1)
		return -EINVAL;
	val = ucontrol->value.enumerated.item[0] << e->shift_l;
	mask = (bitmask - 1) << e->shift_l;
	if (e->shift_l != e->shift_r) {
		if (ucontrol->value.enumerated.item[1] > e->max - 1)
			return -EINVAL;
		val |= ucontrol->value.enumerated.item[1] << e->shift_r;
		mask |= (bitmask - 1) << e->shift_r;
	}

	return snd_soc_update_bits_locked(codec, e->reg, mask, val);
}
EXPORT_SYMBOL_GPL(snd_soc_put_enum_double);

/**
 * snd_soc_get_value_enum_double - semi enumerated double mixer get callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value of a double semi enumerated mixer.
 *
 * Semi enumerated mixer: the enumerated items are referred as values. Can be
 * used for handling bitfield coded enumeration for example.
 *
 * Returns 0 for success.
 */
int snd_soc_get_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	unsigned int reg_val, val, mux;

	reg_val = snd_soc_read(codec, e->reg);
	val = (reg_val >> e->shift_l) & e->mask;
	for (mux = 0; mux < e->max; mux++) {
		if (val == e->values[mux])
			break;
	}
	ucontrol->value.enumerated.item[0] = mux;
	if (e->shift_l != e->shift_r) {
		val = (reg_val >> e->shift_r) & e->mask;
		for (mux = 0; mux < e->max; mux++) {
			if (val == e->values[mux])
				break;
		}
		ucontrol->value.enumerated.item[1] = mux;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_value_enum_double);

/**
 * snd_soc_put_value_enum_double - semi enumerated double mixer put callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to set the value of a double semi enumerated mixer.
 *
 * Semi enumerated mixer: the enumerated items are referred as values. Can be
 * used for handling bitfield coded enumeration for example.
 *
 * Returns 0 for success.
 */
int snd_soc_put_value_enum_double(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	unsigned int val;
	unsigned int mask;

	if (ucontrol->value.enumerated.item[0] > e->max - 1)
		return -EINVAL;
	val = e->values[ucontrol->value.enumerated.item[0]] << e->shift_l;
	mask = e->mask << e->shift_l;
	if (e->shift_l != e->shift_r) {
		if (ucontrol->value.enumerated.item[1] > e->max - 1)
			return -EINVAL;
		val |= e->values[ucontrol->value.enumerated.item[1]] << e->shift_r;
		mask |= e->mask << e->shift_r;
	}

	return snd_soc_update_bits_locked(codec, e->reg, mask, val);
}
EXPORT_SYMBOL_GPL(snd_soc_put_value_enum_double);

/**
 * snd_soc_info_enum_ext - external enumerated single mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about an external enumerated
 * single mixer.
 *
 * Returns 0 for success.
 */
int snd_soc_info_enum_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	uinfo->count = 1;
	uinfo->value.enumerated.items = e->max;

	if (uinfo->value.enumerated.item > e->max - 1)
		uinfo->value.enumerated.item = e->max - 1;
	strcpy(uinfo->value.enumerated.name,
		snd_soc_get_enum_text(e, uinfo->value.enumerated.item));
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_enum_ext);

/**
 * snd_soc_info_volsw_ext - external single mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about a single external mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_info_volsw_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	int max = kcontrol->private_value;

	if (max == 1 && !strstr(kcontrol->id.name, " Volume"))
		uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	else
		uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;

	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = max;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_volsw_ext);

/**
 * snd_soc_info_multi_ext - external single mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about a single external mixer control.
 * that accepts multiple input.
 *
 * Returns 0 for success.
 */
int snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_multi_mixer_control *mc =
		(struct soc_multi_mixer_control *)kcontrol->private_value;
	int platform_max;

	if (!mc->platform_max)
		mc->platform_max = mc->max;
	platform_max = mc->platform_max;

	if (platform_max == 1 && !strnstr(kcontrol->id.name, " Volume", 30))
		uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	else
		uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;

	uinfo->count = mc->count;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = platform_max;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_multi_ext);

/**
 * snd_soc_info_volsw - single mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about a single mixer control, or a double
 * mixer control that spans 2 registers.
 *
 * Returns 0 for success.
 */
int snd_soc_info_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int platform_max;

	if (!mc->platform_max)
		mc->platform_max = mc->max;
	platform_max = mc->platform_max;

	if (platform_max == 1 && !strstr(kcontrol->id.name, " Volume"))
		uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	else
		uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;

	uinfo->count = snd_soc_volsw_is_stereo(mc) ? 2 : 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = platform_max;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_volsw);

/**
 * snd_soc_get_volsw - single mixer get callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value of a single mixer control, or a double mixer
 * control that spans 2 registers.
 *
 * Returns 0 for success.
 */
int snd_soc_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int max = mc->max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;

	ucontrol->value.integer.value[0] =
		(snd_soc_read(codec, reg) >> shift) & mask;
	if (invert)
		ucontrol->value.integer.value[0] =
			max - ucontrol->value.integer.value[0];

	if (snd_soc_volsw_is_stereo(mc)) {
		if (reg == reg2)
			ucontrol->value.integer.value[1] =
				(snd_soc_read(codec, reg) >> rshift) & mask;
		else
			ucontrol->value.integer.value[1] =
				(snd_soc_read(codec, reg2) >> shift) & mask;
		if (invert)
			ucontrol->value.integer.value[1] =
				max - ucontrol->value.integer.value[1];
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_volsw);

/**
 * snd_soc_put_volsw - single mixer put callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to set the value of a single mixer control, or a double mixer
 * control that spans 2 registers.
 *
 * Returns 0 for success.
 */
int snd_soc_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int max = mc->max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;
	int err;
	bool type_2r = 0;
	unsigned int val2 = 0;
	unsigned int val, val_mask;

	val = (ucontrol->value.integer.value[0] & mask);
	if (invert)
		val = max - val;
	val_mask = mask << shift;
	val = val << shift;
	if (snd_soc_volsw_is_stereo(mc)) {
		val2 = (ucontrol->value.integer.value[1] & mask);
		if (invert)
			val2 = max - val2;
		if (reg == reg2) {
			val_mask |= mask << rshift;
			val |= val2 << rshift;
		} else {
			val2 = val2 << shift;
			type_2r = 1;
		}
	}
	err = snd_soc_update_bits_locked(codec, reg, val_mask, val);
	if (err < 0)
		return err;

	if (type_2r)
		err = snd_soc_update_bits_locked(codec, reg2, val_mask, val2);

	return err;
}
EXPORT_SYMBOL_GPL(snd_soc_put_volsw);

#ifdef CONFIG_SND_SOC_WM5110
/* LGE_CHANGE, yeri.lee@lge.com, 2013-10-31, wm5110 Bring up */
/**
 * snd_soc_get_volsw_sx - single mixer get callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value of a single mixer control, or a double mixer
 * control that spans 2 registers.
 *
 * Returns 0 for success.
 */
int snd_soc_get_volsw_sx(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_mixer_control *mc =
	    (struct soc_mixer_control *)kcontrol->private_value;

	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int max = mc->max;
	int min = mc->min;
	int mask = (1 << (fls(min + max) - 1)) - 1;

	ucontrol->value.integer.value[0] =
	    ((snd_soc_read(codec, reg) >> shift) - min) & mask;

	if (snd_soc_volsw_is_stereo(mc))
		ucontrol->value.integer.value[1] =
			((snd_soc_read(codec, reg2) >> rshift) - min) & mask;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_volsw_sx);

/**
 * snd_soc_put_volsw_sx - double mixer set callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to set the value of a double mixer control that spans 2 registers.
 *
 * Returns 0 for success.
 */
int snd_soc_put_volsw_sx(struct snd_kcontrol *kcontrol,
			 struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_mixer_control *mc =
	    (struct soc_mixer_control *)kcontrol->private_value;

	unsigned int reg = mc->reg;
	unsigned int reg2 = mc->rreg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int max = mc->max;
	int min = mc->min;
	int mask = (1 << (fls(min + max) - 1)) - 1;
	int err;
	unsigned short val, val_mask, val2 = 0;

	val_mask = mask << shift;
	val = (ucontrol->value.integer.value[0] + min) & mask;
	val = val << shift;

	if (snd_soc_update_bits_locked(codec, reg, val_mask, val))
			return err;

	if (snd_soc_volsw_is_stereo(mc)) {
		val_mask = mask << rshift;
		val2 = (ucontrol->value.integer.value[1] + min) & mask;
		val2 = val2 << rshift;

		if (snd_soc_update_bits_locked(codec, reg2, val_mask, val2))
			return err;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_put_volsw_sx);
#endif

/**
 * snd_soc_info_volsw_s8 - signed mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information about a signed mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_info_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int platform_max;
	int min = mc->min;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;

	if (!mc->platform_max)
		mc->platform_max = mc->max;
	platform_max = mc->platform_max;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = shift == rshift ? 1 : 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = platform_max - min;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_volsw_s8);

/**
 * snd_soc_get_volsw_s8 - signed mixer get callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value of a signed mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_get_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int min = mc->min;
	int val = snd_soc_read(codec, reg);

	ucontrol->value.integer.value[0] =
		((signed char)((val >> shift) & 0xff))-min;
	if (shift != rshift)
		ucontrol->value.integer.value[1] =
			((signed char)((val >> rshift) & 0xff))-min;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_volsw_s8);

/**
 * snd_soc_put_volsw_sgn - signed mixer put callback
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to set the value of a signed mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_put_volsw_s8(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int min = mc->min;
	unsigned int val, val2, val_mask;

	val = ((ucontrol->value.integer.value[0]+min) & 0xff) << shift;
	val_mask = 0xff << shift;
	if (shift != rshift) {
		val2 = (ucontrol->value.integer.value[1]+min) & 0xff;
		val |= val2 << rshift;
		val_mask |= 0xff << rshift;
	}

	return snd_soc_update_bits_locked(codec, reg, val_mask, val);
}
EXPORT_SYMBOL_GPL(snd_soc_put_volsw_s8);

#ifdef CONFIG_SND_SOC_WM5110
/* LGE_CHANGE, yeri.lee@lge.com, 2013-10-31, wm5110 Bring up */
/**
 * snd_soc_info_volsw_range - single mixer info callback with range.
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Callback to provide information, within a range, about a single
 * mixer control.
 *
 * returns 0 for success.
 */
int snd_soc_info_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int platform_max;
	int min = mc->min;

	if (!mc->platform_max)
		mc->platform_max = mc->max;
	platform_max = mc->platform_max;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = platform_max - min;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_volsw_range);

/**
 * snd_soc_put_volsw_range - single mixer put value callback with range.
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to set the value, within a range, for a single mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_put_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int shift = mc->shift;
	int min = mc->min;
	int max = mc->max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;
	unsigned int val, val_mask;

	val = ((ucontrol->value.integer.value[0] + min) & mask);
	if (invert)
		val = max - val;
	val_mask = mask << shift;
	val = val << shift;

	return snd_soc_update_bits_locked(codec, reg, val_mask, val);
}
EXPORT_SYMBOL_GPL(snd_soc_put_volsw_range);

/**
 * snd_soc_get_volsw_range - single mixer get callback with range
 * @kcontrol: mixer control
 * @ucontrol: control element information
 *
 * Callback to get the value, within a range, of a single mixer control.
 *
 * Returns 0 for success.
 */
int snd_soc_get_volsw_range(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int shift = mc->shift;
	int min = mc->min;
	int max = mc->max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;

	ucontrol->value.integer.value[0] =
		(snd_soc_read(codec, reg) >> shift) & mask;
	if (invert)
		ucontrol->value.integer.value[0] =
			max - ucontrol->value.integer.value[0];
	ucontrol->value.integer.value[0] =
		ucontrol->value.integer.value[0] - min;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_volsw_range);
#endif

/**
 * snd_soc_limit_volume - Set new limit to an existing volume control.
 *
 * @codec: where to look for the control
 * @name: Name of the control
 * @max: new maximum limit
 *
 * Return 0 for success, else error.
 */
int snd_soc_limit_volume(struct snd_soc_codec *codec,
	const char *name, int max)
{
	struct snd_card *card = codec->card->snd_card;
	struct snd_kcontrol *kctl;
	struct soc_mixer_control *mc;
	int found = 0;
	int ret = -EINVAL;

	/* Sanity check for name and max */
	if (unlikely(!name || max <= 0))
		return -EINVAL;

	list_for_each_entry(kctl, &card->controls, list) {
		if (!strncmp(kctl->id.name, name, sizeof(kctl->id.name))) {
			found = 1;
			break;
		}
	}
	if (found) {
		mc = (struct soc_mixer_control *)kctl->private_value;
		if (max <= mc->max) {
			mc->platform_max = max;
			ret = 0;
		}
	}
	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_limit_volume);

/**
 * snd_soc_info_volsw_2r_sx - double with tlv and variable data size
 *  mixer info callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Returns 0 for success.
 */
int snd_soc_info_volsw_2r_sx(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int max = mc->max;
	int min = mc->min;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = max-min;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_info_volsw_2r_sx);

/**
 * snd_soc_get_volsw_2r_sx - double with tlv and variable data size
 *  mixer get callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Returns 0 for success.
 */
int snd_soc_get_volsw_2r_sx(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int mask = (1<<mc->shift)-1;
	int min = mc->min;
	int val = snd_soc_read(codec, mc->reg) & mask;
	int valr = snd_soc_read(codec, mc->rreg) & mask;

	ucontrol->value.integer.value[0] = ((val & 0xff)-min) & mask;
	ucontrol->value.integer.value[1] = ((valr & 0xff)-min) & mask;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_get_volsw_2r_sx);

/**
 * snd_soc_put_volsw_2r_sx - double with tlv and variable data size
 *  mixer put callback
 * @kcontrol: mixer control
 * @uinfo: control element information
 *
 * Returns 0 for success.
 */
int snd_soc_put_volsw_2r_sx(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int mask = (1<<mc->shift)-1;
	int min = mc->min;
	int ret;
	unsigned int val, valr, oval, ovalr;

	val = ((ucontrol->value.integer.value[0]+min) & 0xff);
	val &= mask;
	valr = ((ucontrol->value.integer.value[1]+min) & 0xff);
	valr &= mask;

	oval = snd_soc_read(codec, mc->reg) & mask;
	ovalr = snd_soc_read(codec, mc->rreg) & mask;

	ret = 0;
	if (oval != val) {
		ret = snd_soc_write(codec, mc->reg, val);
		if (ret < 0)
			return ret;
	}
	if (ovalr != valr) {
		ret = snd_soc_write(codec, mc->rreg, valr);
		if (ret < 0)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_put_volsw_2r_sx);

#ifdef CONFIG_SND_SOC_WM5110
/* LGE_CHANGE, yeri.lee@lge.com, 2013-10-31, wm5110 Bring up */
int snd_soc_bytes_info(struct snd_kcontrol *kcontrol,
		       struct snd_ctl_elem_info *uinfo)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_bytes *params = (struct soc_bytes *)kcontrol->private_value;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_BYTES;
	uinfo->count =
		(unsigned int)params->num_regs *
		(unsigned int)codec->val_bytes;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_bytes_info);

int snd_soc_bytes_get(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol)
{
	struct soc_bytes *params = (void *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	int ret;

	if (codec->using_regmap)
		ret = regmap_raw_read(codec->control_data, params->base,
				      ucontrol->value.bytes.data,
				      params->num_regs * codec->val_bytes);
	else
		ret = -EINVAL;

	/* Hide any masked bytes to ensure consistent data reporting */
	if (ret == 0 && params->mask) {
		switch (codec->val_bytes) {
		case 1:
			ucontrol->value.bytes.data[0] &= ~params->mask;
			break;
		case 2:
			((u16 *)(&ucontrol->value.bytes.data))[0]
				&= ~params->mask;
			break;
		case 4:
			((u32 *)(&ucontrol->value.bytes.data))[0]
				&= ~params->mask;
			break;
		default:
			return -EINVAL;
		}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_bytes_get);

int snd_soc_bytes_put(struct snd_kcontrol *kcontrol,
		      struct snd_ctl_elem_value *ucontrol)
{
	struct soc_bytes *params = (void *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	int ret, len;
	unsigned int val;
	void *data;

	if (!codec->using_regmap || !params->num_regs)
		return -EINVAL;

	data = ucontrol->value.bytes.data;
	len = params->num_regs * codec->val_bytes;

	/*
	 * If we've got a mask then we need to preserve the register
	 * bits.  We shouldn't modify the incoming data so take a
	 * copy.
	 */
	if (params->mask) {
		ret = regmap_read(codec->control_data, params->base, &val);
		if (ret != 0)
			return ret;

		val &= params->mask;

		data = kmemdup(data, len, GFP_KERNEL);
		if (!data)
			return -ENOMEM;

		switch (codec->val_bytes) {
		case 1:
			((u8 *)data)[0] &= ~params->mask;
			((u8 *)data)[0] |= val;
			break;
		case 2:
			((u16 *)data)[0] &= cpu_to_be16(~params->mask);
			((u16 *)data)[0] |= cpu_to_be16(val);
			break;
		case 4:
			((u32 *)data)[0] &= cpu_to_be32(~params->mask);
			((u32 *)data)[0] |= cpu_to_be32(val);
			break;
		default:
			return -EINVAL;
		}
	}

	ret = regmap_raw_write(codec->control_data, params->base,
			       data, len);

	if (params->mask)
		kfree(data);

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_bytes_put);
#endif

/**
 * snd_soc_dai_set_sysclk - configure DAI system or master clock.
 * @dai: DAI
 * @clk_id: DAI specific clock ID
 * @freq: new clock frequency in Hz
 * @dir: new clock direction - input/output.
 *
 * Configures the DAI master (MCLK) or system (SYSCLK) clocking.
 */
int snd_soc_dai_set_sysclk(struct snd_soc_dai *dai, int clk_id,
	unsigned int freq, int dir)
{
	if (dai->driver && dai->driver->ops->set_sysclk)
		return dai->driver->ops->set_sysclk(dai, clk_id, freq, dir);
	else if (dai->codec && dai->codec->driver->set_sysclk)
		return dai->codec->driver->set_sysclk(dai->codec, clk_id, 0,
						      freq, dir);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_sysclk);

/**
 * snd_soc_codec_set_sysclk - configure CODEC system or master clock.
 * @codec: CODEC
 * @clk_id: DAI specific clock ID
 * @source: Source for the clock
 * @freq: new clock frequency in Hz
 * @dir: new clock direction - input/output.
 *
 * Configures the CODEC master (MCLK) or system (SYSCLK) clocking.
 */
int snd_soc_codec_set_sysclk(struct snd_soc_codec *codec, int clk_id,
			     int source, unsigned int freq, int dir)
{
	if (codec->driver->set_sysclk)
		return codec->driver->set_sysclk(codec, clk_id, source,
						 freq, dir);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_set_sysclk);

/**
 * snd_soc_dai_set_clkdiv - configure DAI clock dividers.
 * @dai: DAI
 * @div_id: DAI specific clock divider ID
 * @div: new clock divisor.
 *
 * Configures the clock dividers. This is used to derive the best DAI bit and
 * frame clocks from the system or master clock. It's best to set the DAI bit
 * and frame clocks as low as possible to save system power.
 */
int snd_soc_dai_set_clkdiv(struct snd_soc_dai *dai,
	int div_id, int div)
{
	if (dai->driver && dai->driver->ops->set_clkdiv)
		return dai->driver->ops->set_clkdiv(dai, div_id, div);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_clkdiv);

/**
 * snd_soc_dai_set_pll - configure DAI PLL.
 * @dai: DAI
 * @pll_id: DAI specific PLL ID
 * @source: DAI specific source for the PLL
 * @freq_in: PLL input clock frequency in Hz
 * @freq_out: requested PLL output clock frequency in Hz
 *
 * Configures and enables PLL to generate output clock based on input clock.
 */
int snd_soc_dai_set_pll(struct snd_soc_dai *dai, int pll_id, int source,
	unsigned int freq_in, unsigned int freq_out)
{
	if (dai->driver && dai->driver->ops->set_pll)
		return dai->driver->ops->set_pll(dai, pll_id, source,
					 freq_in, freq_out);
	else if (dai->codec && dai->codec->driver->set_pll)
		return dai->codec->driver->set_pll(dai->codec, pll_id, source,
						   freq_in, freq_out);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_pll);

/*
 * snd_soc_codec_set_pll - configure codec PLL.
 * @codec: CODEC
 * @pll_id: DAI specific PLL ID
 * @source: DAI specific source for the PLL
 * @freq_in: PLL input clock frequency in Hz
 * @freq_out: requested PLL output clock frequency in Hz
 *
 * Configures and enables PLL to generate output clock based on input clock.
 */
int snd_soc_codec_set_pll(struct snd_soc_codec *codec, int pll_id, int source,
			  unsigned int freq_in, unsigned int freq_out)
{
	if (codec->driver->set_pll)
		return codec->driver->set_pll(codec, pll_id, source,
					      freq_in, freq_out);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_set_pll);

/**
 * snd_soc_dai_set_fmt - configure DAI hardware audio format.
 * @dai: DAI
 * @fmt: SND_SOC_DAIFMT_ format value.
 *
 * Configures the DAI hardware format and clocking.
 */
int snd_soc_dai_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	if (dai->driver && dai->driver->ops->set_fmt)
		return dai->driver->ops->set_fmt(dai, fmt);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_fmt);

/**
 * snd_soc_dai_set_tdm_slot - configure DAI TDM.
 * @dai: DAI
 * @tx_mask: bitmask representing active TX slots.
 * @rx_mask: bitmask representing active RX slots.
 * @slots: Number of slots in use.
 * @slot_width: Width in bits for each slot.
 *
 * Configures a DAI for TDM operation. Both mask and slots are codec and DAI
 * specific.
 */
int snd_soc_dai_set_tdm_slot(struct snd_soc_dai *dai,
	unsigned int tx_mask, unsigned int rx_mask, int slots, int slot_width)
{
	if (dai->driver && dai->driver->ops->set_tdm_slot)
		return dai->driver->ops->set_tdm_slot(dai, tx_mask, rx_mask,
				slots, slot_width);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_tdm_slot);

/**
 * snd_soc_dai_set_channel_map - configure DAI audio channel map
 * @dai: DAI
 * @tx_num: how many TX channels
 * @tx_slot: pointer to an array which imply the TX slot number channel
 *           0~num-1 uses
 * @rx_num: how many RX channels
 * @rx_slot: pointer to an array which imply the RX slot number channel
 *           0~num-1 uses
 *
 * configure the relationship between channel number and TDM slot number.
 */
int snd_soc_dai_set_channel_map(struct snd_soc_dai *dai,
	unsigned int tx_num, unsigned int *tx_slot,
	unsigned int rx_num, unsigned int *rx_slot)
{
	if (dai->driver && dai->driver->ops->set_channel_map)
		return dai->driver->ops->set_channel_map(dai, tx_num, tx_slot,
			rx_num, rx_slot);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_channel_map);

/**
 * snd_soc_dai_get_channel_map - configure DAI audio channel map
 * @dai: DAI
 * @tx_num: how many TX channels
 * @tx_slot: pointer to an array which imply the TX slot number channel
 *           0~num-1 uses
 * @rx_num: how many RX channels
 * @rx_slot: pointer to an array which imply the RX slot number channel
 *           0~num-1 uses
 *
 * configure the relationship between channel number and TDM slot number.
 */
int snd_soc_dai_get_channel_map(struct snd_soc_dai *dai,
	unsigned int *tx_num, unsigned int *tx_slot,
	unsigned int *rx_num, unsigned int *rx_slot)
{
	if (dai->driver && dai->driver->ops->get_channel_map)
		return dai->driver->ops->get_channel_map(dai, tx_num, tx_slot,
			rx_num, rx_slot);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_get_channel_map);
/**
 * snd_soc_dai_set_tristate - configure DAI system or master clock.
 * @dai: DAI
 * @tristate: tristate enable
 *
 * Tristates the DAI so that others can use it.
 */
int snd_soc_dai_set_tristate(struct snd_soc_dai *dai, int tristate)
{
	if (dai->driver && dai->driver->ops->set_tristate)
		return dai->driver->ops->set_tristate(dai, tristate);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_set_tristate);

/**
 * snd_soc_dai_digital_mute - configure DAI system or master clock.
 * @dai: DAI
 * @mute: mute enable
 *
 * Mutes the DAI DAC.
 */
int snd_soc_dai_digital_mute(struct snd_soc_dai *dai, int mute)
{
	if (dai->driver && dai->driver->ops->digital_mute)
		return dai->driver->ops->digital_mute(dai, mute);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(snd_soc_dai_digital_mute);

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * snd_soc_register_card - Register a card with the ASoC core
 *
 * @card: Card to register
 *
 */
int snd_soc_register_card(struct snd_soc_card *card)
{
<<<<<<< HEAD
	int i;
	int ret = 0;

	if (!card->name || !card->dev)
		return -EINVAL;

	for (i = 0; i < card->num_links; i++) {
		struct snd_soc_dai_link *link = &card->dai_link[i];

		/*
		 * Codec must be specified by 1 of name or OF node,
		 * not both or neither.
		 */
		if (!!link->codec_name == !!link->codec_of_node) {
			dev_err(card->dev,
				"Neither/both codec name/of_node are set for %s\n",
				link->name);
			return -EINVAL;
		}

		/*
		 * Platform may be specified by either name or OF node, but
		 * can be left unspecified, and a dummy platform will be used.
		 */
		if (link->platform_name && link->platform_of_node) {
			dev_err(card->dev,
				"Both platform name/of_node are set for %s\n", link->name);
			return -EINVAL;
		}

		/*
		 * CPU DAI must be specified by 1 of name or OF node,
		 * not both or neither.
		 */
		if (!!link->cpu_dai_name == !!link->cpu_dai_of_node) {
			dev_err(card->dev,
				"Neither/both cpu_dai name/of_node are set for %s\n",
				link->name);
			return -EINVAL;
		}
	}

	dev_set_drvdata(card->dev, card);

	snd_soc_initialize_card_lists(card);

	soc_init_card_debugfs(card);

	card->rtd = kzalloc(sizeof(struct snd_soc_pcm_runtime) *
			    (card->num_links + card->num_aux_devs),
			    GFP_KERNEL);
	if (card->rtd == NULL)
		return -ENOMEM;
	card->num_rtd = 0;
	card->rtd_aux = &card->rtd[card->num_links];

	for (i = 0; i < card->num_links; i++)
		card->rtd[i].dai_link = &card->dai_link[i];

	INIT_LIST_HEAD(&card->list);
	INIT_LIST_HEAD(&card->dapm_dirty);
	card->instantiated = 0;
	mutex_init(&card->mutex);
	mutex_init(&card->dpcm_mutex);
	mutex_init(&card->dapm_power_mutex);
	mutex_init(&card->dapm_mutex);
	ret = snd_soc_instantiate_card(card);
	if (ret != 0) {
		soc_cleanup_card_debugfs(card);
		if (card->rtd)
			kfree(card->rtd);
	}


	return ret;
=======
	if (!card->name || !card->dev)
		return -EINVAL;

	dev_set_drvdata(card->dev, card);

	INIT_LIST_HEAD(&card->widgets);
	INIT_LIST_HEAD(&card->paths);
	INIT_LIST_HEAD(&card->dapm_list);
	INIT_LIST_HEAD(&card->aux_comp_list);
	INIT_LIST_HEAD(&card->component_dev_list);
	INIT_LIST_HEAD(&card->list);
	INIT_LIST_HEAD(&card->rtd_list);
	INIT_LIST_HEAD(&card->dapm_dirty);
	INIT_LIST_HEAD(&card->dobj_list);

	card->instantiated = 0;
	mutex_init(&card->mutex);
	mutex_init(&card->dapm_mutex);
	mutex_init(&card->pcm_mutex);

	return snd_soc_bind_card(card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(snd_soc_register_card);

/**
 * snd_soc_unregister_card - Unregister a card with the ASoC core
 *
 * @card: Card to unregister
 *
 */
<<<<<<< HEAD
int snd_soc_unregister_card(struct snd_soc_card *card)
{
	if (card->instantiated)
		soc_cleanup_card_resources(card);
	dev_dbg(card->dev, "Unregistered card '%s'\n", card->name);

	return 0;
=======
void snd_soc_unregister_card(struct snd_soc_card *card)
{
	mutex_lock(&client_mutex);
	snd_soc_unbind_card(card, true);
	mutex_unlock(&client_mutex);
	dev_dbg(card->dev, "ASoC: Unregistered card '%s'\n", card->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_card);

/*
 * Simplify DAI link configuration by removing ".-1" from device names
 * and sanitizing names.
 */
static char *fmt_single_name(struct device *dev, int *id)
{
<<<<<<< HEAD
	char *found, name[NAME_SIZE];
	int id1, id2;

	if (dev_name(dev) == NULL)
		return NULL;

	strlcpy(name, dev_name(dev), NAME_SIZE);
=======
	const char *devname = dev_name(dev);
	char *found, *name;
	unsigned int id1, id2;

	if (devname == NULL)
		return NULL;

	name = devm_kstrdup(dev, devname, GFP_KERNEL);
	if (!name)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* are we a "%s.%d" name (platform and SPI components) */
	found = strstr(name, dev->driver->name);
	if (found) {
		/* get ID */
		if (sscanf(&found[strlen(dev->driver->name)], ".%d", id) == 1) {

			/* discard ID from name if ID == -1 */
			if (*id == -1)
				found[strlen(dev->driver->name)] = '\0';
		}

<<<<<<< HEAD
	} else {
		/* I2C component devices are named "bus-addr"  */
		if (sscanf(name, "%x-%x", &id1, &id2) == 2) {
			char tmp[NAME_SIZE];

			/* create unique ID number from I2C addr and bus */
			*id = ((id1 & 0xffff) << 16) + id2;

			/* sanitize component name for DAI link creation */
			snprintf(tmp, NAME_SIZE, "%s.%s", dev->driver->name, name);
			strlcpy(name, tmp, NAME_SIZE);
		} else
			*id = 0;
	}

	return kstrdup(name, GFP_KERNEL);
=======
	/* I2C component devices are named "bus-addr" */
	} else if (sscanf(name, "%x-%x", &id1, &id2) == 2) {

		/* create unique ID number from I2C addr and bus */
		*id = ((id1 & 0xffff) << 16) + id2;

		devm_kfree(dev, name);

		/* sanitize component name for DAI link creation */
		name = devm_kasprintf(dev, GFP_KERNEL, "%s.%s", dev->driver->name, devname);
	} else {
		*id = 0;
	}

	return name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Simplify DAI link naming for single devices with multiple DAIs by removing
 * any ".-1" and using the DAI name (instead of device name).
 */
static inline char *fmt_multiple_name(struct device *dev,
		struct snd_soc_dai_driver *dai_drv)
{
	if (dai_drv->name == NULL) {
<<<<<<< HEAD
		printk(KERN_ERR "asoc: error - multiple DAI %s registered with no name\n",
				dev_name(dev));
		return NULL;
	}

	return kstrdup(dai_drv->name, GFP_KERNEL);
}

/**
 * snd_soc_register_dai - Register a DAI with the ASoC core
 *
 * @dai: DAI to register
 */
int snd_soc_register_dai(struct device *dev,
		struct snd_soc_dai_driver *dai_drv)
{
	struct snd_soc_dai *dai;

	dev_dbg(dev, "dai register %s\n", dev_name(dev));

	dai = kzalloc(sizeof(struct snd_soc_dai), GFP_KERNEL);
	if (dai == NULL)
		return -ENOMEM;

	/* create DAI component name */
	dai->name = fmt_single_name(dev, &dai->id);
	if (dai->name == NULL) {
		kfree(dai);
		return -ENOMEM;
	}

	dai->dev = dev;
	dai->driver = dai_drv;
	if (!dai->driver->ops)
		dai->driver->ops = &null_dai_ops;

	mutex_lock(&client_mutex);
	list_add(&dai->list, &dai_list);
	mutex_unlock(&client_mutex);

	pr_debug("Registered DAI '%s'\n", dai->name);

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_register_dai);

/**
 * snd_soc_unregister_dai - Unregister a DAI from the ASoC core
 *
 * @dai: DAI to unregister
 */
void snd_soc_unregister_dai(struct device *dev)
{
	struct snd_soc_dai *dai;

	list_for_each_entry(dai, &dai_list, list) {
		if (dev == dai->dev)
			goto found;
	}
	return;

found:
	mutex_lock(&client_mutex);
	list_del(&dai->list);
	mutex_unlock(&client_mutex);

	pr_debug("Unregistered DAI '%s'\n", dai->name);
	kfree(dai->name);
	kfree(dai);
=======
		dev_err(dev,
			"ASoC: error - multiple DAI %s registered with no name\n",
			dev_name(dev));
		return NULL;
	}

	return devm_kstrdup(dev, dai_drv->name, GFP_KERNEL);
}

void snd_soc_unregister_dai(struct snd_soc_dai *dai)
{
	dev_dbg(dai->dev, "ASoC: Unregistered DAI '%s'\n", dai->name);
	list_del(&dai->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_dai);

/**
<<<<<<< HEAD
 * snd_soc_register_dais - Register multiple DAIs with the ASoC core
 *
 * @dai: Array of DAIs to register
 * @count: Number of DAIs
 */
int snd_soc_register_dais(struct device *dev,
		struct snd_soc_dai_driver *dai_drv, size_t count)
{
	struct snd_soc_dai *dai;
	int i, ret = 0;

	dev_dbg(dev, "dai register %s #%Zu\n", dev_name(dev), count);

	for (i = 0; i < count; i++) {

		dai = kzalloc(sizeof(struct snd_soc_dai), GFP_KERNEL);
=======
 * snd_soc_register_dai - Register a DAI dynamically & create its widgets
 *
 * @component: The component the DAIs are registered for
 * @dai_drv: DAI driver to use for the DAI
 * @legacy_dai_naming: if %true, use legacy single-name format;
 * 	if %false, use multiple-name format;
 *
 * Topology can use this API to register DAIs when probing a component.
 * These DAIs's widgets will be freed in the card cleanup and the DAIs
 * will be freed in the component cleanup.
 */
struct snd_soc_dai *snd_soc_register_dai(struct snd_soc_component *component,
					 struct snd_soc_dai_driver *dai_drv,
					 bool legacy_dai_naming)
{
	struct device *dev = component->dev;
	struct snd_soc_dai *dai;

	lockdep_assert_held(&client_mutex);

	dai = devm_kzalloc(dev, sizeof(*dai), GFP_KERNEL);
	if (dai == NULL)
		return NULL;

	/*
	 * Back in the old days when we still had component-less DAIs,
	 * instead of having a static name, component-less DAIs would
	 * inherit the name of the parent device so it is possible to
	 * register multiple instances of the DAI. We still need to keep
	 * the same naming style even though those DAIs are not
	 * component-less anymore.
	 */
	if (legacy_dai_naming &&
	    (dai_drv->id == 0 || dai_drv->name == NULL)) {
		dai->name = fmt_single_name(dev, &dai->id);
	} else {
		dai->name = fmt_multiple_name(dev, dai_drv);
		if (dai_drv->id)
			dai->id = dai_drv->id;
		else
			dai->id = component->num_dai;
	}
	if (!dai->name)
		return NULL;

	dai->component = component;
	dai->dev = dev;
	dai->driver = dai_drv;

	/* see for_each_component_dais */
	list_add_tail(&dai->list, &component->dai_list);
	component->num_dai++;

	dev_dbg(dev, "ASoC: Registered DAI '%s'\n", dai->name);
	return dai;
}
EXPORT_SYMBOL_GPL(snd_soc_register_dai);

/**
 * snd_soc_unregister_dais - Unregister DAIs from the ASoC core
 *
 * @component: The component for which the DAIs should be unregistered
 */
static void snd_soc_unregister_dais(struct snd_soc_component *component)
{
	struct snd_soc_dai *dai, *_dai;

	for_each_component_dais_safe(component, dai, _dai)
		snd_soc_unregister_dai(dai);
}

/**
 * snd_soc_register_dais - Register a DAI with the ASoC core
 *
 * @component: The component the DAIs are registered for
 * @dai_drv: DAI driver to use for the DAIs
 * @count: Number of DAIs
 */
static int snd_soc_register_dais(struct snd_soc_component *component,
				 struct snd_soc_dai_driver *dai_drv,
				 size_t count)
{
	struct snd_soc_dai *dai;
	unsigned int i;
	int ret;

	for (i = 0; i < count; i++) {
		dai = snd_soc_register_dai(component, dai_drv + i, count == 1 &&
					   component->driver->legacy_dai_naming);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (dai == NULL) {
			ret = -ENOMEM;
			goto err;
		}
<<<<<<< HEAD

		/* create DAI component name */
		dai->name = fmt_multiple_name(dev, &dai_drv[i]);
		if (dai->name == NULL) {
			kfree(dai);
			ret = -EINVAL;
			goto err;
		}

		dai->dev = dev;
		dai->driver = &dai_drv[i];

		if (dai->driver->id)
			dai->id = dai->driver->id;
		else
			dai->id = i;
		if (!dai->driver->ops)
			dai->driver->ops = &null_dai_ops;

		mutex_lock(&client_mutex);
		list_add(&dai->list, &dai_list);
		mutex_unlock(&client_mutex);

		pr_debug("Registered DAI '%s'\n", dai->name);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

err:
<<<<<<< HEAD
	for (i--; i >= 0; i--)
		snd_soc_unregister_dai(dev);

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_register_dais);

/**
 * snd_soc_unregister_dais - Unregister multiple DAIs from the ASoC core
 *
 * @dai: Array of DAIs to unregister
 * @count: Number of DAIs
 */
void snd_soc_unregister_dais(struct device *dev, size_t count)
{
	int i;

	for (i = 0; i < count; i++)
		snd_soc_unregister_dai(dev);
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_dais);

/**
 * snd_soc_register_platform - Register a platform with the ASoC core
 *
 * @platform: platform to register
 */
int snd_soc_register_platform(struct device *dev,
		struct snd_soc_platform_driver *platform_drv)
{
	struct snd_soc_platform *platform;

	dev_dbg(dev, "platform register %s\n", dev_name(dev));

	platform = kzalloc(sizeof(struct snd_soc_platform), GFP_KERNEL);
	if (platform == NULL)
		return -ENOMEM;

	/* create platform component name */
	platform->name = fmt_single_name(dev, &platform->id);
	if (platform->name == NULL) {
		kfree(platform);
		return -ENOMEM;
	}

	platform->dev = dev;
	platform->driver = platform_drv;
	platform->dapm.dev = dev;
	platform->dapm.platform = platform;
	platform->dapm.stream_event = platform_drv->stream_event;

	mutex_lock(&client_mutex);
	list_add(&platform->list, &platform_list);
	mutex_unlock(&client_mutex);

	pr_debug("Registered platform '%s'\n", platform->name);

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_register_platform);

/**
 * snd_soc_unregister_platform - Unregister a platform from the ASoC core
 *
 * @platform: platform to unregister
 */
void snd_soc_unregister_platform(struct device *dev)
{
	struct snd_soc_platform *platform;

	list_for_each_entry(platform, &platform_list, list) {
		if (dev == platform->dev)
			goto found;
	}
	return;

found:
	mutex_lock(&client_mutex);
	list_del(&platform->list);
	mutex_unlock(&client_mutex);

	pr_debug("Unregistered platform '%s'\n", platform->name);
	kfree(platform->name);
	kfree(platform);
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_platform);

static u64 codec_format_map[] = {
	SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S16_BE,
	SNDRV_PCM_FMTBIT_U16_LE | SNDRV_PCM_FMTBIT_U16_BE,
	SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S24_BE,
	SNDRV_PCM_FMTBIT_U24_LE | SNDRV_PCM_FMTBIT_U24_BE,
	SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S32_BE,
	SNDRV_PCM_FMTBIT_U32_LE | SNDRV_PCM_FMTBIT_U32_BE,
	SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_U24_3BE,
	SNDRV_PCM_FMTBIT_U24_3LE | SNDRV_PCM_FMTBIT_U24_3BE,
	SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S20_3BE,
	SNDRV_PCM_FMTBIT_U20_3LE | SNDRV_PCM_FMTBIT_U20_3BE,
	SNDRV_PCM_FMTBIT_S18_3LE | SNDRV_PCM_FMTBIT_S18_3BE,
	SNDRV_PCM_FMTBIT_U18_3LE | SNDRV_PCM_FMTBIT_U18_3BE,
	SNDRV_PCM_FMTBIT_FLOAT_LE | SNDRV_PCM_FMTBIT_FLOAT_BE,
	SNDRV_PCM_FMTBIT_FLOAT64_LE | SNDRV_PCM_FMTBIT_FLOAT64_BE,
	SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_LE
	| SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_BE,
};

/* Fix up the DAI formats for endianness: codecs don't actually see
=======
	snd_soc_unregister_dais(component);

	return ret;
}

#define ENDIANNESS_MAP(name) \
	(SNDRV_PCM_FMTBIT_##name##LE | SNDRV_PCM_FMTBIT_##name##BE)
static u64 endianness_format_map[] = {
	ENDIANNESS_MAP(S16_),
	ENDIANNESS_MAP(U16_),
	ENDIANNESS_MAP(S24_),
	ENDIANNESS_MAP(U24_),
	ENDIANNESS_MAP(S32_),
	ENDIANNESS_MAP(U32_),
	ENDIANNESS_MAP(S24_3),
	ENDIANNESS_MAP(U24_3),
	ENDIANNESS_MAP(S20_3),
	ENDIANNESS_MAP(U20_3),
	ENDIANNESS_MAP(S18_3),
	ENDIANNESS_MAP(U18_3),
	ENDIANNESS_MAP(FLOAT_),
	ENDIANNESS_MAP(FLOAT64_),
	ENDIANNESS_MAP(IEC958_SUBFRAME_),
};

/*
 * Fix up the DAI formats for endianness: codecs don't actually see
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the endianness of the data but we're using the CPU format
 * definitions which do need to include endianness so we ensure that
 * codec DAIs always have both big and little endian variants set.
 */
<<<<<<< HEAD
static void fixup_codec_formats(struct snd_soc_pcm_stream *stream)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(codec_format_map); i++)
		if (stream->formats & codec_format_map[i])
			stream->formats |= codec_format_map[i];
}

/**
 * snd_soc_card_change_online_state - Mark if soc card is online/offline
 *
 * @soc_card : soc_card to mark
 */
void snd_soc_card_change_online_state(struct snd_soc_card *soc_card, int online)
{
	snd_card_change_online_state(soc_card->snd_card, online);
}
EXPORT_SYMBOL(snd_soc_card_change_online_state);

/**
 * snd_soc_register_codec - Register a codec with the ASoC core
 *
 * @codec: codec to register
 */
int snd_soc_register_codec(struct device *dev,
			   const struct snd_soc_codec_driver *codec_drv,
			   struct snd_soc_dai_driver *dai_drv,
			   int num_dai)
{
	size_t reg_size;
	struct snd_soc_codec *codec;
	int ret, i;

	dev_dbg(dev, "codec register %s\n", dev_name(dev));

	codec = kzalloc(sizeof(struct snd_soc_codec), GFP_KERNEL);
	if (codec == NULL)
		return -ENOMEM;

	/* create CODEC component name */
	codec->name = fmt_single_name(dev, &codec->id);
	if (codec->name == NULL) {
		kfree(codec);
		return -ENOMEM;
	}

	if (codec_drv->compress_type)
		codec->compress_type = codec_drv->compress_type;
	else
		codec->compress_type = SND_SOC_FLAT_COMPRESSION;

	codec->write = codec_drv->write;
	codec->read = codec_drv->read;
	codec->volatile_register = codec_drv->volatile_register;
	codec->readable_register = codec_drv->readable_register;
	codec->writable_register = codec_drv->writable_register;
	codec->dapm.bias_level = SND_SOC_BIAS_OFF;
	codec->dapm.dev = dev;
	codec->dapm.codec = codec;
	codec->dapm.seq_notifier = codec_drv->seq_notifier;
	codec->dapm.stream_event = codec_drv->stream_event;
	codec->dev = dev;
	codec->driver = codec_drv;
	codec->num_dai = num_dai;
	mutex_init(&codec->mutex);

	/* allocate CODEC register cache */
	if (codec_drv->reg_cache_size && codec_drv->reg_word_size) {
		reg_size = codec_drv->reg_cache_size * codec_drv->reg_word_size;
		codec->reg_size = reg_size;
		/* it is necessary to make a copy of the default register cache
		 * because in the case of using a compression type that requires
		 * the default register cache to be marked as __devinitconst the
		 * kernel might have freed the array by the time we initialize
		 * the cache.
		 */
		if (codec_drv->reg_cache_default) {
			codec->reg_def_copy = kmemdup(codec_drv->reg_cache_default,
						      reg_size, GFP_KERNEL);
			if (!codec->reg_def_copy) {
				ret = -ENOMEM;
				goto fail;
			}
		}
	}

	if (codec_drv->reg_access_size && codec_drv->reg_access_default) {
		if (!codec->volatile_register)
			codec->volatile_register = snd_soc_default_volatile_register;
		if (!codec->readable_register)
			codec->readable_register = snd_soc_default_readable_register;
		if (!codec->writable_register)
			codec->writable_register = snd_soc_default_writable_register;
	}

	for (i = 0; i < num_dai; i++) {
		fixup_codec_formats(&dai_drv[i].playback);
		fixup_codec_formats(&dai_drv[i].capture);
	}

	/* register any DAIs */
	if (num_dai) {
		ret = snd_soc_register_dais(dev, dai_drv, num_dai);
		if (ret < 0)
			goto fail;
	}

	mutex_lock(&client_mutex);
	list_add(&codec->list, &codec_list);
	mutex_unlock(&client_mutex);

	pr_debug("Registered codec '%s'\n", codec->name);
	return 0;

fail:
	kfree(codec->reg_def_copy);
	codec->reg_def_copy = NULL;
	kfree(codec->name);
	kfree(codec);
	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_register_codec);

/**
 * snd_soc_unregister_codec - Unregister a codec from the ASoC core
 *
 * @codec: codec to unregister
 */
void snd_soc_unregister_codec(struct device *dev)
{
	struct snd_soc_codec *codec;
	int i;

	list_for_each_entry(codec, &codec_list, list) {
		if (dev == codec->dev)
			goto found;
	}
	return;

found:
	if (codec->num_dai)
		for (i = 0; i < codec->num_dai; i++)
			snd_soc_unregister_dai(dev);

	mutex_lock(&client_mutex);
	list_del(&codec->list);
	mutex_unlock(&client_mutex);

	pr_debug("Unregistered codec '%s'\n", codec->name);

	snd_soc_cache_exit(codec);
	kfree(codec->reg_def_copy);
	kfree(codec->name);
	kfree(codec);
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_codec);
=======
static void convert_endianness_formats(struct snd_soc_pcm_stream *stream)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(endianness_format_map); i++)
		if (stream->formats & endianness_format_map[i])
			stream->formats |= endianness_format_map[i];
}

static void snd_soc_try_rebind_card(void)
{
	struct snd_soc_card *card, *c;

	list_for_each_entry_safe(card, c, &unbind_card_list, list)
		if (!snd_soc_bind_card(card))
			list_del(&card->list);
}

static void snd_soc_del_component_unlocked(struct snd_soc_component *component)
{
	struct snd_soc_card *card = component->card;

	snd_soc_unregister_dais(component);

	if (card)
		snd_soc_unbind_card(card, false);

	list_del(&component->list);
}

int snd_soc_component_initialize(struct snd_soc_component *component,
				 const struct snd_soc_component_driver *driver,
				 struct device *dev)
{
	INIT_LIST_HEAD(&component->dai_list);
	INIT_LIST_HEAD(&component->dobj_list);
	INIT_LIST_HEAD(&component->card_list);
	INIT_LIST_HEAD(&component->list);
	mutex_init(&component->io_mutex);

	component->name = fmt_single_name(dev, &component->id);
	if (!component->name) {
		dev_err(dev, "ASoC: Failed to allocate name\n");
		return -ENOMEM;
	}

	component->dev		= dev;
	component->driver	= driver;

#ifdef CONFIG_DEBUG_FS
	if (!component->debugfs_prefix)
		component->debugfs_prefix = driver->debugfs_prefix;
#endif

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_component_initialize);

int snd_soc_add_component(struct snd_soc_component *component,
			  struct snd_soc_dai_driver *dai_drv,
			  int num_dai)
{
	int ret;
	int i;

	mutex_lock(&client_mutex);

	if (component->driver->endianness) {
		for (i = 0; i < num_dai; i++) {
			convert_endianness_formats(&dai_drv[i].playback);
			convert_endianness_formats(&dai_drv[i].capture);
		}
	}

	ret = snd_soc_register_dais(component, dai_drv, num_dai);
	if (ret < 0) {
		dev_err(component->dev, "ASoC: Failed to register DAIs: %d\n",
			ret);
		goto err_cleanup;
	}

	if (!component->driver->write && !component->driver->read) {
		if (!component->regmap)
			component->regmap = dev_get_regmap(component->dev,
							   NULL);
		if (component->regmap)
			snd_soc_component_setup_regmap(component);
	}

	/* see for_each_component */
	list_add(&component->list, &component_list);

err_cleanup:
	if (ret < 0)
		snd_soc_del_component_unlocked(component);

	mutex_unlock(&client_mutex);

	if (ret == 0)
		snd_soc_try_rebind_card();

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_add_component);

int snd_soc_register_component(struct device *dev,
			const struct snd_soc_component_driver *component_driver,
			struct snd_soc_dai_driver *dai_drv,
			int num_dai)
{
	struct snd_soc_component *component;
	int ret;

	component = devm_kzalloc(dev, sizeof(*component), GFP_KERNEL);
	if (!component)
		return -ENOMEM;

	ret = snd_soc_component_initialize(component, component_driver, dev);
	if (ret < 0)
		return ret;

	return snd_soc_add_component(component, dai_drv, num_dai);
}
EXPORT_SYMBOL_GPL(snd_soc_register_component);

/**
 * snd_soc_unregister_component_by_driver - Unregister component using a given driver
 * from the ASoC core
 *
 * @dev: The device to unregister
 * @component_driver: The component driver to unregister
 */
void snd_soc_unregister_component_by_driver(struct device *dev,
					    const struct snd_soc_component_driver *component_driver)
{
	struct snd_soc_component *component;

	if (!component_driver)
		return;

	mutex_lock(&client_mutex);
	component = snd_soc_lookup_component_nolocked(dev, component_driver->name);
	if (!component)
		goto out;

	snd_soc_del_component_unlocked(component);

out:
	mutex_unlock(&client_mutex);
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_component_by_driver);

/**
 * snd_soc_unregister_component - Unregister all related component
 * from the ASoC core
 *
 * @dev: The device to unregister
 */
void snd_soc_unregister_component(struct device *dev)
{
	mutex_lock(&client_mutex);
	while (1) {
		struct snd_soc_component *component = snd_soc_lookup_component_nolocked(dev, NULL);

		if (!component)
			break;

		snd_soc_del_component_unlocked(component);
	}
	mutex_unlock(&client_mutex);
}
EXPORT_SYMBOL_GPL(snd_soc_unregister_component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Retrieve a card's name from device tree */
int snd_soc_of_parse_card_name(struct snd_soc_card *card,
			       const char *propname)
{
<<<<<<< HEAD
	struct device_node *np = card->dev->of_node;
	int ret;

=======
	struct device_node *np;
	int ret;

	if (!card->dev) {
		pr_err("card->dev is not set before calling %s\n", __func__);
		return -EINVAL;
	}

	np = card->dev->of_node;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = of_property_read_string_index(np, propname, 0, &card->name);
	/*
	 * EINVAL means the property does not exist. This is fine providing
	 * card->name was previously set, which is checked later in
	 * snd_soc_register_card.
	 */
	if (ret < 0 && ret != -EINVAL) {
		dev_err(card->dev,
<<<<<<< HEAD
			"Property '%s' could not be read: %d\n",
=======
			"ASoC: Property '%s' could not be read: %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			propname, ret);
		return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_card_name);

<<<<<<< HEAD
=======
static const struct snd_soc_dapm_widget simple_widgets[] = {
	SND_SOC_DAPM_MIC("Microphone", NULL),
	SND_SOC_DAPM_LINE("Line", NULL),
	SND_SOC_DAPM_HP("Headphone", NULL),
	SND_SOC_DAPM_SPK("Speaker", NULL),
};

int snd_soc_of_parse_audio_simple_widgets(struct snd_soc_card *card,
					  const char *propname)
{
	struct device_node *np = card->dev->of_node;
	struct snd_soc_dapm_widget *widgets;
	const char *template, *wname;
	int i, j, num_widgets;

	num_widgets = of_property_count_strings(np, propname);
	if (num_widgets < 0) {
		dev_err(card->dev,
			"ASoC: Property '%s' does not exist\n",	propname);
		return -EINVAL;
	}
	if (!num_widgets) {
		dev_err(card->dev, "ASoC: Property '%s's length is zero\n",
			propname);
		return -EINVAL;
	}
	if (num_widgets & 1) {
		dev_err(card->dev,
			"ASoC: Property '%s' length is not even\n", propname);
		return -EINVAL;
	}

	num_widgets /= 2;

	widgets = devm_kcalloc(card->dev, num_widgets, sizeof(*widgets),
			       GFP_KERNEL);
	if (!widgets) {
		dev_err(card->dev,
			"ASoC: Could not allocate memory for widgets\n");
		return -ENOMEM;
	}

	for (i = 0; i < num_widgets; i++) {
		int ret = of_property_read_string_index(np, propname,
							2 * i, &template);
		if (ret) {
			dev_err(card->dev,
				"ASoC: Property '%s' index %d read error:%d\n",
				propname, 2 * i, ret);
			return -EINVAL;
		}

		for (j = 0; j < ARRAY_SIZE(simple_widgets); j++) {
			if (!strncmp(template, simple_widgets[j].name,
				     strlen(simple_widgets[j].name))) {
				widgets[i] = simple_widgets[j];
				break;
			}
		}

		if (j >= ARRAY_SIZE(simple_widgets)) {
			dev_err(card->dev,
				"ASoC: DAPM widget '%s' is not supported\n",
				template);
			return -EINVAL;
		}

		ret = of_property_read_string_index(np, propname,
						    (2 * i) + 1,
						    &wname);
		if (ret) {
			dev_err(card->dev,
				"ASoC: Property '%s' index %d read error:%d\n",
				propname, (2 * i) + 1, ret);
			return -EINVAL;
		}

		widgets[i].name = wname;
	}

	card->of_dapm_widgets = widgets;
	card->num_of_dapm_widgets = num_widgets;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_audio_simple_widgets);

int snd_soc_of_parse_pin_switches(struct snd_soc_card *card, const char *prop)
{
	const unsigned int nb_controls_max = 16;
	const char **strings, *control_name;
	struct snd_kcontrol_new *controls;
	struct device *dev = card->dev;
	unsigned int i, nb_controls;
	int ret;

	if (!of_property_read_bool(dev->of_node, prop))
		return 0;

	strings = devm_kcalloc(dev, nb_controls_max,
			       sizeof(*strings), GFP_KERNEL);
	if (!strings)
		return -ENOMEM;

	ret = of_property_read_string_array(dev->of_node, prop,
					    strings, nb_controls_max);
	if (ret < 0)
		return ret;

	nb_controls = (unsigned int)ret;

	controls = devm_kcalloc(dev, nb_controls,
				sizeof(*controls), GFP_KERNEL);
	if (!controls)
		return -ENOMEM;

	for (i = 0; i < nb_controls; i++) {
		control_name = devm_kasprintf(dev, GFP_KERNEL,
					      "%s Switch", strings[i]);
		if (!control_name)
			return -ENOMEM;

		controls[i].iface = SNDRV_CTL_ELEM_IFACE_MIXER;
		controls[i].name = control_name;
		controls[i].info = snd_soc_dapm_info_pin_switch;
		controls[i].get = snd_soc_dapm_get_pin_switch;
		controls[i].put = snd_soc_dapm_put_pin_switch;
		controls[i].private_value = (unsigned long)strings[i];
	}

	card->controls = controls;
	card->num_controls = nb_controls;

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_pin_switches);

int snd_soc_of_get_slot_mask(struct device_node *np,
			     const char *prop_name,
			     unsigned int *mask)
{
	u32 val;
	const __be32 *of_slot_mask = of_get_property(np, prop_name, &val);
	int i;

	if (!of_slot_mask)
		return 0;
	val /= sizeof(u32);
	for (i = 0; i < val; i++)
		if (be32_to_cpup(&of_slot_mask[i]))
			*mask |= (1 << i);

	return val;
}
EXPORT_SYMBOL_GPL(snd_soc_of_get_slot_mask);

int snd_soc_of_parse_tdm_slot(struct device_node *np,
			      unsigned int *tx_mask,
			      unsigned int *rx_mask,
			      unsigned int *slots,
			      unsigned int *slot_width)
{
	u32 val;
	int ret;

	if (tx_mask)
		snd_soc_of_get_slot_mask(np, "dai-tdm-slot-tx-mask", tx_mask);
	if (rx_mask)
		snd_soc_of_get_slot_mask(np, "dai-tdm-slot-rx-mask", rx_mask);

	if (of_property_read_bool(np, "dai-tdm-slot-num")) {
		ret = of_property_read_u32(np, "dai-tdm-slot-num", &val);
		if (ret)
			return ret;

		if (slots)
			*slots = val;
	}

	if (of_property_read_bool(np, "dai-tdm-slot-width")) {
		ret = of_property_read_u32(np, "dai-tdm-slot-width", &val);
		if (ret)
			return ret;

		if (slot_width)
			*slot_width = val;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_tdm_slot);

void snd_soc_dlc_use_cpu_as_platform(struct snd_soc_dai_link_component *platforms,
				     struct snd_soc_dai_link_component *cpus)
{
	platforms->of_node	= cpus->of_node;
	platforms->dai_args	= cpus->dai_args;
}
EXPORT_SYMBOL_GPL(snd_soc_dlc_use_cpu_as_platform);

void snd_soc_of_parse_node_prefix(struct device_node *np,
				  struct snd_soc_codec_conf *codec_conf,
				  struct device_node *of_node,
				  const char *propname)
{
	const char *str;
	int ret;

	ret = of_property_read_string(np, propname, &str);
	if (ret < 0) {
		/* no prefix is not error */
		return;
	}

	codec_conf->dlc.of_node	= of_node;
	codec_conf->name_prefix	= str;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_node_prefix);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_soc_of_parse_audio_routing(struct snd_soc_card *card,
				   const char *propname)
{
	struct device_node *np = card->dev->of_node;
	int num_routes;
	struct snd_soc_dapm_route *routes;
<<<<<<< HEAD
	int i, ret;
=======
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	num_routes = of_property_count_strings(np, propname);
	if (num_routes < 0 || num_routes & 1) {
		dev_err(card->dev,
<<<<<<< HEAD
		     "Property '%s' does not exist or its length is not even\n",
		     propname);
		return -EINVAL;
	}
	num_routes /= 2;
	if (!num_routes) {
		dev_err(card->dev,
			"Property '%s's length is zero\n",
			propname);
		return -EINVAL;
	}

	routes = devm_kzalloc(card->dev, num_routes * sizeof(*routes),
			      GFP_KERNEL);
	if (!routes) {
		dev_err(card->dev,
			"Could not allocate DAPM route table\n");
		return -EINVAL;
	}

	for (i = 0; i < num_routes; i++) {
		ret = of_property_read_string_index(np, propname,
			2 * i, &routes[i].sink);
		if (ret) {
			dev_err(card->dev,
				"Property '%s' index %d could not be read: %d\n",
=======
			"ASoC: Property '%s' does not exist or its length is not even\n",
			propname);
		return -EINVAL;
	}
	num_routes /= 2;

	routes = devm_kcalloc(card->dev, num_routes, sizeof(*routes),
			      GFP_KERNEL);
	if (!routes) {
		dev_err(card->dev,
			"ASoC: Could not allocate DAPM route table\n");
		return -ENOMEM;
	}

	for (i = 0; i < num_routes; i++) {
		int ret = of_property_read_string_index(np, propname,
							2 * i, &routes[i].sink);
		if (ret) {
			dev_err(card->dev,
				"ASoC: Property '%s' index %d could not be read: %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				propname, 2 * i, ret);
			return -EINVAL;
		}
		ret = of_property_read_string_index(np, propname,
			(2 * i) + 1, &routes[i].source);
		if (ret) {
			dev_err(card->dev,
<<<<<<< HEAD
				"Property '%s' index %d could not be read: %d\n",
=======
				"ASoC: Property '%s' index %d could not be read: %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				propname, (2 * i) + 1, ret);
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	card->num_dapm_routes = num_routes;
	card->dapm_routes = routes;
=======
	card->num_of_dapm_routes = num_routes;
	card->of_dapm_routes = routes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_audio_routing);

<<<<<<< HEAD
static int __init snd_soc_init(void)
{
#ifdef CONFIG_DEBUG_FS
	snd_soc_debugfs_root = debugfs_create_dir("asoc", NULL);
	if (IS_ERR(snd_soc_debugfs_root) || !snd_soc_debugfs_root) {
		printk(KERN_WARNING
		       "ASoC: Failed to create debugfs directory\n");
		snd_soc_debugfs_root = NULL;
	}

	if (!debugfs_create_file("codecs", 0444, snd_soc_debugfs_root, NULL,
				 &codec_list_fops))
		pr_warn("ASoC: Failed to create CODEC list debugfs file\n");

	if (!debugfs_create_file("dais", 0444, snd_soc_debugfs_root, NULL,
				 &dai_list_fops))
		pr_warn("ASoC: Failed to create DAI list debugfs file\n");

	if (!debugfs_create_file("platforms", 0444, snd_soc_debugfs_root, NULL,
				 &platform_list_fops))
		pr_warn("ASoC: Failed to create platform list debugfs file\n");
#endif

	snd_soc_util_init();

	return platform_driver_register(&soc_driver);
=======
int snd_soc_of_parse_aux_devs(struct snd_soc_card *card, const char *propname)
{
	struct device_node *node = card->dev->of_node;
	struct snd_soc_aux_dev *aux;
	int num, i;

	num = of_count_phandle_with_args(node, propname, NULL);
	if (num == -ENOENT) {
		return 0;
	} else if (num < 0) {
		dev_err(card->dev, "ASOC: Property '%s' could not be read: %d\n",
			propname, num);
		return num;
	}

	aux = devm_kcalloc(card->dev, num, sizeof(*aux), GFP_KERNEL);
	if (!aux)
		return -ENOMEM;
	card->aux_dev = aux;
	card->num_aux_devs = num;

	for_each_card_pre_auxs(card, i, aux) {
		aux->dlc.of_node = of_parse_phandle(node, propname, i);
		if (!aux->dlc.of_node)
			return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_of_parse_aux_devs);

unsigned int snd_soc_daifmt_clock_provider_flipped(unsigned int dai_fmt)
{
	unsigned int inv_dai_fmt = dai_fmt & ~SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK;

	switch (dai_fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_CBP_CFP:
		inv_dai_fmt |= SND_SOC_DAIFMT_CBC_CFC;
		break;
	case SND_SOC_DAIFMT_CBP_CFC:
		inv_dai_fmt |= SND_SOC_DAIFMT_CBC_CFP;
		break;
	case SND_SOC_DAIFMT_CBC_CFP:
		inv_dai_fmt |= SND_SOC_DAIFMT_CBP_CFC;
		break;
	case SND_SOC_DAIFMT_CBC_CFC:
		inv_dai_fmt |= SND_SOC_DAIFMT_CBP_CFP;
		break;
	}

	return inv_dai_fmt;
}
EXPORT_SYMBOL_GPL(snd_soc_daifmt_clock_provider_flipped);

unsigned int snd_soc_daifmt_clock_provider_from_bitmap(unsigned int bit_frame)
{
	/*
	 * bit_frame is return value from
	 *	snd_soc_daifmt_parse_clock_provider_raw()
	 */

	/* Codec base */
	switch (bit_frame) {
	case 0x11:
		return SND_SOC_DAIFMT_CBP_CFP;
	case 0x10:
		return SND_SOC_DAIFMT_CBP_CFC;
	case 0x01:
		return SND_SOC_DAIFMT_CBC_CFP;
	default:
		return SND_SOC_DAIFMT_CBC_CFC;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_daifmt_clock_provider_from_bitmap);

unsigned int snd_soc_daifmt_parse_format(struct device_node *np,
					 const char *prefix)
{
	int ret;
	char prop[128];
	unsigned int format = 0;
	int bit, frame;
	const char *str;
	struct {
		char *name;
		unsigned int val;
	} of_fmt_table[] = {
		{ "i2s",	SND_SOC_DAIFMT_I2S },
		{ "right_j",	SND_SOC_DAIFMT_RIGHT_J },
		{ "left_j",	SND_SOC_DAIFMT_LEFT_J },
		{ "dsp_a",	SND_SOC_DAIFMT_DSP_A },
		{ "dsp_b",	SND_SOC_DAIFMT_DSP_B },
		{ "ac97",	SND_SOC_DAIFMT_AC97 },
		{ "pdm",	SND_SOC_DAIFMT_PDM},
		{ "msb",	SND_SOC_DAIFMT_MSB },
		{ "lsb",	SND_SOC_DAIFMT_LSB },
	};

	if (!prefix)
		prefix = "";

	/*
	 * check "dai-format = xxx"
	 * or    "[prefix]format = xxx"
	 * SND_SOC_DAIFMT_FORMAT_MASK area
	 */
	ret = of_property_read_string(np, "dai-format", &str);
	if (ret < 0) {
		snprintf(prop, sizeof(prop), "%sformat", prefix);
		ret = of_property_read_string(np, prop, &str);
	}
	if (ret == 0) {
		int i;

		for (i = 0; i < ARRAY_SIZE(of_fmt_table); i++) {
			if (strcmp(str, of_fmt_table[i].name) == 0) {
				format |= of_fmt_table[i].val;
				break;
			}
		}
	}

	/*
	 * check "[prefix]continuous-clock"
	 * SND_SOC_DAIFMT_CLOCK_MASK area
	 */
	snprintf(prop, sizeof(prop), "%scontinuous-clock", prefix);
	if (of_property_read_bool(np, prop))
		format |= SND_SOC_DAIFMT_CONT;
	else
		format |= SND_SOC_DAIFMT_GATED;

	/*
	 * check "[prefix]bitclock-inversion"
	 * check "[prefix]frame-inversion"
	 * SND_SOC_DAIFMT_INV_MASK area
	 */
	snprintf(prop, sizeof(prop), "%sbitclock-inversion", prefix);
	bit = !!of_get_property(np, prop, NULL);

	snprintf(prop, sizeof(prop), "%sframe-inversion", prefix);
	frame = !!of_get_property(np, prop, NULL);

	switch ((bit << 4) + frame) {
	case 0x11:
		format |= SND_SOC_DAIFMT_IB_IF;
		break;
	case 0x10:
		format |= SND_SOC_DAIFMT_IB_NF;
		break;
	case 0x01:
		format |= SND_SOC_DAIFMT_NB_IF;
		break;
	default:
		/* SND_SOC_DAIFMT_NB_NF is default */
		break;
	}

	return format;
}
EXPORT_SYMBOL_GPL(snd_soc_daifmt_parse_format);

unsigned int snd_soc_daifmt_parse_clock_provider_raw(struct device_node *np,
						     const char *prefix,
						     struct device_node **bitclkmaster,
						     struct device_node **framemaster)
{
	char prop[128];
	unsigned int bit, frame;

	if (!prefix)
		prefix = "";

	/*
	 * check "[prefix]bitclock-master"
	 * check "[prefix]frame-master"
	 */
	snprintf(prop, sizeof(prop), "%sbitclock-master", prefix);
	bit = !!of_get_property(np, prop, NULL);
	if (bit && bitclkmaster)
		*bitclkmaster = of_parse_phandle(np, prop, 0);

	snprintf(prop, sizeof(prop), "%sframe-master", prefix);
	frame = !!of_get_property(np, prop, NULL);
	if (frame && framemaster)
		*framemaster = of_parse_phandle(np, prop, 0);

	/*
	 * return bitmap.
	 * It will be parameter of
	 *	snd_soc_daifmt_clock_provider_from_bitmap()
	 */
	return (bit << 4) + frame;
}
EXPORT_SYMBOL_GPL(snd_soc_daifmt_parse_clock_provider_raw);

int snd_soc_get_stream_cpu(struct snd_soc_dai_link *dai_link, int stream)
{
	/*
	 * [Normal]
	 *
	 * Playback
	 *	CPU  : SNDRV_PCM_STREAM_PLAYBACK
	 *	Codec: SNDRV_PCM_STREAM_PLAYBACK
	 *
	 * Capture
	 *	CPU  : SNDRV_PCM_STREAM_CAPTURE
	 *	Codec: SNDRV_PCM_STREAM_CAPTURE
	 */
	if (!dai_link->c2c_params)
		return stream;

	/*
	 * [Codec2Codec]
	 *
	 * Playback
	 *	CPU  : SNDRV_PCM_STREAM_CAPTURE
	 *	Codec: SNDRV_PCM_STREAM_PLAYBACK
	 *
	 * Capture
	 *	CPU  : SNDRV_PCM_STREAM_PLAYBACK
	 *	Codec: SNDRV_PCM_STREAM_CAPTURE
	 */
	if (stream == SNDRV_PCM_STREAM_CAPTURE)
		return SNDRV_PCM_STREAM_PLAYBACK;

	return SNDRV_PCM_STREAM_CAPTURE;
}
EXPORT_SYMBOL_GPL(snd_soc_get_stream_cpu);

int snd_soc_get_dai_id(struct device_node *ep)
{
	struct snd_soc_component *component;
	struct snd_soc_dai_link_component dlc = {
		.of_node = of_graph_get_port_parent(ep),
	};
	int ret;


	/*
	 * For example HDMI case, HDMI has video/sound port,
	 * but ALSA SoC needs sound port number only.
	 * Thus counting HDMI DT port/endpoint doesn't work.
	 * Then, it should have .of_xlate_dai_id
	 */
	ret = -ENOTSUPP;
	mutex_lock(&client_mutex);
	component = soc_find_component(&dlc);
	if (component)
		ret = snd_soc_component_of_xlate_dai_id(component, ep);
	mutex_unlock(&client_mutex);

	of_node_put(dlc.of_node);

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_get_dai_id);

int snd_soc_get_dlc(const struct of_phandle_args *args, struct snd_soc_dai_link_component *dlc)
{
	struct snd_soc_component *pos;
	int ret = -EPROBE_DEFER;

	mutex_lock(&client_mutex);
	for_each_component(pos) {
		struct device_node *component_of_node = soc_component_to_node(pos);

		if (component_of_node != args->np || !pos->num_dai)
			continue;

		ret = snd_soc_component_of_xlate_dai_name(pos, args, &dlc->dai_name);
		if (ret == -ENOTSUPP) {
			struct snd_soc_dai *dai;
			int id = -1;

			switch (args->args_count) {
			case 0:
				id = 0; /* same as dai_drv[0] */
				break;
			case 1:
				id = args->args[0];
				break;
			default:
				/* not supported */
				break;
			}

			if (id < 0 || id >= pos->num_dai) {
				ret = -EINVAL;
				continue;
			}

			ret = 0;

			/* find target DAI */
			for_each_component_dais(pos, dai) {
				if (id == 0)
					break;
				id--;
			}

			dlc->dai_name	= snd_soc_dai_name_get(dai);
		} else if (ret) {
			/*
			 * if another error than ENOTSUPP is returned go on and
			 * check if another component is provided with the same
			 * node. This may happen if a device provides several
			 * components
			 */
			continue;
		}

		break;
	}

	if (ret == 0)
		dlc->of_node = args->np;

	mutex_unlock(&client_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_get_dlc);

int snd_soc_of_get_dlc(struct device_node *of_node,
		       struct of_phandle_args *args,
		       struct snd_soc_dai_link_component *dlc,
		       int index)
{
	struct of_phandle_args __args;
	int ret;

	if (!args)
		args = &__args;

	ret = of_parse_phandle_with_args(of_node, "sound-dai",
					 "#sound-dai-cells", index, args);
	if (ret)
		return ret;

	return snd_soc_get_dlc(args, dlc);
}
EXPORT_SYMBOL_GPL(snd_soc_of_get_dlc);

int snd_soc_get_dai_name(const struct of_phandle_args *args,
			 const char **dai_name)
{
	struct snd_soc_dai_link_component dlc;
	int ret = snd_soc_get_dlc(args, &dlc);

	if (ret == 0)
		*dai_name = dlc.dai_name;

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_get_dai_name);

int snd_soc_of_get_dai_name(struct device_node *of_node,
			    const char **dai_name, int index)
{
	struct snd_soc_dai_link_component dlc;
	int ret = snd_soc_of_get_dlc(of_node, NULL, &dlc, index);

	if (ret == 0)
		*dai_name = dlc.dai_name;

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_of_get_dai_name);

struct snd_soc_dai *snd_soc_get_dai_via_args(const struct of_phandle_args *dai_args)
{
	struct snd_soc_dai *dai;
	struct snd_soc_component *component;

	mutex_lock(&client_mutex);
	for_each_component(component) {
		for_each_component_dais(component, dai)
			if (snd_soc_is_match_dai_args(dai->driver->dai_args, dai_args))
				goto found;
	}
	dai = NULL;
found:
	mutex_unlock(&client_mutex);
	return dai;
}
EXPORT_SYMBOL_GPL(snd_soc_get_dai_via_args);

static void __snd_soc_of_put_component(struct snd_soc_dai_link_component *component)
{
	if (component->of_node) {
		of_node_put(component->of_node);
		component->of_node = NULL;
	}
}

static int __snd_soc_of_get_dai_link_component_alloc(
	struct device *dev, struct device_node *of_node,
	struct snd_soc_dai_link_component **ret_component,
	int *ret_num)
{
	struct snd_soc_dai_link_component *component;
	int num;

	/* Count the number of CPUs/CODECs */
	num = of_count_phandle_with_args(of_node, "sound-dai", "#sound-dai-cells");
	if (num <= 0) {
		if (num == -ENOENT)
			dev_err(dev, "No 'sound-dai' property\n");
		else
			dev_err(dev, "Bad phandle in 'sound-dai'\n");
		return num;
	}
	component = devm_kcalloc(dev, num, sizeof(*component), GFP_KERNEL);
	if (!component)
		return -ENOMEM;

	*ret_component	= component;
	*ret_num	= num;

	return 0;
}

/*
 * snd_soc_of_put_dai_link_codecs - Dereference device nodes in the codecs array
 * @dai_link: DAI link
 *
 * Dereference device nodes acquired by snd_soc_of_get_dai_link_codecs().
 */
void snd_soc_of_put_dai_link_codecs(struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_dai_link_component *component;
	int index;

	for_each_link_codecs(dai_link, index, component)
		__snd_soc_of_put_component(component);
}
EXPORT_SYMBOL_GPL(snd_soc_of_put_dai_link_codecs);

/*
 * snd_soc_of_get_dai_link_codecs - Parse a list of CODECs in the devicetree
 * @dev: Card device
 * @of_node: Device node
 * @dai_link: DAI link
 *
 * Builds an array of CODEC DAI components from the DAI link property
 * 'sound-dai'.
 * The array is set in the DAI link and the number of DAIs is set accordingly.
 * The device nodes in the array (of_node) must be dereferenced by calling
 * snd_soc_of_put_dai_link_codecs() on @dai_link.
 *
 * Returns 0 for success
 */
int snd_soc_of_get_dai_link_codecs(struct device *dev,
				   struct device_node *of_node,
				   struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_dai_link_component *component;
	int index, ret;

	ret = __snd_soc_of_get_dai_link_component_alloc(dev, of_node,
					 &dai_link->codecs, &dai_link->num_codecs);
	if (ret < 0)
		return ret;

	/* Parse the list */
	for_each_link_codecs(dai_link, index, component) {
		ret = snd_soc_of_get_dlc(of_node, NULL, component, index);
		if (ret)
			goto err;
	}
	return 0;
err:
	snd_soc_of_put_dai_link_codecs(dai_link);
	dai_link->codecs = NULL;
	dai_link->num_codecs = 0;
	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_of_get_dai_link_codecs);

/*
 * snd_soc_of_put_dai_link_cpus - Dereference device nodes in the codecs array
 * @dai_link: DAI link
 *
 * Dereference device nodes acquired by snd_soc_of_get_dai_link_cpus().
 */
void snd_soc_of_put_dai_link_cpus(struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_dai_link_component *component;
	int index;

	for_each_link_cpus(dai_link, index, component)
		__snd_soc_of_put_component(component);
}
EXPORT_SYMBOL_GPL(snd_soc_of_put_dai_link_cpus);

/*
 * snd_soc_of_get_dai_link_cpus - Parse a list of CPU DAIs in the devicetree
 * @dev: Card device
 * @of_node: Device node
 * @dai_link: DAI link
 *
 * Is analogous to snd_soc_of_get_dai_link_codecs but parses a list of CPU DAIs
 * instead.
 *
 * Returns 0 for success
 */
int snd_soc_of_get_dai_link_cpus(struct device *dev,
				 struct device_node *of_node,
				 struct snd_soc_dai_link *dai_link)
{
	struct snd_soc_dai_link_component *component;
	int index, ret;

	/* Count the number of CPUs */
	ret = __snd_soc_of_get_dai_link_component_alloc(dev, of_node,
					 &dai_link->cpus, &dai_link->num_cpus);
	if (ret < 0)
		return ret;

	/* Parse the list */
	for_each_link_cpus(dai_link, index, component) {
		ret = snd_soc_of_get_dlc(of_node, NULL, component, index);
		if (ret)
			goto err;
	}
	return 0;
err:
	snd_soc_of_put_dai_link_cpus(dai_link);
	dai_link->cpus = NULL;
	dai_link->num_cpus = 0;
	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_of_get_dai_link_cpus);

static int __init snd_soc_init(void)
{
	int ret;

	snd_soc_debugfs_init();
	ret = snd_soc_util_init();
	if (ret)
		goto err_util_init;

	ret = platform_driver_register(&soc_driver);
	if (ret)
		goto err_register;
	return 0;

err_register:
	snd_soc_util_exit();
err_util_init:
	snd_soc_debugfs_exit();
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
module_init(snd_soc_init);

static void __exit snd_soc_exit(void)
{
	snd_soc_util_exit();
<<<<<<< HEAD

#ifdef CONFIG_DEBUG_FS
	debugfs_remove_recursive(snd_soc_debugfs_root);
#endif
=======
	snd_soc_debugfs_exit();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_driver_unregister(&soc_driver);
}
module_exit(snd_soc_exit);

/* Module information */
MODULE_AUTHOR("Liam Girdwood, lrg@slimlogic.co.uk");
MODULE_DESCRIPTION("ALSA SoC Core");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:soc-audio");
