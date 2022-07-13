<<<<<<< HEAD
/*
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *                   Creative Labs, Inc.
 *  Routines for control of EMU10K1 chips
 *
 *  BUGS:
 *    --
 *
 *  TODO:
 *    --
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *                   Lee Revell <rlrevell@joe-job.com>
 *                   James Courtier-Dutton <James@superbug.co.uk>
 *                   Oswald Buddenhagen <oswald.buddenhagen@gmx.de>
 *                   Creative Labs, Inc.
 *
 *  Routines for control of EMU10K1 chips
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/time.h>
#include <sound/core.h>
#include <sound/emu10k1.h>
#include <linux/delay.h>
#include <linux/export.h>
#include "p17v.h"

<<<<<<< HEAD
=======
static inline bool check_ptr_reg(struct snd_emu10k1 *emu, unsigned int reg)
{
	if (snd_BUG_ON(!emu))
		return false;
	if (snd_BUG_ON(reg & (emu->audigy ? (0xffff0000 & ~A_PTR_ADDRESS_MASK)
					  : (0xffff0000 & ~PTR_ADDRESS_MASK))))
		return false;
	if (snd_BUG_ON(reg & 0x0000ffff & ~PTR_CHANNELNUM_MASK))
		return false;
	return true;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unsigned int snd_emu10k1_ptr_read(struct snd_emu10k1 * emu, unsigned int reg, unsigned int chn)
{
	unsigned long flags;
	unsigned int regptr, val;
	unsigned int mask;

<<<<<<< HEAD
	mask = emu->audigy ? A_PTR_ADDRESS_MASK : PTR_ADDRESS_MASK;
	regptr = ((reg << 16) & mask) | (chn & PTR_CHANNELNUM_MASK);
=======
	regptr = (reg << 16) | chn;
	if (!check_ptr_reg(emu, regptr))
		return 0;

	spin_lock_irqsave(&emu->emu_lock, flags);
	outl(regptr, emu->port + PTR);
	val = inl(emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (reg & 0xff000000) {
		unsigned char size, offset;
		
		size = (reg >> 24) & 0x3f;
		offset = (reg >> 16) & 0x1f;
<<<<<<< HEAD
		mask = ((1 << size) - 1) << offset;
		
		spin_lock_irqsave(&emu->emu_lock, flags);
		outl(regptr, emu->port + PTR);
		val = inl(emu->port + DATA);
		spin_unlock_irqrestore(&emu->emu_lock, flags);
		
		return (val & mask) >> offset;
	} else {
		spin_lock_irqsave(&emu->emu_lock, flags);
		outl(regptr, emu->port + PTR);
		val = inl(emu->port + DATA);
		spin_unlock_irqrestore(&emu->emu_lock, flags);
=======
		mask = (1 << size) - 1;
		
		return (val >> offset) & mask;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return val;
	}
}

EXPORT_SYMBOL(snd_emu10k1_ptr_read);

void snd_emu10k1_ptr_write(struct snd_emu10k1 *emu, unsigned int reg, unsigned int chn, unsigned int data)
{
	unsigned int regptr;
	unsigned long flags;
	unsigned int mask;

<<<<<<< HEAD
	if (!emu) {
		snd_printk(KERN_ERR "ptr_write: emu is null!\n");
		dump_stack();
		return;
	}
	mask = emu->audigy ? A_PTR_ADDRESS_MASK : PTR_ADDRESS_MASK;
	regptr = ((reg << 16) & mask) | (chn & PTR_CHANNELNUM_MASK);
=======
	regptr = (reg << 16) | chn;
	if (!check_ptr_reg(emu, regptr))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (reg & 0xff000000) {
		unsigned char size, offset;

		size = (reg >> 24) & 0x3f;
		offset = (reg >> 16) & 0x1f;
<<<<<<< HEAD
		mask = ((1 << size) - 1) << offset;
		data = (data << offset) & mask;
=======
		mask = (1 << size) - 1;
		if (snd_BUG_ON(data & ~mask))
			return;
		mask <<= offset;
		data <<= offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		spin_lock_irqsave(&emu->emu_lock, flags);
		outl(regptr, emu->port + PTR);
		data |= inl(emu->port + DATA) & ~mask;
<<<<<<< HEAD
		outl(data, emu->port + DATA);
		spin_unlock_irqrestore(&emu->emu_lock, flags);		
	} else {
		spin_lock_irqsave(&emu->emu_lock, flags);
		outl(regptr, emu->port + PTR);
		outl(data, emu->port + DATA);
		spin_unlock_irqrestore(&emu->emu_lock, flags);
	}
=======
	} else {
		spin_lock_irqsave(&emu->emu_lock, flags);
		outl(regptr, emu->port + PTR);
	}
	outl(data, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(snd_emu10k1_ptr_write);

<<<<<<< HEAD
=======
void snd_emu10k1_ptr_write_multiple(struct snd_emu10k1 *emu, unsigned int chn, ...)
{
	va_list va;
	u32 addr_mask;
	unsigned long flags;

	if (snd_BUG_ON(!emu))
		return;
	if (snd_BUG_ON(chn & ~PTR_CHANNELNUM_MASK))
		return;
	addr_mask = ~((emu->audigy ? A_PTR_ADDRESS_MASK : PTR_ADDRESS_MASK) >> 16);

	va_start(va, chn);
	spin_lock_irqsave(&emu->emu_lock, flags);
	for (;;) {
		u32 data;
		u32 reg = va_arg(va, u32);
		if (reg == REGLIST_END)
			break;
		data = va_arg(va, u32);
		if (snd_BUG_ON(reg & addr_mask))  // Only raw registers supported here
			continue;
		outl((reg << 16) | chn, emu->port + PTR);
		outl(data, emu->port + DATA);
	}
	spin_unlock_irqrestore(&emu->emu_lock, flags);
	va_end(va);
}

EXPORT_SYMBOL(snd_emu10k1_ptr_write_multiple);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unsigned int snd_emu10k1_ptr20_read(struct snd_emu10k1 * emu, 
					  unsigned int reg, 
					  unsigned int chn)
{
	unsigned long flags;
	unsigned int regptr, val;
  
	regptr = (reg << 16) | chn;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	outl(regptr, emu->port + 0x20 + PTR);
	val = inl(emu->port + 0x20 + DATA);
=======
	outl(regptr, emu->port + PTR2);
	val = inl(emu->port + DATA2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&emu->emu_lock, flags);
	return val;
}

void snd_emu10k1_ptr20_write(struct snd_emu10k1 *emu, 
				   unsigned int reg, 
				   unsigned int chn, 
				   unsigned int data)
{
	unsigned int regptr;
	unsigned long flags;

	regptr = (reg << 16) | chn;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	outl(regptr, emu->port + 0x20 + PTR);
	outl(data, emu->port + 0x20 + DATA);
=======
	outl(regptr, emu->port + PTR2);
	outl(data, emu->port + DATA2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

int snd_emu10k1_spi_write(struct snd_emu10k1 * emu,
				   unsigned int data)
{
	unsigned int reset, set;
	unsigned int reg, tmp;
	int n, result;
	int err = 0;

	/* This function is not re-entrant, so protect against it. */
	spin_lock(&emu->spi_lock);
	if (emu->card_capabilities->ca0108_chip)
