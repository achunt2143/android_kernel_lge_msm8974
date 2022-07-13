<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * Support for OMAP AES HW acceleration.
 *
 * Copyright (c) 2010 Nokia Corporation
 * Author: Dmitry Kasatkin <dmitry.kasatkin@nokia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/err.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/crypto.h>
#include <linux/interrupt.h>
#include <crypto/scatterwalk.h>
#include <crypto/aes.h>

#include <plat/cpu.h>
#include <plat/dma.h>

/* OMAP TRM gives bitfields as start:end, where start is the higher bit
   number. For example 7:0 */
#define FLD_MASK(start, end)	(((1 << ((start) - (end) + 1)) - 1) << (end))
#define FLD_VAL(val, start, end) (((val) << (end)) & FLD_MASK(start, end))

#define AES_REG_KEY(x)			(0x1C - ((x ^ 0x01) * 0x04))
#define AES_REG_IV(x)			(0x20 + ((x) * 0x04))

#define AES_REG_CTRL			0x30
#define AES_REG_CTRL_CTR_WIDTH		(1 << 7)
#define AES_REG_CTRL_CTR		(1 << 6)
#define AES_REG_CTRL_CBC		(1 << 5)
#define AES_REG_CTRL_KEY_SIZE		(3 << 3)
#define AES_REG_CTRL_DIRECTION		(1 << 2)
#define AES_REG_CTRL_INPUT_READY	(1 << 1)
#define AES_REG_CTRL_OUTPUT_READY	(1 << 0)

#define AES_REG_DATA			0x34
#define AES_REG_DATA_N(x)		(0x34 + ((x) * 0x04))

#define AES_REG_REV			0x44
#define AES_REG_REV_MAJOR		0xF0
#define AES_REG_REV_MINOR		0x0F

#define AES_REG_MASK			0x48
#define AES_REG_MASK_SIDLE		(1 << 6)
#define AES_REG_MASK_START		(1 << 5)
#define AES_REG_MASK_DMA_OUT_EN		(1 << 3)
#define AES_REG_MASK_DMA_IN_EN		(1 << 2)
#define AES_REG_MASK_SOFTRESET		(1 << 1)
#define AES_REG_AUTOIDLE		(1 << 0)

#define AES_REG_SYSSTATUS		0x4C
#define AES_REG_SYSSTATUS_RESETDONE	(1 << 0)

#define DEFAULT_TIMEOUT		(5*HZ)

#define FLAGS_MODE_MASK		0x000f
#define FLAGS_ENCRYPT		BIT(0)
#define FLAGS_CBC		BIT(1)
#define FLAGS_GIV		BIT(2)

#define FLAGS_INIT		BIT(4)
#define FLAGS_FAST		BIT(5)
#define FLAGS_BUSY		BIT(6)

struct omap_aes_ctx {
	struct omap_aes_dev *dd;

	int		keylen;
	u32		key[AES_KEYSIZE_256 / sizeof(u32)];
	unsigned long	flags;
};

struct omap_aes_reqctx {
	unsigned long mode;
};

#define OMAP_AES_QUEUE_LENGTH	1
#define OMAP_AES_CACHE_SIZE	0

struct omap_aes_dev {
	struct list_head	list;
	unsigned long		phys_base;
	void __iomem		*io_base;
	struct clk		*iclk;
	struct omap_aes_ctx	*ctx;
	struct device		*dev;
	unsigned long		flags;
	int			err;

	spinlock_t		lock;
	struct crypto_queue	queue;

	struct tasklet_struct	done_task;
	struct tasklet_struct	queue_task;

	struct ablkcipher_request	*req;
	size_t				total;
	struct scatterlist		*in_sg;
	size_t				in_offset;
	struct scatterlist		*out_sg;
	size_t				out_offset;

	size_t			buflen;
	void			*buf_in;
	size_t			dma_size;
	int			dma_in;
	int			dma_lch_in;
	dma_addr_t		dma_addr_in;
	void			*buf_out;
	int			dma_out;
	int			dma_lch_out;
	dma_addr_t		dma_addr_out;
};
=======
 * Copyright (c) 2011 Texas Instruments Incorporated
 */

#define pr_fmt(fmt) "%20s: " fmt, __func__
#define prn(num) pr_debug(#num "=%d\n", num)
#define prx(num) pr_debug(#num "=%x\n", num)

#include <crypto/aes.h>
#include <crypto/gcm.h>
#include <crypto/internal/aead.h>
#include <crypto/internal/engine.h>
#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/scatterlist.h>
#include <linux/string.h>

#include "omap-crypto.h"
#include "omap-aes.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* keep registered devices data here */
static LIST_HEAD(dev_list);
static DEFINE_SPINLOCK(list_lock);

<<<<<<< HEAD
static inline u32 omap_aes_read(struct omap_aes_dev *dd, u32 offset)
{
	return __raw_readl(dd->io_base + offset);
}

