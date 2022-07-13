<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * CAAM/SEC 4.x driver backend
 * Private/internal definitions between modules
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 *
=======
 * Copyright 2019, 2023 NXP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef INTERN_H
#define INTERN_H

<<<<<<< HEAD
#define JOBR_UNASSIGNED 0
#define JOBR_ASSIGNED 1
=======
#include "ctrl.h"
#include <crypto/engine.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Currently comes from Kconfig param as a ^2 (driver-required) */
#define JOBR_DEPTH (1 << CONFIG_CRYPTO_DEV_FSL_CAAM_RINGSIZE)

<<<<<<< HEAD
=======
/*
 * Maximum size for crypto-engine software queue based on Job Ring
 * size (JOBR_DEPTH) and a THRESHOLD (reserved for the non-crypto-API
 * requests that are not passed through crypto-engine)
 */
#define THRESHOLD 15
#define CRYPTO_ENGINE_MAX_QLEN (JOBR_DEPTH - THRESHOLD)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Kconfig params for interrupt coalescing if selected (else zero) */
#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_INTC
#define JOBR_INTC JRCFG_ICEN
#define JOBR_INTC_TIME_THLD CONFIG_CRYPTO_DEV_FSL_CAAM_INTC_TIME_THLD
#define JOBR_INTC_COUNT_THLD CONFIG_CRYPTO_DEV_FSL_CAAM_INTC_COUNT_THLD
#else
#define JOBR_INTC 0
#define JOBR_INTC_TIME_THLD 0
#define JOBR_INTC_COUNT_THLD 0
#endif

/*
 * Storage for tracking each in-process entry moving across a ring
 * Each entry on an output ring needs one of these
 */
struct caam_jrentry_info {
	void (*callbk)(struct device *dev, u32 *desc, u32 status, void *arg);
	void *cbkarg;	/* Argument per ring entry */
	u32 *desc_addr_virt;	/* Stored virt addr for postprocessing */
	dma_addr_t desc_addr_dma;	/* Stored bus addr for done matching */
	u32 desc_size;	/* Stored size for postprocessing, header derived */
};

<<<<<<< HEAD
/* Private sub-storage for a single JobR */
struct caam_drv_private_jr {
	struct device *parentdev;	/* points back to controller dev */
	int ridx;
	struct caam_job_ring __iomem *rregs;	/* JobR's register space */
	struct tasklet_struct irqtask[NR_CPUS];
	int irq;			/* One per queue */
	int assign;			/* busy/free */

	/* Job ring info */
	int ringsize;	/* Size of rings (assume input = output) */
	struct caam_jrentry_info *entinfo;	/* Alloc'ed 1 per ring entry */
	spinlock_t inplock ____cacheline_aligned; /* Input ring index lock */
	int inp_ring_write_index;	/* Input index "tail" */
	int head;			/* entinfo (s/w ring) head index */
	dma_addr_t *inpring;	/* Base of input ring, alloc DMA-safe */
	spinlock_t outlock ____cacheline_aligned; /* Output ring index lock */
	int out_ring_read_index;	/* Output index "tail" */
	int tail;			/* entinfo (s/w ring) tail index */
	struct jr_outentry *outring;	/* Base of output ring, DMA-safe */
=======
struct caam_jr_state {
	dma_addr_t inpbusaddr;
	dma_addr_t outbusaddr;
};

struct caam_jr_dequeue_params {
	struct device *dev;
	int enable_itr;
};

/* Private sub-storage for a single JobR */
struct caam_drv_private_jr {
	struct list_head	list_node;	/* Job Ring device list */
	struct device		*dev;
	int ridx;
	struct caam_job_ring __iomem *rregs;	/* JobR's register space */
	struct tasklet_struct irqtask;
	struct caam_jr_dequeue_params tasklet_params;
	int irq;			/* One per queue */
	bool hwrng;

	/* Number of scatterlist crypt transforms active on the JobR */
	atomic_t tfm_count ____cacheline_aligned;

	/* Job ring info */
	struct caam_jrentry_info *entinfo;	/* Alloc'ed 1 per ring entry */
	spinlock_t inplock ____cacheline_aligned; /* Input ring index lock */
	u32 inpring_avail;	/* Number of free entries in input ring */
	int head;			/* entinfo (s/w ring) head index */
	void *inpring;			/* Base of input ring, alloc
					 * DMA-safe */
	int out_ring_read_index;	/* Output index "tail" */
	int tail;			/* entinfo (s/w ring) tail index */
	void *outring;			/* Base of output ring, DMA-safe */
	struct crypto_engine *engine;

	struct caam_jr_state state;	/* State of the JR during PM */
};

struct caam_ctl_state {
	struct masterid deco_mid[16];
	struct masterid jr_mid[4];
	u32 mcr;
	u32 scfgr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Driver-private storage for a single CAAM block instance
 */
struct caam_drv_private {
<<<<<<< HEAD

	struct device *dev;
	struct device **jrdev; /* Alloc'ed array per sub-device */
	spinlock_t jr_alloc_lock;
	struct platform_device *pdev;

	/* Physical-presence section */
	struct caam_ctrl *ctrl; /* controller region */
	struct caam_deco **deco; /* DECO/CCB views */
	struct caam_assurance *ac;
	struct caam_queue_if *qi; /* QI control region */
=======
	/* Physical-presence section */
	struct caam_ctrl __iomem *ctrl; /* controller region */
	struct caam_deco __iomem *deco; /* DECO/CCB views */
	struct caam_assurance __iomem *assure;
	struct caam_queue_if __iomem *qi; /* QI control region */
	struct caam_job_ring __iomem *jr[4];	/* JobR's register space */

