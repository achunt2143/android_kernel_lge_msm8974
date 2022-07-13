/*
 * Copyright (c) 2008-2011 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <asm/unaligned.h>

#include "ath9k.h"

#define REG_WRITE_D(_ah, _reg, _val) \
	ath9k_hw_common(_ah)->ops->write((_ah), (_val), (_reg))
#define REG_READ_D(_ah, _reg) \
	ath9k_hw_common(_ah)->ops->read((_ah), (_reg))

<<<<<<< HEAD
=======
void ath9k_debug_sync_cause(struct ath_softc *sc, u32 sync_cause)
{
	if (sync_cause)
		sc->debug.stats.istats.sync_cause_all++;
	if (sync_cause & AR_INTR_SYNC_RTC_IRQ)
		sc->debug.stats.istats.sync_rtc_irq++;
	if (sync_cause & AR_INTR_SYNC_MAC_IRQ)
		sc->debug.stats.istats.sync_mac_irq++;
	if (sync_cause & AR_INTR_SYNC_EEPROM_ILLEGAL_ACCESS)
		sc->debug.stats.istats.eeprom_illegal_access++;
	if (sync_cause & AR_INTR_SYNC_APB_TIMEOUT)
		sc->debug.stats.istats.apb_timeout++;
	if (sync_cause & AR_INTR_SYNC_PCI_MODE_CONFLICT)
		sc->debug.stats.istats.pci_mode_conflict++;
	if (sync_cause & AR_INTR_SYNC_HOST1_FATAL)
		sc->debug.stats.istats.host1_fatal++;
	if (sync_cause & AR_INTR_SYNC_HOST1_PERR)
		sc->debug.stats.istats.host1_perr++;
	if (sync_cause & AR_INTR_SYNC_TRCV_FIFO_PERR)
		sc->debug.stats.istats.trcv_fifo_perr++;
	if (sync_cause & AR_INTR_SYNC_RADM_CPL_EP)
		sc->debug.stats.istats.radm_cpl_ep++;
	if (sync_cause & AR_INTR_SYNC_RADM_CPL_DLLP_ABORT)
		sc->debug.stats.istats.radm_cpl_dllp_abort++;
	if (sync_cause & AR_INTR_SYNC_RADM_CPL_TLP_ABORT)
		sc->debug.stats.istats.radm_cpl_tlp_abort++;
	if (sync_cause & AR_INTR_SYNC_RADM_CPL_ECRC_ERR)
		sc->debug.stats.istats.radm_cpl_ecrc_err++;
	if (sync_cause & AR_INTR_SYNC_RADM_CPL_TIMEOUT)
		sc->debug.stats.istats.radm_cpl_timeout++;
	if (sync_cause & AR_INTR_SYNC_LOCAL_TIMEOUT)
		sc->debug.stats.istats.local_timeout++;
	if (sync_cause & AR_INTR_SYNC_PM_ACCESS)
		sc->debug.stats.istats.pm_access++;
	if (sync_cause & AR_INTR_SYNC_MAC_AWAKE)
		sc->debug.stats.istats.mac_awake++;
	if (sync_cause & AR_INTR_SYNC_MAC_ASLEEP)
		sc->debug.stats.istats.mac_asleep++;
	if (sync_cause & AR_INTR_SYNC_MAC_SLEEP_ACCESS)
		sc->debug.stats.istats.mac_sleep_access++;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t ath9k_debugfs_read_buf(struct file *file, char __user *user_buf,
				      size_t count, loff_t *ppos)
{
	u8 *buf = file->private_data;
	return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf));
}

static int ath9k_debugfs_release_buf(struct inode *inode, struct file *file)
{
	vfree(file->private_data);
	return 0;
}

#ifdef CONFIG_ATH_DEBUG

static ssize_t read_file_debug(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "0x%08x\n", common->debug_mask);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_debug(struct file *file, const char __user *user_buf,
<<<<<<< HEAD
			     size_t count, loff_t *ppos)
=======
				size_t count, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long mask;
<<<<<<< HEAD
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &mask))
		return -EINVAL;
=======
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &mask);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	common->debug_mask = mask;
	return count;
}

static const struct file_operations fops_debug = {
	.read = read_file_debug,
	.write = write_file_debug,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

#endif

#define DMA_BUF_LEN 1024

<<<<<<< HEAD
static ssize_t read_file_tx_chainmask(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "0x%08x\n", ah->txchainmask);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_tx_chainmask(struct file *file, const char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	unsigned long mask;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &mask))
		return -EINVAL;

	ah->txchainmask = mask;
	ah->caps.tx_chainmask = mask;
	return count;
}

static const struct file_operations fops_tx_chainmask = {
	.read = read_file_tx_chainmask,
	.write = write_file_tx_chainmask,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};


static ssize_t read_file_rx_chainmask(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "0x%08x\n", ah->rxchainmask);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_rx_chainmask(struct file *file, const char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	unsigned long mask;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &mask))
		return -EINVAL;

	ah->rxchainmask = mask;
	ah->caps.rx_chainmask = mask;
	return count;
}

static const struct file_operations fops_rx_chainmask = {
	.read = read_file_rx_chainmask,
	.write = write_file_rx_chainmask,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_disable_ani(struct file *file, char __user *user_buf,
=======

static ssize_t read_file_ani(struct file *file, char __user *user_buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "%d\n", common->disable_ani);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_disable_ani(struct file *file,
				      const char __user *user_buf,
				      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long disable_ani;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &disable_ani))
		return -EINVAL;

	common->disable_ani = !!disable_ani;

	if (disable_ani) {
		sc->sc_flags &= ~SC_OP_ANI_RUN;
		del_timer_sync(&common->ani.timer);
	} else {
		sc->sc_flags |= SC_OP_ANI_RUN;
		ath_start_ani(common);
=======
	struct ath_hw *ah = sc->sc_ah;
	unsigned int len = 0;
	const unsigned int size = 1024;
	ssize_t retval = 0;
	char *buf;
	int i;
	struct {
		const char *name;
		unsigned int val;
	} ani_info[] = {
		{ "ANI RESET", ah->stats.ast_ani_reset },
		{ "OFDM LEVEL", ah->ani.ofdmNoiseImmunityLevel },
		{ "CCK LEVEL", ah->ani.cckNoiseImmunityLevel },
		{ "SPUR UP", ah->stats.ast_ani_spurup },
		{ "SPUR DOWN", ah->stats.ast_ani_spurdown },
		{ "OFDM WS-DET ON", ah->stats.ast_ani_ofdmon },
		{ "OFDM WS-DET OFF", ah->stats.ast_ani_ofdmoff },
		{ "MRC-CCK ON", ah->stats.ast_ani_ccklow },
		{ "MRC-CCK OFF", ah->stats.ast_ani_cckhigh },
		{ "FIR-STEP UP", ah->stats.ast_ani_stepup },
		{ "FIR-STEP DOWN", ah->stats.ast_ani_stepdown },
		{ "INV LISTENTIME", ah->stats.ast_ani_lneg_or_lzero },
		{ "OFDM ERRORS", ah->stats.ast_ani_ofdmerrs },
		{ "CCK ERRORS", ah->stats.ast_ani_cckerrs },
	};

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	len += scnprintf(buf + len, size - len, "%15s: %s\n", "ANI",
			 common->disable_ani ? "DISABLED" : "ENABLED");

	if (common->disable_ani)
		goto exit;

	for (i = 0; i < ARRAY_SIZE(ani_info); i++)
		len += scnprintf(buf + len, size - len, "%15s: %u\n",
				 ani_info[i].name, ani_info[i].val);

exit:
	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static ssize_t write_file_ani(struct file *file,
			      const char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long ani;
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &ani);
	if (ret)
		return ret;

	if (ani > 1)
		return -EINVAL;

	common->disable_ani = !ani;

	if (common->disable_ani) {
		clear_bit(ATH_OP_ANI_RUN, &common->op_flags);
		ath_stop_ani(sc);
	} else {
		ath_check_ani(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return count;
}

<<<<<<< HEAD
static const struct file_operations fops_disable_ani = {
	.read = read_file_disable_ani,
	.write = write_file_disable_ani,
=======
static const struct file_operations fops_ani = {
	.read = read_file_ani,
	.write = write_file_ani,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

<<<<<<< HEAD
static ssize_t read_file_dma(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	char *buf;
	int retval;
	unsigned int len = 0;
=======
#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT

static ssize_t read_file_bt_ant_diversity(struct file *file,
					  char __user *user_buf,
					  size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "%d\n", common->bt_ant_diversity);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_bt_ant_diversity(struct file *file,
					   const char __user *user_buf,
					   size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath9k_hw_capabilities *pCap = &sc->sc_ah->caps;
	unsigned long bt_ant_diversity;
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &bt_ant_diversity);
	if (ret)
		return ret;

	if (!(pCap->hw_caps & ATH9K_HW_CAP_BT_ANT_DIV))
		goto exit;

	common->bt_ant_diversity = !!bt_ant_diversity;
	ath9k_ps_wakeup(sc);
	ath9k_hw_set_bt_ant_diversity(sc->sc_ah, common->bt_ant_diversity);
	ath_dbg(common, CONFIG, "Enable WLAN/BT RX Antenna diversity: %d\n",
		common->bt_ant_diversity);
	ath9k_ps_restore(sc);
exit:
	return count;
}

static const struct file_operations fops_bt_ant_diversity = {
	.read = read_file_bt_ant_diversity,
	.write = write_file_bt_ant_diversity,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

#endif

void ath9k_debug_stat_ant(struct ath_softc *sc,
			  struct ath_hw_antcomb_conf *div_ant_conf,
			  int main_rssi_avg, int alt_rssi_avg)
{
	struct ath_antenna_stats *as_main = &sc->debug.stats.ant_stats[ANT_MAIN];
	struct ath_antenna_stats *as_alt = &sc->debug.stats.ant_stats[ANT_ALT];

	as_main->lna_attempt_cnt[div_ant_conf->main_lna_conf]++;
	as_alt->lna_attempt_cnt[div_ant_conf->alt_lna_conf]++;

	as_main->rssi_avg = main_rssi_avg;
	as_alt->rssi_avg = alt_rssi_avg;
}

static ssize_t read_file_antenna_diversity(struct file *file,
					   char __user *user_buf,
					   size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_hw_capabilities *pCap = &ah->caps;
	struct ath_antenna_stats *as_main = &sc->debug.stats.ant_stats[ANT_MAIN];
	struct ath_antenna_stats *as_alt = &sc->debug.stats.ant_stats[ANT_ALT];
	struct ath_hw_antcomb_conf div_ant_conf;
	unsigned int len = 0;
	const unsigned int size = 1024;
	ssize_t retval = 0;
	char *buf;
	static const char *lna_conf_str[4] = {
		"LNA1_MINUS_LNA2", "LNA2", "LNA1", "LNA1_PLUS_LNA2"
	};

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	if (!(pCap->hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB)) {
		len += scnprintf(buf + len, size - len, "%s\n",
				 "Antenna Diversity Combining is disabled");
		goto exit;
	}

	ath9k_ps_wakeup(sc);
	ath9k_hw_antdiv_comb_conf_get(ah, &div_ant_conf);
	len += scnprintf(buf + len, size - len, "Current MAIN config : %s\n",
			 lna_conf_str[div_ant_conf.main_lna_conf]);
	len += scnprintf(buf + len, size - len, "Current ALT config  : %s\n",
			 lna_conf_str[div_ant_conf.alt_lna_conf]);
	len += scnprintf(buf + len, size - len, "Average MAIN RSSI   : %d\n",
			 as_main->rssi_avg);
	len += scnprintf(buf + len, size - len, "Average ALT RSSI    : %d\n\n",
			 as_alt->rssi_avg);
	ath9k_ps_restore(sc);

	len += scnprintf(buf + len, size - len, "Packet Receive Cnt:\n");
	len += scnprintf(buf + len, size - len, "-------------------\n");

	len += scnprintf(buf + len, size - len, "%30s%15s\n",
			 "MAIN", "ALT");
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "TOTAL COUNT",
			 as_main->recv_cnt,
			 as_alt->recv_cnt);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1",
			 as_main->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1],
			 as_alt->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA2",
			 as_main->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA2],
			 as_alt->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA2]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1 + LNA2",
			 as_main->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2],
			 as_alt->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1 - LNA2",
			 as_main->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2],
			 as_alt->lna_recv_cnt[ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2]);

	len += scnprintf(buf + len, size - len, "\nLNA Config Attempts:\n");
	len += scnprintf(buf + len, size - len, "--------------------\n");

	len += scnprintf(buf + len, size - len, "%30s%15s\n",
			 "MAIN", "ALT");
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1",
			 as_main->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1],
			 as_alt->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA2",
			 as_main->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA2],
			 as_alt->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA2]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1 + LNA2",
			 as_main->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2],
			 as_alt->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1_PLUS_LNA2]);
	len += scnprintf(buf + len, size - len, "%-14s:%15d%15d\n",
			 "LNA1 - LNA2",
			 as_main->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2],
			 as_alt->lna_attempt_cnt[ATH_ANT_DIV_COMB_LNA1_MINUS_LNA2]);

exit:
	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static const struct file_operations fops_antenna_diversity = {
	.read = read_file_antenna_diversity,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static int read_file_dma(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 val[ATH9K_NUM_DMA_DEBUG_REGS];
	int i, qcuOffset = 0, dcuOffset = 0;
	u32 *qcuBase = &val[0], *dcuBase = &val[4];

<<<<<<< HEAD
	buf = kmalloc(DMA_BUF_LEN, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_wakeup(sc);

	REG_WRITE_D(ah, AR_MACMISC,
		  ((AR_MACMISC_DMA_OBS_LINE_8 << AR_MACMISC_DMA_OBS_S) |
		   (AR_MACMISC_MISC_OBS_BUS_1 <<
		    AR_MACMISC_MISC_OBS_BUS_MSB_S)));

<<<<<<< HEAD
	len += snprintf(buf + len, DMA_BUF_LEN - len,
			"Raw DMA Debug values:\n");

	for (i = 0; i < ATH9K_NUM_DMA_DEBUG_REGS; i++) {
		if (i % 4 == 0)
			len += snprintf(buf + len, DMA_BUF_LEN - len, "\n");

		val[i] = REG_READ_D(ah, AR_DMADBG_0 + (i * sizeof(u32)));
		len += snprintf(buf + len, DMA_BUF_LEN - len, "%d: %08x ",
				i, val[i]);
	}

	len += snprintf(buf + len, DMA_BUF_LEN - len, "\n\n");
	len += snprintf(buf + len, DMA_BUF_LEN - len,
			"Num QCU: chain_st fsp_ok fsp_st DCU: chain_st\n");
=======
	seq_puts(file, "Raw DMA Debug values:\n");

	for (i = 0; i < ATH9K_NUM_DMA_DEBUG_REGS; i++) {
		if (i % 4 == 0)
			seq_puts(file, "\n");

		val[i] = REG_READ_D(ah, AR_DMADBG_0 + (i * sizeof(u32)));
		seq_printf(file, "%d: %08x ", i, val[i]);
	}

	seq_puts(file, "\n\n");
	seq_puts(file, "Num QCU: chain_st fsp_ok fsp_st DCU: chain_st\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < ATH9K_NUM_QUEUES; i++, qcuOffset += 4, dcuOffset += 5) {
		if (i == 8) {
			qcuOffset = 0;
			qcuBase++;
		}

		if (i == 6) {
			dcuOffset = 0;
			dcuBase++;
		}

<<<<<<< HEAD
		len += snprintf(buf + len, DMA_BUF_LEN - len,
			"%2d          %2x      %1x     %2x           %2x\n",
			i, (*qcuBase & (0x7 << qcuOffset)) >> qcuOffset,
			(*qcuBase & (0x8 << qcuOffset)) >> (qcuOffset + 3),
			val[2] & (0x7 << (i * 3)) >> (i * 3),
			(*dcuBase & (0x1f << dcuOffset)) >> dcuOffset);
	}

	len += snprintf(buf + len, DMA_BUF_LEN - len, "\n");

	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"qcu_stitch state:   %2x    qcu_fetch state:        %2x\n",
		(val[3] & 0x003c0000) >> 18, (val[3] & 0x03c00000) >> 22);
	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"qcu_complete state: %2x    dcu_complete state:     %2x\n",
		(val[3] & 0x1c000000) >> 26, (val[6] & 0x3));
	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"dcu_arb state:      %2x    dcu_fp state:           %2x\n",
		(val[5] & 0x06000000) >> 25, (val[5] & 0x38000000) >> 27);
	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"chan_idle_dur:     %3d    chan_idle_dur_valid:     %1d\n",
		(val[6] & 0x000003fc) >> 2, (val[6] & 0x00000400) >> 10);
	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"txfifo_valid_0:      %1d    txfifo_valid_1:          %1d\n",
		(val[6] & 0x00000800) >> 11, (val[6] & 0x00001000) >> 12);
	len += snprintf(buf + len, DMA_BUF_LEN - len,
		"txfifo_dcu_num_0:   %2d    txfifo_dcu_num_1:       %2d\n",
		(val[6] & 0x0001e000) >> 13, (val[6] & 0x001e0000) >> 17);

	len += snprintf(buf + len, DMA_BUF_LEN - len, "pcu observe: 0x%x\n",
			REG_READ_D(ah, AR_OBS_BUS_1));
	len += snprintf(buf + len, DMA_BUF_LEN - len,
			"AR_CR: 0x%x\n", REG_READ_D(ah, AR_CR));

	ath9k_ps_restore(sc);

	if (len > DMA_BUF_LEN)
		len = DMA_BUF_LEN;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);
	return retval;
}

static const struct file_operations fops_dma = {
	.read = read_file_dma,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};


=======
		seq_printf(file, "%2d          %2x      %1x     %2x           %2x\n",
			   i, (*qcuBase & (0x7 << qcuOffset)) >> qcuOffset,
			   (*qcuBase & (0x8 << qcuOffset)) >> (qcuOffset + 3),
			   (val[2] & (0x7 << (i * 3))) >> (i * 3),
			   (*dcuBase & (0x1f << dcuOffset)) >> dcuOffset);
	}

	seq_puts(file, "\n");

	seq_printf(file, "qcu_stitch state:   %2x    qcu_fetch state:        %2x\n",
		   (val[3] & 0x003c0000) >> 18, (val[3] & 0x03c00000) >> 22);
	seq_printf(file, "qcu_complete state: %2x    dcu_complete state:     %2x\n",
		   (val[3] & 0x1c000000) >> 26, (val[6] & 0x3));
	seq_printf(file, "dcu_arb state:      %2x    dcu_fp state:           %2x\n",
		   (val[5] & 0x06000000) >> 25, (val[5] & 0x38000000) >> 27);
	seq_printf(file, "chan_idle_dur:     %3d    chan_idle_dur_valid:     %1d\n",
		   (val[6] & 0x000003fc) >> 2, (val[6] & 0x00000400) >> 10);
	seq_printf(file, "txfifo_valid_0:      %1d    txfifo_valid_1:          %1d\n",
		   (val[6] & 0x00000800) >> 11, (val[6] & 0x00001000) >> 12);
	seq_printf(file, "txfifo_dcu_num_0:   %2d    txfifo_dcu_num_1:       %2d\n",
		   (val[6] & 0x0001e000) >> 13, (val[6] & 0x001e0000) >> 17);

	seq_printf(file, "pcu observe: 0x%x\n", REG_READ_D(ah, AR_OBS_BUS_1));
	seq_printf(file, "AR_CR: 0x%x\n", REG_READ_D(ah, AR_CR));

	ath9k_ps_restore(sc);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ath_debug_stat_interrupt(struct ath_softc *sc, enum ath9k_int status)
{
	if (status)
		sc->debug.stats.istats.total++;
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		if (status & ATH9K_INT_RXLP)
			sc->debug.stats.istats.rxlp++;
		if (status & ATH9K_INT_RXHP)
			sc->debug.stats.istats.rxhp++;
		if (status & ATH9K_INT_BB_WATCHDOG)
			sc->debug.stats.istats.bb_watchdog++;
	} else {
		if (status & ATH9K_INT_RX)
			sc->debug.stats.istats.rxok++;
	}
	if (status & ATH9K_INT_RXEOL)
		sc->debug.stats.istats.rxeol++;
	if (status & ATH9K_INT_RXORN)
		sc->debug.stats.istats.rxorn++;
	if (status & ATH9K_INT_TX)
		sc->debug.stats.istats.txok++;
	if (status & ATH9K_INT_TXURN)
		sc->debug.stats.istats.txurn++;
<<<<<<< HEAD
	if (status & ATH9K_INT_MIB)
		sc->debug.stats.istats.mib++;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status & ATH9K_INT_RXPHY)
		sc->debug.stats.istats.rxphyerr++;
	if (status & ATH9K_INT_RXKCM)
		sc->debug.stats.istats.rx_keycache_miss++;
	if (status & ATH9K_INT_SWBA)
		sc->debug.stats.istats.swba++;
	if (status & ATH9K_INT_BMISS)
		sc->debug.stats.istats.bmiss++;
	if (status & ATH9K_INT_BNR)
		sc->debug.stats.istats.bnr++;
	if (status & ATH9K_INT_CST)
		sc->debug.stats.istats.cst++;
	if (status & ATH9K_INT_GTT)
		sc->debug.stats.istats.gtt++;
	if (status & ATH9K_INT_TIM)
		sc->debug.stats.istats.tim++;
	if (status & ATH9K_INT_CABEND)
		sc->debug.stats.istats.cabend++;
	if (status & ATH9K_INT_DTIMSYNC)
		sc->debug.stats.istats.dtimsync++;
	if (status & ATH9K_INT_DTIM)
		sc->debug.stats.istats.dtim++;
	if (status & ATH9K_INT_TSFOOR)
		sc->debug.stats.istats.tsfoor++;
<<<<<<< HEAD
}

static ssize_t read_file_interrupt(struct file *file, char __user *user_buf,
				   size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	char buf[512];
	unsigned int len = 0;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		len += snprintf(buf + len, sizeof(buf) - len,
			"%8s: %10u\n", "RXLP", sc->debug.stats.istats.rxlp);
		len += snprintf(buf + len, sizeof(buf) - len,
			"%8s: %10u\n", "RXHP", sc->debug.stats.istats.rxhp);
		len += snprintf(buf + len, sizeof(buf) - len,
			"%8s: %10u\n", "WATCHDOG",
			sc->debug.stats.istats.bb_watchdog);
	} else {
		len += snprintf(buf + len, sizeof(buf) - len,
			"%8s: %10u\n", "RX", sc->debug.stats.istats.rxok);
	}
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "RXEOL", sc->debug.stats.istats.rxeol);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "RXORN", sc->debug.stats.istats.rxorn);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "TX", sc->debug.stats.istats.txok);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "TXURN", sc->debug.stats.istats.txurn);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "MIB", sc->debug.stats.istats.mib);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "RXPHY", sc->debug.stats.istats.rxphyerr);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "RXKCM", sc->debug.stats.istats.rx_keycache_miss);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "SWBA", sc->debug.stats.istats.swba);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "BMISS", sc->debug.stats.istats.bmiss);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "BNR", sc->debug.stats.istats.bnr);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "CST", sc->debug.stats.istats.cst);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "GTT", sc->debug.stats.istats.gtt);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "TIM", sc->debug.stats.istats.tim);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "CABEND", sc->debug.stats.istats.cabend);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "DTIMSYNC", sc->debug.stats.istats.dtimsync);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "DTIM", sc->debug.stats.istats.dtim);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "TSFOOR", sc->debug.stats.istats.tsfoor);
	len += snprintf(buf + len, sizeof(buf) - len,
		"%8s: %10u\n", "TOTAL", sc->debug.stats.istats.total);


	if (len > sizeof(buf))
		len = sizeof(buf);

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static const struct file_operations fops_interrupt = {
	.read = read_file_interrupt,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

#define PR_QNUM(_n) sc->tx.txq_map[_n]->axq_qnum
#define PR(str, elem)							\
	do {								\
		len += snprintf(buf + len, size - len,			\
				"%s%13u%11u%10u%10u\n", str,		\
		sc->debug.stats.txstats[PR_QNUM(WME_AC_BE)].elem, \
		sc->debug.stats.txstats[PR_QNUM(WME_AC_BK)].elem, \
		sc->debug.stats.txstats[PR_QNUM(WME_AC_VI)].elem, \
		sc->debug.stats.txstats[PR_QNUM(WME_AC_VO)].elem); \
		if (len >= size)			  \
			goto done;			  \
} while(0)

#define PRX(str, elem)							\
do {									\
	len += snprintf(buf + len, size - len,				\
			"%s%13u%11u%10u%10u\n", str,			\
			(unsigned int)(sc->tx.txq_map[WME_AC_BE]->elem),	\
			(unsigned int)(sc->tx.txq_map[WME_AC_BK]->elem),	\
			(unsigned int)(sc->tx.txq_map[WME_AC_VI]->elem),	\
			(unsigned int)(sc->tx.txq_map[WME_AC_VO]->elem));	\
	if (len >= size)						\
		goto done;						\
} while(0)

#define PRQLE(str, elem)						\
do {									\
	len += snprintf(buf + len, size - len,				\
			"%s%13i%11i%10i%10i\n", str,			\
			list_empty(&sc->tx.txq_map[WME_AC_BE]->elem),	\
			list_empty(&sc->tx.txq_map[WME_AC_BK]->elem),	\
			list_empty(&sc->tx.txq_map[WME_AC_VI]->elem),	\
			list_empty(&sc->tx.txq_map[WME_AC_VO]->elem));	\
	if (len >= size)						\
		goto done;						\
} while (0)

static ssize_t read_file_xmit(struct file *file, char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	char *buf;
	unsigned int len = 0, size = 8000;
	int i;
	ssize_t retval = 0;
	char tmp[32];

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	len += sprintf(buf, "Num-Tx-Queues: %i  tx-queues-setup: 0x%x"
		       " poll-work-seen: %u\n"
		       "%30s %10s%10s%10s\n\n",
		       ATH9K_NUM_TX_QUEUES, sc->tx.txqsetup,
		       sc->tx_complete_poll_work_seen,
		       "BE", "BK", "VI", "VO");
=======
	if (status & ATH9K_INT_MCI)
		sc->debug.stats.istats.mci++;
	if (status & ATH9K_INT_GENTIMER)
		sc->debug.stats.istats.gen_timer++;
}

static int read_file_interrupt(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;

#define PR_IS(a, s)						\
	do {							\
		seq_printf(file, "%21s: %10u\n", a,		\
			   sc->debug.stats.istats.s);		\
	} while (0)

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
		PR_IS("RXLP", rxlp);
		PR_IS("RXHP", rxhp);
		PR_IS("WATCHDOG", bb_watchdog);
	} else {
		PR_IS("RX", rxok);
	}
	PR_IS("RXEOL", rxeol);
	PR_IS("RXORN", rxorn);
	PR_IS("TX", txok);
	PR_IS("TXURN", txurn);
	PR_IS("MIB", mib);
	PR_IS("RXPHY", rxphyerr);
	PR_IS("RXKCM", rx_keycache_miss);
	PR_IS("SWBA", swba);
	PR_IS("BMISS", bmiss);
	PR_IS("BNR", bnr);
	PR_IS("CST", cst);
	PR_IS("GTT", gtt);
	PR_IS("TIM", tim);
	PR_IS("CABEND", cabend);
	PR_IS("DTIMSYNC", dtimsync);
	PR_IS("DTIM", dtim);
	PR_IS("TSFOOR", tsfoor);
	PR_IS("MCI", mci);
	PR_IS("GENTIMER", gen_timer);
	PR_IS("TOTAL", total);

	seq_puts(file, "SYNC_CAUSE stats:\n");

	PR_IS("Sync-All", sync_cause_all);
	PR_IS("RTC-IRQ", sync_rtc_irq);
	PR_IS("MAC-IRQ", sync_mac_irq);
	PR_IS("EEPROM-Illegal-Access", eeprom_illegal_access);
	PR_IS("APB-Timeout", apb_timeout);
	PR_IS("PCI-Mode-Conflict", pci_mode_conflict);
	PR_IS("HOST1-Fatal", host1_fatal);
	PR_IS("HOST1-Perr", host1_perr);
	PR_IS("TRCV-FIFO-Perr", trcv_fifo_perr);
	PR_IS("RADM-CPL-EP", radm_cpl_ep);
	PR_IS("RADM-CPL-DLLP-Abort", radm_cpl_dllp_abort);
	PR_IS("RADM-CPL-TLP-Abort", radm_cpl_tlp_abort);
	PR_IS("RADM-CPL-ECRC-Err", radm_cpl_ecrc_err);
	PR_IS("RADM-CPL-Timeout", radm_cpl_timeout);
	PR_IS("Local-Bus-Timeout", local_timeout);
	PR_IS("PM-Access", pm_access);
	PR_IS("MAC-Awake", mac_awake);
	PR_IS("MAC-Asleep", mac_asleep);
	PR_IS("MAC-Sleep-Access", mac_sleep_access);

	return 0;
}

static int read_file_xmit(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;

	seq_printf(file, "%30s %10s%10s%10s\n\n", "BE", "BK", "VI", "VO");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	PR("MPDUs Queued:    ", queued);
	PR("MPDUs Completed: ", completed);
	PR("MPDUs XRetried:  ", xretries);
	PR("Aggregates:      ", a_aggr);
	PR("AMPDUs Queued HW:", a_queued_hw);
<<<<<<< HEAD
	PR("AMPDUs Queued SW:", a_queued_sw);
	PR("AMPDUs Completed:", a_completed);
	PR("AMPDUs Retried:  ", a_retries);
	PR("AMPDUs XRetried: ", a_xretries);
=======
	PR("AMPDUs Completed:", a_completed);
	PR("AMPDUs Retried:  ", a_retries);
	PR("AMPDUs XRetried: ", a_xretries);
	PR("TXERR Filtered:  ", txerr_filtered);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	PR("FIFO Underrun:   ", fifo_underrun);
	PR("TXOP Exceeded:   ", xtxop);
	PR("TXTIMER Expiry:  ", timer_exp);
	PR("DESC CFG Error:  ", desc_cfg_err);
	PR("DATA Underrun:   ", data_underrun);
	PR("DELIM Underrun:  ", delim_underrun);
	PR("TX-Pkts-All:     ", tx_pkts_all);
	PR("TX-Bytes-All:    ", tx_bytes_all);
<<<<<<< HEAD
	PR("hw-put-tx-buf:   ", puttxbuf);
	PR("hw-tx-start:     ", txstart);
	PR("hw-tx-proc-desc: ", txprocdesc);
	len += snprintf(buf + len, size - len,
			"%s%11p%11p%10p%10p\n", "txq-memory-address:",
			sc->tx.txq_map[WME_AC_BE],
			sc->tx.txq_map[WME_AC_BK],
			sc->tx.txq_map[WME_AC_VI],
			sc->tx.txq_map[WME_AC_VO]);
	if (len >= size)
		goto done;

	PRX("axq-qnum:        ", axq_qnum);
	PRX("axq-depth:       ", axq_depth);
	PRX("axq-ampdu_depth: ", axq_ampdu_depth);
	PRX("axq-stopped      ", stopped);
	PRX("tx-in-progress   ", axq_tx_inprogress);
	PRX("pending-frames   ", pending_frames);
	PRX("txq_headidx:     ", txq_headidx);
	PRX("txq_tailidx:     ", txq_headidx);

	PRQLE("axq_q empty:       ", axq_q);
	PRQLE("axq_acq empty:     ", axq_acq);
	for (i = 0; i < ATH_TXFIFO_DEPTH; i++) {
		snprintf(tmp, sizeof(tmp) - 1, "txq_fifo[%i] empty: ", i);
		PRQLE(tmp, txq_fifo[i]);
	}

	/* Print out more detailed queue-info */
	for (i = 0; i <= WME_AC_BK; i++) {
		struct ath_txq *txq = &(sc->tx.txq[i]);
		struct ath_atx_ac *ac;
		struct ath_atx_tid *tid;
		if (len >= size)
			goto done;
		spin_lock_bh(&txq->axq_lock);
		if (!list_empty(&txq->axq_acq)) {
			ac = list_first_entry(&txq->axq_acq, struct ath_atx_ac,
					      list);
			len += snprintf(buf + len, size - len,
					"txq[%i] first-ac: %p sched: %i\n",
					i, ac, ac->sched);
			if (list_empty(&ac->tid_q) || (len >= size))
				goto done_for;
			tid = list_first_entry(&ac->tid_q, struct ath_atx_tid,
					       list);
			len += snprintf(buf + len, size - len,
					" first-tid: %p sched: %i paused: %i\n",
					tid, tid->sched, tid->paused);
		}
	done_for:
		spin_unlock_bh(&txq->axq_lock);
	}

