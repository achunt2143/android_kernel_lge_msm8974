<<<<<<< HEAD
/**
 * debugfs.c - DesignWare USB3 DRD Controller DebugFS file
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/module.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * debugfs.c - DesignWare USB3 DRD Controller DebugFS file
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - https://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ptrace.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

#include <linux/usb/ch9.h>

#include "core.h"
#include "gadget.h"
#include "io.h"
#include "debug.h"

<<<<<<< HEAD
#define dump_register(nm)				\
{							\
	.name	= __stringify(nm),			\
	.offset	= DWC3_ ##nm - DWC3_GLOBALS_REGS_START,	\
}

=======
#define DWC3_LSP_MUX_UNSELECTED 0xfffff

#define dump_register(nm)				\
{							\
	.name	= __stringify(nm),			\
	.offset	= DWC3_ ##nm,				\
}

#define dump_ep_register_set(n)			\
	{					\
		.name = "DEPCMDPAR2("__stringify(n)")",	\
		.offset = DWC3_DEP_BASE(n) +	\
			DWC3_DEPCMDPAR2,	\
	},					\
	{					\
		.name = "DEPCMDPAR1("__stringify(n)")",	\
		.offset = DWC3_DEP_BASE(n) +	\
			DWC3_DEPCMDPAR1,	\
	},					\
	{					\
		.name = "DEPCMDPAR0("__stringify(n)")",	\
		.offset = DWC3_DEP_BASE(n) +	\
			DWC3_DEPCMDPAR0,	\
	},					\
	{					\
		.name = "DEPCMD("__stringify(n)")",	\
		.offset = DWC3_DEP_BASE(n) +	\
			DWC3_DEPCMD,		\
	}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct debugfs_reg32 dwc3_regs[] = {
	dump_register(GSBUSCFG0),
	dump_register(GSBUSCFG1),
	dump_register(GTXTHRCFG),
	dump_register(GRXTHRCFG),
	dump_register(GCTL),
	dump_register(GEVTEN),
	dump_register(GSTS),
<<<<<<< HEAD
=======
	dump_register(GUCTL1),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_register(GSNPSID),
	dump_register(GGPIO),
	dump_register(GUID),
	dump_register(GUCTL),
	dump_register(GBUSERRADDR0),
	dump_register(GBUSERRADDR1),
	dump_register(GPRTBIMAP0),
	dump_register(GPRTBIMAP1),
	dump_register(GHWPARAMS0),
	dump_register(GHWPARAMS1),
	dump_register(GHWPARAMS2),
	dump_register(GHWPARAMS3),
	dump_register(GHWPARAMS4),
	dump_register(GHWPARAMS5),
	dump_register(GHWPARAMS6),
	dump_register(GHWPARAMS7),
	dump_register(GDBGFIFOSPACE),
	dump_register(GDBGLTSSM),
<<<<<<< HEAD
=======
	dump_register(GDBGBMU),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_register(GPRTBIMAP_HS0),
	dump_register(GPRTBIMAP_HS1),
	dump_register(GPRTBIMAP_FS0),
	dump_register(GPRTBIMAP_FS1),
<<<<<<< HEAD
=======
	dump_register(GUCTL2),
	dump_register(VER_NUMBER),
	dump_register(VER_TYPE),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dump_register(GUSB2PHYCFG(0)),
	dump_register(GUSB2PHYCFG(1)),
	dump_register(GUSB2PHYCFG(2)),
	dump_register(GUSB2PHYCFG(3)),
	dump_register(GUSB2PHYCFG(4)),
	dump_register(GUSB2PHYCFG(5)),
	dump_register(GUSB2PHYCFG(6)),
	dump_register(GUSB2PHYCFG(7)),
	dump_register(GUSB2PHYCFG(8)),
	dump_register(GUSB2PHYCFG(9)),
	dump_register(GUSB2PHYCFG(10)),
	dump_register(GUSB2PHYCFG(11)),
	dump_register(GUSB2PHYCFG(12)),
	dump_register(GUSB2PHYCFG(13)),
	dump_register(GUSB2PHYCFG(14)),
	dump_register(GUSB2PHYCFG(15)),

	dump_register(GUSB2I2CCTL(0)),
	dump_register(GUSB2I2CCTL(1)),
	dump_register(GUSB2I2CCTL(2)),
	dump_register(GUSB2I2CCTL(3)),
	dump_register(GUSB2I2CCTL(4)),
	dump_register(GUSB2I2CCTL(5)),
	dump_register(GUSB2I2CCTL(6)),
	dump_register(GUSB2I2CCTL(7)),
	dump_register(GUSB2I2CCTL(8)),
	dump_register(GUSB2I2CCTL(9)),
	dump_register(GUSB2I2CCTL(10)),
	dump_register(GUSB2I2CCTL(11)),
	dump_register(GUSB2I2CCTL(12)),
	dump_register(GUSB2I2CCTL(13)),
	dump_register(GUSB2I2CCTL(14)),
	dump_register(GUSB2I2CCTL(15)),

	dump_register(GUSB2PHYACC(0)),
	dump_register(GUSB2PHYACC(1)),
	dump_register(GUSB2PHYACC(2)),
	dump_register(GUSB2PHYACC(3)),
	dump_register(GUSB2PHYACC(4)),
	dump_register(GUSB2PHYACC(5)),
	dump_register(GUSB2PHYACC(6)),
	dump_register(GUSB2PHYACC(7)),
	dump_register(GUSB2PHYACC(8)),
	dump_register(GUSB2PHYACC(9)),
	dump_register(GUSB2PHYACC(10)),
	dump_register(GUSB2PHYACC(11)),
	dump_register(GUSB2PHYACC(12)),
	dump_register(GUSB2PHYACC(13)),
	dump_register(GUSB2PHYACC(14)),
	dump_register(GUSB2PHYACC(15)),

	dump_register(GUSB3PIPECTL(0)),
	dump_register(GUSB3PIPECTL(1)),
	dump_register(GUSB3PIPECTL(2)),
	dump_register(GUSB3PIPECTL(3)),
	dump_register(GUSB3PIPECTL(4)),
	dump_register(GUSB3PIPECTL(5)),
	dump_register(GUSB3PIPECTL(6)),
	dump_register(GUSB3PIPECTL(7)),
	dump_register(GUSB3PIPECTL(8)),
	dump_register(GUSB3PIPECTL(9)),
	dump_register(GUSB3PIPECTL(10)),
	dump_register(GUSB3PIPECTL(11)),
	dump_register(GUSB3PIPECTL(12)),
	dump_register(GUSB3PIPECTL(13)),
	dump_register(GUSB3PIPECTL(14)),
	dump_register(GUSB3PIPECTL(15)),

	dump_register(GTXFIFOSIZ(0)),
	dump_register(GTXFIFOSIZ(1)),
	dump_register(GTXFIFOSIZ(2)),
	dump_register(GTXFIFOSIZ(3)),
	dump_register(GTXFIFOSIZ(4)),
	dump_register(GTXFIFOSIZ(5)),
	dump_register(GTXFIFOSIZ(6)),
	dump_register(GTXFIFOSIZ(7)),
	dump_register(GTXFIFOSIZ(8)),
	dump_register(GTXFIFOSIZ(9)),
	dump_register(GTXFIFOSIZ(10)),
	dump_register(GTXFIFOSIZ(11)),
	dump_register(GTXFIFOSIZ(12)),
	dump_register(GTXFIFOSIZ(13)),
	dump_register(GTXFIFOSIZ(14)),
	dump_register(GTXFIFOSIZ(15)),
	dump_register(GTXFIFOSIZ(16)),
	dump_register(GTXFIFOSIZ(17)),
	dump_register(GTXFIFOSIZ(18)),
	dump_register(GTXFIFOSIZ(19)),
	dump_register(GTXFIFOSIZ(20)),
	dump_register(GTXFIFOSIZ(21)),
	dump_register(GTXFIFOSIZ(22)),
	dump_register(GTXFIFOSIZ(23)),
	dump_register(GTXFIFOSIZ(24)),
	dump_register(GTXFIFOSIZ(25)),
	dump_register(GTXFIFOSIZ(26)),
	dump_register(GTXFIFOSIZ(27)),
	dump_register(GTXFIFOSIZ(28)),
	dump_register(GTXFIFOSIZ(29)),
	dump_register(GTXFIFOSIZ(30)),
	dump_register(GTXFIFOSIZ(31)),

	dump_register(GRXFIFOSIZ(0)),
	dump_register(GRXFIFOSIZ(1)),
	dump_register(GRXFIFOSIZ(2)),
	dump_register(GRXFIFOSIZ(3)),
	dump_register(GRXFIFOSIZ(4)),
	dump_register(GRXFIFOSIZ(5)),
	dump_register(GRXFIFOSIZ(6)),
	dump_register(GRXFIFOSIZ(7)),
	dump_register(GRXFIFOSIZ(8)),
	dump_register(GRXFIFOSIZ(9)),
	dump_register(GRXFIFOSIZ(10)),
	dump_register(GRXFIFOSIZ(11)),
	dump_register(GRXFIFOSIZ(12)),
	dump_register(GRXFIFOSIZ(13)),
	dump_register(GRXFIFOSIZ(14)),
	dump_register(GRXFIFOSIZ(15)),
	dump_register(GRXFIFOSIZ(16)),
	dump_register(GRXFIFOSIZ(17)),
	dump_register(GRXFIFOSIZ(18)),
	dump_register(GRXFIFOSIZ(19)),
	dump_register(GRXFIFOSIZ(20)),
	dump_register(GRXFIFOSIZ(21)),
	dump_register(GRXFIFOSIZ(22)),
	dump_register(GRXFIFOSIZ(23)),
	dump_register(GRXFIFOSIZ(24)),
	dump_register(GRXFIFOSIZ(25)),
	dump_register(GRXFIFOSIZ(26)),
	dump_register(GRXFIFOSIZ(27)),
	dump_register(GRXFIFOSIZ(28)),
	dump_register(GRXFIFOSIZ(29)),
	dump_register(GRXFIFOSIZ(30)),
	dump_register(GRXFIFOSIZ(31)),

	dump_register(GEVNTADRLO(0)),
	dump_register(GEVNTADRHI(0)),
	dump_register(GEVNTSIZ(0)),
	dump_register(GEVNTCOUNT(0)),

	dump_register(GHWPARAMS8),
<<<<<<< HEAD
=======
	dump_register(GUCTL3),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_register(GFLADJ),
	dump_register(DCFG),
	dump_register(DCTL),
	dump_register(DEVTEN),
	dump_register(DSTS),
	dump_register(DGCMDPAR),
	dump_register(DGCMD),
	dump_register(DALEPENA),

<<<<<<< HEAD
	dump_register(DEPCMDPAR2(0)),
	dump_register(DEPCMDPAR2(1)),
	dump_register(DEPCMDPAR2(2)),
	dump_register(DEPCMDPAR2(3)),
	dump_register(DEPCMDPAR2(4)),
	dump_register(DEPCMDPAR2(5)),
	dump_register(DEPCMDPAR2(6)),
	dump_register(DEPCMDPAR2(7)),
	dump_register(DEPCMDPAR2(8)),
	dump_register(DEPCMDPAR2(9)),
	dump_register(DEPCMDPAR2(10)),
	dump_register(DEPCMDPAR2(11)),
	dump_register(DEPCMDPAR2(12)),
	dump_register(DEPCMDPAR2(13)),
	dump_register(DEPCMDPAR2(14)),
	dump_register(DEPCMDPAR2(15)),
	dump_register(DEPCMDPAR2(16)),
	dump_register(DEPCMDPAR2(17)),
	dump_register(DEPCMDPAR2(18)),
	dump_register(DEPCMDPAR2(19)),
	dump_register(DEPCMDPAR2(20)),
	dump_register(DEPCMDPAR2(21)),
	dump_register(DEPCMDPAR2(22)),
	dump_register(DEPCMDPAR2(23)),
	dump_register(DEPCMDPAR2(24)),
	dump_register(DEPCMDPAR2(25)),
	dump_register(DEPCMDPAR2(26)),
	dump_register(DEPCMDPAR2(27)),
	dump_register(DEPCMDPAR2(28)),
	dump_register(DEPCMDPAR2(29)),
	dump_register(DEPCMDPAR2(30)),
	dump_register(DEPCMDPAR2(31)),

	dump_register(DEPCMDPAR1(0)),
	dump_register(DEPCMDPAR1(1)),
	dump_register(DEPCMDPAR1(2)),
	dump_register(DEPCMDPAR1(3)),
	dump_register(DEPCMDPAR1(4)),
	dump_register(DEPCMDPAR1(5)),
	dump_register(DEPCMDPAR1(6)),
	dump_register(DEPCMDPAR1(7)),
	dump_register(DEPCMDPAR1(8)),
	dump_register(DEPCMDPAR1(9)),
	dump_register(DEPCMDPAR1(10)),
	dump_register(DEPCMDPAR1(11)),
	dump_register(DEPCMDPAR1(12)),
	dump_register(DEPCMDPAR1(13)),
	dump_register(DEPCMDPAR1(14)),
	dump_register(DEPCMDPAR1(15)),
	dump_register(DEPCMDPAR1(16)),
	dump_register(DEPCMDPAR1(17)),
	dump_register(DEPCMDPAR1(18)),
	dump_register(DEPCMDPAR1(19)),
	dump_register(DEPCMDPAR1(20)),
	dump_register(DEPCMDPAR1(21)),
	dump_register(DEPCMDPAR1(22)),
	dump_register(DEPCMDPAR1(23)),
	dump_register(DEPCMDPAR1(24)),
	dump_register(DEPCMDPAR1(25)),
	dump_register(DEPCMDPAR1(26)),
	dump_register(DEPCMDPAR1(27)),
	dump_register(DEPCMDPAR1(28)),
	dump_register(DEPCMDPAR1(29)),
	dump_register(DEPCMDPAR1(30)),
	dump_register(DEPCMDPAR1(31)),

	dump_register(DEPCMDPAR0(0)),
	dump_register(DEPCMDPAR0(1)),
	dump_register(DEPCMDPAR0(2)),
	dump_register(DEPCMDPAR0(3)),
	dump_register(DEPCMDPAR0(4)),
	dump_register(DEPCMDPAR0(5)),
	dump_register(DEPCMDPAR0(6)),
	dump_register(DEPCMDPAR0(7)),
	dump_register(DEPCMDPAR0(8)),
	dump_register(DEPCMDPAR0(9)),
	dump_register(DEPCMDPAR0(10)),
	dump_register(DEPCMDPAR0(11)),
	dump_register(DEPCMDPAR0(12)),
	dump_register(DEPCMDPAR0(13)),
	dump_register(DEPCMDPAR0(14)),
	dump_register(DEPCMDPAR0(15)),
	dump_register(DEPCMDPAR0(16)),
	dump_register(DEPCMDPAR0(17)),
	dump_register(DEPCMDPAR0(18)),
	dump_register(DEPCMDPAR0(19)),
	dump_register(DEPCMDPAR0(20)),
	dump_register(DEPCMDPAR0(21)),
	dump_register(DEPCMDPAR0(22)),
	dump_register(DEPCMDPAR0(23)),
	dump_register(DEPCMDPAR0(24)),
	dump_register(DEPCMDPAR0(25)),
	dump_register(DEPCMDPAR0(26)),
	dump_register(DEPCMDPAR0(27)),
	dump_register(DEPCMDPAR0(28)),
	dump_register(DEPCMDPAR0(29)),
	dump_register(DEPCMDPAR0(30)),
	dump_register(DEPCMDPAR0(31)),

	dump_register(DEPCMD(0)),
	dump_register(DEPCMD(1)),
	dump_register(DEPCMD(2)),
	dump_register(DEPCMD(3)),
	dump_register(DEPCMD(4)),
	dump_register(DEPCMD(5)),
	dump_register(DEPCMD(6)),
	dump_register(DEPCMD(7)),
	dump_register(DEPCMD(8)),
	dump_register(DEPCMD(9)),
	dump_register(DEPCMD(10)),
	dump_register(DEPCMD(11)),
	dump_register(DEPCMD(12)),
	dump_register(DEPCMD(13)),
	dump_register(DEPCMD(14)),
	dump_register(DEPCMD(15)),
	dump_register(DEPCMD(16)),
	dump_register(DEPCMD(17)),
	dump_register(DEPCMD(18)),
	dump_register(DEPCMD(19)),
	dump_register(DEPCMD(20)),
	dump_register(DEPCMD(21)),
	dump_register(DEPCMD(22)),
	dump_register(DEPCMD(23)),
	dump_register(DEPCMD(24)),
	dump_register(DEPCMD(25)),
	dump_register(DEPCMD(26)),
	dump_register(DEPCMD(27)),
	dump_register(DEPCMD(28)),
	dump_register(DEPCMD(29)),
	dump_register(DEPCMD(30)),
	dump_register(DEPCMD(31)),

	dump_register(OCFG),
	dump_register(OCTL),
=======
	dump_ep_register_set(0),
	dump_ep_register_set(1),
	dump_ep_register_set(2),
	dump_ep_register_set(3),
	dump_ep_register_set(4),
	dump_ep_register_set(5),
	dump_ep_register_set(6),
	dump_ep_register_set(7),
	dump_ep_register_set(8),
	dump_ep_register_set(9),
	dump_ep_register_set(10),
	dump_ep_register_set(11),
	dump_ep_register_set(12),
	dump_ep_register_set(13),
	dump_ep_register_set(14),
	dump_ep_register_set(15),
	dump_ep_register_set(16),
	dump_ep_register_set(17),
	dump_ep_register_set(18),
	dump_ep_register_set(19),
	dump_ep_register_set(20),
	dump_ep_register_set(21),
	dump_ep_register_set(22),
	dump_ep_register_set(23),
	dump_ep_register_set(24),
	dump_ep_register_set(25),
	dump_ep_register_set(26),
	dump_ep_register_set(27),
	dump_ep_register_set(28),
	dump_ep_register_set(29),
	dump_ep_register_set(30),
	dump_ep_register_set(31),

	dump_register(OCFG),
	dump_register(OCTL),
	dump_register(OEVT),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_register(OEVTEN),
	dump_register(OSTS),
};

<<<<<<< HEAD
static int dwc3_regdump_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;

	seq_printf(s, "DesignWare USB3 Core Register Dump\n");
	debugfs_print_regs32(s, dwc3_regs, ARRAY_SIZE(dwc3_regs),
			     dwc->regs, "");
	return 0;
}

static int dwc3_regdump_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_regdump_show, inode->i_private);
}

static const struct file_operations dwc3_regdump_fops = {
	.open			= dwc3_regdump_open,
	.read			= seq_read,
=======
static void dwc3_host_lsp(struct seq_file *s)
{
	struct dwc3		*dwc = s->private;
	bool			dbc_enabled;
	u32			sel;
	u32			reg;
	u32			val;

	dbc_enabled = !!(dwc->hwparams.hwparams1 & DWC3_GHWPARAMS1_ENDBC);

	sel = dwc->dbg_lsp_select;
	if (sel == DWC3_LSP_MUX_UNSELECTED) {
		seq_puts(s, "Write LSP selection to print for host\n");
		return;
	}

	reg = DWC3_GDBGLSPMUX_HOSTSELECT(sel);

	dwc3_writel(dwc->regs, DWC3_GDBGLSPMUX, reg);
	val = dwc3_readl(dwc->regs, DWC3_GDBGLSP);
	seq_printf(s, "GDBGLSP[%d] = 0x%08x\n", sel, val);

	if (dbc_enabled && sel < 256) {
		reg |= DWC3_GDBGLSPMUX_ENDBC;
		dwc3_writel(dwc->regs, DWC3_GDBGLSPMUX, reg);
		val = dwc3_readl(dwc->regs, DWC3_GDBGLSP);
		seq_printf(s, "GDBGLSP_DBC[%d] = 0x%08x\n", sel, val);
	}
}

static void dwc3_gadget_lsp(struct seq_file *s)
{
	struct dwc3		*dwc = s->private;
	int			i;
	u32			reg;

	for (i = 0; i < 16; i++) {
		reg = DWC3_GDBGLSPMUX_DEVSELECT(i);
		dwc3_writel(dwc->regs, DWC3_GDBGLSPMUX, reg);
		reg = dwc3_readl(dwc->regs, DWC3_GDBGLSP);
		seq_printf(s, "GDBGLSP[%d] = 0x%08x\n", i, reg);
	}
}

static int dwc3_lsp_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	unsigned int		current_mode;
	unsigned long		flags;
	u32			reg;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_GSTS);
	current_mode = DWC3_GSTS_CURMOD(reg);

	switch (current_mode) {
	case DWC3_GSTS_CURMOD_HOST:
		dwc3_host_lsp(s);
		break;
	case DWC3_GSTS_CURMOD_DEVICE:
		dwc3_gadget_lsp(s);
		break;
	default:
		seq_puts(s, "Mode is unknown, no LSP register printed\n");
		break;
	}
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_lsp_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_lsp_show, inode->i_private);
}

static ssize_t dwc3_lsp_write(struct file *file, const char __user *ubuf,
			      size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	char			buf[32] = { 0 };
	u32			sel;
	int			ret;

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	ret = kstrtouint(buf, 0, &sel);
	if (ret)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	dwc->dbg_lsp_select = sel;
	spin_unlock_irqrestore(&dwc->lock, flags);

	return count;
}

static const struct file_operations dwc3_lsp_fops = {
	.open			= dwc3_lsp_open,
	.write			= dwc3_lsp_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.release		= single_release,
};

static int dwc3_mode_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	u32			reg;
<<<<<<< HEAD
=======
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	spin_unlock_irqrestore(&dwc->lock, flags);

	switch (DWC3_GCTL_PRTCAP(reg)) {
	case DWC3_GCTL_PRTCAP_HOST:
<<<<<<< HEAD
		seq_printf(s, "host\n");
		break;
	case DWC3_GCTL_PRTCAP_DEVICE:
		seq_printf(s, "device\n");
		break;
	case DWC3_GCTL_PRTCAP_OTG:
		seq_printf(s, "OTG\n");
=======
		seq_puts(s, "host\n");
		break;
	case DWC3_GCTL_PRTCAP_DEVICE:
		seq_puts(s, "device\n");
		break;
	case DWC3_GCTL_PRTCAP_OTG:
		seq_puts(s, "otg\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		seq_printf(s, "UNKNOWN %08x\n", DWC3_GCTL_PRTCAP(reg));
	}

<<<<<<< HEAD
=======
	pm_runtime_put_sync(dwc->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int dwc3_mode_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_mode_show, inode->i_private);
}

static ssize_t dwc3_mode_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct dwc3		*dwc = s->private;
<<<<<<< HEAD
	unsigned long		flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32			mode = 0;
	char			buf[32];

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

<<<<<<< HEAD
	if (!strncmp(buf, "host", 4))
		mode |= DWC3_GCTL_PRTCAP_HOST;

	if (!strncmp(buf, "device", 6))
		mode |= DWC3_GCTL_PRTCAP_DEVICE;

	if (!strncmp(buf, "otg", 3))
		mode |= DWC3_GCTL_PRTCAP_OTG;

	if (mode) {
		spin_lock_irqsave(&dwc->lock, flags);
		dwc3_set_mode(dwc, mode);
		spin_unlock_irqrestore(&dwc->lock, flags);
	}
=======
	if (dwc->dr_mode != USB_DR_MODE_OTG)
		return count;

	if (!strncmp(buf, "host", 4))
		mode = DWC3_GCTL_PRTCAP_HOST;

	if (!strncmp(buf, "device", 6))
		mode = DWC3_GCTL_PRTCAP_DEVICE;

	if (!strncmp(buf, "otg", 3))
		mode = DWC3_GCTL_PRTCAP_OTG;

	dwc3_set_mode(dwc, mode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static const struct file_operations dwc3_mode_fops = {
	.open			= dwc3_mode_open,
	.write			= dwc3_mode_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int dwc3_testmode_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	u32			reg;
<<<<<<< HEAD
=======
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_DCTL);
	reg &= DWC3_DCTL_TSTCTRL_MASK;
	reg >>= 1;
	spin_unlock_irqrestore(&dwc->lock, flags);

	switch (reg) {
	case 0:
<<<<<<< HEAD
		seq_printf(s, "no test\n");
		break;
	case TEST_J:
		seq_printf(s, "test_j\n");
		break;
	case TEST_K:
		seq_printf(s, "test_k\n");
		break;
	case TEST_SE0_NAK:
		seq_printf(s, "test_se0_nak\n");
		break;
	case TEST_PACKET:
		seq_printf(s, "test_packet\n");
		break;
	case TEST_FORCE_EN:
		seq_printf(s, "test_force_enable\n");
=======
		seq_puts(s, "no test\n");
		break;
	case USB_TEST_J:
		seq_puts(s, "test_j\n");
		break;
	case USB_TEST_K:
		seq_puts(s, "test_k\n");
		break;
	case USB_TEST_SE0_NAK:
		seq_puts(s, "test_se0_nak\n");
		break;
	case USB_TEST_PACKET:
		seq_puts(s, "test_packet\n");
		break;
	case USB_TEST_FORCE_ENABLE:
		seq_puts(s, "test_force_enable\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		seq_printf(s, "UNKNOWN %d\n", reg);
	}

<<<<<<< HEAD
=======
	pm_runtime_put_sync(dwc->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int dwc3_testmode_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_testmode_show, inode->i_private);
}

static ssize_t dwc3_testmode_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	u32			testmode = 0;
	char			buf[32];
<<<<<<< HEAD
=======
	int			ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "test_j", 6))
<<<<<<< HEAD
		testmode = TEST_J;
	else if (!strncmp(buf, "test_k", 6))
		testmode = TEST_K;
	else if (!strncmp(buf, "test_se0_nak", 12))
		testmode = TEST_SE0_NAK;
	else if (!strncmp(buf, "test_packet", 11))
		testmode = TEST_PACKET;
	else if (!strncmp(buf, "test_force_enable", 17))
		testmode = TEST_FORCE_EN;
	else
		testmode = 0;

=======
		testmode = USB_TEST_J;
	else if (!strncmp(buf, "test_k", 6))
		testmode = USB_TEST_K;
	else if (!strncmp(buf, "test_se0_nak", 12))
		testmode = USB_TEST_SE0_NAK;
	else if (!strncmp(buf, "test_packet", 11))
		testmode = USB_TEST_PACKET;
	else if (!strncmp(buf, "test_force_enable", 17))
		testmode = USB_TEST_FORCE_ENABLE;
	else
		testmode = 0;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock_irqsave(&dwc->lock, flags);
	dwc3_gadget_set_test_mode(dwc, testmode);
	spin_unlock_irqrestore(&dwc->lock, flags);

<<<<<<< HEAD
=======
	pm_runtime_put_sync(dwc->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static const struct file_operations dwc3_testmode_fops = {
	.open			= dwc3_testmode_open,
	.write			= dwc3_testmode_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int dwc3_link_state_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	enum dwc3_link_state	state;
	u32			reg;
<<<<<<< HEAD

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_DSTS);
	state = DWC3_DSTS_USBLNKST(reg);
	spin_unlock_irqrestore(&dwc->lock, flags);

	switch (state) {
	case DWC3_LINK_STATE_U0:
		seq_printf(s, "U0\n");
		break;
	case DWC3_LINK_STATE_U1:
		seq_printf(s, "U1\n");
		break;
	case DWC3_LINK_STATE_U2:
		seq_printf(s, "U2\n");
		break;
	case DWC3_LINK_STATE_U3:
		seq_printf(s, "U3\n");
		break;
	case DWC3_LINK_STATE_SS_DIS:
		seq_printf(s, "SS.Disabled\n");
		break;
	case DWC3_LINK_STATE_RX_DET:
		seq_printf(s, "Rx.Detect\n");
		break;
	case DWC3_LINK_STATE_SS_INACT:
		seq_printf(s, "SS.Inactive\n");
		break;
	case DWC3_LINK_STATE_POLL:
		seq_printf(s, "Poll\n");
		break;
	case DWC3_LINK_STATE_RECOV:
		seq_printf(s, "Recovery\n");
		break;
	case DWC3_LINK_STATE_HRESET:
		seq_printf(s, "HRESET\n");
		break;
	case DWC3_LINK_STATE_CMPLY:
		seq_printf(s, "Compliance\n");
		break;
	case DWC3_LINK_STATE_LPBK:
		seq_printf(s, "Loopback\n");
		break;
	default:
		seq_printf(s, "UNKNOWN %d\n", reg);
	}
=======
	u8			speed;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_GSTS);
	if (DWC3_GSTS_CURMOD(reg) != DWC3_GSTS_CURMOD_DEVICE) {
		seq_puts(s, "Not available\n");
		spin_unlock_irqrestore(&dwc->lock, flags);
		pm_runtime_put_sync(dwc->dev);
		return 0;
	}

	reg = dwc3_readl(dwc->regs, DWC3_DSTS);
	state = DWC3_DSTS_USBLNKST(reg);
	speed = reg & DWC3_DSTS_CONNECTSPD;

	seq_printf(s, "%s\n", (speed >= DWC3_DSTS_SUPERSPEED) ?
		   dwc3_gadget_link_string(state) :
		   dwc3_gadget_hs_link_string(state));
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int dwc3_link_state_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_link_state_show, inode->i_private);
}

static ssize_t dwc3_link_state_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct dwc3		*dwc = s->private;
	unsigned long		flags;
	enum dwc3_link_state	state = 0;
	char			buf[32];
<<<<<<< HEAD
=======
	u32			reg;
	u8			speed;
	int			ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "SS.Disabled", 11))
		state = DWC3_LINK_STATE_SS_DIS;
	else if (!strncmp(buf, "Rx.Detect", 9))
		state = DWC3_LINK_STATE_RX_DET;
	else if (!strncmp(buf, "SS.Inactive", 11))
		state = DWC3_LINK_STATE_SS_INACT;
	else if (!strncmp(buf, "Recovery", 8))
		state = DWC3_LINK_STATE_RECOV;
	else if (!strncmp(buf, "Compliance", 10))
		state = DWC3_LINK_STATE_CMPLY;
	else if (!strncmp(buf, "Loopback", 8))
		state = DWC3_LINK_STATE_LPBK;
	else
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&dwc->lock, flags);
	dwc3_gadget_set_link_state(dwc, state);
	spin_unlock_irqrestore(&dwc->lock, flags);

=======
	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_GSTS);
	if (DWC3_GSTS_CURMOD(reg) != DWC3_GSTS_CURMOD_DEVICE) {
		spin_unlock_irqrestore(&dwc->lock, flags);
		pm_runtime_put_sync(dwc->dev);
		return -EINVAL;
	}

	reg = dwc3_readl(dwc->regs, DWC3_DSTS);
	speed = reg & DWC3_DSTS_CONNECTSPD;

	if (speed < DWC3_DSTS_SUPERSPEED &&
	    state != DWC3_LINK_STATE_RECOV) {
		spin_unlock_irqrestore(&dwc->lock, flags);
		pm_runtime_put_sync(dwc->dev);
		return -EINVAL;
	}

	dwc3_gadget_set_link_state(dwc, state);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static const struct file_operations dwc3_link_state_fops = {
	.open			= dwc3_link_state_open,
	.write			= dwc3_link_state_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

<<<<<<< HEAD
static int ep_num;
static ssize_t dwc3_store_ep_num(struct file *file, const char __user *ubuf,
				 size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct dwc3		*dwc = s->private;
	char			kbuf[10];
	unsigned int		num, dir, temp;
	unsigned long		flags;

	memset(kbuf, 0, 10);

	if (copy_from_user(kbuf, ubuf, count > 10 ? 10 : count))
		return -EFAULT;

	if (sscanf(kbuf, "%u %u", &num, &dir) != 2)
		return -EINVAL;

	if (dir != 0 && dir != 1)
		return -EINVAL;

	temp = (num << 1) + dir;
	if (temp >= DWC3_ENDPOINTS_NUM)
		return -EINVAL;

	spin_lock_irqsave(&dwc->lock, flags);
	ep_num = temp;
	spin_unlock_irqrestore(&dwc->lock, flags);

	return count;
}

static int dwc3_ep_req_list_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	struct dwc3_ep		*dep;
	struct dwc3_request	*req = NULL;
	struct list_head	*ptr = NULL;
	unsigned long		flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dep = dwc->eps[ep_num];

	seq_printf(s, "%s request list: flags: 0x%x\n", dep->name, dep->flags);
	list_for_each(ptr, &dep->request_list) {
		req = list_entry(ptr, struct dwc3_request, list);

		seq_printf(s,
			"req:0x%p len: %d sts: %d dma:0x%pa num_sgs: %d\n",
			req, req->request.length, req->request.status,
			&req->request.dma, req->request.num_sgs);
	}
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static int dwc3_ep_req_list_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_ep_req_list_show, inode->i_private);
}

static const struct file_operations dwc3_ep_req_list_fops = {
	.open			= dwc3_ep_req_list_open,
	.write			= dwc3_store_ep_num,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int dwc3_ep_queued_req_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	struct dwc3_ep		*dep;
	struct dwc3_request	*req = NULL;
	struct list_head	*ptr = NULL;
	unsigned long		flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dep = dwc->eps[ep_num];

	seq_printf(s, "%s queued reqs to HW: flags:0x%x\n", dep->name,
								dep->flags);
	list_for_each(ptr, &dep->req_queued) {
		req = list_entry(ptr, struct dwc3_request, list);

		seq_printf(s,
			"req:0x%p len:%d sts:%d dma:%pa nsg:%d trb:0x%p\n",
			req, req->request.length, req->request.status,
			&req->request.dma, req->request.num_sgs, req->trb);
	}
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static int dwc3_ep_queued_req_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_ep_queued_req_show, inode->i_private);
}

const struct file_operations dwc3_ep_req_queued_fops = {
	.open			= dwc3_ep_queued_req_open,
	.write			= dwc3_store_ep_num,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int dwc3_ep_trbs_show(struct seq_file *s, void *unused)
{
	struct dwc3		*dwc = s->private;
	struct dwc3_ep		*dep;
	struct dwc3_trb		*trb;
	unsigned long		flags;
	int			j;

	if (!ep_num)
		return 0;

	spin_lock_irqsave(&dwc->lock, flags);
	dep = dwc->eps[ep_num];

	seq_printf(s, "%s trb pool: flags:0x%x freeslot:%d busyslot:%d\n",
		dep->name, dep->flags, dep->free_slot, dep->busy_slot);
	for (j = 0; j < DWC3_TRB_NUM; j++) {
		trb = &dep->trb_pool[j];
		seq_printf(s, "trb:0x%p bph:0x%x bpl:0x%x size:0x%x ctrl: %x\n",
			trb, trb->bph, trb->bpl, trb->size, trb->ctrl);
	}
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static int dwc3_ep_trbs_list_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_ep_trbs_show, inode->i_private);
}

const struct file_operations dwc3_ep_trb_list_fops = {
	.open			= dwc3_ep_trbs_list_open,
	.write			= dwc3_store_ep_num,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static unsigned int ep_addr_rxdbg_mask = 1;
module_param(ep_addr_rxdbg_mask, uint, S_IRUGO | S_IWUSR);
static unsigned int ep_addr_txdbg_mask = 1;
module_param(ep_addr_txdbg_mask, uint, S_IRUGO | S_IWUSR);

/* Maximum debug message length */
#define DBG_DATA_MSG   64UL

