<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	sp5100_tco :	TCO timer driver for sp5100 chipsets
 *
 *	(c) Copyright 2009 Google Inc., All Rights Reserved.
 *
 *	Based on i8xx_tco.c:
 *	(c) Copyright 2000 kernel concepts <nils@kernelconcepts.de>, All Rights
 *	Reserved.
<<<<<<< HEAD
 *				http://www.kernelconcepts.de
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	See AMD Publication 43009 "AMD SB700/710/750 Register Reference Guide"
=======
 *				https://www.kernelconcepts.de
 *
 *	See AMD Publication 43009 "AMD SB700/710/750 Register Reference Guide",
 *	    AMD Publication 44413 "AMD SP5100 Register Reference Guide"
 *	    AMD Publication 45482 "AMD SB800-Series Southbridges Register
 *	                                                      Reference Guide"
 *	    AMD Publication 48751 "BIOS and Kernel Developer’s Guide (BKDG)
 *				for AMD Family 16h Models 00h-0Fh Processors"
 *	    AMD Publication 51192 "AMD Bolton FCH Register Reference Guide"
 *	    AMD Publication 52740 "BIOS and Kernel Developer’s Guide (BKDG)
 *				for AMD Family 16h Models 30h-3Fh Processors"
 *	    AMD Publication 55570-B1-PUB "Processor Programming Reference (PPR)
 *				for AMD Family 17h Model 18h, Revision B1
 *				Processors (PUB)
 *	    AMD Publication 55772-A1-PUB "Processor Programming Reference (PPR)
 *				for AMD Family 17h Model 20h, Revision A1
 *				Processors (PUB)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 *	Includes, defines, variables, module parameters, ...
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include "sp5100_tco.h"

/* Module and version information */
#define TCO_VERSION "0.01"
#define TCO_MODULE_NAME "SP5100 TCO timer"
#define TCO_DRIVER_NAME   TCO_MODULE_NAME ", v" TCO_VERSION

/* internal variables */
static u32 tcobase_phys;
static void __iomem *tcobase;
static unsigned int pm_iobase;
static DEFINE_SPINLOCK(tco_lock);	/* Guards the hardware */
static unsigned long timer_alive;
static char tco_expect_close;
static struct pci_dev *sp5100_tco_pci;

/* the watchdog platform device */
static struct platform_device *sp5100_tco_platform_device;

/* module parameters */

=======
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/watchdog.h>

#include "sp5100_tco.h"

#define TCO_DRIVER_NAME	"sp5100-tco"

/* internal variables */

enum tco_reg_layout {
	sp5100, sb800, efch, efch_mmio
};

struct sp5100_tco {
	struct watchdog_device wdd;
	void __iomem *tcobase;
	enum tco_reg_layout tco_reg_layout;
};

/* the watchdog platform device */
static struct platform_device *sp5100_tco_platform_device;
/* the associated PCI device */
static struct pci_dev *sp5100_tco_pci;

/* module parameters */

#define WATCHDOG_ACTION 0
static bool action = WATCHDOG_ACTION;
module_param(action, bool, 0);
MODULE_PARM_DESC(action, "Action taken when watchdog expires, 0 to reset, 1 to poweroff (default="
		 __MODULE_STRING(WATCHDOG_ACTION) ")");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define WATCHDOG_HEARTBEAT 60	/* 60 sec default heartbeat. */
static int heartbeat = WATCHDOG_HEARTBEAT;  /* in seconds */
module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds. (default="
		 __MODULE_STRING(WATCHDOG_HEARTBEAT) ")");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
<<<<<<< HEAD
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started"
=======
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started."
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		" (default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

/*
 * Some TCO specific functions
 */
<<<<<<< HEAD
static void tco_timer_start(void)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&tco_lock, flags);
	val = readl(SP5100_WDT_CONTROL(tcobase));
	val |= SP5100_WDT_START_STOP_BIT;
	writel(val, SP5100_WDT_CONTROL(tcobase));
	spin_unlock_irqrestore(&tco_lock, flags);
}

static void tco_timer_stop(void)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&tco_lock, flags);
	val = readl(SP5100_WDT_CONTROL(tcobase));
	val &= ~SP5100_WDT_START_STOP_BIT;
	writel(val, SP5100_WDT_CONTROL(tcobase));
	spin_unlock_irqrestore(&tco_lock, flags);
}

