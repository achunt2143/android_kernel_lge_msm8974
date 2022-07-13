<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Modifications for inclusion into the Linux staging tree are
 * Copyright(c) 2010 Larry Finger. All rights reserved.
 *
 * Contact information:
 * WLAN FAE <wlanfae@realtek.com>
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/
#ifndef __OSDEP_INTF_H_
#define __OSDEP_INTF_H_

#include "osdep_service.h"
#include "drv_types.h"

<<<<<<< HEAD
#define RND4(x)	(((x >> 2) + (((x & 3) == 0) ?  0 : 1)) << 2)
=======
#define RND4(x)	(((x >> 2) + ((x & 3) != 0)) << 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct intf_priv {
	u8 *intf_dev;
	/* when in USB, IO is through interrupt in/out endpoints */
	struct usb_device *udev;
	struct urb *piorw_urb;
<<<<<<< HEAD
	struct semaphore io_retevt;
=======
	struct completion io_retevt_comp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int r871x_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);

#endif	/*_OSDEP_INTF_H_*/
