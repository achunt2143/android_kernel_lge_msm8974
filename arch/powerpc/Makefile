# This file is included by the global makefile so that you can add your own
<<<<<<< HEAD
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture.
=======
# architecture-specific flags and dependencies.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
# Changes for PPC by Gary Thomas
# Rewritten by Cort Dougan and Paul Mackerras
#

<<<<<<< HEAD
=======
ifdef cross_compiling
  ifeq ($(CROSS_COMPILE),)
    # Auto detect cross compiler prefix.
    # Look for: (powerpc(64(le)?)?)(-unknown)?-linux(-gnu)?-
    CC_ARCHES := powerpc powerpc64 powerpc64le
    CC_SUFFIXES := linux linux-gnu unknown-linux-gnu
    CROSS_COMPILE := $(call cc-cross-prefix, $(foreach a,$(CC_ARCHES), \
                       $(foreach s,$(CC_SUFFIXES),$(a)-$(s)-)))
  endif
endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
HAS_BIARCH	:= $(call cc-option-yn, -m32)

# Set default 32 bits cross compilers for vdso and boot wrapper
CROSS32_COMPILE ?=

<<<<<<< HEAD
CROSS32CC		:= $(CROSS32_COMPILE)gcc
CROSS32AR		:= $(CROSS32_COMPILE)ar

ifeq ($(HAS_BIARCH),y)
ifeq ($(CROSS32_COMPILE),)
CROSS32CC	:= $(CC) -m32
CROSS32AR	:= GNUTARGET=elf32-powerpc $(AR)
endif
endif

export CROSS32CC CROSS32AR

ifeq ($(CROSS_COMPILE),)
KBUILD_DEFCONFIG := $(shell uname -m)_defconfig
else
KBUILD_DEFCONFIG := ppc64_defconfig
endif

ifeq ($(CONFIG_PPC64),y)
OLDARCH	:= ppc64
=======
# If we're on a ppc/ppc64/ppc64le machine use that defconfig, otherwise just use
# ppc64le_defconfig because we have nothing better to go on.
uname := $(shell uname -m)
KBUILD_DEFCONFIG := $(if $(filter ppc%,$(uname)),$(uname),ppc64le)_defconfig
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

new_nm := $(shell if $(NM) --help 2>&1 | grep -- '--synthetic' > /dev/null; then echo y; else echo n; fi)

ifeq ($(new_nm),y)
NM		:= $(NM) --synthetic
endif

<<<<<<< HEAD
else
OLDARCH	:= ppc
endif

# It seems there are times we use this Makefile without
# including the config file, but this replicates the old behaviour
ifeq ($(CONFIG_WORD_SIZE),)
CONFIG_WORD_SIZE := 32
endif

UTS_MACHINE := $(OLDARCH)

ifeq ($(HAS_BIARCH),y)
override AS	+= -a$(CONFIG_WORD_SIZE)
override LD	+= -m elf$(CONFIG_WORD_SIZE)ppc
override CC	+= -m$(CONFIG_WORD_SIZE)
override AR	:= GNUTARGET=elf$(CONFIG_WORD_SIZE)-powerpc $(AR)
=======
# BITS is used as extension for files which are available in a 32 bit
# and a 64 bit version to simplify shared Makefiles.
# e.g.: obj-y += foo_$(BITS).o
export BITS

ifdef CONFIG_PPC64
        BITS := 64
else
        BITS := 32
endif

machine-y = ppc
machine-$(CONFIG_PPC64) += 64
machine-$(CONFIG_CPU_LITTLE_ENDIAN) += le
UTS_MACHINE := $(subst $(space),,$(machine-y))

ifeq ($(CONFIG_PPC64)$(CONFIG_LD_IS_BFD),yy)
# Have the linker provide sfpr if possible.
# There is a corresponding test in arch/powerpc/lib/Makefile
KBUILD_LDFLAGS_MODULE += --save-restore-funcs
else
KBUILD_LDFLAGS_MODULE += arch/powerpc/lib/crtsavres.o
endif

