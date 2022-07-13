<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for ALSA
# Copyright (c) 1999 by Jaroslav Kysela <perex@perex.cz>
#

snd-mixer-oss-objs := mixer_oss.o

snd-pcm-oss-y := pcm_oss.o
snd-pcm-oss-$(CONFIG_SND_PCM_OSS_PLUGINS) += pcm_plugin.o \
	io.o copy.o linear.o mulaw.o route.o rate.o

obj-$(CONFIG_SND_MIXER_OSS) += snd-mixer-oss.o
obj-$(CONFIG_SND_PCM_OSS) += snd-pcm-oss.o
