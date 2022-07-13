/*
 * Copyright 2009 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs
 */

<<<<<<< HEAD
#include "drmP.h"

#include "nouveau_drv.h"
#include "nouveau_i2c.h"
#include "nouveau_connector.h"
#include "nouveau_encoder.h"
#include "nouveau_crtc.h"
#include "nouveau_gpio.h"

/******************************************************************************
 * aux channel util functions
 *****************************************************************************/
#define AUX_DBG(fmt, args...) do {                                             \
	if (nouveau_reg_debug & NOUVEAU_REG_DEBUG_AUXCH) {                     \
		NV_PRINTK(KERN_DEBUG, dev, "AUXCH(%d): " fmt, ch, ##args);     \
	}                                                                      \
} while (0)
#define AUX_ERR(fmt, args...) NV_ERROR(dev, "AUXCH(%d): " fmt, ch, ##args)

static void
auxch_fini(struct drm_device *dev, int ch)
{
	nv_mask(dev, 0x00e4e4 + (ch * 0x50), 0x00310000, 0x00000000);
}

static int
auxch_init(struct drm_device *dev, int ch)
{
	const u32 unksel = 1; /* nfi which to use, or if it matters.. */
	const u32 ureq = unksel ? 0x00100000 : 0x00200000;
	const u32 urep = unksel ? 0x01000000 : 0x02000000;
	u32 ctrl, timeout;

	/* wait up to 1ms for any previous transaction to be done... */
	timeout = 1000;
	do {
		ctrl = nv_rd32(dev, 0x00e4e4 + (ch * 0x50));
		udelay(1);
		if (!timeout--) {
			AUX_ERR("begin idle timeout 0x%08x", ctrl);
			return -EBUSY;
		}
	} while (ctrl & 0x03010000);

	/* set some magic, and wait up to 1ms for it to appear */
	nv_mask(dev, 0x00e4e4 + (ch * 0x50), 0x00300000, ureq);
	timeout = 1000;
	do {
		ctrl = nv_rd32(dev, 0x00e4e4 + (ch * 0x50));
		udelay(1);
		if (!timeout--) {
			AUX_ERR("magic wait 0x%08x\n", ctrl);
			auxch_fini(dev, ch);
			return -EBUSY;
		}
	} while ((ctrl & 0x03000000) != urep);

	return 0;
}

static int
auxch_tx(struct drm_device *dev, int ch, u8 type, u32 addr, u8 *data, u8 size)
{
	u32 ctrl, stat, timeout, retries;
	u32 xbuf[4] = {};
	int ret, i;

	AUX_DBG("%d: 0x%08x %d\n", type, addr, size);

	ret = auxch_init(dev, ch);
	if (ret)
		goto out;

	stat = nv_rd32(dev, 0x00e4e8 + (ch * 0x50));
	if (!(stat & 0x10000000)) {
		AUX_DBG("sink not detected\n");
		ret = -ENXIO;
		goto out;
	}

	if (!(type & 1)) {
		memcpy(xbuf, data, size);
		for (i = 0; i < 16; i += 4) {
			AUX_DBG("wr 0x%08x\n", xbuf[i / 4]);
			nv_wr32(dev, 0x00e4c0 + (ch * 0x50) + i, xbuf[i / 4]);
		}
	}

	ctrl  = nv_rd32(dev, 0x00e4e4 + (ch * 0x50));
	ctrl &= ~0x0001f0ff;
	ctrl |= type << 12;
	ctrl |= size - 1;
	nv_wr32(dev, 0x00e4e0 + (ch * 0x50), addr);

	/* retry transaction a number of times on failure... */
	ret = -EREMOTEIO;
	for (retries = 0; retries < 32; retries++) {
		/* reset, and delay a while if this is a retry */
		nv_wr32(dev, 0x00e4e4 + (ch * 0x50), 0x80000000 | ctrl);
		nv_wr32(dev, 0x00e4e4 + (ch * 0x50), 0x00000000 | ctrl);
		if (retries)
			udelay(400);

		/* transaction request, wait up to 1ms for it to complete */
		nv_wr32(dev, 0x00e4e4 + (ch * 0x50), 0x00010000 | ctrl);

		timeout = 1000;
		do {
			ctrl = nv_rd32(dev, 0x00e4e4 + (ch * 0x50));
			udelay(1);
			if (!timeout--) {
				AUX_ERR("tx req timeout 0x%08x\n", ctrl);
				goto out;
			}
		} while (ctrl & 0x00010000);

		/* read status, and check if transaction completed ok */
		stat = nv_mask(dev, 0x00e4e8 + (ch * 0x50), 0, 0);
		if (!(stat & 0x000f0f00)) {
			ret = 0;
			break;
		}

		AUX_DBG("%02d 0x%08x 0x%08x\n", retries, ctrl, stat);
	}

	if (type & 1) {
		for (i = 0; i < 16; i += 4) {
			xbuf[i / 4] = nv_rd32(dev, 0x00e4d0 + (ch * 0x50) + i);
			AUX_DBG("rd 0x%08x\n", xbuf[i / 4]);
		}
		memcpy(data, xbuf, size);
	}

out:
	auxch_fini(dev, ch);
	return ret;
}