<<<<<<< HEAD
		reg = 0x3c; /* PTR20, reg 0x3c */
=======
		reg = P17V_SPI;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else {
		/* For other chip types the SPI register
		 * is currently unknown. */
		err = 1;
		goto spi_write_exit;
	}
	if (data > 0xffff) {
		/* Only 16bit values allowed */
		err = 1;
		goto spi_write_exit;
	}

	tmp = snd_emu10k1_ptr20_read(emu, reg, 0);
	reset = (tmp & ~0x3ffff) | 0x20000; /* Set xxx20000 */
	set = reset | 0x10000; /* Set xxx1xxxx */
	snd_emu10k1_ptr20_write(emu, reg, 0, reset | data);
	tmp = snd_emu10k1_ptr20_read(emu, reg, 0); /* write post */
	snd_emu10k1_ptr20_write(emu, reg, 0, set | data);
	result = 1;
	/* Wait for status bit to return to 0 */
	for (n = 0; n < 100; n++) {
		udelay(10);
		tmp = snd_emu10k1_ptr20_read(emu, reg, 0);
		if (!(tmp & 0x10000)) {
			result = 0;
			break;
		}
	}
	if (result) {
		/* Timed out */
		err = 1;
		goto spi_write_exit;
	}
	snd_emu10k1_ptr20_write(emu, reg, 0, reset | data);
	tmp = snd_emu10k1_ptr20_read(emu, reg, 0); /* Write post */
	err = 0;