static void tco_timer_keepalive(void)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&tco_lock, flags);
	val = readl(SP5100_WDT_CONTROL(tcobase));
	val |= SP5100_WDT_TRIGGER_BIT;
	writel(val, SP5100_WDT_CONTROL(tcobase));
	spin_unlock_irqrestore(&tco_lock, flags);
}

static int tco_timer_set_heartbeat(int t)
{
	unsigned long flags;

	if (t < 0 || t > 0xffff)
		return -EINVAL;

	/* Write new heartbeat to watchdog */
	spin_lock_irqsave(&tco_lock, flags);
	writel(t, SP5100_WDT_COUNT(tcobase));
	spin_unlock_irqrestore(&tco_lock, flags);

	heartbeat = t;
	return 0;
}

/*
 *	/dev/watchdog handling
 */

static int sp5100_tco_open(struct inode *inode, struct file *file)
{
	/* /dev/watchdog can only be opened once */
	if (test_and_set_bit(0, &timer_alive))
		return -EBUSY;

	/* Reload and activate timer */
	tco_timer_start();
	tco_timer_keepalive();
	return nonseekable_open(inode, file);
}

static int sp5100_tco_release(struct inode *inode, struct file *file)
{
	/* Shut off the timer. */
	if (tco_expect_close == 42) {
		tco_timer_stop();
	} else {
		pr_crit("Unexpected close, not stopping watchdog!\n");
		tco_timer_keepalive();
	}
	clear_bit(0, &timer_alive);
	tco_expect_close = 0;
	return 0;
}

static ssize_t sp5100_tco_write(struct file *file, const char __user *data,
				size_t len, loff_t *ppos)
{
	/* See if we got the magic character 'V' and reload the timer */
	if (len) {
		if (!nowayout) {
			size_t i;

			/* note: just in case someone wrote the magic character
			 * five months ago... */
			tco_expect_close = 0;

			/* scan to see whether or not we got the magic character
			 */
			for (i = 0; i != len; i++) {
				char c;
				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					tco_expect_close = 42;
			}
		}

		/* someone wrote to us, we should reload the timer */
		tco_timer_keepalive();
	}
	return len;
}

static long sp5100_tco_ioctl(struct file *file, unsigned int cmd,
			     unsigned long arg)
{
	int new_options, retval = -EINVAL;
	int new_heartbeat;
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	static const struct watchdog_info ident = {
		.options =		WDIOF_SETTIMEOUT |
					WDIOF_KEEPALIVEPING |
					WDIOF_MAGICCLOSE,
		.firmware_version =	0,
		.identity =		TCO_MODULE_NAME,
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &ident,
			sizeof(ident)) ? -EFAULT : 0;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);
	case WDIOC_SETOPTIONS:
		if (get_user(new_options, p))
			return -EFAULT;
		if (new_options & WDIOS_DISABLECARD) {
			tco_timer_stop();
			retval = 0;
		}
		if (new_options & WDIOS_ENABLECARD) {
			tco_timer_start();
			tco_timer_keepalive();
			retval = 0;
		}
		return retval;
	case WDIOC_KEEPALIVE:
		tco_timer_keepalive();
		return 0;
	case WDIOC_SETTIMEOUT:
		if (get_user(new_heartbeat, p))
			return -EFAULT;
		if (tco_timer_set_heartbeat(new_heartbeat))
			return -EINVAL;
		tco_timer_keepalive();
		/* Fall through */
	case WDIOC_GETTIMEOUT:
		return put_user(heartbeat, p);
	default:
		return -ENOTTY;
	}
}

/*
 * Kernel Interfaces
 */

static const struct file_operations sp5100_tco_fops = {
	.owner =		THIS_MODULE,
	.llseek =		no_llseek,
	.write =		sp5100_tco_write,
	.unlocked_ioctl =	sp5100_tco_ioctl,
	.open =			sp5100_tco_open,
	.release =		sp5100_tco_release,
};

