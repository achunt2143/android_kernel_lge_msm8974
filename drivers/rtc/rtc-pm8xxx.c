<<<<<<< HEAD
/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include <linux/mfd/pm8xxx/core.h>
#include <linux/mfd/pm8xxx/rtc.h>


/* RTC Register offsets from RTC CTRL REG */
#define PM8XXX_ALARM_CTRL_OFFSET	0x01
#define PM8XXX_RTC_WRITE_OFFSET		0x02
#define PM8XXX_RTC_READ_OFFSET		0x06
#define PM8XXX_ALARM_RW_OFFSET		0x0A

/* RTC_CTRL register bit fields */
#define PM8xxx_RTC_ENABLE		BIT(7)
#define PM8xxx_RTC_ALARM_ENABLE		BIT(1)
#define PM8xxx_RTC_ALARM_CLEAR		BIT(0)
#define PM8xxx_RTC_ABORT_ENABLE		BIT(0)
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * pm8xxx RTC driver
 *
 * Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 * Copyright (c) 2023, Linaro Limited
 */
#include <linux/of.h>
#include <linux/module.h>
#include <linux/nvmem-consumer.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pm_wakeirq.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include <asm/unaligned.h>

/* RTC_CTRL register bit fields */
#define PM8xxx_RTC_ENABLE		BIT(7)
#define PM8xxx_RTC_ALARM_CLEAR		BIT(0)
#define PM8xxx_RTC_ALARM_ENABLE		BIT(7)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define NUM_8_BIT_RTC_REGS		0x4

/**
<<<<<<< HEAD
 * struct pm8xxx_rtc -  rtc driver internal structure
 * @rtc:		rtc device for this driver.
 * @rtc_alarm_irq:	rtc alarm irq number.
 * @rtc_base:		address of rtc control register.
 * @rtc_read_base:	base address of read registers.
 * @rtc_write_base:	base address of write registers.
 * @alarm_rw_base:	base address of alarm registers.
 * @ctrl_reg:		rtc control register.
 * @rtc_dev:		device structure.
 * @ctrl_reg_lock:	spinlock protecting access to ctrl_reg.
 */
struct pm8xxx_rtc {
	struct rtc_device *rtc;
	int rtc_alarm_irq;
	int rtc_base;
	int rtc_read_base;
	int rtc_write_base;
	int alarm_rw_base;
	u8  ctrl_reg;
	struct device *rtc_dev;
	spinlock_t ctrl_reg_lock;
};

/*
 * The RTC registers need to be read/written one byte at a time. This is a
 * hardware limitation.
 */
