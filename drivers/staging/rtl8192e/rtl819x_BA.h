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
#ifndef _BATYPE_H_
#define _BATYPE_H_

#define		TOTAL_TXBA_NUM	16
#define	TOTAL_RXBA_NUM	16

#define	BA_SETUP_TIMEOUT	200
#define	BA_INACT_TIMEOUT	60000
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
#ifndef _BATYPE_H_
#define _BATYPE_H_

#define	BA_SETUP_TIMEOUT	200
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define	BA_POLICY_DELAYED		0
#define	BA_POLICY_IMMEDIATE	1

#define	ADDBA_STATUS_SUCCESS			0
#define	ADDBA_STATUS_REFUSED		37
#define	ADDBA_STATUS_INVALID_PARAM	38

<<<<<<< HEAD
#define	DELBA_REASON_QSTA_LEAVING	36
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	DELBA_REASON_END_BA			37
#define	DELBA_REASON_UNKNOWN_BA	38
#define	DELBA_REASON_TIMEOUT			39
union sequence_control {
<<<<<<< HEAD
	u16 ShortData;
	struct {
		u16	FragNum:4;
		u16	SeqNum:12;
=======
	u16 short_data;
	struct {
		u16	frag_num:4;
		u16	seq_num:12;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} field;
};

union ba_param_set {
<<<<<<< HEAD
	u8 charData[2];
	u16 shortData;
	struct {
		u16 AMSDU_Support:1;
		u16 BAPolicy:1;
		u16 TID:4;
		u16 BufferSize:10;
=======
	u8 char_data[2];
	u16 short_data;
	struct {
		u16 amsdu_support:1;
		u16 ba_policy:1;
		u16 tid:4;
		u16 buffer_size:10;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} field;
};

union delba_param_set {
<<<<<<< HEAD
	u8 charData[2];
	u16 shortData;
	struct {
		u16 Reserved:11;
		u16 Initiator:1;
		u16 TID:4;
=======
	u8 char_data[2];
	u16 short_data;
	struct {
		u16 reserved:11;
		u16 initiator:1;
		u16 tid:4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} field;
};

struct ba_record {
<<<<<<< HEAD
	struct timer_list		Timer;
	u8				bValid;
	u8				DialogToken;
	union ba_param_set BaParamSet;
	u16				BaTimeoutValue;
	union sequence_control BaStartSeqCtrl;
=======
	struct timer_list timer;
	u8 b_valid;
	u8 dialog_token;
	union ba_param_set ba_param_set;
	u16 ba_timeout_value;
	union sequence_control ba_start_seq_ctrl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
