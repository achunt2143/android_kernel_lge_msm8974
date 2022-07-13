<<<<<<< HEAD
#ifndef __LINUX_USB_PCI_QUIRKS_H
#define __LINUX_USB_PCI_QUIRKS_H

#ifdef CONFIG_PCI
void uhci_reset_hc(struct pci_dev *pdev, unsigned long base);
int uhci_check_and_reset_hc(struct pci_dev *pdev, unsigned long base);
int usb_amd_find_chipset_info(void);
void usb_amd_dev_put(void);
void usb_amd_quirk_pll_disable(void);
void usb_amd_quirk_pll_enable(void);
bool usb_is_intel_switchable_xhci(struct pci_dev *pdev);
void usb_enable_xhci_ports(struct pci_dev *xhci_pdev);
void usb_disable_xhci_ports(struct pci_dev *xhci_pdev);
#else
static inline void usb_amd_quirk_pll_disable(void) {}
static inline void usb_amd_quirk_pll_enable(void) {}
static inline void usb_amd_dev_put(void) {}
static inline void usb_disable_xhci_ports(struct pci_dev *xhci_pdev) {}
#endif  /* CONFIG_PCI */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_USB_PCI_QUIRKS_H
#define __LINUX_USB_PCI_QUIRKS_H

#ifdef CONFIG_USB_PCI_AMD
int usb_hcd_amd_remote_wakeup_quirk(struct pci_dev *pdev);
bool usb_amd_hang_symptom_quirk(void);
bool usb_amd_prefetch_quirk(void);
void usb_amd_dev_put(void);
bool usb_amd_quirk_pll_check(void);
void usb_amd_quirk_pll_disable(void);
void usb_amd_quirk_pll_enable(void);
void sb800_prefetch(struct device *dev, int on);
bool usb_amd_pt_check_port(struct device *device, int port);
#else
static inline bool usb_amd_hang_symptom_quirk(void)
{
	return false;
};
static inline bool usb_amd_prefetch_quirk(void)
{
	return false;
}
static inline void usb_amd_quirk_pll_disable(void) {}
static inline void usb_amd_quirk_pll_enable(void) {}
static inline void usb_amd_dev_put(void) {}
static inline bool usb_amd_quirk_pll_check(void)
{
	return false;
}
static inline void sb800_prefetch(struct device *dev, int on) {}
static inline bool usb_amd_pt_check_port(struct device *device, int port)
{
	return false;
}
#endif /* CONFIG_USB_PCI_AMD */

#ifdef CONFIG_USB_PCI
void uhci_reset_hc(struct pci_dev *pdev, unsigned long base);
int uhci_check_and_reset_hc(struct pci_dev *pdev, unsigned long base);
void usb_asmedia_modifyflowcontrol(struct pci_dev *pdev);
void usb_enable_intel_xhci_ports(struct pci_dev *xhci_pdev);
void usb_disable_xhci_ports(struct pci_dev *xhci_pdev);
#else
struct pci_dev;
static inline void usb_asmedia_modifyflowcontrol(struct pci_dev *pdev) {}
static inline void usb_disable_xhci_ports(struct pci_dev *xhci_pdev) {}
#endif  /* CONFIG_USB_PCI */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif  /*  __LINUX_USB_PCI_QUIRKS_H  */
