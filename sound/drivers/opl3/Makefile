<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for ALSA
# Copyright (c) 2001 by Jaroslav Kysela <perex@perex.cz>
#

snd-opl3-lib-objs := opl3_lib.o opl3_synth.o
snd-opl3-synth-y := opl3_seq.o opl3_midi.o opl3_drums.o
<<<<<<< HEAD
snd-opl3-synth-$(CONFIG_SND_SEQUENCER_OSS) += opl3_oss.o
=======
ifneq ($(CONFIG_SND_SEQUENCER_OSS),)
snd-opl3-synth-y += opl3_oss.o
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_SND_OPL3_LIB) += snd-opl3-lib.o
obj-$(CONFIG_SND_OPL4_LIB) += snd-opl3-lib.o
obj-$(CONFIG_SND_OPL3_LIB_SEQ) += snd-opl3-synth.o
