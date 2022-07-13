<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# sparc/Makefile
#
# Makefile for the architecture dependent flags and dependencies on the
# Sparc and sparc64.
#
# Copyright (C) 1994,1996,1998 David S. Miller (davem@caip.rutgers.edu)
# Copyright (C) 1998 Jakub Jelinek (jj@ultra.linux.cz)

# We are not yet configured - so test on arch
<<<<<<< HEAD
ifeq ($(ARCH),sparc)
        KBUILD_DEFCONFIG := sparc32_defconfig
else
        KBUILD_DEFCONFIG := sparc64_defconfig
=======
ifeq ($(ARCH),sparc64)
        KBUILD_DEFCONFIG := sparc64_defconfig
else
        KBUILD_DEFCONFIG := sparc32_defconfig
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif

ifeq ($(CONFIG_SPARC32),y)
#####
# sparc32
#

<<<<<<< HEAD
#
# Uncomment the first KBUILD_CFLAGS if you are doing kgdb source level
# debugging of the kernel to get the proper debugging information.

AS             := $(AS) -32
LDFLAGS        := -m elf32_sparc
CHECKFLAGS     += -D__sparc__
export BITS    := 32
UTS_MACHINE    := sparc

#KBUILD_CFLAGS += -g -pipe -fcall-used-g5 -fcall-used-g7
KBUILD_CFLAGS += -m32 -pipe -mno-fpu -fcall-used-g5 -fcall-used-g7
KBUILD_AFLAGS += -m32 -Wa,-Av8

#LDFLAGS_vmlinux = -N -Ttext 0xf0004000
#  Since 2.5.40, the first stage is left not btfix-ed.
#  Actual linking is done with "make image".
LDFLAGS_vmlinux = -r
=======
CHECKFLAGS     += -D__sparc__
KBUILD_LDFLAGS := -m elf32_sparc
export BITS    := 32
UTS_MACHINE    := sparc

# We are adding -Wa,-Av8 to KBUILD_CFLAGS to deal with a specs bug in some
# versions of gcc.  Some gcc versions won't pass -Av8 to binutils when you
# give -mcpu=v8.  This silently worked with older bintutils versions but
# does not any more.
KBUILD_CFLAGS  += -m32 -mcpu=v8 -pipe -mno-fpu -fcall-used-g5 -fcall-used-g7
KBUILD_CFLAGS  += -Wa,-Av8

KBUILD_AFLAGS  += -m32 -Wa,-Av8
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

else
#####
# sparc64
#

<<<<<<< HEAD
CHECKFLAGS      += -D__sparc__ -D__sparc_v9__ -D__arch64__ -m64

LDFLAGS              := -m elf64_sparc
export BITS          := 64
UTS_MACHINE          := sparc64

KBUILD_CFLAGS += -m64 -pipe -mno-fpu -mcpu=ultrasparc -mcmodel=medlow   \
                 -ffixed-g4 -ffixed-g5 -fcall-used-g7 -Wno-sign-compare \
                 -Wa,--undeclared-regs
=======
CHECKFLAGS    += -D__sparc__ -D__sparc_v9__ -D__arch64__
KBUILD_LDFLAGS := -m elf64_sparc
export BITS   := 64
UTS_MACHINE   := sparc64

KBUILD_CFLAGS += -m64 -pipe -mno-fpu -mcpu=ultrasparc -mcmodel=medlow
KBUILD_CFLAGS += -ffixed-g4 -ffixed-g5 -fcall-used-g7 -Wno-sign-compare
KBUILD_CFLAGS += -Wa,--undeclared-regs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
KBUILD_CFLAGS += $(call cc-option,-mtune=ultrasparc3)
KBUILD_AFLAGS += -m64 -mcpu=ultrasparc -Wa,--undeclared-regs

ifeq ($(CONFIG_MCOUNT),y)
  KBUILD_CFLAGS += -pg
endif

endif

<<<<<<< HEAD
head-y                 := arch/sparc/kernel/head_$(BITS).o
head-y                 += arch/sparc/kernel/init_task.o

core-y                 += arch/sparc/kernel/
core-y                 += arch/sparc/mm/ arch/sparc/math-emu/

libs-y                 += arch/sparc/prom/
libs-y                 += arch/sparc/lib/

drivers-$(CONFIG_OPROFILE)	+= arch/sparc/oprofile/

# Export what is needed by arch/sparc/boot/Makefile
export VMLINUX_INIT VMLINUX_MAIN
VMLINUX_INIT := $(head-y) $(init-y)
VMLINUX_MAIN := $(core-y) kernel/ mm/ fs/ ipc/ security/ crypto/ block/
VMLINUX_MAIN += $(patsubst %/, %/lib.a, $(libs-y)) $(libs-y)
VMLINUX_MAIN += $(drivers-y) $(net-y)

ifdef CONFIG_KALLSYMS
export kallsyms.o := .tmp_kallsyms2.o
endif
=======
libs-y                 += arch/sparc/prom/
libs-y                 += arch/sparc/lib/

drivers-$(CONFIG_PM) += arch/sparc/power/
drivers-$(CONFIG_FB_CORE) += arch/sparc/video/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

boot := arch/sparc/boot

# Default target
all: zImage

image zImage uImage tftpboot.img vmlinux.aout: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

<<<<<<< HEAD
archclean:
	$(Q)$(MAKE) $(clean)=$(boot)
=======
install:
	$(call cmd,install)

archheaders:
	$(Q)$(MAKE) $(build)=arch/sparc/kernel/syscalls all

vdso-install-$(CONFIG_SPARC64)	+= arch/sparc/vdso/vdso64.so.dbg
vdso-install-$(CONFIG_COMPAT)	+= arch/sparc/vdso/vdso32.so.dbg
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# This is the image used for packaging
KBUILD_IMAGE := $(boot)/zImage

# Don't use tabs in echo arguments.
<<<<<<< HEAD
ifeq ($(ARCH),sparc)
define archhelp
  echo  '* image        - kernel image ($(boot)/image)'
  echo  '* zImage       - stripped kernel image ($(boot)/zImage)'
  echo  '  uImage       - U-Boot SPARC32 Image (only for LEON)'
  echo  '  tftpboot.img - image prepared for tftp'
endef
else
define archhelp
  echo  '* vmlinux      - standard sparc64 kernel'
  echo  '* zImage       - stripped and compressed sparc64 kernel ($(boot)/zImage)'
  echo  '  vmlinux.aout - a.out kernel for sparc64'
  echo  '  tftpboot.img - image prepared for tftp'
endef
endif
=======
define archhelp
  echo  '* vmlinux      - standard SPARC kernel'
  echo  '  image        - kernel image ($(boot)/image)'
  echo  '* zImage       - stripped/compressed kernel image ($(boot)/zImage)'
  echo  '  uImage       - U-Boot SPARC32 Image (only for LEON)'
  echo  '  vmlinux.aout - a.out kernel for SPARC64'
  echo  '  tftpboot.img - image prepared for tftp'
endef
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
