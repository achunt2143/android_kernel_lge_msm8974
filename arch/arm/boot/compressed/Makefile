<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# linux/arch/arm/boot/compressed/Makefile
#
# create a compressed vmlinuz image from the original vmlinux
#

OBJS		=

<<<<<<< HEAD
# Ensure that MMCIF loader code appears early in the image
# to minimise that number of bocks that have to be read in
# order to load it.
ifeq ($(CONFIG_ZBOOT_ROM_MMCIF),y)
OBJS		+= mmcif-sh7372.o
endif

# Ensure that SDHI loader code appears early in the image
# to minimise that number of bocks that have to be read in
# order to load it.
ifeq ($(CONFIG_ZBOOT_ROM_SH_MOBILE_SDHI),y)
OBJS		+= sdhi-shmobile.o
OBJS		+= sdhi-sh7372.o
endif

AFLAGS_head.o += -DTEXT_OFFSET=$(TEXT_OFFSET)
HEAD	= head.o
OBJS	+= misc.o decompress.o
ifeq ($(CONFIG_KERNEL_LZ4),y)
	CFLAGS_decompress.o := -Os
endif
FONTC	= $(srctree)/drivers/video/console/font_acorn_8x8.c
=======
HEAD	= head.o
OBJS	+= misc.o decompress.o
ifeq ($(CONFIG_DEBUG_UNCOMPRESS),y)
OBJS	+= debug.o
AFLAGS_head.o += -DDEBUG
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# string library code (-Os is enforced to keep it much smaller)
OBJS		+= string.o
CFLAGS_string.o	:= -Os

<<<<<<< HEAD
=======
ifeq ($(CONFIG_ARM_VIRT_EXT),y)
OBJS		+= hyp-stub.o
endif

GCOV_PROFILE		:= n
KASAN_SANITIZE		:= n

# Prevents link failures: __sanitizer_cov_trace_pc() is not linked in.
KCOV_INSTRUMENT		:= n
UBSAN_SANITIZE		:= n

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Architecture dependencies
#
ifeq ($(CONFIG_ARCH_ACORN),y)
OBJS		+= ll_char_wr.o font.o
endif

<<<<<<< HEAD
ifeq ($(CONFIG_ARCH_SHARK),y)
OBJS		+= head-shark.o ofw-shark.o
endif

ifeq ($(CONFIG_ARCH_P720T),y)
# Borrow this code from SA1100
OBJS		+= head-sa1100.o
endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ifeq ($(CONFIG_ARCH_SA1100),y)
OBJS		+= head-sa1100.o
endif

<<<<<<< HEAD
ifeq ($(CONFIG_ARCH_VT8500),y)
OBJS		+= head-vt8500.o
endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ifeq ($(CONFIG_CPU_XSCALE),y)
OBJS		+= head-xscale.o
endif

ifeq ($(CONFIG_PXA_SHARPSL_DETECT_MACH_ID),y)
OBJS		+= head-sharpsl.o
endif

ifeq ($(CONFIG_CPU_ENDIAN_BE32),y)
ifeq ($(CONFIG_CPU_CP15),y)
OBJS		+= big-endian.o
else
# The endian should be set by h/w design.
endif
endif

<<<<<<< HEAD
ifeq ($(CONFIG_ARCH_SHMOBILE),y)
OBJS		+= head-shmobile.o
endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# We now have a PIC decompressor implementation.  Decompressors running
# from RAM should not define ZTEXTADDR.  Decompressors running directly
# from ROM or Flash must define ZTEXTADDR (preferably via the config)
# FIXME: Previous assignment to ztextaddr-y is lost here. See SHARK
ifeq ($(CONFIG_ZBOOT_ROM),y)
ZTEXTADDR	:= $(CONFIG_ZBOOT_ROM_TEXT)
ZBSSADDR	:= $(CONFIG_ZBOOT_ROM_BSS)
else
ZTEXTADDR	:= 0
ZBSSADDR	:= ALIGN(8)
endif

<<<<<<< HEAD
SEDFLAGS	= s/TEXT_START/$(ZTEXTADDR)/;s/BSS_START/$(ZBSSADDR)/

suffix_$(CONFIG_KERNEL_GZIP) = gzip
suffix_$(CONFIG_KERNEL_LZO)  = lzo
suffix_$(CONFIG_KERNEL_LZMA) = lzma
suffix_$(CONFIG_KERNEL_XZ)   = xzkern
suffix_$(CONFIG_KERNEL_LZ4)   = lz4

