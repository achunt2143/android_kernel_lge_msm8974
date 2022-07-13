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

#include <linux/nl80211.h>
#include <linux/delay.h>
#include "ath9k.h"
#include "btcoex.h"

<<<<<<< HEAD
static u8 parse_mpdudensity(u8 mpdudensity)
=======
static void ath9k_flush(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			u32 queues, bool drop);

u8 ath9k_parse_mpdudensity(u8 mpdudensity)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * 802.11n D2.0 defined values for "Minimum MPDU Start Spacing":
	 *   0 for no restriction
	 *   1 for 1/4 us
	 *   2 for 1/2 us
	 *   3 for 1 us
	 *   4 for 2 us
	 *   5 for 4 us
	 *   6 for 8 us
	 *   7 for 16 us
	 */
	switch (mpdudensity) {
	case 0:
		return 0;
	case 1:
	case 2:
	case 3:
		/* Our lower layer calculations limit our precision to
		   1 microsecond */
		return 1;
	case 4:
		return 2;
	case 5:
		return 4;
	case 6:
		return 8;
	case 7:
		return 16;
	default:
		return 0;
	}
}

<<<<<<< HEAD
static bool ath9k_has_pending_frames(struct ath_softc *sc, struct ath_txq *txq)
=======
static bool ath9k_has_pending_frames(struct ath_softc *sc, struct ath_txq *txq,
				     bool sw_pending)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	bool pending = false;

	spin_lock_bh(&txq->axq_lock);

<<<<<<< HEAD
	if (txq->axq_depth || !list_empty(&txq->axq_acq))
		pending = true;

=======
	if (txq->axq_depth) {
		pending = true;
		goto out;
	}

	if (!sw_pending)
		goto out;

	if (txq->mac80211_qnum >= 0) {
		struct ath_acq *acq;

		acq = &sc->cur_chan->acq[txq->mac80211_qnum];
		if (!list_empty(&acq->acq_new) || !list_empty(&acq->acq_old))
			pending = true;
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_bh(&txq->axq_lock);
	return pending;
}

static bool ath9k_setpower(struct ath_softc *sc, enum ath9k_power_mode mode)
{
	unsigned long flags;
	bool ret;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	ret = ath9k_hw_setpower(sc->sc_ah, mode);
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);

	return ret;
}

<<<<<<< HEAD
=======
void ath_ps_full_sleep(struct timer_list *t)
{
	struct ath_softc *sc = from_timer(sc, t, sleep_timer);
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long flags;
	bool reset;

	spin_lock_irqsave(&common->cc_lock, flags);
	ath_hw_cycle_counters_update(common);
	spin_unlock_irqrestore(&common->cc_lock, flags);

	ath9k_hw_setrxabort(sc->sc_ah, 1);
	ath9k_hw_stopdmarecv(sc->sc_ah, &reset);

	ath9k_hw_setpower(sc->sc_ah, ATH9K_PM_FULL_SLEEP);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ath9k_ps_wakeup(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long flags;
	enum ath9k_power_mode power_mode;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	if (++sc->ps_usecount != 1)
		goto unlock;

<<<<<<< HEAD
=======
	del_timer_sync(&sc->sleep_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	power_mode = sc->sc_ah->power_mode;
	ath9k_hw_setpower(sc->sc_ah, ATH9K_PM_AWAKE);

	/*
	 * While the hardware is asleep, the cycle counters contain no
	 * useful data. Better clear them now so that they don't mess up
	 * survey data results.
	 */
	if (power_mode != ATH9K_PM_AWAKE) {
		spin_lock(&common->cc_lock);
		ath_hw_cycle_counters_update(common);
		memset(&common->cc_survey, 0, sizeof(common->cc_survey));
<<<<<<< HEAD
=======
		memset(&common->cc_ani, 0, sizeof(common->cc_ani));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&common->cc_lock);
	}

 unlock:
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
}

void ath9k_ps_restore(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	enum ath9k_power_mode mode;
	unsigned long flags;

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
	if (--sc->ps_usecount != 0)
		goto unlock;

<<<<<<< HEAD
	if (sc->ps_idle && (sc->ps_flags & PS_WAIT_FOR_TX_ACK))
		mode = ATH9K_PM_FULL_SLEEP;
	else if (sc->ps_enabled &&
		 !(sc->ps_flags & (PS_WAIT_FOR_BEACON |
			      PS_WAIT_FOR_CAB |
			      PS_WAIT_FOR_PSPOLL_DATA |
			      PS_WAIT_FOR_TX_ACK)))
		mode = ATH9K_PM_NETWORK_SLEEP;
	else
		goto unlock;
=======
	if (sc->ps_idle) {
		mod_timer(&sc->sleep_timer, jiffies + HZ / 10);
		goto unlock;
	}

	if (sc->ps_enabled &&
		   !(sc->ps_flags & (PS_WAIT_FOR_BEACON |
				     PS_WAIT_FOR_CAB |
				     PS_WAIT_FOR_PSPOLL_DATA |
				     PS_WAIT_FOR_TX_ACK |
				     PS_WAIT_FOR_ANI))) {
		mode = ATH9K_PM_NETWORK_SLEEP;
		if (ath9k_hw_btcoex_is_enabled(sc->sc_ah))
			ath9k_btcoex_stop_gen_timer(sc);
	} else {
		goto unlock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&common->cc_lock);
	ath_hw_cycle_counters_update(common);
	spin_unlock(&common->cc_lock);

	ath9k_hw_setpower(sc->sc_ah, mode);

 unlock:
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
}

<<<<<<< HEAD
void ath_start_ani(struct ath_common *common)
{
	struct ath_hw *ah = common->ah;
	unsigned long timestamp = jiffies_to_msecs(jiffies);
	struct ath_softc *sc = (struct ath_softc *) common->priv;

	if (!(sc->sc_flags & SC_OP_ANI_RUN))
		return;

	if (sc->sc_flags & SC_OP_OFFCHANNEL)
		return;

	common->ani.longcal_timer = timestamp;
	common->ani.shortcal_timer = timestamp;
	common->ani.checkani_timer = timestamp;

	mod_timer(&common->ani.timer,
		  jiffies +
			msecs_to_jiffies((u32)ah->config.ani_poll_interval));
}

static void ath_update_survey_nf(struct ath_softc *sc, int channel)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_channel *chan = &ah->channels[channel];
	struct survey_info *survey = &sc->survey[channel];

	if (chan->noisefloor) {
		survey->filled |= SURVEY_INFO_NOISE_DBM;
		survey->noise = ath9k_hw_getchan_noise(ah, chan);
	}
}

/*
 * Updates the survey statistics and returns the busy time since last
 * update in %, if the measurement duration was long enough for the
 * result to be useful, -1 otherwise.
 */
static int ath_update_survey_stats(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	int pos = ah->curchan - &ah->channels[0];
	struct survey_info *survey = &sc->survey[pos];
	struct ath_cycle_counters *cc = &common->cc_survey;
	unsigned int div = common->clockrate * 1000;
	int ret = 0;

	if (!ah->curchan)
		return -1;

	if (ah->power_mode == ATH9K_PM_AWAKE)
		ath_hw_cycle_counters_update(common);

	if (cc->cycles > 0) {
		survey->filled |= SURVEY_INFO_CHANNEL_TIME |
			SURVEY_INFO_CHANNEL_TIME_BUSY |
			SURVEY_INFO_CHANNEL_TIME_RX |
			SURVEY_INFO_CHANNEL_TIME_TX;
		survey->channel_time += cc->cycles / div;
		survey->channel_time_busy += cc->rx_busy / div;
		survey->channel_time_rx += cc->rx_frame / div;
		survey->channel_time_tx += cc->tx_frame / div;
	}

	if (cc->cycles < div)
		return -1;

	if (cc->cycles > 0)
		ret = cc->rx_busy * 100 / cc->cycles;

	memset(cc, 0, sizeof(*cc));

	ath_update_survey_nf(sc, pos);

	return ret;
}

static void __ath_cancel_work(struct ath_softc *sc)
{
	cancel_work_sync(&sc->paprd_work);
	cancel_work_sync(&sc->hw_check_work);
	cancel_delayed_work_sync(&sc->tx_complete_work);
	cancel_delayed_work_sync(&sc->hw_pll_work);
}

static void ath_cancel_work(struct ath_softc *sc)
=======
static void __ath_cancel_work(struct ath_softc *sc)
{
	cancel_work_sync(&sc->paprd_work);
	cancel_delayed_work_sync(&sc->hw_check_work);
	cancel_delayed_work_sync(&sc->hw_pll_work);

#ifdef CONFIG_ATH9K_BTCOEX_SUPPORT
	if (ath9k_hw_mci_is_enabled(sc->sc_ah))
		cancel_work_sync(&sc->mci_work);
#endif
}

void ath_cancel_work(struct ath_softc *sc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__ath_cancel_work(sc);
	cancel_work_sync(&sc->hw_reset_work);
}

<<<<<<< HEAD
static bool ath_prepare_reset(struct ath_softc *sc, bool retry_tx, bool flush)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	bool ret = true;

	ieee80211_stop_queues(sc->hw);

	sc->hw_busy_count = 0;
	del_timer_sync(&common->ani.timer);

	ath9k_debug_samp_bb_mac(sc);
=======
void ath_restart_work(struct ath_softc *sc)
{
	ieee80211_queue_delayed_work(sc->hw, &sc->hw_check_work,
				     msecs_to_jiffies(ATH_HW_CHECK_POLL_INT));

	if (AR_SREV_9340(sc->sc_ah) || AR_SREV_9330(sc->sc_ah))
		ieee80211_queue_delayed_work(sc->hw, &sc->hw_pll_work,
				     msecs_to_jiffies(ATH_PLL_WORK_INTERVAL));

	ath_start_ani(sc);
}

static bool ath_prepare_reset(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	bool ret = true;

	ieee80211_stop_queues(sc->hw);
	ath_stop_ani(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_hw_disable_interrupts(ah);

	if (AR_SREV_9300_20_OR_LATER(ah)) {
		ret &= ath_stoprecv(sc);
<<<<<<< HEAD
		ret &= ath_drain_all_txq(sc, retry_tx);
	} else {
		ret &= ath_drain_all_txq(sc, retry_tx);
		ret &= ath_stoprecv(sc);
	}

	if (!flush) {
		if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
			ath_rx_tasklet(sc, 1, true);
		ath_rx_tasklet(sc, 1, false);
	} else {
		ath_flushrecv(sc);
	}

=======
		ret &= ath_drain_all_txq(sc);
	} else {
		ret &= ath_drain_all_txq(sc);
		ret &= ath_stoprecv(sc);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static bool ath_complete_reset(struct ath_softc *sc, bool start)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD

	if (ath_startrecv(sc) != 0) {
		ath_err(common, "Unable to restart recv logic\n");
		return false;
	}

	ath9k_cmn_update_txpow(ah, sc->curtxpow,
			       sc->config.txpowlimit, &sc->curtxpow);
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	if (!(sc->sc_flags & (SC_OP_OFFCHANNEL)) && start) {
		if (sc->sc_flags & SC_OP_BEACONS)
			ath_set_beacon(sc);

		ieee80211_queue_delayed_work(sc->hw, &sc->tx_complete_work, 0);
		ieee80211_queue_delayed_work(sc->hw, &sc->hw_pll_work, HZ/2);
		if (!common->disable_ani)
			ath_start_ani(common);
	}

	if ((ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB) && sc->ant_rx != 3) {
		struct ath_hw_antcomb_conf div_ant_conf;
		u8 lna_conf;

		ath9k_hw_antdiv_comb_conf_get(ah, &div_ant_conf);

		if (sc->ant_rx == 1)
			lna_conf = ATH_ANT_DIV_COMB_LNA1;
		else
			lna_conf = ATH_ANT_DIV_COMB_LNA2;
		div_ant_conf.main_lna_conf = lna_conf;
		div_ant_conf.alt_lna_conf = lna_conf;

		ath9k_hw_antdiv_comb_conf_set(ah, &div_ant_conf);
	}

	ieee80211_wake_queues(sc->hw);
=======
	unsigned long flags;

	ath9k_calculate_summary_state(sc, sc->cur_chan);
	ath_startrecv(sc);
	ath9k_cmn_update_txpow(ah, sc->cur_chan->cur_txpower,
			       sc->cur_chan->txpower,
			       &sc->cur_chan->cur_txpower);
	clear_bit(ATH_OP_HW_RESET, &common->op_flags);

	if (!sc->cur_chan->offchannel && start) {
		/* restore per chanctx TSF timer */
		if (sc->cur_chan->tsf_val) {
			u32 offset;

			offset = ath9k_hw_get_tsf_offset(&sc->cur_chan->tsf_ts,
							 NULL);
			ath9k_hw_settsf64(ah, sc->cur_chan->tsf_val + offset);
		}


		if (!test_bit(ATH_OP_BEACONS, &common->op_flags))
			goto work;

		if (ah->opmode == NL80211_IFTYPE_STATION &&
		    test_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags)) {
			spin_lock_irqsave(&sc->sc_pm_lock, flags);
			sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
			spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
		} else {
			ath9k_set_beacon(sc);
		}
	work:
		ath_restart_work(sc);
		ath_txq_schedule_all(sc);
	}

	sc->gtt_cnt = 0;

	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);
	ieee80211_wake_queues(sc->hw);
	ath9k_p2p_ps_timer(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return true;
}

<<<<<<< HEAD
static int ath_reset_internal(struct ath_softc *sc, struct ath9k_channel *hchan,
			      bool retry_tx)
=======
static int ath_reset_internal(struct ath_softc *sc, struct ath9k_channel *hchan)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_hw_cal_data *caldata = NULL;
	bool fastcc = true;
<<<<<<< HEAD
	bool flush = false;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int r;

	__ath_cancel_work(sc);

<<<<<<< HEAD
	spin_lock_bh(&sc->sc_pcu_lock);

	if (!(sc->sc_flags & SC_OP_OFFCHANNEL)) {
		fastcc = false;
		caldata = &sc->caldata;
=======
	disable_irq(sc->irq);
	tasklet_disable(&sc->intr_tq);
	tasklet_disable(&sc->bcon_tasklet);
	spin_lock_bh(&sc->sc_pcu_lock);

	if (!sc->cur_chan->offchannel) {
		fastcc = false;
		caldata = &sc->cur_chan->caldata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!hchan) {
		fastcc = false;
<<<<<<< HEAD
		flush = true;
		hchan = ah->curchan;
	}

	if (!ath_prepare_reset(sc, retry_tx, flush))
		fastcc = false;
=======
		hchan = ah->curchan;
	}

	if (!hchan) {
		fastcc = false;
		hchan = ath9k_cmn_get_channel(sc->hw, ah, &sc->cur_chan->chandef);
	}

	if (!ath_prepare_reset(sc))
		fastcc = false;

	if (ath9k_is_chanctx_enabled())
		fastcc = false;

	spin_lock_bh(&sc->chan_lock);
	sc->cur_chandef = sc->cur_chan->chandef;
	spin_unlock_bh(&sc->chan_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath_dbg(common, CONFIG, "Reset to %u MHz, HT40: %d fastcc: %d\n",
		hchan->channel, IS_CHAN_HT40(hchan), fastcc);

	r = ath9k_hw_reset(ah, hchan, caldata, fastcc);
	if (r) {
		ath_err(common,
			"Unable to reset channel, reset status %d\n", r);
<<<<<<< HEAD
		goto out;
	}

=======

		ath9k_hw_enable_interrupts(ah);
		ath9k_queue_reset(sc, RESET_TYPE_BB_HANG);

		goto out;
	}

	if (ath9k_hw_mci_is_enabled(sc->sc_ah) &&
	    sc->cur_chan->offchannel)
		ath9k_mci_set_txpower(sc, true, false);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ath_complete_reset(sc, true))
		r = -EIO;

out:
<<<<<<< HEAD
	spin_unlock_bh(&sc->sc_pcu_lock);
	return r;
}


/*
 * Set/change channels.  If the channel is really being changed, it's done
 * by reseting the chip.  To accomplish this we must first cleanup any pending
 * DMA, then restart stuff.
*/
static int ath_set_channel(struct ath_softc *sc, struct ieee80211_hw *hw,
		    struct ath9k_channel *hchan)
{
	int r;