static struct miscdevice sp5100_tco_miscdev = {
	.minor =	WATCHDOG_MINOR,
	.name =		"watchdog",
	.fops =		&sp5100_tco_fops,
=======

static enum tco_reg_layout tco_reg_layout(struct pci_dev *dev)
{
	if (dev->vendor == PCI_VENDOR_ID_ATI &&
	    dev->device == PCI_DEVICE_ID_ATI_SBX00_SMBUS &&
	    dev->revision < 0x40) {
		return sp5100;
	} else if (dev->vendor == PCI_VENDOR_ID_AMD &&
	    sp5100_tco_pci->device == PCI_DEVICE_ID_AMD_KERNCZ_SMBUS &&
	    sp5100_tco_pci->revision >= AMD_ZEN_SMBUS_PCI_REV) {
		return efch_mmio;
	} else if ((dev->vendor == PCI_VENDOR_ID_AMD || dev->vendor == PCI_VENDOR_ID_HYGON) &&
	    ((dev->device == PCI_DEVICE_ID_AMD_HUDSON2_SMBUS &&
	     dev->revision >= 0x41) ||
	    (dev->device == PCI_DEVICE_ID_AMD_KERNCZ_SMBUS &&
	     dev->revision >= 0x49))) {
		return efch;
	}
	return sb800;
}

static int tco_timer_start(struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);
	u32 val;

	val = readl(SP5100_WDT_CONTROL(tco->tcobase));
	val |= SP5100_WDT_START_STOP_BIT;
	writel(val, SP5100_WDT_CONTROL(tco->tcobase));

	/* This must be a distinct write. */
	val |= SP5100_WDT_TRIGGER_BIT;
	writel(val, SP5100_WDT_CONTROL(tco->tcobase));

	return 0;
}

static int tco_timer_stop(struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);
	u32 val;

	val = readl(SP5100_WDT_CONTROL(tco->tcobase));
	val &= ~SP5100_WDT_START_STOP_BIT;
	writel(val, SP5100_WDT_CONTROL(tco->tcobase));

	return 0;
}

static int tco_timer_ping(struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);
	u32 val;

	val = readl(SP5100_WDT_CONTROL(tco->tcobase));
	val |= SP5100_WDT_TRIGGER_BIT;
	writel(val, SP5100_WDT_CONTROL(tco->tcobase));

	return 0;
}

static int tco_timer_set_timeout(struct watchdog_device *wdd,
				 unsigned int t)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);

	/* Write new heartbeat to watchdog */
	writel(t, SP5100_WDT_COUNT(tco->tcobase));

	wdd->timeout = t;

	return 0;
}

static unsigned int tco_timer_get_timeleft(struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);

	return readl(SP5100_WDT_COUNT(tco->tcobase));
}

static u8 sp5100_tco_read_pm_reg8(u8 index)
{
	outb(index, SP5100_IO_PM_INDEX_REG);
	return inb(SP5100_IO_PM_DATA_REG);
}

static void sp5100_tco_update_pm_reg8(u8 index, u8 reset, u8 set)
{
	u8 val;

	outb(index, SP5100_IO_PM_INDEX_REG);
	val = inb(SP5100_IO_PM_DATA_REG);
	val &= reset;
	val |= set;
	outb(val, SP5100_IO_PM_DATA_REG);
}

static void tco_timer_enable(struct sp5100_tco *tco)
{
	u32 val;

	switch (tco->tco_reg_layout) {
	case sb800:
		/* For SB800 or later */
		/* Set the Watchdog timer resolution to 1 sec */
		sp5100_tco_update_pm_reg8(SB800_PM_WATCHDOG_CONFIG,
					  0xff, SB800_PM_WATCHDOG_SECOND_RES);

		/* Enable watchdog decode bit and watchdog timer */
		sp5100_tco_update_pm_reg8(SB800_PM_WATCHDOG_CONTROL,
					  ~SB800_PM_WATCHDOG_DISABLE,
					  SB800_PCI_WATCHDOG_DECODE_EN);
		break;
	case sp5100:
		/* For SP5100 or SB7x0 */
		/* Enable watchdog decode bit */
		pci_read_config_dword(sp5100_tco_pci,
				      SP5100_PCI_WATCHDOG_MISC_REG,
				      &val);

		val |= SP5100_PCI_WATCHDOG_DECODE_EN;

		pci_write_config_dword(sp5100_tco_pci,
				       SP5100_PCI_WATCHDOG_MISC_REG,
				       val);

		/* Enable Watchdog timer and set the resolution to 1 sec */
		sp5100_tco_update_pm_reg8(SP5100_PM_WATCHDOG_CONTROL,
					  ~SP5100_PM_WATCHDOG_DISABLE,
					  SP5100_PM_WATCHDOG_SECOND_RES);
		break;
	case efch:
		/* Set the Watchdog timer resolution to 1 sec and enable */
		sp5100_tco_update_pm_reg8(EFCH_PM_DECODEEN3,
					  ~EFCH_PM_WATCHDOG_DISABLE,
					  EFCH_PM_DECODEEN_SECOND_RES);
		break;
	default:
		break;
	}
}