done:
	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static ssize_t read_file_stations(struct file *file, char __user *user_buf,
				  size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	char *buf;
	unsigned int len = 0, size = 64000;
	struct ath_node *an = NULL;
	ssize_t retval = 0;
	int q;

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	len += snprintf(buf + len, size - len,
			"Stations:\n"
			" tid: addr sched paused buf_q-empty an ac baw\n"
			" ac: addr sched tid_q-empty txq\n");

	spin_lock(&sc->nodes_lock);
	list_for_each_entry(an, &sc->nodes, list) {
		unsigned short ma = an->maxampdu;
		if (ma == 0)
			ma = 65535; /* see ath_lookup_rate */
		len += snprintf(buf + len, size - len,
				"iface: %pM  sta: %pM max-ampdu: %hu mpdu-density: %uus\n",
				an->vif->addr, an->sta->addr, ma,
				(unsigned int)(an->mpdudensity));
		if (len >= size)
			goto done;

		for (q = 0; q < WME_NUM_TID; q++) {
			struct ath_atx_tid *tid = &(an->tid[q]);
			len += snprintf(buf + len, size - len,
					" tid: %p %s %s %i %p %p %hu\n",
					tid, tid->sched ? "sched" : "idle",
					tid->paused ? "paused" : "running",
					skb_queue_empty(&tid->buf_q),
					tid->an, tid->ac, tid->baw_size);
			if (len >= size)
				goto done;
		}

		for (q = 0; q < WME_NUM_AC; q++) {
			struct ath_atx_ac *ac = &(an->ac[q]);
			len += snprintf(buf + len, size - len,
					" ac: %p %s %i %p\n",
					ac, ac->sched ? "sched" : "idle",
					list_empty(&ac->tid_q), ac->txq);
			if (len >= size)
				goto done;
		}
	}

done:
	spin_unlock(&sc->nodes_lock);
	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static ssize_t read_file_misc(struct file *file, char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_hw *hw = sc->hw;
	struct ath9k_vif_iter_data iter_data;
	char buf[512];
	unsigned int len = 0;
	ssize_t retval = 0;
	unsigned int reg;
	u32 rxfilter;

	len += snprintf(buf + len, sizeof(buf) - len,
			"BSSID: %pM\n", common->curbssid);
	len += snprintf(buf + len, sizeof(buf) - len,
			"BSSID-MASK: %pM\n", common->bssidmask);
	len += snprintf(buf + len, sizeof(buf) - len,
			"OPMODE: %s\n", ath_opmode_to_string(sc->sc_ah->opmode));
=======
	PR("HW-put-tx-buf:   ", puttxbuf);
	PR("HW-tx-start:     ", txstart);
	PR("HW-tx-proc-desc: ", txprocdesc);
	PR("TX-Failed:       ", txfailed);

	return 0;
}

static void print_queue(struct ath_softc *sc, struct ath_txq *txq,
			struct seq_file *file)
{
	ath_txq_lock(sc, txq);

	seq_printf(file, "%s: %d ", "qnum", txq->axq_qnum);
	seq_printf(file, "%s: %2d ", "qdepth", txq->axq_depth);
	seq_printf(file, "%s: %2d ", "ampdu-depth", txq->axq_ampdu_depth);
	seq_printf(file, "%s: %3d\n", "pending", txq->pending_frames);

	ath_txq_unlock(sc, txq);
}

static int read_file_queues(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;
	struct ath_txq *txq;
	int i;
	static const char *qname[4] = {
		"VO", "VI", "BE", "BK"
	};

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		txq = sc->tx.txq_map[i];
		seq_printf(file, "(%s):  ", qname[i]);
		print_queue(sc, txq, file);
	}

	seq_puts(file, "(CAB): ");
	print_queue(sc, sc->beacon.cabq, file);

	return 0;
}

