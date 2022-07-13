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

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/ath9k_platform.h>
#include <linux/module.h>
<<<<<<< HEAD

#include "ath9k.h"

=======
#include <linux/of.h>
#include <linux/of_net.h>
#include <linux/nvmem-consumer.h>
#include <linux/relay.h>
#include <linux/dmi.h>
#include <net/ieee80211_radiotap.h>

#include "ath9k.h"

struct ath9k_eeprom_ctx {
	struct completion complete;
	struct ath_hw *ah;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static char *dev_info = "ath9k";

MODULE_AUTHOR("Atheros Communications");
MODULE_DESCRIPTION("Support for Atheros 802.11n wireless LAN cards.");
<<<<<<< HEAD
MODULE_SUPPORTED_DEVICE("Atheros 802.11n WLAN cards");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int ath9k_debug = ATH_DBG_DEFAULT;
module_param_named(debug, ath9k_debug, uint, 0);
MODULE_PARM_DESC(debug, "Debugging mask");

int ath9k_modparam_nohwcrypt;
module_param_named(nohwcrypt, ath9k_modparam_nohwcrypt, int, 0444);
MODULE_PARM_DESC(nohwcrypt, "Disable hardware encryption");

<<<<<<< HEAD
int led_blink;
module_param_named(blink, led_blink, int, 0444);
MODULE_PARM_DESC(blink, "Enable LED blink on activity");

=======
int ath9k_led_blink;
module_param_named(blink, ath9k_led_blink, int, 0444);
MODULE_PARM_DESC(blink, "Enable LED blink on activity");

static int ath9k_led_active_high = -1;
module_param_named(led_active_high, ath9k_led_active_high, int, 0444);
MODULE_PARM_DESC(led_active_high, "Invert LED polarity");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ath9k_btcoex_enable;
module_param_named(btcoex_enable, ath9k_btcoex_enable, int, 0444);
MODULE_PARM_DESC(btcoex_enable, "Enable wifi-BT coexistence");

<<<<<<< HEAD
bool is_ath9k_unloaded;
/* We use the hw_value as an index into our private channel structure */

#define CHAN2G(_freq, _idx)  { \
	.band = IEEE80211_BAND_2GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 20, \
}

#define CHAN5G(_freq, _idx) { \
	.band = IEEE80211_BAND_5GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 20, \
}

/* Some 2 GHz radios are actually tunable on 2312-2732
 * on 5 MHz steps, we support the channels which we know
 * we have calibration data for all cards though to make
 * this static */
static const struct ieee80211_channel ath9k_2ghz_chantable[] = {
	CHAN2G(2412, 0), /* Channel 1 */
	CHAN2G(2417, 1), /* Channel 2 */
	CHAN2G(2422, 2), /* Channel 3 */
	CHAN2G(2427, 3), /* Channel 4 */
	CHAN2G(2432, 4), /* Channel 5 */
	CHAN2G(2437, 5), /* Channel 6 */
	CHAN2G(2442, 6), /* Channel 7 */
	CHAN2G(2447, 7), /* Channel 8 */
	CHAN2G(2452, 8), /* Channel 9 */
	CHAN2G(2457, 9), /* Channel 10 */
	CHAN2G(2462, 10), /* Channel 11 */
	CHAN2G(2467, 11), /* Channel 12 */
	CHAN2G(2472, 12), /* Channel 13 */
	CHAN2G(2484, 13), /* Channel 14 */
};

/* Some 5 GHz radios are actually tunable on XXXX-YYYY
 * on 5 MHz steps, we support the channels which we know
 * we have calibration data for all cards though to make
 * this static */
static const struct ieee80211_channel ath9k_5ghz_chantable[] = {
	/* _We_ call this UNII 1 */
	CHAN5G(5180, 14), /* Channel 36 */
	CHAN5G(5200, 15), /* Channel 40 */
	CHAN5G(5220, 16), /* Channel 44 */
	CHAN5G(5240, 17), /* Channel 48 */
	/* _We_ call this UNII 2 */
	CHAN5G(5260, 18), /* Channel 52 */
	CHAN5G(5280, 19), /* Channel 56 */
	CHAN5G(5300, 20), /* Channel 60 */
	CHAN5G(5320, 21), /* Channel 64 */
	/* _We_ call this "Middle band" */
	CHAN5G(5500, 22), /* Channel 100 */
	CHAN5G(5520, 23), /* Channel 104 */
	CHAN5G(5540, 24), /* Channel 108 */
	CHAN5G(5560, 25), /* Channel 112 */
	CHAN5G(5580, 26), /* Channel 116 */
	CHAN5G(5600, 27), /* Channel 120 */
	CHAN5G(5620, 28), /* Channel 124 */
	CHAN5G(5640, 29), /* Channel 128 */
	CHAN5G(5660, 30), /* Channel 132 */
	CHAN5G(5680, 31), /* Channel 136 */
	CHAN5G(5700, 32), /* Channel 140 */
	/* _We_ call this UNII 3 */
	CHAN5G(5745, 33), /* Channel 149 */
	CHAN5G(5765, 34), /* Channel 153 */
	CHAN5G(5785, 35), /* Channel 157 */
	CHAN5G(5805, 36), /* Channel 161 */
	CHAN5G(5825, 37), /* Channel 165 */
};

/* Atheros hardware rate code addition for short premble */
#define SHPCHECK(__hw_rate, __flags) \
	((__flags & IEEE80211_RATE_SHORT_PREAMBLE) ? (__hw_rate | 0x04 ) : 0)

#define RATE(_bitrate, _hw_rate, _flags) {              \
	.bitrate        = (_bitrate),                   \
	.flags          = (_flags),                     \
	.hw_value       = (_hw_rate),                   \
	.hw_value_short = (SHPCHECK(_hw_rate, _flags))  \
}

static struct ieee80211_rate ath9k_legacy_rates[] = {
	RATE(10, 0x1b, 0),
	RATE(20, 0x1a, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(55, 0x19, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(110, 0x18, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(60, 0x0b, 0),
	RATE(90, 0x0f, 0),
	RATE(120, 0x0a, 0),
	RATE(180, 0x0e, 0),
	RATE(240, 0x09, 0),
	RATE(360, 0x0d, 0),
	RATE(480, 0x08, 0),
	RATE(540, 0x0c, 0),
};
=======
static int ath9k_bt_ant_diversity;
module_param_named(bt_ant_diversity, ath9k_bt_ant_diversity, int, 0444);
MODULE_PARM_DESC(bt_ant_diversity, "Enable WLAN/BT RX antenna diversity");

static int ath9k_ps_enable;
module_param_named(ps_enable, ath9k_ps_enable, int, 0444);
MODULE_PARM_DESC(ps_enable, "Enable WLAN PowerSave");

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT

int ath9k_use_chanctx;
module_param_named(use_chanctx, ath9k_use_chanctx, int, 0444);
MODULE_PARM_DESC(use_chanctx, "Enable channel context for concurrency");

#endif /* CONFIG_ATH9K_CHANNEL_CONTEXT */

int ath9k_use_msi;
module_param_named(use_msi, ath9k_use_msi, int, 0444);
MODULE_PARM_DESC(use_msi, "Use MSI instead of INTx if possible");

bool is_ath9k_unloaded;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_MAC80211_LEDS
static const struct ieee80211_tpt_blink ath9k_tpt_blink[] = {
	{ .throughput = 0 * 1024, .blink_time = 334 },
	{ .throughput = 1 * 1024, .blink_time = 260 },
	{ .throughput = 5 * 1024, .blink_time = 220 },
	{ .throughput = 10 * 1024, .blink_time = 190 },
	{ .throughput = 20 * 1024, .blink_time = 170 },
	{ .throughput = 50 * 1024, .blink_time = 150 },
	{ .throughput = 70 * 1024, .blink_time = 130 },
	{ .throughput = 100 * 1024, .blink_time = 110 },
	{ .throughput = 200 * 1024, .blink_time = 80 },
	{ .throughput = 300 * 1024, .blink_time = 50 },
};
#endif

<<<<<<< HEAD
static void ath9k_deinit_softc(struct ath_softc *sc);

=======
static int __init set_use_msi(const struct dmi_system_id *dmi)
{
	ath9k_use_msi = 1;
	return 1;
}

static const struct dmi_system_id ath9k_quirks[] __initconst = {
	{
		.callback = set_use_msi,
		.ident = "Dell Inspiron 24-3460",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron 24-3460"),
		},
	},
	{
		.callback = set_use_msi,
		.ident = "Dell Vostro 3262",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Vostro 3262"),
		},
	},
	{
		.callback = set_use_msi,
		.ident = "Dell Inspiron 3472",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron 3472"),
		},
	},
	{
		.callback = set_use_msi,
		.ident = "Dell Vostro 15-3572",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Vostro 15-3572"),
		},
	},
	{
		.callback = set_use_msi,
		.ident = "Dell Inspiron 14-3473",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron 14-3473"),
		},
	},
	{}
};