ifdef CONFIG_CPU_LITTLE_ENDIAN
KBUILD_CFLAGS	+= -mlittle-endian
KBUILD_LDFLAGS	+= -EL
LDEMULATION	:= lppc
GNUTARGET	:= powerpcle
MULTIPLEWORD	:= -mno-multiple
KBUILD_CFLAGS_MODULE += $(call cc-option,-mno-save-toc-indirect)
else
KBUILD_CFLAGS += $(call cc-option,-mbig-endian)
KBUILD_LDFLAGS	+= -EB
LDEMULATION	:= ppc
GNUTARGET	:= powerpc
MULTIPLEWORD	:= -mmultiple
endif

ifdef CONFIG_PPC64
ifndef CONFIG_CC_IS_CLANG
cflags-$(CONFIG_PPC64_ELF_ABI_V1)	+= $(call cc-option,-mabi=elfv1)
cflags-$(CONFIG_PPC64_ELF_ABI_V1)	+= $(call cc-option,-mcall-aixdesc)
aflags-$(CONFIG_PPC64_ELF_ABI_V1)	+= $(call cc-option,-mabi=elfv1)
aflags-$(CONFIG_PPC64_ELF_ABI_V2)	+= -mabi=elfv2
endif
endif

ifndef CONFIG_CC_IS_CLANG
  cflags-$(CONFIG_CPU_LITTLE_ENDIAN)	+= -mno-strict-align
endif

cflags-$(CONFIG_CPU_BIG_ENDIAN)		+= $(call cc-option,-mbig-endian)
cflags-$(CONFIG_CPU_LITTLE_ENDIAN)	+= -mlittle-endian
aflags-$(CONFIG_CPU_BIG_ENDIAN)		+= $(call cc-option,-mbig-endian)
aflags-$(CONFIG_CPU_LITTLE_ENDIAN)	+= -mlittle-endian

ifeq ($(HAS_BIARCH),y)
KBUILD_CFLAGS	+= -m$(BITS)
KBUILD_AFLAGS	+= -m$(BITS)
KBUILD_LDFLAGS	+= -m elf$(BITS)$(LDEMULATION)
endif

cflags-$(CONFIG_STACKPROTECTOR)	+= -mstack-protector-guard=tls
ifdef CONFIG_PPC64
cflags-$(CONFIG_STACKPROTECTOR)	+= -mstack-protector-guard-reg=r13
else
cflags-$(CONFIG_STACKPROTECTOR)	+= -mstack-protector-guard-reg=r2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif

LDFLAGS_vmlinux-y := -Bstatic
LDFLAGS_vmlinux-$(CONFIG_RELOCATABLE) := -pie
<<<<<<< HEAD
LDFLAGS_vmlinux	:= $(LDFLAGS_vmlinux-y)

asinstr := $(call as-instr,lis 9$(comma)foo@high,-DHAVE_AS_ATHIGH=1)

CFLAGS-$(CONFIG_PPC64)	:= -mminimal-toc -mtraceback=no -mcall-aixdesc
CFLAGS-$(CONFIG_PPC32)	:= -ffixed-r2 -mmultiple
KBUILD_CPPFLAGS	+= -Iarch/$(ARCH) $(asinstr)
KBUILD_AFLAGS	+= -Iarch/$(ARCH)
KBUILD_CFLAGS	+= -msoft-float -pipe -Iarch/$(ARCH) $(CFLAGS-y)
CPP		= $(CC) -E $(KBUILD_CFLAGS)

CHECKFLAGS	+= -m$(CONFIG_WORD_SIZE) -D__powerpc__ -D__powerpc$(CONFIG_WORD_SIZE)__

ifeq ($(CONFIG_PPC64),y)
GCC_BROKEN_VEC	:= $(call cc-ifversion, -lt, 0400, y)

ifeq ($(CONFIG_POWER4_ONLY),y)
ifeq ($(CONFIG_ALTIVEC),y)
ifeq ($(GCC_BROKEN_VEC),y)
	KBUILD_CFLAGS += $(call cc-option,-mcpu=970)
else
	KBUILD_CFLAGS += $(call cc-option,-mcpu=power4)
endif
else
	KBUILD_CFLAGS += $(call cc-option,-mcpu=power4)
