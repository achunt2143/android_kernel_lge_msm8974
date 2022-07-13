<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-dove/common.h
 *
 * Core functions for Marvell Dove 88AP510 System On Chip
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ARCH_DOVE_COMMON_H
#define __ARCH_DOVE_COMMON_H

<<<<<<< HEAD
struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

extern struct sys_timer dove_timer;
=======
#include <linux/reboot.h>

struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

extern void dove_timer_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Basic Dove init functions used early by machine-setup.
 */
void dove_map_io(void);
void dove_init(void);
void dove_init_early(void);
void dove_init_irq(void);
<<<<<<< HEAD
void dove_setup_cpu_mbus(void);
void dove_ge00_init(struct mv643xx_eth_platform_data *eth_data);
void dove_sata_init(struct mv_sata_platform_data *sata_data);
void dove_pcie_init(int init_port0, int init_port1);
=======
void dove_setup_cpu_wins(void);
void dove_ge00_init(struct mv643xx_eth_platform_data *eth_data);
void dove_sata_init(struct mv_sata_platform_data *sata_data);
#ifdef CONFIG_PCI
void dove_pcie_init(int init_port0, int init_port1);
#else
static inline void dove_pcie_init(int init_port0, int init_port1) { }
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void dove_ehci0_init(void);
void dove_ehci1_init(void);
void dove_uart0_init(void);
void dove_uart1_init(void);
void dove_uart2_init(void);
void dove_uart3_init(void);
void dove_spi0_init(void);
void dove_spi1_init(void);
void dove_i2c_init(void);
void dove_sdio0_init(void);
void dove_sdio1_init(void);
<<<<<<< HEAD
void dove_restart(char, const char *);
=======
void dove_restart(enum reboot_mode, const char *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
