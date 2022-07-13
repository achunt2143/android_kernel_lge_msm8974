<<<<<<< HEAD
/*
 * Copyright STMicroelectronics, 2007.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright STMicroelectronics, 2007.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/amba/bus.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <plat/gpio-nomadik.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/mach/map.h>
#include <asm/hardware/vic.h>

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>

#include "clock.h"
#include "cpu-8815.h"

#define __MEM_4K_RESOURCE(x) \
	.res = {.start = (x), .end = (x) + SZ_4K - 1, .flags = IORESOURCE_MEM}

/* The 8815 has 4 GPIO blocks, let's register them immediately */

#define GPIO_RESOURCE(block)						\
	{								\
		.start	= NOMADIK_GPIO##block##_BASE,			\
		.end	= NOMADIK_GPIO##block##_BASE + SZ_4K - 1,	\
		.flags	= IORESOURCE_MEM,				\
	},								\
	{								\
		.start	= IRQ_GPIO##block,				\
		.end	= IRQ_GPIO##block,				\
		.flags	= IORESOURCE_IRQ,				\
	}

#define GPIO_DEVICE(block)						\
	{								\
		.name 		= "gpio",				\
		.id		= block,				\
		.num_resources 	= 2,					\
		.resource	= &cpu8815_gpio_resources[block * 2],	\
		.dev = {						\
			.platform_data = &cpu8815_gpio[block],		\
		},							\
	}

static struct nmk_gpio_platform_data cpu8815_gpio[] = {
	{
		.name = "GPIO-0-31",
		.first_gpio = 0,
		.first_irq = NOMADIK_GPIO_TO_IRQ(0),
	}, {
		.name = "GPIO-32-63",
		.first_gpio = 32,
		.first_irq = NOMADIK_GPIO_TO_IRQ(32),
	}, {
		.name = "GPIO-64-95",
		.first_gpio = 64,
		.first_irq = NOMADIK_GPIO_TO_IRQ(64),
	}, {
		.name = "GPIO-96-127", /* 124..127 not routed to pin */
		.first_gpio = 96,
		.first_irq = NOMADIK_GPIO_TO_IRQ(96),
	}
};

static struct resource cpu8815_gpio_resources[] = {
	GPIO_RESOURCE(0),
	GPIO_RESOURCE(1),
	GPIO_RESOURCE(2),
	GPIO_RESOURCE(3),
};

static struct platform_device cpu8815_platform_gpio[] = {
	GPIO_DEVICE(0),
	GPIO_DEVICE(1),
	GPIO_DEVICE(2),
	GPIO_DEVICE(3),
};

static AMBA_APB_DEVICE(cpu8815_amba_rng, "rng", 0, NOMADIK_RNG_BASE, { }, NULL);

static struct platform_device *platform_devs[] __initdata = {
	cpu8815_platform_gpio + 0,
	cpu8815_platform_gpio + 1,
	cpu8815_platform_gpio + 2,
	cpu8815_platform_gpio + 3,
};

static struct amba_device *amba_devs[] __initdata = {
	&cpu8815_amba_rng_device
};

static int __init cpu8815_init(void)
{
	int i;

	platform_add_devices(platform_devs, ARRAY_SIZE(platform_devs));
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++)
		amba_device_register(amba_devs[i], &iomem_resource);
	return 0;
}
arch_initcall(cpu8815_init);

/* All SoC devices live in the same area (see hardware.h) */
static struct map_desc nomadik_io_desc[] __initdata = {
	{
		.virtual =	NOMADIK_IO_VIRTUAL,
		.pfn =		__phys_to_pfn(NOMADIK_IO_PHYSICAL),
		.length =	NOMADIK_IO_SIZE,
		.type = 	MT_DEVICE,
	}
	/* static ram and secured ram may be added later */
};

void __init cpu8815_map_io(void)
{
	iotable_init(nomadik_io_desc, ARRAY_SIZE(nomadik_io_desc));
}

void __init cpu8815_init_irq(void)
{
	/* This modified VIC cell has two register blocks, at 0 and 0x20 */
	vic_init(io_p2v(NOMADIK_IC_BASE + 0x00), IRQ_VIC_START +  0, ~0, 0);
	vic_init(io_p2v(NOMADIK_IC_BASE + 0x20), IRQ_VIC_START + 32, ~0, 0);

	/*
	 * Init clocks here so that they are available for system timer
	 * initialization.
	 */
	clk_init();
}

/*
 * This function is called from the board init ("init_machine").
 */
 void __init cpu8815_platform_init(void)
{
#ifdef CONFIG_CACHE_L2X0
	/* At full speed latency must be >=2, so 0x249 in low bits */
	l2x0_init(io_p2v(NOMADIK_L2CC_BASE), 0x00730249, 0xfe000fff);
#endif
	 return;
}

