<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for caching in a mounted filesystem
#

cachefiles-y := \
<<<<<<< HEAD
	bind.o \
	daemon.o \
	interface.o \
	key.o \
	main.o \
	namei.o \
	rdwr.o \
	security.o \
	xattr.o

cachefiles-$(CONFIG_CACHEFILES_HISTOGRAM) += proc.o
=======
	cache.o \
	daemon.o \
	interface.o \
	io.o \
	key.o \
	main.o \
	namei.o \
	security.o \
	volume.o \
	xattr.o

cachefiles-$(CONFIG_CACHEFILES_ERROR_INJECTION) += error_inject.o
cachefiles-$(CONFIG_CACHEFILES_ONDEMAND) += ondemand.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_CACHEFILES) := cachefiles.o
