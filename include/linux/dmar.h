<<<<<<< HEAD
/*
 * Copyright (c) 2006, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2006, Intel Corporation.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) Ashok Raj <ashok.raj@intel.com>
 * Copyright (C) Shaohua Li <shaohua.li@intel.com>
 */

#ifndef __DMAR_H__
#define __DMAR_H__

#include <linux/acpi.h>
#include <linux/types.h>
#include <linux/msi.h>
#include <linux/irqreturn.h>
<<<<<<< HEAD

struct acpi_dmar_header;

/* DMAR Flags */
#define DMAR_INTR_REMAP		0x1
#define DMAR_X2APIC_OPT_OUT	0x2

struct intel_iommu;
=======
#include <linux/rwsem.h>
#include <linux/rculist.h>

struct acpi_dmar_header;

#define DMAR_UNITS_SUPPORTED	1024

/* DMAR Flags */
#define DMAR_INTR_REMAP		0x1
#define DMAR_X2APIC_OPT_OUT	0x2
#define DMAR_PLATFORM_OPT_IN	0x4

struct intel_iommu;

struct dmar_dev_scope {
	struct device __rcu *dev;
	u8 bus;
	u8 devfn;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_DMAR_TABLE
extern struct acpi_table_header *dmar_tbl;
struct dmar_drhd_unit {
	struct list_head list;		/* list of drhd units	*/
	struct  acpi_dmar_header *hdr;	/* ACPI header		*/
	u64	reg_base_addr;		/* register base address*/
<<<<<<< HEAD
	struct	pci_dev **devices; 	/* target device array	*/
=======
	unsigned long reg_size;		/* size of register set */
	struct	dmar_dev_scope *devices;/* target device array	*/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	devices_cnt;		/* target device count	*/
	u16	segment;		/* PCI domain		*/
	u8	ignored:1; 		/* ignore drhd		*/
	u8	include_all:1;
<<<<<<< HEAD
	struct intel_iommu *iommu;
};

extern struct list_head dmar_drhd_units;

#define for_each_drhd_unit(drhd) \
	list_for_each_entry(drhd, &dmar_drhd_units, list)

#define for_each_active_iommu(i, drhd)					\
	list_for_each_entry(drhd, &dmar_drhd_units, list)		\
		if (i=drhd->iommu, drhd->ignored) {} else

#define for_each_iommu(i, drhd)						\
	list_for_each_entry(drhd, &dmar_drhd_units, list)		\
		if (i=drhd->iommu, 0) {} else 

extern int dmar_table_init(void);
extern int dmar_dev_scope_init(void);

/* Intel IOMMU detection */
extern int detect_intel_iommu(void);
extern int enable_drhd_fault_handling(void);

extern int parse_ioapics_under_ir(void);
extern int alloc_iommu(struct dmar_drhd_unit *);
#else
static inline int detect_intel_iommu(void)
{
	return -ENODEV;
}

static inline int dmar_table_init(void)
{
	return -ENODEV;
}
static inline int enable_drhd_fault_handling(void)
{
	return -1;
}
#endif /* !CONFIG_DMAR_TABLE */
=======
	u8	gfx_dedicated:1;	/* graphic dedicated	*/
	struct intel_iommu *iommu;
};

struct dmar_pci_path {
	u8 bus;
	u8 device;
	u8 function;
};

struct dmar_pci_notify_info {
	struct pci_dev			*dev;
	unsigned long			event;
	int				bus;
	u16				seg;
	u16				level;
	struct dmar_pci_path		path[];
}  __attribute__((packed));

extern struct rw_semaphore dmar_global_lock;
extern struct list_head dmar_drhd_units;

#define for_each_drhd_unit(drhd)					\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list,		\
				dmar_rcu_check())

#define for_each_active_drhd_unit(drhd)					\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list,		\
				dmar_rcu_check())			\
		if (drhd->ignored) {} else

#define for_each_active_iommu(i, drhd)					\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list,		\
				dmar_rcu_check())			\
		if (i=drhd->iommu, drhd->ignored) {} else

#define for_each_iommu(i, drhd)						\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list,		\
				dmar_rcu_check())			\
		if (i=drhd->iommu, 0) {} else 

static inline bool dmar_rcu_check(void)
{
	return rwsem_is_locked(&dmar_global_lock) ||
	       system_state == SYSTEM_BOOTING;
}

