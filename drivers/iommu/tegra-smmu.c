<<<<<<< HEAD
/*
 * IOMMU API for SMMU in Tegra30
 *
 * Copyright (c) 2011-2012, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define pr_fmt(fmt)	"%s(): " fmt, __func__

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/iommu.h>
#include <linux/io.h>

#include <asm/page.h>
#include <asm/cacheflush.h>

#include <mach/iomap.h>
#include <mach/smmu.h>

/* bitmap of the page sizes currently supported */
#define SMMU_IOMMU_PGSIZES	(SZ_4K)

#define SMMU_CONFIG				0x10
#define SMMU_CONFIG_DISABLE			0
#define SMMU_CONFIG_ENABLE			1

#define SMMU_TLB_CONFIG				0x14
#define SMMU_TLB_CONFIG_STATS__MASK		(1 << 31)
#define SMMU_TLB_CONFIG_STATS__ENABLE		(1 << 31)
#define SMMU_TLB_CONFIG_HIT_UNDER_MISS__ENABLE	(1 << 29)
#define SMMU_TLB_CONFIG_ACTIVE_LINES__VALUE	0x10
#define SMMU_TLB_CONFIG_RESET_VAL		0x20000010

#define SMMU_PTC_CONFIG				0x18
#define SMMU_PTC_CONFIG_STATS__MASK		(1 << 31)
#define SMMU_PTC_CONFIG_STATS__ENABLE		(1 << 31)
#define SMMU_PTC_CONFIG_CACHE__ENABLE		(1 << 29)
#define SMMU_PTC_CONFIG_INDEX_MAP__PATTERN	0x3f
#define SMMU_PTC_CONFIG_RESET_VAL		0x2000003f

#define SMMU_PTB_ASID				0x1c
#define SMMU_PTB_ASID_CURRENT_SHIFT		0

#define SMMU_PTB_DATA				0x20
#define SMMU_PTB_DATA_RESET_VAL			0
#define SMMU_PTB_DATA_ASID_NONSECURE_SHIFT	29
#define SMMU_PTB_DATA_ASID_WRITABLE_SHIFT	30
#define SMMU_PTB_DATA_ASID_READABLE_SHIFT	31

#define SMMU_TLB_FLUSH				0x30
#define SMMU_TLB_FLUSH_VA_MATCH_ALL		0
#define SMMU_TLB_FLUSH_VA_MATCH_SECTION		2
#define SMMU_TLB_FLUSH_VA_MATCH_GROUP		3
#define SMMU_TLB_FLUSH_ASID_SHIFT		29
#define SMMU_TLB_FLUSH_ASID_MATCH_DISABLE	0
#define SMMU_TLB_FLUSH_ASID_MATCH_ENABLE	1
#define SMMU_TLB_FLUSH_ASID_MATCH_SHIFT		31

#define SMMU_PTC_FLUSH				0x34
#define SMMU_PTC_FLUSH_TYPE_ALL			0
#define SMMU_PTC_FLUSH_TYPE_ADR			1
#define SMMU_PTC_FLUSH_ADR_SHIFT		4

#define SMMU_ASID_SECURITY			0x38

#define SMMU_STATS_TLB_HIT_COUNT		0x1f0
#define SMMU_STATS_TLB_MISS_COUNT		0x1f4
#define SMMU_STATS_PTC_HIT_COUNT		0x1f8
#define SMMU_STATS_PTC_MISS_COUNT		0x1fc

#define SMMU_TRANSLATION_ENABLE_0		0x228
#define SMMU_TRANSLATION_ENABLE_1		0x22c
#define SMMU_TRANSLATION_ENABLE_2		0x230

#define SMMU_AFI_ASID	0x238   /* PCIE */
#define SMMU_AVPC_ASID	0x23c   /* AVP */
#define SMMU_DC_ASID	0x240   /* Display controller */
#define SMMU_DCB_ASID	0x244   /* Display controller B */
#define SMMU_EPP_ASID	0x248   /* Encoder pre-processor */
#define SMMU_G2_ASID	0x24c   /* 2D engine */
#define SMMU_HC_ASID	0x250   /* Host1x */
#define SMMU_HDA_ASID	0x254   /* High-def audio */
#define SMMU_ISP_ASID	0x258   /* Image signal processor */
#define SMMU_MPE_ASID	0x264   /* MPEG encoder */
#define SMMU_NV_ASID	0x268   /* (3D) */
#define SMMU_NV2_ASID	0x26c   /* (3D) */
#define SMMU_PPCS_ASID	0x270   /* AHB */
#define SMMU_SATA_ASID	0x278   /* SATA */
#define SMMU_VDE_ASID	0x27c   /* Video decoder */
#define SMMU_VI_ASID	0x280   /* Video input */

#define SMMU_PDE_NEXT_SHIFT		28

/* AHB Arbiter Registers */
#define AHB_XBAR_CTRL				0xe0
#define AHB_XBAR_CTRL_SMMU_INIT_DONE_DONE	1
#define AHB_XBAR_CTRL_SMMU_INIT_DONE_SHIFT	17