static int pm8xxx_read_wrapper(struct pm8xxx_rtc *rtc_dd, u8 *rtc_val,
		int base, int count)
{
	int i, rc;
	struct device *parent = rtc_dd->rtc_dev->parent;

	for (i = 0; i < count; i++) {
		rc = pm8xxx_readb(parent, base + i, &rtc_val[i]);
		if (rc < 0) {
			dev_err(rtc_dd->rtc_dev, "PMIC read failed\n");
			return rc;
		}
=======
 * struct pm8xxx_rtc_regs - describe RTC registers per PMIC versions
 * @ctrl:		address of control register
 * @write:		base address of write registers
 * @read:		base address of read registers
 * @alarm_ctrl:		address of alarm control register
 * @alarm_ctrl2:	address of alarm control2 register
 * @alarm_rw:		base address of alarm read-write registers
 * @alarm_en:		alarm enable mask
 */
struct pm8xxx_rtc_regs {
	unsigned int ctrl;
	unsigned int write;
	unsigned int read;
	unsigned int alarm_ctrl;
	unsigned int alarm_ctrl2;
	unsigned int alarm_rw;
	unsigned int alarm_en;
};

/**
 * struct pm8xxx_rtc -  RTC driver internal structure
 * @rtc:		RTC device
 * @regmap:		regmap used to access registers
 * @allow_set_time:	whether the time can be set
 * @alarm_irq:		alarm irq number
 * @regs:		register description
 * @dev:		device structure
 * @nvmem_cell:		nvmem cell for offset
 * @offset:		offset from epoch in seconds
 */
struct pm8xxx_rtc {
	struct rtc_device *rtc;
	struct regmap *regmap;
	bool allow_set_time;
	int alarm_irq;
	const struct pm8xxx_rtc_regs *regs;
	struct device *dev;
	struct nvmem_cell *nvmem_cell;
	u32 offset;
};

static int pm8xxx_rtc_read_nvmem_offset(struct pm8xxx_rtc *rtc_dd)
{
	size_t len;
	void *buf;
	int rc;

	buf = nvmem_cell_read(rtc_dd->nvmem_cell, &len);
	if (IS_ERR(buf)) {
		rc = PTR_ERR(buf);
		dev_dbg(rtc_dd->dev, "failed to read nvmem offset: %d\n", rc);
		return rc;
	}

	if (len != sizeof(u32)) {
		dev_dbg(rtc_dd->dev, "unexpected nvmem cell size %zu\n", len);
		kfree(buf);
		return -EINVAL;
	}

	rtc_dd->offset = get_unaligned_le32(buf);

	kfree(buf);

	return 0;
}

static int pm8xxx_rtc_write_nvmem_offset(struct pm8xxx_rtc *rtc_dd, u32 offset)
{
	u8 buf[sizeof(u32)];
	int rc;

	put_unaligned_le32(offset, buf);

	rc = nvmem_cell_write(rtc_dd->nvmem_cell, buf, sizeof(buf));
	if (rc < 0) {
		dev_dbg(rtc_dd->dev, "failed to write nvmem offset: %d\n", rc);
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int pm8xxx_write_wrapper(struct pm8xxx_rtc *rtc_dd, u8 *rtc_val,
		int base, int count)
{
	int i, rc;
	struct device *parent = rtc_dd->rtc_dev->parent;

	for (i = 0; i < count; i++) {
		rc = pm8xxx_writeb(parent, base + i, rtc_val[i]);
		if (rc < 0) {
			dev_err(rtc_dd->rtc_dev, "PMIC write failed\n");
			return rc;
		}
	}

=======
static int pm8xxx_rtc_read_offset(struct pm8xxx_rtc *rtc_dd)
{
	if (!rtc_dd->nvmem_cell)
		return 0;

	return pm8xxx_rtc_read_nvmem_offset(rtc_dd);
}

static int pm8xxx_rtc_read_raw(struct pm8xxx_rtc *rtc_dd, u32 *secs)
{
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned int reg;
	int rc;

	rc = regmap_bulk_read(rtc_dd->regmap, regs->read, value, sizeof(value));
	if (rc)
		return rc;

	/*
	 * Read the LSB again and check if there has been a carry over.
	 * If there has, redo the read operation.
	 */
	rc = regmap_read(rtc_dd->regmap, regs->read, &reg);
	if (rc < 0)
		return rc;

	if (reg < value[0]) {
		rc = regmap_bulk_read(rtc_dd->regmap, regs->read, value,
				      sizeof(value));
		if (rc)
			return rc;
	}

	*secs = get_unaligned_le32(value);

	return 0;
}

static int pm8xxx_rtc_update_offset(struct pm8xxx_rtc *rtc_dd, u32 secs)
{
	u32 raw_secs;
	u32 offset;
	int rc;

	if (!rtc_dd->nvmem_cell)
		return -ENODEV;

	rc = pm8xxx_rtc_read_raw(rtc_dd, &raw_secs);
	if (rc)
		return rc;

	offset = secs - raw_secs;

	if (offset == rtc_dd->offset)
		return 0;

	rc = pm8xxx_rtc_write_nvmem_offset(rtc_dd, offset);
	if (rc)
		return rc;

	rtc_dd->offset = offset;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Steps to write the RTC registers.
 * 1. Disable alarm if enabled.
<<<<<<< HEAD
 * 2. Write 0x00 to LSB.
 * 3. Write Byte[1], Byte[2], Byte[3] then Byte[0].
 * 4. Enable alarm if disabled in step 1.
 */
static int pm8xxx_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	int rc, i;
	unsigned long secs, irq_flags;
	u8 value[NUM_8_BIT_RTC_REGS], reg = 0, alarm_enabled = 0, ctrl_reg;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rtc_tm_to_time(tm, &secs);

	for (i = 0; i < NUM_8_BIT_RTC_REGS; i++) {
		value[i] = secs & 0xFF;
		secs >>= 8;
	}

	dev_dbg(dev, "Seconds value to be written to RTC = %lu\n", secs);

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);
	ctrl_reg = rtc_dd->ctrl_reg;

	if (ctrl_reg & PM8xxx_RTC_ALARM_ENABLE) {
		alarm_enabled = 1;
		ctrl_reg &= ~PM8xxx_RTC_ALARM_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
				1);
		if (rc < 0) {
			dev_err(dev, "Write to RTC control register "
								"failed\n");
			goto rtc_rw_fail;
		}
		rtc_dd->ctrl_reg = ctrl_reg;
	} else
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Write 0 to Byte[0] */
	reg = 0;
	rc = pm8xxx_write_wrapper(rtc_dd, &reg, rtc_dd->rtc_write_base, 1);
	if (rc < 0) {
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

	/* Write Byte[1], Byte[2], Byte[3] */
	rc = pm8xxx_write_wrapper(rtc_dd, value + 1,
					rtc_dd->rtc_write_base + 1, 3);
	if (rc < 0) {
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

	/* Write Byte[0] */
	rc = pm8xxx_write_wrapper(rtc_dd, value, rtc_dd->rtc_write_base, 1);
	if (rc < 0) {
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

	if (alarm_enabled) {
		ctrl_reg |= PM8xxx_RTC_ALARM_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
									1);
		if (rc < 0) {
			dev_err(dev, "Write to RTC control register "
								"failed\n");
			goto rtc_rw_fail;
		}
		rtc_dd->ctrl_reg = ctrl_reg;
	}

rtc_rw_fail:
	if (alarm_enabled)
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	return rc;
=======
 * 2. Disable rtc if enabled.
 * 3. Write 0x00 to LSB.
 * 4. Write Byte[1], Byte[2], Byte[3] then Byte[0].
 * 5. Enable rtc if disabled in step 2.
 * 6. Enable alarm if disabled in step 1.
 */
static int __pm8xxx_rtc_set_time(struct pm8xxx_rtc *rtc_dd, u32 secs)
{
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	u8 value[NUM_8_BIT_RTC_REGS];
	bool alarm_enabled;
	int rc;

	put_unaligned_le32(secs, value);

	rc = regmap_update_bits_check(rtc_dd->regmap, regs->alarm_ctrl,
				      regs->alarm_en, 0, &alarm_enabled);
	if (rc)
		return rc;

	/* Disable RTC */
	rc = regmap_update_bits(rtc_dd->regmap, regs->ctrl, PM8xxx_RTC_ENABLE, 0);
	if (rc)
		return rc;

	/* Write 0 to Byte[0] */
	rc = regmap_write(rtc_dd->regmap, regs->write, 0);
	if (rc)
		return rc;

	/* Write Byte[1], Byte[2], Byte[3] */
	rc = regmap_bulk_write(rtc_dd->regmap, regs->write + 1,
			       &value[1], sizeof(value) - 1);
	if (rc)
		return rc;

	/* Write Byte[0] */
	rc = regmap_write(rtc_dd->regmap, regs->write, value[0]);
	if (rc)
		return rc;

	/* Enable RTC */
	rc = regmap_update_bits(rtc_dd->regmap, regs->ctrl, PM8xxx_RTC_ENABLE,
				PM8xxx_RTC_ENABLE);
	if (rc)
		return rc;

	if (alarm_enabled) {
		rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl,
					regs->alarm_en, regs->alarm_en);
		if (rc)
			return rc;
	}

	return 0;
}

static int pm8xxx_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	u32 secs;
	int rc;

	secs = rtc_tm_to_time64(tm);

	if (rtc_dd->allow_set_time)
		rc = __pm8xxx_rtc_set_time(rtc_dd, secs);
	else
		rc = pm8xxx_rtc_update_offset(rtc_dd, secs);

	if (rc)
		return rc;

	dev_dbg(dev, "set time: %ptRd %ptRt (%u + %u)\n", tm, tm,
			secs - rtc_dd->offset, rtc_dd->offset);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pm8xxx_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	int rc;
	u8 value[NUM_8_BIT_RTC_REGS], reg;
	unsigned long secs;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rc = pm8xxx_read_wrapper(rtc_dd, value, rtc_dd->rtc_read_base,
							NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
		dev_err(dev, "RTC read data register failed\n");
		return rc;
	}

	/*
	 * Read the LSB again and check if there has been a carry over.
	 * If there is, redo the read operation.
	 */
	rc = pm8xxx_read_wrapper(rtc_dd, &reg, rtc_dd->rtc_read_base, 1);
	if (rc < 0) {
		dev_err(dev, "RTC read data register failed\n");
		return rc;
	}

	if (unlikely(reg < value[0])) {
		rc = pm8xxx_read_wrapper(rtc_dd, value,
				rtc_dd->rtc_read_base, NUM_8_BIT_RTC_REGS);
		if (rc < 0) {
			dev_err(dev, "RTC read data register failed\n");
			return rc;
		}
	}

	secs = value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24);

	rtc_time_to_tm(secs, tm);

	rc = rtc_valid_tm(tm);
	if (rc < 0) {
		dev_err(dev, "Invalid time read from RTC\n");
		return rc;
	}

	dev_dbg(dev, "secs = %lu, h:m:s == %d:%d:%d, d/m/y = %d/%d/%d\n",
				secs, tm->tm_hour, tm->tm_min, tm->tm_sec,
				tm->tm_mday, tm->tm_mon, tm->tm_year);

=======
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	u32 secs;
	int rc;

	rc = pm8xxx_rtc_read_raw(rtc_dd, &secs);
	if (rc)
		return rc;

	secs += rtc_dd->offset;
	rtc_time64_to_tm(secs, tm);

	dev_dbg(dev, "read time: %ptRd %ptRt (%u + %u)\n", tm, tm,
			secs - rtc_dd->offset, rtc_dd->offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int pm8xxx_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
<<<<<<< HEAD
	int rc, i;
	u8 value[NUM_8_BIT_RTC_REGS], ctrl_reg;
	unsigned long secs, irq_flags;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rtc_tm_to_time(&alarm->time, &secs);

	for (i = 0; i < NUM_8_BIT_RTC_REGS; i++) {
		value[i] = secs & 0xFF;
		secs >>= 8;
	}

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

	rc = pm8xxx_write_wrapper(rtc_dd, value, rtc_dd->alarm_rw_base,
							NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
		dev_err(dev, "Write to RTC ALARM register failed\n");
		goto rtc_rw_fail;
	}

	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg = alarm->enabled ? (ctrl_reg | PM8xxx_RTC_ALARM_ENABLE) :
					(ctrl_reg & ~PM8xxx_RTC_ALARM_ENABLE);

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(dev, "Write to RTC control register failed\n");
		goto rtc_rw_fail;
	}

	rtc_dd->ctrl_reg = ctrl_reg;

	dev_dbg(dev, "Alarm Set for h:r:s=%d:%d:%d, d/m/y=%d/%d/%d\n",
				alarm->time.tm_hour, alarm->time.tm_min,
				alarm->time.tm_sec, alarm->time.tm_mday,
				alarm->time.tm_mon, alarm->time.tm_year);
rtc_rw_fail:
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
	return rc;
=======
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	u8 value[NUM_8_BIT_RTC_REGS];
	u32 secs;
	int rc;

	secs = rtc_tm_to_time64(&alarm->time);
	secs -= rtc_dd->offset;
	put_unaligned_le32(secs, value);

	rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl,
				regs->alarm_en, 0);
	if (rc)
		return rc;

	rc = regmap_bulk_write(rtc_dd->regmap, regs->alarm_rw, value,
			       sizeof(value));
	if (rc)
		return rc;

	if (alarm->enabled) {
		rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl,
					regs->alarm_en, regs->alarm_en);
		if (rc)
			return rc;
	}

	dev_dbg(dev, "set alarm: %ptRd %ptRt\n", &alarm->time, &alarm->time);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pm8xxx_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
<<<<<<< HEAD
	int rc;
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned long secs;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rc = pm8xxx_read_wrapper(rtc_dd, value, rtc_dd->alarm_rw_base,
			NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
		dev_err(dev, "RTC alarm time read failed\n");
		return rc;
	}

	secs = value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24);

	rtc_time_to_tm(secs, &alarm->time);

	rc = rtc_valid_tm(&alarm->time);
	if (rc < 0) {
		dev_err(dev, "Invalid alarm time read from RTC\n");
		return rc;
	}

	dev_dbg(dev, "Alarm set for - h:r:s=%d:%d:%d, d/m/y=%d/%d/%d\n",
				alarm->time.tm_hour, alarm->time.tm_min,
				alarm->time.tm_sec, alarm->time.tm_mday,
				alarm->time.tm_mon, alarm->time.tm_year);
=======
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned int ctrl_reg;
	u32 secs;
	int rc;

	rc = regmap_bulk_read(rtc_dd->regmap, regs->alarm_rw, value,
			      sizeof(value));
	if (rc)
		return rc;

	secs = get_unaligned_le32(value);
	secs += rtc_dd->offset;
	rtc_time64_to_tm(secs, &alarm->time);

	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl, &ctrl_reg);
	if (rc)
		return rc;

	alarm->enabled = !!(ctrl_reg & PM8xxx_RTC_ALARM_ENABLE);

	dev_dbg(dev, "read alarm: %ptRd %ptRt\n", &alarm->time, &alarm->time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int pm8xxx_rtc_alarm_irq_enable(struct device *dev, unsigned int enable)
{
<<<<<<< HEAD
	int rc;
	unsigned long irq_flags;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	u8 ctrl_reg;

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);
	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg = (enable) ? (ctrl_reg | PM8xxx_RTC_ALARM_ENABLE) :
				(ctrl_reg & ~PM8xxx_RTC_ALARM_ENABLE);

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(dev, "Write to RTC control register failed\n");
		goto rtc_rw_fail;
	}

	rtc_dd->ctrl_reg = ctrl_reg;

