<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * unicode.c
 *
 * PURPOSE
 *	Routines for converting between UTF-8 and OSTA Compressed Unicode.
 *      Also handles filename mangling
 *
 * DESCRIPTION
 *	OSTA Compressed Unicode is explained in the OSTA UDF specification.
 *		http://www.osta.org/
 *	UTF-8 is explained in the IETF RFC XXXX.
 *		ftp://ftp.internic.net/rfc/rfcxxxx.txt
 *
<<<<<<< HEAD
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include "udfdecl.h"

#include <linux/kernel.h>
#include <linux/string.h>	/* for memset */
#include <linux/nls.h>
#include <linux/crc-itu-t.h>
#include <linux/slab.h>

#include "udf_sb.h"

<<<<<<< HEAD
static int udf_translate_to_linux(uint8_t *, int, uint8_t *, int, uint8_t *,
				  int);

static int udf_char_to_ustr(struct ustr *dest, const uint8_t *src, int strlen)
{
	if ((!dest) || (!src) || (!strlen) || (strlen > UDF_NAME_LEN - 2))
		return 0;

	memset(dest, 0, sizeof(struct ustr));
	memcpy(dest->u_name, src, strlen);
	dest->u_cmpID = 0x08;
	dest->u_len = strlen;

	return strlen;
}

/*
 * udf_build_ustr
 */
int udf_build_ustr(struct ustr *dest, dstring *ptr, int size)
{
	int usesize;

	if (!dest || !ptr || !size)
		return -1;
	BUG_ON(size < 2);

	usesize = min_t(size_t, ptr[size - 1], sizeof(dest->u_name));
	usesize = min(usesize, size - 2);
	dest->u_cmpID = ptr[0];
	dest->u_len = usesize;
	memcpy(dest->u_name, ptr + 1, usesize);
	memset(dest->u_name + usesize, 0, sizeof(dest->u_name) - usesize);

	return 0;
}

/*
 * udf_build_ustr_exact
 */
static int udf_build_ustr_exact(struct ustr *dest, dstring *ptr, int exactsize)
{
	if ((!dest) || (!ptr) || (!exactsize))
		return -1;

	memset(dest, 0, sizeof(struct ustr));
	dest->u_cmpID = ptr[0];
	dest->u_len = exactsize - 1;
	memcpy(dest->u_name, ptr + 1, exactsize - 1);

	return 0;
}

/*
 * udf_ocu_to_utf8
 *
 * PURPOSE
 *	Convert OSTA Compressed Unicode to the UTF-8 equivalent.
 *
 * PRE-CONDITIONS
 *	utf			Pointer to UTF-8 output buffer.
 *	ocu			Pointer to OSTA Compressed Unicode input buffer
 *				of size UDF_NAME_LEN bytes.
 * 				both of type "struct ustr *"
 *
 * POST-CONDITIONS
 *	<return>		Zero on success.
 *
 * HISTORY
 *	November 12, 1997 - Andrew E. Mileski
 *	Written, tested, and released.
 */
int udf_CS0toUTF8(struct ustr *utf_o, const struct ustr *ocu_i)
{
	const uint8_t *ocu;
	uint8_t cmp_id, ocu_len;
	int i;

	ocu_len = ocu_i->u_len;
	if (ocu_len == 0) {
		memset(utf_o, 0, sizeof(struct ustr));
		return 0;
	}

	cmp_id = ocu_i->u_cmpID;
	if (cmp_id != 8 && cmp_id != 16) {
		memset(utf_o, 0, sizeof(struct ustr));
		pr_err("unknown compression code (%d) stri=%s\n",
		       cmp_id, ocu_i->u_name);
		return 0;
	}

	ocu = ocu_i->u_name;
	utf_o->u_len = 0;
	for (i = 0; (i < ocu_len) && (utf_o->u_len <= (UDF_NAME_LEN - 3));) {

		/* Expand OSTA compressed Unicode to Unicode */
		uint32_t c = ocu[i++];
		if (cmp_id == 16)
			c = (c << 8) | ocu[i++];

		/* Compress Unicode to UTF-8 */
		if (c < 0x80U)
			utf_o->u_name[utf_o->u_len++] = (uint8_t)c;
		else if (c < 0x800U) {
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0xc0 | (c >> 6));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 | (c & 0x3f));
		} else {
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0xe0 | (c >> 12));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 |
							  ((c >> 6) & 0x3f));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 | (c & 0x3f));
		}
	}
	utf_o->u_cmpID = 8;

	return utf_o->u_len;
}

