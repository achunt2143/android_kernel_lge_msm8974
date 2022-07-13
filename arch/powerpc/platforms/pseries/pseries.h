<<<<<<< HEAD
/*
 * Copyright 2006 IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2006 IBM Corporation.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _PSERIES_PSERIES_H
#define _PSERIES_PSERIES_H

#include <linux/interrupt.h>
<<<<<<< HEAD

struct device_node;

extern void request_event_sources_irqs(struct device_node *np,
=======
#include <asm/rtas.h>

struct device_node;

void __init request_event_sources_irqs(struct device_node *np,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       irq_handler_t handler, const char *name);

#include <linux/of.h>

<<<<<<< HEAD
extern void __init fw_feature_init(const char *hypertas, unsigned long len);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct pt_regs;

extern int pSeries_system_reset_exception(struct pt_regs *regs);
extern int pSeries_machine_check_exception(struct pt_regs *regs);
<<<<<<< HEAD

#ifdef CONFIG_SMP
extern void smp_init_pseries_mpic(void);
extern void smp_init_pseries_xics(void);
#else
static inline void smp_init_pseries_mpic(void) { };
static inline void smp_init_pseries_xics(void) { };
#endif

#ifdef CONFIG_KEXEC
extern void setup_kexec_cpu_down_xics(void);
extern void setup_kexec_cpu_down_mpic(void);
#else
static inline void setup_kexec_cpu_down_xics(void) { }
static inline void setup_kexec_cpu_down_mpic(void) { }
#endif
=======
extern long pseries_machine_check_realmode(struct pt_regs *regs);
void pSeries_machine_check_log_err(void);

#ifdef CONFIG_SMP
extern void smp_init_pseries(void);

/* Get state of physical CPU from query_cpu_stopped */
int smp_query_cpu_stopped(unsigned int pcpu);
#define QCSS_STOPPED 0
#define QCSS_STOPPING 1
#define QCSS_NOT_STOPPED 2
#define QCSS_HARDWARE_ERROR -1
#define QCSS_HARDWARE_BUSY -2
#else
static inline void smp_init_pseries(void) { }
#endif

extern void pseries_kexec_cpu_down(int crash_shutdown, int secondary);
void pseries_machine_kexec(struct kimage *image);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void pSeries_final_fixup(void);

/* Poweron flag used for enabling auto ups restart */
extern unsigned long rtas_poweron_auto;

<<<<<<< HEAD
/* Provided by HVC VIO */
extern void hvc_vio_init_early(void);

/* Dynamic logical Partitioning/Mobility */
extern void dlpar_free_cc_nodes(struct device_node *);
extern void dlpar_free_cc_property(struct property *);
extern struct device_node *dlpar_configure_connector(u32);
extern int dlpar_attach_node(struct device_node *);
extern int dlpar_detach_node(struct device_node *);

/* Snooze Delay, pseries_idle */
DECLARE_PER_CPU(long, smt_snooze_delay);
=======
/* Dynamic logical Partitioning/Mobility */
extern void dlpar_free_cc_nodes(struct device_node *);
extern void dlpar_free_cc_property(struct property *);
extern struct device_node *dlpar_configure_connector(__be32,
						struct device_node *);
extern int dlpar_attach_node(struct device_node *, struct device_node *);
extern int dlpar_detach_node(struct device_node *);
extern int dlpar_acquire_drc(u32 drc_index);
extern int dlpar_release_drc(u32 drc_index);
extern int dlpar_unisolate_drc(u32 drc_index);
extern void post_mobility_fixup(void);

void queue_hotplug_event(struct pseries_hp_errorlog *hp_errlog);
int handle_dlpar_errorlog(struct pseries_hp_errorlog *hp_errlog);

#ifdef CONFIG_MEMORY_HOTPLUG
int dlpar_memory(struct pseries_hp_errorlog *hp_elog);
int dlpar_hp_pmem(struct pseries_hp_errorlog *hp_elog);
#else
static inline int dlpar_memory(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
static inline int dlpar_hp_pmem(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
#endif

#ifdef CONFIG_HOTPLUG_CPU
int dlpar_cpu(struct pseries_hp_errorlog *hp_elog);
void pseries_cpu_hotplug_init(void);
#else
static inline int dlpar_cpu(struct pseries_hp_errorlog *hp_elog)
{
	return -EOPNOTSUPP;
}
static inline void pseries_cpu_hotplug_init(void) { }
#endif

/* PCI root bridge prepare function override for pseries */
struct pci_host_bridge;
int pseries_root_bridge_prepare(struct pci_host_bridge *bridge);

extern struct pci_controller_ops pseries_pci_controller_ops;
int pseries_msi_allocate_domains(struct pci_controller *phb);
void pseries_msi_free_domains(struct pci_controller *phb);

extern int CMO_PrPSP;
extern int CMO_SecPSP;
extern unsigned long CMO_PageSize;

static inline int cmo_get_primary_psp(void)
{
	return CMO_PrPSP;
}

static inline int cmo_get_secondary_psp(void)
{
	return CMO_SecPSP;
}

static inline unsigned long cmo_get_page_size(void)
{
	return CMO_PageSize;
}

int dlpar_workqueue_init(void);

extern u32 pseries_security_flavor;
void pseries_setup_security_mitigations(void);

#ifdef CONFIG_PPC_64S_HASH_MMU
void pseries_lpar_read_hblkrm_characteristics(void);
#else
static inline void pseries_lpar_read_hblkrm_characteristics(void) { }
#endif

void pseries_rng_init(void);
#ifdef CONFIG_SPAPR_TCE_IOMMU
struct iommu_group *pSeries_pci_device_group(struct pci_controller *hose,
					     struct pci_dev *pdev);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _PSERIES_PSERIES_H */