#define SMMU_NUM_ASIDS				4
#define SMMU_TLB_FLUSH_VA_SECTION__MASK		0xffc00000
#define SMMU_TLB_FLUSH_VA_SECTION__SHIFT	12 /* right shift */
#define SMMU_TLB_FLUSH_VA_GROUP__MASK		0xffffc000
#define SMMU_TLB_FLUSH_VA_GROUP__SHIFT		12 /* right shift */
#define SMMU_TLB_FLUSH_VA(iova, which)	\
	((((iova) & SMMU_TLB_FLUSH_VA_##which##__MASK) >> \
		SMMU_TLB_FLUSH_VA_##which##__SHIFT) |	\
	SMMU_TLB_FLUSH_VA_MATCH_##which)
#define SMMU_PTB_ASID_CUR(n)	\
		((n) << SMMU_PTB_ASID_CURRENT_SHIFT)
#define SMMU_TLB_FLUSH_ASID_MATCH_disable		\
		(SMMU_TLB_FLUSH_ASID_MATCH_DISABLE <<	\
			SMMU_TLB_FLUSH_ASID_MATCH_SHIFT)
#define SMMU_TLB_FLUSH_ASID_MATCH__ENABLE		\
		(SMMU_TLB_FLUSH_ASID_MATCH_ENABLE <<	\
			SMMU_TLB_FLUSH_ASID_MATCH_SHIFT)

#define SMMU_PAGE_SHIFT 12
#define SMMU_PAGE_SIZE	(1 << SMMU_PAGE_SHIFT)

#define SMMU_PDIR_COUNT	1024
#define SMMU_PDIR_SIZE	(sizeof(unsigned long) * SMMU_PDIR_COUNT)
#define SMMU_PTBL_COUNT	1024
#define SMMU_PTBL_SIZE	(sizeof(unsigned long) * SMMU_PTBL_COUNT)
#define SMMU_PDIR_SHIFT	12
#define SMMU_PDE_SHIFT	12
#define SMMU_PTE_SHIFT	12
#define SMMU_PFN_MASK	0x000fffff

#define SMMU_ADDR_TO_PFN(addr)	((addr) >> 12)
#define SMMU_ADDR_TO_PDN(addr)	((addr) >> 22)
#define SMMU_PDN_TO_ADDR(pdn)	((pdn) << 22)

#define _READABLE	(1 << SMMU_PTB_DATA_ASID_READABLE_SHIFT)
#define _WRITABLE	(1 << SMMU_PTB_DATA_ASID_WRITABLE_SHIFT)
#define _NONSECURE	(1 << SMMU_PTB_DATA_ASID_NONSECURE_SHIFT)
#define _PDE_NEXT	(1 << SMMU_PDE_NEXT_SHIFT)
#define _MASK_ATTR	(_READABLE | _WRITABLE | _NONSECURE)

#define _PDIR_ATTR	(_READABLE | _WRITABLE | _NONSECURE)

#define _PDE_ATTR	(_READABLE | _WRITABLE | _NONSECURE)
#define _PDE_ATTR_N	(_PDE_ATTR | _PDE_NEXT)
#define _PDE_VACANT(pdn)	(((pdn) << 10) | _PDE_ATTR)

#define _PTE_ATTR	(_READABLE | _WRITABLE | _NONSECURE)
#define _PTE_VACANT(addr)	(((addr) >> SMMU_PAGE_SHIFT) | _PTE_ATTR)

#define SMMU_MK_PDIR(page, attr)	\
		((page_to_phys(page) >> SMMU_PDIR_SHIFT) | (attr))
#define SMMU_MK_PDE(page, attr)		\
		(unsigned long)((page_to_phys(page) >> SMMU_PDE_SHIFT) | (attr))
#define SMMU_EX_PTBL_PAGE(pde)		\
		pfn_to_page((unsigned long)(pde) & SMMU_PFN_MASK)
#define SMMU_PFN_TO_PTE(pfn, attr)	(unsigned long)((pfn) | (attr))

#define SMMU_ASID_ENABLE(asid)	((asid) | (1 << 31))
#define SMMU_ASID_DISABLE	0
#define SMMU_ASID_ASID(n)	((n) & ~SMMU_ASID_ENABLE(0))

#define smmu_client_enable_hwgrp(c, m)	smmu_client_set_hwgrp(c, m, 1)
#define smmu_client_disable_hwgrp(c)	smmu_client_set_hwgrp(c, 0, 0)
#define __smmu_client_enable_hwgrp(c, m) __smmu_client_set_hwgrp(c, m, 1)
#define __smmu_client_disable_hwgrp(c)	__smmu_client_set_hwgrp(c, 0, 0)

#define HWGRP_INIT(client) [HWGRP_##client] = SMMU_##client##_ASID

static const u32 smmu_hwgrp_asid_reg[] = {
	HWGRP_INIT(AFI),
	HWGRP_INIT(AVPC),
	HWGRP_INIT(DC),
	HWGRP_INIT(DCB),
	HWGRP_INIT(EPP),
	HWGRP_INIT(G2),
	HWGRP_INIT(HC),
	HWGRP_INIT(HDA),
	HWGRP_INIT(ISP),
	HWGRP_INIT(MPE),
	HWGRP_INIT(NV),
	HWGRP_INIT(NV2),
	HWGRP_INIT(PPCS),
	HWGRP_INIT(SATA),
	HWGRP_INIT(VDE),
	HWGRP_INIT(VI),
};
#define HWGRP_ASID_REG(x) (smmu_hwgrp_asid_reg[x])

/*
 * Per client for address space
 */
struct smmu_client {
	struct device		*dev;
	struct list_head	list;
	struct smmu_as		*as;
	u32			hwgrp;
};

/*
 * Per address space
 */
struct smmu_as {
	struct smmu_device	*smmu;	/* back pointer to container */
	unsigned int		asid;
	spinlock_t		lock;	/* for pagetable */
	struct page		*pdir_page;
	unsigned long		pdir_attr;
	unsigned long		pde_attr;
	unsigned long		pte_attr;
	unsigned int		*pte_count;

	struct list_head	client;
	spinlock_t		client_lock; /* for client list */
};

/*
 * Per SMMU device - IOMMU device
 */
struct smmu_device {
	void __iomem	*regs, *regs_ahbarb;
	unsigned long	iovmm_base;	/* remappable base address */
	unsigned long	page_count;	/* total remappable size */
	spinlock_t	lock;
	char		*name;
	struct device	*dev;
	int		num_as;
	struct smmu_as	*as;		/* Run-time allocated array */
	struct page *avp_vector_page;	/* dummy page shared by all AS's */

	/*
	 * Register image savers for suspend/resume
	 */
	unsigned long translation_enable_0;
	unsigned long translation_enable_1;
	unsigned long translation_enable_2;
	unsigned long asid_security;
};

static struct smmu_device *smmu_handle; /* unique for a system */

/*
 *	SMMU/AHB register accessors
 */
static inline u32 smmu_read(struct smmu_device *smmu, size_t offs)
{
	return readl(smmu->regs + offs);
}
static inline void smmu_write(struct smmu_device *smmu, u32 val, size_t offs)
{
	writel(val, smmu->regs + offs);
}

static inline u32 ahb_read(struct smmu_device *smmu, size_t offs)
{
	return readl(smmu->regs_ahbarb + offs);
}
static inline void ahb_write(struct smmu_device *smmu, u32 val, size_t offs)
{
	writel(val, smmu->regs_ahbarb + offs);
}

#define VA_PAGE_TO_PA(va, page)	\
	(page_to_phys(page) + ((unsigned long)(va) & ~PAGE_MASK))

#define FLUSH_CPU_DCACHE(va, page, size)	\
	do {	\
		unsigned long _pa_ = VA_PAGE_TO_PA(va, page);		\
		__cpuc_flush_dcache_area((void *)(va), (size_t)(size));	\
		outer_flush_range(_pa_, _pa_+(size_t)(size));		\
	} while (0)

/*
 * Any interaction between any block on PPSB and a block on APB or AHB
 * must have these read-back barriers to ensure the APB/AHB bus
 * transaction is complete before initiating activity on the PPSB
 * block.
 */
#define FLUSH_SMMU_REGS(smmu)	smmu_read(smmu, SMMU_CONFIG)

#define smmu_client_hwgrp(c) (u32)((c)->dev->platform_data)

static int __smmu_client_set_hwgrp(struct smmu_client *c,
				   unsigned long map, int on)
{
	int i;
	struct smmu_as *as = c->as;
	u32 val, offs, mask = SMMU_ASID_ENABLE(as->asid);
	struct smmu_device *smmu = as->smmu;

	WARN_ON(!on && map);
	if (on && !map)
		return -EINVAL;
	if (!on)
		map = smmu_client_hwgrp(c);

	for_each_set_bit(i, &map, HWGRP_COUNT) {
		offs = HWGRP_ASID_REG(i);
		val = smmu_read(smmu, offs);
		if (on) {
			if (WARN_ON(val & mask))
				goto err_hw_busy;
			val |= mask;
		} else {
			WARN_ON((val & mask) == mask);
			val &= ~mask;
		}
		smmu_write(smmu, val, offs);
	}
	FLUSH_SMMU_REGS(smmu);
	c->hwgrp = map;
	return 0;

err_hw_busy:
	for_each_set_bit(i, &map, HWGRP_COUNT) {
		offs = HWGRP_ASID_REG(i);
		val = smmu_read(smmu, offs);
		val &= ~mask;
		smmu_write(smmu, val, offs);
	}
	return -EBUSY;
}

static int smmu_client_set_hwgrp(struct smmu_client *c, u32 map, int on)
{
	u32 val;
	unsigned long flags;
	struct smmu_as *as = c->as;
	struct smmu_device *smmu = as->smmu;

	spin_lock_irqsave(&smmu->lock, flags);
	val = __smmu_client_set_hwgrp(c, map, on);
	spin_unlock_irqrestore(&smmu->lock, flags);
	return val;
}

/*
 * Flush all TLB entries and all PTC entries
 * Caller must lock smmu
 */
static void smmu_flush_regs(struct smmu_device *smmu, int enable)
{
	u32 val;

	smmu_write(smmu, SMMU_PTC_FLUSH_TYPE_ALL, SMMU_PTC_FLUSH);
	FLUSH_SMMU_REGS(smmu);
	val = SMMU_TLB_FLUSH_VA_MATCH_ALL |
		SMMU_TLB_FLUSH_ASID_MATCH_disable;
	smmu_write(smmu, val, SMMU_TLB_FLUSH);

	if (enable)
		smmu_write(smmu, SMMU_CONFIG_ENABLE, SMMU_CONFIG);
	FLUSH_SMMU_REGS(smmu);
}

static void smmu_setup_regs(struct smmu_device *smmu)
{
	int i;
	u32 val;

	for (i = 0; i < smmu->num_as; i++) {
		struct smmu_as *as = &smmu->as[i];
		struct smmu_client *c;

		smmu_write(smmu, SMMU_PTB_ASID_CUR(as->asid), SMMU_PTB_ASID);
		val = as->pdir_page ?
			SMMU_MK_PDIR(as->pdir_page, as->pdir_attr) :
			SMMU_PTB_DATA_RESET_VAL;
		smmu_write(smmu, val, SMMU_PTB_DATA);

		list_for_each_entry(c, &as->client, list)
			__smmu_client_set_hwgrp(c, c->hwgrp, 1);
	}

	smmu_write(smmu, smmu->translation_enable_0, SMMU_TRANSLATION_ENABLE_0);
	smmu_write(smmu, smmu->translation_enable_1, SMMU_TRANSLATION_ENABLE_1);
	smmu_write(smmu, smmu->translation_enable_2, SMMU_TRANSLATION_ENABLE_2);
	smmu_write(smmu, smmu->asid_security, SMMU_ASID_SECURITY);
	smmu_write(smmu, SMMU_TLB_CONFIG_RESET_VAL, SMMU_TLB_CONFIG);
	smmu_write(smmu, SMMU_PTC_CONFIG_RESET_VAL, SMMU_PTC_CONFIG);

	smmu_flush_regs(smmu, 1);

	val = ahb_read(smmu, AHB_XBAR_CTRL);
	val |= AHB_XBAR_CTRL_SMMU_INIT_DONE_DONE <<
		AHB_XBAR_CTRL_SMMU_INIT_DONE_SHIFT;
	ahb_write(smmu, val, AHB_XBAR_CTRL);
}

static void flush_ptc_and_tlb(struct smmu_device *smmu,
		      struct smmu_as *as, dma_addr_t iova,
		      unsigned long *pte, struct page *page, int is_pde)
{
	u32 val;
	unsigned long tlb_flush_va = is_pde
		?  SMMU_TLB_FLUSH_VA(iova, SECTION)
		:  SMMU_TLB_FLUSH_VA(iova, GROUP);

	val = SMMU_PTC_FLUSH_TYPE_ADR | VA_PAGE_TO_PA(pte, page);
	smmu_write(smmu, val, SMMU_PTC_FLUSH);
	FLUSH_SMMU_REGS(smmu);
	val = tlb_flush_va |
		SMMU_TLB_FLUSH_ASID_MATCH__ENABLE |
		(as->asid << SMMU_TLB_FLUSH_ASID_SHIFT);
	smmu_write(smmu, val, SMMU_TLB_FLUSH);
	FLUSH_SMMU_REGS(smmu);
}

static void free_ptbl(struct smmu_as *as, dma_addr_t iova)
{
	unsigned long pdn = SMMU_ADDR_TO_PDN(iova);
	unsigned long *pdir = (unsigned long *)page_address(as->pdir_page);

	if (pdir[pdn] != _PDE_VACANT(pdn)) {
		dev_dbg(as->smmu->dev, "pdn: %lx\n", pdn);

		ClearPageReserved(SMMU_EX_PTBL_PAGE(pdir[pdn]));
		__free_page(SMMU_EX_PTBL_PAGE(pdir[pdn]));
		pdir[pdn] = _PDE_VACANT(pdn);
		FLUSH_CPU_DCACHE(&pdir[pdn], as->pdir_page, sizeof pdir[pdn]);
		flush_ptc_and_tlb(as->smmu, as, iova, &pdir[pdn],
				  as->pdir_page, 1);
	}
}

static void free_pdir(struct smmu_as *as)
{
	unsigned addr;
	int count;
	struct device *dev = as->smmu->dev;

	if (!as->pdir_page)
		return;

	addr = as->smmu->iovmm_base;
	count = as->smmu->page_count;
	while (count-- > 0) {
		free_ptbl(as, addr);
		addr += SMMU_PAGE_SIZE * SMMU_PTBL_COUNT;
	}
	ClearPageReserved(as->pdir_page);
	__free_page(as->pdir_page);
	as->pdir_page = NULL;
	devm_kfree(dev, as->pte_count);
	as->pte_count = NULL;
}

/*
 * Maps PTBL for given iova and returns the PTE address
 * Caller must unmap the mapped PTBL returned in *ptbl_page_p
 */
static unsigned long *locate_pte(struct smmu_as *as,
				 dma_addr_t iova, bool allocate,
				 struct page **ptbl_page_p,
				 unsigned int **count)
{
	unsigned long ptn = SMMU_ADDR_TO_PFN(iova);
	unsigned long pdn = SMMU_ADDR_TO_PDN(iova);
	unsigned long *pdir = page_address(as->pdir_page);
	unsigned long *ptbl;

	if (pdir[pdn] != _PDE_VACANT(pdn)) {
		/* Mapped entry table already exists */
		*ptbl_page_p = SMMU_EX_PTBL_PAGE(pdir[pdn]);
		ptbl = page_address(*ptbl_page_p);
	} else if (!allocate) {
		return NULL;
	} else {
		int pn;
		unsigned long addr = SMMU_PDN_TO_ADDR(pdn);

		/* Vacant - allocate a new page table */
		dev_dbg(as->smmu->dev, "New PTBL pdn: %lx\n", pdn);

		*ptbl_page_p = alloc_page(GFP_ATOMIC);
		if (!*ptbl_page_p) {
			dev_err(as->smmu->dev,
				"failed to allocate smmu_device page table\n");
			return NULL;
		}
		SetPageReserved(*ptbl_page_p);
		ptbl = (unsigned long *)page_address(*ptbl_page_p);
		for (pn = 0; pn < SMMU_PTBL_COUNT;
		     pn++, addr += SMMU_PAGE_SIZE) {
			ptbl[pn] = _PTE_VACANT(addr);
		}
		FLUSH_CPU_DCACHE(ptbl, *ptbl_page_p, SMMU_PTBL_SIZE);
		pdir[pdn] = SMMU_MK_PDE(*ptbl_page_p,
					as->pde_attr | _PDE_NEXT);
		FLUSH_CPU_DCACHE(&pdir[pdn], as->pdir_page, sizeof pdir[pdn]);
		flush_ptc_and_tlb(as->smmu, as, iova, &pdir[pdn],
				  as->pdir_page, 1);
	}
	*count = &as->pte_count[pdn];

	return &ptbl[ptn % SMMU_PTBL_COUNT];
}

#ifdef CONFIG_SMMU_SIG_DEBUG
static void put_signature(struct smmu_as *as,
			  dma_addr_t iova, unsigned long pfn)
{
	struct page *page;
	unsigned long *vaddr;

	page = pfn_to_page(pfn);
	vaddr = page_address(page);
	if (!vaddr)
		return;

	vaddr[0] = iova;
	vaddr[1] = pfn << PAGE_SHIFT;
	FLUSH_CPU_DCACHE(vaddr, page, sizeof(vaddr[0]) * 2);
}
#else
static inline void put_signature(struct smmu_as *as,
				 unsigned long addr, unsigned long pfn)
{
}
#endif

/*
 * Caller must lock/unlock as
 */
static int alloc_pdir(struct smmu_as *as)
{
	unsigned long *pdir;
	int pdn;
	u32 val;
	struct smmu_device *smmu = as->smmu;

	if (as->pdir_page)
		return 0;

	as->pte_count = devm_kzalloc(smmu->dev,
		     sizeof(as->pte_count[0]) * SMMU_PDIR_COUNT, GFP_ATOMIC);
	if (!as->pte_count) {
		dev_err(smmu->dev,
			"failed to allocate smmu_device PTE cunters\n");
		return -ENOMEM;
	}
	as->pdir_page = alloc_page(GFP_ATOMIC | __GFP_DMA);
	if (!as->pdir_page) {
		dev_err(smmu->dev,
			"failed to allocate smmu_device page directory\n");
		devm_kfree(smmu->dev, as->pte_count);
		as->pte_count = NULL;
		return -ENOMEM;
	}
	SetPageReserved(as->pdir_page);
	pdir = page_address(as->pdir_page);

	for (pdn = 0; pdn < SMMU_PDIR_COUNT; pdn++)
		pdir[pdn] = _PDE_VACANT(pdn);
	FLUSH_CPU_DCACHE(pdir, as->pdir_page, SMMU_PDIR_SIZE);
	val = SMMU_PTC_FLUSH_TYPE_ADR | VA_PAGE_TO_PA(pdir, as->pdir_page);
	smmu_write(smmu, val, SMMU_PTC_FLUSH);
	FLUSH_SMMU_REGS(as->smmu);
	val = SMMU_TLB_FLUSH_VA_MATCH_ALL |
		SMMU_TLB_FLUSH_ASID_MATCH__ENABLE |
		(as->asid << SMMU_TLB_FLUSH_ASID_SHIFT);
	smmu_write(smmu, val, SMMU_TLB_FLUSH);
	FLUSH_SMMU_REGS(as->smmu);
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011-2014 NVIDIA CORPORATION.  All rights reserved.
 */

#include <linux/bitops.h>
#include <linux/debugfs.h>
#include <linux/err.h>
#include <linux/iommu.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>

#include <soc/tegra/ahb.h>
#include <soc/tegra/mc.h>

struct tegra_smmu_group {
	struct list_head list;
	struct tegra_smmu *smmu;
	const struct tegra_smmu_group_soc *soc;
	struct iommu_group *group;
	unsigned int swgroup;
};

struct tegra_smmu {
	void __iomem *regs;
	struct device *dev;

	struct tegra_mc *mc;
	const struct tegra_smmu_soc *soc;

	struct list_head groups;

	unsigned long pfn_mask;
	unsigned long tlb_mask;

	unsigned long *asids;
	struct mutex lock;

	struct list_head list;

	struct dentry *debugfs;

	struct iommu_device iommu;	/* IOMMU Core code handle */
};

struct tegra_smmu_as {
	struct iommu_domain domain;
	struct tegra_smmu *smmu;
	unsigned int use_count;
	spinlock_t lock;
	u32 *count;
	struct page **pts;
	struct page *pd;
	dma_addr_t pd_dma;
	unsigned id;
	u32 attr;
};

static struct tegra_smmu_as *to_smmu_as(struct iommu_domain *dom)
{
	return container_of(dom, struct tegra_smmu_as, domain);
}

static inline void smmu_writel(struct tegra_smmu *smmu, u32 value,
			       unsigned long offset)
{
	writel(value, smmu->regs + offset);
}

static inline u32 smmu_readl(struct tegra_smmu *smmu, unsigned long offset)
{
	return readl(smmu->regs + offset);
}

#define SMMU_CONFIG 0x010
#define  SMMU_CONFIG_ENABLE (1 << 0)

#define SMMU_TLB_CONFIG 0x14
#define  SMMU_TLB_CONFIG_HIT_UNDER_MISS (1 << 29)
#define  SMMU_TLB_CONFIG_ROUND_ROBIN_ARBITRATION (1 << 28)
#define  SMMU_TLB_CONFIG_ACTIVE_LINES(smmu) \
	((smmu)->soc->num_tlb_lines & (smmu)->tlb_mask)

#define SMMU_PTC_CONFIG 0x18
#define  SMMU_PTC_CONFIG_ENABLE (1 << 29)
#define  SMMU_PTC_CONFIG_REQ_LIMIT(x) (((x) & 0x0f) << 24)
#define  SMMU_PTC_CONFIG_INDEX_MAP(x) ((x) & 0x3f)

#define SMMU_PTB_ASID 0x01c
#define  SMMU_PTB_ASID_VALUE(x) ((x) & 0x7f)

#define SMMU_PTB_DATA 0x020
#define  SMMU_PTB_DATA_VALUE(dma, attr) ((dma) >> 12 | (attr))

#define SMMU_MK_PDE(dma, attr) ((dma) >> SMMU_PTE_SHIFT | (attr))

#define SMMU_TLB_FLUSH 0x030
#define  SMMU_TLB_FLUSH_VA_MATCH_ALL     (0 << 0)
#define  SMMU_TLB_FLUSH_VA_MATCH_SECTION (2 << 0)
#define  SMMU_TLB_FLUSH_VA_MATCH_GROUP   (3 << 0)
#define  SMMU_TLB_FLUSH_VA_SECTION(addr) ((((addr) & 0xffc00000) >> 12) | \
					  SMMU_TLB_FLUSH_VA_MATCH_SECTION)
#define  SMMU_TLB_FLUSH_VA_GROUP(addr)   ((((addr) & 0xffffc000) >> 12) | \
					  SMMU_TLB_FLUSH_VA_MATCH_GROUP)
#define  SMMU_TLB_FLUSH_ASID_MATCH       (1 << 31)

#define SMMU_PTC_FLUSH 0x034
#define  SMMU_PTC_FLUSH_TYPE_ALL (0 << 0)
#define  SMMU_PTC_FLUSH_TYPE_ADR (1 << 0)

#define SMMU_PTC_FLUSH_HI 0x9b8
#define  SMMU_PTC_FLUSH_HI_MASK 0x3

/* per-SWGROUP SMMU_*_ASID register */
#define SMMU_ASID_ENABLE (1 << 31)
#define SMMU_ASID_MASK 0x7f
#define SMMU_ASID_VALUE(x) ((x) & SMMU_ASID_MASK)

/* page table definitions */
#define SMMU_NUM_PDE 1024
#define SMMU_NUM_PTE 1024

#define SMMU_SIZE_PD (SMMU_NUM_PDE * 4)
#define SMMU_SIZE_PT (SMMU_NUM_PTE * 4)

#define SMMU_PDE_SHIFT 22
#define SMMU_PTE_SHIFT 12

#define SMMU_PAGE_MASK		(~(SMMU_SIZE_PT-1))
#define SMMU_OFFSET_IN_PAGE(x)	((unsigned long)(x) & ~SMMU_PAGE_MASK)
#define SMMU_PFN_PHYS(x)	((phys_addr_t)(x) << SMMU_PTE_SHIFT)
#define SMMU_PHYS_PFN(x)	((unsigned long)((x) >> SMMU_PTE_SHIFT))

#define SMMU_PD_READABLE	(1 << 31)
#define SMMU_PD_WRITABLE	(1 << 30)
#define SMMU_PD_NONSECURE	(1 << 29)

#define SMMU_PDE_READABLE	(1 << 31)
#define SMMU_PDE_WRITABLE	(1 << 30)
#define SMMU_PDE_NONSECURE	(1 << 29)
#define SMMU_PDE_NEXT		(1 << 28)

#define SMMU_PTE_READABLE	(1 << 31)
#define SMMU_PTE_WRITABLE	(1 << 30)
#define SMMU_PTE_NONSECURE	(1 << 29)

#define SMMU_PDE_ATTR		(SMMU_PDE_READABLE | SMMU_PDE_WRITABLE | \
				 SMMU_PDE_NONSECURE)

static unsigned int iova_pd_index(unsigned long iova)
{
	return (iova >> SMMU_PDE_SHIFT) & (SMMU_NUM_PDE - 1);
}

static unsigned int iova_pt_index(unsigned long iova)
{
	return (iova >> SMMU_PTE_SHIFT) & (SMMU_NUM_PTE - 1);
}

static bool smmu_dma_addr_valid(struct tegra_smmu *smmu, dma_addr_t addr)
{
	addr >>= 12;
	return (addr & smmu->pfn_mask) == addr;
}

static dma_addr_t smmu_pde_to_dma(struct tegra_smmu *smmu, u32 pde)
{
	return (dma_addr_t)(pde & smmu->pfn_mask) << 12;
}

static void smmu_flush_ptc_all(struct tegra_smmu *smmu)
{
	smmu_writel(smmu, SMMU_PTC_FLUSH_TYPE_ALL, SMMU_PTC_FLUSH);
}

static inline void smmu_flush_ptc(struct tegra_smmu *smmu, dma_addr_t dma,
				  unsigned long offset)
{
	u32 value;

	offset &= ~(smmu->mc->soc->atom_size - 1);

	if (smmu->mc->soc->num_address_bits > 32) {
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
		value = (dma >> 32) & SMMU_PTC_FLUSH_HI_MASK;
#else
		value = 0;
#endif
		smmu_writel(smmu, value, SMMU_PTC_FLUSH_HI);
	}

	value = (dma + offset) | SMMU_PTC_FLUSH_TYPE_ADR;
	smmu_writel(smmu, value, SMMU_PTC_FLUSH);
}

static inline void smmu_flush_tlb(struct tegra_smmu *smmu)
{
	smmu_writel(smmu, SMMU_TLB_FLUSH_VA_MATCH_ALL, SMMU_TLB_FLUSH);
}

static inline void smmu_flush_tlb_asid(struct tegra_smmu *smmu,
				       unsigned long asid)
{
	u32 value;

	if (smmu->soc->num_asids == 4)
		value = (asid & 0x3) << 29;
	else
		value = (asid & 0x7f) << 24;

	value |= SMMU_TLB_FLUSH_ASID_MATCH | SMMU_TLB_FLUSH_VA_MATCH_ALL;
	smmu_writel(smmu, value, SMMU_TLB_FLUSH);
}

static inline void smmu_flush_tlb_section(struct tegra_smmu *smmu,
					  unsigned long asid,
					  unsigned long iova)
{
	u32 value;

	if (smmu->soc->num_asids == 4)
		value = (asid & 0x3) << 29;
	else
		value = (asid & 0x7f) << 24;

	value |= SMMU_TLB_FLUSH_ASID_MATCH | SMMU_TLB_FLUSH_VA_SECTION(iova);
	smmu_writel(smmu, value, SMMU_TLB_FLUSH);
}

static inline void smmu_flush_tlb_group(struct tegra_smmu *smmu,
					unsigned long asid,
					unsigned long iova)
{
	u32 value;

	if (smmu->soc->num_asids == 4)
		value = (asid & 0x3) << 29;
	else
		value = (asid & 0x7f) << 24;

	value |= SMMU_TLB_FLUSH_ASID_MATCH | SMMU_TLB_FLUSH_VA_GROUP(iova);
	smmu_writel(smmu, value, SMMU_TLB_FLUSH);
}

static inline void smmu_flush(struct tegra_smmu *smmu)
{
	smmu_readl(smmu, SMMU_PTB_ASID);
}

static int tegra_smmu_alloc_asid(struct tegra_smmu *smmu, unsigned int *idp)
{
	unsigned long id;

	id = find_first_zero_bit(smmu->asids, smmu->soc->num_asids);
	if (id >= smmu->soc->num_asids)
		return -ENOSPC;

	set_bit(id, smmu->asids);
	*idp = id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void __smmu_iommu_unmap(struct smmu_as *as, dma_addr_t iova)
{
	unsigned long *pte;
	struct page *page;
	unsigned int *count;

	pte = locate_pte(as, iova, false, &page, &count);
	if (WARN_ON(!pte))
		return;

	if (WARN_ON(*pte == _PTE_VACANT(iova)))
		return;

	*pte = _PTE_VACANT(iova);
	FLUSH_CPU_DCACHE(pte, page, sizeof(*pte));
	flush_ptc_and_tlb(as->smmu, as, iova, pte, page, 0);
	if (!--(*count)) {
		free_ptbl(as, iova);
		smmu_flush_regs(as->smmu, 0);
	}
}

static void __smmu_iommu_map_pfn(struct smmu_as *as, dma_addr_t iova,
				 unsigned long pfn)
{
	struct smmu_device *smmu = as->smmu;
	unsigned long *pte;
	unsigned int *count;
	struct page *page;

	pte = locate_pte(as, iova, true, &page, &count);
	if (WARN_ON(!pte))
		return;

	if (*pte == _PTE_VACANT(iova))
		(*count)++;
	*pte = SMMU_PFN_TO_PTE(pfn, as->pte_attr);
	if (unlikely((*pte == _PTE_VACANT(iova))))
		(*count)--;
	FLUSH_CPU_DCACHE(pte, page, sizeof(*pte));
	flush_ptc_and_tlb(smmu, as, iova, pte, page, 0);
	put_signature(as, iova, pfn);
}

static int smmu_iommu_map(struct iommu_domain *domain, unsigned long iova,
			  phys_addr_t pa, size_t bytes, int prot)
{
	struct smmu_as *as = domain->priv;
	unsigned long pfn = __phys_to_pfn(pa);
	unsigned long flags;

	dev_dbg(as->smmu->dev, "[%d] %08lx:%08x\n", as->asid, iova, pa);

	if (!pfn_valid(pfn))
		return -ENOMEM;

	spin_lock_irqsave(&as->lock, flags);
	__smmu_iommu_map_pfn(as, iova, pfn);
	spin_unlock_irqrestore(&as->lock, flags);
	return 0;
}

static size_t smmu_iommu_unmap(struct iommu_domain *domain, unsigned long iova,
			       size_t bytes)
{
	struct smmu_as *as = domain->priv;
	unsigned long flags;

	dev_dbg(as->smmu->dev, "[%d] %08lx\n", as->asid, iova);

	spin_lock_irqsave(&as->lock, flags);
	__smmu_iommu_unmap(as, iova);
	spin_unlock_irqrestore(&as->lock, flags);
	return SMMU_PAGE_SIZE;
}

static phys_addr_t smmu_iommu_iova_to_phys(struct iommu_domain *domain,
					   unsigned long iova)
{
	struct smmu_as *as = domain->priv;
	unsigned long *pte;
	unsigned int *count;
	struct page *page;
	unsigned long pfn;
	unsigned long flags;

	spin_lock_irqsave(&as->lock, flags);

	pte = locate_pte(as, iova, true, &page, &count);
	pfn = *pte & SMMU_PFN_MASK;
	WARN_ON(!pfn_valid(pfn));
	dev_dbg(as->smmu->dev,
		"iova:%08lx pfn:%08lx asid:%d\n", iova, pfn, as->asid);

	spin_unlock_irqrestore(&as->lock, flags);
	return PFN_PHYS(pfn);
}

static int smmu_iommu_domain_has_cap(struct iommu_domain *domain,
				     unsigned long cap)
{
	return 0;
}

static int smmu_iommu_attach_dev(struct iommu_domain *domain,
				 struct device *dev)
{
	struct smmu_as *as = domain->priv;
	struct smmu_device *smmu = as->smmu;
	struct smmu_client *client, *c;
	u32 map;
	int err;

	client = devm_kzalloc(smmu->dev, sizeof(*c), GFP_KERNEL);
	if (!client)
		return -ENOMEM;
	client->dev = dev;
	client->as = as;
	map = (unsigned long)dev->platform_data;
	if (!map)
		return -EINVAL;

	err = smmu_client_enable_hwgrp(client, map);
	if (err)
		goto err_hwgrp;

	spin_lock(&as->client_lock);
	list_for_each_entry(c, &as->client, list) {
		if (c->dev == dev) {
			dev_err(smmu->dev,
				"%s is already attached\n", dev_name(c->dev));
			err = -EINVAL;
			goto err_client;
		}
	}
	list_add(&client->list, &as->client);
	spin_unlock(&as->client_lock);

	/*
	 * Reserve "page zero" for AVP vectors using a common dummy
	 * page.
	 */
	if (map & HWG_AVPC) {
		struct page *page;

		page = as->smmu->avp_vector_page;
		__smmu_iommu_map_pfn(as, 0, page_to_pfn(page));

		pr_info("Reserve \"page zero\" for AVP vectors using a common dummy\n");
	}

	dev_dbg(smmu->dev, "%s is attached\n", dev_name(c->dev));
	return 0;

err_client:
	smmu_client_disable_hwgrp(client);
	spin_unlock(&as->client_lock);
err_hwgrp:
	devm_kfree(smmu->dev, client);
	return err;
}

static void smmu_iommu_detach_dev(struct iommu_domain *domain,
				  struct device *dev)
{
	struct smmu_as *as = domain->priv;
	struct smmu_device *smmu = as->smmu;
	struct smmu_client *c;

	spin_lock(&as->client_lock);

	list_for_each_entry(c, &as->client, list) {
		if (c->dev == dev) {
			smmu_client_disable_hwgrp(c);
			list_del(&c->list);
			devm_kfree(smmu->dev, c);
			c->as = NULL;
			dev_dbg(smmu->dev,
				"%s is detached\n", dev_name(c->dev));
			goto out;
		}
	}
	dev_err(smmu->dev, "Couldn't find %s\n", dev_name(c->dev));
out:
	spin_unlock(&as->client_lock);
}

static int smmu_iommu_domain_init(struct iommu_domain *domain)
{
	int i;
	unsigned long flags;
	struct smmu_as *as;
	struct smmu_device *smmu = smmu_handle;

	/* Look for a free AS with lock held */
	for  (i = 0; i < smmu->num_as; i++) {
		struct smmu_as *tmp = &smmu->as[i];

		spin_lock_irqsave(&tmp->lock, flags);
		if (!tmp->pdir_page) {
			as = tmp;
			goto found;
		}
		spin_unlock_irqrestore(&tmp->lock, flags);
	}
	dev_err(smmu->dev, "no free AS\n");
	return -ENODEV;

found:
	if (alloc_pdir(as) < 0)
		goto err_alloc_pdir;

	spin_lock(&smmu->lock);

	/* Update PDIR register */
	smmu_write(smmu, SMMU_PTB_ASID_CUR(as->asid), SMMU_PTB_ASID);
	smmu_write(smmu,
		   SMMU_MK_PDIR(as->pdir_page, as->pdir_attr), SMMU_PTB_DATA);
	FLUSH_SMMU_REGS(smmu);

	spin_unlock(&smmu->lock);

	spin_unlock_irqrestore(&as->lock, flags);
	domain->priv = as;

	dev_dbg(smmu->dev, "smmu_as@%p\n", as);
	return 0;

err_alloc_pdir:
	spin_unlock_irqrestore(&as->lock, flags);
	return -ENODEV;
}

static void smmu_iommu_domain_destroy(struct iommu_domain *domain)
{
	struct smmu_as *as = domain->priv;
	struct smmu_device *smmu = as->smmu;
	unsigned long flags;

	spin_lock_irqsave(&as->lock, flags);

	if (as->pdir_page) {
		spin_lock(&smmu->lock);
		smmu_write(smmu, SMMU_PTB_ASID_CUR(as->asid), SMMU_PTB_ASID);
		smmu_write(smmu, SMMU_PTB_DATA_RESET_VAL, SMMU_PTB_DATA);
		FLUSH_SMMU_REGS(smmu);
		spin_unlock(&smmu->lock);

		free_pdir(as);
	}

	if (!list_empty(&as->client)) {
		struct smmu_client *c;

		list_for_each_entry(c, &as->client, list)
			smmu_iommu_detach_dev(domain, c->dev);
	}

	spin_unlock_irqrestore(&as->lock, flags);

	domain->priv = NULL;
	dev_dbg(smmu->dev, "smmu_as@%p\n", as);
}

static struct iommu_ops smmu_iommu_ops = {
	.domain_init	= smmu_iommu_domain_init,
	.domain_destroy	= smmu_iommu_domain_destroy,
	.attach_dev	= smmu_iommu_attach_dev,
	.detach_dev	= smmu_iommu_detach_dev,
	.map		= smmu_iommu_map,
	.unmap		= smmu_iommu_unmap,
	.iova_to_phys	= smmu_iommu_iova_to_phys,
	.domain_has_cap	= smmu_iommu_domain_has_cap,
	.pgsize_bitmap	= SMMU_IOMMU_PGSIZES,
};

static int tegra_smmu_suspend(struct device *dev)
{
	struct smmu_device *smmu = dev_get_drvdata(dev);

	smmu->translation_enable_0 = smmu_read(smmu, SMMU_TRANSLATION_ENABLE_0);
	smmu->translation_enable_1 = smmu_read(smmu, SMMU_TRANSLATION_ENABLE_1);
	smmu->translation_enable_2 = smmu_read(smmu, SMMU_TRANSLATION_ENABLE_2);
	smmu->asid_security = smmu_read(smmu, SMMU_ASID_SECURITY);
	return 0;
}

static int tegra_smmu_resume(struct device *dev)
{
	struct smmu_device *smmu = dev_get_drvdata(dev);
	unsigned long flags;

	spin_lock_irqsave(&smmu->lock, flags);
	smmu_setup_regs(smmu);
	spin_unlock_irqrestore(&smmu->lock, flags);
	return 0;
}

static int tegra_smmu_probe(struct platform_device *pdev)
{
	struct smmu_device *smmu;
	struct resource *regs, *regs2, *window;
	struct device *dev = &pdev->dev;
	int i, err = 0;

	if (smmu_handle)
		return -EIO;

	BUILD_BUG_ON(PAGE_SHIFT != SMMU_PAGE_SHIFT);

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	regs2 = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	window = platform_get_resource(pdev, IORESOURCE_MEM, 2);
	if (!regs || !regs2 || !window) {
		dev_err(dev, "No SMMU resources\n");
		return -ENODEV;
	}

	smmu = devm_kzalloc(dev, sizeof(*smmu), GFP_KERNEL);
	if (!smmu) {
		dev_err(dev, "failed to allocate smmu_device\n");
		return -ENOMEM;
	}

	smmu->dev = dev;
	smmu->num_as = SMMU_NUM_ASIDS;
	smmu->iovmm_base = (unsigned long)window->start;
	smmu->page_count = resource_size(window) >> SMMU_PAGE_SHIFT;
	smmu->regs = devm_ioremap(dev, regs->start, resource_size(regs));
	smmu->regs_ahbarb = devm_ioremap(dev, regs2->start,
					 resource_size(regs2));
	if (!smmu->regs || !smmu->regs_ahbarb) {
		dev_err(dev, "failed to remap SMMU registers\n");
		err = -ENXIO;
		goto fail;
	}

	smmu->translation_enable_0 = ~0;
	smmu->translation_enable_1 = ~0;
	smmu->translation_enable_2 = ~0;
	smmu->asid_security = 0;

	smmu->as = devm_kzalloc(dev,
			sizeof(smmu->as[0]) * smmu->num_as, GFP_KERNEL);
	if (!smmu->as) {
		dev_err(dev, "failed to allocate smmu_as\n");
		err = -ENOMEM;
		goto fail;
	}

	for (i = 0; i < smmu->num_as; i++) {
		struct smmu_as *as = &smmu->as[i];

		as->smmu = smmu;
		as->asid = i;
		as->pdir_attr = _PDIR_ATTR;
		as->pde_attr = _PDE_ATTR;
		as->pte_attr = _PTE_ATTR;

		spin_lock_init(&as->lock);
		INIT_LIST_HEAD(&as->client);
	}
	spin_lock_init(&smmu->lock);
	smmu_setup_regs(smmu);
	platform_set_drvdata(pdev, smmu);

	smmu->avp_vector_page = alloc_page(GFP_KERNEL);
	if (!smmu->avp_vector_page)
		goto fail;

	smmu_handle = smmu;
	return 0;

fail:
	if (smmu->avp_vector_page)
		__free_page(smmu->avp_vector_page);
	if (smmu->regs)
		devm_iounmap(dev, smmu->regs);
	if (smmu->regs_ahbarb)
		devm_iounmap(dev, smmu->regs_ahbarb);
	if (smmu && smmu->as) {
		for (i = 0; i < smmu->num_as; i++) {
			if (smmu->as[i].pdir_page) {
				ClearPageReserved(smmu->as[i].pdir_page);
				__free_page(smmu->as[i].pdir_page);
			}
		}
		devm_kfree(dev, smmu->as);
	}
	devm_kfree(dev, smmu);
	return err;
}

static int tegra_smmu_remove(struct platform_device *pdev)
{
	struct smmu_device *smmu = platform_get_drvdata(pdev);
	struct device *dev = smmu->dev;

	smmu_write(smmu, SMMU_CONFIG_DISABLE, SMMU_CONFIG);
	platform_set_drvdata(pdev, NULL);
	if (smmu->as) {
		int i;

		for (i = 0; i < smmu->num_as; i++)
			free_pdir(&smmu->as[i]);
		devm_kfree(dev, smmu->as);
	}
	if (smmu->avp_vector_page)
		__free_page(smmu->avp_vector_page);
	if (smmu->regs)
		devm_iounmap(dev, smmu->regs);
	if (smmu->regs_ahbarb)
		devm_iounmap(dev, smmu->regs_ahbarb);
	devm_kfree(dev, smmu);
	smmu_handle = NULL;
	return 0;
}

const struct dev_pm_ops tegra_smmu_pm_ops = {
	.suspend	= tegra_smmu_suspend,
	.resume		= tegra_smmu_resume,
};

static struct platform_driver tegra_smmu_driver = {
	.probe		= tegra_smmu_probe,
	.remove		= tegra_smmu_remove,
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "tegra-smmu",
		.pm	= &tegra_smmu_pm_ops,
	},
};

static int __devinit tegra_smmu_init(void)
{
	bus_set_iommu(&platform_bus_type, &smmu_iommu_ops);
	return platform_driver_register(&tegra_smmu_driver);
}

static void __exit tegra_smmu_exit(void)
{
	platform_driver_unregister(&tegra_smmu_driver);
}

subsys_initcall(tegra_smmu_init);
module_exit(tegra_smmu_exit);

MODULE_DESCRIPTION("IOMMU API for SMMU in Tegra30");
MODULE_AUTHOR("Hiroshi DOYU <hdoyu@nvidia.com>");
MODULE_LICENSE("GPL v2");
=======
static void tegra_smmu_free_asid(struct tegra_smmu *smmu, unsigned int id)
{
	clear_bit(id, smmu->asids);
}

static struct iommu_domain *tegra_smmu_domain_alloc_paging(struct device *dev)
{
	struct tegra_smmu_as *as;

	as = kzalloc(sizeof(*as), GFP_KERNEL);
	if (!as)
		return NULL;

	as->attr = SMMU_PD_READABLE | SMMU_PD_WRITABLE | SMMU_PD_NONSECURE;

	as->pd = alloc_page(GFP_KERNEL | __GFP_DMA | __GFP_ZERO);
	if (!as->pd) {
		kfree(as);
		return NULL;
	}

	as->count = kcalloc(SMMU_NUM_PDE, sizeof(u32), GFP_KERNEL);
	if (!as->count) {
		__free_page(as->pd);
		kfree(as);
		return NULL;
	}

	as->pts = kcalloc(SMMU_NUM_PDE, sizeof(*as->pts), GFP_KERNEL);
	if (!as->pts) {
		kfree(as->count);
		__free_page(as->pd);
		kfree(as);
		return NULL;
	}

	spin_lock_init(&as->lock);

	/* setup aperture */
	as->domain.geometry.aperture_start = 0;
	as->domain.geometry.aperture_end = 0xffffffff;
	as->domain.geometry.force_aperture = true;

	return &as->domain;
}

static void tegra_smmu_domain_free(struct iommu_domain *domain)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);

	/* TODO: free page directory and page tables */

	WARN_ON_ONCE(as->use_count);
	kfree(as->count);
	kfree(as->pts);
	kfree(as);
}