endif
else
	KBUILD_CFLAGS += $(call cc-option,-mtune=power4)
endif
endif

KBUILD_LDFLAGS_MODULE += arch/powerpc/lib/crtsavres.o

ifeq ($(CONFIG_TUNE_CELL),y)
	KBUILD_CFLAGS += $(call cc-option,-mtune=cell)
endif

# No AltiVec instruction when building kernel
KBUILD_CFLAGS += $(call cc-option,-mno-altivec)
=======
LDFLAGS_vmlinux-$(CONFIG_RELOCATABLE) += -z notext
LDFLAGS_vmlinux	:= $(LDFLAGS_vmlinux-y)

ifdef CONFIG_PPC64
ifndef CONFIG_PPC_KERNEL_PCREL
ifeq ($(call cc-option-yn,-mcmodel=medium),y)
	# -mcmodel=medium breaks modules because it uses 32bit offsets from
	# the TOC pointer to create pointers where possible. Pointers into the
	# percpu data area are created by this method.
	#
	# The kernel module loader relocates the percpu data section from the
	# original location (starting with 0xd...) to somewhere in the base
	# kernel percpu data space (starting with 0xc...). We need a full
	# 64bit relocation for this to work, hence -mcmodel=large.
	KBUILD_CFLAGS_MODULE += -mcmodel=large
else
	export NO_MINIMAL_TOC := -mno-minimal-toc
endif
endif
endif

CFLAGS-$(CONFIG_PPC64)	:= $(call cc-option,-mtraceback=no)
ifdef CONFIG_PPC64_ELF_ABI_V2
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mabi=elfv2,$(call cc-option,-mcall-aixdesc))
else
ifndef CONFIG_CC_IS_CLANG
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mabi=elfv1)
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mcall-aixdesc)
endif
endif
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mcmodel=medium,$(call cc-option,-mminimal-toc))
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mno-pointers-to-nested-functions)
CFLAGS-$(CONFIG_PPC64)	+= $(call cc-option,-mlong-double-128)

# Clang unconditionally reserves r2 on ppc32 and does not support the flag
# https://llvm.org/pr39555
CFLAGS-$(CONFIG_PPC32)	:= $(call cc-option, -ffixed-r2)

# Clang doesn't support -mmultiple / -mno-multiple
# https://llvm.org/pr39556
CFLAGS-$(CONFIG_PPC32)	+= $(call cc-option, $(MULTIPLEWORD))

CFLAGS-$(CONFIG_PPC32)	+= $(call cc-option,-mno-readonly-in-sdata)

ifdef CONFIG_FUNCTION_TRACER
ifdef CONFIG_ARCH_USING_PATCHABLE_FUNCTION_ENTRY
KBUILD_CPPFLAGS	+= -DCC_USING_PATCHABLE_FUNCTION_ENTRY
CC_FLAGS_FTRACE := -fpatchable-function-entry=2
else
CC_FLAGS_FTRACE := -pg
ifdef CONFIG_MPROFILE_KERNEL
CC_FLAGS_FTRACE += -mprofile-kernel
endif
endif
endif

CFLAGS-$(CONFIG_TARGET_CPU_BOOL) += -mcpu=$(CONFIG_TARGET_CPU)
AFLAGS-$(CONFIG_TARGET_CPU_BOOL) += -mcpu=$(CONFIG_TARGET_CPU)

CFLAGS-y += $(CONFIG_TUNE_CPU)

asinstr := $(call as-instr,lis 9$(comma)foo@high,-DHAVE_AS_ATHIGH=1)

KBUILD_CPPFLAGS	+= -I $(srctree)/arch/powerpc $(asinstr)
KBUILD_AFLAGS	+= $(AFLAGS-y)
KBUILD_CFLAGS	+= $(call cc-option,-msoft-float)
KBUILD_CFLAGS	+= $(CFLAGS-y)
CPP		= $(CC) -E $(KBUILD_CFLAGS)

CHECKFLAGS	+= -m$(BITS) -D__powerpc__ -D__powerpc$(BITS)__
ifdef CONFIG_CPU_BIG_ENDIAN
CHECKFLAGS	+= -D__BIG_ENDIAN__
else
CHECKFLAGS	+= -D__LITTLE_ENDIAN__
endif

