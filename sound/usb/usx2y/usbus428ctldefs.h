<<<<<<< HEAD
/*
 *
 * Copyright (c) 2003 by Karsten Wiese <annabellesgarden@yahoo.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

enum E_In84{
	eFader0 = 0,
	eFader1,
	eFader2,
	eFader3,
	eFader4,
	eFader5,
	eFader6,
	eFader7,
	eFaderM,
	eTransport,
	eModifier = 10,
	eFilterSelect,
	eSelect,
	eMute,

	eSwitch   = 15,
	eWheelGain,
	eWheelFreq,
	eWheelQ,
	eWheelPan,
	eWheel    = 20
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *
 * Copyright (c) 2003 by Karsten Wiese <annabellesgarden@yahoo.de>
 */

enum E_IN84 {
	E_FADER_0 = 0,
	E_FADER_1,
	E_FADER_2,
	E_FADER_3,
	E_FADER_4,
	E_FADER_5,
	E_FADER_6,
	E_FADER_7,
	E_FADER_M,
	E_TRANSPORT,
	E_MODIFIER = 10,
	E_FILTER_SELECT,
	E_SELECT,
	E_MUTE,

	E_SWITCH   = 15,
	E_WHEEL_GAIN,
	E_WHEEL_FREQ,
	E_WHEEL_Q,
	E_WHEEL_PAN,
	E_WHEEL    = 20
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define T_RECORD   1
#define T_PLAY     2
#define T_STOP     4
#define T_F_FWD    8
#define T_REW   0x10
#define T_SOLO  0x20
#define T_REC   0x40
#define T_NULL  0x80


struct us428_ctls {
<<<<<<< HEAD
	unsigned char   Fader[9];
	unsigned char 	Transport;
	unsigned char 	Modifier;
	unsigned char 	FilterSelect;
	unsigned char 	Select;
	unsigned char   Mute;
	unsigned char   UNKNOWN;
	unsigned char   Switch;	     
	unsigned char   Wheel[5];
};

struct us428_setByte {
	unsigned char Offset,
		Value;
};

enum {
	eLT_Volume = 0,
	eLT_Light
};

struct usX2Y_volume {
	unsigned char Channel,
		LH,
		LL,
		RH,
		RL;
};

struct us428_lights {
	struct us428_setByte Light[7];
=======
	unsigned char	fader[9];
	unsigned char	transport;
	unsigned char	modifier;
	unsigned char	filters_elect;
	unsigned char	select;
	unsigned char	mute;
	unsigned char	unknown;
	unsigned char	wswitch;
	unsigned char	wheel[5];
};

struct us428_set_byte {
	unsigned char offset,
		value;
};

enum {
	ELT_VOLUME = 0,
	ELT_LIGHT
};

struct usx2y_volume {
	unsigned char channel,
		lh,
		ll,
		rh,
		rl;
};

struct us428_lights {
	struct us428_set_byte light[7];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct us428_p4out {
	char type;
	union {
<<<<<<< HEAD
		struct usX2Y_volume vol;
=======
		struct usx2y_volume vol;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct us428_lights lights;
	} val;
};

<<<<<<< HEAD
#define N_us428_ctl_BUFS 16
#define N_us428_p4out_BUFS 16
struct us428ctls_sharedmem{
	struct us428_ctls	CtlSnapShot[N_us428_ctl_BUFS];
	int			CtlSnapShotDiffersAt[N_us428_ctl_BUFS];
	int			CtlSnapShotLast, CtlSnapShotRed;
	struct us428_p4out	p4out[N_us428_p4out_BUFS];
	int			p4outLast, p4outSent;
};
=======
#define N_US428_CTL_BUFS 16
#define N_US428_P4OUT_BUFS 16
struct us428ctls_sharedmem {
	struct us428_ctls	ctl_snapshot[N_US428_CTL_BUFS];
	int			ctl_snapshot_differs_at[N_US428_CTL_BUFS];
	int			ctl_snapshot_last, ctl_snapshot_red;
	struct us428_p4out	p4out[N_US428_P4OUT_BUFS];
	int			p4out_last, p4out_sent;
};

#define US428_SHAREDMEM_PAGES	PAGE_ALIGN(sizeof(struct us428ctls_sharedmem))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