u8 *
nouveau_dp_bios_data(struct drm_device *dev, struct dcb_entry *dcb, u8 **entry)
{
	struct bit_entry d;
	u8 *table;
	int i;

	if (bit_table(dev, 'd', &d)) {
		NV_ERROR(dev, "BIT 'd' table not found\n");
		return NULL;
	}

	if (d.version != 1) {
		NV_ERROR(dev, "BIT 'd' table version %d unknown\n", d.version);
		return NULL;
	}

	table = ROMPTR(dev, d.data[0]);
	if (!table) {
		NV_ERROR(dev, "displayport table pointer invalid\n");
		return NULL;
	}

	switch (table[0]) {
	case 0x20:
	case 0x21:
	case 0x30:
	case 0x40:
		break;
	default:
		NV_ERROR(dev, "displayport table 0x%02x unknown\n", table[0]);
		return NULL;
	}

	for (i = 0; i < table[3]; i++) {
		*entry = ROMPTR(dev, table[table[1] + (i * table[2])]);
		if (*entry && bios_encoder_match(dcb, ROM32((*entry)[0])))
			return table;
	}

	NV_ERROR(dev, "displayport encoder table not found\n");
	return NULL;
}

/******************************************************************************
 * link training
 *****************************************************************************/
struct dp_state {
	struct dp_train_func *func;
	struct dcb_entry *dcb;
	int auxch;
	int crtc;
	u8 *dpcd;
	int link_nr;
	u32 link_bw;
	u8  stat[6];
	u8  conf[4];
};

static void
dp_set_link_config(struct drm_device *dev, struct dp_state *dp)
{
	u8 sink[2];

	NV_DEBUG_KMS(dev, "%d lanes at %d KB/s\n", dp->link_nr, dp->link_bw);

	/* set desired link configuration on the source */
	dp->func->link_set(dev, dp->dcb, dp->crtc, dp->link_nr, dp->link_bw,
			   dp->dpcd[2] & DP_ENHANCED_FRAME_CAP);

	/* inform the sink of the new configuration */
	sink[0] = dp->link_bw / 27000;
	sink[1] = dp->link_nr;
	if (dp->dpcd[2] & DP_ENHANCED_FRAME_CAP)
		sink[1] |= DP_LANE_COUNT_ENHANCED_FRAME_EN;

	auxch_tx(dev, dp->auxch, 8, DP_LINK_BW_SET, sink, 2);
}

static void
dp_set_training_pattern(struct drm_device *dev, struct dp_state *dp, u8 pattern)
{
	u8 sink_tp;

	NV_DEBUG_KMS(dev, "training pattern %d\n", pattern);

	dp->func->train_set(dev, dp->dcb, pattern);

	auxch_tx(dev, dp->auxch, 9, DP_TRAINING_PATTERN_SET, &sink_tp, 1);
	sink_tp &= ~DP_TRAINING_PATTERN_MASK;
	sink_tp |= pattern;
	auxch_tx(dev, dp->auxch, 8, DP_TRAINING_PATTERN_SET, &sink_tp, 1);
}

static int
dp_link_train_commit(struct drm_device *dev, struct dp_state *dp)
{
	int i;

	for (i = 0; i < dp->link_nr; i++) {
		u8 lane = (dp->stat[4 + (i >> 1)] >> ((i & 1) * 4)) & 0xf;
		u8 lpre = (lane & 0x0c) >> 2;
		u8 lvsw = (lane & 0x03) >> 0;

		dp->conf[i] = (lpre << 3) | lvsw;
		if (lvsw == DP_TRAIN_VOLTAGE_SWING_1200)
			dp->conf[i] |= DP_TRAIN_MAX_SWING_REACHED;
		if ((lpre << 3) == DP_TRAIN_PRE_EMPHASIS_9_5)
			dp->conf[i] |= DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;

		NV_DEBUG_KMS(dev, "config lane %d %02x\n", i, dp->conf[i]);
		dp->func->train_adj(dev, dp->dcb, i, lvsw, lpre);
	}

	return auxch_tx(dev, dp->auxch, 8, DP_TRAINING_LANE0_SET, dp->conf, 4);
}

static int
dp_link_train_update(struct drm_device *dev, struct dp_state *dp, u32 delay)
{
	int ret;

	udelay(delay);

	ret = auxch_tx(dev, dp->auxch, 9, DP_LANE0_1_STATUS, dp->stat, 6);
	if (ret)
		return ret;

	NV_DEBUG_KMS(dev, "status %02x %02x %02x %02x %02x %02x\n",
		     dp->stat[0], dp->stat[1], dp->stat[2], dp->stat[3],
		     dp->stat[4], dp->stat[5]);
	return 0;
}

