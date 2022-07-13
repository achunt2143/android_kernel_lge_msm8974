<<<<<<< HEAD
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
<<<<<<< HEAD

#include "zcomp.h"
#include "zcomp_lzo.h"
#ifdef CONFIG_ZRAM_LZ4_COMPRESS
#include "zcomp_lz4.h"
#endif

/*
 * single zcomp_strm backend
 */
struct zcomp_strm_single {
	struct mutex strm_lock;
	struct zcomp_strm *zstrm;
};

/*
 * multi zcomp_strm backend
 */
struct zcomp_strm_multi {
	/* protect strm list */
	spinlock_t strm_lock;
	/* max possible number of zstrm streams */
	int max_strm;
	/* number of available zstrm streams */
	int avail_strm;
	/* list of available strms */
	struct list_head idle_strm;
	wait_queue_head_t strm_wait;
};

static struct zcomp_backend *backends[] = {
	&zcomp_lzo,
#ifdef CONFIG_ZRAM_LZ4_COMPRESS
	&zcomp_lz4,
#endif
	NULL
};

static struct zcomp_backend *find_backend(const char *compress)
{
	int i = 0;
	while (backends[i]) {
		if (sysfs_streq(compress, backends[i]->name))
			break;
		i++;
	}
	return backends[i];
}

static void zcomp_strm_free(struct zcomp *comp, struct zcomp_strm *zstrm)
{
	if (zstrm->private)
		comp->backend->destroy(zstrm->private);
	free_pages((unsigned long)zstrm->buffer, 1);
	kfree(zstrm);
}

/*
 * allocate new zcomp_strm structure with ->private initialized by
 * backend, return NULL on error
 */
static struct zcomp_strm *zcomp_strm_alloc(struct zcomp *comp, gfp_t flags)
{
	struct zcomp_strm *zstrm = kmalloc(sizeof(*zstrm), flags);
	if (!zstrm)
		return NULL;

	zstrm->private = comp->backend->create(flags);
=======
#include <linux/cpu.h>
#include <linux/crypto.h>
#include <linux/vmalloc.h>

#include "zcomp.h"

static const char * const backends[] = {
#if IS_ENABLED(CONFIG_CRYPTO_LZO)
	"lzo",
	"lzo-rle",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_LZ4)
	"lz4",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_LZ4HC)
	"lz4hc",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_842)
	"842",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_ZSTD)
	"zstd",
#endif
};

static void zcomp_strm_free(struct zcomp_strm *zstrm)
{
	if (!IS_ERR_OR_NULL(zstrm->tfm))
		crypto_free_comp(zstrm->tfm);
	vfree(zstrm->buffer);
	zstrm->tfm = NULL;
	zstrm->buffer = NULL;
}

/*
 * Initialize zcomp_strm structure with ->tfm initialized by backend, and
 * ->buffer. Return a negative value on error.
 */