static u32 sp5100_tco_read_pm_reg32(u8 index)
{
	u32 val = 0;
	int i;

	for (i = 3; i >= 0; i--)
		val = (val << 8) + sp5100_tco_read_pm_reg8(index + i);

	return val;
}

static u32 sp5100_tco_request_region(struct device *dev,
				     u32 mmio_addr,
				     const char *dev_name)
{
	if (!devm_request_mem_region(dev, mmio_addr, SP5100_WDT_MEM_MAP_SIZE,
				     dev_name)) {
		dev_dbg(dev, "MMIO address 0x%08x already in use\n", mmio_addr);
		return 0;
	}

	return mmio_addr;
}

static u32 sp5100_tco_prepare_base(struct sp5100_tco *tco,
				   u32 mmio_addr,
				   u32 alt_mmio_addr,
				   const char *dev_name)
{
	struct device *dev = tco->wdd.parent;

	dev_dbg(dev, "Got 0x%08x from SBResource_MMIO register\n", mmio_addr);

	if (!mmio_addr && !alt_mmio_addr)
		return -ENODEV;

	/* Check for MMIO address and alternate MMIO address conflicts */
	if (mmio_addr)
		mmio_addr = sp5100_tco_request_region(dev, mmio_addr, dev_name);

	if (!mmio_addr && alt_mmio_addr)
		mmio_addr = sp5100_tco_request_region(dev, alt_mmio_addr, dev_name);

	if (!mmio_addr) {
		dev_err(dev, "Failed to reserve MMIO or alternate MMIO region\n");
		return -EBUSY;
	}

	tco->tcobase = devm_ioremap(dev, mmio_addr, SP5100_WDT_MEM_MAP_SIZE);
	if (!tco->tcobase) {
		dev_err(dev, "MMIO address 0x%08x failed mapping\n", mmio_addr);
		devm_release_mem_region(dev, mmio_addr, SP5100_WDT_MEM_MAP_SIZE);
		return -ENOMEM;
	}

	dev_info(dev, "Using 0x%08x for watchdog MMIO address\n", mmio_addr);

	return 0;
}

static int sp5100_tco_timer_init(struct sp5100_tco *tco)
{
	struct watchdog_device *wdd = &tco->wdd;
	struct device *dev = wdd->parent;
	u32 val;

	val = readl(SP5100_WDT_CONTROL(tco->tcobase));
	if (val & SP5100_WDT_DISABLED) {
		dev_err(dev, "Watchdog hardware is disabled\n");
		return -ENODEV;
	}

	/*
	 * Save WatchDogFired status, because WatchDogFired flag is
	 * cleared here.
	 */
	if (val & SP5100_WDT_FIRED)
		wdd->bootstatus = WDIOF_CARDRESET;

	/* Set watchdog action */
	if (action)
		val |= SP5100_WDT_ACTION_RESET;
	else
		val &= ~SP5100_WDT_ACTION_RESET;
	writel(val, SP5100_WDT_CONTROL(tco->tcobase));

	/* Set a reasonable heartbeat before we stop the timer */
	tco_timer_set_timeout(wdd, wdd->timeout);

	/*
	 * Stop the TCO before we change anything so we don't race with
	 * a zeroed timer.
	 */
	tco_timer_stop(wdd);

	return 0;
}

static u8 efch_read_pm_reg8(void __iomem *addr, u8 index)
{
	return readb(addr + index);
}

static void efch_update_pm_reg8(void __iomem *addr, u8 index, u8 reset, u8 set)
{
	u8 val;

	val = readb(addr + index);
	val &= reset;
	val |= set;
	writeb(val, addr + index);
}

static void tco_timer_enable_mmio(void __iomem *addr)
{
	efch_update_pm_reg8(addr, EFCH_PM_DECODEEN3,
			    ~EFCH_PM_WATCHDOG_DISABLE,
			    EFCH_PM_DECODEEN_SECOND_RES);
}