/*
 *
 * udf_utf8_to_ocu
 *
 * PURPOSE
 *	Convert UTF-8 to the OSTA Compressed Unicode equivalent.
 *
 * DESCRIPTION
 *	This routine is only called by udf_lookup().
 *
 * PRE-CONDITIONS
 *	ocu			Pointer to OSTA Compressed Unicode output
 *				buffer of size UDF_NAME_LEN bytes.
 *	utf			Pointer to UTF-8 input buffer.
 *	utf_len			Length of UTF-8 input buffer in bytes.
 *
 * POST-CONDITIONS
 *	<return>		Zero on success.
 *
 * HISTORY
 *	November 12, 1997 - Andrew E. Mileski
 *	Written, tested, and released.
 */
static int udf_UTF8toCS0(dstring *ocu, struct ustr *utf, int length)
{
	unsigned c, i, max_val, utf_char;
	int utf_cnt, u_len;

	memset(ocu, 0, sizeof(dstring) * length);
	ocu[0] = 8;
	max_val = 0xffU;

try_again:
	u_len = 0U;
	utf_char = 0U;
	utf_cnt = 0U;
	for (i = 0U; i < utf->u_len; i++) {
		c = (uint8_t)utf->u_name[i];

		/* Complete a multi-byte UTF-8 character */
		if (utf_cnt) {
			utf_char = (utf_char << 6) | (c & 0x3fU);
			if (--utf_cnt)
				continue;
		} else {
			/* Check for a multi-byte UTF-8 character */
			if (c & 0x80U) {
				/* Start a multi-byte UTF-8 character */
				if ((c & 0xe0U) == 0xc0U) {
					utf_char = c & 0x1fU;
					utf_cnt = 1;
				} else if ((c & 0xf0U) == 0xe0U) {
					utf_char = c & 0x0fU;
					utf_cnt = 2;
				} else if ((c & 0xf8U) == 0xf0U) {
					utf_char = c & 0x07U;
					utf_cnt = 3;
				} else if ((c & 0xfcU) == 0xf8U) {
					utf_char = c & 0x03U;
					utf_cnt = 4;
				} else if ((c & 0xfeU) == 0xfcU) {
					utf_char = c & 0x01U;
					utf_cnt = 5;
				} else {
					goto error_out;
				}
				continue;
			} else {
				/* Single byte UTF-8 character (most common) */
				utf_char = c;
			}
		}

		/* Choose no compression if necessary */
		if (utf_char > max_val) {
			if (max_val == 0xffU) {
				max_val = 0xffffU;
				ocu[0] = (uint8_t)0x10U;
				goto try_again;
			}
			goto error_out;
		}

		if (max_val == 0xffffU)
			ocu[++u_len] = (uint8_t)(utf_char >> 8);
		ocu[++u_len] = (uint8_t)(utf_char & 0xffU);
	}

	if (utf_cnt) {
error_out:
		ocu[++u_len] = '?';
		printk(KERN_DEBUG pr_fmt("bad UTF-8 character\n"));
	}

	ocu[length - 1] = (uint8_t)u_len + 1;

	return u_len + 1;
}

