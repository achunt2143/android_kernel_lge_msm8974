<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/mmc/core/sdio_bus.h
 *
 *  Copyright 2007 Pierre Ossman
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _MMC_CORE_SDIO_BUS_H
#define _MMC_CORE_SDIO_BUS_H

<<<<<<< HEAD
=======
struct mmc_card;
struct sdio_func;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct sdio_func *sdio_alloc_func(struct mmc_card *card);
int sdio_add_func(struct sdio_func *func);
void sdio_remove_func(struct sdio_func *func);

int sdio_register_bus(void);
void sdio_unregister_bus(void);

#endif

