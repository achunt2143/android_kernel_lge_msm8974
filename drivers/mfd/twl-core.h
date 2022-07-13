<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __TWL_CORE_H__
#define __TWL_CORE_H__

extern int twl6030_init_irq(struct device *dev, int irq_num);
<<<<<<< HEAD
extern int twl6030_exit_irq(void);
extern int twl4030_init_irq(struct device *dev, int irq_num);
extern int twl4030_exit_irq(void);
=======
extern void twl6030_exit_irq(void);
extern int twl4030_init_irq(struct device *dev, int irq_num);
extern void twl4030_exit_irq(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int twl4030_init_chip_irq(const char *chip);

#endif /*  __TWL_CORE_H__ */
