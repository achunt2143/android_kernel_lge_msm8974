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

#ifndef __MEM_H__
#define __MEM_H__

extern int phys_mapping(unsigned long phys, unsigned long long *offset_out);

extern unsigned long uml_physmem;
<<<<<<< HEAD
static inline unsigned long to_phys(void *virt)
=======
static inline unsigned long uml_to_phys(void *virt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return(((unsigned long) virt) - uml_physmem);
}

<<<<<<< HEAD
static inline void *to_virt(unsigned long phys)
=======
static inline void *uml_to_virt(unsigned long phys)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return((void *) uml_physmem + phys);
}

#endif
