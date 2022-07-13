<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * xHCI host controller driver PCI Bus Glue.
 *
 * Copyright (C) 2008 Intel Corp.
 *
 * Author: Sarah Sharp
 * Some code borrowed from the Linux EHCI driver.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/module.h>
<<<<<<< HEAD

#include "xhci.h"
=======
#include <linux/acpi.h>
#include <linux/reset.h>
#include <linux/suspend.h>

#include "xhci.h"
#include "xhci-trace.h"
#include "xhci-pci.h"

#define SSIC_PORT_NUM		2
#define SSIC_PORT_CFG2		0x880c
#define SSIC_PORT_CFG2_OFFSET	0x30
#define PROG_DONE		(1 << 30)
#define SSIC_PORT_UNUSED	(1 << 31)
#define SPARSE_DISABLE_BIT	17
#define SPARSE_CNTL_ENABLE	0xC12C
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Device for a quirk */
#define PCI_VENDOR_ID_FRESCO_LOGIC	0x1b73
#define PCI_DEVICE_ID_FRESCO_LOGIC_PDK	0x1000
<<<<<<< HEAD
#define PCI_DEVICE_ID_FRESCO_LOGIC_FL1400	0x1400

#define PCI_VENDOR_ID_ETRON		0x1b6f
#define PCI_DEVICE_ID_ASROCK_P67	0x7023

#define PCI_DEVICE_ID_INTEL_LYNXPOINT_XHCI	0x8c31
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_XHCI	0x9c31
#define PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI		0x22b5
#define PCI_DEVICE_ID_INTEL_SUNRISEPOINT_H_XHCI		0xa12f
#define PCI_DEVICE_ID_INTEL_SUNRISEPOINT_LP_XHCI	0x9d2f

static const char hcd_name[] = "xhci_hcd";

=======
#define PCI_DEVICE_ID_FRESCO_LOGIC_FL1009	0x1009
#define PCI_DEVICE_ID_FRESCO_LOGIC_FL1100	0x1100
#define PCI_DEVICE_ID_FRESCO_LOGIC_FL1400	0x1400

#define PCI_VENDOR_ID_ETRON		0x1b6f
#define PCI_DEVICE_ID_EJ168		0x7023

#define PCI_DEVICE_ID_INTEL_LYNXPOINT_XHCI	0x8c31
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_XHCI	0x9c31
#define PCI_DEVICE_ID_INTEL_WILDCATPOINT_LP_XHCI	0x9cb1
#define PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI		0x22b5
#define PCI_DEVICE_ID_INTEL_SUNRISEPOINT_H_XHCI		0xa12f
#define PCI_DEVICE_ID_INTEL_SUNRISEPOINT_LP_XHCI	0x9d2f
#define PCI_DEVICE_ID_INTEL_BROXTON_M_XHCI		0x0aa8
#define PCI_DEVICE_ID_INTEL_BROXTON_B_XHCI		0x1aa8
#define PCI_DEVICE_ID_INTEL_APL_XHCI			0x5aa8
#define PCI_DEVICE_ID_INTEL_DNV_XHCI			0x19d0
#define PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_2C_XHCI	0x15b5
#define PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_4C_XHCI	0x15b6
#define PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_LP_XHCI	0x15c1
#define PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_C_2C_XHCI	0x15db
#define PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_C_4C_XHCI	0x15d4
#define PCI_DEVICE_ID_INTEL_TITAN_RIDGE_2C_XHCI		0x15e9
#define PCI_DEVICE_ID_INTEL_TITAN_RIDGE_4C_XHCI		0x15ec
#define PCI_DEVICE_ID_INTEL_TITAN_RIDGE_DD_XHCI		0x15f0
#define PCI_DEVICE_ID_INTEL_ICE_LAKE_XHCI		0x8a13
#define PCI_DEVICE_ID_INTEL_CML_XHCI			0xa3af
#define PCI_DEVICE_ID_INTEL_TIGER_LAKE_XHCI		0x9a13
#define PCI_DEVICE_ID_INTEL_MAPLE_RIDGE_XHCI		0x1138
#define PCI_DEVICE_ID_INTEL_ALDER_LAKE_PCH_XHCI		0x51ed
#define PCI_DEVICE_ID_INTEL_ALDER_LAKE_N_PCH_XHCI	0x54ed

#define PCI_DEVICE_ID_AMD_RENOIR_XHCI			0x1639
#define PCI_DEVICE_ID_AMD_PROMONTORYA_4			0x43b9
#define PCI_DEVICE_ID_AMD_PROMONTORYA_3			0x43ba
#define PCI_DEVICE_ID_AMD_PROMONTORYA_2			0x43bb
#define PCI_DEVICE_ID_AMD_PROMONTORYA_1			0x43bc

#define PCI_DEVICE_ID_ASMEDIA_1042_XHCI			0x1042
#define PCI_DEVICE_ID_ASMEDIA_1042A_XHCI		0x1142
#define PCI_DEVICE_ID_ASMEDIA_1142_XHCI			0x1242
#define PCI_DEVICE_ID_ASMEDIA_2142_XHCI			0x2142
#define PCI_DEVICE_ID_ASMEDIA_3242_XHCI			0x3242

static const char hcd_name[] = "xhci_hcd";

static struct hc_driver __read_mostly xhci_pci_hc_driver;

static int xhci_pci_setup(struct usb_hcd *hcd);
static int xhci_pci_run(struct usb_hcd *hcd);
static int xhci_pci_update_hub_device(struct usb_hcd *hcd, struct usb_device *hdev,
				      struct usb_tt *tt, gfp_t mem_flags);

static const struct xhci_driver_overrides xhci_pci_overrides __initconst = {
	.reset = xhci_pci_setup,
	.start = xhci_pci_run,
	.update_hub_device = xhci_pci_update_hub_device,
};

