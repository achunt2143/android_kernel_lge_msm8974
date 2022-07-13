<<<<<<< HEAD
/**
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
 * This source file is released under GPL v2 license (no other versions).
 * See the COPYING file included in the main directory of this source
 * distribution for the license terms and conditions.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @File	ctresource.h
 *
 * @Brief
 * This file contains the definition of generic hardware resources for
 * resource management.
 *
 * @Author	Liu Chun
 * @Date 	May 13 2008
<<<<<<< HEAD
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef CTRESOURCE_H
#define CTRESOURCE_H

#include <linux/types.h>

enum RSCTYP {
	SRC,
	SRCIMP,
	AMIXER,
	SUM,
	DAIO,
	NUM_RSCTYP	/* This must be the last one and less than 16 */
};

struct rsc_ops;

struct rsc {
	u32 idx:12;	/* The index of a resource */
	u32 type:4;	/* The type (RSCTYP) of a resource */
	u32 conj:12;	/* Current conjugate index */
	u32 msr:4;	/* The Master Sample Rate a resource working on */
	void *ctrl_blk;	/* Chip specific control info block for a resource */
<<<<<<< HEAD
	void *hw;	/* Chip specific object for hardware access means */
	struct rsc_ops *ops;	/* Generic resource operations */
};

struct rsc_ops {
	int (*master)(struct rsc *rsc);	/* Move to master resource */
	int (*next_conj)(struct rsc *rsc); /* Move to next conjugate resource */
=======
	struct hw *hw;	/* Chip specific object for hardware access means */
	const struct rsc_ops *ops;	/* Generic resource operations */
};

struct rsc_ops {
	void (*master)(struct rsc *rsc); /* Move to master resource */
	void (*next_conj)(struct rsc *rsc); /* Move to next conjugate resource */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*index)(const struct rsc *rsc); /* Return the index of resource */
	/* Return the output slot number */
	int (*output_slot)(const struct rsc *rsc);
};

<<<<<<< HEAD
int rsc_init(struct rsc *rsc, u32 idx, enum RSCTYP type, u32 msr, void *hw);
=======
int
rsc_init(struct rsc *rsc, u32 idx, enum RSCTYP type, u32 msr, struct hw *hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rsc_uninit(struct rsc *rsc);

struct rsc_mgr {
	enum RSCTYP type; /* The type (RSCTYP) of resource to manage */
	unsigned int amount; /* The total amount of a kind of resource */
	unsigned int avail; /* The amount of currently available resources */
	unsigned char *rscs; /* The bit-map for resource allocation */
	void *ctrl_blk; /* Chip specific control info block */
<<<<<<< HEAD
	void *hw; /* Chip specific object for hardware access */
=======
	struct hw *hw; /* Chip specific object for hardware access */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Resource management is based on bit-map mechanism */
int rsc_mgr_init(struct rsc_mgr *mgr, enum RSCTYP type,
<<<<<<< HEAD
		 unsigned int amount, void *hw);
=======
		 unsigned int amount, struct hw *hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rsc_mgr_uninit(struct rsc_mgr *mgr);
int mgr_get_resource(struct rsc_mgr *mgr, unsigned int n, unsigned int *ridx);
int mgr_put_resource(struct rsc_mgr *mgr, unsigned int n, unsigned int idx);

#endif /* CTRESOURCE_H */