static int udf_CS0toNLS(struct nls_table *nls, struct ustr *utf_o,
			const struct ustr *ocu_i)
{
	const uint8_t *ocu;
	uint8_t cmp_id, ocu_len;
	int i, len;


	ocu_len = ocu_i->u_len;
	if (ocu_len == 0) {
		memset(utf_o, 0, sizeof(struct ustr));
		return 0;
	}

	cmp_id = ocu_i->u_cmpID;
	if (cmp_id != 8 && cmp_id != 16) {
		memset(utf_o, 0, sizeof(struct ustr));
		pr_err("unknown compression code (%d) stri=%s\n",
		       cmp_id, ocu_i->u_name);
		return 0;
	}

	ocu = ocu_i->u_name;
	utf_o->u_len = 0;
	for (i = 0; (i < ocu_len) && (utf_o->u_len <= (UDF_NAME_LEN - 3));) {
		/* Expand OSTA compressed Unicode to Unicode */
		uint32_t c = ocu[i++];
		if (cmp_id == 16)
			c = (c << 8) | ocu[i++];

		len = nls->uni2char(c, &utf_o->u_name[utf_o->u_len],
				    UDF_NAME_LEN - utf_o->u_len);
		/* Valid character? */
		if (len >= 0)
			utf_o->u_len += len;
		else
			utf_o->u_name[utf_o->u_len++] = '?';
	}
	utf_o->u_cmpID = 8;

	return utf_o->u_len;
}

