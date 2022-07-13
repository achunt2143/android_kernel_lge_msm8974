<<<<<<< HEAD
#ifndef __GEF_PIC_H__
#define __GEF_PIC_H__

#include <linux/init.h>

void gef_pic_cascade(unsigned int, struct irq_desc *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __GEF_PIC_H__
#define __GEF_PIC_H__

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unsigned int gef_pic_get_irq(void);
void gef_pic_init(struct device_node *);

#endif /* __GEF_PIC_H__ */

