<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# This file contains the font map for the default (hardware) font
#
FONTMAPFILE = cp437.uni

obj-$(CONFIG_VT)			+= vt_ioctl.o vc_screen.o \
<<<<<<< HEAD
					   selection.o keyboard.o
obj-$(CONFIG_CONSOLE_TRANSLATIONS)	+= consolemap.o consolemap_deftbl.o
obj-$(CONFIG_HW_CONSOLE)		+= vt.o defkeymap.o
=======
					   selection.o keyboard.o \
					   vt.o defkeymap.o
obj-$(CONFIG_CONSOLE_TRANSLATIONS)	+= consolemap.o consolemap_deftbl.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# Files generated that shall be removed upon make clean
clean-files := consolemap_deftbl.c defkeymap.c

<<<<<<< HEAD
quiet_cmd_conmk = CONMK   $@
      cmd_conmk = scripts/conmakehash $< > $@

$(obj)/consolemap_deftbl.c: $(src)/$(FONTMAPFILE)
=======
hostprogs += conmakehash

quiet_cmd_conmk = CONMK   $@
      cmd_conmk = $(obj)/conmakehash $< $(shell basename $<) > $@

$(obj)/consolemap_deftbl.c: $(src)/$(FONTMAPFILE) $(obj)/conmakehash
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	$(call cmd,conmk)

$(obj)/defkeymap.o:  $(obj)/defkeymap.c

# Uncomment if you're changing the keymap and have an appropriate
# loadkeys version for the map. By default, we'll use the shipped
# versions.
# GENERATE_KEYMAP := 1

ifdef GENERATE_KEYMAP

$(obj)/defkeymap.c: $(obj)/%.c: $(src)/%.map
<<<<<<< HEAD
	loadkeys --mktable $< > $@.tmp
	sed -e 's/^static *//' $@.tmp > $@
	rm $@.tmp
=======
	loadkeys --mktable --unicode $< > $@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

endif
