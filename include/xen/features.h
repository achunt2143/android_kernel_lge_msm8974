<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 * features.h
 *
 * Query the features reported by Xen.
 *
 * Copyright (c) 2006, Ian Campbell
 */

#ifndef __XEN_FEATURES_H__
#define __XEN_FEATURES_H__

#include <xen/interface/features.h>

void xen_setup_features(void);

extern u8 xen_features[XENFEAT_NR_SUBMAPS * 32];

static inline int xen_feature(int flag)
{
	return xen_features[flag];
}

#endif /* __ASM_XEN_FEATURES_H__ */
