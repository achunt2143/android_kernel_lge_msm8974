<<<<<<< HEAD
/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spmi.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/of_spmi.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/syscore_ops.h>
#include <mach/qpnp-int.h>
#include "spmi-dbgfs.h"

#define SPMI_PMIC_ARB_NAME		"spmi_pmic_arb"

/* PMIC Arbiter configuration registers */
#define PMIC_ARB_VERSION		0x0000
#define PMIC_ARB_INT_EN			0x0004
#define PMIC_ARB_PROTOCOL_IRQ_STATUS	(0x700 + 0x820)
#define PMIC_ARB_GENI_CTRL		0x0024
#define PMIC_ARB_GENI_STATUS	0x0028
/* PMIC Arbiter channel registers */
#define PMIC_ARB_CMD(N)			(0x0800 + (0x80 * (N)))
#define PMIC_ARB_CONFIG(N)		(0x0804 + (0x80 * (N)))
#define PMIC_ARB_STATUS(N)		(0x0808 + (0x80 * (N)))
#define PMIC_ARB_WDATA0(N)		(0x0810 + (0x80 * (N)))
#define PMIC_ARB_WDATA1(N)		(0x0814 + (0x80 * (N)))
#define PMIC_ARB_RDATA0(N)		(0x0818 + (0x80 * (N)))
#define PMIC_ARB_RDATA1(N)		(0x081C + (0x80 * (N)))

/* Interrupt Controller */
#define SPMI_PIC_OWNER_ACC_STATUS(M, N)	(0x0000 + ((32 * (M)) + (4 * (N))))
#define SPMI_PIC_ACC_ENABLE(N)		(0x0200 + (4 * (N)))
#define SPMI_PIC_IRQ_STATUS(N)		(0x0600 + (4 * (N)))
#define SPMI_PIC_IRQ_CLEAR(N)		(0x0A00 + (4 * (N)))
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2015, 2017, 2021, The Linux Foundation. All rights reserved.
 */
#include <linux/bitmap.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/irqdomain.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spmi.h>

/* PMIC Arbiter configuration registers */
#define PMIC_ARB_VERSION		0x0000
#define PMIC_ARB_VERSION_V2_MIN		0x20010000
#define PMIC_ARB_VERSION_V3_MIN		0x30000000
#define PMIC_ARB_VERSION_V5_MIN		0x50000000
#define PMIC_ARB_VERSION_V7_MIN		0x70000000
#define PMIC_ARB_INT_EN			0x0004

#define PMIC_ARB_FEATURES		0x0004
#define PMIC_ARB_FEATURES_PERIPH_MASK	GENMASK(10, 0)

#define PMIC_ARB_FEATURES1		0x0008

/* PMIC Arbiter channel registers offsets */
#define PMIC_ARB_CMD			0x00
#define PMIC_ARB_CONFIG			0x04
#define PMIC_ARB_STATUS			0x08
#define PMIC_ARB_WDATA0			0x10
#define PMIC_ARB_WDATA1			0x14
#define PMIC_ARB_RDATA0			0x18
#define PMIC_ARB_RDATA1			0x1C
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Mapping Table */
#define SPMI_MAPPING_TABLE_REG(N)	(0x0B00 + (4 * (N)))
#define SPMI_MAPPING_BIT_INDEX(X)	(((X) >> 18) & 0xF)
#define SPMI_MAPPING_BIT_IS_0_FLAG(X)	(((X) >> 17) & 0x1)
#define SPMI_MAPPING_BIT_IS_0_RESULT(X)	(((X) >> 9) & 0xFF)
#define SPMI_MAPPING_BIT_IS_1_FLAG(X)	(((X) >> 8) & 0x1)
#define SPMI_MAPPING_BIT_IS_1_RESULT(X)	(((X) >> 0) & 0xFF)

<<<<<<< HEAD
#define SPMI_MAPPING_TABLE_LEN		255
#define SPMI_MAPPING_TABLE_TREE_DEPTH	16	/* Maximum of 16-bits */

/* Ownership Table */
#define SPMI_OWNERSHIP_TABLE_REG(N)	(0x0700 + (4 * (N)))
=======
#define SPMI_MAPPING_TABLE_TREE_DEPTH	16	/* Maximum of 16-bits */
#define PMIC_ARB_MAX_PPID		BIT(12) /* PPID is 12bit */
#define PMIC_ARB_APID_VALID		BIT(15)
#define PMIC_ARB_CHAN_IS_IRQ_OWNER(reg)	((reg) & BIT(24))
#define INVALID_EE				0xFF

/* Ownership Table */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SPMI_OWNERSHIP_PERIPH2OWNER(X)	((X) & 0x7)