static int sp5100_tco_setupdevice_mmio(struct device *dev,
				       struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);
	const char *dev_name = SB800_DEVNAME;
	u32 mmio_addr = 0, alt_mmio_addr = 0;
	struct resource *res;
	void __iomem *addr;
	int ret;
	u32 val;

	res = request_mem_region_muxed(EFCH_PM_ACPI_MMIO_PM_ADDR,
				       EFCH_PM_ACPI_MMIO_PM_SIZE,
				       "sp5100_tco");

	if (!res) {
		dev_err(dev,
			"Memory region 0x%08x already in use\n",
			EFCH_PM_ACPI_MMIO_PM_ADDR);
		return -EBUSY;
	}

	addr = ioremap(EFCH_PM_ACPI_MMIO_PM_ADDR, EFCH_PM_ACPI_MMIO_PM_SIZE);
	if (!addr) {
		dev_err(dev, "Address mapping failed\n");
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * EFCH_PM_DECODEEN_WDT_TMREN is dual purpose. This bitfield
	 * enables sp5100_tco register MMIO space decoding. The bitfield
	 * also starts the timer operation. Enable if not already enabled.
	 */
	val = efch_read_pm_reg8(addr, EFCH_PM_DECODEEN);
	if (!(val & EFCH_PM_DECODEEN_WDT_TMREN)) {
		efch_update_pm_reg8(addr, EFCH_PM_DECODEEN, 0xff,
				    EFCH_PM_DECODEEN_WDT_TMREN);
	}

	/* Error if the timer could not be enabled */
	val = efch_read_pm_reg8(addr, EFCH_PM_DECODEEN);
	if (!(val & EFCH_PM_DECODEEN_WDT_TMREN)) {
		dev_err(dev, "Failed to enable the timer\n");
		ret = -EFAULT;
		goto out;
	}

	mmio_addr = EFCH_PM_WDT_ADDR;

	/* Determine alternate MMIO base address */
	val = efch_read_pm_reg8(addr, EFCH_PM_ISACONTROL);
	if (val & EFCH_PM_ISACONTROL_MMIOEN)
		alt_mmio_addr = EFCH_PM_ACPI_MMIO_ADDR +
			EFCH_PM_ACPI_MMIO_WDT_OFFSET;

	ret = sp5100_tco_prepare_base(tco, mmio_addr, alt_mmio_addr, dev_name);
	if (!ret) {
		tco_timer_enable_mmio(addr);
		ret = sp5100_tco_timer_init(tco);
	}

out:
	if (addr)
		iounmap(addr);

	release_resource(res);
	kfree(res);

	return ret;
}

static int sp5100_tco_setupdevice(struct device *dev,
				  struct watchdog_device *wdd)
{
	struct sp5100_tco *tco = watchdog_get_drvdata(wdd);
	const char *dev_name;
	u32 mmio_addr = 0, val;
	u32 alt_mmio_addr = 0;
	int ret;

	if (tco->tco_reg_layout == efch_mmio)
		return sp5100_tco_setupdevice_mmio(dev, wdd);

	/* Request the IO ports used by this driver */
	if (!request_muxed_region(SP5100_IO_PM_INDEX_REG,
				  SP5100_PM_IOPORTS_SIZE, "sp5100_tco")) {
		dev_err(dev, "I/O address 0x%04x already in use\n",
			SP5100_IO_PM_INDEX_REG);
		return -EBUSY;
	}

	/*
	 * Determine type of southbridge chipset.
	 */
	switch (tco->tco_reg_layout) {
	case sp5100:
		dev_name = SP5100_DEVNAME;
		mmio_addr = sp5100_tco_read_pm_reg32(SP5100_PM_WATCHDOG_BASE) &
								0xfffffff8;

		/*
		 * Secondly, find the watchdog timer MMIO address
		 * from SBResource_MMIO register.
		 */

		/* Read SBResource_MMIO from PCI config(PCI_Reg: 9Ch) */
		pci_read_config_dword(sp5100_tco_pci,
				      SP5100_SB_RESOURCE_MMIO_BASE,
				      &val);

		/* Verify MMIO is enabled and using bar0 */
		if ((val & SB800_ACPI_MMIO_MASK) == SB800_ACPI_MMIO_DECODE_EN)
			alt_mmio_addr = (val & ~0xfff) + SB800_PM_WDT_MMIO_OFFSET;
		break;
	case sb800:
		dev_name = SB800_DEVNAME;
		mmio_addr = sp5100_tco_read_pm_reg32(SB800_PM_WATCHDOG_BASE) &
								0xfffffff8;

		/* Read SBResource_MMIO from AcpiMmioEn(PM_Reg: 24h) */
		val = sp5100_tco_read_pm_reg32(SB800_PM_ACPI_MMIO_EN);

		/* Verify MMIO is enabled and using bar0 */
		if ((val & SB800_ACPI_MMIO_MASK) == SB800_ACPI_MMIO_DECODE_EN)
			alt_mmio_addr = (val & ~0xfff) + SB800_PM_WDT_MMIO_OFFSET;
		break;
	case efch:
		dev_name = SB800_DEVNAME;
		val = sp5100_tco_read_pm_reg8(EFCH_PM_DECODEEN);
		if (val & EFCH_PM_DECODEEN_WDT_TMREN)
			mmio_addr = EFCH_PM_WDT_ADDR;

		val = sp5100_tco_read_pm_reg8(EFCH_PM_ISACONTROL);
		if (val & EFCH_PM_ISACONTROL_MMIOEN)
			alt_mmio_addr = EFCH_PM_ACPI_MMIO_ADDR +
				EFCH_PM_ACPI_MMIO_WDT_OFFSET;
		break;
	default:
		return -ENODEV;
	}