static void xhci_msix_sync_irqs(struct xhci_hcd *xhci)
{
	struct usb_hcd *hcd = xhci_to_hcd(xhci);

	if (hcd->msix_enabled) {
		struct pci_dev *pdev = to_pci_dev(hcd->self.controller);

		/* for now, the driver only supports one primary interrupter */
		synchronize_irq(pci_irq_vector(pdev, 0));
	}
}

/* Free any IRQs and disable MSI-X */
static void xhci_cleanup_msix(struct xhci_hcd *xhci)
{
	struct usb_hcd *hcd = xhci_to_hcd(xhci);
	struct pci_dev *pdev = to_pci_dev(hcd->self.controller);

	/* return if using legacy interrupt */
	if (hcd->irq > 0)
		return;

	free_irq(pci_irq_vector(pdev, 0), xhci_to_hcd(xhci));
	pci_free_irq_vectors(pdev);
	hcd->msix_enabled = 0;
}

/* Try enabling MSI-X with MSI and legacy IRQ as fallback */
static int xhci_try_enable_msi(struct usb_hcd *hcd)
{
	struct pci_dev *pdev = to_pci_dev(hcd->self.controller);
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
	int ret;

	/*
	 * Some Fresco Logic host controllers advertise MSI, but fail to
	 * generate interrupts.  Don't even try to enable MSI.
	 */
	if (xhci->quirks & XHCI_BROKEN_MSI)
		goto legacy_irq;

	/* unregister the legacy interrupt */
	if (hcd->irq)
		free_irq(hcd->irq, hcd);
	hcd->irq = 0;

	/*
	 * calculate number of MSI-X vectors supported.
	 * - HCS_MAX_INTRS: the max number of interrupts the host can handle,
	 *   with max number of interrupters based on the xhci HCSPARAMS1.
	 * - num_online_cpus: maximum MSI-X vectors per CPUs core.
	 *   Add additional 1 vector to ensure always available interrupt.
	 */
	xhci->nvecs = min(num_online_cpus() + 1,
			  HCS_MAX_INTRS(xhci->hcs_params1));

	/* TODO: Check with MSI Soc for sysdev */
	xhci->nvecs = pci_alloc_irq_vectors(pdev, 1, xhci->nvecs,
					    PCI_IRQ_MSIX | PCI_IRQ_MSI);
	if (xhci->nvecs < 0) {
		xhci_dbg_trace(xhci, trace_xhci_dbg_init,
			       "failed to allocate IRQ vectors");
		goto legacy_irq;
	}

	ret = request_irq(pci_irq_vector(pdev, 0), xhci_msi_irq, 0, "xhci_hcd",
			  xhci_to_hcd(xhci));
	if (ret)
		goto free_irq_vectors;

	hcd->msi_enabled = 1;
	hcd->msix_enabled = pdev->msix_enabled;
	return 0;

free_irq_vectors:
	xhci_dbg_trace(xhci, trace_xhci_dbg_init, "disable %s interrupt",
		       pdev->msix_enabled ? "MSI-X" : "MSI");
	pci_free_irq_vectors(pdev);

legacy_irq:
	if (!pdev->irq) {
		xhci_err(xhci, "No msi-x/msi found and no IRQ in BIOS\n");
		return -EINVAL;
	}

	if (!strlen(hcd->irq_descr))
		snprintf(hcd->irq_descr, sizeof(hcd->irq_descr), "%s:usb%d",
			 hcd->driver->description, hcd->self.busnum);

	/* fall back to legacy interrupt */
	ret = request_irq(pdev->irq, &usb_hcd_irq, IRQF_SHARED, hcd->irq_descr, hcd);
	if (ret) {
		xhci_err(xhci, "request interrupt %d failed\n", pdev->irq);
		return ret;
	}
	hcd->irq = pdev->irq;
	return 0;
}

static int xhci_pci_run(struct usb_hcd *hcd)
{
	int ret;

	if (usb_hcd_is_primary_hcd(hcd)) {
		ret = xhci_try_enable_msi(hcd);
		if (ret)
			return ret;
	}

	return xhci_run(hcd);
}

static void xhci_pci_stop(struct usb_hcd *hcd)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);

	xhci_stop(hcd);

	if (usb_hcd_is_primary_hcd(hcd))
		xhci_cleanup_msix(xhci);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* called after powerup, by probe or system-pm "wakeup" */
static int xhci_pci_reinit(struct xhci_hcd *xhci, struct pci_dev *pdev)
{
	/*
	 * TODO: Implement finding debug ports later.
	 * TODO: see if there are any quirks that need to be added to handle
	 * new extended capabilities.
	 */

	/* PCI Memory-Write-Invalidate cycle support is optional (uncommon) */
	if (!pci_set_mwi(pdev))
		xhci_dbg(xhci, "MWI active\n");

	xhci_dbg(xhci, "Finished xhci_pci_reinit\n");
	return 0;
}