static int
dp_link_train_cr(struct drm_device *dev, struct dp_state *dp)
{
	bool cr_done = false, abort = false;
	int voltage = dp->conf[0] & DP_TRAIN_VOLTAGE_SWING_MASK;
	int tries = 0, i;

	dp_set_training_pattern(dev, dp, DP_TRAINING_PATTERN_1);

	do {
		if (dp_link_train_commit(dev, dp) ||
		    dp_link_train_update(dev, dp, 100))
			break;

		cr_done = true;
		for (i = 0; i < dp->link_nr; i++) {
			u8 lane = (dp->stat[i >> 1] >> ((i & 1) * 4)) & 0xf;
			if (!(lane & DP_LANE_CR_DONE)) {
				cr_done = false;
				if (dp->conf[i] & DP_TRAIN_MAX_SWING_REACHED)
					abort = true;
				break;
			}
		}

		if ((dp->conf[0] & DP_TRAIN_VOLTAGE_SWING_MASK) != voltage) {
			voltage = dp->conf[0] & DP_TRAIN_VOLTAGE_SWING_MASK;
			tries = 0;
		}
	} while (!cr_done && !abort && ++tries < 5);

	return cr_done ? 0 : -1;
}

static int
dp_link_train_eq(struct drm_device *dev, struct dp_state *dp)
{
	bool eq_done, cr_done = true;
	int tries = 0, i;

	dp_set_training_pattern(dev, dp, DP_TRAINING_PATTERN_2);

	do {
		if (dp_link_train_update(dev, dp, 400))
			break;

		eq_done = !!(dp->stat[2] & DP_INTERLANE_ALIGN_DONE);
		for (i = 0; i < dp->link_nr && eq_done; i++) {
			u8 lane = (dp->stat[i >> 1] >> ((i & 1) * 4)) & 0xf;
			if (!(lane & DP_LANE_CR_DONE))
				cr_done = false;
			if (!(lane & DP_LANE_CHANNEL_EQ_DONE) ||
			    !(lane & DP_LANE_SYMBOL_LOCKED))
				eq_done = false;
		}

		if (dp_link_train_commit(dev, dp))
			break;
	} while (!eq_done && cr_done && ++tries <= 5);

	return eq_done ? 0 : -1;
}

static void
dp_set_downspread(struct drm_device *dev, struct dp_state *dp, bool enable)
{
	u16 script = 0x0000;
	u8 *entry, *table = nouveau_dp_bios_data(dev, dp->dcb, &entry);
	if (table) {
		if (table[0] >= 0x20 && table[0] <= 0x30) {
			if (enable) script = ROM16(entry[12]);
			else        script = ROM16(entry[14]);
		} else
		if (table[0] == 0x40) {
			if (enable) script = ROM16(entry[11]);
			else        script = ROM16(entry[13]);
		}
	}

	nouveau_bios_run_init_table(dev, script, dp->dcb, dp->crtc);
}

static void
dp_link_train_init(struct drm_device *dev, struct dp_state *dp)
{
	u16 script = 0x0000;
	u8 *entry, *table = nouveau_dp_bios_data(dev, dp->dcb, &entry);
	if (table) {
		if (table[0] >= 0x20 && table[0] <= 0x30)
			script = ROM16(entry[6]);
		else
		if (table[0] == 0x40)
			script = ROM16(entry[5]);
	}

	nouveau_bios_run_init_table(dev, script, dp->dcb, dp->crtc);
}

static void
dp_link_train_fini(struct drm_device *dev, struct dp_state *dp)
{
	u16 script = 0x0000;
	u8 *entry, *table = nouveau_dp_bios_data(dev, dp->dcb, &entry);
	if (table) {
		if (table[0] >= 0x20 && table[0] <= 0x30)
			script = ROM16(entry[8]);
		else
		if (table[0] == 0x40)
			script = ROM16(entry[7]);
	}

	nouveau_bios_run_init_table(dev, script, dp->dcb, dp->crtc);
}

bool
nouveau_dp_link_train(struct drm_encoder *encoder, u32 datarate,
		      struct dp_train_func *func)
{
	struct nouveau_encoder *nv_encoder = nouveau_encoder(encoder);
	struct nouveau_crtc *nv_crtc = nouveau_crtc(encoder->crtc);
	struct nouveau_connector *nv_connector =
		nouveau_encoder_connector_get(nv_encoder);
	struct drm_device *dev = encoder->dev;
	struct nouveau_i2c_chan *auxch;
	const u32 bw_list[] = { 270000, 162000, 0 };
	const u32 *link_bw = bw_list;
	struct dp_state dp;

	auxch = nouveau_i2c_find(dev, nv_encoder->dcb->i2c_index);
	if (!auxch)
		return false;

	dp.func = func;
	dp.dcb = nv_encoder->dcb;
	dp.crtc = nv_crtc->index;
	dp.auxch = auxch->drive;
	dp.dpcd = nv_encoder->dp.dpcd;

	/* adjust required bandwidth for 8B/10B coding overhead */
	datarate = (datarate / 8) * 10;

	/* some sinks toggle hotplug in response to some of the actions
	 * we take during link training (DP_SET_POWER is one), we need
	 * to ignore them for the moment to avoid races.
	 */
	nouveau_gpio_irq(dev, 0, nv_connector->hpd, 0xff, false);