/* Maximum number of messages */
#define DBG_DATA_MAX   128UL

static struct {
	char     (buf[DBG_DATA_MAX])[DBG_DATA_MSG];   /* buffer */
	unsigned idx;   /* index */
	unsigned tty;   /* print to console? */
	rwlock_t lck;   /* lock */
} dbg_dwc3_data = {
	.idx = 0,
	.tty = 0,
	.lck = __RW_LOCK_UNLOCKED(lck)
};

/**
 * dbg_dec: decrements debug event index
 * @idx: buffer index
 */
static inline void __maybe_unused dbg_dec(unsigned *idx)
{
	*idx = (*idx - 1) % DBG_DATA_MAX;
}

/**
 * dbg_inc: increments debug event index
 * @idx: buffer index
 */
static inline void dbg_inc(unsigned *idx)
{
	*idx = (*idx + 1) % DBG_DATA_MAX;
}

#define TIME_BUF_LEN  20
/*get_timestamp - returns time of day in us */
static char *get_timestamp(char *tbuf)
{
	unsigned long long t;
	unsigned long nanosec_rem;

	t = cpu_clock(smp_processor_id());
	nanosec_rem = do_div(t, 1000000000)/1000;
	scnprintf(tbuf, TIME_BUF_LEN, "[%5lu.%06lu] ", (unsigned long)t,
		nanosec_rem);
	return tbuf;
}