static void xhci_pci_quirks(struct device *dev, struct xhci_hcd *xhci)
{
<<<<<<< HEAD
	struct pci_dev		*pdev = to_pci_dev(dev);
=======
	struct pci_dev                  *pdev = to_pci_dev(dev);
	struct xhci_driver_data         *driver_data;
	const struct pci_device_id      *id;

	id = pci_match_id(to_pci_driver(pdev->dev.driver)->id_table, pdev);

	if (id && id->driver_data) {
		driver_data = (struct xhci_driver_data *)id->driver_data;
		xhci->quirks |= driver_data->quirks;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Look for vendor-specific quirks */
	if (pdev->vendor == PCI_VENDOR_ID_FRESCO_LOGIC &&
			(pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_PDK ||
			 pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_FL1400)) {
		if (pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_PDK &&
				pdev->revision == 0x0) {
			xhci->quirks |= XHCI_RESET_EP_QUIRK;
<<<<<<< HEAD
			xhci_dbg(xhci, "QUIRK: Fresco Logic xHC needs configure"
					" endpoint cmd after reset endpoint\n");
=======
			xhci_dbg_trace(xhci, trace_xhci_dbg_quirks,
				"XHCI_RESET_EP_QUIRK for this evaluation HW is deprecated");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_PDK &&
				pdev->revision == 0x4) {
			xhci->quirks |= XHCI_SLOW_SUSPEND;
<<<<<<< HEAD
			xhci_dbg(xhci,
=======
			xhci_dbg_trace(xhci, trace_xhci_dbg_quirks,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"QUIRK: Fresco Logic xHC revision %u"
				"must be suspended extra slowly",
				pdev->revision);
		}
<<<<<<< HEAD
=======
		if (pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_PDK)
			xhci->quirks |= XHCI_BROKEN_STREAMS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Fresco Logic confirms: all revisions of this chip do not
		 * support MSI, even though some of them claim to in their PCI
		 * capabilities.
		 */
		xhci->quirks |= XHCI_BROKEN_MSI;
<<<<<<< HEAD
		xhci_dbg(xhci, "QUIRK: Fresco Logic revision %u "
				"has broken MSI implementation\n",
=======
		xhci_dbg_trace(xhci, trace_xhci_dbg_quirks,
				"QUIRK: Fresco Logic revision %u "
				"has broken MSI implementation",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				pdev->revision);
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
	}

<<<<<<< HEAD
=======
	if (pdev->vendor == PCI_VENDOR_ID_FRESCO_LOGIC &&
			pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_FL1009)
		xhci->quirks |= XHCI_BROKEN_STREAMS;

	if (pdev->vendor == PCI_VENDOR_ID_FRESCO_LOGIC &&
			pdev->device == PCI_DEVICE_ID_FRESCO_LOGIC_FL1100)
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pdev->vendor == PCI_VENDOR_ID_NEC)
		xhci->quirks |= XHCI_NEC_HOST;

	if (pdev->vendor == PCI_VENDOR_ID_AMD && xhci->hci_version == 0x96)
		xhci->quirks |= XHCI_AMD_0x96_HOST;

	/* AMD PLL quirk */
<<<<<<< HEAD
	if (pdev->vendor == PCI_VENDOR_ID_AMD && usb_amd_find_chipset_info())
		xhci->quirks |= XHCI_AMD_PLL_FIX;

	if (pdev->vendor == PCI_VENDOR_ID_AMD)
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;

	if (pdev->vendor == PCI_VENDOR_ID_INTEL)
		xhci->quirks |= XHCI_AVOID_BEI;
=======
	if (pdev->vendor == PCI_VENDOR_ID_AMD && usb_amd_quirk_pll_check())
		xhci->quirks |= XHCI_AMD_PLL_FIX;

	if (pdev->vendor == PCI_VENDOR_ID_AMD &&
		(pdev->device == 0x145c ||
		 pdev->device == 0x15e0 ||
		 pdev->device == 0x15e1 ||
		 pdev->device == 0x43bb))
		xhci->quirks |= XHCI_SUSPEND_DELAY;

	if (pdev->vendor == PCI_VENDOR_ID_AMD &&
	    (pdev->device == 0x15e0 || pdev->device == 0x15e1))
		xhci->quirks |= XHCI_SNPS_BROKEN_SUSPEND;

	if (pdev->vendor == PCI_VENDOR_ID_AMD && pdev->device == 0x15e5) {
		xhci->quirks |= XHCI_DISABLE_SPARSE;
		xhci->quirks |= XHCI_RESET_ON_RESUME;
	}

	if (pdev->vendor == PCI_VENDOR_ID_AMD) {
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
		if (pdev->device == 0x43f7)
			xhci->quirks |= XHCI_DEFAULT_PM_RUNTIME_ALLOW;
	}

	if ((pdev->vendor == PCI_VENDOR_ID_AMD) &&
		((pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_4) ||
		(pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_3) ||
		(pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_2) ||
		(pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_1)))
		xhci->quirks |= XHCI_U2_DISABLE_WAKE;

	if (pdev->vendor == PCI_VENDOR_ID_AMD &&
		pdev->device == PCI_DEVICE_ID_AMD_RENOIR_XHCI)
		xhci->quirks |= XHCI_BROKEN_D3COLD_S2I;

	if (pdev->vendor == PCI_VENDOR_ID_INTEL) {
		xhci->quirks |= XHCI_LPM_SUPPORT;
		xhci->quirks |= XHCI_INTEL_HOST;
		xhci->quirks |= XHCI_AVOID_BEI;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
			pdev->device == PCI_DEVICE_ID_INTEL_PANTHERPOINT_XHCI) {
		xhci->quirks |= XHCI_EP_LIMIT_QUIRK;
		xhci->limit_active_eps = 64;
		xhci->quirks |= XHCI_SW_BW_CHECKING;
		/*
		 * PPT desktop boards DH77EB and DH77DF will power back on after
		 * a few seconds of being shutdown.  The fix for this is to
		 * switch the ports from xHCI to EHCI on shutdown.  We can't use
		 * DMI information to find those particular boards (since each
		 * vendor will change the board name), so we have to key off all
		 * PPT chipsets.
		 */
		xhci->quirks |= XHCI_SPURIOUS_REBOOT;
<<<<<<< HEAD
=======
	}
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
		(pdev->device == PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_WILDCATPOINT_LP_XHCI)) {
		xhci->quirks |= XHCI_SPURIOUS_REBOOT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xhci->quirks |= XHCI_SPURIOUS_WAKEUP;
	}
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
		(pdev->device == PCI_DEVICE_ID_INTEL_SUNRISEPOINT_LP_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_SUNRISEPOINT_H_XHCI ||
<<<<<<< HEAD
		 pdev->device == PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI)) {
		xhci->quirks |= XHCI_PME_STUCK_QUIRK;
	}
	if (pdev->vendor == PCI_VENDOR_ID_ETRON &&
			pdev->device == PCI_DEVICE_ID_ASROCK_P67) {
		xhci->quirks |= XHCI_RESET_ON_RESUME;
		xhci_dbg(xhci, "QUIRK: Resetting on resume\n");
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
	}
	if (pdev->vendor == PCI_VENDOR_ID_RENESAS &&
			pdev->device == 0x0015)
		xhci->quirks |= XHCI_RESET_ON_RESUME;
	if (pdev->vendor == PCI_VENDOR_ID_VIA)
		xhci->quirks |= XHCI_RESET_ON_RESUME;
}