static int read_file_misc(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath9k_vif_iter_data iter_data;
	struct ath_chanctx *ctx;
	unsigned int reg;
	u32 rxfilter, i;

	seq_printf(file, "BSSID: %pM\n", common->curbssid);
	seq_printf(file, "BSSID-MASK: %pM\n", common->bssidmask);
	seq_printf(file, "OPMODE: %s\n",
		   ath_opmode_to_string(sc->sc_ah->opmode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath9k_ps_wakeup(sc);
	rxfilter = ath9k_hw_getrxfilter(sc->sc_ah);
	ath9k_ps_restore(sc);

<<<<<<< HEAD
	len += snprintf(buf + len, sizeof(buf) - len,
			"RXFILTER: 0x%x", rxfilter);

	if (rxfilter & ATH9K_RX_FILTER_UCAST)
		len += snprintf(buf + len, sizeof(buf) - len, " UCAST");
	if (rxfilter & ATH9K_RX_FILTER_MCAST)
		len += snprintf(buf + len, sizeof(buf) - len, " MCAST");
	if (rxfilter & ATH9K_RX_FILTER_BCAST)
		len += snprintf(buf + len, sizeof(buf) - len, " BCAST");
	if (rxfilter & ATH9K_RX_FILTER_CONTROL)
		len += snprintf(buf + len, sizeof(buf) - len, " CONTROL");
	if (rxfilter & ATH9K_RX_FILTER_BEACON)
		len += snprintf(buf + len, sizeof(buf) - len, " BEACON");
	if (rxfilter & ATH9K_RX_FILTER_PROM)
		len += snprintf(buf + len, sizeof(buf) - len, " PROM");
	if (rxfilter & ATH9K_RX_FILTER_PROBEREQ)
		len += snprintf(buf + len, sizeof(buf) - len, " PROBEREQ");
	if (rxfilter & ATH9K_RX_FILTER_PHYERR)
		len += snprintf(buf + len, sizeof(buf) - len, " PHYERR");
	if (rxfilter & ATH9K_RX_FILTER_MYBEACON)
		len += snprintf(buf + len, sizeof(buf) - len, " MYBEACON");
	if (rxfilter & ATH9K_RX_FILTER_COMP_BAR)
		len += snprintf(buf + len, sizeof(buf) - len, " COMP_BAR");
	if (rxfilter & ATH9K_RX_FILTER_PSPOLL)
		len += snprintf(buf + len, sizeof(buf) - len, " PSPOLL");
	if (rxfilter & ATH9K_RX_FILTER_PHYRADAR)
		len += snprintf(buf + len, sizeof(buf) - len, " PHYRADAR");
	if (rxfilter & ATH9K_RX_FILTER_MCAST_BCAST_ALL)
		len += snprintf(buf + len, sizeof(buf) - len, " MCAST_BCAST_ALL");
	if (rxfilter & ATH9K_RX_FILTER_CONTROL_WRAPPER)
		len += snprintf(buf + len, sizeof(buf) - len, " CONTROL_WRAPPER");

	len += snprintf(buf + len, sizeof(buf) - len, "\n");

	reg = sc->sc_ah->imask;

	len += snprintf(buf + len, sizeof(buf) - len, "INTERRUPT-MASK: 0x%x", reg);

	if (reg & ATH9K_INT_SWBA)
		len += snprintf(buf + len, sizeof(buf) - len, " SWBA");
	if (reg & ATH9K_INT_BMISS)
		len += snprintf(buf + len, sizeof(buf) - len, " BMISS");
	if (reg & ATH9K_INT_CST)
		len += snprintf(buf + len, sizeof(buf) - len, " CST");
	if (reg & ATH9K_INT_RX)
		len += snprintf(buf + len, sizeof(buf) - len, " RX");
	if (reg & ATH9K_INT_RXHP)
		len += snprintf(buf + len, sizeof(buf) - len, " RXHP");
	if (reg & ATH9K_INT_RXLP)
		len += snprintf(buf + len, sizeof(buf) - len, " RXLP");
	if (reg & ATH9K_INT_BB_WATCHDOG)
		len += snprintf(buf + len, sizeof(buf) - len, " BB_WATCHDOG");

	len += snprintf(buf + len, sizeof(buf) - len, "\n");

	ath9k_calculate_iter_data(hw, NULL, &iter_data);

	len += snprintf(buf + len, sizeof(buf) - len,
			"VIF-COUNTS: AP: %i STA: %i MESH: %i WDS: %i"
			" ADHOC: %i TOTAL: %hi BEACON-VIF: %hi\n",
			iter_data.naps, iter_data.nstations, iter_data.nmeshes,
			iter_data.nwds, iter_data.nadhocs,
			sc->nvifs, sc->nbcnvifs);

	if (len > sizeof(buf))
		len = sizeof(buf);

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	return retval;
}

static ssize_t read_file_reset(struct file *file, char __user *user_buf,
			       size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	char buf[512];
	unsigned int len = 0;

	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "Baseband Hang",
			sc->debug.stats.reset[RESET_TYPE_BB_HANG]);
	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "Baseband Watchdog",
			sc->debug.stats.reset[RESET_TYPE_BB_WATCHDOG]);
	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "Fatal HW Error",
			sc->debug.stats.reset[RESET_TYPE_FATAL_INT]);
	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "TX HW error",
			sc->debug.stats.reset[RESET_TYPE_TX_ERROR]);
	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "TX Path Hang",
			sc->debug.stats.reset[RESET_TYPE_TX_HANG]);
	len += snprintf(buf + len, sizeof(buf) - len,
			"%17s: %2d\n", "PLL RX Hang",
			sc->debug.stats.reset[RESET_TYPE_PLL_HANG]);

	if (len > sizeof(buf))
		len = sizeof(buf);

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

