<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Unified Makefile for i386 and x86_64

# select defconfig based on actual architecture
ifeq ($(ARCH),x86)
<<<<<<< HEAD
        KBUILD_DEFCONFIG := i386_defconfig
=======
  ifeq ($(shell uname -m | sed -e 's/i.86/i386/'),i386)
        KBUILD_DEFCONFIG := i386_defconfig
  else
        KBUILD_DEFCONFIG := x86_64_defconfig
  endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
else
        KBUILD_DEFCONFIG := $(ARCH)_defconfig
endif

<<<<<<< HEAD
=======
ifdef CONFIG_CC_IS_GCC
RETPOLINE_CFLAGS	:= $(call cc-option,-mindirect-branch=thunk-extern -mindirect-branch-register)
RETPOLINE_VDSO_CFLAGS	:= $(call cc-option,-mindirect-branch=thunk-inline -mindirect-branch-register)
endif
ifdef CONFIG_CC_IS_CLANG
RETPOLINE_CFLAGS	:= -mretpoline-external-thunk
RETPOLINE_VDSO_CFLAGS	:= -mretpoline
endif
RETPOLINE_CFLAGS	+= $(call cc-option,-mindirect-branch-cs-prefix)

ifdef CONFIG_MITIGATION_RETHUNK
RETHUNK_CFLAGS		:= -mfunction-return=thunk-extern
RETPOLINE_CFLAGS	+= $(RETHUNK_CFLAGS)
endif

export RETHUNK_CFLAGS
export RETPOLINE_CFLAGS
export RETPOLINE_VDSO_CFLAGS

# For gcc stack alignment is specified with -mpreferred-stack-boundary,
# clang has the option -mstack-alignment for that purpose.
ifneq ($(call cc-option, -mpreferred-stack-boundary=4),)
      cc_stack_align4 := -mpreferred-stack-boundary=2
      cc_stack_align8 := -mpreferred-stack-boundary=3
else ifneq ($(call cc-option, -mstack-alignment=16),)
      cc_stack_align4 := -mstack-alignment=4
      cc_stack_align8 := -mstack-alignment=8
endif

# How to compile the 16-bit code.  Note we always compile for -march=i386;
# that way we can complain to the user if the CPU is insufficient.
REALMODE_CFLAGS	:= -std=gnu11 -m16 -g -Os -DDISABLE_BRANCH_PROFILING -D__DISABLE_EXPORTS \
		   -Wall -Wstrict-prototypes -march=i386 -mregparm=3 \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic \
		   -mno-mmx -mno-sse $(call cc-option,-fcf-protection=none)

REALMODE_CFLAGS += -ffreestanding
REALMODE_CFLAGS += -fno-stack-protector
REALMODE_CFLAGS += -Wno-address-of-packed-member
REALMODE_CFLAGS += $(cc_stack_align4)
REALMODE_CFLAGS += $(CLANG_FLAGS)
ifdef CONFIG_CC_IS_CLANG
REALMODE_CFLAGS += -Wno-gnu
endif
export REALMODE_CFLAGS

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# BITS is used as extension for files which are available in a 32 bit
# and a 64 bit version to simplify shared Makefiles.
# e.g.: obj-y += foo_$(BITS).o
export BITS

<<<<<<< HEAD
=======
#
# Prevent GCC from generating any FP code by mistake.
#
# This must happen before we try the -mpreferred-stack-boundary, see:
#
#    https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53383
#
KBUILD_CFLAGS += -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx
KBUILD_RUSTFLAGS += --target=$(objtree)/scripts/target.json
KBUILD_RUSTFLAGS += -Ctarget-feature=-sse,-sse2,-sse3,-ssse3,-sse4.1,-sse4.2,-avx,-avx2

ifeq ($(CONFIG_X86_KERNEL_IBT),y)
#
# Kernel IBT has S_CET.NOTRACK_EN=0, as such the compilers must not generate
# NOTRACK prefixes. Current generation compilers unconditionally employ NOTRACK
# for jump-tables, as such, disable jump-tables for now.
#
# (jump-tables are implicitly disabled by RETPOLINE)
#
#   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104816
#
KBUILD_CFLAGS += $(call cc-option,-fcf-protection=branch -fno-jump-tables)
KBUILD_RUSTFLAGS += -Zcf-protection=branch -Zno-jump-tables
else
KBUILD_CFLAGS += $(call cc-option,-fcf-protection=none)
endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ifeq ($(CONFIG_X86_32),y)
        BITS := 32
        UTS_MACHINE := i386
        CHECKFLAGS += -D__i386__