static const struct tegra_smmu_swgroup *
tegra_smmu_find_swgroup(struct tegra_smmu *smmu, unsigned int swgroup)
{
	const struct tegra_smmu_swgroup *group = NULL;
	unsigned int i;

	for (i = 0; i < smmu->soc->num_swgroups; i++) {
		if (smmu->soc->swgroups[i].swgroup == swgroup) {
			group = &smmu->soc->swgroups[i];
			break;
		}
	}

	return group;
}

static void tegra_smmu_enable(struct tegra_smmu *smmu, unsigned int swgroup,
			      unsigned int asid)
{
	const struct tegra_smmu_swgroup *group;
	unsigned int i;
	u32 value;

	group = tegra_smmu_find_swgroup(smmu, swgroup);
	if (group) {
		value = smmu_readl(smmu, group->reg);
		value &= ~SMMU_ASID_MASK;
		value |= SMMU_ASID_VALUE(asid);
		value |= SMMU_ASID_ENABLE;
		smmu_writel(smmu, value, group->reg);
	} else {
		pr_warn("%s group from swgroup %u not found\n", __func__,
				swgroup);
		/* No point moving ahead if group was not found */
		return;
	}

	for (i = 0; i < smmu->soc->num_clients; i++) {
		const struct tegra_mc_client *client = &smmu->soc->clients[i];

		if (client->swgroup != swgroup)
			continue;

		value = smmu_readl(smmu, client->regs.smmu.reg);
		value |= BIT(client->regs.smmu.bit);
		smmu_writel(smmu, value, client->regs.smmu.reg);
	}
}