static void ath9k_deinit_softc(struct ath_softc *sc);

static void ath9k_op_ps_wakeup(struct ath_common *common)
{
	ath9k_ps_wakeup(common->priv);
}

static void ath9k_op_ps_restore(struct ath_common *common)
{
	ath9k_ps_restore(common->priv);
}

static const struct ath_ps_ops ath9k_ps_ops = {
	.wakeup = ath9k_op_ps_wakeup,
	.restore = ath9k_op_ps_restore,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Read and write, they both share the same lock. We do this to serialize
 * reads and writes on Atheros 802.11n PCI devices only. This is required
 * as the FIFO on these devices can only accept sanely 2 requests.
 */

static void ath9k_iowrite32(void *hw_priv, u32 val, u32 reg_offset)
{
<<<<<<< HEAD
	struct ath_hw *ah = (struct ath_hw *) hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = (struct ath_softc *) common->priv;
=======
	struct ath_hw *ah = hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = common->priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (NR_CPUS > 1 && ah->config.serialize_regmode == SER_REG_MODE_ON) {
		unsigned long flags;
		spin_lock_irqsave(&sc->sc_serial_rw, flags);
		iowrite32(val, sc->mem + reg_offset);
		spin_unlock_irqrestore(&sc->sc_serial_rw, flags);
	} else
		iowrite32(val, sc->mem + reg_offset);
}

static unsigned int ath9k_ioread32(void *hw_priv, u32 reg_offset)
{
<<<<<<< HEAD
	struct ath_hw *ah = (struct ath_hw *) hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = (struct ath_softc *) common->priv;
=======
	struct ath_hw *ah = hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = common->priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 val;

	if (NR_CPUS > 1 && ah->config.serialize_regmode == SER_REG_MODE_ON) {
		unsigned long flags;
		spin_lock_irqsave(&sc->sc_serial_rw, flags);
		val = ioread32(sc->mem + reg_offset);
		spin_unlock_irqrestore(&sc->sc_serial_rw, flags);
	} else
		val = ioread32(sc->mem + reg_offset);
	return val;
}

<<<<<<< HEAD
=======
static void ath9k_multi_ioread32(void *hw_priv, u32 *addr,
                                u32 *val, u16 count)
{
	int i;

	for (i = 0; i < count; i++)
		val[i] = ath9k_ioread32(hw_priv, addr[i]);
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned int __ath9k_reg_rmw(struct ath_softc *sc, u32 reg_offset,
				    u32 set, u32 clr)
{
	u32 val;

	val = ioread32(sc->mem + reg_offset);
	val &= ~clr;
	val |= set;
	iowrite32(val, sc->mem + reg_offset);

	return val;
}

static unsigned int ath9k_reg_rmw(void *hw_priv, u32 reg_offset, u32 set, u32 clr)
{
<<<<<<< HEAD
	struct ath_hw *ah = (struct ath_hw *) hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = (struct ath_softc *) common->priv;
	unsigned long uninitialized_var(flags);
=======
	struct ath_hw *ah = hw_priv;
	struct ath_common *common = ath9k_hw_common(ah);
	struct ath_softc *sc = common->priv;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 val;

	if (NR_CPUS > 1 && ah->config.serialize_regmode == SER_REG_MODE_ON) {
		spin_lock_irqsave(&sc->sc_serial_rw, flags);
		val = __ath9k_reg_rmw(sc, reg_offset, set, clr);
		spin_unlock_irqrestore(&sc->sc_serial_rw, flags);
	} else
		val = __ath9k_reg_rmw(sc, reg_offset, set, clr);

	return val;
}

/**************************/
/*     Initialization     */
/**************************/

<<<<<<< HEAD
static void setup_ht_cap(struct ath_softc *sc,
			 struct ieee80211_sta_ht_cap *ht_info)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u8 tx_streams, rx_streams;
	int i, max_streams;

	ht_info->ht_supported = true;
	ht_info->cap = IEEE80211_HT_CAP_SUP_WIDTH_20_40 |
		       IEEE80211_HT_CAP_SM_PS |
		       IEEE80211_HT_CAP_SGI_40 |
		       IEEE80211_HT_CAP_DSSSCCK40;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_LDPC)
		ht_info->cap |= IEEE80211_HT_CAP_LDPC_CODING;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_SGI_20)
		ht_info->cap |= IEEE80211_HT_CAP_SGI_20;

	ht_info->ampdu_factor = IEEE80211_HT_MAX_AMPDU_64K;
	ht_info->ampdu_density = IEEE80211_HT_MPDU_DENSITY_8;

	if (AR_SREV_9330(ah) || AR_SREV_9485(ah))
		max_streams = 1;
	else if (AR_SREV_9462(ah))
		max_streams = 2;
	else if (AR_SREV_9300_20_OR_LATER(ah))
		max_streams = 3;
	else
		max_streams = 2;

	if (AR_SREV_9280_20_OR_LATER(ah)) {
		if (max_streams >= 2)
			ht_info->cap |= IEEE80211_HT_CAP_TX_STBC;
		ht_info->cap |= (1 << IEEE80211_HT_CAP_RX_STBC_SHIFT);
	}

	/* set up supported mcs set */
	memset(&ht_info->mcs, 0, sizeof(ht_info->mcs));
	tx_streams = ath9k_cmn_count_streams(ah->txchainmask, max_streams);
	rx_streams = ath9k_cmn_count_streams(ah->rxchainmask, max_streams);

	ath_dbg(common, CONFIG, "TX streams %d, RX streams: %d\n",
		tx_streams, rx_streams);

	if (tx_streams != rx_streams) {
		ht_info->mcs.tx_params |= IEEE80211_HT_MCS_TX_RX_DIFF;
		ht_info->mcs.tx_params |= ((tx_streams - 1) <<
				IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT);
	}

	for (i = 0; i < rx_streams; i++)
		ht_info->mcs.rx_mask[i] = 0xff;

	ht_info->mcs.tx_params |= IEEE80211_HT_MCS_TX_DEFINED;
}

static int ath9k_reg_notifier(struct wiphy *wiphy,
			      struct regulatory_request *request)
=======
static void ath9k_reg_notifier(struct wiphy *wiphy,
			       struct regulatory_request *request)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_regulatory *reg = ath9k_hw_regulatory(ah);
<<<<<<< HEAD
	int ret;

	ret = ath_reg_notifier_apply(wiphy, request, reg);

	/* Set tx power */
	if (ah->curchan) {
		sc->config.txpowlimit = 2 * ah->curchan->chan->max_power;
		ath9k_ps_wakeup(sc);
		ath9k_hw_set_txpowerlimit(ah, sc->config.txpowlimit, false);
		sc->curtxpow = ath9k_hw_regulatory(ah)->power_limit;
		ath9k_ps_restore(sc);
	}

	return ret;
