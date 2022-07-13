<<<<<<< HEAD
#ifndef _PARISC_MMU_H_
#define _PARISC_MMU_H_

/* On parisc, we store the space id here */
typedef unsigned long mm_context_t;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PARISC_MMU_H_
#define _PARISC_MMU_H_

typedef struct {
	unsigned long space_id;
	unsigned long vdso_base;
} mm_context_t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _PARISC_MMU_H_ */
