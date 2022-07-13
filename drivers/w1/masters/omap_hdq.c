<<<<<<< HEAD
/*
 * drivers/w1/masters/omap_hdq.c
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2007,2012 Texas Instruments, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/sched.h>

#include <asm/irq.h>
#include <mach/hardware.h>

#include "../w1.h"
#include "../w1_int.h"
=======
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>

#include <linux/w1.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define	MOD_NAME	"OMAP_HDQ:"

#define OMAP_HDQ_REVISION			0x00
#define OMAP_HDQ_TX_DATA			0x04
#define OMAP_HDQ_RX_DATA			0x08
#define OMAP_HDQ_CTRL_STATUS			0x0c
<<<<<<< HEAD
#define OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK	(1<<6)
#define OMAP_HDQ_CTRL_STATUS_CLOCKENABLE	(1<<5)
#define OMAP_HDQ_CTRL_STATUS_GO			(1<<4)
#define OMAP_HDQ_CTRL_STATUS_INITIALIZATION	(1<<2)
#define OMAP_HDQ_CTRL_STATUS_DIR		(1<<1)
#define OMAP_HDQ_CTRL_STATUS_MODE		(1<<0)
#define OMAP_HDQ_INT_STATUS			0x10
#define OMAP_HDQ_INT_STATUS_TXCOMPLETE		(1<<2)
#define OMAP_HDQ_INT_STATUS_RXCOMPLETE		(1<<1)
#define OMAP_HDQ_INT_STATUS_TIMEOUT		(1<<0)
#define OMAP_HDQ_SYSCONFIG			0x14
#define OMAP_HDQ_SYSCONFIG_SOFTRESET		(1<<1)
#define OMAP_HDQ_SYSCONFIG_AUTOIDLE		(1<<0)
#define OMAP_HDQ_SYSSTATUS			0x18
#define OMAP_HDQ_SYSSTATUS_RESETDONE		(1<<0)
=======
#define OMAP_HDQ_CTRL_STATUS_SINGLE		BIT(7)
#define OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK	BIT(6)
#define OMAP_HDQ_CTRL_STATUS_CLOCKENABLE	BIT(5)
#define OMAP_HDQ_CTRL_STATUS_GO                 BIT(4)
#define OMAP_HDQ_CTRL_STATUS_PRESENCE		BIT(3)
#define OMAP_HDQ_CTRL_STATUS_INITIALIZATION	BIT(2)
#define OMAP_HDQ_CTRL_STATUS_DIR		BIT(1)
#define OMAP_HDQ_INT_STATUS			0x10
#define OMAP_HDQ_INT_STATUS_TXCOMPLETE		BIT(2)
#define OMAP_HDQ_INT_STATUS_RXCOMPLETE		BIT(1)
#define OMAP_HDQ_INT_STATUS_TIMEOUT		BIT(0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define OMAP_HDQ_FLAG_CLEAR			0
#define OMAP_HDQ_FLAG_SET			1
#define OMAP_HDQ_TIMEOUT			(HZ/5)

#define OMAP_HDQ_MAX_USER			4

static DECLARE_WAIT_QUEUE_HEAD(hdq_wait_queue);
<<<<<<< HEAD
static int w1_id;
=======

static int w1_id;
module_param(w1_id, int, 0400);
MODULE_PARM_DESC(w1_id, "1-wire id for the slave detection in HDQ mode");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct hdq_data {
	struct device		*dev;
	void __iomem		*hdq_base;
<<<<<<< HEAD
	/* lock status update */
	struct  mutex		hdq_mutex;
	int			hdq_usecount;
	struct	clk		*hdq_ick;
	struct	clk		*hdq_fck;
	u8			hdq_irqstatus;
	/* device lock */
	spinlock_t		hdq_spinlock;
	/*
	 * Used to control the call to omap_hdq_get and omap_hdq_put.
	 * HDQ Protocol: Write the CMD|REG_address first, followed by
	 * the data wrire or read.
	 */
	int			init_trans;
};

static int __devinit omap_hdq_probe(struct platform_device *pdev);
static int omap_hdq_remove(struct platform_device *pdev);

static struct platform_driver omap_hdq_driver = {
	.probe =	omap_hdq_probe,
	.remove =	omap_hdq_remove,
	.driver =	{
		.name =	"omap_hdq",
	},
};

static u8 omap_w1_read_byte(void *_hdq);
static void omap_w1_write_byte(void *_hdq, u8 byte);
static u8 omap_w1_reset_bus(void *_hdq);
static void omap_w1_search_bus(void *_hdq, struct w1_master *master_dev,
		u8 search_type,	w1_slave_found_callback slave_found);


