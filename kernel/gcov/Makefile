<<<<<<< HEAD
ccflags-y := -DSRCTREE='"$(srctree)"' -DOBJTREE='"$(objtree)"'

obj-$(CONFIG_GCOV_KERNEL) := base.o fs.o gcc_3_4.o
=======
# SPDX-License-Identifier: GPL-2.0
ccflags-y := -DSRCTREE='"$(srctree)"' -DOBJTREE='"$(objtree)"'

obj-y := base.o fs.o
obj-$(CONFIG_CC_IS_GCC) += gcc_base.o gcc_4_7.o
CFLAGS_gcc_base.o += -Wno-missing-prototypes -Wno-missing-declarations
obj-$(CONFIG_CC_IS_CLANG) += clang.o
CFLAGS_clang.o += -Wno-missing-prototypes -Wno-missing-declarations
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
