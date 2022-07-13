<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef MFD_TMIO_H
#define MFD_TMIO_H

#include <linux/device.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/jiffies.h>
<<<<<<< HEAD
=======
#include <linux/mmc/card.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

#define tmio_ioread8(addr) readb(addr)
#define tmio_ioread16(addr) readw(addr)
#define tmio_ioread16_rep(r, b, l) readsw(r, b, l)
#define tmio_ioread32(addr) \
<<<<<<< HEAD
	(((u32) readw((addr))) | (((u32) readw((addr) + 2)) << 16))
=======
	(((u32)readw((addr))) | (((u32)readw((addr) + 2)) << 16))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define tmio_iowrite8(val, addr) writeb((val), (addr))
#define tmio_iowrite16(val, addr) writew((val), (addr))
#define tmio_iowrite16_rep(r, b, l) writesw(r, b, l)
#define tmio_iowrite32(val, addr) \
	do { \
<<<<<<< HEAD
	writew((val),       (addr)); \
	writew((val) >> 16, (addr) + 2); \
	} while (0)

#define CNF_CMD     0x04
#define CNF_CTL_BASE   0x10
#define CNF_INT_PIN  0x3d
#define CNF_STOP_CLK_CTL 0x40
#define CNF_GCLK_CTL 0x41
#define CNF_SD_CLK_MODE 0x42
#define CNF_PIN_STATUS 0x44
#define CNF_PWR_CTL_1 0x48
#define CNF_PWR_CTL_2 0x49
#define CNF_PWR_CTL_3 0x4a
#define CNF_CARD_DETECT_MODE 0x4c
#define CNF_SD_SLOT 0x50
#define CNF_EXT_GCLK_CTL_1 0xf0
#define CNF_EXT_GCLK_CTL_2 0xf1
#define CNF_EXT_GCLK_CTL_3 0xf9
#define CNF_SD_LED_EN_1 0xfa
#define CNF_SD_LED_EN_2 0xfe

#define   SDCREN 0x2   /* Enable access to MMC CTL regs. (flag in COMMAND_REG)*/

=======
		writew((val),       (addr)); \
		writew((val) >> 16, (addr) + 2); \
	} while (0)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define sd_config_write8(base, shift, reg, val) \
	tmio_iowrite8((val), (base) + ((reg) << (shift)))
#define sd_config_write16(base, shift, reg, val) \
	tmio_iowrite16((val), (base) + ((reg) << (shift)))
#define sd_config_write32(base, shift, reg, val) \
	do { \
		tmio_iowrite16((val), (base) + ((reg) << (shift)));   \
		tmio_iowrite16((val) >> 16, (base) + ((reg + 2) << (shift))); \
	} while (0)

/* tmio MMC platform flags */
<<<<<<< HEAD
#define TMIO_MMC_WRPROTECT_DISABLE	(1 << 0)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Some controllers can support a 2-byte block size when the bus width
 * is configured in 4-bit mode.
 */
<<<<<<< HEAD
#define TMIO_MMC_BLKSZ_2BYTES		(1 << 1)
/*
 * Some controllers can support SDIO IRQ signalling.
 */
#define TMIO_MMC_SDIO_IRQ		(1 << 2)
/*
 * Some platforms can detect card insertion events with controller powered
 * down, using a GPIO IRQ, in which case they have to fill in cd_irq, cd_gpio,
 * and cd_flags fields of struct tmio_mmc_data.
 */
#define TMIO_MMC_HAS_COLD_CD		(1 << 3)
=======
#define TMIO_MMC_BLKSZ_2BYTES		BIT(1)
/*
 * Some controllers can support SDIO IRQ signalling.
 */
#define TMIO_MMC_SDIO_IRQ		BIT(2)

/* Some features are only available or tested on R-Car Gen2 or later */
#define TMIO_MMC_MIN_RCAR2		BIT(3)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Some controllers require waiting for the SD bus to become
 * idle before writing to some registers.
 */
<<<<<<< HEAD
#define TMIO_MMC_HAS_IDLE_WAIT		(1 << 4)
/*
 * A GPIO is used for card hotplug detection. We need an extra flag for this,
 * because 0 is a valid GPIO number too, and requiring users to specify
 * cd_gpio < 0 to disable GPIO hotplug would break backwards compatibility.
 */