rtc_rw_fail:
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
	return rc;
}

static struct rtc_class_ops pm8xxx_rtc_ops = {
	.read_time	= pm8xxx_rtc_read_time,
=======
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	u8 value[NUM_8_BIT_RTC_REGS] = {0};
	unsigned int val;
	int rc;

	if (enable)
		val = regs->alarm_en;
	else
		val = 0;

	rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl,
				regs->alarm_en, val);
	if (rc)
		return rc;

	/* Clear alarm register */
	if (!enable) {
		rc = regmap_bulk_write(rtc_dd->regmap, regs->alarm_rw, value,
				       sizeof(value));
		if (rc)
			return rc;
	}

	return 0;
}

static const struct rtc_class_ops pm8xxx_rtc_ops = {
	.read_time	= pm8xxx_rtc_read_time,
	.set_time	= pm8xxx_rtc_set_time,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set_alarm	= pm8xxx_rtc_set_alarm,
	.read_alarm	= pm8xxx_rtc_read_alarm,
	.alarm_irq_enable = pm8xxx_rtc_alarm_irq_enable,
};

static irqreturn_t pm8xxx_alarm_trigger(int irq, void *dev_id)
{
	struct pm8xxx_rtc *rtc_dd = dev_id;
<<<<<<< HEAD
	u8 ctrl_reg;
	int rc;
	unsigned long irq_flags;

	rtc_update_irq(rtc_dd->rtc, 1, RTC_IRQF | RTC_AF);

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Clear the alarm enable bit */
	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg &= ~PM8xxx_RTC_ALARM_ENABLE;

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
		dev_err(rtc_dd->rtc_dev, "Write to RTC control register "
								"failed\n");
		goto rtc_alarm_handled;
	}

	rtc_dd->ctrl_reg = ctrl_reg;
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Clear RTC alarm register */
	rc = pm8xxx_read_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base +
						PM8XXX_ALARM_CTRL_OFFSET, 1);
	if (rc < 0) {
		dev_err(rtc_dd->rtc_dev, "RTC Alarm control register read "
								"failed\n");
		goto rtc_alarm_handled;
	}

	ctrl_reg &= ~PM8xxx_RTC_ALARM_CLEAR;
	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base +
						PM8XXX_ALARM_CTRL_OFFSET, 1);
	if (rc < 0)
		dev_err(rtc_dd->rtc_dev, "Write to RTC Alarm control register"
								" failed\n");