static int zcomp_strm_init(struct zcomp_strm *zstrm, struct zcomp *comp)
{
	zstrm->tfm = crypto_alloc_comp(comp->name, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * allocate 2 pages. 1 for compressed data, plus 1 extra for the
	 * case when compressed size is larger than the original one
	 */
<<<<<<< HEAD
	zstrm->buffer = (void *)__get_free_pages(flags | __GFP_ZERO, 1);
	if (!zstrm->private || !zstrm->buffer) {
		zcomp_strm_free(comp, zstrm);
		zstrm = NULL;
	}
	return zstrm;
}

/*
 * get idle zcomp_strm or wait until other process release
 * (zcomp_strm_release()) one for us
 */
static struct zcomp_strm *zcomp_strm_multi_find(struct zcomp *comp)
{
	struct zcomp_strm_multi *zs = comp->stream;
	struct zcomp_strm *zstrm;

	while (1) {
		spin_lock(&zs->strm_lock);
		if (!list_empty(&zs->idle_strm)) {
			zstrm = list_entry(zs->idle_strm.next,
					struct zcomp_strm, list);
			list_del(&zstrm->list);
			spin_unlock(&zs->strm_lock);
			return zstrm;
		}
		/* zstrm streams limit reached, wait for idle stream */
		if (zs->avail_strm >= zs->max_strm) {
			spin_unlock(&zs->strm_lock);
			wait_event(zs->strm_wait, !list_empty(&zs->idle_strm));
			continue;
		}
		/* allocate new zstrm stream */
		zs->avail_strm++;
		spin_unlock(&zs->strm_lock);
		/*
		 * This function can be called in swapout/fs write path
		 * so we can't use GFP_FS|IO. And it assumes we already
		 * have at least one stream in zram initialization so we
		 * don't do best effort to allocate more stream in here.
		 * A default stream will work well without further multiple
		 * streams. That's why we use NORETRY | NOWARN.
		 */
		zstrm = zcomp_strm_alloc(comp, GFP_NOIO | __GFP_NORETRY |
					__GFP_NOWARN);
		if (!zstrm) {
			spin_lock(&zs->strm_lock);
			zs->avail_strm--;
			spin_unlock(&zs->strm_lock);
			wait_event(zs->strm_wait, !list_empty(&zs->idle_strm));
			continue;
		}
		break;
	}
	return zstrm;
}

/* add stream back to idle list and wake up waiter or free the stream */
static void zcomp_strm_multi_release(struct zcomp *comp, struct zcomp_strm *zstrm)
{
	struct zcomp_strm_multi *zs = comp->stream;

	spin_lock(&zs->strm_lock);
	if (zs->avail_strm <= zs->max_strm) {
		list_add(&zstrm->list, &zs->idle_strm);
		spin_unlock(&zs->strm_lock);
		wake_up(&zs->strm_wait);
		return;
	}

	zs->avail_strm--;
	spin_unlock(&zs->strm_lock);
	zcomp_strm_free(comp, zstrm);
}

/* change max_strm limit */
static bool zcomp_strm_multi_set_max_streams(struct zcomp *comp, int num_strm)
{
	struct zcomp_strm_multi *zs = comp->stream;
	struct zcomp_strm *zstrm;

	spin_lock(&zs->strm_lock);
	zs->max_strm = num_strm;
	/*
	 * if user has lowered the limit and there are idle streams,
	 * immediately free as much streams (and memory) as we can.
	 */
	while (zs->avail_strm > num_strm && !list_empty(&zs->idle_strm)) {
		zstrm = list_entry(zs->idle_strm.next,
				struct zcomp_strm, list);
		list_del(&zstrm->list);
		zcomp_strm_free(comp, zstrm);
		zs->avail_strm--;
	}
	spin_unlock(&zs->strm_lock);
	return true;
}

static void zcomp_strm_multi_destroy(struct zcomp *comp)
{
	struct zcomp_strm_multi *zs = comp->stream;
	struct zcomp_strm *zstrm;

	while (!list_empty(&zs->idle_strm)) {
		zstrm = list_entry(zs->idle_strm.next,
				struct zcomp_strm, list);
		list_del(&zstrm->list);
		zcomp_strm_free(comp, zstrm);
	}
	kfree(zs);
}

static int zcomp_strm_multi_create(struct zcomp *comp, int max_strm)
{
	struct zcomp_strm *zstrm;
	struct zcomp_strm_multi *zs;

	comp->destroy = zcomp_strm_multi_destroy;
	comp->strm_find = zcomp_strm_multi_find;
	comp->strm_release = zcomp_strm_multi_release;
	comp->set_max_streams = zcomp_strm_multi_set_max_streams;
	zs = kmalloc(sizeof(struct zcomp_strm_multi), GFP_KERNEL);
	if (!zs)
		return -ENOMEM;

	comp->stream = zs;
	spin_lock_init(&zs->strm_lock);
	INIT_LIST_HEAD(&zs->idle_strm);
	init_waitqueue_head(&zs->strm_wait);
	zs->max_strm = max_strm;
	zs->avail_strm = 1;

	zstrm = zcomp_strm_alloc(comp, GFP_KERNEL);
	if (!zstrm) {
		kfree(zs);
		return -ENOMEM;
	}
	list_add(&zstrm->list, &zs->idle_strm);
	return 0;
}

static struct zcomp_strm *zcomp_strm_single_find(struct zcomp *comp)
{
	struct zcomp_strm_single *zs = comp->stream;
	mutex_lock(&zs->strm_lock);
	return zs->zstrm;
}

static void zcomp_strm_single_release(struct zcomp *comp,
		struct zcomp_strm *zstrm)
{
	struct zcomp_strm_single *zs = comp->stream;
	mutex_unlock(&zs->strm_lock);
}

static bool zcomp_strm_single_set_max_streams(struct zcomp *comp, int num_strm)
{
	/* zcomp_strm_single support only max_comp_streams == 1 */
	return false;
}

static void zcomp_strm_single_destroy(struct zcomp *comp)
{
	struct zcomp_strm_single *zs = comp->stream;
	zcomp_strm_free(comp, zs->zstrm);
	kfree(zs);
}

static int zcomp_strm_single_create(struct zcomp *comp)
{
	struct zcomp_strm_single *zs;

	comp->destroy = zcomp_strm_single_destroy;
	comp->strm_find = zcomp_strm_single_find;
	comp->strm_release = zcomp_strm_single_release;
	comp->set_max_streams = zcomp_strm_single_set_max_streams;
	zs = kmalloc(sizeof(struct zcomp_strm_single), GFP_KERNEL);
	if (!zs)
		return -ENOMEM;

	comp->stream = zs;
	mutex_init(&zs->strm_lock);
	zs->zstrm = zcomp_strm_alloc(comp, GFP_KERNEL);
	if (!zs->zstrm) {
		kfree(zs);
		return -ENOMEM;
	}
	return 0;
=======
	zstrm->buffer = vzalloc(2 * PAGE_SIZE);
	if (IS_ERR_OR_NULL(zstrm->tfm) || !zstrm->buffer) {
		zcomp_strm_free(zstrm);
		return -ENOMEM;
	}
	return 0;
}

bool zcomp_available_algorithm(const char *comp)
{
	/*
	 * Crypto does not ignore a trailing new line symbol,
	 * so make sure you don't supply a string containing
	 * one.
	 * This also means that we permit zcomp initialisation
	 * with any compressing algorithm known to crypto api.
	 */
	return crypto_has_comp(comp, 0, 0) == 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* show available compressors */
ssize_t zcomp_available_show(const char *comp, char *buf)
{
<<<<<<< HEAD
	ssize_t sz = 0;
	int i = 0;

	while (backends[i]) {
		if (sysfs_streq(comp, backends[i]->name))
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"[%s] ", backends[i]->name);
		else
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"%s ", backends[i]->name);
		i++;
	}
=======
	bool known_algorithm = false;
	ssize_t sz = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(backends); i++) {
		if (!strcmp(comp, backends[i])) {
			known_algorithm = true;
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"[%s] ", backends[i]);
		} else {
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"%s ", backends[i]);
		}
	}

	/*
	 * Out-of-tree module known to crypto api or a missing
	 * entry in `backends'.
	 */
	if (!known_algorithm && crypto_has_comp(comp, 0, 0) == 1)
		sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
				"[%s] ", comp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sz += scnprintf(buf + sz, PAGE_SIZE - sz, "\n");
	return sz;
}