	/* enable down-spreading, if possible */
	dp_set_downspread(dev, &dp, nv_encoder->dp.dpcd[3] & 1);

	/* execute pre-train script from vbios */
	dp_link_train_init(dev, &dp);

	/* start off at highest link rate supported by encoder and display */
	while (*link_bw > nv_encoder->dp.link_bw)
		link_bw++;

	while (link_bw[0]) {
		/* find minimum required lane count at this link rate */
		dp.link_nr = nv_encoder->dp.link_nr;
		while ((dp.link_nr >> 1) * link_bw[0] > datarate)
			dp.link_nr >>= 1;

		/* drop link rate to minimum with this lane count */
		while ((link_bw[1] * dp.link_nr) > datarate)
			link_bw++;
		dp.link_bw = link_bw[0];

		/* program selected link configuration */
		dp_set_link_config(dev, &dp);

		/* attempt to train the link at this configuration */
		memset(dp.stat, 0x00, sizeof(dp.stat));
		if (!dp_link_train_cr(dev, &dp) &&
		    !dp_link_train_eq(dev, &dp))
			break;

		/* retry at lower rate */
		link_bw++;
	}

	/* finish link training */
	dp_set_training_pattern(dev, &dp, DP_TRAINING_PATTERN_DISABLE);

	/* execute post-train script from vbios */
	dp_link_train_fini(dev, &dp);

	/* re-enable hotplug detect */
	nouveau_gpio_irq(dev, 0, nv_connector->hpd, 0xff, true);
	return true;
}

void
nouveau_dp_dpms(struct drm_encoder *encoder, int mode, u32 datarate,
		struct dp_train_func *func)
{
	struct nouveau_encoder *nv_encoder = nouveau_encoder(encoder);
	struct nouveau_i2c_chan *auxch;
	u8 status;

	auxch = nouveau_i2c_find(encoder->dev, nv_encoder->dcb->i2c_index);
	if (!auxch)
		return;

	if (mode == DRM_MODE_DPMS_ON)
		status = DP_SET_POWER_D0;
	else
		status = DP_SET_POWER_D3;

	nouveau_dp_auxch(auxch, 8, DP_SET_POWER, &status, 1);

	if (mode == DRM_MODE_DPMS_ON)
		nouveau_dp_link_train(encoder, datarate, func);
}

bool
nouveau_dp_detect(struct drm_encoder *encoder)
{
	struct nouveau_encoder *nv_encoder = nouveau_encoder(encoder);
	struct drm_device *dev = encoder->dev;
	struct nouveau_i2c_chan *auxch;
	u8 *dpcd = nv_encoder->dp.dpcd;
	int ret;

	auxch = nouveau_i2c_find(dev, nv_encoder->dcb->i2c_index);
	if (!auxch)
		return false;

	ret = auxch_tx(dev, auxch->drive, 9, DP_DPCD_REV, dpcd, 8);
	if (ret)
		return false;

	nv_encoder->dp.link_bw = 27000 * dpcd[1];
	nv_encoder->dp.link_nr = dpcd[2] & DP_MAX_LANE_COUNT_MASK;

	NV_DEBUG_KMS(dev, "display: %dx%d dpcd 0x%02x\n",
		     nv_encoder->dp.link_nr, nv_encoder->dp.link_bw, dpcd[0]);
	NV_DEBUG_KMS(dev, "encoder: %dx%d\n",
		     nv_encoder->dcb->dpconf.link_nr,
		     nv_encoder->dcb->dpconf.link_bw);

	if (nv_encoder->dcb->dpconf.link_nr < nv_encoder->dp.link_nr)
		nv_encoder->dp.link_nr = nv_encoder->dcb->dpconf.link_nr;
	if (nv_encoder->dcb->dpconf.link_bw < nv_encoder->dp.link_bw)
		nv_encoder->dp.link_bw = nv_encoder->dcb->dpconf.link_bw;

	NV_DEBUG_KMS(dev, "maximum: %dx%d\n",
		     nv_encoder->dp.link_nr, nv_encoder->dp.link_bw);

	return true;
}

int
nouveau_dp_auxch(struct nouveau_i2c_chan *auxch, int cmd, int addr,
		 uint8_t *data, int data_nr)
{
	return auxch_tx(auxch->dev, auxch->drive, cmd, addr, data, data_nr);
}

static int
nouveau_dp_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	struct nouveau_i2c_chan *auxch = (struct nouveau_i2c_chan *)adap;
	struct i2c_msg *msg = msgs;
	int ret, mcnt = num;

	while (mcnt--) {
		u8 remaining = msg->len;
		u8 *ptr = msg->buf;

		while (remaining) {
			u8 cnt = (remaining > 16) ? 16 : remaining;
			u8 cmd;

			if (msg->flags & I2C_M_RD)
				cmd = AUX_I2C_READ;
			else
				cmd = AUX_I2C_WRITE;

			if (mcnt || remaining > 16)
				cmd |= AUX_I2C_MOT;

			ret = nouveau_dp_auxch(auxch, cmd, msg->addr, ptr, cnt);
			if (ret < 0)
				return ret;

			ptr += cnt;
			remaining -= cnt;
		}

		msg++;
	}

	return num;
}

