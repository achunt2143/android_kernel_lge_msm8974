<<<<<<< HEAD
/*
 *  Copyright (c) by Lee Revell <rlrevell@joe-job.com>
 *                   Clemens Ladisch <clemens@ladisch.de>
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
 *  Copyright (c) by Lee Revell <rlrevell@joe-job.com>
 *                   Clemens Ladisch <clemens@ladisch.de>
 *                   Oswald Buddenhagen <oswald.buddenhagen@gmx.de>
 *
 *  Routines for control of EMU10K1 chips
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/time.h>
#include <sound/core.h>
#include <sound/emu10k1.h>

static int snd_emu10k1_timer_start(struct snd_timer *timer)
{
	struct snd_emu10k1 *emu;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int delay;

	emu = snd_timer_chip(timer);
	delay = timer->sticks - 1;
	if (delay < 5 ) /* minimum time is 5 ticks */
		delay = 5;
<<<<<<< HEAD
	spin_lock_irqsave(&emu->reg_lock, flags);
	snd_emu10k1_intr_enable(emu, INTE_INTERVALTIMERENB);
	outw(delay & TIMER_RATE_MASK, emu->port + TIMER);
	spin_unlock_irqrestore(&emu->reg_lock, flags);
=======
	snd_emu10k1_intr_enable(emu, INTE_INTERVALTIMERENB);
	outw(delay & TIMER_RATE_MASK, emu->port + TIMER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int snd_emu10k1_timer_stop(struct snd_timer *timer)
{
	struct snd_emu10k1 *emu;
<<<<<<< HEAD
	unsigned long flags;

	emu = snd_timer_chip(timer);
	spin_lock_irqsave(&emu->reg_lock, flags);
	snd_emu10k1_intr_disable(emu, INTE_INTERVALTIMERENB);
	spin_unlock_irqrestore(&emu->reg_lock, flags);
	return 0;
}

static int snd_emu10k1_timer_precise_resolution(struct snd_timer *timer,
					       unsigned long *num, unsigned long *den)
{
	*num = 1;
	*den = 48000;
	return 0;
}

static struct snd_timer_hardware snd_emu10k1_timer_hw = {
	.flags = SNDRV_TIMER_HW_AUTO,
	.resolution = 20833, /* 1 sample @ 48KHZ = 20.833...us */
	.ticks = 1024,
	.start = snd_emu10k1_timer_start,
	.stop = snd_emu10k1_timer_stop,
	.precise_resolution = snd_emu10k1_timer_precise_resolution,
};

int __devinit snd_emu10k1_timer(struct snd_emu10k1 *emu, int device)
=======

	emu = snd_timer_chip(timer);
	snd_emu10k1_intr_disable(emu, INTE_INTERVALTIMERENB);
	return 0;
}

static unsigned long snd_emu10k1_timer_c_resolution(struct snd_timer *timer)
{
	struct snd_emu10k1 *emu = snd_timer_chip(timer);

	if (emu->card_capabilities->emu_model &&
	    emu->emu1010.word_clock == 44100)
		return 22676;  // 1 sample @ 44.1 kHz = 22.675736...us
	else
		return 20833;  // 1 sample @ 48 kHz = 20.833...us
}

static int snd_emu10k1_timer_precise_resolution(struct snd_timer *timer,
					       unsigned long *num, unsigned long *den)
{
	struct snd_emu10k1 *emu = snd_timer_chip(timer);

	*num = 1;
	if (emu->card_capabilities->emu_model)
		*den = emu->emu1010.word_clock;
	else
		*den = 48000;
	return 0;
}

static const struct snd_timer_hardware snd_emu10k1_timer_hw = {
	.flags = SNDRV_TIMER_HW_AUTO,
	.ticks = 1024,
	.start = snd_emu10k1_timer_start,
	.stop = snd_emu10k1_timer_stop,
	.c_resolution = snd_emu10k1_timer_c_resolution,
	.precise_resolution = snd_emu10k1_timer_precise_resolution,
};

int snd_emu10k1_timer(struct snd_emu10k1 *emu, int device)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct snd_timer *timer = NULL;
	struct snd_timer_id tid;
	int err;

	tid.dev_class = SNDRV_TIMER_CLASS_CARD;
	tid.dev_sclass = SNDRV_TIMER_SCLASS_NONE;
	tid.card = emu->card->number;
	tid.device = device;
	tid.subdevice = 0;
<<<<<<< HEAD
	if ((err = snd_timer_new(emu->card, "EMU10K1", &tid, &timer)) >= 0) {
=======
	err = snd_timer_new(emu->card, "EMU10K1", &tid, &timer);
	if (err >= 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		strcpy(timer->name, "EMU10K1 timer");
		timer->private_data = emu;
		timer->hw = snd_emu10k1_timer_hw;
	}
	emu->timer = timer;
	return err;
}
