<<<<<<< HEAD
# Makefile for the CAPI subsystem.

# Ordering constraints: kernelcapi.o first

# Each configuration option enables a list of files.

obj-$(CONFIG_ISDN_CAPI)			+= kernelcapi.o
obj-$(CONFIG_ISDN_CAPI_CAPI20)		+= capi.o 
obj-$(CONFIG_ISDN_CAPI_CAPIDRV)		+= capidrv.o

# Multipart objects.

kernelcapi-y				:= kcapi.o capiutil.o capilib.o
kernelcapi-$(CONFIG_PROC_FS)		+= kcapi_proc.o
=======
# SPDX-License-Identifier: GPL-2.0
# Makefile for the CAPI subsystem used by BT_CMTP

obj-$(CONFIG_BT_CMTP)			+= kernelcapi.o
kernelcapi-y				:= kcapi.o capiutil.o capi.o kcapi_proc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
