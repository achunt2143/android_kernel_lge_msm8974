#
# arch/arm/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2001 by Russell King

<<<<<<< HEAD
LDFLAGS_vmlinux	:=-p --no-undefined -X
ifeq ($(CONFIG_CPU_ENDIAN_BE8),y)
LDFLAGS_vmlinux	+= --be8
endif

OBJCOPYFLAGS	:=-O binary -R .comment -S
GZFLAGS		:=-9
#KBUILD_CFLAGS	+=-pipe
# Explicitly specifiy 32-bit ARM ISA since toolchain default can be -mthumb:
KBUILD_CFLAGS	+=$(call cc-option,-marm,)
=======
LDFLAGS_vmlinux	:= --no-undefined -X --pic-veneer -z norelro
ifeq ($(CONFIG_CPU_ENDIAN_BE8),y)
LDFLAGS_vmlinux	+= --be8
KBUILD_LDFLAGS_MODULE	+= --be8
endif

GZFLAGS		:=-9
#KBUILD_CFLAGS	+=-pipe
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# Never generate .eh_frame
KBUILD_CFLAGS	+= $(call cc-option,-fno-dwarf2-cfi-asm)

<<<<<<< HEAD
# Do not use arch/arm/defconfig - it's always outdated.
# Select a platform tht is kept up-to-date
KBUILD_DEFCONFIG := versatile_defconfig
=======
# Disable FDPIC ABI
KBUILD_CFLAGS	+= $(call cc-option,-mno-fdpic)

# This should work on most of the modern platforms
KBUILD_DEFCONFIG := multi_v7_defconfig
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# defines filename extension depending memory management type.
ifeq ($(CONFIG_MMU),)
MMUEXT		:= -nommu
<<<<<<< HEAD
endif

ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS	+=-fno-omit-frame-pointer -mapcs -mno-sched-prolog
endif

ifeq ($(CONFIG_CC_STACKPROTECTOR),y)
KBUILD_CFLAGS	+=-fstack-protector
=======
KBUILD_CFLAGS	+= $(call cc-option,-mno-unaligned-access)
endif

ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS	+=-fno-omit-frame-pointer
ifeq ($(CONFIG_CC_IS_GCC),y)
KBUILD_CFLAGS += -mapcs -mno-sched-prolog
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
KBUILD_CPPFLAGS	+= -mbig-endian
<<<<<<< HEAD
AS		+= -EB
LD		+= -EB
else
KBUILD_CPPFLAGS	+= -mlittle-endian
AS		+= -EL
LD		+= -EL
endif

comma = ,

=======
CHECKFLAGS	+= -D__ARMEB__
KBUILD_LDFLAGS	+= -EB
else
KBUILD_CPPFLAGS	+= -mlittle-endian
CHECKFLAGS	+= -D__ARMEL__
KBUILD_LDFLAGS	+= -EL
endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# The Scalar Replacement of Aggregates (SRA) optimization pass in GCC 4.9 and
# later may result in code being generated that handles signed short and signed
# char struct members incorrectly. So disable it.
# (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65932)
#
KBUILD_CFLAGS	+= $(call cc-option,-fno-ipa-sra)

# This selects which instruction set is used.
<<<<<<< HEAD
# Note that GCC does not numerically define an architecture version
# macro, but instead defines a whole series of macros which makes
# testing for a specific architecture or later rather impossible.
arch-$(CONFIG_CPU_32v7)		:=-D__LINUX_ARM_ARCH__=7 $(call cc-option,-march=armv7-a,-march=armv5t -Wa$(comma)-march=armv7-a)
arch-$(CONFIG_CPU_32v6)		:=-D__LINUX_ARM_ARCH__=6 $(call cc-option,-march=armv6,-march=armv5t -Wa$(comma)-march=armv6)
# Only override the compiler option if ARMv6. The ARMv6K extensions are
# always available in ARMv7
ifeq ($(CONFIG_CPU_32v6),y)
arch-$(CONFIG_CPU_32v6K)	:=-D__LINUX_ARM_ARCH__=6 $(call cc-option,-march=armv6k,-march=armv5t -Wa$(comma)-march=armv6k)
endif
arch-$(CONFIG_CPU_32v5)		:=-D__LINUX_ARM_ARCH__=5 $(call cc-option,-march=armv5te,-march=armv4t)
arch-$(CONFIG_CPU_32v4T)	:=-D__LINUX_ARM_ARCH__=4 -march=armv4t
arch-$(CONFIG_CPU_32v4)		:=-D__LINUX_ARM_ARCH__=4 -march=armv4
arch-$(CONFIG_CPU_32v3)		:=-D__LINUX_ARM_ARCH__=3 -march=armv3