=======

	ath_reg_notifier_apply(wiphy, request, reg);

	/* synchronize DFS detector if regulatory domain changed */
	if (sc->dfs_detector != NULL)
		sc->dfs_detector->set_dfs_domain(sc->dfs_detector,
						 request->dfs_region);

	/* Set tx power */
	if (!ah->curchan)
		return;

	sc->cur_chan->txpower = 2 * ah->curchan->chan->max_power;
	ath9k_ps_wakeup(sc);
	ath9k_hw_set_txpowerlimit(ah, sc->cur_chan->txpower, false);
	ath9k_cmn_update_txpow(ah, sc->cur_chan->cur_txpower,
			       sc->cur_chan->txpower,
			       &sc->cur_chan->cur_txpower);
	ath9k_ps_restore(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *  This function will allocate both the DMA descriptor structure, and the
 *  buffers it contains.  These are used to contain the descriptors used
 *  by the system.
*/
int ath_descdma_setup(struct ath_softc *sc, struct ath_descdma *dd,
		      struct list_head *head, const char *name,
		      int nbuf, int ndesc, bool is_tx)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	u8 *ds;
<<<<<<< HEAD
	struct ath_buf *bf;
	int i, bsize, error, desc_len;
=======
	int i, bsize, desc_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath_dbg(common, CONFIG, "%s DMA: %u buffers %u desc/buf\n",
		name, nbuf, ndesc);

	INIT_LIST_HEAD(head);

	if (is_tx)
		desc_len = sc->sc_ah->caps.tx_desc_len;
	else
		desc_len = sizeof(struct ath_desc);

	/* ath_desc must be a multiple of DWORDs */
	if ((desc_len % 4) != 0) {
		ath_err(common, "ath_desc not DWORD aligned\n");
		BUG_ON((desc_len % 4) != 0);
<<<<<<< HEAD
		error = -ENOMEM;
		goto fail;
=======
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	dd->dd_desc_len = desc_len * nbuf * ndesc;

	/*
	 * Need additional DMA memory because we can't use
	 * descriptors that cross the 4K page boundary. Assume
	 * one skipped descriptor per 4K page.
	 */
	if (!(sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_4KB_SPLITTRANS)) {
		u32 ndesc_skipped =
			ATH_DESC_4KB_BOUND_NUM_SKIPPED(dd->dd_desc_len);
		u32 dma_len;

		while (ndesc_skipped) {
			dma_len = ndesc_skipped * desc_len;
			dd->dd_desc_len += dma_len;

			ndesc_skipped = ATH_DESC_4KB_BOUND_NUM_SKIPPED(dma_len);
		}
	}

	/* allocate descriptors */
<<<<<<< HEAD
	dd->dd_desc = dma_alloc_coherent(sc->dev, dd->dd_desc_len,
					 &dd->dd_desc_paddr, GFP_KERNEL);
	if (dd->dd_desc == NULL) {
		error = -ENOMEM;
		goto fail;
	}
	ds = (u8 *) dd->dd_desc;
=======
	dd->dd_desc = dmam_alloc_coherent(sc->dev, dd->dd_desc_len,
					  &dd->dd_desc_paddr, GFP_KERNEL);
	if (!dd->dd_desc)
		return -ENOMEM;

	ds = dd->dd_desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath_dbg(common, CONFIG, "%s DMA map: %p (%u) -> %llx (%u)\n",
		name, ds, (u32) dd->dd_desc_len,
		ito64(dd->dd_desc_paddr), /*XXX*/(u32) dd->dd_desc_len);

	/* allocate buffers */
<<<<<<< HEAD
	bsize = sizeof(struct ath_buf) * nbuf;
	bf = kzalloc(bsize, GFP_KERNEL);
	if (bf == NULL) {
		error = -ENOMEM;
		goto fail2;
	}
	dd->dd_bufptr = bf;

	for (i = 0; i < nbuf; i++, bf++, ds += (desc_len * ndesc)) {
		bf->bf_desc = ds;
		bf->bf_daddr = DS2PHYS(dd, ds);

		if (!(sc->sc_ah->caps.hw_caps &
		      ATH9K_HW_CAP_4KB_SPLITTRANS)) {
			/*
			 * Skip descriptor addresses which can cause 4KB
			 * boundary crossing (addr + length) with a 32 dword
			 * descriptor fetch.
			 */
			while (ATH_DESC_4KB_BOUND_CHECK(bf->bf_daddr)) {
				BUG_ON((caddr_t) bf->bf_desc >=
				       ((caddr_t) dd->dd_desc +
					dd->dd_desc_len));

				ds += (desc_len * ndesc);
				bf->bf_desc = ds;
				bf->bf_daddr = DS2PHYS(dd, ds);
			}
		}
		list_add_tail(&bf->list, head);
	}
	return 0;
fail2:
	dma_free_coherent(sc->dev, dd->dd_desc_len, dd->dd_desc,
			  dd->dd_desc_paddr);
fail:
	memset(dd, 0, sizeof(*dd));
	return error;
=======
	if (is_tx) {
		struct ath_buf *bf;

		bsize = sizeof(struct ath_buf) * nbuf;
		bf = devm_kzalloc(sc->dev, bsize, GFP_KERNEL);
		if (!bf)
			return -ENOMEM;

		for (i = 0; i < nbuf; i++, bf++, ds += (desc_len * ndesc)) {
			bf->bf_desc = ds;
			bf->bf_daddr = DS2PHYS(dd, ds);

			if (!(sc->sc_ah->caps.hw_caps &
				  ATH9K_HW_CAP_4KB_SPLITTRANS)) {
				/*
				 * Skip descriptor addresses which can cause 4KB
				 * boundary crossing (addr + length) with a 32 dword
				 * descriptor fetch.
				 */
				while (ATH_DESC_4KB_BOUND_CHECK(bf->bf_daddr)) {
					BUG_ON((caddr_t) bf->bf_desc >=
						   ((caddr_t) dd->dd_desc +
						dd->dd_desc_len));

					ds += (desc_len * ndesc);
					bf->bf_desc = ds;
					bf->bf_daddr = DS2PHYS(dd, ds);
				}
			}
			list_add_tail(&bf->list, head);
		}
	} else {
		struct ath_rxbuf *bf;

		bsize = sizeof(struct ath_rxbuf) * nbuf;
		bf = devm_kzalloc(sc->dev, bsize, GFP_KERNEL);
		if (!bf)
			return -ENOMEM;

		for (i = 0; i < nbuf; i++, bf++, ds += (desc_len * ndesc)) {
			bf->bf_desc = ds;
			bf->bf_daddr = DS2PHYS(dd, ds);

			if (!(sc->sc_ah->caps.hw_caps &
				  ATH9K_HW_CAP_4KB_SPLITTRANS)) {
				/*
				 * Skip descriptor addresses which can cause 4KB
				 * boundary crossing (addr + length) with a 32 dword
				 * descriptor fetch.
				 */
				while (ATH_DESC_4KB_BOUND_CHECK(bf->bf_daddr)) {
					BUG_ON((caddr_t) bf->bf_desc >=
						   ((caddr_t) dd->dd_desc +
						dd->dd_desc_len));

					ds += (desc_len * ndesc);
					bf->bf_desc = ds;
					bf->bf_daddr = DS2PHYS(dd, ds);
				}
			}
			list_add_tail(&bf->list, head);
		}
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_init_queues(struct ath_softc *sc)
{
	int i = 0;

	sc->beacon.beaconq = ath9k_hw_beaconq_setup(sc->sc_ah);
	sc->beacon.cabq = ath_txq_setup(sc, ATH9K_TX_QUEUE_CAB, 0);
<<<<<<< HEAD

	sc->config.cabqReadytime = ATH_CABQ_READY_TIME;
	ath_cabq_update(sc);

	for (i = 0; i < WME_NUM_AC; i++) {
=======
	ath_cabq_update(sc);

	sc->tx.uapsdq = ath_txq_setup(sc, ATH9K_TX_QUEUE_UAPSD, 0);

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sc->tx.txq_map[i] = ath_txq_setup(sc, ATH9K_TX_QUEUE_DATA, i);
		sc->tx.txq_map[i]->mac80211_qnum = i;
	}
	return 0;
}

<<<<<<< HEAD
static int ath9k_init_channels_rates(struct ath_softc *sc)
{
	void *channels;

	BUILD_BUG_ON(ARRAY_SIZE(ath9k_2ghz_chantable) +
		     ARRAY_SIZE(ath9k_5ghz_chantable) !=
		     ATH9K_NUM_CHANNELS);

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ) {
		channels = kmemdup(ath9k_2ghz_chantable,
			sizeof(ath9k_2ghz_chantable), GFP_KERNEL);
		if (!channels)
		    return -ENOMEM;

		sc->sbands[IEEE80211_BAND_2GHZ].channels = channels;
		sc->sbands[IEEE80211_BAND_2GHZ].band = IEEE80211_BAND_2GHZ;
		sc->sbands[IEEE80211_BAND_2GHZ].n_channels =
			ARRAY_SIZE(ath9k_2ghz_chantable);
		sc->sbands[IEEE80211_BAND_2GHZ].bitrates = ath9k_legacy_rates;
		sc->sbands[IEEE80211_BAND_2GHZ].n_bitrates =
			ARRAY_SIZE(ath9k_legacy_rates);
	}

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ) {
		channels = kmemdup(ath9k_5ghz_chantable,
			sizeof(ath9k_5ghz_chantable), GFP_KERNEL);
		if (!channels) {
			if (sc->sbands[IEEE80211_BAND_2GHZ].channels)
				kfree(sc->sbands[IEEE80211_BAND_2GHZ].channels);
			return -ENOMEM;
		}

		sc->sbands[IEEE80211_BAND_5GHZ].channels = channels;
		sc->sbands[IEEE80211_BAND_5GHZ].band = IEEE80211_BAND_5GHZ;
		sc->sbands[IEEE80211_BAND_5GHZ].n_channels =
			ARRAY_SIZE(ath9k_5ghz_chantable);
		sc->sbands[IEEE80211_BAND_5GHZ].bitrates =
			ath9k_legacy_rates + 4;
		sc->sbands[IEEE80211_BAND_5GHZ].n_bitrates =
			ARRAY_SIZE(ath9k_legacy_rates) - 4;
	}
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ath9k_init_misc(struct ath_softc *sc)
{
	struct ath_common *common = ath9k_hw_common(sc->sc_ah);
	int i = 0;

<<<<<<< HEAD
	setup_timer(&common->ani.timer, ath_ani_calibrate, (unsigned long)sc);

	sc->config.txpowlimit = ATH_TXPOWER_MAX;
	memcpy(common->bssidmask, ath_bcast_mac, ETH_ALEN);
	sc->beacon.slottime = ATH9K_SLOT_TIME_9;
=======
	timer_setup(&common->ani.timer, ath_ani_calibrate, 0);

	common->last_rssi = ATH_RSSI_DUMMY_MARKER;
	eth_broadcast_addr(common->bssidmask);
	sc->beacon.slottime = 9;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < ARRAY_SIZE(sc->beacon.bslot); i++)
		sc->beacon.bslot[i] = NULL;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB)
		sc->ant_comb.count = ATH_ANT_DIV_COMB_INIT_COUNT;
<<<<<<< HEAD
=======

	sc->spec_priv.ah = sc->sc_ah;
	sc->spec_priv.spec_config.enabled = 0;
	sc->spec_priv.spec_config.short_repeat = true;
	sc->spec_priv.spec_config.count = 8;
	sc->spec_priv.spec_config.endless = false;
	sc->spec_priv.spec_config.period = 0xFF;
	sc->spec_priv.spec_config.fft_period = 0xF;
}

