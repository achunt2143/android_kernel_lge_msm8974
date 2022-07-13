<<<<<<< HEAD
/*
 * Copyright (c) International Business Machines Corp., 2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (c) International Business Machines Corp., 2006
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: Artem Bityutskiy (Битюцкий Артём)
 */

#ifndef __UBI_DEBUG_H__
#define __UBI_DEBUG_H__

<<<<<<< HEAD
#ifdef CONFIG_MTD_UBI_DEBUG
=======
void ubi_dump_flash(struct ubi_device *ubi, int pnum, int offset, int len);
void ubi_dump_ec_hdr(const struct ubi_ec_hdr *ec_hdr);
void ubi_dump_vid_hdr(const struct ubi_vid_hdr *vid_hdr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/random.h>

#define ubi_assert(expr)  do {                                               \
	if (unlikely(!(expr))) {                                             \
<<<<<<< HEAD
		printk(KERN_CRIT "UBI assert failed in %s at %u (pid %d)\n", \
		       __func__, __LINE__, current->pid);                    \
		ubi_dbg_dump_stack();                                        \
	}                                                                    \
} while (0)

#define dbg_err(fmt, ...) ubi_err(fmt, ##__VA_ARGS__)

#define ubi_dbg_dump_stack() dump_stack()

#define ubi_dbg_print_hex_dump(l, ps, pt, r, g, b, len, a)  \
		print_hex_dump(l, ps, pt, r, g, b, len, a)

#define ubi_dbg_msg(type, fmt, ...) \
	pr_debug("UBI DBG " type ": " fmt "\n", ##__VA_ARGS__)

/* Just a debugging messages not related to any specific UBI subsystem */
#define dbg_msg(fmt, ...)                                    \
	printk(KERN_DEBUG "UBI DBG (pid %d): %s: " fmt "\n", \
	       current->pid, __func__, ##__VA_ARGS__)
=======
		pr_crit("UBI assert failed in %s at %u (pid %d)\n",          \
		       __func__, __LINE__, current->pid);                    \
		dump_stack();                                                \
	}                                                                    \
} while (0)

#define ubi_dbg_print_hex_dump(l, ps, pt, r, g, b, len, a)                   \
		print_hex_dump(l, ps, pt, r, g, b, len, a)