void cpu8815_restart(char mode, const char *cmd)
{
	void __iomem *src_rstsr = io_p2v(NOMADIK_SRC_BASE + 0x18);
=======
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>

/*
 * These are the only hard-coded address offsets we still have to use.
 */
#define NOMADIK_FSMC_BASE	0x10100000	/* FSMC registers */
#define NOMADIK_SDRAMC_BASE	0x10110000	/* SDRAM Controller */
#define NOMADIK_CLCDC_BASE	0x10120000	/* CLCD Controller */
#define NOMADIK_MDIF_BASE	0x10120000	/* MDIF */
#define NOMADIK_DMA0_BASE	0x10130000	/* DMA0 Controller */
#define NOMADIK_IC_BASE		0x10140000	/* Vectored Irq Controller */
#define NOMADIK_DMA1_BASE	0x10150000	/* DMA1 Controller */
#define NOMADIK_USB_BASE	0x10170000	/* USB-OTG conf reg base */
#define NOMADIK_CRYP_BASE	0x10180000	/* Crypto processor */
#define NOMADIK_SHA1_BASE	0x10190000	/* SHA-1 Processor */
#define NOMADIK_XTI_BASE	0x101A0000	/* XTI */
#define NOMADIK_RNG_BASE	0x101B0000	/* Random number generator */
#define NOMADIK_SRC_BASE	0x101E0000	/* SRC base */
#define NOMADIK_WDOG_BASE	0x101E1000	/* Watchdog */
#define NOMADIK_MTU0_BASE	0x101E2000	/* Multiple Timer 0 */
#define NOMADIK_MTU1_BASE	0x101E3000	/* Multiple Timer 1 */
#define NOMADIK_GPIO0_BASE	0x101E4000	/* GPIO0 */
#define NOMADIK_GPIO1_BASE	0x101E5000	/* GPIO1 */
#define NOMADIK_GPIO2_BASE	0x101E6000	/* GPIO2 */
#define NOMADIK_GPIO3_BASE	0x101E7000	/* GPIO3 */
#define NOMADIK_RTC_BASE	0x101E8000	/* Real Time Clock base */
#define NOMADIK_PMU_BASE	0x101E9000	/* Power Management Unit */
#define NOMADIK_OWM_BASE	0x101EA000	/* One wire master */
#define NOMADIK_SCR_BASE	0x101EF000	/* Secure Control registers */
#define NOMADIK_MSP2_BASE	0x101F0000	/* MSP 2 interface */
#define NOMADIK_MSP1_BASE	0x101F1000	/* MSP 1 interface */
#define NOMADIK_UART2_BASE	0x101F2000	/* UART 2 interface */
#define NOMADIK_SSIRx_BASE	0x101F3000	/* SSI 8-ch rx interface */
#define NOMADIK_SSITx_BASE	0x101F4000	/* SSI 8-ch tx interface */
#define NOMADIK_MSHC_BASE	0x101F5000	/* Memory Stick(Pro) Host */
#define NOMADIK_SDI_BASE	0x101F6000	/* SD-card/MM-Card */
#define NOMADIK_I2C1_BASE	0x101F7000	/* I2C1 interface */
#define NOMADIK_I2C0_BASE	0x101F8000	/* I2C0 interface */
#define NOMADIK_MSP0_BASE	0x101F9000	/* MSP 0 interface */
#define NOMADIK_FIRDA_BASE	0x101FA000	/* FIrDA interface */
#define NOMADIK_UART1_BASE	0x101FB000	/* UART 1 interface */
#define NOMADIK_SSP_BASE	0x101FC000	/* SSP interface */
#define NOMADIK_UART0_BASE	0x101FD000	/* UART 0 interface */
#define NOMADIK_SGA_BASE	0x101FE000	/* SGA interface */
#define NOMADIK_L2CC_BASE	0x10210000	/* L2 Cache controller */
#define NOMADIK_UART1_VBASE	0xF01FB000

/* This is needed for LL-debug/earlyprintk/debug-macro.S */
static struct map_desc cpu8815_io_desc[] __initdata = {
	{
		.virtual =	NOMADIK_UART1_VBASE,
		.pfn =		__phys_to_pfn(NOMADIK_UART1_BASE),
		.length =	SZ_4K,
		.type =		MT_DEVICE,
	},
};

static void __init cpu8815_map_io(void)
{
	iotable_init(cpu8815_io_desc, ARRAY_SIZE(cpu8815_io_desc));
}

static void cpu8815_restart(enum reboot_mode mode, const char *cmd)
{
	void __iomem *srcbase = ioremap(NOMADIK_SRC_BASE, SZ_4K);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* FIXME: use egpio when implemented */

	/* Write anything to Reset status register */
<<<<<<< HEAD
	writel(1, src_rstsr);
}
=======
	writel(1, srcbase + 0x18);
}

static const char * cpu8815_board_compat[] = {
	"st,nomadik-nhk-15",
	"calaosystems,usb-s8815",
	NULL,
};

DT_MACHINE_START(NOMADIK_DT, "Nomadik STn8815")
	.l2c_aux_val	= 0,
	.l2c_aux_mask	= ~0,
	.map_io		= cpu8815_map_io,
	.restart	= cpu8815_restart,
	.dt_compat      = cpu8815_board_compat,
MACHINE_END
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