=======
	seq_printf(file, "RXFILTER: 0x%x", rxfilter);

	if (rxfilter & ATH9K_RX_FILTER_UCAST)
		seq_puts(file, " UCAST");
	if (rxfilter & ATH9K_RX_FILTER_MCAST)
		seq_puts(file, " MCAST");
	if (rxfilter & ATH9K_RX_FILTER_BCAST)
		seq_puts(file, " BCAST");
	if (rxfilter & ATH9K_RX_FILTER_CONTROL)
		seq_puts(file, " CONTROL");
	if (rxfilter & ATH9K_RX_FILTER_BEACON)
		seq_puts(file, " BEACON");
	if (rxfilter & ATH9K_RX_FILTER_PROM)
		seq_puts(file, " PROM");
	if (rxfilter & ATH9K_RX_FILTER_PROBEREQ)
		seq_puts(file, " PROBEREQ");
	if (rxfilter & ATH9K_RX_FILTER_PHYERR)
		seq_puts(file, " PHYERR");
	if (rxfilter & ATH9K_RX_FILTER_MYBEACON)
		seq_puts(file, " MYBEACON");
	if (rxfilter & ATH9K_RX_FILTER_COMP_BAR)
		seq_puts(file, " COMP_BAR");
	if (rxfilter & ATH9K_RX_FILTER_PSPOLL)
		seq_puts(file, " PSPOLL");
	if (rxfilter & ATH9K_RX_FILTER_PHYRADAR)
		seq_puts(file, " PHYRADAR");
	if (rxfilter & ATH9K_RX_FILTER_MCAST_BCAST_ALL)
		seq_puts(file, " MCAST_BCAST_ALL");
	if (rxfilter & ATH9K_RX_FILTER_CONTROL_WRAPPER)
		seq_puts(file, " CONTROL_WRAPPER");

	seq_puts(file, "\n");

	reg = sc->sc_ah->imask;

	seq_printf(file, "INTERRUPT-MASK: 0x%x", reg);

	if (reg & ATH9K_INT_SWBA)
		seq_puts(file, " SWBA");
	if (reg & ATH9K_INT_BMISS)
		seq_puts(file, " BMISS");
	if (reg & ATH9K_INT_CST)
		seq_puts(file, " CST");
	if (reg & ATH9K_INT_RX)
		seq_puts(file, " RX");
	if (reg & ATH9K_INT_RXHP)
		seq_puts(file, " RXHP");
	if (reg & ATH9K_INT_RXLP)
		seq_puts(file, " RXLP");
	if (reg & ATH9K_INT_BB_WATCHDOG)
		seq_puts(file, " BB_WATCHDOG");

	seq_puts(file, "\n");

	i = 0;
	ath_for_each_chanctx(sc, ctx) {
		if (list_empty(&ctx->vifs))
			continue;
		ath9k_calculate_iter_data(sc, ctx, &iter_data);

		seq_printf(file,
			   "VIFS: CTX %i(%i) AP: %i STA: %i MESH: %i",
			   i++, (int)(ctx->assigned), iter_data.naps,
			   iter_data.nstations,
			   iter_data.nmeshes);
		seq_printf(file, " ADHOC: %i OCB: %i TOTAL: %hi BEACON-VIF: %hi\n",
			   iter_data.nadhocs, iter_data.nocbs, sc->cur_chan->nvifs,
			   sc->nbcnvifs);
	}

	return 0;
}

static int read_file_reset(struct seq_file *file, void *data)
{
	struct ath_softc *sc = file->private;
	static const char * const reset_cause[__RESET_TYPE_MAX] = {
		[RESET_TYPE_USER] = "User reset",
		[RESET_TYPE_BB_HANG] = "Baseband Hang",
		[RESET_TYPE_BB_WATCHDOG] = "Baseband Watchdog",
		[RESET_TYPE_FATAL_INT] = "Fatal HW Error",
		[RESET_TYPE_TX_ERROR] = "TX HW error",
		[RESET_TYPE_TX_GTT] = "Transmit timeout",
		[RESET_TYPE_TX_HANG] = "TX Path Hang",
		[RESET_TYPE_PLL_HANG] = "PLL RX Hang",
		[RESET_TYPE_MAC_HANG] = "MAC Hang",
		[RESET_TYPE_BEACON_STUCK] = "Stuck Beacon",
		[RESET_TYPE_MCI] = "MCI Reset",
		[RESET_TYPE_CALIBRATION] = "Calibration error",
		[RESET_TX_DMA_ERROR] = "Tx DMA stop error",
		[RESET_RX_DMA_ERROR] = "Rx DMA stop error",
	};
	int i;

	for (i = 0; i < ARRAY_SIZE(reset_cause); i++) {
		if (!reset_cause[i])
		    continue;

		seq_printf(file, "%17s: %2d\n", reset_cause[i],
			   sc->debug.stats.reset[i]);
	}

	return 0;
}

static int open_file_reset(struct inode *inode, struct file *f)
{
	return single_open(f, read_file_reset, inode->i_private);
}

static ssize_t write_file_reset(struct file *file,
				const char __user *user_buf,
				size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file_inode(file)->i_private;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	unsigned long val;
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &val);
	if (ret)
		return ret;

	if (val != 1)
		return -EINVAL;

	/* avoid rearming hw_reset_work on shutdown */
	mutex_lock(&sc->mutex);
	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
		mutex_unlock(&sc->mutex);
		return -EBUSY;
	}

	ath9k_queue_reset(sc, RESET_TYPE_USER);
	mutex_unlock(&sc->mutex);

	return count;
}

