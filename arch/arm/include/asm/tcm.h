<<<<<<< HEAD
/*
 *
 * Copyright (C) 2008-2009 ST-Ericsson AB
 * License terms: GNU General Public License (GPL) version 2
 *
 * Author: Rickard Andersson <rickard.andersson@stericsson.com>
 * Author: Linus Walleij <linus.walleij@stericsson.com>
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *
 * Copyright (C) 2008-2009 ST-Ericsson AB
 *
 * Author: Rickard Andersson <rickard.andersson@stericsson.com>
 * Author: Linus Walleij <linus.walleij@stericsson.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASMARM_TCM_H
#define __ASMARM_TCM_H

<<<<<<< HEAD
#ifndef CONFIG_HAVE_TCM
#error "You should not be including tcm.h unless you have a TCM!"
#endif
=======
#ifdef CONFIG_HAVE_TCM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/compiler.h>

/* Tag variables with this */
<<<<<<< HEAD
#define __tcmdata __section(.tcm.data)
/* Tag constants with this */
#define __tcmconst __section(.tcm.rodata)
/* Tag functions inside TCM called from outside TCM with this */
#define __tcmfunc __attribute__((long_call)) __section(.tcm.text) noinline
/* Tag function inside TCM called from inside TCM  with this */
#define __tcmlocalfunc __section(.tcm.text)
=======
#define __tcmdata __section(".tcm.data")
/* Tag constants with this */
#define __tcmconst __section(".tcm.rodata")
/* Tag functions inside TCM called from outside TCM with this */
#define __tcmfunc __attribute__((long_call)) __section(".tcm.text") noinline
/* Tag function inside TCM called from inside TCM  with this */
#define __tcmlocalfunc __section(".tcm.text")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void *tcm_alloc(size_t len);
void tcm_free(void *addr, size_t len);
bool tcm_dtcm_present(void);
bool tcm_itcm_present(void);

<<<<<<< HEAD
=======
void __init tcm_init(void);
#else
/* No TCM support, just blank inlines to be optimized out */
static inline void tcm_init(void)
{
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