	if (sc->sc_flags & SC_OP_INVALID)
		return -EIO;

	r = ath_reset_internal(sc, hchan, false);
=======
	enable_irq(sc->irq);
	spin_unlock_bh(&sc->sc_pcu_lock);
	tasklet_enable(&sc->bcon_tasklet);
	tasklet_enable(&sc->intr_tq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return r;
}

<<<<<<< HEAD
static void ath_paprd_activate(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_hw_cal_data *caldata = ah->caldata;
	int chain;

	if (!caldata || !caldata->paprd_done)
		return;

	ath9k_ps_wakeup(sc);
	ar9003_paprd_enable(ah, false);
	for (chain = 0; chain < AR9300_MAX_CHAINS; chain++) {
		if (!(ah->txchainmask & BIT(chain)))
			continue;

		ar9003_paprd_populate_single_table(ah, caldata, chain);
	}

	ar9003_paprd_enable(ah, true);
	ath9k_ps_restore(sc);
}

static bool ath_paprd_send_frame(struct ath_softc *sc, struct sk_buff *skb, int chain)
{
	struct ieee80211_hw *hw = sc->hw;
	struct ieee80211_tx_info *tx_info = IEEE80211_SKB_CB(skb);
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_tx_control txctl;
	int time_left;

	memset(&txctl, 0, sizeof(txctl));
	txctl.txq = sc->tx.txq_map[WME_AC_BE];

	memset(tx_info, 0, sizeof(*tx_info));
	tx_info->band = hw->conf.channel->band;
	tx_info->flags |= IEEE80211_TX_CTL_NO_ACK;
	tx_info->control.rates[0].idx = 0;
	tx_info->control.rates[0].count = 1;
	tx_info->control.rates[0].flags = IEEE80211_TX_RC_MCS;
	tx_info->control.rates[1].idx = -1;

	init_completion(&sc->paprd_complete);
	txctl.paprd = BIT(chain);

	if (ath_tx_start(hw, skb, &txctl) != 0) {
		ath_dbg(common, CALIBRATE, "PAPRD TX failed\n");
		dev_kfree_skb_any(skb);
		return false;
	}

	time_left = wait_for_completion_timeout(&sc->paprd_complete,
			msecs_to_jiffies(ATH_PAPRD_TIMEOUT));

	if (!time_left)
		ath_dbg(common, CALIBRATE,
			"Timeout waiting for paprd training on TX chain %d\n",
			chain);

	return !!time_left;
}

void ath_paprd_calibrate(struct work_struct *work)
{
	struct ath_softc *sc = container_of(work, struct ath_softc, paprd_work);
	struct ieee80211_hw *hw = sc->hw;
	struct ath_hw *ah = sc->sc_ah;
	struct ieee80211_hdr *hdr;
	struct sk_buff *skb = NULL;
	struct ath9k_hw_cal_data *caldata = ah->caldata;
	struct ath_common *common = ath9k_hw_common(ah);
	int ftype;
	int chain_ok = 0;
	int chain;
	int len = 1800;

	if (!caldata)
		return;

	ath9k_ps_wakeup(sc);

	if (ar9003_paprd_init_table(ah) < 0)
		goto fail_paprd;

	skb = alloc_skb(len, GFP_KERNEL);
	if (!skb)
		goto fail_paprd;

	skb_put(skb, len);
	memset(skb->data, 0, len);
	hdr = (struct ieee80211_hdr *)skb->data;
	ftype = IEEE80211_FTYPE_DATA | IEEE80211_STYPE_NULLFUNC;
	hdr->frame_control = cpu_to_le16(ftype);
	hdr->duration_id = cpu_to_le16(10);
	memcpy(hdr->addr1, hw->wiphy->perm_addr, ETH_ALEN);
	memcpy(hdr->addr2, hw->wiphy->perm_addr, ETH_ALEN);
	memcpy(hdr->addr3, hw->wiphy->perm_addr, ETH_ALEN);

	for (chain = 0; chain < AR9300_MAX_CHAINS; chain++) {
		if (!(ah->txchainmask & BIT(chain)))
			continue;

		chain_ok = 0;

		ath_dbg(common, CALIBRATE,
			"Sending PAPRD frame for thermal measurement on chain %d\n",
			chain);
		if (!ath_paprd_send_frame(sc, skb, chain))
			goto fail_paprd;

		ar9003_paprd_setup_gain_table(ah, chain);

		ath_dbg(common, CALIBRATE,
			"Sending PAPRD training frame on chain %d\n", chain);
		if (!ath_paprd_send_frame(sc, skb, chain))
			goto fail_paprd;

		if (!ar9003_paprd_is_done(ah)) {
			ath_dbg(common, CALIBRATE,
				"PAPRD not yet done on chain %d\n", chain);
			break;
		}

		if (ar9003_paprd_create_curve(ah, caldata, chain)) {
			ath_dbg(common, CALIBRATE,
				"PAPRD create curve failed on chain %d\n",
								   chain);
			break;
		}

		chain_ok = 1;
	}
	kfree_skb(skb);

	if (chain_ok) {
		caldata->paprd_done = true;
		ath_paprd_activate(sc);
	}

fail_paprd:
	ath9k_ps_restore(sc);
}

/*
 *  This routine performs the periodic noise floor calibration function
 *  that is used to adjust and optimize the chip performance.  This
 *  takes environmental changes (location, temperature) into account.
 *  When the task is complete, it reschedules itself depending on the
 *  appropriate interval that was calculated.
 */
void ath_ani_calibrate(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *)data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	bool longcal = false;
	bool shortcal = false;
	bool aniflag = false;
	unsigned int timestamp = jiffies_to_msecs(jiffies);
	u32 cal_interval, short_cal_interval, long_cal_interval;
	unsigned long flags;

	if (ah->caldata && ah->caldata->nfcal_interference)
		long_cal_interval = ATH_LONG_CALINTERVAL_INT;
	else
		long_cal_interval = ATH_LONG_CALINTERVAL;

	short_cal_interval = (ah->opmode == NL80211_IFTYPE_AP) ?
		ATH_AP_SHORT_CALINTERVAL : ATH_STA_SHORT_CALINTERVAL;

	/* Only calibrate if awake */
	if (sc->sc_ah->power_mode != ATH9K_PM_AWAKE)
		goto set_timer;

	ath9k_ps_wakeup(sc);

	/* Long calibration runs independently of short calibration. */
	if ((timestamp - common->ani.longcal_timer) >= long_cal_interval) {
		longcal = true;
		common->ani.longcal_timer = timestamp;
	}

	/* Short calibration applies only while caldone is false */
	if (!common->ani.caldone) {
		if ((timestamp - common->ani.shortcal_timer) >= short_cal_interval) {
			shortcal = true;
			common->ani.shortcal_timer = timestamp;
			common->ani.resetcal_timer = timestamp;
		}
	} else {
		if ((timestamp - common->ani.resetcal_timer) >=
		    ATH_RESTART_CALINTERVAL) {
			common->ani.caldone = ath9k_hw_reset_calvalid(ah);
			if (common->ani.caldone)
				common->ani.resetcal_timer = timestamp;
		}
	}

	/* Verify whether we must check ANI */
	if (sc->sc_ah->config.enable_ani
	    && (timestamp - common->ani.checkani_timer) >=
	    ah->config.ani_poll_interval) {
		aniflag = true;
		common->ani.checkani_timer = timestamp;
	}

	/* Call ANI routine if necessary */
	if (aniflag) {
		spin_lock_irqsave(&common->cc_lock, flags);
		ath9k_hw_ani_monitor(ah, ah->curchan);
		ath_update_survey_stats(sc);
		spin_unlock_irqrestore(&common->cc_lock, flags);
	}

	/* Perform calibration if necessary */
	if (longcal || shortcal) {
		common->ani.caldone =
			ath9k_hw_calibrate(ah, ah->curchan,
						ah->rxchainmask, longcal);
	}

	ath_dbg(common, ANI,
		"Calibration @%lu finished: %s %s %s, caldone: %s\n",
		jiffies,
		longcal ? "long" : "", shortcal ? "short" : "",
		aniflag ? "ani" : "", common->ani.caldone ? "true" : "false");

	ath9k_ps_restore(sc);

set_timer:
	/*
	* Set timer interval based on previous results.
	* The interval must be the shortest necessary to satisfy ANI,
	* short calibration and long calibration.
	*/
	ath9k_debug_samp_bb_mac(sc);
	cal_interval = ATH_LONG_CALINTERVAL;
	if (sc->sc_ah->config.enable_ani)
		cal_interval = min(cal_interval,
				   (u32)ah->config.ani_poll_interval);
	if (!common->ani.caldone)
		cal_interval = min(cal_interval, (u32)short_cal_interval);

	mod_timer(&common->ani.timer, jiffies + msecs_to_jiffies(cal_interval));
	if ((sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_PAPRD) && ah->caldata) {
		if (!ah->caldata->paprd_done)
			ieee80211_queue_work(sc->hw, &sc->paprd_work);
		else if (!ah->paprd_table_write_done)
			ath_paprd_activate(sc);
	}
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ath_node_attach(struct ath_softc *sc, struct ieee80211_sta *sta,
			    struct ieee80211_vif *vif)
{
	struct ath_node *an;
	an = (struct ath_node *)sta->drv_priv;

<<<<<<< HEAD
#ifdef CONFIG_ATH9K_DEBUGFS
	spin_lock(&sc->nodes_lock);
	list_add(&an->list, &sc->nodes);
	spin_unlock(&sc->nodes_lock);
#endif
	an->sta = sta;
	an->vif = vif;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_HT) {
		ath_tx_node_init(sc, an);
		an->maxampdu = 1 << (IEEE80211_HT_MAX_AMPDU_FACTOR +
				     sta->ht_cap.ampdu_factor);
		an->mpdudensity = parse_mpdudensity(sta->ht_cap.ampdu_density);
	}
=======
	an->sc = sc;
	an->sta = sta;
	an->vif = vif;
	memset(&an->key_idx, 0, sizeof(an->key_idx));

	ath_tx_node_init(sc, an);

	ath_dynack_node_init(sc->sc_ah, an);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ath_node_detach(struct ath_softc *sc, struct ieee80211_sta *sta)
{
	struct ath_node *an = (struct ath_node *)sta->drv_priv;
<<<<<<< HEAD

#ifdef CONFIG_ATH9K_DEBUGFS
	spin_lock(&sc->nodes_lock);
	list_del(&an->list);
	spin_unlock(&sc->nodes_lock);
	an->sta = NULL;
#endif

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_HT)
		ath_tx_node_cleanup(sc, an);
}


void ath9k_tasklet(unsigned long data)
{
	struct ath_softc *sc = (struct ath_softc *)data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	u32 status = sc->intrstatus;
	u32 rxmask;

	ath9k_ps_wakeup(sc);
	spin_lock(&sc->sc_pcu_lock);

	if ((status & ATH9K_INT_FATAL) ||
	    (status & ATH9K_INT_BB_WATCHDOG)) {
#ifdef CONFIG_ATH9K_DEBUGFS
		enum ath_reset_type type;

		if (status & ATH9K_INT_FATAL)
			type = RESET_TYPE_FATAL_INT;
		else
			type = RESET_TYPE_BB_WATCHDOG;

		RESET_STAT_INC(sc, type);
#endif
		ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
		goto out;
	}

	/*
	 * Only run the baseband hang check if beacons stop working in AP or
	 * IBSS mode, because it has a high false positive rate. For station
	 * mode it should not be necessary, since the upper layers will detect
	 * this through a beacon miss automatically and the following channel
	 * change will trigger a hardware reset anyway
	 */
	if (ath9k_hw_numtxpending(ah, sc->beacon.beaconq) != 0 &&
	    !ath9k_hw_check_alive(ah))
		ieee80211_queue_work(sc->hw, &sc->hw_check_work);

=======
	ath_tx_node_cleanup(sc, an);

	ath_dynack_node_deinit(sc->sc_ah, an);
}

void ath9k_tasklet(struct tasklet_struct *t)
{
	struct ath_softc *sc = from_tasklet(sc, t, intr_tq);
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	enum ath_reset_type type;
	unsigned long flags;
	u32 status;
	u32 rxmask;

	spin_lock_irqsave(&sc->intr_lock, flags);
	status = sc->intrstatus;
	sc->intrstatus = 0;
	spin_unlock_irqrestore(&sc->intr_lock, flags);

	ath9k_ps_wakeup(sc);
	spin_lock(&sc->sc_pcu_lock);

	if (status & ATH9K_INT_FATAL) {
		type = RESET_TYPE_FATAL_INT;
		ath9k_queue_reset(sc, type);
		ath_dbg(common, RESET, "FATAL: Skipping interrupts\n");
		goto out;
	}

	if ((ah->config.hw_hang_checks & HW_BB_WATCHDOG) &&
	    (status & ATH9K_INT_BB_WATCHDOG)) {
		spin_lock_irqsave(&common->cc_lock, flags);
		ath_hw_cycle_counters_update(common);
		ar9003_hw_bb_watchdog_dbg_info(ah);
		spin_unlock_irqrestore(&common->cc_lock, flags);

		if (ar9003_hw_bb_watchdog_check(ah)) {
			type = RESET_TYPE_BB_WATCHDOG;
			ath9k_queue_reset(sc, type);

			ath_dbg(common, RESET,
				"BB_WATCHDOG: Skipping interrupts\n");
			goto out;
		}
	}

	if (status & ATH9K_INT_GTT) {
		sc->gtt_cnt++;

		if ((sc->gtt_cnt >= MAX_GTT_CNT) && !ath9k_hw_check_alive(ah)) {
			type = RESET_TYPE_TX_GTT;
			ath9k_queue_reset(sc, type);
			ath_dbg(common, RESET,
				"GTT: Skipping interrupts\n");
			goto out;
		}
	}

	spin_lock_irqsave(&sc->sc_pm_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((status & ATH9K_INT_TSFOOR) && sc->ps_enabled) {
		/*
		 * TSF sync does not look correct; remain awake to sync with
		 * the next Beacon.
		 */
		ath_dbg(common, PS, "TSFOOR - Sync with next Beacon\n");
		sc->ps_flags |= PS_WAIT_FOR_BEACON | PS_BEACON_SYNC;
	}
<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		rxmask = (ATH9K_INT_RXHP | ATH9K_INT_RXLP | ATH9K_INT_RXEOL |
			  ATH9K_INT_RXORN);
	else
		rxmask = (ATH9K_INT_RX | ATH9K_INT_RXEOL | ATH9K_INT_RXORN);

	if (status & rxmask) {
		/* Check for high priority Rx first */
		if ((ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) &&
		    (status & ATH9K_INT_RXHP))
			ath_rx_tasklet(sc, 0, true);

		ath_rx_tasklet(sc, 0, false);
	}

	if (status & ATH9K_INT_TX) {
<<<<<<< HEAD
		if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
			ath_tx_edma_tasklet(sc);
		else
			ath_tx_tasklet(sc);
	}

	ath9k_btcoex_handle_interrupt(sc, status);

out:
	/* re-enable hardware interrupt */
	ath9k_hw_enable_interrupts(ah);

=======
		if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
			/*
			 * For EDMA chips, TX completion is enabled for the
			 * beacon queue, so if a beacon has been transmitted
			 * successfully after a GTT interrupt, the GTT counter
			 * gets reset to zero here.
			 */
			sc->gtt_cnt = 0;

			ath_tx_edma_tasklet(sc);
		} else {
			ath_tx_tasklet(sc);
		}

		wake_up(&sc->tx_wait);
	}

	if (status & ATH9K_INT_GENTIMER)
		ath_gen_timer_isr(sc->sc_ah);

	ath9k_btcoex_handle_interrupt(sc, status);

	/* re-enable hardware interrupt */
	ath9k_hw_resume_interrupts(ah);
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&sc->sc_pcu_lock);
	ath9k_ps_restore(sc);
}

irqreturn_t ath_isr(int irq, void *dev)
{
#define SCHED_INTR (				\
		ATH9K_INT_FATAL |		\
		ATH9K_INT_BB_WATCHDOG |		\
		ATH9K_INT_RXORN |		\
		ATH9K_INT_RXEOL |		\
		ATH9K_INT_RX |			\
		ATH9K_INT_RXLP |		\
		ATH9K_INT_RXHP |		\
		ATH9K_INT_TX |			\
		ATH9K_INT_BMISS |		\
		ATH9K_INT_CST |			\
<<<<<<< HEAD
=======
		ATH9K_INT_GTT |			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ATH9K_INT_TSFOOR |		\
		ATH9K_INT_GENTIMER |		\
		ATH9K_INT_MCI)

	struct ath_softc *sc = dev;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	enum ath9k_int status;