static void tegra_smmu_disable(struct tegra_smmu *smmu, unsigned int swgroup,
			       unsigned int asid)
{
	const struct tegra_smmu_swgroup *group;
	unsigned int i;
	u32 value;

	group = tegra_smmu_find_swgroup(smmu, swgroup);
	if (group) {
		value = smmu_readl(smmu, group->reg);
		value &= ~SMMU_ASID_MASK;
		value |= SMMU_ASID_VALUE(asid);
		value &= ~SMMU_ASID_ENABLE;
		smmu_writel(smmu, value, group->reg);
	}

	for (i = 0; i < smmu->soc->num_clients; i++) {
		const struct tegra_mc_client *client = &smmu->soc->clients[i];

		if (client->swgroup != swgroup)
			continue;

		value = smmu_readl(smmu, client->regs.smmu.reg);
		value &= ~BIT(client->regs.smmu.bit);
		smmu_writel(smmu, value, client->regs.smmu.reg);
	}
}

static int tegra_smmu_as_prepare(struct tegra_smmu *smmu,
				 struct tegra_smmu_as *as)
{
	u32 value;
	int err = 0;

	mutex_lock(&smmu->lock);

	if (as->use_count > 0) {
		as->use_count++;
		goto unlock;
	}

	as->pd_dma = dma_map_page(smmu->dev, as->pd, 0, SMMU_SIZE_PD,
				  DMA_TO_DEVICE);
	if (dma_mapping_error(smmu->dev, as->pd_dma)) {
		err = -ENOMEM;
		goto unlock;
	}

