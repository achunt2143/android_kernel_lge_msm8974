<<<<<<< HEAD
/*
 * Support PCI/PCIe on PowerNV platforms
 *
 * Currently supports only P5IOC2
 *
 * Copyright 2011 Benjamin Herrenschmidt, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Support PCI/PCIe on PowerNV platforms
 *
 * Copyright 2011 Benjamin Herrenschmidt, IBM Corp.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/msi.h>

#include <asm/sections.h>
#include <asm/io.h>
#include <asm/prom.h>
#include <asm/pci-bridge.h>
#include <asm/machdep.h>
#include <asm/ppc-pci.h>
#include <asm/opal.h>
#include <asm/iommu.h>
#include <asm/tce.h>
#include <asm/abs_addr.h>
#include <asm/firmware.h>
=======
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/msi.h>
#include <linux/iommu.h>
#include <linux/sched/mm.h>

#include <asm/sections.h>
#include <asm/io.h>
#include <asm/pci-bridge.h>
#include <asm/machdep.h>
#include <asm/msi_bitmap.h>
#include <asm/ppc-pci.h>
#include <asm/pnv-pci.h>
#include <asm/opal.h>
#include <asm/iommu.h>
#include <asm/tce.h>
#include <asm/firmware.h>
#include <asm/eeh_event.h>
#include <asm/eeh.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "powernv.h"
#include "pci.h"

<<<<<<< HEAD
/* Delay in usec */
#define PCI_RESET_DELAY_US	3000000

#define cfg_dbg(fmt...)	do { } while(0)
//#define cfg_dbg(fmt...)	printk(fmt)

#ifdef CONFIG_PCI_MSI
static int pnv_msi_check_device(struct pci_dev* pdev, int nvec, int type)
{
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct pnv_phb *phb = hose->private_data;

	return (phb && phb->msi_map) ? 0 : -ENODEV;
}

static unsigned int pnv_get_one_msi(struct pnv_phb *phb)
{
	unsigned long flags;
	unsigned int id, rc;

	spin_lock_irqsave(&phb->lock, flags);

	id = find_next_zero_bit(phb->msi_map, phb->msi_count, phb->msi_next);
	if (id >= phb->msi_count && phb->msi_next)
		id = find_next_zero_bit(phb->msi_map, phb->msi_count, 0);
	if (id >= phb->msi_count) {
		rc = 0;
		goto out;
	}
	__set_bit(id, phb->msi_map);
	rc = id + phb->msi_base;
out:
	spin_unlock_irqrestore(&phb->lock, flags);
	return rc;
}

static void pnv_put_msi(struct pnv_phb *phb, unsigned int hwirq)
{
	unsigned long flags;
	unsigned int id;

	if (WARN_ON(hwirq < phb->msi_base ||
		    hwirq >= (phb->msi_base + phb->msi_count)))
		return;
	id = hwirq - phb->msi_base;

	spin_lock_irqsave(&phb->lock, flags);
	__clear_bit(id, phb->msi_map);
	spin_unlock_irqrestore(&phb->lock, flags);
}

static int pnv_setup_msi_irqs(struct pci_dev *pdev, int nvec, int type)
{
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct pnv_phb *phb = hose->private_data;
	struct msi_desc *entry;
	struct msi_msg msg;
	unsigned int hwirq, virq;
	int rc;

	if (WARN_ON(!phb))
		return -ENODEV;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		if (!entry->msi_attrib.is_64 && !phb->msi32_support) {
			pr_warn("%s: Supports only 64-bit MSIs\n",
				pci_name(pdev));
			return -ENXIO;
		}
		hwirq = pnv_get_one_msi(phb);
		if (!hwirq) {
			pr_warn("%s: Failed to find a free MSI\n",
				pci_name(pdev));
			return -ENOSPC;
		}
		virq = irq_create_mapping(NULL, hwirq);
		if (virq == NO_IRQ) {
			pr_warn("%s: Failed to map MSI to linux irq\n",
				pci_name(pdev));
			pnv_put_msi(phb, hwirq);
			return -ENOMEM;
		}
		rc = phb->msi_setup(phb, pdev, hwirq, entry->msi_attrib.is_64,
				    &msg);
		if (rc) {
			pr_warn("%s: Failed to setup MSI\n", pci_name(pdev));
			irq_dispose_mapping(virq);
			pnv_put_msi(phb, hwirq);
			return rc;
		}
		irq_set_msi_desc(virq, entry);
		write_msi_msg(virq, &msg);
	}
	return 0;
}

static void pnv_teardown_msi_irqs(struct pci_dev *pdev)
{
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct pnv_phb *phb = hose->private_data;
	struct msi_desc *entry;
	irq_hw_number_t hwirq;

	if (WARN_ON(!phb))
		return;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		if (entry->irq == NO_IRQ)
			continue;
		hwirq = virq_to_hw(entry->irq);
		irq_set_msi_desc(entry->irq, NULL);
		irq_dispose_mapping(entry->irq);
		pnv_put_msi(phb, hwirq);
	}
}
#endif /* CONFIG_PCI_MSI */