static struct w1_bus_master omap_w1_master = {
	.read_byte	= omap_w1_read_byte,
	.write_byte	= omap_w1_write_byte,
	.reset_bus	= omap_w1_reset_bus,
	.search		= omap_w1_search_bus,
=======
	/* lock read/write/break operations */
	struct  mutex		hdq_mutex;
	/* interrupt status and a lock for it */
	u8			hdq_irqstatus;
	spinlock_t		hdq_spinlock;
	/* mode: 0-HDQ 1-W1 */
	int                     mode;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* HDQ register I/O routines */
static inline u8 hdq_reg_in(struct hdq_data *hdq_data, u32 offset)
{
<<<<<<< HEAD
	return __raw_readb(hdq_data->hdq_base + offset);
=======
	return __raw_readl(hdq_data->hdq_base + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void hdq_reg_out(struct hdq_data *hdq_data, u32 offset, u8 val)
{
<<<<<<< HEAD
	__raw_writeb(val, hdq_data->hdq_base + offset);
=======
	__raw_writel(val, hdq_data->hdq_base + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline u8 hdq_reg_merge(struct hdq_data *hdq_data, u32 offset,
			u8 val, u8 mask)
{
<<<<<<< HEAD
	u8 new_val = (__raw_readb(hdq_data->hdq_base + offset) & ~mask)
			| (val & mask);
	__raw_writeb(new_val, hdq_data->hdq_base + offset);
=======
	u8 new_val = (__raw_readl(hdq_data->hdq_base + offset) & ~mask)
			| (val & mask);
	__raw_writel(new_val, hdq_data->hdq_base + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return new_val;
}

/*
 * Wait for one or more bits in flag change.
 * HDQ_FLAG_SET: wait until any bit in the flag is set.
 * HDQ_FLAG_CLEAR: wait until all bits in the flag are cleared.
 * return 0 on success and -ETIMEDOUT in the case of timeout.
 */
static int hdq_wait_for_flag(struct hdq_data *hdq_data, u32 offset,
		u8 flag, u8 flag_set, u8 *status)
{
	int ret = 0;
	unsigned long timeout = jiffies + OMAP_HDQ_TIMEOUT;

	if (flag_set == OMAP_HDQ_FLAG_CLEAR) {
		/* wait for the flag clear */
		while (((*status = hdq_reg_in(hdq_data, offset)) & flag)
			&& time_before(jiffies, timeout)) {
			schedule_timeout_uninterruptible(1);
		}
		if (*status & flag)
			ret = -ETIMEDOUT;
	} else if (flag_set == OMAP_HDQ_FLAG_SET) {
		/* wait for the flag set */
		while (!((*status = hdq_reg_in(hdq_data, offset)) & flag)
			&& time_before(jiffies, timeout)) {
			schedule_timeout_uninterruptible(1);
		}
		if (!(*status & flag))
			ret = -ETIMEDOUT;
	} else
		return -EINVAL;

	return ret;
}

<<<<<<< HEAD
=======
/* Clear saved irqstatus after using an interrupt */
static u8 hdq_reset_irqstatus(struct hdq_data *hdq_data, u8 bits)
{
	unsigned long irqflags;
	u8 status;

	spin_lock_irqsave(&hdq_data->hdq_spinlock, irqflags);
	status = hdq_data->hdq_irqstatus;
	/* this is a read-modify-write */
	hdq_data->hdq_irqstatus &= ~bits;
	spin_unlock_irqrestore(&hdq_data->hdq_spinlock, irqflags);

	return status;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* write out a byte and fill *status with HDQ_INT_STATUS */
static int hdq_write_byte(struct hdq_data *hdq_data, u8 val, u8 *status)
{
	int ret;
	u8 tmp_status;
<<<<<<< HEAD
	unsigned long irqflags;

	*status = 0;

	spin_lock_irqsave(&hdq_data->hdq_spinlock, irqflags);
	/* clear interrupt flags via a dummy read */
	hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);
	/* ISR loads it with new INT_STATUS */
	hdq_data->hdq_irqstatus = 0;
	spin_unlock_irqrestore(&hdq_data->hdq_spinlock, irqflags);

=======

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0) {
		ret = -EINTR;
		goto rtn;
	}

	if (hdq_data->hdq_irqstatus)
		dev_err(hdq_data->dev, "TX irqstatus not cleared (%02x)\n",
			hdq_data->hdq_irqstatus);

	*status = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hdq_reg_out(hdq_data, OMAP_HDQ_TX_DATA, val);

	/* set the GO bit */
	hdq_reg_merge(hdq_data, OMAP_HDQ_CTRL_STATUS, OMAP_HDQ_CTRL_STATUS_GO,
		OMAP_HDQ_CTRL_STATUS_DIR | OMAP_HDQ_CTRL_STATUS_GO);
	/* wait for the TXCOMPLETE bit */
	ret = wait_event_timeout(hdq_wait_queue,
<<<<<<< HEAD
		hdq_data->hdq_irqstatus, OMAP_HDQ_TIMEOUT);
	if (ret == 0) {
		dev_dbg(hdq_data->dev, "TX wait elapsed\n");
		goto out;
	}

	*status = hdq_data->hdq_irqstatus;
	/* check irqstatus */
	if (!(*status & OMAP_HDQ_INT_STATUS_TXCOMPLETE)) {
		dev_dbg(hdq_data->dev, "timeout waiting for"
			"TXCOMPLETE/RXCOMPLETE, %x", *status);
=======
		(hdq_data->hdq_irqstatus & OMAP_HDQ_INT_STATUS_TXCOMPLETE),
		OMAP_HDQ_TIMEOUT);
	*status = hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_TXCOMPLETE);
	if (ret == 0) {
		dev_dbg(hdq_data->dev, "TX wait elapsed\n");
		ret = -ETIMEDOUT;
		goto out;
	}

	/* check irqstatus */
	if (!(*status & OMAP_HDQ_INT_STATUS_TXCOMPLETE)) {
		dev_dbg(hdq_data->dev, "timeout waiting for"
			" TXCOMPLETE/RXCOMPLETE, %x\n", *status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ETIMEDOUT;
		goto out;
	}

	/* wait for the GO bit return to zero */
	ret = hdq_wait_for_flag(hdq_data, OMAP_HDQ_CTRL_STATUS,
			OMAP_HDQ_CTRL_STATUS_GO,
			OMAP_HDQ_FLAG_CLEAR, &tmp_status);
	if (ret) {
		dev_dbg(hdq_data->dev, "timeout waiting GO bit"
<<<<<<< HEAD
			"return to zero, %x", tmp_status);
	}

out:
=======
			" return to zero, %x\n", tmp_status);
	}

out:
	mutex_unlock(&hdq_data->hdq_mutex);
rtn:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* HDQ Interrupt service routine */
static irqreturn_t hdq_isr(int irq, void *_hdq)
{
	struct hdq_data *hdq_data = _hdq;
	unsigned long irqflags;

	spin_lock_irqsave(&hdq_data->hdq_spinlock, irqflags);
<<<<<<< HEAD
	hdq_data->hdq_irqstatus = hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);
	spin_unlock_irqrestore(&hdq_data->hdq_spinlock, irqflags);
	dev_dbg(hdq_data->dev, "hdq_isr: %x", hdq_data->hdq_irqstatus);
=======
	hdq_data->hdq_irqstatus |= hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);
	spin_unlock_irqrestore(&hdq_data->hdq_spinlock, irqflags);
	dev_dbg(hdq_data->dev, "hdq_isr: %x\n", hdq_data->hdq_irqstatus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hdq_data->hdq_irqstatus &
		(OMAP_HDQ_INT_STATUS_TXCOMPLETE | OMAP_HDQ_INT_STATUS_RXCOMPLETE
		| OMAP_HDQ_INT_STATUS_TIMEOUT)) {
		/* wake up sleeping process */
		wake_up(&hdq_wait_queue);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
/* HDQ Mode: always return success */
static u8 omap_w1_reset_bus(void *_hdq)
{
	return 0;
}

/* W1 search callback function */
=======
/* W1 search callback function  in HDQ mode */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void omap_w1_search_bus(void *_hdq, struct w1_master *master_dev,
		u8 search_type, w1_slave_found_callback slave_found)
{
	u64 module_id, rn_le, cs, id;

	if (w1_id)
		module_id = w1_id;
	else
		module_id = 0x1;

	rn_le = cpu_to_le64(module_id);
	/*
	 * HDQ might not obey truly the 1-wire spec.
	 * So calculate CRC based on module parameter.
	 */
	cs = w1_calc_crc8((u8 *)&rn_le, 7);
	id = (cs << 56) | module_id;

	slave_found(master_dev, id);
}

<<<<<<< HEAD
static int _omap_hdq_reset(struct hdq_data *hdq_data)
{
	int ret;
	u8 tmp_status;

	hdq_reg_out(hdq_data, OMAP_HDQ_SYSCONFIG, OMAP_HDQ_SYSCONFIG_SOFTRESET);
	/*
	 * Select HDQ mode & enable clocks.
	 * It is observed that INT flags can't be cleared via a read and GO/INIT
	 * won't return to zero if interrupt is disabled. So we always enable
	 * interrupt.
	 */
	hdq_reg_out(hdq_data, OMAP_HDQ_CTRL_STATUS,
		OMAP_HDQ_CTRL_STATUS_CLOCKENABLE |
		OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK);

	/* wait for reset to complete */
	ret = hdq_wait_for_flag(hdq_data, OMAP_HDQ_SYSSTATUS,
		OMAP_HDQ_SYSSTATUS_RESETDONE, OMAP_HDQ_FLAG_SET, &tmp_status);
	if (ret)
		dev_dbg(hdq_data->dev, "timeout waiting HDQ reset, %x",
				tmp_status);
	else {
		hdq_reg_out(hdq_data, OMAP_HDQ_CTRL_STATUS,
			OMAP_HDQ_CTRL_STATUS_CLOCKENABLE |
			OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK);
		hdq_reg_out(hdq_data, OMAP_HDQ_SYSCONFIG,
			OMAP_HDQ_SYSCONFIG_AUTOIDLE);
	}

	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Issue break pulse to the device */
static int omap_hdq_break(struct hdq_data *hdq_data)
{
	int ret = 0;
	u8 tmp_status;
<<<<<<< HEAD
	unsigned long irqflags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0) {
		dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
		ret = -EINTR;
		goto rtn;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&hdq_data->hdq_spinlock, irqflags);
	/* clear interrupt flags via a dummy read */
	hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);
	/* ISR loads it with new INT_STATUS */
	hdq_data->hdq_irqstatus = 0;
	spin_unlock_irqrestore(&hdq_data->hdq_spinlock, irqflags);
=======
	if (hdq_data->hdq_irqstatus)
		dev_err(hdq_data->dev, "break irqstatus not cleared (%02x)\n",
			hdq_data->hdq_irqstatus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* set the INIT and GO bit */
	hdq_reg_merge(hdq_data, OMAP_HDQ_CTRL_STATUS,
		OMAP_HDQ_CTRL_STATUS_INITIALIZATION | OMAP_HDQ_CTRL_STATUS_GO,
		OMAP_HDQ_CTRL_STATUS_DIR | OMAP_HDQ_CTRL_STATUS_INITIALIZATION |
		OMAP_HDQ_CTRL_STATUS_GO);

	/* wait for the TIMEOUT bit */
	ret = wait_event_timeout(hdq_wait_queue,
<<<<<<< HEAD
		hdq_data->hdq_irqstatus, OMAP_HDQ_TIMEOUT);
=======
		(hdq_data->hdq_irqstatus & OMAP_HDQ_INT_STATUS_TIMEOUT),
		OMAP_HDQ_TIMEOUT);
	tmp_status = hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_TIMEOUT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret == 0) {
		dev_dbg(hdq_data->dev, "break wait elapsed\n");
		ret = -EINTR;
		goto out;
	}

<<<<<<< HEAD
	tmp_status = hdq_data->hdq_irqstatus;
	/* check irqstatus */
	if (!(tmp_status & OMAP_HDQ_INT_STATUS_TIMEOUT)) {
		dev_dbg(hdq_data->dev, "timeout waiting for TIMEOUT, %x",
				tmp_status);
		ret = -ETIMEDOUT;
		goto out;
	}
=======
	/* check irqstatus */
	if (!(tmp_status & OMAP_HDQ_INT_STATUS_TIMEOUT)) {
		dev_dbg(hdq_data->dev, "timeout waiting for TIMEOUT, %x\n",
			tmp_status);
		ret = -ETIMEDOUT;
		goto out;
	}

	/*
	 * check for the presence detect bit to get
	 * set to show that the slave is responding
	 */
	if (!(hdq_reg_in(hdq_data, OMAP_HDQ_CTRL_STATUS) &
			OMAP_HDQ_CTRL_STATUS_PRESENCE)) {
		dev_dbg(hdq_data->dev, "Presence bit not set\n");
		ret = -ETIMEDOUT;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * wait for both INIT and GO bits rerurn to zero.
	 * zero wait time expected for interrupt mode.
	 */
	ret = hdq_wait_for_flag(hdq_data, OMAP_HDQ_CTRL_STATUS,
			OMAP_HDQ_CTRL_STATUS_INITIALIZATION |
			OMAP_HDQ_CTRL_STATUS_GO, OMAP_HDQ_FLAG_CLEAR,
			&tmp_status);
	if (ret)
		dev_dbg(hdq_data->dev, "timeout waiting INIT&GO bits"
<<<<<<< HEAD
			"return to zero, %x", tmp_status);
=======
			" return to zero, %x\n", tmp_status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	mutex_unlock(&hdq_data->hdq_mutex);
rtn:
	return ret;
}

static int hdq_read_byte(struct hdq_data *hdq_data, u8 *val)
{
	int ret = 0;
	u8 status;
<<<<<<< HEAD
	unsigned long timeout = jiffies + OMAP_HDQ_TIMEOUT;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0) {
		ret = -EINTR;
		goto rtn;
	}

<<<<<<< HEAD
	if (!hdq_data->hdq_usecount) {
=======
	if (pm_runtime_suspended(hdq_data->dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

	if (!(hdq_data->hdq_irqstatus & OMAP_HDQ_INT_STATUS_RXCOMPLETE)) {
		hdq_reg_merge(hdq_data, OMAP_HDQ_CTRL_STATUS,
			OMAP_HDQ_CTRL_STATUS_DIR | OMAP_HDQ_CTRL_STATUS_GO,
			OMAP_HDQ_CTRL_STATUS_DIR | OMAP_HDQ_CTRL_STATUS_GO);
		/*
<<<<<<< HEAD
		 * The RX comes immediately after TX. It
		 * triggers another interrupt before we
		 * sleep. So we have to wait for RXCOMPLETE bit.
		 */
		while (!(hdq_data->hdq_irqstatus
			& OMAP_HDQ_INT_STATUS_RXCOMPLETE)
			&& time_before(jiffies, timeout)) {
			schedule_timeout_uninterruptible(1);
		}
		hdq_reg_merge(hdq_data, OMAP_HDQ_CTRL_STATUS, 0,
			OMAP_HDQ_CTRL_STATUS_DIR);
		status = hdq_data->hdq_irqstatus;
		/* check irqstatus */
		if (!(status & OMAP_HDQ_INT_STATUS_RXCOMPLETE)) {
			dev_dbg(hdq_data->dev, "timeout waiting for"
				"RXCOMPLETE, %x", status);
			ret = -ETIMEDOUT;
			goto out;
		}
=======
		 * The RX comes immediately after TX.
		 */
		wait_event_timeout(hdq_wait_queue,
				   (hdq_data->hdq_irqstatus
				    & (OMAP_HDQ_INT_STATUS_RXCOMPLETE |
				       OMAP_HDQ_INT_STATUS_TIMEOUT)),
				   OMAP_HDQ_TIMEOUT);
		status = hdq_reset_irqstatus(hdq_data,
					     OMAP_HDQ_INT_STATUS_RXCOMPLETE |
					     OMAP_HDQ_INT_STATUS_TIMEOUT);
		hdq_reg_merge(hdq_data, OMAP_HDQ_CTRL_STATUS, 0,
			OMAP_HDQ_CTRL_STATUS_DIR);

		/* check irqstatus */
		if (!(status & OMAP_HDQ_INT_STATUS_RXCOMPLETE)) {
			dev_dbg(hdq_data->dev, "timeout waiting for"
				" RXCOMPLETE, %x", status);
			ret = -ETIMEDOUT;
			goto out;
		}
	} else { /* interrupt had occurred before hdq_read_byte was called */
		hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_RXCOMPLETE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* the data is ready. Read it in! */
	*val = hdq_reg_in(hdq_data, OMAP_HDQ_RX_DATA);
out:
	mutex_unlock(&hdq_data->hdq_mutex);
rtn:
<<<<<<< HEAD
	return 0;

}

/* Enable clocks and set the controller to HDQ mode */
static int omap_hdq_get(struct hdq_data *hdq_data)
{
	int ret = 0;

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0) {
		ret = -EINTR;
		goto rtn;
	}

	if (OMAP_HDQ_MAX_USER == hdq_data->hdq_usecount) {
		dev_dbg(hdq_data->dev, "attempt to exceed the max use count");
		ret = -EINVAL;
		goto out;
	} else {
		hdq_data->hdq_usecount++;
		try_module_get(THIS_MODULE);
		if (1 == hdq_data->hdq_usecount) {
			if (clk_enable(hdq_data->hdq_ick)) {
				dev_dbg(hdq_data->dev, "Can not enable ick\n");
				ret = -ENODEV;
				goto clk_err;
			}
			if (clk_enable(hdq_data->hdq_fck)) {
				dev_dbg(hdq_data->dev, "Can not enable fck\n");
				clk_disable(hdq_data->hdq_ick);
				ret = -ENODEV;
				goto clk_err;
			}

			/* make sure HDQ is out of reset */
			if (!(hdq_reg_in(hdq_data, OMAP_HDQ_SYSSTATUS) &
				OMAP_HDQ_SYSSTATUS_RESETDONE)) {
				ret = _omap_hdq_reset(hdq_data);
				if (ret)
					/* back up the count */
					hdq_data->hdq_usecount--;
			} else {
				/* select HDQ mode & enable clocks */
				hdq_reg_out(hdq_data, OMAP_HDQ_CTRL_STATUS,
					OMAP_HDQ_CTRL_STATUS_CLOCKENABLE |
					OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK);
				hdq_reg_out(hdq_data, OMAP_HDQ_SYSCONFIG,
					OMAP_HDQ_SYSCONFIG_AUTOIDLE);
				hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);
			}
		}
	}

clk_err:
	clk_put(hdq_data->hdq_ick);
	clk_put(hdq_data->hdq_fck);
out:
	mutex_unlock(&hdq_data->hdq_mutex);
rtn:
	return ret;
}

/* Disable clocks to the module */
static int omap_hdq_put(struct hdq_data *hdq_data)
{
	int ret = 0;

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0)
		return -EINTR;

	if (0 == hdq_data->hdq_usecount) {
		dev_dbg(hdq_data->dev, "attempt to decrement use count"
			"when it is zero");
		ret = -EINVAL;
	} else {
		hdq_data->hdq_usecount--;
		module_put(THIS_MODULE);
		if (0 == hdq_data->hdq_usecount) {
			clk_disable(hdq_data->hdq_ick);
			clk_disable(hdq_data->hdq_fck);
		}
	}
	mutex_unlock(&hdq_data->hdq_mutex);

	return ret;
=======
	return ret;

}

/*
 * W1 triplet callback function - used for searching ROM addresses.
 * Registered only when controller is in 1-wire mode.
 */
static u8 omap_w1_triplet(void *_hdq, u8 bdir)
{
	u8 id_bit, comp_bit;
	int err;
	u8 ret = 0x3; /* no slaves responded */
	struct hdq_data *hdq_data = _hdq;
	u8 ctrl = OMAP_HDQ_CTRL_STATUS_SINGLE | OMAP_HDQ_CTRL_STATUS_GO |
		  OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK;
	u8 mask = ctrl | OMAP_HDQ_CTRL_STATUS_DIR;

	err = pm_runtime_get_sync(hdq_data->dev);
	if (err < 0) {
		pm_runtime_put_noidle(hdq_data->dev);

		return err;
	}

	err = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (err < 0) {
		dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
		goto rtn;
	}

	/* read id_bit */
	hdq_reg_merge(_hdq, OMAP_HDQ_CTRL_STATUS,
		      ctrl | OMAP_HDQ_CTRL_STATUS_DIR, mask);
	err = wait_event_timeout(hdq_wait_queue,
				 (hdq_data->hdq_irqstatus
				  & OMAP_HDQ_INT_STATUS_RXCOMPLETE),
				 OMAP_HDQ_TIMEOUT);
	/* Must clear irqstatus for another RXCOMPLETE interrupt */
	hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_RXCOMPLETE);

	if (err == 0) {
		dev_dbg(hdq_data->dev, "RX wait elapsed\n");
		goto out;
	}
	id_bit = (hdq_reg_in(_hdq, OMAP_HDQ_RX_DATA) & 0x01);

	/* read comp_bit */
	hdq_reg_merge(_hdq, OMAP_HDQ_CTRL_STATUS,
		      ctrl | OMAP_HDQ_CTRL_STATUS_DIR, mask);
	err = wait_event_timeout(hdq_wait_queue,
				 (hdq_data->hdq_irqstatus
				  & OMAP_HDQ_INT_STATUS_RXCOMPLETE),
				 OMAP_HDQ_TIMEOUT);
	/* Must clear irqstatus for another RXCOMPLETE interrupt */
	hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_RXCOMPLETE);

	if (err == 0) {
		dev_dbg(hdq_data->dev, "RX wait elapsed\n");
		goto out;
	}
	comp_bit = (hdq_reg_in(_hdq, OMAP_HDQ_RX_DATA) & 0x01);

	if (id_bit && comp_bit) {
		ret = 0x03;  /* no slaves responded */
		goto out;
	}
	if (!id_bit && !comp_bit) {
		/* Both bits are valid, take the direction given */
		ret = bdir ? 0x04 : 0;
	} else {
		/* Only one bit is valid, take that direction */
		bdir = id_bit;
		ret = id_bit ? 0x05 : 0x02;
	}

	/* write bdir bit */
	hdq_reg_out(_hdq, OMAP_HDQ_TX_DATA, bdir);
	hdq_reg_merge(_hdq, OMAP_HDQ_CTRL_STATUS, ctrl, mask);
	err = wait_event_timeout(hdq_wait_queue,
				 (hdq_data->hdq_irqstatus
				  & OMAP_HDQ_INT_STATUS_TXCOMPLETE),
				 OMAP_HDQ_TIMEOUT);
	/* Must clear irqstatus for another TXCOMPLETE interrupt */
	hdq_reset_irqstatus(hdq_data, OMAP_HDQ_INT_STATUS_TXCOMPLETE);

	if (err == 0) {
		dev_dbg(hdq_data->dev, "TX wait elapsed\n");
		goto out;
	}

	hdq_reg_merge(_hdq, OMAP_HDQ_CTRL_STATUS, 0,
		      OMAP_HDQ_CTRL_STATUS_SINGLE);

out:
	mutex_unlock(&hdq_data->hdq_mutex);
rtn:
	pm_runtime_mark_last_busy(hdq_data->dev);
	pm_runtime_put_autosuspend(hdq_data->dev);

	return ret;
}

/* reset callback */
static u8 omap_w1_reset_bus(void *_hdq)
{
	struct hdq_data *hdq_data = _hdq;
	int err;

	err = pm_runtime_get_sync(hdq_data->dev);
	if (err < 0) {
		pm_runtime_put_noidle(hdq_data->dev);

		return err;
	}

	omap_hdq_break(hdq_data);

	pm_runtime_mark_last_busy(hdq_data->dev);
	pm_runtime_put_autosuspend(hdq_data->dev);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Read a byte of data from the device */
static u8 omap_w1_read_byte(void *_hdq)
{
	struct hdq_data *hdq_data = _hdq;
	u8 val = 0;
	int ret;

<<<<<<< HEAD
	ret = hdq_read_byte(hdq_data, &val);
	if (ret) {
		ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
		if (ret < 0) {
			dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
			return -EINTR;
		}
		hdq_data->init_trans = 0;
		mutex_unlock(&hdq_data->hdq_mutex);
		omap_hdq_put(hdq_data);
		return -1;
	}

	/* Write followed by a read, release the module */
	if (hdq_data->init_trans) {
		ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
		if (ret < 0) {
			dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
			return -EINTR;
		}
		hdq_data->init_trans = 0;
		mutex_unlock(&hdq_data->hdq_mutex);
		omap_hdq_put(hdq_data);
	}
=======
	ret = pm_runtime_get_sync(hdq_data->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(hdq_data->dev);

		return -1;
	}

	ret = hdq_read_byte(hdq_data, &val);
	if (ret)
		val = -1;

	pm_runtime_mark_last_busy(hdq_data->dev);
	pm_runtime_put_autosuspend(hdq_data->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return val;
}

/* Write a byte of data to the device */
static void omap_w1_write_byte(void *_hdq, u8 byte)
{
	struct hdq_data *hdq_data = _hdq;
	int ret;
	u8 status;

<<<<<<< HEAD
	/* First write to initialize the transfer */
	if (hdq_data->init_trans == 0)
		omap_hdq_get(hdq_data);

	ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
	if (ret < 0) {
		dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
		return;
	}
	hdq_data->init_trans++;
	mutex_unlock(&hdq_data->hdq_mutex);

	ret = hdq_write_byte(hdq_data, byte, &status);
	if (ret == 0) {
		dev_dbg(hdq_data->dev, "TX failure:Ctrl status %x\n", status);
		return;
	}

	/* Second write, data transferred. Release the module */
	if (hdq_data->init_trans > 1) {
		omap_hdq_put(hdq_data);
		ret = mutex_lock_interruptible(&hdq_data->hdq_mutex);
		if (ret < 0) {
			dev_dbg(hdq_data->dev, "Could not acquire mutex\n");
			return;
		}
		hdq_data->init_trans = 0;
		mutex_unlock(&hdq_data->hdq_mutex);
	}

	return;
}

static int __devinit omap_hdq_probe(struct platform_device *pdev)
{
	struct hdq_data *hdq_data;
	struct resource *res;
	int ret, irq;
	u8 rev;

	hdq_data = kmalloc(sizeof(*hdq_data), GFP_KERNEL);
	if (!hdq_data) {
		dev_dbg(&pdev->dev, "unable to allocate memory\n");
		ret = -ENOMEM;
		goto err_kmalloc;
	}

	hdq_data->dev = &pdev->dev;
	platform_set_drvdata(pdev, hdq_data);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_dbg(&pdev->dev, "unable to get resource\n");
		ret = -ENXIO;
		goto err_resource;
	}

	hdq_data->hdq_base = ioremap(res->start, SZ_4K);
	if (!hdq_data->hdq_base) {
		dev_dbg(&pdev->dev, "ioremap failed\n");
		ret = -EINVAL;
		goto err_ioremap;
	}

	/* get interface & functional clock objects */
	hdq_data->hdq_ick = clk_get(&pdev->dev, "ick");
	if (IS_ERR(hdq_data->hdq_ick)) {
		dev_dbg(&pdev->dev, "Can't get HDQ ick clock object\n");
		ret = PTR_ERR(hdq_data->hdq_ick);
		goto err_ick;
	}

	hdq_data->hdq_fck = clk_get(&pdev->dev, "fck");
	if (IS_ERR(hdq_data->hdq_fck)) {
		dev_dbg(&pdev->dev, "Can't get HDQ fck clock object\n");
		ret = PTR_ERR(hdq_data->hdq_fck);
		goto err_fck;
	}

	hdq_data->hdq_usecount = 0;
	mutex_init(&hdq_data->hdq_mutex);

	if (clk_enable(hdq_data->hdq_ick)) {
		dev_dbg(&pdev->dev, "Can not enable ick\n");
		ret = -ENODEV;
		goto err_intfclk;
	}

	if (clk_enable(hdq_data->hdq_fck)) {
		dev_dbg(&pdev->dev, "Can not enable fck\n");
		ret = -ENODEV;
		goto err_fnclk;
=======
	ret = pm_runtime_get_sync(hdq_data->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(hdq_data->dev);

		return;
	}

	/*
	 * We need to reset the slave before
	 * issuing the SKIP ROM command, else
	 * the slave will not work.
	 */
	if (byte == W1_SKIP_ROM)
		omap_hdq_break(hdq_data);

	ret = hdq_write_byte(hdq_data, byte, &status);
	if (ret < 0) {
		dev_dbg(hdq_data->dev, "TX failure:Ctrl status %x\n", status);
		goto out_err;
	}

out_err:
	pm_runtime_mark_last_busy(hdq_data->dev);
	pm_runtime_put_autosuspend(hdq_data->dev);
}

static struct w1_bus_master omap_w1_master = {
	.read_byte	= omap_w1_read_byte,
	.write_byte	= omap_w1_write_byte,
	.reset_bus	= omap_w1_reset_bus,
};

static int __maybe_unused omap_hdq_runtime_suspend(struct device *dev)
{
	struct hdq_data *hdq_data = dev_get_drvdata(dev);

	hdq_reg_out(hdq_data, 0, hdq_data->mode);
	hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);

	return 0;
}

static int __maybe_unused omap_hdq_runtime_resume(struct device *dev)
{
	struct hdq_data *hdq_data = dev_get_drvdata(dev);

	/* select HDQ/1W mode & enable clocks */
	hdq_reg_out(hdq_data, OMAP_HDQ_CTRL_STATUS,
		    OMAP_HDQ_CTRL_STATUS_CLOCKENABLE |
		    OMAP_HDQ_CTRL_STATUS_INTERRUPTMASK |
		    hdq_data->mode);
	hdq_reg_in(hdq_data, OMAP_HDQ_INT_STATUS);

	return 0;
}

static const struct dev_pm_ops omap_hdq_pm_ops = {
	SET_RUNTIME_PM_OPS(omap_hdq_runtime_suspend,
			   omap_hdq_runtime_resume, NULL)
};

static int omap_hdq_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct hdq_data *hdq_data;
	int ret, irq;
	u8 rev;
	const char *mode;

	hdq_data = devm_kzalloc(dev, sizeof(*hdq_data), GFP_KERNEL);
	if (!hdq_data)
		return -ENOMEM;

	hdq_data->dev = dev;
	platform_set_drvdata(pdev, hdq_data);

	hdq_data->hdq_base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(hdq_data->hdq_base))
		return PTR_ERR(hdq_data->hdq_base);

