<<<<<<< HEAD
obj-$(CONFIG_ACPI_APEI)		+= apei.o
obj-$(CONFIG_ACPI_APEI_GHES)	+= ghes.o
obj-$(CONFIG_ACPI_APEI_EINJ)	+= einj.o
obj-$(CONFIG_ACPI_APEI_ERST_DEBUG) += erst-dbg.o

apei-y := apei-base.o hest.o cper.o erst.o
=======
# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_ACPI_APEI)		+= apei.o
obj-$(CONFIG_ACPI_APEI_GHES)	+= ghes.o
obj-$(CONFIG_ACPI_APEI_EINJ)	+= einj.o
einj-y				:= einj-core.o
einj-$(CONFIG_ACPI_APEI_EINJ_CXL) += einj-cxl.o
obj-$(CONFIG_ACPI_APEI_ERST_DEBUG) += erst-dbg.o

apei-y := apei-base.o hest.o erst.o bert.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