# Since 'cortex-a15' is a superset of the 'armv7-a' arch spec, we need to
# explicitly redefine the arch options to not include '-march=armv7-a' when
# generating code for Krait, which is compatible with the instruction set of the
# Cortex-A15, because GCC will warn us about ambiguous ISA restrictions caused
# by seemingly conflicting -march and -mcpu options.
# If $(CC) does not support the -mcpu=cortex-a15 option, fall back on passing
# -march=armv7-a to specify the ISA restriction, though this is suboptimal. To
# keep things simpler, we don't bother with a fallback option if the compiler
# doesn't even support -march=armv7-a, since in that situation we would have
# bigger problems.
arch-$(CONFIG_ARCH_MSM_KRAIT)	:=-D__LINUX_ARM_ARCH__=7 $(call cc-option,-mcpu=cortex-a15,-march=armv7-a)
arch-$(CONFIG_MSM_CORTEX_A7)	:=-D__LINUX_ARM_ARCH__=7 $(call cc-option,-mcpu=cortex-a15,-march=armv7-a)

# This selects how we optimise for the processor.
tune-$(CONFIG_CPU_ARM610)	:=-mtune=arm610
tune-$(CONFIG_CPU_ARM710)	:=-mtune=arm710
=======
arch-$(CONFIG_CPU_32v7M)	:=-march=armv7-m
arch-$(CONFIG_CPU_32v7)		:=-march=armv7-a
arch-$(CONFIG_CPU_32v6)		:=-march=armv6
# Only override the compiler option if ARMv6. The ARMv6K extensions are
# always available in ARMv7
ifeq ($(CONFIG_CPU_32v6),y)
arch-$(CONFIG_CPU_32v6K)	:=-march=armv6k
endif
arch-$(CONFIG_CPU_32v5)		:=-march=armv5te
arch-$(CONFIG_CPU_32v4T)	:=-march=armv4t
arch-$(CONFIG_CPU_32v4)		:=-march=armv4
arch-$(CONFIG_CPU_32v3)		:=-march=armv3m

# Note that GCC does not numerically define an architecture version
# macro, but instead defines a whole series of macros which makes
# testing for a specific architecture or later rather impossible.
cpp-$(CONFIG_CPU_32v7M)		:=-D__LINUX_ARM_ARCH__=7
cpp-$(CONFIG_CPU_32v7)		:=-D__LINUX_ARM_ARCH__=7
cpp-$(CONFIG_CPU_32v6)		:=-D__LINUX_ARM_ARCH__=6
# Only override the compiler option if ARMv6. The ARMv6K extensions are
# always available in ARMv7
ifeq ($(CONFIG_CPU_32v6),y)
cpp-$(CONFIG_CPU_32v6K)		:=-D__LINUX_ARM_ARCH__=6
endif
cpp-$(CONFIG_CPU_32v5)		:=-D__LINUX_ARM_ARCH__=5
cpp-$(CONFIG_CPU_32v4T)		:=-D__LINUX_ARM_ARCH__=4
cpp-$(CONFIG_CPU_32v4)		:=-D__LINUX_ARM_ARCH__=4
cpp-$(CONFIG_CPU_32v3)		:=-D__LINUX_ARM_ARCH__=3

