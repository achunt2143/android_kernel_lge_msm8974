<<<<<<< HEAD
###
# scripts contains sources for various helper programs used throughout
# the kernel for the build process.
# ---------------------------------------------------------------------------
# kallsyms:      Find all symbols in vmlinux
# pnmttologo:    Convert pnm files to logo files
# conmakehash:   Create chartable
# conmakehash:	 Create arrays for initializing the kernel console tables
# docproc:       Used in Documentation/DocBook

HOST_EXTRACFLAGS += -I$(srctree)/tools/include

hostprogs-$(CONFIG_KALLSYMS)     += kallsyms
hostprogs-$(CONFIG_LOGO)         += pnmtologo
hostprogs-$(CONFIG_VT)           += conmakehash
hostprogs-$(CONFIG_IKCONFIG)     += bin2c
hostprogs-$(BUILD_C_RECORDMCOUNT) += recordmcount

always		:= $(hostprogs-y) $(hostprogs-m)

# The following hostprogs-y programs are only build on demand
hostprogs-y += unifdef docproc

# These targets are used internally to avoid "is up to date" messages
PHONY += build_unifdef
build_unifdef: scripts/unifdef FORCE
	@:
build_docproc: scripts/docproc FORCE
	@:

subdir-$(CONFIG_MODVERSIONS) += genksyms
subdir-y                     += mod
subdir-$(CONFIG_SECURITY_SELINUX) += selinux
subdir-$(CONFIG_DTC)         += dtc

# Let clean descend into subdirs
subdir-	+= basic kconfig package selinux
=======
# SPDX-License-Identifier: GPL-2.0
###
# scripts contains sources for various helper programs used throughout
# the kernel for the build process.

hostprogs-always-$(CONFIG_KALLSYMS)			+= kallsyms
hostprogs-always-$(BUILD_C_RECORDMCOUNT)		+= recordmcount
hostprogs-always-$(CONFIG_BUILDTIME_TABLE_SORT)		+= sorttable
hostprogs-always-$(CONFIG_ASN1)				+= asn1_compiler
hostprogs-always-$(CONFIG_MODULE_SIG_FORMAT)		+= sign-file
hostprogs-always-$(CONFIG_SYSTEM_EXTRA_CERTIFICATE)	+= insert-sys-cert
hostprogs-always-$(CONFIG_RUST_KERNEL_DOCTESTS)		+= rustdoc_test_builder
hostprogs-always-$(CONFIG_RUST_KERNEL_DOCTESTS)		+= rustdoc_test_gen

ifneq ($(or $(CONFIG_X86_64),$(CONFIG_LOONGARCH)),)
always-$(CONFIG_RUST)					+= target.json
filechk_rust_target = $< < include/config/auto.conf

$(obj)/target.json: scripts/generate_rust_target include/config/auto.conf FORCE
	$(call filechk,rust_target)
endif

hostprogs += generate_rust_target
generate_rust_target-rust := y
rustdoc_test_builder-rust := y
rustdoc_test_gen-rust := y

HOSTCFLAGS_sorttable.o = -I$(srctree)/tools/include
HOSTLDLIBS_sorttable = -lpthread
HOSTCFLAGS_asn1_compiler.o = -I$(srctree)/include
HOSTCFLAGS_sign-file.o = $(shell $(HOSTPKG_CONFIG) --cflags libcrypto 2> /dev/null)
HOSTLDLIBS_sign-file = $(shell $(HOSTPKG_CONFIG) --libs libcrypto 2> /dev/null || echo -lcrypto)

ifdef CONFIG_UNWINDER_ORC
ifeq ($(ARCH),x86_64)
SRCARCH := x86
endif
ifeq ($(ARCH),loongarch)
SRCARCH := loongarch
endif
HOSTCFLAGS_sorttable.o += -I$(srctree)/tools/arch/$(SRCARCH)/include
HOSTCFLAGS_sorttable.o += -DUNWINDER_ORC_ENABLED
endif

ifdef CONFIG_BUILDTIME_MCOUNT_SORT
HOSTCFLAGS_sorttable.o += -DMCOUNT_SORT_ENABLED
endif

# The following programs are only built on demand
hostprogs += unifdef

# The module linker script is preprocessed on demand
targets += module.lds

subdir-$(CONFIG_GCC_PLUGINS) += gcc-plugins
subdir-$(CONFIG_MODVERSIONS) += genksyms
subdir-$(CONFIG_SECURITY_SELINUX) += selinux

# Let clean descend into subdirs
subdir-	+= basic dtc gdb kconfig mod
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