	ret = sp5100_tco_prepare_base(tco, mmio_addr, alt_mmio_addr, dev_name);
	if (!ret) {
		/* Setup the watchdog timer */
		tco_timer_enable(tco);
		ret = sp5100_tco_timer_init(tco);
	}

	release_region(SP5100_IO_PM_INDEX_REG, SP5100_PM_IOPORTS_SIZE);
	return ret;
}

static struct watchdog_info sp5100_tco_wdt_info = {
	.identity = "SP5100 TCO timer",
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

static const struct watchdog_ops sp5100_tco_wdt_ops = {
	.owner = THIS_MODULE,
	.start = tco_timer_start,
	.stop = tco_timer_stop,
	.ping = tco_timer_ping,
	.set_timeout = tco_timer_set_timeout,
	.get_timeleft = tco_timer_get_timeleft,
};

static int sp5100_tco_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct watchdog_device *wdd;
	struct sp5100_tco *tco;
	int ret;

	tco = devm_kzalloc(dev, sizeof(*tco), GFP_KERNEL);
	if (!tco)
		return -ENOMEM;

	tco->tco_reg_layout = tco_reg_layout(sp5100_tco_pci);

	wdd = &tco->wdd;
	wdd->parent = dev;
	wdd->info = &sp5100_tco_wdt_info;
	wdd->ops = &sp5100_tco_wdt_ops;
	wdd->timeout = WATCHDOG_HEARTBEAT;
	wdd->min_timeout = 1;
	wdd->max_timeout = 0xffff;

	watchdog_init_timeout(wdd, heartbeat, NULL);
	watchdog_set_nowayout(wdd, nowayout);
	watchdog_stop_on_reboot(wdd);
	watchdog_stop_on_unregister(wdd);
	watchdog_set_drvdata(wdd, tco);

	ret = sp5100_tco_setupdevice(dev, wdd);
	if (ret)
		return ret;

	ret = devm_watchdog_register_device(dev, wdd);
	if (ret)
		return ret;

	/* Show module parameters */
	dev_info(dev, "initialized. heartbeat=%d sec (nowayout=%d)\n",
		 wdd->timeout, nowayout);

	return 0;
}