<<<<<<< HEAD
=======
	u32 sync_cause = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool sched = false;

	/*
	 * The hardware is not ready/present, don't
	 * touch anything. Note this can happen early
	 * on if the IRQ is shared.
	 */
<<<<<<< HEAD
	if (sc->sc_flags & SC_OP_INVALID)
		return IRQ_NONE;


	/* shared irq, not for us */

=======
	if (!ah || test_bit(ATH_OP_INVALID, &common->op_flags))
		return IRQ_NONE;

	/* shared irq, not for us */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ath9k_hw_intrpend(ah))
		return IRQ_NONE;

	/*
	 * Figure out the reason(s) for the interrupt.  Note
	 * that the hal returns a pseudo-ISR that may include
	 * bits we haven't explicitly enabled so we mask the
	 * value to insure we only process bits we requested.
	 */
<<<<<<< HEAD
	ath9k_hw_getisr(ah, &status);	/* NB: clears ISR too */
	status &= ah->imask;	/* discard unasked-for bits */

=======
	ath9k_hw_getisr(ah, &status, &sync_cause); /* NB: clears ISR too */
	ath9k_debug_sync_cause(sc, sync_cause);
	status &= ah->imask;	/* discard unasked-for bits */

	if (test_bit(ATH_OP_HW_RESET, &common->op_flags)) {
		ath9k_hw_kill_interrupts(sc->sc_ah);
		return IRQ_HANDLED;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If there are no status bits set, then this interrupt was not
	 * for me (should have been caught above).
	 */
	if (!status)
		return IRQ_NONE;

	/* Cache the status */
<<<<<<< HEAD
	sc->intrstatus = status;
=======
	spin_lock(&sc->intr_lock);
	sc->intrstatus |= status;
	spin_unlock(&sc->intr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (status & SCHED_INTR)
		sched = true;

	/*
<<<<<<< HEAD
	 * If a FATAL or RXORN interrupt is received, we have to reset the
	 * chip immediately.
	 */
	if ((status & ATH9K_INT_FATAL) || ((status & ATH9K_INT_RXORN) &&
	    !(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)))
		goto chip_reset;

	if ((ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) &&
	    (status & ATH9K_INT_BB_WATCHDOG)) {

		spin_lock(&common->cc_lock);
		ath_hw_cycle_counters_update(common);
		ar9003_hw_bb_watchdog_dbg_info(ah);
		spin_unlock(&common->cc_lock);

		goto chip_reset;
	}
=======
	 * If a FATAL interrupt is received, we have to reset the chip
	 * immediately.
	 */
	if (status & ATH9K_INT_FATAL)
		goto chip_reset;

	if ((ah->config.hw_hang_checks & HW_BB_WATCHDOG) &&
	    (status & ATH9K_INT_BB_WATCHDOG))
		goto chip_reset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (status & ATH9K_INT_SWBA)
		tasklet_schedule(&sc->bcon_tasklet);

	if (status & ATH9K_INT_TXURN)
		ath9k_hw_updatetxtriglevel(ah, true);

	if (status & ATH9K_INT_RXEOL) {
		ah->imask &= ~(ATH9K_INT_RXEOL | ATH9K_INT_RXORN);
		ath9k_hw_set_interrupts(ah);
	}

<<<<<<< HEAD
	if (status & ATH9K_INT_MIB) {
		/*
		 * Disable interrupts until we service the MIB
		 * interrupt; otherwise it will continue to
		 * fire.
		 */
		ath9k_hw_disable_interrupts(ah);
		/*
		 * Let the hal handle the event. We assume
		 * it will clear whatever condition caused
		 * the interrupt.
		 */
		spin_lock(&common->cc_lock);
		ath9k_hw_proc_mib_event(ah);
		spin_unlock(&common->cc_lock);
		ath9k_hw_enable_interrupts(ah);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP))
		if (status & ATH9K_INT_TIM_TIMER) {
			if (ATH_DBG_WARN_ON_ONCE(sc->ps_idle))
				goto chip_reset;
			/* Clear RxAbort bit so that we can
			 * receive frames */
			ath9k_setpower(sc, ATH9K_PM_AWAKE);
<<<<<<< HEAD
			ath9k_hw_setrxabort(sc->sc_ah, 0);
			sc->ps_flags |= PS_WAIT_FOR_BEACON;
=======
			spin_lock(&sc->sc_pm_lock);
			ath9k_hw_setrxabort(sc->sc_ah, 0);
			sc->ps_flags |= PS_WAIT_FOR_BEACON;
			spin_unlock(&sc->sc_pm_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

chip_reset:

	ath_debug_stat_interrupt(sc, status);

	if (sched) {
		/* turn off every interrupt */
<<<<<<< HEAD
		ath9k_hw_disable_interrupts(ah);
=======
		ath9k_hw_kill_interrupts(ah);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tasklet_schedule(&sc->intr_tq);
	}

	return IRQ_HANDLED;

#undef SCHED_INTR
}

<<<<<<< HEAD
static int ath_reset(struct ath_softc *sc, bool retry_tx)
{
	int r;

	ath9k_ps_wakeup(sc);

	r = ath_reset_internal(sc, NULL, retry_tx);

	if (retry_tx) {
		int i;
		for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
			if (ATH_TXQ_SETUP(sc, i)) {
				spin_lock_bh(&sc->tx.txq[i].axq_lock);
				ath_txq_schedule(sc, &sc->tx.txq[i]);
				spin_unlock_bh(&sc->tx.txq[i].axq_lock);
			}
		}
	}

=======
/*
 * This function is called when a HW reset cannot be deferred
 * and has to be immediate.
 */
int ath_reset(struct ath_softc *sc, struct ath9k_channel *hchan)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int r;

	ath9k_hw_kill_interrupts(sc->sc_ah);
	set_bit(ATH_OP_HW_RESET, &common->op_flags);

	ath9k_ps_wakeup(sc);
	r = ath_reset_internal(sc, hchan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_restore(sc);

	return r;
}

<<<<<<< HEAD
=======
/*
 * When a HW reset can be deferred, it is added to the
 * hw_reset_work workqueue, but we set ATH_OP_HW_RESET before
 * queueing.
 */
void ath9k_queue_reset(struct ath_softc *sc, enum ath_reset_type type)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
#ifdef CONFIG_ATH9K_DEBUGFS
	RESET_STAT_INC(sc, type);
#endif
	ath9k_hw_kill_interrupts(sc->sc_ah);
	set_bit(ATH_OP_HW_RESET, &common->op_flags);
	ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ath_reset_work(struct work_struct *work)
{
	struct ath_softc *sc = container_of(work, struct ath_softc, hw_reset_work);

<<<<<<< HEAD
	ath_reset(sc, true);
}

void ath_hw_check(struct work_struct *work)
{
	struct ath_softc *sc = container_of(work, struct ath_softc, hw_check_work);
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	unsigned long flags;
	int busy;

	ath9k_ps_wakeup(sc);
	if (ath9k_hw_check_alive(sc->sc_ah))
		goto out;

	spin_lock_irqsave(&common->cc_lock, flags);
	busy = ath_update_survey_stats(sc);
	spin_unlock_irqrestore(&common->cc_lock, flags);

	ath_dbg(common, RESET, "Possible baseband hang, busy=%d (try %d)\n",
		busy, sc->hw_busy_count + 1);
	if (busy >= 99) {
		if (++sc->hw_busy_count >= 3) {
			RESET_STAT_INC(sc, RESET_TYPE_BB_HANG);
			ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
		}

	} else if (busy >= 0)
		sc->hw_busy_count = 0;

out:
	ath9k_ps_restore(sc);
}

static void ath_hw_pll_rx_hang_check(struct ath_softc *sc, u32 pll_sqsum)
{
	static int count;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (pll_sqsum >= 0x40000) {
		count++;
		if (count == 3) {
			/* Rx is hung for more than 500ms. Reset it */
			ath_dbg(common, RESET, "Possible RX hang, resetting\n");
			RESET_STAT_INC(sc, RESET_TYPE_PLL_HANG);
			ieee80211_queue_work(sc->hw, &sc->hw_reset_work);
			count = 0;
		}
	} else
		count = 0;
}

void ath_hw_pll_work(struct work_struct *work)
{
	struct ath_softc *sc = container_of(work, struct ath_softc,
					    hw_pll_work.work);
	u32 pll_sqsum;

	/*
	 * ensure that the PLL WAR is executed only
	 * after the STA is associated (or) if the
	 * beaconing had started in interfaces that
	 * uses beacons.
	 */
	if (!(sc->sc_flags & SC_OP_BEACONS))
		return;

	if (AR_SREV_9485(sc->sc_ah)) {

		ath9k_ps_wakeup(sc);
		pll_sqsum = ar9003_get_pll_sqsum_dvc(sc->sc_ah);
		ath9k_ps_restore(sc);

		ath_hw_pll_rx_hang_check(sc, pll_sqsum);

		ieee80211_queue_delayed_work(sc->hw, &sc->hw_pll_work, HZ/5);
	}
}

=======
	ath9k_ps_wakeup(sc);
	ath_reset_internal(sc, NULL);
	ath9k_ps_restore(sc);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**********************/
/* mac80211 callbacks */
/**********************/

static int ath9k_start(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	struct ieee80211_channel *curchan = hw->conf.channel;
=======
	struct ieee80211_channel *curchan = sc->cur_chan->chandef.chan;
	struct ath_chanctx *ctx = sc->cur_chan;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ath9k_channel *init_channel;
	int r;

	ath_dbg(common, CONFIG,
		"Starting driver with initial channel: %d MHz\n",
		curchan->center_freq);

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

<<<<<<< HEAD
	init_channel = ath9k_cmn_get_curchannel(hw, ah);
=======
	init_channel = ath9k_cmn_get_channel(hw, ah, &ctx->chandef);
	sc->cur_chandef = hw->conf.chandef;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Reset SERDES registers */
	ath9k_hw_configpcipowersave(ah, false);

	/*
	 * The basic interface to setting the hardware in a good
	 * state is ``reset''.  On return the hardware is known to
	 * be powered up and with interrupts disabled.  This must
	 * be followed by initialization of the appropriate bits
	 * and then setup of the interrupt mask.
	 */
	spin_lock_bh(&sc->sc_pcu_lock);

	atomic_set(&ah->intr_ref_cnt, -1);

	r = ath9k_hw_reset(ah, init_channel, ah->caldata, false);
	if (r) {
		ath_err(common,
			"Unable to reset hardware; reset status %d (freq %u MHz)\n",
			r, curchan->center_freq);
<<<<<<< HEAD
		spin_unlock_bh(&sc->sc_pcu_lock);
		goto mutex_unlock;
=======
		ah->reset_power_on = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Setup our intr mask. */
	ah->imask = ATH9K_INT_TX | ATH9K_INT_RXEOL |
		    ATH9K_INT_RXORN | ATH9K_INT_FATAL |
		    ATH9K_INT_GLOBAL;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_EDMA)
		ah->imask |= ATH9K_INT_RXHP |
<<<<<<< HEAD
			     ATH9K_INT_RXLP |
			     ATH9K_INT_BB_WATCHDOG;
	else
		ah->imask |= ATH9K_INT_RX;

	ah->imask |= ATH9K_INT_GTT;
=======
			     ATH9K_INT_RXLP;
	else
		ah->imask |= ATH9K_INT_RX;

	if (ah->config.hw_hang_checks & HW_BB_WATCHDOG)
		ah->imask |= ATH9K_INT_BB_WATCHDOG;

	/*
	 * Enable GTT interrupts only for AR9003/AR9004 chips
	 * for now.
	 */
	if (AR_SREV_9300_20_OR_LATER(ah))
		ah->imask |= ATH9K_INT_GTT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ah->caps.hw_caps & ATH9K_HW_CAP_HT)
		ah->imask |= ATH9K_INT_CST;

<<<<<<< HEAD
	if (ah->caps.hw_caps & ATH9K_HW_CAP_MCI)
		ah->imask |= ATH9K_INT_MCI;

	sc->sc_flags &= ~SC_OP_INVALID;
	sc->sc_ah->is_monitoring = false;

	if (!ath_complete_reset(sc, false)) {
		r = -EIO;
		spin_unlock_bh(&sc->sc_pcu_lock);
		goto mutex_unlock;
	}

	if (ah->led_pin >= 0) {
		ath9k_hw_cfg_output(ah, ah->led_pin,
				    AR_GPIO_OUTPUT_MUX_AS_OUTPUT);
		ath9k_hw_set_gpio(ah, ah->led_pin, 0);
=======
	ath_mci_enable(sc);

	clear_bit(ATH_OP_INVALID, &common->op_flags);
	sc->sc_ah->is_monitoring = false;

	if (!ath_complete_reset(sc, false))
		ah->reset_power_on = false;

	if (ah->led_pin >= 0) {
		ath9k_hw_set_gpio(ah, ah->led_pin,
				  (ah->config.led_active_high) ? 1 : 0);
		ath9k_hw_gpio_request_out(ah, ah->led_pin, NULL,
					  AR_GPIO_OUTPUT_MUX_AS_OUTPUT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Reset key cache to sane defaults (all entries cleared) instead of
	 * semi-random values after suspend/resume.
	 */
	ath9k_cmn_init_crypto(sc->sc_ah);

<<<<<<< HEAD
	spin_unlock_bh(&sc->sc_pcu_lock);

	ath9k_start_btcoex(sc);

	if (ah->caps.pcie_lcr_extsync_en && common->bus_ops->extn_synch_en)
		common->bus_ops->extn_synch_en(common);

mutex_unlock:
=======
	ath9k_hw_reset_tsf(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	ath9k_rng_start(sc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&sc->mutex);

	ath9k_ps_restore(sc);

<<<<<<< HEAD
	return r;
}

static void ath9k_tx(struct ieee80211_hw *hw, struct sk_buff *skb)
=======
	return 0;
}

static void ath9k_tx(struct ieee80211_hw *hw,
		     struct ieee80211_tx_control *control,
		     struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_tx_control txctl;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) skb->data;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sc->ps_enabled) {
		/*
		 * mac80211 does not set PM field for normal data frames, so we
		 * need to update that based on the current PS mode.
		 */
		if (ieee80211_is_data(hdr->frame_control) &&
		    !ieee80211_is_nullfunc(hdr->frame_control) &&
		    !ieee80211_has_pm(hdr->frame_control)) {
			ath_dbg(common, PS,
				"Add PM=1 for a TX frame while in PS mode\n");
			hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);
		}
	}

<<<<<<< HEAD
	/*
	 * Cannot tx while the hardware is in full sleep, it first needs a full
	 * chip reset to recover from that
	 */
	if (unlikely(sc->sc_ah->power_mode == ATH9K_PM_FULL_SLEEP))
		goto exit;

	if (unlikely(sc->sc_ah->power_mode != ATH9K_PM_AWAKE)) {
=======
	if (unlikely(sc->sc_ah->power_mode == ATH9K_PM_NETWORK_SLEEP)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * We are using PS-Poll and mac80211 can request TX while in
		 * power save mode. Need to wake up hardware for the TX to be
		 * completed and if needed, also for RX of buffered frames.
		 */
		ath9k_ps_wakeup(sc);
<<<<<<< HEAD
=======
		spin_lock_irqsave(&sc->sc_pm_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP))
			ath9k_hw_setrxabort(sc->sc_ah, 0);
		if (ieee80211_is_pspoll(hdr->frame_control)) {
			ath_dbg(common, PS,
				"Sending PS-Poll to pick a buffered frame\n");
			sc->ps_flags |= PS_WAIT_FOR_PSPOLL_DATA;
		} else {
			ath_dbg(common, PS, "Wake up to complete TX\n");
			sc->ps_flags |= PS_WAIT_FOR_TX_ACK;
		}
		/*
		 * The actual restore operation will happen only after
<<<<<<< HEAD
		 * the sc_flags bit is cleared. We are just dropping
		 * the ps_usecount here.
		 */
		ath9k_ps_restore(sc);
	}

	memset(&txctl, 0, sizeof(struct ath_tx_control));
	txctl.txq = sc->tx.txq_map[skb_get_queue_mapping(skb)];
=======
		 * the ps_flags bit is cleared. We are just dropping
		 * the ps_usecount here.
		 */
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
		ath9k_ps_restore(sc);
	}

	/*
	 * Cannot tx while the hardware is in full sleep, it first needs a full
	 * chip reset to recover from that
	 */
	if (unlikely(sc->sc_ah->power_mode == ATH9K_PM_FULL_SLEEP)) {
		ath_err(common, "TX while HW is in FULL_SLEEP mode\n");
		goto exit;
	}

	memset(&txctl, 0, sizeof(struct ath_tx_control));
	txctl.txq = sc->tx.txq_map[skb_get_queue_mapping(skb)];
	txctl.sta = control->sta;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath_dbg(common, XMIT, "transmitting packet, skb: %p\n", skb);

	if (ath_tx_start(hw, skb, &txctl) != 0) {
		ath_dbg(common, XMIT, "TX failed\n");
<<<<<<< HEAD
=======
		TX_STAT_INC(sc, txctl.txq->axq_qnum, txfailed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto exit;
	}

	return;
exit:
	ieee80211_free_txskb(hw, skb);
}

<<<<<<< HEAD
=======
static bool ath9k_txq_list_has_key(struct list_head *txq_list, u32 keyix)
{
	struct ath_buf *bf;
	struct ieee80211_tx_info *txinfo;
	struct ath_frame_info *fi;

	list_for_each_entry(bf, txq_list, list) {
		if (bf->bf_state.stale || !bf->bf_mpdu)
			continue;

		txinfo = IEEE80211_SKB_CB(bf->bf_mpdu);
		fi = (struct ath_frame_info *)&txinfo->status.status_driver_data[0];
		if (fi->keyix == keyix)
			return true;
	}

	return false;
}

static bool ath9k_txq_has_key(struct ath_softc *sc, u32 keyix)
{
	struct ath_hw *ah = sc->sc_ah;
	int i, j;
	struct ath_txq *txq;
	bool key_in_use = false;

	for (i = 0; !key_in_use && i < ATH9K_NUM_TX_QUEUES; i++) {
		if (!ATH_TXQ_SETUP(sc, i))
			continue;
		txq = &sc->tx.txq[i];
		if (!txq->axq_depth)
			continue;
		if (!ath9k_hw_numtxpending(ah, txq->axq_qnum))
			continue;

		ath_txq_lock(sc, txq);
		key_in_use = ath9k_txq_list_has_key(&txq->axq_q, keyix);
		if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_EDMA) {
			int idx = txq->txq_tailidx;

			for (j = 0; !key_in_use &&
			     !list_empty(&txq->txq_fifo[idx]) &&
			     j < ATH_TXFIFO_DEPTH; j++) {
				key_in_use = ath9k_txq_list_has_key(
					&txq->txq_fifo[idx], keyix);
				INCR(idx, ATH_TXFIFO_DEPTH);
			}
		}
		ath_txq_unlock(sc, txq);
	}

	return key_in_use;
}