#define ubi_dbg_msg(type, fmt, ...) \
	pr_debug("UBI DBG " type " (pid %d): " fmt "\n", current->pid,       \
		 ##__VA_ARGS__)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* General debugging messages */
#define dbg_gen(fmt, ...) ubi_dbg_msg("gen", fmt, ##__VA_ARGS__)
/* Messages from the eraseblock association sub-system */
#define dbg_eba(fmt, ...) ubi_dbg_msg("eba", fmt, ##__VA_ARGS__)
/* Messages from the wear-leveling sub-system */
#define dbg_wl(fmt, ...)  ubi_dbg_msg("wl", fmt, ##__VA_ARGS__)
/* Messages from the input/output sub-system */
#define dbg_io(fmt, ...)  ubi_dbg_msg("io", fmt, ##__VA_ARGS__)
/* Initialization and build messages */
#define dbg_bld(fmt, ...) ubi_dbg_msg("bld", fmt, ##__VA_ARGS__)

<<<<<<< HEAD
void ubi_dbg_dump_ec_hdr(const struct ubi_ec_hdr *ec_hdr);
void ubi_dbg_dump_vid_hdr(const struct ubi_vid_hdr *vid_hdr);
void ubi_dbg_dump_vol_info(const struct ubi_volume *vol);
void ubi_dbg_dump_vtbl_record(const struct ubi_vtbl_record *r, int idx);
void ubi_dbg_dump_sv(const struct ubi_scan_volume *sv);
void ubi_dbg_dump_seb(const struct ubi_scan_leb *seb, int type);
void ubi_dbg_dump_mkvol_req(const struct ubi_mkvol_req *req);
void ubi_dbg_dump_flash(struct ubi_device *ubi, int pnum, int offset, int len);
int ubi_dbg_check_all_ff(struct ubi_device *ubi, int pnum, int offset, int len);
int ubi_dbg_check_write(struct ubi_device *ubi, const void *buf, int pnum,
			int offset, int len);
int ubi_debugging_init_dev(struct ubi_device *ubi);
void ubi_debugging_exit_dev(struct ubi_device *ubi);
=======
void ubi_dump_vol_info(const struct ubi_volume *vol);
void ubi_dump_vtbl_record(const struct ubi_vtbl_record *r, int idx);
void ubi_dump_av(const struct ubi_ainf_volume *av);
void ubi_dump_aeb(const struct ubi_ainf_peb *aeb, int type);
void ubi_dump_mkvol_req(const struct ubi_mkvol_req *req);
int ubi_self_check_all_ff(struct ubi_device *ubi, int pnum, int offset,
			  int len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int ubi_debugfs_init(void);
void ubi_debugfs_exit(void);
int ubi_debugfs_init_dev(struct ubi_device *ubi);
void ubi_debugfs_exit_dev(struct ubi_device *ubi);

<<<<<<< HEAD
/*
 * The UBI debugfs directory name pattern and maximum name length (3 for "ubi"
 * + 2 for the number plus 1 for the trailing zero byte.
 */
#define UBI_DFS_DIR_NAME "ubi%d"
#define UBI_DFS_DIR_LEN  (3 + 2 + 1)

/**
 * struct ubi_debug_info - debugging information for an UBI device.
 *
 * @chk_gen: if UBI general extra checks are enabled
 * @chk_io: if UBI I/O extra checks are enabled
 * @disable_bgt: disable the background task for testing purposes
 * @emulate_bitflips: emulate bit-flips for testing purposes
 * @emulate_io_failures: emulate write/erase failures for testing purposes
 * @dfs_dir_name: name of debugfs directory containing files of this UBI device
 * @dfs_dir: direntry object of the UBI device debugfs directory
 * @dfs_chk_gen: debugfs knob to enable UBI general extra checks
 * @dfs_chk_io: debugfs knob to enable UBI I/O extra checks
 * @dfs_disable_bgt: debugfs knob to disable the background task
 * @dfs_emulate_bitflips: debugfs knob to emulate bit-flips
 * @dfs_emulate_io_failures: debugfs knob to emulate write/erase failures
 */
struct ubi_debug_info {
	unsigned int chk_gen:1;
	unsigned int chk_io:1;
	unsigned int disable_bgt:1;
	unsigned int emulate_bitflips:1;
	unsigned int emulate_io_failures:1;
	char dfs_dir_name[UBI_DFS_DIR_LEN + 1];
	struct dentry *dfs_dir;
	struct dentry *dfs_chk_gen;
	struct dentry *dfs_chk_io;
	struct dentry *dfs_disable_bgt;
	struct dentry *dfs_emulate_bitflips;
	struct dentry *dfs_emulate_io_failures;
};
=======
/**
 * The following function is a legacy implementation of UBI fault-injection
 * hook. When using more powerful fault injection capabilities, the legacy
 * fault injection interface should be retained.
 */
int ubi_dbg_power_cut(struct ubi_device *ubi, int caller);

static inline int ubi_dbg_bitflip(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_bitflips)
		return !get_random_u32_below(200);
	return 0;
}

static inline int ubi_dbg_write_failure(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_io_failures)
		return !get_random_u32_below(500);
	return 0;
}

static inline int ubi_dbg_erase_failure(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_io_failures)
		return !get_random_u32_below(400);
	return 0;
}

/**
 * MASK_XXX: Mask for emulate_failures in ubi_debug_info.The mask is used to
 * precisely control the type and process of fault injection.
 */
/* Emulate a power cut when writing EC/VID header */
#define MASK_POWER_CUT_EC			(1 << 0)
#define MASK_POWER_CUT_VID			(1 << 1)
/* Emulate a power cut when writing data*/
#define MASK_POWER_CUT_DATA			(1 << 2)
/* Emulate bit-flips */
#define MASK_BITFLIPS				(1 << 3)
/* Emulate ecc error */
#define MASK_ECCERR				(1 << 4)
/* Emulates -EIO during data read */
#define MASK_READ_FAILURE			(1 << 5)
#define MASK_READ_FAILURE_EC			(1 << 6)
#define MASK_READ_FAILURE_VID			(1 << 7)
/* Emulates -EIO during data write */
#define MASK_WRITE_FAILURE			(1 << 8)
/* Emulates -EIO during erase a PEB*/
#define MASK_ERASE_FAILURE			(1 << 9)
/* Return UBI_IO_FF when reading EC/VID header */
#define MASK_IO_FF_EC				(1 << 10)
#define MASK_IO_FF_VID				(1 << 11)
/* Return UBI_IO_FF_BITFLIPS when reading EC/VID header */
#define MASK_IO_FF_BITFLIPS_EC			(1 << 12)
#define MASK_IO_FF_BITFLIPS_VID			(1 << 13)
/* Return UBI_IO_BAD_HDR when reading EC/VID header */
#define MASK_BAD_HDR_EC				(1 << 14)
#define MASK_BAD_HDR_VID			(1 << 15)
/* Return UBI_IO_BAD_HDR_EBADMSG when reading EC/VID header */
#define MASK_BAD_HDR_EBADMSG_EC			(1 << 16)
#define MASK_BAD_HDR_EBADMSG_VID		(1 << 17)