static u32
nouveau_dp_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

const struct i2c_algorithm nouveau_dp_i2c_algo = {
	.master_xfer = nouveau_dp_i2c_xfer,
	.functionality = nouveau_dp_i2c_func
};
=======
#include <drm/display/drm_dp_helper.h>

#include "nouveau_drv.h"
#include "nouveau_connector.h"
#include "nouveau_encoder.h"
#include "nouveau_crtc.h"

#include <nvif/if0011.h>

MODULE_PARM_DESC(mst, "Enable DisplayPort multi-stream (default: enabled)");
static int nouveau_mst = 1;
module_param_named(mst, nouveau_mst, int, 0400);

static bool
nouveau_dp_has_sink_count(struct drm_connector *connector,
			  struct nouveau_encoder *outp)
{
	return drm_dp_read_sink_count_cap(connector, outp->dp.dpcd, &outp->dp.desc);
}

static bool
nouveau_dp_probe_lttpr(struct nouveau_encoder *outp)
{
	u8 rev, size = sizeof(rev);
	int ret;

	ret = nvif_outp_dp_aux_xfer(&outp->outp, DP_AUX_NATIVE_READ, &size,
				    DP_LT_TUNABLE_PHY_REPEATER_FIELD_DATA_STRUCTURE_REV,
				    &rev);
	if (ret || size < sizeof(rev) || rev < 0x14)
		return false;

	return true;
}

static enum drm_connector_status
nouveau_dp_probe_dpcd(struct nouveau_connector *nv_connector,
		      struct nouveau_encoder *outp)
{
	struct drm_connector *connector = &nv_connector->base;
	struct drm_dp_aux *aux = &nv_connector->aux;
	struct nv50_mstm *mstm = NULL;
	enum drm_connector_status status = connector_status_disconnected;
	int ret;
	u8 *dpcd = outp->dp.dpcd;

	outp->dp.lttpr.nr = 0;
	outp->dp.rate_nr  = 0;
	outp->dp.link_nr  = 0;
	outp->dp.link_bw  = 0;

	if (connector->connector_type != DRM_MODE_CONNECTOR_eDP &&
	    nouveau_dp_probe_lttpr(outp) &&
	    !drm_dp_read_dpcd_caps(aux, dpcd) &&
	    !drm_dp_read_lttpr_common_caps(aux, dpcd, outp->dp.lttpr.caps)) {
		int nr = drm_dp_lttpr_count(outp->dp.lttpr.caps);

		if (nr) {
			drm_dp_dpcd_writeb(aux, DP_PHY_REPEATER_MODE,
						DP_PHY_REPEATER_MODE_TRANSPARENT);

			if (nr > 0) {
				ret = drm_dp_dpcd_writeb(aux, DP_PHY_REPEATER_MODE,
							      DP_PHY_REPEATER_MODE_NON_TRANSPARENT);
				if (ret != 1) {
					drm_dp_dpcd_writeb(aux, DP_PHY_REPEATER_MODE,
								DP_PHY_REPEATER_MODE_TRANSPARENT);
				} else {
					outp->dp.lttpr.nr = nr;
				}
			}
		}
	}

	ret = drm_dp_read_dpcd_caps(aux, dpcd);
	if (ret < 0)
		goto out;

	outp->dp.link_nr = dpcd[DP_MAX_LANE_COUNT] & DP_MAX_LANE_COUNT_MASK;
	if (outp->dcb->dpconf.link_nr < outp->dp.link_nr)
		outp->dp.link_nr = outp->dcb->dpconf.link_nr;

	if (outp->dp.lttpr.nr) {
		int links = drm_dp_lttpr_max_lane_count(outp->dp.lttpr.caps);

		if (links && links < outp->dp.link_nr)
			outp->dp.link_nr = links;
	}

	if (connector->connector_type == DRM_MODE_CONNECTOR_eDP && dpcd[DP_DPCD_REV] >= 0x13) {
		__le16 rates[DP_MAX_SUPPORTED_RATES];

		ret = drm_dp_dpcd_read(aux, DP_SUPPORTED_LINK_RATES, rates, sizeof(rates));
		if (ret == sizeof(rates)) {
			for (int i = 0; i < ARRAY_SIZE(rates); i++) {
				u32 rate = (le16_to_cpu(rates[i]) * 200) / 10;
				int j;

				if (!rate)
					break;

				for (j = 0; j < outp->dp.rate_nr; j++) {
					if (rate > outp->dp.rate[j].rate) {
						for (int k = outp->dp.rate_nr; k > j; k--)
							outp->dp.rate[k] = outp->dp.rate[k - 1];
						break;
					}
				}

				outp->dp.rate[j].dpcd = i;
				outp->dp.rate[j].rate = rate;
				outp->dp.rate_nr++;
			}
		}
	}