# Borrowed libfdt files for the ATAG compatibility mode

libfdt		:= fdt_rw.c fdt_ro.c fdt_wip.c fdt.c
libfdt_hdrs	:= fdt.h libfdt.h libfdt_internal.h

libfdt_objs	:= $(addsuffix .o, $(basename $(libfdt)))

$(addprefix $(obj)/,$(libfdt) $(libfdt_hdrs)): $(obj)/%: $(srctree)/scripts/dtc/libfdt/%
	$(call cmd,shipped)

$(addprefix $(obj)/,$(libfdt_objs) atags_to_fdt.o): \
	$(addprefix $(obj)/,$(libfdt_hdrs))

ifeq ($(CONFIG_ARM_ATAG_DTB_COMPAT),y)
OBJS	+= $(libfdt_objs) atags_to_fdt.o
endif

targets       := vmlinux vmlinux.lds \
		 piggy.$(suffix_y) piggy.$(suffix_y).o \
		 lib1funcs.o lib1funcs.S ashldi3.o ashldi3.S \
		 font.o font.c head.o misc.o $(OBJS)

# Make sure files are removed during clean
extra-y       += piggy.gzip piggy.lzo piggy.lzma piggy.xzkern piggy.lz4 \
		 lib1funcs.S ashldi3.S $(libfdt) $(libfdt_hdrs)

ifeq ($(CONFIG_FUNCTION_TRACER),y)
ORIG_CFLAGS := $(KBUILD_CFLAGS)
KBUILD_CFLAGS = $(subst -pg, , $(ORIG_CFLAGS))
endif

ccflags-y := -fpic -fno-builtin -I$(obj)

# Supply kernel BSS size to the decompressor via a linker symbol.
KBSS_SZ = $(shell $(CROSS_COMPILE)size $(obj)/../../../../vmlinux | \
		awk 'END{print $$3}')
=======
MALLOC_SIZE	:= 65536

AFLAGS_head.o += -DTEXT_OFFSET=$(TEXT_OFFSET) -DMALLOC_SIZE=$(MALLOC_SIZE)
CPPFLAGS_vmlinux.lds := -DTEXT_START="$(ZTEXTADDR)" -DBSS_START="$(ZBSSADDR)"
CPPFLAGS_vmlinux.lds += -DTEXT_OFFSET="$(TEXT_OFFSET)"
CPPFLAGS_vmlinux.lds += -DMALLOC_SIZE="$(MALLOC_SIZE)"

compress-$(CONFIG_KERNEL_GZIP) = gzip
compress-$(CONFIG_KERNEL_LZO)  = lzo_with_size
compress-$(CONFIG_KERNEL_LZMA) = lzma_with_size
compress-$(CONFIG_KERNEL_XZ)   = xzkern_with_size
compress-$(CONFIG_KERNEL_LZ4)  = lz4_with_size

libfdt_objs := fdt_rw.o fdt_ro.o fdt_wip.o fdt.o

ifeq ($(CONFIG_ARM_ATAG_DTB_COMPAT),y)
CFLAGS_REMOVE_atags_to_fdt.o += -Wframe-larger-than=${CONFIG_FRAME_WARN}
CFLAGS_atags_to_fdt.o += -Wframe-larger-than=1280
OBJS	+= $(libfdt_objs) atags_to_fdt.o
endif
ifeq ($(CONFIG_USE_OF),y)
OBJS	+= $(libfdt_objs) fdt_check_mem_start.o
endif

OBJS	+= lib1funcs.o ashldi3.o bswapsdi2.o

targets       := vmlinux vmlinux.lds piggy_data piggy.o \
		 head.o $(OBJS)

KBUILD_CFLAGS += -DDISABLE_BRANCH_PROFILING

ccflags-y := -fpic $(call cc-option,-mno-single-pic-base,) -fno-builtin \
	     -I$(srctree)/scripts/dtc/libfdt -fno-stack-protector \
	     -I$(obj) $(DISABLE_ARM_SSP_PER_TASK_PLUGIN)
ccflags-remove-$(CONFIG_FUNCTION_TRACER) += -pg
asflags-y := -DZIMAGE

# Supply kernel BSS size to the decompressor via a linker symbol.
KBSS_SZ = $(shell echo $$(($$($(NM) vmlinux | \
		sed -n -e 's/^\([^ ]*\) [ABD] __bss_start$$/-0x\1/p' \
		       -e 's/^\([^ ]*\) [ABD] __bss_stop$$/+0x\1/p') )) )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