/* Channel Status fields */
enum pmic_arb_chnl_status {
<<<<<<< HEAD
	PMIC_ARB_STATUS_DONE	= (1 << 0),
	PMIC_ARB_STATUS_FAILURE	= (1 << 1),
	PMIC_ARB_STATUS_DENIED	= (1 << 2),
	PMIC_ARB_STATUS_DROPPED	= (1 << 3),
=======
	PMIC_ARB_STATUS_DONE	= BIT(0),
	PMIC_ARB_STATUS_FAILURE	= BIT(1),
	PMIC_ARB_STATUS_DENIED	= BIT(2),
	PMIC_ARB_STATUS_DROPPED	= BIT(3),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Command register fields */
#define PMIC_ARB_CMD_MAX_BYTE_COUNT	8

/* Command Opcodes */
enum pmic_arb_cmd_op_code {
	PMIC_ARB_OP_EXT_WRITEL = 0,
	PMIC_ARB_OP_EXT_READL = 1,
	PMIC_ARB_OP_EXT_WRITE = 2,
	PMIC_ARB_OP_RESET = 3,
	PMIC_ARB_OP_SLEEP = 4,
	PMIC_ARB_OP_SHUTDOWN = 5,
	PMIC_ARB_OP_WAKEUP = 6,
	PMIC_ARB_OP_AUTHENTICATE = 7,
	PMIC_ARB_OP_MSTR_READ = 8,
	PMIC_ARB_OP_MSTR_WRITE = 9,
	PMIC_ARB_OP_EXT_READ = 13,
	PMIC_ARB_OP_WRITE = 14,
	PMIC_ARB_OP_READ = 15,
	PMIC_ARB_OP_ZERO_WRITE = 16,
};

<<<<<<< HEAD
/* Maximum number of support PMIC peripherals */
#define PMIC_ARB_MAX_PERIPHS		256
#define PMIC_ARB_PERIPH_ID_VALID	(1 << 15)
#define PMIC_ARB_TIMEOUT_US		100
=======
/*
 * PMIC arbiter version 5 uses different register offsets for read/write vs
 * observer channels.
 */
enum pmic_arb_channel {
	PMIC_ARB_CHANNEL_RW,
	PMIC_ARB_CHANNEL_OBS,
};

/* Maximum number of support PMIC peripherals */
#define PMIC_ARB_MAX_PERIPHS		512
#define PMIC_ARB_MAX_PERIPHS_V7		1024
#define PMIC_ARB_TIMEOUT_US		1000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PMIC_ARB_MAX_TRANS_BYTES	(8)

#define PMIC_ARB_APID_MASK		0xFF
#define PMIC_ARB_PPID_MASK		0xFFF

/* interrupt enable bit */
#define SPMI_PIC_ACC_ENABLE_BIT		BIT(0)

<<<<<<< HEAD
/**
 * base - base address of the PMIC Arbiter core registers.
 * intr - base address of the SPMI interrupt control registers
 */
struct spmi_pmic_arb_dev {
	struct spmi_controller	controller;
	struct device		*dev;
	struct device		*slave;
	void __iomem		*base;
	void __iomem		*intr;
	void __iomem		*cnfg;
	int			pic_irq;
	bool			allow_wakeup;
	spinlock_t		lock;
	u8			owner;
	u8			channel;
	u8			min_apid;
	u8			max_apid;
	u16			periph_id_map[PMIC_ARB_MAX_PERIPHS];
	u32			mapping_table[SPMI_MAPPING_TABLE_LEN];
	u32			prev_prtcl_irq_stat;
};

static struct spmi_pmic_arb_dev *the_pmic_arb;

static u32 pmic_arb_read(struct spmi_pmic_arb_dev *dev, u32 offset)
{
	u32 val = readl_relaxed(dev->base + offset);

	pr_debug("address 0x%p, val 0x%x\n", dev->base + offset, val);
	return val;
}

static void pmic_arb_write(struct spmi_pmic_arb_dev *dev, u32 offset, u32 val)
{
	pr_debug("address 0x%p, val 0x%x\n", dev->base + offset, val);
	writel_relaxed(val, dev->base + offset);
}

static void pmic_arb_save_stat_before_txn(struct spmi_pmic_arb_dev *dev)
{
	dev->prev_prtcl_irq_stat =
		readl_relaxed(dev->cnfg + PMIC_ARB_PROTOCOL_IRQ_STATUS);
}

static int pmic_arb_diagnosis(struct spmi_pmic_arb_dev *dev, u32 status)
{
	if (status & PMIC_ARB_STATUS_DENIED) {
		dev_err(dev->dev,
		    "wait_for_done: transaction denied by SPMI master (0x%x)\n",
		    status);
		return -EPERM;
	}

	if (status & PMIC_ARB_STATUS_FAILURE) {
		dev_err(dev->dev,
		    "wait_for_done: transaction failed (0x%x)\n", status);
		return -EIO;
	}

	if (status & PMIC_ARB_STATUS_DROPPED) {
		dev_err(dev->dev,
		    "wait_for_done: transaction dropped pmic-arb busy (0x%x)\n",
		    status);
		return -EAGAIN;
	}

	return 0;
}

static int pmic_arb_wait_for_done(struct spmi_pmic_arb_dev *dev)
{
	u32 status = 0;
	u32 timeout = PMIC_ARB_TIMEOUT_US;
	u32 offset = PMIC_ARB_STATUS(dev->channel);

	while (timeout--) {
		status = pmic_arb_read(dev, offset);

		if (status & PMIC_ARB_STATUS_DONE)
			return pmic_arb_diagnosis(dev, status);

		udelay(1);
	}

	dev_err(dev->dev, "wait_for_done:: timeout, status 0x%x\n", status);
	return -ETIMEDOUT;
}

/**
 * pa_read_data: reads pmic-arb's register and copy 1..4 bytes to buf
 * @bc byte count -1. range: 0..3
 * @reg register's address
 * @buf output parameter, length must be bc+1
 */
static void pa_read_data(struct spmi_pmic_arb_dev *dev, u8 *buf, u32 reg, u8 bc)
{
	u32 data = pmic_arb_read(dev, reg);
=======
#define spec_to_hwirq(slave_id, periph_id, irq_id, apid) \
	((((slave_id) & 0xF)   << 28) | \
	(((periph_id) & 0xFF)  << 20) | \
	(((irq_id)    & 0x7)   << 16) | \
	(((apid)      & 0x3FF) << 0))

#define hwirq_to_sid(hwirq)  (((hwirq) >> 28) & 0xF)
#define hwirq_to_per(hwirq)  (((hwirq) >> 20) & 0xFF)
#define hwirq_to_irq(hwirq)  (((hwirq) >> 16) & 0x7)
#define hwirq_to_apid(hwirq) (((hwirq) >> 0)  & 0x3FF)

struct pmic_arb_ver_ops;

struct apid_data {
	u16		ppid;
	u8		write_ee;
	u8		irq_ee;
};

/**
 * struct spmi_pmic_arb - SPMI PMIC Arbiter object
 *
 * @rd_base:		on v1 "core", on v2 "observer" register base off DT.
 * @wr_base:		on v1 "core", on v2 "chnls"    register base off DT.
 * @intr:		address of the SPMI interrupt control registers.
 * @cnfg:		address of the PMIC Arbiter configuration registers.
 * @lock:		lock to synchronize accesses.
 * @channel:		execution environment channel to use for accesses.
 * @irq:		PMIC ARB interrupt.
 * @ee:			the current Execution Environment
 * @bus_instance:	on v7: 0 = primary SPMI bus, 1 = secondary SPMI bus
 * @min_apid:		minimum APID (used for bounding IRQ search)
 * @max_apid:		maximum APID
 * @base_apid:		on v7: minimum APID associated with the particular SPMI
 *			bus instance
 * @apid_count:		on v5 and v7: number of APIDs associated with the
 *			particular SPMI bus instance
 * @mapping_table:	in-memory copy of PPID -> APID mapping table.
 * @domain:		irq domain object for PMIC IRQ domain
 * @spmic:		SPMI controller object
 * @ver_ops:		version dependent operations.
 * @ppid_to_apid:	in-memory copy of PPID -> APID mapping table.
 * @last_apid:		Highest value APID in use
 * @apid_data:		Table of data for all APIDs
 * @max_periphs:	Number of elements in apid_data[]
 */
struct spmi_pmic_arb {
	void __iomem		*rd_base;
	void __iomem		*wr_base;
	void __iomem		*intr;
	void __iomem		*cnfg;
	void __iomem		*core;
	resource_size_t		core_size;
	raw_spinlock_t		lock;
	u8			channel;
	int			irq;
	u8			ee;
	u32			bus_instance;
	u16			min_apid;
	u16			max_apid;
	u16			base_apid;
	int			apid_count;
	u32			*mapping_table;
	DECLARE_BITMAP(mapping_table_valid, PMIC_ARB_MAX_PERIPHS);
	struct irq_domain	*domain;
	struct spmi_controller	*spmic;
	const struct pmic_arb_ver_ops *ver_ops;
	u16			*ppid_to_apid;
	u16			last_apid;
	struct apid_data	*apid_data;
	int			max_periphs;
};

/**
 * struct pmic_arb_ver_ops - version dependent functionality.
 *
 * @ver_str:		version string.
 * @ppid_to_apid:	finds the apid for a given ppid.
 * @non_data_cmd:	on v1 issues an spmi non-data command.
 *			on v2 no HW support, returns -EOPNOTSUPP.
 * @offset:		on v1 offset of per-ee channel.
 *			on v2 offset of per-ee and per-ppid channel.
 * @fmt_cmd:		formats a GENI/SPMI command.
 * @owner_acc_status:	on v1 address of PMIC_ARB_SPMI_PIC_OWNERm_ACC_STATUSn
 *			on v2 address of SPMI_PIC_OWNERm_ACC_STATUSn.
 * @acc_enable:		on v1 address of PMIC_ARB_SPMI_PIC_ACC_ENABLEn
 *			on v2 address of SPMI_PIC_ACC_ENABLEn.
 * @irq_status:		on v1 address of PMIC_ARB_SPMI_PIC_IRQ_STATUSn
 *			on v2 address of SPMI_PIC_IRQ_STATUSn.
 * @irq_clear:		on v1 address of PMIC_ARB_SPMI_PIC_IRQ_CLEARn
 *			on v2 address of SPMI_PIC_IRQ_CLEARn.
 * @apid_map_offset:	offset of PMIC_ARB_REG_CHNLn
 * @apid_owner:		on v2 and later address of SPMI_PERIPHn_2OWNER_TABLE_REG
 */
struct pmic_arb_ver_ops {
	const char *ver_str;
	int (*ppid_to_apid)(struct spmi_pmic_arb *pmic_arb, u16 ppid);
	/* spmi commands (read_cmd, write_cmd, cmd) functionality */
	int (*offset)(struct spmi_pmic_arb *pmic_arb, u8 sid, u16 addr,
			enum pmic_arb_channel ch_type);
	u32 (*fmt_cmd)(u8 opc, u8 sid, u16 addr, u8 bc);
	int (*non_data_cmd)(struct spmi_controller *ctrl, u8 opc, u8 sid);
	/* Interrupts controller functionality (offset of PIC registers) */
	void __iomem *(*owner_acc_status)(struct spmi_pmic_arb *pmic_arb, u8 m,
					  u16 n);
	void __iomem *(*acc_enable)(struct spmi_pmic_arb *pmic_arb, u16 n);
	void __iomem *(*irq_status)(struct spmi_pmic_arb *pmic_arb, u16 n);
	void __iomem *(*irq_clear)(struct spmi_pmic_arb *pmic_arb, u16 n);
	u32 (*apid_map_offset)(u16 n);
	void __iomem *(*apid_owner)(struct spmi_pmic_arb *pmic_arb, u16 n);
};

static inline void pmic_arb_base_write(struct spmi_pmic_arb *pmic_arb,
				       u32 offset, u32 val)
{
	writel_relaxed(val, pmic_arb->wr_base + offset);
}

static inline void pmic_arb_set_rd_cmd(struct spmi_pmic_arb *pmic_arb,
				       u32 offset, u32 val)
{
	writel_relaxed(val, pmic_arb->rd_base + offset);
}

/**
 * pmic_arb_read_data: reads pmic-arb's register and copy 1..4 bytes to buf
 * @bc:		byte count -1. range: 0..3
 * @reg:	register's address
 * @buf:	output parameter, length must be bc + 1
 */
static void
pmic_arb_read_data(struct spmi_pmic_arb *pmic_arb, u8 *buf, u32 reg, u8 bc)
{
	u32 data = __raw_readl(pmic_arb->rd_base + reg);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(buf, &data, (bc & 3) + 1);
}

/**
<<<<<<< HEAD
 * pa_write_data: write 1..4 bytes from buf to pmic-arb's register
 * @bc byte-count -1. range: 0..3
 * @reg register's address
 * @buf buffer to write. length must be bc+1
 */
static void
pa_write_data(struct spmi_pmic_arb_dev *dev, u8 *buf, u32 reg, u8 bc)
{
	u32 data = 0;
	memcpy(&data, buf, (bc & 3) + 1);
	pmic_arb_write(dev, reg, data);
}

static void pmic_arb_dbg_err_dump(struct spmi_pmic_arb_dev *pmic_arb, int ret,
		const char *msg, u8 opc, u8 sid, u16 addr, u8 bc, u8 *buf)
{
	u32 irq_stat  = readl_relaxed(pmic_arb->cnfg
				+ PMIC_ARB_PROTOCOL_IRQ_STATUS);
	u32 geni_stat = readl_relaxed(pmic_arb->cnfg + PMIC_ARB_GENI_STATUS);
	u32 geni_ctrl = readl_relaxed(pmic_arb->cnfg + PMIC_ARB_GENI_CTRL);

	bc += 1; /* actual byte count */

	if (buf)
		dev_err(pmic_arb->dev,
		"error:%d on data %s  opcode:0x%x sid:%d addr:0x%x bc:%d buf:%*phC\n",
			ret, msg, opc, sid, addr, bc, bc, buf);
	else
		dev_err(pmic_arb->dev,
		"error:%d on non-data-cmd opcode:0x%x sid:%d\n",
			ret, opc, sid);
	dev_err(pmic_arb->dev,
		"PROTOCOL_IRQ_STATUS before:0x%x after:0x%x GENI_STATUS:0x%x GENI_CTRL:0x%x\n",
		irq_stat, pmic_arb->prev_prtcl_irq_stat, geni_stat, geni_ctrl);
=======
 * pmic_arb_write_data: write 1..4 bytes from buf to pmic-arb's register
 * @bc:		byte-count -1. range: 0..3.
 * @reg:	register's address.
 * @buf:	buffer to write. length must be bc + 1.
 */
static void pmic_arb_write_data(struct spmi_pmic_arb *pmic_arb, const u8 *buf,
				u32 reg, u8 bc)
{
	u32 data = 0;

	memcpy(&data, buf, (bc & 3) + 1);
	__raw_writel(data, pmic_arb->wr_base + reg);
}

static int pmic_arb_wait_for_done(struct spmi_controller *ctrl,
				  void __iomem *base, u8 sid, u16 addr,
				  enum pmic_arb_channel ch_type)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	u32 status = 0;
	u32 timeout = PMIC_ARB_TIMEOUT_US;
	u32 offset;
	int rc;

	rc = pmic_arb->ver_ops->offset(pmic_arb, sid, addr, ch_type);
	if (rc < 0)
		return rc;

	offset = rc;
	offset += PMIC_ARB_STATUS;

	while (timeout--) {
		status = readl_relaxed(base + offset);

		if (status & PMIC_ARB_STATUS_DONE) {
			if (status & PMIC_ARB_STATUS_DENIED) {
				dev_err(&ctrl->dev, "%s: %#x %#x: transaction denied (%#x)\n",
					__func__, sid, addr, status);
				return -EPERM;
			}

			if (status & PMIC_ARB_STATUS_FAILURE) {
				dev_err(&ctrl->dev, "%s: %#x %#x: transaction failed (%#x)\n",
					__func__, sid, addr, status);
				WARN_ON(1);
				return -EIO;
			}

			if (status & PMIC_ARB_STATUS_DROPPED) {
				dev_err(&ctrl->dev, "%s: %#x %#x: transaction dropped (%#x)\n",
					__func__, sid, addr, status);
				return -EIO;
			}

			return 0;
		}
		udelay(1);
	}

	dev_err(&ctrl->dev, "%s: %#x %#x: timeout, status %#x\n",
		__func__, sid, addr, status);
	return -ETIMEDOUT;
}

static int
pmic_arb_non_data_cmd_v1(struct spmi_controller *ctrl, u8 opc, u8 sid)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	unsigned long flags;
	u32 cmd;
	int rc;
	u32 offset;

	rc = pmic_arb->ver_ops->offset(pmic_arb, sid, 0, PMIC_ARB_CHANNEL_RW);
	if (rc < 0)
		return rc;

	offset = rc;
	cmd = ((opc | 0x40) << 27) | ((sid & 0xf) << 20);

	raw_spin_lock_irqsave(&pmic_arb->lock, flags);
	pmic_arb_base_write(pmic_arb, offset + PMIC_ARB_CMD, cmd);
	rc = pmic_arb_wait_for_done(ctrl, pmic_arb->wr_base, sid, 0,
				    PMIC_ARB_CHANNEL_RW);
	raw_spin_unlock_irqrestore(&pmic_arb->lock, flags);

	return rc;
}

static int
pmic_arb_non_data_cmd_v2(struct spmi_controller *ctrl, u8 opc, u8 sid)
{
	return -EOPNOTSUPP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Non-data command */
static int pmic_arb_cmd(struct spmi_controller *ctrl, u8 opc, u8 sid)
{
<<<<<<< HEAD
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	unsigned long flags;
	u32 cmd;
	int rc;

	pr_debug("op:0x%x sid:%d\n", opc, sid);
=======
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);

	dev_dbg(&ctrl->dev, "cmd op:0x%x sid:%d\n", opc, sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check for valid non-data command */
	if (opc < SPMI_CMD_RESET || opc > SPMI_CMD_WAKEUP)
		return -EINVAL;

<<<<<<< HEAD
	opc -= SPMI_CMD_RESET - PMIC_ARB_OP_RESET;

	cmd = (opc << 27) | ((sid & 0xf) << 20);

	spin_lock_irqsave(&pmic_arb->lock, flags);
	pmic_arb_save_stat_before_txn(pmic_arb);
	pmic_arb_write(pmic_arb, PMIC_ARB_CMD(pmic_arb->channel), cmd);
	rc = pmic_arb_wait_for_done(pmic_arb);
	spin_unlock_irqrestore(&pmic_arb->lock, flags);

	if (rc)
		pmic_arb_dbg_err_dump(pmic_arb, rc, "cmd", opc, sid, 0, 0, 0);
	return rc;
}

static int pmic_arb_read_cmd(struct spmi_controller *ctrl,
				u8 opc, u8 sid, u16 addr, u8 bc, u8 *buf)
{
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	unsigned long flags;
	u32 cmd;
	int rc;

	if (bc >= PMIC_ARB_MAX_TRANS_BYTES) {
		dev_err(pmic_arb->dev
		, "pmic-arb supports 1..%d bytes per trans, but:%d requested"
					, PMIC_ARB_MAX_TRANS_BYTES, bc+1);
		return  -EINVAL;
	}
	dev_dbg(pmic_arb->dev, "client-rd op:0x%x sid:%d addr:0x%x bc:%d\n",
							opc, sid, addr, bc + 1);
=======
	return pmic_arb->ver_ops->non_data_cmd(ctrl, opc, sid);
}

static int pmic_arb_fmt_read_cmd(struct spmi_pmic_arb *pmic_arb, u8 opc, u8 sid,
				 u16 addr, size_t len, u32 *cmd, u32 *offset)
{
	u8 bc = len - 1;
	int rc;

	rc = pmic_arb->ver_ops->offset(pmic_arb, sid, addr,
				       PMIC_ARB_CHANNEL_OBS);
	if (rc < 0)
		return rc;

	*offset = rc;
	if (bc >= PMIC_ARB_MAX_TRANS_BYTES) {
		dev_err(&pmic_arb->spmic->dev, "pmic-arb supports 1..%d bytes per trans, but:%zu requested",
			PMIC_ARB_MAX_TRANS_BYTES, len);
		return  -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check the opcode */
	if (opc >= 0x60 && opc <= 0x7F)
		opc = PMIC_ARB_OP_READ;
	else if (opc >= 0x20 && opc <= 0x2F)
		opc = PMIC_ARB_OP_EXT_READ;
	else if (opc >= 0x38 && opc <= 0x3F)
		opc = PMIC_ARB_OP_EXT_READL;
	else
		return -EINVAL;

<<<<<<< HEAD
	cmd = (opc << 27) | ((sid & 0xf) << 20) | (addr << 4) | (bc & 0x7);

	spin_lock_irqsave(&pmic_arb->lock, flags);
	pmic_arb_save_stat_before_txn(pmic_arb);
	pmic_arb_write(pmic_arb, PMIC_ARB_CMD(pmic_arb->channel), cmd);
	rc = pmic_arb_wait_for_done(pmic_arb);
	if (rc)
		goto done;

	/* Read from FIFO, note 'bc' is actually number of bytes minus 1 */
	pa_read_data(pmic_arb, buf, PMIC_ARB_RDATA0(pmic_arb->channel)
							, min_t(u8, bc, 3));

	if (bc > 3)
		pa_read_data(pmic_arb, buf + 4,
				PMIC_ARB_RDATA1(pmic_arb->channel), bc - 4);

done:
	spin_unlock_irqrestore(&pmic_arb->lock, flags);
	if (rc)
		pmic_arb_dbg_err_dump(pmic_arb, rc, "read", opc, sid, addr, bc,
									buf);
	return rc;
}

static int pmic_arb_write_cmd(struct spmi_controller *ctrl,
				u8 opc, u8 sid, u16 addr, u8 bc, u8 *buf)
{
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	unsigned long flags;
	u32 cmd;
	int rc;

	if (bc >= PMIC_ARB_MAX_TRANS_BYTES) {
		dev_err(pmic_arb->dev
		, "pmic-arb supports 1..%d bytes per trans, but:%d requested"
					, PMIC_ARB_MAX_TRANS_BYTES, bc+1);
		return  -EINVAL;
	}
	dev_dbg(pmic_arb->dev, "client-wr op:0x%x sid:%d addr:0x%x bc:%d\n",
							opc, sid, addr, bc + 1);
=======
	*cmd = pmic_arb->ver_ops->fmt_cmd(opc, sid, addr, bc);

	return 0;
}

static int pmic_arb_read_cmd_unlocked(struct spmi_controller *ctrl, u32 cmd,
				      u32 offset, u8 sid, u16 addr, u8 *buf,
				      size_t len)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	u8 bc = len - 1;
	int rc;

	pmic_arb_set_rd_cmd(pmic_arb, offset + PMIC_ARB_CMD, cmd);
	rc = pmic_arb_wait_for_done(ctrl, pmic_arb->rd_base, sid, addr,
				    PMIC_ARB_CHANNEL_OBS);
	if (rc)
		return rc;

	pmic_arb_read_data(pmic_arb, buf, offset + PMIC_ARB_RDATA0,
		     min_t(u8, bc, 3));

	if (bc > 3)
		pmic_arb_read_data(pmic_arb, buf + 4, offset + PMIC_ARB_RDATA1,
					bc - 4);
	return 0;
}

static int pmic_arb_read_cmd(struct spmi_controller *ctrl, u8 opc, u8 sid,
			     u16 addr, u8 *buf, size_t len)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	unsigned long flags;
	u32 cmd, offset;
	int rc;

	rc = pmic_arb_fmt_read_cmd(pmic_arb, opc, sid, addr, len, &cmd,
				   &offset);
	if (rc)
		return rc;

	raw_spin_lock_irqsave(&pmic_arb->lock, flags);
	rc = pmic_arb_read_cmd_unlocked(ctrl, cmd, offset, sid, addr, buf, len);
	raw_spin_unlock_irqrestore(&pmic_arb->lock, flags);

	return rc;
}

static int pmic_arb_fmt_write_cmd(struct spmi_pmic_arb *pmic_arb, u8 opc,
				  u8 sid, u16 addr, size_t len, u32 *cmd,
				  u32 *offset)
{
	u8 bc = len - 1;
	int rc;

	rc = pmic_arb->ver_ops->offset(pmic_arb, sid, addr,
					PMIC_ARB_CHANNEL_RW);
	if (rc < 0)
		return rc;

	*offset = rc;
	if (bc >= PMIC_ARB_MAX_TRANS_BYTES) {
		dev_err(&pmic_arb->spmic->dev, "pmic-arb supports 1..%d bytes per trans, but:%zu requested",
			PMIC_ARB_MAX_TRANS_BYTES, len);
		return  -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check the opcode */
	if (opc >= 0x40 && opc <= 0x5F)
		opc = PMIC_ARB_OP_WRITE;
<<<<<<< HEAD
	else if (opc >= 0x00 && opc <= 0x0F)
		opc = PMIC_ARB_OP_EXT_WRITE;
	else if (opc >= 0x30 && opc <= 0x37)
		opc = PMIC_ARB_OP_EXT_WRITEL;
	else if (opc >= 0x80 && opc <= 0xFF)
=======
	else if (opc <= 0x0F)
		opc = PMIC_ARB_OP_EXT_WRITE;
	else if (opc >= 0x30 && opc <= 0x37)
		opc = PMIC_ARB_OP_EXT_WRITEL;
	else if (opc >= 0x80)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		opc = PMIC_ARB_OP_ZERO_WRITE;
	else
		return -EINVAL;

<<<<<<< HEAD
	cmd = (opc << 27) | ((sid & 0xf) << 20) | (addr << 4) | (bc & 0x7);

	/* Write data to FIFOs */
	spin_lock_irqsave(&pmic_arb->lock, flags);
	pmic_arb_save_stat_before_txn(pmic_arb);
	pa_write_data(pmic_arb, buf, PMIC_ARB_WDATA0(pmic_arb->channel)
							, min_t(u8, bc, 3));
	if (bc > 3)
		pa_write_data(pmic_arb, buf + 4,
				PMIC_ARB_WDATA1(pmic_arb->channel), bc - 4);

	/* Start the transaction */
	pmic_arb_write(pmic_arb, PMIC_ARB_CMD(pmic_arb->channel), cmd);
	rc = pmic_arb_wait_for_done(pmic_arb);
	spin_unlock_irqrestore(&pmic_arb->lock, flags);

	if (rc)
		pmic_arb_dbg_err_dump(pmic_arb, rc, "write", opc, sid, addr, bc,
									buf);
=======
	*cmd = pmic_arb->ver_ops->fmt_cmd(opc, sid, addr, bc);

	return 0;
}

static int pmic_arb_write_cmd_unlocked(struct spmi_controller *ctrl, u32 cmd,
				      u32 offset, u8 sid, u16 addr,
				      const u8 *buf, size_t len)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	u8 bc = len - 1;

	/* Write data to FIFOs */
	pmic_arb_write_data(pmic_arb, buf, offset + PMIC_ARB_WDATA0,
				min_t(u8, bc, 3));
	if (bc > 3)
		pmic_arb_write_data(pmic_arb, buf + 4, offset + PMIC_ARB_WDATA1,
					bc - 4);

	/* Start the transaction */
	pmic_arb_base_write(pmic_arb, offset + PMIC_ARB_CMD, cmd);
	return pmic_arb_wait_for_done(ctrl, pmic_arb->wr_base, sid, addr,
				      PMIC_ARB_CHANNEL_RW);
}

static int pmic_arb_write_cmd(struct spmi_controller *ctrl, u8 opc, u8 sid,
			      u16 addr, const u8 *buf, size_t len)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	unsigned long flags;
	u32 cmd, offset;
	int rc;