spi_write_exit:
	spin_unlock(&emu->spi_lock);
	return err;
}

/* The ADC does not support i2c read, so only write is implemented */
int snd_emu10k1_i2c_write(struct snd_emu10k1 *emu,
				u32 reg,
				u32 value)
{
	u32 tmp;
	int timeout = 0;
	int status;
	int retry;
	int err = 0;

	if ((reg > 0x7f) || (value > 0x1ff)) {
<<<<<<< HEAD
		snd_printk(KERN_ERR "i2c_write: invalid values.\n");
=======
		dev_err(emu->card->dev, "i2c_write: invalid values.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	/* This function is not re-entrant, so protect against it. */
	spin_lock(&emu->i2c_lock);

	tmp = reg << 25 | value << 16;

	/* This controls the I2C connected to the WM8775 ADC Codec */
	snd_emu10k1_ptr20_write(emu, P17V_I2C_1, 0, tmp);
	tmp = snd_emu10k1_ptr20_read(emu, P17V_I2C_1, 0); /* write post */

	for (retry = 0; retry < 10; retry++) {
		/* Send the data to i2c */
		tmp = 0;
		tmp = tmp | (I2C_A_ADC_LAST|I2C_A_ADC_START|I2C_A_ADC_ADD);
		snd_emu10k1_ptr20_write(emu, P17V_I2C_ADDR, 0, tmp);

		/* Wait till the transaction ends */
		while (1) {
			mdelay(1);
			status = snd_emu10k1_ptr20_read(emu, P17V_I2C_ADDR, 0);
			timeout++;
			if ((status & I2C_A_ADC_START) == 0)
				break;

			if (timeout > 1000) {
<<<<<<< HEAD
                		snd_printk(KERN_WARNING
=======
				dev_warn(emu->card->dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   "emu10k1:I2C:timeout status=0x%x\n",
					   status);
				break;
			}
		}
		//Read back and see if the transaction is successful
		if ((status & I2C_A_ADC_ABORT) == 0)
			break;
	}

	if (retry == 10) {
<<<<<<< HEAD
		snd_printk(KERN_ERR "Writing to ADC failed!\n");
		snd_printk(KERN_ERR "status=0x%x, reg=%d, value=%d\n",
=======
		dev_err(emu->card->dev, "Writing to ADC failed!\n");
		dev_err(emu->card->dev, "status=0x%x, reg=%d, value=%d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			status, reg, value);
		/* dump_stack(); */
		err = -EINVAL;
	}
    
	spin_unlock(&emu->i2c_lock);
	return err;
}

<<<<<<< HEAD
int snd_emu1010_fpga_write(struct snd_emu10k1 * emu, u32 reg, u32 value)
{
	unsigned long flags;

	if (reg > 0x3f)
		return 1;
	reg += 0x40; /* 0x40 upwards are registers. */
	if (value > 0x3f) /* 0 to 0x3f are values */
		return 1;
	spin_lock_irqsave(&emu->emu_lock, flags);
	outl(reg, emu->port + A_IOCFG);
	udelay(10);
	outl(reg | 0x80, emu->port + A_IOCFG);  /* High bit clocks the value into the fpga. */
	udelay(10);
	outl(value, emu->port + A_IOCFG);
	udelay(10);
	outl(value | 0x80 , emu->port + A_IOCFG);  /* High bit clocks the value into the fpga. */
	spin_unlock_irqrestore(&emu->emu_lock, flags);

	return 0;
}

int snd_emu1010_fpga_read(struct snd_emu10k1 * emu, u32 reg, u32 *value)
{
	unsigned long flags;
	if (reg > 0x3f)
		return 1;
	reg += 0x40; /* 0x40 upwards are registers. */
	spin_lock_irqsave(&emu->emu_lock, flags);
	outl(reg, emu->port + A_IOCFG);
	udelay(10);
	outl(reg | 0x80, emu->port + A_IOCFG);  /* High bit clocks the value into the fpga. */
	udelay(10);
	*value = ((inl(emu->port + A_IOCFG) >> 8) & 0x7f);
	spin_unlock_irqrestore(&emu->emu_lock, flags);

	return 0;
=======
static void snd_emu1010_fpga_write_locked(struct snd_emu10k1 *emu, u32 reg, u32 value)
{
	if (snd_BUG_ON(reg > 0x3f))
		return;
	reg += 0x40; /* 0x40 upwards are registers. */
	if (snd_BUG_ON(value > 0x3f)) /* 0 to 0x3f are values */
		return;
	outw(reg, emu->port + A_GPIO);
	udelay(10);
	outw(reg | 0x80, emu->port + A_GPIO);  /* High bit clocks the value into the fpga. */
	udelay(10);
	outw(value, emu->port + A_GPIO);
	udelay(10);
	outw(value | 0x80 , emu->port + A_GPIO);  /* High bit clocks the value into the fpga. */
	udelay(10);
}

void snd_emu1010_fpga_write(struct snd_emu10k1 *emu, u32 reg, u32 value)
{
	if (snd_BUG_ON(!mutex_is_locked(&emu->emu1010.lock)))
		return;
	snd_emu1010_fpga_write_locked(emu, reg, value);
}

void snd_emu1010_fpga_write_lock(struct snd_emu10k1 *emu, u32 reg, u32 value)
{
	snd_emu1010_fpga_lock(emu);
	snd_emu1010_fpga_write_locked(emu, reg, value);
	snd_emu1010_fpga_unlock(emu);
}

void snd_emu1010_fpga_read(struct snd_emu10k1 *emu, u32 reg, u32 *value)
{
	// The higest input pin is used as the designated interrupt trigger,
	// so it needs to be masked out.
	// But note that any other input pin change will also cause an IRQ,
	// so using this function often causes an IRQ as a side effect.
	u32 mask = emu->card_capabilities->ca0108_chip ? 0x1f : 0x7f;

	if (snd_BUG_ON(!mutex_is_locked(&emu->emu1010.lock)))
		return;
	if (snd_BUG_ON(reg > 0x3f))
		return;
	reg += 0x40; /* 0x40 upwards are registers. */
	outw(reg, emu->port + A_GPIO);
	udelay(10);
	outw(reg | 0x80, emu->port + A_GPIO);  /* High bit clocks the value into the fpga. */
	udelay(10);
	*value = ((inw(emu->port + A_GPIO) >> 8) & mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Each Destination has one and only one Source,
 * but one Source can feed any number of Destinations simultaneously.
 */
<<<<<<< HEAD
int snd_emu1010_fpga_link_dst_src_write(struct snd_emu10k1 * emu, u32 dst, u32 src)
{
	snd_emu1010_fpga_write(emu, 0x00, ((dst >> 8) & 0x3f) );
	snd_emu1010_fpga_write(emu, 0x01, (dst & 0x3f) );
	snd_emu1010_fpga_write(emu, 0x02, ((src >> 8) & 0x3f) );
	snd_emu1010_fpga_write(emu, 0x03, (src & 0x3f) );

	return 0;
=======
void snd_emu1010_fpga_link_dst_src_write(struct snd_emu10k1 *emu, u32 dst, u32 src)
{
	if (snd_BUG_ON(dst & ~0x71f))
		return;
	if (snd_BUG_ON(src & ~0x71f))
		return;
	snd_emu1010_fpga_write(emu, EMU_HANA_DESTHI, dst >> 8);
	snd_emu1010_fpga_write(emu, EMU_HANA_DESTLO, dst & 0x1f);
	snd_emu1010_fpga_write(emu, EMU_HANA_SRCHI, src >> 8);
	snd_emu1010_fpga_write(emu, EMU_HANA_SRCLO, src & 0x1f);
}

u32 snd_emu1010_fpga_link_dst_src_read(struct snd_emu10k1 *emu, u32 dst)
{
	u32 hi, lo;

	if (snd_BUG_ON(dst & ~0x71f))
		return 0;
	snd_emu1010_fpga_write(emu, EMU_HANA_DESTHI, dst >> 8);
	snd_emu1010_fpga_write(emu, EMU_HANA_DESTLO, dst & 0x1f);
	snd_emu1010_fpga_read(emu, EMU_HANA_SRCHI, &hi);
	snd_emu1010_fpga_read(emu, EMU_HANA_SRCLO, &lo);
	return (hi << 8) | lo;
}

int snd_emu1010_get_raw_rate(struct snd_emu10k1 *emu, u8 src)
{
	u32 reg_lo, reg_hi, value, value2;

	switch (src) {
	case EMU_HANA_WCLOCK_HANA_SPDIF_IN:
		snd_emu1010_fpga_read(emu, EMU_HANA_SPDIF_MODE, &value);
		if (value & EMU_HANA_SPDIF_MODE_RX_INVALID)
			return 0;
		reg_lo = EMU_HANA_WC_SPDIF_LO;
		reg_hi = EMU_HANA_WC_SPDIF_HI;
		break;
	case EMU_HANA_WCLOCK_HANA_ADAT_IN:
		reg_lo = EMU_HANA_WC_ADAT_LO;
		reg_hi = EMU_HANA_WC_ADAT_HI;
		break;
	case EMU_HANA_WCLOCK_SYNC_BNC:
		reg_lo = EMU_HANA_WC_BNC_LO;
		reg_hi = EMU_HANA_WC_BNC_HI;
		break;
	case EMU_HANA_WCLOCK_2ND_HANA:
		reg_lo = EMU_HANA2_WC_SPDIF_LO;
		reg_hi = EMU_HANA2_WC_SPDIF_HI;
		break;
	default:
		return 0;
	}
	snd_emu1010_fpga_read(emu, reg_hi, &value);
	snd_emu1010_fpga_read(emu, reg_lo, &value2);
	// FIXME: The /4 is valid for 0404b, but contradicts all other info.
	return 0x1770000 / 4 / (((value << 5) | value2) + 1);
}

void snd_emu1010_update_clock(struct snd_emu10k1 *emu)
{
	int clock;
	u32 leds;

	switch (emu->emu1010.wclock) {
	case EMU_HANA_WCLOCK_INT_44_1K | EMU_HANA_WCLOCK_1X:
		clock = 44100;
		leds = EMU_HANA_DOCK_LEDS_2_44K;
		break;
	case EMU_HANA_WCLOCK_INT_48K | EMU_HANA_WCLOCK_1X:
		clock = 48000;
		leds = EMU_HANA_DOCK_LEDS_2_48K;
		break;
	default:
		clock = snd_emu1010_get_raw_rate(
				emu, emu->emu1010.wclock & EMU_HANA_WCLOCK_SRC_MASK);
		// The raw rate reading is rather coarse (it cannot accurately
		// represent 44.1 kHz) and fluctuates slightly. Luckily, the
		// clock comes from digital inputs, which use standardized rates.
		// So we round to the closest standard rate and ignore discrepancies.
		if (clock < 46000) {
			clock = 44100;
			leds = EMU_HANA_DOCK_LEDS_2_EXT | EMU_HANA_DOCK_LEDS_2_44K;
		} else {
			clock = 48000;
			leds = EMU_HANA_DOCK_LEDS_2_EXT | EMU_HANA_DOCK_LEDS_2_48K;
		}
		break;
	}
	emu->emu1010.word_clock = clock;

	// FIXME: this should probably represent the AND of all currently
	// used sources' lock status. But we don't know how to get that ...
	leds |= EMU_HANA_DOCK_LEDS_2_LOCK;

	snd_emu1010_fpga_write(emu, EMU_HANA_DOCK_LEDS_2, leds);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void snd_emu10k1_intr_enable(struct snd_emu10k1 *emu, unsigned int intrenb)
{
	unsigned long flags;
	unsigned int enable;

	spin_lock_irqsave(&emu->emu_lock, flags);
	enable = inl(emu->port + INTE) | intrenb;
	outl(enable, emu->port + INTE);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_intr_disable(struct snd_emu10k1 *emu, unsigned int intrenb)
{
	unsigned long flags;
	unsigned int enable;

	spin_lock_irqsave(&emu->emu_lock, flags);
	enable = inl(emu->port + INTE) & ~intrenb;
	outl(enable, emu->port + INTE);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_intr_enable(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(CLIEH << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val |= 1 << (voicenum - 32);
	} else {
		outl(CLIEL << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val |= 1 << voicenum;
	}
	outl(val, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_intr_disable(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(CLIEH << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val &= ~(1 << (voicenum - 32));
	} else {
		outl(CLIEL << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val &= ~(1 << voicenum);
	}
	outl(val, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_intr_ack(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(CLIPH << 16, emu->port + PTR);
		voicenum = 1 << (voicenum - 32);
	} else {
		outl(CLIPL << 16, emu->port + PTR);
		voicenum = 1 << voicenum;
	}
	outl(voicenum, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_half_loop_intr_enable(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(HLIEH << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val |= 1 << (voicenum - 32);
	} else {
		outl(HLIEL << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val |= 1 << voicenum;
	}
	outl(val, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_half_loop_intr_disable(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(HLIEH << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val &= ~(1 << (voicenum - 32));
	} else {
		outl(HLIEL << 16, emu->port + PTR);
		val = inl(emu->port + DATA);
		val &= ~(1 << voicenum);
	}
	outl(val, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_half_loop_intr_ack(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(HLIPH << 16, emu->port + PTR);
		voicenum = 1 << (voicenum - 32);
	} else {
		outl(HLIPL << 16, emu->port + PTR);
		voicenum = 1 << voicenum;
	}
	outl(voicenum, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

<<<<<<< HEAD
=======
#if 0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void snd_emu10k1_voice_set_loop_stop(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int sol;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(SOLEH << 16, emu->port + PTR);
		sol = inl(emu->port + DATA);
		sol |= 1 << (voicenum - 32);
	} else {
		outl(SOLEL << 16, emu->port + PTR);
		sol = inl(emu->port + DATA);
		sol |= 1 << voicenum;
	}
	outl(sol, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_clear_loop_stop(struct snd_emu10k1 *emu, unsigned int voicenum)
{
	unsigned long flags;
	unsigned int sol;

	spin_lock_irqsave(&emu->emu_lock, flags);
<<<<<<< HEAD
	/* voice interrupt */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (voicenum >= 32) {
		outl(SOLEH << 16, emu->port + PTR);
		sol = inl(emu->port + DATA);
		sol &= ~(1 << (voicenum - 32));
	} else {
		outl(SOLEL << 16, emu->port + PTR);
		sol = inl(emu->port + DATA);
		sol &= ~(1 << voicenum);
	}
	outl(sol, emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}
<<<<<<< HEAD
=======
#endif

void snd_emu10k1_voice_set_loop_stop_multiple(struct snd_emu10k1 *emu, u64 voices)
{
	unsigned long flags;

	spin_lock_irqsave(&emu->emu_lock, flags);
	outl(SOLEL << 16, emu->port + PTR);
	outl(inl(emu->port + DATA) | (u32)voices, emu->port + DATA);
	outl(SOLEH << 16, emu->port + PTR);
	outl(inl(emu->port + DATA) | (u32)(voices >> 32), emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

void snd_emu10k1_voice_clear_loop_stop_multiple(struct snd_emu10k1 *emu, u64 voices)
{
	unsigned long flags;

	spin_lock_irqsave(&emu->emu_lock, flags);
	outl(SOLEL << 16, emu->port + PTR);
	outl(inl(emu->port + DATA) & (u32)~voices, emu->port + DATA);
	outl(SOLEH << 16, emu->port + PTR);
	outl(inl(emu->port + DATA) & (u32)(~voices >> 32), emu->port + DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}

int snd_emu10k1_voice_clear_loop_stop_multiple_atomic(struct snd_emu10k1 *emu, u64 voices)
{
	unsigned long flags;
	u32 soll, solh;
	int ret = -EIO;

	spin_lock_irqsave(&emu->emu_lock, flags);

	outl(SOLEL << 16, emu->port + PTR);
	soll = inl(emu->port + DATA);
	outl(SOLEH << 16, emu->port + PTR);
	solh = inl(emu->port + DATA);

	soll &= (u32)~voices;
	solh &= (u32)(~voices >> 32);

	for (int tries = 0; tries < 1000; tries++) {
		const u32 quart = 1U << (REG_SIZE(WC_CURRENTCHANNEL) - 2);
		// First we wait for the third quarter of the sample cycle ...
		u32 wc = inl(emu->port + WC);
		u32 cc = REG_VAL_GET(WC_CURRENTCHANNEL, wc);
		if (cc >= quart * 2 && cc < quart * 3) {
			// ... and release the low voices, while the high ones are serviced.
			outl(SOLEL << 16, emu->port + PTR);
			outl(soll, emu->port + DATA);
			// Then we wait for the first quarter of the next sample cycle ...
			for (; tries < 1000; tries++) {
				cc = REG_VAL_GET(WC_CURRENTCHANNEL, inl(emu->port + WC));
				if (cc < quart)
					goto good;
				// We will block for 10+ us with interrupts disabled. This is
				// not nice at all, but necessary for reasonable reliability.
				udelay(1);
			}
			break;
		good:
			// ... and release the high voices, while the low ones are serviced.
			outl(SOLEH << 16, emu->port + PTR);
			outl(solh, emu->port + DATA);
			// Finally we verify that nothing interfered in fact.
			if (REG_VAL_GET(WC_SAMPLECOUNTER, inl(emu->port + WC)) ==
			    ((REG_VAL_GET(WC_SAMPLECOUNTER, wc) + 1) & REG_MASK0(WC_SAMPLECOUNTER))) {
				ret = 0;
			} else {
				ret = -EAGAIN;
			}
			break;
		}
		// Don't block for too long
		spin_unlock_irqrestore(&emu->emu_lock, flags);
		udelay(1);
		spin_lock_irqsave(&emu->emu_lock, flags);
	}

	spin_unlock_irqrestore(&emu->emu_lock, flags);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void snd_emu10k1_wait(struct snd_emu10k1 *emu, unsigned int wait)
{
	volatile unsigned count;
	unsigned int newtime = 0, curtime;

	curtime = inl(emu->port + WC) >> 6;
	while (wait-- > 0) {
		count = 0;
		while (count++ < 16384) {
			newtime = inl(emu->port + WC) >> 6;
			if (newtime != curtime)
				break;
		}
		if (count > 16384)
			break;
		curtime = newtime;
	}
}

unsigned short snd_emu10k1_ac97_read(struct snd_ac97 *ac97, unsigned short reg)
{
	struct snd_emu10k1 *emu = ac97->private_data;
	unsigned long flags;
	unsigned short val;

	spin_lock_irqsave(&emu->emu_lock, flags);
	outb(reg, emu->port + AC97ADDRESS);
	val = inw(emu->port + AC97DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
	return val;
}

void snd_emu10k1_ac97_write(struct snd_ac97 *ac97, unsigned short reg, unsigned short data)
{
	struct snd_emu10k1 *emu = ac97->private_data;
	unsigned long flags;

	spin_lock_irqsave(&emu->emu_lock, flags);
	outb(reg, emu->port + AC97ADDRESS);
	outw(data, emu->port + AC97DATA);
	spin_unlock_irqrestore(&emu->emu_lock, flags);
}
<<<<<<< HEAD

/*
 *  convert rate to pitch
 */

unsigned int snd_emu10k1_rate_to_pitch(unsigned int rate)
{
	static u32 logMagTable[128] = {
		0x00000, 0x02dfc, 0x05b9e, 0x088e6, 0x0b5d6, 0x0e26f, 0x10eb3, 0x13aa2,
		0x1663f, 0x1918a, 0x1bc84, 0x1e72e, 0x2118b, 0x23b9a, 0x2655d, 0x28ed5,
		0x2b803, 0x2e0e8, 0x30985, 0x331db, 0x359eb, 0x381b6, 0x3a93d, 0x3d081,
		0x3f782, 0x41e42, 0x444c1, 0x46b01, 0x49101, 0x4b6c4, 0x4dc49, 0x50191,
		0x5269e, 0x54b6f, 0x57006, 0x59463, 0x5b888, 0x5dc74, 0x60029, 0x623a7,
		0x646ee, 0x66a00, 0x68cdd, 0x6af86, 0x6d1fa, 0x6f43c, 0x7164b, 0x73829,
		0x759d4, 0x77b4f, 0x79c9a, 0x7bdb5, 0x7dea1, 0x7ff5e, 0x81fed, 0x8404e,
		0x86082, 0x88089, 0x8a064, 0x8c014, 0x8df98, 0x8fef1, 0x91e20, 0x93d26,
		0x95c01, 0x97ab4, 0x9993e, 0x9b79f, 0x9d5d9, 0x9f3ec, 0xa11d8, 0xa2f9d,
		0xa4d3c, 0xa6ab5, 0xa8808, 0xaa537, 0xac241, 0xadf26, 0xafbe7, 0xb1885,
		0xb3500, 0xb5157, 0xb6d8c, 0xb899f, 0xba58f, 0xbc15e, 0xbdd0c, 0xbf899,
		0xc1404, 0xc2f50, 0xc4a7b, 0xc6587, 0xc8073, 0xc9b3f, 0xcb5ed, 0xcd07c,
		0xceaec, 0xd053f, 0xd1f73, 0xd398a, 0xd5384, 0xd6d60, 0xd8720, 0xda0c3,
		0xdba4a, 0xdd3b4, 0xded03, 0xe0636, 0xe1f4e, 0xe384a, 0xe512c, 0xe69f3,
		0xe829f, 0xe9b31, 0xeb3a9, 0xecc08, 0xee44c, 0xefc78, 0xf148a, 0xf2c83,
		0xf4463, 0xf5c2a, 0xf73da, 0xf8b71, 0xfa2f0, 0xfba57, 0xfd1a7, 0xfe8df
	};
	static char logSlopeTable[128] = {
		0x5c, 0x5c, 0x5b, 0x5a, 0x5a, 0x59, 0x58, 0x58,
		0x57, 0x56, 0x56, 0x55, 0x55, 0x54, 0x53, 0x53,
		0x52, 0x52, 0x51, 0x51, 0x50, 0x50, 0x4f, 0x4f,
		0x4e, 0x4d, 0x4d, 0x4d, 0x4c, 0x4c, 0x4b, 0x4b,
		0x4a, 0x4a, 0x49, 0x49, 0x48, 0x48, 0x47, 0x47,
		0x47, 0x46, 0x46, 0x45, 0x45, 0x45, 0x44, 0x44,
		0x43, 0x43, 0x43, 0x42, 0x42, 0x42, 0x41, 0x41,
		0x41, 0x40, 0x40, 0x40, 0x3f, 0x3f, 0x3f, 0x3e,
		0x3e, 0x3e, 0x3d, 0x3d, 0x3d, 0x3c, 0x3c, 0x3c,
		0x3b, 0x3b, 0x3b, 0x3b, 0x3a, 0x3a, 0x3a, 0x39,
		0x39, 0x39, 0x39, 0x38, 0x38, 0x38, 0x38, 0x37,
		0x37, 0x37, 0x37, 0x36, 0x36, 0x36, 0x36, 0x35,
		0x35, 0x35, 0x35, 0x34, 0x34, 0x34, 0x34, 0x34,
		0x33, 0x33, 0x33, 0x33, 0x32, 0x32, 0x32, 0x32,
		0x32, 0x31, 0x31, 0x31, 0x31, 0x31, 0x30, 0x30,
		0x30, 0x30, 0x30, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f
	};
	int i;

	if (rate == 0)
		return 0;	/* Bail out if no leading "1" */
	rate *= 11185;		/* Scale 48000 to 0x20002380 */
	for (i = 31; i > 0; i--) {
		if (rate & 0x80000000) {	/* Detect leading "1" */
			return (((unsigned int) (i - 15) << 20) +
			       logMagTable[0x7f & (rate >> 24)] +
					(0x7f & (rate >> 17)) *
					logSlopeTable[0x7f & (rate >> 24)]);
		}
		rate <<= 1;
	}

	return 0;		/* Should never reach this point */
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