	if (!outp->dp.rate_nr) {
		const u32 rates[] = { 810000, 540000, 270000, 162000 };
		u32 max_rate = dpcd[DP_MAX_LINK_RATE] * 27000;

		if (outp->dp.lttpr.nr) {
			int rate = drm_dp_lttpr_max_link_rate(outp->dp.lttpr.caps);

			if (rate && rate < max_rate)
				max_rate = rate;
		}

		max_rate = min_t(int, max_rate, outp->dcb->dpconf.link_bw);

		for (int i = 0; i < ARRAY_SIZE(rates); i++) {
			if (rates[i] <= max_rate) {
				outp->dp.rate[outp->dp.rate_nr].dpcd = -1;
				outp->dp.rate[outp->dp.rate_nr].rate = rates[i];
				outp->dp.rate_nr++;
			}
		}

		if (WARN_ON(!outp->dp.rate_nr))
			goto out;
	}

	ret = nvif_outp_dp_rates(&outp->outp, outp->dp.rate, outp->dp.rate_nr);
	if (ret)
		goto out;

	for (int i = 0; i < outp->dp.rate_nr; i++) {
		u32 link_bw = outp->dp.rate[i].rate;

		if (link_bw > outp->dp.link_bw)
			outp->dp.link_bw = link_bw;
	}

	ret = drm_dp_read_desc(aux, &outp->dp.desc, drm_dp_is_branch(dpcd));
	if (ret < 0)
		goto out;

	if (nouveau_mst) {
		mstm = outp->dp.mstm;
		if (mstm)
			mstm->can_mst = drm_dp_read_mst_cap(aux, dpcd);
	}

	if (nouveau_dp_has_sink_count(connector, outp)) {
		ret = drm_dp_read_sink_count(aux);
		if (ret < 0)
			goto out;

		outp->dp.sink_count = ret;

		/*
		 * Dongle connected, but no display. Don't bother reading
		 * downstream port info
		 */
		if (!outp->dp.sink_count)
			return connector_status_disconnected;
	}

	ret = drm_dp_read_downstream_info(aux, dpcd,
					  outp->dp.downstream_ports);
	if (ret < 0)
		goto out;

	status = connector_status_connected;
out:
	if (status != connector_status_connected) {
		/* Clear any cached info */
		outp->dp.sink_count = 0;
	}
	return status;
}

int
nouveau_dp_detect(struct nouveau_connector *nv_connector,
		  struct nouveau_encoder *nv_encoder)
{
	struct drm_device *dev = nv_encoder->base.base.dev;
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_connector *connector = &nv_connector->base;
	struct nv50_mstm *mstm = nv_encoder->dp.mstm;
	enum drm_connector_status status;
	u8 *dpcd = nv_encoder->dp.dpcd;
	int ret = NOUVEAU_DP_NONE, hpd;

	/* eDP ports don't support hotplugging - so there's no point in probing eDP ports unless we
	 * haven't probed them once before.
	 */
	if (connector->connector_type == DRM_MODE_CONNECTOR_eDP) {
		if (connector->status == connector_status_connected)
			return NOUVEAU_DP_SST;
		else if (connector->status == connector_status_disconnected)
			return NOUVEAU_DP_NONE;
	}

	// Ensure that the aux bus is enabled for probing
	drm_dp_dpcd_set_powered(&nv_connector->aux, true);

	mutex_lock(&nv_encoder->dp.hpd_irq_lock);
	if (mstm) {
		/* If we're not ready to handle MST state changes yet, just
		 * report the last status of the connector. We'll reprobe it
		 * once we've resumed.
		 */
		if (mstm->suspended) {
			if (mstm->is_mst)
				ret = NOUVEAU_DP_MST;
			else if (connector->status ==
				 connector_status_connected)
				ret = NOUVEAU_DP_SST;

			goto out;
		}
	}

	hpd = nvif_outp_detect(&nv_encoder->outp);
	if (hpd == NOT_PRESENT) {
		nvif_outp_dp_aux_pwr(&nv_encoder->outp, false);
		goto out;
	}
	nvif_outp_dp_aux_pwr(&nv_encoder->outp, true);

	status = nouveau_dp_probe_dpcd(nv_connector, nv_encoder);
	if (status == connector_status_disconnected) {
		nvif_outp_dp_aux_pwr(&nv_encoder->outp, false);
		goto out;
	}

	/* If we're in MST mode, we're done here */
	if (mstm && mstm->can_mst && mstm->is_mst) {
		ret = NOUVEAU_DP_MST;
		goto out;
	}

	NV_DEBUG(drm, "sink dpcd version: 0x%02x\n", dpcd[DP_DPCD_REV]);
	for (int i = 0; i < nv_encoder->dp.rate_nr; i++)
		NV_DEBUG(drm, "sink rate %d: %d\n", i, nv_encoder->dp.rate[i].rate);

	NV_DEBUG(drm, "encoder: %dx%d\n", nv_encoder->dcb->dpconf.link_nr,
					  nv_encoder->dcb->dpconf.link_bw);
	NV_DEBUG(drm, "maximum: %dx%d\n", nv_encoder->dp.link_nr,
					  nv_encoder->dp.link_bw);