static void ath9k_init_pcoem_platform(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_hw_capabilities *pCap = &ah->caps;
	struct ath_common *common = ath9k_hw_common(ah);

	if (!IS_ENABLED(CONFIG_ATH9K_PCOEM))
		return;

	if (common->bus_ops->ath_bus_type != ATH_PCI)
		return;

	if (sc->driver_data & (ATH9K_PCI_CUS198 |
			       ATH9K_PCI_CUS230)) {
		ah->config.xlna_gpio = 9;
		ah->config.xatten_margin_cfg = true;
		ah->config.alt_mingainidx = true;
		ah->config.ant_ctrl_comm2g_switch_enable = 0x000BBB88;
		sc->ant_comb.low_rssi_thresh = 20;
		sc->ant_comb.fast_div_bias = 3;

		ath_info(common, "Set parameters for %s\n",
			 (sc->driver_data & ATH9K_PCI_CUS198) ?
			 "CUS198" : "CUS230");
	}

	if (sc->driver_data & ATH9K_PCI_CUS217)
		ath_info(common, "CUS217 card detected\n");

	if (sc->driver_data & ATH9K_PCI_CUS252)
		ath_info(common, "CUS252 card detected\n");

	if (sc->driver_data & ATH9K_PCI_AR9565_1ANT)
		ath_info(common, "WB335 1-ANT card detected\n");

	if (sc->driver_data & ATH9K_PCI_AR9565_2ANT)
		ath_info(common, "WB335 2-ANT card detected\n");

	if (sc->driver_data & ATH9K_PCI_KILLER)
		ath_info(common, "Killer Wireless card detected\n");

	/*
	 * Some WB335 cards do not support antenna diversity. Since
	 * we use a hardcoded value for AR9565 instead of using the
	 * EEPROM/OTP data, remove the combining feature from
	 * the HW capabilities bitmap.
	 */
	if (sc->driver_data & (ATH9K_PCI_AR9565_1ANT | ATH9K_PCI_AR9565_2ANT)) {
		if (!(sc->driver_data & ATH9K_PCI_BT_ANT_DIV))
			pCap->hw_caps &= ~ATH9K_HW_CAP_ANT_DIV_COMB;
	}

	if (sc->driver_data & ATH9K_PCI_BT_ANT_DIV) {
		pCap->hw_caps |= ATH9K_HW_CAP_BT_ANT_DIV;
		ath_info(common, "Set BT/WLAN RX diversity capability\n");
	}

	if (sc->driver_data & ATH9K_PCI_D3_L1_WAR) {
		ah->config.pcie_waen = 0x0040473b;
		ath_info(common, "Enable WAR for ASPM D3/L1\n");
	}

	/*
	 * The default value of pll_pwrsave is 1.
	 * For certain AR9485 cards, it is set to 0.
	 * For AR9462, AR9565 it's set to 7.
	 */
	ah->config.pll_pwrsave = 1;

	if (sc->driver_data & ATH9K_PCI_NO_PLL_PWRSAVE) {
		ah->config.pll_pwrsave = 0;
		ath_info(common, "Disable PLL PowerSave\n");
	}

	if (sc->driver_data & ATH9K_PCI_LED_ACT_HI)
		ah->config.led_active_high = true;
}

static void ath9k_eeprom_request_cb(const struct firmware *eeprom_blob,
				    void *ctx)
{
	struct ath9k_eeprom_ctx *ec = ctx;

	if (eeprom_blob)
		ec->ah->eeprom_blob = eeprom_blob;

	complete(&ec->complete);
}

static int ath9k_eeprom_request(struct ath_softc *sc, const char *name)
{
	struct ath9k_eeprom_ctx ec;
	struct ath_hw *ah = sc->sc_ah;
	int err;

	/* try to load the EEPROM content asynchronously */
	init_completion(&ec.complete);
	ec.ah = sc->sc_ah;

	err = request_firmware_nowait(THIS_MODULE, 1, name, sc->dev, GFP_KERNEL,
				      &ec, ath9k_eeprom_request_cb);
	if (err < 0) {
		ath_err(ath9k_hw_common(ah),
			"EEPROM request failed\n");
		return err;
	}

	wait_for_completion(&ec.complete);

	if (!ah->eeprom_blob) {
		ath_err(ath9k_hw_common(ah),
			"Unable to load EEPROM file %s\n", name);
		return -EINVAL;
	}

	return 0;
}