# This selects how we optimise for the processor.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
tune-$(CONFIG_CPU_ARM7TDMI)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM720T)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM740T)	:=-mtune=arm7tdmi
tune-$(CONFIG_CPU_ARM9TDMI)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM940T)	:=-mtune=arm9tdmi
<<<<<<< HEAD
tune-$(CONFIG_CPU_ARM946E)	:=$(call cc-option,-mtune=arm9e,-mtune=arm9tdmi)
=======
tune-$(CONFIG_CPU_ARM946E)	:=-mtune=arm9e
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
tune-$(CONFIG_CPU_ARM920T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM922T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM925T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM926T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_FA526)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_SA110)	:=-mtune=strongarm110
tune-$(CONFIG_CPU_SA1100)	:=-mtune=strongarm1100
<<<<<<< HEAD
tune-$(CONFIG_CPU_XSCALE)	:=$(call cc-option,-mtune=xscale,-mtune=strongarm110) -Wa,-mcpu=xscale
tune-$(CONFIG_CPU_XSC3)		:=$(call cc-option,-mtune=xscale,-mtune=strongarm110) -Wa,-mcpu=xscale
tune-$(CONFIG_CPU_FEROCEON)	:=$(call cc-option,-mtune=marvell-f,-mtune=xscale)
tune-$(CONFIG_CPU_V6)		:=$(call cc-option,-mtune=arm1136j-s,-mtune=strongarm)
tune-$(CONFIG_CPU_V6K)		:=$(call cc-option,-mtune=arm1136j-s,-mtune=strongarm)

ifeq ($(CONFIG_AEABI),y)
CFLAGS_ABI	:=-mabi=aapcs-linux -mno-thumb-interwork
=======
tune-$(CONFIG_CPU_XSCALE)	:=-mtune=xscale
tune-$(CONFIG_CPU_XSC3)		:=-mtune=xscale
tune-$(CONFIG_CPU_FEROCEON)	:=-mtune=xscale
tune-$(CONFIG_CPU_V6)		:=-mtune=arm1136j-s
tune-$(CONFIG_CPU_V6K)		:=-mtune=arm1136j-s

ifeq ($(CONFIG_AEABI),y)
CFLAGS_ABI	:=-mabi=aapcs-linux -mfpu=vfp
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
else
CFLAGS_ABI	:=$(call cc-option,-mapcs-32,-mabi=apcs-gnu) $(call cc-option,-mno-thumb-interwork,)
endif

ifeq ($(CONFIG_ARM_UNWIND),y)
CFLAGS_ABI	+=-funwind-tables
endif

<<<<<<< HEAD
ifeq ($(CONFIG_THUMB2_KERNEL),y)
AFLAGS_AUTOIT	:=$(call as-option,-Wa$(comma)-mimplicit-it=always,-Wa$(comma)-mauto-it)
AFLAGS_NOWARN	:=$(call as-option,-Wa$(comma)-mno-warn-deprecated,-Wa$(comma)-W)
CFLAGS_THUMB2	:=-mthumb $(AFLAGS_AUTOIT) $(AFLAGS_NOWARN)
AFLAGS_THUMB2	:=$(CFLAGS_THUMB2) -Wa$(comma)-mthumb
# Work around buggy relocation from gas if requested:
ifeq ($(CONFIG_THUMB2_AVOID_R_ARM_THM_JUMP11),y)
CFLAGS_MODULE	+=-fno-optimize-sibling-calls
endif
endif