static void ath9k_pending_key_del(struct ath_softc *sc, u8 keyix)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	if (!test_bit(keyix, ah->pending_del_keymap) ||
	    ath9k_txq_has_key(sc, keyix))
		return;

	/* No more TXQ frames point to this key cache entry, so delete it. */
	clear_bit(keyix, ah->pending_del_keymap);
	ath_key_delete(common, keyix);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ath9k_stop(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	bool prev_idle;
<<<<<<< HEAD

	mutex_lock(&sc->mutex);

	ath_cancel_work(sc);

	if (sc->sc_flags & SC_OP_INVALID) {
=======
	int i;

	ath9k_deinit_channel_context(sc);

	mutex_lock(&sc->mutex);

	ath9k_rng_stop(sc);

	ath_cancel_work(sc);

	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ath_dbg(common, ANY, "Device not present\n");
		mutex_unlock(&sc->mutex);
		return;
	}

	/* Ensure HW is awake when we try to shut it down. */
	ath9k_ps_wakeup(sc);

<<<<<<< HEAD
	ath9k_stop_btcoex(sc);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock_bh(&sc->sc_pcu_lock);

	/* prevent tasklets to enable interrupts once we disable them */
	ah->imask &= ~ATH9K_INT_GLOBAL;

	/* make sure h/w will not generate any interrupt
	 * before setting the invalid flag. */
	ath9k_hw_disable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	/* we can now sync irq and kill any running tasklets, since we already
	 * disabled interrupts and not holding a spin lock */
	synchronize_irq(sc->irq);
	tasklet_kill(&sc->intr_tq);
	tasklet_kill(&sc->bcon_tasklet);

	prev_idle = sc->ps_idle;
	sc->ps_idle = true;

	spin_lock_bh(&sc->sc_pcu_lock);

	if (ah->led_pin >= 0) {
<<<<<<< HEAD
		ath9k_hw_set_gpio(ah, ah->led_pin, 1);
		ath9k_hw_cfg_gpio_input(ah, ah->led_pin);
	}

	ath_prepare_reset(sc, false, true);
=======
		ath9k_hw_set_gpio(ah, ah->led_pin,
				  (ah->config.led_active_high) ? 0 : 1);
		ath9k_hw_gpio_request_in(ah, ah->led_pin, NULL);
	}

	ath_prepare_reset(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sc->rx.frag) {
		dev_kfree_skb_any(sc->rx.frag);
		sc->rx.frag = NULL;
	}

	if (!ah->curchan)
<<<<<<< HEAD
		ah->curchan = ath9k_cmn_get_curchannel(hw, ah);

	ath9k_hw_reset(ah, ah->curchan, ah->caldata, false);
=======
		ah->curchan = ath9k_cmn_get_channel(hw, ah,
						    &sc->cur_chan->chandef);

	ath9k_hw_reset(ah, ah->curchan, ah->caldata, false);

	set_bit(ATH_OP_INVALID, &common->op_flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_hw_phy_disable(ah);

	ath9k_hw_configpcipowersave(ah, true);

	spin_unlock_bh(&sc->sc_pcu_lock);

<<<<<<< HEAD
	ath9k_ps_restore(sc);

	sc->sc_flags |= SC_OP_INVALID;
=======
	for (i = 0; i < ATH_KEYMAX; i++)
		ath9k_pending_key_del(sc, i);

	/* Clear key cache entries explicitly to get rid of any potentially
	 * remaining keys.
	 */
	ath9k_cmn_init_crypto(sc->sc_ah);

	ath9k_ps_restore(sc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sc->ps_idle = prev_idle;

	mutex_unlock(&sc->mutex);

	ath_dbg(common, CONFIG, "Driver halt\n");
}

<<<<<<< HEAD
bool ath9k_uses_beacons(int type)
=======
static bool ath9k_uses_beacons(int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	switch (type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		return true;
	default:
		return false;
	}
}

<<<<<<< HEAD
static void ath9k_reclaim_beacon(struct ath_softc *sc,
				 struct ieee80211_vif *vif)
{
	struct ath_vif *avp = (void *)vif->drv_priv;

	ath9k_set_beaconing_status(sc, false);
	ath_beacon_return(sc, avp);
	ath9k_set_beaconing_status(sc, true);
	sc->sc_flags &= ~SC_OP_BEACONS;
}

static void ath9k_vif_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	struct ath9k_vif_iter_data *iter_data = data;
	int i;

	if (iter_data->hw_macaddr)
		for (i = 0; i < ETH_ALEN; i++)
			iter_data->mask[i] &=
				~(iter_data->hw_macaddr[i] ^ mac[i]);
=======
static void ath9k_vif_iter_set_beacon(struct ath9k_vif_iter_data *iter_data,
				      struct ieee80211_vif *vif)
{
	/* Use the first (configured) interface, but prefering AP interfaces. */
	if (!iter_data->primary_beacon_vif) {
		iter_data->primary_beacon_vif = vif;
	} else {
		if (iter_data->primary_beacon_vif->type != NL80211_IFTYPE_AP &&
		    vif->type == NL80211_IFTYPE_AP)
			iter_data->primary_beacon_vif = vif;
	}

	iter_data->beacons = true;
	iter_data->nbcnvifs += 1;
}

static void ath9k_vif_iter(struct ath9k_vif_iter_data *iter_data,
			   u8 *mac, struct ieee80211_vif *vif)
{
	struct ath_vif *avp = (struct ath_vif *)vif->drv_priv;
	int i;

	if (iter_data->has_hw_macaddr) {
		for (i = 0; i < ETH_ALEN; i++)
			iter_data->mask[i] &=
				~(iter_data->hw_macaddr[i] ^ mac[i]);
	} else {
		memcpy(iter_data->hw_macaddr, mac, ETH_ALEN);
		iter_data->has_hw_macaddr = true;
	}

	if (!vif->bss_conf.use_short_slot)
		iter_data->slottime = 20;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (vif->type) {
	case NL80211_IFTYPE_AP:
		iter_data->naps++;
<<<<<<< HEAD
		break;
	case NL80211_IFTYPE_STATION:
		iter_data->nstations++;
		break;
	case NL80211_IFTYPE_ADHOC:
		iter_data->nadhocs++;
		break;
	case NL80211_IFTYPE_MESH_POINT:
		iter_data->nmeshes++;
		break;
	case NL80211_IFTYPE_WDS:
		iter_data->nwds++;
=======
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
		break;
	case NL80211_IFTYPE_STATION:
		iter_data->nstations++;
		if (avp->assoc && !iter_data->primary_sta)
			iter_data->primary_sta = vif;
		break;
	case NL80211_IFTYPE_OCB:
		iter_data->nocbs++;
		break;
	case NL80211_IFTYPE_ADHOC:
		iter_data->nadhocs++;
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
		break;
	case NL80211_IFTYPE_MESH_POINT:
		iter_data->nmeshes++;
		if (vif->bss_conf.enable_beacon)
			ath9k_vif_iter_set_beacon(iter_data, vif);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
/* Called with sc->mutex held. */
void ath9k_calculate_iter_data(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct ath9k_vif_iter_data *iter_data)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	/*
	 * Pick the MAC address of the first interface as the new hardware
	 * MAC address. The hardware will use it together with the BSSID mask
	 * when matching addresses.
	 */
	memset(iter_data, 0, sizeof(*iter_data));
	iter_data->hw_macaddr = common->macaddr;
	memset(&iter_data->mask, 0xff, ETH_ALEN);

	if (vif)
		ath9k_vif_iter(iter_data, vif->addr, vif);

	/* Get list of all active MAC addresses */
	ieee80211_iterate_active_interfaces_atomic(sc->hw, ath9k_vif_iter,
						   iter_data);
}

/* Called with sc->mutex held. */
static void ath9k_calculate_summary_state(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
=======
static void ath9k_update_bssid_mask(struct ath_softc *sc,
				    struct ath_chanctx *ctx,
				    struct ath9k_vif_iter_data *iter_data)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp;
	int i;

	if (!ath9k_is_chanctx_enabled())
		return;

	list_for_each_entry(avp, &ctx->vifs, list) {
		if (ctx->nvifs_assigned != 1)
			continue;

		if (!iter_data->has_hw_macaddr)
			continue;

		ether_addr_copy(common->curbssid, avp->bssid);

		/* perm_addr will be used as the p2p device address. */
		for (i = 0; i < ETH_ALEN; i++)
			iter_data->mask[i] &=
				~(iter_data->hw_macaddr[i] ^
				  sc->hw->wiphy->perm_addr[i]);
	}
}

/* Called with sc->mutex held. */
void ath9k_calculate_iter_data(struct ath_softc *sc,
			       struct ath_chanctx *ctx,
			       struct ath9k_vif_iter_data *iter_data)
{
	struct ath_vif *avp;

	/*
	 * The hardware will use primary station addr together with the
	 * BSSID mask when matching addresses.
	 */
	memset(iter_data, 0, sizeof(*iter_data));
	eth_broadcast_addr(iter_data->mask);
	iter_data->slottime = 9;

	list_for_each_entry(avp, &ctx->vifs, list)
		ath9k_vif_iter(iter_data, avp->vif->addr, avp->vif);

	ath9k_update_bssid_mask(sc, ctx, iter_data);
}

static void ath9k_set_assoc_state(struct ath_softc *sc,
				  struct ieee80211_vif *vif, bool changed)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (struct ath_vif *)vif->drv_priv;
	unsigned long flags;

	set_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);

	ether_addr_copy(common->curbssid, avp->bssid);
	common->curaid = avp->aid;
	ath9k_hw_write_associd(sc->sc_ah);

	if (changed) {
		common->last_rssi = ATH_RSSI_DUMMY_MARKER;
		sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

		spin_lock_irqsave(&sc->sc_pm_lock, flags);
		sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
	}

	if (ath9k_hw_mci_is_enabled(sc->sc_ah))
		ath9k_mci_update_wlan_channels(sc, false);

	ath_dbg(common, CONFIG,
		"Primary Station interface: %pM, BSSID: %pM\n",
		vif->addr, common->curbssid);
}

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
static void ath9k_set_offchannel_state(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_vif *vif = NULL;

	ath9k_ps_wakeup(sc);

	if (sc->offchannel.state < ATH_OFFCHANNEL_ROC_START)
		vif = sc->offchannel.scan_vif;
	else
		vif = sc->offchannel.roc_vif;

	if (WARN_ON(!vif))
		goto exit;

	eth_zero_addr(common->curbssid);
	eth_broadcast_addr(common->bssidmask);
	memcpy(common->macaddr, vif->addr, ETH_ALEN);
	common->curaid = 0;
	ah->opmode = vif->type;
	ah->imask &= ~ATH9K_INT_SWBA;
	ah->imask &= ~ATH9K_INT_TSFOOR;
	ah->slottime = 9;

	ath_hw_setbssidmask(common);
	ath9k_hw_setopmode(ah);
	ath9k_hw_write_associd(sc->sc_ah);
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_init_global_settings(ah);

exit:
	ath9k_ps_restore(sc);
}
#endif

/* Called with sc->mutex held. */
void ath9k_calculate_summary_state(struct ath_softc *sc,
				   struct ath_chanctx *ctx)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath9k_vif_iter_data iter_data;

<<<<<<< HEAD
	ath9k_calculate_iter_data(hw, vif, &iter_data);

	/* Set BSSID mask. */
	memcpy(common->bssidmask, iter_data.mask, ETH_ALEN);
	ath_hw_setbssidmask(common);

	/* Set op-mode & TSF */
	if (iter_data.naps > 0) {
		ath9k_hw_set_tsfadjust(ah, 1);
		sc->sc_flags |= SC_OP_TSF_RESET;
		ah->opmode = NL80211_IFTYPE_AP;
	} else {
		ath9k_hw_set_tsfadjust(ah, 0);
		sc->sc_flags &= ~SC_OP_TSF_RESET;

		if (iter_data.nmeshes)
			ah->opmode = NL80211_IFTYPE_MESH_POINT;
		else if (iter_data.nwds)
			ah->opmode = NL80211_IFTYPE_AP;
=======
	ath_chanctx_check_active(sc, ctx);

	if (ctx != sc->cur_chan)
		return;

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
	if (ctx == &sc->offchannel.chan)
		return ath9k_set_offchannel_state(sc);
#endif

	ath9k_ps_wakeup(sc);
	ath9k_calculate_iter_data(sc, ctx, &iter_data);

	if (iter_data.has_hw_macaddr)
		memcpy(common->macaddr, iter_data.hw_macaddr, ETH_ALEN);

	memcpy(common->bssidmask, iter_data.mask, ETH_ALEN);
	ath_hw_setbssidmask(common);

	if (iter_data.naps > 0) {
		ath9k_hw_set_tsfadjust(ah, true);
		ah->opmode = NL80211_IFTYPE_AP;
	} else {
		ath9k_hw_set_tsfadjust(ah, false);
		if (iter_data.beacons)
			ath9k_beacon_ensure_primary_slot(sc);

		if (iter_data.nmeshes)
			ah->opmode = NL80211_IFTYPE_MESH_POINT;
		else if (iter_data.nocbs)
			ah->opmode = NL80211_IFTYPE_OCB;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else if (iter_data.nadhocs)
			ah->opmode = NL80211_IFTYPE_ADHOC;
		else
			ah->opmode = NL80211_IFTYPE_STATION;
	}

<<<<<<< HEAD
	/*
	 * Enable MIB interrupts when there are hardware phy counters.
	 */
	if ((iter_data.nstations + iter_data.nadhocs + iter_data.nmeshes) > 0) {
		if (ah->config.enable_ani)
			ah->imask |= ATH9K_INT_MIB;
		ah->imask |= ATH9K_INT_TSFOOR;
	} else {
		ah->imask &= ~ATH9K_INT_MIB;
		ah->imask &= ~ATH9K_INT_TSFOOR;
	}

	ath9k_hw_set_interrupts(ah);

	/* Set up ANI */
	if (iter_data.naps > 0) {
		sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

		if (!common->disable_ani) {
			sc->sc_flags |= SC_OP_ANI_RUN;
			ath_start_ani(common);
		}

	} else {
		sc->sc_flags &= ~SC_OP_ANI_RUN;
		del_timer_sync(&common->ani.timer);
	}
}

