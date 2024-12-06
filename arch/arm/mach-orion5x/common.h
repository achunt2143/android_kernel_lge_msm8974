<<<<<<< HEAD
#ifndef __ARCH_ORION5X_COMMON_H
#define __ARCH_ORION5X_COMMON_H

struct dsa_platform_data;
struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARCH_ORION5X_COMMON_H
#define __ARCH_ORION5X_COMMON_H

#include <linux/reboot.h>

struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

#define ORION_MBUS_PCIE_MEM_TARGET    0x04
#define ORION_MBUS_PCIE_MEM_ATTR      0x59
#define ORION_MBUS_PCIE_IO_TARGET     0x04
#define ORION_MBUS_PCIE_IO_ATTR       0x51
#define ORION_MBUS_PCIE_WA_TARGET     0x04
#define ORION_MBUS_PCIE_WA_ATTR       0x79
#define ORION_MBUS_PCI_MEM_TARGET     0x03
#define ORION_MBUS_PCI_MEM_ATTR       0x59
#define ORION_MBUS_PCI_IO_TARGET      0x03
#define ORION_MBUS_PCI_IO_ATTR        0x51
#define ORION_MBUS_DEVBUS_BOOT_TARGET 0x01
#define ORION_MBUS_DEVBUS_BOOT_ATTR   0x0f
#define ORION_MBUS_DEVBUS_TARGET(cs)  0x01
#define ORION_MBUS_DEVBUS_ATTR(cs)    (~(1 << cs))
#define ORION_MBUS_SRAM_TARGET        0x09
#define ORION_MBUS_SRAM_ATTR          0x00

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Basic Orion init functions used early by machine-setup.
 */
void orion5x_map_io(void);
void orion5x_init_early(void);
void orion5x_init_irq(void);
void orion5x_init(void);
<<<<<<< HEAD
extern int orion5x_tclk;
extern struct sys_timer orion5x_timer;

/*
 * Enumerations and functions for Orion windows mapping. Used by Orion core
 * functions to map its interfaces and by the machine-setup to map its on-
 * board devices. Details in /mach-orion/addr-map.c
 */
void orion5x_setup_cpu_mbus_bridge(void);
void orion5x_setup_dev_boot_win(u32 base, u32 size);
void orion5x_setup_dev0_win(u32 base, u32 size);
void orion5x_setup_dev1_win(u32 base, u32 size);
void orion5x_setup_dev2_win(u32 base, u32 size);
void orion5x_setup_pcie_wa_win(u32 base, u32 size);
void orion5x_setup_sram_win(void);
=======
void orion5x_id(u32 *dev, u32 *rev, char **dev_name);
void clk_init(void);
extern int orion5x_tclk;
extern void orion5x_timer_init(void);

void orion5x_setup_wins(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void orion5x_ehci0_init(void);
void orion5x_ehci1_init(void);
void orion5x_eth_init(struct mv643xx_eth_platform_data *eth_data);
<<<<<<< HEAD
void orion5x_eth_switch_init(struct dsa_platform_data *d, int irq);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void orion5x_i2c_init(void);
void orion5x_sata_init(struct mv_sata_platform_data *sata_data);
void orion5x_spi_init(void);
void orion5x_uart0_init(void);
void orion5x_uart1_init(void);
void orion5x_xor_init(void);
<<<<<<< HEAD
void orion5x_restart(char, const char *);
=======
void orion5x_restart(enum reboot_mode, const char *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * PCIe/PCI functions.
 */
struct pci_bus;
<<<<<<< HEAD
struct pci_sys_data;
=======
struct pci_host_bridge;
struct pci_sys_data;
struct pci_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void orion5x_pcie_id(u32 *dev, u32 *rev);
void orion5x_pci_disable(void);
void orion5x_pci_set_cardbus_mode(void);
int orion5x_pci_sys_setup(int nr, struct pci_sys_data *sys);
<<<<<<< HEAD
struct pci_bus *orion5x_pci_sys_scan_bus(int nr, struct pci_sys_data *sys);
int orion5x_pci_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);

struct meminfo;
struct tag;
extern void __init tag_fixup_mem32(struct tag *, char **, struct meminfo *);
=======
int orion5x_pci_sys_scan_bus(int nr, struct pci_host_bridge *bridge);
int orion5x_pci_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);

struct tag;
extern void __init tag_fixup_mem32(struct tag *, char **);

#ifdef CONFIG_MACH_MSS2_DT
extern void mss2_init(void);
#else
static inline void mss2_init(void) {}
#endif

#ifdef CONFIG_MACH_D2NET_DT
void d2net_init(void);
#else
static inline void d2net_init(void) {}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*****************************************************************************
 * Helpers to access Orion registers
 ****************************************************************************/
/*
 * These are not preempt-safe.  Locks, if needed, must be taken
 * care of by the caller.
 */
#define orion5x_setbits(r, mask)	writel(readl(r) | (mask), (r))
#define orion5x_clrbits(r, mask)	writel(readl(r) & ~(mask), (r))

#endif