/*
 * Make sure PME works on some Intel xHCI controllers by writing 1 to clear
 * the Internal PME flag bit in vendor specific PMCTRL register at offset 0x80a4
 */
static void xhci_pme_quirk(struct xhci_hcd *xhci)
{
	u32 val;
	void __iomem *reg;

	reg = (void __iomem *) xhci->cap_regs + 0x80a4;
	val = readl(reg);
	writel(val | BIT(28), reg);
	readl(reg);
}

=======
		 pdev->device == PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_BROXTON_M_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_BROXTON_B_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_APL_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_DNV_XHCI ||
		 pdev->device == PCI_DEVICE_ID_INTEL_CML_XHCI)) {
		xhci->quirks |= XHCI_PME_STUCK_QUIRK;
	}
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
	    pdev->device == PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI)
		xhci->quirks |= XHCI_SSIC_PORT_UNUSED;
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
	    (pdev->device == PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_SUNRISEPOINT_LP_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_APL_XHCI))
		xhci->quirks |= XHCI_INTEL_USB_ROLE_SW;
	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
	    (pdev->device == PCI_DEVICE_ID_INTEL_CHERRYVIEW_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_SUNRISEPOINT_LP_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_SUNRISEPOINT_H_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_APL_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_DNV_XHCI))
		xhci->quirks |= XHCI_MISSING_CAS;

	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
	    (pdev->device == PCI_DEVICE_ID_INTEL_ALDER_LAKE_PCH_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ALDER_LAKE_N_PCH_XHCI))
		xhci->quirks |= XHCI_RESET_TO_DEFAULT;

	if (pdev->vendor == PCI_VENDOR_ID_INTEL &&
	    (pdev->device == PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_2C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_4C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_LP_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_C_2C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ALPINE_RIDGE_C_4C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_TITAN_RIDGE_2C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_TITAN_RIDGE_4C_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_TITAN_RIDGE_DD_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_ICE_LAKE_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_TIGER_LAKE_XHCI ||
	     pdev->device == PCI_DEVICE_ID_INTEL_MAPLE_RIDGE_XHCI))
		xhci->quirks |= XHCI_DEFAULT_PM_RUNTIME_ALLOW;

	if (pdev->vendor == PCI_VENDOR_ID_ETRON &&
			pdev->device == PCI_DEVICE_ID_EJ168) {
		xhci->quirks |= XHCI_RESET_ON_RESUME;
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
		xhci->quirks |= XHCI_BROKEN_STREAMS;
	}
	if (pdev->vendor == PCI_VENDOR_ID_RENESAS &&
	    pdev->device == 0x0014) {
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
		xhci->quirks |= XHCI_ZERO_64B_REGS;
	}
	if (pdev->vendor == PCI_VENDOR_ID_RENESAS &&
	    pdev->device == 0x0015) {
		xhci->quirks |= XHCI_RESET_ON_RESUME;
		xhci->quirks |= XHCI_ZERO_64B_REGS;
	}
	if (pdev->vendor == PCI_VENDOR_ID_VIA)
		xhci->quirks |= XHCI_RESET_ON_RESUME;

	/* See https://bugzilla.kernel.org/show_bug.cgi?id=79511 */
	if (pdev->vendor == PCI_VENDOR_ID_VIA &&
			pdev->device == 0x3432)
		xhci->quirks |= XHCI_BROKEN_STREAMS;

	if (pdev->vendor == PCI_VENDOR_ID_VIA && pdev->device == 0x3483)
		xhci->quirks |= XHCI_LPM_SUPPORT;

	if (pdev->vendor == PCI_VENDOR_ID_ASMEDIA &&
		pdev->device == PCI_DEVICE_ID_ASMEDIA_1042_XHCI) {
		/*
		 * try to tame the ASMedia 1042 controller which reports 0.96
		 * but appears to behave more like 1.0
		 */
		xhci->quirks |= XHCI_SPURIOUS_SUCCESS;
		xhci->quirks |= XHCI_BROKEN_STREAMS;
	}
	if (pdev->vendor == PCI_VENDOR_ID_ASMEDIA &&
		pdev->device == PCI_DEVICE_ID_ASMEDIA_1042A_XHCI) {
		xhci->quirks |= XHCI_TRUST_TX_LENGTH;
		xhci->quirks |= XHCI_NO_64BIT_SUPPORT;
	}
	if (pdev->vendor == PCI_VENDOR_ID_ASMEDIA &&
	    (pdev->device == PCI_DEVICE_ID_ASMEDIA_1142_XHCI ||
	     pdev->device == PCI_DEVICE_ID_ASMEDIA_2142_XHCI ||
	     pdev->device == PCI_DEVICE_ID_ASMEDIA_3242_XHCI))
		xhci->quirks |= XHCI_NO_64BIT_SUPPORT;

	if (pdev->vendor == PCI_VENDOR_ID_ASMEDIA &&
		pdev->device == PCI_DEVICE_ID_ASMEDIA_1042A_XHCI)
		xhci->quirks |= XHCI_ASMEDIA_MODIFY_FLOWCONTROL;

	if (pdev->vendor == PCI_VENDOR_ID_TI && pdev->device == 0x8241)
		xhci->quirks |= XHCI_LIMIT_ENDPOINT_INTERVAL_7;

	if ((pdev->vendor == PCI_VENDOR_ID_BROADCOM ||
	     pdev->vendor == PCI_VENDOR_ID_CAVIUM) &&
	     pdev->device == 0x9026)
		xhci->quirks |= XHCI_RESET_PLL_ON_DISCONNECT;

	if (pdev->vendor == PCI_VENDOR_ID_AMD &&
	    (pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_2 ||
	     pdev->device == PCI_DEVICE_ID_AMD_PROMONTORYA_4))
		xhci->quirks |= XHCI_NO_SOFT_RETRY;

	if (pdev->vendor == PCI_VENDOR_ID_ZHAOXIN) {
		xhci->quirks |= XHCI_ZHAOXIN_HOST;
		xhci->quirks |= XHCI_LPM_SUPPORT;

		if (pdev->device == 0x9202) {
			xhci->quirks |= XHCI_RESET_ON_RESUME;
			xhci->quirks |= XHCI_ZHAOXIN_TRB_FETCH;
		}

		if (pdev->device == 0x9203)
			xhci->quirks |= XHCI_ZHAOXIN_TRB_FETCH;
	}

	/* xHC spec requires PCI devices to support D3hot and D3cold */
	if (xhci->hci_version >= 0x120)
		xhci->quirks |= XHCI_DEFAULT_PM_RUNTIME_ALLOW;

	if (xhci->quirks & XHCI_RESET_ON_RESUME)
		xhci_dbg_trace(xhci, trace_xhci_dbg_quirks,
				"QUIRK: Resetting on resume");
}