ifdef CONFIG_476FPE_ERR46
	KBUILD_LDFLAGS_MODULE += --ppc476-workaround \
		-T $(srctree)/arch/powerpc/platforms/44x/ppc476_modules.lds
endif

# No prefix or pcrel
ifdef CONFIG_PPC_KERNEL_PREFIXED
KBUILD_CFLAGS += $(call cc-option,-mprefixed)
else
KBUILD_CFLAGS += $(call cc-option,-mno-prefixed)
endif
ifdef CONFIG_PPC_KERNEL_PCREL
KBUILD_CFLAGS += $(call cc-option,-mpcrel)
else
KBUILD_CFLAGS += $(call cc-option,-mno-pcrel)
endif

# No AltiVec or VSX or MMA instructions when building kernel
KBUILD_CFLAGS += $(call cc-option,-mno-altivec)
KBUILD_CFLAGS += $(call cc-option,-mno-vsx)
KBUILD_CFLAGS += $(call cc-option,-mno-mma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# No SPE instruction when building kernel
# (We use all available options to help semi-broken compilers)
KBUILD_CFLAGS += $(call cc-option,-mno-spe)
KBUILD_CFLAGS += $(call cc-option,-mspe=no)

<<<<<<< HEAD
# Enable unit-at-a-time mode when possible. It shrinks the
# kernel considerably.
KBUILD_CFLAGS += $(call cc-option,-funit-at-a-time)

# FIXME: the module load should be taught about the additional relocs
# generated by this.
# revert to pre-gcc-4.4 behaviour of .eh_frame
KBUILD_CFLAGS	+= $(call cc-option,-fno-dwarf2-cfi-asm)

# Never use string load/store instructions as they are
# often slow when they are implemented at all
KBUILD_CFLAGS		+= -mno-string

ifeq ($(CONFIG_6xx),y)
KBUILD_CFLAGS		+= -mcpu=powerpc
endif

# Work around a gcc code-gen bug with -fno-omit-frame-pointer.
ifeq ($(CONFIG_FUNCTION_TRACER),y)
KBUILD_CFLAGS		+= -mno-sched-epilog
endif

cpu-as-$(CONFIG_4xx)		+= -Wa,-m405
cpu-as-$(CONFIG_ALTIVEC)	+= -Wa,-maltivec
cpu-as-$(CONFIG_E500)		+= -Wa,-me500
cpu-as-$(CONFIG_E200)		+= -Wa,-me200

KBUILD_AFLAGS += $(cpu-as-y)
KBUILD_CFLAGS += $(cpu-as-y)

head-y				:= arch/powerpc/kernel/head_$(CONFIG_WORD_SIZE).o
head-$(CONFIG_8xx)		:= arch/powerpc/kernel/head_8xx.o
head-$(CONFIG_40x)		:= arch/powerpc/kernel/head_40x.o
head-$(CONFIG_44x)		:= arch/powerpc/kernel/head_44x.o
head-$(CONFIG_FSL_BOOKE)	:= arch/powerpc/kernel/head_fsl_booke.o

head-$(CONFIG_PPC64)		+= arch/powerpc/kernel/entry_64.o
head-$(CONFIG_PPC_FPU)		+= arch/powerpc/kernel/fpu.o
head-$(CONFIG_ALTIVEC)		+= arch/powerpc/kernel/vector.o

core-y				+= arch/powerpc/kernel/ \
				   arch/powerpc/mm/ \
				   arch/powerpc/lib/ \
				   arch/powerpc/sysdev/ \
				   arch/powerpc/platforms/ \
				   arch/powerpc/math-emu/ \
				   arch/powerpc/net/
core-$(CONFIG_XMON)		+= arch/powerpc/xmon/
core-$(CONFIG_KVM) 		+= arch/powerpc/kvm/
core-$(CONFIG_PERF_EVENTS)	+= arch/powerpc/perf/

drivers-$(CONFIG_OPROFILE)	+= arch/powerpc/oprofile/
=======
# Don't emit .eh_frame since we have no use for it
KBUILD_CFLAGS += -fno-asynchronous-unwind-tables

# Never use string load/store instructions as they are
# often slow when they are implemented at all
KBUILD_CFLAGS		+= $(call cc-option,-mno-string)

KBUILD_AFLAGS += $(aflags-y)
KBUILD_CFLAGS += $(cflags-y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# Default to zImage, override when needed
all: zImage

# With make 3.82 we cannot mix normal and wildcard targets
BOOT_TARGETS1 := zImage zImage.initrd uImage
BOOT_TARGETS2 := zImage% dtbImage% treeImage.% cuImage.% simpleImage.% uImage.%

PHONY += $(BOOT_TARGETS1) $(BOOT_TARGETS2)

<<<<<<< HEAD
boot := arch/$(ARCH)/boot

ifeq ($(CONFIG_RELOCATABLE),y)
quiet_cmd_relocs_check = CALL    $<
      cmd_relocs_check = perl $< "$(OBJDUMP)" "$(obj)/vmlinux"

PHONY += relocs_check
relocs_check: arch/powerpc/relocs_check.pl vmlinux
	$(call cmd,relocs_check)

zImage: relocs_check
endif

$(BOOT_TARGETS1): vmlinux
	$(Q)$(MAKE) ARCH=ppc64 $(build)=$(boot) $(patsubst %,$(boot)/%,$@)
$(BOOT_TARGETS2): vmlinux
	$(Q)$(MAKE) ARCH=ppc64 $(build)=$(boot) $(patsubst %,$(boot)/%,$@)


bootwrapper_install:
	$(Q)$(MAKE) ARCH=ppc64 $(build)=$(boot) $(patsubst %,$(boot)/%,$@)

%.dtb:
	$(Q)$(MAKE) ARCH=ppc64 $(build)=$(boot) $(patsubst %,$(boot)/%,$@)

define archhelp
  @echo '* zImage          - Build default images selected by kernel config'
  @echo '  zImage.*        - Compressed kernel image (arch/$(ARCH)/boot/zImage.*)'
  @echo '  uImage          - U-Boot native image format'
  @echo '  cuImage.<dt>    - Backwards compatible U-Boot image for older'
  @echo '                    versions which do not support device trees'
  @echo '  dtbImage.<dt>   - zImage with an embedded device tree blob'
  @echo '  simpleImage.<dt> - Firmware independent image.'
  @echo '  treeImage.<dt>  - Support for older IBM 4xx firmware (not U-Boot)'
  @echo '  install         - Install kernel using'
  @echo '                    (your) ~/bin/$(INSTALLKERNEL) or'
  @echo '                    (distribution) /sbin/$(INSTALLKERNEL) or'
  @echo '                    install to $$(INSTALL_PATH) and run lilo'
  @echo '  *_defconfig     - Select default config from arch/$(ARCH)/configs'
  @echo ''
  @echo '  Targets with <dt> embed a device tree blob inside the image'
  @echo '  These targets support board with firmware that does not'
  @echo '  support passing a device tree directly.  Replace <dt> with the'
  @echo '  name of a dts file from the arch/$(ARCH)/boot/dts/ directory'
  @echo '  (minus the .dts extension).'
endef

install:
	$(Q)$(MAKE) $(build)=$(boot) install

vdso_install:
ifeq ($(CONFIG_PPC64),y)
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso64 $@
endif
	$(Q)$(MAKE) $(build)=arch/$(ARCH)/kernel/vdso32 $@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

archprepare: checkbin

# Use the file '.tmp_gas_check' for binutils tests, as gas won't output
# to stdout and these checks are run even on install targets.
TOUT	:= .tmp_gas_check
# Ensure this is binutils 2.12.1 (or 2.12.90.0.7) or later for altivec
# instructions.
# gcc-3.4 and binutils-2.14 are a fatal combination.

checkbin:
	@if test "$(call cc-version)" = "0304" ; then \
		if ! /bin/echo mftb 5 | $(AS) -v -mppc -many -o $(TOUT) >/dev/null 2>&1 ; then \
			echo -n '*** ${VERSION}.${PATCHLEVEL} kernels no longer build '; \
			echo 'correctly with gcc-3.4 and your version of binutils.'; \
			echo '*** Please upgrade your binutils or downgrade your gcc'; \
			false; \
		fi ; \
	fi
	@if test "$(call cc-fullversion)" = "040200" \
	    && test "x${CONFIG_MODULES}${CONFIG_PPC64}" = "xyy" ; then \
		echo -n '*** GCC-4.2.0 cannot compile the 64-bit powerpc ' ; \
		echo 'kernel with modules enabled.' ; \
		echo -n '*** Please use a different GCC version or ' ; \
		echo 'disable kernel modules' ; \
		false ; \
	fi

CLEAN_FILES += $(TOUT)

=======
boot := arch/powerpc/boot

$(BOOT_TARGETS1): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(patsubst %,$(boot)/%,$@)
$(BOOT_TARGETS2): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(patsubst %,$(boot)/%,$@)


PHONY += bootwrapper_install
bootwrapper_install:
	$(Q)$(MAKE) $(build)=$(boot) $(patsubst %,$(boot)/%,$@)

include $(srctree)/scripts/Makefile.defconf

generated_configs += ppc64le_defconfig
ppc64le_defconfig:
	$(call merge_into_defconfig,ppc64_defconfig,le)

generated_configs += ppc64le_guest_defconfig
ppc64le_guest_defconfig:
	$(call merge_into_defconfig,ppc64_defconfig,le guest kvm_guest)

generated_configs += ppc64_guest_defconfig
ppc64_guest_defconfig:
	$(call merge_into_defconfig,ppc64_defconfig,be guest kvm_guest)

generated_configs += pseries_le_defconfig
pseries_le_defconfig: ppc64le_guest_defconfig

generated_configs += pseries_defconfig
pseries_defconfig: ppc64le_guest_defconfig

generated_configs += powernv_be_defconfig
powernv_be_defconfig:
	$(call merge_into_defconfig,powernv_defconfig,be)

generated_configs += mpc85xx_defconfig
mpc85xx_defconfig:
	$(call merge_into_defconfig,mpc85xx_base.config,\
		85xx-32bit 85xx-hw fsl-emb-nonhw)

generated_configs += mpc85xx_smp_defconfig
mpc85xx_smp_defconfig:
	$(call merge_into_defconfig,mpc85xx_base.config,\
		85xx-32bit 85xx-smp 85xx-hw fsl-emb-nonhw)

generated_configs += corenet32_smp_defconfig
corenet32_smp_defconfig:
	$(call merge_into_defconfig,corenet_base.config,\
		85xx-32bit 85xx-smp 85xx-hw fsl-emb-nonhw dpaa)

generated_configs += corenet64_smp_defconfig
corenet64_smp_defconfig:
	$(call merge_into_defconfig,corenet_base.config,\
		85xx-64bit 85xx-smp altivec 85xx-hw fsl-emb-nonhw dpaa)

generated_configs += mpc86xx_defconfig
mpc86xx_defconfig:
	$(call merge_into_defconfig,mpc86xx_base.config,\
		86xx-hw fsl-emb-nonhw)

generated_configs += mpc86xx_smp_defconfig
mpc86xx_smp_defconfig:
	$(call merge_into_defconfig,mpc86xx_base.config,\
		86xx-smp 86xx-hw fsl-emb-nonhw)

generated_configs += ppc32_allmodconfig
ppc32_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/book3s_32.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc40x_allmodconfig
ppc40x_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/40x.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc44x_allmodconfig
ppc44x_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/44x.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc8xx_allmodconfig
ppc8xx_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/8xx.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc85xx_allmodconfig
ppc85xx_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/85xx-32bit.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc_defconfig
ppc_defconfig:
	$(call merge_into_defconfig,book3s_32.config,)

generated_configs += ppc64le_allmodconfig
ppc64le_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/le.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc64le_allnoconfig
ppc64le_allnoconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/ppc64le.config \
		-f $(srctree)/Makefile allnoconfig

generated_configs += ppc64_book3e_allmodconfig
ppc64_book3e_allmodconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/85xx-64bit.config \
		-f $(srctree)/Makefile allmodconfig

generated_configs += ppc32_randconfig
ppc32_randconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/32-bit.config \
		-f $(srctree)/Makefile randconfig

generated_configs += ppc64_randconfig
ppc64_randconfig:
	$(Q)$(MAKE) KCONFIG_ALLCONFIG=$(srctree)/arch/powerpc/configs/64-bit.config \
		-f $(srctree)/Makefile randconfig

PHONY += $(generated_configs)

define archhelp
  echo '* zImage          - Build default images selected by kernel config'
  echo '  zImage.*        - Compressed kernel image (arch/powerpc/boot/zImage.*)'
  echo '  uImage          - U-Boot native image format'
  echo '  cuImage.<dt>    - Backwards compatible U-Boot image for older'
  echo '                    versions which do not support device trees'
  echo '  dtbImage.<dt>   - zImage with an embedded device tree blob'
  echo '  simpleImage.<dt> - Firmware independent image.'
  echo '  treeImage.<dt>  - Support for older IBM 4xx firmware (not U-Boot)'
  echo '  install         - Install kernel using'
  echo '                    (your) ~/bin/$(INSTALLKERNEL) or'
  echo '                    (distribution) /sbin/$(INSTALLKERNEL) or'
  echo '                    install to $$(INSTALL_PATH) and run lilo'
  echo '  *_defconfig     - Select default config from arch/powerpc/configs'
  echo ''
  echo '  Targets with <dt> embed a device tree blob inside the image'
  echo '  These targets support board with firmware that does not'
  echo '  support passing a device tree directly.  Replace <dt> with the'
  echo '  name of a dts file from the arch/powerpc/boot/dts/ directory'
  echo '  (minus the .dts extension).'
  echo
  $(foreach cfg,$(generated_configs),
    printf "  %-27s - Build for %s\\n" $(cfg) $(subst _defconfig,,$(cfg));)
endef

PHONY += install
install:
	$(call cmd,install)

ifeq ($(KBUILD_EXTMOD),)
# We need to generate vdso-offsets.h before compiling certain files in kernel/.
# In order to do that, we should use the archprepare target, but we can't since
# asm-offsets.h is included in some files used to generate vdso-offsets.h, and
# asm-offsets.h is built in prepare0, for which archprepare is a dependency.
# Therefore we need to generate the header after prepare0 has been made, hence
# this hack.
prepare: vdso_prepare
vdso_prepare: prepare0
	$(if $(CONFIG_VDSO32),$(Q)$(MAKE) \
		$(build)=arch/powerpc/kernel/vdso include/generated/vdso32-offsets.h)
	$(if $(CONFIG_PPC64),$(Q)$(MAKE) \
		$(build)=arch/powerpc/kernel/vdso include/generated/vdso64-offsets.h)
endif

archprepare: checkbin

archheaders:
	$(Q)$(MAKE) $(build)=arch/powerpc/kernel/syscalls all

ifdef CONFIG_STACKPROTECTOR
prepare: stack_protector_prepare

PHONY += stack_protector_prepare
stack_protector_prepare: prepare0
ifdef CONFIG_PPC64
	$(eval KBUILD_CFLAGS += -mstack-protector-guard-offset=$(shell awk '{if ($$2 == "PACA_CANARY") print $$3;}' include/generated/asm-offsets.h))
else
	$(eval KBUILD_CFLAGS += -mstack-protector-guard-offset=$(shell awk '{if ($$2 == "TASK_CANARY") print $$3;}' include/generated/asm-offsets.h))
endif
endif

PHONY += checkbin
checkbin:
	@if test "x${CONFIG_FTRACE_MCOUNT_USE_RECORDMCOUNT}" = "xy" -a \
		"x${CONFIG_LD_IS_BFD}" = "xy" -a \
		"${CONFIG_LD_VERSION}" = "23700" ; then \
		echo -n '*** binutils 2.37 drops unused section symbols, which recordmcount ' ; \
		echo 'is unable to handle.' ; \
		echo '*** Please use a different binutils version.' ; \
		false ; \
	fi
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