/* Called with sc->mutex held, vif counts set up properly. */
static void ath9k_do_vif_add_setup(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;

	ath9k_calculate_summary_state(hw, vif);

	if (ath9k_uses_beacons(vif->type)) {
		int error;
		/* This may fail because upper levels do not have beacons
		 * properly configured yet.  That's OK, we assume it
		 * will be properly configured and then we will be notified
		 * in the info_changed method and set up beacons properly
		 * there.
		 */
		ath9k_set_beaconing_status(sc, false);
		error = ath_beacon_alloc(sc, vif);
		if (!error)
			ath_beacon_config(sc, vif);
		ath9k_set_beaconing_status(sc, true);
	}
}


=======
	ath9k_hw_setopmode(ah);

	ctx->switch_after_beacon = false;
	if ((iter_data.nstations + iter_data.nadhocs + iter_data.nmeshes) > 0)
		ah->imask |= ATH9K_INT_TSFOOR;
	else {
		ah->imask &= ~ATH9K_INT_TSFOOR;
		if (iter_data.naps == 1 && iter_data.beacons)
			ctx->switch_after_beacon = true;
	}

	if (ah->opmode == NL80211_IFTYPE_STATION) {
		bool changed = (iter_data.primary_sta != ctx->primary_sta);

		if (iter_data.primary_sta) {
			iter_data.primary_beacon_vif = iter_data.primary_sta;
			iter_data.beacons = true;
			ath9k_set_assoc_state(sc, iter_data.primary_sta,
					      changed);
			ctx->primary_sta = iter_data.primary_sta;
		} else {
			ctx->primary_sta = NULL;
			eth_zero_addr(common->curbssid);
			common->curaid = 0;
			ath9k_hw_write_associd(sc->sc_ah);
			if (ath9k_hw_mci_is_enabled(sc->sc_ah))
				ath9k_mci_update_wlan_channels(sc, true);
		}
	}
	sc->nbcnvifs = iter_data.nbcnvifs;
	ath9k_beacon_config(sc, iter_data.primary_beacon_vif,
			    iter_data.beacons);
	ath9k_hw_set_interrupts(ah);

	if (ah->slottime != iter_data.slottime) {
		ah->slottime = iter_data.slottime;
		ath9k_hw_init_global_settings(ah);
	}

	if (iter_data.primary_sta)
		set_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);
	else
		clear_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags);

	ath_dbg(common, CONFIG,
		"macaddr: %pM, bssid: %pM, bssidmask: %pM\n",
		common->macaddr, common->curbssid, common->bssidmask);

	ath9k_ps_restore(sc);
}

static void ath9k_tpc_vif_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	int *power = data;

	if (vif->bss_conf.txpower == INT_MIN)
		return;

	if (*power < vif->bss_conf.txpower)
		*power = vif->bss_conf.txpower;
}

/* Called with sc->mutex held. */
void ath9k_set_txpower(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	int power;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_regulatory *reg = ath9k_hw_regulatory(ah);

	ath9k_ps_wakeup(sc);
	if (ah->tpc_enabled) {
		power = (vif) ? vif->bss_conf.txpower : -1;
		ieee80211_iterate_active_interfaces_atomic(
				sc->hw, IEEE80211_IFACE_ITER_RESUME_ALL,
				ath9k_tpc_vif_iter, &power);
		if (power == -1)
			power = sc->hw->conf.power_level;
	} else {
		power = sc->hw->conf.power_level;
	}
	sc->cur_chan->txpower = 2 * power;
	ath9k_hw_set_txpowerlimit(ah, sc->cur_chan->txpower, false);
	sc->cur_chan->cur_txpower = reg->max_power_level;
	ath9k_ps_restore(sc);
}

static void ath9k_assign_hw_queues(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	int i;

	if (!ath9k_is_chanctx_enabled())
		return;

	for (i = 0; i < IEEE80211_NUM_ACS; i++)
		vif->hw_queue[i] = i;

	if (vif->type == NL80211_IFTYPE_AP ||
	    vif->type == NL80211_IFTYPE_MESH_POINT)
		vif->cab_queue = hw->queues - 2;
	else
		vif->cab_queue = IEEE80211_INVAL_HW_QUEUE;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ath9k_add_interface(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
<<<<<<< HEAD
	int ret = 0;

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	switch (vif->type) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_MESH_POINT:
		break;
	default:
		ath_err(common, "Interface type %d not yet supported\n",
			vif->type);
		ret = -EOPNOTSUPP;
		goto out;
	}

	if (ath9k_uses_beacons(vif->type)) {
		if (sc->nbcnvifs >= ATH_BCBUF) {
			ath_err(common, "Not enough beacon buffers when adding"
				" new interface of type: %i\n",
				vif->type);
			ret = -ENOBUFS;
			goto out;
		}
	}

	ath_dbg(common, CONFIG, "Attach a VIF of type: %d\n", vif->type);

	sc->nvifs++;

	ath9k_do_vif_add_setup(hw, vif);
out:
	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);
	return ret;
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_node *an = &avp->mcast_node;

	mutex_lock(&sc->mutex);
	if (IS_ENABLED(CONFIG_ATH9K_TX99)) {
		if (sc->cur_chan->nvifs >= 1) {
			mutex_unlock(&sc->mutex);
			return -EOPNOTSUPP;
		}
		sc->tx99_vif = vif;
	}

	ath_dbg(common, CONFIG, "Attach a VIF of type: %d\n", vif->type);
	sc->cur_chan->nvifs++;

	if (vif->type == NL80211_IFTYPE_STATION && ath9k_is_chanctx_enabled())
		vif->driver_flags |= IEEE80211_VIF_GET_NOA_UPDATE;

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_assign_slot(sc, vif);

	avp->vif = vif;
	if (!ath9k_is_chanctx_enabled()) {
		avp->chanctx = sc->cur_chan;
		list_add_tail(&avp->list, &avp->chanctx->vifs);
	}

	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_assign_hw_queues(hw, vif);

	ath9k_set_txpower(sc, vif);

	an->sc = sc;
	an->sta = NULL;
	an->vif = vif;
	an->no_ps_filter = true;
	ath_tx_node_init(sc, an);

	mutex_unlock(&sc->mutex);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_change_interface(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  enum nl80211_iftype new_type,
				  bool p2p)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
	int ret = 0;

	ath_dbg(common, CONFIG, "Change Interface\n");
	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);

	/* See if new interface type is valid. */
	if ((new_type == NL80211_IFTYPE_ADHOC) &&
	    (sc->nvifs > 1)) {
		ath_err(common, "When using ADHOC, it must be the only"
			" interface.\n");
		ret = -EINVAL;
		goto out;
	}

	if (ath9k_uses_beacons(new_type) &&
	    !ath9k_uses_beacons(vif->type)) {
		if (sc->nbcnvifs >= ATH_BCBUF) {
			ath_err(common, "No beacon slot available\n");
			ret = -ENOBUFS;
			goto out;
		}
	}

	/* Clean up old vif stuff */
	if (ath9k_uses_beacons(vif->type))
		ath9k_reclaim_beacon(sc, vif);

	/* Add new settings */
	vif->type = new_type;
	vif->p2p = p2p;

	ath9k_do_vif_add_setup(hw, vif);
out:
	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);
	return ret;
=======
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);

	if (IS_ENABLED(CONFIG_ATH9K_TX99)) {
		mutex_unlock(&sc->mutex);
		return -EOPNOTSUPP;
	}

	ath_dbg(common, CONFIG, "Change Interface\n");

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_remove_slot(sc, vif);

	vif->type = new_type;
	vif->p2p = p2p;

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_assign_slot(sc, vif);

	ath9k_assign_hw_queues(hw, vif);
	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_set_txpower(sc, vif);

	mutex_unlock(&sc->mutex);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ath9k_remove_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD

	ath_dbg(common, CONFIG, "Detach Interface\n");

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	sc->nvifs--;

	/* Reclaim beacon resources */
	if (ath9k_uses_beacons(vif->type))
		ath9k_reclaim_beacon(sc, vif);

	ath9k_calculate_summary_state(hw, NULL);

	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);
=======
	struct ath_vif *avp = (void *)vif->drv_priv;

	ath_dbg(common, CONFIG, "Detach Interface\n");

	mutex_lock(&sc->mutex);

	ath9k_p2p_remove_vif(sc, vif);

	sc->cur_chan->nvifs--;
	sc->tx99_vif = NULL;
	if (!ath9k_is_chanctx_enabled())
		list_del(&avp->list);

	if (ath9k_uses_beacons(vif->type))
		ath9k_beacon_remove_slot(sc, vif);

	ath_tx_node_cleanup(sc, &avp->mcast_node);

	ath9k_calculate_summary_state(sc, avp->chanctx);

	ath9k_set_txpower(sc, NULL);

	mutex_unlock(&sc->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ath9k_enable_ps(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
=======
	struct ath_common *common = ath9k_hw_common(ah);

	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sc->ps_enabled = true;
	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP)) {
		if ((ah->imask & ATH9K_INT_TIM_TIMER) == 0) {
			ah->imask |= ATH9K_INT_TIM_TIMER;
			ath9k_hw_set_interrupts(ah);
		}
		ath9k_hw_setrxabort(ah, 1);
	}
<<<<<<< HEAD
=======
	ath_dbg(common, PS, "PowerSave enabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ath9k_disable_ps(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
=======
	struct ath_common *common = ath9k_hw_common(ah);

	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sc->ps_enabled = false;
	ath9k_hw_setpower(ah, ATH9K_PM_AWAKE);
	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_AUTOSLEEP)) {
		ath9k_hw_setrxabort(ah, 0);
		sc->ps_flags &= ~(PS_WAIT_FOR_BEACON |
				  PS_WAIT_FOR_CAB |
				  PS_WAIT_FOR_PSPOLL_DATA |
				  PS_WAIT_FOR_TX_ACK);
		if (ah->imask & ATH9K_INT_TIM_TIMER) {
			ah->imask &= ~ATH9K_INT_TIM_TIMER;
			ath9k_hw_set_interrupts(ah);
		}
	}
<<<<<<< HEAD

