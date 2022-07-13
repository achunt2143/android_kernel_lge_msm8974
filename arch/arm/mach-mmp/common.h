<<<<<<< HEAD
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

struct sys_timer;

extern void timer_init(int irq);

extern void __init icu_init_irq(void);
extern void __init mmp_map_io(void);
extern void mmp_restart(char, const char *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/reboot.h>

extern void mmp_timer_init(int irq, unsigned long rate);

extern void __init mmp_map_io(void);
extern void __init mmp2_map_io(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