#ifdef CONFIG_ACPI
static void xhci_pme_acpi_rtd3_enable(struct pci_dev *dev)
{
	static const guid_t intel_dsm_guid =
		GUID_INIT(0xac340cb7, 0xe901, 0x45bf,
			  0xb7, 0xe6, 0x2b, 0x34, 0xec, 0x93, 0x1e, 0x23);
	union acpi_object *obj;

	obj = acpi_evaluate_dsm(ACPI_HANDLE(&dev->dev), &intel_dsm_guid, 3, 1,
				NULL);
	ACPI_FREE(obj);
}

static void xhci_find_lpm_incapable_ports(struct usb_hcd *hcd, struct usb_device *hdev)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	struct xhci_hub *rhub = &xhci->usb3_rhub;
	int ret;
	int i;

	/* This is not the usb3 roothub we are looking for */
	if (hcd != rhub->hcd)
		return;

	if (hdev->maxchild > rhub->num_ports) {
		dev_err(&hdev->dev, "USB3 roothub port number mismatch\n");
		return;
	}

	for (i = 0; i < hdev->maxchild; i++) {
		ret = usb_acpi_port_lpm_incapable(hdev, i);

		dev_dbg(&hdev->dev, "port-%d disable U1/U2 _DSM: %d\n", i + 1, ret);

		if (ret >= 0) {
			rhub->ports[i]->lpm_incapable = ret;
			continue;
		}
	}
}

#else
static void xhci_pme_acpi_rtd3_enable(struct pci_dev *dev) { }
static void xhci_find_lpm_incapable_ports(struct usb_hcd *hcd, struct usb_device *hdev) { }
#endif /* CONFIG_ACPI */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* called during probe() after chip reset completes */
static int xhci_pci_setup(struct usb_hcd *hcd)
{
	struct xhci_hcd		*xhci;
	struct pci_dev		*pdev = to_pci_dev(hcd->self.controller);
	int			retval;

<<<<<<< HEAD
=======
	xhci = hcd_to_xhci(hcd);
	if (!xhci->sbrn)
		pci_read_config_byte(pdev, XHCI_SBRN_OFFSET, &xhci->sbrn);

	/* imod_interval is the interrupt moderation value in nanoseconds. */
	xhci->imod_interval = 40000;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	retval = xhci_gen_setup(hcd, xhci_pci_quirks);
	if (retval)
		return retval;

<<<<<<< HEAD
	xhci = hcd_to_xhci(hcd);
	if (!usb_hcd_is_primary_hcd(hcd))
		return 0;

	pci_read_config_byte(pdev, XHCI_SBRN_OFFSET, &xhci->sbrn);
	xhci_dbg(xhci, "Got SBRN %u\n", (unsigned int) xhci->sbrn);

	/* Find any debug ports */
	retval = xhci_pci_reinit(xhci, pdev);
	if (!retval)
		return retval;

	kfree(xhci);
	return retval;
=======
	if (!usb_hcd_is_primary_hcd(hcd))
		return 0;

	if (xhci->quirks & XHCI_PME_STUCK_QUIRK)
		xhci_pme_acpi_rtd3_enable(pdev);

	xhci_dbg(xhci, "Got SBRN %u\n", (unsigned int) xhci->sbrn);

	/* Find any debug ports */
	return xhci_pci_reinit(xhci, pdev);
}

static int xhci_pci_update_hub_device(struct usb_hcd *hcd, struct usb_device *hdev,
				      struct usb_tt *tt, gfp_t mem_flags)
{
	/* Check if acpi claims some USB3 roothub ports are lpm incapable */
	if (!hdev->parent)
		xhci_find_lpm_incapable_ports(hcd, hdev);