rtc_alarm_handled:
	return IRQ_HANDLED;
}

static int __devinit pm8xxx_rtc_probe(struct platform_device *pdev)
{
	int rc;
	u8 ctrl_reg;
	bool rtc_write_enable = false;
	struct pm8xxx_rtc *rtc_dd;
	struct resource *rtc_resource;
	const struct pm8xxx_rtc_platform_data *pdata =
						dev_get_platdata(&pdev->dev);

	if (pdata != NULL)
		rtc_write_enable = pdata->rtc_write_enable;

	rtc_dd = kzalloc(sizeof(*rtc_dd), GFP_KERNEL);
	if (rtc_dd == NULL) {
		dev_err(&pdev->dev, "Unable to allocate memory!\n");
		return -ENOMEM;
	}

	/* Initialise spinlock to protect RTC control register */
	spin_lock_init(&rtc_dd->ctrl_reg_lock);

	rtc_dd->rtc_alarm_irq = platform_get_irq(pdev, 0);
	if (rtc_dd->rtc_alarm_irq < 0) {
		dev_err(&pdev->dev, "Alarm IRQ resource absent!\n");
		rc = -ENXIO;
		goto fail_rtc_enable;
	}

	rtc_resource = platform_get_resource_byname(pdev, IORESOURCE_IO,
							"pmic_rtc_base");
	if (!(rtc_resource && rtc_resource->start)) {
		dev_err(&pdev->dev, "RTC IO resource absent!\n");
		rc = -ENXIO;
		goto fail_rtc_enable;
	}

	rtc_dd->rtc_base = rtc_resource->start;

	/* Setup RTC register addresses */
	rtc_dd->rtc_write_base = rtc_dd->rtc_base + PM8XXX_RTC_WRITE_OFFSET;
	rtc_dd->rtc_read_base = rtc_dd->rtc_base + PM8XXX_RTC_READ_OFFSET;
	rtc_dd->alarm_rw_base = rtc_dd->rtc_base + PM8XXX_ALARM_RW_OFFSET;

	rtc_dd->rtc_dev = &pdev->dev;

	/* Check if the RTC is on, else turn it on */
	rc = pm8xxx_read_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(&pdev->dev, "RTC control register read failed!\n");
		goto fail_rtc_enable;
	}

	if (!(ctrl_reg & PM8xxx_RTC_ENABLE)) {
		ctrl_reg |= PM8xxx_RTC_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
									1);
		if (rc < 0) {
			dev_err(&pdev->dev, "Write to RTC control register "
								"failed\n");
			goto fail_rtc_enable;
		}
	}

	/* Enable abort enable feature */
	ctrl_reg |= PM8xxx_RTC_ABORT_ENABLE;
	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(&pdev->dev, "PM8xxx write failed!\n");
		goto fail_rtc_enable;
	}

	rtc_dd->ctrl_reg = ctrl_reg;
	if (rtc_write_enable == true)
		pm8xxx_rtc_ops.set_time = pm8xxx_rtc_set_time;

	platform_set_drvdata(pdev, rtc_dd);

	/* Register the RTC device */
	rtc_dd->rtc = rtc_device_register("pm8xxx_rtc", &pdev->dev,
				&pm8xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc_dd->rtc)) {
		dev_err(&pdev->dev, "%s: RTC registration failed (%ld)\n",
					__func__, PTR_ERR(rtc_dd->rtc));
		rc = PTR_ERR(rtc_dd->rtc);
		goto fail_rtc_enable;
	}

	/* Request the alarm IRQ */
	rc = request_any_context_irq(rtc_dd->rtc_alarm_irq,
				 pm8xxx_alarm_trigger, IRQF_TRIGGER_RISING,
				 "pm8xxx_rtc_alarm", rtc_dd);
	if (rc < 0) {
		dev_err(&pdev->dev, "Request IRQ failed (%d)\n", rc);
		goto fail_req_irq;
	}

	device_init_wakeup(&pdev->dev, 1);

	dev_dbg(&pdev->dev, "Probe success !!\n");

	return 0;

