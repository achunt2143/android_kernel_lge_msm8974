<<<<<<< HEAD
/*
 * intel_scu_ipc.c: Driver for the Intel SCU IPC mechanism
 *
 * (C) Copyright 2008-2010 Intel Corporation
 * Author: Sreedhara DS (sreedhara.ds@intel.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for the Intel SCU IPC mechanism
 *
 * (C) Copyright 2008-2010,2015 Intel Corporation
 * Author: Sreedhara DS (sreedhara.ds@intel.com)
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * SCU running in ARC processor communicates with other entity running in IA
 * core through IPC mechanism which in turn messaging between IA core ad SCU.
 * SCU has two IPC mechanism IPC-1 and IPC-2. IPC-1 is used between IA32 and
 * SCU where IPC-2 is used between P-Unit and SCU. This driver delas with
 * IPC-1 Driver provides an API for power control unit registers (e.g. MSIC)
 * along with other APIs.
 */
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/pm.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/sfi.h>
#include <linux/module.h>
#include <asm/mrst.h>
#include <asm/intel_scu_ipc.h>

/* IPC defines the following message types */
#define IPCMSG_WATCHDOG_TIMER 0xF8 /* Set Kernel Watchdog Threshold */
#define IPCMSG_BATTERY        0xEF /* Coulomb Counter Accumulator */
#define IPCMSG_FW_UPDATE      0xFE /* Firmware update */
#define IPCMSG_PCNTRL         0xFF /* Power controller unit read/write */
#define IPCMSG_FW_REVISION    0xF4 /* Get firmware revision */
=======

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/slab.h>

#include <asm/intel_scu_ipc.h>

/* IPC defines the following message types */
#define IPCMSG_PCNTRL         0xff /* Power controller unit read/write */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Command id associated with message IPCMSG_PCNTRL */
#define IPC_CMD_PCNTRL_W      0 /* Register write */
#define IPC_CMD_PCNTRL_R      1 /* Register read */
#define IPC_CMD_PCNTRL_M      2 /* Register read-modify-write */

/*
 * IPC register summary
 *
<<<<<<< HEAD
 * IPC register blocks are memory mapped at fixed address of 0xFF11C000
 * To read or write information to the SCU, driver writes to IPC-1 memory
 * mapped registers (base address 0xFF11C000). The following is the IPC
 * mechanism
=======
 * IPC register blocks are memory mapped at fixed address of PCI BAR 0.
 * To read or write information to the SCU, driver writes to IPC-1 memory
 * mapped registers. The following is the IPC mechanism
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * 1. IA core cDMI interface claims this transaction and converts it to a
 *    Transaction Layer Packet (TLP) message which is sent across the cDMI.
 *
 * 2. South Complex cDMI block receives this message and writes it to
 *    the IPC-1 register block, causing an interrupt to the SCU
 *
 * 3. SCU firmware decodes this interrupt and IPC message and the appropriate
 *    message handler is called within firmware.
 */

<<<<<<< HEAD
#define IPC_BASE_ADDR     0xFF11C000	/* IPC1 base register address */
#define IPC_MAX_ADDR      0x100		/* Maximum IPC regisers */
#define IPC_WWBUF_SIZE    20		/* IPC Write buffer Size */
#define IPC_RWBUF_SIZE    20		/* IPC Read buffer Size */
#define IPC_I2C_BASE      0xFF12B000	/* I2C control register base address */
#define IPC_I2C_MAX_ADDR  0x10		/* Maximum I2C regisers */

static int ipc_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void ipc_remove(struct pci_dev *pdev);

struct intel_scu_ipc_dev {
	struct pci_dev *pdev;
	void __iomem *ipc_base;
	void __iomem *i2c_base;
};

static struct intel_scu_ipc_dev  ipcdev; /* Only one for now */

static int platform;		/* Platform type */

/*
 * IPC Read Buffer (Read Only):
 * 16 byte buffer for receiving data from SCU, if IPC command
 * processing results in response data
 */
#define IPC_READ_BUFFER		0x90

#define IPC_I2C_CNTRL_ADDR	0
#define I2C_DATA_ADDR		0x04

static DEFINE_MUTEX(ipclock); /* lock used to prevent multiple call to SCU */

/*
=======
#define IPC_WWBUF_SIZE    20		/* IPC Write buffer Size */
#define IPC_RWBUF_SIZE    20		/* IPC Read buffer Size */
#define IPC_IOC	          0x100		/* IPC command register IOC bit */

struct intel_scu_ipc_dev {
	struct device dev;
	struct resource mem;
	struct module *owner;
	int irq;
	void __iomem *ipc_base;
	struct completion cmd_complete;
};

#define IPC_STATUS		0x04
#define IPC_STATUS_IRQ		BIT(2)
#define IPC_STATUS_ERR		BIT(1)
#define IPC_STATUS_BUSY		BIT(0)

/*
 * IPC Write/Read Buffers:
 * 16 byte buffer for sending and receiving data to and from SCU.
 */
#define IPC_WRITE_BUFFER	0x80
#define IPC_READ_BUFFER		0x90