	struct iommu_domain *domain;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Detected geometry block. Filled in from device tree if powerpc,
	 * or from register-based version detection code
	 */
	u8 total_jobrs;		/* Total Job Rings in device */
	u8 qi_present;		/* Nonzero if QI present in device */
<<<<<<< HEAD
	int secvio_irq;		/* Security violation interrupt number */

	/* which jr allocated to scatterlist crypto */
	atomic_t tfm_count ____cacheline_aligned;
	int num_jrs_for_algapi;
	struct device **algapi_jr;
	/* list of registered crypto algorithms (mk generic context handle?) */
	struct list_head alg_list;

=======
	u8 blob_present;	/* Nonzero if BLOB support present in device */
	u8 mc_en;		/* Nonzero if MC f/w is active */
	u8 optee_en;		/* Nonzero if OP-TEE f/w is active */
	bool pr_support;        /* RNG prediction resistance available */
	int secvio_irq;		/* Security violation interrupt number */
	int virt_en;		/* Virtualization enabled in CAAM */
	int era;		/* CAAM Era (internal HW revision) */

#define	RNG4_MAX_HANDLES 2
	/* RNG4 block */
	u32 rng4_sh_init;	/* This bitmap shows which of the State
				   Handles of the RNG4 block are initialized
				   by this driver */

	struct clk_bulk_data *clks;
	int num_clks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * debugfs entries for developer view into driver/device
	 * variables at runtime.
	 */
#ifdef CONFIG_DEBUG_FS
<<<<<<< HEAD
	struct dentry *dfs_root;
	struct dentry *ctl; /* controller dir */
	struct dentry *ctl_rq_dequeued, *ctl_ob_enc_req, *ctl_ib_dec_req;
	struct dentry *ctl_ob_enc_bytes, *ctl_ob_prot_bytes;
	struct dentry *ctl_ib_dec_bytes, *ctl_ib_valid_bytes;
	struct dentry *ctl_faultaddr, *ctl_faultdetail, *ctl_faultstatus;

	struct debugfs_blob_wrapper ctl_kek_wrap, ctl_tkek_wrap, ctl_tdsk_wrap;
	struct dentry *ctl_kek, *ctl_tkek, *ctl_tdsk;
#endif
};

void caam_jr_algapi_init(struct device *dev);
void caam_jr_algapi_remove(struct device *dev);
=======
	struct dentry *ctl; /* controller dir */
	struct debugfs_blob_wrapper ctl_kek_wrap, ctl_tkek_wrap, ctl_tdsk_wrap;
#endif

	int caam_off_during_pm;		/* If the CAAM is reset after suspend */
	struct caam_ctl_state state;	/* State of the CTL during PM */
};

#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_CRYPTO_API

int caam_algapi_init(struct device *dev);
void caam_algapi_exit(void);

#else

static inline int caam_algapi_init(struct device *dev)
{
	return 0;
}

static inline void caam_algapi_exit(void)
{
}

#endif /* CONFIG_CRYPTO_DEV_FSL_CAAM_CRYPTO_API */

#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_AHASH_API

int caam_algapi_hash_init(struct device *dev);
void caam_algapi_hash_exit(void);

#else

static inline int caam_algapi_hash_init(struct device *dev)
{
	return 0;
}

static inline void caam_algapi_hash_exit(void)
{
}

#endif /* CONFIG_CRYPTO_DEV_FSL_CAAM_AHASH_API */

#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_PKC_API

int caam_pkc_init(struct device *dev);
void caam_pkc_exit(void);

#else

static inline int caam_pkc_init(struct device *dev)
{
	return 0;
}

static inline void caam_pkc_exit(void)
{
}

#endif /* CONFIG_CRYPTO_DEV_FSL_CAAM_PKC_API */

#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_RNG_API

int caam_rng_init(struct device *dev);
void caam_rng_exit(struct device *dev);

#else

static inline int caam_rng_init(struct device *dev)
{
	return 0;
}

static inline void caam_rng_exit(struct device *dev) {}

#endif /* CONFIG_CRYPTO_DEV_FSL_CAAM_RNG_API */

#ifdef CONFIG_CRYPTO_DEV_FSL_CAAM_PRNG_API

int caam_prng_register(struct device *dev);
void caam_prng_unregister(void *data);

#else

static inline int caam_prng_register(struct device *dev)
{
	return 0;
}

static inline void caam_prng_unregister(void *data) {}
#endif /* CONFIG_CRYPTO_DEV_FSL_CAAM_PRNG_API */

#ifdef CONFIG_CAAM_QI

int caam_qi_algapi_init(struct device *dev);
void caam_qi_algapi_exit(void);

#else

static inline int caam_qi_algapi_init(struct device *dev)
{
	return 0;
}

static inline void caam_qi_algapi_exit(void)
{
}

#endif /* CONFIG_CAAM_QI */

static inline u64 caam_get_dma_mask(struct device *dev)
{
	struct device_node *nprop = dev->of_node;

	if (caam_ptr_sz != sizeof(u64))
		return DMA_BIT_MASK(32);

	if (caam_dpaa2)
		return DMA_BIT_MASK(49);

	if (of_device_is_compatible(nprop, "fsl,sec-v5.0-job-ring") ||
	    of_device_is_compatible(nprop, "fsl,sec-v5.0"))
		return DMA_BIT_MASK(40);

	return DMA_BIT_MASK(36);
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* INTERN_H */