# Need -Uarm for gcc < 3.x
KBUILD_CFLAGS	+=$(CFLAGS_ABI) $(CFLAGS_THUMB2) $(arch-y) $(tune-y) $(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uarm
KBUILD_AFLAGS	+=$(CFLAGS_ABI) $(AFLAGS_THUMB2) $(arch-y) $(tune-y) -include asm/unified.h -msoft-float

CHECKFLAGS	+= -D__arm__

#Default value
head-y		:= arch/arm/kernel/head$(MMUEXT).o arch/arm/kernel/init_task.o
textofs-y	:= 0x00008000
textofs-$(CONFIG_ARCH_CLPS711X) := 0x00028000
# We don't want the htc bootloader to corrupt kernel during resume
textofs-$(CONFIG_PM_H1940)      := 0x00108000
=======
ifeq ($(CONFIG_CC_IS_CLANG),y)
CFLAGS_ABI	+= -meabi gnu
endif

ifeq ($(CONFIG_CURRENT_POINTER_IN_TPIDRURO),y)
KBUILD_CFLAGS	+= -mtp=cp15
endif

# Accept old syntax despite ".syntax unified"
AFLAGS_NOWARN	:=$(call as-option,-Wa$(comma)-mno-warn-deprecated,-Wa$(comma)-W)

ifeq ($(CONFIG_THUMB2_KERNEL),y)
CFLAGS_ISA	:=-Wa,-mimplicit-it=always $(AFLAGS_NOWARN)
AFLAGS_ISA	:=$(CFLAGS_ISA) -Wa$(comma)-mthumb
CFLAGS_ISA	+=-mthumb
else
CFLAGS_ISA	:=$(call cc-option,-marm,) $(AFLAGS_NOWARN)
AFLAGS_ISA	:=$(CFLAGS_ISA)
endif

# Need -Uarm for gcc < 3.x
KBUILD_CPPFLAGS	+=$(cpp-y)
KBUILD_CFLAGS	+=$(CFLAGS_ABI) $(CFLAGS_ISA) $(arch-y) $(tune-y) $(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uarm
KBUILD_AFLAGS	+=$(CFLAGS_ABI) $(AFLAGS_ISA) -Wa,$(arch-y) $(tune-y) -include asm/unified.h -msoft-float

CHECKFLAGS	+= -D__arm__

# Text offset. This list is sorted numerically by address in order to
# provide a means to avoid/resolve conflicts in multi-arch kernels.
# Note: the 32kB below this value is reserved for use by the kernel
# during boot, and this offset is critical to the functioning of
# kexec-tools.
textofs-y	:= 0x00008000
# RTD1195 has Boot ROM at start of address space
textofs-$(CONFIG_ARCH_REALTEK)  := 0x00108000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# SA1111 DMA bug: we don't want the kernel to live in precious DMA-able memory
ifeq ($(CONFIG_ARCH_SA1100),y)
textofs-$(CONFIG_SA1111) := 0x00208000
endif
<<<<<<< HEAD

# Machine directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
machine-$(CONFIG_ARCH_AT91)		:= at91
machine-$(CONFIG_ARCH_BCMRING)		:= bcmring
machine-$(CONFIG_ARCH_CLPS711X)		:= clps711x
machine-$(CONFIG_ARCH_CNS3XXX)		:= cns3xxx
machine-$(CONFIG_ARCH_DAVINCI)		:= davinci
machine-$(CONFIG_ARCH_DOVE)		:= dove
machine-$(CONFIG_ARCH_EBSA110)		:= ebsa110
machine-$(CONFIG_ARCH_EP93XX)		:= ep93xx
machine-$(CONFIG_ARCH_GEMINI)		:= gemini
machine-$(CONFIG_ARCH_H720X)		:= h720x
machine-$(CONFIG_ARCH_HIGHBANK)		:= highbank
machine-$(CONFIG_ARCH_INTEGRATOR)	:= integrator
machine-$(CONFIG_ARCH_IOP13XX)		:= iop13xx
machine-$(CONFIG_ARCH_IOP32X)		:= iop32x
machine-$(CONFIG_ARCH_IOP33X)		:= iop33x
machine-$(CONFIG_ARCH_IXP2000)		:= ixp2000
machine-$(CONFIG_ARCH_IXP23XX)		:= ixp23xx
machine-$(CONFIG_ARCH_IXP4XX)		:= ixp4xx
machine-$(CONFIG_ARCH_KIRKWOOD)		:= kirkwood
machine-$(CONFIG_ARCH_KS8695)		:= ks8695
machine-$(CONFIG_ARCH_LPC32XX)		:= lpc32xx
machine-$(CONFIG_ARCH_MMP)		:= mmp
machine-$(CONFIG_ARCH_MSM)		:= msm
machine-$(CONFIG_ARCH_MV78XX0)		:= mv78xx0
machine-$(CONFIG_ARCH_IMX_V4_V5)	:= imx
machine-$(CONFIG_ARCH_IMX_V6_V7)	:= imx
machine-$(CONFIG_ARCH_MXS)		:= mxs
machine-$(CONFIG_ARCH_NETX)		:= netx
machine-$(CONFIG_ARCH_NOMADIK)		:= nomadik
machine-$(CONFIG_ARCH_OMAP1)		:= omap1
machine-$(CONFIG_ARCH_OMAP2)		:= omap2
machine-$(CONFIG_ARCH_OMAP3)		:= omap2
machine-$(CONFIG_ARCH_OMAP4)		:= omap2
machine-$(CONFIG_ARCH_ORION5X)		:= orion5x
machine-$(CONFIG_ARCH_PICOXCELL)	:= picoxcell
machine-$(CONFIG_ARCH_PNX4008)		:= pnx4008
machine-$(CONFIG_ARCH_PRIMA2)		:= prima2
machine-$(CONFIG_ARCH_PXA)		:= pxa
machine-$(CONFIG_ARCH_REALVIEW)		:= realview
machine-$(CONFIG_ARCH_RPC)		:= rpc
machine-$(CONFIG_ARCH_S3C24XX)		:= s3c24xx s3c2412 s3c2440
machine-$(CONFIG_ARCH_S3C64XX)		:= s3c64xx
machine-$(CONFIG_ARCH_S5P64X0)		:= s5p64x0
machine-$(CONFIG_ARCH_S5PC100)		:= s5pc100
machine-$(CONFIG_ARCH_S5PV210)		:= s5pv210
machine-$(CONFIG_ARCH_EXYNOS4)		:= exynos
machine-$(CONFIG_ARCH_EXYNOS5)		:= exynos
machine-$(CONFIG_ARCH_SA1100)		:= sa1100
machine-$(CONFIG_ARCH_SHARK)		:= shark
machine-$(CONFIG_ARCH_SHMOBILE) 	:= shmobile
machine-$(CONFIG_ARCH_TEGRA)		:= tegra
machine-$(CONFIG_ARCH_U300)		:= u300
machine-$(CONFIG_ARCH_U8500)		:= ux500
machine-$(CONFIG_ARCH_VERSATILE)	:= versatile
machine-$(CONFIG_ARCH_VEXPRESS)		:= vexpress
machine-$(CONFIG_ARCH_VT8500)		:= vt8500
machine-$(CONFIG_ARCH_W90X900)		:= w90x900
machine-$(CONFIG_FOOTBRIDGE)		:= footbridge
machine-$(CONFIG_MACH_SPEAR300)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR310)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR320)		:= spear3xx
machine-$(CONFIG_MACH_SPEAR600)		:= spear6xx
machine-$(CONFIG_ARCH_ZYNQ)		:= zynq