static void pnv_pci_dump_p7ioc_diag_data(struct pnv_phb *phb)
{
	struct OpalIoP7IOCPhbErrorData *data = &phb->diag.p7ioc;
	int i;

	pr_info("PHB %d diagnostic data:\n", phb->hose->global_number);

	pr_info("  brdgCtl              = 0x%08x\n", data->brdgCtl);

	pr_info("  portStatusReg        = 0x%08x\n", data->portStatusReg);
	pr_info("  rootCmplxStatus      = 0x%08x\n", data->rootCmplxStatus);
	pr_info("  busAgentStatus       = 0x%08x\n", data->busAgentStatus);

	pr_info("  deviceStatus         = 0x%08x\n", data->deviceStatus);
	pr_info("  slotStatus           = 0x%08x\n", data->slotStatus);
	pr_info("  linkStatus           = 0x%08x\n", data->linkStatus);
	pr_info("  devCmdStatus         = 0x%08x\n", data->devCmdStatus);
	pr_info("  devSecStatus         = 0x%08x\n", data->devSecStatus);

	pr_info("  rootErrorStatus      = 0x%08x\n", data->rootErrorStatus);
	pr_info("  uncorrErrorStatus    = 0x%08x\n", data->uncorrErrorStatus);
	pr_info("  corrErrorStatus      = 0x%08x\n", data->corrErrorStatus);
	pr_info("  tlpHdr1              = 0x%08x\n", data->tlpHdr1);
	pr_info("  tlpHdr2              = 0x%08x\n", data->tlpHdr2);
	pr_info("  tlpHdr3              = 0x%08x\n", data->tlpHdr3);
	pr_info("  tlpHdr4              = 0x%08x\n", data->tlpHdr4);
	pr_info("  sourceId             = 0x%08x\n", data->sourceId);

	pr_info("  errorClass           = 0x%016llx\n", data->errorClass);
	pr_info("  correlator           = 0x%016llx\n", data->correlator);

	pr_info("  p7iocPlssr           = 0x%016llx\n", data->p7iocPlssr);
	pr_info("  p7iocCsr             = 0x%016llx\n", data->p7iocCsr);
	pr_info("  lemFir               = 0x%016llx\n", data->lemFir);
	pr_info("  lemErrorMask         = 0x%016llx\n", data->lemErrorMask);
	pr_info("  lemWOF               = 0x%016llx\n", data->lemWOF);
	pr_info("  phbErrorStatus       = 0x%016llx\n", data->phbErrorStatus);
	pr_info("  phbFirstErrorStatus  = 0x%016llx\n", data->phbFirstErrorStatus);
	pr_info("  phbErrorLog0         = 0x%016llx\n", data->phbErrorLog0);
	pr_info("  phbErrorLog1         = 0x%016llx\n", data->phbErrorLog1);
	pr_info("  mmioErrorStatus      = 0x%016llx\n", data->mmioErrorStatus);
	pr_info("  mmioFirstErrorStatus = 0x%016llx\n", data->mmioFirstErrorStatus);
	pr_info("  mmioErrorLog0        = 0x%016llx\n", data->mmioErrorLog0);
	pr_info("  mmioErrorLog1        = 0x%016llx\n", data->mmioErrorLog1);
	pr_info("  dma0ErrorStatus      = 0x%016llx\n", data->dma0ErrorStatus);
	pr_info("  dma0FirstErrorStatus = 0x%016llx\n", data->dma0FirstErrorStatus);
	pr_info("  dma0ErrorLog0        = 0x%016llx\n", data->dma0ErrorLog0);
	pr_info("  dma0ErrorLog1        = 0x%016llx\n", data->dma0ErrorLog1);
	pr_info("  dma1ErrorStatus      = 0x%016llx\n", data->dma1ErrorStatus);
	pr_info("  dma1FirstErrorStatus = 0x%016llx\n", data->dma1FirstErrorStatus);
	pr_info("  dma1ErrorLog0        = 0x%016llx\n", data->dma1ErrorLog0);
	pr_info("  dma1ErrorLog1        = 0x%016llx\n", data->dma1ErrorLog1);

	for (i = 0; i < OPAL_P7IOC_NUM_PEST_REGS; i++) {
		if ((data->pestA[i] >> 63) == 0 &&
		    (data->pestB[i] >> 63) == 0)
			continue;
		pr_info("  PE[%3d] PESTA        = 0x%016llx\n", i, data->pestA[i]);
		pr_info("          PESTB        = 0x%016llx\n", data->pestB[i]);
	}
}

