<<<<<<< HEAD


ifneq '$(CONFIG_OPENRISC_BUILTIN_DTB)' '""'
BUILTIN_DTB := $(patsubst "%",%,$(CONFIG_OPENRISC_BUILTIN_DTB)).dtb.o
else
BUILTIN_DTB :=
endif
obj-y += $(BUILTIN_DTB)

clean-files := *.dtb.S

#DTC_FLAGS ?= -p 1024

$(obj)/%.dtb: $(src)/dts/%.dts FORCE
	$(call if_changed_dep,dtc)
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for bootable kernel images
#

targets += vmlinux.bin

OBJCOPYFLAGS_vmlinux.bin := -O binary
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