static struct platform_driver sp5100_tco_driver = {
	.probe		= sp5100_tco_probe,
	.driver		= {
		.name	= TCO_DRIVER_NAME,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Data for PCI driver interface
 *
 * This data only exists for exporting the supported
 * PCI ids via MODULE_DEVICE_TABLE.  We do not actually
 * register a pci_driver, because someone else might
 * want to register another driver on the same PCI id.
 */
<<<<<<< HEAD
static DEFINE_PCI_DEVICE_TABLE(sp5100_tco_pci_tbl) = {
	{ PCI_VENDOR_ID_ATI, PCI_DEVICE_ID_ATI_SBX00_SMBUS, PCI_ANY_ID,
	  PCI_ANY_ID, },
=======
static const struct pci_device_id sp5100_tco_pci_tbl[] = {
	{ PCI_VENDOR_ID_ATI, PCI_DEVICE_ID_ATI_SBX00_SMBUS, PCI_ANY_ID,
	  PCI_ANY_ID, },
	{ PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_HUDSON2_SMBUS, PCI_ANY_ID,
	  PCI_ANY_ID, },
	{ PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_KERNCZ_SMBUS, PCI_ANY_ID,
	  PCI_ANY_ID, },
	{ PCI_VENDOR_ID_HYGON, PCI_DEVICE_ID_AMD_KERNCZ_SMBUS, PCI_ANY_ID,
	  PCI_ANY_ID, },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0, },			/* End of list */
};
MODULE_DEVICE_TABLE(pci, sp5100_tco_pci_tbl);

<<<<<<< HEAD
/*
 * Init & exit routines
 */

static unsigned char __devinit sp5100_tco_setupdevice(void)
{
	struct pci_dev *dev = NULL;
	u32 val;
=======
static int __init sp5100_tco_init(void)
{
	struct pci_dev *dev = NULL;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Match the PCI device */
	for_each_pci_dev(dev) {
		if (pci_match_id(sp5100_tco_pci_tbl, dev) != NULL) {
			sp5100_tco_pci = dev;
			break;
		}
	}

	if (!sp5100_tco_pci)
<<<<<<< HEAD
		return 0;

	/* Request the IO ports used by this driver */
	pm_iobase = SP5100_IO_PM_INDEX_REG;
	if (!request_region(pm_iobase, SP5100_PM_IOPORTS_SIZE, "SP5100 TCO")) {
		pr_err("I/O address 0x%04x already in use\n", pm_iobase);
		goto exit;
	}

	/* Find the watchdog base address. */
	outb(SP5100_PM_WATCHDOG_BASE3, SP5100_IO_PM_INDEX_REG);
	val = inb(SP5100_IO_PM_DATA_REG);
	outb(SP5100_PM_WATCHDOG_BASE2, SP5100_IO_PM_INDEX_REG);
	val = val << 8 | inb(SP5100_IO_PM_DATA_REG);
	outb(SP5100_PM_WATCHDOG_BASE1, SP5100_IO_PM_INDEX_REG);
	val = val << 8 | inb(SP5100_IO_PM_DATA_REG);
	outb(SP5100_PM_WATCHDOG_BASE0, SP5100_IO_PM_INDEX_REG);
	/* Low three bits of BASE0 are reserved. */
	val = val << 8 | (inb(SP5100_IO_PM_DATA_REG) & 0xf8);

	if (!request_mem_region_exclusive(val, SP5100_WDT_MEM_MAP_SIZE,
								"SP5100 TCO")) {
		pr_err("mmio address 0x%04x already in use\n", val);
		goto unreg_region;
	}
	tcobase_phys = val;

	tcobase = ioremap(val, SP5100_WDT_MEM_MAP_SIZE);
	if (tcobase == 0) {
		pr_err("failed to get tcobase address\n");
		goto unreg_mem_region;
	}

	/* Enable watchdog decode bit */
	pci_read_config_dword(sp5100_tco_pci,
			      SP5100_PCI_WATCHDOG_MISC_REG,
			      &val);

	val |= SP5100_PCI_WATCHDOG_DECODE_EN;

	pci_write_config_dword(sp5100_tco_pci,
			       SP5100_PCI_WATCHDOG_MISC_REG,
			       val);

	/* Enable Watchdog timer and set the resolution to 1 sec. */
	outb(SP5100_PM_WATCHDOG_CONTROL, SP5100_IO_PM_INDEX_REG);
	val = inb(SP5100_IO_PM_DATA_REG);
	val |= SP5100_PM_WATCHDOG_SECOND_RES;
	val &= ~SP5100_PM_WATCHDOG_DISABLE;
	outb(val, SP5100_IO_PM_DATA_REG);

	/* Check that the watchdog action is set to reset the system. */
	val = readl(SP5100_WDT_CONTROL(tcobase));
	val &= ~SP5100_PM_WATCHDOG_ACTION_RESET;
	writel(val, SP5100_WDT_CONTROL(tcobase));

	/* Set a reasonable heartbeat before we stop the timer */
	tco_timer_set_heartbeat(heartbeat);

	/*
	 * Stop the TCO before we change anything so we don't race with
	 * a zeroed timer.
	 */
	tco_timer_stop();

	/* Done */
	return 1;

unreg_mem_region:
	release_mem_region(tcobase_phys, SP5100_WDT_MEM_MAP_SIZE);
unreg_region:
	release_region(pm_iobase, SP5100_PM_IOPORTS_SIZE);
exit:
	return 0;
}

static int __devinit sp5100_tco_init(struct platform_device *dev)
{
	int ret;
	u32 val;

	/* Check whether or not the hardware watchdog is there. If found, then
	 * set it up.
	 */
	if (!sp5100_tco_setupdevice())
		return -ENODEV;

	/* Check to see if last reboot was due to watchdog timeout */
	pr_info("Watchdog reboot %sdetected\n",
		readl(SP5100_WDT_CONTROL(tcobase)) & SP5100_PM_WATCHDOG_FIRED ?
		"" : "not ");

	/* Clear out the old status */
	val = readl(SP5100_WDT_CONTROL(tcobase));
	val &= ~SP5100_PM_WATCHDOG_FIRED;
	writel(val, SP5100_WDT_CONTROL(tcobase));

	/*
	 * Check that the heartbeat value is within it's range.
	 * If not, reset to the default.
	 */
	if (tco_timer_set_heartbeat(heartbeat)) {
		heartbeat = WATCHDOG_HEARTBEAT;
		tco_timer_set_heartbeat(heartbeat);
	}

	ret = misc_register(&sp5100_tco_miscdev);
	if (ret != 0) {
		pr_err("cannot register miscdev on minor=%d (err=%d)\n",
		       WATCHDOG_MINOR, ret);
		goto exit;
	}

	clear_bit(0, &timer_alive);

	pr_info("initialized (0x%p). heartbeat=%d sec (nowayout=%d)\n",
		tcobase, heartbeat, nowayout);

	return 0;

exit:
	iounmap(tcobase);
	release_mem_region(tcobase_phys, SP5100_WDT_MEM_MAP_SIZE);
	release_region(pm_iobase, SP5100_PM_IOPORTS_SIZE);
	return ret;
}

static void __devexit sp5100_tco_cleanup(void)
{
	/* Stop the timer before we leave */
	if (!nowayout)
		tco_timer_stop();

	/* Deregister */
	misc_deregister(&sp5100_tco_miscdev);
	iounmap(tcobase);
	release_mem_region(tcobase_phys, SP5100_WDT_MEM_MAP_SIZE);
	release_region(pm_iobase, SP5100_PM_IOPORTS_SIZE);
}

static int __devexit sp5100_tco_remove(struct platform_device *dev)
{
	if (tcobase)
		sp5100_tco_cleanup();
	return 0;
}

static void sp5100_tco_shutdown(struct platform_device *dev)
{
	tco_timer_stop();
}

static struct platform_driver sp5100_tco_driver = {
	.probe		= sp5100_tco_init,
	.remove		= __devexit_p(sp5100_tco_remove),
	.shutdown	= sp5100_tco_shutdown,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= TCO_MODULE_NAME,
	},
};

static int __init sp5100_tco_init_module(void)
{
	int err;

	pr_info("SP5100 TCO WatchDog Timer Driver v%s\n", TCO_VERSION);
=======
		return -ENODEV;

	pr_info("SP5100/SB800 TCO WatchDog Timer Driver\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = platform_driver_register(&sp5100_tco_driver);
	if (err)
		return err;

<<<<<<< HEAD
	sp5100_tco_platform_device = platform_device_register_simple(
					TCO_MODULE_NAME, -1, NULL, 0);
=======
	sp5100_tco_platform_device =
		platform_device_register_simple(TCO_DRIVER_NAME, -1, NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(sp5100_tco_platform_device)) {
		err = PTR_ERR(sp5100_tco_platform_device);
		goto unreg_platform_driver;
	}

	return 0;

unreg_platform_driver:
	platform_driver_unregister(&sp5100_tco_driver);
	return err;
}

<<<<<<< HEAD
static void __exit sp5100_tco_cleanup_module(void)
{
	platform_device_unregister(sp5100_tco_platform_device);
	platform_driver_unregister(&sp5100_tco_driver);
	pr_info("SP5100 TCO Watchdog Module Unloaded\n");
}

module_init(sp5100_tco_init_module);
module_exit(sp5100_tco_cleanup_module);

MODULE_AUTHOR("Priyanka Gupta");
MODULE_DESCRIPTION("TCO timer driver for SP5100 chipset");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
static void __exit sp5100_tco_exit(void)
{
	platform_device_unregister(sp5100_tco_platform_device);
	platform_driver_unregister(&sp5100_tco_driver);
}

module_init(sp5100_tco_init);
module_exit(sp5100_tco_exit);

MODULE_AUTHOR("Priyanka Gupta");
MODULE_DESCRIPTION("TCO timer driver for SP5100/SB800 chipset");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
