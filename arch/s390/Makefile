<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# s390/Makefile
#
# This file is included by the global makefile so that you can add your own
<<<<<<< HEAD
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
=======
# architecture-specific flags and dependencies.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Copyright (C) 1994 by Linus Torvalds
#

<<<<<<< HEAD
ifndef CONFIG_64BIT
LD_BFD		:= elf32-s390
LDFLAGS		:= -m elf_s390
KBUILD_CFLAGS	+= -m31
KBUILD_AFLAGS	+= -m31
UTS_MACHINE	:= s390
STACK_SIZE	:= 8192
CHECKFLAGS	+= -D__s390__ -msize-long
else
LD_BFD		:= elf64-s390
LDFLAGS		:= -m elf64_s390
KBUILD_AFLAGS_MODULE += -fpic -D__PIC__
KBUILD_CFLAGS_MODULE += -fpic -D__PIC__
KBUILD_CFLAGS	+= -m64
KBUILD_AFLAGS	+= -m64
UTS_MACHINE	:= s390x
STACK_SIZE	:= 16384
CHECKFLAGS	+= -D__s390__ -D__s390x__
endif

export LD_BFD

cflags-$(CONFIG_MARCH_G5)   += $(call cc-option,-march=g5)
cflags-$(CONFIG_MARCH_Z900) += $(call cc-option,-march=z900)
cflags-$(CONFIG_MARCH_Z990) += $(call cc-option,-march=z990)
cflags-$(CONFIG_MARCH_Z9_109) += $(call cc-option,-march=z9-109)
cflags-$(CONFIG_MARCH_Z10) += $(call cc-option,-march=z10)
cflags-$(CONFIG_MARCH_Z196) += $(call cc-option,-march=z196)

#KBUILD_IMAGE is necessary for make rpm
KBUILD_IMAGE	:=arch/s390/boot/image
=======
LD_BFD		:= elf64-s390
KBUILD_LDFLAGS	:= -m elf64_s390
KBUILD_AFLAGS_MODULE += -fPIC
KBUILD_CFLAGS_MODULE += -fPIC
KBUILD_AFLAGS	+= -m64
KBUILD_CFLAGS	+= -m64
ifdef CONFIG_PIE_BUILD
KBUILD_CFLAGS	+= -fPIE
LDFLAGS_vmlinux	:= -pie -z notext
else
KBUILD_CFLAGS	+= $(call cc-option,-munaligned-symbols,)
LDFLAGS_vmlinux	:= --emit-relocs --discard-none
extra_tools	:= relocs
endif
aflags_dwarf	:= -Wa,-gdwarf-2
KBUILD_AFLAGS_DECOMPRESSOR := $(CLANG_FLAGS) -m64 -D__ASSEMBLY__
ifndef CONFIG_AS_IS_LLVM
KBUILD_AFLAGS_DECOMPRESSOR += $(if $(CONFIG_DEBUG_INFO),$(aflags_dwarf))
endif
KBUILD_CFLAGS_DECOMPRESSOR := $(CLANG_FLAGS) -m64 -O2 -mpacked-stack
KBUILD_CFLAGS_DECOMPRESSOR += -DDISABLE_BRANCH_PROFILING -D__NO_FORTIFY
KBUILD_CFLAGS_DECOMPRESSOR += -D__DECOMPRESSOR
KBUILD_CFLAGS_DECOMPRESSOR += -fno-delete-null-pointer-checks -msoft-float -mbackchain
KBUILD_CFLAGS_DECOMPRESSOR += -fno-asynchronous-unwind-tables
KBUILD_CFLAGS_DECOMPRESSOR += -ffreestanding
KBUILD_CFLAGS_DECOMPRESSOR += -fno-stack-protector
KBUILD_CFLAGS_DECOMPRESSOR += -fPIE
KBUILD_CFLAGS_DECOMPRESSOR += $(call cc-disable-warning, address-of-packed-member)
KBUILD_CFLAGS_DECOMPRESSOR += $(if $(CONFIG_DEBUG_INFO),-g)
KBUILD_CFLAGS_DECOMPRESSOR += $(if $(CONFIG_DEBUG_INFO_DWARF4), $(call cc-option, -gdwarf-4,))
KBUILD_CFLAGS_DECOMPRESSOR += $(if $(CONFIG_CC_NO_ARRAY_BOUNDS),-Wno-array-bounds)

UTS_MACHINE	:= s390x
STACK_SIZE	:= $(if $(CONFIG_KASAN),65536,16384)
CHECKFLAGS	+= -D__s390__ -D__s390x__

export LD_BFD

