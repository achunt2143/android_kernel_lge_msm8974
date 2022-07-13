<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef BCM63XX_BOARD_H_
#define BCM63XX_BOARD_H_

const char *board_get_name(void);

void board_prom_init(void);

void board_setup(void);

int board_register_devices(void);

#endif /* ! BCM63XX_BOARD_H_ */
