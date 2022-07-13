<<<<<<< HEAD
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ZCOMP_H_
#define _ZCOMP_H_
<<<<<<< HEAD

#include <linux/mutex.h>

struct zcomp_strm {
	/* compression/decompression buffer */
	void *buffer;
	/*
	 * The private data of the compression stream, only compression
	 * stream backend can touch this (e.g. compression algorithm
	 * working memory)
	 */
	void *private;
	/* used in multi stream backend, protected by backend strm_lock */
	struct list_head list;
};

/* static compression backend */
struct zcomp_backend {
	int (*compress)(const unsigned char *src, unsigned char *dst,
			size_t *dst_len, void *private);

	int (*decompress)(const unsigned char *src, size_t src_len,
			unsigned char *dst);

	void *(*create)(gfp_t flags);
	void (*destroy)(void *private);

	const char *name;
=======
#include <linux/local_lock.h>

struct zcomp_strm {
	/* The members ->buffer and ->tfm are protected by ->lock. */
	local_lock_t lock;
	/* compression/decompression buffer */
	void *buffer;
	struct crypto_comp *tfm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* dynamic per-device compression frontend */
struct zcomp {
<<<<<<< HEAD
	void *stream;
	struct zcomp_backend *backend;

	struct zcomp_strm *(*strm_find)(struct zcomp *comp);
	void (*strm_release)(struct zcomp *comp, struct zcomp_strm *zstrm);
	bool (*set_max_streams)(struct zcomp *comp, int num_strm);
	void (*destroy)(struct zcomp *comp);
};

ssize_t zcomp_available_show(const char *comp, char *buf);

struct zcomp *zcomp_create(const char *comp, int max_strm);
void zcomp_destroy(struct zcomp *comp);

struct zcomp_strm *zcomp_strm_find(struct zcomp *comp);
void zcomp_strm_release(struct zcomp *comp, struct zcomp_strm *zstrm);

int zcomp_compress(struct zcomp *comp, struct zcomp_strm *zstrm,
		const unsigned char *src, size_t *dst_len);

int zcomp_decompress(struct zcomp *comp, const unsigned char *src,
		size_t src_len, unsigned char *dst);

bool zcomp_set_max_streams(struct zcomp *comp, int num_strm);
=======
	struct zcomp_strm __percpu *stream;
	const char *name;
	struct hlist_node node;
};

int zcomp_cpu_up_prepare(unsigned int cpu, struct hlist_node *node);
int zcomp_cpu_dead(unsigned int cpu, struct hlist_node *node);
ssize_t zcomp_available_show(const char *comp, char *buf);
bool zcomp_available_algorithm(const char *comp);

struct zcomp *zcomp_create(const char *alg);
void zcomp_destroy(struct zcomp *comp);

struct zcomp_strm *zcomp_stream_get(struct zcomp *comp);
void zcomp_stream_put(struct zcomp *comp);

int zcomp_compress(struct zcomp_strm *zstrm,
		const void *src, unsigned int *dst_len);

int zcomp_decompress(struct zcomp_strm *zstrm,
		const void *src, unsigned int src_len, void *dst);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ZCOMP_H_ */