static const struct file_operations fops_reset = {
	.read = seq_read,
	.write = write_file_reset,
	.open = open_file_reset,
	.owner = THIS_MODULE,
	.llseek = seq_lseek,
	.release = single_release,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ath_debug_stat_tx(struct ath_softc *sc, struct ath_buf *bf,
		       struct ath_tx_status *ts, struct ath_txq *txq,
		       unsigned int flags)
{
<<<<<<< HEAD
#define TX_SAMP_DBG(c) (sc->debug.bb_mac_samp[sc->debug.sampidx].ts\
			[sc->debug.tsidx].c)
	int qnum = txq->axq_qnum;

	TX_STAT_INC(qnum, tx_pkts_all);
=======
	int qnum = txq->axq_qnum;

	TX_STAT_INC(sc, qnum, tx_pkts_all);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sc->debug.stats.txstats[qnum].tx_bytes_all += bf->bf_mpdu->len;

	if (bf_isampdu(bf)) {
		if (flags & ATH_TX_ERROR)
<<<<<<< HEAD
			TX_STAT_INC(qnum, a_xretries);
		else
			TX_STAT_INC(qnum, a_completed);
	} else {
		if (ts->ts_status & ATH9K_TXERR_XRETRY)
			TX_STAT_INC(qnum, xretries);
		else
			TX_STAT_INC(qnum, completed);
	}

	if (ts->ts_status & ATH9K_TXERR_FIFO)
		TX_STAT_INC(qnum, fifo_underrun);
	if (ts->ts_status & ATH9K_TXERR_XTXOP)
		TX_STAT_INC(qnum, xtxop);
	if (ts->ts_status & ATH9K_TXERR_TIMER_EXPIRED)
		TX_STAT_INC(qnum, timer_exp);
	if (ts->ts_flags & ATH9K_TX_DESC_CFG_ERR)
		TX_STAT_INC(qnum, desc_cfg_err);
	if (ts->ts_flags & ATH9K_TX_DATA_UNDERRUN)
		TX_STAT_INC(qnum, data_underrun);
	if (ts->ts_flags & ATH9K_TX_DELIM_UNDERRUN)
		TX_STAT_INC(qnum, delim_underrun);

#ifdef CONFIG_ATH9K_MAC_DEBUG
	spin_lock(&sc->debug.samp_lock);
	TX_SAMP_DBG(jiffies) = jiffies;
	TX_SAMP_DBG(rssi_ctl0) = ts->ts_rssi_ctl0;
	TX_SAMP_DBG(rssi_ctl1) = ts->ts_rssi_ctl1;
	TX_SAMP_DBG(rssi_ctl2) = ts->ts_rssi_ctl2;
	TX_SAMP_DBG(rssi_ext0) = ts->ts_rssi_ext0;
	TX_SAMP_DBG(rssi_ext1) = ts->ts_rssi_ext1;
	TX_SAMP_DBG(rssi_ext2) = ts->ts_rssi_ext2;
	TX_SAMP_DBG(rateindex) = ts->ts_rateindex;
	TX_SAMP_DBG(isok) = !!(ts->ts_status & ATH9K_TXERR_MASK);
	TX_SAMP_DBG(rts_fail_cnt) = ts->ts_shortretry;
	TX_SAMP_DBG(data_fail_cnt) = ts->ts_longretry;
	TX_SAMP_DBG(rssi) = ts->ts_rssi;
	TX_SAMP_DBG(tid) = ts->tid;
	TX_SAMP_DBG(qid) = ts->qid;

	if (ts->ts_flags & ATH9K_TX_BA) {
		TX_SAMP_DBG(ba_low) = ts->ba_low;
		TX_SAMP_DBG(ba_high) = ts->ba_high;
	} else {
		TX_SAMP_DBG(ba_low) = 0;
		TX_SAMP_DBG(ba_high) = 0;
	}

	sc->debug.tsidx = (sc->debug.tsidx + 1) % ATH_DBG_MAX_SAMPLES;
	spin_unlock(&sc->debug.samp_lock);
#endif

#undef TX_SAMP_DBG
}

static const struct file_operations fops_xmit = {
	.read = read_file_xmit,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static const struct file_operations fops_stations = {
	.read = read_file_stations,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static const struct file_operations fops_misc = {
	.read = read_file_misc,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static const struct file_operations fops_reset = {
	.read = read_file_reset,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_recv(struct file *file, char __user *user_buf,
			      size_t count, loff_t *ppos)
{
#define PHY_ERR(s, p) \
	len += snprintf(buf + len, size - len, "%22s : %10u\n", s, \
			sc->debug.stats.rxstats.phy_err_stats[p]);

	struct ath_softc *sc = file->private_data;
	char *buf;
	unsigned int len = 0, size = 1600;
	ssize_t retval = 0;

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "CRC ERR",
			sc->debug.stats.rxstats.crc_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "DECRYPT CRC ERR",
			sc->debug.stats.rxstats.decrypt_crc_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "PHY ERR",
			sc->debug.stats.rxstats.phy_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "MIC ERR",
			sc->debug.stats.rxstats.mic_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "PRE-DELIM CRC ERR",
			sc->debug.stats.rxstats.pre_delim_crc_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "POST-DELIM CRC ERR",
			sc->debug.stats.rxstats.post_delim_crc_err);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "DECRYPT BUSY ERR",
			sc->debug.stats.rxstats.decrypt_busy_err);

	PHY_ERR("UNDERRUN ERR", ATH9K_PHYERR_UNDERRUN);
	PHY_ERR("TIMING ERR", ATH9K_PHYERR_TIMING);
	PHY_ERR("PARITY ERR", ATH9K_PHYERR_PARITY);
	PHY_ERR("RATE ERR", ATH9K_PHYERR_RATE);
	PHY_ERR("LENGTH ERR", ATH9K_PHYERR_LENGTH);
	PHY_ERR("RADAR ERR", ATH9K_PHYERR_RADAR);
	PHY_ERR("SERVICE ERR", ATH9K_PHYERR_SERVICE);
	PHY_ERR("TOR ERR", ATH9K_PHYERR_TOR);
	PHY_ERR("OFDM-TIMING ERR", ATH9K_PHYERR_OFDM_TIMING);
	PHY_ERR("OFDM-SIGNAL-PARITY ERR", ATH9K_PHYERR_OFDM_SIGNAL_PARITY);
	PHY_ERR("OFDM-RATE ERR", ATH9K_PHYERR_OFDM_RATE_ILLEGAL);
	PHY_ERR("OFDM-LENGTH ERR", ATH9K_PHYERR_OFDM_LENGTH_ILLEGAL);
	PHY_ERR("OFDM-POWER-DROP ERR", ATH9K_PHYERR_OFDM_POWER_DROP);
	PHY_ERR("OFDM-SERVICE ERR", ATH9K_PHYERR_OFDM_SERVICE);
	PHY_ERR("OFDM-RESTART ERR", ATH9K_PHYERR_OFDM_RESTART);
	PHY_ERR("FALSE-RADAR-EXT ERR", ATH9K_PHYERR_FALSE_RADAR_EXT);
	PHY_ERR("CCK-TIMING ERR", ATH9K_PHYERR_CCK_TIMING);
	PHY_ERR("CCK-HEADER-CRC ERR", ATH9K_PHYERR_CCK_HEADER_CRC);
	PHY_ERR("CCK-RATE ERR", ATH9K_PHYERR_CCK_RATE_ILLEGAL);
	PHY_ERR("CCK-SERVICE ERR", ATH9K_PHYERR_CCK_SERVICE);
	PHY_ERR("CCK-RESTART ERR", ATH9K_PHYERR_CCK_RESTART);
	PHY_ERR("CCK-LENGTH ERR", ATH9K_PHYERR_CCK_LENGTH_ILLEGAL);
	PHY_ERR("CCK-POWER-DROP ERR", ATH9K_PHYERR_CCK_POWER_DROP);
	PHY_ERR("HT-CRC ERR", ATH9K_PHYERR_HT_CRC_ERROR);
	PHY_ERR("HT-LENGTH ERR", ATH9K_PHYERR_HT_LENGTH_ILLEGAL);
	PHY_ERR("HT-RATE ERR", ATH9K_PHYERR_HT_RATE_ILLEGAL);

	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "RX-Pkts-All",
			sc->debug.stats.rxstats.rx_pkts_all);
	len += snprintf(buf + len, size - len,
			"%22s : %10u\n", "RX-Bytes-All",
			sc->debug.stats.rxstats.rx_bytes_all);

	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;

#undef PHY_ERR
=======
			TX_STAT_INC(sc, qnum, a_xretries);
		else
			TX_STAT_INC(sc, qnum, a_completed);
	} else {
		if (ts->ts_status & ATH9K_TXERR_XRETRY)
			TX_STAT_INC(sc, qnum, xretries);
		else
			TX_STAT_INC(sc, qnum, completed);
	}

	if (ts->ts_status & ATH9K_TXERR_FILT)
		TX_STAT_INC(sc, qnum, txerr_filtered);
	if (ts->ts_status & ATH9K_TXERR_FIFO)
		TX_STAT_INC(sc, qnum, fifo_underrun);
	if (ts->ts_status & ATH9K_TXERR_XTXOP)
		TX_STAT_INC(sc, qnum, xtxop);
	if (ts->ts_status & ATH9K_TXERR_TIMER_EXPIRED)
		TX_STAT_INC(sc, qnum, timer_exp);
	if (ts->ts_flags & ATH9K_TX_DESC_CFG_ERR)
		TX_STAT_INC(sc, qnum, desc_cfg_err);
	if (ts->ts_flags & ATH9K_TX_DATA_UNDERRUN)
		TX_STAT_INC(sc, qnum, data_underrun);
	if (ts->ts_flags & ATH9K_TX_DELIM_UNDERRUN)
		TX_STAT_INC(sc, qnum, delim_underrun);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ath_debug_stat_rx(struct ath_softc *sc, struct ath_rx_status *rs)
{
<<<<<<< HEAD
#define RX_STAT_INC(c) sc->debug.stats.rxstats.c++
#define RX_PHY_ERR_INC(c) sc->debug.stats.rxstats.phy_err_stats[c]++
#define RX_SAMP_DBG(c) (sc->debug.bb_mac_samp[sc->debug.sampidx].rs\
			[sc->debug.rsidx].c)

	RX_STAT_INC(rx_pkts_all);
	sc->debug.stats.rxstats.rx_bytes_all += rs->rs_datalen;

	if (rs->rs_status & ATH9K_RXERR_CRC)
		RX_STAT_INC(crc_err);
	if (rs->rs_status & ATH9K_RXERR_DECRYPT)
		RX_STAT_INC(decrypt_crc_err);
	if (rs->rs_status & ATH9K_RXERR_MIC)
		RX_STAT_INC(mic_err);
	if (rs->rs_status & ATH9K_RX_DELIM_CRC_PRE)
		RX_STAT_INC(pre_delim_crc_err);
	if (rs->rs_status & ATH9K_RX_DELIM_CRC_POST)
		RX_STAT_INC(post_delim_crc_err);
	if (rs->rs_status & ATH9K_RX_DECRYPT_BUSY)
		RX_STAT_INC(decrypt_busy_err);

	if (rs->rs_status & ATH9K_RXERR_PHY) {
		RX_STAT_INC(phy_err);
		if (rs->rs_phyerr < ATH9K_PHYERR_MAX)
			RX_PHY_ERR_INC(rs->rs_phyerr);
	}

#ifdef CONFIG_ATH9K_MAC_DEBUG
	spin_lock(&sc->debug.samp_lock);
	RX_SAMP_DBG(jiffies) = jiffies;
	RX_SAMP_DBG(rssi_ctl0) = rs->rs_rssi_ctl0;
	RX_SAMP_DBG(rssi_ctl1) = rs->rs_rssi_ctl1;
	RX_SAMP_DBG(rssi_ctl2) = rs->rs_rssi_ctl2;
	RX_SAMP_DBG(rssi_ext0) = rs->rs_rssi_ext0;
	RX_SAMP_DBG(rssi_ext1) = rs->rs_rssi_ext1;
	RX_SAMP_DBG(rssi_ext2) = rs->rs_rssi_ext2;
	RX_SAMP_DBG(antenna) = rs->rs_antenna;
	RX_SAMP_DBG(rssi) = rs->rs_rssi;
	RX_SAMP_DBG(rate) = rs->rs_rate;
	RX_SAMP_DBG(is_mybeacon) = rs->is_mybeacon;

	sc->debug.rsidx = (sc->debug.rsidx + 1) % ATH_DBG_MAX_SAMPLES;
	spin_unlock(&sc->debug.samp_lock);

#endif

#undef RX_STAT_INC
#undef RX_PHY_ERR_INC
#undef RX_SAMP_DBG
}