mflags-$(CONFIG_MARCH_Z10)    := -march=z10
mflags-$(CONFIG_MARCH_Z196)   := -march=z196
mflags-$(CONFIG_MARCH_ZEC12)  := -march=zEC12
mflags-$(CONFIG_MARCH_Z13)    := -march=z13
mflags-$(CONFIG_MARCH_Z14)    := -march=z14
mflags-$(CONFIG_MARCH_Z15)    := -march=z15
mflags-$(CONFIG_MARCH_Z16)    := -march=z16

export CC_FLAGS_MARCH := $(mflags-y)

aflags-y += $(mflags-y)
cflags-y += $(mflags-y)

cflags-$(CONFIG_MARCH_Z10_TUNE)		+= -mtune=z10
cflags-$(CONFIG_MARCH_Z196_TUNE)	+= -mtune=z196
cflags-$(CONFIG_MARCH_ZEC12_TUNE)	+= -mtune=zEC12
cflags-$(CONFIG_MARCH_Z13_TUNE)		+= -mtune=z13
cflags-$(CONFIG_MARCH_Z14_TUNE)		+= -mtune=z14
cflags-$(CONFIG_MARCH_Z15_TUNE)		+= -mtune=z15
cflags-$(CONFIG_MARCH_Z16_TUNE)		+= -mtune=z16

cflags-y += -Wa,-I$(srctree)/arch/$(ARCH)/include
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#
# Prevent tail-call optimizations, to get clearer backtraces:
#
cflags-$(CONFIG_FRAME_POINTER) += -fno-optimize-sibling-calls

<<<<<<< HEAD
# old style option for packed stacks
ifeq ($(call cc-option-yn,-mkernel-backchain),y)
cflags-$(CONFIG_PACK_STACK)  += -mkernel-backchain -D__PACK_STACK
aflags-$(CONFIG_PACK_STACK)  += -D__PACK_STACK
cflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
aflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
ifdef CONFIG_SMALL_STACK
STACK_SIZE := $(shell echo $$(($(STACK_SIZE)/2)) )
endif
endif

# new style option for packed stacks
ifeq ($(call cc-option-yn,-mpacked-stack),y)
cflags-$(CONFIG_PACK_STACK)  += -mpacked-stack -D__PACK_STACK
aflags-$(CONFIG_PACK_STACK)  += -D__PACK_STACK
cflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
aflags-$(CONFIG_SMALL_STACK) += -D__SMALL_STACK
ifdef CONFIG_SMALL_STACK
STACK_SIZE := $(shell echo $$(($(STACK_SIZE)/2)) )
endif
endif

ifeq ($(call cc-option-yn,-mstack-size=8192 -mstack-guard=128),y)
cflags-$(CONFIG_CHECK_STACK) += -mstack-size=$(STACK_SIZE)
ifneq ($(call cc-option-yn,-mstack-size=8192),y)
cflags-$(CONFIG_CHECK_STACK) += -mstack-guard=$(CONFIG_STACK_GUARD)
endif
endif

ifeq ($(call cc-option-yn,-mwarn-dynamicstack),y)
cflags-$(CONFIG_WARN_DYNAMIC_STACK) += -mwarn-dynamicstack
endif

KBUILD_CFLAGS	+= -mbackchain -msoft-float $(cflags-y)
KBUILD_CFLAGS	+= -pipe -fno-strength-reduce -Wno-sign-compare
KBUILD_AFLAGS	+= $(aflags-y)

OBJCOPYFLAGS	:= -O binary

head-y		:= arch/s390/kernel/head.o
head-y		+= arch/s390/kernel/$(if $(CONFIG_64BIT),head64.o,head31.o)
head-y		+= arch/s390/kernel/init_task.o

# See arch/s390/Kbuild for content of core part of the kernel
core-y		+= arch/s390/

libs-y		+= arch/s390/lib/
drivers-y	+= drivers/s390/

# must be linked after kernel
drivers-$(CONFIG_OPROFILE)	+= arch/s390/oprofile/

boot		:= arch/s390/boot

all: image bzImage

install: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

image bzImage: vmlinux
=======
KBUILD_AFLAGS_DECOMPRESSOR += $(aflags-y)
KBUILD_CFLAGS_DECOMPRESSOR += $(cflags-y)

ifneq ($(call cc-option,-mstack-size=8192 -mstack-guard=128),)
  CC_FLAGS_CHECK_STACK := -mstack-size=$(STACK_SIZE)
  ifeq ($(call cc-option,-mstack-size=8192),)
    CC_FLAGS_CHECK_STACK += -mstack-guard=$(CONFIG_STACK_GUARD)
  endif
  export CC_FLAGS_CHECK_STACK
  cflags-$(CONFIG_CHECK_STACK) += $(CC_FLAGS_CHECK_STACK)
endif