fail_req_irq:
	rtc_device_unregister(rtc_dd->rtc);
fail_rtc_enable:
	platform_set_drvdata(pdev, NULL);
	kfree(rtc_dd);
	return rc;
}

static int __devexit pm8xxx_rtc_remove(struct platform_device *pdev)
{
	struct pm8xxx_rtc *rtc_dd = platform_get_drvdata(pdev);

	device_init_wakeup(&pdev->dev, 0);
	free_irq(rtc_dd->rtc_alarm_irq, rtc_dd);
	rtc_device_unregister(rtc_dd->rtc);
	platform_set_drvdata(pdev, NULL);
	kfree(rtc_dd);
=======
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	int rc;

	rtc_update_irq(rtc_dd->rtc, 1, RTC_IRQF | RTC_AF);

	/* Disable alarm */
	rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl,
				regs->alarm_en, 0);
	if (rc)
		return IRQ_NONE;

	/* Clear alarm status */
	rc = regmap_update_bits(rtc_dd->regmap, regs->alarm_ctrl2,
				PM8xxx_RTC_ALARM_CLEAR, 0);
	if (rc)
		return IRQ_NONE;

	return IRQ_HANDLED;
}

static int pm8xxx_rtc_enable(struct pm8xxx_rtc *rtc_dd)
{
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;

	return regmap_update_bits(rtc_dd->regmap, regs->ctrl, PM8xxx_RTC_ENABLE,
				  PM8xxx_RTC_ENABLE);
}

