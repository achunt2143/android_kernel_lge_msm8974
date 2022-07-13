<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# linux/arch/sh/boot/compressed/Makefile
#
# create a compressed vmlinux image from the original vmlinux
#

<<<<<<< HEAD
targets		:= vmlinux vmlinux.bin vmlinux.bin.gz \
		   vmlinux.bin.bz2 vmlinux.bin.lzma \
		   vmlinux.bin.xz vmlinux.bin.lzo \
		   head_$(BITS).o misc.o piggy.o

OBJECTS = $(obj)/head_$(BITS).o $(obj)/misc.o $(obj)/cache.o

GCOV_PROFILE := n
=======
OBJECTS := head_32.o misc.o cache.o piggy.o \
           ashiftrt.o ashldi3.o ashrsi3.o ashlsi3.o lshrsi3.o

targets := vmlinux vmlinux.bin vmlinux.bin.gz vmlinux.bin.bz2 \
           vmlinux.bin.lzma vmlinux.bin.xz vmlinux.bin.lzo $(OBJECTS)

GCOV_PROFILE := n
UBSAN_SANITIZE := n
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#
# IMAGE_OFFSET is the load offset of the compression loader
#
ifeq ($(CONFIG_32BIT),y)
IMAGE_OFFSET	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_MEMORY_START)  + \
			$(CONFIG_BOOT_LINK_OFFSET)]')
else
IMAGE_OFFSET	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_PAGE_OFFSET)  + \
			$(KERNEL_MEMORY) + \
			$(CONFIG_BOOT_LINK_OFFSET)]')
endif

<<<<<<< HEAD
ifeq ($(CONFIG_MCOUNT),y)
ORIG_CFLAGS := $(KBUILD_CFLAGS)
KBUILD_CFLAGS = $(subst -pg, , $(ORIG_CFLAGS))
endif
=======
ccflags-remove-$(CONFIG_MCOUNT) += -pg
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

LDFLAGS_vmlinux := --oformat $(ld-bfd) -Ttext $(IMAGE_OFFSET) -e startup \
		   -T $(obj)/../../kernel/vmlinux.lds

<<<<<<< HEAD
#
# Pull in the necessary libgcc bits from the in-kernel implementation.
#
lib1funcs-$(CONFIG_SUPERH32)	:= ashiftrt.S ashldi3.c ashrsi3.S ashlsi3.S \
				   lshrsi3.S
lib1funcs-obj			:= \
	$(addsuffix .o, $(basename $(addprefix $(obj)/, $(lib1funcs-y))))

lib1funcs-dir		:= $(srctree)/arch/$(SRCARCH)/lib
ifeq ($(BITS),64)
	lib1funcs-dir 	:= $(addsuffix $(BITS), $(lib1funcs-dir))
endif

KBUILD_CFLAGS += -I$(lib1funcs-dir)

$(addprefix $(obj)/,$(lib1funcs-y)): $(obj)/%: $(lib1funcs-dir)/% FORCE
	$(call cmd,shipped)

$(obj)/vmlinux: $(OBJECTS) $(obj)/piggy.o $(lib1funcs-obj) FORCE
	$(call if_changed,ld)
	@:
=======
KBUILD_CFLAGS += -DDISABLE_BRANCH_PROFILING

$(obj)/vmlinux: $(addprefix $(obj)/, $(OBJECTS)) FORCE
	$(call if_changed,ld)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

<<<<<<< HEAD
vmlinux.bin.all-y := $(obj)/vmlinux.bin

$(obj)/vmlinux.bin.gz: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,gzip)
$(obj)/vmlinux.bin.bz2: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,bzip2)
$(obj)/vmlinux.bin.lzma: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,lzma)
$(obj)/vmlinux.bin.xz: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,xzkern)
$(obj)/vmlinux.bin.lzo: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,lzo)
=======
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)
$(obj)/vmlinux.bin.bz2: $(obj)/vmlinux.bin FORCE
	$(call if_changed,bzip2_with_size)
$(obj)/vmlinux.bin.lzma: $(obj)/vmlinux.bin FORCE
	$(call if_changed,lzma_with_size)
$(obj)/vmlinux.bin.xz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,xzkern_with_size)
$(obj)/vmlinux.bin.lzo: $(obj)/vmlinux.bin FORCE
	$(call if_changed,lzo_with_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

OBJCOPYFLAGS += -R .empty_zero_page

LDFLAGS_piggy.o := -r --format binary --oformat $(ld-bfd) -T

<<<<<<< HEAD
$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.$(suffix-y) FORCE
=======
$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.$(suffix_y) FORCE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	$(call if_changed,ld)
