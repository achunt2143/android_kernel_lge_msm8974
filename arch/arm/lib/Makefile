<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# linux/arch/arm/lib/Makefile
#
# Copyright (C) 1995-2000 Russell King
#

<<<<<<< HEAD
lib-y		:= backtrace.o changebit.o csumipv6.o csumpartial.o   \
		   csumpartialcopy.o csumpartialcopyuser.o clearbit.o \
		   delay.o delay-loop.o findbit.o memchr.o memcpy.o   \
		   memmove.o memset.o memzero.o setbit.o              \
		   strncpy_from_user.o strnlen_user.o                 \
=======
lib-y		:= changebit.o csumipv6.o csumpartial.o               \
		   csumpartialcopy.o csumpartialcopyuser.o clearbit.o \
		   delay.o delay-loop.o findbit.o memchr.o memcpy.o   \
		   memmove.o memset.o setbit.o                        \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   strchr.o strrchr.o                                 \
		   testchangebit.o testclearbit.o testsetbit.o        \
		   ashldi3.o ashrdi3.o lshrdi3.o muldi3.o             \
		   ucmpdi2.o lib1funcs.o div64.o                      \
		   io-readsb.o io-writesb.o io-readsl.o io-writesl.o  \
<<<<<<< HEAD
		   call_with_stack.o

mmu-y	:= clear_user.o copy_page.o getuser.o putuser.o cmp_page.o

# the code in uaccess.S is not preemption safe and
# probably faster on ARMv3 only
ifeq ($(CONFIG_PREEMPT),y)
  mmu-y	+= copy_from_user.o copy_to_user.o
else
ifneq ($(CONFIG_CPU_32v3),y)
  mmu-y	+= copy_from_user.o copy_to_user.o
else
  mmu-y	+= uaccess.o
endif
=======
		   call_with_stack.o bswapsdi2.o

mmu-y		:= clear_user.o copy_page.o getuser.o putuser.o       \
		   copy_from_user.o copy_to_user.o

ifdef CONFIG_CC_IS_CLANG
  lib-y	+= backtrace-clang.o
else
  lib-y	+= backtrace.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
endif

# using lib_ here won't override already available weak symbols
obj-$(CONFIG_UACCESS_WITH_MEMCPY) += uaccess_with_memcpy.o

lib-$(CONFIG_MMU) += $(mmu-y)

ifeq ($(CONFIG_CPU_32v3),y)
  lib-y	+= io-readsw-armv3.o io-writesw-armv3.o
else
  lib-y	+= io-readsw-armv4.o io-writesw-armv4.o
endif

<<<<<<< HEAD
lib-$(CONFIG_ARCH_RPC)		+= ecard.o io-acorn.o floppydma.o
lib-$(CONFIG_ARCH_SHARK)	+= io-shark.o

$(obj)/csumpartialcopy.o:	$(obj)/csumpartialcopygeneric.S
$(obj)/csumpartialcopyuser.o:	$(obj)/csumpartialcopygeneric.S
=======
$(obj)/csumpartialcopy.o:	$(obj)/csumpartialcopygeneric.S
$(obj)/csumpartialcopyuser.o:	$(obj)/csumpartialcopygeneric.S

ifeq ($(CONFIG_KERNEL_MODE_NEON),y)
  NEON_FLAGS			:= -march=armv7-a -mfloat-abi=softfp -mfpu=neon
  CFLAGS_xor-neon.o		+= $(NEON_FLAGS)
  obj-$(CONFIG_XOR_BLOCKS)	+= xor-neon.o
endif

obj-$(CONFIG_FUNCTION_ERROR_INJECTION) += error-inject.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