# Platform directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
plat-$(CONFIG_ARCH_MXC)		:= mxc
plat-$(CONFIG_ARCH_OMAP)	:= omap
plat-$(CONFIG_ARCH_S3C64XX)	:= samsung
plat-$(CONFIG_ARCH_ZYNQ)	:= versatile
plat-$(CONFIG_PLAT_IOP)		:= iop
plat-$(CONFIG_PLAT_NOMADIK)	:= nomadik
plat-$(CONFIG_PLAT_ORION)	:= orion
plat-$(CONFIG_PLAT_PXA)		:= pxa
plat-$(CONFIG_PLAT_S3C24XX)	:= s3c24xx samsung
plat-$(CONFIG_PLAT_S5P)		:= s5p samsung
plat-$(CONFIG_PLAT_SPEAR)	:= spear
plat-$(CONFIG_PLAT_VERSATILE)	:= versatile

ifeq ($(CONFIG_ARCH_EBSA110),y)
# This is what happens if you forget the IOCS16 line.
# PCMCIA cards stop working.
CFLAGS_3c589_cs.o :=-DISA_SIXTEEN_BIT_PERIPHERAL
export CFLAGS_3c589_cs.o
endif
=======
textofs-$(CONFIG_ARCH_QCOM_RESERVE_SMEM) := 0x00208000
textofs-$(CONFIG_ARCH_MESON) := 0x00208000
textofs-$(CONFIG_ARCH_AXXIA) := 0x00308000

