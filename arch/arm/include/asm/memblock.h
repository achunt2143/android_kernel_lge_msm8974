<<<<<<< HEAD
#ifndef _ASM_ARM_MEMBLOCK_H
#define _ASM_ARM_MEMBLOCK_H

struct meminfo;
struct machine_desc;

extern void arm_memblock_init(struct meminfo *, struct machine_desc *);

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ARM_MEMBLOCK_H
#define _ASM_ARM_MEMBLOCK_H

struct machine_desc;

void arm_memblock_init(const struct machine_desc *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
phys_addr_t arm_memblock_steal(phys_addr_t size, phys_addr_t align);

#endif