#ifdef CONFIG_MTD_UBI_FAULT_INJECTION

extern bool should_fail_eccerr(void);
extern bool should_fail_bitflips(void);
extern bool should_fail_read_failure(void);
extern bool should_fail_write_failure(void);
extern bool should_fail_erase_failure(void);
extern bool should_fail_power_cut(void);
extern bool should_fail_io_ff(void);
extern bool should_fail_io_ff_bitflips(void);
extern bool should_fail_bad_hdr(void);
extern bool should_fail_bad_hdr_ebadmsg(void);

static inline bool ubi_dbg_fail_bitflip(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_failures & MASK_BITFLIPS)
		return should_fail_bitflips();
	return false;
}

static inline bool ubi_dbg_fail_write(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_failures & MASK_WRITE_FAILURE)
		return should_fail_write_failure();
	return false;
}

static inline bool ubi_dbg_fail_erase(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_failures & MASK_ERASE_FAILURE)
		return should_fail_erase_failure();
	return false;
}

static inline bool ubi_dbg_fail_power_cut(const struct ubi_device *ubi,
					  unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_power_cut();
	return false;
}

static inline bool ubi_dbg_fail_read(const struct ubi_device *ubi,
				     unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_read_failure();
	return false;
}

static inline bool ubi_dbg_fail_eccerr(const struct ubi_device *ubi)
{
	if (ubi->dbg.emulate_failures & MASK_ECCERR)
		return should_fail_eccerr();
	return false;
}

static inline bool ubi_dbg_fail_ff(const struct ubi_device *ubi,
				   unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_io_ff();
	return false;
}

static inline bool ubi_dbg_fail_ff_bitflips(const struct ubi_device *ubi,
					    unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_io_ff_bitflips();
	return false;
}

static inline bool ubi_dbg_fail_bad_hdr(const struct ubi_device *ubi,
					 unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_bad_hdr();
	return false;
}

static inline bool ubi_dbg_fail_bad_hdr_ebadmsg(const struct ubi_device *ubi,
						 unsigned int caller)
{
	if (ubi->dbg.emulate_failures & caller)
		return should_fail_bad_hdr_ebadmsg();
	return false;
}
#else /* CONFIG_MTD_UBI_FAULT_INJECTION */

#define ubi_dbg_fail_bitflip(u)             false
#define ubi_dbg_fail_write(u)               false
#define ubi_dbg_fail_erase(u)               false
#define ubi_dbg_fail_power_cut(u, c)        false
#define ubi_dbg_fail_read(u, c)             false
#define ubi_dbg_fail_eccerr(u)              false
#define ubi_dbg_fail_ff(u, c)               false
#define ubi_dbg_fail_ff_bitflips(u, v)      false
#define ubi_dbg_fail_bad_hdr(u, c)          false
#define ubi_dbg_fail_bad_hdr_ebadmsg(u, c)  false

#endif

/**
 * ubi_dbg_is_power_cut - if it is time to emulate power cut.
 * @ubi: UBI device description object
 *
 * Returns true if power cut should be emulated, otherwise returns false.
 */
static inline bool ubi_dbg_is_power_cut(struct ubi_device *ubi,
					unsigned int caller)
{
	if (ubi_dbg_power_cut(ubi, caller))
		return true;
	return ubi_dbg_fail_power_cut(ubi, caller);
}

/**
 * ubi_dbg_is_bitflip - if it is time to emulate a bit-flip.
 * @ubi: UBI device description object
 *
 * Returns true if a bit-flip should be emulated, otherwise returns false.
 */
static inline bool ubi_dbg_is_bitflip(const struct ubi_device *ubi)
{
	if (ubi_dbg_bitflip(ubi))
		return true;
	return ubi_dbg_fail_bitflip(ubi);
}

/**
 * ubi_dbg_is_write_failure - if it is time to emulate a write failure.
 * @ubi: UBI device description object
 *
 * Returns true if a write failure should be emulated, otherwise returns
 * false.
 */
static inline bool ubi_dbg_is_write_failure(const struct ubi_device *ubi)
{
	if (ubi_dbg_write_failure(ubi))
		return true;
	return ubi_dbg_fail_write(ubi);
}