<<<<<<< HEAD
        biarch := $(call cc-option,-m32)
        KBUILD_AFLAGS += $(biarch)
        KBUILD_CFLAGS += $(biarch)

        ifdef CONFIG_RELOCATABLE
                LDFLAGS_vmlinux := --emit-relocs
        endif

        KBUILD_CFLAGS += -msoft-float -mregparm=3 -freg-struct-return

        # prevent gcc from keeping the stack 16 byte aligned
        KBUILD_CFLAGS += $(call cc-option,-mpreferred-stack-boundary=2)

        # Disable unit-at-a-time mode on pre-gcc-4.0 compilers, it makes gcc use
        # a lot more stack due to the lack of sharing of stacklots:
        KBUILD_CFLAGS += $(call cc-ifversion, -lt, 0400, \
				$(call cc-option,-fno-unit-at-a-time))
=======
        KBUILD_AFLAGS += -m32
        KBUILD_CFLAGS += -m32

        KBUILD_CFLAGS += -msoft-float -mregparm=3 -freg-struct-return

        # Never want PIC in a 32-bit kernel, prevent breakage with GCC built
        # with nonstandard options
        KBUILD_CFLAGS += -fno-pic

        # Align the stack to the register width instead of using the default
        # alignment of 16 bytes. This reduces stack usage and the number of
        # alignment instructions.
        KBUILD_CFLAGS += $(cc_stack_align4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

        # CPU-specific tuning. Anything which can be shared with UML should go here.
        include $(srctree)/arch/x86/Makefile_32.cpu
        KBUILD_CFLAGS += $(cflags-y)

        # temporary until string.h is fixed
        KBUILD_CFLAGS += -ffreestanding
<<<<<<< HEAD
else
        BITS := 64
        UTS_MACHINE := x86_64
        CHECKFLAGS += -D__x86_64__ -m64
=======

    ifeq ($(CONFIG_STACKPROTECTOR),y)
        ifeq ($(CONFIG_SMP),y)
			KBUILD_CFLAGS += -mstack-protector-guard-reg=fs -mstack-protector-guard-symbol=__stack_chk_guard
        else
			KBUILD_CFLAGS += -mstack-protector-guard=global
        endif
    endif
else
        BITS := 64
        UTS_MACHINE := x86_64
        CHECKFLAGS += -D__x86_64__
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

        KBUILD_AFLAGS += -m64
        KBUILD_CFLAGS += -m64

<<<<<<< HEAD
        # FIXME - should be integrated in Makefile.cpu (Makefile_32.cpu)
        cflags-$(CONFIG_MK8) += $(call cc-option,-march=k8)
        cflags-$(CONFIG_MPSC) += $(call cc-option,-march=nocona)

        cflags-$(CONFIG_MCORE2) += \
                $(call cc-option,-march=core2,$(call cc-option,-mtune=generic))
	cflags-$(CONFIG_MATOM) += $(call cc-option,-march=atom) \
		$(call cc-option,-mtune=atom,$(call cc-option,-mtune=generic))
        cflags-$(CONFIG_GENERIC_CPU) += $(call cc-option,-mtune=generic)
        KBUILD_CFLAGS += $(cflags-y)

        KBUILD_CFLAGS += -mno-red-zone
        KBUILD_CFLAGS += -mcmodel=kernel

        # -funit-at-a-time shrinks the kernel .text considerably
        # unfortunately it makes reading oopses harder.
        KBUILD_CFLAGS += $(call cc-option,-funit-at-a-time)

        # this works around some issues with generating unwind tables in older gccs
        # newer gccs do it by default
        KBUILD_CFLAGS += -maccumulate-outgoing-args
endif

ifdef CONFIG_CC_STACKPROTECTOR
	cc_has_sp := $(srctree)/scripts/gcc-x86_$(BITS)-has-stack-protector.sh
        ifeq ($(shell $(CONFIG_SHELL) $(cc_has_sp) $(CC) $(KBUILD_CPPFLAGS) $(biarch)),y)
                stackp-y := -fstack-protector
                KBUILD_CFLAGS += $(stackp-y)
        else
                $(warning stack protector enabled but no compiler support)
        endif
endif

ifdef CONFIG_X86_X32
	x32_ld_ok := $(call try-run,\
			/bin/echo -e '1: .quad 1b' | \
			$(CC) $(KBUILD_AFLAGS) -c -x assembler -o "$$TMP" - && \
			$(OBJCOPY) -O elf32-x86-64 "$$TMP" "$$TMPO" && \
			$(LD) -m elf32_x86_64 "$$TMPO" -o "$$TMP",y,n)
        ifeq ($(x32_ld_ok),y)
                CONFIG_X86_X32_ABI := y
                KBUILD_AFLAGS += -DCONFIG_X86_X32_ABI
                KBUILD_CFLAGS += -DCONFIG_X86_X32_ABI
        else
                $(warning CONFIG_X86_X32 enabled but no binutils support)
        endif
endif
export CONFIG_X86_X32_ABI

# Don't unroll struct assignments with kmemcheck enabled
ifeq ($(CONFIG_KMEMCHECK),y)
	KBUILD_CFLAGS += $(call cc-option,-fno-builtin-memcpy)
endif

# Stackpointer is addressed different for 32 bit and 64 bit x86
sp-$(CONFIG_X86_32) := esp
sp-$(CONFIG_X86_64) := rsp

# do binutils support CFI?
cfi := $(call as-instr,.cfi_startproc\n.cfi_rel_offset $(sp-y)$(comma)0\n.cfi_endproc,-DCONFIG_AS_CFI=1)
# is .cfi_signal_frame supported too?
cfi-sigframe := $(call as-instr,.cfi_startproc\n.cfi_signal_frame\n.cfi_endproc,-DCONFIG_AS_CFI_SIGNAL_FRAME=1)
cfi-sections := $(call as-instr,.cfi_sections .debug_frame,-DCONFIG_AS_CFI_SECTIONS=1)

# does binutils support specific instructions?
asinstr := $(call as-instr,fxsaveq (%rax),-DCONFIG_AS_FXSAVEQ=1)

KBUILD_AFLAGS += $(cfi) $(cfi-sigframe) $(cfi-sections) $(asinstr)
KBUILD_CFLAGS += $(cfi) $(cfi-sigframe) $(cfi-sections) $(asinstr)

LDFLAGS := -m elf_$(UTS_MACHINE)

# Speed up the build
KBUILD_CFLAGS += -pipe
=======
        # Align jump targets to 1 byte, not the default 16 bytes:
        KBUILD_CFLAGS += $(call cc-option,-falign-jumps=1)

        # Pack loops tightly as well:
        KBUILD_CFLAGS += $(call cc-option,-falign-loops=1)

        # Don't autogenerate traditional x87 instructions
        KBUILD_CFLAGS += -mno-80387
        KBUILD_CFLAGS += $(call cc-option,-mno-fp-ret-in-387)

        # By default gcc and clang use a stack alignment of 16 bytes for x86.
        # However the standard kernel entry on x86-64 leaves the stack on an
        # 8-byte boundary. If the compiler isn't informed about the actual
        # alignment it will generate extra alignment instructions for the
        # default alignment which keep the stack *mis*aligned.
        # Furthermore an alignment to the register width reduces stack usage
        # and the number of alignment instructions.
        KBUILD_CFLAGS += $(cc_stack_align8)

	# Use -mskip-rax-setup if supported.
	KBUILD_CFLAGS += $(call cc-option,-mskip-rax-setup)

        # FIXME - should be integrated in Makefile.cpu (Makefile_32.cpu)
        cflags-$(CONFIG_MK8)		+= -march=k8
        cflags-$(CONFIG_MPSC)		+= -march=nocona
        cflags-$(CONFIG_MCORE2)		+= -march=core2
        cflags-$(CONFIG_MATOM)		+= -march=atom
        cflags-$(CONFIG_GENERIC_CPU)	+= -mtune=generic
        KBUILD_CFLAGS += $(cflags-y)

        rustflags-$(CONFIG_MK8)		+= -Ctarget-cpu=k8
        rustflags-$(CONFIG_MPSC)	+= -Ctarget-cpu=nocona
        rustflags-$(CONFIG_MCORE2)	+= -Ctarget-cpu=core2
        rustflags-$(CONFIG_MATOM)	+= -Ctarget-cpu=atom
        rustflags-$(CONFIG_GENERIC_CPU)	+= -Ztune-cpu=generic
        KBUILD_RUSTFLAGS += $(rustflags-y)

        KBUILD_CFLAGS += -mno-red-zone
        KBUILD_CFLAGS += -mcmodel=kernel
        KBUILD_RUSTFLAGS += -Cno-redzone=y
        KBUILD_RUSTFLAGS += -Ccode-model=kernel
endif

#
# If the function graph tracer is used with mcount instead of fentry,
# '-maccumulate-outgoing-args' is needed to prevent a GCC bug
# (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42109)
#
ifdef CONFIG_FUNCTION_GRAPH_TRACER
  ifndef CONFIG_HAVE_FENTRY
	ACCUMULATE_OUTGOING_ARGS := 1
  endif
endif

ifeq ($(ACCUMULATE_OUTGOING_ARGS), 1)
	# This compiler flag is not supported by Clang:
	KBUILD_CFLAGS += $(call cc-option,-maccumulate-outgoing-args,)
endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# Workaround for a gcc prelease that unfortunately was shipped in a suse release
KBUILD_CFLAGS += -Wno-sign-compare
#
KBUILD_CFLAGS += -fno-asynchronous-unwind-tables
<<<<<<< HEAD
# prevent gcc from generating any FP code by mistake
KBUILD_CFLAGS += $(call cc-option,-mno-sse -mno-mmx -mno-sse2 -mno-3dnow,)
KBUILD_CFLAGS += $(call cc-option,-mno-avx,)

KBUILD_CFLAGS += $(mflags-y)
KBUILD_AFLAGS += $(mflags-y)

archscripts:
=======

# Avoid indirect branches in kernel to deal with Spectre
ifdef CONFIG_MITIGATION_RETPOLINE
  KBUILD_CFLAGS += $(RETPOLINE_CFLAGS)
  # Additionally, avoid generating expensive indirect jumps which
  # are subject to retpolines for small number of switch cases.
  # clang turns off jump table generation by default when under
  # retpoline builds, however, gcc does not for x86. This has
  # only been fixed starting from gcc stable version 8.4.0 and
  # onwards, but not for older ones. See gcc bug #86952.
  ifndef CONFIG_CC_IS_CLANG
    KBUILD_CFLAGS += -fno-jump-tables
  endif
endif

ifdef CONFIG_MITIGATION_SLS
  KBUILD_CFLAGS += -mharden-sls=all
endif

ifdef CONFIG_CALL_PADDING
PADDING_CFLAGS := -fpatchable-function-entry=$(CONFIG_FUNCTION_PADDING_BYTES),$(CONFIG_FUNCTION_PADDING_BYTES)
KBUILD_CFLAGS += $(PADDING_CFLAGS)
export PADDING_CFLAGS
endif

KBUILD_LDFLAGS += -m elf_$(UTS_MACHINE)

ifdef CONFIG_X86_NEED_RELOCS
LDFLAGS_vmlinux := --emit-relocs --discard-none
else
LDFLAGS_vmlinux :=
endif

#
# The 64-bit kernel must be aligned to 2MB.  Pass -z max-page-size=0x200000 to
# the linker to force 2MB page size regardless of the default page size used
# by the linker.
#
ifdef CONFIG_X86_64
LDFLAGS_vmlinux += -z max-page-size=0x200000
endif


archscripts: scripts_basic
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	$(Q)$(MAKE) $(build)=arch/x86/tools relocs

###
# Syscall table generation

archheaders:
<<<<<<< HEAD
	$(Q)$(MAKE) $(build)=arch/x86/syscalls all
=======
	$(Q)$(MAKE) $(build)=arch/x86/entry/syscalls all
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

###
# Kernel objects

<<<<<<< HEAD
head-y := arch/x86/kernel/head_$(BITS).o
head-y += arch/x86/kernel/head$(BITS).o
head-y += arch/x86/kernel/head.o
head-y += arch/x86/kernel/init_task.o

libs-y  += arch/x86/lib/

# See arch/x86/Kbuild for content of core part of the kernel
core-y += arch/x86/

=======
libs-y  += arch/x86/lib/

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# drivers-y are linked after core-y
drivers-$(CONFIG_MATH_EMULATION) += arch/x86/math-emu/
drivers-$(CONFIG_PCI)            += arch/x86/pci/

<<<<<<< HEAD
# must be linked after kernel/
drivers-$(CONFIG_OPROFILE) += arch/x86/oprofile/

# suspend and hibernation support
drivers-$(CONFIG_PM) += arch/x86/power/

drivers-$(CONFIG_FB) += arch/x86/video/
=======
# suspend and hibernation support
drivers-$(CONFIG_PM) += arch/x86/power/

drivers-$(CONFIG_FB_CORE) += arch/x86/video/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

####
# boot loader support. Several targets are kept for legacy purposes

boot := arch/x86/boot

<<<<<<< HEAD
BOOT_TARGETS = bzlilo bzdisk fdimage fdimage144 fdimage288 isoimage
=======
BOOT_TARGETS = bzdisk fdimage fdimage144 fdimage288 hdimage isoimage
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

PHONY += bzImage $(BOOT_TARGETS)

# Default kernel to build
all: bzImage

# KBUILD_IMAGE specify target image being built
KBUILD_IMAGE := $(boot)/bzImage

bzImage: vmlinux
ifeq ($(CONFIG_X86_DECODER_SELFTEST),y)
	$(Q)$(MAKE) $(build)=arch/x86/tools posttest
endif
	$(Q)$(MAKE) $(build)=$(boot) $(KBUILD_IMAGE)
	$(Q)mkdir -p $(objtree)/arch/$(UTS_MACHINE)/boot
	$(Q)ln -fsn ../../x86/boot/bzImage $(objtree)/arch/$(UTS_MACHINE)/boot/$@

$(BOOT_TARGETS): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

PHONY += install
install:
<<<<<<< HEAD
	$(Q)$(MAKE) $(build)=$(boot) $@

PHONY += vdso_install
vdso_install:
	$(Q)$(MAKE) $(build)=arch/x86/vdso $@
=======
	$(call cmd,install)

vdso-install-$(CONFIG_X86_64)		+= arch/x86/entry/vdso/vdso64.so.dbg
vdso-install-$(CONFIG_X86_X32_ABI)	+= arch/x86/entry/vdso/vdsox32.so.dbg
vdso-install-$(CONFIG_COMPAT_32)	+= arch/x86/entry/vdso/vdso32.so.dbg

archprepare: checkbin
checkbin:
ifdef CONFIG_MITIGATION_RETPOLINE
ifeq ($(RETPOLINE_CFLAGS),)
	@echo "You are building kernel with non-retpoline compiler." >&2
	@echo "Please update your compiler." >&2
	@false
endif
endif

ifdef CONFIG_UNWINDER_ORC
orc_hash_h := arch/$(SRCARCH)/include/generated/asm/orc_hash.h
orc_hash_sh := $(srctree)/scripts/orc_hash.sh
targets += $(orc_hash_h)
quiet_cmd_orc_hash = GEN     $@
      cmd_orc_hash = mkdir -p $(dir $@); \
		     $(CONFIG_SHELL) $(orc_hash_sh) < $< > $@
$(orc_hash_h): $(srctree)/arch/x86/include/asm/orc_types.h $(orc_hash_sh) FORCE
	$(call if_changed,orc_hash)
archprepare: $(orc_hash_h)
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

archclean:
	$(Q)rm -rf $(objtree)/arch/i386
	$(Q)rm -rf $(objtree)/arch/x86_64
<<<<<<< HEAD
	$(Q)$(MAKE) $(clean)=$(boot)
	$(Q)$(MAKE) $(clean)=arch/x86/tools

define archhelp
  echo  '* bzImage      - Compressed kernel image (arch/x86/boot/bzImage)'
  echo  '  install      - Install kernel using'
  echo  '                  (your) ~/bin/$(INSTALLKERNEL) or'
  echo  '                  (distribution) /sbin/$(INSTALLKERNEL) or'
  echo  '                  install to $$(INSTALL_PATH) and run lilo'
  echo  '  fdimage      - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  fdimage144   - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  fdimage288   - Create 2.8MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  isoimage     - Create a boot CD-ROM image (arch/x86/boot/image.iso)'
  echo  '                  bzdisk/fdimage*/isoimage also accept:'
  echo  '                  FDARGS="..."  arguments for the booted kernel'
  echo  '                  FDINITRD=file initrd for the booted kernel'
=======

define archhelp
  echo  '* bzImage		- Compressed kernel image (arch/x86/boot/bzImage)'
  echo  '  install		- Install kernel using (your) ~/bin/$(INSTALLKERNEL) or'
  echo  '			  (distribution) /sbin/$(INSTALLKERNEL) or install to '
  echo  '			  $$(INSTALL_PATH) and run lilo'
  echo  ''
  echo  '  fdimage		- Create 1.4MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  fdimage144		- Create 1.4MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  fdimage288		- Create 2.8MB boot floppy image (arch/x86/boot/fdimage)'
  echo  '  hdimage		- Create a BIOS/EFI hard disk image (arch/x86/boot/hdimage)'
  echo  '  isoimage		- Create a boot CD-ROM image (arch/x86/boot/image.iso)'
  echo  '			  bzdisk/fdimage*/hdimage/isoimage also accept:'
  echo  '			  FDARGS="..."  arguments for the booted kernel'
  echo  '			  FDINITRD=file initrd for the booted kernel'

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endef