# Machine directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
machine-$(CONFIG_ARCH_ACTIONS)		+= actions
machine-$(CONFIG_ARCH_ALPINE)		+= alpine
machine-$(CONFIG_ARCH_ARTPEC)		+= artpec
machine-$(CONFIG_ARCH_ASPEED)           += aspeed
machine-$(CONFIG_ARCH_AT91)		+= at91
machine-$(CONFIG_ARCH_AXXIA)		+= axxia
machine-$(CONFIG_ARCH_BCM)		+= bcm
machine-$(CONFIG_ARCH_BERLIN)		+= berlin
machine-$(CONFIG_ARCH_CLPS711X)		+= clps711x
machine-$(CONFIG_ARCH_DAVINCI)		+= davinci
machine-$(CONFIG_ARCH_DIGICOLOR)	+= digicolor
machine-$(CONFIG_ARCH_DOVE)		+= dove
machine-$(CONFIG_ARCH_EP93XX)		+= ep93xx
machine-$(CONFIG_ARCH_EXYNOS)		+= exynos
machine-$(CONFIG_ARCH_FOOTBRIDGE)	+= footbridge
machine-$(CONFIG_ARCH_GEMINI)		+= gemini
machine-$(CONFIG_ARCH_HIGHBANK)		+= highbank
machine-$(CONFIG_ARCH_HISI)		+= hisi
machine-$(CONFIG_ARCH_HPE)		+= hpe
machine-$(CONFIG_ARCH_IXP4XX)		+= ixp4xx
machine-$(CONFIG_ARCH_KEYSTONE)		+= keystone
machine-$(CONFIG_ARCH_LPC18XX)		+= lpc18xx
machine-$(CONFIG_ARCH_LPC32XX)		+= lpc32xx
machine-$(CONFIG_ARCH_MESON)		+= meson
machine-$(CONFIG_ARCH_MMP)		+= mmp
machine-$(CONFIG_ARCH_MV78XX0)		+= mv78xx0
machine-$(CONFIG_ARCH_MVEBU)		+= mvebu
machine-$(CONFIG_ARCH_MXC)		+= imx
machine-$(CONFIG_ARCH_MEDIATEK)		+= mediatek
machine-$(CONFIG_ARCH_MILBEAUT)		+= milbeaut
machine-$(CONFIG_ARCH_MXS)		+= mxs
machine-$(CONFIG_ARCH_MSTARV7)		+= mstar
machine-$(CONFIG_ARCH_NOMADIK)		+= nomadik
machine-$(CONFIG_ARCH_NPCM)		+= npcm
machine-$(CONFIG_ARCH_OMAP1)		+= omap1
machine-$(CONFIG_ARCH_OMAP2PLUS)	+= omap2
machine-$(CONFIG_ARCH_ORION5X)		+= orion5x
machine-$(CONFIG_ARCH_PXA)		+= pxa
machine-$(CONFIG_ARCH_QCOM)		+= qcom
machine-$(CONFIG_ARCH_REALTEK)		+= realtek
machine-$(CONFIG_ARCH_ROCKCHIP)		+= rockchip
machine-$(CONFIG_ARCH_RPC)		+= rpc
machine-$(CONFIG_PLAT_SAMSUNG)		+= s3c
machine-$(CONFIG_ARCH_S5PV210)		+= s5pv210
machine-$(CONFIG_ARCH_SA1100)		+= sa1100
machine-$(CONFIG_ARCH_RENESAS)	 	+= shmobile
machine-$(CONFIG_ARCH_INTEL_SOCFPGA)	+= socfpga
machine-$(CONFIG_ARCH_STI)		+= sti
machine-$(CONFIG_ARCH_STM32)		+= stm32
machine-$(CONFIG_ARCH_SUNXI)		+= sunxi
machine-$(CONFIG_ARCH_TEGRA)		+= tegra
machine-$(CONFIG_ARCH_U8500)		+= ux500
machine-$(CONFIG_ARCH_VT8500)		+= vt8500
machine-$(CONFIG_ARCH_ZYNQ)		+= zynq
machine-$(CONFIG_PLAT_VERSATILE)	+= versatile
machine-$(CONFIG_PLAT_SPEAR)		+= spear

