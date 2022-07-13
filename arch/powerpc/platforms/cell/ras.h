<<<<<<< HEAD
#ifndef RAS_H
#define RAS_H

extern void cbe_system_error_exception(struct pt_regs *regs);
extern void cbe_maintenance_exception(struct pt_regs *regs);
extern void cbe_thermal_exception(struct pt_regs *regs);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RAS_H
#define RAS_H

#include <asm/interrupt.h>

DECLARE_INTERRUPT_HANDLER(cbe_system_error_exception);
DECLARE_INTERRUPT_HANDLER(cbe_maintenance_exception);
DECLARE_INTERRUPT_HANDLER(cbe_thermal_exception);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void cbe_ras_init(void);

#endif /* RAS_H */
