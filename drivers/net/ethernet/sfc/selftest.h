<<<<<<< HEAD
/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2010 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/****************************************************************************
 * Driver for Solarflare network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2012 Solarflare Communications Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef EFX_SELFTEST_H
#define EFX_SELFTEST_H

#include "net_driver.h"

/*
 * Self tests
 */

struct efx_loopback_self_tests {
<<<<<<< HEAD
	int tx_sent[EFX_TXQ_TYPES];
	int tx_done[EFX_TXQ_TYPES];
=======
	int tx_sent[EFX_MAX_TXQ_PER_CHANNEL];
	int tx_done[EFX_MAX_TXQ_PER_CHANNEL];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rx_good;
	int rx_bad;
};

#define EFX_MAX_PHY_TESTS 20

/* Efx self test results
 * For fields which are not counters, 1 indicates success and -1
<<<<<<< HEAD
 * indicates failure.
=======
 * indicates failure; 0 indicates test could not be run.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct efx_self_tests {
	/* online tests */
	int phy_alive;
	int nvram;
	int interrupt;
	int eventq_dma[EFX_MAX_CHANNELS];
	int eventq_int[EFX_MAX_CHANNELS];
	/* offline tests */
<<<<<<< HEAD
=======
	int memory;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int registers;
	int phy_ext[EFX_MAX_PHY_TESTS];
	struct efx_loopback_self_tests loopback[LOOPBACK_TEST_MAX + 1];
};

<<<<<<< HEAD
extern void efx_loopback_rx_packet(struct efx_nic *efx,
				   const char *buf_ptr, int pkt_len);
extern int efx_selftest(struct efx_nic *efx,
			struct efx_self_tests *tests,
			unsigned flags);
extern void efx_selftest_async_start(struct efx_nic *efx);
extern void efx_selftest_async_cancel(struct efx_nic *efx);
extern void efx_selftest_async_work(struct work_struct *data);
=======
void efx_loopback_rx_packet(struct efx_nic *efx, const char *buf_ptr,
			    int pkt_len);
int efx_selftest(struct efx_nic *efx, struct efx_self_tests *tests,
		 unsigned flags);
void efx_selftest_async_init(struct efx_nic *efx);
void efx_selftest_async_start(struct efx_nic *efx);
void efx_selftest_async_cancel(struct efx_nic *efx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* EFX_SELFTEST_H */
