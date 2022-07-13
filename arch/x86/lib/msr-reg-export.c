<<<<<<< HEAD
#include <linux/module.h>
#include <asm/msr.h>

EXPORT_SYMBOL(native_rdmsr_safe_regs);
EXPORT_SYMBOL(native_wrmsr_safe_regs);
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/export.h>
#include <asm/msr.h>

EXPORT_SYMBOL(rdmsr_safe_regs);
EXPORT_SYMBOL(wrmsr_safe_regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