static int udf_NLStoCS0(struct nls_table *nls, dstring *ocu, struct ustr *uni,
			int length)
{
	int len;
	unsigned i, max_val;
	uint16_t uni_char;
	int u_len;

	memset(ocu, 0, sizeof(dstring) * length);
	ocu[0] = 8;
	max_val = 0xffU;

try_again:
	u_len = 0U;
	for (i = 0U; i < uni->u_len; i++) {
		len = nls->char2uni(&uni->u_name[i], uni->u_len - i, &uni_char);
		if (!len)
			continue;
		/* Invalid character, deal with it */
		if (len < 0) {
=======
#define PLANE_SIZE 0x10000
#define UNICODE_MAX 0x10ffff
#define SURROGATE_MASK 0xfffff800
#define SURROGATE_PAIR 0x0000d800
#define SURROGATE_LOW  0x00000400
#define SURROGATE_CHAR_BITS 10
#define SURROGATE_CHAR_MASK ((1 << SURROGATE_CHAR_BITS) - 1)

#define ILLEGAL_CHAR_MARK	'_'
#define EXT_MARK		'.'
#define CRC_MARK		'#'
#define EXT_SIZE		5
/* Number of chars we need to store generated CRC to make filename unique */
#define CRC_LEN			5

static unicode_t get_utf16_char(const uint8_t *str_i, int str_i_max_len,
				int str_i_idx, int u_ch, unicode_t *ret)
{
	unicode_t c;
	int start_idx = str_i_idx;

	/* Expand OSTA compressed Unicode to Unicode */
	c = str_i[str_i_idx++];
	if (u_ch > 1)
		c = (c << 8) | str_i[str_i_idx++];
	if ((c & SURROGATE_MASK) == SURROGATE_PAIR) {
		unicode_t next;

		/* Trailing surrogate char */
		if (str_i_idx >= str_i_max_len) {
			c = UNICODE_MAX + 1;
			goto out;
		}

		/* Low surrogate must follow the high one... */
		if (c & SURROGATE_LOW) {
			c = UNICODE_MAX + 1;
			goto out;
		}

		WARN_ON_ONCE(u_ch != 2);
		next = str_i[str_i_idx++] << 8;
		next |= str_i[str_i_idx++];
		if ((next & SURROGATE_MASK) != SURROGATE_PAIR ||
		    !(next & SURROGATE_LOW)) {
			c = UNICODE_MAX + 1;
			goto out;
		}

		c = PLANE_SIZE +
		    ((c & SURROGATE_CHAR_MASK) << SURROGATE_CHAR_BITS) +
		    (next & SURROGATE_CHAR_MASK);
	}
out:
	*ret = c;
	return str_i_idx - start_idx;
}


static int udf_name_conv_char(uint8_t *str_o, int str_o_max_len,
			      int *str_o_idx,
			      const uint8_t *str_i, int str_i_max_len,
			      int *str_i_idx,
			      int u_ch, int *needsCRC,
			      int (*conv_f)(wchar_t, unsigned char *, int),
			      int translate)
{
	unicode_t c;
	int illChar = 0;
	int len, gotch = 0;

	while (!gotch && *str_i_idx < str_i_max_len) {
		if (*str_o_idx >= str_o_max_len) {
			*needsCRC = 1;
			return gotch;
		}

		len = get_utf16_char(str_i, str_i_max_len, *str_i_idx, u_ch,
				     &c);
		/* These chars cannot be converted. Replace them. */
		if (c == 0 || c > UNICODE_MAX || (conv_f && c > MAX_WCHAR_T) ||
		    (translate && c == '/')) {
			illChar = 1;
			if (!translate)
				gotch = 1;
		} else if (illChar)
			break;
		else
			gotch = 1;
		*str_i_idx += len;
	}
	if (illChar) {
		*needsCRC = 1;
		c = ILLEGAL_CHAR_MARK;
		gotch = 1;
	}
	if (gotch) {
		if (conv_f) {
			len = conv_f(c, &str_o[*str_o_idx],
				     str_o_max_len - *str_o_idx);
		} else {
			len = utf32_to_utf8(c, &str_o[*str_o_idx],
					    str_o_max_len - *str_o_idx);
			if (len < 0)
				len = -ENAMETOOLONG;
		}
		/* Valid character? */
		if (len >= 0)
			*str_o_idx += len;
		else if (len == -ENAMETOOLONG) {
			*needsCRC = 1;
			gotch = 0;
		} else {
			str_o[(*str_o_idx)++] = ILLEGAL_CHAR_MARK;
			*needsCRC = 1;
		}
	}
	return gotch;
}

static int udf_name_from_CS0(struct super_block *sb,
			     uint8_t *str_o, int str_max_len,
			     const uint8_t *ocu, int ocu_len,
			     int translate)
{
	uint32_t c;
	uint8_t cmp_id;
	int idx, len;
	int u_ch;
	int needsCRC = 0;
	int ext_i_len, ext_max_len;
	int str_o_len = 0;	/* Length of resulting output */
	int ext_o_len = 0;	/* Extension output length */
	int ext_crc_len = 0;	/* Extension output length if used with CRC */
	int i_ext = -1;		/* Extension position in input buffer */
	int o_crc = 0;		/* Rightmost possible output pos for CRC+ext */
	unsigned short valueCRC;
	uint8_t ext[EXT_SIZE * NLS_MAX_CHARSET_SIZE + 1];
	uint8_t crc[CRC_LEN];
	int (*conv_f)(wchar_t, unsigned char *, int);

	if (str_max_len <= 0)
		return 0;

	if (ocu_len == 0) {
		memset(str_o, 0, str_max_len);
		return 0;
	}

	if (UDF_SB(sb)->s_nls_map)
		conv_f = UDF_SB(sb)->s_nls_map->uni2char;
	else
		conv_f = NULL;

	cmp_id = ocu[0];
	if (cmp_id != 8 && cmp_id != 16) {
		memset(str_o, 0, str_max_len);
		pr_err("unknown compression code (%u)\n", cmp_id);
		return -EINVAL;
	}
	u_ch = cmp_id >> 3;

	ocu++;
	ocu_len--;

	if (ocu_len % u_ch) {
		pr_err("incorrect filename length (%d)\n", ocu_len + 1);
		return -EINVAL;
	}

	if (translate) {
		/* Look for extension */
		for (idx = ocu_len - u_ch, ext_i_len = 0;
		     (idx >= 0) && (ext_i_len < EXT_SIZE);
		     idx -= u_ch, ext_i_len++) {
			c = ocu[idx];
			if (u_ch > 1)
				c = (c << 8) | ocu[idx + 1];

			if (c == EXT_MARK) {
				if (ext_i_len)
					i_ext = idx;
				break;
			}
		}
		if (i_ext >= 0) {
			/* Convert extension */
			ext_max_len = min_t(int, sizeof(ext), str_max_len);
			ext[ext_o_len++] = EXT_MARK;
			idx = i_ext + u_ch;
			while (udf_name_conv_char(ext, ext_max_len, &ext_o_len,
						  ocu, ocu_len, &idx,
						  u_ch, &needsCRC,
						  conv_f, translate)) {
				if ((ext_o_len + CRC_LEN) < str_max_len)
					ext_crc_len = ext_o_len;
			}
		}
	}

	idx = 0;
	while (1) {
		if (translate && (idx == i_ext)) {
			if (str_o_len > (str_max_len - ext_o_len))
				needsCRC = 1;
			break;
		}

		if (!udf_name_conv_char(str_o, str_max_len, &str_o_len,
					ocu, ocu_len, &idx,
					u_ch, &needsCRC, conv_f, translate))
			break;

		if (translate &&
		    (str_o_len <= (str_max_len - ext_o_len - CRC_LEN)))
			o_crc = str_o_len;
	}

	if (translate) {
		if (str_o_len > 0 && str_o_len <= 2 && str_o[0] == '.' &&
		    (str_o_len == 1 || str_o[1] == '.'))
			needsCRC = 1;
		if (needsCRC) {
			str_o_len = o_crc;
			valueCRC = crc_itu_t(0, ocu, ocu_len);
			crc[0] = CRC_MARK;
			crc[1] = hex_asc_upper_hi(valueCRC >> 8);
			crc[2] = hex_asc_upper_lo(valueCRC >> 8);
			crc[3] = hex_asc_upper_hi(valueCRC);
			crc[4] = hex_asc_upper_lo(valueCRC);
			len = min_t(int, CRC_LEN, str_max_len - str_o_len);
			memcpy(&str_o[str_o_len], crc, len);
			str_o_len += len;
			ext_o_len = ext_crc_len;
		}
		if (ext_o_len > 0) {
			memcpy(&str_o[str_o_len], ext, ext_o_len);
			str_o_len += ext_o_len;
		}
	}

	return str_o_len;
}

static int udf_name_to_CS0(struct super_block *sb,
			   uint8_t *ocu, int ocu_max_len,
			   const uint8_t *str_i, int str_len)
{
	int i, len;
	unsigned int max_val;
	int u_len, u_ch;
	unicode_t uni_char;
	int (*conv_f)(const unsigned char *, int, wchar_t *);

	if (ocu_max_len <= 0)
		return 0;

	if (UDF_SB(sb)->s_nls_map)
		conv_f = UDF_SB(sb)->s_nls_map->char2uni;
	else
		conv_f = NULL;

	memset(ocu, 0, ocu_max_len);
	ocu[0] = 8;
	max_val = 0xff;
	u_ch = 1;

try_again:
	u_len = 1;
	for (i = 0; i < str_len; i += len) {
		/* Name didn't fit? */
		if (u_len + u_ch > ocu_max_len)
			return 0;
		if (conv_f) {
			wchar_t wchar;

			len = conv_f(&str_i[i], str_len - i, &wchar);
			if (len > 0)
				uni_char = wchar;
		} else {
			len = utf8_to_utf32(&str_i[i], str_len - i,
					    &uni_char);
		}
		/* Invalid character, deal with it */
		if (len <= 0 || uni_char > UNICODE_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			len = 1;
			uni_char = '?';
		}

		if (uni_char > max_val) {
<<<<<<< HEAD
			max_val = 0xffffU;
			ocu[0] = (uint8_t)0x10U;
			goto try_again;
		}

		if (max_val == 0xffffU)
			ocu[++u_len] = (uint8_t)(uni_char >> 8);
		ocu[++u_len] = (uint8_t)(uni_char & 0xffU);
		i += len - 1;
	}

	ocu[length - 1] = (uint8_t)u_len + 1;
	return u_len + 1;
}

int udf_get_filename(struct super_block *sb, uint8_t *sname, int slen,
		     uint8_t *dname, int dlen)
{
	struct ustr *filename, *unifilename;
	int len = 0;

	filename = kmalloc(sizeof(struct ustr), GFP_NOFS);
	if (!filename)
		return 0;

	unifilename = kmalloc(sizeof(struct ustr), GFP_NOFS);
	if (!unifilename)
		goto out1;

	if (udf_build_ustr_exact(unifilename, sname, slen))
		goto out2;

	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8)) {
		if (!udf_CS0toUTF8(filename, unifilename)) {
			udf_debug("Failed in udf_get_filename: sname = %s\n",
				  sname);
			goto out2;
		}
	} else if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP)) {
		if (!udf_CS0toNLS(UDF_SB(sb)->s_nls_map, filename,
				  unifilename)) {
			udf_debug("Failed in udf_get_filename: sname = %s\n",
				  sname);
			goto out2;
		}
	} else
		goto out2;

	len = udf_translate_to_linux(dname, dlen,
				     filename->u_name, filename->u_len,
				     unifilename->u_name, unifilename->u_len);