LDFLAGS_vmlinux = --defsym _kernel_bss_size=$(KBSS_SZ)
# Supply ZRELADDR to the decompressor via a linker symbol.
ifneq ($(CONFIG_AUTO_ZRELADDR),y)
LDFLAGS_vmlinux += --defsym zreladdr=$(ZRELADDR)
endif
ifeq ($(CONFIG_CPU_ENDIAN_BE8),y)
LDFLAGS_vmlinux += --be8
endif
<<<<<<< HEAD
# ?
LDFLAGS_vmlinux += -p
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Report unresolved symbol references
LDFLAGS_vmlinux += --no-undefined
# Delete all temporary local symbols
LDFLAGS_vmlinux += -X
<<<<<<< HEAD
# Next argument is a linker script
LDFLAGS_vmlinux += -T

# For __aeabi_uidivmod
lib1funcs = $(obj)/lib1funcs.o

$(obj)/lib1funcs.S: $(srctree)/arch/$(SRCARCH)/lib/lib1funcs.S
	$(call cmd,shipped)

# For __aeabi_llsl
ashldi3 = $(obj)/ashldi3.o

$(obj)/ashldi3.S: $(srctree)/arch/$(SRCARCH)/lib/ashldi3.S
	$(call cmd,shipped)

=======
# Report orphan sections
ifdef CONFIG_LD_ORPHAN_WARN
LDFLAGS_vmlinux += --orphan-handling=$(CONFIG_LD_ORPHAN_WARN_LEVEL)
endif
# Next argument is a linker script
LDFLAGS_vmlinux += -T

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# We need to prevent any GOTOFF relocs being used with references
# to symbols in the .bss section since we cannot relocate them
# independently from the rest at run time.  This can be achieved by
# ensuring that no private .bss symbols exist, as global symbols
# always have a GOT entry which is what we need.
# The .data section is already discarded by the linker script so no need
# to bother about it here.
check_for_bad_syms = \
<<<<<<< HEAD
bad_syms=$$($(CROSS_COMPILE)nm $@ | sed -n 's/^.\{8\} [bc] \(.*\)/\1/p') && \
[ -z "$$bad_syms" ] || \
  ( echo "following symbols must have non local/private scope:" >&2; \
    echo "$$bad_syms" >&2; rm -f $@; false )
=======
bad_syms=$$($(NM) $@ | sed -n 's/^.\{8\} [bc] \(.*\)/\1/p') && \
[ -z "$$bad_syms" ] || \
  ( echo "following symbols must have non local/private scope:" >&2; \
    echo "$$bad_syms" >&2; false )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

check_for_multiple_zreladdr = \
if [ $(words $(ZRELADDR)) -gt 1 -a "$(CONFIG_AUTO_ZRELADDR)" = "" ]; then \
	echo 'multiple zreladdrs: $(ZRELADDR)'; \
	echo 'This needs CONFIG_AUTO_ZRELADDR to be set'; \
	false; \
fi

<<<<<<< HEAD
$(obj)/vmlinux: $(obj)/vmlinux.lds $(obj)/$(HEAD) $(obj)/piggy.$(suffix_y).o \
		$(addprefix $(obj)/, $(OBJS)) $(lib1funcs) $(ashldi3) FORCE
=======
efi-obj-$(CONFIG_EFI_STUB) := $(objtree)/drivers/firmware/efi/libstub/lib.a

$(obj)/vmlinux: $(obj)/vmlinux.lds $(obj)/$(HEAD) $(obj)/piggy.o \
		$(addprefix $(obj)/, $(OBJS)) \
		$(efi-obj-y) FORCE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	@$(check_for_multiple_zreladdr)
	$(call if_changed,ld)
	@$(check_for_bad_syms)

<<<<<<< HEAD
$(obj)/piggy.$(suffix_y): $(obj)/../Image FORCE
	$(call if_changed,$(suffix_y))

$(obj)/piggy.$(suffix_y).o:  $(obj)/piggy.$(suffix_y) FORCE

CFLAGS_font.o := -Dstatic=

$(obj)/font.c: $(FONTC)
	$(call cmd,shipped)

$(obj)/vmlinux.lds: $(obj)/vmlinux.lds.in arch/arm/boot/Makefile $(KCONFIG_CONFIG)
	@sed "$(SEDFLAGS)" < $< > $@
=======
$(obj)/piggy_data: $(obj)/../Image FORCE
	$(call if_changed,$(compress-y))

$(obj)/piggy.o: $(obj)/piggy_data

CFLAGS_font.o := -Dstatic=
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
