<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for key management
#

<<<<<<< HEAD
=======
#
# Core
#
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
obj-y := \
	gc.o \
	key.o \
	keyring.o \
	keyctl.o \
	permission.o \
	process_keys.o \
	request_key.o \
	request_key_auth.o \
	user_defined.o
<<<<<<< HEAD

obj-$(CONFIG_TRUSTED_KEYS) += trusted.o
obj-$(CONFIG_ENCRYPTED_KEYS) += encrypted-keys/
obj-$(CONFIG_KEYS_COMPAT) += compat.o
obj-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_SYSCTL) += sysctl.o
=======
compat-obj-$(CONFIG_KEY_DH_OPERATIONS) += compat_dh.o
obj-$(CONFIG_COMPAT) += compat.o $(compat-obj-y)
obj-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_SYSCTL) += sysctl.o
obj-$(CONFIG_PERSISTENT_KEYRINGS) += persistent.o
obj-$(CONFIG_KEY_DH_OPERATIONS) += dh.o
obj-$(CONFIG_ASYMMETRIC_KEY_TYPE) += keyctl_pkey.o

#
# Key types
#
obj-$(CONFIG_BIG_KEYS) += big_key.o
obj-$(CONFIG_TRUSTED_KEYS) += trusted-keys/
obj-$(CONFIG_ENCRYPTED_KEYS) += encrypted-keys/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