static int allow_dbg_print(u8 ep_num)
{
	int dir, num;

	/* allow bus wide events */
	if (ep_num == 0xff)
		return 1;

	dir = ep_num & 0x1;
	num = ep_num >> 1;
	num = 1 << num;

	if (dir && (num & ep_addr_txdbg_mask))
		return 1;
	if (!dir && (num & ep_addr_rxdbg_mask))
		return 1;

	return 0;
}

/**
 * dbg_print:  prints the common part of the event
 * @addr:   endpoint address
 * @name:   event name
 * @status: status
 * @extra:  extra information
 */
void dbg_print(u8 ep_num, const char *name, int status, const char *extra)
{
	unsigned long flags;
	char tbuf[TIME_BUF_LEN];

	if (!allow_dbg_print(ep_num))
		return;

	write_lock_irqsave(&dbg_dwc3_data.lck, flags);

	scnprintf(dbg_dwc3_data.buf[dbg_dwc3_data.idx], DBG_DATA_MSG,
		  "%s\t? %02X %-7.7s %4i ?\t%s\n",
		  get_timestamp(tbuf), ep_num, name, status, extra);

	dbg_inc(&dbg_dwc3_data.idx);

	write_unlock_irqrestore(&dbg_dwc3_data.lck, flags);

	if (dbg_dwc3_data.tty != 0)
		pr_notice("%s\t? %02X %-7.7s %4i ?\t%s\n",
			  get_timestamp(tbuf), ep_num, name, status, extra);
}

