<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Compact binary representation of ihex records. Some devices need their
 * firmware loaded in strange orders rather than a single big blob, but
 * actually parsing ihex-as-text within the kernel seems silly. Thus,...
 */

#ifndef __LINUX_IHEX_H__
#define __LINUX_IHEX_H__

#include <linux/types.h>
#include <linux/firmware.h>
#include <linux/device.h>

/* Intel HEX files actually limit the length to 256 bytes, but we have
   drivers which would benefit from using separate records which are
   longer than that, so we extend to 16 bits of length */
struct ihex_binrec {
	__be32 addr;
	__be16 len;
<<<<<<< HEAD
	uint8_t data[0];
} __attribute__((packed));

/* Find the next record, taking into account the 4-byte alignment */
static inline const struct ihex_binrec *
ihex_next_binrec(const struct ihex_binrec *rec)
{
	int next = ((be16_to_cpu(rec->len) + 5) & ~3) - 2;
	rec = (void *)&rec->data[next];
=======
	uint8_t data[];
} __attribute__((packed));

static inline uint16_t ihex_binrec_size(const struct ihex_binrec *p)
{
	return be16_to_cpu(p->len) + sizeof(*p);
}

/* Find the next record, taking into account the 4-byte alignment */
static inline const struct ihex_binrec *
__ihex_next_binrec(const struct ihex_binrec *rec)
{
	const void *p = rec;

	return p + ALIGN(ihex_binrec_size(rec), 4);
}

static inline const struct ihex_binrec *
ihex_next_binrec(const struct ihex_binrec *rec)
{
	rec = __ihex_next_binrec(rec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return be16_to_cpu(rec->len) ? rec : NULL;
}

/* Check that ihex_next_binrec() won't take us off the end of the image... */
static inline int ihex_validate_fw(const struct firmware *fw)
{
<<<<<<< HEAD
	const struct ihex_binrec *rec;
	size_t ofs = 0;

	while (ofs <= fw->size - sizeof(*rec)) {
		rec = (void *)&fw->data[ofs];

		/* Zero length marks end of records */
		if (!be16_to_cpu(rec->len))
			return 0;

		/* Point to next record... */
		ofs += (sizeof(*rec) + be16_to_cpu(rec->len) + 3) & ~3;
=======
	const struct ihex_binrec *end, *rec;

	rec = (const void *)fw->data;
	end = (const void *)&fw->data[fw->size - sizeof(*end)];

	for (; rec <= end; rec = __ihex_next_binrec(rec)) {
		/* Zero length marks end of records */
		if (rec == end && !be16_to_cpu(rec->len))
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return -EINVAL;
}

/* Request firmware and validate it so that we can trust we won't
 * run off the end while reading records... */
static inline int request_ihex_firmware(const struct firmware **fw,
					const char *fw_name,
					struct device *dev)
{
	const struct firmware *lfw;
	int ret;

	ret = request_firmware(&lfw, fw_name, dev);
	if (ret)
		return ret;
	ret = ihex_validate_fw(lfw);
	if (ret) {
		dev_err(dev, "Firmware \"%s\" not valid IHEX records\n",
			fw_name);
		release_firmware(lfw);
		return ret;
	}
	*fw = lfw;
	return 0;
}
#endif /* __LINUX_IHEX_H__ */
