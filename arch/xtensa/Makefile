#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2001 - 2005  Tensilica Inc.
<<<<<<< HEAD
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
=======
# Copyright (C) 2014 Cadence Design Systems Inc.
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# Core configuration.
# (Use VAR=<xtensa_config> to use another default compiler.)

<<<<<<< HEAD
variant-$(CONFIG_XTENSA_VARIANT_FSF)		:= fsf
variant-$(CONFIG_XTENSA_VARIANT_DC232B)		:= dc232b
variant-$(CONFIG_XTENSA_VARIANT_S6000)		:= s6000
variant-$(CONFIG_XTENSA_VARIANT_LINUX_CUSTOM)	:= custom

VARIANT = $(variant-y)
export VARIANT

# Platform configuration

platform-$(CONFIG_XTENSA_PLATFORM_XT2000)	:= xt2000
platform-$(CONFIG_XTENSA_PLATFORM_ISS)		:= iss
platform-$(CONFIG_XTENSA_PLATFORM_S6105)	:= s6105

PLATFORM = $(platform-y)
export PLATFORM

# temporarily until string.h is fixed
KBUILD_CFLAGS += -ffreestanding

KBUILD_CFLAGS += -pipe -mlongcalls

KBUILD_CFLAGS += $(call cc-option,-mforce-no-pic,)

vardirs := $(patsubst %,arch/xtensa/variants/%/,$(variant-y))
plfdirs := $(patsubst %,arch/xtensa/platforms/%/,$(platform-y))

ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(vardirs) $(plfdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(vardirs) $(plfdirs))
endif

KBUILD_DEFCONFIG := iss_defconfig

# ramdisk/initrd support
# You need a compressed ramdisk image, named ramdisk.gz in
# arch/xtensa/boot/ramdisk

core-$(CONFIG_EMBEDDED_RAMDISK)	+= arch/xtensa/boot/ramdisk/

# Test for cross compiling

ifneq ($(VARIANT),)
  COMPILE_ARCH = $(shell uname -m)

  ifneq ($(COMPILE_ARCH), xtensa)
=======
variant-y := $(CONFIG_XTENSA_VARIANT_NAME)

VARIANT = $(variant-y)

ifneq ($(VARIANT),)
  ifdef cross_compiling
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    ifndef CROSS_COMPILE
      CROSS_COMPILE = xtensa_$(VARIANT)-
    endif
  endif
endif

<<<<<<< HEAD
# Only build variant and/or platform if it includes a Makefile

buildvar := $(shell test -a $(srctree)/arch/xtensa/variants/$(VARIANT)/Makefile && echo arch/xtensa/variants/$(VARIANT)/)
buildplf := $(shell test -a $(srctree)/arch/xtensa/platforms/$(PLATFORM)/Makefile && echo arch/xtensa/platforms/$(PLATFORM)/)

# Find libgcc.a

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y		:= arch/xtensa/kernel/head.o
core-y		+= arch/xtensa/kernel/ arch/xtensa/mm/
core-y		+= $(buildvar) $(buildplf)

libs-y		+= arch/xtensa/lib/ $(LIBGCC)

boot		:= arch/xtensa/boot

all: zImage

bzImage : zImage

zImage zImage.initrd: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

define archhelp
  @echo '* zImage      - Compressed kernel image (arch/xtensa/boot/images/zImage.*)'
endef

=======
# Platform configuration

platform-$(CONFIG_XTENSA_PLATFORM_XT2000)	:= xt2000
platform-$(CONFIG_XTENSA_PLATFORM_ISS)		:= iss
platform-$(CONFIG_XTENSA_PLATFORM_XTFPGA)	:= xtfpga

# temporarily until string.h is fixed
KBUILD_CFLAGS += -ffreestanding -D__linux__
KBUILD_CFLAGS += -pipe -mlongcalls -mtext-section-literals
KBUILD_CFLAGS += $(call cc-option,-mforce-no-pic,)
KBUILD_CFLAGS += $(call cc-option,-mno-serialize-volatile,)
KBUILD_CFLAGS += $(call cc-option,-mno-fdpic,)
ifneq ($(CONFIG_KERNEL_ABI_CALL0),)
KBUILD_CFLAGS += -mabi=call0
KBUILD_AFLAGS += -mabi=call0
endif

KBUILD_AFLAGS += -mlongcalls -mtext-section-literals
KBUILD_AFLAGS += $(call cc-option,-mno-fdpic,)

KBUILD_LDFLAGS += -m elf32xtensa

ifneq ($(CONFIG_LD_NO_RELAX),)
KBUILD_LDFLAGS += --no-relax
endif

CHECKFLAGS += -D$(if $(CONFIG_CPU_BIG_ENDIAN),__XTENSA_EB__,__XTENSA_EL__)

vardirs := $(patsubst %,arch/xtensa/variants/%/,$(variant-y))
plfdirs := $(patsubst %,arch/xtensa/platforms/%/,$(platform-y))

KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(vardirs) $(plfdirs))

KBUILD_DEFCONFIG := iss_defconfig

libs-y		+= arch/xtensa/lib/

boot		:= arch/xtensa/boot

all Image zImage uImage xipImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

archheaders:
	$(Q)$(MAKE) $(build)=arch/xtensa/kernel/syscalls all

define archhelp
  @echo '* Image       - Kernel ELF image with reset vector'
  @echo '* zImage      - Compressed kernel image (arch/xtensa/boot/images/zImage.*)'
  @echo '* uImage      - U-Boot wrapped image'
  @echo '  xipImage    - XIP image'
endef
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
