<<<<<<< HEAD
/*
 * UUID/GUID definition
 *
 * Copyright (C) 2010, Intel Corp.
 *	Huang Ying <ying.huang@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINUX_UUID_H_
#define _LINUX_UUID_H_

#include <linux/types.h>
#include <linux/string.h>

typedef struct {
	__u8 b[16];
} uuid_le;

typedef struct {
	__u8 b[16];
} uuid_be;

#define UUID_LE(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)		\
((uuid_le)								\
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * UUID/GUID definition
 *
 * Copyright (C) 2010, 2016 Intel Corp.
 *	Huang Ying <ying.huang@intel.com>
 */
#ifndef _LINUX_UUID_H_
#define _LINUX_UUID_H_

#include <linux/string.h>

#define UUID_SIZE 16

typedef struct {
	__u8 b[UUID_SIZE];
} guid_t;

typedef struct {
	__u8 b[UUID_SIZE];
} uuid_t;

#define GUID_INIT(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)			\
((guid_t)								\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{{ (a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff, ((a) >> 24) & 0xff, \
   (b) & 0xff, ((b) >> 8) & 0xff,					\
   (c) & 0xff, ((c) >> 8) & 0xff,					\
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})

<<<<<<< HEAD
#define UUID_BE(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)		\
((uuid_be)								\
=======
#define UUID_INIT(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7)			\
((uuid_t)								\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{{ ((a) >> 24) & 0xff, ((a) >> 16) & 0xff, ((a) >> 8) & 0xff, (a) & 0xff, \
   ((b) >> 8) & 0xff, (b) & 0xff,					\
   ((c) >> 8) & 0xff, (c) & 0xff,					\
   (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7) }})

<<<<<<< HEAD
#define NULL_UUID_LE							\
	UUID_LE(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00,	\
		0x00, 0x00, 0x00, 0x00)

#define NULL_UUID_BE							\
	UUID_BE(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00,	\
		0x00, 0x00, 0x00, 0x00)

static inline int uuid_le_cmp(const uuid_le u1, const uuid_le u2)
{
	return memcmp(&u1, &u2, sizeof(uuid_le));
}

static inline int uuid_be_cmp(const uuid_be u1, const uuid_be u2)
{
	return memcmp(&u1, &u2, sizeof(uuid_be));
}

extern void uuid_le_gen(uuid_le *u);
extern void uuid_be_gen(uuid_be *u);
=======
/*
 * The length of a UUID string ("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee")
 * not including trailing NUL.
 */
#define	UUID_STRING_LEN		36

extern const guid_t guid_null;
extern const uuid_t uuid_null;

static inline bool guid_equal(const guid_t *u1, const guid_t *u2)
{
	return memcmp(u1, u2, sizeof(guid_t)) == 0;
}

static inline void guid_copy(guid_t *dst, const guid_t *src)
{
	memcpy(dst, src, sizeof(guid_t));
}

static inline void import_guid(guid_t *dst, const __u8 *src)
{
	memcpy(dst, src, sizeof(guid_t));
}

static inline void export_guid(__u8 *dst, const guid_t *src)
{
	memcpy(dst, src, sizeof(guid_t));
}

static inline bool guid_is_null(const guid_t *guid)
{
	return guid_equal(guid, &guid_null);
}

static inline bool uuid_equal(const uuid_t *u1, const uuid_t *u2)
{
	return memcmp(u1, u2, sizeof(uuid_t)) == 0;
}

static inline void uuid_copy(uuid_t *dst, const uuid_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline void import_uuid(uuid_t *dst, const __u8 *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline void export_uuid(__u8 *dst, const uuid_t *src)
{
	memcpy(dst, src, sizeof(uuid_t));
}

static inline bool uuid_is_null(const uuid_t *uuid)
{
	return uuid_equal(uuid, &uuid_null);
}

void generate_random_uuid(unsigned char uuid[16]);
void generate_random_guid(unsigned char guid[16]);

extern void guid_gen(guid_t *u);
extern void uuid_gen(uuid_t *u);

bool __must_check uuid_is_valid(const char *uuid);

extern const u8 guid_index[16];
extern const u8 uuid_index[16];

int guid_parse(const char *uuid, guid_t *u);
int uuid_parse(const char *uuid, uuid_t *u);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
