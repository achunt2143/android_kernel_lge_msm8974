<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for ALSA
# Copyright (c) 2001 by Jaroslav Kysela <perex@perex.cz>
#

snd-emux-synth-objs := emux.o emux_synth.o emux_seq.o emux_nrpn.o \
<<<<<<< HEAD
		       emux_effect.o emux_proc.o emux_hwdep.o soundfont.o \
		       $(if $(CONFIG_SND_SEQUENCER_OSS),emux_oss.o)

# Toplevel Module Dependencies
obj-$(CONFIG_SND_SBAWE_SEQ) += snd-emux-synth.o
obj-$(CONFIG_SND_EMU10K1_SEQ) += snd-emux-synth.o
=======
		       emux_effect.o emux_hwdep.o soundfont.o
snd-emux-synth-$(CONFIG_SND_PROC_FS) += emux_proc.o
ifneq ($(CONFIG_SND_SEQUENCER_OSS),)
snd-emux-synth-y += emux_oss.o
endif

obj-$(CONFIG_SND_SYNTH_EMUX) += snd-emux-synth.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