static const struct pm8xxx_rtc_regs pm8921_regs = {
	.ctrl		= 0x11d,
	.write		= 0x11f,
	.read		= 0x123,
	.alarm_rw	= 0x127,
	.alarm_ctrl	= 0x11d,
	.alarm_ctrl2	= 0x11e,
	.alarm_en	= BIT(1),
};

static const struct pm8xxx_rtc_regs pm8058_regs = {
	.ctrl		= 0x1e8,
	.write		= 0x1ea,
	.read		= 0x1ee,
	.alarm_rw	= 0x1f2,
	.alarm_ctrl	= 0x1e8,
	.alarm_ctrl2	= 0x1e9,
	.alarm_en	= BIT(1),
};

static const struct pm8xxx_rtc_regs pm8941_regs = {
	.ctrl		= 0x6046,
	.write		= 0x6040,
	.read		= 0x6048,
	.alarm_rw	= 0x6140,
	.alarm_ctrl	= 0x6146,
	.alarm_ctrl2	= 0x6148,
	.alarm_en	= BIT(7),
};

static const struct pm8xxx_rtc_regs pmk8350_regs = {
	.ctrl		= 0x6146,
	.write		= 0x6140,
	.read		= 0x6148,
	.alarm_rw	= 0x6240,
	.alarm_ctrl	= 0x6246,
	.alarm_ctrl2	= 0x6248,
	.alarm_en	= BIT(7),
};