=======
	ath_dbg(common, PS, "PowerSave disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_config(struct ieee80211_hw *hw, u32 changed)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ieee80211_conf *conf = &hw->conf;
<<<<<<< HEAD
	bool reset_channel = false;
=======
	struct ath_chanctx *ctx = sc->cur_chan;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	if (changed & IEEE80211_CONF_CHANGE_IDLE) {
		sc->ps_idle = !!(conf->flags & IEEE80211_CONF_IDLE);
<<<<<<< HEAD
		if (sc->ps_idle)
			ath_cancel_work(sc);
		else
=======
		if (sc->ps_idle) {
			ath_cancel_work(sc);
			ath9k_stop_btcoex(sc);
		} else {
			ath9k_start_btcoex(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * The chip needs a reset to properly wake up from
			 * full sleep
			 */
<<<<<<< HEAD
			reset_channel = ah->chip_fullsleep;
=======
			ath_chanctx_set_channel(sc, ctx, &ctx->chandef);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * We just prepare to enable PS. We have to wait until our AP has
	 * ACK'd our null data frame to disable RX otherwise we'll ignore
	 * those ACKs and end up retransmitting the same null data frames.
	 * IEEE80211_CONF_CHANGE_PS is only passed by mac80211 for STA mode.
	 */
	if (changed & IEEE80211_CONF_CHANGE_PS) {
		unsigned long flags;
		spin_lock_irqsave(&sc->sc_pm_lock, flags);
		if (conf->flags & IEEE80211_CONF_PS)
			ath9k_enable_ps(sc);
		else
			ath9k_disable_ps(sc);
		spin_unlock_irqrestore(&sc->sc_pm_lock, flags);
	}

	if (changed & IEEE80211_CONF_CHANGE_MONITOR) {
		if (conf->flags & IEEE80211_CONF_MONITOR) {
			ath_dbg(common, CONFIG, "Monitor mode is enabled\n");
			sc->sc_ah->is_monitoring = true;
		} else {
			ath_dbg(common, CONFIG, "Monitor mode is disabled\n");
			sc->sc_ah->is_monitoring = false;
		}
	}

<<<<<<< HEAD
	if ((changed & IEEE80211_CONF_CHANGE_CHANNEL) || reset_channel) {
		struct ieee80211_channel *curchan = hw->conf.channel;
		int pos = curchan->hw_value;
		int old_pos = -1;
		unsigned long flags;

		if (ah->curchan)
			old_pos = ah->curchan - &ah->channels[0];

		if (hw->conf.flags & IEEE80211_CONF_OFFCHANNEL)
			sc->sc_flags |= SC_OP_OFFCHANNEL;
		else
			sc->sc_flags &= ~SC_OP_OFFCHANNEL;

		ath_dbg(common, CONFIG, "Set channel: %d MHz type: %d\n",
			curchan->center_freq, conf->channel_type);

		/* update survey stats for the old channel before switching */
		spin_lock_irqsave(&common->cc_lock, flags);
		ath_update_survey_stats(sc);
		spin_unlock_irqrestore(&common->cc_lock, flags);

		ath9k_cmn_update_ichannel(&sc->sc_ah->channels[pos],
					  curchan, conf->channel_type);

		/*
		 * If the operating channel changes, change the survey in-use flags
		 * along with it.
		 * Reset the survey data for the new channel, unless we're switching
		 * back to the operating channel from an off-channel operation.
		 */
		if (!(hw->conf.flags & IEEE80211_CONF_OFFCHANNEL) &&
		    sc->cur_survey != &sc->survey[pos]) {

			if (sc->cur_survey)
				sc->cur_survey->filled &= ~SURVEY_INFO_IN_USE;

			sc->cur_survey = &sc->survey[pos];

			memset(sc->cur_survey, 0, sizeof(struct survey_info));
			sc->cur_survey->filled |= SURVEY_INFO_IN_USE;
		} else if (!(sc->survey[pos].filled & SURVEY_INFO_IN_USE)) {
			memset(&sc->survey[pos], 0, sizeof(struct survey_info));
		}

		if (ath_set_channel(sc, hw, &sc->sc_ah->channels[pos]) < 0) {
			ath_err(common, "Unable to set channel\n");
			mutex_unlock(&sc->mutex);
			return -EINVAL;
		}

		/*
		 * The most recent snapshot of channel->noisefloor for the old
		 * channel is only available after the hardware reset. Copy it to
		 * the survey stats now.
		 */
		if (old_pos >= 0)
			ath_update_survey_nf(sc, old_pos);
	}

	if (changed & IEEE80211_CONF_CHANGE_POWER) {
		ath_dbg(common, CONFIG, "Set power: %d\n", conf->power_level);
		sc->config.txpowlimit = 2 * conf->power_level;
		ath9k_cmn_update_txpow(ah, sc->curtxpow,
				       sc->config.txpowlimit, &sc->curtxpow);
	}
=======
	if (!ath9k_is_chanctx_enabled() && (changed & IEEE80211_CONF_CHANGE_CHANNEL)) {
		ctx->offchannel = !!(conf->flags & IEEE80211_CONF_OFFCHANNEL);
		ath_chanctx_set_channel(sc, ctx, &hw->conf.chandef);
	}

	if (changed & IEEE80211_CONF_CHANGE_POWER)
		ath9k_set_txpower(sc, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);

	return 0;
}

#define SUPPORTED_FILTERS			\
<<<<<<< HEAD
	(FIF_PROMISC_IN_BSS |			\
	FIF_ALLMULTI |				\
=======
	(FIF_ALLMULTI |				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	FIF_CONTROL |				\
	FIF_PSPOLL |				\
	FIF_OTHER_BSS |				\
	FIF_BCN_PRBRESP_PROMISC |		\
	FIF_PROBE_REQ |				\
<<<<<<< HEAD
=======
	FIF_MCAST_ACTION |			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	FIF_FCSFAIL)

/* FIXME: sc->sc_full_reset ? */
static void ath9k_configure_filter(struct ieee80211_hw *hw,
				   unsigned int changed_flags,
				   unsigned int *total_flags,
				   u64 multicast)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
	u32 rfilt;

	changed_flags &= SUPPORTED_FILTERS;
	*total_flags &= SUPPORTED_FILTERS;

	sc->rx.rxfilter = *total_flags;
=======
	struct ath_chanctx *ctx;
	u32 rfilt;

	*total_flags &= SUPPORTED_FILTERS;

	spin_lock_bh(&sc->chan_lock);
	ath_for_each_chanctx(sc, ctx)
		ctx->rxfilter = *total_flags;
#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
	sc->offchannel.chan.rxfilter = *total_flags;
#endif
	spin_unlock_bh(&sc->chan_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_wakeup(sc);
	rfilt = ath_calcrxfilter(sc);
	ath9k_hw_setrxfilter(sc->sc_ah, rfilt);
	ath9k_ps_restore(sc);

	ath_dbg(ath9k_hw_common(sc->sc_ah), CONFIG, "Set HW RX filter: 0x%x\n",
		rfilt);
}

static int ath9k_sta_add(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_node *an = (struct ath_node *) sta->drv_priv;
	struct ieee80211_key_conf ps_key = { };
	int key;

	ath_node_attach(sc, sta, vif);

	if (vif->type != NL80211_IFTYPE_AP &&
	    vif->type != NL80211_IFTYPE_AP_VLAN)
		return 0;

	key = ath_key_config(common, vif, sta, &ps_key);
<<<<<<< HEAD
	if (key > 0)
		an->ps_key = key;
=======
	if (key > 0) {
		an->ps_key = key;
		an->key_idx[0] = key;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void ath9k_del_ps_key(struct ath_softc *sc,
			     struct ieee80211_vif *vif,
			     struct ieee80211_sta *sta)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_node *an = (struct ath_node *) sta->drv_priv;
<<<<<<< HEAD
	struct ieee80211_key_conf ps_key = { .hw_key_idx = an->ps_key };
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!an->ps_key)
	    return;

<<<<<<< HEAD
	ath_key_delete(common, &ps_key);
	an->ps_key = 0;
=======
	ath_key_delete(common, an->ps_key);
	an->ps_key = 0;
	an->key_idx[0] = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_sta_remove(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;

	ath9k_del_ps_key(sc, vif, sta);
	ath_node_detach(sc, sta);

	return 0;
}

<<<<<<< HEAD
=======
static int ath9k_sta_state(struct ieee80211_hw *hw,
			   struct ieee80211_vif *vif,
			   struct ieee80211_sta *sta,
			   enum ieee80211_sta_state old_state,
			   enum ieee80211_sta_state new_state)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	if (old_state == IEEE80211_STA_NOTEXIST &&
	    new_state == IEEE80211_STA_NONE) {
		ret = ath9k_sta_add(hw, vif, sta);
		ath_dbg(common, CONFIG,
			"Add station: %pM\n", sta->addr);
	} else if (old_state == IEEE80211_STA_NONE &&
		   new_state == IEEE80211_STA_NOTEXIST) {
		ret = ath9k_sta_remove(hw, vif, sta);
		ath_dbg(common, CONFIG,
			"Remove station: %pM\n", sta->addr);
	}

	if (ath9k_is_chanctx_enabled()) {
		if (vif->type == NL80211_IFTYPE_STATION) {
			if (old_state == IEEE80211_STA_ASSOC &&
			    new_state == IEEE80211_STA_AUTHORIZED)
				ath_chanctx_event(sc, vif,
						  ATH_CHANCTX_EVENT_AUTHORIZED);
		}
	}

	return ret;
}

static void ath9k_sta_set_tx_filter(struct ath_hw *ah,
				    struct ath_node *an,
				    bool set)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
		if (!an->key_idx[i])
			continue;
		ath9k_hw_set_tx_filter(ah, an->key_idx[i], set);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ath9k_sta_notify(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 enum sta_notify_cmd cmd,
			 struct ieee80211_sta *sta)
{
	struct ath_softc *sc = hw->priv;
	struct ath_node *an = (struct ath_node *) sta->drv_priv;

<<<<<<< HEAD
	if (!sta->ht_cap.ht_supported)
		return;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (cmd) {
	case STA_NOTIFY_SLEEP:
		an->sleeping = true;
		ath_tx_aggr_sleep(sta, sc, an);
<<<<<<< HEAD
		break;
	case STA_NOTIFY_AWAKE:
=======
		ath9k_sta_set_tx_filter(sc->sc_ah, an, true);
		break;
	case STA_NOTIFY_AWAKE:
		ath9k_sta_set_tx_filter(sc->sc_ah, an, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		an->sleeping = false;
		ath_tx_aggr_wakeup(sc, an);
		break;
	}
}

static int ath9k_conf_tx(struct ieee80211_hw *hw,
<<<<<<< HEAD
			 struct ieee80211_vif *vif, u16 queue,
=======
			 struct ieee80211_vif *vif,
			 unsigned int link_id, u16 queue,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 const struct ieee80211_tx_queue_params *params)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_txq *txq;
	struct ath9k_tx_queue_info qi;
	int ret = 0;

<<<<<<< HEAD
	if (queue >= WME_NUM_AC)
=======
	if (queue >= IEEE80211_NUM_ACS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	txq = sc->tx.txq_map[queue];

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	memset(&qi, 0, sizeof(struct ath9k_tx_queue_info));

	qi.tqi_aifs = params->aifs;
	qi.tqi_cwmin = params->cw_min;
	qi.tqi_cwmax = params->cw_max;
<<<<<<< HEAD
	qi.tqi_burstTime = params->txop;
=======
	qi.tqi_burstTime = params->txop * 32;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath_dbg(common, CONFIG,
		"Configure tx [queue/halq] [%d/%d], aifs: %d, cw_min: %d, cw_max: %d, txop: %d\n",
		queue, txq->axq_qnum, params->aifs, params->cw_min,
		params->cw_max, params->txop);

<<<<<<< HEAD
=======
	ath_update_max_aggr_framelen(sc, queue, qi.tqi_burstTime);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = ath_txq_update(sc, txq->axq_qnum, &qi);
	if (ret)
		ath_err(common, "TXQ Update failed\n");

<<<<<<< HEAD
	if (sc->sc_ah->opmode == NL80211_IFTYPE_ADHOC)
		if (queue == WME_AC_BE && !ret)
			ath_beaconq_config(sc);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);

	return ret;
}

static int ath9k_set_key(struct ieee80211_hw *hw,
			 enum set_key_cmd cmd,
			 struct ieee80211_vif *vif,
			 struct ieee80211_sta *sta,
			 struct ieee80211_key_conf *key)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
<<<<<<< HEAD
	int ret = 0;
=======
	struct ath_node *an = NULL;
	int ret = 0, i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ath9k_modparam_nohwcrypt)
		return -ENOSPC;

	if ((vif->type == NL80211_IFTYPE_ADHOC ||
	     vif->type == NL80211_IFTYPE_MESH_POINT) &&
	    (key->cipher == WLAN_CIPHER_SUITE_TKIP ||
	     key->cipher == WLAN_CIPHER_SUITE_CCMP) &&
	    !(key->flags & IEEE80211_KEY_FLAG_PAIRWISE)) {
		/*
		 * For now, disable hw crypto for the RSN IBSS group keys. This
		 * could be optimized in the future to use a modified key cache
		 * design to support per-STA RX GTK, but until that gets
		 * implemented, use of software crypto for group addressed
		 * frames is a acceptable to allow RSN IBSS to be used.
		 */
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	ath_dbg(common, CONFIG, "Set HW Key\n");
=======
	/* There may be MPDUs queued for the outgoing PTK key. Flush queues to
	 * make sure these are not send unencrypted or with a wrong (new) key
	 */
	if (cmd == DISABLE_KEY && key->flags & IEEE80211_KEY_FLAG_PAIRWISE) {
		ieee80211_stop_queues(hw);
		ath9k_flush(hw, vif, 0, true);
		ieee80211_wake_queues(hw);
	}

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	ath_dbg(common, CONFIG, "Set HW Key %d\n", cmd);
	if (sta)
		an = (struct ath_node *)sta->drv_priv;

	/* Delete pending key cache entries if no more frames are pointing to
	 * them in TXQs.
	 */
	for (i = 0; i < ATH_KEYMAX; i++)
		ath9k_pending_key_del(sc, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (cmd) {
	case SET_KEY:
		if (sta)
			ath9k_del_ps_key(sc, vif, sta);

<<<<<<< HEAD
=======
		key->hw_key_idx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = ath_key_config(common, vif, sta, key);
		if (ret >= 0) {
			key->hw_key_idx = ret;
			/* push IV and Michael MIC generation to stack */
			key->flags |= IEEE80211_KEY_FLAG_GENERATE_IV;
			if (key->cipher == WLAN_CIPHER_SUITE_TKIP)
				key->flags |= IEEE80211_KEY_FLAG_GENERATE_MMIC;
<<<<<<< HEAD
			if (sc->sc_ah->sw_mgmt_crypto &&
			    key->cipher == WLAN_CIPHER_SUITE_CCMP)
				key->flags |= IEEE80211_KEY_FLAG_SW_MGMT;
			ret = 0;
		}
		break;
	case DISABLE_KEY:
		ath_key_delete(common, key);
=======
			if (sc->sc_ah->sw_mgmt_crypto_tx &&
			    key->cipher == WLAN_CIPHER_SUITE_CCMP)
				key->flags |= IEEE80211_KEY_FLAG_SW_MGMT_TX;
			ret = 0;
		}
		if (an && key->hw_key_idx) {
			for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
				if (an->key_idx[i])
					continue;
				an->key_idx[i] = key->hw_key_idx;
				break;
			}
			WARN_ON(i == ARRAY_SIZE(an->key_idx));
		}
		break;
	case DISABLE_KEY:
		if (ath9k_txq_has_key(sc, key->hw_key_idx)) {
			/* Delay key cache entry deletion until there are no
			 * remaining TXQ frames pointing to this entry.
			 */
			set_bit(key->hw_key_idx, sc->sc_ah->pending_del_keymap);
			ath_hw_keysetmac(common, key->hw_key_idx, NULL);
		} else {
			ath_key_delete(common, key->hw_key_idx);
		}
		if (an) {
			for (i = 0; i < ARRAY_SIZE(an->key_idx); i++) {
				if (an->key_idx[i] != key->hw_key_idx)
					continue;
				an->key_idx[i] = 0;
				break;
			}
		}
		key->hw_key_idx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ret = -EINVAL;
	}

	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return ret;
}
<<<<<<< HEAD
static void ath9k_bss_iter(void *data, u8 *mac, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = data;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;
	struct ath_vif *avp = (void *)vif->drv_priv;

	/*
	 * Skip iteration if primary station vif's bss info
	 * was not changed
	 */
	if (sc->sc_flags & SC_OP_PRIM_STA_VIF)
		return;

	if (bss_conf->assoc) {
		sc->sc_flags |= SC_OP_PRIM_STA_VIF;
		avp->primary_sta_vif = true;
		memcpy(common->curbssid, bss_conf->bssid, ETH_ALEN);
		common->curaid = bss_conf->aid;
		ath9k_hw_write_associd(sc->sc_ah);
		ath_dbg(common, CONFIG, "Bss Info ASSOC %d, bssid: %pM\n",
			bss_conf->aid, common->curbssid);
		ath_beacon_config(sc, vif);
		/*
		 * Request a re-configuration of Beacon related timers
		 * on the receipt of the first Beacon frame (i.e.,
		 * after time sync with the AP).
		 */
		sc->ps_flags |= PS_BEACON_SYNC | PS_WAIT_FOR_BEACON;
		/* Reset rssi stats */
		sc->last_rssi = ATH_RSSI_DUMMY_MARKER;
		sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

		if (!common->disable_ani) {
			sc->sc_flags |= SC_OP_ANI_RUN;
			ath_start_ani(common);
		}

	}
}

static void ath9k_config_bss(struct ath_softc *sc, struct ieee80211_vif *vif)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_bss_conf *bss_conf = &vif->bss_conf;
	struct ath_vif *avp = (void *)vif->drv_priv;

	if (sc->sc_ah->opmode != NL80211_IFTYPE_STATION)
		return;

	/* Reconfigure bss info */
	if (avp->primary_sta_vif && !bss_conf->assoc) {
		ath_dbg(common, CONFIG, "Bss Info DISASSOC %d, bssid %pM\n",
			common->curaid, common->curbssid);
		sc->sc_flags &= ~(SC_OP_PRIM_STA_VIF | SC_OP_BEACONS);
		avp->primary_sta_vif = false;
		memset(common->curbssid, 0, ETH_ALEN);
		common->curaid = 0;
	}

	ieee80211_iterate_active_interfaces_atomic(
			sc->hw, ath9k_bss_iter, sc);

	/*
	 * None of station vifs are associated.
	 * Clear bssid & aid
	 */
	if (!(sc->sc_flags & SC_OP_PRIM_STA_VIF)) {
		ath9k_hw_write_associd(sc->sc_ah);
		/* Stop ANI */
		sc->sc_flags &= ~SC_OP_ANI_RUN;
		del_timer_sync(&common->ani.timer);
		memset(&sc->caldata, 0, sizeof(sc->caldata));
	}
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void ath9k_bss_info_changed(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_bss_conf *bss_conf,
<<<<<<< HEAD
				   u32 changed)
{
=======
				   u64 changed)
{
#define CHECK_ANI				\
	(BSS_CHANGED_ASSOC |			\
	 BSS_CHANGED_IBSS |			\
	 BSS_CHANGED_BEACON_ENABLED)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	int slottime;
<<<<<<< HEAD
	int error;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath9k_ps_wakeup(sc);
	mutex_lock(&sc->mutex);

	if (changed & BSS_CHANGED_ASSOC) {
<<<<<<< HEAD
		ath9k_config_bss(sc, vif);

		ath_dbg(common, CONFIG, "BSSID: %pM aid: 0x%x\n",
			common->curbssid, common->curaid);
	}

	if (changed & BSS_CHANGED_IBSS) {
		/* There can be only one vif available */
		memcpy(common->curbssid, bss_conf->bssid, ETH_ALEN);
		common->curaid = bss_conf->aid;
		ath9k_hw_write_associd(sc->sc_ah);

		if (bss_conf->ibss_joined) {
			sc->sc_ah->stats.avgbrssi = ATH_RSSI_DUMMY_MARKER;

			if (!common->disable_ani) {
				sc->sc_flags |= SC_OP_ANI_RUN;
				ath_start_ani(common);
			}

		} else {
			sc->sc_flags &= ~SC_OP_ANI_RUN;
			del_timer_sync(&common->ani.timer);
		}
	}

	/* Enable transmission of beacons (AP, IBSS, MESH) */
	if ((changed & BSS_CHANGED_BEACON) ||
	    ((changed & BSS_CHANGED_BEACON_ENABLED) && bss_conf->enable_beacon)) {
		ath9k_set_beaconing_status(sc, false);
		error = ath_beacon_alloc(sc, vif);
		if (!error)
			ath_beacon_config(sc, vif);
		ath9k_set_beaconing_status(sc, true);
	}

	if (changed & BSS_CHANGED_ERP_SLOT) {
=======
		ath_dbg(common, CONFIG, "BSSID %pM Changed ASSOC %d\n",
			bss_conf->bssid, vif->cfg.assoc);

		memcpy(avp->bssid, bss_conf->bssid, ETH_ALEN);
		avp->aid = vif->cfg.aid;
		avp->assoc = vif->cfg.assoc;

		ath9k_calculate_summary_state(sc, avp->chanctx);
	}

	if ((changed & BSS_CHANGED_IBSS) ||
	      (changed & BSS_CHANGED_OCB)) {
		memcpy(common->curbssid, bss_conf->bssid, ETH_ALEN);
		common->curaid = vif->cfg.aid;
		ath9k_hw_write_associd(sc->sc_ah);
	}

	if ((changed & BSS_CHANGED_BEACON_ENABLED) ||
	    (changed & BSS_CHANGED_BEACON_INT) ||
	    (changed & BSS_CHANGED_BEACON_INFO)) {
		ath9k_calculate_summary_state(sc, avp->chanctx);
	}

	if ((avp->chanctx == sc->cur_chan) &&
	    (changed & BSS_CHANGED_ERP_SLOT)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (bss_conf->use_short_slot)
			slottime = 9;
		else
			slottime = 20;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (vif->type == NL80211_IFTYPE_AP) {
			/*
			 * Defer update, so that connected stations can adjust
			 * their settings at the same time.
			 * See beacon.c for more details
			 */
			sc->beacon.slottime = slottime;
			sc->beacon.updateslot = UPDATE;
		} else {
			ah->slottime = slottime;
			ath9k_hw_init_global_settings(ah);
		}
	}

<<<<<<< HEAD
	/* Disable transmission of beacons */
	if ((changed & BSS_CHANGED_BEACON_ENABLED) &&
	    !bss_conf->enable_beacon) {
		ath9k_set_beaconing_status(sc, false);
		avp->is_bslot_active = false;
		ath9k_set_beaconing_status(sc, true);
	}

	if (changed & BSS_CHANGED_BEACON_INT) {
		/*
		 * In case of AP mode, the HW TSF has to be reset
		 * when the beacon interval changes.
		 */
		if (vif->type == NL80211_IFTYPE_AP) {
			sc->sc_flags |= SC_OP_TSF_RESET;
			ath9k_set_beaconing_status(sc, false);
			error = ath_beacon_alloc(sc, vif);
			if (!error)
				ath_beacon_config(sc, vif);
			ath9k_set_beaconing_status(sc, true);
		} else
			ath_beacon_config(sc, vif);
=======
	if (changed & BSS_CHANGED_P2P_PS)
		ath9k_p2p_bss_info_changed(sc, vif);

	if (changed & CHECK_ANI)
		ath_check_ani(sc);

	if (changed & BSS_CHANGED_TXPOWER) {
		ath_dbg(common, CONFIG, "vif %pM power %d dbm power_type %d\n",
			vif->addr, bss_conf->txpower, bss_conf->txpower_type);
		ath9k_set_txpower(sc, vif);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	mutex_unlock(&sc->mutex);
	ath9k_ps_restore(sc);
<<<<<<< HEAD
=======

#undef CHECK_ANI
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u64 ath9k_get_tsf(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64 tsf;

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	tsf = ath9k_hw_gettsf64(sc->sc_ah);
=======
	/* Get current TSF either from HW or kernel time. */
	if (sc->cur_chan == avp->chanctx) {
		tsf = ath9k_hw_gettsf64(sc->sc_ah);
	} else {
		tsf = sc->cur_chan->tsf_val +
		      ath9k_hw_get_tsf_offset(&sc->cur_chan->tsf_ts, NULL);
	}
	tsf += le64_to_cpu(avp->tsf_adjust);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return tsf;
}

static void ath9k_set_tsf(struct ieee80211_hw *hw,
			  struct ieee80211_vif *vif,
			  u64 tsf)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	ath9k_hw_settsf64(sc->sc_ah, tsf);
=======
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);
	ath9k_ps_wakeup(sc);
	tsf -= le64_to_cpu(avp->tsf_adjust);
	ktime_get_raw_ts64(&avp->chanctx->tsf_ts);
	if (sc->cur_chan == avp->chanctx)
		ath9k_hw_settsf64(sc->sc_ah, tsf);
	avp->chanctx->tsf_val = tsf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);
}

static void ath9k_reset_tsf(struct ieee80211_hw *hw, struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
<<<<<<< HEAD
=======
	struct ath_vif *avp = (void *)vif->drv_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&sc->mutex);

	ath9k_ps_wakeup(sc);
<<<<<<< HEAD
	ath9k_hw_reset_tsf(sc->sc_ah);
=======
	ktime_get_raw_ts64(&avp->chanctx->tsf_ts);
	if (sc->cur_chan == avp->chanctx)
		ath9k_hw_reset_tsf(sc->sc_ah);
	avp->chanctx->tsf_val = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_ps_restore(sc);

	mutex_unlock(&sc->mutex);
}

static int ath9k_ampdu_action(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
<<<<<<< HEAD
			      enum ieee80211_ampdu_mlme_action action,
			      struct ieee80211_sta *sta,
			      u16 tid, u16 *ssn, u8 buf_size)
{
	struct ath_softc *sc = hw->priv;
	int ret = 0;

	local_bh_disable();
=======
			      struct ieee80211_ampdu_params *params)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	bool flush = false;
	int ret = 0;
	struct ieee80211_sta *sta = params->sta;
	struct ath_node *an = (struct ath_node *)sta->drv_priv;
	enum ieee80211_ampdu_mlme_action action = params->action;
	u16 tid = params->tid;
	u16 *ssn = &params->ssn;
	struct ath_atx_tid *atid;

	mutex_lock(&sc->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (action) {
	case IEEE80211_AMPDU_RX_START:
		break;
	case IEEE80211_AMPDU_RX_STOP:
		break;
	case IEEE80211_AMPDU_TX_START:
<<<<<<< HEAD
		ath9k_ps_wakeup(sc);
		ret = ath_tx_aggr_start(sc, sta, tid, ssn);
		if (!ret)
			ieee80211_start_tx_ba_cb_irqsafe(vif, sta->addr, tid);
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_STOP:
		ath9k_ps_wakeup(sc);
		ath_tx_aggr_stop(sc, sta, tid);
		ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_OPERATIONAL:
		ath9k_ps_wakeup(sc);
		ath_tx_aggr_resume(sc, sta, tid);
		ath9k_ps_restore(sc);
=======
		if (ath9k_is_chanctx_enabled()) {
			if (test_bit(ATH_OP_SCANNING, &common->op_flags)) {
				ret = -EBUSY;
				break;
			}
		}
		ath9k_ps_wakeup(sc);
		ret = ath_tx_aggr_start(sc, sta, tid, ssn);
		if (!ret)
			ret = IEEE80211_AMPDU_TX_START_IMMEDIATE;
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_STOP_FLUSH:
	case IEEE80211_AMPDU_TX_STOP_FLUSH_CONT:
		flush = true;
		fallthrough;
	case IEEE80211_AMPDU_TX_STOP_CONT:
		ath9k_ps_wakeup(sc);
		ath_tx_aggr_stop(sc, sta, tid);
		if (!flush)
			ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);
		ath9k_ps_restore(sc);
		break;
	case IEEE80211_AMPDU_TX_OPERATIONAL:
		atid = ath_node_to_tid(an, tid);
		atid->baw_size = IEEE80211_MIN_AMPDU_BUF <<
					sta->deflink.ht_cap.ampdu_factor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ath_err(ath9k_hw_common(sc->sc_ah), "Unknown AMPDU action\n");
	}

<<<<<<< HEAD
	local_bh_enable();
=======
	mutex_unlock(&sc->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static int ath9k_get_survey(struct ieee80211_hw *hw, int idx,
			     struct survey_info *survey)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *chan;
	unsigned long flags;
	int pos;

<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return -EOPNOTSUPP;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock_irqsave(&common->cc_lock, flags);
	if (idx == 0)
		ath_update_survey_stats(sc);

<<<<<<< HEAD
	sband = hw->wiphy->bands[IEEE80211_BAND_2GHZ];
=======
	sband = hw->wiphy->bands[NL80211_BAND_2GHZ];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sband && idx >= sband->n_channels) {
		idx -= sband->n_channels;
		sband = NULL;
	}

	if (!sband)
<<<<<<< HEAD
		sband = hw->wiphy->bands[IEEE80211_BAND_5GHZ];
=======
		sband = hw->wiphy->bands[NL80211_BAND_5GHZ];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!sband || idx >= sband->n_channels) {
		spin_unlock_irqrestore(&common->cc_lock, flags);
		return -ENOENT;
	}

	chan = &sband->channels[idx];
	pos = chan->hw_value;
	memcpy(survey, &sc->survey[pos], sizeof(*survey));
	survey->channel = chan;
	spin_unlock_irqrestore(&common->cc_lock, flags);

	return 0;
}