	return xhci_update_hub_device(hcd, hdev, tt, mem_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We need to register our own PCI probe function (instead of the USB core's
 * function) in order to create a second roothub under xHCI.
 */
static int xhci_pci_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int retval;
	struct xhci_hcd *xhci;
<<<<<<< HEAD
	struct hc_driver *driver;
	struct usb_hcd *hcd;

	driver = (struct hc_driver *)id->driver_data;
=======
	struct usb_hcd *hcd;
	struct xhci_driver_data *driver_data;
	struct reset_control *reset;

	driver_data = (struct xhci_driver_data *)id->driver_data;
	if (driver_data && driver_data->quirks & XHCI_RENESAS_FW_QUIRK) {
		retval = renesas_xhci_check_request_fw(dev, id);
		if (retval)
			return retval;
	}

	reset = devm_reset_control_get_optional_exclusive(&dev->dev, NULL);
	if (IS_ERR(reset))
		return PTR_ERR(reset);
	reset_control_reset(reset);

	/* Prevent runtime suspending between USB-2 and USB-3 initialization */
	pm_runtime_get_noresume(&dev->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Register the USB 2.0 roothub.
	 * FIXME: USB core must know to register the USB 2.0 roothub first.
	 * This is sort of silly, because we could just set the HCD driver flags
	 * to say USB 2.0, but I'm not sure what the implications would be in
	 * the other parts of the HCD code.
	 */
<<<<<<< HEAD
	retval = usb_hcd_pci_probe(dev, id);

	if (retval)
		return retval;
=======
	retval = usb_hcd_pci_probe(dev, &xhci_pci_hc_driver);

	if (retval)
		goto put_runtime_pm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* USB 2.0 roothub is stored in the PCI device now. */
	hcd = dev_get_drvdata(&dev->dev);
	xhci = hcd_to_xhci(hcd);
<<<<<<< HEAD
	xhci->shared_hcd = usb_create_shared_hcd(driver, &dev->dev,
				pci_name(dev), hcd);
=======
	xhci->reset = reset;
	xhci->shared_hcd = usb_create_shared_hcd(&xhci_pci_hc_driver, &dev->dev,
						 pci_name(dev), hcd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!xhci->shared_hcd) {
		retval = -ENOMEM;
		goto dealloc_usb2_hcd;
	}

<<<<<<< HEAD
	/* Set the xHCI pointer before xhci_pci_setup() (aka hcd_driver.reset)
	 * is called by usb_add_hcd().
	 */
	*((struct xhci_hcd **) xhci->shared_hcd->hcd_priv) = xhci;
=======
	retval = xhci_ext_cap_init(xhci);
	if (retval)
		goto put_usb3_hcd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = usb_add_hcd(xhci->shared_hcd, dev->irq,
			IRQF_SHARED);
	if (retval)
		goto put_usb3_hcd;
	/* Roothub already marked as USB 3.0 speed */
<<<<<<< HEAD
=======

	if (!(xhci->quirks & XHCI_BROKEN_STREAMS) &&
			HCC_MAX_PSA(xhci->hcc_params) >= 4)
		xhci->shared_hcd->can_do_streams = 1;

	/* USB-2 and USB-3 roothubs initialized, allow runtime pm suspend */
	pm_runtime_put_noidle(&dev->dev);

	if (pci_choose_state(dev, PMSG_SUSPEND) == PCI_D0)
		pm_runtime_forbid(&dev->dev);
	else if (xhci->quirks & XHCI_DEFAULT_PM_RUNTIME_ALLOW)
		pm_runtime_allow(&dev->dev);

	dma_set_max_seg_size(&dev->dev, UINT_MAX);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

put_usb3_hcd:
	usb_put_hcd(xhci->shared_hcd);
dealloc_usb2_hcd:
	usb_hcd_pci_remove(dev);
<<<<<<< HEAD
=======
put_runtime_pm:
	pm_runtime_put_noidle(&dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

static void xhci_pci_remove(struct pci_dev *dev)
{
	struct xhci_hcd *xhci;

	xhci = hcd_to_xhci(pci_get_drvdata(dev));
<<<<<<< HEAD
	if (xhci->shared_hcd) {
		usb_remove_hcd(xhci->shared_hcd);
		usb_put_hcd(xhci->shared_hcd);
	}
	usb_hcd_pci_remove(dev);
=======

	xhci->xhc_state |= XHCI_STATE_REMOVING;

	if (xhci->quirks & XHCI_DEFAULT_PM_RUNTIME_ALLOW)
		pm_runtime_forbid(&dev->dev);

	if (xhci->shared_hcd) {
		usb_remove_hcd(xhci->shared_hcd);
		usb_put_hcd(xhci->shared_hcd);
		xhci->shared_hcd = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Workaround for spurious wakeups at shutdown with HSW */
	if (xhci->quirks & XHCI_SPURIOUS_WAKEUP)
		pci_set_power_state(dev, PCI_D3hot);

<<<<<<< HEAD
	kfree(xhci);
}

#ifdef CONFIG_PM
static int xhci_pci_suspend(struct usb_hcd *hcd, bool do_wakeup)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	int	retval = 0;

	if (hcd->state != HC_STATE_SUSPENDED ||
			xhci->shared_hcd->state != HC_STATE_SUSPENDED)
		return -EINVAL;

	if (xhci->quirks & XHCI_PME_STUCK_QUIRK)
		xhci_pme_quirk(xhci);

	retval = xhci_suspend(xhci, do_wakeup);

	return retval;
}

static int xhci_pci_resume(struct usb_hcd *hcd, bool hibernated)
=======
	usb_hcd_pci_remove(dev);
}

/*
 * In some Intel xHCI controllers, in order to get D3 working,
 * through a vendor specific SSIC CONFIG register at offset 0x883c,
 * SSIC PORT need to be marked as "unused" before putting xHCI
 * into D3. After D3 exit, the SSIC port need to be marked as "used".
 * Without this change, xHCI might not enter D3 state.
 */
static void xhci_ssic_port_unused_quirk(struct usb_hcd *hcd, bool suspend)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	u32 val;
	void __iomem *reg;
	int i;

	for (i = 0; i < SSIC_PORT_NUM; i++) {
		reg = (void __iomem *) xhci->cap_regs +
				SSIC_PORT_CFG2 +
				i * SSIC_PORT_CFG2_OFFSET;

		/* Notify SSIC that SSIC profile programming is not done. */
		val = readl(reg) & ~PROG_DONE;
		writel(val, reg);

		/* Mark SSIC port as unused(suspend) or used(resume) */
		val = readl(reg);
		if (suspend)
			val |= SSIC_PORT_UNUSED;
		else
			val &= ~SSIC_PORT_UNUSED;
		writel(val, reg);

		/* Notify SSIC that SSIC profile programming is done */
		val = readl(reg) | PROG_DONE;
		writel(val, reg);
		readl(reg);
	}
}

/*
 * Make sure PME works on some Intel xHCI controllers by writing 1 to clear
 * the Internal PME flag bit in vendor specific PMCTRL register at offset 0x80a4
 */
static void xhci_pme_quirk(struct usb_hcd *hcd)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	void __iomem *reg;
	u32 val;

	reg = (void __iomem *) xhci->cap_regs + 0x80a4;
	val = readl(reg);
	writel(val | BIT(28), reg);
	readl(reg);
}

static void xhci_sparse_control_quirk(struct usb_hcd *hcd)
{
	u32 reg;

	reg = readl(hcd->regs + SPARSE_CNTL_ENABLE);
	reg &= ~BIT(SPARSE_DISABLE_BIT);
	writel(reg, hcd->regs + SPARSE_CNTL_ENABLE);
}

static int xhci_pci_suspend(struct usb_hcd *hcd, bool do_wakeup)
{
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	struct pci_dev		*pdev = to_pci_dev(hcd->self.controller);
	int			ret;

	/*
	 * Systems with the TI redriver that loses port status change events
	 * need to have the registers polled during D3, so avoid D3cold.
	 */
	if (xhci->quirks & XHCI_COMP_MODE_QUIRK)
		pci_d3cold_disable(pdev);

#ifdef CONFIG_SUSPEND
	/* d3cold is broken, but only when s2idle is used */
	if (pm_suspend_target_state == PM_SUSPEND_TO_IDLE &&
	    xhci->quirks & (XHCI_BROKEN_D3COLD_S2I))
		pci_d3cold_disable(pdev);
#endif

	if (xhci->quirks & XHCI_PME_STUCK_QUIRK)
		xhci_pme_quirk(hcd);

	if (xhci->quirks & XHCI_SSIC_PORT_UNUSED)
		xhci_ssic_port_unused_quirk(hcd, true);

	if (xhci->quirks & XHCI_DISABLE_SPARSE)
		xhci_sparse_control_quirk(hcd);

	ret = xhci_suspend(xhci, do_wakeup);

	/* synchronize irq when using MSI-X */
	xhci_msix_sync_irqs(xhci);

	if (ret && (xhci->quirks & XHCI_SSIC_PORT_UNUSED))
		xhci_ssic_port_unused_quirk(hcd, false);

	return ret;
}

static int xhci_pci_resume(struct usb_hcd *hcd, pm_message_t msg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xhci_hcd		*xhci = hcd_to_xhci(hcd);
	struct pci_dev		*pdev = to_pci_dev(hcd->self.controller);
	int			retval = 0;

<<<<<<< HEAD
=======
	reset_control_reset(xhci->reset);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The BIOS on systems with the Intel Panther Point chipset may or may
	 * not support xHCI natively.  That means that during system resume, it
	 * may switch the ports back to EHCI so that users can use their
	 * keyboard to select a kernel from GRUB after resume from hibernate.
	 *
	 * The BIOS is supposed to remember whether the OS had xHCI ports
	 * enabled before resume, and switch the ports back to xHCI when the
	 * BIOS/OS semaphore is written, but we all know we can't trust BIOS
	 * writers.
	 *
	 * Unconditionally switch the ports back to xHCI after a system resume.
<<<<<<< HEAD
	 * We can't tell whether the EHCI or xHCI controller will be resumed
	 * first, so we have to do the port switchover in both drivers.  Writing
	 * a '1' to the port switchover registers should have no effect if the
	 * port was already switched over.
	 */
	if (usb_is_intel_switchable_xhci(pdev))
		usb_enable_xhci_ports(pdev);

	if (xhci->quirks & XHCI_PME_STUCK_QUIRK)
		xhci_pme_quirk(xhci);

	retval = xhci_resume(xhci, hibernated);
	return retval;
}
#endif /* CONFIG_PM */

static const struct hc_driver xhci_pci_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"xHCI Host Controller",
	.hcd_priv_size =	sizeof(struct xhci_hcd *),

