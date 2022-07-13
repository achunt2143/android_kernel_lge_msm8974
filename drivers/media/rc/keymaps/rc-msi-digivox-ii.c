<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MSI DIGIVOX mini II remote controller keytable
 *
 * Copyright (C) 2010 Antti Palosaari <crope@iki.fi>
<<<<<<< HEAD
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table msi_digivox_ii[] = {
<<<<<<< HEAD
	{ 0x0002, KEY_2 },
	{ 0x0003, KEY_UP },              /* up */
	{ 0x0004, KEY_3 },
	{ 0x0005, KEY_CHANNELDOWN },
	{ 0x0008, KEY_5 },
	{ 0x0009, KEY_0 },
	{ 0x000b, KEY_8 },
	{ 0x000d, KEY_DOWN },            /* down */
	{ 0x0010, KEY_9 },
	{ 0x0011, KEY_7 },
	{ 0x0014, KEY_VOLUMEUP },
	{ 0x0015, KEY_CHANNELUP },
	{ 0x0016, KEY_OK },
	{ 0x0017, KEY_POWER2 },
	{ 0x001a, KEY_1 },
	{ 0x001c, KEY_4 },
	{ 0x001d, KEY_6 },
	{ 0x001f, KEY_VOLUMEDOWN },
=======
	{ 0x0302, KEY_NUMERIC_2 },
	{ 0x0303, KEY_UP },              /* up */
	{ 0x0304, KEY_NUMERIC_3 },
	{ 0x0305, KEY_CHANNELDOWN },
	{ 0x0308, KEY_NUMERIC_5 },
	{ 0x0309, KEY_NUMERIC_0 },
	{ 0x030b, KEY_NUMERIC_8 },
	{ 0x030d, KEY_DOWN },            /* down */
	{ 0x0310, KEY_NUMERIC_9 },
	{ 0x0311, KEY_NUMERIC_7 },
	{ 0x0314, KEY_VOLUMEUP },
	{ 0x0315, KEY_CHANNELUP },
	{ 0x0316, KEY_OK },
	{ 0x0317, KEY_POWER2 },
	{ 0x031a, KEY_NUMERIC_1 },
	{ 0x031c, KEY_NUMERIC_4 },
	{ 0x031d, KEY_NUMERIC_6 },
	{ 0x031f, KEY_VOLUMEDOWN },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct rc_map_list msi_digivox_ii_map = {
	.map = {
<<<<<<< HEAD
		.scan    = msi_digivox_ii,
		.size    = ARRAY_SIZE(msi_digivox_ii),
		.rc_type = RC_TYPE_NEC,
		.name    = RC_MAP_MSI_DIGIVOX_II,
=======
		.scan     = msi_digivox_ii,
		.size     = ARRAY_SIZE(msi_digivox_ii),
		.rc_proto = RC_PROTO_NEC,
		.name     = RC_MAP_MSI_DIGIVOX_II,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
};

static int __init init_rc_map_msi_digivox_ii(void)
{
	return rc_map_register(&msi_digivox_ii_map);
}

static void __exit exit_rc_map_msi_digivox_ii(void)
{
	rc_map_unregister(&msi_digivox_ii_map);
}

module_init(init_rc_map_msi_digivox_ii)
module_exit(exit_rc_map_msi_digivox_ii)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antti Palosaari <crope@iki.fi>");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("MSI DIGIVOX mini II remote controller keytable");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