/**
 * dbg_done: prints a DONE event
 * @addr:   endpoint address
 * @td:     transfer descriptor
 * @status: status
 */
void dbg_done(u8 ep_num, const u32 count, int status)
{
	char msg[DBG_DATA_MSG];

	if (!allow_dbg_print(ep_num))
		return;

	scnprintf(msg, sizeof(msg), "%d", count);
	dbg_print(ep_num, "DONE", status, msg);
}

/**
 * dbg_event: prints a generic event
 * @addr:   endpoint address
 * @name:   event name
 * @status: status
 */
void dbg_event(u8 ep_num, const char *name, int status)
{
	if (!allow_dbg_print(ep_num))
		return;

	if (name != NULL)
		dbg_print(ep_num, name, status, "");
}

/*
 * dbg_queue: prints a QUEUE event
 * @addr:   endpoint address
 * @req:    USB request
 * @status: status
 */
void dbg_queue(u8 ep_num, const struct usb_request *req, int status)
{
	char msg[DBG_DATA_MSG];

	if (!allow_dbg_print(ep_num))
		return;

	if (req != NULL) {
		scnprintf(msg, sizeof(msg),
			  "%d %d", !req->no_interrupt, req->length);
		dbg_print(ep_num, "QUEUE", status, msg);
	}
}

