<<<<<<< HEAD
subdir-				:= realmode

obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_rm.o wakeup_$(BITS).o
=======
# SPDX-License-Identifier: GPL-2.0

obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_$(BITS).o
obj-$(CONFIG_ACPI_APEI)		+= apei.o
obj-$(CONFIG_ACPI_CPPC_LIB)	+= cppc.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

ifneq ($(CONFIG_ACPI_PROCESSOR),)
obj-y				+= cstate.o
endif

<<<<<<< HEAD
$(obj)/wakeup_rm.o:    $(obj)/realmode/wakeup.bin

$(obj)/realmode/wakeup.bin: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/realmode

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