	mutex_init(&hdq_data->hdq_mutex);

	ret = of_property_read_string(pdev->dev.of_node, "ti,mode", &mode);
	if (ret < 0 || !strcmp(mode, "hdq")) {
		hdq_data->mode = 0;
		omap_w1_master.search = omap_w1_search_bus;
	} else {
		hdq_data->mode = 1;
		omap_w1_master.triplet = omap_w1_triplet;
	}

	pm_runtime_enable(&pdev->dev);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev, 300);
	ret = pm_runtime_get_sync(&pdev->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(&pdev->dev);
		dev_dbg(&pdev->dev, "pm_runtime_get_sync failed\n");
		goto err_w1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rev = hdq_reg_in(hdq_data, OMAP_HDQ_REVISION);
	dev_info(&pdev->dev, "OMAP HDQ Hardware Rev %c.%c. Driver in %s mode\n",
		(rev >> 4) + '0', (rev & 0x0f) + '0', "Interrupt");

	spin_lock_init(&hdq_data->hdq_spinlock);

	irq = platform_get_irq(pdev, 0);
	if (irq	< 0) {
<<<<<<< HEAD
		ret = -ENXIO;
		goto err_irq;
	}

	ret = request_irq(irq, hdq_isr, IRQF_DISABLED, "omap_hdq", hdq_data);
=======
		dev_dbg(&pdev->dev, "Failed to get IRQ: %d\n", irq);
		ret = irq;
		goto err_irq;
	}

	ret = devm_request_irq(dev, irq, hdq_isr, 0, "omap_hdq", hdq_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0) {
		dev_dbg(&pdev->dev, "could not request irq\n");
		goto err_irq;
	}

	omap_hdq_break(hdq_data);