	/* We can't handle 64-bit DMA addresses */
	if (!smmu_dma_addr_valid(smmu, as->pd_dma)) {
		err = -ENOMEM;
		goto err_unmap;
	}

	err = tegra_smmu_alloc_asid(smmu, &as->id);
	if (err < 0)
		goto err_unmap;

	smmu_flush_ptc(smmu, as->pd_dma, 0);
	smmu_flush_tlb_asid(smmu, as->id);

	smmu_writel(smmu, as->id & 0x7f, SMMU_PTB_ASID);
	value = SMMU_PTB_DATA_VALUE(as->pd_dma, as->attr);
	smmu_writel(smmu, value, SMMU_PTB_DATA);
	smmu_flush(smmu);

	as->smmu = smmu;
	as->use_count++;

	mutex_unlock(&smmu->lock);

	return 0;

err_unmap:
	dma_unmap_page(smmu->dev, as->pd_dma, SMMU_SIZE_PD, DMA_TO_DEVICE);
unlock:
	mutex_unlock(&smmu->lock);

	return err;
}

static void tegra_smmu_as_unprepare(struct tegra_smmu *smmu,
				    struct tegra_smmu_as *as)
{
	mutex_lock(&smmu->lock);

	if (--as->use_count > 0) {
		mutex_unlock(&smmu->lock);
		return;
	}