#define	dmar_rcu_dereference(p)	rcu_dereference_check((p), dmar_rcu_check())

#define for_each_dev_scope(devs, cnt, i, tmp)				\
	for ((i) = 0; ((tmp) = (i) < (cnt) ?				\
	    dmar_rcu_dereference((devs)[(i)].dev) : NULL, (i) < (cnt)); \
	    (i)++)

#define for_each_active_dev_scope(devs, cnt, i, tmp)			\
	for_each_dev_scope((devs), (cnt), (i), (tmp))			\
		if (!(tmp)) { continue; } else

extern int dmar_table_init(void);
extern int dmar_dev_scope_init(void);
extern void dmar_register_bus_notifier(void);
extern void *dmar_alloc_dev_scope(void *start, void *end, int *cnt);
extern void dmar_free_dev_scope(struct dmar_dev_scope **devices, int *cnt);
extern int dmar_insert_dev_scope(struct dmar_pci_notify_info *info,
				 void *start, void*end, u16 segment,
				 struct dmar_dev_scope *devices,
				 int devices_cnt);
extern int dmar_remove_dev_scope(struct dmar_pci_notify_info *info,
				 u16 segment, struct dmar_dev_scope *devices,
				 int count);
/* Intel IOMMU detection */
void detect_intel_iommu(void);
extern int enable_drhd_fault_handling(void);
extern int dmar_device_add(acpi_handle handle);
extern int dmar_device_remove(acpi_handle handle);

static inline int dmar_res_noop(struct acpi_dmar_header *hdr, void *arg)
{
	return 0;
}

#ifdef CONFIG_DMAR_DEBUG
void dmar_fault_dump_ptes(struct intel_iommu *iommu, u16 source_id,
			  unsigned long long addr, u32 pasid);
#else
static inline void dmar_fault_dump_ptes(struct intel_iommu *iommu, u16 source_id,
					unsigned long long addr, u32 pasid) {}
#endif

#ifdef CONFIG_INTEL_IOMMU
extern int iommu_detected, no_iommu;
extern int intel_iommu_init(void);
extern void intel_iommu_shutdown(void);
extern int dmar_parse_one_rmrr(struct acpi_dmar_header *header, void *arg);
extern int dmar_parse_one_atsr(struct acpi_dmar_header *header, void *arg);
extern int dmar_check_one_atsr(struct acpi_dmar_header *hdr, void *arg);
extern int dmar_parse_one_satc(struct acpi_dmar_header *hdr, void *arg);
extern int dmar_release_one_atsr(struct acpi_dmar_header *hdr, void *arg);
extern int dmar_iommu_hotplug(struct dmar_drhd_unit *dmaru, bool insert);
extern int dmar_iommu_notify_scope_dev(struct dmar_pci_notify_info *info);
#else /* !CONFIG_INTEL_IOMMU: */
static inline int intel_iommu_init(void) { return -ENODEV; }
static inline void intel_iommu_shutdown(void) { }

#define	dmar_parse_one_rmrr		dmar_res_noop
#define	dmar_parse_one_atsr		dmar_res_noop
#define	dmar_check_one_atsr		dmar_res_noop
#define	dmar_release_one_atsr		dmar_res_noop
#define	dmar_parse_one_satc		dmar_res_noop

static inline int dmar_iommu_notify_scope_dev(struct dmar_pci_notify_info *info)
{
	return 0;
}

static inline int dmar_iommu_hotplug(struct dmar_drhd_unit *dmaru, bool insert)
{
	return 0;
}
#endif /* CONFIG_INTEL_IOMMU */

#ifdef CONFIG_IRQ_REMAP
extern int dmar_ir_hotplug(struct dmar_drhd_unit *dmaru, bool insert);
#else  /* CONFIG_IRQ_REMAP */
static inline int dmar_ir_hotplug(struct dmar_drhd_unit *dmaru, bool insert)
{ return 0; }
#endif /* CONFIG_IRQ_REMAP */

extern bool dmar_platform_optin(void);

#else /* CONFIG_DMAR_TABLE */

static inline int dmar_device_add(void *handle)
{
	return 0;
}

static inline int dmar_device_remove(void *handle)
{
	return 0;
}

static inline bool dmar_platform_optin(void)
{
	return false;
}

static inline void detect_intel_iommu(void)
{
}