<<<<<<< HEAD
	/* don't clock the HDQ until it is needed */
	clk_disable(hdq_data->hdq_ick);
	clk_disable(hdq_data->hdq_fck);
=======
	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	omap_w1_master.data = hdq_data;

	ret = w1_add_master_device(&omap_w1_master);
	if (ret) {
		dev_dbg(&pdev->dev, "Failure in registering w1 master\n");
		goto err_w1;
	}

	return 0;

<<<<<<< HEAD
err_w1:
err_irq:
	clk_disable(hdq_data->hdq_fck);

err_fnclk:
	clk_disable(hdq_data->hdq_ick);

err_intfclk:
	clk_put(hdq_data->hdq_fck);

err_fck:
	clk_put(hdq_data->hdq_ick);

err_ick:
	iounmap(hdq_data->hdq_base);

err_ioremap:
err_resource:
	platform_set_drvdata(pdev, NULL);
	kfree(hdq_data);

err_kmalloc:
	return ret;

}

static int omap_hdq_remove(struct platform_device *pdev)
{
	struct hdq_data *hdq_data = platform_get_drvdata(pdev);

	mutex_lock(&hdq_data->hdq_mutex);

	if (hdq_data->hdq_usecount) {
		dev_dbg(&pdev->dev, "removed when use count is not zero\n");
		mutex_unlock(&hdq_data->hdq_mutex);
		return -EBUSY;
	}

	mutex_unlock(&hdq_data->hdq_mutex);

	/* remove module dependency */
	clk_put(hdq_data->hdq_ick);
	clk_put(hdq_data->hdq_fck);
	free_irq(INT_24XX_HDQ_IRQ, hdq_data);
	platform_set_drvdata(pdev, NULL);
	iounmap(hdq_data->hdq_base);
	kfree(hdq_data);

	return 0;
}