	rc = pmic_arb_fmt_write_cmd(pmic_arb, opc, sid, addr, len, &cmd,
				    &offset);
	if (rc)
		return rc;

	raw_spin_lock_irqsave(&pmic_arb->lock, flags);
	rc = pmic_arb_write_cmd_unlocked(ctrl, cmd, offset, sid, addr, buf,
					 len);
	raw_spin_unlock_irqrestore(&pmic_arb->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rc;
}

<<<<<<< HEAD
/* APID to PPID */
static u16 get_peripheral_id(struct spmi_pmic_arb_dev *pmic_arb, u8 apid)
{
	return pmic_arb->periph_id_map[apid] & PMIC_ARB_PPID_MASK;
}

/* APID to PPID, returns valid flag */
static int is_apid_valid(struct spmi_pmic_arb_dev *pmic_arb, u8 apid)
{
	return pmic_arb->periph_id_map[apid] & PMIC_ARB_PERIPH_ID_VALID;
}

static u32 search_mapping_table(struct spmi_pmic_arb_dev *pmic_arb, u16 ppid)
{
	u32 *mapping_table = pmic_arb->mapping_table;
	u32 apid = PMIC_ARB_MAX_PERIPHS;
	int index = 0;
	u32 data;
	int i;

	for (i = 0; i < SPMI_MAPPING_TABLE_TREE_DEPTH; ++i) {
		data = mapping_table[index];

		if (ppid & (1 << SPMI_MAPPING_BIT_INDEX(data))) {
=======
static int pmic_arb_masked_write(struct spmi_controller *ctrl, u8 sid, u16 addr,
				 const u8 *buf, const u8 *mask, size_t len)
{
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	u32 read_cmd, read_offset, write_cmd, write_offset;
	u8 temp[PMIC_ARB_MAX_TRANS_BYTES];
	unsigned long flags;
	int rc, i;

	rc = pmic_arb_fmt_read_cmd(pmic_arb, SPMI_CMD_EXT_READL, sid, addr, len,
				   &read_cmd, &read_offset);
	if (rc)
		return rc;

	rc = pmic_arb_fmt_write_cmd(pmic_arb, SPMI_CMD_EXT_WRITEL, sid, addr,
				    len, &write_cmd, &write_offset);
	if (rc)
		return rc;

	raw_spin_lock_irqsave(&pmic_arb->lock, flags);
	rc = pmic_arb_read_cmd_unlocked(ctrl, read_cmd, read_offset, sid, addr,
					temp, len);
	if (rc)
		goto done;

	for (i = 0; i < len; i++)
		temp[i] = (temp[i] & ~mask[i]) | (buf[i] & mask[i]);

	rc = pmic_arb_write_cmd_unlocked(ctrl, write_cmd, write_offset, sid,
					 addr, temp, len);
done:
	raw_spin_unlock_irqrestore(&pmic_arb->lock, flags);

	return rc;
}

enum qpnpint_regs {
	QPNPINT_REG_RT_STS		= 0x10,
	QPNPINT_REG_SET_TYPE		= 0x11,
	QPNPINT_REG_POLARITY_HIGH	= 0x12,
	QPNPINT_REG_POLARITY_LOW	= 0x13,
	QPNPINT_REG_LATCHED_CLR		= 0x14,
	QPNPINT_REG_EN_SET		= 0x15,
	QPNPINT_REG_EN_CLR		= 0x16,
	QPNPINT_REG_LATCHED_STS		= 0x18,
};

struct spmi_pmic_arb_qpnpint_type {
	u8 type; /* 1 -> edge */
	u8 polarity_high;
	u8 polarity_low;
} __packed;

/* Simplified accessor functions for irqchip callbacks */
static void qpnpint_spmi_write(struct irq_data *d, u8 reg, void *buf,
			       size_t len)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	u8 sid = hwirq_to_sid(d->hwirq);
	u8 per = hwirq_to_per(d->hwirq);

	if (pmic_arb_write_cmd(pmic_arb->spmic, SPMI_CMD_EXT_WRITEL, sid,
			       (per << 8) + reg, buf, len))
		dev_err_ratelimited(&pmic_arb->spmic->dev, "failed irqchip transaction on %x\n",
				    d->irq);
}

static void qpnpint_spmi_read(struct irq_data *d, u8 reg, void *buf, size_t len)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	u8 sid = hwirq_to_sid(d->hwirq);
	u8 per = hwirq_to_per(d->hwirq);

	if (pmic_arb_read_cmd(pmic_arb->spmic, SPMI_CMD_EXT_READL, sid,
			      (per << 8) + reg, buf, len))
		dev_err_ratelimited(&pmic_arb->spmic->dev, "failed irqchip transaction on %x\n",
				    d->irq);
}

static int qpnpint_spmi_masked_write(struct irq_data *d, u8 reg,
				     const void *buf, const void *mask,
				     size_t len)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	u8 sid = hwirq_to_sid(d->hwirq);
	u8 per = hwirq_to_per(d->hwirq);
	int rc;

	rc = pmic_arb_masked_write(pmic_arb->spmic, sid, (per << 8) + reg, buf,
				   mask, len);
	if (rc)
		dev_err_ratelimited(&pmic_arb->spmic->dev, "failed irqchip transaction on %x rc=%d\n",
				    d->irq, rc);
	return rc;
}

static void cleanup_irq(struct spmi_pmic_arb *pmic_arb, u16 apid, int id)
{
	u16 ppid = pmic_arb->apid_data[apid].ppid;
	u8 sid = ppid >> 8;
	u8 per = ppid & 0xFF;
	u8 irq_mask = BIT(id);

	dev_err_ratelimited(&pmic_arb->spmic->dev, "%s apid=%d sid=0x%x per=0x%x irq=%d\n",
			__func__, apid, sid, per, id);
	writel_relaxed(irq_mask, pmic_arb->ver_ops->irq_clear(pmic_arb, apid));
}

static int periph_interrupt(struct spmi_pmic_arb *pmic_arb, u16 apid)
{
	unsigned int irq;
	u32 status, id;
	int handled = 0;
	u8 sid = (pmic_arb->apid_data[apid].ppid >> 8) & 0xF;
	u8 per = pmic_arb->apid_data[apid].ppid & 0xFF;

	status = readl_relaxed(pmic_arb->ver_ops->irq_status(pmic_arb, apid));
	while (status) {
		id = ffs(status) - 1;
		status &= ~BIT(id);
		irq = irq_find_mapping(pmic_arb->domain,
					spec_to_hwirq(sid, per, id, apid));
		if (irq == 0) {
			cleanup_irq(pmic_arb, apid, id);
			continue;
		}
		generic_handle_irq(irq);
		handled++;
	}

	return handled;
}

static void pmic_arb_chained_irq(struct irq_desc *desc)
{
	struct spmi_pmic_arb *pmic_arb = irq_desc_get_handler_data(desc);
	const struct pmic_arb_ver_ops *ver_ops = pmic_arb->ver_ops;
	struct irq_chip *chip = irq_desc_get_chip(desc);
	int first = pmic_arb->min_apid;
	int last = pmic_arb->max_apid;
	/*
	 * acc_offset will be non-zero for the secondary SPMI bus instance on
	 * v7 controllers.
	 */
	int acc_offset = pmic_arb->base_apid >> 5;
	u8 ee = pmic_arb->ee;
	u32 status, enable, handled = 0;
	int i, id, apid;
	/* status based dispatch */
	bool acc_valid = false;
	u32 irq_status = 0;

	chained_irq_enter(chip, desc);

	for (i = first >> 5; i <= last >> 5; ++i) {
		status = readl_relaxed(ver_ops->owner_acc_status(pmic_arb, ee, i - acc_offset));
		if (status)
			acc_valid = true;

		while (status) {
			id = ffs(status) - 1;
			status &= ~BIT(id);
			apid = id + i * 32;
			if (apid < first || apid > last) {
				WARN_ONCE(true, "spurious spmi irq received for apid=%d\n",
					apid);
				continue;
			}
			enable = readl_relaxed(
					ver_ops->acc_enable(pmic_arb, apid));
			if (enable & SPMI_PIC_ACC_ENABLE_BIT)
				if (periph_interrupt(pmic_arb, apid) != 0)
					handled++;
		}
	}

	/* ACC_STATUS is empty but IRQ fired check IRQ_STATUS */
	if (!acc_valid) {
		for (i = first; i <= last; i++) {
			/* skip if APPS is not irq owner */
			if (pmic_arb->apid_data[i].irq_ee != pmic_arb->ee)
				continue;

			irq_status = readl_relaxed(
					     ver_ops->irq_status(pmic_arb, i));
			if (irq_status) {
				enable = readl_relaxed(
					     ver_ops->acc_enable(pmic_arb, i));
				if (enable & SPMI_PIC_ACC_ENABLE_BIT) {
					dev_dbg(&pmic_arb->spmic->dev,
						"Dispatching IRQ for apid=%d status=%x\n",
						i, irq_status);
					if (periph_interrupt(pmic_arb, i) != 0)
						handled++;
				}
			}
		}
	}

	if (handled == 0)
		handle_bad_irq(desc);

	chained_irq_exit(chip, desc);
}

static void qpnpint_irq_ack(struct irq_data *d)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	u8 irq = hwirq_to_irq(d->hwirq);
	u16 apid = hwirq_to_apid(d->hwirq);
	u8 data;