/* Timeout in jiffies */
#define IPC_TIMEOUT		(10 * HZ)

static struct intel_scu_ipc_dev *ipcdev; /* Only one for now */
static DEFINE_MUTEX(ipclock); /* lock used to prevent multiple call to SCU */

static struct class intel_scu_ipc_class = {
	.name = "intel_scu_ipc",
};

/**
 * intel_scu_ipc_dev_get() - Get SCU IPC instance
 *
 * The recommended new API takes SCU IPC instance as parameter and this
 * function can be called by driver to get the instance. This also makes
 * sure the driver providing the IPC functionality cannot be unloaded
 * while the caller has the instance.
 *
 * Call intel_scu_ipc_dev_put() to release the instance.
 *
 * Returns %NULL if SCU IPC is not currently available.
 */
struct intel_scu_ipc_dev *intel_scu_ipc_dev_get(void)
{
	struct intel_scu_ipc_dev *scu = NULL;

	mutex_lock(&ipclock);
	if (ipcdev) {
		get_device(&ipcdev->dev);
		/*
		 * Prevent the IPC provider from being unloaded while it
		 * is being used.
		 */
		if (!try_module_get(ipcdev->owner))
			put_device(&ipcdev->dev);
		else
			scu = ipcdev;
	}

	mutex_unlock(&ipclock);
	return scu;
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_dev_get);

/**
 * intel_scu_ipc_dev_put() - Put SCU IPC instance
 * @scu: SCU IPC instance
 *
 * This function releases the SCU IPC instance retrieved from
 * intel_scu_ipc_dev_get() and allows the driver providing IPC to be
 * unloaded.
 */
void intel_scu_ipc_dev_put(struct intel_scu_ipc_dev *scu)
{
	if (scu) {
		module_put(scu->owner);
		put_device(&scu->dev);
	}
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_dev_put);

struct intel_scu_ipc_devres {
	struct intel_scu_ipc_dev *scu;
};

static void devm_intel_scu_ipc_dev_release(struct device *dev, void *res)
{
	struct intel_scu_ipc_devres *dr = res;
	struct intel_scu_ipc_dev *scu = dr->scu;

	intel_scu_ipc_dev_put(scu);
}

/**
 * devm_intel_scu_ipc_dev_get() - Allocate managed SCU IPC device
 * @dev: Device requesting the SCU IPC device
 *
 * The recommended new API takes SCU IPC instance as parameter and this
 * function can be called by driver to get the instance. This also makes
 * sure the driver providing the IPC functionality cannot be unloaded
 * while the caller has the instance.
 *
 * Returns %NULL if SCU IPC is not currently available.
 */
struct intel_scu_ipc_dev *devm_intel_scu_ipc_dev_get(struct device *dev)
{
	struct intel_scu_ipc_devres *dr;
	struct intel_scu_ipc_dev *scu;

	dr = devres_alloc(devm_intel_scu_ipc_dev_release, sizeof(*dr), GFP_KERNEL);
	if (!dr)
		return NULL;

	scu = intel_scu_ipc_dev_get();
	if (!scu) {
		devres_free(dr);
		return NULL;
	}

	dr->scu = scu;
	devres_add(dev, dr);

	return scu;
}
EXPORT_SYMBOL_GPL(devm_intel_scu_ipc_dev_get);

/*
 * Send ipc command
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Command Register (Write Only):
 * A write to this register results in an interrupt to the SCU core processor
 * Format:
 * |rfu2(8) | size(8) | command id(4) | rfu1(3) | ioc(1) | command(8)|
 */
<<<<<<< HEAD
static inline void ipc_command(u32 cmd) /* Send ipc command */
{
	writel(cmd, ipcdev.ipc_base);
}

/*
=======
static inline void ipc_command(struct intel_scu_ipc_dev *scu, u32 cmd)
{
	reinit_completion(&scu->cmd_complete);
	writel(cmd | IPC_IOC, scu->ipc_base);
}

/*
 * Write ipc data
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * IPC Write Buffer (Write Only):
 * 16-byte buffer for sending data associated with IPC command to
 * SCU. Size of the data is specified in the IPC_COMMAND_REG register
 */
