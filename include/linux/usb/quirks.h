<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This file holds the definitions of quirks found in USB devices.
 * Only quirks that affect the whole device, not an interface,
 * belong here.
 */

#ifndef __LINUX_USB_QUIRKS_H
#define __LINUX_USB_QUIRKS_H

/* string descriptors must not be fetched using a 255-byte read */
<<<<<<< HEAD
#define USB_QUIRK_STRING_FETCH_255	0x00000001

/* device can't resume correctly so reset it instead */
#define USB_QUIRK_RESET_RESUME		0x00000002

/* device can't handle Set-Interface requests */
#define USB_QUIRK_NO_SET_INTF		0x00000004

/* device can't handle its Configuration or Interface strings */
#define USB_QUIRK_CONFIG_INTF_STRINGS	0x00000008

/*device will morph if reset, don't use reset for handling errors */
#define USB_QUIRK_RESET_MORPHS		0x00000010

/* device has more interface descriptions than the bNumInterfaces count,
   and can't handle talking to these interfaces */
#define USB_QUIRK_HONOR_BNUMINTERFACES	0x00000020

/* device needs a pause during initialization, after we read the device
   descriptor */
#define USB_QUIRK_DELAY_INIT		0x00000040

#define USB_QUIRK_OTG_PET		0x00000080

/* device can't handle device_qualifier descriptor requests */
#define USB_QUIRK_DEVICE_QUALIFIER	0x00000100

/*
 * For high speed and super speed interupt endpoints, the USB 2.0 and
=======
#define USB_QUIRK_STRING_FETCH_255		BIT(0)

/* device can't resume correctly so reset it instead */
#define USB_QUIRK_RESET_RESUME			BIT(1)

/* device can't handle Set-Interface requests */
#define USB_QUIRK_NO_SET_INTF			BIT(2)

/* device can't handle its Configuration or Interface strings */
#define USB_QUIRK_CONFIG_INTF_STRINGS		BIT(3)

/* device can't be reset(e.g morph devices), don't use reset */
#define USB_QUIRK_RESET				BIT(4)

/* device has more interface descriptions than the bNumInterfaces count,
   and can't handle talking to these interfaces */
#define USB_QUIRK_HONOR_BNUMINTERFACES		BIT(5)

/* device needs a pause during initialization, after we read the device
   descriptor */
#define USB_QUIRK_DELAY_INIT			BIT(6)

/*
 * For high speed and super speed interrupt endpoints, the USB 2.0 and
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * USB 3.0 spec require the interval in microframes
 * (1 microframe = 125 microseconds) to be calculated as
 * interval = 2 ^ (bInterval-1).
 *
 * Devices with this quirk report their bInterval as the result of this
 * calculation instead of the exponent variable used in the calculation.
 */
<<<<<<< HEAD
#define USB_QUIRK_LINEAR_UFRAME_INTR_BINTERVAL	0x00000080
=======
#define USB_QUIRK_LINEAR_UFRAME_INTR_BINTERVAL	BIT(7)

/* device can't handle device_qualifier descriptor requests */
#define USB_QUIRK_DEVICE_QUALIFIER		BIT(8)

/* device generates spurious wakeup, ignore remote wakeup capability */
#define USB_QUIRK_IGNORE_REMOTE_WAKEUP		BIT(9)

/* device can't handle Link Power Management */
#define USB_QUIRK_NO_LPM			BIT(10)

/*
 * Device reports its bInterval as linear frames instead of the
 * USB 2.0 calculation.
 */
#define USB_QUIRK_LINEAR_FRAME_INTR_BINTERVAL	BIT(11)

/*
 * Device needs to be disconnected before suspend to prevent spurious
 * wakeup.
 */
#define USB_QUIRK_DISCONNECT_SUSPEND		BIT(12)

/* Device needs a pause after every control message. */
#define USB_QUIRK_DELAY_CTRL_MSG		BIT(13)

/* Hub needs extra delay after resetting its port. */
#define USB_QUIRK_HUB_SLOW_RESET		BIT(14)

/* device has endpoints that should be ignored */
#define USB_QUIRK_ENDPOINT_IGNORE		BIT(15)

/* short SET_ADDRESS request timeout */
#define USB_QUIRK_SHORT_SET_ADDRESS_REQ_TIMEOUT	BIT(16)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __LINUX_USB_QUIRKS_H */
