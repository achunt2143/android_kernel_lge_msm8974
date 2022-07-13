<<<<<<< HEAD
/* 
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ARCH_UM_MMU_H
#define __ARCH_UM_MMU_H

<<<<<<< HEAD
#include "mm_id.h"
=======
#include <mm_id.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/mm_context.h>

typedef struct mm_context {
	struct mm_id id;
	struct uml_arch_mm_context arch;
<<<<<<< HEAD
	struct page *stub_pages[2];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} mm_context_t;

extern void __switch_mm(struct mm_id * mm_idp);

/* Avoid tangled inclusion with asm/ldt.h */
extern long init_new_ldt(struct mm_context *to_mm, struct mm_context *from_mm);
extern void free_ldt(struct mm_context *mm);

#endif