static void ath9k_eeprom_release(struct ath_softc *sc)
{
	release_firmware(sc->sc_ah->eeprom_blob);
}

static int ath9k_nvmem_request_eeprom(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct nvmem_cell *cell;
	void *buf;
	size_t len;
	int err;

	cell = devm_nvmem_cell_get(sc->dev, "calibration");
	if (IS_ERR(cell)) {
		err = PTR_ERR(cell);

		/* nvmem cell might not be defined, or the nvmem
		 * subsystem isn't included. In this case, follow
		 * the established "just return 0;" convention of
		 * ath9k_init_platform to say:
		 * "All good. Nothing to see here. Please go on."
		 */
		if (err == -ENOENT || err == -EOPNOTSUPP)
			return 0;

		return err;
	}

	buf = nvmem_cell_read(cell, &len);
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	/* run basic sanity checks on the returned nvram cell length.
	 * That length has to be a multiple of a "u16" (i.e.: & 1).
	 * Furthermore, it has to be more than "let's say" 512 bytes
	 * but less than the maximum of AR9300_EEPROM_SIZE (16kb).
	 */
	if ((len & 1) == 1 || len < 512 || len >= AR9300_EEPROM_SIZE) {
		kfree(buf);
		return -EINVAL;
	}

	/* devres manages the calibration values release on shutdown */
	ah->nvmem_blob = devm_kmemdup(sc->dev, buf, len, GFP_KERNEL);
	kfree(buf);
	if (!ah->nvmem_blob)
		return -ENOMEM;

	ah->nvmem_blob_len = len;
	ah->ah_flags &= ~AH_USE_EEPROM;
	ah->ah_flags |= AH_NO_EEP_SWAP;

	return 0;
}

static int ath9k_init_platform(struct ath_softc *sc)
{
	struct ath9k_platform_data *pdata = sc->dev->platform_data;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	int ret;

	if (!pdata)
		return 0;

	if (!pdata->use_eeprom) {
		ah->ah_flags &= ~AH_USE_EEPROM;
		ah->gpio_mask = pdata->gpio_mask;
		ah->gpio_val = pdata->gpio_val;
		ah->led_pin = pdata->led_pin;
		ah->is_clk_25mhz = pdata->is_clk_25mhz;
		ah->get_mac_revision = pdata->get_mac_revision;
		ah->external_reset = pdata->external_reset;
		ah->disable_2ghz = pdata->disable_2ghz;
		ah->disable_5ghz = pdata->disable_5ghz;

		if (!pdata->endian_check)
			ah->ah_flags |= AH_NO_EEP_SWAP;
	}

	if (pdata->eeprom_name) {
		ret = ath9k_eeprom_request(sc, pdata->eeprom_name);
		if (ret)
			return ret;
	}

	if (pdata->led_active_high)
		ah->config.led_active_high = true;

	if (pdata->tx_gain_buffalo)
		ah->config.tx_gain_buffalo = true;

	if (pdata->macaddr)
		ether_addr_copy(common->macaddr, pdata->macaddr);

	return 0;
}

