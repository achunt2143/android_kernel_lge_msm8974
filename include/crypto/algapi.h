<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API for algorithms (i.e., low-level API).
 *
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _CRYPTO_ALGAPI_H
#define _CRYPTO_ALGAPI_H

<<<<<<< HEAD
#include <linux/crypto.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>

struct module;
struct rtattr;
struct seq_file;
=======
#include <crypto/utils.h>
#include <linux/align.h>
#include <linux/cache.h>
#include <linux/crypto.h>
#include <linux/types.h>
#include <linux/workqueue.h>

/*
 * Maximum values for blocksize and alignmask, used to allocate
 * static buffers that are big enough for any combination of
 * algs and architectures. Ciphers have a lower maximum size.
 */
#define MAX_ALGAPI_BLOCKSIZE		160
#define MAX_ALGAPI_ALIGNMASK		127
#define MAX_CIPHER_BLOCKSIZE		16
#define MAX_CIPHER_ALIGNMASK		15

#ifdef ARCH_DMA_MINALIGN
#define CRYPTO_DMA_ALIGN ARCH_DMA_MINALIGN
#else
#define CRYPTO_DMA_ALIGN CRYPTO_MINALIGN
#endif

#define CRYPTO_DMA_PADDING ((CRYPTO_DMA_ALIGN - 1) & ~(CRYPTO_MINALIGN - 1))

/*
 * Autoloaded crypto modules should only use a prefixed name to avoid allowing
 * arbitrary modules to be loaded. Loading from userspace may still need the
 * unprefixed names, so retains those aliases as well.
 * This uses __MODULE_INFO directly instead of MODULE_ALIAS because pre-4.3
 * gcc (e.g. avr32 toolchain) uses __LINE__ for uniqueness, and this macro
 * expands twice on the same line. Instead, use a separate base name for the
 * alias.
 */
#define MODULE_ALIAS_CRYPTO(name)	\
		__MODULE_INFO(alias, alias_userspace, name);	\
		__MODULE_INFO(alias, alias_crypto, "crypto-" name)

struct crypto_aead;
struct crypto_instance;
struct module;
struct notifier_block;
struct rtattr;
struct scatterlist;
struct seq_file;
struct sk_buff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct crypto_type {
	unsigned int (*ctxsize)(struct crypto_alg *alg, u32 type, u32 mask);
	unsigned int (*extsize)(struct crypto_alg *alg);
<<<<<<< HEAD
	int (*init)(struct crypto_tfm *tfm, u32 type, u32 mask);
	int (*init_tfm)(struct crypto_tfm *tfm);
	void (*show)(struct seq_file *m, struct crypto_alg *alg);
	int (*report)(struct sk_buff *skb, struct crypto_alg *alg);
	struct crypto_alg *(*lookup)(const char *name, u32 type, u32 mask);
=======
	int (*init_tfm)(struct crypto_tfm *tfm);
	void (*show)(struct seq_file *m, struct crypto_alg *alg);
	int (*report)(struct sk_buff *skb, struct crypto_alg *alg);
	void (*free)(struct crypto_instance *inst);
#ifdef CONFIG_CRYPTO_STATS
	int (*report_stat)(struct sk_buff *skb, struct crypto_alg *alg);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned int type;
	unsigned int maskclear;
	unsigned int maskset;
	unsigned int tfmsize;
};

struct crypto_instance {
	struct crypto_alg alg;

	struct crypto_template *tmpl;
<<<<<<< HEAD
	struct hlist_node list;
=======

	union {
		/* Node in list of instances after registration. */
		struct hlist_node list;
		/* List of attached spawns before registration. */
		struct crypto_spawn *spawns;
	};

	struct work_struct free_work;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	void *__ctx[] CRYPTO_MINALIGN_ATTR;
};

struct crypto_template {
	struct list_head list;
	struct hlist_head instances;
	struct module *module;

<<<<<<< HEAD
	struct crypto_instance *(*alloc)(struct rtattr **tb);
	void (*free)(struct crypto_instance *inst);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*create)(struct crypto_template *tmpl, struct rtattr **tb);

	char name[CRYPTO_MAX_ALG_NAME];
};

