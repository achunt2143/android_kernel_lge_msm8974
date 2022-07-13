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
#ifndef RTL8225H
#define RTL8225H

#define RTL819X_TOTAL_RF_PATH 2
<<<<<<< HEAD
extern void PHY_SetRF8256Bandwidth(struct net_device *dev,
				   enum ht_channel_width Bandwidth);
extern bool PHY_RF8256_Config(struct net_device *dev);
extern bool phy_RF8256_Config_ParaFile(struct net_device *dev);
extern void PHY_SetRF8256CCKTxPower(struct net_device *dev, u8	powerlevel);
extern void PHY_SetRF8256OFDMTxPower(struct net_device *dev, u8 powerlevel);
=======
void rtl92e_set_bandwidth(struct net_device *dev,
			  enum ht_channel_width bandwidth);
bool rtl92e_config_rf(struct net_device *dev);
void rtl92e_set_cck_tx_power(struct net_device *dev, u8	powerlevel);
void rtl92e_set_ofdm_tx_power(struct net_device *dev, u8 powerlevel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