<<<<<<< HEAD
bool zcomp_set_max_streams(struct zcomp *comp, int num_strm)
{
	return comp->set_max_streams(comp, num_strm);
}

struct zcomp_strm *zcomp_strm_find(struct zcomp *comp)
{
	return comp->strm_find(comp);
}

void zcomp_strm_release(struct zcomp *comp, struct zcomp_strm *zstrm)
{
	comp->strm_release(comp, zstrm);
}

int zcomp_compress(struct zcomp *comp, struct zcomp_strm *zstrm,
		const unsigned char *src, size_t *dst_len)
{
	return comp->backend->compress(src, zstrm->buffer, dst_len,
			zstrm->private);
}

int zcomp_decompress(struct zcomp *comp, const unsigned char *src,
		size_t src_len, unsigned char *dst)
{
	return comp->backend->decompress(src, src_len, dst);
=======
struct zcomp_strm *zcomp_stream_get(struct zcomp *comp)
{
	local_lock(&comp->stream->lock);
	return this_cpu_ptr(comp->stream);
}

void zcomp_stream_put(struct zcomp *comp)
{
	local_unlock(&comp->stream->lock);
}

int zcomp_compress(struct zcomp_strm *zstrm,
		const void *src, unsigned int *dst_len)
{
	/*
	 * Our dst memory (zstrm->buffer) is always `2 * PAGE_SIZE' sized
	 * because sometimes we can endup having a bigger compressed data
	 * due to various reasons: for example compression algorithms tend
	 * to add some padding to the compressed buffer. Speaking of padding,
	 * comp algorithm `842' pads the compressed length to multiple of 8
	 * and returns -ENOSP when the dst memory is not big enough, which
	 * is not something that ZRAM wants to see. We can handle the
	 * `compressed_size > PAGE_SIZE' case easily in ZRAM, but when we
	 * receive -ERRNO from the compressing backend we can't help it
	 * anymore. To make `842' happy we need to tell the exact size of
	 * the dst buffer, zram_drv will take care of the fact that
	 * compressed buffer is too big.
	 */
	*dst_len = PAGE_SIZE * 2;

	return crypto_comp_compress(zstrm->tfm,
			src, PAGE_SIZE,
			zstrm->buffer, dst_len);
}

int zcomp_decompress(struct zcomp_strm *zstrm,
		const void *src, unsigned int src_len, void *dst)
{
	unsigned int dst_len = PAGE_SIZE;

	return crypto_comp_decompress(zstrm->tfm,
			src, src_len,
			dst, &dst_len);
}

int zcomp_cpu_up_prepare(unsigned int cpu, struct hlist_node *node)
{
	struct zcomp *comp = hlist_entry(node, struct zcomp, node);
	struct zcomp_strm *zstrm;
	int ret;

	zstrm = per_cpu_ptr(comp->stream, cpu);
	local_lock_init(&zstrm->lock);

	ret = zcomp_strm_init(zstrm, comp);
	if (ret)
		pr_err("Can't allocate a compression stream\n");
	return ret;
}

int zcomp_cpu_dead(unsigned int cpu, struct hlist_node *node)
{
	struct zcomp *comp = hlist_entry(node, struct zcomp, node);
	struct zcomp_strm *zstrm;

	zstrm = per_cpu_ptr(comp->stream, cpu);
	zcomp_strm_free(zstrm);
	return 0;
}

static int zcomp_init(struct zcomp *comp)
{
	int ret;

	comp->stream = alloc_percpu(struct zcomp_strm);
	if (!comp->stream)
		return -ENOMEM;

	ret = cpuhp_state_add_instance(CPUHP_ZCOMP_PREPARE, &comp->node);
	if (ret < 0)
		goto cleanup;
	return 0;

cleanup:
	free_percpu(comp->stream);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void zcomp_destroy(struct zcomp *comp)
{
<<<<<<< HEAD
	comp->destroy(comp);
=======
	cpuhp_state_remove_instance(CPUHP_ZCOMP_PREPARE, &comp->node);
	free_percpu(comp->stream);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(comp);
}

/*
 * search available compressors for requested algorithm.
 * allocate new zcomp and initialize it. return compressing
 * backend pointer or ERR_PTR if things went bad. ERR_PTR(-EINVAL)
 * if requested algorithm is not supported, ERR_PTR(-ENOMEM) in
<<<<<<< HEAD
 * case of allocation error.
 */
struct zcomp *zcomp_create(const char *compress, int max_strm)
{
	struct zcomp *comp;
	struct zcomp_backend *backend;

	backend = find_backend(compress);
	if (!backend)
=======
 * case of allocation error, or any other error potentially
 * returned by zcomp_init().
 */
struct zcomp *zcomp_create(const char *alg)
{
	struct zcomp *comp;
	int error;

	/*
	 * Crypto API will execute /sbin/modprobe if the compression module
	 * is not loaded yet. We must do it here, otherwise we are about to
	 * call /sbin/modprobe under CPU hot-plug lock.
	 */
	if (!zcomp_available_algorithm(alg))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-EINVAL);

	comp = kzalloc(sizeof(struct zcomp), GFP_KERNEL);
	if (!comp)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	comp->backend = backend;
	if (max_strm > 1)
		zcomp_strm_multi_create(comp, max_strm);
	else
		zcomp_strm_single_create(comp);
	if (!comp->stream) {
		kfree(comp);
		return ERR_PTR(-ENOMEM);
=======
	comp->name = alg;
	error = zcomp_init(comp);
	if (error) {
		kfree(comp);
		return ERR_PTR(error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return comp;
}