	tegra_smmu_free_asid(smmu, as->id);

	dma_unmap_page(smmu->dev, as->pd_dma, SMMU_SIZE_PD, DMA_TO_DEVICE);

	as->smmu = NULL;

	mutex_unlock(&smmu->lock);
}

static int tegra_smmu_attach_dev(struct iommu_domain *domain,
				 struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct tegra_smmu *smmu = dev_iommu_priv_get(dev);
	struct tegra_smmu_as *as = to_smmu_as(domain);
	unsigned int index;
	int err;

	if (!fwspec)
		return -ENOENT;

	for (index = 0; index < fwspec->num_ids; index++) {
		err = tegra_smmu_as_prepare(smmu, as);
		if (err)
			goto disable;

		tegra_smmu_enable(smmu, fwspec->ids[index], as->id);
	}

	if (index == 0)
		return -ENODEV;

	return 0;

disable:
	while (index--) {
		tegra_smmu_disable(smmu, fwspec->ids[index], as->id);
		tegra_smmu_as_unprepare(smmu, as);
	}

	return err;
}

static int tegra_smmu_identity_attach(struct iommu_domain *identity_domain,
				      struct device *dev)
{
	struct iommu_domain *domain = iommu_get_domain_for_dev(dev);
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct tegra_smmu_as *as;
	struct tegra_smmu *smmu;
	unsigned int index;

	if (!fwspec)
		return -ENODEV;

	if (domain == identity_domain || !domain)
		return 0;

	as = to_smmu_as(domain);
	smmu = as->smmu;
	for (index = 0; index < fwspec->num_ids; index++) {
		tegra_smmu_disable(smmu, fwspec->ids[index], as->id);
		tegra_smmu_as_unprepare(smmu, as);
	}
	return 0;
}

static struct iommu_domain_ops tegra_smmu_identity_ops = {
	.attach_dev = tegra_smmu_identity_attach,
};

static struct iommu_domain tegra_smmu_identity_domain = {
	.type = IOMMU_DOMAIN_IDENTITY,
	.ops = &tegra_smmu_identity_ops,
};

static void tegra_smmu_set_pde(struct tegra_smmu_as *as, unsigned long iova,
			       u32 value)
{
	unsigned int pd_index = iova_pd_index(iova);
	struct tegra_smmu *smmu = as->smmu;
	u32 *pd = page_address(as->pd);
	unsigned long offset = pd_index * sizeof(*pd);

	/* Set the page directory entry first */
	pd[pd_index] = value;

	/* The flush the page directory entry from caches */
	dma_sync_single_range_for_device(smmu->dev, as->pd_dma, offset,
					 sizeof(*pd), DMA_TO_DEVICE);

	/* And flush the iommu */
	smmu_flush_ptc(smmu, as->pd_dma, offset);
	smmu_flush_tlb_section(smmu, as->id, iova);
	smmu_flush(smmu);
}

static u32 *tegra_smmu_pte_offset(struct page *pt_page, unsigned long iova)
{
	u32 *pt = page_address(pt_page);

	return pt + iova_pt_index(iova);
}