/**
 * ubi_dbg_is_erase_failure - if its time to emulate an erase failure.
 * @ubi: UBI device description object
 *
 * Returns true if an erase failure should be emulated, otherwise returns
 * false.
 */
static inline bool ubi_dbg_is_erase_failure(const struct ubi_device *ubi)
{
	if (ubi_dbg_erase_failure(ubi))
		return true;
	return ubi_dbg_fail_erase(ubi);
}

/**
 * ubi_dbg_is_eccerr - if it is time to emulate ECC error.
 * @ubi: UBI device description object
 *
 * Returns true if a ECC error should be emulated, otherwise returns false.
 */
static inline bool ubi_dbg_is_eccerr(const struct ubi_device *ubi)
{
	return ubi_dbg_fail_eccerr(ubi);
}

/**
 * ubi_dbg_is_read_failure - if it is time to emulate a read failure.
 * @ubi: UBI device description object
 *
 * Returns true if a read failure should be emulated, otherwise returns
 * false.
 */
static inline bool ubi_dbg_is_read_failure(const struct ubi_device *ubi,
					   unsigned int caller)
{
	return ubi_dbg_fail_read(ubi, caller);
}

/**
 * ubi_dbg_is_ff - if it is time to emulate that read region is only 0xFF.
 * @ubi: UBI device description object
 *
 * Returns true if read region should be emulated 0xFF, otherwise
 * returns false.
 */
static inline bool ubi_dbg_is_ff(const struct ubi_device *ubi,
				 unsigned int caller)
{
	return ubi_dbg_fail_ff(ubi, caller);
}

/**
 * ubi_dbg_is_ff_bitflips - if it is time to emulate that read region is only 0xFF
 * with error reported by the MTD driver
 *
 * @ubi: UBI device description object
 *
 * Returns true if read region should be emulated 0xFF and error
 * reported by the MTD driver, otherwise returns false.
 */
static inline bool ubi_dbg_is_ff_bitflips(const struct ubi_device *ubi,
					  unsigned int caller)
{
	return ubi_dbg_fail_ff_bitflips(ubi, caller);
}

/**
 * ubi_dbg_is_bad_hdr - if it is time to emulate a bad header
 * @ubi: UBI device description object
 *
 * Returns true if a bad header error should be emulated, otherwise
 * returns false.
 */
static inline bool ubi_dbg_is_bad_hdr(const struct ubi_device *ubi,
				      unsigned int caller)
{
	return ubi_dbg_fail_bad_hdr(ubi, caller);
}

/**
 * ubi_dbg_is_bad_hdr_ebadmsg - if it is time to emulate a bad header with
 * ECC error.
 *
 * @ubi: UBI device description object
 *
 * Returns true if a bad header with ECC error should be emulated, otherwise
 * returns false.
 */
static inline bool ubi_dbg_is_bad_hdr_ebadmsg(const struct ubi_device *ubi,
					      unsigned int caller)
{
	return ubi_dbg_fail_bad_hdr_ebadmsg(ubi, caller);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * ubi_dbg_is_bgt_disabled - if the background thread is disabled.
 * @ubi: UBI device description object
 *
 * Returns non-zero if the UBI background thread is disabled for testing
 * purposes.
 */
static inline int ubi_dbg_is_bgt_disabled(const struct ubi_device *ubi)
{
<<<<<<< HEAD
	return ubi->dbg->disable_bgt;
}

/**
 * ubi_dbg_is_bitflip - if it is time to emulate a bit-flip.
 * @ubi: UBI device description object
 *
 * Returns non-zero if a bit-flip should be emulated, otherwise returns zero.
 */
static inline int ubi_dbg_is_bitflip(const struct ubi_device *ubi)
{
	if (ubi->dbg->emulate_bitflips)
		return !(random32() % 200);
	return 0;
}

/**
 * ubi_dbg_is_write_failure - if it is time to emulate a write failure.
 * @ubi: UBI device description object
 *
 * Returns non-zero if a write failure should be emulated, otherwise returns
 * zero.
 */
static inline int ubi_dbg_is_write_failure(const struct ubi_device *ubi)
{
	if (ubi->dbg->emulate_io_failures)
		return !(random32() % 500);
	return 0;
}

/**
 * ubi_dbg_is_erase_failure - if its time to emulate an erase failure.
 * @ubi: UBI device description object
 *
 * Returns non-zero if an erase failure should be emulated, otherwise returns
 * zero.
 */
static inline int ubi_dbg_is_erase_failure(const struct ubi_device *ubi)
{
	if (ubi->dbg->emulate_io_failures)
		return !(random32() % 400);
	return 0;
}

#else

/* Use "if (0)" to make compiler check arguments even if debugging is off */
#define ubi_assert(expr)  do {                                               \
	if (0) {                                                             \
		printk(KERN_CRIT "UBI assert failed in %s at %u (pid %d)\n", \
		       __func__, __LINE__, current->pid);                    \
	}                                                                    \
} while (0)