#endif /* CONFIG_DMAR_TABLE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct irte {
	union {
		struct {
<<<<<<< HEAD
			__u64	present 	: 1,
				fpd		: 1,
				dst_mode	: 1,
				redir_hint	: 1,
				trigger_mode	: 1,
				dlvry_mode	: 3,
				avail		: 4,
				__reserved_1	: 4,
				vector		: 8,
				__reserved_2	: 8,
				dest_id		: 32;
		};
		__u64 low;
	};

	union {
		struct {
			__u64	sid		: 16,
				sq		: 2,
				svt		: 2,
				__reserved_3	: 44;
		};
		__u64 high;
	};
};

#ifdef CONFIG_IRQ_REMAP
extern int intr_remapping_enabled;
extern int intr_remapping_supported(void);
extern int enable_intr_remapping(void);
extern void disable_intr_remapping(void);
extern int reenable_intr_remapping(int);

extern int get_irte(int irq, struct irte *entry);
extern int modify_irte(int irq, struct irte *irte_modified);
extern int alloc_irte(struct intel_iommu *iommu, int irq, u16 count);
extern int set_irte_irq(int irq, struct intel_iommu *iommu, u16 index,
   			u16 sub_handle);
extern int map_irq_to_irte_handle(int irq, u16 *sub_handle);
extern int free_irte(int irq);

extern struct intel_iommu *map_dev_to_ir(struct pci_dev *dev);
extern struct intel_iommu *map_ioapic_to_ir(int apic);
extern struct intel_iommu *map_hpet_to_ir(u8 id);
extern int set_ioapic_sid(struct irte *irte, int apic);
extern int set_hpet_sid(struct irte *irte, u8 id);
extern int set_msi_sid(struct irte *irte, struct pci_dev *dev);
#else
static inline int alloc_irte(struct intel_iommu *iommu, int irq, u16 count)
{
	return -1;
}
static inline int modify_irte(int irq, struct irte *irte_modified)
{
	return -1;
}
static inline int free_irte(int irq)
{
	return -1;
}
static inline int map_irq_to_irte_handle(int irq, u16 *sub_handle)
{
	return -1;
}
static inline int set_irte_irq(int irq, struct intel_iommu *iommu, u16 index,
			       u16 sub_handle)
{
	return -1;
}
static inline struct intel_iommu *map_dev_to_ir(struct pci_dev *dev)
{
	return NULL;
}
static inline struct intel_iommu *map_ioapic_to_ir(int apic)
{
	return NULL;
}
static inline struct intel_iommu *map_hpet_to_ir(unsigned int hpet_id)
{
	return NULL;
}
static inline int set_ioapic_sid(struct irte *irte, int apic)
{
	return 0;
}
static inline int set_hpet_sid(struct irte *irte, u8 id)
{
	return -1;
}
static inline int set_msi_sid(struct irte *irte, struct pci_dev *dev)
{
	return 0;
}

#define intr_remapping_enabled		(0)

static inline int enable_intr_remapping(void)
{
	return -1;
}

static inline void disable_intr_remapping(void)
{
}

static inline int reenable_intr_remapping(int eim)
{
	return 0;
}
#endif

enum {
	IRQ_REMAP_XAPIC_MODE,
	IRQ_REMAP_X2APIC_MODE,
};

