<<<<<<< HEAD
#ifndef _SCREEN_INFO_H
#define _SCREEN_INFO_H

#include <linux/types.h>

/*
 * These are set up by the setup-routine at boot-time:
 */

struct screen_info {
	__u8  orig_x;		/* 0x00 */
	__u8  orig_y;		/* 0x01 */
	__u16 ext_mem_k;	/* 0x02 */
	__u16 orig_video_page;	/* 0x04 */
	__u8  orig_video_mode;	/* 0x06 */
	__u8  orig_video_cols;	/* 0x07 */
	__u8  flags;		/* 0x08 */
	__u8  unused2;		/* 0x09 */
	__u16 orig_video_ega_bx;/* 0x0a */
	__u16 unused3;		/* 0x0c */
	__u8  orig_video_lines;	/* 0x0e */
	__u8  orig_video_isVGA;	/* 0x0f */
	__u16 orig_video_points;/* 0x10 */

	/* VESA graphic mode -- linear frame buffer */
	__u16 lfb_width;	/* 0x12 */
	__u16 lfb_height;	/* 0x14 */
	__u16 lfb_depth;	/* 0x16 */
	__u32 lfb_base;		/* 0x18 */
	__u32 lfb_size;		/* 0x1c */
	__u16 cl_magic, cl_offset; /* 0x20 */
	__u16 lfb_linelength;	/* 0x24 */
	__u8  red_size;		/* 0x26 */
	__u8  red_pos;		/* 0x27 */
	__u8  green_size;	/* 0x28 */
	__u8  green_pos;	/* 0x29 */
	__u8  blue_size;	/* 0x2a */
	__u8  blue_pos;		/* 0x2b */
	__u8  rsvd_size;	/* 0x2c */
	__u8  rsvd_pos;		/* 0x2d */
	__u16 vesapm_seg;	/* 0x2e */
	__u16 vesapm_off;	/* 0x30 */
	__u16 pages;		/* 0x32 */
	__u16 vesa_attributes;	/* 0x34 */
	__u32 capabilities;     /* 0x36 */
	__u8  _reserved[6];	/* 0x3a */
} __attribute__((packed));

#define VIDEO_TYPE_MDA		0x10	/* Monochrome Text Display	*/
#define VIDEO_TYPE_CGA		0x11	/* CGA Display 			*/
#define VIDEO_TYPE_EGAM		0x20	/* EGA/VGA in Monochrome Mode	*/
#define VIDEO_TYPE_EGAC		0x21	/* EGA in Color Mode		*/
#define VIDEO_TYPE_VGAC		0x22	/* VGA+ in Color Mode		*/
#define VIDEO_TYPE_VLFB		0x23	/* VESA VGA in graphic mode	*/

#define VIDEO_TYPE_PICA_S3	0x30	/* ACER PICA-61 local S3 video	*/
#define VIDEO_TYPE_MIPS_G364	0x31    /* MIPS Magnum 4000 G364 video  */
#define VIDEO_TYPE_SGI          0x33    /* Various SGI graphics hardware */

#define VIDEO_TYPE_TGAC		0x40	/* DEC TGA */

#define VIDEO_TYPE_SUN          0x50    /* Sun frame buffer. */
#define VIDEO_TYPE_SUNPCI       0x51    /* Sun PCI based frame buffer. */

#define VIDEO_TYPE_PMAC		0x60	/* PowerMacintosh frame buffer. */

#define VIDEO_TYPE_EFI		0x70	/* EFI graphic mode		*/

#define VIDEO_FLAGS_NOCURSOR	(1 << 0) /* The video mode has no cursor set */

#ifdef __KERNEL__
extern struct screen_info screen_info;

#define ORIG_X			(screen_info.orig_x)
#define ORIG_Y			(screen_info.orig_y)
#define ORIG_VIDEO_MODE		(screen_info.orig_video_mode)
#define ORIG_VIDEO_COLS 	(screen_info.orig_video_cols)
#define ORIG_VIDEO_EGA_BX	(screen_info.orig_video_ega_bx)
#define ORIG_VIDEO_LINES	(screen_info.orig_video_lines)
#define ORIG_VIDEO_ISVGA	(screen_info.orig_video_isVGA)
#define ORIG_VIDEO_POINTS       (screen_info.orig_video_points)
#endif /* __KERNEL__ */

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SCREEN_INFO_H
#define _SCREEN_INFO_H

