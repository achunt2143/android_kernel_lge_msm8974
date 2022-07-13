<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * wm_adsp.c  --  Wolfson ADSP support
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
 */

#include <linux/ctype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/firmware.h>
<<<<<<< HEAD
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
=======
#include <linux/list.h>
#include <linux/pm.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <linux/debugfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>
#include <sound/initval.h>
#include <sound/tlv.h>

<<<<<<< HEAD
#include <linux/mfd/arizona/registers.h>

#include "arizona.h"
#include "wm_adsp.h"

#define adsp_crit(_dsp, fmt, ...) \
	dev_crit(_dsp->dev, "DSP%d: " fmt, _dsp->num, ##__VA_ARGS__)
#define adsp_err(_dsp, fmt, ...) \
	dev_err(_dsp->dev, "DSP%d: " fmt, _dsp->num, ##__VA_ARGS__)
#define adsp_warn(_dsp, fmt, ...) \
	dev_warn(_dsp->dev, "DSP%d: " fmt, _dsp->num, ##__VA_ARGS__)
#define adsp_info(_dsp, fmt, ...) \
	dev_info(_dsp->dev, "DSP%d: " fmt, _dsp->num, ##__VA_ARGS__)
#define adsp_dbg(_dsp, fmt, ...) \
	dev_dbg(_dsp->dev, "DSP%d: " fmt, _dsp->num, ##__VA_ARGS__)

#define ADSP1_CONTROL_1                   0x00
#define ADSP1_CONTROL_2                   0x02
#define ADSP1_CONTROL_3                   0x03
#define ADSP1_CONTROL_4                   0x04
#define ADSP1_CONTROL_5                   0x06
#define ADSP1_CONTROL_6                   0x07
#define ADSP1_CONTROL_7                   0x08
#define ADSP1_CONTROL_8                   0x09
#define ADSP1_CONTROL_9                   0x0A
#define ADSP1_CONTROL_10                  0x0B
#define ADSP1_CONTROL_11                  0x0C
#define ADSP1_CONTROL_12                  0x0D
#define ADSP1_CONTROL_13                  0x0F
#define ADSP1_CONTROL_14                  0x10
#define ADSP1_CONTROL_15                  0x11
#define ADSP1_CONTROL_16                  0x12
#define ADSP1_CONTROL_17                  0x13
#define ADSP1_CONTROL_18                  0x14
#define ADSP1_CONTROL_19                  0x16
#define ADSP1_CONTROL_20                  0x17
#define ADSP1_CONTROL_21                  0x18
#define ADSP1_CONTROL_22                  0x1A
#define ADSP1_CONTROL_23                  0x1B
#define ADSP1_CONTROL_24                  0x1C
#define ADSP1_CONTROL_25                  0x1E
#define ADSP1_CONTROL_26                  0x20
#define ADSP1_CONTROL_27                  0x21
#define ADSP1_CONTROL_28                  0x22
#define ADSP1_CONTROL_29                  0x23
#define ADSP1_CONTROL_30                  0x24
#define ADSP1_CONTROL_31                  0x26

/*
 * ADSP1 Control 19
 */
#define ADSP1_WDMA_BUFFER_LENGTH_MASK     0x00FF  /* DSP1_WDMA_BUFFER_LENGTH - [7:0] */
#define ADSP1_WDMA_BUFFER_LENGTH_SHIFT         0  /* DSP1_WDMA_BUFFER_LENGTH - [7:0] */
#define ADSP1_WDMA_BUFFER_LENGTH_WIDTH         8  /* DSP1_WDMA_BUFFER_LENGTH - [7:0] */


/*
 * ADSP1 Control 30
 */
#define ADSP1_DBG_CLK_ENA                 0x0008  /* DSP1_DBG_CLK_ENA */
#define ADSP1_DBG_CLK_ENA_MASK            0x0008  /* DSP1_DBG_CLK_ENA */
#define ADSP1_DBG_CLK_ENA_SHIFT                3  /* DSP1_DBG_CLK_ENA */
#define ADSP1_DBG_CLK_ENA_WIDTH                1  /* DSP1_DBG_CLK_ENA */
#define ADSP1_SYS_ENA                     0x0004  /* DSP1_SYS_ENA */
#define ADSP1_SYS_ENA_MASK                0x0004  /* DSP1_SYS_ENA */
#define ADSP1_SYS_ENA_SHIFT                    2  /* DSP1_SYS_ENA */
#define ADSP1_SYS_ENA_WIDTH                    1  /* DSP1_SYS_ENA */
#define ADSP1_CORE_ENA                    0x0002  /* DSP1_CORE_ENA */
#define ADSP1_CORE_ENA_MASK               0x0002  /* DSP1_CORE_ENA */
#define ADSP1_CORE_ENA_SHIFT                   1  /* DSP1_CORE_ENA */
#define ADSP1_CORE_ENA_WIDTH                   1  /* DSP1_CORE_ENA */
#define ADSP1_START                       0x0001  /* DSP1_START */
#define ADSP1_START_MASK                  0x0001  /* DSP1_START */
#define ADSP1_START_SHIFT                      0  /* DSP1_START */
#define ADSP1_START_WIDTH                      1  /* DSP1_START */

#define ADSP2_CONTROL        0x0
#define ADSP2_CLOCKING       0x1
#define ADSP2_STATUS1        0x4
#define ADSP2_WDMA_CONFIG_1 0x30
#define ADSP2_WDMA_CONFIG_2 0x31
#define ADSP2_RDMA_CONFIG_1 0x34

/*
 * ADSP2 Control
 */

#define ADSP2_MEM_ENA                     0x0010  /* DSP1_MEM_ENA */
#define ADSP2_MEM_ENA_MASK                0x0010  /* DSP1_MEM_ENA */
#define ADSP2_MEM_ENA_SHIFT                    4  /* DSP1_MEM_ENA */
#define ADSP2_MEM_ENA_WIDTH                    1  /* DSP1_MEM_ENA */
#define ADSP2_SYS_ENA                     0x0004  /* DSP1_SYS_ENA */
#define ADSP2_SYS_ENA_MASK                0x0004  /* DSP1_SYS_ENA */
#define ADSP2_SYS_ENA_SHIFT                    2  /* DSP1_SYS_ENA */
#define ADSP2_SYS_ENA_WIDTH                    1  /* DSP1_SYS_ENA */
#define ADSP2_CORE_ENA                    0x0002  /* DSP1_CORE_ENA */
#define ADSP2_CORE_ENA_MASK               0x0002  /* DSP1_CORE_ENA */
#define ADSP2_CORE_ENA_SHIFT                   1  /* DSP1_CORE_ENA */
#define ADSP2_CORE_ENA_WIDTH                   1  /* DSP1_CORE_ENA */
#define ADSP2_START                       0x0001  /* DSP1_START */
#define ADSP2_START_MASK                  0x0001  /* DSP1_START */
#define ADSP2_START_SHIFT                      0  /* DSP1_START */
#define ADSP2_START_WIDTH                      1  /* DSP1_START */

/*
 * ADSP2 clocking
 */
#define ADSP2_CLK_SEL_MASK                0x0007  /* CLK_SEL_ENA */
#define ADSP2_CLK_SEL_SHIFT                    0  /* CLK_SEL_ENA */
#define ADSP2_CLK_SEL_WIDTH                    3  /* CLK_SEL_ENA */

/*
 * ADSP2 Status 1
 */
#define ADSP2_RAM_RDY                     0x0001
#define ADSP2_RAM_RDY_MASK                0x0001
#define ADSP2_RAM_RDY_SHIFT                    0
#define ADSP2_RAM_RDY_WIDTH                    1

/* Must remain a power of two */
#define WM_ADSP_CAPTURE_BUFFER_SIZE      1048576

#define WM_ADSP_NUM_FW 5

#define WM_ADSP_FW_MBC_VSS        0
#define WM_ADSP_FW_TX             1
#define WM_ADSP_FW_TX_SPK         2
#define WM_ADSP_FW_RX_ANC         3
#define WM_ADSP_FW_EZ2CONTROL     4

static const char *wm_adsp_fw_text[WM_ADSP_NUM_FW] = {
	[WM_ADSP_FW_MBC_VSS] =    "MBC/VSS",
	[WM_ADSP_FW_TX] =         "Tx",
	[WM_ADSP_FW_TX_SPK] =     "Tx Speaker",
	[WM_ADSP_FW_RX_ANC] =     "Rx ANC",
	[WM_ADSP_FW_EZ2CONTROL] = "Ez2Control",
};

struct wm_adsp_buffer_region_def {
	unsigned int mem_type;
	unsigned int base_offset;
	unsigned int size_offset;
};

struct wm_adsp_fw_caps {
	u32 id;
	struct snd_codec_desc desc;
	int num_host_regions;
	struct wm_adsp_buffer_region_def *host_region_defs;
=======
#include "wm_adsp.h"

#define adsp_crit(_dsp, fmt, ...) \
	dev_crit(_dsp->cs_dsp.dev, "%s: " fmt, _dsp->cs_dsp.name, ##__VA_ARGS__)
#define adsp_err(_dsp, fmt, ...) \
	dev_err(_dsp->cs_dsp.dev, "%s: " fmt, _dsp->cs_dsp.name, ##__VA_ARGS__)
#define adsp_warn(_dsp, fmt, ...) \
	dev_warn(_dsp->cs_dsp.dev, "%s: " fmt, _dsp->cs_dsp.name, ##__VA_ARGS__)
#define adsp_info(_dsp, fmt, ...) \
	dev_info(_dsp->cs_dsp.dev, "%s: " fmt, _dsp->cs_dsp.name, ##__VA_ARGS__)
#define adsp_dbg(_dsp, fmt, ...) \
	dev_dbg(_dsp->cs_dsp.dev, "%s: " fmt, _dsp->cs_dsp.name, ##__VA_ARGS__)

#define compr_err(_obj, fmt, ...) \
	adsp_err(_obj->dsp, "%s: " fmt, _obj->name ? _obj->name : "legacy", \
		 ##__VA_ARGS__)
#define compr_dbg(_obj, fmt, ...) \
	adsp_dbg(_obj->dsp, "%s: " fmt, _obj->name ? _obj->name : "legacy", \
		 ##__VA_ARGS__)

#define ADSP_MAX_STD_CTRL_SIZE               512

static const struct cs_dsp_client_ops wm_adsp1_client_ops;
static const struct cs_dsp_client_ops wm_adsp2_client_ops;

#define WM_ADSP_FW_MBC_VSS  0
#define WM_ADSP_FW_HIFI     1
#define WM_ADSP_FW_TX       2
#define WM_ADSP_FW_TX_SPK   3
#define WM_ADSP_FW_RX       4
#define WM_ADSP_FW_RX_ANC   5
#define WM_ADSP_FW_CTRL     6
#define WM_ADSP_FW_ASR      7
#define WM_ADSP_FW_TRACE    8
#define WM_ADSP_FW_SPK_PROT 9
#define WM_ADSP_FW_SPK_CALI 10
#define WM_ADSP_FW_SPK_DIAG 11
#define WM_ADSP_FW_MISC     12

#define WM_ADSP_NUM_FW      13

static const char *wm_adsp_fw_text[WM_ADSP_NUM_FW] = {
	[WM_ADSP_FW_MBC_VSS] =  "MBC/VSS",
	[WM_ADSP_FW_HIFI] =     "MasterHiFi",
	[WM_ADSP_FW_TX] =       "Tx",
	[WM_ADSP_FW_TX_SPK] =   "Tx Speaker",
	[WM_ADSP_FW_RX] =       "Rx",
	[WM_ADSP_FW_RX_ANC] =   "Rx ANC",
	[WM_ADSP_FW_CTRL] =     "Voice Ctrl",
	[WM_ADSP_FW_ASR] =      "ASR Assist",
	[WM_ADSP_FW_TRACE] =    "Dbg Trace",
	[WM_ADSP_FW_SPK_PROT] = "Protection",
	[WM_ADSP_FW_SPK_CALI] = "Calibration",
	[WM_ADSP_FW_SPK_DIAG] = "Diagnostic",
	[WM_ADSP_FW_MISC] =     "Misc",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct wm_adsp_system_config_xm_hdr {
	__be32 sys_enable;
	__be32 fw_id;
	__be32 fw_rev;
	__be32 boot_status;
	__be32 watchdog;
	__be32 dma_buffer_size;
	__be32 rdma[6];
	__be32 wdma[8];
	__be32 build_job_name[3];
	__be32 build_job_number;
<<<<<<< HEAD
};
=======
} __packed;

struct wm_halo_system_config_xm_hdr {
	__be32 halo_heartbeat;
	__be32 build_job_name[3];
	__be32 build_job_number;
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct wm_adsp_alg_xm_struct {
	__be32 magic;
	__be32 smoothing;
	__be32 threshold;
	__be32 host_buf_ptr;
	__be32 start_seq;
	__be32 high_water_mark;
	__be32 low_water_mark;
	__be64 smoothed_power;
<<<<<<< HEAD
};

struct wm_adsp_host_buffer {
	__be32 X_buf_base;
	__be32 X_buf_size;
	__be32 X_buf_base2;
	__be32 X_buf_size2;
	__be32 Y_buf_base;
	__be32 Y_buf_size;
	__be32 high_water_mark;
	__be32 low_water_mark;
	__be32 next_write_index;
	__be32 next_read_index;
	__be32 overflow;
	__be32 state;
	__be32 wrapped;
	__be32 requested_rewind;
	__be32 applied_rewind;
};

#define WM_ADSP_DATA_WORD_SIZE         3
#define WM_ADSP_RAW_BUFFER_SAMPS       384
#define WM_ADSP_ALG_XM_STRUCT_MAGIC    0x58b90c

#define ADSP2_SYSTEM_CONFIG_XM_PTR \
	(offsetof(struct wmfw_adsp2_id_hdr, xm) / sizeof(__be32))

#define WM_ADSP_ALG_XM_PTR \
	(sizeof(struct wm_adsp_system_config_xm_hdr) / sizeof(__be32))

#define HOST_BUFFER_FIELD(field) \
	(offsetof(struct wm_adsp_host_buffer, field) / sizeof(__be32))
=======
} __packed;

struct wm_adsp_host_buf_coeff_v1 {
	__be32 host_buf_ptr;		/* Host buffer pointer */
	__be32 versions;		/* Version numbers */
	__be32 name[4];			/* The buffer name */
} __packed;

struct wm_adsp_buffer {
	__be32 buf1_base;		/* Base addr of first buffer area */
	__be32 buf1_size;		/* Size of buf1 area in DSP words */
	__be32 buf2_base;		/* Base addr of 2nd buffer area */
	__be32 buf1_buf2_size;		/* Size of buf1+buf2 in DSP words */
	__be32 buf3_base;		/* Base addr of buf3 area */
	__be32 buf_total_size;		/* Size of buf1+buf2+buf3 in DSP words */
	__be32 high_water_mark;		/* Point at which IRQ is asserted */
	__be32 irq_count;		/* bits 1-31 count IRQ assertions */
	__be32 irq_ack;			/* acked IRQ count, bit 0 enables IRQ */
	__be32 next_write_index;	/* word index of next write */
	__be32 next_read_index;		/* word index of next read */
	__be32 error;			/* error if any */
	__be32 oldest_block_index;	/* word index of oldest surviving */
	__be32 requested_rewind;	/* how many blocks rewind was done */
	__be32 reserved_space;		/* internal */
	__be32 min_free;		/* min free space since stream start */
	__be32 blocks_written[2];	/* total blocks written (64 bit) */
	__be32 words_written[2];	/* total words written (64 bit) */
} __packed;

struct wm_adsp_compr;

struct wm_adsp_compr_buf {
	struct list_head list;
	struct wm_adsp *dsp;
	struct wm_adsp_compr *compr;

	struct wm_adsp_buffer_region *regions;
	u32 host_buf_ptr;

	u32 error;
	u32 irq_count;
	int read_index;
	int avail;
	int host_buf_mem_type;

	char *name;
};

struct wm_adsp_compr {
	struct list_head list;
	struct wm_adsp *dsp;
	struct wm_adsp_compr_buf *buf;

	struct snd_compr_stream *stream;
	struct snd_compressed_buffer size;

	u32 *raw_buf;
	unsigned int copied_total;

	unsigned int sample_rate;

	const char *name;
};

#define WM_ADSP_MIN_FRAGMENTS          1
#define WM_ADSP_MAX_FRAGMENTS          256
#define WM_ADSP_MIN_FRAGMENT_SIZE      (16 * CS_DSP_DATA_WORD_SIZE)
#define WM_ADSP_MAX_FRAGMENT_SIZE      (4096 * CS_DSP_DATA_WORD_SIZE)

#define WM_ADSP_ALG_XM_STRUCT_MAGIC    0x49aec7

#define HOST_BUFFER_FIELD(field) \
	(offsetof(struct wm_adsp_buffer, field) / sizeof(__be32))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ALG_XM_FIELD(field) \
	(offsetof(struct wm_adsp_alg_xm_struct, field) / sizeof(__be32))

<<<<<<< HEAD
static const struct {
	int words_per_group;
	int samps_per_group;
} wm_adsp_sample_group[4] = {
	[2] = {
		.words_per_group = 2,
		.samps_per_group = 3,
	},
};

struct wm_adsp_buffer_region_def ez2control_regions[] = {
	{
		.mem_type = WMFW_ADSP2_XM,
		.base_offset = HOST_BUFFER_FIELD(X_buf_base),
		.size_offset = HOST_BUFFER_FIELD(X_buf_size),
	},
	{
		.mem_type = WMFW_ADSP2_XM,
		.base_offset = HOST_BUFFER_FIELD(X_buf_base2),
		.size_offset = HOST_BUFFER_FIELD(X_buf_size2),
	},
	{
		.mem_type = WMFW_ADSP2_YM,
		.base_offset = HOST_BUFFER_FIELD(Y_buf_base),
		.size_offset = HOST_BUFFER_FIELD(Y_buf_size),
	},
};

static const struct wm_adsp_fw_caps ez2control_caps[] = {
	{
		.id = SND_AUDIOCODEC_PCM,
		.desc = {
			.max_ch = 1,
			.sample_rates = SNDRV_PCM_RATE_16000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
		.num_host_regions = ARRAY_SIZE(ez2control_regions),
		.host_region_defs = ez2control_regions,
=======
#define HOST_BUF_COEFF_SUPPORTED_COMPAT_VER	1

#define HOST_BUF_COEFF_COMPAT_VER_MASK		0xFF00
#define HOST_BUF_COEFF_COMPAT_VER_SHIFT		8

static int wm_adsp_buffer_init(struct wm_adsp *dsp);
static int wm_adsp_buffer_free(struct wm_adsp *dsp);

struct wm_adsp_buffer_region {
	unsigned int offset;
	unsigned int cumulative_size;
	unsigned int mem_type;
	unsigned int base_addr;
};

struct wm_adsp_buffer_region_def {
	unsigned int mem_type;
	unsigned int base_offset;
	unsigned int size_offset;
};

static const struct wm_adsp_buffer_region_def default_regions[] = {
	{
		.mem_type = WMFW_ADSP2_XM,
		.base_offset = HOST_BUFFER_FIELD(buf1_base),
		.size_offset = HOST_BUFFER_FIELD(buf1_size),
	},
	{
		.mem_type = WMFW_ADSP2_XM,
		.base_offset = HOST_BUFFER_FIELD(buf2_base),
		.size_offset = HOST_BUFFER_FIELD(buf1_buf2_size),
	},
	{
		.mem_type = WMFW_ADSP2_YM,
		.base_offset = HOST_BUFFER_FIELD(buf3_base),
		.size_offset = HOST_BUFFER_FIELD(buf_total_size),
	},
};

struct wm_adsp_fw_caps {
	u32 id;
	struct snd_codec_desc desc;
	int num_regions;
	const struct wm_adsp_buffer_region_def *region_defs;
};

static const struct wm_adsp_fw_caps ctrl_caps[] = {
	{
		.id = SND_AUDIOCODEC_BESPOKE,
		.desc = {
			.max_ch = 8,
			.sample_rates = { 16000 },
			.num_sample_rates = 1,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
		.num_regions = ARRAY_SIZE(default_regions),
		.region_defs = default_regions,
	},
};

static const struct wm_adsp_fw_caps trace_caps[] = {
	{
		.id = SND_AUDIOCODEC_BESPOKE,
		.desc = {
			.max_ch = 8,
			.sample_rates = {
				4000, 8000, 11025, 12000, 16000, 22050,
				24000, 32000, 44100, 48000, 64000, 88200,
				96000, 176400, 192000
			},
			.num_sample_rates = 15,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
		.num_regions = ARRAY_SIZE(default_regions),
		.region_defs = default_regions,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static const struct {
	const char *file;
	int compr_direction;
	int num_caps;
	const struct wm_adsp_fw_caps *caps;
<<<<<<< HEAD
} wm_adsp_fw[WM_ADSP_NUM_FW] = {
	[WM_ADSP_FW_MBC_VSS] =    { .file = "mbc-vss" },
	[WM_ADSP_FW_TX] =         { .file = "tx" },
	[WM_ADSP_FW_TX_SPK] =     { .file = "tx-spk" },
	[WM_ADSP_FW_RX_ANC] =     { .file = "rx-anc" },
	[WM_ADSP_FW_EZ2CONTROL] = {
		.file = "ez2-control",
		.compr_direction = SND_COMPRESS_CAPTURE,
		.num_caps = ARRAY_SIZE(ez2control_caps),
		.caps = ez2control_caps,
	},
};

struct wm_coeff_ctl_ops {
	int (*xget)(struct snd_kcontrol *kcontrol,
		    struct snd_ctl_elem_value *ucontrol);
	int (*xput)(struct snd_kcontrol *kcontrol,
		    struct snd_ctl_elem_value *ucontrol);
	int (*xinfo)(struct snd_kcontrol *kcontrol,
		     struct snd_ctl_elem_info *uinfo);
};

struct wm_coeff {
	struct device *dev;
	struct list_head ctl_list;
	struct regmap *regmap;
=======
	bool voice_trigger;
} wm_adsp_fw[WM_ADSP_NUM_FW] = {
	[WM_ADSP_FW_MBC_VSS] =  { .file = "mbc-vss" },
	[WM_ADSP_FW_HIFI] =     { .file = "hifi" },
	[WM_ADSP_FW_TX] =       { .file = "tx" },
	[WM_ADSP_FW_TX_SPK] =   { .file = "tx-spk" },
	[WM_ADSP_FW_RX] =       { .file = "rx" },
	[WM_ADSP_FW_RX_ANC] =   { .file = "rx-anc" },
	[WM_ADSP_FW_CTRL] =     {
		.file = "ctrl",
		.compr_direction = SND_COMPRESS_CAPTURE,
		.num_caps = ARRAY_SIZE(ctrl_caps),
		.caps = ctrl_caps,
		.voice_trigger = true,
	},
	[WM_ADSP_FW_ASR] =      { .file = "asr" },
	[WM_ADSP_FW_TRACE] =    {
		.file = "trace",
		.compr_direction = SND_COMPRESS_CAPTURE,
		.num_caps = ARRAY_SIZE(trace_caps),
		.caps = trace_caps,
	},
	[WM_ADSP_FW_SPK_PROT] = {
		.file = "spk-prot",
		.compr_direction = SND_COMPRESS_CAPTURE,
		.num_caps = ARRAY_SIZE(trace_caps),
		.caps = trace_caps,
	},
	[WM_ADSP_FW_SPK_CALI] = { .file = "spk-cali" },
	[WM_ADSP_FW_SPK_DIAG] = { .file = "spk-diag" },
	[WM_ADSP_FW_MISC] =     { .file = "misc" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct wm_coeff_ctl {
	const char *name;
<<<<<<< HEAD
	struct snd_card *card;
	struct wm_adsp_alg_region region;
	struct wm_coeff_ctl_ops ops;
	struct wm_adsp *adsp;
	void *private;
	unsigned int enabled:1;
	struct list_head list;
	void *cache;
	size_t len;
	unsigned int set:1;
	struct snd_kcontrol *kcontrol;
};

static int wm_adsp_fw_get(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	struct wm_adsp *adsp = snd_soc_codec_get_drvdata(codec);

	ucontrol->value.integer.value[0] = adsp[e->shift_l].fw;

	return 0;
}

static int wm_adsp_fw_put(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	struct wm_adsp *adsp = snd_soc_codec_get_drvdata(codec);

	if (ucontrol->value.integer.value[0] == adsp[e->shift_l].fw)
		return 0;

	if (ucontrol->value.integer.value[0] >= WM_ADSP_NUM_FW)
		return -EINVAL;

	if (adsp[e->shift_l].running)
		return -EBUSY;

	adsp[e->shift_l].fw = ucontrol->value.integer.value[0];

	return 0;
}

static const struct soc_enum wm_adsp_fw_enum[] = {
=======
	struct cs_dsp_coeff_ctl *cs_ctl;
	struct soc_bytes_ext bytes_ext;
	struct work_struct work;
};

int wm_adsp_fw_get(struct snd_kcontrol *kcontrol,
		   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	struct wm_adsp *dsp = snd_soc_component_get_drvdata(component);

	ucontrol->value.enumerated.item[0] = dsp[e->shift_l].fw;

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_fw_get);

int wm_adsp_fw_put(struct snd_kcontrol *kcontrol,
		   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	struct wm_adsp *dsp = snd_soc_component_get_drvdata(component);
	int ret = 1;

	if (ucontrol->value.enumerated.item[0] == dsp[e->shift_l].fw)
		return 0;

	if (ucontrol->value.enumerated.item[0] >= WM_ADSP_NUM_FW)
		return -EINVAL;

	mutex_lock(&dsp[e->shift_l].cs_dsp.pwr_lock);

	if (dsp[e->shift_l].cs_dsp.booted || !list_empty(&dsp[e->shift_l].compr_list))
		ret = -EBUSY;
	else
		dsp[e->shift_l].fw = ucontrol->value.enumerated.item[0];

	mutex_unlock(&dsp[e->shift_l].cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_fw_put);

const struct soc_enum wm_adsp_fw_enum[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SOC_ENUM_SINGLE(0, 0, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
	SOC_ENUM_SINGLE(0, 1, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
	SOC_ENUM_SINGLE(0, 2, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
	SOC_ENUM_SINGLE(0, 3, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
<<<<<<< HEAD
};

static const struct soc_enum wm_adsp_rate_enum[] = {
	SOC_VALUE_ENUM_SINGLE(ARIZONA_DSP1_CONTROL_1,
			      ARIZONA_DSP1_RATE_SHIFT, 0xf,
			      ARIZONA_RATE_ENUM_SIZE,
			      arizona_rate_text, arizona_rate_val),
	SOC_VALUE_ENUM_SINGLE(ARIZONA_DSP2_CONTROL_1,
			      ARIZONA_DSP1_RATE_SHIFT, 0xf,
			      ARIZONA_RATE_ENUM_SIZE,
			      arizona_rate_text, arizona_rate_val),
	SOC_VALUE_ENUM_SINGLE(ARIZONA_DSP3_CONTROL_1,
			      ARIZONA_DSP1_RATE_SHIFT, 0xf,
			      ARIZONA_RATE_ENUM_SIZE,
			      arizona_rate_text, arizona_rate_val),
	SOC_VALUE_ENUM_SINGLE(ARIZONA_DSP4_CONTROL_1,
			      ARIZONA_DSP1_RATE_SHIFT, 0xf,
			      ARIZONA_RATE_ENUM_SIZE,
			      arizona_rate_text, arizona_rate_val),
};

const struct snd_kcontrol_new wm_adsp_fw_controls[] = {
	SOC_ENUM_EXT("DSP1 Firmware", wm_adsp_fw_enum[0],
		     wm_adsp_fw_get, wm_adsp_fw_put),
	SOC_ENUM("DSP1 Rate", wm_adsp_rate_enum[0]),
	SOC_ENUM_EXT("DSP2 Firmware", wm_adsp_fw_enum[1],
		     wm_adsp_fw_get, wm_adsp_fw_put),
	SOC_ENUM("DSP2 Rate", wm_adsp_rate_enum[1]),
	SOC_ENUM_EXT("DSP3 Firmware", wm_adsp_fw_enum[2],
		     wm_adsp_fw_get, wm_adsp_fw_put),
	SOC_ENUM("DSP3 Rate", wm_adsp_rate_enum[2]),
	SOC_ENUM_EXT("DSP4 Firmware", wm_adsp_fw_enum[3],
		     wm_adsp_fw_get, wm_adsp_fw_put),
	SOC_ENUM("DSP4 Rate", wm_adsp_rate_enum[3]),
};
EXPORT_SYMBOL_GPL(wm_adsp_fw_controls);

static struct wm_adsp_region const *wm_adsp_find_region(struct wm_adsp *dsp,
							int type)
{
	int i;

	for (i = 0; i < dsp->num_mems; i++)
		if (dsp->mem[i].type == type)
			return &dsp->mem[i];

	return NULL;
}

static unsigned int wm_adsp_region_to_reg(struct wm_adsp_region const *region,
					  unsigned int offset)
{
	switch (region->type) {
	case WMFW_ADSP1_PM:
		return region->base + (offset * 3);
	case WMFW_ADSP1_DM:
		return region->base + (offset * 2);
	case WMFW_ADSP2_XM:
		return region->base + (offset * 2);
	case WMFW_ADSP2_YM:
		return region->base + (offset * 2);
	case WMFW_ADSP1_ZM:
		return region->base + (offset * 2);
	default:
		WARN_ON(NULL != "Unknown memory region type");
		return offset;
	}
}

static int wm_coeff_info(struct snd_kcontrol *kcontrol,
			 struct snd_ctl_elem_info *uinfo)
{
	struct wm_coeff_ctl *ctl = (struct wm_coeff_ctl *)kcontrol->private_value;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_BYTES;
	uinfo->count = ctl->len;
	return 0;
}

static int wm_coeff_write_control(struct snd_kcontrol *kcontrol,
				  const void *buf, size_t len)
{
	struct wm_coeff *wm_coeff= snd_kcontrol_chip(kcontrol);
	struct wm_coeff_ctl *ctl = (struct wm_coeff_ctl *)kcontrol->private_value;
	struct wm_adsp_alg_region *region = &ctl->region;
	const struct wm_adsp_region *mem;
	struct wm_adsp *adsp = ctl->adsp;
	void *scratch;
	int ret;
	unsigned int reg;

	mem = wm_adsp_find_region(adsp, region->type);
	if (!mem) {
		adsp_err(adsp, "No base for region %x\n",
			 region->type);
		return -EINVAL;
	}

	reg = ctl->region.base;
	reg = wm_adsp_region_to_reg(mem, reg);

	scratch = kmemdup(buf, ctl->len, GFP_KERNEL | GFP_DMA);
	if (!scratch)
		return -ENOMEM;

	ret = regmap_raw_write(wm_coeff->regmap, reg, scratch,
			       ctl->len);
	if (ret) {
		adsp_err(adsp, "Failed to write %zu bytes to %x\n",
			 ctl->len, reg);
		kfree(scratch);
		return ret;
	}

	kfree(scratch);

	return 0;
}

static int wm_coeff_put(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct wm_coeff_ctl *ctl = (struct wm_coeff_ctl *)kcontrol->private_value;
	char *p = ucontrol->value.bytes.data;

	memcpy(ctl->cache, p, ctl->len);

	if (!ctl->enabled) {
		ctl->set = 1;
		return 0;
	}

	return wm_coeff_write_control(kcontrol, p, ctl->len);
}

static int wm_coeff_read_control(struct snd_kcontrol *kcontrol,
				 void *buf, size_t len)
{
	struct wm_coeff *wm_coeff= snd_kcontrol_chip(kcontrol);
	struct wm_coeff_ctl *ctl = (struct wm_coeff_ctl *)kcontrol->private_value;
	struct wm_adsp_alg_region *region = &ctl->region;
	const struct wm_adsp_region *mem;
	struct wm_adsp *adsp = ctl->adsp;
	void *scratch;
	int ret;
	unsigned int reg;

	mem = wm_adsp_find_region(adsp, region->type);
	if (!mem) {
		adsp_err(adsp, "No base for region %x\n",
			 region->type);
		return -EINVAL;
	}

	reg = ctl->region.base;
	reg = wm_adsp_region_to_reg(mem, reg);

	scratch = kmalloc(ctl->len, GFP_KERNEL | GFP_DMA);
	if (!scratch)
		return -ENOMEM;

	ret = regmap_raw_read(wm_coeff->regmap, reg, scratch, ctl->len);
	if (ret) {
		adsp_err(adsp, "Failed to read %zu bytes from %x\n",
			 ctl->len, reg);
		kfree(scratch);
		return ret;
	}

	memcpy(buf, scratch, ctl->len);
	kfree(scratch);

	return 0;
}

static int wm_coeff_get(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct wm_coeff_ctl *ctl = (struct wm_coeff_ctl *)kcontrol->private_value;
	char *p = ucontrol->value.bytes.data;

	memcpy(p, ctl->cache, ctl->len);
	return 0;
}

static int wm_coeff_add_kcontrol(struct wm_coeff *wm_coeff,
				 struct wm_coeff_ctl *ctl,
				 const struct snd_kcontrol_new *kctl)
{
	int ret;
	struct snd_kcontrol *kcontrol;

	kcontrol = snd_ctl_new1(kctl, wm_coeff);
	ret = snd_ctl_add(ctl->card, kcontrol);
	if (ret < 0) {
		dev_err(wm_coeff->dev, "Failed to add %s: %d\n",
			kctl->name, ret);
		return ret;
	}
	ctl->kcontrol = kcontrol;
	return 0;
}

struct wmfw_ctl_work {
	struct wm_coeff *wm_coeff;
	struct wm_coeff_ctl *ctl;
	struct work_struct work;
};

static int wmfw_add_ctl(struct wm_coeff *wm_coeff,
			struct wm_coeff_ctl *ctl)
{
	struct snd_kcontrol_new *kcontrol;
	int ret;

	if (!wm_coeff || !ctl || !ctl->name || !ctl->card)
		return -EINVAL;

	kcontrol = kzalloc(sizeof(*kcontrol), GFP_KERNEL);
	if (!kcontrol)
		return -ENOMEM;
	kcontrol->iface = SNDRV_CTL_ELEM_IFACE_MIXER;

	kcontrol->name = ctl->name;
	kcontrol->info = wm_coeff_info;
	kcontrol->get = wm_coeff_get;
	kcontrol->put = wm_coeff_put;
	kcontrol->private_value = (unsigned long)ctl;

	ret = wm_coeff_add_kcontrol(wm_coeff,
				    ctl, kcontrol);
	if (ret < 0)
		goto err_kcontrol;

	kfree(kcontrol);

	list_add(&ctl->list, &wm_coeff->ctl_list);
	return 0;

err_kcontrol:
	kfree(kcontrol);
	return ret;
}

static int wm_adsp_load(struct wm_adsp *dsp)
{
	const struct firmware *firmware;
	struct regmap *regmap = dsp->regmap;
	unsigned int pos = 0;
	const struct wmfw_header *header;
	const struct wmfw_adsp1_sizes *adsp1_sizes;
	const struct wmfw_adsp2_sizes *adsp2_sizes;
	const struct wmfw_footer *footer;
	const struct wmfw_region *region;
	const struct wm_adsp_region *mem;
	const char *region_name;
	char *file, *text;
	void *buf;
	unsigned int reg;
	int regions = 0;
	int ret, offset, type, sizes;

	file = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (file == NULL)
		return -ENOMEM;

	snprintf(file, PAGE_SIZE, "%s-dsp%d-%s.wmfw", dsp->part, dsp->num,
		 wm_adsp_fw[dsp->fw].file);
	file[PAGE_SIZE - 1] = '\0';

	ret = request_firmware(&firmware, file, dsp->dev);
	if (ret != 0) {
		adsp_err(dsp, "Failed to request '%s'\n", file);
		goto out;
	}
	ret = -EINVAL;

	pos = sizeof(*header) + sizeof(*adsp1_sizes) + sizeof(*footer);
	if (pos >= firmware->size) {
		adsp_err(dsp, "%s: file too short, %zu bytes\n",
			 file, firmware->size);
		goto out_fw;
	}

	header = (void*)&firmware->data[0];

	if (memcmp(&header->magic[0], "WMFW", 4) != 0) {
		adsp_err(dsp, "%s: invalid magic\n", file);
		goto out_fw;
	}

	if (header->ver != 0) {
		adsp_err(dsp, "%s: unknown file format %d\n",
			 file, header->ver);
		goto out_fw;
	}

	if (header->core != dsp->type) {
		adsp_err(dsp, "%s: invalid core %d != %d\n",
			 file, header->core, dsp->type);
		goto out_fw;
	}

	switch (dsp->type) {
	case WMFW_ADSP1:
		pos = sizeof(*header) + sizeof(*adsp1_sizes) + sizeof(*footer);
		adsp1_sizes = (void *)&(header[1]);
		footer = (void *)&(adsp1_sizes[1]);
		sizes = sizeof(*adsp1_sizes);

		adsp_dbg(dsp, "%s: %d DM, %d PM, %d ZM\n",
			 file, le32_to_cpu(adsp1_sizes->dm),
			 le32_to_cpu(adsp1_sizes->pm),
			 le32_to_cpu(adsp1_sizes->zm));
		break;

	case WMFW_ADSP2:
		pos = sizeof(*header) + sizeof(*adsp2_sizes) + sizeof(*footer);
		adsp2_sizes = (void *)&(header[1]);
		footer = (void *)&(adsp2_sizes[1]);
		sizes = sizeof(*adsp2_sizes);

		adsp_dbg(dsp, "%s: %d XM, %d YM %d PM, %d ZM\n",
			 file, le32_to_cpu(adsp2_sizes->xm),
			 le32_to_cpu(adsp2_sizes->ym),
			 le32_to_cpu(adsp2_sizes->pm),
			 le32_to_cpu(adsp2_sizes->zm));
		break;

	default:
		BUG_ON(NULL == "Unknown DSP type");
		goto out_fw;
	}

	if (le32_to_cpu(header->len) != sizeof(*header) +
	    sizes + sizeof(*footer)) {
		adsp_err(dsp, "%s: unexpected header length %d\n",
			 file, le32_to_cpu(header->len));
		goto out_fw;
	}

	adsp_dbg(dsp, "%s: timestamp %llu\n", file,
		 le64_to_cpu(footer->timestamp));

	while (pos < firmware->size &&
	       pos - firmware->size > sizeof(*region)) {
		region = (void *)&(firmware->data[pos]);
		region_name = "Unknown";
		reg = 0;
		text = NULL;
		offset = le32_to_cpu(region->offset) & 0xffffff;
		type = be32_to_cpu(region->type) & 0xff;
		mem = wm_adsp_find_region(dsp, type);

		switch (type) {
		case WMFW_NAME_TEXT:
			region_name = "Firmware name";
			text = kzalloc(le32_to_cpu(region->len) + 1,
				       GFP_KERNEL);
			break;
		case WMFW_INFO_TEXT:
			region_name = "Information";
			text = kzalloc(le32_to_cpu(region->len) + 1,
				       GFP_KERNEL);
			break;
		case WMFW_ABSOLUTE:
			region_name = "Absolute";
			reg = offset;
			break;
		case WMFW_ADSP1_PM:
			BUG_ON(!mem);
			region_name = "PM";
			reg = wm_adsp_region_to_reg(mem, offset);
			break;
		case WMFW_ADSP1_DM:
			BUG_ON(!mem);
			region_name = "DM";
			reg = wm_adsp_region_to_reg(mem, offset);
			break;
		case WMFW_ADSP2_XM:
			BUG_ON(!mem);
			region_name = "XM";
			reg = wm_adsp_region_to_reg(mem, offset);
			break;
		case WMFW_ADSP2_YM:
			BUG_ON(!mem);
			region_name = "YM";
			reg = wm_adsp_region_to_reg(mem, offset);
			break;
		case WMFW_ADSP1_ZM:
			BUG_ON(!mem);
			region_name = "ZM";
			reg = wm_adsp_region_to_reg(mem, offset);
			break;
		default:
			adsp_warn(dsp,
				  "%s.%d: Unknown region type %x at %d(%x)\n",
				  file, regions, type, pos, pos);
			break;
		}

		adsp_dbg(dsp, "%s.%d: %d bytes at %d in %s\n", file,
			 regions, le32_to_cpu(region->len), offset,
			 region_name);

		if (text) {
			memcpy(text, region->data, le32_to_cpu(region->len));
			adsp_info(dsp, "%s: %s\n", file, text);
			kfree(text);
		}

		if (reg) {
			buf = kmemdup(region->data, le32_to_cpu(region->len),
				      GFP_KERNEL | GFP_DMA);
			if (!buf) {
				adsp_err(dsp, "Out of memory\n");
				return -ENOMEM;
			}

			ret = regmap_raw_write(regmap, reg, buf,
					       le32_to_cpu(region->len));

			kfree(buf);

			if (ret != 0) {
				adsp_err(dsp,
					"%s.%d: Failed to write %d bytes at %d in %s: %d\n",
					file, regions,
					le32_to_cpu(region->len), offset,
					region_name, ret);
				goto out_fw;
			}
		}

		pos += le32_to_cpu(region->len) + sizeof(*region);
		regions++;
	}

	if (pos > firmware->size)
		adsp_warn(dsp, "%s.%d: %zu bytes at end of file\n",
			  file, regions, pos - firmware->size);

out_fw:
	release_firmware(firmware);
out:
	kfree(file);
=======
	SOC_ENUM_SINGLE(0, 4, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
	SOC_ENUM_SINGLE(0, 5, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
	SOC_ENUM_SINGLE(0, 6, ARRAY_SIZE(wm_adsp_fw_text), wm_adsp_fw_text),
};
EXPORT_SYMBOL_GPL(wm_adsp_fw_enum);

static inline struct wm_coeff_ctl *bytes_ext_to_ctl(struct soc_bytes_ext *ext)
{
	return container_of(ext, struct wm_coeff_ctl, bytes_ext);
}

static int wm_coeff_info(struct snd_kcontrol *kctl,
			 struct snd_ctl_elem_info *uinfo)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;

	switch (cs_ctl->type) {
	case WMFW_CTL_TYPE_ACKED:
		uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
		uinfo->value.integer.min = CS_DSP_ACKED_CTL_MIN_VALUE;
		uinfo->value.integer.max = CS_DSP_ACKED_CTL_MAX_VALUE;
		uinfo->value.integer.step = 1;
		uinfo->count = 1;
		break;
	default:
		uinfo->type = SNDRV_CTL_ELEM_TYPE_BYTES;
		uinfo->count = cs_ctl->len;
		break;
	}

	return 0;
}

static int wm_coeff_put(struct snd_kcontrol *kctl,
			struct snd_ctl_elem_value *ucontrol)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	char *p = ucontrol->value.bytes.data;
	int ret = 0;

	mutex_lock(&cs_ctl->dsp->pwr_lock);
	ret = cs_dsp_coeff_write_ctrl(cs_ctl, 0, p, cs_ctl->len);
	mutex_unlock(&cs_ctl->dsp->pwr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static int wm_coeff_init_control_caches(struct wm_coeff *wm_coeff)
{
	struct wm_coeff_ctl *ctl;
	int ret;

	list_for_each_entry(ctl, &wm_coeff->ctl_list,
			    list) {
		if (!ctl->enabled || ctl->set)
			continue;
		ret = wm_coeff_read_control(ctl->kcontrol,
					    ctl->cache,
					    ctl->len);
		if (ret < 0)
			return ret;
	}
=======
static int wm_coeff_tlv_put(struct snd_kcontrol *kctl,
			    const unsigned int __user *bytes, unsigned int size)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	void *scratch;
	int ret = 0;

	scratch = vmalloc(size);
	if (!scratch)
		return -ENOMEM;

	if (copy_from_user(scratch, bytes, size)) {
		ret = -EFAULT;
	} else {
		mutex_lock(&cs_ctl->dsp->pwr_lock);
		ret = cs_dsp_coeff_write_ctrl(cs_ctl, 0, scratch, size);
		mutex_unlock(&cs_ctl->dsp->pwr_lock);
	}
	vfree(scratch);

	return ret;
}

static int wm_coeff_put_acked(struct snd_kcontrol *kctl,
			      struct snd_ctl_elem_value *ucontrol)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	unsigned int val = ucontrol->value.integer.value[0];
	int ret;

	if (val == 0)
		return 0;	/* 0 means no event */

	mutex_lock(&cs_ctl->dsp->pwr_lock);

	if (cs_ctl->enabled)
		ret = cs_dsp_coeff_write_acked_control(cs_ctl, val);
	else
		ret = -EPERM;

	mutex_unlock(&cs_ctl->dsp->pwr_lock);

	if (ret < 0)
		return ret;

	return 1;
}

static int wm_coeff_get(struct snd_kcontrol *kctl,
			struct snd_ctl_elem_value *ucontrol)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	char *p = ucontrol->value.bytes.data;
	int ret;

	mutex_lock(&cs_ctl->dsp->pwr_lock);
	ret = cs_dsp_coeff_read_ctrl(cs_ctl, 0, p, cs_ctl->len);
	mutex_unlock(&cs_ctl->dsp->pwr_lock);

	return ret;
}

static int wm_coeff_tlv_get(struct snd_kcontrol *kctl,
			    unsigned int __user *bytes, unsigned int size)
{
	struct soc_bytes_ext *bytes_ext =
		(struct soc_bytes_ext *)kctl->private_value;
	struct wm_coeff_ctl *ctl = bytes_ext_to_ctl(bytes_ext);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	int ret = 0;

	mutex_lock(&cs_ctl->dsp->pwr_lock);

	ret = cs_dsp_coeff_read_ctrl(cs_ctl, 0, cs_ctl->cache, size);

	if (!ret && copy_to_user(bytes, cs_ctl->cache, size))
		ret = -EFAULT;

	mutex_unlock(&cs_ctl->dsp->pwr_lock);

	return ret;
}

static int wm_coeff_get_acked(struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_value *ucontrol)
{
	/*
	 * Although it's not useful to read an acked control, we must satisfy
	 * user-side assumptions that all controls are readable and that a
	 * write of the same value should be filtered out (it's valid to send
	 * the same event number again to the firmware). We therefore return 0,
	 * meaning "no event" so valid event numbers will always be a change
	 */
	ucontrol->value.integer.value[0] = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int wm_coeff_sync_controls(struct wm_coeff *wm_coeff)
{
	struct wm_coeff_ctl *ctl;
	int ret;

	list_for_each_entry(ctl, &wm_coeff->ctl_list,
			    list) {
		if (!ctl->enabled)
			continue;
		if (ctl->set) {
			ret = wm_coeff_write_control(ctl->kcontrol,
						     ctl->cache,
						     ctl->len);
			if (ret < 0)
				return ret;
		}
	}

	return 0;
=======
static unsigned int wmfw_convert_flags(unsigned int in, unsigned int len)
{
	unsigned int out, rd, wr, vol;

	if (len > ADSP_MAX_STD_CTRL_SIZE) {
		rd = SNDRV_CTL_ELEM_ACCESS_TLV_READ;
		wr = SNDRV_CTL_ELEM_ACCESS_TLV_WRITE;
		vol = SNDRV_CTL_ELEM_ACCESS_VOLATILE;

		out = SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK;
	} else {
		rd = SNDRV_CTL_ELEM_ACCESS_READ;
		wr = SNDRV_CTL_ELEM_ACCESS_WRITE;
		vol = SNDRV_CTL_ELEM_ACCESS_VOLATILE;

		out = 0;
	}

	if (in) {
		out |= rd;
		if (in & WMFW_CTL_FLAG_WRITEABLE)
			out |= wr;
		if (in & WMFW_CTL_FLAG_VOLATILE)
			out |= vol;
	} else {
		out |= rd | wr | vol;
	}

	return out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void wm_adsp_ctl_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct wmfw_ctl_work *ctl_work = container_of(work,
						      struct wmfw_ctl_work,
						      work);

	wmfw_add_ctl(ctl_work->wm_coeff, ctl_work->ctl);
	kfree(ctl_work);
}

static int wm_adsp_create_control(struct snd_soc_codec *codec,
				  const struct wm_adsp_alg_region *region)

{
	struct wm_adsp *dsp = snd_soc_codec_get_drvdata(codec);
	struct wm_coeff_ctl *ctl;
	struct wmfw_ctl_work *ctl_work;
	char *name;
	char *region_name;
	int ret;

	name = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	switch (region->type) {
	case WMFW_ADSP1_PM:
		region_name = "PM";
		break;
	case WMFW_ADSP1_DM:
		region_name = "DM";
		break;
	case WMFW_ADSP2_XM:
		region_name = "XM";
		break;
	case WMFW_ADSP2_YM:
		region_name = "YM";
		break;
	case WMFW_ADSP1_ZM:
		region_name = "ZM";
		break;
	default:
		return -EINVAL;
	}

	snprintf(name, PAGE_SIZE, "DSP%d %s %x",
		 dsp->num, region_name, region->alg);

	list_for_each_entry(ctl, &dsp->wm_coeff->ctl_list,
			    list) {
		if (!strcmp(ctl->name, name)) {
			if (!ctl->enabled)
				ctl->enabled = 1;
			return 0;
		}
	}

	ctl = kzalloc(sizeof(*ctl), GFP_KERNEL);
	if (!ctl) {
		ret = -ENOMEM;
		goto err_name;
	}
	ctl->region = *region;
=======
	struct wm_coeff_ctl *ctl = container_of(work,
						struct wm_coeff_ctl,
						work);
	struct cs_dsp_coeff_ctl *cs_ctl = ctl->cs_ctl;
	struct wm_adsp *dsp = container_of(cs_ctl->dsp,
					   struct wm_adsp,
					   cs_dsp);
	struct snd_kcontrol_new *kcontrol;

	kcontrol = kzalloc(sizeof(*kcontrol), GFP_KERNEL);
	if (!kcontrol)
		return;

	kcontrol->name = ctl->name;
	kcontrol->info = wm_coeff_info;
	kcontrol->iface = SNDRV_CTL_ELEM_IFACE_MIXER;
	kcontrol->tlv.c = snd_soc_bytes_tlv_callback;
	kcontrol->private_value = (unsigned long)&ctl->bytes_ext;
	kcontrol->access = wmfw_convert_flags(cs_ctl->flags, cs_ctl->len);

	switch (cs_ctl->type) {
	case WMFW_CTL_TYPE_ACKED:
		kcontrol->get = wm_coeff_get_acked;
		kcontrol->put = wm_coeff_put_acked;
		break;
	default:
		if (kcontrol->access & SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK) {
			ctl->bytes_ext.max = cs_ctl->len;
			ctl->bytes_ext.get = wm_coeff_tlv_get;
			ctl->bytes_ext.put = wm_coeff_tlv_put;
		} else {
			kcontrol->get = wm_coeff_get;
			kcontrol->put = wm_coeff_put;
		}
		break;
	}

	snd_soc_add_component_controls(dsp->component, kcontrol, 1);

	kfree(kcontrol);
}

static int wm_adsp_control_add(struct cs_dsp_coeff_ctl *cs_ctl)
{
	struct wm_adsp *dsp = container_of(cs_ctl->dsp, struct wm_adsp, cs_dsp);
	struct cs_dsp *cs_dsp = &dsp->cs_dsp;
	struct wm_coeff_ctl *ctl;
	char name[SNDRV_CTL_ELEM_ID_NAME_MAXLEN];
	const char *region_name;
	int ret;

	if (cs_ctl->flags & WMFW_CTL_FLAG_SYS)
		return 0;

	region_name = cs_dsp_mem_region_name(cs_ctl->alg_region.type);
	if (!region_name) {
		adsp_err(dsp, "Unknown region type: %d\n", cs_ctl->alg_region.type);
		return -EINVAL;
	}

	switch (cs_dsp->fw_ver) {
	case 0:
	case 1:
		ret = scnprintf(name, SNDRV_CTL_ELEM_ID_NAME_MAXLEN,
				"%s %s %x", cs_dsp->name, region_name,
				cs_ctl->alg_region.alg);
		break;
	case 2:
		ret = scnprintf(name, SNDRV_CTL_ELEM_ID_NAME_MAXLEN,
				"%s%c %.12s %x", cs_dsp->name, *region_name,
				wm_adsp_fw_text[dsp->fw], cs_ctl->alg_region.alg);
		break;
	default:
		ret = scnprintf(name, SNDRV_CTL_ELEM_ID_NAME_MAXLEN,
				"%s %.12s %x", cs_dsp->name,
				wm_adsp_fw_text[dsp->fw], cs_ctl->alg_region.alg);
		break;
	}

	if (cs_ctl->subname) {
		int avail = SNDRV_CTL_ELEM_ID_NAME_MAXLEN - ret - 2;
		int skip = 0;

		if (dsp->component->name_prefix)
			avail -= strlen(dsp->component->name_prefix) + 1;

		/* Truncate the subname from the start if it is too long */
		if (cs_ctl->subname_len > avail)
			skip = cs_ctl->subname_len - avail;

		snprintf(name + ret, SNDRV_CTL_ELEM_ID_NAME_MAXLEN - ret,
			 " %.*s", cs_ctl->subname_len - skip, cs_ctl->subname + skip);
	}

	ctl = kzalloc(sizeof(*ctl), GFP_KERNEL);
	if (!ctl)
		return -ENOMEM;
	ctl->cs_ctl = cs_ctl;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctl->name = kmemdup(name, strlen(name) + 1, GFP_KERNEL);
	if (!ctl->name) {
		ret = -ENOMEM;
		goto err_ctl;
	}
<<<<<<< HEAD
	ctl->enabled = 1;
	ctl->set = 0;
	ctl->ops.xget = wm_coeff_get;
	ctl->ops.xput = wm_coeff_put;
	ctl->card = codec->card->snd_card;
	ctl->adsp = dsp;

	ctl->len = region->len;
	ctl->cache = kzalloc(ctl->len, GFP_KERNEL);
	if (!ctl->cache) {
		ret = -ENOMEM;
		goto err_ctl_name;
	}

	ctl_work = kzalloc(sizeof(*ctl_work), GFP_KERNEL);
	if (!ctl_work) {
		ret = -ENOMEM;
		goto err_ctl_cache;
	}

	ctl_work->wm_coeff = dsp->wm_coeff;
	ctl_work->ctl = ctl;
	INIT_WORK(&ctl_work->work, wm_adsp_ctl_work);
	schedule_work(&ctl_work->work);

	kfree(name);

	return 0;

err_ctl_cache:
	kfree(ctl->cache);
err_ctl_name:
	kfree(ctl->name);
err_ctl:
	kfree(ctl);
err_name:
	kfree(name);
	return ret;
}

static int wm_adsp_setup_algs(struct wm_adsp *dsp, struct snd_soc_codec *codec)
{
	struct regmap *regmap = dsp->regmap;
	struct wmfw_adsp1_id_hdr adsp1_id;
	struct wmfw_adsp2_id_hdr adsp2_id;
	struct wmfw_adsp1_alg_hdr *adsp1_alg;
	struct wmfw_adsp2_alg_hdr *adsp2_alg;
	void *alg;
	struct wm_adsp_alg_region *region;
	const struct wm_adsp_region *mem;
	unsigned int pos, term;
	size_t algs;
	__be32 val;
	int i, ret;

	switch (dsp->type) {
	case WMFW_ADSP1:
		mem = wm_adsp_find_region(dsp, WMFW_ADSP1_DM);
		break;
	case WMFW_ADSP2:
		mem = wm_adsp_find_region(dsp, WMFW_ADSP2_XM);
		break;
	default:
		mem = NULL;
		break;
	}

	if (mem == NULL) {
		BUG_ON(mem != NULL);
		return -EINVAL;
	}

	switch (dsp->type) {
	case WMFW_ADSP1:
		ret = regmap_raw_read(regmap, mem->base, &adsp1_id,
				      sizeof(adsp1_id));
		if (ret != 0) {
			adsp_err(dsp, "Failed to read algorithm info: %d\n",
				 ret);
			return ret;
		}

		algs = be32_to_cpu(adsp1_id.algs);
		dsp->fw_id = be32_to_cpu(adsp1_id.fw.id);
		adsp_info(dsp, "Firmware: %x v%d.%d.%d, %zu algorithms\n",
			  dsp->fw_id,
			  (be32_to_cpu(adsp1_id.fw.ver) & 0xff0000) >> 16,
			  (be32_to_cpu(adsp1_id.fw.ver) & 0xff00) >> 8,
			  be32_to_cpu(adsp1_id.fw.ver) & 0xff,
			  algs);

		region = kzalloc(sizeof(*region), GFP_KERNEL);
		if (!region)
			return -ENOMEM;
		region->type = WMFW_ADSP1_ZM;
		region->alg = be32_to_cpu(adsp1_id.fw.id);
		region->base = be32_to_cpu(adsp1_id.zm);
		list_add_tail(&region->list, &dsp->alg_regions);

		region = kzalloc(sizeof(*region), GFP_KERNEL);
		if (!region)
			return -ENOMEM;
		region->type = WMFW_ADSP1_DM;
		region->alg = be32_to_cpu(adsp1_id.fw.id);
		region->base = be32_to_cpu(adsp1_id.dm);
		list_add_tail(&region->list, &dsp->alg_regions);

		pos = sizeof(adsp1_id) / 2;
		term = pos + ((sizeof(*adsp1_alg) * algs) / 2);
		break;

	case WMFW_ADSP2:
		ret = regmap_raw_read(regmap, mem->base, &adsp2_id,
				      sizeof(adsp2_id));
		if (ret != 0) {
			adsp_err(dsp, "Failed to read algorithm info: %d\n",
				 ret);
			return ret;
		}

		algs = be32_to_cpu(adsp2_id.algs);
		dsp->fw_id = be32_to_cpu(adsp2_id.fw.id);
		adsp_info(dsp, "Firmware: %x v%d.%d.%d, %zu algorithms\n",
			  dsp->fw_id,
			  (be32_to_cpu(adsp2_id.fw.ver) & 0xff0000) >> 16,
			  (be32_to_cpu(adsp2_id.fw.ver) & 0xff00) >> 8,
			  be32_to_cpu(adsp2_id.fw.ver) & 0xff,
			  algs);

		region = kzalloc(sizeof(*region), GFP_KERNEL);
		if (!region)
			return -ENOMEM;
		region->type = WMFW_ADSP2_XM;
		region->alg = be32_to_cpu(adsp2_id.fw.id);
		region->base = be32_to_cpu(adsp2_id.xm);
		list_add_tail(&region->list, &dsp->alg_regions);

		region = kzalloc(sizeof(*region), GFP_KERNEL);
		if (!region)
			return -ENOMEM;
		region->type = WMFW_ADSP2_YM;
		region->alg = be32_to_cpu(adsp2_id.fw.id);
		region->base = be32_to_cpu(adsp2_id.ym);
		list_add_tail(&region->list, &dsp->alg_regions);

		region = kzalloc(sizeof(*region), GFP_KERNEL);
		if (!region)
			return -ENOMEM;
		region->type = WMFW_ADSP2_ZM;
		region->alg = be32_to_cpu(adsp2_id.fw.id);
		region->base = be32_to_cpu(adsp2_id.zm);
		list_add_tail(&region->list, &dsp->alg_regions);

		pos = sizeof(adsp2_id) / 2;
		term = pos + ((sizeof(*adsp2_alg) * algs) / 2);
		break;

	default:
		BUG_ON(NULL == "Unknown DSP type");
		return -EINVAL;
	}

	if (algs == 0) {
		adsp_err(dsp, "No algorithms\n");
		return -EINVAL;
	}

	/* Read the terminator first to validate the length */
	ret = regmap_raw_read(regmap, mem->base + term, &val, sizeof(val));
	if (ret != 0) {
		adsp_err(dsp, "Failed to read algorithm list end: %d\n",
			ret);
		return ret;
	}

	if (be32_to_cpu(val) != 0xbedead)
		adsp_warn(dsp, "Algorithm list end %zx 0x%x != 0xbeadead\n",
			  term, be32_to_cpu(val));

	alg = kzalloc((term - pos) * 2, GFP_KERNEL | GFP_DMA);
	if (!alg)
		return -ENOMEM;

	ret = regmap_raw_read(regmap, mem->base + pos, alg, (term - pos) * 2);
	if (ret != 0) {
		adsp_err(dsp, "Failed to read algorithm list: %d\n",
			ret);
		goto out;
	}

	adsp1_alg = alg;
	adsp2_alg = alg;

	for (i = 0; i < algs; i++) {
		switch (dsp->type) {
		case WMFW_ADSP1:
			adsp_info(dsp, "%d: ID %x v%d.%d.%d DM@%x ZM@%x\n",
				  i, be32_to_cpu(adsp1_alg[i].alg.id),
				  (be32_to_cpu(adsp1_alg[i].alg.ver) & 0xff0000) >> 16,
				  (be32_to_cpu(adsp1_alg[i].alg.ver) & 0xff00) >> 8,
				  be32_to_cpu(adsp1_alg[i].alg.ver) & 0xff,
				  be32_to_cpu(adsp1_alg[i].dm),
				  be32_to_cpu(adsp1_alg[i].zm));

			region = kzalloc(sizeof(*region), GFP_KERNEL);
			if (!region)
				return -ENOMEM;
			region->type = WMFW_ADSP1_DM;
			region->alg = be32_to_cpu(adsp1_alg[i].alg.id);
			region->base = be32_to_cpu(adsp1_alg[i].dm);
			region->len = 0;
			list_add_tail(&region->list, &dsp->alg_regions);
			if (i + 1 < algs) {
				region->len = be32_to_cpu(adsp1_alg[i + 1].dm);
				region->len -= be32_to_cpu(adsp1_alg[i].dm);
				wm_adsp_create_control(codec, region);
			} else {
				adsp_warn(dsp, "Missing length info for region DM with ID %x\n",
					  be32_to_cpu(adsp1_alg[i].alg.id));
			}

			region = kzalloc(sizeof(*region), GFP_KERNEL);
			if (!region)
				return -ENOMEM;
			region->type = WMFW_ADSP1_ZM;
			region->alg = be32_to_cpu(adsp1_alg[i].alg.id);
			region->base = be32_to_cpu(adsp1_alg[i].zm);
			region->len = 0;
			list_add_tail(&region->list, &dsp->alg_regions);
			if (i + 1 < algs) {
				region->len = be32_to_cpu(adsp1_alg[i + 1].zm);
				region->len -= be32_to_cpu(adsp1_alg[i].zm);
				wm_adsp_create_control(codec, region);
			} else {
				adsp_warn(dsp, "Missing length info for region ZM with ID %x\n",
					  be32_to_cpu(adsp1_alg[i].alg.id));
			}
			break;

		case WMFW_ADSP2:
			adsp_info(dsp,
				  "%d: ID %x v%d.%d.%d XM@%x YM@%x ZM@%x\n",
				  i, be32_to_cpu(adsp2_alg[i].alg.id),
				  (be32_to_cpu(adsp2_alg[i].alg.ver) & 0xff0000) >> 16,
				  (be32_to_cpu(adsp2_alg[i].alg.ver) & 0xff00) >> 8,
				  be32_to_cpu(adsp2_alg[i].alg.ver) & 0xff,
				  be32_to_cpu(adsp2_alg[i].xm),
				  be32_to_cpu(adsp2_alg[i].ym),
				  be32_to_cpu(adsp2_alg[i].zm));

			region = kzalloc(sizeof(*region), GFP_KERNEL);
			if (!region)
				return -ENOMEM;
			region->type = WMFW_ADSP2_XM;
			region->alg = be32_to_cpu(adsp2_alg[i].alg.id);
			region->base = be32_to_cpu(adsp2_alg[i].xm);
			region->len = 0;
			list_add_tail(&region->list, &dsp->alg_regions);
			if (i + 1 < algs) {
				region->len = be32_to_cpu(adsp2_alg[i + 1].xm);
				region->len -= be32_to_cpu(adsp2_alg[i].xm);
				wm_adsp_create_control(codec, region);
			} else {
				adsp_warn(dsp, "Missing length info for region XM with ID %x\n",
					  be32_to_cpu(adsp2_alg[i].alg.id));
			}

			region = kzalloc(sizeof(*region), GFP_KERNEL);
			if (!region)
				return -ENOMEM;
			region->type = WMFW_ADSP2_YM;
			region->alg = be32_to_cpu(adsp2_alg[i].alg.id);
			region->base = be32_to_cpu(adsp2_alg[i].ym);
			region->len = 0;
			list_add_tail(&region->list, &dsp->alg_regions);
			if (i + 1 < algs) {
				region->len = be32_to_cpu(adsp2_alg[i + 1].ym);
				region->len -= be32_to_cpu(adsp2_alg[i].ym);
				wm_adsp_create_control(codec, region);
			} else {
				adsp_warn(dsp, "Missing length info for region YM with ID %x\n",
					  be32_to_cpu(adsp2_alg[i].alg.id));
			}

			region = kzalloc(sizeof(*region), GFP_KERNEL);
			if (!region)
				return -ENOMEM;
			region->type = WMFW_ADSP2_ZM;
			region->alg = be32_to_cpu(adsp2_alg[i].alg.id);
			region->base = be32_to_cpu(adsp2_alg[i].zm);
			region->len = 0;
			list_add_tail(&region->list, &dsp->alg_regions);
			if (i + 1 < algs) {
				region->len = be32_to_cpu(adsp2_alg[i + 1].zm);
				region->len -= be32_to_cpu(adsp2_alg[i].zm);
				wm_adsp_create_control(codec, region);
			} else {
				adsp_warn(dsp, "Missing length info for region ZM with ID %x\n",
					  be32_to_cpu(adsp2_alg[i].alg.id));
			}
			break;
		}
	}

out:
	kfree(alg);
	return ret;
}

static int wm_adsp_load_coeff(struct wm_adsp *dsp)
{
	struct regmap *regmap = dsp->regmap;
	struct wmfw_coeff_hdr *hdr;
	struct wmfw_coeff_item *blk;
	const struct firmware *firmware;
	const struct wm_adsp_region *mem;
	struct wm_adsp_alg_region *alg_region;
	const char *region_name;
	int ret, pos, blocks, type, offset, reg;
	char *file;
	void *buf;
	int tmp;

	file = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (file == NULL)
		return -ENOMEM;

	snprintf(file, PAGE_SIZE, "%s-dsp%d-%s.bin", dsp->part, dsp->num,
		 wm_adsp_fw[dsp->fw].file);
	file[PAGE_SIZE - 1] = '\0';

	ret = request_firmware(&firmware, file, dsp->dev);
	if (ret != 0) {
		adsp_warn(dsp, "Failed to request '%s'\n", file);
		ret = 0;
		goto out;
	}
	ret = -EINVAL;

	if (sizeof(*hdr) >= firmware->size) {
		adsp_err(dsp, "%s: file too short, %zu bytes\n",
			file, firmware->size);
		goto out_fw;
	}

	hdr = (void*)&firmware->data[0];
	if (memcmp(hdr->magic, "WMDR", 4) != 0) {
		adsp_err(dsp, "%s: invalid magic\n", file);
		goto out_fw;
	}

	switch (be32_to_cpu(hdr->rev) & 0xff) {
	case 1:
		break;
	default:
		adsp_err(dsp, "%s: Unsupported coefficient file format %d\n",
			 file, be32_to_cpu(hdr->rev) & 0xff);
		ret = -EINVAL;
		goto out_fw;
	}

	adsp_dbg(dsp, "%s: v%d.%d.%d\n", file,
		(le32_to_cpu(hdr->ver) >> 16) & 0xff,
		(le32_to_cpu(hdr->ver) >>  8) & 0xff,
		le32_to_cpu(hdr->ver) & 0xff);

	pos = le32_to_cpu(hdr->len);

	blocks = 0;
	while (pos < firmware->size &&
	       pos - firmware->size > sizeof(*blk)) {
		blk = (void*)(&firmware->data[pos]);

		type = le16_to_cpu(blk->type);
		offset = le16_to_cpu(blk->offset);

		adsp_dbg(dsp, "%s.%d: %x v%d.%d.%d\n",
			 file, blocks, le32_to_cpu(blk->id),
			 (le32_to_cpu(blk->ver) >> 16) & 0xff,
			 (le32_to_cpu(blk->ver) >>  8) & 0xff,
			 le32_to_cpu(blk->ver) & 0xff);
		adsp_dbg(dsp, "%s.%d: %d bytes at 0x%x in %x\n",
			 file, blocks, le32_to_cpu(blk->len), offset, type);

		reg = 0;
		region_name = "Unknown";
		switch (type) {
		case (WMFW_NAME_TEXT << 8):
		case (WMFW_INFO_TEXT << 8):
			break;
		case (WMFW_ABSOLUTE << 8):
			/*
			 * Old files may use this for global
			 * coefficients.
			 */
			if (le32_to_cpu(blk->id) == dsp->fw_id &&
			    offset == 0) {
				region_name = "global coefficients";
				mem = wm_adsp_find_region(dsp, type);
				if (!mem) {
					adsp_err(dsp, "No ZM\n");
					break;
				}
				reg = wm_adsp_region_to_reg(mem, 0);

			} else {
				region_name = "register";
				reg = offset;
			}
			break;

		case WMFW_ADSP1_DM:
		case WMFW_ADSP1_ZM:
		case WMFW_ADSP2_XM:
		case WMFW_ADSP2_YM:
			adsp_dbg(dsp, "%s.%d: %d bytes in %x for %x\n",
				 file, blocks, le32_to_cpu(blk->len),
				 type, le32_to_cpu(blk->id));

			mem = wm_adsp_find_region(dsp, type);
			if (!mem) {
				adsp_err(dsp, "No base for region %x\n", type);
				break;
			}

			reg = 0;
			list_for_each_entry(alg_region,
					    &dsp->alg_regions, list) {
				if (le32_to_cpu(blk->id) == alg_region->alg &&
				    type == alg_region->type) {
					reg = alg_region->base;
					reg = wm_adsp_region_to_reg(mem,
								    reg);
					reg += offset;
				}
			}

			if (reg == 0)
				adsp_err(dsp, "No %x for algorithm %x\n",
					 type, le32_to_cpu(blk->id));
			break;

		default:
			adsp_err(dsp, "Unknown region type %x\n", type);
			break;
		}

		if (reg) {
			buf = kmemdup(blk->data, le32_to_cpu(blk->len),
				      GFP_KERNEL | GFP_DMA);
			if (!buf) {
				adsp_err(dsp, "Out of memory\n");
				return -ENOMEM;
			}

			ret = regmap_raw_write(regmap, reg, buf,
					       le32_to_cpu(blk->len));
			if (ret != 0) {
				adsp_err(dsp,
					"%s.%d: Failed to write to %x in %s\n",
					file, blocks, reg, region_name);
			}

			kfree(buf);
		}

		tmp = le32_to_cpu(blk->len) % 4;
		if (tmp)
			pos += le32_to_cpu(blk->len) + (4 - tmp) + sizeof(*blk);
		else
			pos += le32_to_cpu(blk->len) + sizeof(*blk);

		blocks++;
	}

	if (pos > firmware->size)
		adsp_warn(dsp, "%s.%d: %zu bytes at end of file\n",
			  file, blocks, pos - firmware->size);

out_fw:
	release_firmware(firmware);
out:
	kfree(file);
	return 0;
}

=======

	cs_ctl->priv = ctl;

	INIT_WORK(&ctl->work, wm_adsp_ctl_work);
	schedule_work(&ctl->work);

	return 0;

err_ctl:
	kfree(ctl);

	return ret;
}

static void wm_adsp_control_remove(struct cs_dsp_coeff_ctl *cs_ctl)
{
	struct wm_coeff_ctl *ctl = cs_ctl->priv;

	cancel_work_sync(&ctl->work);

	kfree(ctl->name);
	kfree(ctl);
}

int wm_adsp_write_ctl(struct wm_adsp *dsp, const char *name, int type,
		      unsigned int alg, void *buf, size_t len)
{
	struct cs_dsp_coeff_ctl *cs_ctl;
	struct wm_coeff_ctl *ctl;
	int ret;

	mutex_lock(&dsp->cs_dsp.pwr_lock);
	cs_ctl = cs_dsp_get_ctl(&dsp->cs_dsp, name, type, alg);
	ret = cs_dsp_coeff_write_ctrl(cs_ctl, 0, buf, len);
	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	if (ret < 0)
		return ret;

	if (ret == 0 || (cs_ctl->flags & WMFW_CTL_FLAG_SYS))
		return 0;

	ctl = cs_ctl->priv;

	return snd_soc_component_notify_control(dsp->component, ctl->name);
}
EXPORT_SYMBOL_GPL(wm_adsp_write_ctl);

int wm_adsp_read_ctl(struct wm_adsp *dsp, const char *name, int type,
		     unsigned int alg, void *buf, size_t len)
{
	int ret;

	mutex_lock(&dsp->cs_dsp.pwr_lock);
	ret = cs_dsp_coeff_read_ctrl(cs_dsp_get_ctl(&dsp->cs_dsp, name, type, alg),
				     0, buf, len);
	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_read_ctl);

static void wm_adsp_release_firmware_files(struct wm_adsp *dsp,
					   const struct firmware *wmfw_firmware,
					   char *wmfw_filename,
					   const struct firmware *coeff_firmware,
					   char *coeff_filename)
{
	if (wmfw_firmware)
		release_firmware(wmfw_firmware);
	kfree(wmfw_filename);

	if (coeff_firmware)
		release_firmware(coeff_firmware);
	kfree(coeff_filename);
}

static int wm_adsp_request_firmware_file(struct wm_adsp *dsp,
					 const struct firmware **firmware, char **filename,
					 const char *dir, const char *system_name,
					 const char *asoc_component_prefix,
					 const char *filetype)
{
	struct cs_dsp *cs_dsp = &dsp->cs_dsp;
	const char *fwf;
	char *s, c;
	int ret = 0;

	if (dsp->fwf_name)
		fwf = dsp->fwf_name;
	else
		fwf = dsp->cs_dsp.name;

	if (system_name && asoc_component_prefix)
		*filename = kasprintf(GFP_KERNEL, "%s%s-%s-%s-%s-%s.%s", dir, dsp->part,
				      fwf, wm_adsp_fw[dsp->fw].file, system_name,
				      asoc_component_prefix, filetype);
	else if (system_name)
		*filename = kasprintf(GFP_KERNEL, "%s%s-%s-%s-%s.%s", dir, dsp->part,
				      fwf, wm_adsp_fw[dsp->fw].file, system_name,
				      filetype);
	else
		*filename = kasprintf(GFP_KERNEL, "%s%s-%s-%s.%s", dir, dsp->part, fwf,
				      wm_adsp_fw[dsp->fw].file, filetype);

	if (*filename == NULL)
		return -ENOMEM;

	/*
	 * Make sure that filename is lower-case and any non alpha-numeric
	 * characters except full stop and forward slash are replaced with
	 * hyphens.
	 */
	s = *filename;
	while (*s) {
		c = *s;
		if (isalnum(c))
			*s = tolower(c);
		else if ((c != '.') && (c != '/'))
			*s = '-';
		s++;
	}

	ret = firmware_request_nowarn(firmware, *filename, cs_dsp->dev);
	if (ret != 0) {
		adsp_dbg(dsp, "Failed to request '%s'\n", *filename);
		kfree(*filename);
		*filename = NULL;
	} else {
		adsp_dbg(dsp, "Found '%s'\n", *filename);
	}

	return ret;
}

static const char *cirrus_dir = "cirrus/";
static int wm_adsp_request_firmware_files(struct wm_adsp *dsp,
					  const struct firmware **wmfw_firmware,
					  char **wmfw_filename,
					  const struct firmware **coeff_firmware,
					  char **coeff_filename)
{
	const char *system_name = dsp->system_name;
	const char *asoc_component_prefix = dsp->component->name_prefix;
	int ret = 0;

	if (system_name && asoc_component_prefix) {
		if (!wm_adsp_request_firmware_file(dsp, wmfw_firmware, wmfw_filename,
						   cirrus_dir, system_name,
						   asoc_component_prefix, "wmfw")) {
			wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
						      cirrus_dir, system_name,
						      asoc_component_prefix, "bin");
			return 0;
		}
	}

	if (system_name) {
		if (!wm_adsp_request_firmware_file(dsp, wmfw_firmware, wmfw_filename,
						   cirrus_dir, system_name,
						   NULL, "wmfw")) {
			if (asoc_component_prefix)
				wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
							      cirrus_dir, system_name,
							      asoc_component_prefix, "bin");

			if (!*coeff_firmware)
				wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
							      cirrus_dir, system_name,
							      NULL, "bin");
			return 0;
		}
	}

	/* Check system-specific bin without wmfw before falling back to generic */
	if (dsp->wmfw_optional && system_name) {
		if (asoc_component_prefix)
			wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
						      cirrus_dir, system_name,
						      asoc_component_prefix, "bin");

		if (!*coeff_firmware)
			wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
						      cirrus_dir, system_name,
						      NULL, "bin");

		if (*coeff_firmware)
			return 0;
	}

	/* Check legacy location */
	if (!wm_adsp_request_firmware_file(dsp, wmfw_firmware, wmfw_filename,
					   "", NULL, NULL, "wmfw")) {
		wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
					      "", NULL, NULL, "bin");
		return 0;
	}

	/* Fall back to generic wmfw and optional matching bin */
	ret = wm_adsp_request_firmware_file(dsp, wmfw_firmware, wmfw_filename,
					    cirrus_dir, NULL, NULL, "wmfw");
	if (!ret || dsp->wmfw_optional) {
		wm_adsp_request_firmware_file(dsp, coeff_firmware, coeff_filename,
					      cirrus_dir, NULL, NULL, "bin");
		return 0;
	}

	adsp_err(dsp, "Failed to request firmware <%s>%s-%s-%s<-%s<%s>>.wmfw\n",
		 cirrus_dir, dsp->part,
		 dsp->fwf_name ? dsp->fwf_name : dsp->cs_dsp.name,
		 wm_adsp_fw[dsp->fw].file, system_name, asoc_component_prefix);

	return -ENOENT;
}

static int wm_adsp_common_init(struct wm_adsp *dsp)
{
	INIT_LIST_HEAD(&dsp->compr_list);
	INIT_LIST_HEAD(&dsp->buffer_list);

	return 0;
}

int wm_adsp1_init(struct wm_adsp *dsp)
{
	int ret;

	dsp->cs_dsp.client_ops = &wm_adsp1_client_ops;

	ret = cs_dsp_adsp1_init(&dsp->cs_dsp);
	if (ret)
		return ret;

	return wm_adsp_common_init(dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp1_init);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int wm_adsp1_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol,
		   int event)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = w->codec;
	struct wm_adsp *dsps = snd_soc_codec_get_drvdata(codec);
	struct wm_adsp *dsp = &dsps[w->shift];
	struct wm_coeff_ctl *ctl;
	int ret;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_30,
				   ADSP1_SYS_ENA, ADSP1_SYS_ENA);

		ret = wm_adsp_load(dsp);
		if (ret != 0)
			goto err;

		ret = wm_adsp_setup_algs(dsp, codec);
		if (ret != 0)
			goto err;

		ret = wm_adsp_load_coeff(dsp);
		if (ret != 0)
			goto err;

		/* Initialize caches for enabled and unset controls */
		ret = wm_coeff_init_control_caches(dsp->wm_coeff);
		if (ret != 0)
			goto err;

		/* Sync set controls */
		ret = wm_coeff_sync_controls(dsp->wm_coeff);
		if (ret != 0)
			goto err;

		/* Start the core running */
		regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_30,
				   ADSP1_CORE_ENA | ADSP1_START,
				   ADSP1_CORE_ENA | ADSP1_START);
		break;

	case SND_SOC_DAPM_PRE_PMD:
		/* Halt the core */
		regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_30,
				   ADSP1_CORE_ENA | ADSP1_START, 0);

		regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_19,
				   ADSP1_WDMA_BUFFER_LENGTH_MASK, 0);

		regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_30,
				   ADSP1_SYS_ENA, 0);

		list_for_each_entry(ctl, &dsp->wm_coeff->ctl_list,
				    list) {
			ctl->enabled = 0;
		}
		break;

=======
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct wm_adsp *dsp = &dsps[w->shift];
	int ret = 0;
	char *wmfw_filename = NULL;
	const struct firmware *wmfw_firmware = NULL;
	char *coeff_filename = NULL;
	const struct firmware *coeff_firmware = NULL;

	dsp->component = component;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		ret = wm_adsp_request_firmware_files(dsp,
						     &wmfw_firmware, &wmfw_filename,
						     &coeff_firmware, &coeff_filename);
		if (ret)
			break;

		ret = cs_dsp_adsp1_power_up(&dsp->cs_dsp,
					    wmfw_firmware, wmfw_filename,
					    coeff_firmware, coeff_filename,
					    wm_adsp_fw_text[dsp->fw]);

		wm_adsp_release_firmware_files(dsp,
					       wmfw_firmware, wmfw_filename,
					       coeff_firmware, coeff_filename);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		cs_dsp_adsp1_power_down(&dsp->cs_dsp);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		break;
	}

<<<<<<< HEAD
	return 0;

err:
	regmap_update_bits(dsp->regmap, dsp->base + ADSP1_CONTROL_30,
			   ADSP1_SYS_ENA, 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp1_event);

<<<<<<< HEAD
static int wm_adsp2_ena(struct wm_adsp *dsp)
{
	unsigned int val;
	int ret, count;

	ret = regmap_update_bits(dsp->regmap, dsp->base + ADSP2_CONTROL,
				 ADSP2_SYS_ENA, ADSP2_SYS_ENA);
	if (ret != 0)
		return ret;

	/* Wait for the RAM to start, should be near instantaneous */
	count = 0;
	do {
		ret = regmap_read(dsp->regmap, dsp->base + ADSP2_STATUS1,
				  &val);
		if (ret != 0)
			return ret;
	} while (!(val & ADSP2_RAM_RDY) && ++count < 10);

	if (!(val & ADSP2_RAM_RDY)) {
		adsp_err(dsp, "Failed to start DSP RAM\n");
		return -EBUSY;
	}

	adsp_dbg(dsp, "RAM ready after %d polls\n", count);

	return 0;
}

int wm_adsp2_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct wm_adsp *dsps = snd_soc_codec_get_drvdata(codec);
	struct wm_adsp *dsp = &dsps[w->shift];
	struct wm_adsp_alg_region *alg_region;
	struct wm_coeff_ctl *ctl;
	unsigned int val;
	int ret;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/*
		 * For simplicity set the DSP clock rate to be the
		 * SYSCLK rate rather than making it configurable.
		 */
		ret = regmap_read(dsp->regmap, ARIZONA_SYSTEM_CLOCK_1, &val);
		if (ret != 0) {
			adsp_err(dsp, "Failed to read SYSCLK state: %d\n",
				 ret);
			return ret;
		}
		val = (val & ARIZONA_SYSCLK_FREQ_MASK)
			>> ARIZONA_SYSCLK_FREQ_SHIFT;

		ret = regmap_update_bits(dsp->regmap,
					 dsp->base + ADSP2_CLOCKING,
					 ADSP2_CLK_SEL_MASK, val);
		if (ret != 0) {
			adsp_err(dsp, "Failed to set clock rate: %d\n",
				 ret);
			return ret;
		}

		if (dsp->dvfs) {
			ret = regmap_read(dsp->regmap,
					  dsp->base + ADSP2_CLOCKING, &val);
			if (ret != 0) {
				dev_err(dsp->dev,
					"Failed to read clocking: %d\n", ret);
				return ret;
			}

			if ((val & ADSP2_CLK_SEL_MASK) >= 3) {
				ret = regulator_enable(dsp->dvfs);
				if (ret != 0) {
					dev_err(dsp->dev,
						"Failed to enable supply: %d\n",
						ret);
					return ret;
				}

				ret = regulator_set_voltage(dsp->dvfs,
							    1800000,
							    1800000);
				if (ret != 0) {
					dev_err(dsp->dev,
						"Failed to raise supply: %d\n",
						ret);
					return ret;
				}
			}
		}

		ret = wm_adsp2_ena(dsp);
		if (ret != 0)
			return ret;

		ret = wm_adsp_load(dsp);
		if (ret != 0)
			goto err;

		ret = wm_adsp_setup_algs(dsp, codec);
		if (ret != 0)
			goto err;

		ret = wm_adsp_load_coeff(dsp);
		if (ret != 0)
			goto err;

		/* Initialize caches for enabled and unset controls */
		ret = wm_coeff_init_control_caches(dsp->wm_coeff);
		if (ret != 0)
			goto err;

		/* Sync set controls */
		ret = wm_coeff_sync_controls(dsp->wm_coeff);
		if (ret != 0)
			goto err;

		ret = regmap_update_bits(dsp->regmap,
					 dsp->base + ADSP2_CONTROL,
					 ADSP2_CORE_ENA | ADSP2_START,
					 ADSP2_CORE_ENA | ADSP2_START);
		if (ret != 0)
			goto err;

		dsp->running = true;
		break;

	case SND_SOC_DAPM_PRE_PMD:
		dsp->running = false;

		regmap_update_bits(dsp->regmap, dsp->base + ADSP2_CONTROL,
				   ADSP2_SYS_ENA | ADSP2_CORE_ENA |
				   ADSP2_START, 0);

		/* Make sure DMAs are quiesced */
		regmap_write(dsp->regmap, dsp->base + ADSP2_WDMA_CONFIG_1, 0);
		regmap_write(dsp->regmap, dsp->base + ADSP2_WDMA_CONFIG_2, 0);
		regmap_write(dsp->regmap, dsp->base + ADSP2_RDMA_CONFIG_1, 0);

		if (dsp->dvfs) {
			ret = regulator_set_voltage(dsp->dvfs, 1200000,
						    1800000);
			if (ret != 0)
				dev_warn(dsp->dev,
					 "Failed to lower supply: %d\n",
					 ret);

			ret = regulator_disable(dsp->dvfs);
			if (ret != 0)
				dev_err(dsp->dev,
					"Failed to enable supply: %d\n",
					ret);
		}

		list_for_each_entry(ctl, &dsp->wm_coeff->ctl_list,
				    list) {
			ctl->enabled = 0;
		}

		while (!list_empty(&dsp->alg_regions)) {
			alg_region = list_first_entry(&dsp->alg_regions,
						      struct wm_adsp_alg_region,
						      list);
			list_del(&alg_region->list);
			kfree(alg_region);
		}
		break;

=======
int wm_adsp2_set_dspclk(struct snd_soc_dapm_widget *w, unsigned int freq)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct wm_adsp *dsp = &dsps[w->shift];

	return cs_dsp_set_dspclk(&dsp->cs_dsp, freq);
}
EXPORT_SYMBOL_GPL(wm_adsp2_set_dspclk);

int wm_adsp2_preloader_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct wm_adsp *dsp = &dsps[mc->shift - 1];

	ucontrol->value.integer.value[0] = dsp->preloaded;

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp2_preloader_get);

int wm_adsp2_preloader_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct wm_adsp *dsp = &dsps[mc->shift - 1];
	char preload[32];

	if (dsp->preloaded == ucontrol->value.integer.value[0])
		return 0;

	snprintf(preload, ARRAY_SIZE(preload), "%s Preload", dsp->cs_dsp.name);

	if (ucontrol->value.integer.value[0] || dsp->toggle_preload)
		snd_soc_component_force_enable_pin(component, preload);
	else
		snd_soc_component_disable_pin(component, preload);

	snd_soc_dapm_sync(dapm);

	flush_work(&dsp->boot_work);

	dsp->preloaded = ucontrol->value.integer.value[0];

	if (dsp->toggle_preload) {
		snd_soc_component_disable_pin(component, preload);
		snd_soc_dapm_sync(dapm);
	}

	return 1;
}
EXPORT_SYMBOL_GPL(wm_adsp2_preloader_put);

int wm_adsp_power_up(struct wm_adsp *dsp, bool load_firmware)
{
	int ret = 0;
	char *wmfw_filename = NULL;
	const struct firmware *wmfw_firmware = NULL;
	char *coeff_filename = NULL;
	const struct firmware *coeff_firmware = NULL;

	if (load_firmware) {
		ret = wm_adsp_request_firmware_files(dsp,
						     &wmfw_firmware, &wmfw_filename,
						     &coeff_firmware, &coeff_filename);
		if (ret)
			return ret;
	}

	ret = cs_dsp_power_up(&dsp->cs_dsp,
			      wmfw_firmware, wmfw_filename,
			      coeff_firmware, coeff_filename,
			      wm_adsp_fw_text[dsp->fw]);

	wm_adsp_release_firmware_files(dsp,
				       wmfw_firmware, wmfw_filename,
				       coeff_firmware, coeff_filename);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_power_up);

void wm_adsp_power_down(struct wm_adsp *dsp)
{
	cs_dsp_power_down(&dsp->cs_dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp_power_down);

static void wm_adsp_boot_work(struct work_struct *work)
{
	struct wm_adsp *dsp = container_of(work,
					   struct wm_adsp,
					   boot_work);

	wm_adsp_power_up(dsp, true);
}

int wm_adsp_early_event(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct wm_adsp *dsp = &dsps[w->shift];

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		queue_work(system_unbound_wq, &dsp->boot_work);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		wm_adsp_power_down(dsp);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		break;
	}

	return 0;
<<<<<<< HEAD
err:
	regmap_update_bits(dsp->regmap, dsp->base + ADSP2_CONTROL,
			   ADSP2_SYS_ENA | ADSP2_CORE_ENA | ADSP2_START, 0);
	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp2_event);

int wm_adsp2_init(struct wm_adsp *adsp, bool dvfs)
{
	int ret;

	/*
	 * Disable the DSP memory by default when in reset for a small
	 * power saving.
	 */
	ret = regmap_update_bits(adsp->regmap, adsp->base + ADSP2_CONTROL,
				 ADSP2_MEM_ENA, 0);
	if (ret != 0) {
		adsp_err(adsp, "Failed to clear memory retention: %d\n", ret);
		return ret;
	}

	INIT_LIST_HEAD(&adsp->alg_regions);

	adsp->wm_coeff = kzalloc(sizeof(*adsp->wm_coeff),
				 GFP_KERNEL);
	if (!adsp->wm_coeff)
		return -ENOMEM;
	adsp->wm_coeff->regmap = adsp->regmap;
	adsp->wm_coeff->dev = adsp->dev;
	INIT_LIST_HEAD(&adsp->wm_coeff->ctl_list);

	if (dvfs) {
		adsp->dvfs = devm_regulator_get(adsp->dev, "DCVDD");
		if (IS_ERR(adsp->dvfs)) {
			ret = PTR_ERR(adsp->dvfs);
			dev_err(adsp->dev, "Failed to get DCVDD: %d\n", ret);
			goto out_coeff;
		}

		ret = regulator_enable(adsp->dvfs);
		if (ret != 0) {
			dev_err(adsp->dev, "Failed to enable DCVDD: %d\n",
				ret);
			goto out_coeff;
		}

		ret = regulator_set_voltage(adsp->dvfs, 1200000, 1800000);
		if (ret != 0) {
			dev_err(adsp->dev, "Failed to initialise DVFS: %d\n",
				ret);
			goto out_coeff;
		}

		ret = regulator_disable(adsp->dvfs);
		if (ret != 0) {
			dev_err(adsp->dev, "Failed to disable DCVDD: %d\n",
				ret);
			goto out_coeff;
		}
	}

	return 0;

out_coeff:
	kfree(adsp->wm_coeff);
	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp2_init);

bool wm_adsp_compress_supported(const struct wm_adsp* adsp,
				const struct snd_compr_stream* stream)
{
	if (adsp->fw >= 0 && adsp->fw < WM_ADSP_NUM_FW) {
		if (wm_adsp_fw[adsp->fw].num_caps == 0)
			return false;

		if (wm_adsp_fw[adsp->fw].compr_direction == stream->direction)
			return true;
	}

	return false;
}
EXPORT_SYMBOL_GPL(wm_adsp_compress_supported);

bool wm_adsp_format_supported(const struct wm_adsp *adsp,
			      const struct snd_compr_stream *stream,
			      const struct snd_compr_params *params)
{
	const struct wm_adsp_fw_caps *caps;
	int i;

	for (i = 0; i < wm_adsp_fw[adsp->fw].num_caps; i++) {
		caps = &wm_adsp_fw[adsp->fw].caps[i];
=======
}
EXPORT_SYMBOL_GPL(wm_adsp_early_event);

static int wm_adsp_pre_run(struct cs_dsp *cs_dsp)
{
	struct wm_adsp *dsp = container_of(cs_dsp, struct wm_adsp, cs_dsp);

	if (!dsp->pre_run)
		return 0;

	return (*dsp->pre_run)(dsp);
}

static int wm_adsp_event_post_run(struct cs_dsp *cs_dsp)
{
	struct wm_adsp *dsp = container_of(cs_dsp, struct wm_adsp, cs_dsp);

	if (wm_adsp_fw[dsp->fw].num_caps != 0)
		return wm_adsp_buffer_init(dsp);

	return 0;
}

static void wm_adsp_event_post_stop(struct cs_dsp *cs_dsp)
{
	struct wm_adsp *dsp = container_of(cs_dsp, struct wm_adsp, cs_dsp);

	if (wm_adsp_fw[dsp->fw].num_caps != 0)
		wm_adsp_buffer_free(dsp);

	dsp->fatal_error = false;
}

int wm_adsp_run(struct wm_adsp *dsp)
{
	flush_work(&dsp->boot_work);

	return cs_dsp_run(&dsp->cs_dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp_run);

void wm_adsp_stop(struct wm_adsp *dsp)
{
	cs_dsp_stop(&dsp->cs_dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp_stop);

int wm_adsp_event(struct snd_soc_dapm_widget *w,
		  struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct wm_adsp *dsps = snd_soc_component_get_drvdata(component);
	struct wm_adsp *dsp = &dsps[w->shift];

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		return wm_adsp_run(dsp);
	case SND_SOC_DAPM_PRE_PMD:
		wm_adsp_stop(dsp);
		return 0;
	default:
		return 0;
	}
}
EXPORT_SYMBOL_GPL(wm_adsp_event);

int wm_adsp2_component_probe(struct wm_adsp *dsp, struct snd_soc_component *component)
{
	char preload[32];

	if (!dsp->cs_dsp.no_core_startstop) {
		snprintf(preload, ARRAY_SIZE(preload), "%s Preload", dsp->cs_dsp.name);
		snd_soc_component_disable_pin(component, preload);
	}

	cs_dsp_init_debugfs(&dsp->cs_dsp, component->debugfs_root);

	dsp->component = component;

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp2_component_probe);

int wm_adsp2_component_remove(struct wm_adsp *dsp, struct snd_soc_component *component)
{
	cs_dsp_cleanup_debugfs(&dsp->cs_dsp);

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp2_component_remove);

int wm_adsp2_init(struct wm_adsp *dsp)
{
	int ret;

	INIT_WORK(&dsp->boot_work, wm_adsp_boot_work);

	dsp->sys_config_size = sizeof(struct wm_adsp_system_config_xm_hdr);
	dsp->cs_dsp.client_ops = &wm_adsp2_client_ops;

	ret = cs_dsp_adsp2_init(&dsp->cs_dsp);
	if (ret)
		return ret;

	return wm_adsp_common_init(dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp2_init);

int wm_halo_init(struct wm_adsp *dsp)
{
	int ret;

	INIT_WORK(&dsp->boot_work, wm_adsp_boot_work);

	dsp->sys_config_size = sizeof(struct wm_halo_system_config_xm_hdr);
	dsp->cs_dsp.client_ops = &wm_adsp2_client_ops;

	ret = cs_dsp_halo_init(&dsp->cs_dsp);
	if (ret)
		return ret;

	return wm_adsp_common_init(dsp);
}
EXPORT_SYMBOL_GPL(wm_halo_init);

void wm_adsp2_remove(struct wm_adsp *dsp)
{
	cs_dsp_remove(&dsp->cs_dsp);
}
EXPORT_SYMBOL_GPL(wm_adsp2_remove);

static inline int wm_adsp_compr_attached(struct wm_adsp_compr *compr)
{
	return compr->buf != NULL;
}

static int wm_adsp_compr_attach(struct wm_adsp_compr *compr)
{
	struct wm_adsp_compr_buf *buf = NULL, *tmp;

	if (compr->dsp->fatal_error)
		return -EINVAL;

	list_for_each_entry(tmp, &compr->dsp->buffer_list, list) {
		if (!tmp->name || !strcmp(compr->name, tmp->name)) {
			buf = tmp;
			break;
		}
	}

	if (!buf)
		return -EINVAL;

	compr->buf = buf;
	buf->compr = compr;

	return 0;
}

static void wm_adsp_compr_detach(struct wm_adsp_compr *compr)
{
	if (!compr)
		return;

	/* Wake the poll so it can see buffer is no longer attached */
	if (compr->stream)
		snd_compr_fragment_elapsed(compr->stream);

	if (wm_adsp_compr_attached(compr)) {
		compr->buf->compr = NULL;
		compr->buf = NULL;
	}
}

int wm_adsp_compr_open(struct wm_adsp *dsp, struct snd_compr_stream *stream)
{
	struct wm_adsp_compr *compr, *tmp;
	struct snd_soc_pcm_runtime *rtd = stream->private_data;
	int ret = 0;

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	if (wm_adsp_fw[dsp->fw].num_caps == 0) {
		adsp_err(dsp, "%s: Firmware does not support compressed API\n",
			 snd_soc_rtd_to_codec(rtd, 0)->name);
		ret = -ENXIO;
		goto out;
	}

	if (wm_adsp_fw[dsp->fw].compr_direction != stream->direction) {
		adsp_err(dsp, "%s: Firmware does not support stream direction\n",
			 snd_soc_rtd_to_codec(rtd, 0)->name);
		ret = -EINVAL;
		goto out;
	}

	list_for_each_entry(tmp, &dsp->compr_list, list) {
		if (!strcmp(tmp->name, snd_soc_rtd_to_codec(rtd, 0)->name)) {
			adsp_err(dsp, "%s: Only a single stream supported per dai\n",
				 snd_soc_rtd_to_codec(rtd, 0)->name);
			ret = -EBUSY;
			goto out;
		}
	}

	compr = kzalloc(sizeof(*compr), GFP_KERNEL);
	if (!compr) {
		ret = -ENOMEM;
		goto out;
	}

	compr->dsp = dsp;
	compr->stream = stream;
	compr->name = snd_soc_rtd_to_codec(rtd, 0)->name;

	list_add_tail(&compr->list, &dsp->compr_list);

	stream->runtime->private_data = compr;

out:
	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_open);

int wm_adsp_compr_free(struct snd_soc_component *component,
		       struct snd_compr_stream *stream)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	struct wm_adsp *dsp = compr->dsp;

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	wm_adsp_compr_detach(compr);
	list_del(&compr->list);

	kfree(compr->raw_buf);
	kfree(compr);

	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_free);

static int wm_adsp_compr_check_params(struct snd_compr_stream *stream,
				      struct snd_compr_params *params)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	struct wm_adsp *dsp = compr->dsp;
	const struct wm_adsp_fw_caps *caps;
	const struct snd_codec_desc *desc;
	int i, j;

	if (params->buffer.fragment_size < WM_ADSP_MIN_FRAGMENT_SIZE ||
	    params->buffer.fragment_size > WM_ADSP_MAX_FRAGMENT_SIZE ||
	    params->buffer.fragments < WM_ADSP_MIN_FRAGMENTS ||
	    params->buffer.fragments > WM_ADSP_MAX_FRAGMENTS ||
	    params->buffer.fragment_size % CS_DSP_DATA_WORD_SIZE) {
		compr_err(compr, "Invalid buffer fragsize=%d fragments=%d\n",
			  params->buffer.fragment_size,
			  params->buffer.fragments);

		return -EINVAL;
	}

	for (i = 0; i < wm_adsp_fw[dsp->fw].num_caps; i++) {
		caps = &wm_adsp_fw[dsp->fw].caps[i];
		desc = &caps->desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (caps->id != params->codec.id)
			continue;

		if (stream->direction == SND_COMPRESS_PLAYBACK) {
<<<<<<< HEAD
			if (caps->desc.max_ch < params->codec.ch_out)
				continue;
		} else {
			if (caps->desc.max_ch < params->codec.ch_in)
				continue;
		}

		if ((caps->desc.sample_rates & params->codec.sample_rate) &&
		    (caps->desc.formats & (1 << params->codec.format)))
			return true;
	}

	return false;
}
EXPORT_SYMBOL_GPL(wm_adsp_format_supported);

void wm_adsp_get_caps(const struct wm_adsp *adsp,
		      const struct snd_compr_stream *stream,
		      struct snd_compr_caps *caps)
{
	int i;

	if (wm_adsp_fw[adsp->fw].caps) {
		for (i = 0; i < wm_adsp_fw[adsp->fw].num_caps; i++)
			caps->codecs[i] = wm_adsp_fw[adsp->fw].caps[i].id;

		caps->num_codecs = i;
		caps->direction = wm_adsp_fw[adsp->fw].compr_direction;
	}
}
EXPORT_SYMBOL_GPL(wm_adsp_get_caps);

static int wm_adsp_read_data_block(struct wm_adsp* adsp, int mem_type,
				   unsigned int mem_addr,
				   unsigned int num_words,
				   u32* data)
{
	struct wm_adsp_region const *region = wm_adsp_find_region(adsp,
								  mem_type);
	unsigned int i, reg;
	int ret;

	if (!region)
		return -EINVAL;

	reg = wm_adsp_region_to_reg(region, mem_addr);

	ret = regmap_raw_read(adsp->regmap, reg, data,
			      sizeof(*data) * num_words);
	if (ret < 0)
		return ret;

	for (i = 0; i < num_words; ++i)
		data[i] = be32_to_cpu(data[i]) & 0x00ffffffu;

	return 0;
}

static int wm_adsp_read_data_word(struct wm_adsp* adsp, int mem_type,
				  unsigned int mem_addr, u32* data)
{
	return wm_adsp_read_data_block(adsp, mem_type, mem_addr, 1, data);
}

static int wm_adsp_write_data_word(struct wm_adsp* adsp, int mem_type,
				   unsigned int mem_addr, u32 data)
{
	struct wm_adsp_region const *region = wm_adsp_find_region(adsp,
								  mem_type);
	unsigned int reg;

	if (!region)
		return -EINVAL;

	reg = wm_adsp_region_to_reg(region, mem_addr);

	data = cpu_to_be32(data & 0x00ffffffu);

	return regmap_raw_write(adsp->regmap, reg, &data, sizeof(data));
}

static inline unsigned int wm_adsp_words_to_samps(const struct wm_adsp *adsp,
						  unsigned int words)
{
	return (words * WM_ADSP_DATA_WORD_SIZE) / adsp->sample_size;
}

static inline unsigned int wm_adsp_samps_to_words(const struct wm_adsp *adsp,
						  unsigned int samples,
						  unsigned int offset)
{
	unsigned int groups;
	unsigned int samps_per_group =
		wm_adsp_sample_group[adsp->sample_size].samps_per_group;
	unsigned int words_per_group =
		wm_adsp_sample_group[adsp->sample_size].words_per_group;

	samples += offset;
	samples += samps_per_group - 1;
	groups = samples / samps_per_group;

	return groups * words_per_group;
}

static inline int wm_adsp_host_buffer_read(struct wm_adsp *adsp,
					   unsigned int field_offset, u32* data)
{
	return wm_adsp_read_data_word(adsp, WMFW_ADSP2_XM,
				      adsp->host_buf_ptr + field_offset, data);
}

static inline int wm_adsp_host_buffer_write(struct wm_adsp *adsp,
					    unsigned int field_offset, u32 data)
{
	return wm_adsp_write_data_word(adsp, WMFW_ADSP2_XM,
				       adsp->host_buf_ptr + field_offset,
				       data);
}

static int wm_adsp_populate_buffer_regions(struct wm_adsp *adsp)
{
	int i, ret;
	u32 size;
	u32 cumulative_samps = 0;
	struct wm_adsp_buffer_region_def *host_region_defs =
		wm_adsp_fw[adsp->fw].caps->host_region_defs;
	struct wm_adsp_buffer_region *region;

	for (i = 0; i < wm_adsp_fw[adsp->fw].caps->num_host_regions; ++i) {
		region = &adsp->host_regions[i];

		region->offset_samps = cumulative_samps;
		region->mem_type = host_region_defs[i].mem_type;

		ret = wm_adsp_host_buffer_read(adsp,
					       host_region_defs[i].base_offset,
					       &region->base_addr);
		if (ret < 0)
			return ret;

		ret = wm_adsp_host_buffer_read(adsp,
					       host_region_defs[i].size_offset,
					       &size);
		if (ret < 0)
			return ret;

		cumulative_samps += wm_adsp_words_to_samps(adsp, size);

		region->cumulative_samps = cumulative_samps;
	}

	return 0;
}

static void wm_adsp_extract_16bit(struct wm_adsp *adsp, int num_samps,
				  int group_offset)
{
	int i;
	int16_t sample;
	u32* raw_buf = adsp->raw_capt_buf;
	int words_per_group =
		wm_adsp_sample_group[sizeof(int16_t)].words_per_group;

	for (i = 0; i < num_samps; ++i) {
		switch (group_offset++) {
		case 2:
			sample = (raw_buf[1] >> 8) & 0xffff;
			raw_buf += words_per_group;
			group_offset = 0;
			break;
		case 1:
			sample = ((raw_buf[0] & 0xff) << 8) |
				 (raw_buf[1] & 0xff);
			break;
		default:
			sample = (raw_buf[0] >> 8) & 0xffff;
			break;
		}

		*(int16_t*)(adsp->capt_buf.buf + adsp->capt_buf.head) = sample;

		adsp->capt_buf.head += adsp->sample_size;
		adsp->capt_buf.head &= adsp->capt_buf_size - 1;
	}
}

static int wm_adsp_read_samples(struct wm_adsp *adsp, int32_t read_index,
				int avail)
{
	int circ_space_s = CIRC_SPACE(adsp->capt_buf.head,
				      adsp->capt_buf.tail,
				      adsp->capt_buf_size) / adsp->sample_size;
	int samps_per_group =
		wm_adsp_sample_group[adsp->sample_size].samps_per_group;
	int words_per_group =
		wm_adsp_sample_group[adsp->sample_size].words_per_group;
	int mem_type;
	unsigned int adsp_addr, adsp_read_len;
	int group_index, group_offset;
	int num_samps;
	int i, ret;

	/* Calculate read parameters */
	for (i = 0; i < wm_adsp_fw[adsp->fw].caps->num_host_regions; ++i) {
		if (read_index < adsp->host_regions[i].cumulative_samps)
			break;
	}

	if (i == wm_adsp_fw[adsp->fw].caps->num_host_regions)
		return -EINVAL;

	num_samps = adsp->host_regions[i].cumulative_samps - read_index;
	group_index = (read_index - adsp->host_regions[i].offset_samps) /
		      samps_per_group;
	group_offset = read_index % samps_per_group;
	mem_type = adsp->host_regions[i].mem_type;
	adsp_addr = adsp->host_regions[i].base_addr +
		    (group_index * words_per_group);

	if (circ_space_s < num_samps)
		num_samps = circ_space_s;
	if (avail < num_samps)
		num_samps = avail;
	if (num_samps >= adsp->raw_buf_samps) {
		num_samps = adsp->raw_buf_samps;
		num_samps -= group_offset;
	}
	if (!num_samps)
		return 0;

	/* Read data from DSP */
	adsp_read_len = wm_adsp_samps_to_words(adsp, num_samps, group_offset);
	ret = wm_adsp_read_data_block(adsp, mem_type, adsp_addr,
				      adsp_read_len, adsp->raw_capt_buf);
	if (ret != 0)
		return ret;

	/* Extract samples from raw buffer into the capture buffer */
	switch (adsp->sample_size) {
	case 2:
		wm_adsp_extract_16bit(adsp, num_samps, group_offset);
		break;
	default:
		return -EINVAL;
	}

	return num_samps;
}

static int wm_adsp_capture_block(struct wm_adsp *adsp, int* avail)
{
	int last_region = wm_adsp_fw[adsp->fw].caps->num_host_regions - 1;
	int host_size_samps =
		adsp->host_regions[last_region].cumulative_samps;
	int num_samps;
	u32 next_read_index, next_write_index;
	int32_t write_index, read_index;
	int ret;

	/* Get current host buffer status */
	ret = wm_adsp_host_buffer_read(adsp,
				       HOST_BUFFER_FIELD(next_read_index),
				       &next_read_index);
	if (ret < 0)
		return ret;
	ret = wm_adsp_host_buffer_read(adsp,
				       HOST_BUFFER_FIELD(next_write_index),
				       &next_write_index);
	if (ret < 0)
		return ret;

	read_index = sign_extend32(next_read_index, 23);
	write_index = sign_extend32(next_write_index, 23);

	if (read_index < 0)
		return -EIO;	/* stream has not yet started */

	*avail = write_index - read_index;
	if (*avail < 0)
		*avail += host_size_samps;

	/* Read data from DSP */
	num_samps = wm_adsp_read_samples(adsp, read_index, *avail);
	if (num_samps <= 0)
		return num_samps;

	/* update read index to account for samples read */
	next_read_index += num_samps;
	if (next_read_index == host_size_samps)
		next_read_index = 0;

	ret = wm_adsp_host_buffer_write(adsp,
				        HOST_BUFFER_FIELD(next_read_index),
				        next_read_index);
	if (ret < 0)
		return ret;

	return num_samps;
}

int wm_adsp_stream_alloc(struct wm_adsp* adsp,
			 const struct snd_compr_params *params)
{
	int ret;
	unsigned int size;

	switch (params->codec.format) {
	case SNDRV_PCM_FORMAT_S16_LE:
		adsp->sample_size = 2;
		break;
	default:
		return -EINVAL;
	}

	if (!adsp->capt_buf.buf) {
		adsp->capt_buf_size = WM_ADSP_CAPTURE_BUFFER_SIZE;
		adsp->capt_buf.buf = kzalloc(adsp->capt_buf_size, GFP_KERNEL);

		if (!adsp->capt_buf.buf)
			return -ENOMEM;
	}

	adsp->capt_buf.head = 0;
	adsp->capt_buf.tail = 0;

	if (!adsp->raw_capt_buf) {
		adsp->raw_buf_samps = WM_ADSP_RAW_BUFFER_SAMPS;
		size = wm_adsp_samps_to_words(adsp, adsp->raw_buf_samps, 0) *
		       sizeof(*adsp->raw_capt_buf);
		adsp->raw_capt_buf = kzalloc(size, GFP_KERNEL);

		if (!adsp->raw_capt_buf) {
			ret = -ENOMEM;
			goto err_capt_buf;
		}
	}

	if (!adsp->host_regions) {
		size = wm_adsp_fw[adsp->fw].caps->num_host_regions *
		       sizeof(*adsp->host_regions);
		adsp->host_regions = kzalloc(size, GFP_KERNEL);

		if (!adsp->host_regions) {
			ret = -ENOMEM;
			goto err_raw_capt_buf;
		}
=======
			if (desc->max_ch < params->codec.ch_out)
				continue;
		} else {
			if (desc->max_ch < params->codec.ch_in)
				continue;
		}

		if (!(desc->formats & (1 << params->codec.format)))
			continue;

		for (j = 0; j < desc->num_sample_rates; ++j)
			if (desc->sample_rates[j] == params->codec.sample_rate)
				return 0;
	}

	compr_err(compr, "Invalid params id=%u ch=%u,%u rate=%u fmt=%u\n",
		  params->codec.id, params->codec.ch_in, params->codec.ch_out,
		  params->codec.sample_rate, params->codec.format);
	return -EINVAL;
}

static inline unsigned int wm_adsp_compr_frag_words(struct wm_adsp_compr *compr)
{
	return compr->size.fragment_size / CS_DSP_DATA_WORD_SIZE;
}

int wm_adsp_compr_set_params(struct snd_soc_component *component,
			     struct snd_compr_stream *stream,
			     struct snd_compr_params *params)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	unsigned int size;
	int ret;

	ret = wm_adsp_compr_check_params(stream, params);
	if (ret)
		return ret;

	compr->size = params->buffer;

	compr_dbg(compr, "fragment_size=%d fragments=%d\n",
		  compr->size.fragment_size, compr->size.fragments);

	size = wm_adsp_compr_frag_words(compr) * sizeof(*compr->raw_buf);
	compr->raw_buf = kmalloc(size, GFP_DMA | GFP_KERNEL);
	if (!compr->raw_buf)
		return -ENOMEM;

	compr->sample_rate = params->codec.sample_rate;

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_set_params);

int wm_adsp_compr_get_caps(struct snd_soc_component *component,
			   struct snd_compr_stream *stream,
			   struct snd_compr_caps *caps)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	int fw = compr->dsp->fw;
	int i;

	if (wm_adsp_fw[fw].caps) {
		for (i = 0; i < wm_adsp_fw[fw].num_caps; i++)
			caps->codecs[i] = wm_adsp_fw[fw].caps[i].id;

		caps->num_codecs = i;
		caps->direction = wm_adsp_fw[fw].compr_direction;

		caps->min_fragment_size = WM_ADSP_MIN_FRAGMENT_SIZE;
		caps->max_fragment_size = WM_ADSP_MAX_FRAGMENT_SIZE;
		caps->min_fragments = WM_ADSP_MIN_FRAGMENTS;
		caps->max_fragments = WM_ADSP_MAX_FRAGMENTS;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_get_caps);

static inline int wm_adsp_buffer_read(struct wm_adsp_compr_buf *buf,
				      unsigned int field_offset, u32 *data)
{
	return cs_dsp_read_data_word(&buf->dsp->cs_dsp, buf->host_buf_mem_type,
				     buf->host_buf_ptr + field_offset, data);
}

static inline int wm_adsp_buffer_write(struct wm_adsp_compr_buf *buf,
				       unsigned int field_offset, u32 data)
{
	return cs_dsp_write_data_word(&buf->dsp->cs_dsp, buf->host_buf_mem_type,
				      buf->host_buf_ptr + field_offset,
				      data);
}

static int wm_adsp_buffer_populate(struct wm_adsp_compr_buf *buf)
{
	const struct wm_adsp_fw_caps *caps = wm_adsp_fw[buf->dsp->fw].caps;
	struct wm_adsp_buffer_region *region;
	u32 offset = 0;
	int i, ret;

	buf->regions = kcalloc(caps->num_regions, sizeof(*buf->regions),
			       GFP_KERNEL);
	if (!buf->regions)
		return -ENOMEM;

	for (i = 0; i < caps->num_regions; ++i) {
		region = &buf->regions[i];

		region->offset = offset;
		region->mem_type = caps->region_defs[i].mem_type;

		ret = wm_adsp_buffer_read(buf, caps->region_defs[i].base_offset,
					  &region->base_addr);
		if (ret < 0)
			goto err;

		ret = wm_adsp_buffer_read(buf, caps->region_defs[i].size_offset,
					  &offset);
		if (ret < 0)
			goto err;

		region->cumulative_size = offset;

		compr_dbg(buf,
			  "region=%d type=%d base=%08x off=%08x size=%08x\n",
			  i, region->mem_type, region->base_addr,
			  region->offset, region->cumulative_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
err_raw_capt_buf:
	kfree(adsp->raw_capt_buf);
err_capt_buf:
	kfree(adsp->capt_buf.buf);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_alloc);

int wm_adsp_stream_free(struct wm_adsp* adsp)
{
	if (adsp->host_regions) {
		kfree(adsp->host_regions);
		adsp->host_regions = NULL;
	}

	if (adsp->raw_capt_buf) {
		kfree(adsp->raw_capt_buf);
		adsp->raw_capt_buf = NULL;
	}

	if (adsp->capt_buf.buf) {
		kfree(adsp->capt_buf.buf);
		adsp->capt_buf.buf = NULL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_free);

int wm_adsp_stream_start(struct wm_adsp *adsp)
{
	u32 xm_base, magic;
	int i, ret;

	ret = wm_adsp_read_data_word(adsp, WMFW_ADSP2_XM,
				     ADSP2_SYSTEM_CONFIG_XM_PTR, &xm_base);
	if (ret < 0)
		return ret;

	ret = wm_adsp_read_data_word(adsp, WMFW_ADSP2_XM,
				     xm_base + WM_ADSP_ALG_XM_PTR +
				     ALG_XM_FIELD(magic),
				     &magic);
=======
err:
	kfree(buf->regions);
	return ret;
}

static void wm_adsp_buffer_clear(struct wm_adsp_compr_buf *buf)
{
	buf->irq_count = 0xFFFFFFFF;
	buf->read_index = -1;
	buf->avail = 0;
}

static struct wm_adsp_compr_buf *wm_adsp_buffer_alloc(struct wm_adsp *dsp)
{
	struct wm_adsp_compr_buf *buf;

	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	if (!buf)
		return NULL;

	buf->dsp = dsp;

	wm_adsp_buffer_clear(buf);

	return buf;
}

static int wm_adsp_buffer_parse_legacy(struct wm_adsp *dsp)
{
	struct cs_dsp_alg_region *alg_region;
	struct wm_adsp_compr_buf *buf;
	u32 xmalg, addr, magic;
	int i, ret;

	alg_region = cs_dsp_find_alg_region(&dsp->cs_dsp, WMFW_ADSP2_XM, dsp->cs_dsp.fw_id);
	if (!alg_region) {
		adsp_err(dsp, "No algorithm region found\n");
		return -EINVAL;
	}

	xmalg = dsp->sys_config_size / sizeof(__be32);

	addr = alg_region->base + xmalg + ALG_XM_FIELD(magic);
	ret = cs_dsp_read_data_word(&dsp->cs_dsp, WMFW_ADSP2_XM, addr, &magic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return ret;

	if (magic != WM_ADSP_ALG_XM_STRUCT_MAGIC)
<<<<<<< HEAD
		return -EINVAL;

	for (i = 0; i < 5; ++i) {
		ret = wm_adsp_read_data_word(adsp, WMFW_ADSP2_XM,
					     xm_base + WM_ADSP_ALG_XM_PTR +
					     ALG_XM_FIELD(host_buf_ptr),
					     &adsp->host_buf_ptr);
		if (ret < 0)
			return ret;

		if (adsp->host_buf_ptr)
			break;

		msleep(1);
	}

	if (!adsp->host_buf_ptr)
		return -EIO;

	ret = wm_adsp_host_buffer_read(adsp,
				       HOST_BUFFER_FIELD(low_water_mark),
				       &adsp->low_water_mark);
	if (ret < 0)
		return ret;

	ret = wm_adsp_populate_buffer_regions(adsp);
	if (ret < 0)
		return ret;

	return 0;
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_start);

int wm_adsp_stream_capture(struct wm_adsp *adsp)
{
	int avail = 0;
	int amount_read;
	int total_read = 0;
	int ret = 0;

	adsp->buffer_drain_pending = false;

	do {
		amount_read = 0;
		do {
			ret = wm_adsp_capture_block(adsp, &avail);
			if (ret < 0)
				return ret;

			amount_read += ret;
		} while (ret > 0);

		total_read += amount_read;
	} while (amount_read > 0 && avail > adsp->low_water_mark);

	if (avail > adsp->low_water_mark)
		adsp->buffer_drain_pending = true;

	return total_read * adsp->sample_size;
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_capture);

int wm_adsp_stream_read(struct wm_adsp *adsp, char __user *buf, size_t count)
{
	int avail, to_end;

	if (!adsp->running)
		return -EIO;

	avail = CIRC_CNT(adsp->capt_buf.head,
			 adsp->capt_buf.tail,
			 adsp->capt_buf_size);
	to_end = CIRC_CNT_TO_END(adsp->capt_buf.head,
				 adsp->capt_buf.tail,
				 adsp->capt_buf_size);

	if (avail < count)
		count = avail;

	if (count > to_end) {
		if (copy_to_user(buf,
				 adsp->capt_buf.buf +
				 adsp->capt_buf.tail,
				 to_end))
			return -EFAULT;
		if (copy_to_user(buf + to_end, adsp->capt_buf.buf,
				 count - to_end))
			return -EFAULT;
	} else {
		if (copy_to_user(buf,
				 adsp->capt_buf.buf +
				 adsp->capt_buf.tail,
				 count))
			return -EFAULT;
	}

	adsp->capt_buf.tail += count;
	adsp->capt_buf.tail &= adsp->capt_buf_size - 1;

	if (adsp->buffer_drain_pending)
		wm_adsp_stream_capture(adsp);

	return count;
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_read);

int wm_adsp_stream_avail(const struct wm_adsp *adsp)
{
	return CIRC_CNT(adsp->capt_buf.head,
			adsp->capt_buf.tail,
			adsp->capt_buf_size);
}
EXPORT_SYMBOL_GPL(wm_adsp_stream_avail);
=======
		return -ENODEV;

	buf = wm_adsp_buffer_alloc(dsp);
	if (!buf)
		return -ENOMEM;

	addr = alg_region->base + xmalg + ALG_XM_FIELD(host_buf_ptr);
	for (i = 0; i < 5; ++i) {
		ret = cs_dsp_read_data_word(&dsp->cs_dsp, WMFW_ADSP2_XM, addr,
					    &buf->host_buf_ptr);
		if (ret < 0)
			goto err;

		if (buf->host_buf_ptr)
			break;

		usleep_range(1000, 2000);
	}

	if (!buf->host_buf_ptr) {
		ret = -EIO;
		goto err;
	}

	buf->host_buf_mem_type = WMFW_ADSP2_XM;

	ret = wm_adsp_buffer_populate(buf);
	if (ret < 0)
		goto err;

	list_add_tail(&buf->list, &dsp->buffer_list);

	compr_dbg(buf, "legacy host_buf_ptr=%x\n", buf->host_buf_ptr);

	return 0;

err:
	kfree(buf);

	return ret;
}

static int wm_adsp_buffer_parse_coeff(struct cs_dsp_coeff_ctl *cs_ctl)
{
	struct wm_adsp_host_buf_coeff_v1 coeff_v1;
	struct wm_adsp_compr_buf *buf;
	struct wm_adsp *dsp = container_of(cs_ctl->dsp, struct wm_adsp, cs_dsp);
	unsigned int version = 0;
	int ret, i;

	for (i = 0; i < 5; ++i) {
		ret = cs_dsp_coeff_read_ctrl(cs_ctl, 0, &coeff_v1,
					     min(cs_ctl->len, sizeof(coeff_v1)));
		if (ret < 0)
			return ret;

		if (coeff_v1.host_buf_ptr)
			break;

		usleep_range(1000, 2000);
	}

	if (!coeff_v1.host_buf_ptr) {
		adsp_err(dsp, "Failed to acquire host buffer\n");
		return -EIO;
	}

	buf = wm_adsp_buffer_alloc(dsp);
	if (!buf)
		return -ENOMEM;

	buf->host_buf_mem_type = cs_ctl->alg_region.type;
	buf->host_buf_ptr = be32_to_cpu(coeff_v1.host_buf_ptr);

	ret = wm_adsp_buffer_populate(buf);
	if (ret < 0)
		goto err;

	/*
	 * v0 host_buffer coefficients didn't have versioning, so if the
	 * control is one word, assume version 0.
	 */
	if (cs_ctl->len == 4)
		goto done;

	version = be32_to_cpu(coeff_v1.versions) & HOST_BUF_COEFF_COMPAT_VER_MASK;
	version >>= HOST_BUF_COEFF_COMPAT_VER_SHIFT;

	if (version > HOST_BUF_COEFF_SUPPORTED_COMPAT_VER) {
		adsp_err(dsp,
			 "Host buffer coeff ver %u > supported version %u\n",
			 version, HOST_BUF_COEFF_SUPPORTED_COMPAT_VER);
		ret = -EINVAL;
		goto err;
	}

	cs_dsp_remove_padding((u32 *)&coeff_v1.name, ARRAY_SIZE(coeff_v1.name));

	buf->name = kasprintf(GFP_KERNEL, "%s-dsp-%s", dsp->part,
			      (char *)&coeff_v1.name);

done:
	list_add_tail(&buf->list, &dsp->buffer_list);

	compr_dbg(buf, "host_buf_ptr=%x coeff version %u\n",
		  buf->host_buf_ptr, version);

	return version;

err:
	kfree(buf);

	return ret;
}

static int wm_adsp_buffer_init(struct wm_adsp *dsp)
{
	struct cs_dsp_coeff_ctl *cs_ctl;
	int ret;

	list_for_each_entry(cs_ctl, &dsp->cs_dsp.ctl_list, list) {
		if (cs_ctl->type != WMFW_CTL_TYPE_HOST_BUFFER)
			continue;

		if (!cs_ctl->enabled)
			continue;

		ret = wm_adsp_buffer_parse_coeff(cs_ctl);
		if (ret < 0) {
			adsp_err(dsp, "Failed to parse coeff: %d\n", ret);
			goto error;
		} else if (ret == 0) {
			/* Only one buffer supported for version 0 */
			return 0;
		}
	}

	if (list_empty(&dsp->buffer_list)) {
		/* Fall back to legacy support */
		ret = wm_adsp_buffer_parse_legacy(dsp);
		if (ret == -ENODEV)
			adsp_info(dsp, "Legacy support not available\n");
		else if (ret)
			adsp_warn(dsp, "Failed to parse legacy: %d\n", ret);
	}

	return 0;

error:
	wm_adsp_buffer_free(dsp);
	return ret;
}

static int wm_adsp_buffer_free(struct wm_adsp *dsp)
{
	struct wm_adsp_compr_buf *buf, *tmp;

	list_for_each_entry_safe(buf, tmp, &dsp->buffer_list, list) {
		wm_adsp_compr_detach(buf->compr);

		kfree(buf->name);
		kfree(buf->regions);
		list_del(&buf->list);
		kfree(buf);
	}

	return 0;
}

static int wm_adsp_buffer_get_error(struct wm_adsp_compr_buf *buf)
{
	int ret;

	ret = wm_adsp_buffer_read(buf, HOST_BUFFER_FIELD(error), &buf->error);
	if (ret < 0) {
		compr_err(buf, "Failed to check buffer error: %d\n", ret);
		return ret;
	}
	if (buf->error != 0) {
		compr_err(buf, "Buffer error occurred: %d\n", buf->error);
		return -EIO;
	}

	return 0;
}

int wm_adsp_compr_trigger(struct snd_soc_component *component,
			  struct snd_compr_stream *stream, int cmd)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	struct wm_adsp *dsp = compr->dsp;
	int ret = 0;

	compr_dbg(compr, "Trigger: %d\n", cmd);

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		if (!wm_adsp_compr_attached(compr)) {
			ret = wm_adsp_compr_attach(compr);
			if (ret < 0) {
				compr_err(compr, "Failed to link buffer and stream: %d\n",
					  ret);
				break;
			}
		}

		ret = wm_adsp_buffer_get_error(compr->buf);
		if (ret < 0)
			break;

		/* Trigger the IRQ at one fragment of data */
		ret = wm_adsp_buffer_write(compr->buf,
					   HOST_BUFFER_FIELD(high_water_mark),
					   wm_adsp_compr_frag_words(compr));
		if (ret < 0) {
			compr_err(compr, "Failed to set high water mark: %d\n",
				  ret);
			break;
		}
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		if (wm_adsp_compr_attached(compr))
			wm_adsp_buffer_clear(compr->buf);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_trigger);

static inline int wm_adsp_buffer_size(struct wm_adsp_compr_buf *buf)
{
	int last_region = wm_adsp_fw[buf->dsp->fw].caps->num_regions - 1;

	return buf->regions[last_region].cumulative_size;
}

static int wm_adsp_buffer_update_avail(struct wm_adsp_compr_buf *buf)
{
	u32 next_read_index, next_write_index;
	int write_index, read_index, avail;
	int ret;

	/* Only sync read index if we haven't already read a valid index */
	if (buf->read_index < 0) {
		ret = wm_adsp_buffer_read(buf,
				HOST_BUFFER_FIELD(next_read_index),
				&next_read_index);
		if (ret < 0)
			return ret;

		read_index = sign_extend32(next_read_index, 23);

		if (read_index < 0) {
			compr_dbg(buf, "Avail check on unstarted stream\n");
			return 0;
		}

		buf->read_index = read_index;
	}

	ret = wm_adsp_buffer_read(buf, HOST_BUFFER_FIELD(next_write_index),
			&next_write_index);
	if (ret < 0)
		return ret;

	write_index = sign_extend32(next_write_index, 23);

	avail = write_index - buf->read_index;
	if (avail < 0)
		avail += wm_adsp_buffer_size(buf);

	compr_dbg(buf, "readindex=0x%x, writeindex=0x%x, avail=%d\n",
		  buf->read_index, write_index, avail * CS_DSP_DATA_WORD_SIZE);

	buf->avail = avail;

	return 0;
}

int wm_adsp_compr_handle_irq(struct wm_adsp *dsp)
{
	struct wm_adsp_compr_buf *buf;
	struct wm_adsp_compr *compr;
	int ret = 0;

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	if (list_empty(&dsp->buffer_list)) {
		ret = -ENODEV;
		goto out;
	}

	adsp_dbg(dsp, "Handling buffer IRQ\n");

	list_for_each_entry(buf, &dsp->buffer_list, list) {
		compr = buf->compr;

		ret = wm_adsp_buffer_get_error(buf);
		if (ret < 0)
			goto out_notify; /* Wake poll to report error */

		ret = wm_adsp_buffer_read(buf, HOST_BUFFER_FIELD(irq_count),
					  &buf->irq_count);
		if (ret < 0) {
			compr_err(buf, "Failed to get irq_count: %d\n", ret);
			goto out;
		}

		ret = wm_adsp_buffer_update_avail(buf);
		if (ret < 0) {
			compr_err(buf, "Error reading avail: %d\n", ret);
			goto out;
		}

		if (wm_adsp_fw[dsp->fw].voice_trigger && buf->irq_count == 2)
			ret = WM_ADSP_COMPR_VOICE_TRIGGER;

out_notify:
		if (compr && compr->stream)
			snd_compr_fragment_elapsed(compr->stream);
	}

out:
	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_handle_irq);

static int wm_adsp_buffer_reenable_irq(struct wm_adsp_compr_buf *buf)
{
	if (buf->irq_count & 0x01)
		return 0;

	compr_dbg(buf, "Enable IRQ(0x%x) for next fragment\n", buf->irq_count);

	buf->irq_count |= 0x01;

	return wm_adsp_buffer_write(buf, HOST_BUFFER_FIELD(irq_ack),
				    buf->irq_count);
}

int wm_adsp_compr_pointer(struct snd_soc_component *component,
			  struct snd_compr_stream *stream,
			  struct snd_compr_tstamp *tstamp)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	struct wm_adsp *dsp = compr->dsp;
	struct wm_adsp_compr_buf *buf;
	int ret = 0;

	compr_dbg(compr, "Pointer request\n");

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	buf = compr->buf;

	if (dsp->fatal_error || !buf || buf->error) {
		snd_compr_stop_error(stream, SNDRV_PCM_STATE_XRUN);
		ret = -EIO;
		goto out;
	}

	if (buf->avail < wm_adsp_compr_frag_words(compr)) {
		ret = wm_adsp_buffer_update_avail(buf);
		if (ret < 0) {
			compr_err(compr, "Error reading avail: %d\n", ret);
			goto out;
		}

		/*
		 * If we really have less than 1 fragment available tell the
		 * DSP to inform us once a whole fragment is available.
		 */
		if (buf->avail < wm_adsp_compr_frag_words(compr)) {
			ret = wm_adsp_buffer_get_error(buf);
			if (ret < 0) {
				if (buf->error)
					snd_compr_stop_error(stream,
							SNDRV_PCM_STATE_XRUN);
				goto out;
			}

			ret = wm_adsp_buffer_reenable_irq(buf);
			if (ret < 0) {
				compr_err(compr, "Failed to re-enable buffer IRQ: %d\n",
					  ret);
				goto out;
			}
		}
	}

	tstamp->copied_total = compr->copied_total;
	tstamp->copied_total += buf->avail * CS_DSP_DATA_WORD_SIZE;
	tstamp->sampling_rate = compr->sample_rate;

out:
	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_pointer);

static int wm_adsp_buffer_capture_block(struct wm_adsp_compr *compr, int target)
{
	struct wm_adsp_compr_buf *buf = compr->buf;
	unsigned int adsp_addr;
	int mem_type, nwords, max_read;
	int i, ret;

	/* Calculate read parameters */
	for (i = 0; i < wm_adsp_fw[buf->dsp->fw].caps->num_regions; ++i)
		if (buf->read_index < buf->regions[i].cumulative_size)
			break;

	if (i == wm_adsp_fw[buf->dsp->fw].caps->num_regions)
		return -EINVAL;

	mem_type = buf->regions[i].mem_type;
	adsp_addr = buf->regions[i].base_addr +
		    (buf->read_index - buf->regions[i].offset);

	max_read = wm_adsp_compr_frag_words(compr);
	nwords = buf->regions[i].cumulative_size - buf->read_index;

	if (nwords > target)
		nwords = target;
	if (nwords > buf->avail)
		nwords = buf->avail;
	if (nwords > max_read)
		nwords = max_read;
	if (!nwords)
		return 0;

	/* Read data from DSP */
	ret = cs_dsp_read_raw_data_block(&buf->dsp->cs_dsp, mem_type, adsp_addr,
					 nwords, (__be32 *)compr->raw_buf);
	if (ret < 0)
		return ret;

	cs_dsp_remove_padding(compr->raw_buf, nwords);

	/* update read index to account for words read */
	buf->read_index += nwords;
	if (buf->read_index == wm_adsp_buffer_size(buf))
		buf->read_index = 0;

	ret = wm_adsp_buffer_write(buf, HOST_BUFFER_FIELD(next_read_index),
				   buf->read_index);
	if (ret < 0)
		return ret;

	/* update avail to account for words read */
	buf->avail -= nwords;

	return nwords;
}

static int wm_adsp_compr_read(struct wm_adsp_compr *compr,
			      char __user *buf, size_t count)
{
	struct wm_adsp *dsp = compr->dsp;
	int ntotal = 0;
	int nwords, nbytes;

	compr_dbg(compr, "Requested read of %zu bytes\n", count);

	if (dsp->fatal_error || !compr->buf || compr->buf->error) {
		snd_compr_stop_error(compr->stream, SNDRV_PCM_STATE_XRUN);
		return -EIO;
	}

	count /= CS_DSP_DATA_WORD_SIZE;

	do {
		nwords = wm_adsp_buffer_capture_block(compr, count);
		if (nwords < 0) {
			compr_err(compr, "Failed to capture block: %d\n",
				  nwords);
			return nwords;
		}

		nbytes = nwords * CS_DSP_DATA_WORD_SIZE;

		compr_dbg(compr, "Read %d bytes\n", nbytes);

		if (copy_to_user(buf + ntotal, compr->raw_buf, nbytes)) {
			compr_err(compr, "Failed to copy data to user: %d, %d\n",
				  ntotal, nbytes);
			return -EFAULT;
		}

		count -= nwords;
		ntotal += nbytes;
	} while (nwords > 0 && count > 0);

	compr->copied_total += ntotal;

	return ntotal;
}

int wm_adsp_compr_copy(struct snd_soc_component *component,
		       struct snd_compr_stream *stream, char __user *buf,
		       size_t count)
{
	struct wm_adsp_compr *compr = stream->runtime->private_data;
	struct wm_adsp *dsp = compr->dsp;
	int ret;

	mutex_lock(&dsp->cs_dsp.pwr_lock);

	if (stream->direction == SND_COMPRESS_CAPTURE)
		ret = wm_adsp_compr_read(compr, buf, count);
	else
		ret = -ENOTSUPP;

	mutex_unlock(&dsp->cs_dsp.pwr_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(wm_adsp_compr_copy);

static void wm_adsp_fatal_error(struct cs_dsp *cs_dsp)
{
	struct wm_adsp *dsp = container_of(cs_dsp, struct wm_adsp, cs_dsp);
	struct wm_adsp_compr *compr;

	dsp->fatal_error = true;

	list_for_each_entry(compr, &dsp->compr_list, list) {
		if (compr->stream)
			snd_compr_fragment_elapsed(compr->stream);
	}
}

irqreturn_t wm_adsp2_bus_error(int irq, void *data)
{
	struct wm_adsp *dsp = (struct wm_adsp *)data;

	cs_dsp_adsp2_bus_error(&dsp->cs_dsp);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL_GPL(wm_adsp2_bus_error);

irqreturn_t wm_halo_bus_error(int irq, void *data)
{
	struct wm_adsp *dsp = (struct wm_adsp *)data;

	cs_dsp_halo_bus_error(&dsp->cs_dsp);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL_GPL(wm_halo_bus_error);

irqreturn_t wm_halo_wdt_expire(int irq, void *data)
{
	struct wm_adsp *dsp = data;

	cs_dsp_halo_wdt_expire(&dsp->cs_dsp);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL_GPL(wm_halo_wdt_expire);

static const struct cs_dsp_client_ops wm_adsp1_client_ops = {
	.control_add = wm_adsp_control_add,
	.control_remove = wm_adsp_control_remove,
};

static const struct cs_dsp_client_ops wm_adsp2_client_ops = {
	.control_add = wm_adsp_control_add,
	.control_remove = wm_adsp_control_remove,
	.pre_run = wm_adsp_pre_run,
	.post_run = wm_adsp_event_post_run,
	.post_stop = wm_adsp_event_post_stop,
	.watchdog_expired = wm_adsp_fatal_error,
};

MODULE_LICENSE("GPL v2");
MODULE_IMPORT_NS(FW_CS_DSP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