	/*
	 * generic hardware linkage
	 */
	.irq =			xhci_irq,
	.flags =		HCD_MEMORY | HCD_USB3 | HCD_SHARED,

	/*
	 * basic lifecycle operations
	 */
	.reset =		xhci_pci_setup,
	.start =		xhci_run,
#ifdef CONFIG_PM
	.pci_suspend =          xhci_pci_suspend,
	.pci_resume =           xhci_pci_resume,
#endif
	.stop =			xhci_stop,
	.shutdown =		xhci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		xhci_urb_enqueue,
	.urb_dequeue =		xhci_urb_dequeue,
	.alloc_dev =		xhci_alloc_dev,
	.free_dev =		xhci_free_dev,
	.alloc_streams =	xhci_alloc_streams,
	.free_streams =		xhci_free_streams,
	.add_endpoint =		xhci_add_endpoint,
	.drop_endpoint =	xhci_drop_endpoint,
	.endpoint_reset =	xhci_endpoint_reset,
	.check_bandwidth =	xhci_check_bandwidth,
	.reset_bandwidth =	xhci_reset_bandwidth,
	.address_device =	xhci_address_device,
	.update_hub_device =	xhci_update_hub_device,
	.reset_device =		xhci_discover_or_reset_device,

	/*
	 * scheduling support
	 */
	.get_frame_number =	xhci_get_frame,

	/* Root hub support */
	.hub_control =		xhci_hub_control,
	.hub_status_data =	xhci_hub_status_data,
	.bus_suspend =		xhci_bus_suspend,
	.bus_resume =		xhci_bus_resume,
	/*
	 * call back when device connected and addressed
	 */
	.update_device =        xhci_update_device,
	.set_usb2_hw_lpm =	xhci_set_usb2_hardware_lpm,
};

/*-------------------------------------------------------------------------*/