# legacy platforms provide their own mach/*.h headers globally,
# these three are mutually exclusive
machdirs-$(CONFIG_ARCH_FOOTBRIDGE)	+= arch/arm/mach-footbridge
machdirs-$(CONFIG_ARCH_RPC)		+= arch/arm/mach-rpc
machdirs-$(CONFIG_ARCH_SA1100)		+= arch/arm/mach-sa1100
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%/include,$(machdirs-y))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# The byte offset of the kernel image in RAM from the start of RAM.
TEXT_OFFSET := $(textofs-y)

<<<<<<< HEAD
# The first directory contains additional information for the boot setup code
ifneq ($(machine-y),)
MACHINE  := arch/arm/mach-$(word 1,$(machine-y))/
else
MACHINE  :=
endif

machdirs := $(patsubst %,arch/arm/mach-%/,$(machine-y))
platdirs := $(patsubst %,arch/arm/plat-%/,$(plat-y))

ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(machdirs) $(platdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(machdirs) $(platdirs))
endif

export	TEXT_OFFSET GZFLAGS MMUEXT

# Do we have FASTFPE?
FASTFPE		:=arch/arm/fastfpe
ifeq ($(FASTFPE),$(wildcard $(FASTFPE)))
FASTFPE_OBJ	:=$(FASTFPE)/
endif

core-$(CONFIG_FPE_NWFPE)	+= arch/arm/nwfpe/
core-$(CONFIG_FPE_FASTFPE)	+= $(FASTFPE_OBJ)
core-$(CONFIG_VFP)		+= arch/arm/vfp/

# If we have a machine-specific directory, then include it in the build.
core-y				+= arch/arm/kernel/ arch/arm/mm/ arch/arm/common/
core-y				+= arch/arm/net/
core-y				+= arch/arm/crypto/
core-y				+= $(machdirs) $(platdirs)

drivers-$(CONFIG_OPROFILE)      += arch/arm/oprofile/
core-y				+= arch/arm/perfmon/
=======
export	TEXT_OFFSET GZFLAGS MMUEXT

# If we have a machine-specific directory, then include it in the build.
core-y				+= $(patsubst %,arch/arm/mach-%/,$(machine-y))
# For cleaning
core-				+= $(patsubst %,arch/arm/mach-%/,$(machine-))

core-$(CONFIG_PLAT_ORION)	+= arch/arm/plat-orion/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

libs-y				:= arch/arm/lib/ $(libs-y)

# Default target when executing plain make
<<<<<<< HEAD
ifeq ($(CONFIG_XIP_KERNEL),y)
KBUILD_IMAGE := xipImage
else ifeq ($(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE),y)
KBUILD_IMAGE := zImage-dtb
else
KBUILD_IMAGE := zImage
endif

# Build the DT binary blobs if we have OF configured
ifeq ($(CONFIG_USE_OF),y)
KBUILD_DTBS := dtbs
endif

all:	$(KBUILD_IMAGE) $(KBUILD_DTBS)

boot := arch/arm/boot

archprepare:
	$(Q)$(MAKE) $(build)=arch/arm/tools include/generated/mach-types.h
=======
boot := arch/arm/boot
ifeq ($(CONFIG_XIP_KERNEL),y)
KBUILD_IMAGE := $(boot)/xipImage
else
KBUILD_IMAGE := $(boot)/zImage
endif

