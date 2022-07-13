<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/arm/include/asm/mach/arch.h
 *
 *  Copyright (C) 2000 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASSEMBLY__

struct tag;
struct meminfo;
struct sys_timer;
=======
 */

#include <linux/types.h>

#ifndef __ASSEMBLY__
#include <linux/reboot.h>

struct tag;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct pt_regs;
struct smp_operations;
#ifdef CONFIG_SMP
#define smp_ops(ops) (&(ops))
<<<<<<< HEAD
#else
#define smp_ops(ops) (struct smp_operations *)NULL
=======
#define smp_init_ops(ops) (&(ops))
#else
#define smp_ops(ops) (struct smp_operations *)NULL
#define smp_init_ops(ops) (bool (*)(void))NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

struct machine_desc {
	unsigned int		nr;		/* architecture number	*/
	const char		*name;		/* architecture name	*/
	unsigned long		atag_offset;	/* tagged list (relative) */
	const char *const 	*dt_compat;	/* array of device tree
						 * 'compatible' strings	*/

	unsigned int		nr_irqs;	/* number of IRQs */

#ifdef CONFIG_ZONE_DMA
<<<<<<< HEAD
	unsigned long		dma_zone_size;	/* size of DMA-able area */
=======
	phys_addr_t		dma_zone_size;	/* size of DMA-able area */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	unsigned int		video_start;	/* start of video RAM	*/
	unsigned int		video_end;	/* end of video RAM	*/

	unsigned char		reserve_lp0 :1;	/* never has lp0	*/
	unsigned char		reserve_lp1 :1;	/* never has lp1	*/
	unsigned char		reserve_lp2 :1;	/* never has lp2	*/
<<<<<<< HEAD
	char			restart_mode;	/* default restart mode	*/
	struct smp_operations  *smp;    /* SMP operations  */
	void			(*fixup)(struct tag *, char **,
					 struct meminfo *);
	void			(*reserve)(void);/* reserve mem blocks	*/
	void			(*map_io)(void);/* IO mapping function	*/
	void			(*init_very_early)(void);
	void			(*init_early)(void);
	void			(*init_irq)(void);
	struct sys_timer	*timer;		/* system tick timer	*/
	void			(*init_machine)(void);
#ifdef CONFIG_MULTI_IRQ_HANDLER
	void			(*handle_irq)(struct pt_regs *);
#endif
	void			(*restart)(char, const char *);
=======
	enum reboot_mode	reboot_mode;	/* default restart mode	*/
	unsigned		l2c_aux_val;	/* L2 cache aux value	*/
	unsigned		l2c_aux_mask;	/* L2 cache aux mask	*/
	void			(*l2c_write_sec)(unsigned long, unsigned);
	const struct smp_operations	*smp;	/* SMP operations	*/
	bool			(*smp_init)(void);
	void			(*fixup)(struct tag *, char **);
	void			(*dt_fixup)(void);
	long long		(*pv_fixup)(void);
	void			(*reserve)(void);/* reserve mem blocks	*/
	void			(*map_io)(void);/* IO mapping function	*/
	void			(*init_early)(void);
	void			(*init_irq)(void);
	void			(*init_time)(void);
	void			(*init_machine)(void);
	void			(*init_late)(void);
	void			(*restart)(enum reboot_mode, const char *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Current machine - only accessible during boot.
 */
<<<<<<< HEAD
extern struct machine_desc *machine_desc;
=======
extern const struct machine_desc *machine_desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Machine type table - also only accessible during boot
 */
<<<<<<< HEAD
extern struct machine_desc __arch_info_begin[], __arch_info_end[];
=======
extern const struct machine_desc __arch_info_begin[], __arch_info_end[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define for_each_machine_desc(p)			\
	for (p = __arch_info_begin; p < __arch_info_end; p++)

/*
 * Set of macros to define architecture features.  This is built into
 * a table by the linker.
 */
#define MACHINE_START(_type,_name)			\
static const struct machine_desc __mach_desc_##_type	\
 __used							\
<<<<<<< HEAD
 __attribute__((__section__(".arch.info.init"))) = {	\
=======
 __section(".arch.info.init") = {			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.nr		= MACH_TYPE_##_type,		\
	.name		= _name,

#define MACHINE_END				\
};

#define DT_MACHINE_START(_name, _namestr)		\
static const struct machine_desc __mach_desc_##_name	\
 __used							\
<<<<<<< HEAD
 __attribute__((__section__(".arch.info.init"))) = {	\
=======
 __section(".arch.info.init") = {			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.nr		= ~0,				\
	.name		= _namestr,

#endif