	writel_relaxed(BIT(irq), pmic_arb->ver_ops->irq_clear(pmic_arb, apid));

	data = BIT(irq);
	qpnpint_spmi_write(d, QPNPINT_REG_LATCHED_CLR, &data, 1);
}

static void qpnpint_irq_mask(struct irq_data *d)
{
	u8 irq = hwirq_to_irq(d->hwirq);
	u8 data = BIT(irq);

	qpnpint_spmi_write(d, QPNPINT_REG_EN_CLR, &data, 1);
}

static void qpnpint_irq_unmask(struct irq_data *d)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	const struct pmic_arb_ver_ops *ver_ops = pmic_arb->ver_ops;
	u8 irq = hwirq_to_irq(d->hwirq);
	u16 apid = hwirq_to_apid(d->hwirq);
	u8 buf[2];

	writel_relaxed(SPMI_PIC_ACC_ENABLE_BIT,
			ver_ops->acc_enable(pmic_arb, apid));

	qpnpint_spmi_read(d, QPNPINT_REG_EN_SET, &buf[0], 1);
	if (!(buf[0] & BIT(irq))) {
		/*
		 * Since the interrupt is currently disabled, write to both the
		 * LATCHED_CLR and EN_SET registers so that a spurious interrupt
		 * cannot be triggered when the interrupt is enabled
		 */
		buf[0] = BIT(irq);
		buf[1] = BIT(irq);
		qpnpint_spmi_write(d, QPNPINT_REG_LATCHED_CLR, &buf, 2);
	}
}

static int qpnpint_irq_set_type(struct irq_data *d, unsigned int flow_type)
{
	struct spmi_pmic_arb_qpnpint_type type = {0};
	struct spmi_pmic_arb_qpnpint_type mask;
	irq_flow_handler_t flow_handler;
	u8 irq_bit = BIT(hwirq_to_irq(d->hwirq));
	int rc;

	if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)) {
		type.type = irq_bit;
		if (flow_type & IRQF_TRIGGER_RISING)
			type.polarity_high = irq_bit;
		if (flow_type & IRQF_TRIGGER_FALLING)
			type.polarity_low = irq_bit;

		flow_handler = handle_edge_irq;
	} else {
		if ((flow_type & (IRQF_TRIGGER_HIGH)) &&
		    (flow_type & (IRQF_TRIGGER_LOW)))
			return -EINVAL;

		if (flow_type & IRQF_TRIGGER_HIGH)
			type.polarity_high = irq_bit;
		else
			type.polarity_low = irq_bit;

		flow_handler = handle_level_irq;
	}

	mask.type = irq_bit;
	mask.polarity_high = irq_bit;
	mask.polarity_low = irq_bit;

	rc = qpnpint_spmi_masked_write(d, QPNPINT_REG_SET_TYPE, &type, &mask,
				       sizeof(type));
	irq_set_handler_locked(d, flow_handler);

	return rc;
}

static int qpnpint_irq_set_wake(struct irq_data *d, unsigned int on)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);

	return irq_set_irq_wake(pmic_arb->irq, on);
}

static int qpnpint_get_irqchip_state(struct irq_data *d,
				     enum irqchip_irq_state which,
				     bool *state)
{
	u8 irq = hwirq_to_irq(d->hwirq);
	u8 status = 0;

	if (which != IRQCHIP_STATE_LINE_LEVEL)
		return -EINVAL;

	qpnpint_spmi_read(d, QPNPINT_REG_RT_STS, &status, 1);
	*state = !!(status & BIT(irq));

	return 0;
}

static int qpnpint_irq_domain_activate(struct irq_domain *domain,
				       struct irq_data *d, bool reserve)
{
	struct spmi_pmic_arb *pmic_arb = irq_data_get_irq_chip_data(d);
	u16 periph = hwirq_to_per(d->hwirq);
	u16 apid = hwirq_to_apid(d->hwirq);
	u16 sid = hwirq_to_sid(d->hwirq);
	u16 irq = hwirq_to_irq(d->hwirq);
	u8 buf;

	if (pmic_arb->apid_data[apid].irq_ee != pmic_arb->ee) {
		dev_err(&pmic_arb->spmic->dev, "failed to xlate sid = %#x, periph = %#x, irq = %u: ee=%u but owner=%u\n",
			sid, periph, irq, pmic_arb->ee,
			pmic_arb->apid_data[apid].irq_ee);
		return -ENODEV;
	}

	buf = BIT(irq);
	qpnpint_spmi_write(d, QPNPINT_REG_EN_CLR, &buf, 1);
	qpnpint_spmi_write(d, QPNPINT_REG_LATCHED_CLR, &buf, 1);

	return 0;
}

static struct irq_chip pmic_arb_irqchip = {
	.name		= "pmic_arb",
	.irq_ack	= qpnpint_irq_ack,
	.irq_mask	= qpnpint_irq_mask,
	.irq_unmask	= qpnpint_irq_unmask,
	.irq_set_type	= qpnpint_irq_set_type,
	.irq_set_wake	= qpnpint_irq_set_wake,
	.irq_get_irqchip_state	= qpnpint_get_irqchip_state,
	.flags		= IRQCHIP_MASK_ON_SUSPEND,
};

static int qpnpint_irq_domain_translate(struct irq_domain *d,
					struct irq_fwspec *fwspec,
					unsigned long *out_hwirq,
					unsigned int *out_type)
{
	struct spmi_pmic_arb *pmic_arb = d->host_data;
	u32 *intspec = fwspec->param;
	u16 apid, ppid;
	int rc;

	dev_dbg(&pmic_arb->spmic->dev, "intspec[0] 0x%1x intspec[1] 0x%02x intspec[2] 0x%02x\n",
		intspec[0], intspec[1], intspec[2]);

	if (irq_domain_get_of_node(d) != pmic_arb->spmic->dev.of_node)
		return -EINVAL;
	if (fwspec->param_count != 4)
		return -EINVAL;
	if (intspec[0] > 0xF || intspec[1] > 0xFF || intspec[2] > 0x7)
		return -EINVAL;

	ppid = intspec[0] << 8 | intspec[1];
	rc = pmic_arb->ver_ops->ppid_to_apid(pmic_arb, ppid);
	if (rc < 0) {
		dev_err(&pmic_arb->spmic->dev, "failed to xlate sid = %#x, periph = %#x, irq = %u rc = %d\n",
		intspec[0], intspec[1], intspec[2], rc);
		return rc;
	}

	apid = rc;
	/* Keep track of {max,min}_apid for bounding search during interrupt */
	if (apid > pmic_arb->max_apid)
		pmic_arb->max_apid = apid;
	if (apid < pmic_arb->min_apid)
		pmic_arb->min_apid = apid;

	*out_hwirq = spec_to_hwirq(intspec[0], intspec[1], intspec[2], apid);
	*out_type  = intspec[3] & IRQ_TYPE_SENSE_MASK;

	dev_dbg(&pmic_arb->spmic->dev, "out_hwirq = %lu\n", *out_hwirq);

	return 0;
}