ifdef CONFIG_EXPOLINE
  ifdef CONFIG_EXPOLINE_EXTERN
    KBUILD_LDFLAGS_MODULE += arch/s390/lib/expoline/expoline.o
    CC_FLAGS_EXPOLINE := -mindirect-branch=thunk-extern
    CC_FLAGS_EXPOLINE += -mfunction-return=thunk-extern
  else
    CC_FLAGS_EXPOLINE := -mindirect-branch=thunk
    CC_FLAGS_EXPOLINE += -mfunction-return=thunk
  endif
  CC_FLAGS_EXPOLINE += -mindirect-branch-table
  export CC_FLAGS_EXPOLINE
  cflags-y += $(CC_FLAGS_EXPOLINE) -DCC_USING_EXPOLINE
  aflags-y += -DCC_USING_EXPOLINE
endif

ifdef CONFIG_FUNCTION_TRACER
  ifeq ($(call cc-option,-mfentry -mnop-mcount),)
    # make use of hotpatch feature if the compiler supports it
    cc_hotpatch	:= -mhotpatch=0,3
    ifneq ($(call cc-option,$(cc_hotpatch)),)
      CC_FLAGS_FTRACE := $(cc_hotpatch)
      KBUILD_AFLAGS	+= -DCC_USING_HOTPATCH
      KBUILD_CFLAGS	+= -DCC_USING_HOTPATCH
    endif
  endif
endif

# Test CFI features of binutils
cfi := $(call as-instr,.cfi_startproc\n.cfi_val_offset 15$(comma)-160\n.cfi_endproc,-DCONFIG_AS_CFI_VAL_OFFSET=1)

KBUILD_CFLAGS	+= -mpacked-stack -mbackchain -msoft-float $(cflags-y)
KBUILD_CFLAGS	+= -pipe -Wno-sign-compare
KBUILD_CFLAGS	+= -fno-asynchronous-unwind-tables $(cfi)
KBUILD_AFLAGS	+= $(aflags-y) $(cfi)
export KBUILD_AFLAGS_DECOMPRESSOR
export KBUILD_CFLAGS_DECOMPRESSOR

OBJCOPYFLAGS	:= -O binary

libs-y		+= arch/s390/lib/

boot		:= arch/s390/boot
syscalls	:= arch/s390/kernel/syscalls
tools		:= arch/s390/tools

all: bzImage

#KBUILD_IMAGE is necessary for packaging targets like rpm-pkg, deb-pkg...
KBUILD_IMAGE	:= $(boot)/bzImage

install:
	$(call cmd,install)

bzImage: vmlinux
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

zfcpdump:
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

<<<<<<< HEAD
vdso_install:
ifeq ($(CONFIG_64BIT),y)
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso64 $@
endif
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso32 $@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

# Don't use tabs in echo arguments
define archhelp
  echo  '* image           - Kernel image for IPL ($(boot)/image)'
  echo	'* bzImage         - Compressed kernel image for IPL ($(boot)/bzImage)'
=======
archheaders:
	$(Q)$(MAKE) $(build)=$(syscalls) uapi

archprepare:
	$(Q)$(MAKE) $(build)=$(syscalls) kapi
	$(Q)$(MAKE) $(build)=$(tools) kapi $(extra_tools)
ifeq ($(KBUILD_EXTMOD),)
# We need to generate vdso-offsets.h before compiling certain files in kernel/.
# In order to do that, we should use the archprepare target, but we can't since
# asm-offsets.h is included in some files used to generate vdso-offsets.h, and
# asm-offsets.h is built in prepare0, for which archprepare is a dependency.
# Therefore we need to generate the header after prepare0 has been made, hence
# this hack.
prepare: vdso_prepare
vdso_prepare: prepare0
	$(Q)$(MAKE) $(build)=arch/s390/kernel/vdso64 include/generated/vdso64-offsets.h
	$(if $(CONFIG_COMPAT),$(Q)$(MAKE) \
		$(build)=arch/s390/kernel/vdso32 include/generated/vdso32-offsets.h)

vdso-install-y			+= arch/s390/kernel/vdso64/vdso64.so.dbg
vdso-install-$(CONFIG_COMPAT)	+= arch/s390/kernel/vdso32/vdso32.so.dbg

ifdef CONFIG_EXPOLINE_EXTERN
modules_prepare: expoline_prepare
expoline_prepare: scripts
	$(Q)$(MAKE) $(build)=arch/s390/lib/expoline arch/s390/lib/expoline/expoline.o
endif
endif

# Don't use tabs in echo arguments
define archhelp
  echo	'* bzImage         - Kernel image for IPL ($(boot)/bzImage)'
  echo	'  install         - Install kernel using'
  echo	'                    (your) ~/bin/$(INSTALLKERNEL) or'
  echo	'                    (distribution) /sbin/$(INSTALLKERNEL) or'
  echo	'                    install to $$(INSTALL_PATH)'
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endef