static u32 *tegra_smmu_pte_lookup(struct tegra_smmu_as *as, unsigned long iova,
				  dma_addr_t *dmap)
{
	unsigned int pd_index = iova_pd_index(iova);
	struct tegra_smmu *smmu = as->smmu;
	struct page *pt_page;
	u32 *pd;

	pt_page = as->pts[pd_index];
	if (!pt_page)
		return NULL;

	pd = page_address(as->pd);
	*dmap = smmu_pde_to_dma(smmu, pd[pd_index]);

	return tegra_smmu_pte_offset(pt_page, iova);
}

static u32 *as_get_pte(struct tegra_smmu_as *as, dma_addr_t iova,
		       dma_addr_t *dmap, struct page *page)
{
	unsigned int pde = iova_pd_index(iova);
	struct tegra_smmu *smmu = as->smmu;

	if (!as->pts[pde]) {
		dma_addr_t dma;

		dma = dma_map_page(smmu->dev, page, 0, SMMU_SIZE_PT,
				   DMA_TO_DEVICE);
		if (dma_mapping_error(smmu->dev, dma)) {
			__free_page(page);
			return NULL;
		}

		if (!smmu_dma_addr_valid(smmu, dma)) {
			dma_unmap_page(smmu->dev, dma, SMMU_SIZE_PT,
				       DMA_TO_DEVICE);
			__free_page(page);
			return NULL;
		}

		as->pts[pde] = page;

		tegra_smmu_set_pde(as, iova, SMMU_MK_PDE(dma, SMMU_PDE_ATTR |
							      SMMU_PDE_NEXT));

		*dmap = dma;
	} else {
		u32 *pd = page_address(as->pd);

		*dmap = smmu_pde_to_dma(smmu, pd[pde]);
	}

	return tegra_smmu_pte_offset(as->pts[pde], iova);
}

static void tegra_smmu_pte_get_use(struct tegra_smmu_as *as, unsigned long iova)
{
	unsigned int pd_index = iova_pd_index(iova);

	as->count[pd_index]++;
}

static void tegra_smmu_pte_put_use(struct tegra_smmu_as *as, unsigned long iova)
{
	unsigned int pde = iova_pd_index(iova);
	struct page *page = as->pts[pde];

	/*
	 * When no entries in this page table are used anymore, return the
	 * memory page to the system.
	 */
	if (--as->count[pde] == 0) {
		struct tegra_smmu *smmu = as->smmu;
		u32 *pd = page_address(as->pd);
		dma_addr_t pte_dma = smmu_pde_to_dma(smmu, pd[pde]);

		tegra_smmu_set_pde(as, iova, 0);

		dma_unmap_page(smmu->dev, pte_dma, SMMU_SIZE_PT, DMA_TO_DEVICE);
		__free_page(page);
		as->pts[pde] = NULL;
	}
}

static void tegra_smmu_set_pte(struct tegra_smmu_as *as, unsigned long iova,
			       u32 *pte, dma_addr_t pte_dma, u32 val)
{
	struct tegra_smmu *smmu = as->smmu;
	unsigned long offset = SMMU_OFFSET_IN_PAGE(pte);

	*pte = val;

	dma_sync_single_range_for_device(smmu->dev, pte_dma, offset,
					 4, DMA_TO_DEVICE);
	smmu_flush_ptc(smmu, pte_dma, offset);
	smmu_flush_tlb_group(smmu, as->id, iova);
	smmu_flush(smmu);
}

static struct page *as_get_pde_page(struct tegra_smmu_as *as,
				    unsigned long iova, gfp_t gfp,
				    unsigned long *flags)
{
	unsigned int pde = iova_pd_index(iova);
	struct page *page = as->pts[pde];

	/* at first check whether allocation needs to be done at all */
	if (page)
		return page;

	/*
	 * In order to prevent exhaustion of the atomic memory pool, we
	 * allocate page in a sleeping context if GFP flags permit. Hence
	 * spinlock needs to be unlocked and re-locked after allocation.
	 */
	if (gfpflags_allow_blocking(gfp))
		spin_unlock_irqrestore(&as->lock, *flags);

	page = alloc_page(gfp | __GFP_DMA | __GFP_ZERO);

	if (gfpflags_allow_blocking(gfp))
		spin_lock_irqsave(&as->lock, *flags);

	/*
	 * In a case of blocking allocation, a concurrent mapping may win
	 * the PDE allocation. In this case the allocated page isn't needed
	 * if allocation succeeded and the allocation failure isn't fatal.
	 */
	if (as->pts[pde]) {
		if (page)
			__free_page(page);

		page = as->pts[pde];
	}

	return page;
}

static int
__tegra_smmu_map(struct iommu_domain *domain, unsigned long iova,
		 phys_addr_t paddr, size_t size, int prot, gfp_t gfp,
		 unsigned long *flags)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);
	dma_addr_t pte_dma;
	struct page *page;
	u32 pte_attrs;
	u32 *pte;

	page = as_get_pde_page(as, iova, gfp, flags);
	if (!page)
		return -ENOMEM;

	pte = as_get_pte(as, iova, &pte_dma, page);
	if (!pte)
		return -ENOMEM;

	/* If we aren't overwriting a pre-existing entry, increment use */
	if (*pte == 0)
		tegra_smmu_pte_get_use(as, iova);

	pte_attrs = SMMU_PTE_NONSECURE;

	if (prot & IOMMU_READ)
		pte_attrs |= SMMU_PTE_READABLE;

	if (prot & IOMMU_WRITE)
		pte_attrs |= SMMU_PTE_WRITABLE;

	tegra_smmu_set_pte(as, iova, pte, pte_dma,
			   SMMU_PHYS_PFN(paddr) | pte_attrs);

	return 0;
}

static size_t
__tegra_smmu_unmap(struct iommu_domain *domain, unsigned long iova,
		   size_t size, struct iommu_iotlb_gather *gather)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);
	dma_addr_t pte_dma;
	u32 *pte;

	pte = tegra_smmu_pte_lookup(as, iova, &pte_dma);
	if (!pte || !*pte)
		return 0;

	tegra_smmu_set_pte(as, iova, pte, pte_dma, 0);
	tegra_smmu_pte_put_use(as, iova);

	return size;
}

static int tegra_smmu_map(struct iommu_domain *domain, unsigned long iova,
			  phys_addr_t paddr, size_t size, size_t count,
			  int prot, gfp_t gfp, size_t *mapped)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&as->lock, flags);
	ret = __tegra_smmu_map(domain, iova, paddr, size, prot, gfp, &flags);
	spin_unlock_irqrestore(&as->lock, flags);

	if (!ret)
		*mapped = size;

	return ret;
}

static size_t tegra_smmu_unmap(struct iommu_domain *domain, unsigned long iova,
			       size_t size, size_t count, struct iommu_iotlb_gather *gather)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);
	unsigned long flags;

	spin_lock_irqsave(&as->lock, flags);
	size = __tegra_smmu_unmap(domain, iova, size, gather);
	spin_unlock_irqrestore(&as->lock, flags);

	return size;
}

static phys_addr_t tegra_smmu_iova_to_phys(struct iommu_domain *domain,
					   dma_addr_t iova)
{
	struct tegra_smmu_as *as = to_smmu_as(domain);
	unsigned long pfn;
	dma_addr_t pte_dma;
	u32 *pte;

	pte = tegra_smmu_pte_lookup(as, iova, &pte_dma);
	if (!pte || !*pte)
		return 0;

	pfn = *pte & as->smmu->pfn_mask;

	return SMMU_PFN_PHYS(pfn) + SMMU_OFFSET_IN_PAGE(iova);
}

static struct tegra_smmu *tegra_smmu_find(struct device_node *np)
{
	struct platform_device *pdev;
	struct tegra_mc *mc;

	pdev = of_find_device_by_node(np);
	if (!pdev)
		return NULL;

	mc = platform_get_drvdata(pdev);
	if (!mc) {
		put_device(&pdev->dev);
		return NULL;
	}

	return mc->smmu;
}

static int tegra_smmu_configure(struct tegra_smmu *smmu, struct device *dev,
				const struct of_phandle_args *args)
{
	const struct iommu_ops *ops = smmu->iommu.ops;
	int err;

	err = iommu_fwspec_init(dev, &dev->of_node->fwnode, ops);
	if (err < 0) {
		dev_err(dev, "failed to initialize fwspec: %d\n", err);
		return err;
	}

	err = ops->of_xlate(dev, args);
	if (err < 0) {
		dev_err(dev, "failed to parse SW group ID: %d\n", err);
		iommu_fwspec_free(dev);
		return err;
	}

	return 0;
}

static struct iommu_device *tegra_smmu_probe_device(struct device *dev)
{
	struct device_node *np = dev->of_node;
	struct tegra_smmu *smmu = NULL;
	struct of_phandle_args args;
	unsigned int index = 0;
	int err;

	while (of_parse_phandle_with_args(np, "iommus", "#iommu-cells", index,
					  &args) == 0) {
		smmu = tegra_smmu_find(args.np);
		if (smmu) {
			err = tegra_smmu_configure(smmu, dev, &args);

			if (err < 0) {
				of_node_put(args.np);
				return ERR_PTR(err);
			}
		}

		of_node_put(args.np);
		index++;
	}

	smmu = dev_iommu_priv_get(dev);
	if (!smmu)
		return ERR_PTR(-ENODEV);

	return &smmu->iommu;
}

static const struct tegra_smmu_group_soc *
tegra_smmu_find_group(struct tegra_smmu *smmu, unsigned int swgroup)
{
	unsigned int i, j;

	for (i = 0; i < smmu->soc->num_groups; i++)
		for (j = 0; j < smmu->soc->groups[i].num_swgroups; j++)
			if (smmu->soc->groups[i].swgroups[j] == swgroup)
				return &smmu->soc->groups[i];

	return NULL;
}

