<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/mmc/core/mmc_ops.h
 *
 *  Copyright 2006-2007 Pierre Ossman
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _MMC_MMC_OPS_H
#define _MMC_MMC_OPS_H

<<<<<<< HEAD
int mmc_select_card(struct mmc_card *card);
int mmc_deselect_cards(struct mmc_host *host);
int mmc_go_idle(struct mmc_host *host);
int mmc_send_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr);
int mmc_all_send_cid(struct mmc_host *host, u32 *cid);
int mmc_set_relative_addr(struct mmc_card *card);
int mmc_send_csd(struct mmc_card *card, u32 *csd);
int mmc_send_ext_csd(struct mmc_card *card, u8 *ext_csd);
int mmc_send_status(struct mmc_card *card, u32 *status);
int mmc_send_cid(struct mmc_host *host, u32 *cid);
int mmc_spi_read_ocr(struct mmc_host *host, int highcap, u32 *ocrp);
int mmc_spi_set_crc(struct mmc_host *host, int use_crc);
int mmc_card_sleepawake(struct mmc_host *host, int sleep);
int mmc_bus_test(struct mmc_card *card, u8 bus_width);
int mmc_send_hpi_cmd(struct mmc_card *card, u32 *status);
=======
#include <linux/types.h>

enum mmc_busy_cmd {
	MMC_BUSY_CMD6,
	MMC_BUSY_ERASE,
	MMC_BUSY_HPI,
	MMC_BUSY_EXTR_SINGLE,
	MMC_BUSY_IO,
};

struct mmc_host;
struct mmc_card;
struct mmc_command;

int mmc_select_card(struct mmc_card *card);
int mmc_deselect_cards(struct mmc_host *host);
int mmc_set_dsr(struct mmc_host *host);
int mmc_go_idle(struct mmc_host *host);
int mmc_send_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr);
int mmc_set_relative_addr(struct mmc_card *card);
int mmc_send_adtc_data(struct mmc_card *card, struct mmc_host *host, u32 opcode,
		       u32 args, void *buf, unsigned len);
int mmc_send_csd(struct mmc_card *card, u32 *csd);
int __mmc_send_status(struct mmc_card *card, u32 *status, unsigned int retries);
int mmc_send_cid(struct mmc_host *host, u32 *cid);
int mmc_spi_read_ocr(struct mmc_host *host, int highcap, u32 *ocrp);
int mmc_spi_set_crc(struct mmc_host *host, int use_crc);
int mmc_bus_test(struct mmc_card *card, u8 bus_width);
int mmc_can_ext_csd(struct mmc_card *card);
int mmc_switch_status(struct mmc_card *card, bool crc_err_fatal);
bool mmc_prepare_busy_cmd(struct mmc_host *host, struct mmc_command *cmd,
			  unsigned int timeout_ms);
int __mmc_poll_for_busy(struct mmc_host *host, unsigned int period_us,
			unsigned int timeout_ms,
			int (*busy_cb)(void *cb_data, bool *busy),
			void *cb_data);
int mmc_poll_for_busy(struct mmc_card *card, unsigned int timeout_ms,
		      bool retry_crc_err, enum mmc_busy_cmd busy_cmd);
int __mmc_switch(struct mmc_card *card, u8 set, u8 index, u8 value,
		unsigned int timeout_ms, unsigned char timing,
		bool send_status, bool retry_crc_err, unsigned int retries);
int mmc_switch(struct mmc_card *card, u8 set, u8 index, u8 value,
		unsigned int timeout_ms);
void mmc_run_bkops(struct mmc_card *card);
int mmc_cmdq_enable(struct mmc_card *card);
int mmc_cmdq_disable(struct mmc_card *card);
int mmc_sanitize(struct mmc_card *card, unsigned int timeout_ms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