/* PCI driver selection metadata; PCI hotplugging uses this */
static const struct pci_device_id pci_ids[] = { {
	/* handle any USB 3.0 xHCI controller */
	PCI_DEVICE_CLASS(PCI_CLASS_SERIAL_USB_XHCI, ~0),
	.driver_data =	(unsigned long) &xhci_pci_hc_driver,
=======
	 * It should not matter whether the EHCI or xHCI controller is
	 * resumed first. It's enough to do the switchover in xHCI because
	 * USB core won't notice anything as the hub driver doesn't start
	 * running again until after all the devices (including both EHCI and
	 * xHCI host controllers) have been resumed.
	 */

	if (pdev->vendor == PCI_VENDOR_ID_INTEL)
		usb_enable_intel_xhci_ports(pdev);

	if (xhci->quirks & XHCI_SSIC_PORT_UNUSED)
		xhci_ssic_port_unused_quirk(hcd, false);

	if (xhci->quirks & XHCI_PME_STUCK_QUIRK)
		xhci_pme_quirk(hcd);

	retval = xhci_resume(xhci, msg);
	return retval;
}

static int xhci_pci_poweroff_late(struct usb_hcd *hcd, bool do_wakeup)
{
	struct xhci_hcd		*xhci = hcd_to_xhci(hcd);
	struct xhci_port	*port;
	struct usb_device	*udev;
	u32			portsc;
	int			i;

	/*
	 * Systems with XHCI_RESET_TO_DEFAULT quirk have boot firmware that
	 * cause significant boot delay if usb ports are in suspended U3 state
	 * during boot. Some USB devices survive in U3 state over S4 hibernate
	 *
	 * Disable ports that are in U3 if remote wake is not enabled for either
	 * host controller or connected device
	 */

	if (!(xhci->quirks & XHCI_RESET_TO_DEFAULT))
		return 0;

	for (i = 0; i < HCS_MAX_PORTS(xhci->hcs_params1); i++) {
		port = &xhci->hw_ports[i];
		portsc = readl(port->addr);

		if ((portsc & PORT_PLS_MASK) != XDEV_U3)
			continue;

		if (!port->slot_id || !xhci->devs[port->slot_id]) {
			xhci_err(xhci, "No dev for slot_id %d for port %d-%d in U3\n",
				 port->slot_id, port->rhub->hcd->self.busnum,
				 port->hcd_portnum + 1);
			continue;
		}

		udev = xhci->devs[port->slot_id]->udev;

		/* if wakeup is enabled then don't disable the port */
		if (udev->do_remote_wakeup && do_wakeup)
			continue;

		xhci_dbg(xhci, "port %d-%d in U3 without wakeup, disable it\n",
			 port->rhub->hcd->self.busnum, port->hcd_portnum + 1);
		portsc = xhci_port_state_to_neutral(portsc);
		writel(portsc | PORT_PE, port->addr);
	}

	return 0;
}

static void xhci_pci_shutdown(struct usb_hcd *hcd)
{
	struct xhci_hcd		*xhci = hcd_to_xhci(hcd);
	struct pci_dev		*pdev = to_pci_dev(hcd->self.controller);

	xhci_shutdown(hcd);
	xhci_cleanup_msix(xhci);

	/* Yet another workaround for spurious wakeups at shutdown with HSW */
	if (xhci->quirks & XHCI_SPURIOUS_WAKEUP)
		pci_set_power_state(pdev, PCI_D3hot);
}

/*-------------------------------------------------------------------------*/

static const struct xhci_driver_data reneses_data = {
	.quirks  = XHCI_RENESAS_FW_QUIRK,
	.firmware = "renesas_usb_fw.mem",
};

/* PCI driver selection metadata; PCI hotplugging uses this */
static const struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE(0x1912, 0x0014),
		.driver_data =  (unsigned long)&reneses_data,
	},
	{ PCI_DEVICE(0x1912, 0x0015),
		.driver_data =  (unsigned long)&reneses_data,
	},
	/* handle any USB 3.0 xHCI controller */
	{ PCI_DEVICE_CLASS(PCI_CLASS_SERIAL_USB_XHCI, ~0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{ /* end: all zeroes */ }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

<<<<<<< HEAD
/* pci driver glue; this is a "new style" PCI driver module */
static struct pci_driver xhci_pci_driver = {
	.name =		(char *) hcd_name,
=======
/*
 * Without CONFIG_USB_XHCI_PCI_RENESAS renesas_xhci_check_request_fw() won't
 * load firmware, so don't encumber the xhci-pci driver with it.
 */
#if IS_ENABLED(CONFIG_USB_XHCI_PCI_RENESAS)
MODULE_FIRMWARE("renesas_usb_fw.mem");
#endif

/* pci driver glue; this is a "new style" PCI driver module */
static struct pci_driver xhci_pci_driver = {
	.name =		hcd_name,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table =	pci_ids,

	.probe =	xhci_pci_probe,
	.remove =	xhci_pci_remove,
	/* suspend and resume implemented later */

	.shutdown = 	usb_hcd_pci_shutdown,
<<<<<<< HEAD
#ifdef CONFIG_PM
	.driver = {
		.pm = &usb_hcd_pci_pm_ops
	},
#endif
};

int __init xhci_register_pci(void)
{
	return pci_register_driver(&xhci_pci_driver);
}

void xhci_unregister_pci(void)
{
	pci_unregister_driver(&xhci_pci_driver);
}
=======
	.driver = {
		.pm = pm_ptr(&usb_hcd_pci_pm_ops),
	},
};

static int __init xhci_pci_init(void)
{
	xhci_init_driver(&xhci_pci_hc_driver, &xhci_pci_overrides);
	xhci_pci_hc_driver.pci_suspend = pm_ptr(xhci_pci_suspend);
	xhci_pci_hc_driver.pci_resume = pm_ptr(xhci_pci_resume);
	xhci_pci_hc_driver.pci_poweroff_late = pm_ptr(xhci_pci_poweroff_late);
	xhci_pci_hc_driver.shutdown = pm_ptr(xhci_pci_shutdown);
	xhci_pci_hc_driver.stop = xhci_pci_stop;
	return pci_register_driver(&xhci_pci_driver);
}
module_init(xhci_pci_init);

static void __exit xhci_pci_exit(void)
{
	pci_unregister_driver(&xhci_pci_driver);
}
module_exit(xhci_pci_exit);

MODULE_DESCRIPTION("xHCI PCI Host Controller Driver");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