static struct lock_class_key qpnpint_irq_lock_class, qpnpint_irq_request_class;

static void qpnpint_irq_domain_map(struct spmi_pmic_arb *pmic_arb,
				   struct irq_domain *domain, unsigned int virq,
				   irq_hw_number_t hwirq, unsigned int type)
{
	irq_flow_handler_t handler;

	dev_dbg(&pmic_arb->spmic->dev, "virq = %u, hwirq = %lu, type = %u\n",
		virq, hwirq, type);

	if (type & IRQ_TYPE_EDGE_BOTH)
		handler = handle_edge_irq;
	else
		handler = handle_level_irq;


	irq_set_lockdep_class(virq, &qpnpint_irq_lock_class,
			      &qpnpint_irq_request_class);
	irq_domain_set_info(domain, virq, hwirq, &pmic_arb_irqchip, pmic_arb,
			    handler, NULL, NULL);
}

static int qpnpint_irq_domain_alloc(struct irq_domain *domain,
				    unsigned int virq, unsigned int nr_irqs,
				    void *data)
{
	struct spmi_pmic_arb *pmic_arb = domain->host_data;
	struct irq_fwspec *fwspec = data;
	irq_hw_number_t hwirq;
	unsigned int type;
	int ret, i;

	ret = qpnpint_irq_domain_translate(domain, fwspec, &hwirq, &type);
	if (ret)
		return ret;

	for (i = 0; i < nr_irqs; i++)
		qpnpint_irq_domain_map(pmic_arb, domain, virq + i, hwirq + i,
				       type);

	return 0;
}