static int __init
omap_hdq_init(void)
{
	return platform_driver_register(&omap_hdq_driver);
}
module_init(omap_hdq_init);

static void __exit
omap_hdq_exit(void)
{
	platform_driver_unregister(&omap_hdq_driver);
}
module_exit(omap_hdq_exit);

module_param(w1_id, int, S_IRUSR);
MODULE_PARM_DESC(w1_id, "1-wire id for the slave detection");

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("HDQ driver Library");
=======
err_irq:
	pm_runtime_put_sync(&pdev->dev);
err_w1:
	pm_runtime_dont_use_autosuspend(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	return ret;
}

static void omap_hdq_remove(struct platform_device *pdev)
{
	int active;

	active = pm_runtime_get_sync(&pdev->dev);
	if (active < 0)
		pm_runtime_put_noidle(&pdev->dev);

	w1_remove_master_device(&omap_w1_master);

	pm_runtime_dont_use_autosuspend(&pdev->dev);
	if (active >= 0)
		pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
}

static const struct of_device_id omap_hdq_dt_ids[] = {
	{ .compatible = "ti,omap3-1w" },
	{ .compatible = "ti,am4372-hdq" },
	{}
};
MODULE_DEVICE_TABLE(of, omap_hdq_dt_ids);

static struct platform_driver omap_hdq_driver = {
	.probe = omap_hdq_probe,
	.remove_new = omap_hdq_remove,
	.driver = {
		.name =	"omap_hdq",
		.of_match_table = omap_hdq_dt_ids,
		.pm = &omap_hdq_pm_ops,
	},
};
module_platform_driver(omap_hdq_driver);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("HDQ-1W driver Library");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
