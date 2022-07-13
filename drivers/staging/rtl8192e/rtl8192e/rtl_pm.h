<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
******************************************************************************/

=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef R8192E_PM_H
#define R8192E_PM_H

#include <linux/types.h>
#include <linux/pci.h>

<<<<<<< HEAD
int rtl8192E_save_state(struct pci_dev *dev, pm_message_t state);
int rtl8192E_suspend(struct pci_dev *dev, pm_message_t state);
int rtl8192E_resume(struct pci_dev *dev);
int rtl8192E_enable_wake(struct pci_dev *dev, pm_message_t state, int enable);
=======
int rtl92e_suspend(struct device *dev_d);
int rtl92e_resume(struct device *dev_d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
