<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_CRC7_H
#define _LINUX_CRC7_H
#include <linux/types.h>

<<<<<<< HEAD
extern const u8 crc7_syndrome_table[256];

static inline u8 crc7_byte(u8 crc, u8 data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}

extern u8 crc7(u8 crc, const u8 *buffer, size_t len);
=======
extern const u8 crc7_be_syndrome_table[256];

static inline u8 crc7_be_byte(u8 crc, u8 data)
{
	return crc7_be_syndrome_table[crc ^ data];
}

extern u8 crc7_be(u8 crc, const u8 *buffer, size_t len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