static const struct file_operations fops_recv = {
	.read = read_file_recv,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_regidx(struct file *file, char __user *user_buf,
                                size_t count, loff_t *ppos)
=======
	ath9k_cmn_debug_stat_rx(&sc->debug.stats.rxstats, rs);
}

static ssize_t read_file_regidx(struct file *file, char __user *user_buf,
				size_t count, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_softc *sc = file->private_data;
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "0x%08x\n", sc->debug.regidx);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_regidx(struct file *file, const char __user *user_buf,
<<<<<<< HEAD
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	unsigned long regidx;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &regidx))
		return -EINVAL;
=======
				 size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	unsigned long regidx;
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &regidx);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sc->debug.regidx = regidx;
	return count;
}

static const struct file_operations fops_regidx = {
	.read = read_file_regidx,
	.write = write_file_regidx,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_regval(struct file *file, char __user *user_buf,
<<<<<<< HEAD
			     size_t count, loff_t *ppos)
=======
				size_t count, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	char buf[32];
	unsigned int len;
	u32 regval;

	ath9k_ps_wakeup(sc);
	regval = REG_READ_D(ah, sc->debug.regidx);
	ath9k_ps_restore(sc);
	len = sprintf(buf, "0x%08x\n", regval);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_regval(struct file *file, const char __user *user_buf,
<<<<<<< HEAD
			     size_t count, loff_t *ppos)
=======
				 size_t count, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	unsigned long regval;
<<<<<<< HEAD
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strict_strtoul(buf, 0, &regval))
		return -EINVAL;
=======
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &regval);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath9k_ps_wakeup(sc);
	REG_WRITE_D(ah, sc->debug.regidx, regval);
	ath9k_ps_restore(sc);
	return count;
}