	if (mstm && mstm->can_mst) {
		ret = nv50_mstm_detect(nv_encoder);
		if (ret == 1) {
			ret = NOUVEAU_DP_MST;
			goto out;
		} else if (ret != 0) {
			nvif_outp_dp_aux_pwr(&nv_encoder->outp, false);
			goto out;
		}
	}
	ret = NOUVEAU_DP_SST;

out:
	if (mstm && !mstm->suspended && ret != NOUVEAU_DP_MST)
		nv50_mstm_remove(mstm);

	/* GSP doesn't like when we try to do aux transactions on a port it considers disconnected,
	 * and since we don't really have a usecase for that anyway - just disable the aux bus here
	 * if we've decided the connector is disconnected
	 */
	if (ret == NOUVEAU_DP_NONE)
		drm_dp_dpcd_set_powered(&nv_connector->aux, false);

	mutex_unlock(&nv_encoder->dp.hpd_irq_lock);
	return ret;
}

void
nouveau_dp_power_down(struct nouveau_encoder *outp)
{
	struct drm_dp_aux *aux = &outp->conn->aux;
	int ret;
	u8 pwr;

	mutex_lock(&outp->dp.hpd_irq_lock);

	ret = drm_dp_dpcd_readb(aux, DP_SET_POWER, &pwr);
	if (ret == 1) {
		pwr &= ~DP_SET_POWER_MASK;
		pwr |=  DP_SET_POWER_D3;
		drm_dp_dpcd_writeb(aux, DP_SET_POWER, pwr);
	}

	outp->dp.lt.nr = 0;
	mutex_unlock(&outp->dp.hpd_irq_lock);
}

static bool
nouveau_dp_train_link(struct nouveau_encoder *outp, bool retrain)
{
	struct drm_dp_aux *aux = &outp->conn->aux;
	bool post_lt = false;
	int ret, retries = 0;

	if ( (outp->dp.dpcd[DP_MAX_LANE_COUNT] & 0x20) &&
	    !(outp->dp.dpcd[DP_MAX_DOWNSPREAD] & DP_TPS4_SUPPORTED))
	    post_lt = true;

retry:
	ret = nvif_outp_dp_train(&outp->outp, outp->dp.dpcd,
					      outp->dp.lttpr.nr,
					      outp->dp.lt.nr,
					      outp->dp.lt.bw,
					      outp->dp.lt.mst,
					      post_lt,
					      retrain);
	if (ret)
		return false;

	if (post_lt) {
		u8 stat[DP_LINK_STATUS_SIZE];
		u8 prev[2];
		u8 time = 0, adjusts = 0, tmp;

		ret = drm_dp_dpcd_read_phy_link_status(aux, DP_PHY_DPRX, stat);
		if (ret)
			return false;

		for (;;) {
			if (!drm_dp_channel_eq_ok(stat, outp->dp.lt.nr)) {
				ret = 1;
				break;
			}

			if (!(stat[2] & 0x02))
				break;

			msleep(5);
			time += 5;

			memcpy(prev, &stat[4], sizeof(prev));
			ret = drm_dp_dpcd_read_phy_link_status(aux, DP_PHY_DPRX, stat);
			if (ret)
				break;

			if (!memcmp(prev, &stat[4], sizeof(prev))) {
				if (time > 200)
					break;
			} else {
				u8 pe[4], vs[4];

				if (adjusts++ == 6)
					break;

				for (int i = 0; i < outp->dp.lt.nr; i++) {
					pe[i] = drm_dp_get_adjust_request_pre_emphasis(stat, i) >>
							DP_TRAIN_PRE_EMPHASIS_SHIFT;
					vs[i] = drm_dp_get_adjust_request_voltage(stat, i) >>
							DP_TRAIN_VOLTAGE_SWING_SHIFT;
				}

				ret = nvif_outp_dp_drive(&outp->outp, outp->dp.lt.nr, pe, vs);
				if (ret)
					break;

				time = 0;
			}
		}

		if (drm_dp_dpcd_readb(aux, DP_LANE_COUNT_SET, &tmp) == 1) {
			tmp &= ~0x20;
			drm_dp_dpcd_writeb(aux, DP_LANE_COUNT_SET, tmp);
		}
	}

	if (ret == 1 && retries++ < 3)
		goto retry;

	return ret == 0;
}