static int ath9k_of_init(struct ath_softc *sc)
{
	struct device_node *np = sc->dev->of_node;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	enum ath_bus_type bus_type = common->bus_ops->ath_bus_type;
	char eeprom_name[100];
	int ret;

	if (!of_device_is_available(np))
		return 0;

	ath_dbg(common, CONFIG, "parsing configuration from OF node\n");

	if (of_property_read_bool(np, "qca,no-eeprom")) {
		/* ath9k-eeprom-<bus>-<id>.bin */
		scnprintf(eeprom_name, sizeof(eeprom_name),
			  "ath9k-eeprom-%s-%s.bin",
			  ath_bus_type_to_string(bus_type), dev_name(ah->dev));

		ret = ath9k_eeprom_request(sc, eeprom_name);
		if (ret)
			return ret;

		ah->ah_flags &= ~AH_USE_EEPROM;
		ah->ah_flags |= AH_NO_EEP_SWAP;
	}

	of_get_mac_address(np, common->macaddr);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ath9k_init_softc(u16 devid, struct ath_softc *sc,
			    const struct ath_bus_ops *bus_ops)
{
<<<<<<< HEAD
	struct ath9k_platform_data *pdata = sc->dev->platform_data;
	struct ath_hw *ah = NULL;
=======
	struct ath_hw *ah = NULL;
	struct ath9k_hw_capabilities *pCap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ath_common *common;
	int ret = 0, i;
	int csz = 0;

<<<<<<< HEAD
	ah = kzalloc(sizeof(struct ath_hw), GFP_KERNEL);
	if (!ah)
		return -ENOMEM;

	ah->hw = sc->hw;
	ah->hw_version.devid = devid;
	ah->reg_ops.read = ath9k_ioread32;
	ah->reg_ops.write = ath9k_iowrite32;
	ah->reg_ops.rmw = ath9k_reg_rmw;
	atomic_set(&ah->intr_ref_cnt, -1);
	sc->sc_ah = ah;

	if (!pdata) {
		ah->ah_flags |= AH_USE_EEPROM;
		sc->sc_ah->led_pin = -1;
	} else {
		sc->sc_ah->gpio_mask = pdata->gpio_mask;
		sc->sc_ah->gpio_val = pdata->gpio_val;
		sc->sc_ah->led_pin = pdata->led_pin;
		ah->is_clk_25mhz = pdata->is_clk_25mhz;
		ah->get_mac_revision = pdata->get_mac_revision;
		ah->external_reset = pdata->external_reset;
	}

	common = ath9k_hw_common(ah);
	common->ops = &ah->reg_ops;
	common->bus_ops = bus_ops;
=======
	ah = devm_kzalloc(sc->dev, sizeof(struct ath_hw), GFP_KERNEL);
	if (!ah)
		return -ENOMEM;

	ah->dev = sc->dev;
	ah->hw = sc->hw;
	ah->hw_version.devid = devid;
	ah->ah_flags |= AH_USE_EEPROM;
	ah->led_pin = -1;
	ah->reg_ops.read = ath9k_ioread32;
	ah->reg_ops.multi_read = ath9k_multi_ioread32;
	ah->reg_ops.write = ath9k_iowrite32;
	ah->reg_ops.rmw = ath9k_reg_rmw;
	pCap = &ah->caps;

	common = ath9k_hw_common(ah);

	/* Will be cleared in ath9k_start() */
	set_bit(ATH_OP_INVALID, &common->op_flags);

	sc->sc_ah = ah;
	sc->dfs_detector = dfs_pattern_detector_init(common, NL80211_DFS_UNSET);
	sc->tx99_power = MAX_RATE_POWER + 1;
	init_waitqueue_head(&sc->tx_wait);
	sc->cur_chan = &sc->chanctx[0];
	if (!ath9k_is_chanctx_enabled())
		sc->cur_chan->hw_queue_base = 0;

	common->ops = &ah->reg_ops;
	common->bus_ops = bus_ops;
	common->ps_ops = &ath9k_ps_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	common->ah = ah;
	common->hw = sc->hw;
	common->priv = sc;
	common->debug_mask = ath9k_debug;
	common->btcoex_enabled = ath9k_btcoex_enable == 1;
	common->disable_ani = false;
<<<<<<< HEAD
	spin_lock_init(&common->cc_lock);

	spin_lock_init(&sc->sc_serial_rw);
	spin_lock_init(&sc->sc_pm_lock);
	mutex_init(&sc->mutex);
#ifdef CONFIG_ATH9K_DEBUGFS
	spin_lock_init(&sc->nodes_lock);
	INIT_LIST_HEAD(&sc->nodes);
#endif
#ifdef CONFIG_ATH9K_MAC_DEBUG
	spin_lock_init(&sc->debug.samp_lock);
#endif
	tasklet_init(&sc->intr_tq, ath9k_tasklet, (unsigned long)sc);
	tasklet_init(&sc->bcon_tasklet, ath_beacon_tasklet,
		     (unsigned long)sc);
=======

	/*
	 * Platform quirks.
	 */
	ath9k_init_pcoem_platform(sc);

	ret = ath9k_init_platform(sc);
	if (ret)
		return ret;

	ret = ath9k_of_init(sc);
	if (ret)
		return ret;

	ret = ath9k_nvmem_request_eeprom(sc);
	if (ret)
		return ret;

	if (ath9k_led_active_high != -1)
		ah->config.led_active_high = ath9k_led_active_high == 1;

	/*
	 * Enable WLAN/BT RX Antenna diversity only when:
	 *
	 * - BTCOEX is disabled.
	 * - the user manually requests the feature.
	 * - the HW cap is set using the platform data.
	 */
	if (!common->btcoex_enabled && ath9k_bt_ant_diversity &&
	    (pCap->hw_caps & ATH9K_HW_CAP_BT_ANT_DIV))
		common->bt_ant_diversity = 1;

	spin_lock_init(&common->cc_lock);
	spin_lock_init(&sc->intr_lock);
	spin_lock_init(&sc->sc_serial_rw);
	spin_lock_init(&sc->sc_pm_lock);
	spin_lock_init(&sc->chan_lock);
	mutex_init(&sc->mutex);
	tasklet_setup(&sc->intr_tq, ath9k_tasklet);
	tasklet_setup(&sc->bcon_tasklet, ath9k_beacon_tasklet);

	timer_setup(&sc->sleep_timer, ath_ps_full_sleep, 0);
	INIT_WORK(&sc->hw_reset_work, ath_reset_work);
	INIT_WORK(&sc->paprd_work, ath_paprd_calibrate);
	INIT_DELAYED_WORK(&sc->hw_pll_work, ath_hw_pll_work);
	INIT_DELAYED_WORK(&sc->hw_check_work, ath_hw_check_work);

	ath9k_init_channel_context(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Cache line size is used to size and align various
	 * structures used to communicate with the hardware.
	 */
	ath_read_cachesize(common, &csz);
	common->cachelsz = csz << 2; /* convert to bytes */

	/* Initializes the hardware for all supported chipsets */
	ret = ath9k_hw_init(ah);
	if (ret)
		goto err_hw;

<<<<<<< HEAD
	if (pdata && pdata->macaddr)
		memcpy(common->macaddr, pdata->macaddr, ETH_ALEN);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = ath9k_init_queues(sc);
	if (ret)
		goto err_queues;

	ret =  ath9k_init_btcoex(sc);
	if (ret)
		goto err_btcoex;

<<<<<<< HEAD
	ret = ath9k_init_channels_rates(sc);
=======
	ret = ath9k_cmn_init_channels_rates(common);
	if (ret)
		goto err_btcoex;

	ret = ath9k_init_p2p(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto err_btcoex;

	ath9k_cmn_init_crypto(sc->sc_ah);
	ath9k_init_misc(sc);
<<<<<<< HEAD
=======
	ath_chanctx_init(sc);
	ath9k_offchannel_init(sc);

	if (common->bus_ops->aspm_init)
		common->bus_ops->aspm_init(common);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

err_btcoex:
	for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++)
		if (ATH_TXQ_SETUP(sc, i))
			ath_tx_cleanupq(sc, &sc->tx.txq[i]);
err_queues:
	ath9k_hw_deinit(ah);
err_hw:
<<<<<<< HEAD

	kfree(ah);
	sc->sc_ah = NULL;

=======
	ath9k_eeprom_release(sc);
	dev_kfree_skb_any(sc->tx99_skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void ath9k_init_band_txpower(struct ath_softc *sc, int band)
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *chan;
	struct ath_hw *ah = sc->sc_ah;
<<<<<<< HEAD
	int i;

	sband = &sc->sbands[band];
	for (i = 0; i < sband->n_channels; i++) {
		chan = &sband->channels[i];
		ah->curchan = &ah->channels[chan->hw_value];
		ath9k_cmn_update_ichannel(ah->curchan, chan, NL80211_CHAN_HT20);
		ath9k_hw_set_txpowerlimit(ah, MAX_RATE_POWER, true);
=======
	struct ath_common *common = ath9k_hw_common(ah);
	struct cfg80211_chan_def chandef;
	int i;

	sband = &common->sbands[band];
	for (i = 0; i < sband->n_channels; i++) {
		chan = &sband->channels[i];
		ah->curchan = &ah->channels[chan->hw_value];
		cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_HT20);
		ath9k_cmn_get_channel(sc->hw, ah, &chandef);
		ath9k_hw_set_txpowerlimit(ah, MAX_COMBINED_POWER, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void ath9k_init_txpower_limits(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_channel *curchan = ah->curchan;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ)
<<<<<<< HEAD
		ath9k_init_band_txpower(sc, IEEE80211_BAND_2GHZ);
	if (ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		ath9k_init_band_txpower(sc, IEEE80211_BAND_5GHZ);
=======
		ath9k_init_band_txpower(sc, NL80211_BAND_2GHZ);
	if (ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		ath9k_init_band_txpower(sc, NL80211_BAND_5GHZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ah->curchan = curchan;
}

<<<<<<< HEAD
void ath9k_reload_chainmask_settings(struct ath_softc *sc)
{
	if (!(sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_HT))
		return;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ)
		setup_ht_cap(sc, &sc->sbands[IEEE80211_BAND_2GHZ].ht_cap);
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		setup_ht_cap(sc, &sc->sbands[IEEE80211_BAND_5GHZ].ht_cap);
}


void ath9k_set_hw_capab(struct ath_softc *sc, struct ieee80211_hw *hw)
=======
static const struct ieee80211_iface_limit if_limits[] = {
	{ .max = 2048,	.types = BIT(NL80211_IFTYPE_STATION) },
	{ .max = 8,	.types =
#ifdef CONFIG_MAC80211_MESH
				 BIT(NL80211_IFTYPE_MESH_POINT) |
#endif
				 BIT(NL80211_IFTYPE_AP) },
	{ .max = 1,	.types = BIT(NL80211_IFTYPE_P2P_CLIENT) |
				 BIT(NL80211_IFTYPE_P2P_GO) },
};

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT

static const struct ieee80211_iface_limit if_limits_multi[] = {
	{ .max = 2,	.types = BIT(NL80211_IFTYPE_STATION) |
				 BIT(NL80211_IFTYPE_AP) |
				 BIT(NL80211_IFTYPE_P2P_CLIENT) |
				 BIT(NL80211_IFTYPE_P2P_GO) },
	{ .max = 1,	.types = BIT(NL80211_IFTYPE_ADHOC) },
	{ .max = 1,	.types = BIT(NL80211_IFTYPE_P2P_DEVICE) },
};

static const struct ieee80211_iface_combination if_comb_multi[] = {
	{
		.limits = if_limits_multi,
		.n_limits = ARRAY_SIZE(if_limits_multi),
		.max_interfaces = 3,
		.num_different_channels = 2,
		.beacon_int_infra_match = true,
	},
};

#endif /* CONFIG_ATH9K_CHANNEL_CONTEXT */

static const struct ieee80211_iface_combination if_comb[] = {
	{
		.limits = if_limits,
		.n_limits = ARRAY_SIZE(if_limits),
		.max_interfaces = 2048,
		.num_different_channels = 1,
		.beacon_int_infra_match = true,
#ifdef CONFIG_ATH9K_DFS_CERTIFIED
		.radar_detect_widths =	BIT(NL80211_CHAN_WIDTH_20_NOHT) |
					BIT(NL80211_CHAN_WIDTH_20) |
					BIT(NL80211_CHAN_WIDTH_40),
#endif
	},
};

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
static void ath9k_set_mcc_capab(struct ath_softc *sc, struct ieee80211_hw *hw)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

<<<<<<< HEAD
	hw->flags = IEEE80211_HW_RX_INCLUDES_FCS |
		IEEE80211_HW_HOST_BROADCAST_PS_BUFFERING |
		IEEE80211_HW_SIGNAL_DBM |
		IEEE80211_HW_SUPPORTS_PS |
		IEEE80211_HW_PS_NULLFUNC_STACK |
		IEEE80211_HW_SPECTRUM_MGMT |
		IEEE80211_HW_REPORTS_TX_ACK_STATUS;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_HT)
		 hw->flags |= IEEE80211_HW_AMPDU_AGGREGATION;

	if (AR_SREV_9160_10_OR_LATER(sc->sc_ah) || ath9k_modparam_nohwcrypt)
		hw->flags |= IEEE80211_HW_MFP_CAPABLE;

	hw->wiphy->interface_modes =
		BIT(NL80211_IFTYPE_P2P_GO) |
		BIT(NL80211_IFTYPE_P2P_CLIENT) |
		BIT(NL80211_IFTYPE_AP) |
		BIT(NL80211_IFTYPE_WDS) |
		BIT(NL80211_IFTYPE_STATION) |
		BIT(NL80211_IFTYPE_ADHOC) |
		BIT(NL80211_IFTYPE_MESH_POINT);
=======
	if (!ath9k_is_chanctx_enabled())
		return;

	ieee80211_hw_set(hw, QUEUE_CONTROL);
	hw->queues = ATH9K_NUM_TX_QUEUES;
	hw->offchannel_tx_hw_queue = hw->queues - 1;
	hw->wiphy->iface_combinations = if_comb_multi;
	hw->wiphy->n_iface_combinations = ARRAY_SIZE(if_comb_multi);
	hw->wiphy->max_scan_ssids = 255;
	hw->wiphy->max_scan_ie_len = IEEE80211_MAX_DATA_LEN;
	hw->wiphy->max_remain_on_channel_duration = 10000;
	hw->chanctx_data_size = sizeof(void *);
	hw->extra_beacon_tailroom =
		sizeof(struct ieee80211_p2p_noa_attr) + 9;

	ath_dbg(common, CHAN_CTX, "Use channel contexts\n");
}
#endif /* CONFIG_ATH9K_CHANNEL_CONTEXT */

static void ath9k_set_hw_capab(struct ath_softc *sc, struct ieee80211_hw *hw)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);

	ieee80211_hw_set(hw, SUPPORTS_HT_CCK_RATES);
	ieee80211_hw_set(hw, SUPPORTS_RC_TABLE);
	ieee80211_hw_set(hw, REPORTS_TX_ACK_STATUS);
	ieee80211_hw_set(hw, SPECTRUM_MGMT);
	ieee80211_hw_set(hw, PS_NULLFUNC_STACK);
	ieee80211_hw_set(hw, SIGNAL_DBM);
	ieee80211_hw_set(hw, RX_INCLUDES_FCS);
	ieee80211_hw_set(hw, HOST_BROADCAST_PS_BUFFERING);
	ieee80211_hw_set(hw, SUPPORT_FAST_XMIT);
	ieee80211_hw_set(hw, SUPPORTS_CLONED_SKBS);

	if (ath9k_ps_enable)
		ieee80211_hw_set(hw, SUPPORTS_PS);

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_HT) {
		ieee80211_hw_set(hw, AMPDU_AGGREGATION);

		if (AR_SREV_9280_20_OR_LATER(ah))
			hw->radiotap_mcs_details |=
				IEEE80211_RADIOTAP_MCS_HAVE_STBC;
	}

	if (AR_SREV_9160_10_OR_LATER(sc->sc_ah) || ath9k_modparam_nohwcrypt)
		ieee80211_hw_set(hw, MFP_CAPABLE);

	hw->wiphy->features |= NL80211_FEATURE_ACTIVE_MONITOR |
			       NL80211_FEATURE_AP_MODE_CHAN_WIDTH_CHANGE |
			       NL80211_FEATURE_P2P_GO_CTWIN;

	if (!IS_ENABLED(CONFIG_ATH9K_TX99)) {
		hw->wiphy->interface_modes =
			BIT(NL80211_IFTYPE_P2P_GO) |
			BIT(NL80211_IFTYPE_P2P_CLIENT) |
			BIT(NL80211_IFTYPE_AP) |
			BIT(NL80211_IFTYPE_STATION) |
			BIT(NL80211_IFTYPE_ADHOC) |
			BIT(NL80211_IFTYPE_MESH_POINT) |
			BIT(NL80211_IFTYPE_OCB);

		if (ath9k_is_chanctx_enabled())
			hw->wiphy->interface_modes |=
					BIT(NL80211_IFTYPE_P2P_DEVICE);

		hw->wiphy->iface_combinations = if_comb;
		hw->wiphy->n_iface_combinations = ARRAY_SIZE(if_comb);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hw->wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;

	hw->wiphy->flags |= WIPHY_FLAG_IBSS_RSN;
	hw->wiphy->flags |= WIPHY_FLAG_SUPPORTS_TDLS;
<<<<<<< HEAD

	hw->queues = 4;
	hw->max_rates = 4;
	hw->channel_change_time = 5000;
	hw->max_listen_interval = 1;
	hw->max_rate_tries = 10;
	hw->sta_data_size = sizeof(struct ath_node);
	hw->vif_data_size = sizeof(struct ath_vif);
=======
	hw->wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL;
	hw->wiphy->flags |= WIPHY_FLAG_SUPPORTS_5_10_MHZ;
	hw->wiphy->flags |= WIPHY_FLAG_HAS_CHANNEL_SWITCH;
	hw->wiphy->flags |= WIPHY_FLAG_AP_UAPSD;

	hw->queues = 4;
	hw->max_rates = 4;
	hw->max_listen_interval = 10;
	hw->max_rate_tries = 10;
	hw->sta_data_size = sizeof(struct ath_node);
	hw->vif_data_size = sizeof(struct ath_vif);
	hw->txq_data_size = sizeof(struct ath_atx_tid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hw->extra_tx_headroom = 4;

	hw->wiphy->available_antennas_rx = BIT(ah->caps.max_rxchains) - 1;
	hw->wiphy->available_antennas_tx = BIT(ah->caps.max_txchains) - 1;

	/* single chain devices with rx diversity */
	if (ah->caps.hw_caps & ATH9K_HW_CAP_ANT_DIV_COMB)
		hw->wiphy->available_antennas_rx = BIT(0) | BIT(1);

	sc->ant_rx = hw->wiphy->available_antennas_rx;
	sc->ant_tx = hw->wiphy->available_antennas_tx;

	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ)
<<<<<<< HEAD
		hw->wiphy->bands[IEEE80211_BAND_2GHZ] =
			&sc->sbands[IEEE80211_BAND_2GHZ];
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		hw->wiphy->bands[IEEE80211_BAND_5GHZ] =
			&sc->sbands[IEEE80211_BAND_5GHZ];

	ath9k_reload_chainmask_settings(sc);

	SET_IEEE80211_PERM_ADDR(hw, common->macaddr);
=======
		hw->wiphy->bands[NL80211_BAND_2GHZ] =
			&common->sbands[NL80211_BAND_2GHZ];
	if (sc->sc_ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		hw->wiphy->bands[NL80211_BAND_5GHZ] =
			&common->sbands[NL80211_BAND_5GHZ];

#ifdef CONFIG_ATH9K_CHANNEL_CONTEXT
	ath9k_set_mcc_capab(sc, hw);
#endif
	ath9k_init_wow(hw);
	ath9k_cmn_reload_chainmask(ah);

	SET_IEEE80211_PERM_ADDR(hw, common->macaddr);

	wiphy_ext_feature_set(hw->wiphy, NL80211_EXT_FEATURE_CQM_RSSI_LIST);
	wiphy_ext_feature_set(hw->wiphy, NL80211_EXT_FEATURE_AIRTIME_FAIRNESS);
	wiphy_ext_feature_set(hw->wiphy,
			      NL80211_EXT_FEATURE_MULTICAST_REGISTRATIONS);
	wiphy_ext_feature_set(hw->wiphy, NL80211_EXT_FEATURE_CAN_REPLACE_PTK0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int ath9k_init_device(u16 devid, struct ath_softc *sc,
		    const struct ath_bus_ops *bus_ops)
{
	struct ieee80211_hw *hw = sc->hw;
	struct ath_common *common;
	struct ath_hw *ah;
	int error = 0;
	struct ath_regulatory *reg;

	/* Bring up device */
	error = ath9k_init_softc(devid, sc, bus_ops);
<<<<<<< HEAD
	if (error != 0)
		goto error_init;
=======
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ah = sc->sc_ah;
	common = ath9k_hw_common(ah);
	ath9k_set_hw_capab(sc, hw);

	/* Initialize regulatory */
	error = ath_regd_init(&common->regulatory, sc->hw->wiphy,
			      ath9k_reg_notifier);
	if (error)
<<<<<<< HEAD
		goto error_regd;
=======
		goto deinit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg = &common->regulatory;

	/* Setup TX DMA */
	error = ath_tx_init(sc, ATH_TXBUF);
	if (error != 0)
<<<<<<< HEAD
		goto error_tx;
=======
		goto deinit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Setup RX DMA */
	error = ath_rx_init(sc, ATH_RXBUF);
	if (error != 0)
<<<<<<< HEAD
		goto error_rx;
=======
		goto deinit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ath9k_init_txpower_limits(sc);

#ifdef CONFIG_MAC80211_LEDS
	/* must be initialized before ieee80211_register_hw */
	sc->led_cdev.default_trigger = ieee80211_create_tpt_led_trigger(sc->hw,
		IEEE80211_TPT_LEDTRIG_FL_RADIO, ath9k_tpt_blink,
		ARRAY_SIZE(ath9k_tpt_blink));
#endif

<<<<<<< HEAD
	INIT_WORK(&sc->hw_reset_work, ath_reset_work);
	INIT_WORK(&sc->hw_check_work, ath_hw_check);
	INIT_WORK(&sc->paprd_work, ath_paprd_calibrate);
	INIT_DELAYED_WORK(&sc->hw_pll_work, ath_hw_pll_work);
=======
	wiphy_read_of_freq_limits(hw->wiphy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Register with mac80211 */
	error = ieee80211_register_hw(hw);
	if (error)
<<<<<<< HEAD
		goto error_register;
=======
		goto rx_cleanup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = ath9k_init_debug(ah);
	if (error) {
		ath_err(common, "Unable to create debugfs files\n");
<<<<<<< HEAD
		goto error_world;
=======
		goto unregister;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Handle world regulatory */
	if (!ath_is_world_regd(reg)) {
		error = regulatory_hint(hw->wiphy, reg->alpha2);
		if (error)
<<<<<<< HEAD
			goto error_world;
	}

	sc->last_rssi = ATH_RSSI_DUMMY_MARKER;

=======
			goto debug_cleanup;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath_init_leds(sc);
	ath_start_rfkill_poll(sc);

	return 0;

<<<<<<< HEAD
error_world:
	ieee80211_unregister_hw(hw);
error_register:
	ath_rx_cleanup(sc);
error_rx:
	ath_tx_cleanup(sc);
error_tx:
	/* Nothing */
error_regd:
	ath9k_deinit_softc(sc);
error_init:
=======
debug_cleanup:
	ath9k_deinit_debug(sc);
unregister:
	ieee80211_unregister_hw(hw);
rx_cleanup:
	ath_rx_cleanup(sc);
deinit:
	ath9k_deinit_softc(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*****************************/
/*     De-Initialization     */
/*****************************/

static void ath9k_deinit_softc(struct ath_softc *sc)
{
	int i = 0;

<<<<<<< HEAD
	if (sc->sbands[IEEE80211_BAND_2GHZ].channels)
		kfree(sc->sbands[IEEE80211_BAND_2GHZ].channels);

	if (sc->sbands[IEEE80211_BAND_5GHZ].channels)
		kfree(sc->sbands[IEEE80211_BAND_5GHZ].channels);

=======
	ath9k_deinit_p2p(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ath9k_deinit_btcoex(sc);

	for (i = 0; i < ATH9K_NUM_TX_QUEUES; i++)
		if (ATH_TXQ_SETUP(sc, i))
			ath_tx_cleanupq(sc, &sc->tx.txq[i]);

<<<<<<< HEAD
	ath9k_hw_deinit(sc->sc_ah);

	kfree(sc->sc_ah);
	sc->sc_ah = NULL;
=======
	del_timer_sync(&sc->sleep_timer);
	ath9k_hw_deinit(sc->sc_ah);
	if (sc->dfs_detector != NULL)
		sc->dfs_detector->exit(sc->dfs_detector);

	ath9k_eeprom_release(sc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ath9k_deinit_device(struct ath_softc *sc)
{
	struct ieee80211_hw *hw = sc->hw;

	ath9k_ps_wakeup(sc);

	wiphy_rfkill_stop_polling(sc->hw->wiphy);
	ath_deinit_leds(sc);

	ath9k_ps_restore(sc);

<<<<<<< HEAD
	ieee80211_unregister_hw(hw);
	ath_rx_cleanup(sc);
	ath_tx_cleanup(sc);
	ath9k_deinit_softc(sc);
}

void ath_descdma_cleanup(struct ath_softc *sc,
			 struct ath_descdma *dd,
			 struct list_head *head)
{
	dma_free_coherent(sc->dev, dd->dd_desc_len, dd->dd_desc,
			  dd->dd_desc_paddr);

	INIT_LIST_HEAD(head);
	kfree(dd->dd_bufptr);
	memset(dd, 0, sizeof(*dd));
}

=======
	ath9k_deinit_debug(sc);
	ath9k_deinit_wow(hw);
	ieee80211_unregister_hw(hw);
	ath_rx_cleanup(sc);
	ath9k_deinit_softc(sc);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/************************/
/*     Module Hooks     */
/************************/

static int __init ath9k_init(void)
{
	int error;

<<<<<<< HEAD
	/* Register rate control algorithm */
	error = ath_rate_control_register();
	if (error != 0) {
		printk(KERN_ERR
			"ath9k: Unable to register rate control "
			"algorithm: %d\n",
			error);
		goto err_out;
	}

	error = ath_pci_init();
	if (error < 0) {
		printk(KERN_ERR
			"ath9k: No PCI devices found, driver not installed.\n");
		error = -ENODEV;
		goto err_rate_unregister;
=======
	error = ath_pci_init();
	if (error < 0) {
		pr_err("No PCI devices found, driver not installed\n");
		error = -ENODEV;
		goto err_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = ath_ahb_init();
	if (error < 0) {
		error = -ENODEV;
		goto err_pci_exit;
	}

<<<<<<< HEAD
=======
	dmi_check_system(ath9k_quirks);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

 err_pci_exit:
	ath_pci_exit();
<<<<<<< HEAD

 err_rate_unregister:
	ath_rate_control_unregister();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 err_out:
	return error;
}
module_init(ath9k_init);

static void __exit ath9k_exit(void)
{
	is_ath9k_unloaded = true;
	ath_ahb_exit();
	ath_pci_exit();
<<<<<<< HEAD
	ath_rate_control_unregister();
	printk(KERN_INFO "%s: Driver unloaded\n", dev_info);
=======
	pr_info("%s: Driver unloaded\n", dev_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
module_exit(ath9k_exit);