static const struct file_operations fops_regval = {
	.read = read_file_regval,
	.write = write_file_regval,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

#define REGDUMP_LINE_SIZE	20

static int open_file_regdump(struct inode *inode, struct file *file)
{
	struct ath_softc *sc = inode->i_private;
	unsigned int len = 0;
	u8 *buf;
<<<<<<< HEAD
	int i;
	unsigned long num_regs, regdump_len, max_reg_offset;

	max_reg_offset = AR_SREV_9300_20_OR_LATER(sc->sc_ah) ? 0x16bd4 : 0xb500;
=======
	int i, j = 0;
	unsigned long num_regs, regdump_len, max_reg_offset;
	static const struct reg_hole {
		u32 start;
		u32 end;
	} reg_hole_list[] = {
		{0x0200, 0x07fc},
		{0x0c00, 0x0ffc},
		{0x2000, 0x3ffc},
		{0x4100, 0x6ffc},
		{0x705c, 0x7ffc},
		{0x0000, 0x0000}
	};

	max_reg_offset = AR_SREV_9300_20_OR_LATER(sc->sc_ah) ? 0x8800 : 0xb500;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	num_regs = max_reg_offset / 4 + 1;
	regdump_len = num_regs * REGDUMP_LINE_SIZE + 1;
	buf = vmalloc(regdump_len);
	if (!buf)
		return -ENOMEM;

	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	for (i = 0; i < num_regs; i++)
		len += scnprintf(buf + len, regdump_len - len,
			"0x%06x 0x%08x\n", i << 2, REG_READ(sc->sc_ah, i << 2));
=======
	for (i = 0; i < num_regs; i++) {
		if (reg_hole_list[j].start == i << 2) {
			i = reg_hole_list[j].end >> 2;
			j++;
			continue;
		}

		len += scnprintf(buf + len, regdump_len - len,
			"0x%06x 0x%08x\n", i << 2, REG_READ(sc->sc_ah, i << 2));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_restore(sc);

	file->private_data = buf;

	return 0;
}

static const struct file_operations fops_regdump = {
	.open = open_file_regdump,
	.read = ath9k_debugfs_read_buf,
	.release = ath9k_debugfs_release_buf,
	.owner = THIS_MODULE,
	.llseek = default_llseek,/* read accesses f_pos */
};

<<<<<<< HEAD
static ssize_t read_file_dump_nfcal(struct file *file, char __user *user_buf,
				    size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_nfcal_hist *h = sc->caldata.nfCalHist;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_conf *conf = &common->hw->conf;
	u32 len = 0, size = 1500;
	u32 i, j;
	ssize_t retval = 0;
	char *buf;
	u8 chainmask = (ah->rxchainmask << 3) | ah->rxchainmask;
	u8 nread;

	buf = kzalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len += snprintf(buf + len, size - len,
			"Channel Noise Floor : %d\n", ah->noise);
	len += snprintf(buf + len, size - len,
			"Chain | privNF | # Readings | NF Readings\n");
=======
static int read_file_dump_nfcal(struct seq_file *file, void *data)
{
	struct ieee80211_hw *hw = dev_get_drvdata(file->private);
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_nfcal_hist *h = sc->cur_chan->caldata.nfCalHist;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_conf *conf = &common->hw->conf;
	u32 i, j;
	u8 chainmask = (ah->rxchainmask << 3) | ah->rxchainmask;
	u8 nread;

	seq_printf(file, "Channel Noise Floor : %d\n", ah->noise);
	seq_puts(file, "Chain | privNF | # Readings | NF Readings\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < NUM_NF_READINGS; i++) {
		if (!(chainmask & (1 << i)) ||
		    ((i >= AR5416_MAX_CHAINS) && !conf_is_ht40(conf)))
			continue;

		nread = AR_PHY_CCA_FILTERWINDOW_LENGTH - h[i].invalidNFcount;
<<<<<<< HEAD
		len += snprintf(buf + len, size - len, " %d\t %d\t %d\t\t",
				i, h[i].privNF, nread);
		for (j = 0; j < nread; j++)
			len += snprintf(buf + len, size - len,
					" %d", h[i].nfCalBuffer[j]);
		len += snprintf(buf + len, size - len, "\n");
	}

=======
		seq_printf(file, " %d\t %d\t %d\t\t", i, h[i].privNF, nread);
		for (j = 0; j < nread; j++)
			seq_printf(file, " %d", h[i].nfCalBuffer[j]);
		seq_puts(file, "\n");
	}

	return 0;
}

#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT
static ssize_t read_file_btcoex(struct file *file, char __user *user_buf,
				size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	u32 len = 0, size = 1500;
	char *buf;
	size_t retval;

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	if (!sc->sc_ah->common.btcoex_enabled) {
		len = scnprintf(buf, size, "%s\n",
				"BTCOEX is disabled");
		goto exit;
	}

	len = ath9k_dump_btcoex(sc, buf, size);
exit:
	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static const struct file_operations fops_btcoex = {
	.read = read_file_btcoex,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};
#endif

#ifdef CONFIG_ATH9K_DYNACK
static ssize_t read_file_ackto(struct file *file, char __user *user_buf,
			       size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "%u %c\n", ah->dynack.ackto,
		      (ah->dynack.enabled) ? 'A' : 'S');

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static const struct file_operations fops_ackto = {
	.read = read_file_ackto,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};
#endif

#ifdef CONFIG_ATH9K_WOW

static ssize_t read_file_wow(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	unsigned int len = 0, size = 32;
	ssize_t retval;
	char *buf;

	buf = kzalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len += scnprintf(buf + len, size - len, "WOW: %s\n",
			 sc->force_wow ? "ENABLED" : "DISABLED");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

<<<<<<< HEAD
static const struct file_operations fops_dump_nfcal = {
	.read = read_file_dump_nfcal,
=======
static ssize_t write_file_wow(struct file *file, const char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	unsigned long val;
	ssize_t ret;

	ret = kstrtoul_from_user(user_buf, count, 0, &val);
	if (ret)
		return ret;

	if (val != 1)
		return -EINVAL;

	if (!sc->force_wow) {
		sc->force_wow = true;
		ath9k_init_wow(sc->hw);
	}

	return count;
}

static const struct file_operations fops_wow = {
	.read = read_file_wow,
	.write = write_file_wow,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

<<<<<<< HEAD
static ssize_t read_file_base_eeprom(struct file *file, char __user *user_buf,
=======
#endif

static ssize_t read_file_tpc(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	unsigned int len = 0, size = 32;
	ssize_t retval;
	char *buf;

	buf = kzalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len += scnprintf(buf + len, size - len, "%s\n",
			 ah->tpc_enabled ? "ENABLED" : "DISABLED");

	if (len > size)
		len = size;

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static ssize_t write_file_tpc(struct file *file, const char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
	unsigned long val;
	ssize_t ret;
	bool tpc_enabled;

	ret = kstrtoul_from_user(user_buf, count, 0, &val);
	if (ret)
		return ret;

	if (val > 1)
		return -EINVAL;

	tpc_enabled = !!val;

	if (tpc_enabled != ah->tpc_enabled) {
		ah->tpc_enabled = tpc_enabled;

		mutex_lock(&sc->mutex);
		ath9k_set_txpower(sc, NULL);
		mutex_unlock(&sc->mutex);
	}

	return count;
}

static const struct file_operations fops_tpc = {
	.read = read_file_tpc,
	.write = write_file_tpc,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_nf_override(struct file *file,
				     char __user *user_buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
	u32 len = 0, size = 1500;
	ssize_t retval = 0;
	char *buf;

	buf = kzalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len = ah->eep_ops->dump_eeprom(ah, true, buf, len, size);

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static const struct file_operations fops_base_eeprom = {
	.read = read_file_base_eeprom,
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

static ssize_t read_file_modal_eeprom(struct file *file, char __user *user_buf,
=======
	char buf[32];
	unsigned int len;

	if (ah->nf_override == 0)
		len = sprintf(buf, "off\n");
	else
		len = sprintf(buf, "%d\n", ah->nf_override);

	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t write_file_nf_override(struct file *file,
				      const char __user *user_buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      size_t count, loff_t *ppos)
{
	struct ath_softc *sc = file->private_data;
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
	u32 len = 0, size = 6000;
	char *buf;
	size_t retval;

	buf = kzalloc(size, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	len = ah->eep_ops->dump_eeprom(ah, false, buf, len, size);

	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);
	kfree(buf);

	return retval;
}

static const struct file_operations fops_modal_eeprom = {
	.read = read_file_modal_eeprom,
=======
	long val;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (strncmp("off", buf, 3) == 0)
		val = 0;
	else if (kstrtol(buf, 0, &val))
		return -EINVAL;

	if (val > 0)
		return -EINVAL;

	if (val < -120)
		return -EINVAL;

	ah->nf_override = val;

	if (ah->curchan) {
		ath9k_ps_wakeup(sc);
		ath9k_hw_loadnf(ah, ah->curchan);
		ath9k_ps_restore(sc);
	}

	return count;
}

static const struct file_operations fops_nf_override = {
	.read = read_file_nf_override,
	.write = write_file_nf_override,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = simple_open,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

<<<<<<< HEAD
#ifdef CONFIG_ATH9K_MAC_DEBUG

void ath9k_debug_samp_bb_mac(struct ath_softc *sc)
{
#define ATH_SAMP_DBG(c) (sc->debug.bb_mac_samp[sc->debug.sampidx].c)
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	unsigned long flags;
	int i;

	ath9k_ps_wakeup(sc);

	spin_lock_bh(&sc->debug.samp_lock);

	spin_lock_irqsave(&common->cc_lock, flags);
	ath_hw_cycle_counters_update(common);

	ATH_SAMP_DBG(cc.cycles) = common->cc_ani.cycles;
	ATH_SAMP_DBG(cc.rx_busy) = common->cc_ani.rx_busy;
	ATH_SAMP_DBG(cc.rx_frame) = common->cc_ani.rx_frame;
	ATH_SAMP_DBG(cc.tx_frame) = common->cc_ani.tx_frame;
	spin_unlock_irqrestore(&common->cc_lock, flags);

	ATH_SAMP_DBG(noise) = ah->noise;

	REG_WRITE_D(ah, AR_MACMISC,
		  ((AR_MACMISC_DMA_OBS_LINE_8 << AR_MACMISC_DMA_OBS_S) |
		   (AR_MACMISC_MISC_OBS_BUS_1 <<
		    AR_MACMISC_MISC_OBS_BUS_MSB_S)));

	for (i = 0; i < ATH9K_NUM_DMA_DEBUG_REGS; i++)
		ATH_SAMP_DBG(dma_dbg_reg_vals[i]) = REG_READ_D(ah,
				AR_DMADBG_0 + (i * sizeof(u32)));

	ATH_SAMP_DBG(pcu_obs) = REG_READ_D(ah, AR_OBS_BUS_1);
	ATH_SAMP_DBG(pcu_cr) = REG_READ_D(ah, AR_CR);

	memcpy(ATH_SAMP_DBG(nfCalHist), sc->caldata.nfCalHist,
			sizeof(ATH_SAMP_DBG(nfCalHist)));

	sc->debug.sampidx = (sc->debug.sampidx + 1) % ATH_DBG_MAX_SAMPLES;
	spin_unlock_bh(&sc->debug.samp_lock);
	ath9k_ps_restore(sc);

#undef ATH_SAMP_DBG
}

static int open_file_bb_mac_samps(struct inode *inode, struct file *file)
{
#define ATH_SAMP_DBG(c) bb_mac_samp[sampidx].c
	struct ath_softc *sc = inode->i_private;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_conf *conf = &common->hw->conf;
	struct ath_dbg_bb_mac_samp *bb_mac_samp;
	struct ath9k_nfcal_hist *h;
	int i, j, qcuOffset = 0, dcuOffset = 0;
	u32 *qcuBase, *dcuBase, size = 30000, len = 0;
	u32 sampidx = 0;
	u8 *buf;
	u8 chainmask = (ah->rxchainmask << 3) | ah->rxchainmask;
	u8 nread;

	if (sc->sc_flags & SC_OP_INVALID)
		return -EAGAIN;

	buf = vmalloc(size);
	if (!buf)
		return -ENOMEM;
	bb_mac_samp = vmalloc(sizeof(*bb_mac_samp) * ATH_DBG_MAX_SAMPLES);
	if (!bb_mac_samp) {
		vfree(buf);
		return -ENOMEM;
	}
	/* Account the current state too */
	ath9k_debug_samp_bb_mac(sc);

	spin_lock_bh(&sc->debug.samp_lock);
	memcpy(bb_mac_samp, sc->debug.bb_mac_samp,
			sizeof(*bb_mac_samp) * ATH_DBG_MAX_SAMPLES);
	len += snprintf(buf + len, size - len,
			"Current Sample Index: %d\n", sc->debug.sampidx);
	spin_unlock_bh(&sc->debug.samp_lock);

	len += snprintf(buf + len, size - len,
			"Raw DMA Debug Dump:\n");
	len += snprintf(buf + len, size - len, "Sample |\t");
	for (i = 0; i < ATH9K_NUM_DMA_DEBUG_REGS; i++)
		len += snprintf(buf + len, size - len, " DMA Reg%d |\t", i);
	len += snprintf(buf + len, size - len, "\n");

	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		len += snprintf(buf + len, size - len, "%d\t", sampidx);

		for (i = 0; i < ATH9K_NUM_DMA_DEBUG_REGS; i++)
			len += snprintf(buf + len, size - len, " %08x\t",
					ATH_SAMP_DBG(dma_dbg_reg_vals[i]));
		len += snprintf(buf + len, size - len, "\n");
	}
	len += snprintf(buf + len, size - len, "\n");

	len += snprintf(buf + len, size - len,
			"Sample Num QCU: chain_st fsp_ok fsp_st DCU: chain_st\n");
	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		qcuBase = &ATH_SAMP_DBG(dma_dbg_reg_vals[0]);
		dcuBase = &ATH_SAMP_DBG(dma_dbg_reg_vals[4]);

		for (i = 0; i < ATH9K_NUM_QUEUES; i++,
				qcuOffset += 4, dcuOffset += 5) {
			if (i == 8) {
				qcuOffset = 0;
				qcuBase++;
			}

			if (i == 6) {
				dcuOffset = 0;
				dcuBase++;
			}
			if (!sc->debug.stats.txstats[i].queued)
				continue;

			len += snprintf(buf + len, size - len,
				"%4d %7d    %2x      %1x     %2x         %2x\n",
				sampidx, i,
				(*qcuBase & (0x7 << qcuOffset)) >> qcuOffset,
				(*qcuBase & (0x8 << qcuOffset)) >>
				(qcuOffset + 3),
				ATH_SAMP_DBG(dma_dbg_reg_vals[2]) &
				(0x7 << (i * 3)) >> (i * 3),
				(*dcuBase & (0x1f << dcuOffset)) >> dcuOffset);
		}
		len += snprintf(buf + len, size - len, "\n");
	}
	len += snprintf(buf + len, size - len,
			"samp qcu_sh qcu_fh qcu_comp dcu_comp dcu_arb dcu_fp "
			"ch_idle_dur ch_idle_dur_val txfifo_val0 txfifo_val1 "
			"txfifo_dcu0 txfifo_dcu1 pcu_obs AR_CR\n");

	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		qcuBase = &ATH_SAMP_DBG(dma_dbg_reg_vals[0]);
		dcuBase = &ATH_SAMP_DBG(dma_dbg_reg_vals[4]);

		len += snprintf(buf + len, size - len, "%4d %5x %5x ", sampidx,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[3]) & 0x003c0000) >> 18,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[3]) & 0x03c00000) >> 22);
		len += snprintf(buf + len, size - len, "%7x %8x ",
			(ATH_SAMP_DBG(dma_dbg_reg_vals[3]) & 0x1c000000) >> 26,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x3));
		len += snprintf(buf + len, size - len, "%7x %7x ",
			(ATH_SAMP_DBG(dma_dbg_reg_vals[5]) & 0x06000000) >> 25,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[5]) & 0x38000000) >> 27);
		len += snprintf(buf + len, size - len, "%7d %12d ",
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x000003fc) >> 2,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x00000400) >> 10);
		len += snprintf(buf + len, size - len, "%12d %12d ",
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x00000800) >> 11,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x00001000) >> 12);
		len += snprintf(buf + len, size - len, "%12d %12d ",
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x0001e000) >> 13,
			(ATH_SAMP_DBG(dma_dbg_reg_vals[6]) & 0x001e0000) >> 17);
		len += snprintf(buf + len, size - len, "0x%07x 0x%07x\n",
				ATH_SAMP_DBG(pcu_obs), ATH_SAMP_DBG(pcu_cr));
	}

	len += snprintf(buf + len, size - len,
			"Sample ChNoise Chain privNF #Reading Readings\n");
	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		h = ATH_SAMP_DBG(nfCalHist);
		if (!ATH_SAMP_DBG(noise))
			continue;

		for (i = 0; i < NUM_NF_READINGS; i++) {
			if (!(chainmask & (1 << i)) ||
			    ((i >= AR5416_MAX_CHAINS) && !conf_is_ht40(conf)))
				continue;

			nread = AR_PHY_CCA_FILTERWINDOW_LENGTH -
				h[i].invalidNFcount;
			len += snprintf(buf + len, size - len,
					"%4d %5d %4d\t   %d\t %d\t",
					sampidx, ATH_SAMP_DBG(noise),
					i, h[i].privNF, nread);
			for (j = 0; j < nread; j++)
				len += snprintf(buf + len, size - len,
					" %d", h[i].nfCalBuffer[j]);
			len += snprintf(buf + len, size - len, "\n");
		}
	}
	len += snprintf(buf + len, size - len, "\nCycle counters:\n"
			"Sample Total    Rxbusy   Rxframes Txframes\n");
	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		if (!ATH_SAMP_DBG(cc.cycles))
			continue;
		len += snprintf(buf + len, size - len,
				"%4d %08x %08x %08x %08x\n",
				sampidx, ATH_SAMP_DBG(cc.cycles),
				ATH_SAMP_DBG(cc.rx_busy),
				ATH_SAMP_DBG(cc.rx_frame),
				ATH_SAMP_DBG(cc.tx_frame));
	}

	len += snprintf(buf + len, size - len, "Tx status Dump :\n");
	len += snprintf(buf + len, size - len,
			"Sample rssi:- ctl0 ctl1 ctl2 ext0 ext1 ext2 comb "
			"isok rts_fail data_fail rate tid qid "
					"ba_low  ba_high tx_before(ms)\n");
	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		for (i = 0; i < ATH_DBG_MAX_SAMPLES; i++) {
			if (!ATH_SAMP_DBG(ts[i].jiffies))
				continue;
			len += snprintf(buf + len, size - len, "%-14d"
				"%-4d %-4d %-4d %-4d %-4d %-4d %-4d %-4d %-8d "
				"%-9d %-4d %-3d %-3d %08x %08x %-11d\n",
				sampidx,
				ATH_SAMP_DBG(ts[i].rssi_ctl0),
				ATH_SAMP_DBG(ts[i].rssi_ctl1),
				ATH_SAMP_DBG(ts[i].rssi_ctl2),
				ATH_SAMP_DBG(ts[i].rssi_ext0),
				ATH_SAMP_DBG(ts[i].rssi_ext1),
				ATH_SAMP_DBG(ts[i].rssi_ext2),
				ATH_SAMP_DBG(ts[i].rssi),
				ATH_SAMP_DBG(ts[i].isok),
				ATH_SAMP_DBG(ts[i].rts_fail_cnt),
				ATH_SAMP_DBG(ts[i].data_fail_cnt),
				ATH_SAMP_DBG(ts[i].rateindex),
				ATH_SAMP_DBG(ts[i].tid),
				ATH_SAMP_DBG(ts[i].qid),
				ATH_SAMP_DBG(ts[i].ba_low),
				ATH_SAMP_DBG(ts[i].ba_high),
				jiffies_to_msecs(jiffies -
					ATH_SAMP_DBG(ts[i].jiffies)));
		}
	}

	len += snprintf(buf + len, size - len, "Rx status Dump :\n");
	len += snprintf(buf + len, size - len, "Sample rssi:- ctl0 ctl1 ctl2 "
			"ext0 ext1 ext2 comb beacon ant rate rx_before(ms)\n");
	for (sampidx = 0; sampidx < ATH_DBG_MAX_SAMPLES; sampidx++) {
		for (i = 0; i < ATH_DBG_MAX_SAMPLES; i++) {
			if (!ATH_SAMP_DBG(rs[i].jiffies))
				continue;
			len += snprintf(buf + len, size - len, "%-14d"
				"%-4d %-4d %-4d %-4d %-4d %-4d %-4d %-9s %-2d %02x %-13d\n",
				sampidx,
				ATH_SAMP_DBG(rs[i].rssi_ctl0),
				ATH_SAMP_DBG(rs[i].rssi_ctl1),
				ATH_SAMP_DBG(rs[i].rssi_ctl2),
				ATH_SAMP_DBG(rs[i].rssi_ext0),
				ATH_SAMP_DBG(rs[i].rssi_ext1),
				ATH_SAMP_DBG(rs[i].rssi_ext2),
				ATH_SAMP_DBG(rs[i].rssi),
				ATH_SAMP_DBG(rs[i].is_mybeacon) ?
				"True" : "False",
				ATH_SAMP_DBG(rs[i].antenna),
				ATH_SAMP_DBG(rs[i].rate),
				jiffies_to_msecs(jiffies -
					ATH_SAMP_DBG(rs[i].jiffies)));
		}
	}

	vfree(bb_mac_samp);
	file->private_data = buf;

	return 0;
