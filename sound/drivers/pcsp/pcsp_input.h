<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PC-Speaker driver for Linux
 *
 * Copyright (C) 2001-2008  Stas Sergeev
 */

#ifndef __PCSP_INPUT_H__
#define __PCSP_INPUT_H__

<<<<<<< HEAD
int __devinit pcspkr_input_init(struct input_dev **rdev, struct device *dev);
int pcspkr_input_remove(struct input_dev *dev);
=======
int pcspkr_input_init(struct input_dev **rdev, struct device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void pcspkr_stop_sound(void);

#endif
