<<<<<<< HEAD
/*
 * Cryptographic API.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Cryptographic API.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/crypto.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <linux/lzo.h>
=======
#include <linux/mm.h>
#include <linux/lzo.h>
#include <crypto/internal/scompress.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct lzo_ctx {
	void *lzo_comp_mem;
};

<<<<<<< HEAD
=======
static void *lzo_alloc_ctx(struct crypto_scomp *tfm)
{
	void *ctx;

	ctx = kvmalloc(LZO1X_MEM_COMPRESS, GFP_KERNEL);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

	return ctx;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int lzo_init(struct crypto_tfm *tfm)
{
	struct lzo_ctx *ctx = crypto_tfm_ctx(tfm);

<<<<<<< HEAD
	ctx->lzo_comp_mem = vmalloc(LZO1X_MEM_COMPRESS);
	if (!ctx->lzo_comp_mem)
=======
	ctx->lzo_comp_mem = lzo_alloc_ctx(NULL);
	if (IS_ERR(ctx->lzo_comp_mem))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
=======
static void lzo_free_ctx(struct crypto_scomp *tfm, void *ctx)
{
	kvfree(ctx);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void lzo_exit(struct crypto_tfm *tfm)
{
	struct lzo_ctx *ctx = crypto_tfm_ctx(tfm);

<<<<<<< HEAD
	vfree(ctx->lzo_comp_mem);
}

static int lzo_compress(struct crypto_tfm *tfm, const u8 *src,
			    unsigned int slen, u8 *dst, unsigned int *dlen)
{
	struct lzo_ctx *ctx = crypto_tfm_ctx(tfm);
	size_t tmp_len = *dlen; /* size_t(ulong) <-> uint on 64 bit */
	int err;

	err = lzo1x_1_compress(src, slen, dst, &tmp_len, ctx->lzo_comp_mem);
=======
	lzo_free_ctx(NULL, ctx->lzo_comp_mem);
}

static int __lzo_compress(const u8 *src, unsigned int slen,
			  u8 *dst, unsigned int *dlen, void *ctx)
{
	size_t tmp_len = *dlen; /* size_t(ulong) <-> uint on 64 bit */
	int err;

	err = lzo1x_1_compress(src, slen, dst, &tmp_len, ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (err != LZO_E_OK)
		return -EINVAL;

	*dlen = tmp_len;
	return 0;
}

<<<<<<< HEAD
static int lzo_decompress(struct crypto_tfm *tfm, const u8 *src,
			      unsigned int slen, u8 *dst, unsigned int *dlen)
=======
static int lzo_compress(struct crypto_tfm *tfm, const u8 *src,
			unsigned int slen, u8 *dst, unsigned int *dlen)
{
	struct lzo_ctx *ctx = crypto_tfm_ctx(tfm);

	return __lzo_compress(src, slen, dst, dlen, ctx->lzo_comp_mem);
}

static int lzo_scompress(struct crypto_scomp *tfm, const u8 *src,
			 unsigned int slen, u8 *dst, unsigned int *dlen,
			 void *ctx)
{
	return __lzo_compress(src, slen, dst, dlen, ctx);
}

static int __lzo_decompress(const u8 *src, unsigned int slen,
			    u8 *dst, unsigned int *dlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	size_t tmp_len = *dlen; /* size_t(ulong) <-> uint on 64 bit */

	err = lzo1x_decompress_safe(src, slen, dst, &tmp_len);

	if (err != LZO_E_OK)
		return -EINVAL;

	*dlen = tmp_len;
	return 0;
<<<<<<< HEAD

=======
}

static int lzo_decompress(struct crypto_tfm *tfm, const u8 *src,
			  unsigned int slen, u8 *dst, unsigned int *dlen)
{
	return __lzo_decompress(src, slen, dst, dlen);
}

static int lzo_sdecompress(struct crypto_scomp *tfm, const u8 *src,
			   unsigned int slen, u8 *dst, unsigned int *dlen,
			   void *ctx)
{
	return __lzo_decompress(src, slen, dst, dlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct crypto_alg alg = {
	.cra_name		= "lzo",
<<<<<<< HEAD
	.cra_flags		= CRYPTO_ALG_TYPE_COMPRESS,
	.cra_ctxsize		= sizeof(struct lzo_ctx),
	.cra_module		= THIS_MODULE,
	.cra_list		= LIST_HEAD_INIT(alg.cra_list),
	.cra_init		= lzo_init,
	.cra_exit		= lzo_exit,
	.cra_u			= { .compress = {
	.coa_compress 		= lzo_compress,
	.coa_decompress  	= lzo_decompress } }
=======
	.cra_driver_name	= "lzo-generic",
	.cra_flags		= CRYPTO_ALG_TYPE_COMPRESS,
	.cra_ctxsize		= sizeof(struct lzo_ctx),
	.cra_module		= THIS_MODULE,
	.cra_init		= lzo_init,
	.cra_exit		= lzo_exit,
	.cra_u			= { .compress = {
	.coa_compress		= lzo_compress,
	.coa_decompress		= lzo_decompress } }
};

static struct scomp_alg scomp = {
	.alloc_ctx		= lzo_alloc_ctx,
	.free_ctx		= lzo_free_ctx,
	.compress		= lzo_scompress,
	.decompress		= lzo_sdecompress,
	.base			= {
		.cra_name	= "lzo",
		.cra_driver_name = "lzo-scomp",
		.cra_module	 = THIS_MODULE,
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init lzo_mod_init(void)
{
<<<<<<< HEAD
	return crypto_register_alg(&alg);
=======
	int ret;

	ret = crypto_register_alg(&alg);
	if (ret)
		return ret;

	ret = crypto_register_scomp(&scomp);
	if (ret) {
		crypto_unregister_alg(&alg);
		return ret;
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit lzo_mod_fini(void)
{
	crypto_unregister_alg(&alg);
<<<<<<< HEAD
}

module_init(lzo_mod_init);
=======
	crypto_unregister_scomp(&scomp);
}

subsys_initcall(lzo_mod_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(lzo_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LZO Compression Algorithm");
<<<<<<< HEAD
=======
MODULE_ALIAS_CRYPTO("lzo");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