=======
			union {
				/* Shared between remapped and posted mode*/
				struct {
					__u64	present		: 1,  /*  0      */
						fpd		: 1,  /*  1      */
						__res0		: 6,  /*  2 -  6 */
						avail		: 4,  /*  8 - 11 */
						__res1		: 3,  /* 12 - 14 */
						pst		: 1,  /* 15      */
						vector		: 8,  /* 16 - 23 */
						__res2		: 40; /* 24 - 63 */
				};

				/* Remapped mode */
				struct {
					__u64	r_present	: 1,  /*  0      */
						r_fpd		: 1,  /*  1      */
						dst_mode	: 1,  /*  2      */
						redir_hint	: 1,  /*  3      */
						trigger_mode	: 1,  /*  4      */
						dlvry_mode	: 3,  /*  5 -  7 */
						r_avail		: 4,  /*  8 - 11 */
						r_res0		: 4,  /* 12 - 15 */
						r_vector	: 8,  /* 16 - 23 */
						r_res1		: 8,  /* 24 - 31 */
						dest_id		: 32; /* 32 - 63 */
				};

				/* Posted mode */
				struct {
					__u64	p_present	: 1,  /*  0      */
						p_fpd		: 1,  /*  1      */
						p_res0		: 6,  /*  2 -  7 */
						p_avail		: 4,  /*  8 - 11 */
						p_res1		: 2,  /* 12 - 13 */
						p_urgent	: 1,  /* 14      */
						p_pst		: 1,  /* 15      */
						p_vector	: 8,  /* 16 - 23 */
						p_res2		: 14, /* 24 - 37 */
						pda_l		: 26; /* 38 - 63 */
				};
				__u64 low;
			};

			union {
				/* Shared between remapped and posted mode*/
				struct {
					__u64	sid		: 16,  /* 64 - 79  */
						sq		: 2,   /* 80 - 81  */
						svt		: 2,   /* 82 - 83  */
						__res3		: 44;  /* 84 - 127 */
				};

				/* Posted mode*/
				struct {
					__u64	p_sid		: 16,  /* 64 - 79  */
						p_sq		: 2,   /* 80 - 81  */
						p_svt		: 2,   /* 82 - 83  */
						p_res3		: 12,  /* 84 - 95  */
						pda_h		: 32;  /* 96 - 127 */
				};
				__u64 high;
			};
		};
#ifdef CONFIG_IRQ_REMAP
		__u128 irte;
#endif
	};
};

static inline void dmar_copy_shared_irte(struct irte *dst, struct irte *src)
{
	dst->present	= src->present;
	dst->fpd	= src->fpd;
	dst->avail	= src->avail;
	dst->pst	= src->pst;
	dst->vector	= src->vector;
	dst->sid	= src->sid;
	dst->sq		= src->sq;
	dst->svt	= src->svt;
}

#define PDA_LOW_BIT    26
#define PDA_HIGH_BIT   32

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Can't use the common MSI interrupt functions
 * since DMAR is not a pci device
 */
struct irq_data;
extern void dmar_msi_unmask(struct irq_data *data);
extern void dmar_msi_mask(struct irq_data *data);
extern void dmar_msi_read(int irq, struct msi_msg *msg);
extern void dmar_msi_write(int irq, struct msi_msg *msg);
extern int dmar_set_interrupt(struct intel_iommu *iommu);
extern irqreturn_t dmar_fault(int irq, void *dev_id);
<<<<<<< HEAD
extern int arch_setup_dmar_msi(unsigned int irq);

#ifdef CONFIG_INTEL_IOMMU
extern int iommu_detected, no_iommu;
extern struct list_head dmar_rmrr_units;
struct dmar_rmrr_unit {
	struct list_head list;		/* list of rmrr units	*/
	struct acpi_dmar_header *hdr;	/* ACPI header		*/
	u64	base_address;		/* reserved base address*/
	u64	end_address;		/* reserved end address */
	struct pci_dev **devices;	/* target devices */
	int	devices_cnt;		/* target device count */
};

#define for_each_rmrr_units(rmrr) \
	list_for_each_entry(rmrr, &dmar_rmrr_units, list)

struct dmar_atsr_unit {
	struct list_head list;		/* list of ATSR units */
	struct acpi_dmar_header *hdr;	/* ACPI header */
	struct pci_dev **devices;	/* target devices */
	int devices_cnt;		/* target device count */
	u8 include_all:1;		/* include all ports */
};

int dmar_parse_rmrr_atsr_dev(void);
extern int dmar_parse_one_rmrr(struct acpi_dmar_header *header);
extern int dmar_parse_one_atsr(struct acpi_dmar_header *header);
extern int dmar_parse_dev_scope(void *start, void *end, int *cnt,
				struct pci_dev ***devices, u16 segment);
extern int intel_iommu_init(void);
#else /* !CONFIG_INTEL_IOMMU: */
static inline int intel_iommu_init(void) { return -ENODEV; }
static inline int dmar_parse_one_rmrr(struct acpi_dmar_header *header)
{
	return 0;
}
static inline int dmar_parse_one_atsr(struct acpi_dmar_header *header)
{
	return 0;
}
static inline int dmar_parse_rmrr_atsr_dev(void)
{
	return 0;
}
#endif /* CONFIG_INTEL_IOMMU */
=======
extern int dmar_alloc_hwirq(int id, int node, void *arg);
extern void dmar_free_hwirq(int irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __DMAR_H__ */
