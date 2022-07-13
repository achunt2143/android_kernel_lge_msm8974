<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * tegra_pcm.h - Definitions for Tegra PCM driver
 *
 * Author: Stephen Warren <swarren@nvidia.com>
<<<<<<< HEAD
 * Copyright (C) 2010 - NVIDIA, Inc.
=======
 * Copyright (C) 2010,2012 - NVIDIA, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Based on code copyright/by:
 *
 * Copyright (c) 2009-2010, NVIDIA Corporation.
 * Scott Peterson <speterson@nvidia.com>
 *
 * Copyright (C) 2010 Google, Inc.
 * Iliyan Malchev <malchev@google.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __TEGRA_PCM_H__
#define __TEGRA_PCM_H__

<<<<<<< HEAD
#include <mach/dma.h>

struct tegra_pcm_dma_params {
	unsigned long addr;
	unsigned long wrap;
	unsigned long width;
	unsigned long req_sel;
};

struct tegra_runtime_data {
	struct snd_pcm_substream *substream;
	spinlock_t lock;
	int running;
	int dma_pos;
	int dma_pos_end;
	int period_index;
	int dma_req_idx;
	struct tegra_dma_req dma_req[2];
	struct tegra_dma_channel *dma_chan;
};
=======
#include <sound/dmaengine_pcm.h>
#include <sound/asound.h>

int tegra_pcm_construct(struct snd_soc_component *component,
			struct snd_soc_pcm_runtime *rtd);
int tegra_pcm_open(struct snd_soc_component *component,
		   struct snd_pcm_substream *substream);
int tegra_pcm_close(struct snd_soc_component *component,
		    struct snd_pcm_substream *substream);
int tegra_pcm_hw_params(struct snd_soc_component *component,
			struct snd_pcm_substream *substream,
			struct snd_pcm_hw_params *params);
snd_pcm_uframes_t tegra_pcm_pointer(struct snd_soc_component *component,
				    struct snd_pcm_substream *substream);
int tegra_pcm_platform_register(struct device *dev);
int devm_tegra_pcm_platform_register(struct device *dev);
int tegra_pcm_platform_register_with_chan_names(struct device *dev,
				struct snd_dmaengine_pcm_config *config,
				char *txdmachan, char *rxdmachan);
void tegra_pcm_platform_unregister(struct device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