#define TMIO_MMC_USE_GPIO_CD		(1 << 5)

int tmio_core_mmc_enable(void __iomem *cnf, int shift, unsigned long base);
int tmio_core_mmc_resume(void __iomem *cnf, int shift, unsigned long base);
void tmio_core_mmc_pwr(void __iomem *cnf, int shift, int state);
void tmio_core_mmc_clk_div(void __iomem *cnf, int shift, int state);

struct tmio_mmc_dma {
	void *chan_priv_tx;
	void *chan_priv_rx;
	int alignment_shift;
};

struct tmio_mmc_host;
=======
#define TMIO_MMC_HAS_IDLE_WAIT		BIT(4)

/*
 * Use the busy timeout feature. Probably all TMIO versions support it. Yet,
 * we don't have documentation for old variants, so we enable only known good
 * variants with this flag. Can be removed once all variants are known good.
 */
#define TMIO_MMC_USE_BUSY_TIMEOUT	BIT(5)

/*
 * Some controllers have CMD12 automatically
 * issue/non-issue register
 */
#define TMIO_MMC_HAVE_CMD12_CTRL	BIT(7)

/* Controller has some SDIO status bits which must be 1 */
#define TMIO_MMC_SDIO_STATUS_SETBITS	BIT(8)

/*
 * Some controllers have a 32-bit wide data port register
 */
#define TMIO_MMC_32BIT_DATA_PORT	BIT(9)

/*
 * Some controllers allows to set SDx actual clock
 */
#define TMIO_MMC_CLK_ACTUAL		BIT(10)

/* Some controllers have a CBSY bit */
#define TMIO_MMC_HAVE_CBSY		BIT(11)

struct dma_chan;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * data for the MMC controller
 */
struct tmio_mmc_data {
<<<<<<< HEAD
	unsigned int			hclk;
	unsigned long			capabilities;
	unsigned long			flags;
	u32				ocr_mask;	/* available voltages */
	struct tmio_mmc_dma		*dma;
	struct device			*dev;
	unsigned int			cd_gpio;
	void (*set_pwr)(struct platform_device *host, int state);
	void (*set_clk_div)(struct platform_device *host, int state);
	int (*get_cd)(struct platform_device *host);
	int (*write16_hook)(struct tmio_mmc_host *host, int addr);
};

/*
 * This function is deprecated and will be removed soon. Please, convert your
 * platform to use drivers/mmc/core/cd-gpio.c
 */
#include <linux/mmc/host.h>
static inline void tmio_mmc_cd_wakeup(struct tmio_mmc_data *pdata)
{
	if (pdata)
		mmc_detect_change(dev_get_drvdata(pdata->dev),
				  msecs_to_jiffies(100));
}

/*
=======
	void				*chan_priv_tx;
	void				*chan_priv_rx;
	unsigned int			hclk;
	unsigned long			capabilities;
	unsigned long			capabilities2;
	unsigned long			flags;
	u32				ocr_mask;	/* available voltages */
	dma_addr_t			dma_rx_offset;
	unsigned int			max_blk_count;
	unsigned short			max_segs;
	void (*set_pwr)(struct platform_device *host, int state);
	void (*set_clk_div)(struct platform_device *host, int state);
};

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * data for the NAND controller
 */
struct tmio_nand_data {
	struct nand_bbt_descr	*badblock_pattern;
	struct mtd_partition	*partition;
	unsigned int		num_partitions;
<<<<<<< HEAD
=======
	const char *const	*part_parsers;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define FBIO_TMIO_ACC_WRITE	0x7C639300
#define FBIO_TMIO_ACC_SYNC	0x7C639301

struct tmio_fb_data {
	int			(*lcd_set_power)(struct platform_device *fb_dev,
<<<<<<< HEAD
								bool on);
	int			(*lcd_mode)(struct platform_device *fb_dev,
					const struct fb_videomode *mode);
=======
						 bool on);
	int			(*lcd_mode)(struct platform_device *fb_dev,
					    const struct fb_videomode *mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			num_modes;
	struct fb_videomode	*modes;

	/* in mm: size of screen */
	int			height;
	int			width;
};

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