static int pmic_arb_ppid_to_apid_v1(struct spmi_pmic_arb *pmic_arb, u16 ppid)
{
	u32 *mapping_table = pmic_arb->mapping_table;
	int index = 0, i;
	u16 apid_valid;
	u16 apid;
	u32 data;

	apid_valid = pmic_arb->ppid_to_apid[ppid];
	if (apid_valid & PMIC_ARB_APID_VALID) {
		apid = apid_valid & ~PMIC_ARB_APID_VALID;
		return apid;
	}

	for (i = 0; i < SPMI_MAPPING_TABLE_TREE_DEPTH; ++i) {
		if (!test_and_set_bit(index, pmic_arb->mapping_table_valid))
			mapping_table[index] = readl_relaxed(pmic_arb->cnfg +
						SPMI_MAPPING_TABLE_REG(index));

		data = mapping_table[index];

		if (ppid & BIT(SPMI_MAPPING_BIT_INDEX(data))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (SPMI_MAPPING_BIT_IS_1_FLAG(data)) {
				index = SPMI_MAPPING_BIT_IS_1_RESULT(data);
			} else {
				apid = SPMI_MAPPING_BIT_IS_1_RESULT(data);
<<<<<<< HEAD
				break;
=======
				pmic_arb->ppid_to_apid[ppid]
					= apid | PMIC_ARB_APID_VALID;
				pmic_arb->apid_data[apid].ppid = ppid;
				return apid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		} else {
			if (SPMI_MAPPING_BIT_IS_0_FLAG(data)) {
				index = SPMI_MAPPING_BIT_IS_0_RESULT(data);
			} else {
				apid = SPMI_MAPPING_BIT_IS_0_RESULT(data);
<<<<<<< HEAD
				break;
=======
				pmic_arb->ppid_to_apid[ppid]
					= apid | PMIC_ARB_APID_VALID;
				pmic_arb->apid_data[apid].ppid = ppid;
				return apid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

<<<<<<< HEAD
	return apid;
}

/* PPID to APID */
static uint32_t map_peripheral_id(struct spmi_pmic_arb_dev *pmic_arb, u16 ppid)
{
	u32 apid = search_mapping_table(pmic_arb, ppid);
	u32 old_ppid;
	u32 owner;

	/* If the apid was found, add it to the lookup table */
	if (apid < PMIC_ARB_MAX_PERIPHS) {
		old_ppid = get_peripheral_id(pmic_arb, apid);

		owner = SPMI_OWNERSHIP_PERIPH2OWNER(
				readl_relaxed(pmic_arb->cnfg +
					SPMI_OWNERSHIP_TABLE_REG(apid)));

		/* Check ownership */
		if (owner != pmic_arb->owner) {
			dev_err(pmic_arb->dev, "PPID 0x%x incorrect owner %d\n",
				ppid, owner);
			return PMIC_ARB_MAX_PERIPHS;
		}

		/* Check if already mapped */
		if (pmic_arb->periph_id_map[apid] & PMIC_ARB_PERIPH_ID_VALID) {
			if (ppid != old_ppid) {
				dev_err(pmic_arb->dev,
					"PPID 0x%x: APID 0x%x already mapped\n",
					ppid, apid);
				return PMIC_ARB_MAX_PERIPHS;
			}
			return apid;
		}

		pmic_arb->periph_id_map[apid] = ppid | PMIC_ARB_PERIPH_ID_VALID;

		if (apid > pmic_arb->max_apid)
			pmic_arb->max_apid = apid;

		if (apid < pmic_arb->min_apid)
			pmic_arb->min_apid = apid;

		return apid;
	}

	dev_err(pmic_arb->dev, "Unknown ppid 0x%x\n", ppid);
	return PMIC_ARB_MAX_PERIPHS;
}

/* Enable interrupt at the PMIC Arbiter PIC */
static int pmic_arb_pic_enable(struct spmi_controller *ctrl,
				struct qpnp_irq_spec *spec, uint32_t data)
{
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	u8 apid = data & PMIC_ARB_APID_MASK;
	unsigned long flags;
	u32 status;

	dev_dbg(pmic_arb->dev, "PIC enable, apid:0x%x, sid:0x%x, pid:0x%x\n",
				apid, spec->slave, spec->per);

	if (data < pmic_arb->min_apid || data > pmic_arb->max_apid) {
		dev_err(pmic_arb->dev, "int enable: invalid APID %d\n", data);
		return -EINVAL;
	}

	if (!is_apid_valid(pmic_arb, apid)) {
		dev_err(pmic_arb->dev, "int enable: int not supported\n");
		return -EINVAL;
	}

	spin_lock_irqsave(&pmic_arb->lock, flags);
	status = readl_relaxed(pmic_arb->intr + SPMI_PIC_ACC_ENABLE(apid));
	if (!(status & SPMI_PIC_ACC_ENABLE_BIT)) {
		status = status | SPMI_PIC_ACC_ENABLE_BIT;
		writel_relaxed(status,
				pmic_arb->intr + SPMI_PIC_ACC_ENABLE(apid));
		/* Interrupt needs to be enabled before returning to caller */
		wmb();
	}
	spin_unlock_irqrestore(&pmic_arb->lock, flags);
	return 0;
}

/* Disable interrupt at the PMIC Arbiter PIC */
static int pmic_arb_pic_disable(struct spmi_controller *ctrl,
				struct qpnp_irq_spec *spec, uint32_t data)
{
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	u8 apid = data & PMIC_ARB_APID_MASK;
	unsigned long flags;
	u32 status;

	dev_dbg(pmic_arb->dev, "PIC disable, apid:0x%x, sid:0x%x, pid:0x%x\n",
				apid, spec->slave, spec->per);

	if (data < pmic_arb->min_apid || data > pmic_arb->max_apid) {
		dev_err(pmic_arb->dev, "int disable: invalid APID %d\n", data);
		return -EINVAL;
	}

	if (!is_apid_valid(pmic_arb, apid)) {
		dev_err(pmic_arb->dev, "int disable: int not supported\n");
		return -EINVAL;
	}

	spin_lock_irqsave(&pmic_arb->lock, flags);
	status = readl_relaxed(pmic_arb->intr + SPMI_PIC_ACC_ENABLE(apid));
	if (status & SPMI_PIC_ACC_ENABLE_BIT) {
		/* clear the enable bit and write */
		status = status & ~SPMI_PIC_ACC_ENABLE_BIT;
		writel_relaxed(status,
				pmic_arb->intr + SPMI_PIC_ACC_ENABLE(apid));
		/* Interrupt needs to be disabled before returning to caller */
		wmb();
	}
	spin_unlock_irqrestore(&pmic_arb->lock, flags);
	return 0;
}

static irqreturn_t
periph_interrupt(struct spmi_pmic_arb_dev *pmic_arb, u8 apid, bool show)
{
	u16 ppid = get_peripheral_id(pmic_arb, apid);
	void __iomem *intr = pmic_arb->intr;
	u8 sid = (ppid >> 8) & 0x0F;
	u8 pid = ppid & 0xFF;
	u32 status;
	int i;

	if (!is_apid_valid(pmic_arb, apid)) {
		dev_err(pmic_arb->dev,
		"periph_interrupt(apid:0x%x sid:0x%x pid:0x%x) unknown peripheral\n",
			apid, sid, pid);
		/* return IRQ_NONE; */
	}

	status = readl_relaxed(intr + SPMI_PIC_ACC_ENABLE(apid));
	if (!(status & SPMI_PIC_ACC_ENABLE_BIT)) {
		/*
		 * All interrupts from this peripheral are disabled
		 * don't bother calling the qpnpint handler
		 */
		return IRQ_HANDLED;
	}

	/* Read the peripheral specific interrupt bits */
	status = readl_relaxed(intr + SPMI_PIC_IRQ_STATUS(apid));

	if (!show) {
		/* Clear the peripheral interrupts */
		writel_relaxed(status, intr + SPMI_PIC_IRQ_CLEAR(apid));
		/* Irq needs to be cleared/acknowledged before exiting ISR */
		mb();
	}

	dev_dbg(pmic_arb->dev,
		"interrupt, apid:0x%x, sid:0x%x, pid:0x%x, intr:0x%x\n",
						apid, sid, pid, status);

	/* Send interrupt notification */
	for (i = 0; status && i < 8; ++i, status >>= 1) {
		if (status & 0x1) {
			struct qpnp_irq_spec irq_spec = {
				.slave = sid,
				.per = pid,
				.irq = i,
			};
			if (show)
				qpnpint_show_irq(&pmic_arb->controller,
								&irq_spec);
			else
				qpnpint_handle_irq(&pmic_arb->controller,
								&irq_spec);
		}
	}
	return IRQ_HANDLED;
}

/* Peripheral interrupt handler */
static irqreturn_t
__pmic_arb_periph_irq(int irq, void *dev_id, bool show)
{
	struct spmi_pmic_arb_dev *pmic_arb = dev_id;
	void __iomem *intr = pmic_arb->intr;
	u8 ee = pmic_arb->owner;
	u32 ret = IRQ_NONE;
	u32 status;

	int first = pmic_arb->min_apid >> 5;
	int last = pmic_arb->max_apid >> 5;
	int i, j;

	dev_dbg(pmic_arb->dev, "Peripheral interrupt detected\n");

	/* Check the accumulated interrupt status */
	for (i = first; i <= last; ++i) {
		status = readl_relaxed(intr + SPMI_PIC_OWNER_ACC_STATUS(ee, i));

		for (j = 0; status && j < 32; ++j, status >>= 1) {
			if (status & 0x1) {
				u8 id = (i * 32) + j;

				ret |= periph_interrupt(pmic_arb, id, show);
			}
		}
	}

	return ret;
}

static irqreturn_t pmic_arb_periph_irq(int irq, void *dev_id)
{
	return __pmic_arb_periph_irq(irq, dev_id, false);
}

static void spmi_pmic_arb_resume(void)
{
	if (qpnpint_show_resume_irq())
		__pmic_arb_periph_irq(the_pmic_arb->pic_irq,
						the_pmic_arb, true);
}

static struct syscore_ops spmi_pmic_arb_syscore_ops = {
	.resume = spmi_pmic_arb_resume,
};

/* Callback to register an APID for specific slave/peripheral */
static int pmic_arb_intr_priv_data(struct spmi_controller *ctrl,
				struct qpnp_irq_spec *spec, uint32_t *data)
{
	struct spmi_pmic_arb_dev *pmic_arb = spmi_get_ctrldata(ctrl);
	u16 ppid = ((spec->slave & 0x0F) << 8) | (spec->per & 0xFF);
	*data = map_peripheral_id(pmic_arb, ppid);
	return 0;
}

static int pmic_arb_mapping_data_show(struct seq_file *file, void *unused)
{
	struct spmi_pmic_arb_dev *pmic_arb = file->private;
	int first = pmic_arb->min_apid;
	int last = pmic_arb->max_apid;
	int i;

	for (i = first; i <= last; ++i) {
		if (!is_apid_valid(pmic_arb, i))
			continue;

		seq_printf(file, "APID 0x%.2x = PPID 0x%.3x. Enabled:%d\n",
			i, get_peripheral_id(pmic_arb, i),
			readl_relaxed(pmic_arb->intr + SPMI_PIC_ACC_ENABLE(i)));
	}

	return 0;
}

static int pmic_arb_mapping_data_open(struct inode *inode, struct file *file)
{
	return single_open(file, pmic_arb_mapping_data_show, inode->i_private);
}

static const struct file_operations pmic_arb_dfs_fops = {
	.open		= pmic_arb_mapping_data_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __devinit
spmi_pmic_arb_get_property(struct platform_device *pdev, char *pname, u32 *prop)
{
	int ret = of_property_read_u32(pdev->dev.of_node, pname, prop);

	if (ret)
		dev_err(&pdev->dev, "missing property: %s\n", pname);
	else
		pr_debug("%s = 0x%x\n", pname, *prop);

	return ret;
}

static struct qpnp_local_int spmi_pmic_arb_intr_cb = {
	.mask = pmic_arb_pic_disable,
	.unmask = pmic_arb_pic_enable,
	.register_priv_data = pmic_arb_intr_priv_data,
};

static int __devinit spmi_pmic_arb_probe(struct platform_device *pdev)
{
	struct spmi_pmic_arb_dev *pmic_arb;
	struct resource *mem_res;
	u32 cell_index;
	u32 prop;
	int ret = 0;
	int i;

	pr_debug("SPMI PMIC Arbiter\n");

	pmic_arb = devm_kzalloc(&pdev->dev,
				sizeof(struct spmi_pmic_arb_dev), GFP_KERNEL);
	if (!pmic_arb) {
		dev_err(&pdev->dev, "can not allocate pmic_arb data\n");
		return -ENOMEM;
	}

	mem_res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "core");
	if (!mem_res) {
		dev_err(&pdev->dev, "missing base memory resource\n");
		return -ENODEV;
	}

	pmic_arb->base = devm_ioremap(&pdev->dev,
					mem_res->start, resource_size(mem_res));
	if (!pmic_arb->base) {
		dev_err(&pdev->dev, "ioremap of 'base' failed\n");
		return -ENOMEM;
	}

	mem_res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "intr");
	if (!mem_res) {
		dev_err(&pdev->dev, "missing mem resource (interrupts)\n");
		return -ENODEV;
	}

	pmic_arb->intr = devm_ioremap(&pdev->dev,
					mem_res->start, resource_size(mem_res));
	if (!pmic_arb->intr) {
		dev_err(&pdev->dev, "ioremap of 'intr' failed\n");
		return -ENOMEM;
	}

	mem_res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "cnfg");
	if (!mem_res) {
		dev_err(&pdev->dev, "missing mem resource (configuration)\n");
		return -ENODEV;
	}

	pmic_arb->cnfg = devm_ioremap(&pdev->dev,
					mem_res->start, resource_size(mem_res));
	if (!pmic_arb->cnfg) {
		dev_err(&pdev->dev, "ioremap of 'cnfg' failed\n");
		return -ENOMEM;
	}

	for (i = 0; i < ARRAY_SIZE(pmic_arb->mapping_table); ++i)
		pmic_arb->mapping_table[i] = readl_relaxed(
				pmic_arb->cnfg + SPMI_MAPPING_TABLE_REG(i));

	pmic_arb->pic_irq = platform_get_irq(pdev, 0);
	if (!pmic_arb->pic_irq) {
		dev_err(&pdev->dev, "missing IRQ resource\n");
		return -ENODEV;
	}

	ret = devm_request_irq(&pdev->dev, pmic_arb->pic_irq,
		pmic_arb_periph_irq, IRQF_TRIGGER_HIGH, pdev->name, pmic_arb);
	if (ret) {
		dev_err(&pdev->dev, "request IRQ failed\n");
		return ret;
	}

	/* Get properties from the device tree */
	ret = spmi_pmic_arb_get_property(pdev, "cell-index", &cell_index);
	if (ret)
		return -ENODEV;

	ret = spmi_pmic_arb_get_property(pdev, "qcom,pmic-arb-ee", &prop);
	if (ret)
		return -ENODEV;
	pmic_arb->owner = (u8)prop;

	ret = spmi_pmic_arb_get_property(pdev, "qcom,pmic-arb-channel", &prop);
	if (ret)
		return -ENODEV;
	pmic_arb->channel = (u8)prop;

	pmic_arb->allow_wakeup = !of_property_read_bool(pdev->dev.of_node,
					"qcom,not-wakeup");
	if (pmic_arb->allow_wakeup) {
		ret = irq_set_irq_wake(pmic_arb->pic_irq, 1);
		if (unlikely(ret)) {
			pr_err("Unable to set wakeup irq, err=%d\n", ret);
			return -ENODEV;
		}
	}

	pmic_arb->max_apid = 0;
	pmic_arb->min_apid = PMIC_ARB_MAX_PERIPHS - 1;

	pmic_arb->dev = &pdev->dev;
	platform_set_drvdata(pdev, pmic_arb);
	spmi_set_ctrldata(&pmic_arb->controller, pmic_arb);

	spin_lock_init(&pmic_arb->lock);

	pmic_arb->controller.nr = cell_index;
	pmic_arb->controller.dev.parent = pdev->dev.parent;
	pmic_arb->controller.dev.of_node = of_node_get(pdev->dev.of_node);

	/* Callbacks */
	pmic_arb->controller.cmd = pmic_arb_cmd;
	pmic_arb->controller.read_cmd = pmic_arb_read_cmd;
	pmic_arb->controller.write_cmd =  pmic_arb_write_cmd;

	ret = spmi_add_controller(&pmic_arb->controller);
	if (ret)
		goto err_add_controller;

	/* Register the interrupt enable/disable functions */
	ret = qpnpint_register_controller(pmic_arb->controller.dev.of_node,
					  &pmic_arb->controller,
					  &spmi_pmic_arb_intr_cb);
	if (ret) {
		dev_err(&pdev->dev, "Unable to register controller %d\n",
					cell_index);
		goto err_reg_controller;
	}

	/* Register device(s) from the device tree */
	of_spmi_register_devices(&pmic_arb->controller);

	/* Add debugfs file for mapping data */
	if (spmi_dfs_create_file(&pmic_arb->controller, "mapping",
					pmic_arb, &pmic_arb_dfs_fops) == NULL)
		dev_err(&pdev->dev, "error creating 'mapping' debugfs file\n");

	pr_debug("PMIC Arb Version 0x%x\n",
			pmic_arb_read(pmic_arb, PMIC_ARB_VERSION));

	the_pmic_arb = pmic_arb;
	register_syscore_ops(&spmi_pmic_arb_syscore_ops);

	return 0;

err_reg_controller:
	spmi_del_controller(&pmic_arb->controller);
err_add_controller:
	platform_set_drvdata(pdev, NULL);
	if (pmic_arb->allow_wakeup)
		irq_set_irq_wake(pmic_arb->pic_irq, 0);
	return ret;
}

static int __devexit spmi_pmic_arb_remove(struct platform_device *pdev)
{
	struct spmi_pmic_arb_dev *pmic_arb = platform_get_drvdata(pdev);
	int ret;

	ret = qpnpint_unregister_controller(pmic_arb->controller.dev.of_node);
	if (ret)
		dev_err(&pdev->dev, "Unable to unregister controller %d\n",
					pmic_arb->controller.nr);

	if (pmic_arb->allow_wakeup)
		irq_set_irq_wake(pmic_arb->pic_irq, 0);
	platform_set_drvdata(pdev, NULL);
	spmi_del_controller(&pmic_arb->controller);
	return ret;
}

static struct of_device_id spmi_pmic_arb_match_table[] = {
	{	.compatible = "qcom,spmi-pmic-arb",
	},
	{}
};

static struct platform_driver spmi_pmic_arb_driver = {
	.probe		= spmi_pmic_arb_probe,
	.remove		= __exit_p(spmi_pmic_arb_remove),
	.driver		= {
		.name	= SPMI_PMIC_ARB_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = spmi_pmic_arb_match_table,
	},
};

static int __init spmi_pmic_arb_init(void)
{
	return platform_driver_register(&spmi_pmic_arb_driver);
}
postcore_initcall(spmi_pmic_arb_init);

static void __exit spmi_pmic_arb_exit(void)
{
	platform_driver_unregister(&spmi_pmic_arb_driver);
}
module_exit(spmi_pmic_arb_exit);

MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
=======
	return -ENODEV;
}

/* v1 offset per ee */
static int pmic_arb_offset_v1(struct spmi_pmic_arb *pmic_arb, u8 sid, u16 addr,
			enum pmic_arb_channel ch_type)
{
	return 0x800 + 0x80 * pmic_arb->channel;
}

static u16 pmic_arb_find_apid(struct spmi_pmic_arb *pmic_arb, u16 ppid)
{
	struct apid_data *apidd = &pmic_arb->apid_data[pmic_arb->last_apid];
	u32 regval, offset;
	u16 id, apid;

	for (apid = pmic_arb->last_apid; ; apid++, apidd++) {
		offset = pmic_arb->ver_ops->apid_map_offset(apid);
		if (offset >= pmic_arb->core_size)
			break;

		regval = readl_relaxed(pmic_arb->ver_ops->apid_owner(pmic_arb,
								     apid));
		apidd->irq_ee = SPMI_OWNERSHIP_PERIPH2OWNER(regval);
		apidd->write_ee = apidd->irq_ee;

		regval = readl_relaxed(pmic_arb->core + offset);
		if (!regval)
			continue;

		id = (regval >> 8) & PMIC_ARB_PPID_MASK;
		pmic_arb->ppid_to_apid[id] = apid | PMIC_ARB_APID_VALID;
		apidd->ppid = id;
		if (id == ppid) {
			apid |= PMIC_ARB_APID_VALID;
			break;
		}
	}
	pmic_arb->last_apid = apid & ~PMIC_ARB_APID_VALID;

	return apid;
}

static int pmic_arb_ppid_to_apid_v2(struct spmi_pmic_arb *pmic_arb, u16 ppid)
{
	u16 apid_valid;

	apid_valid = pmic_arb->ppid_to_apid[ppid];
	if (!(apid_valid & PMIC_ARB_APID_VALID))
		apid_valid = pmic_arb_find_apid(pmic_arb, ppid);
	if (!(apid_valid & PMIC_ARB_APID_VALID))
		return -ENODEV;

	return apid_valid & ~PMIC_ARB_APID_VALID;
}

static int pmic_arb_read_apid_map_v5(struct spmi_pmic_arb *pmic_arb)
{
	struct apid_data *apidd;
	struct apid_data *prev_apidd;
	u16 i, apid, ppid, apid_max;
	bool valid, is_irq_ee;
	u32 regval, offset;

	/*
	 * In order to allow multiple EEs to write to a single PPID in arbiter
	 * version 5 and 7, there is more than one APID mapped to each PPID.
	 * The owner field for each of these mappings specifies the EE which is
	 * allowed to write to the APID.  The owner of the last (highest) APID
	 * which has the IRQ owner bit set for a given PPID will receive
	 * interrupts from the PPID.
	 *
	 * In arbiter version 7, the APID numbering space is divided between
	 * the primary bus (0) and secondary bus (1) such that:
	 * APID = 0 to N-1 are assigned to the primary bus
	 * APID = N to N+M-1 are assigned to the secondary bus
	 * where N = number of APIDs supported by the primary bus and
	 *       M = number of APIDs supported by the secondary bus
	 */
	apidd = &pmic_arb->apid_data[pmic_arb->base_apid];
	apid_max = pmic_arb->base_apid + pmic_arb->apid_count;
	for (i = pmic_arb->base_apid; i < apid_max; i++, apidd++) {
		offset = pmic_arb->ver_ops->apid_map_offset(i);
		if (offset >= pmic_arb->core_size)
			break;

		regval = readl_relaxed(pmic_arb->core + offset);
		if (!regval)
			continue;
		ppid = (regval >> 8) & PMIC_ARB_PPID_MASK;
		is_irq_ee = PMIC_ARB_CHAN_IS_IRQ_OWNER(regval);

		regval = readl_relaxed(pmic_arb->ver_ops->apid_owner(pmic_arb,
								     i));
		apidd->write_ee = SPMI_OWNERSHIP_PERIPH2OWNER(regval);

		apidd->irq_ee = is_irq_ee ? apidd->write_ee : INVALID_EE;

		valid = pmic_arb->ppid_to_apid[ppid] & PMIC_ARB_APID_VALID;
		apid = pmic_arb->ppid_to_apid[ppid] & ~PMIC_ARB_APID_VALID;
		prev_apidd = &pmic_arb->apid_data[apid];

		if (!valid || apidd->write_ee == pmic_arb->ee) {
			/* First PPID mapping or one for this EE */
			pmic_arb->ppid_to_apid[ppid] = i | PMIC_ARB_APID_VALID;
		} else if (valid && is_irq_ee &&
			   prev_apidd->write_ee == pmic_arb->ee) {
			/*
			 * Duplicate PPID mapping after the one for this EE;
			 * override the irq owner
			 */
			prev_apidd->irq_ee = apidd->irq_ee;
		}

		apidd->ppid = ppid;
		pmic_arb->last_apid = i;
	}

	/* Dump the mapping table for debug purposes. */
	dev_dbg(&pmic_arb->spmic->dev, "PPID APID Write-EE IRQ-EE\n");
	for (ppid = 0; ppid < PMIC_ARB_MAX_PPID; ppid++) {
		apid = pmic_arb->ppid_to_apid[ppid];
		if (apid & PMIC_ARB_APID_VALID) {
			apid &= ~PMIC_ARB_APID_VALID;
			apidd = &pmic_arb->apid_data[apid];
			dev_dbg(&pmic_arb->spmic->dev, "%#03X %3u %2u %2u\n",
			      ppid, apid, apidd->write_ee, apidd->irq_ee);
		}
	}

	return 0;
}

static int pmic_arb_ppid_to_apid_v5(struct spmi_pmic_arb *pmic_arb, u16 ppid)
{
	if (!(pmic_arb->ppid_to_apid[ppid] & PMIC_ARB_APID_VALID))
		return -ENODEV;

	return pmic_arb->ppid_to_apid[ppid] & ~PMIC_ARB_APID_VALID;
}

/* v2 offset per ppid and per ee */
static int pmic_arb_offset_v2(struct spmi_pmic_arb *pmic_arb, u8 sid, u16 addr,
			   enum pmic_arb_channel ch_type)
{
	u16 apid;
	u16 ppid;
	int rc;

	ppid = sid << 8 | ((addr >> 8) & 0xFF);
	rc = pmic_arb_ppid_to_apid_v2(pmic_arb, ppid);
	if (rc < 0)
		return rc;

	apid = rc;
	return 0x1000 * pmic_arb->ee + 0x8000 * apid;
}

/*
 * v5 offset per ee and per apid for observer channels and per apid for
 * read/write channels.
 */
static int pmic_arb_offset_v5(struct spmi_pmic_arb *pmic_arb, u8 sid, u16 addr,
			   enum pmic_arb_channel ch_type)
{
	u16 apid;
	int rc;
	u32 offset = 0;
	u16 ppid = (sid << 8) | (addr >> 8);

	rc = pmic_arb_ppid_to_apid_v5(pmic_arb, ppid);
	if (rc < 0)
		return rc;

	apid = rc;
	switch (ch_type) {
	case PMIC_ARB_CHANNEL_OBS:
		offset = 0x10000 * pmic_arb->ee + 0x80 * apid;
		break;
	case PMIC_ARB_CHANNEL_RW:
		if (pmic_arb->apid_data[apid].write_ee != pmic_arb->ee) {
			dev_err(&pmic_arb->spmic->dev, "disallowed SPMI write to sid=%u, addr=0x%04X\n",
				sid, addr);
			return -EPERM;
		}
		offset = 0x10000 * apid;
		break;
	}

	return offset;
}

/*
 * v7 offset per ee and per apid for observer channels and per apid for
 * read/write channels.
 */
static int pmic_arb_offset_v7(struct spmi_pmic_arb *pmic_arb, u8 sid, u16 addr,
			   enum pmic_arb_channel ch_type)
{
	u16 apid;
	int rc;
	u32 offset = 0;
	u16 ppid = (sid << 8) | (addr >> 8);

	rc = pmic_arb->ver_ops->ppid_to_apid(pmic_arb, ppid);
	if (rc < 0)
		return rc;

	apid = rc;
	switch (ch_type) {
	case PMIC_ARB_CHANNEL_OBS:
		offset = 0x8000 * pmic_arb->ee + 0x20 * apid;
		break;
	case PMIC_ARB_CHANNEL_RW:
		if (pmic_arb->apid_data[apid].write_ee != pmic_arb->ee) {
			dev_err(&pmic_arb->spmic->dev, "disallowed SPMI write to sid=%u, addr=0x%04X\n",
				sid, addr);
			return -EPERM;
		}
		offset = 0x1000 * apid;
		break;
	}

	return offset;
}

static u32 pmic_arb_fmt_cmd_v1(u8 opc, u8 sid, u16 addr, u8 bc)
{
	return (opc << 27) | ((sid & 0xf) << 20) | (addr << 4) | (bc & 0x7);
}

static u32 pmic_arb_fmt_cmd_v2(u8 opc, u8 sid, u16 addr, u8 bc)
{
	return (opc << 27) | ((addr & 0xff) << 4) | (bc & 0x7);
}

static void __iomem *
pmic_arb_owner_acc_status_v1(struct spmi_pmic_arb *pmic_arb, u8 m, u16 n)
{
	return pmic_arb->intr + 0x20 * m + 0x4 * n;
}

static void __iomem *
pmic_arb_owner_acc_status_v2(struct spmi_pmic_arb *pmic_arb, u8 m, u16 n)
{
	return pmic_arb->intr + 0x100000 + 0x1000 * m + 0x4 * n;
}

static void __iomem *
pmic_arb_owner_acc_status_v3(struct spmi_pmic_arb *pmic_arb, u8 m, u16 n)
{
	return pmic_arb->intr + 0x200000 + 0x1000 * m + 0x4 * n;
}

static void __iomem *
pmic_arb_owner_acc_status_v5(struct spmi_pmic_arb *pmic_arb, u8 m, u16 n)
{
	return pmic_arb->intr + 0x10000 * m + 0x4 * n;
}

static void __iomem *
pmic_arb_owner_acc_status_v7(struct spmi_pmic_arb *pmic_arb, u8 m, u16 n)
{
	return pmic_arb->intr + 0x1000 * m + 0x4 * n;
}

static void __iomem *
pmic_arb_acc_enable_v1(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0x200 + 0x4 * n;
}

static void __iomem *
pmic_arb_acc_enable_v2(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0x1000 * n;
}

static void __iomem *
pmic_arb_acc_enable_v5(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x100 + 0x10000 * n;
}

static void __iomem *
pmic_arb_acc_enable_v7(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x100 + 0x1000 * n;
}

static void __iomem *
pmic_arb_irq_status_v1(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0x600 + 0x4 * n;
}

static void __iomem *
pmic_arb_irq_status_v2(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0x4 + 0x1000 * n;
}

static void __iomem *
pmic_arb_irq_status_v5(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x104 + 0x10000 * n;
}

static void __iomem *
pmic_arb_irq_status_v7(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x104 + 0x1000 * n;
}

static void __iomem *
pmic_arb_irq_clear_v1(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0xA00 + 0x4 * n;
}

static void __iomem *
pmic_arb_irq_clear_v2(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->intr + 0x8 + 0x1000 * n;
}

static void __iomem *
pmic_arb_irq_clear_v5(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x108 + 0x10000 * n;
}

static void __iomem *
pmic_arb_irq_clear_v7(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->wr_base + 0x108 + 0x1000 * n;
}

static u32 pmic_arb_apid_map_offset_v2(u16 n)
{
	return 0x800 + 0x4 * n;
}

static u32 pmic_arb_apid_map_offset_v5(u16 n)
{
	return 0x900 + 0x4 * n;
}

static u32 pmic_arb_apid_map_offset_v7(u16 n)
{
	return 0x2000 + 0x4 * n;
}

static void __iomem *
pmic_arb_apid_owner_v2(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->cnfg + 0x700 + 0x4 * n;
}

/*
 * For arbiter version 7, APID ownership table registers have independent
 * numbering space for each SPMI bus instance, so each is indexed starting from
 * 0.
 */
static void __iomem *
pmic_arb_apid_owner_v7(struct spmi_pmic_arb *pmic_arb, u16 n)
{
	return pmic_arb->cnfg + 0x4 * (n - pmic_arb->base_apid);
}

static const struct pmic_arb_ver_ops pmic_arb_v1 = {
	.ver_str		= "v1",
	.ppid_to_apid		= pmic_arb_ppid_to_apid_v1,
	.non_data_cmd		= pmic_arb_non_data_cmd_v1,
	.offset			= pmic_arb_offset_v1,
	.fmt_cmd		= pmic_arb_fmt_cmd_v1,
	.owner_acc_status	= pmic_arb_owner_acc_status_v1,
	.acc_enable		= pmic_arb_acc_enable_v1,
	.irq_status		= pmic_arb_irq_status_v1,
	.irq_clear		= pmic_arb_irq_clear_v1,
	.apid_map_offset	= pmic_arb_apid_map_offset_v2,
	.apid_owner		= pmic_arb_apid_owner_v2,
};

static const struct pmic_arb_ver_ops pmic_arb_v2 = {
	.ver_str		= "v2",
	.ppid_to_apid		= pmic_arb_ppid_to_apid_v2,
	.non_data_cmd		= pmic_arb_non_data_cmd_v2,
	.offset			= pmic_arb_offset_v2,
	.fmt_cmd		= pmic_arb_fmt_cmd_v2,
	.owner_acc_status	= pmic_arb_owner_acc_status_v2,
	.acc_enable		= pmic_arb_acc_enable_v2,
	.irq_status		= pmic_arb_irq_status_v2,
	.irq_clear		= pmic_arb_irq_clear_v2,
	.apid_map_offset	= pmic_arb_apid_map_offset_v2,
	.apid_owner		= pmic_arb_apid_owner_v2,
};

static const struct pmic_arb_ver_ops pmic_arb_v3 = {
	.ver_str		= "v3",
	.ppid_to_apid		= pmic_arb_ppid_to_apid_v2,
	.non_data_cmd		= pmic_arb_non_data_cmd_v2,
	.offset			= pmic_arb_offset_v2,
	.fmt_cmd		= pmic_arb_fmt_cmd_v2,
	.owner_acc_status	= pmic_arb_owner_acc_status_v3,
	.acc_enable		= pmic_arb_acc_enable_v2,
	.irq_status		= pmic_arb_irq_status_v2,
	.irq_clear		= pmic_arb_irq_clear_v2,
	.apid_map_offset	= pmic_arb_apid_map_offset_v2,
	.apid_owner		= pmic_arb_apid_owner_v2,
};

static const struct pmic_arb_ver_ops pmic_arb_v5 = {
	.ver_str		= "v5",
	.ppid_to_apid		= pmic_arb_ppid_to_apid_v5,
	.non_data_cmd		= pmic_arb_non_data_cmd_v2,
	.offset			= pmic_arb_offset_v5,
	.fmt_cmd		= pmic_arb_fmt_cmd_v2,
	.owner_acc_status	= pmic_arb_owner_acc_status_v5,
	.acc_enable		= pmic_arb_acc_enable_v5,
	.irq_status		= pmic_arb_irq_status_v5,
	.irq_clear		= pmic_arb_irq_clear_v5,
	.apid_map_offset	= pmic_arb_apid_map_offset_v5,
	.apid_owner		= pmic_arb_apid_owner_v2,
};

static const struct pmic_arb_ver_ops pmic_arb_v7 = {
	.ver_str		= "v7",
	.ppid_to_apid		= pmic_arb_ppid_to_apid_v5,
	.non_data_cmd		= pmic_arb_non_data_cmd_v2,
	.offset			= pmic_arb_offset_v7,
	.fmt_cmd		= pmic_arb_fmt_cmd_v2,
	.owner_acc_status	= pmic_arb_owner_acc_status_v7,
	.acc_enable		= pmic_arb_acc_enable_v7,
	.irq_status		= pmic_arb_irq_status_v7,
	.irq_clear		= pmic_arb_irq_clear_v7,
	.apid_map_offset	= pmic_arb_apid_map_offset_v7,
	.apid_owner		= pmic_arb_apid_owner_v7,
};

static const struct irq_domain_ops pmic_arb_irq_domain_ops = {
	.activate = qpnpint_irq_domain_activate,
	.alloc = qpnpint_irq_domain_alloc,
	.free = irq_domain_free_irqs_common,
	.translate = qpnpint_irq_domain_translate,
};

static int spmi_pmic_arb_probe(struct platform_device *pdev)
{
	struct spmi_pmic_arb *pmic_arb;
	struct spmi_controller *ctrl;
	struct resource *res;
	void __iomem *core;
	u32 *mapping_table;
	u32 channel, ee, hw_ver;
	int err;

	ctrl = devm_spmi_controller_alloc(&pdev->dev, sizeof(*pmic_arb));
	if (IS_ERR(ctrl))
		return PTR_ERR(ctrl);

	pmic_arb = spmi_controller_get_drvdata(ctrl);
	pmic_arb->spmic = ctrl;

	/*
	 * Please don't replace this with devm_platform_ioremap_resource() or
	 * devm_ioremap_resource().  These both result in a call to
	 * devm_request_mem_region() which prevents multiple mappings of this
	 * register address range.  SoCs with PMIC arbiter v7 may define two
	 * arbiter devices, for the two physical SPMI interfaces, which  share
	 * some register address ranges (i.e. "core", "obsrvr", and "chnls").
	 * Ensure that both devices probe successfully by calling devm_ioremap()
	 * which does not result in a devm_request_mem_region() call.
	 */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "core");
	core = devm_ioremap(&ctrl->dev, res->start, resource_size(res));
	if (IS_ERR(core))
		return PTR_ERR(core);

	pmic_arb->core_size = resource_size(res);

	pmic_arb->ppid_to_apid = devm_kcalloc(&ctrl->dev, PMIC_ARB_MAX_PPID,
					      sizeof(*pmic_arb->ppid_to_apid),
					      GFP_KERNEL);
	if (!pmic_arb->ppid_to_apid)
		return -ENOMEM;

	hw_ver = readl_relaxed(core + PMIC_ARB_VERSION);

	if (hw_ver < PMIC_ARB_VERSION_V2_MIN) {
		pmic_arb->ver_ops = &pmic_arb_v1;
		pmic_arb->wr_base = core;
		pmic_arb->rd_base = core;
	} else {
		pmic_arb->core = core;

		if (hw_ver < PMIC_ARB_VERSION_V3_MIN)
			pmic_arb->ver_ops = &pmic_arb_v2;
		else if (hw_ver < PMIC_ARB_VERSION_V5_MIN)
			pmic_arb->ver_ops = &pmic_arb_v3;
		else if (hw_ver < PMIC_ARB_VERSION_V7_MIN)
			pmic_arb->ver_ops = &pmic_arb_v5;
		else
			pmic_arb->ver_ops = &pmic_arb_v7;

		res = platform_get_resource_byname(pdev, IORESOURCE_MEM,
						   "obsrvr");
		pmic_arb->rd_base = devm_ioremap(&ctrl->dev, res->start,
						 resource_size(res));
		if (IS_ERR(pmic_arb->rd_base))
			return PTR_ERR(pmic_arb->rd_base);

		res = platform_get_resource_byname(pdev, IORESOURCE_MEM,
						   "chnls");
		pmic_arb->wr_base = devm_ioremap(&ctrl->dev, res->start,
						 resource_size(res));
		if (IS_ERR(pmic_arb->wr_base))
			return PTR_ERR(pmic_arb->wr_base);
	}

	pmic_arb->max_periphs = PMIC_ARB_MAX_PERIPHS;

	if (hw_ver >= PMIC_ARB_VERSION_V7_MIN) {
		pmic_arb->max_periphs = PMIC_ARB_MAX_PERIPHS_V7;
		/* Optional property for v7: */
		of_property_read_u32(pdev->dev.of_node, "qcom,bus-id",
					&pmic_arb->bus_instance);
		if (pmic_arb->bus_instance > 1) {
			dev_err(&pdev->dev, "invalid bus instance (%u) specified\n",
				pmic_arb->bus_instance);
			return -EINVAL;
		}

		if (pmic_arb->bus_instance == 0) {
			pmic_arb->base_apid = 0;
			pmic_arb->apid_count =
				readl_relaxed(core + PMIC_ARB_FEATURES) &
				PMIC_ARB_FEATURES_PERIPH_MASK;
		} else {
			pmic_arb->base_apid =
				readl_relaxed(core + PMIC_ARB_FEATURES) &
				PMIC_ARB_FEATURES_PERIPH_MASK;
			pmic_arb->apid_count =
				readl_relaxed(core + PMIC_ARB_FEATURES1) &
				PMIC_ARB_FEATURES_PERIPH_MASK;
		}

		if (pmic_arb->base_apid + pmic_arb->apid_count > pmic_arb->max_periphs) {
			dev_err(&pdev->dev, "Unsupported APID count %d detected\n",
				pmic_arb->base_apid + pmic_arb->apid_count);
			return -EINVAL;
		}
	} else if (hw_ver >= PMIC_ARB_VERSION_V5_MIN) {
		pmic_arb->base_apid = 0;
		pmic_arb->apid_count = readl_relaxed(core + PMIC_ARB_FEATURES) &
					PMIC_ARB_FEATURES_PERIPH_MASK;

		if (pmic_arb->apid_count > pmic_arb->max_periphs) {
			dev_err(&pdev->dev, "Unsupported APID count %d detected\n",
				pmic_arb->apid_count);
			return -EINVAL;
		}
	}

	pmic_arb->apid_data = devm_kcalloc(&ctrl->dev, pmic_arb->max_periphs,
					   sizeof(*pmic_arb->apid_data),
					   GFP_KERNEL);
	if (!pmic_arb->apid_data)
		return -ENOMEM;

	dev_info(&ctrl->dev, "PMIC arbiter version %s (0x%x)\n",
		 pmic_arb->ver_ops->ver_str, hw_ver);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "intr");
	pmic_arb->intr = devm_ioremap_resource(&ctrl->dev, res);
	if (IS_ERR(pmic_arb->intr))
		return PTR_ERR(pmic_arb->intr);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "cnfg");
	pmic_arb->cnfg = devm_ioremap_resource(&ctrl->dev, res);
	if (IS_ERR(pmic_arb->cnfg))
		return PTR_ERR(pmic_arb->cnfg);

	pmic_arb->irq = platform_get_irq_byname(pdev, "periph_irq");
	if (pmic_arb->irq < 0)
		return pmic_arb->irq;

	err = of_property_read_u32(pdev->dev.of_node, "qcom,channel", &channel);
	if (err) {
		dev_err(&pdev->dev, "channel unspecified.\n");
		return err;
	}

	if (channel > 5) {
		dev_err(&pdev->dev, "invalid channel (%u) specified.\n",
			channel);
		return -EINVAL;
	}

	pmic_arb->channel = channel;

	err = of_property_read_u32(pdev->dev.of_node, "qcom,ee", &ee);
	if (err) {
		dev_err(&pdev->dev, "EE unspecified.\n");
		return err;
	}

	if (ee > 5) {
		dev_err(&pdev->dev, "invalid EE (%u) specified\n", ee);
		return -EINVAL;
	}

	pmic_arb->ee = ee;
	mapping_table = devm_kcalloc(&ctrl->dev, pmic_arb->max_periphs,
					sizeof(*mapping_table), GFP_KERNEL);
	if (!mapping_table)
		return -ENOMEM;

	pmic_arb->mapping_table = mapping_table;
	/* Initialize max_apid/min_apid to the opposite bounds, during
	 * the irq domain translation, we are sure to update these */
	pmic_arb->max_apid = 0;
	pmic_arb->min_apid = pmic_arb->max_periphs - 1;

	platform_set_drvdata(pdev, ctrl);
	raw_spin_lock_init(&pmic_arb->lock);

	ctrl->cmd = pmic_arb_cmd;
	ctrl->read_cmd = pmic_arb_read_cmd;
	ctrl->write_cmd = pmic_arb_write_cmd;

	if (hw_ver >= PMIC_ARB_VERSION_V5_MIN) {
		err = pmic_arb_read_apid_map_v5(pmic_arb);
		if (err) {
			dev_err(&pdev->dev, "could not read APID->PPID mapping table, rc= %d\n",
				err);
			return err;
		}
	}

	dev_dbg(&pdev->dev, "adding irq domain\n");
	pmic_arb->domain = irq_domain_add_tree(pdev->dev.of_node,
					 &pmic_arb_irq_domain_ops, pmic_arb);
	if (!pmic_arb->domain) {
		dev_err(&pdev->dev, "unable to create irq_domain\n");
		return -ENOMEM;
	}

	irq_set_chained_handler_and_data(pmic_arb->irq, pmic_arb_chained_irq,
					pmic_arb);
	err = spmi_controller_add(ctrl);
	if (err)
		goto err_domain_remove;

	return 0;

err_domain_remove:
	irq_set_chained_handler_and_data(pmic_arb->irq, NULL, NULL);
	irq_domain_remove(pmic_arb->domain);
	return err;
}

static void spmi_pmic_arb_remove(struct platform_device *pdev)
{
	struct spmi_controller *ctrl = platform_get_drvdata(pdev);
	struct spmi_pmic_arb *pmic_arb = spmi_controller_get_drvdata(ctrl);
	spmi_controller_remove(ctrl);
	irq_set_chained_handler_and_data(pmic_arb->irq, NULL, NULL);
	irq_domain_remove(pmic_arb->domain);
}

static const struct of_device_id spmi_pmic_arb_match_table[] = {
	{ .compatible = "qcom,spmi-pmic-arb", },
	{},
};
MODULE_DEVICE_TABLE(of, spmi_pmic_arb_match_table);

static struct platform_driver spmi_pmic_arb_driver = {
	.probe		= spmi_pmic_arb_probe,
	.remove_new	= spmi_pmic_arb_remove,
	.driver		= {
		.name	= "spmi_pmic_arb",
		.of_match_table = spmi_pmic_arb_match_table,
	},
};
module_platform_driver(spmi_pmic_arb_driver);

MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:spmi_pmic_arb");