#include <uapi/linux/screen_info.h>

#include <linux/bits.h>

/**
 * SCREEN_INFO_MAX_RESOURCES - maximum number of resources per screen_info
 */
#define SCREEN_INFO_MAX_RESOURCES	3

struct pci_dev;
struct resource;

static inline bool __screen_info_has_lfb(unsigned int type)
{
	return (type == VIDEO_TYPE_VLFB) || (type == VIDEO_TYPE_EFI);
}

static inline u64 __screen_info_lfb_base(const struct screen_info *si)
{
	u64 lfb_base = si->lfb_base;

	if (si->capabilities & VIDEO_CAPABILITY_64BIT_BASE)
		lfb_base |= (u64)si->ext_lfb_base << 32;

	return lfb_base;
}

static inline void __screen_info_set_lfb_base(struct screen_info *si, u64 lfb_base)
{
	si->lfb_base = lfb_base & GENMASK_ULL(31, 0);
	si->ext_lfb_base = (lfb_base & GENMASK_ULL(63, 32)) >> 32;

	if (si->ext_lfb_base)
		si->capabilities |= VIDEO_CAPABILITY_64BIT_BASE;
	else
		si->capabilities &= ~VIDEO_CAPABILITY_64BIT_BASE;
}

static inline u64 __screen_info_lfb_size(const struct screen_info *si, unsigned int type)
{
	u64 lfb_size = si->lfb_size;

	if (type == VIDEO_TYPE_VLFB)
		lfb_size <<= 16;
	return lfb_size;
}

static inline unsigned int __screen_info_video_type(unsigned int type)
{
	switch (type) {
	case VIDEO_TYPE_MDA:
	case VIDEO_TYPE_CGA:
	case VIDEO_TYPE_EGAM:
	case VIDEO_TYPE_EGAC:
	case VIDEO_TYPE_VGAC:
	case VIDEO_TYPE_VLFB:
	case VIDEO_TYPE_PICA_S3:
	case VIDEO_TYPE_MIPS_G364:
	case VIDEO_TYPE_SGI:
	case VIDEO_TYPE_TGAC:
	case VIDEO_TYPE_SUN:
	case VIDEO_TYPE_SUNPCI:
	case VIDEO_TYPE_PMAC:
	case VIDEO_TYPE_EFI:
		return type;
	default:
		return 0;
	}
}

/**
 * screen_info_video_type() - Decodes the video type from struct screen_info
 * @si: an instance of struct screen_info
 *
 * Returns:
 * A VIDEO_TYPE_ constant representing si's type of video display, or 0 otherwise.
 */
static inline unsigned int screen_info_video_type(const struct screen_info *si)
{
	unsigned int type;

	// check if display output is on
	if (!si->orig_video_isVGA)
		return 0;

	// check for a known VIDEO_TYPE_ constant
	type = __screen_info_video_type(si->orig_video_isVGA);
	if (type)
		return si->orig_video_isVGA;

	// check if text mode has been initialized
	if (!si->orig_video_lines || !si->orig_video_cols)
		return 0;

	// 80x25 text, mono
	if (si->orig_video_mode == 0x07) {
		if ((si->orig_video_ega_bx & 0xff) != 0x10)
			return VIDEO_TYPE_EGAM;
		else
			return VIDEO_TYPE_MDA;
	}

	// EGA/VGA, 16 colors
	if ((si->orig_video_ega_bx & 0xff) != 0x10) {
		if (si->orig_video_isVGA)
			return VIDEO_TYPE_VGAC;
		else
			return VIDEO_TYPE_EGAC;
	}

	// the rest...
	return VIDEO_TYPE_CGA;
}

ssize_t screen_info_resources(const struct screen_info *si, struct resource *r, size_t num);

#if defined(CONFIG_PCI)
void screen_info_apply_fixups(void);
struct pci_dev *screen_info_pci_dev(const struct screen_info *si);
#else
static inline void screen_info_apply_fixups(void)
{ }
static inline struct pci_dev *screen_info_pci_dev(const struct screen_info *si)
{
	return NULL;
}
#endif

extern struct screen_info screen_info;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _SCREEN_INFO_H */