out2:
	kfree(unifilename);
out1:
	kfree(filename);
	return len;
}

int udf_put_filename(struct super_block *sb, const uint8_t *sname,
		     uint8_t *dname, int flen)
{
	struct ustr unifilename;
	int namelen;

	if (!udf_char_to_ustr(&unifilename, sname, flen))
		return 0;

	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8)) {
		namelen = udf_UTF8toCS0(dname, &unifilename, UDF_NAME_LEN);
		if (!namelen)
			return 0;
	} else if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP)) {
		namelen = udf_NLStoCS0(UDF_SB(sb)->s_nls_map, dname,
					&unifilename, UDF_NAME_LEN);
		if (!namelen)
			return 0;
	} else
		return 0;

	return namelen;
}

#define ILLEGAL_CHAR_MARK	'_'
#define EXT_MARK		'.'
#define CRC_MARK		'#'
#define EXT_SIZE 		5
/* Number of chars we need to store generated CRC to make filename unique */
#define CRC_LEN			5

static int udf_translate_to_linux(uint8_t *newName, int newLen,
				  uint8_t *udfName, int udfLen,
				  uint8_t *fidName, int fidNameLen)
{
	int index, newIndex = 0, needsCRC = 0;
	int extIndex = 0, newExtIndex = 0, hasExt = 0;
	unsigned short valueCRC;
	uint8_t curr;
	const uint8_t hexChar[] = "0123456789ABCDEF";

	if (udfName[0] == '.' &&
	    (udfLen == 1 || (udfLen == 2 && udfName[1] == '.'))) {
		needsCRC = 1;
		newIndex = udfLen;
		memcpy(newName, udfName, udfLen);
	} else {
		for (index = 0; index < udfLen; index++) {
			curr = udfName[index];
			if (curr == '/' || curr == 0) {
				needsCRC = 1;
				curr = ILLEGAL_CHAR_MARK;
				while (index + 1 < udfLen &&
						(udfName[index + 1] == '/' ||
						 udfName[index + 1] == 0))
					index++;
			}
			if (curr == EXT_MARK &&
					(udfLen - index - 1) <= EXT_SIZE) {
				if (udfLen == index + 1)
					hasExt = 0;
				else {
					hasExt = 1;
					extIndex = index;
					newExtIndex = newIndex;
				}
			}
			if (newIndex < newLen)
				newName[newIndex++] = curr;
			else
				needsCRC = 1;
		}
	}
	if (needsCRC) {
		uint8_t ext[EXT_SIZE];
		int localExtIndex = 0;

		if (hasExt) {
			int maxFilenameLen;
			for (index = 0;
			     index < EXT_SIZE && extIndex + index + 1 < udfLen;
			     index++) {
				curr = udfName[extIndex + index + 1];

				if (curr == '/' || curr == 0) {
					needsCRC = 1;
					curr = ILLEGAL_CHAR_MARK;
					while (extIndex + index + 2 < udfLen &&
					      (index + 1 < EXT_SIZE &&
						(udfName[extIndex + index + 2] == '/' ||
						 udfName[extIndex + index + 2] == 0)))
						index++;
				}
				ext[localExtIndex++] = curr;
			}
			maxFilenameLen = newLen - CRC_LEN - localExtIndex;
			if (newIndex > maxFilenameLen)
				newIndex = maxFilenameLen;
			else
				newIndex = newExtIndex;
		} else if (newIndex > newLen - CRC_LEN)
			newIndex = newLen - CRC_LEN;
		newName[newIndex++] = CRC_MARK;
		valueCRC = crc_itu_t(0, fidName, fidNameLen);
		newName[newIndex++] = hexChar[(valueCRC & 0xf000) >> 12];
		newName[newIndex++] = hexChar[(valueCRC & 0x0f00) >> 8];
		newName[newIndex++] = hexChar[(valueCRC & 0x00f0) >> 4];
		newName[newIndex++] = hexChar[(valueCRC & 0x000f)];

		if (hasExt) {
			newName[newIndex++] = EXT_MARK;
			for (index = 0; index < localExtIndex; index++)
				newName[newIndex++] = ext[index];
		}
	}

	return newIndex;
}
=======
			unicode_t c;

			if (max_val == 0xff) {
				max_val = 0xffff;
				ocu[0] = 0x10;
				u_ch = 2;
				goto try_again;
			}
			/*
			 * Use UTF-16 encoding for chars outside we
			 * cannot encode directly.
			 */
			if (u_len + 2 * u_ch > ocu_max_len)
				return 0;

			uni_char -= PLANE_SIZE;
			c = SURROGATE_PAIR |
			    ((uni_char >> SURROGATE_CHAR_BITS) &
			     SURROGATE_CHAR_MASK);
			ocu[u_len++] = (uint8_t)(c >> 8);
			ocu[u_len++] = (uint8_t)(c & 0xff);
			uni_char = SURROGATE_PAIR | SURROGATE_LOW |
					(uni_char & SURROGATE_CHAR_MASK);
		}

		if (max_val == 0xffff)
			ocu[u_len++] = (uint8_t)(uni_char >> 8);
		ocu[u_len++] = (uint8_t)(uni_char & 0xff);
	}

	return u_len;
}

