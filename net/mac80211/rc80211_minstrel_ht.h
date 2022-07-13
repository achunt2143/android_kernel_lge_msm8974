<<<<<<< HEAD
/*
 * Copyright (C) 2010 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2010 Felix Fietkau <nbd@openwrt.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __RC_MINSTREL_HT_H
#define __RC_MINSTREL_HT_H

<<<<<<< HEAD
=======
#include <linux/bitfield.h>

/* number of highest throughput rates to consider*/
#define MAX_THR_RATES 4
#define SAMPLE_COLUMNS	10	/* number of columns in sample table */

/* scaled fraction values */
#define MINSTREL_SCALE  12
#define MINSTREL_FRAC(val, div) (((val) << MINSTREL_SCALE) / div)
#define MINSTREL_TRUNC(val) ((val) >> MINSTREL_SCALE)

#define EWMA_LEVEL	96	/* ewma weighting factor [/EWMA_DIV] */
#define EWMA_DIV	128

/*
 * Coefficients for moving average with noise filter (period=16),
 * scaled by 10 bits
 *
 * a1 = exp(-pi * sqrt(2) / period)
 * coeff2 = 2 * a1 * cos(sqrt(2) * 2 * pi / period)
 * coeff3 = -sqr(a1)
 * coeff1 = 1 - coeff2 - coeff3
 */
#define MINSTREL_AVG_COEFF1		(MINSTREL_FRAC(1, 1) - \
					 MINSTREL_AVG_COEFF2 - \
					 MINSTREL_AVG_COEFF3)
#define MINSTREL_AVG_COEFF2		0x00001499
#define MINSTREL_AVG_COEFF3		-0x0000092e

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The number of streams can be changed to 2 to reduce code
 * size and memory footprint.
 */
<<<<<<< HEAD
#define MINSTREL_MAX_STREAMS	3
#define MINSTREL_STREAM_GROUPS	4

/* scaled fraction values */
#define MINSTREL_SCALE	16
#define MINSTREL_FRAC(val, div) (((val) << MINSTREL_SCALE) / div)
#define MINSTREL_TRUNC(val) ((val) >> MINSTREL_SCALE)

#define MCS_GROUP_RATES	8

struct mcs_group {
	u32 flags;
	unsigned int streams;
	unsigned int duration[MCS_GROUP_RATES];
};

=======
#define MINSTREL_MAX_STREAMS		4
#define MINSTREL_HT_STREAM_GROUPS	4 /* BW(=2) * SGI(=2) */
#define MINSTREL_VHT_STREAM_GROUPS	6 /* BW(=3) * SGI(=2) */

#define MINSTREL_HT_GROUPS_NB	(MINSTREL_MAX_STREAMS *		\
				 MINSTREL_HT_STREAM_GROUPS)
#define MINSTREL_VHT_GROUPS_NB	(MINSTREL_MAX_STREAMS *		\
				 MINSTREL_VHT_STREAM_GROUPS)
#define MINSTREL_LEGACY_GROUPS_NB	2
#define MINSTREL_GROUPS_NB	(MINSTREL_HT_GROUPS_NB +	\
				 MINSTREL_VHT_GROUPS_NB +	\
				 MINSTREL_LEGACY_GROUPS_NB)

#define MINSTREL_HT_GROUP_0	0
#define MINSTREL_CCK_GROUP	(MINSTREL_HT_GROUP_0 + MINSTREL_HT_GROUPS_NB)
#define MINSTREL_OFDM_GROUP	(MINSTREL_CCK_GROUP + 1)
#define MINSTREL_VHT_GROUP_0	(MINSTREL_OFDM_GROUP + 1)

#define MCS_GROUP_RATES		10

#define MI_RATE_IDX_MASK	GENMASK(3, 0)
#define MI_RATE_GROUP_MASK	GENMASK(15, 4)

#define MI_RATE(_group, _idx)				\
	(FIELD_PREP(MI_RATE_GROUP_MASK, _group) |	\
	 FIELD_PREP(MI_RATE_IDX_MASK, _idx))

#define MI_RATE_IDX(_rate) FIELD_GET(MI_RATE_IDX_MASK, _rate)
#define MI_RATE_GROUP(_rate) FIELD_GET(MI_RATE_GROUP_MASK, _rate)

#define MINSTREL_SAMPLE_RATES		5 /* rates per sample type */
#define MINSTREL_SAMPLE_INTERVAL	(HZ / 50)

struct minstrel_priv {
	struct ieee80211_hw *hw;
	unsigned int cw_min;
	unsigned int cw_max;
	unsigned int max_retry;
	unsigned int segment_size;
	unsigned int update_interval;

	u8 cck_rates[4];
	u8 ofdm_rates[NUM_NL80211_BANDS][8];

#ifdef CONFIG_MAC80211_DEBUGFS
	/*
	 * enable fixed rate processing per RC
	 *   - write static index to debugfs:ieee80211/phyX/rc/fixed_rate_idx
	 *   - write -1 to enable RC processing again
	 *   - setting will be applied on next update
	 */
	u32 fixed_rate_idx;
#endif
};