<<<<<<< HEAD
static void ath9k_set_coverage_class(struct ieee80211_hw *hw, u8 coverage_class)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;

	mutex_lock(&sc->mutex);
	ah->coverage_class = coverage_class;

	ath9k_ps_wakeup(sc);
	ath9k_hw_init_global_settings(ah);
	ath9k_ps_restore(sc);

	mutex_unlock(&sc->mutex);
}

static void ath9k_flush(struct ieee80211_hw *hw, bool drop)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	int timeout = 200; /* ms */
	int i, j;
	bool drain_txq;

	mutex_lock(&sc->mutex);
	cancel_delayed_work_sync(&sc->tx_complete_work);

	if (ah->ah_flags & AH_UNPLUGGED) {
		ath_dbg(common, ANY, "Device has been unplugged!\n");
		mutex_unlock(&sc->mutex);
		return;
	}

	if (sc->sc_flags & SC_OP_INVALID) {
		ath_dbg(common, ANY, "Device not present\n");
		mutex_unlock(&sc->mutex);
		return;
	}

	for (j = 0; j < timeout; j++) {
		bool npend = false;

		if (j)
			usleep_range(1000, 2000);

		for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
			if (!ATH_TXQ_SETUP(sc, i))
				continue;

			npend = ath9k_has_pending_frames(sc, &sc->tx.txq[i]);

			if (npend)
				break;
		}

		if (!npend)
		    break;
	}

	if (drop) {
		ath9k_ps_wakeup(sc);
		spin_lock_bh(&sc->sc_pcu_lock);
		drain_txq = ath_drain_all_txq(sc, false);
		spin_unlock_bh(&sc->sc_pcu_lock);

		if (!drain_txq)
			ath_reset(sc, false);

		ath9k_ps_restore(sc);
		ieee80211_wake_queues(hw);
	}

	ieee80211_queue_delayed_work(hw, &sc->tx_complete_work, 0);
	mutex_unlock(&sc->mutex);
}

static bool ath9k_tx_frames_pending(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	int i;
=======
static void ath9k_enable_dynack(struct ath_softc *sc)
{
#ifdef CONFIG_ATH9K_DYNACK
	u32 rfilt;
	struct ath_hw *ah = sc->sc_ah;

	ath_dynack_reset(ah);

	ah->dynack.enabled = true;
	rfilt = ath_calcrxfilter(sc);
	ath9k_hw_setrxfilter(ah, rfilt);
#endif
}

static void ath9k_set_coverage_class(struct ieee80211_hw *hw,
				     s16 coverage_class)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;

	if (IS_ENABLED(CONFIG_ATH9K_TX99))
		return;

	mutex_lock(&sc->mutex);

	if (coverage_class >= 0) {
		ah->coverage_class = coverage_class;
		if (ah->dynack.enabled) {
			u32 rfilt;

			ah->dynack.enabled = false;
			rfilt = ath_calcrxfilter(sc);
			ath9k_hw_setrxfilter(ah, rfilt);
		}
		ath9k_ps_wakeup(sc);
		ath9k_hw_init_global_settings(ah);
		ath9k_ps_restore(sc);
	} else if (!ah->dynack.enabled) {
		ath9k_enable_dynack(sc);
	}

	mutex_unlock(&sc->mutex);
}

static bool ath9k_has_tx_pending(struct ath_softc *sc,
				 bool sw_pending)
{
	int i, npend = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++) {
		if (!ATH_TXQ_SETUP(sc, i))
			continue;

<<<<<<< HEAD
		if (ath9k_has_pending_frames(sc, &sc->tx.txq[i]))
			return true;
	}
	return false;
=======
		npend = ath9k_has_pending_frames(sc, &sc->tx.txq[i],
						 sw_pending);
		if (npend)
			break;
	}

	return !!npend;
}

static void ath9k_flush(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			u32 queues, bool drop)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (ath9k_is_chanctx_enabled()) {
		if (!test_bit(ATH_OP_MULTI_CHANNEL, &common->op_flags))
			goto flush;

		/*
		 * If MCC is active, extend the flush timeout
		 * and wait for the HW/SW queues to become
		 * empty. This needs to be done outside the
		 * sc->mutex lock to allow the channel scheduler
		 * to switch channel contexts.
		 *
		 * The vif queues have been stopped in mac80211,
		 * so there won't be any incoming frames.
		 */
		__ath9k_flush(hw, queues, drop, true, true);
		return;
	}
flush:
	mutex_lock(&sc->mutex);
	__ath9k_flush(hw, queues, drop, true, false);
	mutex_unlock(&sc->mutex);
}

void __ath9k_flush(struct ieee80211_hw *hw, u32 queues, bool drop,
		   bool sw_pending, bool timeout_override)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	int timeout;
	bool drain_txq;

	cancel_delayed_work_sync(&sc->hw_check_work);

	if (ah->ah_flags & AH_UNPLUGGED) {
		ath_dbg(common, ANY, "Device has been unplugged!\n");
		return;
	}

	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
		ath_dbg(common, ANY, "Device not present\n");
		return;
	}

	spin_lock_bh(&sc->chan_lock);
	if (timeout_override)
		timeout = HZ / 5;
	else
		timeout = sc->cur_chan->flush_timeout;
	spin_unlock_bh(&sc->chan_lock);

	ath_dbg(common, CHAN_CTX,
		"Flush timeout: %d\n", jiffies_to_msecs(timeout));

	if (wait_event_timeout(sc->tx_wait, !ath9k_has_tx_pending(sc, sw_pending),
			       timeout) > 0)
		drop = false;

	if (drop) {
		ath9k_ps_wakeup(sc);
		spin_lock_bh(&sc->sc_pcu_lock);
		drain_txq = ath_drain_all_txq(sc);
		spin_unlock_bh(&sc->sc_pcu_lock);

		if (!drain_txq)
			ath_reset(sc, NULL);

		ath9k_ps_restore(sc);
	}

	ieee80211_queue_delayed_work(hw, &sc->hw_check_work,
				     msecs_to_jiffies(ATH_HW_CHECK_POLL_INT));
}

static bool ath9k_tx_frames_pending(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;

	return ath9k_has_tx_pending(sc, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_tx_last_beacon(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ieee80211_vif *vif;
	struct ath_vif *avp;
	struct ath_buf *bf;
	struct ath_tx_status ts;
	bool edma = !!(ah->caps.hw_caps & ATH9K_HW_CAP_EDMA);
	int status;

	vif = sc->beacon.bslot[0];
	if (!vif)
		return 0;

<<<<<<< HEAD
	avp = (void *)vif->drv_priv;
	if (!avp->is_bslot_active)
		return 0;

=======
	if (!vif->bss_conf.enable_beacon)
		return 0;

	avp = (void *)vif->drv_priv;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sc->beacon.tx_processed && !edma) {
		tasklet_disable(&sc->bcon_tasklet);

		bf = avp->av_bcbuf;
		if (!bf || !bf->bf_mpdu)
			goto skip;

		status = ath9k_hw_txprocdesc(ah, bf->bf_desc, &ts);
		if (status == -EINPROGRESS)
			goto skip;

		sc->beacon.tx_processed = true;
		sc->beacon.tx_last = !(ts.ts_status & ATH9K_TXERR_MASK);

skip:
		tasklet_enable(&sc->bcon_tasklet);
	}

	return sc->beacon.tx_last;
}

static int ath9k_get_stats(struct ieee80211_hw *hw,
			   struct ieee80211_low_level_stats *stats)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_mib_stats *mib_stats = &ah->ah_mibStats;

	stats->dot11ACKFailureCount = mib_stats->ackrcv_bad;
	stats->dot11RTSFailureCount = mib_stats->rts_bad;
	stats->dot11FCSErrorCount = mib_stats->fcs_bad;
	stats->dot11RTSSuccessCount = mib_stats->rts_good;
	return 0;
}

static u32 fill_chainmask(u32 cap, u32 new)
{
	u32 filled = 0;
	int i;

	for (i = 0; cap && new; i++, cap >>= 1) {
		if (!(cap & BIT(0)))
			continue;

		if (new & BIT(0))
			filled |= BIT(i);

		new >>= 1;
	}

	return filled;
}