#undef ATH_SAMP_DBG
}

static const struct file_operations fops_samps = {
	.open = open_file_bb_mac_samps,
	.read = ath9k_debugfs_read_buf,
	.release = ath9k_debugfs_release_buf,
	.owner = THIS_MODULE,
	.llseek = default_llseek,
};

#endif
=======
/* Ethtool support for get-stats */

#define AMKSTR(nm) #nm "_BE", #nm "_BK", #nm "_VI", #nm "_VO"
static const char ath9k_gstrings_stats[][ETH_GSTRING_LEN] = {
	"tx_pkts_nic",
	"tx_bytes_nic",
	"rx_pkts_nic",
	"rx_bytes_nic",
	AMKSTR(d_tx_pkts),
	AMKSTR(d_tx_bytes),
	AMKSTR(d_tx_mpdus_queued),
	AMKSTR(d_tx_mpdus_completed),
	AMKSTR(d_tx_mpdu_xretries),
	AMKSTR(d_tx_aggregates),
	AMKSTR(d_tx_ampdus_queued_hw),
	AMKSTR(d_tx_ampdus_completed),
	AMKSTR(d_tx_ampdu_retries),
	AMKSTR(d_tx_ampdu_xretries),
	AMKSTR(d_tx_fifo_underrun),
	AMKSTR(d_tx_op_exceeded),
	AMKSTR(d_tx_timer_expiry),
	AMKSTR(d_tx_desc_cfg_err),
	AMKSTR(d_tx_data_underrun),
	AMKSTR(d_tx_delim_underrun),
	"d_rx_crc_err",
	"d_rx_decrypt_crc_err",
	"d_rx_phy_err",
	"d_rx_mic_err",
	"d_rx_pre_delim_crc_err",
	"d_rx_post_delim_crc_err",
	"d_rx_decrypt_busy_err",

	"d_rx_phyerr_radar",
	"d_rx_phyerr_ofdm_timing",
	"d_rx_phyerr_cck_timing",

};
#define ATH9K_SSTATS_LEN ARRAY_SIZE(ath9k_gstrings_stats)

void ath9k_get_et_strings(struct ieee80211_hw *hw,
			  struct ieee80211_vif *vif,
			  u32 sset, u8 *data)
{
	if (sset == ETH_SS_STATS)
		memcpy(data, ath9k_gstrings_stats,
		       sizeof(ath9k_gstrings_stats));
}

int ath9k_get_et_sset_count(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif, int sset)
{
	if (sset == ETH_SS_STATS)
		return ATH9K_SSTATS_LEN;
	return 0;
}

#define AWDATA(elem)							\
	do {								\
		data[i++] = sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BE)].elem; \
		data[i++] = sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BK)].elem; \
		data[i++] = sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VI)].elem; \
		data[i++] = sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VO)].elem; \
	} while (0)

#define AWDATA_RX(elem)						\
	do {							\
		data[i++] = sc->debug.stats.rxstats.elem;	\
	} while (0)

void ath9k_get_et_stats(struct ieee80211_hw *hw,
			struct ieee80211_vif *vif,
			struct ethtool_stats *stats, u64 *data)
{
	struct ath_softc *sc = hw->priv;
	int i = 0;

	data[i++] = (sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BE)].tx_pkts_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BK)].tx_pkts_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VI)].tx_pkts_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VO)].tx_pkts_all);
	data[i++] = (sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BE)].tx_bytes_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_BK)].tx_bytes_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VI)].tx_bytes_all +
		     sc->debug.stats.txstats[PR_QNUM(IEEE80211_AC_VO)].tx_bytes_all);
	AWDATA_RX(rx_pkts_all);
	AWDATA_RX(rx_bytes_all);

	AWDATA(tx_pkts_all);
	AWDATA(tx_bytes_all);
	AWDATA(queued);
	AWDATA(completed);
	AWDATA(xretries);
	AWDATA(a_aggr);
	AWDATA(a_queued_hw);
	AWDATA(a_completed);
	AWDATA(a_retries);
	AWDATA(a_xretries);
	AWDATA(fifo_underrun);
	AWDATA(xtxop);
	AWDATA(timer_exp);
	AWDATA(desc_cfg_err);
	AWDATA(data_underrun);
	AWDATA(delim_underrun);

	AWDATA_RX(crc_err);
	AWDATA_RX(decrypt_crc_err);
	AWDATA_RX(phy_err);
	AWDATA_RX(mic_err);
	AWDATA_RX(pre_delim_crc_err);
	AWDATA_RX(post_delim_crc_err);
	AWDATA_RX(decrypt_busy_err);

	AWDATA_RX(phy_err_stats[ATH9K_PHYERR_RADAR]);
	AWDATA_RX(phy_err_stats[ATH9K_PHYERR_OFDM_TIMING]);
	AWDATA_RX(phy_err_stats[ATH9K_PHYERR_CCK_TIMING]);

	WARN_ON(i != ATH9K_SSTATS_LEN);
}

void ath9k_deinit_debug(struct ath_softc *sc)
{
	ath9k_cmn_spectral_deinit_debug(&sc->spec_priv);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int ath9k_init_debug(struct ath_hw *ah)
{
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	struct ath_softc *sc = (struct ath_softc *) common->priv;

	sc->debug.debugfs_phy = debugfs_create_dir("ath9k",
						   sc->hw->wiphy->debugfsdir);
	if (!sc->debug.debugfs_phy)
		return -ENOMEM;

#ifdef CONFIG_ATH_DEBUG
	debugfs_create_file("debug", S_IRUSR | S_IWUSR, sc->debug.debugfs_phy,
=======
	struct ath_softc *sc = common->priv;

	sc->debug.debugfs_phy = debugfs_create_dir("ath9k",
						   sc->hw->wiphy->debugfsdir);
	if (IS_ERR(sc->debug.debugfs_phy))
		return -ENOMEM;

#ifdef CONFIG_ATH_DEBUG
	debugfs_create_file("debug", 0600, sc->debug.debugfs_phy,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    sc, &fops_debug);
#endif

	ath9k_dfs_init_debug(sc);
<<<<<<< HEAD

	debugfs_create_file("dma", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_dma);
	debugfs_create_file("interrupt", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_interrupt);
	debugfs_create_file("xmit", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_xmit);
	debugfs_create_file("stations", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_stations);
	debugfs_create_file("misc", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_misc);
	debugfs_create_file("reset", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_reset);
	debugfs_create_file("recv", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_recv);
	debugfs_create_file("rx_chainmask", S_IRUSR | S_IWUSR,
			    sc->debug.debugfs_phy, sc, &fops_rx_chainmask);
	debugfs_create_file("tx_chainmask", S_IRUSR | S_IWUSR,
			    sc->debug.debugfs_phy, sc, &fops_tx_chainmask);
	debugfs_create_file("disable_ani", S_IRUSR | S_IWUSR,
			    sc->debug.debugfs_phy, sc, &fops_disable_ani);
	debugfs_create_file("regidx", S_IRUSR | S_IWUSR, sc->debug.debugfs_phy,
			    sc, &fops_regidx);
	debugfs_create_file("regval", S_IRUSR | S_IWUSR, sc->debug.debugfs_phy,
			    sc, &fops_regval);
	debugfs_create_bool("ignore_extcca", S_IRUSR | S_IWUSR,
			    sc->debug.debugfs_phy,
			    &ah->config.cwm_ignore_extcca);
	debugfs_create_file("regdump", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_regdump);
	debugfs_create_file("dump_nfcal", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_dump_nfcal);
	debugfs_create_file("base_eeprom", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_base_eeprom);
	debugfs_create_file("modal_eeprom", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_modal_eeprom);
#ifdef CONFIG_ATH9K_MAC_DEBUG
	debugfs_create_file("samples", S_IRUSR, sc->debug.debugfs_phy, sc,
			    &fops_samps);
#endif

	debugfs_create_u32("gpio_mask", S_IRUSR | S_IWUSR,
			   sc->debug.debugfs_phy, &sc->sc_ah->gpio_mask);

	debugfs_create_u32("gpio_val", S_IRUSR | S_IWUSR,
			   sc->debug.debugfs_phy, &sc->sc_ah->gpio_val);
=======
	ath9k_tx99_init_debug(sc);
	ath9k_cmn_spectral_init_debug(&sc->spec_priv, sc->debug.debugfs_phy);

	debugfs_create_devm_seqfile(sc->dev, "dma", sc->debug.debugfs_phy,
				    read_file_dma);
	debugfs_create_devm_seqfile(sc->dev, "interrupt", sc->debug.debugfs_phy,
				    read_file_interrupt);
	debugfs_create_devm_seqfile(sc->dev, "xmit", sc->debug.debugfs_phy,
				    read_file_xmit);
	debugfs_create_devm_seqfile(sc->dev, "queues", sc->debug.debugfs_phy,
				    read_file_queues);
	debugfs_create_devm_seqfile(sc->dev, "misc", sc->debug.debugfs_phy,
				    read_file_misc);
	debugfs_create_file("reset", 0600, sc->debug.debugfs_phy,
			    sc, &fops_reset);

	ath9k_cmn_debug_recv(sc->debug.debugfs_phy, &sc->debug.stats.rxstats);
	ath9k_cmn_debug_phy_err(sc->debug.debugfs_phy, &sc->debug.stats.rxstats);

	debugfs_create_u8("rx_chainmask", 0400, sc->debug.debugfs_phy,
			  &ah->rxchainmask);
	debugfs_create_u8("tx_chainmask", 0400, sc->debug.debugfs_phy,
			  &ah->txchainmask);
	debugfs_create_file("ani", 0600,
			    sc->debug.debugfs_phy, sc, &fops_ani);
	debugfs_create_bool("paprd", 0600, sc->debug.debugfs_phy,
			    &sc->sc_ah->config.enable_paprd);
	debugfs_create_file("regidx", 0600, sc->debug.debugfs_phy,
			    sc, &fops_regidx);
	debugfs_create_file("regval", 0600, sc->debug.debugfs_phy,
			    sc, &fops_regval);
	debugfs_create_bool("ignore_extcca", 0600,
			    sc->debug.debugfs_phy,
			    &ah->config.cwm_ignore_extcca);
	debugfs_create_file("regdump", 0400, sc->debug.debugfs_phy, sc,
			    &fops_regdump);
	debugfs_create_devm_seqfile(sc->dev, "dump_nfcal",
				    sc->debug.debugfs_phy,
				    read_file_dump_nfcal);

	ath9k_cmn_debug_base_eeprom(sc->debug.debugfs_phy, sc->sc_ah);
	ath9k_cmn_debug_modal_eeprom(sc->debug.debugfs_phy, sc->sc_ah);

	debugfs_create_u32("gpio_mask", 0600,
			   sc->debug.debugfs_phy, &sc->sc_ah->gpio_mask);
	debugfs_create_u32("gpio_val", 0600,
			   sc->debug.debugfs_phy, &sc->sc_ah->gpio_val);
	debugfs_create_file("antenna_diversity", 0400,
			    sc->debug.debugfs_phy, sc, &fops_antenna_diversity);
#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT
	debugfs_create_file("bt_ant_diversity", 0600,
			    sc->debug.debugfs_phy, sc, &fops_bt_ant_diversity);
	debugfs_create_file("btcoex", 0400, sc->debug.debugfs_phy, sc,
			    &fops_btcoex);
#endif

#ifdef CONFIG_ATH9K_WOW
	debugfs_create_file("wow", 0600, sc->debug.debugfs_phy, sc, &fops_wow);
#endif

#ifdef CONFIG_ATH9K_DYNACK
	debugfs_create_file("ack_to", 0400, sc->debug.debugfs_phy,
			    sc, &fops_ackto);
#endif
	debugfs_create_file("tpc", 0600, sc->debug.debugfs_phy, sc, &fops_tpc);

	debugfs_create_file("nf_override", 0600,
			    sc->debug.debugfs_phy, sc, &fops_nf_override);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