struct mcs_group {
	u16 flags;
	u8 streams;
	u8 shift;
	u8 bw;
	u16 duration[MCS_GROUP_RATES];
};

extern const s16 minstrel_cck_bitrates[4];
extern const s16 minstrel_ofdm_bitrates[8];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern const struct mcs_group minstrel_mcs_groups[];

struct minstrel_rate_stats {
	/* current / last sampling period attempts/success counters */
<<<<<<< HEAD
	unsigned int attempts, last_attempts;
	unsigned int success, last_success;

	/* total attempts/success counters */
	u64 att_hist, succ_hist;

	/* current throughput */
	unsigned int cur_tp;

	/* packet delivery probabilities */
	unsigned int cur_prob, probability;

	/* maximum retry counts */
	unsigned int retry_count;
	unsigned int retry_count_rtscts;

	bool retry_updated;
	u8 sample_skipped;
=======
	u16 attempts, last_attempts;
	u16 success, last_success;

	/* total attempts/success counters */
	u32 att_hist, succ_hist;

	/* prob_avg - moving average of prob */
	u16 prob_avg;
	u16 prob_avg_1;

	/* maximum retry counts */
	u8 retry_count;
	u8 retry_count_rtscts;

	bool retry_updated;
};

enum minstrel_sample_type {
	MINSTREL_SAMPLE_TYPE_INC,
	MINSTREL_SAMPLE_TYPE_JUMP,
	MINSTREL_SAMPLE_TYPE_SLOW,
	__MINSTREL_SAMPLE_TYPE_MAX
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct minstrel_mcs_group_data {
	u8 index;
	u8 column;

<<<<<<< HEAD
	/* bitfield of supported MCS rates of this group */
	u8 supported;

	/* selected primary rates */
	unsigned int max_tp_rate;
	unsigned int max_tp_rate2;
	unsigned int max_prob_rate;
=======
	/* sorted rate set within a MCS group*/
	u16 max_group_tp_rate[MAX_THR_RATES];
	u16 max_group_prob_rate;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* MCS rate statistics */
	struct minstrel_rate_stats rates[MCS_GROUP_RATES];
};

<<<<<<< HEAD
struct minstrel_ht_sta {
=======
struct minstrel_sample_category {
	u8 sample_group;
	u16 sample_rates[MINSTREL_SAMPLE_RATES];
	u16 cur_sample_rates[MINSTREL_SAMPLE_RATES];
};

struct minstrel_ht_sta {
	struct ieee80211_sta *sta;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* ampdu length (average, per sampling interval) */
	unsigned int ampdu_len;
	unsigned int ampdu_packets;

	/* ampdu length (EWMA) */
	unsigned int avg_ampdu_len;

<<<<<<< HEAD
	/* best throughput rate */
	unsigned int max_tp_rate;

	/* second best throughput rate */
	unsigned int max_tp_rate2;

	/* best probability rate */
	unsigned int max_prob_rate;

	/* time of last status update */
	unsigned long stats_update;
=======
	/* overall sorted rate set */
	u16 max_tp_rate[MAX_THR_RATES];
	u16 max_prob_rate;

	/* time of last status update */
	unsigned long last_stats_update;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* overhead time in usec for each frame */
	unsigned int overhead;
	unsigned int overhead_rtscts;
<<<<<<< HEAD
=======
	unsigned int overhead_legacy;
	unsigned int overhead_legacy_rtscts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unsigned int total_packets;
	unsigned int sample_packets;

	/* tx flags to add for frames for this sta */
	u32 tx_flags;
<<<<<<< HEAD

	u8 sample_wait;
	u8 sample_tries;
	u8 sample_count;
	u8 sample_slow;

	/* current MCS group to be sampled */
	u8 sample_group;

	/* MCS rate group info and statistics */
	struct minstrel_mcs_group_data groups[MINSTREL_MAX_STREAMS * MINSTREL_STREAM_GROUPS];
};

struct minstrel_ht_sta_priv {
	union {
		struct minstrel_ht_sta ht;
		struct minstrel_sta_info legacy;
	};
#ifdef CONFIG_MAC80211_DEBUGFS
	struct dentry *dbg_stats;
#endif
	void *ratelist;
	void *sample_table;
	bool is_ht;
};

void minstrel_ht_add_sta_debugfs(void *priv, void *priv_sta, struct dentry *dir);
void minstrel_ht_remove_sta_debugfs(void *priv, void *priv_sta);
=======
	bool use_short_preamble;
	u8 band;

	u8 sample_seq;
	u16 sample_rate;

	unsigned long sample_time;
	struct minstrel_sample_category sample[__MINSTREL_SAMPLE_TYPE_MAX];

	/* Bitfield of supported MCS rates of all groups */
	u16 supported[MINSTREL_GROUPS_NB];

	/* MCS rate group info and statistics */
	struct minstrel_mcs_group_data groups[MINSTREL_GROUPS_NB];
};

void minstrel_ht_add_sta_debugfs(void *priv, void *priv_sta, struct dentry *dir);
int minstrel_ht_get_tp_avg(struct minstrel_ht_sta *mi, int group, int rate,
			   int prob_avg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