bool
nouveau_dp_train(struct nouveau_encoder *outp, bool mst, u32 khz, u8 bpc)
{
	struct nouveau_drm *drm = nouveau_drm(outp->base.base.dev);
	struct drm_dp_aux *aux = &outp->conn->aux;
	u32 min_rate;
	u8 pwr;
	bool ret = true;

	if (mst)
		min_rate = outp->dp.link_nr * outp->dp.rate[0].rate;
	else
		min_rate = DIV_ROUND_UP(khz * bpc * 3, 8);

	NV_DEBUG(drm, "%s link training (mst:%d min_rate:%d)\n",
		 outp->base.base.name, mst, min_rate);

	mutex_lock(&outp->dp.hpd_irq_lock);

	if (drm_dp_dpcd_readb(aux, DP_SET_POWER, &pwr) == 1) {
		if ((pwr & DP_SET_POWER_MASK) != DP_SET_POWER_D0) {
			pwr &= ~DP_SET_POWER_MASK;
			pwr |=  DP_SET_POWER_D0;
			drm_dp_dpcd_writeb(aux, DP_SET_POWER, pwr);
		}
	}

	for (int nr = outp->dp.link_nr; nr; nr >>= 1) {
		for (int rate = 0; rate < outp->dp.rate_nr; rate++) {
			if (outp->dp.rate[rate].rate * nr >= min_rate) {
				outp->dp.lt.nr = nr;
				outp->dp.lt.bw = outp->dp.rate[rate].rate;
				outp->dp.lt.mst = mst;
				if (nouveau_dp_train_link(outp, false))
					goto done;
			}
		}
	}

	ret = false;
done:
	mutex_unlock(&outp->dp.hpd_irq_lock);
	return ret;
}

static bool
nouveau_dp_link_check_locked(struct nouveau_encoder *outp)
{
	u8 link_status[DP_LINK_STATUS_SIZE];

	if (!outp || !outp->dp.lt.nr)
		return true;

	if (drm_dp_dpcd_read_phy_link_status(&outp->conn->aux, DP_PHY_DPRX, link_status) < 0)
		return false;

	if (drm_dp_channel_eq_ok(link_status, outp->dp.lt.nr))
		return true;

	return nouveau_dp_train_link(outp, true);
}

bool
nouveau_dp_link_check(struct nouveau_connector *nv_connector)
{
	struct nouveau_encoder *outp = nv_connector->dp_encoder;
	bool link_ok = true;

	if (outp) {
		mutex_lock(&outp->dp.hpd_irq_lock);
		if (outp->dp.lt.nr)
			link_ok = nouveau_dp_link_check_locked(outp);
		mutex_unlock(&outp->dp.hpd_irq_lock);
	}

	return link_ok;
}

void
nouveau_dp_irq(struct work_struct *work)
{
	struct nouveau_connector *nv_connector =
		container_of(work, typeof(*nv_connector), irq_work);
	struct drm_connector *connector = &nv_connector->base;
	struct nouveau_encoder *outp = find_encoder(connector, DCB_OUTPUT_DP);
	struct nouveau_drm *drm = nouveau_drm(outp->base.base.dev);
	struct nv50_mstm *mstm;
	u64 hpd = 0;
	int ret;

	if (!outp)
		return;

	mstm = outp->dp.mstm;
	NV_DEBUG(drm, "service %s\n", connector->name);

	mutex_lock(&outp->dp.hpd_irq_lock);

	if (mstm && mstm->is_mst) {
		if (!nv50_mstm_service(drm, nv_connector, mstm))
			hpd |= NVIF_CONN_EVENT_V0_UNPLUG;
	} else {
		drm_dp_cec_irq(&nv_connector->aux);

		if (nouveau_dp_has_sink_count(connector, outp)) {
			ret = drm_dp_read_sink_count(&nv_connector->aux);
			if (ret != outp->dp.sink_count)
				hpd |= NVIF_CONN_EVENT_V0_PLUG;
			if (ret >= 0)
				outp->dp.sink_count = ret;
		}
	}

	mutex_unlock(&outp->dp.hpd_irq_lock);

	nouveau_connector_hpd(nv_connector, NVIF_CONN_EVENT_V0_IRQ | hpd);
}

/* TODO:
 * - Validate against the DP caps advertised by the GPU (we don't check these
 *   yet)
 */
enum drm_mode_status
nv50_dp_mode_valid(struct nouveau_encoder *outp,
		   const struct drm_display_mode *mode,
		   unsigned *out_clock)
{
	const unsigned int min_clock = 25000;
	unsigned int max_rate, mode_rate, ds_max_dotclock, clock = mode->clock;
	/* Check with the minmum bpc always, so we can advertise better modes.
	 * In particlar not doing this causes modes to be dropped on HDR
	 * displays as we might check with a bpc of 16 even.
	 */
	const u8 bpp = 6 * 3;

	if (mode->flags & DRM_MODE_FLAG_INTERLACE && !outp->caps.dp_interlace)
		return MODE_NO_INTERLACE;

	if ((mode->flags & DRM_MODE_FLAG_3D_MASK) == DRM_MODE_FLAG_3D_FRAME_PACKING)
		clock *= 2;

	max_rate = outp->dp.link_nr * outp->dp.link_bw;
	mode_rate = DIV_ROUND_UP(clock * bpp, 8);
	if (mode_rate > max_rate)
		return MODE_CLOCK_HIGH;

	ds_max_dotclock = drm_dp_downstream_max_dotclock(outp->dp.dpcd, outp->dp.downstream_ports);
	if (ds_max_dotclock && clock > ds_max_dotclock)
		return MODE_CLOCK_HIGH;

	if (clock < min_clock)
		return MODE_CLOCK_LOW;

	if (out_clock)
		*out_clock = clock;

	return MODE_OK;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