/*
 * Convert CS0 dstring to output charset. Warning: This function may truncate
 * input string if it is too long as it is used for informational strings only
 * and it is better to truncate the string than to refuse mounting a media.
 */
int udf_dstrCS0toChar(struct super_block *sb, uint8_t *utf_o, int o_len,
		      const uint8_t *ocu_i, int i_len)
{
	int s_len = 0;

	if (i_len > 0) {
		s_len = ocu_i[i_len - 1];
		if (s_len >= i_len) {
			pr_warn("incorrect dstring lengths (%d/%d),"
				" truncating\n", s_len, i_len);
			s_len = i_len - 1;
			/* 2-byte encoding? Need to round properly... */
			if (ocu_i[0] == 16)
				s_len -= (s_len - 1) & 2;
		}
	}

	return udf_name_from_CS0(sb, utf_o, o_len, ocu_i, s_len, 0);
}

int udf_get_filename(struct super_block *sb, const uint8_t *sname, int slen,
		     uint8_t *dname, int dlen)
{
	int ret;

	if (!slen)
		return -EIO;

	if (dlen <= 0)
		return 0;

	ret = udf_name_from_CS0(sb, dname, dlen, sname, slen, 1);
	/* Zero length filename isn't valid... */
	if (ret == 0)
		ret = -EINVAL;
	return ret;
}

int udf_put_filename(struct super_block *sb, const uint8_t *sname, int slen,
		     uint8_t *dname, int dlen)
{
	return udf_name_to_CS0(sb, dname, dlen, sname, slen);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
