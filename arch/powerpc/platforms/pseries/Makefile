<<<<<<< HEAD
ccflags-$(CONFIG_PPC64)			:= -mno-minimal-toc
ccflags-$(CONFIG_PPC_PSERIES_DEBUG)	+= -DDEBUG

obj-y			:= lpar.o hvCall.o nvram.o reconfig.o \
			   setup.o iommu.o event_sources.o ras.o \
			   firmware.o power.o dlpar.o mobility.o
obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_SCANLOG)	+= scanlog.o
obj-$(CONFIG_EEH)	+= eeh.o eeh_dev.o eeh_cache.o eeh_driver.o \
			   eeh_event.o eeh_sysfs.o eeh_pseries.o
obj-$(CONFIG_KEXEC)	+= kexec.o
obj-$(CONFIG_PCI)	+= pci.o pci_dlpar.o
obj-$(CONFIG_PSERIES_MSI)	+= msi.o
obj-$(CONFIG_PSERIES_ENERGY)	+= pseries_energy.o

obj-$(CONFIG_HOTPLUG_CPU)	+= hotplug-cpu.o
obj-$(CONFIG_MEMORY_HOTPLUG)	+= hotplug-memory.o
=======
# SPDX-License-Identifier: GPL-2.0
ccflags-$(CONFIG_PPC64)			:= $(NO_MINIMAL_TOC)
ccflags-$(CONFIG_PPC_PSERIES_DEBUG)	+= -DDEBUG

obj-y			:= lpar.o hvCall.o nvram.o reconfig.o \
			   of_helpers.o rtas-work-area.o papr-sysparm.o \
			   papr-vpd.o \
			   setup.o iommu.o event_sources.o ras.o \
			   firmware.o power.o dlpar.o mobility.o rng.o \
			   pci.o pci_dlpar.o eeh_pseries.o msi.o \
			   papr_platform_attributes.o dtl.o
obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_KEXEC_CORE)	+= kexec.o
obj-$(CONFIG_PSERIES_ENERGY)	+= pseries_energy.o

obj-$(CONFIG_HOTPLUG_CPU)	+= hotplug-cpu.o
obj-$(CONFIG_MEMORY_HOTPLUG)	+= hotplug-memory.o pmem.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_HVC_CONSOLE)	+= hvconsole.o
obj-$(CONFIG_HVCS)		+= hvcserver.o
obj-$(CONFIG_HCALL_STATS)	+= hvCall_inst.o
obj-$(CONFIG_CMM)		+= cmm.o
<<<<<<< HEAD
obj-$(CONFIG_DTL)		+= dtl.o
obj-$(CONFIG_IO_EVENT_IRQ)	+= io_event_irq.o
obj-$(CONFIG_PSERIES_IDLE)	+= processor_idle.o

ifeq ($(CONFIG_PPC_PSERIES),y)
obj-$(CONFIG_SUSPEND)		+= suspend.o
endif
=======
obj-$(CONFIG_IO_EVENT_IRQ)	+= io_event_irq.o
obj-$(CONFIG_LPARCFG)		+= lparcfg.o
obj-$(CONFIG_IBMVIO)		+= vio.o
obj-$(CONFIG_IBMEBUS)		+= ibmebus.o
obj-$(CONFIG_PAPR_SCM)		+= papr_scm.o
obj-$(CONFIG_PPC_SPLPAR)	+= vphn.o
obj-$(CONFIG_PPC_SVM)		+= svm.o
obj-$(CONFIG_FA_DUMP)		+= rtas-fadump.o
obj-$(CONFIG_PSERIES_PLPKS)	+= plpks.o
obj-$(CONFIG_PPC_SECURE_BOOT)	+= plpks-secvar.o
obj-$(CONFIG_PSERIES_PLPKS_SED)	+= plpks_sed_ops.o
obj-$(CONFIG_SUSPEND)		+= suspend.o
obj-$(CONFIG_PPC_VAS)		+= vas.o vas-sysfs.o

obj-$(CONFIG_ARCH_HAS_CC_PLATFORM)	+= cc_platform.o

# nothing that operates in real mode is safe for KASAN
KASAN_SANITIZE_ras.o := n
KASAN_SANITIZE_kexec.o := n
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