static void pnv_pci_dump_phb_diag_data(struct pnv_phb *phb)
{
	switch(phb->model) {
	case PNV_PHB_MODEL_P7IOC:
		pnv_pci_dump_p7ioc_diag_data(phb);
		break;
	default:
		pr_warning("PCI %d: Can't decode this PHB diag data\n",
			   phb->hose->global_number);
=======
static DEFINE_MUTEX(tunnel_mutex);

int pnv_pci_get_slot_id(struct device_node *np, uint64_t *id)
{
	struct device_node *node = np;
	u32 bdfn;
	u64 phbid;
	int ret;

	ret = of_property_read_u32(np, "reg", &bdfn);
	if (ret)
		return -ENXIO;

	bdfn = ((bdfn & 0x00ffff00) >> 8);
	for (node = np; node; node = of_get_parent(node)) {
		if (!PCI_DN(node)) {
			of_node_put(node);
			break;
		}

		if (!of_device_is_compatible(node, "ibm,ioda2-phb") &&
		    !of_device_is_compatible(node, "ibm,ioda3-phb") &&
		    !of_device_is_compatible(node, "ibm,ioda2-npu2-opencapi-phb")) {
			of_node_put(node);
			continue;
		}

		ret = of_property_read_u64(node, "ibm,opal-phbid", &phbid);
		if (ret) {
			of_node_put(node);
			return -ENXIO;
		}

		if (of_device_is_compatible(node, "ibm,ioda2-npu2-opencapi-phb"))
			*id = PCI_PHB_SLOT_ID(phbid);
		else
			*id = PCI_SLOT_ID(phbid, bdfn);
		return 0;
	}

	return -ENODEV;
}
EXPORT_SYMBOL_GPL(pnv_pci_get_slot_id);

int pnv_pci_get_device_tree(uint32_t phandle, void *buf, uint64_t len)
{
	int64_t rc;

	if (!opal_check_token(OPAL_GET_DEVICE_TREE))
		return -ENXIO;

	rc = opal_get_device_tree(phandle, (uint64_t)buf, len);
	if (rc < OPAL_SUCCESS)
		return -EIO;

	return rc;
}
EXPORT_SYMBOL_GPL(pnv_pci_get_device_tree);

int pnv_pci_get_presence_state(uint64_t id, uint8_t *state)
{
	int64_t rc;

	if (!opal_check_token(OPAL_PCI_GET_PRESENCE_STATE))
		return -ENXIO;

	rc = opal_pci_get_presence_state(id, (uint64_t)state);
	if (rc != OPAL_SUCCESS)
		return -EIO;

	return 0;
}
EXPORT_SYMBOL_GPL(pnv_pci_get_presence_state);

int pnv_pci_get_power_state(uint64_t id, uint8_t *state)
{
	int64_t rc;

	if (!opal_check_token(OPAL_PCI_GET_POWER_STATE))
		return -ENXIO;

	rc = opal_pci_get_power_state(id, (uint64_t)state);
	if (rc != OPAL_SUCCESS)
		return -EIO;

	return 0;
}
EXPORT_SYMBOL_GPL(pnv_pci_get_power_state);

int pnv_pci_set_power_state(uint64_t id, uint8_t state, struct opal_msg *msg)
{
	struct opal_msg m;
	int token, ret;
	int64_t rc;

	if (!opal_check_token(OPAL_PCI_SET_POWER_STATE))
		return -ENXIO;

	token = opal_async_get_token_interruptible();
	if (unlikely(token < 0))
		return token;

	rc = opal_pci_set_power_state(token, id, (uint64_t)&state);
	if (rc == OPAL_SUCCESS) {
		ret = 0;
		goto exit;
	} else if (rc != OPAL_ASYNC_COMPLETION) {
		ret = -EIO;
		goto exit;
	}

	ret = opal_async_wait_response(token, &m);
	if (ret < 0)
		goto exit;

	if (msg) {
		ret = 1;
		memcpy(msg, &m, sizeof(m));
	}

exit:
	opal_async_release_token(token);
	return ret;
}
EXPORT_SYMBOL_GPL(pnv_pci_set_power_state);

/* Nicely print the contents of the PE State Tables (PEST). */
static void pnv_pci_dump_pest(__be64 pestA[], __be64 pestB[], int pest_size)
{
	__be64 prevA = ULONG_MAX, prevB = ULONG_MAX;
	bool dup = false;
	int i;

	for (i = 0; i < pest_size; i++) {
		__be64 peA = be64_to_cpu(pestA[i]);
		__be64 peB = be64_to_cpu(pestB[i]);

		if (peA != prevA || peB != prevB) {
			if (dup) {
				pr_info("PE[..%03x] A/B: as above\n", i-1);
				dup = false;
			}
			prevA = peA;
			prevB = peB;
			if (peA & PNV_IODA_STOPPED_STATE ||
			    peB & PNV_IODA_STOPPED_STATE)
				pr_info("PE[%03x] A/B: %016llx %016llx\n",
					i, peA, peB);
		} else if (!dup && (peA & PNV_IODA_STOPPED_STATE ||
				    peB & PNV_IODA_STOPPED_STATE)) {
			dup = true;
		}
	}
}

static void pnv_pci_dump_p7ioc_diag_data(struct pci_controller *hose,
					 struct OpalIoPhbErrorCommon *common)
{
	struct OpalIoP7IOCPhbErrorData *data;

	data = (struct OpalIoP7IOCPhbErrorData *)common;
	pr_info("P7IOC PHB#%x Diag-data (Version: %d)\n",
		hose->global_number, be32_to_cpu(common->version));

	if (data->brdgCtl)
		pr_info("brdgCtl:     %08x\n",
			be32_to_cpu(data->brdgCtl));
	if (data->portStatusReg || data->rootCmplxStatus ||
	    data->busAgentStatus)
		pr_info("UtlSts:      %08x %08x %08x\n",
			be32_to_cpu(data->portStatusReg),
			be32_to_cpu(data->rootCmplxStatus),
			be32_to_cpu(data->busAgentStatus));
	if (data->deviceStatus || data->slotStatus   ||
	    data->linkStatus   || data->devCmdStatus ||
	    data->devSecStatus)
		pr_info("RootSts:     %08x %08x %08x %08x %08x\n",
			be32_to_cpu(data->deviceStatus),
			be32_to_cpu(data->slotStatus),
			be32_to_cpu(data->linkStatus),
			be32_to_cpu(data->devCmdStatus),
			be32_to_cpu(data->devSecStatus));
	if (data->rootErrorStatus   || data->uncorrErrorStatus ||
	    data->corrErrorStatus)
		pr_info("RootErrSts:  %08x %08x %08x\n",
			be32_to_cpu(data->rootErrorStatus),
			be32_to_cpu(data->uncorrErrorStatus),
			be32_to_cpu(data->corrErrorStatus));
	if (data->tlpHdr1 || data->tlpHdr2 ||
	    data->tlpHdr3 || data->tlpHdr4)
		pr_info("RootErrLog:  %08x %08x %08x %08x\n",
			be32_to_cpu(data->tlpHdr1),
			be32_to_cpu(data->tlpHdr2),
			be32_to_cpu(data->tlpHdr3),
			be32_to_cpu(data->tlpHdr4));
	if (data->sourceId || data->errorClass ||
	    data->correlator)
		pr_info("RootErrLog1: %08x %016llx %016llx\n",
			be32_to_cpu(data->sourceId),
			be64_to_cpu(data->errorClass),
			be64_to_cpu(data->correlator));
	if (data->p7iocPlssr || data->p7iocCsr)
		pr_info("PhbSts:      %016llx %016llx\n",
			be64_to_cpu(data->p7iocPlssr),
			be64_to_cpu(data->p7iocCsr));
	if (data->lemFir)
		pr_info("Lem:         %016llx %016llx %016llx\n",
			be64_to_cpu(data->lemFir),
			be64_to_cpu(data->lemErrorMask),
			be64_to_cpu(data->lemWOF));
	if (data->phbErrorStatus)
		pr_info("PhbErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbErrorStatus),
			be64_to_cpu(data->phbFirstErrorStatus),
			be64_to_cpu(data->phbErrorLog0),
			be64_to_cpu(data->phbErrorLog1));
	if (data->mmioErrorStatus)
		pr_info("OutErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->mmioErrorStatus),
			be64_to_cpu(data->mmioFirstErrorStatus),
			be64_to_cpu(data->mmioErrorLog0),
			be64_to_cpu(data->mmioErrorLog1));
	if (data->dma0ErrorStatus)
		pr_info("InAErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->dma0ErrorStatus),
			be64_to_cpu(data->dma0FirstErrorStatus),
			be64_to_cpu(data->dma0ErrorLog0),
			be64_to_cpu(data->dma0ErrorLog1));
	if (data->dma1ErrorStatus)
		pr_info("InBErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->dma1ErrorStatus),
			be64_to_cpu(data->dma1FirstErrorStatus),
			be64_to_cpu(data->dma1ErrorLog0),
			be64_to_cpu(data->dma1ErrorLog1));

	pnv_pci_dump_pest(data->pestA, data->pestB, OPAL_P7IOC_NUM_PEST_REGS);
}

static void pnv_pci_dump_phb3_diag_data(struct pci_controller *hose,
					struct OpalIoPhbErrorCommon *common)
{
	struct OpalIoPhb3ErrorData *data;

	data = (struct OpalIoPhb3ErrorData*)common;
	pr_info("PHB3 PHB#%x Diag-data (Version: %d)\n",
		hose->global_number, be32_to_cpu(common->version));
	if (data->brdgCtl)
		pr_info("brdgCtl:     %08x\n",
			be32_to_cpu(data->brdgCtl));
	if (data->portStatusReg || data->rootCmplxStatus ||
	    data->busAgentStatus)
		pr_info("UtlSts:      %08x %08x %08x\n",
			be32_to_cpu(data->portStatusReg),
			be32_to_cpu(data->rootCmplxStatus),
			be32_to_cpu(data->busAgentStatus));
	if (data->deviceStatus || data->slotStatus   ||
	    data->linkStatus   || data->devCmdStatus ||
	    data->devSecStatus)
		pr_info("RootSts:     %08x %08x %08x %08x %08x\n",
			be32_to_cpu(data->deviceStatus),
			be32_to_cpu(data->slotStatus),
			be32_to_cpu(data->linkStatus),
			be32_to_cpu(data->devCmdStatus),
			be32_to_cpu(data->devSecStatus));
	if (data->rootErrorStatus || data->uncorrErrorStatus ||
	    data->corrErrorStatus)
		pr_info("RootErrSts:  %08x %08x %08x\n",
			be32_to_cpu(data->rootErrorStatus),
			be32_to_cpu(data->uncorrErrorStatus),
			be32_to_cpu(data->corrErrorStatus));
	if (data->tlpHdr1 || data->tlpHdr2 ||
	    data->tlpHdr3 || data->tlpHdr4)
		pr_info("RootErrLog:  %08x %08x %08x %08x\n",
			be32_to_cpu(data->tlpHdr1),
			be32_to_cpu(data->tlpHdr2),
			be32_to_cpu(data->tlpHdr3),
			be32_to_cpu(data->tlpHdr4));
	if (data->sourceId || data->errorClass ||
	    data->correlator)
		pr_info("RootErrLog1: %08x %016llx %016llx\n",
			be32_to_cpu(data->sourceId),
			be64_to_cpu(data->errorClass),
			be64_to_cpu(data->correlator));
	if (data->nFir)
		pr_info("nFir:        %016llx %016llx %016llx\n",
			be64_to_cpu(data->nFir),
			be64_to_cpu(data->nFirMask),
			be64_to_cpu(data->nFirWOF));
	if (data->phbPlssr || data->phbCsr)
		pr_info("PhbSts:      %016llx %016llx\n",
			be64_to_cpu(data->phbPlssr),
			be64_to_cpu(data->phbCsr));
	if (data->lemFir)
		pr_info("Lem:         %016llx %016llx %016llx\n",
			be64_to_cpu(data->lemFir),
			be64_to_cpu(data->lemErrorMask),
			be64_to_cpu(data->lemWOF));
	if (data->phbErrorStatus)
		pr_info("PhbErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbErrorStatus),
			be64_to_cpu(data->phbFirstErrorStatus),
			be64_to_cpu(data->phbErrorLog0),
			be64_to_cpu(data->phbErrorLog1));
	if (data->mmioErrorStatus)
		pr_info("OutErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->mmioErrorStatus),
			be64_to_cpu(data->mmioFirstErrorStatus),
			be64_to_cpu(data->mmioErrorLog0),
			be64_to_cpu(data->mmioErrorLog1));
	if (data->dma0ErrorStatus)
		pr_info("InAErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->dma0ErrorStatus),
			be64_to_cpu(data->dma0FirstErrorStatus),
			be64_to_cpu(data->dma0ErrorLog0),
			be64_to_cpu(data->dma0ErrorLog1));
	if (data->dma1ErrorStatus)
		pr_info("InBErr:      %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->dma1ErrorStatus),
			be64_to_cpu(data->dma1FirstErrorStatus),
			be64_to_cpu(data->dma1ErrorLog0),
			be64_to_cpu(data->dma1ErrorLog1));

	pnv_pci_dump_pest(data->pestA, data->pestB, OPAL_PHB3_NUM_PEST_REGS);
}

static void pnv_pci_dump_phb4_diag_data(struct pci_controller *hose,
					struct OpalIoPhbErrorCommon *common)
{
	struct OpalIoPhb4ErrorData *data;

	data = (struct OpalIoPhb4ErrorData*)common;
	pr_info("PHB4 PHB#%d Diag-data (Version: %d)\n",
		hose->global_number, be32_to_cpu(common->version));
	if (data->brdgCtl)
		pr_info("brdgCtl:    %08x\n",
			be32_to_cpu(data->brdgCtl));
	if (data->deviceStatus || data->slotStatus   ||
	    data->linkStatus   || data->devCmdStatus ||
	    data->devSecStatus)
		pr_info("RootSts:    %08x %08x %08x %08x %08x\n",
			be32_to_cpu(data->deviceStatus),
			be32_to_cpu(data->slotStatus),
			be32_to_cpu(data->linkStatus),
			be32_to_cpu(data->devCmdStatus),
			be32_to_cpu(data->devSecStatus));
	if (data->rootErrorStatus || data->uncorrErrorStatus ||
	    data->corrErrorStatus)
		pr_info("RootErrSts: %08x %08x %08x\n",
			be32_to_cpu(data->rootErrorStatus),
			be32_to_cpu(data->uncorrErrorStatus),
			be32_to_cpu(data->corrErrorStatus));
	if (data->tlpHdr1 || data->tlpHdr2 ||
	    data->tlpHdr3 || data->tlpHdr4)
		pr_info("RootErrLog: %08x %08x %08x %08x\n",
			be32_to_cpu(data->tlpHdr1),
			be32_to_cpu(data->tlpHdr2),
			be32_to_cpu(data->tlpHdr3),
			be32_to_cpu(data->tlpHdr4));
	if (data->sourceId)
		pr_info("sourceId:   %08x\n", be32_to_cpu(data->sourceId));
	if (data->nFir)
		pr_info("nFir:       %016llx %016llx %016llx\n",
			be64_to_cpu(data->nFir),
			be64_to_cpu(data->nFirMask),
			be64_to_cpu(data->nFirWOF));
	if (data->phbPlssr || data->phbCsr)
		pr_info("PhbSts:     %016llx %016llx\n",
			be64_to_cpu(data->phbPlssr),
			be64_to_cpu(data->phbCsr));
	if (data->lemFir)
		pr_info("Lem:        %016llx %016llx %016llx\n",
			be64_to_cpu(data->lemFir),
			be64_to_cpu(data->lemErrorMask),
			be64_to_cpu(data->lemWOF));
	if (data->phbErrorStatus)
		pr_info("PhbErr:     %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbErrorStatus),
			be64_to_cpu(data->phbFirstErrorStatus),
			be64_to_cpu(data->phbErrorLog0),
			be64_to_cpu(data->phbErrorLog1));
	if (data->phbTxeErrorStatus)
		pr_info("PhbTxeErr:  %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbTxeErrorStatus),
			be64_to_cpu(data->phbTxeFirstErrorStatus),
			be64_to_cpu(data->phbTxeErrorLog0),
			be64_to_cpu(data->phbTxeErrorLog1));
	if (data->phbRxeArbErrorStatus)
		pr_info("RxeArbErr:  %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbRxeArbErrorStatus),
			be64_to_cpu(data->phbRxeArbFirstErrorStatus),
			be64_to_cpu(data->phbRxeArbErrorLog0),
			be64_to_cpu(data->phbRxeArbErrorLog1));
	if (data->phbRxeMrgErrorStatus)
		pr_info("RxeMrgErr:  %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbRxeMrgErrorStatus),
			be64_to_cpu(data->phbRxeMrgFirstErrorStatus),
			be64_to_cpu(data->phbRxeMrgErrorLog0),
			be64_to_cpu(data->phbRxeMrgErrorLog1));
	if (data->phbRxeTceErrorStatus)
		pr_info("RxeTceErr:  %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbRxeTceErrorStatus),
			be64_to_cpu(data->phbRxeTceFirstErrorStatus),
			be64_to_cpu(data->phbRxeTceErrorLog0),
			be64_to_cpu(data->phbRxeTceErrorLog1));

	if (data->phbPblErrorStatus)
		pr_info("PblErr:     %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbPblErrorStatus),
			be64_to_cpu(data->phbPblFirstErrorStatus),
			be64_to_cpu(data->phbPblErrorLog0),
			be64_to_cpu(data->phbPblErrorLog1));
	if (data->phbPcieDlpErrorStatus)
		pr_info("PcieDlp:    %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbPcieDlpErrorLog1),
			be64_to_cpu(data->phbPcieDlpErrorLog2),
			be64_to_cpu(data->phbPcieDlpErrorStatus));
	if (data->phbRegbErrorStatus)
		pr_info("RegbErr:    %016llx %016llx %016llx %016llx\n",
			be64_to_cpu(data->phbRegbErrorStatus),
			be64_to_cpu(data->phbRegbFirstErrorStatus),
			be64_to_cpu(data->phbRegbErrorLog0),
			be64_to_cpu(data->phbRegbErrorLog1));


	pnv_pci_dump_pest(data->pestA, data->pestB, OPAL_PHB4_NUM_PEST_REGS);
}

void pnv_pci_dump_phb_diag_data(struct pci_controller *hose,
				unsigned char *log_buff)
{
	struct OpalIoPhbErrorCommon *common;

	if (!hose || !log_buff)
		return;

	common = (struct OpalIoPhbErrorCommon *)log_buff;
	switch (be32_to_cpu(common->ioType)) {
	case OPAL_PHB_ERROR_DATA_TYPE_P7IOC:
		pnv_pci_dump_p7ioc_diag_data(hose, common);
		break;
	case OPAL_PHB_ERROR_DATA_TYPE_PHB3:
		pnv_pci_dump_phb3_diag_data(hose, common);
		break;
	case OPAL_PHB_ERROR_DATA_TYPE_PHB4:
		pnv_pci_dump_phb4_diag_data(hose, common);
		break;
	default:
		pr_warn("%s: Unrecognized ioType %d\n",
			__func__, be32_to_cpu(common->ioType));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void pnv_pci_handle_eeh_config(struct pnv_phb *phb, u32 pe_no)
{
	unsigned long flags, rc;
<<<<<<< HEAD
	int has_diag;

	spin_lock_irqsave(&phb->lock, flags);

	rc = opal_pci_get_phb_diag_data(phb->opal_id, phb->diag.blob, PNV_PCI_DIAG_BUF_SIZE);
	has_diag = (rc == OPAL_SUCCESS);

	rc = opal_pci_eeh_freeze_clear(phb->opal_id, pe_no,
				       OPAL_EEH_ACTION_CLEAR_FREEZE_ALL);
	if (rc) {
		pr_warning("PCI %d: Failed to clear EEH freeze state"
			   " for PE#%d, err %ld\n",
			   phb->hose->global_number, pe_no, rc);

		/* For now, let's only display the diag buffer when we fail to clear
		 * the EEH status. We'll do more sensible things later when we have
		 * proper EEH support. We need to make sure we don't pollute ourselves
		 * with the normal errors generated when probing empty slots
		 */
		if (has_diag)
			pnv_pci_dump_phb_diag_data(phb);
		else
			pr_warning("PCI %d: No diag data available\n",
				   phb->hose->global_number);
	}

	spin_unlock_irqrestore(&phb->lock, flags);
}

static void pnv_pci_config_check_eeh(struct pnv_phb *phb, struct pci_bus *bus,
				     u32 bdfn)
{
	s64	rc;
	u8	fstate;
	u16	pcierr;
	u32	pe_no;

	/* Get PE# if we support IODA */
	pe_no = phb->bdfn_to_pe ? phb->bdfn_to_pe(phb, bus, bdfn & 0xff) : 0;

	/* Read freeze status */
	rc = opal_pci_eeh_freeze_status(phb->opal_id, pe_no, &fstate, &pcierr,
					NULL);
	if (rc) {
		pr_warning("PCI %d: Failed to read EEH status for PE#%d,"
			   " err %lld\n", phb->hose->global_number, pe_no, rc);
		return;
	}
	cfg_dbg(" -> EEH check, bdfn=%04x PE%d fstate=%x\n",
		bdfn, pe_no, fstate);
	if (fstate != 0)
		pnv_pci_handle_eeh_config(phb, pe_no);
}

static int pnv_pci_read_config(struct pci_bus *bus,
			       unsigned int devfn,
			       int where, int size, u32 *val)
{
	struct pci_controller *hose = pci_bus_to_host(bus);
	struct pnv_phb *phb = hose->private_data;
	u32 bdfn = (((uint64_t)bus->number) << 8) | devfn;
	s64 rc;

	if (hose == NULL)
		return PCIBIOS_DEVICE_NOT_FOUND;

=======
	int has_diag, ret = 0;

	spin_lock_irqsave(&phb->lock, flags);

	/* Fetch PHB diag-data */
	rc = opal_pci_get_phb_diag_data2(phb->opal_id, phb->diag_data,
					 phb->diag_data_size);
	has_diag = (rc == OPAL_SUCCESS);

	/* If PHB supports compound PE, to handle it */
	if (phb->unfreeze_pe) {
		ret = phb->unfreeze_pe(phb,
				       pe_no,
				       OPAL_EEH_ACTION_CLEAR_FREEZE_ALL);
	} else {
		rc = opal_pci_eeh_freeze_clear(phb->opal_id,
					     pe_no,
					     OPAL_EEH_ACTION_CLEAR_FREEZE_ALL);
		if (rc) {
			pr_warn("%s: Failure %ld clearing frozen "
				"PHB#%x-PE#%x\n",
				__func__, rc, phb->hose->global_number,
				pe_no);
			ret = -EIO;
		}
	}

	/*
	 * For now, let's only display the diag buffer when we fail to clear
	 * the EEH status. We'll do more sensible things later when we have
	 * proper EEH support. We need to make sure we don't pollute ourselves
	 * with the normal errors generated when probing empty slots
	 */
	if (has_diag && ret)
		pnv_pci_dump_phb_diag_data(phb->hose, phb->diag_data);

	spin_unlock_irqrestore(&phb->lock, flags);
}

static void pnv_pci_config_check_eeh(struct pci_dn *pdn)
{
	struct pnv_phb *phb = pdn->phb->private_data;
	u8	fstate = 0;
	__be16	pcierr = 0;
	unsigned int pe_no;
	s64	rc;

	/*
	 * Get the PE#. During the PCI probe stage, we might not
	 * setup that yet. So all ER errors should be mapped to
	 * reserved PE.
	 */
	pe_no = pdn->pe_number;
	if (pe_no == IODA_INVALID_PE) {
		pe_no = phb->ioda.reserved_pe_idx;
	}

	/*
	 * Fetch frozen state. If the PHB support compound PE,
	 * we need handle that case.
	 */
	if (phb->get_pe_state) {
		fstate = phb->get_pe_state(phb, pe_no);
	} else {
		rc = opal_pci_eeh_freeze_status(phb->opal_id,
						pe_no,
						&fstate,
						&pcierr,
						NULL);
		if (rc) {
			pr_warn("%s: Failure %lld getting PHB#%x-PE#%x state\n",
				__func__, rc, phb->hose->global_number, pe_no);
			return;
		}
	}

	pr_devel(" -> EEH check, bdfn=%04x PE#%x fstate=%x\n",
		 (pdn->busno << 8) | (pdn->devfn), pe_no, fstate);

	/* Clear the frozen state if applicable */
	if (fstate == OPAL_EEH_STOPPED_MMIO_FREEZE ||
	    fstate == OPAL_EEH_STOPPED_DMA_FREEZE  ||
	    fstate == OPAL_EEH_STOPPED_MMIO_DMA_FREEZE) {
		/*
		 * If PHB supports compound PE, freeze it for
		 * consistency.
		 */
		if (phb->freeze_pe)
			phb->freeze_pe(phb, pe_no);

		pnv_pci_handle_eeh_config(phb, pe_no);
	}
}

int pnv_pci_cfg_read(struct pci_dn *pdn,
		     int where, int size, u32 *val)
{
	struct pnv_phb *phb = pdn->phb->private_data;
	u32 bdfn = (pdn->busno << 8) | pdn->devfn;
	s64 rc;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (size) {
	case 1: {
		u8 v8;
		rc = opal_pci_config_read_byte(phb->opal_id, bdfn, where, &v8);
		*val = (rc == OPAL_SUCCESS) ? v8 : 0xff;
		break;
	}
	case 2: {
<<<<<<< HEAD
		u16 v16;
		rc = opal_pci_config_read_half_word(phb->opal_id, bdfn, where,
						   &v16);
		*val = (rc == OPAL_SUCCESS) ? v16 : 0xffff;
		break;
	}
	case 4: {
		u32 v32;
		rc = opal_pci_config_read_word(phb->opal_id, bdfn, where, &v32);
		*val = (rc == OPAL_SUCCESS) ? v32 : 0xffffffff;
=======
		__be16 v16;
		rc = opal_pci_config_read_half_word(phb->opal_id, bdfn, where,
						   &v16);
		*val = (rc == OPAL_SUCCESS) ? be16_to_cpu(v16) : 0xffff;
		break;
	}
	case 4: {
		__be32 v32;
		rc = opal_pci_config_read_word(phb->opal_id, bdfn, where, &v32);
		*val = (rc == OPAL_SUCCESS) ? be32_to_cpu(v32) : 0xffffffff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	default:
		return PCIBIOS_FUNC_NOT_SUPPORTED;
	}
<<<<<<< HEAD
	cfg_dbg("pnv_pci_read_config bus: %x devfn: %x +%x/%x -> %08x\n",
		bus->number, devfn, where, size, *val);

	/* Check if the PHB got frozen due to an error (no response) */
	pnv_pci_config_check_eeh(phb, bus, bdfn);

	return PCIBIOS_SUCCESSFUL;
}

static int pnv_pci_write_config(struct pci_bus *bus,
				unsigned int devfn,
				int where, int size, u32 val)
{
	struct pci_controller *hose = pci_bus_to_host(bus);
	struct pnv_phb *phb = hose->private_data;
	u32 bdfn = (((uint64_t)bus->number) << 8) | devfn;

	if (hose == NULL)
		return PCIBIOS_DEVICE_NOT_FOUND;

	cfg_dbg("pnv_pci_write_config bus: %x devfn: %x +%x/%x -> %08x\n",
		bus->number, devfn, where, size, val);
=======

	pr_devel("%s: bus: %x devfn: %x +%x/%x -> %08x\n",
		 __func__, pdn->busno, pdn->devfn, where, size, *val);
	return PCIBIOS_SUCCESSFUL;
}

int pnv_pci_cfg_write(struct pci_dn *pdn,
		      int where, int size, u32 val)
{
	struct pnv_phb *phb = pdn->phb->private_data;
	u32 bdfn = (pdn->busno << 8) | pdn->devfn;

	pr_devel("%s: bus: %x devfn: %x +%x/%x -> %08x\n",
		 __func__, pdn->busno, pdn->devfn, where, size, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (size) {
	case 1:
		opal_pci_config_write_byte(phb->opal_id, bdfn, where, val);
		break;
	case 2:
		opal_pci_config_write_half_word(phb->opal_id, bdfn, where, val);
		break;
	case 4:
		opal_pci_config_write_word(phb->opal_id, bdfn, where, val);
		break;
	default:
		return PCIBIOS_FUNC_NOT_SUPPORTED;
	}
<<<<<<< HEAD
	/* Check if the PHB got frozen due to an error (no response) */
	pnv_pci_config_check_eeh(phb, bus, bdfn);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return PCIBIOS_SUCCESSFUL;
}

<<<<<<< HEAD
struct pci_ops pnv_pci_ops = {
	.read = pnv_pci_read_config,
	.write = pnv_pci_write_config,
};


static void pnv_tce_invalidate(struct iommu_table *tbl,
			       u64 *startp, u64 *endp)
{
	u64 __iomem *invalidate = (u64 __iomem *)tbl->it_index;
	unsigned long start, end, inc;

	start = __pa(startp);
	end = __pa(endp);


	/* BML uses this case for p6/p7/galaxy2: Shift addr and put in node */
	if (tbl->it_busno) {
		start <<= 12;
		end <<= 12;
		inc = 128 << 12;
		start |= tbl->it_busno;
		end |= tbl->it_busno;
	}
	/* p7ioc-style invalidation, 2 TCEs per write */
	else if (tbl->it_type & TCE_PCI_SWINV_PAIR) {
		start |= (1ull << 63);
		end |= (1ull << 63);
		inc = 16;
	}
	/* Default (older HW) */
	else
		inc = 128;

	end |= inc - 1;		/* round up end to be different than start */

	mb(); /* Ensure above stores are visible */
	while (start <= end) {
		__raw_writeq(start, invalidate);
		start += inc;
	}
	/* The iommu layer will do another mb() for us on build() and
	 * we don't care on free()
	 */
}


static int pnv_tce_build(struct iommu_table *tbl, long index, long npages,
			 unsigned long uaddr, enum dma_data_direction direction,
			 struct dma_attrs *attrs)
{
	u64 proto_tce;
	u64 *tcep, *tces;
	u64 rpn;

	proto_tce = TCE_PCI_READ; // Read allowed

	if (direction != DMA_TO_DEVICE)
		proto_tce |= TCE_PCI_WRITE;

	tces = tcep = ((u64 *)tbl->it_base) + index - tbl->it_offset;
	rpn = __pa(uaddr) >> TCE_SHIFT;

	while (npages--)
		*(tcep++) = proto_tce | (rpn++ << TCE_RPN_SHIFT);

	/* Some implementations won't cache invalid TCEs and thus may not
	 * need that flush. We'll probably turn it_type into a bit mask
	 * of flags if that becomes the case
	 */
	if (tbl->it_type & TCE_PCI_SWINV_CREATE)
		pnv_tce_invalidate(tbl, tces, tcep - 1);

	return 0;
}

static void pnv_tce_free(struct iommu_table *tbl, long index, long npages)
{
	u64 *tcep, *tces;

	tces = tcep = ((u64 *)tbl->it_base) + index - tbl->it_offset;

	while (npages--)
		*(tcep++) = 0;

	if (tbl->it_type & TCE_PCI_SWINV_FREE)
		pnv_tce_invalidate(tbl, tces, tcep - 1);
}

void pnv_pci_setup_iommu_table(struct iommu_table *tbl,
			       void *tce_mem, u64 tce_size,
			       u64 dma_offset)
{
	tbl->it_blocksize = 16;
	tbl->it_base = (unsigned long)tce_mem;
	tbl->it_offset = dma_offset >> IOMMU_PAGE_SHIFT;
	tbl->it_index = 0;
	tbl->it_size = tce_size >> 3;
	tbl->it_busno = 0;
	tbl->it_type = TCE_PCI;
}

static struct iommu_table * __devinit
pnv_pci_setup_bml_iommu(struct pci_controller *hose)
{
	struct iommu_table *tbl;
	const __be64 *basep, *swinvp;
	const __be32 *sizep;

	basep = of_get_property(hose->dn, "linux,tce-base", NULL);
	sizep = of_get_property(hose->dn, "linux,tce-size", NULL);
	if (basep == NULL || sizep == NULL) {
		pr_err("PCI: %s has missing tce entries !\n",
		       hose->dn->full_name);
		return NULL;
	}
	tbl = kzalloc_node(sizeof(struct iommu_table), GFP_KERNEL, hose->node);
	if (WARN_ON(!tbl))
		return NULL;
	pnv_pci_setup_iommu_table(tbl, __va(be64_to_cpup(basep)),
				  be32_to_cpup(sizep), 0);
	iommu_init_table(tbl, hose->node);

	/* Deal with SW invalidated TCEs when needed (BML way) */
	swinvp = of_get_property(hose->dn, "linux,tce-sw-invalidate-info",
				 NULL);
	if (swinvp) {
		tbl->it_busno = swinvp[1];
		tbl->it_index = (unsigned long)ioremap(swinvp[0], 8);
		tbl->it_type = TCE_PCI_SWINV_CREATE | TCE_PCI_SWINV_FREE;
	}
	return tbl;
}

static void __devinit pnv_pci_dma_fallback_setup(struct pci_controller *hose,
						 struct pci_dev *pdev)
{
	struct device_node *np = pci_bus_to_OF_node(hose->bus);
	struct pci_dn *pdn;

	if (np == NULL)
		return;
	pdn = PCI_DN(np);
	if (!pdn->iommu_table)
		pdn->iommu_table = pnv_pci_setup_bml_iommu(hose);
	if (!pdn->iommu_table)
		return;
	set_iommu_table_base(&pdev->dev, pdn->iommu_table);
}

static void __devinit pnv_pci_dma_dev_setup(struct pci_dev *pdev)
{
	struct pci_controller *hose = pci_bus_to_host(pdev->bus);
	struct pnv_phb *phb = hose->private_data;

	/* If we have no phb structure, try to setup a fallback based on
	 * the device-tree (RTAS PCI for example)
	 */
	if (phb && phb->dma_dev_setup)
		phb->dma_dev_setup(phb, pdev);
	else
		pnv_pci_dma_fallback_setup(hose, pdev);
}

/* Fixup wrong class code in p7ioc root complex */
static void __devinit pnv_p7ioc_rc_quirk(struct pci_dev *dev)
{
	dev->class = PCI_CLASS_BRIDGE_PCI << 8;
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_IBM, 0x3b9, pnv_p7ioc_rc_quirk);

static int pnv_pci_probe_mode(struct pci_bus *bus)
{
	struct pci_controller *hose = pci_bus_to_host(bus);
	const __be64 *tstamp;
	u64 now, target;


	/* We hijack this as a way to ensure we have waited long
	 * enough since the reset was lifted on the PCI bus
	 */
	if (bus != hose->bus)
		return PCI_PROBE_NORMAL;
	tstamp = of_get_property(hose->dn, "reset-clear-timestamp", NULL);
	if (!tstamp || !*tstamp)
		return PCI_PROBE_NORMAL;

	now = mftb() / tb_ticks_per_usec;
	target = (be64_to_cpup(tstamp) / tb_ticks_per_usec)
		+ PCI_RESET_DELAY_US;

	pr_devel("pci %04d: Reset target: 0x%llx now: 0x%llx\n",
		 hose->global_number, target, now);

	if (now < target)
		msleep((target - now + 999) / 1000);

	return PCI_PROBE_NORMAL;
}

=======
#ifdef CONFIG_EEH
static bool pnv_pci_cfg_check(struct pci_dn *pdn)
{
	struct eeh_dev *edev = NULL;
	struct pnv_phb *phb = pdn->phb->private_data;

	/* EEH not enabled ? */
	if (!(phb->flags & PNV_PHB_FLAG_EEH))
		return true;

	/* PE reset or device removed ? */
	edev = pdn->edev;
	if (edev) {
		if (edev->pe &&
		    (edev->pe->state & EEH_PE_CFG_BLOCKED))
			return false;

		if (edev->mode & EEH_DEV_REMOVED)
			return false;
	}

	return true;
}
#else
static inline pnv_pci_cfg_check(struct pci_dn *pdn)
{
	return true;
}
#endif /* CONFIG_EEH */

static int pnv_pci_read_config(struct pci_bus *bus,
			       unsigned int devfn,
			       int where, int size, u32 *val)
{
	struct pci_dn *pdn;
	struct pnv_phb *phb;
	int ret;

	*val = 0xFFFFFFFF;
	pdn = pci_get_pdn_by_devfn(bus, devfn);
	if (!pdn)
		return PCIBIOS_DEVICE_NOT_FOUND;

	if (!pnv_pci_cfg_check(pdn))
		return PCIBIOS_DEVICE_NOT_FOUND;

	ret = pnv_pci_cfg_read(pdn, where, size, val);
	phb = pdn->phb->private_data;
	if (phb->flags & PNV_PHB_FLAG_EEH && pdn->edev) {
		if (*val == EEH_IO_ERROR_VALUE(size) &&
		    eeh_dev_check_failure(pdn->edev))
                        return PCIBIOS_DEVICE_NOT_FOUND;
	} else {
		pnv_pci_config_check_eeh(pdn);
	}

	return ret;
}

static int pnv_pci_write_config(struct pci_bus *bus,
				unsigned int devfn,
				int where, int size, u32 val)
{
	struct pci_dn *pdn;
	struct pnv_phb *phb;
	int ret;

	pdn = pci_get_pdn_by_devfn(bus, devfn);
	if (!pdn)
		return PCIBIOS_DEVICE_NOT_FOUND;

	if (!pnv_pci_cfg_check(pdn))
		return PCIBIOS_DEVICE_NOT_FOUND;

	ret = pnv_pci_cfg_write(pdn, where, size, val);
	phb = pdn->phb->private_data;
	if (!(phb->flags & PNV_PHB_FLAG_EEH))
		pnv_pci_config_check_eeh(pdn);

	return ret;
}

struct pci_ops pnv_pci_ops = {
	.read  = pnv_pci_read_config,
	.write = pnv_pci_write_config,
};

struct iommu_table *pnv_pci_table_alloc(int nid)
{
	struct iommu_table *tbl;

	tbl = kzalloc_node(sizeof(struct iommu_table), GFP_KERNEL, nid);
	if (!tbl)
		return NULL;

	INIT_LIST_HEAD_RCU(&tbl->it_group_list);
	kref_init(&tbl->it_kref);

	return tbl;
}

struct device_node *pnv_pci_get_phb_node(struct pci_dev *dev)
{
	struct pci_controller *hose = pci_bus_to_host(dev->bus);

	return of_node_get(hose->dn);
}
EXPORT_SYMBOL(pnv_pci_get_phb_node);

int pnv_pci_set_tunnel_bar(struct pci_dev *dev, u64 addr, int enable)
{
	struct pnv_phb *phb = pci_bus_to_pnvhb(dev->bus);
	u64 tunnel_bar;
	__be64 val;
	int rc;

	if (!opal_check_token(OPAL_PCI_GET_PBCQ_TUNNEL_BAR))
		return -ENXIO;
	if (!opal_check_token(OPAL_PCI_SET_PBCQ_TUNNEL_BAR))
		return -ENXIO;

	mutex_lock(&tunnel_mutex);
	rc = opal_pci_get_pbcq_tunnel_bar(phb->opal_id, &val);
	if (rc != OPAL_SUCCESS) {
		rc = -EIO;
		goto out;
	}
	tunnel_bar = be64_to_cpu(val);
	if (enable) {
		/*
		* Only one device per PHB can use atomics.
		* Our policy is first-come, first-served.
		*/
		if (tunnel_bar) {
			if (tunnel_bar != addr)
				rc = -EBUSY;
			else
				rc = 0;	/* Setting same address twice is ok */
			goto out;
		}
	} else {
		/*
		* The device that owns atomics and wants to release
		* them must pass the same address with enable == 0.
		*/
		if (tunnel_bar != addr) {
			rc = -EPERM;
			goto out;
		}
		addr = 0x0ULL;
	}
	rc = opal_pci_set_pbcq_tunnel_bar(phb->opal_id, addr);
	rc = opal_error_code(rc);
out:
	mutex_unlock(&tunnel_mutex);
	return rc;
}
EXPORT_SYMBOL_GPL(pnv_pci_set_tunnel_bar);

void pnv_pci_shutdown(void)
{
	struct pci_controller *hose;

	list_for_each_entry(hose, &hose_list, list_node)
		if (hose->controller_ops.shutdown)
			hose->controller_ops.shutdown(hose);
}

/* Fixup wrong class code in p7ioc and p8 root complex */
static void pnv_p7ioc_rc_quirk(struct pci_dev *dev)
{
	dev->class = PCI_CLASS_BRIDGE_PCI_NORMAL;
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_IBM, 0x3b9, pnv_p7ioc_rc_quirk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __init pnv_pci_init(void)
{
	struct device_node *np;

	pci_add_flags(PCI_CAN_SKIP_ISA_ALIGN);

<<<<<<< HEAD
	/* OPAL absent, try POPAL first then RTAS detection of PHBs */
	if (!firmware_has_feature(FW_FEATURE_OPAL)) {
#ifdef CONFIG_PPC_POWERNV_RTAS
		init_pci_config_tokens();
		find_and_init_phbs();
#endif /* CONFIG_PPC_POWERNV_RTAS */
	}
	/* OPAL is here, do our normal stuff */
	else {
		int found_ioda = 0;

		/* Look for IODA IO-Hubs. We don't support mixing IODA
		 * and p5ioc2 due to the need to change some global
		 * probing flags
		 */
		for_each_compatible_node(np, NULL, "ibm,ioda-hub") {
			pnv_pci_init_ioda_hub(np);
			found_ioda = 1;
		}

		/* Look for p5ioc2 IO-Hubs */
		if (!found_ioda)
			for_each_compatible_node(np, NULL, "ibm,p5ioc2")
				pnv_pci_init_p5ioc2_hub(np);
	}

	/* Setup the linkage between OF nodes and PHBs */
	pci_devs_phb_init();

	/* Configure IOMMU DMA hooks */
	ppc_md.pci_dma_dev_setup = pnv_pci_dma_dev_setup;
	ppc_md.tce_build = pnv_tce_build;
	ppc_md.tce_free = pnv_tce_free;
	ppc_md.pci_probe_mode = pnv_pci_probe_mode;
	set_pci_dma_ops(&dma_iommu_ops);

	/* Configure MSIs */
#ifdef CONFIG_PCI_MSI
	ppc_md.msi_check_device = pnv_msi_check_device;
	ppc_md.setup_msi_irqs = pnv_setup_msi_irqs;
	ppc_md.teardown_msi_irqs = pnv_teardown_msi_irqs;
#endif
=======
	/* If we don't have OPAL, eg. in sim, just skip PCI probe */
	if (!firmware_has_feature(FW_FEATURE_OPAL))
		return;

#ifdef CONFIG_PCIEPORTBUS
	/*
	 * On PowerNV PCIe devices are (currently) managed in cooperation
	 * with firmware. This isn't *strictly* required, but there's enough
	 * assumptions baked into both firmware and the platform code that
	 * it's unwise to allow the portbus services to be used.
	 *
	 * We need to fix this eventually, but for now set this flag to disable
	 * the portbus driver. The AER service isn't required since that AER
	 * events are handled via EEH. The pciehp hotplug driver can't work
	 * without kernel changes (and portbus binding breaks pnv_php). The
	 * other services also require some thinking about how we're going
	 * to integrate them.
	 */
	pcie_ports_disabled = true;
#endif

	/* Look for ioda2 built-in PHB3's */
	for_each_compatible_node(np, NULL, "ibm,ioda2-phb")
		pnv_pci_init_ioda2_phb(np);

	/* Look for ioda3 built-in PHB4's, we treat them as IODA2 */
	for_each_compatible_node(np, NULL, "ibm,ioda3-phb")
		pnv_pci_init_ioda2_phb(np);

	/* Look for NPU2 OpenCAPI PHBs */
	for_each_compatible_node(np, NULL, "ibm,ioda2-npu2-opencapi-phb")
		pnv_pci_init_npu2_opencapi_phb(np);

	/* Configure IOMMU DMA hooks */
	set_pci_dma_ops(&dma_iommu_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