<<<<<<< HEAD
=======
static bool validate_antenna_mask(struct ath_hw *ah, u32 val)
{
	if (AR_SREV_9300_20_OR_LATER(ah))
		return true;

	switch (val & 0x7) {
	case 0x1:
	case 0x3:
	case 0x7:
		return true;
	case 0x2:
		return (ah->caps.rx_chainmask == 1);
	default:
		return false;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ath9k_set_antenna(struct ieee80211_hw *hw, u32 tx_ant, u32 rx_ant)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;

<<<<<<< HEAD
	if (!rx_ant || !tx_ant)
=======
	if (ah->caps.rx_chainmask != 1)
		rx_ant |= tx_ant;

	if (!validate_antenna_mask(ah, rx_ant) || !tx_ant)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	sc->ant_rx = rx_ant;
	sc->ant_tx = tx_ant;

	if (ah->caps.rx_chainmask == 1)
		return 0;

	/* AR9100 runs into calibration issues if not all rx chains are enabled */
	if (AR_SREV_9100(ah))
		ah->rxchainmask = 0x7;
	else
		ah->rxchainmask = fill_chainmask(ah->caps.rx_chainmask, rx_ant);

	ah->txchainmask = fill_chainmask(ah->caps.tx_chainmask, tx_ant);
<<<<<<< HEAD
	ath9k_reload_chainmask_settings(sc);
=======
	ath9k_cmn_reload_chainmask(ah);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ath9k_get_antenna(struct ieee80211_hw *hw, u32 *tx_ant, u32 *rx_ant)
{
	struct ath_softc *sc = hw->priv;

	*tx_ant = sc->ant_tx;
	*rx_ant = sc->ant_rx;
	return 0;
}

<<<<<<< HEAD
struct ieee80211_ops ath9k_ops = {
=======
static void ath9k_sw_scan_start(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				const u8 *mac_addr)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct cfg80211_chan_def *chandef = &sc->cur_chan->chandef;
	struct ieee80211_channel *chan = chandef->chan;
	int pos = chan->hw_value;
	set_bit(ATH_OP_SCANNING, &common->op_flags);

	/* Reset current survey */
	if (!sc->cur_chan->offchannel) {
		if (sc->cur_survey != &sc->survey[pos]) {
			if (sc->cur_survey)
				sc->cur_survey->filled &= ~SURVEY_INFO_IN_USE;
			sc->cur_survey = &sc->survey[pos];
		}

		memset(sc->cur_survey, 0, sizeof(struct survey_info));
		sc->cur_survey->filled |= SURVEY_INFO_IN_USE;
	}
}

static void ath9k_sw_scan_complete(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	clear_bit(ATH_OP_SCANNING, &common->op_flags);
}

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT

static void ath9k_cancel_pending_offchannel(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	if (sc->offchannel.roc_vif) {
		ath_dbg(common, CHAN_CTX,
			"%s: Aborting RoC\n", __func__);

		del_timer_sync(&sc->offchannel.timer);
		if (sc->offchannel.state >= ATH_OFFCHANNEL_ROC_START)
			ath_roc_complete(sc, ATH_ROC_COMPLETE_ABORT);
	}

	if (test_bit(ATH_OP_SCANNING, &common->op_flags)) {
		ath_dbg(common, CHAN_CTX,
			"%s: Aborting HW scan\n", __func__);

		del_timer_sync(&sc->offchannel.timer);
		ath_scan_complete(sc, true);
	}
}

static int ath9k_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			 struct ieee80211_scan_request *hw_req)
{
	struct cfg80211_scan_request *req = &hw_req->req;
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	mutex_lock(&sc->mutex);

	if (WARN_ON(sc->offchannel.scan_req)) {
		ret = -EBUSY;
		goto out;
	}

	ath9k_ps_wakeup(sc);
	set_bit(ATH_OP_SCANNING, &common->op_flags);
	sc->offchannel.scan_vif = vif;
	sc->offchannel.scan_req = req;
	sc->offchannel.scan_idx = 0;

	ath_dbg(common, CHAN_CTX, "HW scan request received on vif: %pM\n",
		vif->addr);

	if (sc->offchannel.state == ATH_OFFCHANNEL_IDLE) {
		ath_dbg(common, CHAN_CTX, "Starting HW scan\n");
		ath_offchannel_next(sc);
	}

out:
	mutex_unlock(&sc->mutex);

	return ret;
}

static void ath9k_cancel_hw_scan(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	ath_dbg(common, CHAN_CTX, "Cancel HW scan on vif: %pM\n", vif->addr);

	mutex_lock(&sc->mutex);
	del_timer_sync(&sc->offchannel.timer);
	ath_scan_complete(sc, true);
	mutex_unlock(&sc->mutex);
}

static int ath9k_remain_on_channel(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_channel *chan, int duration,
				   enum ieee80211_roc_type type)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int ret = 0;

	mutex_lock(&sc->mutex);

	if (WARN_ON(sc->offchannel.roc_vif)) {
		ret = -EBUSY;
		goto out;
	}

	ath9k_ps_wakeup(sc);
	sc->offchannel.roc_vif = vif;
	sc->offchannel.roc_chan = chan;
	sc->offchannel.roc_duration = duration;

	ath_dbg(common, CHAN_CTX,
		"RoC request on vif: %pM, type: %d duration: %d\n",
		vif->addr, type, duration);

	if (sc->offchannel.state == ATH_OFFCHANNEL_IDLE) {
		ath_dbg(common, CHAN_CTX, "Starting RoC period\n");
		ath_offchannel_next(sc);
	}

out:
	mutex_unlock(&sc->mutex);

	return ret;
}

static int ath9k_cancel_remain_on_channel(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX, "Cancel RoC\n");
	del_timer_sync(&sc->offchannel.timer);

	if (sc->offchannel.roc_vif) {
		if (sc->offchannel.state >= ATH_OFFCHANNEL_ROC_START)
			ath_roc_complete(sc, ATH_ROC_COMPLETE_CANCEL);
	}

	mutex_unlock(&sc->mutex);

	return 0;
}

static int ath9k_add_chanctx(struct ieee80211_hw *hw,
			     struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx, **ptr;
	int pos;

	mutex_lock(&sc->mutex);

	ath_for_each_chanctx(sc, ctx) {
		if (ctx->assigned)
			continue;

		ptr = (void *) conf->drv_priv;
		*ptr = ctx;
		ctx->assigned = true;
		pos = ctx - &sc->chanctx[0];
		ctx->hw_queue_base = pos * IEEE80211_NUM_ACS;

		ath_dbg(common, CHAN_CTX,
			"Add channel context: %d MHz\n",
			conf->def.chan->center_freq);

		ath_chanctx_set_channel(sc, ctx, &conf->def);

		mutex_unlock(&sc->mutex);
		return 0;
	}

	mutex_unlock(&sc->mutex);
	return -ENOSPC;
}


static void ath9k_remove_chanctx(struct ieee80211_hw *hw,
				 struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx = ath_chanctx_get(conf);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Remove channel context: %d MHz\n",
		conf->def.chan->center_freq);

	ctx->assigned = false;
	ctx->hw_queue_base = 0;
	ath_chanctx_event(sc, NULL, ATH_CHANCTX_EVENT_UNASSIGN);

	mutex_unlock(&sc->mutex);
}

static void ath9k_change_chanctx(struct ieee80211_hw *hw,
				 struct ieee80211_chanctx_conf *conf,
				 u32 changed)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_chanctx *ctx = ath_chanctx_get(conf);

	mutex_lock(&sc->mutex);
	ath_dbg(common, CHAN_CTX,
		"Change channel context: %d MHz\n",
		conf->def.chan->center_freq);
	ath_chanctx_set_channel(sc, ctx, &conf->def);
	mutex_unlock(&sc->mutex);
}

static int ath9k_assign_vif_chanctx(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    struct ieee80211_bss_conf *link_conf,
				    struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_chanctx *ctx = ath_chanctx_get(conf);
	int i;

	ath9k_cancel_pending_offchannel(sc);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Assign VIF (addr: %pM, type: %d, p2p: %d) to channel context: %d MHz\n",
		vif->addr, vif->type, vif->p2p,
		conf->def.chan->center_freq);

	avp->chanctx = ctx;
	ctx->nvifs_assigned++;
	list_add_tail(&avp->list, &ctx->vifs);
	ath9k_calculate_summary_state(sc, ctx);
	for (i = 0; i < IEEE80211_NUM_ACS; i++)
		vif->hw_queue[i] = ctx->hw_queue_base + i;

	mutex_unlock(&sc->mutex);

	return 0;
}

static void ath9k_unassign_vif_chanctx(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif,
				       struct ieee80211_bss_conf *link_conf,
				       struct ieee80211_chanctx_conf *conf)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (void *)vif->drv_priv;
	struct ath_chanctx *ctx = ath_chanctx_get(conf);
	int ac;

	ath9k_cancel_pending_offchannel(sc);

	mutex_lock(&sc->mutex);

	ath_dbg(common, CHAN_CTX,
		"Remove VIF (addr: %pM, type: %d, p2p: %d) from channel context: %d MHz\n",
		vif->addr, vif->type, vif->p2p,
		conf->def.chan->center_freq);

	avp->chanctx = NULL;
	ctx->nvifs_assigned--;
	list_del(&avp->list);
	ath9k_calculate_summary_state(sc, ctx);
	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++)
		vif->hw_queue[ac] = IEEE80211_INVAL_HW_QUEUE;

	mutex_unlock(&sc->mutex);
}

static void ath9k_mgd_prepare_tx(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif,
				 struct ieee80211_prep_tx_info *info)
{
	struct ath_softc *sc = hw->priv;
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	struct ath_vif *avp = (struct ath_vif *) vif->drv_priv;
	struct ath_beacon_config *cur_conf;
	struct ath_chanctx *go_ctx;
	unsigned long timeout;
	bool changed = false;
	u32 beacon_int;

	if (!test_bit(ATH_OP_MULTI_CHANNEL, &common->op_flags))
		return;

	if (!avp->chanctx)
		return;

	mutex_lock(&sc->mutex);

	spin_lock_bh(&sc->chan_lock);
	if (sc->next_chan || (sc->cur_chan != avp->chanctx))
		changed = true;
	spin_unlock_bh(&sc->chan_lock);

	if (!changed)
		goto out;

	ath9k_cancel_pending_offchannel(sc);

	go_ctx = ath_is_go_chanctx_present(sc);

	if (go_ctx) {
		/*
		 * Wait till the GO interface gets a chance
		 * to send out an NoA.
		 */
		spin_lock_bh(&sc->chan_lock);
		sc->sched.mgd_prepare_tx = true;
		cur_conf = &go_ctx->beacon;
		beacon_int = TU_TO_USEC(cur_conf->beacon_interval);
		spin_unlock_bh(&sc->chan_lock);

		timeout = usecs_to_jiffies(beacon_int * 2);
		init_completion(&sc->go_beacon);

		mutex_unlock(&sc->mutex);

		if (wait_for_completion_timeout(&sc->go_beacon,
						timeout) == 0) {
			ath_dbg(common, CHAN_CTX,
				"Failed to send new NoA\n");

			spin_lock_bh(&sc->chan_lock);
			sc->sched.mgd_prepare_tx = false;
			spin_unlock_bh(&sc->chan_lock);
		}

		mutex_lock(&sc->mutex);
	}

	ath_dbg(common, CHAN_CTX,
		"%s: Set chanctx state to FORCE_ACTIVE for vif: %pM\n",
		__func__, vif->addr);

	spin_lock_bh(&sc->chan_lock);
	sc->next_chan = avp->chanctx;
	sc->sched.state = ATH_CHANCTX_STATE_FORCE_ACTIVE;
	spin_unlock_bh(&sc->chan_lock);

	ath_chanctx_set_next(sc, true);
out:
	mutex_unlock(&sc->mutex);
}

void ath9k_fill_chanctx_ops(void)
{
	if (!ath9k_is_chanctx_enabled())
		return;

	ath9k_ops.hw_scan                  = ath9k_hw_scan;
	ath9k_ops.cancel_hw_scan           = ath9k_cancel_hw_scan;
	ath9k_ops.remain_on_channel        = ath9k_remain_on_channel;
	ath9k_ops.cancel_remain_on_channel = ath9k_cancel_remain_on_channel;
	ath9k_ops.add_chanctx              = ath9k_add_chanctx;
	ath9k_ops.remove_chanctx           = ath9k_remove_chanctx;
	ath9k_ops.change_chanctx           = ath9k_change_chanctx;
	ath9k_ops.assign_vif_chanctx       = ath9k_assign_vif_chanctx;
	ath9k_ops.unassign_vif_chanctx     = ath9k_unassign_vif_chanctx;
	ath9k_ops.mgd_prepare_tx           = ath9k_mgd_prepare_tx;
}

#endif

static int ath9k_get_txpower(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			     int *dbm)
{
	struct ath_softc *sc = hw->priv;
	struct ath_vif *avp = (void *)vif->drv_priv;

	mutex_lock(&sc->mutex);
	if (avp->chanctx)
		*dbm = avp->chanctx->cur_txpower;
	else
		*dbm = sc->cur_chan->cur_txpower;
	mutex_unlock(&sc->mutex);

	*dbm /= 2;

	return 0;
}

struct ieee80211_ops ath9k_ops = {
	.add_chanctx = ieee80211_emulate_add_chanctx,
	.remove_chanctx = ieee80211_emulate_remove_chanctx,
	.change_chanctx = ieee80211_emulate_change_chanctx,
	.switch_vif_chanctx = ieee80211_emulate_switch_vif_chanctx,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.tx 		    = ath9k_tx,
	.start 		    = ath9k_start,
	.stop 		    = ath9k_stop,
	.add_interface 	    = ath9k_add_interface,
	.change_interface   = ath9k_change_interface,
	.remove_interface   = ath9k_remove_interface,
	.config 	    = ath9k_config,
	.configure_filter   = ath9k_configure_filter,
<<<<<<< HEAD
	.sta_add	    = ath9k_sta_add,
	.sta_remove	    = ath9k_sta_remove,
=======
	.sta_state          = ath9k_sta_state,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.sta_notify         = ath9k_sta_notify,
	.conf_tx 	    = ath9k_conf_tx,
	.bss_info_changed   = ath9k_bss_info_changed,
	.set_key            = ath9k_set_key,
	.get_tsf 	    = ath9k_get_tsf,
	.set_tsf 	    = ath9k_set_tsf,
	.reset_tsf 	    = ath9k_reset_tsf,
	.ampdu_action       = ath9k_ampdu_action,
	.get_survey	    = ath9k_get_survey,
	.rfkill_poll        = ath9k_rfkill_poll_state,
	.set_coverage_class = ath9k_set_coverage_class,
	.flush		    = ath9k_flush,
	.tx_frames_pending  = ath9k_tx_frames_pending,
	.tx_last_beacon     = ath9k_tx_last_beacon,
<<<<<<< HEAD
	.get_stats	    = ath9k_get_stats,
	.set_antenna	    = ath9k_set_antenna,
	.get_antenna	    = ath9k_get_antenna,
=======
	.release_buffered_frames = ath9k_release_buffered_frames,
	.get_stats	    = ath9k_get_stats,
	.set_antenna	    = ath9k_set_antenna,
	.get_antenna	    = ath9k_get_antenna,

#ifdef CONFIG_ATH9K_WOW
	.suspend	    = ath9k_suspend,
	.resume		    = ath9k_resume,
	.set_wakeup	    = ath9k_set_wakeup,
#endif

#ifdef CONFIG_ATH9K_DEBUGFS
	.get_et_sset_count  = ath9k_get_et_sset_count,
	.get_et_stats       = ath9k_get_et_stats,
	.get_et_strings     = ath9k_get_et_strings,
#endif

#if defined(CONFIG_MAC80211_DEBUGFS) && defined(CONFIG_ATH9K_STATION_STATISTICS)
	.sta_add_debugfs    = ath9k_sta_add_debugfs,
#endif
	.sw_scan_start	    = ath9k_sw_scan_start,
	.sw_scan_complete   = ath9k_sw_scan_complete,
	.get_txpower        = ath9k_get_txpower,
	.wake_tx_queue      = ath9k_wake_tx_queue,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