ifeq ($(CONFIG_STACKPROTECTOR_PER_TASK),y)
prepare: stack_protector_prepare
ifeq ($(CONFIG_CC_HAVE_STACKPROTECTOR_TLS),y)
stack_protector_prepare: prepare0
	$(eval KBUILD_CFLAGS += \
		-mstack-protector-guard=tls \
		-mstack-protector-guard-offset=$(shell	\
			awk '{if ($$2 == "TSK_STACK_CANARY") print $$3;}'\
				include/generated/asm-offsets.h))
else
stack_protector_prepare: prepare0
	$(eval SSP_PLUGIN_CFLAGS := \
		-fplugin-arg-arm_ssp_per_task_plugin-offset=$(shell	\
			awk '{if ($$2 == "TSK_STACK_CANARY") print $$3;}'\
				include/generated/asm-offsets.h))
	$(eval KBUILD_CFLAGS += $(SSP_PLUGIN_CFLAGS))
	$(eval GCC_PLUGINS_CFLAGS += $(SSP_PLUGIN_CFLAGS))
endif
endif

all:	$(notdir $(KBUILD_IMAGE))


archheaders:
	$(Q)$(MAKE) $(build)=arch/arm/tools uapi

archprepare:
	$(Q)$(MAKE) $(build)=arch/arm/tools kapi
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# Convert bzImage to zImage
bzImage: zImage

<<<<<<< HEAD
zImage Image xipImage bootpImage uImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

zinstall uinstall install: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $@

%.dtb: scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

dtbs: scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

zImage-dtb: vmlinux scripts
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

# We use MRPROPER_FILES and CLEAN_FILES now
archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

# My testing targets (bypasses dependencies)
bp:;	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/bootpImage
i zi:;	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $@

=======
BOOT_TARGETS	= zImage Image xipImage bootpImage uImage
INSTALL_TARGETS	= zinstall uinstall install

PHONY += bzImage $(BOOT_TARGETS) $(INSTALL_TARGETS)

bootpImage uImage: zImage
zImage: Image

$(BOOT_TARGETS): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@
	@$(kecho) '  Kernel: $(boot)/$@ is ready'

$(INSTALL_TARGETS): KBUILD_IMAGE = $(boot)/$(patsubst %install,%Image,$@)
$(INSTALL_TARGETS):
	$(call cmd,install)

vdso-install-$(CONFIG_VDSO) += arch/arm/vdso/vdso.so.dbg

# My testing targets (bypasses dependencies)
bp:;	$(Q)$(MAKE) $(build)=$(boot) $(boot)/bootpImage

include $(srctree)/scripts/Makefile.defconf
PHONY += multi_v7_lpae_defconfig
multi_v7_lpae_defconfig:
	$(call merge_into_defconfig,multi_v7_defconfig,lpae)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

define archhelp
  echo  '* zImage        - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
  echo  '  Image         - Uncompressed kernel image (arch/$(ARCH)/boot/Image)'
  echo  '* xipImage      - XIP kernel image, if configured (arch/$(ARCH)/boot/xipImage)'
  echo  '  uImage        - U-Boot wrapped zImage'
  echo  '  bootpImage    - Combined zImage and initial RAM disk'
  echo  '                  (supply initrd image via make variable INITRD=<path>)'
<<<<<<< HEAD
  echo  '* dtbs          - Build device tree blobs for enabled boards'
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  echo  '  install       - Install uncompressed kernel'
  echo  '  zinstall      - Install compressed kernel'
  echo  '  uinstall      - Install U-Boot wrapped compressed kernel'
  echo  '                  Install using (your) ~/bin/$(INSTALLKERNEL) or'
  echo  '                  (distribution) /sbin/$(INSTALLKERNEL) or'
  echo  '                  install to $$(INSTALL_PATH) and run lilo'
<<<<<<< HEAD
=======
  echo
  echo  '  multi_v7_lpae_defconfig     - multi_v7_defconfig with CONFIG_ARM_LPAE enabled'
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endef
