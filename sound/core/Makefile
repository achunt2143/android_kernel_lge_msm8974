<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for ALSA
# Copyright (c) 1999,2001 by Jaroslav Kysela <perex@perex.cz>
#

<<<<<<< HEAD
snd-y     := sound.o init.o memory.o info.o control.o misc.o device.o
snd-$(CONFIG_ISA_DMA_API) += isadma.o
snd-$(CONFIG_SND_OSSEMUL) += sound_oss.o info_oss.o
snd-$(CONFIG_SND_VMASTER) += vmaster.o
snd-$(CONFIG_SND_KCTL_JACK) += ctljack.o
snd-$(CONFIG_SND_JACK)	  += jack.o

snd-pcm-objs := pcm.o pcm_native.o pcm_lib.o pcm_timer.o pcm_misc.o \
		pcm_memory.o

snd-page-alloc-y := memalloc.o
snd-page-alloc-$(CONFIG_SND_DMA_SGBUF) += sgbuf.o

snd-rawmidi-objs  := rawmidi.o
snd-timer-objs    := timer.o
snd-hrtimer-objs  := hrtimer.o
snd-rtctimer-objs := rtctimer.o
snd-hwdep-objs    := hwdep.o
=======
snd-y     := sound.o init.o memory.o control.o misc.o device.o
ifneq ($(CONFIG_SND_PROC_FS),)
snd-y += info.o
snd-$(CONFIG_SND_OSSEMUL) += info_oss.o
endif
snd-$(CONFIG_ISA_DMA_API) += isadma.o
snd-$(CONFIG_SND_OSSEMUL) += sound_oss.o
snd-$(CONFIG_SND_VMASTER) += vmaster.o
snd-$(CONFIG_SND_JACK)	  += ctljack.o jack.o

snd-pcm-y := pcm.o pcm_native.o pcm_lib.o pcm_misc.o \
		pcm_memory.o memalloc.o
snd-pcm-$(CONFIG_SND_PCM_TIMER) += pcm_timer.o
snd-pcm-$(CONFIG_SND_PCM_ELD) += pcm_drm_eld.o
snd-pcm-$(CONFIG_SND_PCM_IEC958) += pcm_iec958.o

# for trace-points
CFLAGS_pcm_lib.o := -I$(src)
CFLAGS_pcm_native.o := -I$(src)

snd-pcm-dmaengine-objs := pcm_dmaengine.o

snd-ctl-led-objs  := control_led.o
snd-rawmidi-objs  := rawmidi.o
snd-ump-objs      := ump.o
snd-ump-$(CONFIG_SND_UMP_LEGACY_RAWMIDI) += ump_convert.o
snd-timer-objs    := timer.o
snd-hrtimer-objs  := hrtimer.o
snd-hwdep-objs    := hwdep.o
snd-seq-device-objs := seq_device.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

snd-compress-objs := compress_offload.o

obj-$(CONFIG_SND) 		+= snd.o
<<<<<<< HEAD
obj-$(CONFIG_SND_HWDEP)		+= snd-hwdep.o
obj-$(CONFIG_SND_TIMER)		+= snd-timer.o
obj-$(CONFIG_SND_HRTIMER)	+= snd-hrtimer.o
obj-$(CONFIG_SND_RTCTIMER)	+= snd-rtctimer.o
obj-$(CONFIG_SND_PCM)		+= snd-pcm.o snd-page-alloc.o
obj-$(CONFIG_SND_RAWMIDI)	+= snd-rawmidi.o
=======
obj-$(CONFIG_SND_CTL_LED)	+= snd-ctl-led.o
obj-$(CONFIG_SND_HWDEP)		+= snd-hwdep.o
obj-$(CONFIG_SND_TIMER)		+= snd-timer.o
obj-$(CONFIG_SND_HRTIMER)	+= snd-hrtimer.o
obj-$(CONFIG_SND_PCM)		+= snd-pcm.o
obj-$(CONFIG_SND_DMAENGINE_PCM)	+= snd-pcm-dmaengine.o
obj-$(CONFIG_SND_SEQ_DEVICE)	+= snd-seq-device.o
obj-$(CONFIG_SND_RAWMIDI)	+= snd-rawmidi.o
obj-$(CONFIG_SND_UMP)		+= snd-ump.o

obj-$(CONFIG_SND_CORE_TEST)	+= sound_kunit.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_SND_OSSEMUL)	+= oss/
obj-$(CONFIG_SND_SEQUENCER)	+= seq/

obj-$(CONFIG_SND_COMPRESS_OFFLOAD)	+= snd-compress.o