static void tegra_smmu_group_release(void *iommu_data)
{
	struct tegra_smmu_group *group = iommu_data;
	struct tegra_smmu *smmu = group->smmu;

	mutex_lock(&smmu->lock);
	list_del(&group->list);
	mutex_unlock(&smmu->lock);
}

static struct iommu_group *tegra_smmu_device_group(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct tegra_smmu *smmu = dev_iommu_priv_get(dev);
	const struct tegra_smmu_group_soc *soc;
	unsigned int swgroup = fwspec->ids[0];
	struct tegra_smmu_group *group;
	struct iommu_group *grp;

	/* Find group_soc associating with swgroup */
	soc = tegra_smmu_find_group(smmu, swgroup);

	mutex_lock(&smmu->lock);

	/* Find existing iommu_group associating with swgroup or group_soc */
	list_for_each_entry(group, &smmu->groups, list)
		if ((group->swgroup == swgroup) || (soc && group->soc == soc)) {
			grp = iommu_group_ref_get(group->group);
			mutex_unlock(&smmu->lock);
			return grp;
		}

	group = devm_kzalloc(smmu->dev, sizeof(*group), GFP_KERNEL);
	if (!group) {
		mutex_unlock(&smmu->lock);
		return NULL;
	}

	INIT_LIST_HEAD(&group->list);
	group->swgroup = swgroup;
	group->smmu = smmu;
	group->soc = soc;

	if (dev_is_pci(dev))
		group->group = pci_device_group(dev);
	else
		group->group = generic_device_group(dev);

	if (IS_ERR(group->group)) {
		devm_kfree(smmu->dev, group);
		mutex_unlock(&smmu->lock);
		return NULL;
	}

	iommu_group_set_iommudata(group->group, group, tegra_smmu_group_release);
	if (soc)
		iommu_group_set_name(group->group, soc->name);
	list_add_tail(&group->list, &smmu->groups);
	mutex_unlock(&smmu->lock);

	return group->group;
}

static int tegra_smmu_of_xlate(struct device *dev,
			       const struct of_phandle_args *args)
{
	struct platform_device *iommu_pdev = of_find_device_by_node(args->np);
	struct tegra_mc *mc = platform_get_drvdata(iommu_pdev);
	u32 id = args->args[0];

	/*
	 * Note: we are here releasing the reference of &iommu_pdev->dev, which
	 * is mc->dev. Although some functions in tegra_smmu_ops may keep using
	 * its private data beyond this point, it's still safe to do so because
	 * the SMMU parent device is the same as the MC, so the reference count
	 * isn't strictly necessary.
	 */
	put_device(&iommu_pdev->dev);

	dev_iommu_priv_set(dev, mc->smmu);

	return iommu_fwspec_add_ids(dev, &id, 1);
}

static int tegra_smmu_def_domain_type(struct device *dev)
{
	/*
	 * FIXME: For now we want to run all translation in IDENTITY mode, due
	 * to some device quirks. Better would be to just quirk the troubled
	 * devices.
	 */
	return IOMMU_DOMAIN_IDENTITY;
}

static const struct iommu_ops tegra_smmu_ops = {
	.identity_domain = &tegra_smmu_identity_domain,
	.def_domain_type = &tegra_smmu_def_domain_type,
	.domain_alloc_paging = tegra_smmu_domain_alloc_paging,
	.probe_device = tegra_smmu_probe_device,
	.device_group = tegra_smmu_device_group,
	.of_xlate = tegra_smmu_of_xlate,
	.pgsize_bitmap = SZ_4K,
	.default_domain_ops = &(const struct iommu_domain_ops) {
		.attach_dev	= tegra_smmu_attach_dev,
		.map_pages	= tegra_smmu_map,
		.unmap_pages	= tegra_smmu_unmap,
		.iova_to_phys	= tegra_smmu_iova_to_phys,
		.free		= tegra_smmu_domain_free,
	}
};

static void tegra_smmu_ahb_enable(void)
{
	static const struct of_device_id ahb_match[] = {
		{ .compatible = "nvidia,tegra30-ahb", },
		{ }
	};
	struct device_node *ahb;

	ahb = of_find_matching_node(NULL, ahb_match);
	if (ahb) {
		tegra_ahb_enable_smmu(ahb);
		of_node_put(ahb);
	}
}

static int tegra_smmu_swgroups_show(struct seq_file *s, void *data)
{
	struct tegra_smmu *smmu = s->private;
	unsigned int i;
	u32 value;

	seq_printf(s, "swgroup    enabled  ASID\n");
	seq_printf(s, "------------------------\n");

	for (i = 0; i < smmu->soc->num_swgroups; i++) {
		const struct tegra_smmu_swgroup *group = &smmu->soc->swgroups[i];
		const char *status;
		unsigned int asid;

		value = smmu_readl(smmu, group->reg);

		if (value & SMMU_ASID_ENABLE)
			status = "yes";
		else
			status = "no";

		asid = value & SMMU_ASID_MASK;

		seq_printf(s, "%-9s  %-7s  %#04x\n", group->name, status,
			   asid);
	}

	return 0;
}

DEFINE_SHOW_ATTRIBUTE(tegra_smmu_swgroups);

static int tegra_smmu_clients_show(struct seq_file *s, void *data)
{
	struct tegra_smmu *smmu = s->private;
	unsigned int i;
	u32 value;

	seq_printf(s, "client       enabled\n");
	seq_printf(s, "--------------------\n");

	for (i = 0; i < smmu->soc->num_clients; i++) {
		const struct tegra_mc_client *client = &smmu->soc->clients[i];
		const char *status;

		value = smmu_readl(smmu, client->regs.smmu.reg);

		if (value & BIT(client->regs.smmu.bit))
			status = "yes";
		else
			status = "no";

		seq_printf(s, "%-12s %s\n", client->name, status);
	}

	return 0;
}

DEFINE_SHOW_ATTRIBUTE(tegra_smmu_clients);

static void tegra_smmu_debugfs_init(struct tegra_smmu *smmu)
{
	smmu->debugfs = debugfs_create_dir("smmu", NULL);

	debugfs_create_file("swgroups", S_IRUGO, smmu->debugfs, smmu,
			    &tegra_smmu_swgroups_fops);
	debugfs_create_file("clients", S_IRUGO, smmu->debugfs, smmu,
			    &tegra_smmu_clients_fops);
}

static void tegra_smmu_debugfs_exit(struct tegra_smmu *smmu)
{
	debugfs_remove_recursive(smmu->debugfs);
}

struct tegra_smmu *tegra_smmu_probe(struct device *dev,
				    const struct tegra_smmu_soc *soc,
				    struct tegra_mc *mc)
{
	struct tegra_smmu *smmu;
	u32 value;
	int err;

	smmu = devm_kzalloc(dev, sizeof(*smmu), GFP_KERNEL);
	if (!smmu)
		return ERR_PTR(-ENOMEM);

	/*
	 * This is a bit of a hack. Ideally we'd want to simply return this
	 * value. However iommu_device_register() will attempt to add
	 * all devices to the IOMMU before we get that far. In order
	 * not to rely on global variables to track the IOMMU instance, we
	 * set it here so that it can be looked up from the .probe_device()
	 * callback via the IOMMU device's .drvdata field.
	 */
	mc->smmu = smmu;

	smmu->asids = devm_bitmap_zalloc(dev, soc->num_asids, GFP_KERNEL);
	if (!smmu->asids)
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&smmu->groups);
	mutex_init(&smmu->lock);

	smmu->regs = mc->regs;
	smmu->soc = soc;
	smmu->dev = dev;
	smmu->mc = mc;

	smmu->pfn_mask =
		BIT_MASK(mc->soc->num_address_bits - SMMU_PTE_SHIFT) - 1;
	dev_dbg(dev, "address bits: %u, PFN mask: %#lx\n",
		mc->soc->num_address_bits, smmu->pfn_mask);
	smmu->tlb_mask = (1 << fls(smmu->soc->num_tlb_lines)) - 1;
	dev_dbg(dev, "TLB lines: %u, mask: %#lx\n", smmu->soc->num_tlb_lines,
		smmu->tlb_mask);

	value = SMMU_PTC_CONFIG_ENABLE | SMMU_PTC_CONFIG_INDEX_MAP(0x3f);

	if (soc->supports_request_limit)
		value |= SMMU_PTC_CONFIG_REQ_LIMIT(8);

	smmu_writel(smmu, value, SMMU_PTC_CONFIG);

	value = SMMU_TLB_CONFIG_HIT_UNDER_MISS |
		SMMU_TLB_CONFIG_ACTIVE_LINES(smmu);

	if (soc->supports_round_robin_arbitration)
		value |= SMMU_TLB_CONFIG_ROUND_ROBIN_ARBITRATION;

	smmu_writel(smmu, value, SMMU_TLB_CONFIG);

	smmu_flush_ptc_all(smmu);
	smmu_flush_tlb(smmu);
	smmu_writel(smmu, SMMU_CONFIG_ENABLE, SMMU_CONFIG);
	smmu_flush(smmu);

	tegra_smmu_ahb_enable();

	err = iommu_device_sysfs_add(&smmu->iommu, dev, NULL, dev_name(dev));
	if (err)
		return ERR_PTR(err);

	err = iommu_device_register(&smmu->iommu, &tegra_smmu_ops, dev);
	if (err) {
		iommu_device_sysfs_remove(&smmu->iommu);
		return ERR_PTR(err);
	}

	if (IS_ENABLED(CONFIG_DEBUG_FS))
		tegra_smmu_debugfs_init(smmu);

	return smmu;
}

void tegra_smmu_remove(struct tegra_smmu *smmu)
{
	iommu_device_unregister(&smmu->iommu);
	iommu_device_sysfs_remove(&smmu->iommu);

	if (IS_ENABLED(CONFIG_DEBUG_FS))
		tegra_smmu_debugfs_exit(smmu);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