static const struct of_device_id pm8xxx_id_table[] = {
	{ .compatible = "qcom,pm8921-rtc", .data = &pm8921_regs },
	{ .compatible = "qcom,pm8058-rtc", .data = &pm8058_regs },
	{ .compatible = "qcom,pm8941-rtc", .data = &pm8941_regs },
	{ .compatible = "qcom,pmk8350-rtc", .data = &pmk8350_regs },
	{ },
};
MODULE_DEVICE_TABLE(of, pm8xxx_id_table);

static int pm8xxx_rtc_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct pm8xxx_rtc *rtc_dd;
	int rc;

	match = of_match_node(pm8xxx_id_table, pdev->dev.of_node);
	if (!match)
		return -ENXIO;

	rtc_dd = devm_kzalloc(&pdev->dev, sizeof(*rtc_dd), GFP_KERNEL);
	if (rtc_dd == NULL)
		return -ENOMEM;

	rtc_dd->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!rtc_dd->regmap)
		return -ENXIO;

	rtc_dd->alarm_irq = platform_get_irq(pdev, 0);
	if (rtc_dd->alarm_irq < 0)
		return -ENXIO;

	rtc_dd->allow_set_time = of_property_read_bool(pdev->dev.of_node,
						      "allow-set-time");

	rtc_dd->nvmem_cell = devm_nvmem_cell_get(&pdev->dev, "offset");
	if (IS_ERR(rtc_dd->nvmem_cell)) {
		rc = PTR_ERR(rtc_dd->nvmem_cell);
		if (rc != -ENOENT)
			return rc;
		rtc_dd->nvmem_cell = NULL;
	}

	rtc_dd->regs = match->data;
	rtc_dd->dev = &pdev->dev;

	if (!rtc_dd->allow_set_time) {
		rc = pm8xxx_rtc_read_offset(rtc_dd);
		if (rc)
			return rc;
	}

	rc = pm8xxx_rtc_enable(rtc_dd);
	if (rc)
		return rc;

	platform_set_drvdata(pdev, rtc_dd);

	device_init_wakeup(&pdev->dev, 1);

	rtc_dd->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(rtc_dd->rtc))
		return PTR_ERR(rtc_dd->rtc);

	rtc_dd->rtc->ops = &pm8xxx_rtc_ops;
	rtc_dd->rtc->range_max = U32_MAX;

	rc = devm_request_any_context_irq(&pdev->dev, rtc_dd->alarm_irq,
					  pm8xxx_alarm_trigger,
					  IRQF_TRIGGER_RISING,
					  "pm8xxx_rtc_alarm", rtc_dd);
	if (rc < 0)
		return rc;

	rc = devm_rtc_register_device(rtc_dd->rtc);
	if (rc)
		return rc;

	rc = dev_pm_set_wake_irq(&pdev->dev, rtc_dd->alarm_irq);
	if (rc)
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int pm8xxx_rtc_resume(struct device *dev)
{
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(rtc_dd->rtc_alarm_irq);

	return 0;
}