/**
 * dbg_setup: prints a SETUP event
 * @addr: endpoint address
 * @req:  setup request
 */
void dbg_setup(u8 ep_num, const struct usb_ctrlrequest *req)
{
	char msg[DBG_DATA_MSG];

	if (!allow_dbg_print(ep_num))
		return;

	if (req != NULL) {
		scnprintf(msg, sizeof(msg),
			  "%02X %02X %04X %04X %d", req->bRequestType,
			  req->bRequest, le16_to_cpu(req->wValue),
			  le16_to_cpu(req->wIndex), le16_to_cpu(req->wLength));
		dbg_print(ep_num, "SETUP", 0, msg);
	}
}

/**
 * dbg_print_reg: prints a reg value
 * @name:   reg name
 * @reg: reg value to be printed
 */
void dbg_print_reg(const char *name, int reg)
{
	unsigned long flags;

	write_lock_irqsave(&dbg_dwc3_data.lck, flags);

	scnprintf(dbg_dwc3_data.buf[dbg_dwc3_data.idx], DBG_DATA_MSG,
		  "%s = 0x%08x\n", name, reg);

	dbg_inc(&dbg_dwc3_data.idx);

	write_unlock_irqrestore(&dbg_dwc3_data.lck, flags);

	if (dbg_dwc3_data.tty != 0)
		pr_notice("%s = 0x%08x\n", name, reg);
}