#define dbg_err(fmt, ...) do {                                               \
	if (0)                                                               \
		ubi_err(fmt, ##__VA_ARGS__);                                 \
} while (0)

#define ubi_dbg_msg(fmt, ...) do {                                           \
	if (0)                                                               \
		printk(KERN_DEBUG fmt "\n", ##__VA_ARGS__);                  \
} while (0)

#define dbg_msg(fmt, ...)  ubi_dbg_msg(fmt, ##__VA_ARGS__)
#define dbg_gen(fmt, ...)  ubi_dbg_msg(fmt, ##__VA_ARGS__)
#define dbg_eba(fmt, ...)  ubi_dbg_msg(fmt, ##__VA_ARGS__)
#define dbg_wl(fmt, ...)   ubi_dbg_msg(fmt, ##__VA_ARGS__)
#define dbg_io(fmt, ...)   ubi_dbg_msg(fmt, ##__VA_ARGS__)
#define dbg_bld(fmt, ...)  ubi_dbg_msg(fmt, ##__VA_ARGS__)

static inline void ubi_dbg_dump_stack(void)                          { return; }
static inline void
ubi_dbg_dump_ec_hdr(const struct ubi_ec_hdr *ec_hdr)                 { return; }
static inline void
ubi_dbg_dump_vid_hdr(const struct ubi_vid_hdr *vid_hdr)              { return; }
static inline void
ubi_dbg_dump_vol_info(const struct ubi_volume *vol)                  { return; }
static inline void
ubi_dbg_dump_vtbl_record(const struct ubi_vtbl_record *r, int idx)   { return; }
static inline void ubi_dbg_dump_sv(const struct ubi_scan_volume *sv) { return; }
static inline void ubi_dbg_dump_seb(const struct ubi_scan_leb *seb,
				    int type)                        { return; }
static inline void
ubi_dbg_dump_mkvol_req(const struct ubi_mkvol_req *req)              { return; }
static inline void ubi_dbg_dump_flash(struct ubi_device *ubi,
				      int pnum, int offset, int len) { return; }
static inline void
ubi_dbg_print_hex_dump(const char *l, const char *ps, int pt, int r,
		       int g, const void *b, size_t len, bool a)     { return; }
static inline int ubi_dbg_check_all_ff(struct ubi_device *ubi,
				       int pnum, int offset,
				       int len)                    { return 0; }
static inline int ubi_dbg_check_write(struct ubi_device *ubi,
				      const void *buf, int pnum,
				      int offset, int len)         { return 0; }

static inline int ubi_debugging_init_dev(struct ubi_device *ubi)   { return 0; }
static inline void ubi_debugging_exit_dev(struct ubi_device *ubi)  { return; }
static inline int ubi_debugfs_init(void)                           { return 0; }
static inline void ubi_debugfs_exit(void)                          { return; }
static inline int ubi_debugfs_init_dev(struct ubi_device *ubi)     { return 0; }
static inline void ubi_debugfs_exit_dev(struct ubi_device *ubi)    { return; }

static inline int
ubi_dbg_is_bgt_disabled(const struct ubi_device *ubi)              { return 0; }
static inline int ubi_dbg_is_bitflip(const struct ubi_device *ubi) { return 0; }
static inline int
ubi_dbg_is_write_failure(const struct ubi_device *ubi)             { return 0; }
static inline int
ubi_dbg_is_erase_failure(const struct ubi_device *ubi)             { return 0; }

#endif /* !CONFIG_MTD_UBI_DEBUG */
=======
	return ubi->dbg.disable_bgt;
}

static inline int ubi_dbg_chk_io(const struct ubi_device *ubi)
{
	return ubi->dbg.chk_io;
}

static inline int ubi_dbg_chk_gen(const struct ubi_device *ubi)
{
	return ubi->dbg.chk_gen;
}

static inline int ubi_dbg_chk_fastmap(const struct ubi_device *ubi)
{
	return ubi->dbg.chk_fastmap;
}

static inline void ubi_enable_dbg_chk_fastmap(struct ubi_device *ubi)
{
	ubi->dbg.chk_fastmap = 1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* !__UBI_DEBUG_H__ */