struct crypto_spawn {
	struct list_head list;
	struct crypto_alg *alg;
<<<<<<< HEAD
	struct crypto_instance *inst;
	const struct crypto_type *frontend;
	u32 mask;
=======
	union {
		/* Back pointer to instance after registration.*/
		struct crypto_instance *inst;
		/* Spawn list pointer prior to registration. */
		struct crypto_spawn *next;
	};
	const struct crypto_type *frontend;
	u32 mask;
	bool dead;
	bool registered;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct crypto_queue {
	struct list_head list;
	struct list_head *backlog;

	unsigned int qlen;
	unsigned int max_qlen;
};

struct scatter_walk {
	struct scatterlist *sg;
	unsigned int offset;
};

<<<<<<< HEAD
struct blkcipher_walk {
	union {
		struct {
			struct page *page;
			unsigned long offset;
		} phys;

		struct {
			u8 *page;
			u8 *addr;
		} virt;
	} src, dst;

	struct scatter_walk in;
	unsigned int nbytes;

	struct scatter_walk out;
	unsigned int total;

	void *page;
	u8 *buffer;
	u8 *iv;

	int flags;
	unsigned int blocksize;
};

struct ablkcipher_walk {
	struct {
		struct page *page;
		unsigned int offset;
	} src, dst;