/**
 * store_events: configure if events are going to be also printed to console
 *
 */
static ssize_t dwc3_store_events(struct file *file,
			    const char __user *buf, size_t count, loff_t *ppos)
{
	unsigned tty;

	if (buf == NULL) {
		pr_err("[%s] EINVAL\n", __func__);
		goto done;
	}

	if (sscanf(buf, "%u", &tty) != 1 || tty > 1) {
		pr_err("<1|0>: enable|disable console log\n");
		goto done;
	}

	dbg_dwc3_data.tty = tty;
	pr_info("tty = %u", dbg_dwc3_data.tty);

 done:
	return count;
}

static int dwc3_gadget_data_events_show(struct seq_file *s, void *unused)
{
	unsigned long	flags;
	unsigned	i;

	read_lock_irqsave(&dbg_dwc3_data.lck, flags);

	i = dbg_dwc3_data.idx;
	if (strnlen(dbg_dwc3_data.buf[i], DBG_DATA_MSG))
		seq_printf(s, "%s\n", dbg_dwc3_data.buf[i]);
	for (dbg_inc(&i); i != dbg_dwc3_data.idx; dbg_inc(&i)) {
		if (!strnlen(dbg_dwc3_data.buf[i], DBG_DATA_MSG))
			continue;
		seq_printf(s, "%s\n", dbg_dwc3_data.buf[i]);
	}

	read_unlock_irqrestore(&dbg_dwc3_data.lck, flags);

	return 0;
}