static int pm8xxx_rtc_suspend(struct device *dev)
{
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(rtc_dd->rtc_alarm_irq);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(pm8xxx_rtc_pm_ops, pm8xxx_rtc_suspend, pm8xxx_rtc_resume);

static void pm8xxx_rtc_shutdown(struct platform_device *pdev)
{
	u8 value[4] = {0, 0, 0, 0};
	u8 reg;
	int rc;
	unsigned long irq_flags;
	bool rtc_alarm_powerup = false;
	struct pm8xxx_rtc *rtc_dd = platform_get_drvdata(pdev);
	struct pm8xxx_rtc_platform_data *pdata = pdev->dev.platform_data;

	if (pdata != NULL)
		rtc_alarm_powerup =  pdata->rtc_alarm_powerup;

	if (!rtc_alarm_powerup) {

		spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);
		dev_dbg(&pdev->dev, "Disabling alarm interrupts\n");

		/* Disable RTC alarms */
		reg = rtc_dd->ctrl_reg;
		reg &= ~PM8xxx_RTC_ALARM_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &reg, rtc_dd->rtc_base, 1);
		if (rc < 0) {
			dev_err(rtc_dd->rtc_dev, "Disabling alarm failed\n");
			goto fail_alarm_disable;
		}

		/* Clear Alarm register */
		rc = pm8xxx_write_wrapper(rtc_dd, value,
				rtc_dd->alarm_rw_base, NUM_8_BIT_RTC_REGS);
		if (rc < 0)
			dev_err(rtc_dd->rtc_dev, "Clearing alarm failed\n");

fail_alarm_disable:
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
	}
=======
static void pm8xxx_remove(struct platform_device *pdev)
{
	dev_pm_clear_wake_irq(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver pm8xxx_rtc_driver = {
	.probe		= pm8xxx_rtc_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(pm8xxx_rtc_remove),
	.shutdown	= pm8xxx_rtc_shutdown,
	.driver	= {
		.name	= PM8XXX_RTC_DEV_NAME,
		.owner	= THIS_MODULE,
		.pm	= &pm8xxx_rtc_pm_ops,
=======
	.remove_new	= pm8xxx_remove,
	.driver	= {
		.name		= "rtc-pm8xxx",
		.of_match_table	= pm8xxx_id_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(pm8xxx_rtc_driver);

MODULE_ALIAS("platform:rtc-pm8xxx");
MODULE_DESCRIPTION("PMIC8xxx RTC driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Anirudh Ghayal <aghayal@codeaurora.org>");
<<<<<<< HEAD
=======
MODULE_AUTHOR("Johan Hovold <johan@kernel.org>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