static inline void omap_aes_write(struct omap_aes_dev *dd, u32 offset,
=======
static int aes_fallback_sz = 200;

#ifdef DEBUG
#define omap_aes_read(dd, offset)				\
({								\
	int _read_ret;						\
	_read_ret = __raw_readl(dd->io_base + offset);		\
	pr_debug("omap_aes_read(" #offset "=%#x)= %#x\n",	\
		 offset, _read_ret);				\
	_read_ret;						\
})
#else
inline u32 omap_aes_read(struct omap_aes_dev *dd, u32 offset)
{
	return __raw_readl(dd->io_base + offset);
}
#endif

#ifdef DEBUG
#define omap_aes_write(dd, offset, value)				\
	do {								\
		pr_debug("omap_aes_write(" #offset "=%#x) value=%#x\n",	\
			 offset, value);				\
		__raw_writel(value, dd->io_base + offset);		\
	} while (0)
#else
inline void omap_aes_write(struct omap_aes_dev *dd, u32 offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  u32 value)
{
	__raw_writel(value, dd->io_base + offset);
}
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void omap_aes_write_mask(struct omap_aes_dev *dd, u32 offset,
					u32 value, u32 mask)
{
	u32 val;

	val = omap_aes_read(dd, offset);
	val &= ~mask;
	val |= value;
	omap_aes_write(dd, offset, val);
}

static void omap_aes_write_n(struct omap_aes_dev *dd, u32 offset,
					u32 *value, int count)
{
	for (; count--; value++, offset += 4)
		omap_aes_write(dd, offset, *value);
}

<<<<<<< HEAD
static int omap_aes_wait(struct omap_aes_dev *dd, u32 offset, u32 bit)
{
	unsigned long timeout = jiffies + DEFAULT_TIMEOUT;

	while (!(omap_aes_read(dd, offset) & bit)) {
		if (time_is_before_jiffies(timeout)) {
			dev_err(dd->dev, "omap-aes timeout\n");
			return -ETIMEDOUT;
		}
	}
	return 0;
}

static int omap_aes_hw_init(struct omap_aes_dev *dd)
{
	/*
	 * clocks are enabled when request starts and disabled when finished.
	 * It may be long delays between requests.
	 * Device might go to off mode to save power.
	 */
	clk_enable(dd->iclk);

	if (!(dd->flags & FLAGS_INIT)) {
		/* is it necessary to reset before every operation? */
		omap_aes_write_mask(dd, AES_REG_MASK, AES_REG_MASK_SOFTRESET,
					AES_REG_MASK_SOFTRESET);
		/*
		 * prevent OCP bus error (SRESP) in case an access to the module
		 * is performed while the module is coming out of soft reset
		 */
		__asm__ __volatile__("nop");
		__asm__ __volatile__("nop");

		if (omap_aes_wait(dd, AES_REG_SYSSTATUS,
				AES_REG_SYSSTATUS_RESETDONE))
			return -ETIMEDOUT;

=======
static int omap_aes_hw_init(struct omap_aes_dev *dd)
{
	int err;

	if (!(dd->flags & FLAGS_INIT)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dd->flags |= FLAGS_INIT;
		dd->err = 0;
	}

<<<<<<< HEAD
	return 0;
}

static int omap_aes_write_ctrl(struct omap_aes_dev *dd)
{
	unsigned int key32;
	int i, err;
	u32 val, mask;
=======
	err = pm_runtime_resume_and_get(dd->dev);
	if (err < 0) {
		dev_err(dd->dev, "failed to get sync: %d\n", err);
		return err;
	}

	return 0;
}

void omap_aes_clear_copy_flags(struct omap_aes_dev *dd)
{
	dd->flags &= ~(OMAP_CRYPTO_COPY_MASK << FLAGS_IN_DATA_ST_SHIFT);
	dd->flags &= ~(OMAP_CRYPTO_COPY_MASK << FLAGS_OUT_DATA_ST_SHIFT);
	dd->flags &= ~(OMAP_CRYPTO_COPY_MASK << FLAGS_ASSOC_DATA_ST_SHIFT);
}

int omap_aes_write_ctrl(struct omap_aes_dev *dd)
{
	struct omap_aes_reqctx *rctx;
	unsigned int key32;
	int i, err;
	u32 val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = omap_aes_hw_init(dd);
	if (err)
		return err;

<<<<<<< HEAD
	val = 0;
	if (dd->dma_lch_out >= 0)
		val |= AES_REG_MASK_DMA_OUT_EN;
	if (dd->dma_lch_in >= 0)
		val |= AES_REG_MASK_DMA_IN_EN;

	mask = AES_REG_MASK_DMA_IN_EN | AES_REG_MASK_DMA_OUT_EN;

	omap_aes_write_mask(dd, AES_REG_MASK, val, mask);

	key32 = dd->ctx->keylen / sizeof(u32);

	/* it seems a key should always be set even if it has not changed */
	for (i = 0; i < key32; i++) {
		omap_aes_write(dd, AES_REG_KEY(i),
			__le32_to_cpu(dd->ctx->key[i]));
	}

	if ((dd->flags & FLAGS_CBC) && dd->req->info)
		omap_aes_write_n(dd, AES_REG_IV(0), dd->req->info, 4);
=======
	key32 = dd->ctx->keylen / sizeof(u32);

	/* RESET the key as previous HASH keys should not get affected*/
	if (dd->flags & FLAGS_GCM)
		for (i = 0; i < 0x40; i = i + 4)
			omap_aes_write(dd, i, 0x0);

	for (i = 0; i < key32; i++) {
		omap_aes_write(dd, AES_REG_KEY(dd, i),
			       (__force u32)cpu_to_le32(dd->ctx->key[i]));
	}

	if ((dd->flags & (FLAGS_CBC | FLAGS_CTR)) && dd->req->iv)
		omap_aes_write_n(dd, AES_REG_IV(dd, 0), (void *)dd->req->iv, 4);

	if ((dd->flags & (FLAGS_GCM)) && dd->aead_req->iv) {
		rctx = aead_request_ctx(dd->aead_req);
		omap_aes_write_n(dd, AES_REG_IV(dd, 0), (u32 *)rctx->iv, 4);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	val = FLD_VAL(((dd->ctx->keylen >> 3) - 1), 4, 3);
	if (dd->flags & FLAGS_CBC)
		val |= AES_REG_CTRL_CBC;
<<<<<<< HEAD
	if (dd->flags & FLAGS_ENCRYPT)
		val |= AES_REG_CTRL_DIRECTION;

	mask = AES_REG_CTRL_CBC | AES_REG_CTRL_DIRECTION |
			AES_REG_CTRL_KEY_SIZE;

	omap_aes_write_mask(dd, AES_REG_CTRL, val, mask);

	/* IN */
	omap_set_dma_dest_params(dd->dma_lch_in, 0, OMAP_DMA_AMODE_CONSTANT,
				 dd->phys_base + AES_REG_DATA, 0, 4);

	omap_set_dma_dest_burst_mode(dd->dma_lch_in, OMAP_DMA_DATA_BURST_4);
	omap_set_dma_src_burst_mode(dd->dma_lch_in, OMAP_DMA_DATA_BURST_4);

	/* OUT */
	omap_set_dma_src_params(dd->dma_lch_out, 0, OMAP_DMA_AMODE_CONSTANT,
				dd->phys_base + AES_REG_DATA, 0, 4);

	omap_set_dma_src_burst_mode(dd->dma_lch_out, OMAP_DMA_DATA_BURST_4);
	omap_set_dma_dest_burst_mode(dd->dma_lch_out, OMAP_DMA_DATA_BURST_4);
=======

	if (dd->flags & (FLAGS_CTR | FLAGS_GCM))
		val |= AES_REG_CTRL_CTR | AES_REG_CTRL_CTR_WIDTH_128;

	if (dd->flags & FLAGS_GCM)
		val |= AES_REG_CTRL_GCM;

	if (dd->flags & FLAGS_ENCRYPT)
		val |= AES_REG_CTRL_DIRECTION;

	omap_aes_write_mask(dd, AES_REG_CTRL(dd), val, AES_REG_CTRL_MASK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct omap_aes_dev *omap_aes_find_dev(struct omap_aes_ctx *ctx)
{
	struct omap_aes_dev *dd = NULL, *tmp;

	spin_lock_bh(&list_lock);
	if (!ctx->dd) {
		list_for_each_entry(tmp, &dev_list, list) {
			/* FIXME: take fist available aes core */
			dd = tmp;
			break;
		}
		ctx->dd = dd;
	} else {
		/* already found before */
		dd = ctx->dd;
	}
=======
static void omap_aes_dma_trigger_omap2(struct omap_aes_dev *dd, int length)
{
	u32 mask, val;

	val = dd->pdata->dma_start;

	if (dd->dma_lch_out != NULL)
		val |= dd->pdata->dma_enable_out;
	if (dd->dma_lch_in != NULL)
		val |= dd->pdata->dma_enable_in;

	mask = dd->pdata->dma_enable_out | dd->pdata->dma_enable_in |
	       dd->pdata->dma_start;

	omap_aes_write_mask(dd, AES_REG_MASK(dd), val, mask);

}

static void omap_aes_dma_trigger_omap4(struct omap_aes_dev *dd, int length)
{
	omap_aes_write(dd, AES_REG_LENGTH_N(0), length);
	omap_aes_write(dd, AES_REG_LENGTH_N(1), 0);
	if (dd->flags & FLAGS_GCM)
		omap_aes_write(dd, AES_REG_A_LEN, dd->assoc_len);

	omap_aes_dma_trigger_omap2(dd, length);
}

static void omap_aes_dma_stop(struct omap_aes_dev *dd)
{
	u32 mask;

	mask = dd->pdata->dma_enable_out | dd->pdata->dma_enable_in |
	       dd->pdata->dma_start;

	omap_aes_write_mask(dd, AES_REG_MASK(dd), 0, mask);
}

struct omap_aes_dev *omap_aes_find_dev(struct omap_aes_reqctx *rctx)
{
	struct omap_aes_dev *dd;

	spin_lock_bh(&list_lock);
	dd = list_first_entry(&dev_list, struct omap_aes_dev, list);
	list_move_tail(&dd->list, &dev_list);
	rctx->dd = dd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_bh(&list_lock);

	return dd;
}

<<<<<<< HEAD
static void omap_aes_dma_callback(int lch, u16 ch_status, void *data)
{
	struct omap_aes_dev *dd = data;

	if (ch_status != OMAP_DMA_BLOCK_IRQ) {
		pr_err("omap-aes DMA error status: 0x%hx\n", ch_status);
		dd->err = -EIO;
		dd->flags &= ~FLAGS_INIT; /* request to re-initialize */
	} else if (lch == dd->dma_lch_in) {
		return;
	}

=======
static void omap_aes_dma_out_callback(void *data)
{
	struct omap_aes_dev *dd = data;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* dma_lch_out - completed */
	tasklet_schedule(&dd->done_task);
}

static int omap_aes_dma_init(struct omap_aes_dev *dd)
{
<<<<<<< HEAD
	int err = -ENOMEM;

	dd->dma_lch_out = -1;
	dd->dma_lch_in = -1;

	dd->buf_in = (void *)__get_free_pages(GFP_KERNEL, OMAP_AES_CACHE_SIZE);
	dd->buf_out = (void *)__get_free_pages(GFP_KERNEL, OMAP_AES_CACHE_SIZE);
	dd->buflen = PAGE_SIZE << OMAP_AES_CACHE_SIZE;
	dd->buflen &= ~(AES_BLOCK_SIZE - 1);

	if (!dd->buf_in || !dd->buf_out) {
		dev_err(dd->dev, "unable to alloc pages.\n");
		goto err_alloc;
	}

	/* MAP here */
	dd->dma_addr_in = dma_map_single(dd->dev, dd->buf_in, dd->buflen,
					 DMA_TO_DEVICE);
	if (dma_mapping_error(dd->dev, dd->dma_addr_in)) {
		dev_err(dd->dev, "dma %d bytes error\n", dd->buflen);
		err = -EINVAL;
		goto err_map_in;
	}

	dd->dma_addr_out = dma_map_single(dd->dev, dd->buf_out, dd->buflen,
					  DMA_FROM_DEVICE);
	if (dma_mapping_error(dd->dev, dd->dma_addr_out)) {
		dev_err(dd->dev, "dma %d bytes error\n", dd->buflen);
		err = -EINVAL;
		goto err_map_out;
	}

	err = omap_request_dma(dd->dma_in, "omap-aes-rx",
			       omap_aes_dma_callback, dd, &dd->dma_lch_in);
	if (err) {
		dev_err(dd->dev, "Unable to request DMA channel\n");
		goto err_dma_in;
	}
	err = omap_request_dma(dd->dma_out, "omap-aes-tx",
			       omap_aes_dma_callback, dd, &dd->dma_lch_out);
	if (err) {
		dev_err(dd->dev, "Unable to request DMA channel\n");
=======
	int err;

	dd->dma_lch_out = NULL;
	dd->dma_lch_in = NULL;

	dd->dma_lch_in = dma_request_chan(dd->dev, "rx");
	if (IS_ERR(dd->dma_lch_in)) {
		dev_err(dd->dev, "Unable to request in DMA channel\n");
		return PTR_ERR(dd->dma_lch_in);
	}

	dd->dma_lch_out = dma_request_chan(dd->dev, "tx");
	if (IS_ERR(dd->dma_lch_out)) {
		dev_err(dd->dev, "Unable to request out DMA channel\n");
		err = PTR_ERR(dd->dma_lch_out);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_dma_out;
	}

	return 0;

err_dma_out:
<<<<<<< HEAD
	omap_free_dma(dd->dma_lch_in);
err_dma_in:
	dma_unmap_single(dd->dev, dd->dma_addr_out, dd->buflen,
			 DMA_FROM_DEVICE);
err_map_out:
	dma_unmap_single(dd->dev, dd->dma_addr_in, dd->buflen, DMA_TO_DEVICE);
err_map_in:
	free_pages((unsigned long)dd->buf_out, OMAP_AES_CACHE_SIZE);
	free_pages((unsigned long)dd->buf_in, OMAP_AES_CACHE_SIZE);
err_alloc:
	if (err)
		pr_err("error: %d\n", err);
=======
	dma_release_channel(dd->dma_lch_in);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void omap_aes_dma_cleanup(struct omap_aes_dev *dd)
{
<<<<<<< HEAD
	omap_free_dma(dd->dma_lch_out);
	omap_free_dma(dd->dma_lch_in);
	dma_unmap_single(dd->dev, dd->dma_addr_out, dd->buflen,
			 DMA_FROM_DEVICE);
	dma_unmap_single(dd->dev, dd->dma_addr_in, dd->buflen, DMA_TO_DEVICE);
	free_pages((unsigned long)dd->buf_out, OMAP_AES_CACHE_SIZE);
	free_pages((unsigned long)dd->buf_in, OMAP_AES_CACHE_SIZE);
}

static void sg_copy_buf(void *buf, struct scatterlist *sg,
			      unsigned int start, unsigned int nbytes, int out)
{
	struct scatter_walk walk;

	if (!nbytes)
		return;

	scatterwalk_start(&walk, sg);
	scatterwalk_advance(&walk, start);
	scatterwalk_copychunks(buf, &walk, nbytes, out);
	scatterwalk_done(&walk, out, 0);
}

static int sg_copy(struct scatterlist **sg, size_t *offset, void *buf,
		   size_t buflen, size_t total, int out)
{
	unsigned int count, off = 0;

	while (buflen && total) {
		count = min((*sg)->length - *offset, total);
		count = min(count, buflen);

		if (!count)
			return off;

		/*
		 * buflen and total are AES_BLOCK_SIZE size aligned,
		 * so count should be also aligned
		 */

		sg_copy_buf(buf + off, *sg, *offset, count, out);

		off += count;
		buflen -= count;
		*offset += count;
		total -= count;

		if (*offset == (*sg)->length) {
			*sg = sg_next(*sg);
			if (*sg)
				*offset = 0;
			else
				total = 0;
		}
	}

	return off;
}

static int omap_aes_crypt_dma(struct crypto_tfm *tfm, dma_addr_t dma_addr_in,
			       dma_addr_t dma_addr_out, int length)
{
	struct omap_aes_ctx *ctx = crypto_tfm_ctx(tfm);
	struct omap_aes_dev *dd = ctx->dd;
	int len32;

	pr_debug("len: %d\n", length);

	dd->dma_size = length;

	if (!(dd->flags & FLAGS_FAST))
		dma_sync_single_for_device(dd->dev, dma_addr_in, length,
					   DMA_TO_DEVICE);

	len32 = DIV_ROUND_UP(length, sizeof(u32));

	/* IN */
	omap_set_dma_transfer_params(dd->dma_lch_in, OMAP_DMA_DATA_TYPE_S32,
				     len32, 1, OMAP_DMA_SYNC_PACKET, dd->dma_in,
					OMAP_DMA_DST_SYNC);

	omap_set_dma_src_params(dd->dma_lch_in, 0, OMAP_DMA_AMODE_POST_INC,
				dma_addr_in, 0, 0);

	/* OUT */
	omap_set_dma_transfer_params(dd->dma_lch_out, OMAP_DMA_DATA_TYPE_S32,
				     len32, 1, OMAP_DMA_SYNC_PACKET,
					dd->dma_out, OMAP_DMA_SRC_SYNC);

	omap_set_dma_dest_params(dd->dma_lch_out, 0, OMAP_DMA_AMODE_POST_INC,
				 dma_addr_out, 0, 0);

	omap_start_dma(dd->dma_lch_in);
	omap_start_dma(dd->dma_lch_out);

	/* start DMA or disable idle mode */
	omap_aes_write_mask(dd, AES_REG_MASK, AES_REG_MASK_START,
			    AES_REG_MASK_START);
=======
	if (dd->pio_only)
		return;

	dma_release_channel(dd->dma_lch_out);
	dma_release_channel(dd->dma_lch_in);
}

static int omap_aes_crypt_dma(struct omap_aes_dev *dd,
			      struct scatterlist *in_sg,
			      struct scatterlist *out_sg,
			      int in_sg_len, int out_sg_len)
{
	struct dma_async_tx_descriptor *tx_in, *tx_out = NULL, *cb_desc;
	struct dma_slave_config cfg;
	int ret;

	if (dd->pio_only) {
		scatterwalk_start(&dd->in_walk, dd->in_sg);
		if (out_sg_len)
			scatterwalk_start(&dd->out_walk, dd->out_sg);

		/* Enable DATAIN interrupt and let it take
		   care of the rest */
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x2);
		return 0;
	}

	dma_sync_sg_for_device(dd->dev, dd->in_sg, in_sg_len, DMA_TO_DEVICE);

	memset(&cfg, 0, sizeof(cfg));

	cfg.src_addr = dd->phys_base + AES_REG_DATA_N(dd, 0);
	cfg.dst_addr = dd->phys_base + AES_REG_DATA_N(dd, 0);
	cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	cfg.src_maxburst = DST_MAXBURST;
	cfg.dst_maxburst = DST_MAXBURST;

	/* IN */
	ret = dmaengine_slave_config(dd->dma_lch_in, &cfg);
	if (ret) {
		dev_err(dd->dev, "can't configure IN dmaengine slave: %d\n",
			ret);
		return ret;
	}

	tx_in = dmaengine_prep_slave_sg(dd->dma_lch_in, in_sg, in_sg_len,
					DMA_MEM_TO_DEV,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!tx_in) {
		dev_err(dd->dev, "IN prep_slave_sg() failed\n");
		return -EINVAL;
	}

	/* No callback necessary */
	tx_in->callback_param = dd;
	tx_in->callback = NULL;

	/* OUT */
	if (out_sg_len) {
		ret = dmaengine_slave_config(dd->dma_lch_out, &cfg);
		if (ret) {
			dev_err(dd->dev, "can't configure OUT dmaengine slave: %d\n",
				ret);
			return ret;
		}

		tx_out = dmaengine_prep_slave_sg(dd->dma_lch_out, out_sg,
						 out_sg_len,
						 DMA_DEV_TO_MEM,
						 DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
		if (!tx_out) {
			dev_err(dd->dev, "OUT prep_slave_sg() failed\n");
			return -EINVAL;
		}

		cb_desc = tx_out;
	} else {
		cb_desc = tx_in;
	}

	if (dd->flags & FLAGS_GCM)
		cb_desc->callback = omap_aes_gcm_dma_out_callback;
	else
		cb_desc->callback = omap_aes_dma_out_callback;
	cb_desc->callback_param = dd;


	dmaengine_submit(tx_in);
	if (tx_out)
		dmaengine_submit(tx_out);

	dma_async_issue_pending(dd->dma_lch_in);
	if (out_sg_len)
		dma_async_issue_pending(dd->dma_lch_out);

	/* start DMA */
	dd->pdata->trigger(dd, dd->total);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int omap_aes_crypt_dma_start(struct omap_aes_dev *dd)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(
					crypto_ablkcipher_reqtfm(dd->req));
	int err, fast = 0, in, out;
	size_t count;
	dma_addr_t addr_in, addr_out;

	pr_debug("total: %d\n", dd->total);

	if (sg_is_last(dd->in_sg) && sg_is_last(dd->out_sg)) {
		/* check for alignment */
		in = IS_ALIGNED((u32)dd->in_sg->offset, sizeof(u32));
		out = IS_ALIGNED((u32)dd->out_sg->offset, sizeof(u32));

		fast = in && out;
	}

	if (fast)  {
		count = min(dd->total, sg_dma_len(dd->in_sg));
		count = min(count, sg_dma_len(dd->out_sg));

		if (count != dd->total) {
			pr_err("request length != buffer length\n");
			return -EINVAL;
		}

		pr_debug("fast\n");

		err = dma_map_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
=======
int omap_aes_crypt_dma_start(struct omap_aes_dev *dd)
{
	int err;

	pr_debug("total: %zu\n", dd->total);

	if (!dd->pio_only) {
		err = dma_map_sg(dd->dev, dd->in_sg, dd->in_sg_len,
				 DMA_TO_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!err) {
			dev_err(dd->dev, "dma_map_sg() error\n");
			return -EINVAL;
		}

<<<<<<< HEAD
		err = dma_map_sg(dd->dev, dd->out_sg, 1, DMA_FROM_DEVICE);
		if (!err) {
			dev_err(dd->dev, "dma_map_sg() error\n");
			dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
			return -EINVAL;
		}

		addr_in = sg_dma_address(dd->in_sg);
		addr_out = sg_dma_address(dd->out_sg);

		dd->flags |= FLAGS_FAST;

	} else {
		/* use cache buffers */
		count = sg_copy(&dd->in_sg, &dd->in_offset, dd->buf_in,
				 dd->buflen, dd->total, 0);

		addr_in = dd->dma_addr_in;
		addr_out = dd->dma_addr_out;

		dd->flags &= ~FLAGS_FAST;

	}

	dd->total -= count;

	err = omap_aes_crypt_dma(tfm, addr_in, addr_out, count);
	if (err) {
		dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
		dma_unmap_sg(dd->dev, dd->out_sg, 1, DMA_TO_DEVICE);
=======
		if (dd->out_sg_len) {
			err = dma_map_sg(dd->dev, dd->out_sg, dd->out_sg_len,
					 DMA_FROM_DEVICE);
			if (!err) {
				dev_err(dd->dev, "dma_map_sg() error\n");
				return -EINVAL;
			}
		}
	}

	err = omap_aes_crypt_dma(dd, dd->in_sg, dd->out_sg, dd->in_sg_len,
				 dd->out_sg_len);
	if (err && !dd->pio_only) {
		dma_unmap_sg(dd->dev, dd->in_sg, dd->in_sg_len, DMA_TO_DEVICE);
		if (dd->out_sg_len)
			dma_unmap_sg(dd->dev, dd->out_sg, dd->out_sg_len,
				     DMA_FROM_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

static void omap_aes_finish_req(struct omap_aes_dev *dd, int err)
{
<<<<<<< HEAD
	struct ablkcipher_request *req = dd->req;

	pr_debug("err: %d\n", err);

	clk_disable(dd->iclk);
	dd->flags &= ~FLAGS_BUSY;

	req->base.complete(&req->base, err);
}

static int omap_aes_crypt_dma_stop(struct omap_aes_dev *dd)
{
	int err = 0;
	size_t count;

	pr_debug("total: %d\n", dd->total);

	omap_aes_write_mask(dd, AES_REG_MASK, 0, AES_REG_MASK_START);

	omap_stop_dma(dd->dma_lch_in);
	omap_stop_dma(dd->dma_lch_out);

	if (dd->flags & FLAGS_FAST) {
		dma_unmap_sg(dd->dev, dd->out_sg, 1, DMA_FROM_DEVICE);
		dma_unmap_sg(dd->dev, dd->in_sg, 1, DMA_TO_DEVICE);
	} else {
		dma_sync_single_for_device(dd->dev, dd->dma_addr_out,
					   dd->dma_size, DMA_FROM_DEVICE);

		/* copy data */
		count = sg_copy(&dd->out_sg, &dd->out_offset, dd->buf_out,
				 dd->buflen, dd->dma_size, 1);
		if (count != dd->dma_size) {
			err = -EINVAL;
			pr_err("not all data converted: %u\n", count);
		}
	}

	return err;
}

static int omap_aes_handle_queue(struct omap_aes_dev *dd,
			       struct ablkcipher_request *req)
{
	struct crypto_async_request *async_req, *backlog;
	struct omap_aes_ctx *ctx;
	struct omap_aes_reqctx *rctx;
	unsigned long flags;
	int err, ret = 0;

	spin_lock_irqsave(&dd->lock, flags);
	if (req)
		ret = ablkcipher_enqueue_request(&dd->queue, req);
	if (dd->flags & FLAGS_BUSY) {
		spin_unlock_irqrestore(&dd->lock, flags);
		return ret;
	}
	backlog = crypto_get_backlog(&dd->queue);
	async_req = crypto_dequeue_request(&dd->queue);
	if (async_req)
		dd->flags |= FLAGS_BUSY;
	spin_unlock_irqrestore(&dd->lock, flags);

	if (!async_req)
		return ret;

	if (backlog)
		backlog->complete(backlog, -EINPROGRESS);

	req = ablkcipher_request_cast(async_req);

	/* assign new request to device */
	dd->req = req;
	dd->total = req->nbytes;
	dd->in_offset = 0;
	dd->in_sg = req->src;
	dd->out_offset = 0;
	dd->out_sg = req->dst;

	rctx = ablkcipher_request_ctx(req);
	ctx = crypto_ablkcipher_ctx(crypto_ablkcipher_reqtfm(req));
=======
	struct skcipher_request *req = dd->req;

	pr_debug("err: %d\n", err);

	crypto_finalize_skcipher_request(dd->engine, req, err);

	pm_runtime_mark_last_busy(dd->dev);
	pm_runtime_put_autosuspend(dd->dev);
}

int omap_aes_crypt_dma_stop(struct omap_aes_dev *dd)
{
	pr_debug("total: %zu\n", dd->total);

	omap_aes_dma_stop(dd);


	return 0;
}

static int omap_aes_handle_queue(struct omap_aes_dev *dd,
				 struct skcipher_request *req)
{
	if (req)
		return crypto_transfer_skcipher_request_to_engine(dd->engine, req);

	return 0;
}

static int omap_aes_prepare_req(struct skcipher_request *req,
				struct omap_aes_dev *dd)
{
	struct omap_aes_ctx *ctx = crypto_skcipher_ctx(
			crypto_skcipher_reqtfm(req));
	struct omap_aes_reqctx *rctx = skcipher_request_ctx(req);
	int ret;
	u16 flags;

	/* assign new request to device */
	dd->req = req;
	dd->total = req->cryptlen;
	dd->total_save = req->cryptlen;
	dd->in_sg = req->src;
	dd->out_sg = req->dst;
	dd->orig_out = req->dst;

	flags = OMAP_CRYPTO_COPY_DATA;
	if (req->src == req->dst)
		flags |= OMAP_CRYPTO_FORCE_COPY;

	ret = omap_crypto_align_sg(&dd->in_sg, dd->total, AES_BLOCK_SIZE,
				   dd->in_sgl, flags,
				   FLAGS_IN_DATA_ST_SHIFT, &dd->flags);
	if (ret)
		return ret;

	ret = omap_crypto_align_sg(&dd->out_sg, dd->total, AES_BLOCK_SIZE,
				   &dd->out_sgl, 0,
				   FLAGS_OUT_DATA_ST_SHIFT, &dd->flags);
	if (ret)
		return ret;

	dd->in_sg_len = sg_nents_for_len(dd->in_sg, dd->total);
	if (dd->in_sg_len < 0)
		return dd->in_sg_len;

	dd->out_sg_len = sg_nents_for_len(dd->out_sg, dd->total);
	if (dd->out_sg_len < 0)
		return dd->out_sg_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rctx->mode &= FLAGS_MODE_MASK;
	dd->flags = (dd->flags & ~FLAGS_MODE_MASK) | rctx->mode;

	dd->ctx = ctx;
<<<<<<< HEAD
	ctx->dd = dd;

	err = omap_aes_write_ctrl(dd);
	if (!err)
		err = omap_aes_crypt_dma_start(dd);
	if (err) {
		/* aes_task will not finish it, so do it here */
		omap_aes_finish_req(dd, err);
		tasklet_schedule(&dd->queue_task);
	}

	return ret; /* return ret, which is enqueue return value */
=======
	rctx->dd = dd;

	return omap_aes_write_ctrl(dd);
}

static int omap_aes_crypt_req(struct crypto_engine *engine,
			      void *areq)
{
	struct skcipher_request *req = container_of(areq, struct skcipher_request, base);
	struct omap_aes_reqctx *rctx = skcipher_request_ctx(req);
	struct omap_aes_dev *dd = rctx->dd;

	if (!dd)
		return -ENODEV;

	return omap_aes_prepare_req(req, dd) ?:
	       omap_aes_crypt_dma_start(dd);
}

static void omap_aes_copy_ivout(struct omap_aes_dev *dd, u8 *ivbuf)
{
	int i;

	for (i = 0; i < 4; i++)
		((u32 *)ivbuf)[i] = omap_aes_read(dd, AES_REG_IV(dd, i));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void omap_aes_done_task(unsigned long data)
{
	struct omap_aes_dev *dd = (struct omap_aes_dev *)data;
<<<<<<< HEAD
	int err;

	pr_debug("enter\n");

	err = omap_aes_crypt_dma_stop(dd);

	err = dd->err ? : err;

	if (dd->total && !err) {
		err = omap_aes_crypt_dma_start(dd);
		if (!err)
			return; /* DMA started. Not fininishing. */
	}

	omap_aes_finish_req(dd, err);
	omap_aes_handle_queue(dd, NULL);
=======

	pr_debug("enter done_task\n");

	if (!dd->pio_only) {
		dma_sync_sg_for_device(dd->dev, dd->out_sg, dd->out_sg_len,
				       DMA_FROM_DEVICE);
		dma_unmap_sg(dd->dev, dd->in_sg, dd->in_sg_len, DMA_TO_DEVICE);
		dma_unmap_sg(dd->dev, dd->out_sg, dd->out_sg_len,
			     DMA_FROM_DEVICE);
		omap_aes_crypt_dma_stop(dd);
	}

	omap_crypto_cleanup(dd->in_sg, NULL, 0, dd->total_save,
			    FLAGS_IN_DATA_ST_SHIFT, dd->flags);

	omap_crypto_cleanup(dd->out_sg, dd->orig_out, 0, dd->total_save,
			    FLAGS_OUT_DATA_ST_SHIFT, dd->flags);

	/* Update IV output */
	if (dd->flags & (FLAGS_CBC | FLAGS_CTR))
		omap_aes_copy_ivout(dd, dd->req->iv);

	omap_aes_finish_req(dd, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("exit\n");
}

<<<<<<< HEAD
static void omap_aes_queue_task(unsigned long data)
{
	struct omap_aes_dev *dd = (struct omap_aes_dev *)data;

	omap_aes_handle_queue(dd, NULL);
}

static int omap_aes_crypt(struct ablkcipher_request *req, unsigned long mode)
{
	struct omap_aes_ctx *ctx = crypto_ablkcipher_ctx(
			crypto_ablkcipher_reqtfm(req));
	struct omap_aes_reqctx *rctx = ablkcipher_request_ctx(req);
	struct omap_aes_dev *dd;

	pr_debug("nbytes: %d, enc: %d, cbc: %d\n", req->nbytes,
		  !!(mode & FLAGS_ENCRYPT),
		  !!(mode & FLAGS_CBC));

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks\n");
		return -EINVAL;
	}

	dd = omap_aes_find_dev(ctx);
=======
static int omap_aes_crypt(struct skcipher_request *req, unsigned long mode)
{
	struct omap_aes_ctx *ctx = crypto_skcipher_ctx(
			crypto_skcipher_reqtfm(req));
	struct omap_aes_reqctx *rctx = skcipher_request_ctx(req);
	struct omap_aes_dev *dd;
	int ret;

	if ((req->cryptlen % AES_BLOCK_SIZE) && !(mode & FLAGS_CTR))
		return -EINVAL;

	pr_debug("nbytes: %d, enc: %d, cbc: %d\n", req->cryptlen,
		  !!(mode & FLAGS_ENCRYPT),
		  !!(mode & FLAGS_CBC));

	if (req->cryptlen < aes_fallback_sz) {
		skcipher_request_set_tfm(&rctx->fallback_req, ctx->fallback);
		skcipher_request_set_callback(&rctx->fallback_req,
					      req->base.flags,
					      req->base.complete,
					      req->base.data);
		skcipher_request_set_crypt(&rctx->fallback_req, req->src,
					   req->dst, req->cryptlen, req->iv);

		if (mode & FLAGS_ENCRYPT)
			ret = crypto_skcipher_encrypt(&rctx->fallback_req);
		else
			ret = crypto_skcipher_decrypt(&rctx->fallback_req);
		return ret;
	}
	dd = omap_aes_find_dev(rctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dd)
		return -ENODEV;

	rctx->mode = mode;

	return omap_aes_handle_queue(dd, req);
}

/* ********************** ALG API ************************************ */

<<<<<<< HEAD
static int omap_aes_setkey(struct crypto_ablkcipher *tfm, const u8 *key,
			   unsigned int keylen)
{
	struct omap_aes_ctx *ctx = crypto_ablkcipher_ctx(tfm);
=======
static int omap_aes_setkey(struct crypto_skcipher *tfm, const u8 *key,
			   unsigned int keylen)
{
	struct omap_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (keylen != AES_KEYSIZE_128 && keylen != AES_KEYSIZE_192 &&
		   keylen != AES_KEYSIZE_256)
		return -EINVAL;

	pr_debug("enter, keylen: %d\n", keylen);

	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;

<<<<<<< HEAD
	return 0;
}

static int omap_aes_ecb_encrypt(struct ablkcipher_request *req)
=======
	crypto_skcipher_clear_flags(ctx->fallback, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(ctx->fallback, tfm->base.crt_flags &
						 CRYPTO_TFM_REQ_MASK);

	ret = crypto_skcipher_setkey(ctx->fallback, key, keylen);
	if (!ret)
		return 0;

	return 0;
}

static int omap_aes_ecb_encrypt(struct skcipher_request *req)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT);
}

<<<<<<< HEAD
static int omap_aes_ecb_decrypt(struct ablkcipher_request *req)
=======
static int omap_aes_ecb_decrypt(struct skcipher_request *req)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return omap_aes_crypt(req, 0);
}

<<<<<<< HEAD
static int omap_aes_cbc_encrypt(struct ablkcipher_request *req)
=======
static int omap_aes_cbc_encrypt(struct skcipher_request *req)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT | FLAGS_CBC);
}

<<<<<<< HEAD
static int omap_aes_cbc_decrypt(struct ablkcipher_request *req)
=======
static int omap_aes_cbc_decrypt(struct skcipher_request *req)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return omap_aes_crypt(req, FLAGS_CBC);
}

<<<<<<< HEAD
static int omap_aes_cra_init(struct crypto_tfm *tfm)
{
	pr_debug("enter\n");

	tfm->crt_ablkcipher.reqsize = sizeof(struct omap_aes_reqctx);
=======
static int omap_aes_ctr_encrypt(struct skcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_ENCRYPT | FLAGS_CTR);
}

static int omap_aes_ctr_decrypt(struct skcipher_request *req)
{
	return omap_aes_crypt(req, FLAGS_CTR);
}

static int omap_aes_init_tfm(struct crypto_skcipher *tfm)
{
	const char *name = crypto_tfm_alg_name(&tfm->base);
	struct omap_aes_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *blk;

	blk = crypto_alloc_skcipher(name, 0, CRYPTO_ALG_NEED_FALLBACK);
	if (IS_ERR(blk))
		return PTR_ERR(blk);

	ctx->fallback = blk;

	crypto_skcipher_set_reqsize(tfm, sizeof(struct omap_aes_reqctx) +
					 crypto_skcipher_reqsize(blk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void omap_aes_cra_exit(struct crypto_tfm *tfm)
{
	pr_debug("enter\n");
=======
static void omap_aes_exit_tfm(struct crypto_skcipher *tfm)
{
	struct omap_aes_ctx *ctx = crypto_skcipher_ctx(tfm);

	if (ctx->fallback)
		crypto_free_skcipher(ctx->fallback);

	ctx->fallback = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* ********************** ALGS ************************************ */

<<<<<<< HEAD
static struct crypto_alg algs[] = {
{
	.cra_name		= "ecb(aes)",
	.cra_driver_name	= "ecb-aes-omap",
	.cra_priority		= 100,
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct omap_aes_ctx),
	.cra_alignmask		= 0,
	.cra_type		= &crypto_ablkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_init		= omap_aes_cra_init,
	.cra_exit		= omap_aes_cra_exit,
	.cra_u.ablkcipher = {
		.min_keysize	= AES_MIN_KEY_SIZE,
		.max_keysize	= AES_MAX_KEY_SIZE,
		.setkey		= omap_aes_setkey,
		.encrypt	= omap_aes_ecb_encrypt,
		.decrypt	= omap_aes_ecb_decrypt,
	}
},
{
	.cra_name		= "cbc(aes)",
	.cra_driver_name	= "cbc-aes-omap",
	.cra_priority		= 100,
	.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER |
				  CRYPTO_ALG_KERN_DRIVER_ONLY |
				  CRYPTO_ALG_ASYNC,
	.cra_blocksize		= AES_BLOCK_SIZE,
	.cra_ctxsize		= sizeof(struct omap_aes_ctx),
	.cra_alignmask		= 0,
	.cra_type		= &crypto_ablkcipher_type,
	.cra_module		= THIS_MODULE,
	.cra_init		= omap_aes_cra_init,
	.cra_exit		= omap_aes_cra_exit,
	.cra_u.ablkcipher = {
		.min_keysize	= AES_MIN_KEY_SIZE,
		.max_keysize	= AES_MAX_KEY_SIZE,
		.ivsize		= AES_BLOCK_SIZE,
		.setkey		= omap_aes_setkey,
		.encrypt	= omap_aes_cbc_encrypt,
		.decrypt	= omap_aes_cbc_decrypt,
	}
}
};

=======
static struct skcipher_engine_alg algs_ecb_cbc[] = {
{
	.base = {
		.base.cra_name		= "ecb(aes)",
		.base.cra_driver_name	= "ecb-aes-omap",
		.base.cra_priority	= 300,
		.base.cra_flags		= CRYPTO_ALG_KERN_DRIVER_ONLY |
					  CRYPTO_ALG_ASYNC |
					  CRYPTO_ALG_NEED_FALLBACK,
		.base.cra_blocksize	= AES_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct omap_aes_ctx),
		.base.cra_module	= THIS_MODULE,

		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.setkey			= omap_aes_setkey,
		.encrypt		= omap_aes_ecb_encrypt,
		.decrypt		= omap_aes_ecb_decrypt,
		.init			= omap_aes_init_tfm,
		.exit			= omap_aes_exit_tfm,
	},
	.op.do_one_request = omap_aes_crypt_req,
},
{
	.base = {
		.base.cra_name		= "cbc(aes)",
		.base.cra_driver_name	= "cbc-aes-omap",
		.base.cra_priority	= 300,
		.base.cra_flags		= CRYPTO_ALG_KERN_DRIVER_ONLY |
					  CRYPTO_ALG_ASYNC |
					  CRYPTO_ALG_NEED_FALLBACK,
		.base.cra_blocksize	= AES_BLOCK_SIZE,
		.base.cra_ctxsize	= sizeof(struct omap_aes_ctx),
		.base.cra_module	= THIS_MODULE,

		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.ivsize			= AES_BLOCK_SIZE,
		.setkey			= omap_aes_setkey,
		.encrypt		= omap_aes_cbc_encrypt,
		.decrypt		= omap_aes_cbc_decrypt,
		.init			= omap_aes_init_tfm,
		.exit			= omap_aes_exit_tfm,
	},
	.op.do_one_request = omap_aes_crypt_req,
}
};

static struct skcipher_engine_alg algs_ctr[] = {
{
	.base = {
		.base.cra_name		= "ctr(aes)",
		.base.cra_driver_name	= "ctr-aes-omap",
		.base.cra_priority	= 300,
		.base.cra_flags		= CRYPTO_ALG_KERN_DRIVER_ONLY |
					  CRYPTO_ALG_ASYNC |
					  CRYPTO_ALG_NEED_FALLBACK,
		.base.cra_blocksize	= 1,
		.base.cra_ctxsize	= sizeof(struct omap_aes_ctx),
		.base.cra_module	= THIS_MODULE,

		.min_keysize		= AES_MIN_KEY_SIZE,
		.max_keysize		= AES_MAX_KEY_SIZE,
		.ivsize			= AES_BLOCK_SIZE,
		.setkey			= omap_aes_setkey,
		.encrypt		= omap_aes_ctr_encrypt,
		.decrypt		= omap_aes_ctr_decrypt,
		.init			= omap_aes_init_tfm,
		.exit			= omap_aes_exit_tfm,
	},
	.op.do_one_request = omap_aes_crypt_req,
}
};

static struct omap_aes_algs_info omap_aes_algs_info_ecb_cbc[] = {
	{
		.algs_list	= algs_ecb_cbc,
		.size		= ARRAY_SIZE(algs_ecb_cbc),
	},
};

static struct aead_engine_alg algs_aead_gcm[] = {
{
	.base = {
		.base = {
			.cra_name		= "gcm(aes)",
			.cra_driver_name	= "gcm-aes-omap",
			.cra_priority		= 300,
			.cra_flags		= CRYPTO_ALG_ASYNC |
						  CRYPTO_ALG_KERN_DRIVER_ONLY,
			.cra_blocksize		= 1,
			.cra_ctxsize		= sizeof(struct omap_aes_gcm_ctx),
			.cra_alignmask		= 0xf,
			.cra_module		= THIS_MODULE,
		},
		.init		= omap_aes_gcm_cra_init,
		.ivsize		= GCM_AES_IV_SIZE,
		.maxauthsize	= AES_BLOCK_SIZE,
		.setkey		= omap_aes_gcm_setkey,
		.setauthsize	= omap_aes_gcm_setauthsize,
		.encrypt	= omap_aes_gcm_encrypt,
		.decrypt	= omap_aes_gcm_decrypt,
	},
	.op.do_one_request = omap_aes_gcm_crypt_req,
},
{
	.base = {
		.base = {
			.cra_name		= "rfc4106(gcm(aes))",
			.cra_driver_name	= "rfc4106-gcm-aes-omap",
			.cra_priority		= 300,
			.cra_flags		= CRYPTO_ALG_ASYNC |
						  CRYPTO_ALG_KERN_DRIVER_ONLY,
			.cra_blocksize		= 1,
			.cra_ctxsize		= sizeof(struct omap_aes_gcm_ctx),
			.cra_alignmask		= 0xf,
			.cra_module		= THIS_MODULE,
		},
		.init		= omap_aes_gcm_cra_init,
		.maxauthsize	= AES_BLOCK_SIZE,
		.ivsize		= GCM_RFC4106_IV_SIZE,
		.setkey		= omap_aes_4106gcm_setkey,
		.setauthsize	= omap_aes_4106gcm_setauthsize,
		.encrypt	= omap_aes_4106gcm_encrypt,
		.decrypt	= omap_aes_4106gcm_decrypt,
	},
	.op.do_one_request = omap_aes_gcm_crypt_req,
},
};

static struct omap_aes_aead_algs omap_aes_aead_info = {
	.algs_list	=	algs_aead_gcm,
	.size		=	ARRAY_SIZE(algs_aead_gcm),
};

static const struct omap_aes_pdata omap_aes_pdata_omap2 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc),
	.trigger	= omap_aes_dma_trigger_omap2,
	.key_ofs	= 0x1c,
	.iv_ofs		= 0x20,
	.ctrl_ofs	= 0x30,
	.data_ofs	= 0x34,
	.rev_ofs	= 0x44,
	.mask_ofs	= 0x48,
	.dma_enable_in	= BIT(2),
	.dma_enable_out	= BIT(3),
	.dma_start	= BIT(5),
	.major_mask	= 0xf0,
	.major_shift	= 4,
	.minor_mask	= 0x0f,
	.minor_shift	= 0,
};

#ifdef CONFIG_OF
static struct omap_aes_algs_info omap_aes_algs_info_ecb_cbc_ctr[] = {
	{
		.algs_list	= algs_ecb_cbc,
		.size		= ARRAY_SIZE(algs_ecb_cbc),
	},
	{
		.algs_list	= algs_ctr,
		.size		= ARRAY_SIZE(algs_ctr),
	},
};

static const struct omap_aes_pdata omap_aes_pdata_omap3 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc_ctr,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc_ctr),
	.trigger	= omap_aes_dma_trigger_omap2,
	.key_ofs	= 0x1c,
	.iv_ofs		= 0x20,
	.ctrl_ofs	= 0x30,
	.data_ofs	= 0x34,
	.rev_ofs	= 0x44,
	.mask_ofs	= 0x48,
	.dma_enable_in	= BIT(2),
	.dma_enable_out	= BIT(3),
	.dma_start	= BIT(5),
	.major_mask	= 0xf0,
	.major_shift	= 4,
	.minor_mask	= 0x0f,
	.minor_shift	= 0,
};

static const struct omap_aes_pdata omap_aes_pdata_omap4 = {
	.algs_info	= omap_aes_algs_info_ecb_cbc_ctr,
	.algs_info_size	= ARRAY_SIZE(omap_aes_algs_info_ecb_cbc_ctr),
	.aead_algs_info	= &omap_aes_aead_info,
	.trigger	= omap_aes_dma_trigger_omap4,
	.key_ofs	= 0x3c,
	.iv_ofs		= 0x40,
	.ctrl_ofs	= 0x50,
	.data_ofs	= 0x60,
	.rev_ofs	= 0x80,
	.mask_ofs	= 0x84,
	.irq_status_ofs = 0x8c,
	.irq_enable_ofs = 0x90,
	.dma_enable_in	= BIT(5),
	.dma_enable_out	= BIT(6),
	.major_mask	= 0x0700,
	.major_shift	= 8,
	.minor_mask	= 0x003f,
	.minor_shift	= 0,
};

static irqreturn_t omap_aes_irq(int irq, void *dev_id)
{
	struct omap_aes_dev *dd = dev_id;
	u32 status, i;
	u32 *src, *dst;

	status = omap_aes_read(dd, AES_REG_IRQ_STATUS(dd));
	if (status & AES_REG_IRQ_DATA_IN) {
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x0);

		BUG_ON(!dd->in_sg);

		BUG_ON(_calc_walked(in) > dd->in_sg->length);

		src = sg_virt(dd->in_sg) + _calc_walked(in);

		for (i = 0; i < AES_BLOCK_WORDS; i++) {
			omap_aes_write(dd, AES_REG_DATA_N(dd, i), *src);

			scatterwalk_advance(&dd->in_walk, 4);
			if (dd->in_sg->length == _calc_walked(in)) {
				dd->in_sg = sg_next(dd->in_sg);
				if (dd->in_sg) {
					scatterwalk_start(&dd->in_walk,
							  dd->in_sg);
					src = sg_virt(dd->in_sg) +
					      _calc_walked(in);
				}
			} else {
				src++;
			}
		}

		/* Clear IRQ status */
		status &= ~AES_REG_IRQ_DATA_IN;
		omap_aes_write(dd, AES_REG_IRQ_STATUS(dd), status);

		/* Enable DATA_OUT interrupt */
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x4);

	} else if (status & AES_REG_IRQ_DATA_OUT) {
		omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x0);

		BUG_ON(!dd->out_sg);

		BUG_ON(_calc_walked(out) > dd->out_sg->length);

		dst = sg_virt(dd->out_sg) + _calc_walked(out);

		for (i = 0; i < AES_BLOCK_WORDS; i++) {
			*dst = omap_aes_read(dd, AES_REG_DATA_N(dd, i));
			scatterwalk_advance(&dd->out_walk, 4);
			if (dd->out_sg->length == _calc_walked(out)) {
				dd->out_sg = sg_next(dd->out_sg);
				if (dd->out_sg) {
					scatterwalk_start(&dd->out_walk,
							  dd->out_sg);
					dst = sg_virt(dd->out_sg) +
					      _calc_walked(out);
				}
			} else {
				dst++;
			}
		}

		dd->total -= min_t(size_t, AES_BLOCK_SIZE, dd->total);

		/* Clear IRQ status */
		status &= ~AES_REG_IRQ_DATA_OUT;
		omap_aes_write(dd, AES_REG_IRQ_STATUS(dd), status);

		if (!dd->total)
			/* All bytes read! */
			tasklet_schedule(&dd->done_task);
		else
			/* Enable DATA_IN interrupt for next block */
			omap_aes_write(dd, AES_REG_IRQ_ENABLE(dd), 0x2);
	}

	return IRQ_HANDLED;
}

static const struct of_device_id omap_aes_of_match[] = {
	{
		.compatible	= "ti,omap2-aes",
		.data		= &omap_aes_pdata_omap2,
	},
	{
		.compatible	= "ti,omap3-aes",
		.data		= &omap_aes_pdata_omap3,
	},
	{
		.compatible	= "ti,omap4-aes",
		.data		= &omap_aes_pdata_omap4,
	},
	{},
};
MODULE_DEVICE_TABLE(of, omap_aes_of_match);

static int omap_aes_get_res_of(struct omap_aes_dev *dd,
		struct device *dev, struct resource *res)
{
	struct device_node *node = dev->of_node;
	int err = 0;

	dd->pdata = of_device_get_match_data(dev);
	if (!dd->pdata) {
		dev_err(dev, "no compatible OF match\n");
		err = -EINVAL;
		goto err;
	}

	err = of_address_to_resource(node, 0, res);
	if (err < 0) {
		dev_err(dev, "can't translate OF node address\n");
		err = -EINVAL;
		goto err;
	}

err:
	return err;
}
#else
static const struct of_device_id omap_aes_of_match[] = {
	{},
};

static int omap_aes_get_res_of(struct omap_aes_dev *dd,
		struct device *dev, struct resource *res)
{
	return -EINVAL;
}
#endif

static int omap_aes_get_res_pdev(struct omap_aes_dev *dd,
		struct platform_device *pdev, struct resource *res)
{
	struct device *dev = &pdev->dev;
	struct resource *r;
	int err = 0;

	/* Get the base address */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(dev, "no MEM resource info\n");
		err = -ENODEV;
		goto err;
	}
	memcpy(res, r, sizeof(*res));

	/* Only OMAP2/3 can be non-DT */
	dd->pdata = &omap_aes_pdata_omap2;

err:
	return err;
}

static ssize_t fallback_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	return sprintf(buf, "%d\n", aes_fallback_sz);
}

static ssize_t fallback_store(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t size)
{
	ssize_t status;
	long value;

	status = kstrtol(buf, 0, &value);
	if (status)
		return status;

	/* HW accelerator only works with buffers > 9 */
	if (value < 9) {
		dev_err(dev, "minimum fallback size 9\n");
		return -EINVAL;
	}

	aes_fallback_sz = value;

	return size;
}

static ssize_t queue_len_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct omap_aes_dev *dd = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", dd->engine->queue.max_qlen);
}

static ssize_t queue_len_store(struct device *dev,
			       struct device_attribute *attr, const char *buf,
			       size_t size)
{
	struct omap_aes_dev *dd;
	ssize_t status;
	long value;
	unsigned long flags;

	status = kstrtol(buf, 0, &value);
	if (status)
		return status;

	if (value < 1)
		return -EINVAL;

	/*
	 * Changing the queue size in fly is safe, if size becomes smaller
	 * than current size, it will just not accept new entries until
	 * it has shrank enough.
	 */
	spin_lock_bh(&list_lock);
	list_for_each_entry(dd, &dev_list, list) {
		spin_lock_irqsave(&dd->lock, flags);
		dd->engine->queue.max_qlen = value;
		dd->aead_queue.base.max_qlen = value;
		spin_unlock_irqrestore(&dd->lock, flags);
	}
	spin_unlock_bh(&list_lock);

	return size;
}

static DEVICE_ATTR_RW(queue_len);
static DEVICE_ATTR_RW(fallback);

static struct attribute *omap_aes_attrs[] = {
	&dev_attr_queue_len.attr,
	&dev_attr_fallback.attr,
	NULL,
};

static const struct attribute_group omap_aes_attr_group = {
	.attrs = omap_aes_attrs,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int omap_aes_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct omap_aes_dev *dd;
<<<<<<< HEAD
	struct resource *res;
	int err = -ENOMEM, i, j;
	u32 reg;

	dd = kzalloc(sizeof(struct omap_aes_dev), GFP_KERNEL);
=======
	struct skcipher_engine_alg *algp;
	struct aead_engine_alg *aalg;
	struct resource res;
	int err = -ENOMEM, i, j, irq = -1;
	u32 reg;

	dd = devm_kzalloc(dev, sizeof(struct omap_aes_dev), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dd == NULL) {
		dev_err(dev, "unable to alloc data struct.\n");
		goto err_data;
	}
	dd->dev = dev;
	platform_set_drvdata(pdev, dd);

<<<<<<< HEAD
	spin_lock_init(&dd->lock);
	crypto_init_queue(&dd->queue, OMAP_AES_QUEUE_LENGTH);

	/* Get the base address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "invalid resource type\n");
		err = -ENODEV;
		goto err_res;
	}
	dd->phys_base = res->start;

	/* Get the DMA */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res)
		dev_info(dev, "no DMA info\n");
	else
		dd->dma_out = res->start;

	/* Get the DMA */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!res)
		dev_info(dev, "no DMA info\n");
	else
		dd->dma_in = res->start;

	/* Initializing the clock */
	dd->iclk = clk_get(dev, "ick");
	if (IS_ERR(dd->iclk)) {
		dev_err(dev, "clock intialization failed.\n");
		err = PTR_ERR(dd->iclk);
		goto err_res;
	}

	dd->io_base = ioremap(dd->phys_base, SZ_4K);
	if (!dd->io_base) {
		dev_err(dev, "can't ioremap\n");
		err = -ENOMEM;
		goto err_io;
	}

	clk_enable(dd->iclk);
	reg = omap_aes_read(dd, AES_REG_REV);
	dev_info(dev, "OMAP AES hw accel rev: %u.%u\n",
		 (reg & AES_REG_REV_MAJOR) >> 4, reg & AES_REG_REV_MINOR);
	clk_disable(dd->iclk);

	tasklet_init(&dd->done_task, omap_aes_done_task, (unsigned long)dd);
	tasklet_init(&dd->queue_task, omap_aes_queue_task, (unsigned long)dd);

	err = omap_aes_dma_init(dd);
	if (err)
		goto err_dma;

	INIT_LIST_HEAD(&dd->list);
	spin_lock(&list_lock);
	list_add_tail(&dd->list, &dev_list);
	spin_unlock(&list_lock);

	for (i = 0; i < ARRAY_SIZE(algs); i++) {
		pr_debug("i: %d\n", i);
		INIT_LIST_HEAD(&algs[i].cra_list);
		err = crypto_register_alg(&algs[i]);
		if (err)
			goto err_algs;
	}

	pr_info("probe() done\n");

	return 0;
err_algs:
	for (j = 0; j < i; j++)
		crypto_unregister_alg(&algs[j]);
	omap_aes_dma_cleanup(dd);
err_dma:
	tasklet_kill(&dd->done_task);
	tasklet_kill(&dd->queue_task);
	iounmap(dd->io_base);
err_io:
	clk_put(dd->iclk);
err_res:
	kfree(dd);
=======
	aead_init_queue(&dd->aead_queue, OMAP_AES_QUEUE_LENGTH);

	err = (dev->of_node) ? omap_aes_get_res_of(dd, dev, &res) :
			       omap_aes_get_res_pdev(dd, pdev, &res);
	if (err)
		goto err_res;

	dd->io_base = devm_ioremap_resource(dev, &res);
	if (IS_ERR(dd->io_base)) {
		err = PTR_ERR(dd->io_base);
		goto err_res;
	}
	dd->phys_base = res.start;

	pm_runtime_use_autosuspend(dev);
	pm_runtime_set_autosuspend_delay(dev, DEFAULT_AUTOSUSPEND_DELAY);

	pm_runtime_enable(dev);
	err = pm_runtime_resume_and_get(dev);
	if (err < 0) {
		dev_err(dev, "%s: failed to get_sync(%d)\n",
			__func__, err);
		goto err_pm_disable;
	}

	omap_aes_dma_stop(dd);

	reg = omap_aes_read(dd, AES_REG_REV(dd));

	pm_runtime_put_sync(dev);

	dev_info(dev, "OMAP AES hw accel rev: %u.%u\n",
		 (reg & dd->pdata->major_mask) >> dd->pdata->major_shift,
		 (reg & dd->pdata->minor_mask) >> dd->pdata->minor_shift);

	tasklet_init(&dd->done_task, omap_aes_done_task, (unsigned long)dd);

	err = omap_aes_dma_init(dd);
	if (err == -EPROBE_DEFER) {
		goto err_irq;
	} else if (err && AES_REG_IRQ_STATUS(dd) && AES_REG_IRQ_ENABLE(dd)) {
		dd->pio_only = 1;

		irq = platform_get_irq(pdev, 0);
		if (irq < 0) {
			err = irq;
			goto err_irq;
		}

		err = devm_request_irq(dev, irq, omap_aes_irq, 0,
				dev_name(dev), dd);
		if (err) {
			dev_err(dev, "Unable to grab omap-aes IRQ\n");
			goto err_irq;
		}
	}

	spin_lock_init(&dd->lock);

	INIT_LIST_HEAD(&dd->list);
	spin_lock_bh(&list_lock);
	list_add_tail(&dd->list, &dev_list);
	spin_unlock_bh(&list_lock);

	/* Initialize crypto engine */
	dd->engine = crypto_engine_alloc_init(dev, 1);
	if (!dd->engine) {
		err = -ENOMEM;
		goto err_engine;
	}

	err = crypto_engine_start(dd->engine);
	if (err)
		goto err_engine;

	for (i = 0; i < dd->pdata->algs_info_size; i++) {
		if (!dd->pdata->algs_info[i].registered) {
			for (j = 0; j < dd->pdata->algs_info[i].size; j++) {
				algp = &dd->pdata->algs_info[i].algs_list[j];

				pr_debug("reg alg: %s\n", algp->base.base.cra_name);

				err = crypto_engine_register_skcipher(algp);
				if (err)
					goto err_algs;

				dd->pdata->algs_info[i].registered++;
			}
		}
	}

	if (dd->pdata->aead_algs_info &&
	    !dd->pdata->aead_algs_info->registered) {
		for (i = 0; i < dd->pdata->aead_algs_info->size; i++) {
			aalg = &dd->pdata->aead_algs_info->algs_list[i];

			pr_debug("reg alg: %s\n", aalg->base.base.cra_name);

			err = crypto_engine_register_aead(aalg);
			if (err)
				goto err_aead_algs;

			dd->pdata->aead_algs_info->registered++;
		}
	}

	err = sysfs_create_group(&dev->kobj, &omap_aes_attr_group);
	if (err) {
		dev_err(dev, "could not create sysfs device attrs\n");
		goto err_aead_algs;
	}

	return 0;
err_aead_algs:
	for (i = dd->pdata->aead_algs_info->registered - 1; i >= 0; i--) {
		aalg = &dd->pdata->aead_algs_info->algs_list[i];
		crypto_engine_unregister_aead(aalg);
	}
err_algs:
	for (i = dd->pdata->algs_info_size - 1; i >= 0; i--)
		for (j = dd->pdata->algs_info[i].registered - 1; j >= 0; j--)
			crypto_engine_unregister_skcipher(
					&dd->pdata->algs_info[i].algs_list[j]);

err_engine:
	if (dd->engine)
		crypto_engine_exit(dd->engine);

	omap_aes_dma_cleanup(dd);
err_irq:
	tasklet_kill(&dd->done_task);
err_pm_disable:
	pm_runtime_disable(dev);
err_res:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dd = NULL;
err_data:
	dev_err(dev, "initialization failed.\n");
	return err;
}

<<<<<<< HEAD
static int omap_aes_remove(struct platform_device *pdev)
{
	struct omap_aes_dev *dd = platform_get_drvdata(pdev);
	int i;

	if (!dd)
		return -ENODEV;

	spin_lock(&list_lock);
	list_del(&dd->list);
	spin_unlock(&list_lock);

	for (i = 0; i < ARRAY_SIZE(algs); i++)
		crypto_unregister_alg(&algs[i]);

	tasklet_kill(&dd->done_task);
	tasklet_kill(&dd->queue_task);
	omap_aes_dma_cleanup(dd);
	iounmap(dd->io_base);
	clk_put(dd->iclk);
	kfree(dd);
	dd = NULL;

	return 0;
}

static struct platform_driver omap_aes_driver = {
	.probe	= omap_aes_probe,
	.remove	= omap_aes_remove,
	.driver	= {
		.name	= "omap-aes",
		.owner	= THIS_MODULE,
	},
};

static int __init omap_aes_mod_init(void)
{
	pr_info("loading %s driver\n", "omap-aes");

	if (!cpu_class_is_omap2() || omap_type() != OMAP2_DEVICE_TYPE_SEC) {
		pr_err("Unsupported cpu\n");
		return -ENODEV;
	}

	return  platform_driver_register(&omap_aes_driver);
}

static void __exit omap_aes_mod_exit(void)
{
	platform_driver_unregister(&omap_aes_driver);
}

module_init(omap_aes_mod_init);
module_exit(omap_aes_mod_exit);
=======
static void omap_aes_remove(struct platform_device *pdev)
{
	struct omap_aes_dev *dd = platform_get_drvdata(pdev);
	struct aead_engine_alg *aalg;
	int i, j;

	spin_lock_bh(&list_lock);
	list_del(&dd->list);
	spin_unlock_bh(&list_lock);

	for (i = dd->pdata->algs_info_size - 1; i >= 0; i--)
		for (j = dd->pdata->algs_info[i].registered - 1; j >= 0; j--) {
			crypto_engine_unregister_skcipher(
					&dd->pdata->algs_info[i].algs_list[j]);
			dd->pdata->algs_info[i].registered--;
		}

	for (i = dd->pdata->aead_algs_info->registered - 1; i >= 0; i--) {
		aalg = &dd->pdata->aead_algs_info->algs_list[i];
		crypto_engine_unregister_aead(aalg);
		dd->pdata->aead_algs_info->registered--;
	}

	crypto_engine_exit(dd->engine);

	tasklet_kill(&dd->done_task);
	omap_aes_dma_cleanup(dd);
	pm_runtime_disable(dd->dev);

	sysfs_remove_group(&dd->dev->kobj, &omap_aes_attr_group);
}

#ifdef CONFIG_PM_SLEEP
static int omap_aes_suspend(struct device *dev)
{
	pm_runtime_put_sync(dev);
	return 0;
}

static int omap_aes_resume(struct device *dev)
{
	pm_runtime_get_sync(dev);
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(omap_aes_pm_ops, omap_aes_suspend, omap_aes_resume);

static struct platform_driver omap_aes_driver = {
	.probe	= omap_aes_probe,
	.remove_new = omap_aes_remove,
	.driver	= {
		.name	= "omap-aes",
		.pm	= &omap_aes_pm_ops,
		.of_match_table	= omap_aes_of_match,
	},
};

module_platform_driver(omap_aes_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("OMAP AES hw acceleration support.");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Dmitry Kasatkin");