static int dwc3_gadget_data_events_open(struct inode *inode, struct file *f)
{
	return single_open(f, dwc3_gadget_data_events_show, inode->i_private);
}

const struct file_operations dwc3_gadget_dbg_data_fops = {
	.open			= dwc3_gadget_data_events_open,
	.read			= seq_read,
	.write			= dwc3_store_events,
	.llseek			= seq_lseek,
	.release		= single_release,
};

int __devinit dwc3_debugfs_init(struct dwc3 *dwc)
{
	struct dentry		*root;
	struct dentry		*file;
	int			ret;

	root = debugfs_create_dir(dev_name(dwc->dev), NULL);
	if (!root) {
		ret = -ENOMEM;
		goto err0;
	}

	dwc->root = root;

	file = debugfs_create_file("regdump", S_IRUGO, root, dwc,
			&dwc3_regdump_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("mode", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_mode_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("testmode", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_testmode_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("link_state", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_link_state_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("trbs", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_ep_trb_list_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("requests", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_ep_req_list_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("queued_reqs", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_ep_req_queued_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}

	file = debugfs_create_file("events", S_IRUGO | S_IWUSR, root,
			dwc, &dwc3_gadget_dbg_data_fops);
	if (!file) {
		ret = -ENOMEM;
		goto err1;
	}
	return 0;

err1:
	debugfs_remove_recursive(root);

err0:
	return ret;
}

void __devexit dwc3_debugfs_exit(struct dwc3 *dwc)
{
	debugfs_remove_recursive(dwc->root);
	dwc->root = NULL;
=======
struct dwc3_ep_file_map {
	const char name[25];
	const struct file_operations *const fops;
};

static int dwc3_tx_fifo_size_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			mdwidth;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_TXFIFO);

	/* Convert to bytes */
	mdwidth = dwc3_mdwidth(dwc);

	val *= mdwidth;
	val >>= 3;
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_rx_fifo_size_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			mdwidth;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_RXFIFO);

	/* Convert to bytes */
	mdwidth = dwc3_mdwidth(dwc);

	val *= mdwidth;
	val >>= 3;
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_tx_request_queue_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_TXREQQ);
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_rx_request_queue_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_RXREQQ);
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_rx_info_queue_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_RXINFOQ);
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_descriptor_fetch_queue_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_DESCFETCHQ);
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_event_queue_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u32			val;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	val = dwc3_core_fifo_space(dep, DWC3_EVENTQ);
	seq_printf(s, "%u\n", val);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_transfer_type_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;

	spin_lock_irqsave(&dwc->lock, flags);
	if (!(dep->flags & DWC3_EP_ENABLED) || !dep->endpoint.desc) {
		seq_puts(s, "--\n");
		goto out;
	}

	switch (usb_endpoint_type(dep->endpoint.desc)) {
	case USB_ENDPOINT_XFER_CONTROL:
		seq_puts(s, "control\n");
		break;
	case USB_ENDPOINT_XFER_ISOC:
		seq_puts(s, "isochronous\n");
		break;
	case USB_ENDPOINT_XFER_BULK:
		seq_puts(s, "bulk\n");
		break;
	case USB_ENDPOINT_XFER_INT:
		seq_puts(s, "interrupt\n");
		break;
	default:
		seq_puts(s, "--\n");
	}

out:
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static int dwc3_trb_ring_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	int			i;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	if (dep->number <= 1) {
		seq_puts(s, "--\n");
		goto out;
	}

	seq_puts(s, "buffer_addr,size,type,ioc,isp_imi,csp,chn,lst,hwo\n");

	for (i = 0; i < DWC3_TRB_NUM; i++) {
		struct dwc3_trb *trb = &dep->trb_pool[i];
		unsigned int type = DWC3_TRBCTL_TYPE(trb->ctrl);

		seq_printf(s, "%08x%08x,%d,%s,%d,%d,%d,%d,%d,%d       %c%c\n",
				trb->bph, trb->bpl, trb->size,
				dwc3_trb_type_string(type),
				!!(trb->ctrl & DWC3_TRB_CTRL_IOC),
				!!(trb->ctrl & DWC3_TRB_CTRL_ISP_IMI),
				!!(trb->ctrl & DWC3_TRB_CTRL_CSP),
				!!(trb->ctrl & DWC3_TRB_CTRL_CHN),
				!!(trb->ctrl & DWC3_TRB_CTRL_LST),
				!!(trb->ctrl & DWC3_TRB_CTRL_HWO),
				dep->trb_enqueue == i ? 'E' : ' ',
				dep->trb_dequeue == i ? 'D' : ' ');
	}

out:
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

static int dwc3_ep_info_register_show(struct seq_file *s, void *unused)
{
	struct dwc3_ep		*dep = s->private;
	struct dwc3		*dwc = dep->dwc;
	unsigned long		flags;
	u64			ep_info;
	u32			lower_32_bits;
	u32			upper_32_bits;
	u32			reg;
	int			ret;

	ret = pm_runtime_resume_and_get(dwc->dev);
	if (ret < 0)
		return ret;

	spin_lock_irqsave(&dwc->lock, flags);
	reg = DWC3_GDBGLSPMUX_EPSELECT(dep->number);
	dwc3_writel(dwc->regs, DWC3_GDBGLSPMUX, reg);

	lower_32_bits = dwc3_readl(dwc->regs, DWC3_GDBGEPINFO0);
	upper_32_bits = dwc3_readl(dwc->regs, DWC3_GDBGEPINFO1);

	ep_info = ((u64)upper_32_bits << 32) | lower_32_bits;
	seq_printf(s, "0x%016llx\n", ep_info);
	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_put_sync(dwc->dev);

	return 0;
}

DEFINE_SHOW_ATTRIBUTE(dwc3_tx_fifo_size);
DEFINE_SHOW_ATTRIBUTE(dwc3_rx_fifo_size);
DEFINE_SHOW_ATTRIBUTE(dwc3_tx_request_queue);
DEFINE_SHOW_ATTRIBUTE(dwc3_rx_request_queue);
DEFINE_SHOW_ATTRIBUTE(dwc3_rx_info_queue);
DEFINE_SHOW_ATTRIBUTE(dwc3_descriptor_fetch_queue);
DEFINE_SHOW_ATTRIBUTE(dwc3_event_queue);
DEFINE_SHOW_ATTRIBUTE(dwc3_transfer_type);
DEFINE_SHOW_ATTRIBUTE(dwc3_trb_ring);
DEFINE_SHOW_ATTRIBUTE(dwc3_ep_info_register);

static const struct dwc3_ep_file_map dwc3_ep_file_map[] = {
	{ "tx_fifo_size", &dwc3_tx_fifo_size_fops, },
	{ "rx_fifo_size", &dwc3_rx_fifo_size_fops, },
	{ "tx_request_queue", &dwc3_tx_request_queue_fops, },
	{ "rx_request_queue", &dwc3_rx_request_queue_fops, },
	{ "rx_info_queue", &dwc3_rx_info_queue_fops, },
	{ "descriptor_fetch_queue", &dwc3_descriptor_fetch_queue_fops, },
	{ "event_queue", &dwc3_event_queue_fops, },
	{ "transfer_type", &dwc3_transfer_type_fops, },
	{ "trb_ring", &dwc3_trb_ring_fops, },
	{ "GDBGEPINFO", &dwc3_ep_info_register_fops, },
};

void dwc3_debugfs_create_endpoint_dir(struct dwc3_ep *dep)
{
	struct dentry		*dir;
	int			i;

	dir = debugfs_create_dir(dep->name, dep->dwc->debug_root);
	for (i = 0; i < ARRAY_SIZE(dwc3_ep_file_map); i++) {
		const struct file_operations *fops = dwc3_ep_file_map[i].fops;
		const char *name = dwc3_ep_file_map[i].name;

		debugfs_create_file(name, 0444, dir, dep, fops);
	}
}

void dwc3_debugfs_remove_endpoint_dir(struct dwc3_ep *dep)
{
	debugfs_lookup_and_remove(dep->name, dep->dwc->debug_root);
}

void dwc3_debugfs_init(struct dwc3 *dwc)
{
	struct dentry		*root;

	dwc->regset = kzalloc(sizeof(*dwc->regset), GFP_KERNEL);
	if (!dwc->regset)
		return;

	dwc->dbg_lsp_select = DWC3_LSP_MUX_UNSELECTED;

	dwc->regset->regs = dwc3_regs;
	dwc->regset->nregs = ARRAY_SIZE(dwc3_regs);
	dwc->regset->base = dwc->regs - DWC3_GLOBALS_REGS_START;
	dwc->regset->dev = dwc->dev;

	root = debugfs_create_dir(dev_name(dwc->dev), usb_debug_root);
	dwc->debug_root = root;
	debugfs_create_regset32("regdump", 0444, root, dwc->regset);
	debugfs_create_file("lsp_dump", 0644, root, dwc, &dwc3_lsp_fops);

	if (IS_ENABLED(CONFIG_USB_DWC3_DUAL_ROLE))
		debugfs_create_file("mode", 0644, root, dwc,
				    &dwc3_mode_fops);

	if (IS_ENABLED(CONFIG_USB_DWC3_DUAL_ROLE) ||
			IS_ENABLED(CONFIG_USB_DWC3_GADGET)) {
		debugfs_create_file("testmode", 0644, root, dwc,
				&dwc3_testmode_fops);
		debugfs_create_file("link_state", 0644, root, dwc,
				    &dwc3_link_state_fops);
	}
}

void dwc3_debugfs_exit(struct dwc3 *dwc)
{
	debugfs_lookup_and_remove(dev_name(dwc->dev), usb_debug_root);
	kfree(dwc->regset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