	struct scatter_walk	in;
	unsigned int		nbytes;
	struct scatter_walk	out;
	unsigned int		total;
	struct list_head	buffers;
	u8			*iv_buffer;
	u8			*iv;
	int			flags;
	unsigned int		blocksize;
};

extern const struct crypto_type crypto_ablkcipher_type;
extern const struct crypto_type crypto_aead_type;
extern const struct crypto_type crypto_blkcipher_type;
=======
struct crypto_attr_alg {
	char name[CRYPTO_MAX_ALG_NAME];
};

struct crypto_attr_type {
	u32 type;
	u32 mask;
};

/*
 * Algorithm registration interface.
 */
int crypto_register_alg(struct crypto_alg *alg);
void crypto_unregister_alg(struct crypto_alg *alg);
int crypto_register_algs(struct crypto_alg *algs, int count);
void crypto_unregister_algs(struct crypto_alg *algs, int count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void crypto_mod_put(struct crypto_alg *alg);

int crypto_register_template(struct crypto_template *tmpl);
<<<<<<< HEAD
void crypto_unregister_template(struct crypto_template *tmpl);
=======
int crypto_register_templates(struct crypto_template *tmpls, int count);
void crypto_unregister_template(struct crypto_template *tmpl);
void crypto_unregister_templates(struct crypto_template *tmpls, int count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct crypto_template *crypto_lookup_template(const char *name);

int crypto_register_instance(struct crypto_template *tmpl,
			     struct crypto_instance *inst);
<<<<<<< HEAD
int crypto_unregister_instance(struct crypto_alg *alg);

int crypto_init_spawn(struct crypto_spawn *spawn, struct crypto_alg *alg,
		      struct crypto_instance *inst, u32 mask);
int crypto_init_spawn2(struct crypto_spawn *spawn, struct crypto_alg *alg,
		       struct crypto_instance *inst,
		       const struct crypto_type *frontend);

=======
void crypto_unregister_instance(struct crypto_instance *inst);

int crypto_grab_spawn(struct crypto_spawn *spawn, struct crypto_instance *inst,
		      const char *name, u32 type, u32 mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void crypto_drop_spawn(struct crypto_spawn *spawn);
struct crypto_tfm *crypto_spawn_tfm(struct crypto_spawn *spawn, u32 type,
				    u32 mask);
void *crypto_spawn_tfm2(struct crypto_spawn *spawn);

<<<<<<< HEAD
static inline void crypto_set_spawn(struct crypto_spawn *spawn,
				    struct crypto_instance *inst)
{
	spawn->inst = inst;
}

struct crypto_attr_type *crypto_get_attr_type(struct rtattr **tb);
int crypto_check_attr_type(struct rtattr **tb, u32 type);
const char *crypto_attr_alg_name(struct rtattr *rta);
struct crypto_alg *crypto_attr_alg2(struct rtattr *rta,
				    const struct crypto_type *frontend,
				    u32 type, u32 mask);

static inline struct crypto_alg *crypto_attr_alg(struct rtattr *rta,
						 u32 type, u32 mask)
{
	return crypto_attr_alg2(rta, NULL, type, mask);
}

int crypto_attr_u32(struct rtattr *rta, u32 *num);
void *crypto_alloc_instance2(const char *name, struct crypto_alg *alg,
			     unsigned int head);
struct crypto_instance *crypto_alloc_instance(const char *name,
					      struct crypto_alg *alg);
=======
struct crypto_attr_type *crypto_get_attr_type(struct rtattr **tb);
int crypto_check_attr_type(struct rtattr **tb, u32 type, u32 *mask_ret);
const char *crypto_attr_alg_name(struct rtattr *rta);
int crypto_inst_setname(struct crypto_instance *inst, const char *name,
			struct crypto_alg *alg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void crypto_init_queue(struct crypto_queue *queue, unsigned int max_qlen);
int crypto_enqueue_request(struct crypto_queue *queue,
			   struct crypto_async_request *request);
<<<<<<< HEAD
void *__crypto_dequeue_request(struct crypto_queue *queue, unsigned int offset);
struct crypto_async_request *crypto_dequeue_request(struct crypto_queue *queue);
int crypto_tfm_in_queue(struct crypto_queue *queue, struct crypto_tfm *tfm);

/* These functions require the input/output to be aligned as u32. */
void crypto_inc(u8 *a, unsigned int size);
void crypto_xor(u8 *dst, const u8 *src, unsigned int size);

int blkcipher_walk_done(struct blkcipher_desc *desc,
			struct blkcipher_walk *walk, int err);
int blkcipher_walk_virt(struct blkcipher_desc *desc,
			struct blkcipher_walk *walk);
int blkcipher_walk_phys(struct blkcipher_desc *desc,
			struct blkcipher_walk *walk);
int blkcipher_walk_virt_block(struct blkcipher_desc *desc,
			      struct blkcipher_walk *walk,
			      unsigned int blocksize);

int ablkcipher_walk_done(struct ablkcipher_request *req,
			 struct ablkcipher_walk *walk, int err);
int ablkcipher_walk_phys(struct ablkcipher_request *req,
			 struct ablkcipher_walk *walk);
void __ablkcipher_walk_complete(struct ablkcipher_walk *walk);

static inline void *crypto_tfm_ctx_aligned(struct crypto_tfm *tfm)
{
	return PTR_ALIGN(crypto_tfm_ctx(tfm),
			 crypto_tfm_alg_alignmask(tfm) + 1);
=======
void crypto_enqueue_request_head(struct crypto_queue *queue,
				 struct crypto_async_request *request);
struct crypto_async_request *crypto_dequeue_request(struct crypto_queue *queue);
static inline unsigned int crypto_queue_len(struct crypto_queue *queue)
{
	return queue->qlen;
}

void crypto_inc(u8 *a, unsigned int size);

static inline void *crypto_tfm_ctx(struct crypto_tfm *tfm)
{
	return tfm->__crt_ctx;
}

static inline void *crypto_tfm_ctx_align(struct crypto_tfm *tfm,
					 unsigned int align)
{
	if (align <= crypto_tfm_ctx_alignment())
		align = 1;

	return PTR_ALIGN(crypto_tfm_ctx(tfm), align);
}

static inline unsigned int crypto_dma_align(void)
{
	return CRYPTO_DMA_ALIGN;
}

static inline unsigned int crypto_dma_padding(void)
{
	return (crypto_dma_align() - 1) & ~(crypto_tfm_ctx_alignment() - 1);
}

static inline void *crypto_tfm_ctx_dma(struct crypto_tfm *tfm)
{
	return crypto_tfm_ctx_align(tfm, crypto_dma_align());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline struct crypto_instance *crypto_tfm_alg_instance(
	struct crypto_tfm *tfm)
{
	return container_of(tfm->__crt_alg, struct crypto_instance, alg);
}

static inline void *crypto_instance_ctx(struct crypto_instance *inst)
{
	return inst->__ctx;
}

<<<<<<< HEAD
static inline struct ablkcipher_alg *crypto_ablkcipher_alg(
	struct crypto_ablkcipher *tfm)
{
	return &crypto_ablkcipher_tfm(tfm)->__crt_alg->cra_ablkcipher;
}

static inline void *crypto_ablkcipher_ctx(struct crypto_ablkcipher *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline void *crypto_ablkcipher_ctx_aligned(struct crypto_ablkcipher *tfm)
{
	return crypto_tfm_ctx_aligned(&tfm->base);
}

static inline struct aead_alg *crypto_aead_alg(struct crypto_aead *tfm)
{
	return &crypto_aead_tfm(tfm)->__crt_alg->cra_aead;
}

static inline void *crypto_aead_ctx(struct crypto_aead *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline struct crypto_instance *crypto_aead_alg_instance(
	struct crypto_aead *aead)
{
	return crypto_tfm_alg_instance(&aead->base);
}

static inline struct crypto_blkcipher *crypto_spawn_blkcipher(
	struct crypto_spawn *spawn)
{
	u32 type = CRYPTO_ALG_TYPE_BLKCIPHER;
	u32 mask = CRYPTO_ALG_TYPE_MASK;

	return __crypto_blkcipher_cast(crypto_spawn_tfm(spawn, type, mask));
}

static inline void *crypto_blkcipher_ctx(struct crypto_blkcipher *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline void *crypto_blkcipher_ctx_aligned(struct crypto_blkcipher *tfm)
{
	return crypto_tfm_ctx_aligned(&tfm->base);
}

static inline struct crypto_cipher *crypto_spawn_cipher(
	struct crypto_spawn *spawn)
{
	u32 type = CRYPTO_ALG_TYPE_CIPHER;
	u32 mask = CRYPTO_ALG_TYPE_MASK;

	return __crypto_cipher_cast(crypto_spawn_tfm(spawn, type, mask));
}

static inline struct cipher_alg *crypto_cipher_alg(struct crypto_cipher *tfm)
{
	return &crypto_cipher_tfm(tfm)->__crt_alg->cra_cipher;
}

static inline struct crypto_hash *crypto_spawn_hash(struct crypto_spawn *spawn)
{
	u32 type = CRYPTO_ALG_TYPE_HASH;
	u32 mask = CRYPTO_ALG_TYPE_HASH_MASK;

	return __crypto_hash_cast(crypto_spawn_tfm(spawn, type, mask));
}

static inline void *crypto_hash_ctx(struct crypto_hash *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline void *crypto_hash_ctx_aligned(struct crypto_hash *tfm)
{
	return crypto_tfm_ctx_aligned(&tfm->base);
}

static inline void blkcipher_walk_init(struct blkcipher_walk *walk,
				       struct scatterlist *dst,
				       struct scatterlist *src,
				       unsigned int nbytes)
{
	walk->in.sg = src;
	walk->out.sg = dst;
	walk->total = nbytes;
}

static inline void ablkcipher_walk_init(struct ablkcipher_walk *walk,
					struct scatterlist *dst,
					struct scatterlist *src,
					unsigned int nbytes)
{
	walk->in.sg = src;
	walk->out.sg = dst;
	walk->total = nbytes;
	INIT_LIST_HEAD(&walk->buffers);
}

static inline void ablkcipher_walk_complete(struct ablkcipher_walk *walk)
{
	if (unlikely(!list_empty(&walk->buffers)))
		__ablkcipher_walk_complete(walk);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct crypto_async_request *crypto_get_backlog(
	struct crypto_queue *queue)
{
	return queue->backlog == &queue->list ? NULL :
	       container_of(queue->backlog, struct crypto_async_request, list);
}

<<<<<<< HEAD
static inline int ablkcipher_enqueue_request(struct crypto_queue *queue,
					     struct ablkcipher_request *request)
{
	return crypto_enqueue_request(queue, &request->base);
}

static inline struct ablkcipher_request *ablkcipher_dequeue_request(
	struct crypto_queue *queue)
{
	return ablkcipher_request_cast(crypto_dequeue_request(queue));
}

static inline void *ablkcipher_request_ctx(struct ablkcipher_request *req)
{
	return req->__ctx;
}

static inline int ablkcipher_tfm_in_queue(struct crypto_queue *queue,
					  struct crypto_ablkcipher *tfm)
{
	return crypto_tfm_in_queue(queue, crypto_ablkcipher_tfm(tfm));
}

static inline void *aead_request_ctx(struct aead_request *req)
{
	return req->__ctx;
}

static inline void aead_request_complete(struct aead_request *req, int err)
{
	req->base.complete(&req->base, err);
}

static inline u32 aead_request_flags(struct aead_request *req)
{
	return req->base.flags;
}

static inline struct crypto_alg *crypto_get_attr_alg(struct rtattr **tb,
						     u32 type, u32 mask)
{
	return crypto_attr_alg(tb[1], type, mask);
}

/*
 * Returns CRYPTO_ALG_ASYNC if type/mask requires the use of sync algorithms.
 * Otherwise returns zero.
 */
static inline int crypto_requires_sync(u32 type, u32 mask)
{
	return (type ^ CRYPTO_ALG_ASYNC) & mask & CRYPTO_ALG_ASYNC;
}

noinline unsigned long __crypto_memneq(const void *a, const void *b, size_t size);

/**
 * crypto_memneq - Compare two areas of memory without leaking
 *		   timing information.
 *
 * @a: One area of memory
 * @b: Another area of memory
 * @size: The size of the area.
 *
 * Returns 0 when data is equal, 1 otherwise.
 */
static inline int crypto_memneq(const void *a, const void *b, size_t size)
{
	return __crypto_memneq(a, b, size) != 0UL ? 1 : 0;
=======
static inline u32 crypto_requires_off(struct crypto_attr_type *algt, u32 off)
{
	return (algt->type ^ off) & algt->mask & off;
}

/*
 * When an algorithm uses another algorithm (e.g., if it's an instance of a
 * template), these are the flags that should always be set on the "outer"
 * algorithm if any "inner" algorithm has them set.
 */
#define CRYPTO_ALG_INHERITED_FLAGS	\
	(CRYPTO_ALG_ASYNC | CRYPTO_ALG_NEED_FALLBACK |	\
	 CRYPTO_ALG_ALLOCATES_MEMORY)

/*
 * Given the type and mask that specify the flags restrictions on a template
 * instance being created, return the mask that should be passed to
 * crypto_grab_*() (along with type=0) to honor any request the user made to
 * have any of the CRYPTO_ALG_INHERITED_FLAGS clear.
 */
static inline u32 crypto_algt_inherited_mask(struct crypto_attr_type *algt)
{
	return crypto_requires_off(algt, CRYPTO_ALG_INHERITED_FLAGS);
}

int crypto_register_notifier(struct notifier_block *nb);
int crypto_unregister_notifier(struct notifier_block *nb);

/* Crypto notification events. */
enum {
	CRYPTO_MSG_ALG_REQUEST,
	CRYPTO_MSG_ALG_REGISTER,
	CRYPTO_MSG_ALG_LOADED,
};

static inline void crypto_request_complete(struct crypto_async_request *req,
					   int err)
{
	req->complete(req->data, err);
}

static inline u32 crypto_tfm_alg_type(struct crypto_tfm *tfm)
{
	return tfm->__crt_alg->cra_flags & CRYPTO_ALG_TYPE_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif	/* _CRYPTO_ALGAPI_H */