<<<<<<< HEAD
static inline void ipc_data_writel(u32 data, u32 offset) /* Write ipc data */
{
	writel(data, ipcdev.ipc_base + 0x80 + offset);
=======
static inline void ipc_data_writel(struct intel_scu_ipc_dev *scu, u32 data, u32 offset)
{
	writel(data, scu->ipc_base + IPC_WRITE_BUFFER + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Status Register (Read Only):
 * Driver will read this register to get the ready/busy status of the IPC
 * block and error status of the IPC command that was just processed by SCU
 * Format:
 * |rfu3(8)|error code(8)|initiator id(8)|cmd id(4)|rfu1(2)|error(1)|busy(1)|
 */
<<<<<<< HEAD

static inline u8 ipc_read_status(void)
{
	return __raw_readl(ipcdev.ipc_base + 0x04);
}

static inline u8 ipc_data_readb(u32 offset) /* Read ipc byte data */
{
	return readb(ipcdev.ipc_base + IPC_READ_BUFFER + offset);
}

static inline u32 ipc_data_readl(u32 offset) /* Read ipc u32 data */
{
	return readl(ipcdev.ipc_base + IPC_READ_BUFFER + offset);
}

static inline int busy_loop(void) /* Wait till scu status is busy */
{
	u32 status = 0;
	u32 loop_count = 0;

	status = ipc_read_status();
	while (status & 1) {
		udelay(1); /* scu processing time is in few u secods */
		status = ipc_read_status();
		loop_count++;
		/* break if scu doesn't reset busy bit after huge retry */
		if (loop_count > 100000) {
			dev_err(&ipcdev.pdev->dev, "IPC timed out");
			return -ETIMEDOUT;
		}
	}
	if ((status >> 1) & 1)
=======
static inline u8 ipc_read_status(struct intel_scu_ipc_dev *scu)
{
	return __raw_readl(scu->ipc_base + IPC_STATUS);
}

/* Read ipc byte data */
static inline u8 ipc_data_readb(struct intel_scu_ipc_dev *scu, u32 offset)
{
	return readb(scu->ipc_base + IPC_READ_BUFFER + offset);
}

/* Read ipc u32 data */
static inline u32 ipc_data_readl(struct intel_scu_ipc_dev *scu, u32 offset)
{
	return readl(scu->ipc_base + IPC_READ_BUFFER + offset);
}

/* Wait till scu status is busy */
static inline int busy_loop(struct intel_scu_ipc_dev *scu)
{
	u8 status;
	int err;

	err = readx_poll_timeout(ipc_read_status, scu, status, !(status & IPC_STATUS_BUSY),
				 100, jiffies_to_usecs(IPC_TIMEOUT));
	if (err)
		return err;

	return (status & IPC_STATUS_ERR) ? -EIO : 0;
}

/* Wait till ipc ioc interrupt is received or timeout in 10 HZ */
static inline int ipc_wait_for_interrupt(struct intel_scu_ipc_dev *scu)
{
	int status;

	wait_for_completion_timeout(&scu->cmd_complete, IPC_TIMEOUT);

	status = ipc_read_status(scu);
	if (status & IPC_STATUS_BUSY)
		return -ETIMEDOUT;

	if (status & IPC_STATUS_ERR)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;

	return 0;
}

<<<<<<< HEAD
/* Read/Write power control(PMIC in Langwell, MSIC in PenWell) registers */
static int pwr_reg_rdwr(u16 *addr, u8 *data, u32 count, u32 op, u32 id)
=======
static int intel_scu_ipc_check_status(struct intel_scu_ipc_dev *scu)
{
	return scu->irq > 0 ? ipc_wait_for_interrupt(scu) : busy_loop(scu);
}

static struct intel_scu_ipc_dev *intel_scu_ipc_get(struct intel_scu_ipc_dev *scu)
{
	u8 status;

	if (!scu)
		scu = ipcdev;
	if (!scu)
		return ERR_PTR(-ENODEV);

	status = ipc_read_status(scu);
	if (status & IPC_STATUS_BUSY) {
		dev_dbg(&scu->dev, "device is busy\n");
		return ERR_PTR(-EBUSY);
	}

	return scu;
}

/* Read/Write power control(PMIC in Langwell, MSIC in PenWell) registers */
static int pwr_reg_rdwr(struct intel_scu_ipc_dev *scu, u16 *addr, u8 *data,
			u32 count, u32 op, u32 id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int nc;
	u32 offset = 0;
	int err;
<<<<<<< HEAD
	u8 cbuf[IPC_WWBUF_SIZE] = { };
	u32 *wbuf = (u32 *)&cbuf;

	mutex_lock(&ipclock);

	memset(cbuf, 0, sizeof(cbuf));

	if (ipcdev.pdev == NULL) {
		mutex_unlock(&ipclock);
		return -ENODEV;
=======
	u8 cbuf[IPC_WWBUF_SIZE];
	u32 *wbuf = (u32 *)&cbuf;

	memset(cbuf, 0, sizeof(cbuf));

	mutex_lock(&ipclock);
	scu = intel_scu_ipc_get(scu);
	if (IS_ERR(scu)) {
		mutex_unlock(&ipclock);
		return PTR_ERR(scu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	for (nc = 0; nc < count; nc++, offset += 2) {
		cbuf[offset] = addr[nc];
		cbuf[offset + 1] = addr[nc] >> 8;
	}

	if (id == IPC_CMD_PCNTRL_R) {
		for (nc = 0, offset = 0; nc < count; nc++, offset += 4)
<<<<<<< HEAD
			ipc_data_writel(wbuf[nc], offset);
		ipc_command((count*2) << 16 |  id << 12 | 0 << 8 | op);
=======
			ipc_data_writel(scu, wbuf[nc], offset);
		ipc_command(scu, (count * 2) << 16 | id << 12 | 0 << 8 | op);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (id == IPC_CMD_PCNTRL_W) {
		for (nc = 0; nc < count; nc++, offset += 1)
			cbuf[offset] = data[nc];
		for (nc = 0, offset = 0; nc < count; nc++, offset += 4)
<<<<<<< HEAD
			ipc_data_writel(wbuf[nc], offset);
		ipc_command((count*3) << 16 |  id << 12 | 0 << 8 | op);
	} else if (id == IPC_CMD_PCNTRL_M) {
		cbuf[offset] = data[0];
		cbuf[offset + 1] = data[1];
		ipc_data_writel(wbuf[0], 0); /* Write wbuff */
		ipc_command(4 << 16 |  id << 12 | 0 << 8 | op);
	}

	err = busy_loop();
	if (id == IPC_CMD_PCNTRL_R) { /* Read rbuf */
		/* Workaround: values are read as 0 without memcpy_fromio */
		memcpy_fromio(cbuf, ipcdev.ipc_base + 0x90, 16);
		for (nc = 0; nc < count; nc++)
			data[nc] = ipc_data_readb(nc);
=======
			ipc_data_writel(scu, wbuf[nc], offset);
		ipc_command(scu, (count * 3) << 16 | id << 12 | 0 << 8 | op);
	} else if (id == IPC_CMD_PCNTRL_M) {
		cbuf[offset] = data[0];
		cbuf[offset + 1] = data[1];
		ipc_data_writel(scu, wbuf[0], 0); /* Write wbuff */
		ipc_command(scu, 4 << 16 | id << 12 | 0 << 8 | op);
	}

	err = intel_scu_ipc_check_status(scu);
	if (!err && id == IPC_CMD_PCNTRL_R) { /* Read rbuf */
		/* Workaround: values are read as 0 without memcpy_fromio */
		memcpy_fromio(cbuf, scu->ipc_base + 0x90, 16);
		for (nc = 0; nc < count; nc++)
			data[nc] = ipc_data_readb(scu, nc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&ipclock);
	return err;
}

/**
<<<<<<< HEAD
 *	intel_scu_ipc_ioread8		-	read a word via the SCU
 *	@addr: register on SCU
 *	@data: return pointer for read byte
 *
 *	Read a single register. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_ioread8(u16 addr, u8 *data)
{
	return pwr_reg_rdwr(&addr, data, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_ioread8);

/**
 *	intel_scu_ipc_ioread16		-	read a word via the SCU
 *	@addr: register on SCU
 *	@data: return pointer for read word
 *
 *	Read a register pair. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_ioread16(u16 addr, u16 *data)
{
	u16 x[2] = {addr, addr + 1 };
	return pwr_reg_rdwr(x, (u8 *)data, 2, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_ioread16);

/**
 *	intel_scu_ipc_ioread32		-	read a dword via the SCU
 *	@addr: register on SCU
 *	@data: return pointer for read dword
 *
 *	Read four registers. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_ioread32(u16 addr, u32 *data)
{
	u16 x[4] = {addr, addr + 1, addr + 2, addr + 3};
	return pwr_reg_rdwr(x, (u8 *)data, 4, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_ioread32);

/**
 *	intel_scu_ipc_iowrite8		-	write a byte via the SCU
 *	@addr: register on SCU
 *	@data: byte to write
 *
 *	Write a single register. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_iowrite8(u16 addr, u8 data)
{
	return pwr_reg_rdwr(&addr, &data, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_iowrite8);

/**
 *	intel_scu_ipc_iowrite16		-	write a word via the SCU
 *	@addr: register on SCU
 *	@data: word to write
 *
 *	Write two registers. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_iowrite16(u16 addr, u16 data)
{
	u16 x[2] = {addr, addr + 1 };
	return pwr_reg_rdwr(x, (u8 *)&data, 2, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_iowrite16);

/**
 *	intel_scu_ipc_iowrite32		-	write a dword via the SCU
 *	@addr: register on SCU
 *	@data: dword to write
 *
 *	Write four registers. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_iowrite32(u16 addr, u32 data)
{
	u16 x[4] = {addr, addr + 1, addr + 2, addr + 3};
	return pwr_reg_rdwr(x, (u8 *)&data, 4, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_iowrite32);

/**
 *	intel_scu_ipc_readvv		-	read a set of registers
 *	@addr: register list
 *	@data: bytes to return
 *	@len: length of array
 *
 *	Read registers. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	The largest array length permitted by the hardware is 5 items.
 *
 *	This function may sleep.
 */
int intel_scu_ipc_readv(u16 *addr, u8 *data, int len)
{
	return pwr_reg_rdwr(addr, data, len, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_readv);

/**
 *	intel_scu_ipc_writev		-	write a set of registers
 *	@addr: register list
 *	@data: bytes to write
 *	@len: length of array
 *
 *	Write registers. Returns 0 on success or an error code. All
 *	locking between SCU accesses is handled for the caller.
 *
 *	The largest array length permitted by the hardware is 5 items.
 *
 *	This function may sleep.
 *
 */
int intel_scu_ipc_writev(u16 *addr, u8 *data, int len)
{
	return pwr_reg_rdwr(addr, data, len, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_writev);


/**
 *	intel_scu_ipc_update_register	-	r/m/w a register
 *	@addr: register address
 *	@bits: bits to update
 *	@mask: mask of bits to update
 *
 *	Read-modify-write power control unit register. The first data argument
 *	must be register value and second is mask value
 *	mask is a bitmap that indicates which bits to update.
 *	0 = masked. Don't modify this bit, 1 = modify this bit.
 *	returns 0 on success or an error code.
 *
 *	This function may sleep. Locking between SCU accesses is handled
 *	for the caller.
 */
int intel_scu_ipc_update_register(u16 addr, u8 bits, u8 mask)
{
	u8 data[2] = { bits, mask };
	return pwr_reg_rdwr(&addr, data, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_M);
}
EXPORT_SYMBOL(intel_scu_ipc_update_register);

/**
 *	intel_scu_ipc_simple_command	-	send a simple command
 *	@cmd: command
 *	@sub: sub type
 *
 *	Issue a simple command to the SCU. Do not use this interface if
 *	you must then access data as any data values may be overwritten
 *	by another SCU access by the time this function returns.
 *
 *	This function may sleep. Locking for SCU accesses is handled for
 *	the caller.
 */
int intel_scu_ipc_simple_command(int cmd, int sub)
{
	int err;

	mutex_lock(&ipclock);
	if (ipcdev.pdev == NULL) {
		mutex_unlock(&ipclock);
		return -ENODEV;
	}
	ipc_command(sub << 12 | cmd);
	err = busy_loop();
	mutex_unlock(&ipclock);
	return err;
}
EXPORT_SYMBOL(intel_scu_ipc_simple_command);

/**
 *	intel_scu_ipc_command	-	command with data
 *	@cmd: command
 *	@sub: sub type
 *	@in: input data
 *	@inlen: input length in dwords
 *	@out: output data
 *	@outlein: output length in dwords
 *
 *	Issue a command to the SCU which involves data transfers. Do the
 *	data copies under the lock but leave it for the caller to interpret
 */

int intel_scu_ipc_command(int cmd, int sub, u32 *in, int inlen,
							u32 *out, int outlen)
{
	int i, err;

	mutex_lock(&ipclock);
	if (ipcdev.pdev == NULL) {
		mutex_unlock(&ipclock);
		return -ENODEV;
	}

	for (i = 0; i < inlen; i++)
		ipc_data_writel(*in++, 4 * i);

	ipc_command((inlen << 16) | (sub << 12) | cmd);
	err = busy_loop();

	for (i = 0; i < outlen; i++)
		*out++ = ipc_data_readl(4 * i);

	mutex_unlock(&ipclock);
	return err;
}
EXPORT_SYMBOL(intel_scu_ipc_command);

/*I2C commands */
#define IPC_I2C_WRITE 1 /* I2C Write command */
#define IPC_I2C_READ  2 /* I2C Read command */

/**
 *	intel_scu_ipc_i2c_cntrl		-	I2C read/write operations
 *	@addr: I2C address + command bits
 *	@data: data to read/write
 *
 *	Perform an an I2C read/write operation via the SCU. All locking is
 *	handled for the caller. This function may sleep.
 *
 *	Returns an error code or 0 on success.
 *
 *	This has to be in the IPC driver for the locking.
 */
int intel_scu_ipc_i2c_cntrl(u32 addr, u32 *data)
{
	u32 cmd = 0;

	mutex_lock(&ipclock);
	if (ipcdev.pdev == NULL) {
		mutex_unlock(&ipclock);
		return -ENODEV;
	}
	cmd = (addr >> 24) & 0xFF;
	if (cmd == IPC_I2C_READ) {
		writel(addr, ipcdev.i2c_base + IPC_I2C_CNTRL_ADDR);
		/* Write not getting updated without delay */
		mdelay(1);
		*data = readl(ipcdev.i2c_base + I2C_DATA_ADDR);
	} else if (cmd == IPC_I2C_WRITE) {
		writel(*data, ipcdev.i2c_base + I2C_DATA_ADDR);
		mdelay(1);
		writel(addr, ipcdev.i2c_base + IPC_I2C_CNTRL_ADDR);
	} else {
		dev_err(&ipcdev.pdev->dev,
			"intel_scu_ipc: I2C INVALID_CMD = 0x%x\n", cmd);

		mutex_unlock(&ipclock);
		return -EIO;
	}
	mutex_unlock(&ipclock);
	return 0;
}
EXPORT_SYMBOL(intel_scu_ipc_i2c_cntrl);
=======
 * intel_scu_ipc_dev_ioread8() - Read a byte via the SCU
 * @scu: Optional SCU IPC instance
 * @addr: Register on SCU
 * @data: Return pointer for read byte
 *
 * Read a single register. Returns %0 on success or an error code. All
 * locking between SCU accesses is handled for the caller.
 *
 * This function may sleep.
 */
int intel_scu_ipc_dev_ioread8(struct intel_scu_ipc_dev *scu, u16 addr, u8 *data)
{
	return pwr_reg_rdwr(scu, &addr, data, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_dev_ioread8);

/**
 * intel_scu_ipc_dev_iowrite8() - Write a byte via the SCU
 * @scu: Optional SCU IPC instance
 * @addr: Register on SCU
 * @data: Byte to write
 *
 * Write a single register. Returns %0 on success or an error code. All
 * locking between SCU accesses is handled for the caller.
 *
 * This function may sleep.
 */
int intel_scu_ipc_dev_iowrite8(struct intel_scu_ipc_dev *scu, u16 addr, u8 data)
{
	return pwr_reg_rdwr(scu, &addr, &data, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_dev_iowrite8);

/**
 * intel_scu_ipc_dev_readv() - Read a set of registers
 * @scu: Optional SCU IPC instance
 * @addr: Register list
 * @data: Bytes to return
 * @len: Length of array
 *
 * Read registers. Returns %0 on success or an error code. All locking
 * between SCU accesses is handled for the caller.
 *
 * The largest array length permitted by the hardware is 5 items.
 *
 * This function may sleep.
 */
int intel_scu_ipc_dev_readv(struct intel_scu_ipc_dev *scu, u16 *addr, u8 *data,
			    size_t len)
{
	return pwr_reg_rdwr(scu, addr, data, len, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_R);
}
EXPORT_SYMBOL(intel_scu_ipc_dev_readv);

/**
 * intel_scu_ipc_dev_writev() - Write a set of registers
 * @scu: Optional SCU IPC instance
 * @addr: Register list
 * @data: Bytes to write
 * @len: Length of array
 *
 * Write registers. Returns %0 on success or an error code. All locking
 * between SCU accesses is handled for the caller.
 *
 * The largest array length permitted by the hardware is 5 items.
 *
 * This function may sleep.
 */
int intel_scu_ipc_dev_writev(struct intel_scu_ipc_dev *scu, u16 *addr, u8 *data,
			     size_t len)
{
	return pwr_reg_rdwr(scu, addr, data, len, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_W);
}
EXPORT_SYMBOL(intel_scu_ipc_dev_writev);

/**
 * intel_scu_ipc_dev_update() - Update a register
 * @scu: Optional SCU IPC instance
 * @addr: Register address
 * @data: Bits to update
 * @mask: Mask of bits to update
 *
 * Read-modify-write power control unit register. The first data argument
 * must be register value and second is mask value mask is a bitmap that
 * indicates which bits to update. %0 = masked. Don't modify this bit, %1 =
 * modify this bit. returns %0 on success or an error code.
 *
 * This function may sleep. Locking between SCU accesses is handled
 * for the caller.
 */
int intel_scu_ipc_dev_update(struct intel_scu_ipc_dev *scu, u16 addr, u8 data,
			     u8 mask)
{
	u8 tmp[2] = { data, mask };
	return pwr_reg_rdwr(scu, &addr, tmp, 1, IPCMSG_PCNTRL, IPC_CMD_PCNTRL_M);
}
EXPORT_SYMBOL(intel_scu_ipc_dev_update);

/**
 * intel_scu_ipc_dev_simple_command() - Send a simple command
 * @scu: Optional SCU IPC instance
 * @cmd: Command
 * @sub: Sub type
 *
 * Issue a simple command to the SCU. Do not use this interface if you must
 * then access data as any data values may be overwritten by another SCU
 * access by the time this function returns.
 *
 * This function may sleep. Locking for SCU accesses is handled for the
 * caller.
 */
int intel_scu_ipc_dev_simple_command(struct intel_scu_ipc_dev *scu, int cmd,
				     int sub)
{
	u32 cmdval;
	int err;

	mutex_lock(&ipclock);
	scu = intel_scu_ipc_get(scu);
	if (IS_ERR(scu)) {
		mutex_unlock(&ipclock);
		return PTR_ERR(scu);
	}

	cmdval = sub << 12 | cmd;
	ipc_command(scu, cmdval);
	err = intel_scu_ipc_check_status(scu);
	mutex_unlock(&ipclock);
	if (err)
		dev_err(&scu->dev, "IPC command %#x failed with %d\n", cmdval, err);
	return err;
}
EXPORT_SYMBOL(intel_scu_ipc_dev_simple_command);

/**
 * intel_scu_ipc_dev_command_with_size() - Command with data
 * @scu: Optional SCU IPC instance
 * @cmd: Command
 * @sub: Sub type
 * @in: Input data
 * @inlen: Input length in bytes
 * @size: Input size written to the IPC command register in whatever
 *	  units (dword, byte) the particular firmware requires. Normally
 *	  should be the same as @inlen.
 * @out: Output data
 * @outlen: Output length in bytes
 *
 * Issue a command to the SCU which involves data transfers. Do the
 * data copies under the lock but leave it for the caller to interpret.
 */
int intel_scu_ipc_dev_command_with_size(struct intel_scu_ipc_dev *scu, int cmd,
					int sub, const void *in, size_t inlen,
					size_t size, void *out, size_t outlen)
{
	size_t outbuflen = DIV_ROUND_UP(outlen, sizeof(u32));
	size_t inbuflen = DIV_ROUND_UP(inlen, sizeof(u32));
	u32 cmdval, inbuf[4] = {};
	int i, err;

	if (inbuflen > 4 || outbuflen > 4)
		return -EINVAL;

	mutex_lock(&ipclock);
	scu = intel_scu_ipc_get(scu);
	if (IS_ERR(scu)) {
		mutex_unlock(&ipclock);
		return PTR_ERR(scu);
	}

	memcpy(inbuf, in, inlen);
	for (i = 0; i < inbuflen; i++)
		ipc_data_writel(scu, inbuf[i], 4 * i);

	cmdval = (size << 16) | (sub << 12) | cmd;
	ipc_command(scu, cmdval);
	err = intel_scu_ipc_check_status(scu);

	if (!err) {
		u32 outbuf[4] = {};

		for (i = 0; i < outbuflen; i++)
			outbuf[i] = ipc_data_readl(scu, 4 * i);

		memcpy(out, outbuf, outlen);
	}

	mutex_unlock(&ipclock);
	if (err)
		dev_err(&scu->dev, "IPC command %#x failed with %d\n", cmdval, err);
	return err;
}
EXPORT_SYMBOL(intel_scu_ipc_dev_command_with_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Interrupt handler gets called when ioc bit of IPC_COMMAND_REG set to 1
 * When ioc bit is set to 1, caller api must wait for interrupt handler called
 * which in turn unlocks the caller api. Currently this is not used
 *
 * This is edge triggered so we need take no action to clear anything
 */
static irqreturn_t ioc(int irq, void *dev_id)
{
<<<<<<< HEAD
	return IRQ_HANDLED;
}

/**
 *	ipc_probe	-	probe an Intel SCU IPC
 *	@dev: the PCI device matching
 *	@id: entry in the match table
 *
 *	Enable and install an intel SCU IPC. This appears in the PCI space
 *	but uses some hard coded addresses as well.
 */
static int ipc_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int err;
	resource_size_t pci_resource;

	if (ipcdev.pdev)		/* We support only one SCU */
		return -EBUSY;

	ipcdev.pdev = pci_dev_get(dev);

	err = pci_enable_device(dev);
	if (err)
		return err;

	err = pci_request_regions(dev, "intel_scu_ipc");
	if (err)
		return err;

	pci_resource = pci_resource_start(dev, 0);
	if (!pci_resource)
		return -ENOMEM;

	if (request_irq(dev->irq, ioc, 0, "intel_scu_ipc", &ipcdev))
		return -EBUSY;

	ipcdev.ipc_base = ioremap_nocache(IPC_BASE_ADDR, IPC_MAX_ADDR);
	if (!ipcdev.ipc_base)
		return -ENOMEM;

	ipcdev.i2c_base = ioremap_nocache(IPC_I2C_BASE, IPC_I2C_MAX_ADDR);
	if (!ipcdev.i2c_base) {
		iounmap(ipcdev.ipc_base);
		return -ENOMEM;
	}

	intel_scu_devices_create();

	return 0;
}

/**
 *	ipc_remove	-	remove a bound IPC device
 *	@pdev: PCI device
 *
 *	In practice the SCU is not removable but this function is also
 *	called for each device on a module unload or cleanup which is the
 *	path that will get used.
 *
 *	Free up the mappings and release the PCI resources
 */
static void ipc_remove(struct pci_dev *pdev)
{
	free_irq(pdev->irq, &ipcdev);
	pci_release_regions(pdev);
	pci_dev_put(ipcdev.pdev);
	iounmap(ipcdev.ipc_base);
	iounmap(ipcdev.i2c_base);
	ipcdev.pdev = NULL;
	intel_scu_devices_destroy();
}

static DEFINE_PCI_DEVICE_TABLE(pci_ids) = {
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x082a)},
	{ 0,}
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static struct pci_driver ipc_driver = {
	.name = "intel_scu_ipc",
	.id_table = pci_ids,
	.probe = ipc_probe,
	.remove = ipc_remove,
};


static int __init intel_scu_ipc_init(void)
{
	platform = mrst_identify_cpu();
	if (platform == 0)
		return -ENODEV;
	return  pci_register_driver(&ipc_driver);
}

static void __exit intel_scu_ipc_exit(void)
{
	pci_unregister_driver(&ipc_driver);
}

MODULE_AUTHOR("Sreedhara DS <sreedhara.ds@intel.com>");
MODULE_DESCRIPTION("Intel SCU IPC driver");
MODULE_LICENSE("GPL");

module_init(intel_scu_ipc_init);
=======
	struct intel_scu_ipc_dev *scu = dev_id;
	int status = ipc_read_status(scu);

	writel(status | IPC_STATUS_IRQ, scu->ipc_base + IPC_STATUS);
	complete(&scu->cmd_complete);

	return IRQ_HANDLED;
}

static void intel_scu_ipc_release(struct device *dev)
{
	struct intel_scu_ipc_dev *scu;

	scu = container_of(dev, struct intel_scu_ipc_dev, dev);
	if (scu->irq > 0)
		free_irq(scu->irq, scu);
	iounmap(scu->ipc_base);
	release_mem_region(scu->mem.start, resource_size(&scu->mem));
	kfree(scu);
}

/**
 * __intel_scu_ipc_register() - Register SCU IPC device
 * @parent: Parent device
 * @scu_data: Data used to configure SCU IPC
 * @owner: Module registering the SCU IPC device
 *
 * Call this function to register SCU IPC mechanism under @parent.
 * Returns pointer to the new SCU IPC device or ERR_PTR() in case of
 * failure. The caller may use the returned instance if it needs to do
 * SCU IPC calls itself.
 */
struct intel_scu_ipc_dev *
__intel_scu_ipc_register(struct device *parent,
			 const struct intel_scu_ipc_data *scu_data,
			 struct module *owner)
{
	int err;
	struct intel_scu_ipc_dev *scu;
	void __iomem *ipc_base;

	mutex_lock(&ipclock);
	/* We support only one IPC */
	if (ipcdev) {
		err = -EBUSY;
		goto err_unlock;
	}

	scu = kzalloc(sizeof(*scu), GFP_KERNEL);
	if (!scu) {
		err = -ENOMEM;
		goto err_unlock;
	}

	scu->owner = owner;
	scu->dev.parent = parent;
	scu->dev.class = &intel_scu_ipc_class;
	scu->dev.release = intel_scu_ipc_release;

	if (!request_mem_region(scu_data->mem.start, resource_size(&scu_data->mem),
				"intel_scu_ipc")) {
		err = -EBUSY;
		goto err_free;
	}

	ipc_base = ioremap(scu_data->mem.start, resource_size(&scu_data->mem));
	if (!ipc_base) {
		err = -ENOMEM;
		goto err_release;
	}

	scu->ipc_base = ipc_base;
	scu->mem = scu_data->mem;
	scu->irq = scu_data->irq;
	init_completion(&scu->cmd_complete);

	if (scu->irq > 0) {
		err = request_irq(scu->irq, ioc, 0, "intel_scu_ipc", scu);
		if (err)
			goto err_unmap;
	}

	/*
	 * After this point intel_scu_ipc_release() takes care of
	 * releasing the SCU IPC resources once refcount drops to zero.
	 */
	dev_set_name(&scu->dev, "intel_scu_ipc");
	err = device_register(&scu->dev);
	if (err) {
		put_device(&scu->dev);
		goto err_unlock;
	}

	/* Assign device at last */
	ipcdev = scu;
	mutex_unlock(&ipclock);

	return scu;

err_unmap:
	iounmap(ipc_base);
err_release:
	release_mem_region(scu_data->mem.start, resource_size(&scu_data->mem));
err_free:
	kfree(scu);
err_unlock:
	mutex_unlock(&ipclock);

	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(__intel_scu_ipc_register);

/**
 * intel_scu_ipc_unregister() - Unregister SCU IPC
 * @scu: SCU IPC handle
 *
 * This unregisters the SCU IPC device and releases the acquired
 * resources once the refcount goes to zero.
 */
void intel_scu_ipc_unregister(struct intel_scu_ipc_dev *scu)
{
	mutex_lock(&ipclock);
	if (!WARN_ON(!ipcdev)) {
		ipcdev = NULL;
		device_unregister(&scu->dev);
	}
	mutex_unlock(&ipclock);
}
EXPORT_SYMBOL_GPL(intel_scu_ipc_unregister);

static void devm_intel_scu_ipc_unregister(struct device *dev, void *res)
{
	struct intel_scu_ipc_devres *dr = res;
	struct intel_scu_ipc_dev *scu = dr->scu;

	intel_scu_ipc_unregister(scu);
}

/**
 * __devm_intel_scu_ipc_register() - Register managed SCU IPC device
 * @parent: Parent device
 * @scu_data: Data used to configure SCU IPC
 * @owner: Module registering the SCU IPC device
 *
 * Call this function to register managed SCU IPC mechanism under
 * @parent. Returns pointer to the new SCU IPC device or ERR_PTR() in
 * case of failure. The caller may use the returned instance if it needs
 * to do SCU IPC calls itself.
 */
struct intel_scu_ipc_dev *
__devm_intel_scu_ipc_register(struct device *parent,
			      const struct intel_scu_ipc_data *scu_data,
			      struct module *owner)
{
	struct intel_scu_ipc_devres *dr;
	struct intel_scu_ipc_dev *scu;

	dr = devres_alloc(devm_intel_scu_ipc_unregister, sizeof(*dr), GFP_KERNEL);
	if (!dr)
		return NULL;

	scu = __intel_scu_ipc_register(parent, scu_data, owner);
	if (IS_ERR(scu)) {
		devres_free(dr);
		return scu;
	}

	dr->scu = scu;
	devres_add(parent, dr);

	return scu;
}
EXPORT_SYMBOL_GPL(__devm_intel_scu_ipc_register);

static int __init intel_scu_ipc_init(void)
{
	return class_register(&intel_scu_ipc_class);
}
subsys_initcall(intel_scu_ipc_init);

static void __exit intel_scu_ipc_exit(void)
{
	class_unregister(&intel_scu_ipc_class);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(intel_scu_ipc_exit);
