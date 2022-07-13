<<<<<<< HEAD
/*
 *  linux/include/linux/mtd/nand.h
 *
 *  Copyright © 2000-2010 David Woodhouse <dwmw2@infradead.org>
 *                        Steven J. Hill <sjhill@realitydiluted.com>
 *		          Thomas Gleixner <tglx@linutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Info:
 *	Contains standard defines and IDs for NAND flash devices
 *
 * Changelog:
 *	See git changelog.
 */
#ifndef __LINUX_MTD_NAND_H
#define __LINUX_MTD_NAND_H

#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/bbm.h>

struct mtd_info;
struct nand_flash_dev;
/* Scan and identify a NAND device */
extern int nand_scan(struct mtd_info *mtd, int max_chips);
/*
 * Separate phases of nand_scan(), allowing board driver to intervene
 * and override command or ECC setup according to flash type.
 */
extern int nand_scan_ident(struct mtd_info *mtd, int max_chips,
			   struct nand_flash_dev *table);
extern int nand_scan_tail(struct mtd_info *mtd);

/* Free resources held by the NAND device */
extern void nand_release(struct mtd_info *mtd);

/* Internal helper for board drivers which need to override command function */
extern void nand_wait_ready(struct mtd_info *mtd);

/* locks all blocks present in the device */
extern int nand_lock(struct mtd_info *mtd, loff_t ofs, uint64_t len);

/* unlocks specified locked blocks */
extern int nand_unlock(struct mtd_info *mtd, loff_t ofs, uint64_t len);

/* The maximum number of NAND chips in an array */
#define NAND_MAX_CHIPS		8

/*
 * This constant declares the max. oobsize / page, which
 * is supported now. If you add a chip with bigger oobsize/page
 * adjust this accordingly.
 */
#define NAND_MAX_OOBSIZE	576
#define NAND_MAX_PAGESIZE	8192

/*
 * Constants for hardware specific CLE/ALE/NCE function
 *
 * These are bits which can be or'ed to set/clear multiple
 * bits in one go.
 */
/* Select the chip by setting nCE to low */
#define NAND_NCE		0x01
/* Select the command latch by setting CLE to high */
#define NAND_CLE		0x02
/* Select the address latch by setting ALE to high */
#define NAND_ALE		0x04

#define NAND_CTRL_CLE		(NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE		(NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE	0x80

/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_RESET		0xff

#define NAND_CMD_LOCK		0x2a
#define NAND_CMD_UNLOCK1	0x23
#define NAND_CMD_UNLOCK2	0x24

/* Extended commands for large page devices */
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

/* Extended commands for AG-AND device */
/*
 * Note: the command for NAND_CMD_DEPLETE1 is really 0x00 but
 *       there is no way to distinguish that from NAND_CMD_READ0
 *       until the remaining sequence of commands has been completed
 *       so add a high order bit and mask it off in the command.
 */
#define NAND_CMD_DEPLETE1	0x100
#define NAND_CMD_DEPLETE2	0x38
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_STATUS_ERROR	0x72
/* multi-bank error status (banks 0-3) */
#define NAND_CMD_STATUS_ERROR0	0x73
#define NAND_CMD_STATUS_ERROR1	0x74
#define NAND_CMD_STATUS_ERROR2	0x75
#define NAND_CMD_STATUS_ERROR3	0x76
#define NAND_CMD_STATUS_RESET	0x7f
#define NAND_CMD_STATUS_CLEAR	0xff

#define NAND_CMD_NONE		-1

/* Status bits */
#define NAND_STATUS_FAIL	0x01
#define NAND_STATUS_FAIL_N1	0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY	0x40
#define NAND_STATUS_WP		0x80

/*
 * Constants for ECC_MODES
 */
typedef enum {
	NAND_ECC_NONE,
	NAND_ECC_SOFT,
	NAND_ECC_HW,
	NAND_ECC_HW_SYNDROME,
	NAND_ECC_HW_OOB_FIRST,
	NAND_ECC_SOFT_BCH,
} nand_ecc_modes_t;

/*
 * Constants for Hardware ECC
 */
/* Reset Hardware ECC for read */
#define NAND_ECC_READ		0
/* Reset Hardware ECC for write */
#define NAND_ECC_WRITE		1
/* Enable Hardware ECC before syndrome is read back from flash */
#define NAND_ECC_READSYN	2

/* Bit mask for flags passed to do_nand_read_ecc */
#define NAND_GET_DEVICE		0x80


/*
 * Option constants for bizarre disfunctionality and real
 * features.
 */
/* Chip can not auto increment pages */
#define NAND_NO_AUTOINCR	0x00000001
/* Buswidth is 16 bit */
#define NAND_BUSWIDTH_16	0x00000002
/* Device supports partial programming without padding */
#define NAND_NO_PADDING		0x00000004
/* Chip has cache program function */
#define NAND_CACHEPRG		0x00000008
/* Chip has copy back function */
#define NAND_COPYBACK		0x00000010
/*
 * AND Chip which has 4 banks and a confusing page / block
 * assignment. See Renesas datasheet for further information.
 */
#define NAND_IS_AND		0x00000020
/*
 * Chip has a array of 4 pages which can be read without
 * additional ready /busy waits.
 */
#define NAND_4PAGE_ARRAY	0x00000040
/*
 * Chip requires that BBT is periodically rewritten to prevent
 * bits from adjacent blocks from 'leaking' in altering data.
 * This happens with the Renesas AG-AND chips, possibly others.
 */
#define BBT_AUTO_REFRESH	0x00000080
/*
 * Chip does not require ready check on read. True
 * for all large page devices, as they do not support
 * autoincrement.
 */
#define NAND_NO_READRDY		0x00000100
/* Chip does not allow subpage writes */
#define NAND_NO_SUBPAGE_WRITE	0x00000200

/* Device is one of 'new' xD cards that expose fake nand command set */
#define NAND_BROKEN_XD		0x00000400

/* Device behaves just like nand, but is readonly */
#define NAND_ROM		0x00000800

/* Options valid for Samsung large page devices */
#define NAND_SAMSUNG_LP_OPTIONS \
	(NAND_NO_PADDING | NAND_CACHEPRG | NAND_COPYBACK)

/* Macros to identify the above */
#define NAND_CANAUTOINCR(chip) (!(chip->options & NAND_NO_AUTOINCR))
#define NAND_MUST_PAD(chip) (!(chip->options & NAND_NO_PADDING))
#define NAND_HAS_CACHEPROG(chip) ((chip->options & NAND_CACHEPRG))
#define NAND_HAS_COPYBACK(chip) ((chip->options & NAND_COPYBACK))
/* Large page NAND with SOFT_ECC should support subpage reads */
#define NAND_SUBPAGE_READ(chip) ((chip->ecc.mode == NAND_ECC_SOFT) \
					&& (chip->page_shift > 9))

/* Non chip related options */
/* This option skips the bbt scan during initialization. */
#define NAND_SKIP_BBTSCAN	0x00010000
/*
 * This option is defined if the board driver allocates its own buffers
 * (e.g. because it needs them DMA-coherent).
 */
#define NAND_OWN_BUFFERS	0x00020000
/* Chip may not exist, so silence any errors in scan */
#define NAND_SCAN_SILENT_NODEV	0x00040000

/* Options set by nand scan */
/* Nand scan has allocated controller struct */
#define NAND_CONTROLLER_ALLOC	0x80000000

/* Cell info constants */
#define NAND_CI_CHIPNR_MSK	0x03
#define NAND_CI_CELLTYPE_MSK	0x0C

/* Keep gcc happy */
struct nand_chip;

struct nand_onfi_params {
	/* rev info and features block */
	/* 'O' 'N' 'F' 'I'  */
	u8 sig[4];
	__le16 revision;
	__le16 features;
	__le16 opt_cmd;
	u8 reserved[22];

	/* manufacturer information block */
	char manufacturer[12];
	char model[20];
	u8 jedec_id;
	__le16 date_code;
	u8 reserved2[13];

	/* memory organization block */
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	__le32 data_bytes_per_ppage;
	__le16 spare_bytes_per_ppage;
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	u8 programs_per_page;
	u8 ppage_attr;
	u8 ecc_bits;
	u8 interleaved_bits;
	u8 interleaved_ops;
	u8 reserved3[13];

	/* electrical parameter block */
	u8 io_pin_capacitance_max;
	__le16 async_timing_mode;
	__le16 program_cache_timing_mode;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_ccs;
	__le16 src_sync_timing_mode;
	__le16 src_ssync_features;
	__le16 clk_pin_capacitance_typ;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	u8 input_pin_capacitance_max;
	u8 driver_strenght_support;
	__le16 t_int_r;
	__le16 t_ald;
	u8 reserved4[7];

	/* vendor */
	u8 reserved5[90];

	__le16 crc;
} __attribute__((packed));

#define ONFI_CRC_BASE	0x4F4E

/**
 * struct nand_hw_control - Control structure for hardware controller (e.g ECC generator) shared among independent devices
 * @lock:               protection lock
 * @active:		the mtd device which holds the controller currently
 * @wq:			wait queue to sleep on if a NAND operation is in
 *			progress used instead of the per chip wait queue
 *			when a hw controller is available.
 */
struct nand_hw_control {
	spinlock_t lock;
	struct nand_chip *active;
	wait_queue_head_t wq;
};

/**
 * struct nand_ecc_ctrl - Control structure for ECC
 * @mode:	ECC mode
 * @steps:	number of ECC steps per page
 * @size:	data bytes per ECC step
 * @bytes:	ECC bytes per step
 * @strength:	max number of correctible bits per ECC step
 * @total:	total number of ECC bytes per page
 * @prepad:	padding information for syndrome based ECC generators
 * @postpad:	padding information for syndrome based ECC generators
 * @layout:	ECC layout control struct pointer
 * @priv:	pointer to private ECC control data
 * @hwctl:	function to control hardware ECC generator. Must only
 *		be provided if an hardware ECC is available
 * @calculate:	function for ECC calculation or readback from ECC hardware
 * @correct:	function for ECC correction, matching to ECC generator (sw/hw)
 * @read_page_raw:	function to read a raw page without ECC
 * @write_page_raw:	function to write a raw page without ECC
 * @read_page:	function to read a page according to the ECC generator
 *		requirements.
 * @read_subpage:	function to read parts of the page covered by ECC.
 * @write_page:	function to write a page according to the ECC generator
 *		requirements.
 * @write_oob_raw:	function to write chip OOB data without ECC
 * @read_oob_raw:	function to read chip OOB data without ECC
 * @read_oob:	function to read chip OOB data
 * @write_oob:	function to write chip OOB data
 */
struct nand_ecc_ctrl {
	nand_ecc_modes_t mode;
	int steps;
	int size;
	int bytes;
	int total;
	int strength;
	int prepad;
	int postpad;
	struct nand_ecclayout	*layout;
	void *priv;
	void (*hwctl)(struct mtd_info *mtd, int mode);
	int (*calculate)(struct mtd_info *mtd, const uint8_t *dat,
			uint8_t *ecc_code);
	int (*correct)(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc,
			uint8_t *calc_ecc);
	int (*read_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int page);
	void (*write_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf);
	int (*read_page)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int page);
	int (*read_subpage)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offs, uint32_t len, uint8_t *buf);
	void (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf);
	int (*write_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
	int (*read_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page, int sndcmd);
	int (*read_oob)(struct mtd_info *mtd, struct nand_chip *chip, int page,
			int sndcmd);
	int (*write_oob)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
};

/**
 * struct nand_buffers - buffer structure for read/write
 * @ecccalc:	buffer for calculated ECC
 * @ecccode:	buffer for ECC read from flash
 * @databuf:	buffer for data - dynamically sized
 *
 * Do not change the order of buffers. databuf and oobrbuf must be in
 * consecutive order.
 */
struct nand_buffers {
	uint8_t	ecccalc[NAND_MAX_OOBSIZE];
	uint8_t	ecccode[NAND_MAX_OOBSIZE];
	uint8_t databuf[NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE];
};

/**
 * struct nand_chip - NAND Private Flash Chip Data
 * @IO_ADDR_R:		[BOARDSPECIFIC] address to read the 8 I/O lines of the
 *			flash device
 * @IO_ADDR_W:		[BOARDSPECIFIC] address to write the 8 I/O lines of the
 *			flash device.
 * @read_byte:		[REPLACEABLE] read one byte from the chip
 * @read_word:		[REPLACEABLE] read one word from the chip
 * @write_buf:		[REPLACEABLE] write data from the buffer to the chip
 * @read_buf:		[REPLACEABLE] read data from the chip into the buffer
 * @verify_buf:		[REPLACEABLE] verify buffer contents against the chip
 *			data.
 * @select_chip:	[REPLACEABLE] select chip nr
 * @block_bad:		[REPLACEABLE] check, if the block is bad
 * @block_markbad:	[REPLACEABLE] mark the block bad
 * @cmd_ctrl:		[BOARDSPECIFIC] hardwarespecific function for controlling
 *			ALE/CLE/nCE. Also used to write command and address
 * @init_size:		[BOARDSPECIFIC] hardwarespecific function for setting
 *			mtd->oobsize, mtd->writesize and so on.
 *			@id_data contains the 8 bytes values of NAND_CMD_READID.
 *			Return with the bus width.
 * @dev_ready:		[BOARDSPECIFIC] hardwarespecific function for accessing
 *			device ready/busy line. If set to NULL no access to
 *			ready/busy is available and the ready/busy information
 *			is read from the chip status register.
 * @cmdfunc:		[REPLACEABLE] hardwarespecific function for writing
 *			commands to the chip.
 * @waitfunc:		[REPLACEABLE] hardwarespecific function for wait on
 *			ready.
 * @ecc:		[BOARDSPECIFIC] ECC control structure
 * @buffers:		buffer structure for read/write
 * @hwcontrol:		platform-specific hardware control structure
 * @erase_cmd:		[INTERN] erase command write function, selectable due
 *			to AND support.
 * @scan_bbt:		[REPLACEABLE] function to scan bad block table
 * @chip_delay:		[BOARDSPECIFIC] chip dependent delay for transferring
 *			data from array to read regs (tR).
 * @state:		[INTERN] the current state of the NAND device
 * @oob_poi:		"poison value buffer," used for laying out OOB data
 *			before writing
 * @page_shift:		[INTERN] number of address bits in a page (column
 *			address bits).
 * @phys_erase_shift:	[INTERN] number of address bits in a physical eraseblock
 * @bbt_erase_shift:	[INTERN] number of address bits in a bbt entry
 * @chip_shift:		[INTERN] number of address bits in one chip
 * @options:		[BOARDSPECIFIC] various chip options. They can partly
 *			be set to inform nand_scan about special functionality.
 *			See the defines for further explanation.
 * @bbt_options:	[INTERN] bad block specific options. All options used
 *			here must come from bbm.h. By default, these options
 *			will be copied to the appropriate nand_bbt_descr's.
 * @badblockpos:	[INTERN] position of the bad block marker in the oob
 *			area.
 * @badblockbits:	[INTERN] minimum number of set bits in a good block's
 *			bad block marker position; i.e., BBM == 11110111b is
 *			not bad when badblockbits == 7
 * @cellinfo:		[INTERN] MLC/multichip data from chip ident
 * @numchips:		[INTERN] number of physical chips
 * @chipsize:		[INTERN] the size of one chip for multichip arrays
 * @pagemask:		[INTERN] page number mask = number of (pages / chip) - 1
 * @pagebuf:		[INTERN] holds the pagenumber which is currently in
 *			data_buf.
 * @subpagesize:	[INTERN] holds the subpagesize
 * @onfi_version:	[INTERN] holds the chip ONFI version (BCD encoded),
 *			non 0 if ONFI supported.
 * @onfi_params:	[INTERN] holds the ONFI page parameter when ONFI is
 *			supported, 0 otherwise.
 * @ecclayout:		[REPLACEABLE] the default ECC placement scheme
 * @bbt:		[INTERN] bad block table pointer
 * @bbt_td:		[REPLACEABLE] bad block table descriptor for flash
 *			lookup.
 * @bbt_md:		[REPLACEABLE] bad block table mirror descriptor
 * @badblock_pattern:	[REPLACEABLE] bad block scan pattern used for initial
 *			bad block scan.
 * @controller:		[REPLACEABLE] a pointer to a hardware controller
 *			structure which is shared among multiple independent
 *			devices.
 * @priv:		[OPTIONAL] pointer to private chip data
 * @errstat:		[OPTIONAL] hardware specific function to perform
 *			additional error status checks (determine if errors are
 *			correctable).
 * @write_page:		[REPLACEABLE] High-level page write function
 */

struct nand_chip {
	void __iomem *IO_ADDR_R;
	void __iomem *IO_ADDR_W;

	uint8_t (*read_byte)(struct mtd_info *mtd);
	u16 (*read_word)(struct mtd_info *mtd);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	int (*verify_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	int (*block_bad)(struct mtd_info *mtd, loff_t ofs, int getchip);
	int (*block_markbad)(struct mtd_info *mtd, loff_t ofs);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	int (*init_size)(struct mtd_info *mtd, struct nand_chip *this,
			u8 *id_data);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*cmdfunc)(struct mtd_info *mtd, unsigned command, int column,
			int page_addr);
	int(*waitfunc)(struct mtd_info *mtd, struct nand_chip *this);
	void (*erase_cmd)(struct mtd_info *mtd, int page);
	int (*scan_bbt)(struct mtd_info *mtd);
	int (*errstat)(struct mtd_info *mtd, struct nand_chip *this, int state,
			int status, int page);
	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int page, int cached, int raw);

	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;

	int page_shift;
	int phys_erase_shift;
	int bbt_erase_shift;
	int chip_shift;
	int numchips;
	uint64_t chipsize;
	int pagemask;
	int pagebuf;
	int subpagesize;
	uint8_t cellinfo;
	int badblockpos;
	int badblockbits;

	int onfi_version;
	struct nand_onfi_params	onfi_params;

	flstate_t state;

	uint8_t *oob_poi;
	struct nand_hw_control *controller;
	struct nand_ecclayout *ecclayout;

	struct nand_ecc_ctrl ecc;
	struct nand_buffers *buffers;
	struct nand_hw_control hwcontrol;

	uint8_t *bbt;
	struct nand_bbt_descr *bbt_td;
	struct nand_bbt_descr *bbt_md;

	struct nand_bbt_descr *badblock_pattern;

	void *priv;
};

/*
 * NAND Flash Manufacturer ID Codes
 */
#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01
#define NAND_MFR_MACRONIX	0xc2

/**
 * struct nand_flash_dev - NAND Flash Device ID Structure
 * @name:	Identify the device type
 * @id:		device ID code
 * @pagesize:	Pagesize in bytes. Either 256 or 512 or 0
 *		If the pagesize is 0, then the real pagesize
 *		and the eraseize are determined from the
 *		extended id bytes in the chip
 * @erasesize:	Size of an erase block in the flash device.
 * @chipsize:	Total chipsize in Mega Bytes
 * @options:	Bitfield to store chip relevant options
 */
struct nand_flash_dev {
	char *name;
	int id;
	unsigned long pagesize;
	unsigned long chipsize;
	unsigned long erasesize;
	unsigned long options;
};

/**
 * struct nand_manufacturers - NAND Flash Manufacturer ID Structure
 * @name:	Manufacturer name
 * @id:		manufacturer ID code of device.
*/
struct nand_manufacturers {
	int id;
	char *name;
};

extern struct nand_flash_dev nand_flash_ids[];
extern struct nand_manufacturers nand_manuf_ids[];

extern int nand_scan_bbt(struct mtd_info *mtd, struct nand_bbt_descr *bd);
extern int nand_update_bbt(struct mtd_info *mtd, loff_t offs);
extern int nand_default_bbt(struct mtd_info *mtd);
extern int nand_isbad_bbt(struct mtd_info *mtd, loff_t offs, int allowbbt);
extern int nand_erase_nand(struct mtd_info *mtd, struct erase_info *instr,
			   int allowbbt);
extern int nand_do_read(struct mtd_info *mtd, loff_t from, size_t len,
			size_t *retlen, uint8_t *buf);

/**
 * struct platform_nand_chip - chip level device structure
 * @nr_chips:		max. number of chips to scan for
 * @chip_offset:	chip number offset
 * @nr_partitions:	number of partitions pointed to by partitions (or zero)
 * @partitions:		mtd partition list
 * @chip_delay:		R/B delay value in us
 * @options:		Option flags, e.g. 16bit buswidth
 * @bbt_options:	BBT option flags, e.g. NAND_BBT_USE_FLASH
 * @ecclayout:		ECC layout info structure
 * @part_probe_types:	NULL-terminated array of probe types
 */
struct platform_nand_chip {
	int nr_chips;
	int chip_offset;
	int nr_partitions;
	struct mtd_partition *partitions;
	struct nand_ecclayout *ecclayout;
	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;
	const char **part_probe_types;
};

/* Keep gcc happy */
struct platform_device;

/**
 * struct platform_nand_ctrl - controller level device structure
 * @probe:		platform specific function to probe/setup hardware
 * @remove:		platform specific function to remove/teardown hardware
 * @hwcontrol:		platform specific hardware control structure
 * @dev_ready:		platform specific function to read ready/busy pin
 * @select_chip:	platform specific chip select function
 * @cmd_ctrl:		platform specific function for controlling
 *			ALE/CLE/nCE. Also used to write command and address
 * @write_buf:		platform specific function for write buffer
 * @read_buf:		platform specific function for read buffer
 * @priv:		private data to transport driver specific settings
 *
 * All fields are optional and depend on the hardware driver requirements
 */
struct platform_nand_ctrl {
	int (*probe)(struct platform_device *pdev);
	void (*remove)(struct platform_device *pdev);
	void (*hwcontrol)(struct mtd_info *mtd, int cmd);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Copyright 2017 - Free Electrons
 *
 *  Authors:
 *	Boris Brezillon <boris.brezillon@free-electrons.com>
 *	Peter Pan <peterpandong@micron.com>
 */

#ifndef __LINUX_MTD_NAND_H
#define __LINUX_MTD_NAND_H

#include <linux/mtd/mtd.h>

struct nand_device;

/**
 * struct nand_memory_organization - Memory organization structure
 * @bits_per_cell: number of bits per NAND cell
 * @pagesize: page size
 * @oobsize: OOB area size
 * @pages_per_eraseblock: number of pages per eraseblock
 * @eraseblocks_per_lun: number of eraseblocks per LUN (Logical Unit Number)
 * @max_bad_eraseblocks_per_lun: maximum number of eraseblocks per LUN
 * @planes_per_lun: number of planes per LUN
 * @luns_per_target: number of LUN per target (target is a synonym for die)
 * @ntargets: total number of targets exposed by the NAND device
 */
struct nand_memory_organization {
	unsigned int bits_per_cell;
	unsigned int pagesize;
	unsigned int oobsize;
	unsigned int pages_per_eraseblock;
	unsigned int eraseblocks_per_lun;
	unsigned int max_bad_eraseblocks_per_lun;
	unsigned int planes_per_lun;
	unsigned int luns_per_target;
	unsigned int ntargets;
};

#define NAND_MEMORG(bpc, ps, os, ppe, epl, mbb, ppl, lpt, nt)	\
	{							\
		.bits_per_cell = (bpc),				\
		.pagesize = (ps),				\
		.oobsize = (os),				\
		.pages_per_eraseblock = (ppe),			\
		.eraseblocks_per_lun = (epl),			\
		.max_bad_eraseblocks_per_lun = (mbb),		\
		.planes_per_lun = (ppl),			\
		.luns_per_target = (lpt),			\
		.ntargets = (nt),				\
	}

/**
 * struct nand_row_converter - Information needed to convert an absolute offset
 *			       into a row address
 * @lun_addr_shift: position of the LUN identifier in the row address
 * @eraseblock_addr_shift: position of the eraseblock identifier in the row
 *			   address
 */
struct nand_row_converter {
	unsigned int lun_addr_shift;
	unsigned int eraseblock_addr_shift;
};

/**
 * struct nand_pos - NAND position object
 * @target: the NAND target/die
 * @lun: the LUN identifier
 * @plane: the plane within the LUN
 * @eraseblock: the eraseblock within the LUN
 * @page: the page within the LUN
 *
 * These information are usually used by specific sub-layers to select the
 * appropriate target/die and generate a row address to pass to the device.
 */
struct nand_pos {
	unsigned int target;
	unsigned int lun;
	unsigned int plane;
	unsigned int eraseblock;
	unsigned int page;
};

/**
 * enum nand_page_io_req_type - Direction of an I/O request
 * @NAND_PAGE_READ: from the chip, to the controller
 * @NAND_PAGE_WRITE: from the controller, to the chip
 */
enum nand_page_io_req_type {
	NAND_PAGE_READ = 0,
	NAND_PAGE_WRITE,
};

/**
 * struct nand_page_io_req - NAND I/O request object
 * @type: the type of page I/O: read or write
 * @pos: the position this I/O request is targeting
 * @dataoffs: the offset within the page
 * @datalen: number of data bytes to read from/write to this page
 * @databuf: buffer to store data in or get data from
 * @ooboffs: the OOB offset within the page
 * @ooblen: the number of OOB bytes to read from/write to this page
 * @oobbuf: buffer to store OOB data in or get OOB data from
 * @mode: one of the %MTD_OPS_XXX mode
 *
 * This object is used to pass per-page I/O requests to NAND sub-layers. This
 * way all useful information are already formatted in a useful way and
 * specific NAND layers can focus on translating these information into
 * specific commands/operations.
 */
struct nand_page_io_req {
	enum nand_page_io_req_type type;
	struct nand_pos pos;
	unsigned int dataoffs;
	unsigned int datalen;
	union {
		const void *out;
		void *in;
	} databuf;
	unsigned int ooboffs;
	unsigned int ooblen;
	union {
		const void *out;
		void *in;
	} oobbuf;
	int mode;
};

const struct mtd_ooblayout_ops *nand_get_small_page_ooblayout(void);
const struct mtd_ooblayout_ops *nand_get_large_page_ooblayout(void);
const struct mtd_ooblayout_ops *nand_get_large_page_hamming_ooblayout(void);

/**
 * enum nand_ecc_engine_type - NAND ECC engine type
 * @NAND_ECC_ENGINE_TYPE_INVALID: Invalid value
 * @NAND_ECC_ENGINE_TYPE_NONE: No ECC correction
 * @NAND_ECC_ENGINE_TYPE_SOFT: Software ECC correction
 * @NAND_ECC_ENGINE_TYPE_ON_HOST: On host hardware ECC correction
 * @NAND_ECC_ENGINE_TYPE_ON_DIE: On chip hardware ECC correction
 */
enum nand_ecc_engine_type {
	NAND_ECC_ENGINE_TYPE_INVALID,
	NAND_ECC_ENGINE_TYPE_NONE,
	NAND_ECC_ENGINE_TYPE_SOFT,
	NAND_ECC_ENGINE_TYPE_ON_HOST,
	NAND_ECC_ENGINE_TYPE_ON_DIE,
};

/**
 * enum nand_ecc_placement - NAND ECC bytes placement
 * @NAND_ECC_PLACEMENT_UNKNOWN: The actual position of the ECC bytes is unknown
 * @NAND_ECC_PLACEMENT_OOB: The ECC bytes are located in the OOB area
 * @NAND_ECC_PLACEMENT_INTERLEAVED: Syndrome layout, there are ECC bytes
 *                                  interleaved with regular data in the main
 *                                  area
 */
enum nand_ecc_placement {
	NAND_ECC_PLACEMENT_UNKNOWN,
	NAND_ECC_PLACEMENT_OOB,
	NAND_ECC_PLACEMENT_INTERLEAVED,
};

/**
 * enum nand_ecc_algo - NAND ECC algorithm
 * @NAND_ECC_ALGO_UNKNOWN: Unknown algorithm
 * @NAND_ECC_ALGO_HAMMING: Hamming algorithm
 * @NAND_ECC_ALGO_BCH: Bose-Chaudhuri-Hocquenghem algorithm
 * @NAND_ECC_ALGO_RS: Reed-Solomon algorithm
 */
enum nand_ecc_algo {
	NAND_ECC_ALGO_UNKNOWN,
	NAND_ECC_ALGO_HAMMING,
	NAND_ECC_ALGO_BCH,
	NAND_ECC_ALGO_RS,
};

/**
 * struct nand_ecc_props - NAND ECC properties
 * @engine_type: ECC engine type
 * @placement: OOB placement (if relevant)
 * @algo: ECC algorithm (if relevant)
 * @strength: ECC strength
 * @step_size: Number of bytes per step
 * @flags: Misc properties
 */
struct nand_ecc_props {
	enum nand_ecc_engine_type engine_type;
	enum nand_ecc_placement placement;
	enum nand_ecc_algo algo;
	unsigned int strength;
	unsigned int step_size;
	unsigned int flags;
};

#define NAND_ECCREQ(str, stp) { .strength = (str), .step_size = (stp) }

/* NAND ECC misc flags */
#define NAND_ECC_MAXIMIZE_STRENGTH BIT(0)

/**
 * struct nand_bbt - bad block table object
 * @cache: in memory BBT cache
 */
struct nand_bbt {
	unsigned long *cache;
};

/**
 * struct nand_ops - NAND operations
 * @erase: erase a specific block. No need to check if the block is bad before
 *	   erasing, this has been taken care of by the generic NAND layer
 * @markbad: mark a specific block bad. No need to check if the block is
 *	     already marked bad, this has been taken care of by the generic
 *	     NAND layer. This method should just write the BBM (Bad Block
 *	     Marker) so that future call to struct_nand_ops->isbad() return
 *	     true
 * @isbad: check whether a block is bad or not. This method should just read
 *	   the BBM and return whether the block is bad or not based on what it
 *	   reads
 *
 * These are all low level operations that should be implemented by specialized
 * NAND layers (SPI NAND, raw NAND, ...).
 */
struct nand_ops {
	int (*erase)(struct nand_device *nand, const struct nand_pos *pos);
	int (*markbad)(struct nand_device *nand, const struct nand_pos *pos);
	bool (*isbad)(struct nand_device *nand, const struct nand_pos *pos);
};

/**
 * struct nand_ecc_context - Context for the ECC engine
 * @conf: basic ECC engine parameters
 * @nsteps: number of ECC steps
 * @total: total number of bytes used for storing ECC codes, this is used by
 *         generic OOB layouts
 * @priv: ECC engine driver private data
 */
struct nand_ecc_context {
	struct nand_ecc_props conf;
	unsigned int nsteps;
	unsigned int total;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void *priv;
};

/**
<<<<<<< HEAD
 * struct platform_nand_data - container structure for platform-specific data
 * @chip:		chip level chip structure
 * @ctrl:		controller level device structure
 */
struct platform_nand_data {
	struct platform_nand_chip chip;
	struct platform_nand_ctrl ctrl;
};

/* Some helpers to access the data structures */
static inline
struct platform_nand_chip *get_platform_nandchip(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	return chip->priv;
}

=======
 * struct nand_ecc_engine_ops - ECC engine operations
 * @init_ctx: given a desired user configuration for the pointed NAND device,
 *            requests the ECC engine driver to setup a configuration with
 *            values it supports.
 * @cleanup_ctx: clean the context initialized by @init_ctx.
 * @prepare_io_req: is called before reading/writing a page to prepare the I/O
 *                  request to be performed with ECC correction.
 * @finish_io_req: is called after reading/writing a page to terminate the I/O
 *                 request and ensure proper ECC correction.
 */
struct nand_ecc_engine_ops {
	int (*init_ctx)(struct nand_device *nand);
	void (*cleanup_ctx)(struct nand_device *nand);
	int (*prepare_io_req)(struct nand_device *nand,
			      struct nand_page_io_req *req);
	int (*finish_io_req)(struct nand_device *nand,
			     struct nand_page_io_req *req);
};

/**
 * enum nand_ecc_engine_integration - How the NAND ECC engine is integrated
 * @NAND_ECC_ENGINE_INTEGRATION_INVALID: Invalid value
 * @NAND_ECC_ENGINE_INTEGRATION_PIPELINED: Pipelined engine, performs on-the-fly
 *                                         correction, does not need to copy
 *                                         data around
 * @NAND_ECC_ENGINE_INTEGRATION_EXTERNAL: External engine, needs to bring the
 *                                        data into its own area before use
 */
enum nand_ecc_engine_integration {
	NAND_ECC_ENGINE_INTEGRATION_INVALID,
	NAND_ECC_ENGINE_INTEGRATION_PIPELINED,
	NAND_ECC_ENGINE_INTEGRATION_EXTERNAL,
};

/**
 * struct nand_ecc_engine - ECC engine abstraction for NAND devices
 * @dev: Host device
 * @node: Private field for registration time
 * @ops: ECC engine operations
 * @integration: How the engine is integrated with the host
 *               (only relevant on %NAND_ECC_ENGINE_TYPE_ON_HOST engines)
 * @priv: Private data
 */
struct nand_ecc_engine {
	struct device *dev;
	struct list_head node;
	struct nand_ecc_engine_ops *ops;
	enum nand_ecc_engine_integration integration;
	void *priv;
};

void of_get_nand_ecc_user_config(struct nand_device *nand);
int nand_ecc_init_ctx(struct nand_device *nand);
void nand_ecc_cleanup_ctx(struct nand_device *nand);
int nand_ecc_prepare_io_req(struct nand_device *nand,
			    struct nand_page_io_req *req);
int nand_ecc_finish_io_req(struct nand_device *nand,
			   struct nand_page_io_req *req);
bool nand_ecc_is_strong_enough(struct nand_device *nand);

#if IS_REACHABLE(CONFIG_MTD_NAND_CORE)
int nand_ecc_register_on_host_hw_engine(struct nand_ecc_engine *engine);
int nand_ecc_unregister_on_host_hw_engine(struct nand_ecc_engine *engine);
#else
static inline int
nand_ecc_register_on_host_hw_engine(struct nand_ecc_engine *engine)
{
	return -ENOTSUPP;
}
static inline int
nand_ecc_unregister_on_host_hw_engine(struct nand_ecc_engine *engine)
{
	return -ENOTSUPP;
}
#endif

struct nand_ecc_engine *nand_ecc_get_sw_engine(struct nand_device *nand);
struct nand_ecc_engine *nand_ecc_get_on_die_hw_engine(struct nand_device *nand);
struct nand_ecc_engine *nand_ecc_get_on_host_hw_engine(struct nand_device *nand);
void nand_ecc_put_on_host_hw_engine(struct nand_device *nand);
struct device *nand_ecc_get_engine_dev(struct device *host);

#if IS_ENABLED(CONFIG_MTD_NAND_ECC_SW_HAMMING)
struct nand_ecc_engine *nand_ecc_sw_hamming_get_engine(void);
#else
static inline struct nand_ecc_engine *nand_ecc_sw_hamming_get_engine(void)
{
	return NULL;
}
#endif /* CONFIG_MTD_NAND_ECC_SW_HAMMING */

#if IS_ENABLED(CONFIG_MTD_NAND_ECC_SW_BCH)
struct nand_ecc_engine *nand_ecc_sw_bch_get_engine(void);
#else
static inline struct nand_ecc_engine *nand_ecc_sw_bch_get_engine(void)
{
	return NULL;
}
#endif /* CONFIG_MTD_NAND_ECC_SW_BCH */

/**
 * struct nand_ecc_req_tweak_ctx - Help for automatically tweaking requests
 * @orig_req: Pointer to the original IO request
 * @nand: Related NAND device, to have access to its memory organization
 * @page_buffer_size: Real size of the page buffer to use (can be set by the
 *                    user before the tweaking mechanism initialization)
 * @oob_buffer_size: Real size of the OOB buffer to use (can be set by the
 *                   user before the tweaking mechanism initialization)
 * @spare_databuf: Data bounce buffer
 * @spare_oobbuf: OOB bounce buffer
 * @bounce_data: Flag indicating a data bounce buffer is used
 * @bounce_oob: Flag indicating an OOB bounce buffer is used
 */
struct nand_ecc_req_tweak_ctx {
	struct nand_page_io_req orig_req;
	struct nand_device *nand;
	unsigned int page_buffer_size;
	unsigned int oob_buffer_size;
	void *spare_databuf;
	void *spare_oobbuf;
	bool bounce_data;
	bool bounce_oob;
};

int nand_ecc_init_req_tweaking(struct nand_ecc_req_tweak_ctx *ctx,
			       struct nand_device *nand);
void nand_ecc_cleanup_req_tweaking(struct nand_ecc_req_tweak_ctx *ctx);
void nand_ecc_tweak_req(struct nand_ecc_req_tweak_ctx *ctx,
			struct nand_page_io_req *req);
void nand_ecc_restore_req(struct nand_ecc_req_tweak_ctx *ctx,
			  struct nand_page_io_req *req);

/**
 * struct nand_ecc - Information relative to the ECC
 * @defaults: Default values, depend on the underlying subsystem
 * @requirements: ECC requirements from the NAND chip perspective
 * @user_conf: User desires in terms of ECC parameters
 * @ctx: ECC context for the ECC engine, derived from the device @requirements
 *       the @user_conf and the @defaults
 * @ondie_engine: On-die ECC engine reference, if any
 * @engine: ECC engine actually bound
 */
struct nand_ecc {
	struct nand_ecc_props defaults;
	struct nand_ecc_props requirements;
	struct nand_ecc_props user_conf;
	struct nand_ecc_context ctx;
	struct nand_ecc_engine *ondie_engine;
	struct nand_ecc_engine *engine;
};

/**
 * struct nand_device - NAND device
 * @mtd: MTD instance attached to the NAND device
 * @memorg: memory layout
 * @ecc: NAND ECC object attached to the NAND device
 * @rowconv: position to row address converter
 * @bbt: bad block table info
 * @ops: NAND operations attached to the NAND device
 *
 * Generic NAND object. Specialized NAND layers (raw NAND, SPI NAND, OneNAND)
 * should declare their own NAND object embedding a nand_device struct (that's
 * how inheritance is done).
 * struct_nand_device->memorg and struct_nand_device->ecc.requirements should
 * be filled at device detection time to reflect the NAND device
 * capabilities/requirements. Once this is done nanddev_init() can be called.
 * It will take care of converting NAND information into MTD ones, which means
 * the specialized NAND layers should never manually tweak
 * struct_nand_device->mtd except for the ->_read/write() hooks.
 */
struct nand_device {
	struct mtd_info mtd;
	struct nand_memory_organization memorg;
	struct nand_ecc ecc;
	struct nand_row_converter rowconv;
	struct nand_bbt bbt;
	const struct nand_ops *ops;
};

/**
 * struct nand_io_iter - NAND I/O iterator
 * @req: current I/O request
 * @oobbytes_per_page: maximum number of OOB bytes per page
 * @dataleft: remaining number of data bytes to read/write
 * @oobleft: remaining number of OOB bytes to read/write
 *
 * Can be used by specialized NAND layers to iterate over all pages covered
 * by an MTD I/O request, which should greatly simplifies the boiler-plate
 * code needed to read/write data from/to a NAND device.
 */
struct nand_io_iter {
	struct nand_page_io_req req;
	unsigned int oobbytes_per_page;
	unsigned int dataleft;
	unsigned int oobleft;
};

/**
 * mtd_to_nanddev() - Get the NAND device attached to the MTD instance
 * @mtd: MTD instance
 *
 * Return: the NAND device embedding @mtd.
 */
static inline struct nand_device *mtd_to_nanddev(struct mtd_info *mtd)
{
	return container_of(mtd, struct nand_device, mtd);
}

/**
 * nanddev_to_mtd() - Get the MTD device attached to a NAND device
 * @nand: NAND device
 *
 * Return: the MTD device embedded in @nand.
 */
static inline struct mtd_info *nanddev_to_mtd(struct nand_device *nand)
{
	return &nand->mtd;
}

/*
 * nanddev_bits_per_cell() - Get the number of bits per cell
 * @nand: NAND device
 *
 * Return: the number of bits per cell.
 */
static inline unsigned int nanddev_bits_per_cell(const struct nand_device *nand)
{
	return nand->memorg.bits_per_cell;
}

/**
 * nanddev_page_size() - Get NAND page size
 * @nand: NAND device
 *
 * Return: the page size.
 */
static inline size_t nanddev_page_size(const struct nand_device *nand)
{
	return nand->memorg.pagesize;
}

/**
 * nanddev_per_page_oobsize() - Get NAND OOB size
 * @nand: NAND device
 *
 * Return: the OOB size.
 */
static inline unsigned int
nanddev_per_page_oobsize(const struct nand_device *nand)
{
	return nand->memorg.oobsize;
}

/**
 * nanddev_pages_per_eraseblock() - Get the number of pages per eraseblock
 * @nand: NAND device
 *
 * Return: the number of pages per eraseblock.
 */
static inline unsigned int
nanddev_pages_per_eraseblock(const struct nand_device *nand)
{
	return nand->memorg.pages_per_eraseblock;
}

/**
 * nanddev_pages_per_target() - Get the number of pages per target
 * @nand: NAND device
 *
 * Return: the number of pages per target.
 */
static inline unsigned int
nanddev_pages_per_target(const struct nand_device *nand)
{
	return nand->memorg.pages_per_eraseblock *
	       nand->memorg.eraseblocks_per_lun *
	       nand->memorg.luns_per_target;
}

/**
 * nanddev_per_page_oobsize() - Get NAND erase block size
 * @nand: NAND device
 *
 * Return: the eraseblock size.
 */
static inline size_t nanddev_eraseblock_size(const struct nand_device *nand)
{
	return nand->memorg.pagesize * nand->memorg.pages_per_eraseblock;
}

/**
 * nanddev_eraseblocks_per_lun() - Get the number of eraseblocks per LUN
 * @nand: NAND device
 *
 * Return: the number of eraseblocks per LUN.
 */
static inline unsigned int
nanddev_eraseblocks_per_lun(const struct nand_device *nand)
{
	return nand->memorg.eraseblocks_per_lun;
}

/**
 * nanddev_eraseblocks_per_target() - Get the number of eraseblocks per target
 * @nand: NAND device
 *
 * Return: the number of eraseblocks per target.
 */
static inline unsigned int
nanddev_eraseblocks_per_target(const struct nand_device *nand)
{
	return nand->memorg.eraseblocks_per_lun * nand->memorg.luns_per_target;
}

/**
 * nanddev_target_size() - Get the total size provided by a single target/die
 * @nand: NAND device
 *
 * Return: the total size exposed by a single target/die in bytes.
 */
static inline u64 nanddev_target_size(const struct nand_device *nand)
{
	return (u64)nand->memorg.luns_per_target *
	       nand->memorg.eraseblocks_per_lun *
	       nand->memorg.pages_per_eraseblock *
	       nand->memorg.pagesize;
}

/**
 * nanddev_ntarget() - Get the total of targets
 * @nand: NAND device
 *
 * Return: the number of targets/dies exposed by @nand.
 */
static inline unsigned int nanddev_ntargets(const struct nand_device *nand)
{
	return nand->memorg.ntargets;
}

/**
 * nanddev_neraseblocks() - Get the total number of eraseblocks
 * @nand: NAND device
 *
 * Return: the total number of eraseblocks exposed by @nand.
 */
static inline unsigned int nanddev_neraseblocks(const struct nand_device *nand)
{
	return nand->memorg.ntargets * nand->memorg.luns_per_target *
	       nand->memorg.eraseblocks_per_lun;
}

/**
 * nanddev_size() - Get NAND size
 * @nand: NAND device
 *
 * Return: the total size (in bytes) exposed by @nand.
 */
static inline u64 nanddev_size(const struct nand_device *nand)
{
	return nanddev_target_size(nand) * nanddev_ntargets(nand);
}

/**
 * nanddev_get_memorg() - Extract memory organization info from a NAND device
 * @nand: NAND device
 *
 * This can be used by the upper layer to fill the memorg info before calling
 * nanddev_init().
 *
 * Return: the memorg object embedded in the NAND device.
 */
static inline struct nand_memory_organization *
nanddev_get_memorg(struct nand_device *nand)
{
	return &nand->memorg;
}

/**
 * nanddev_get_ecc_conf() - Extract the ECC configuration from a NAND device
 * @nand: NAND device
 */
static inline const struct nand_ecc_props *
nanddev_get_ecc_conf(struct nand_device *nand)
{
	return &nand->ecc.ctx.conf;
}

/**
 * nanddev_get_ecc_nsteps() - Extract the number of ECC steps
 * @nand: NAND device
 */
static inline unsigned int
nanddev_get_ecc_nsteps(struct nand_device *nand)
{
	return nand->ecc.ctx.nsteps;
}

/**
 * nanddev_get_ecc_bytes_per_step() - Extract the number of ECC bytes per step
 * @nand: NAND device
 */
static inline unsigned int
nanddev_get_ecc_bytes_per_step(struct nand_device *nand)
{
	return nand->ecc.ctx.total / nand->ecc.ctx.nsteps;
}

/**
 * nanddev_get_ecc_requirements() - Extract the ECC requirements from a NAND
 *                                  device
 * @nand: NAND device
 */
static inline const struct nand_ecc_props *
nanddev_get_ecc_requirements(struct nand_device *nand)
{
	return &nand->ecc.requirements;
}

/**
 * nanddev_set_ecc_requirements() - Assign the ECC requirements of a NAND
 *                                  device
 * @nand: NAND device
 * @reqs: Requirements
 */
static inline void
nanddev_set_ecc_requirements(struct nand_device *nand,
			     const struct nand_ecc_props *reqs)
{
	nand->ecc.requirements = *reqs;
}

int nanddev_init(struct nand_device *nand, const struct nand_ops *ops,
		 struct module *owner);
void nanddev_cleanup(struct nand_device *nand);

/**
 * nanddev_register() - Register a NAND device
 * @nand: NAND device
 *
 * Register a NAND device.
 * This function is just a wrapper around mtd_device_register()
 * registering the MTD device embedded in @nand.
 *
 * Return: 0 in case of success, a negative error code otherwise.
 */
static inline int nanddev_register(struct nand_device *nand)
{
	return mtd_device_register(&nand->mtd, NULL, 0);
}

/**
 * nanddev_unregister() - Unregister a NAND device
 * @nand: NAND device
 *
 * Unregister a NAND device.
 * This function is just a wrapper around mtd_device_unregister()
 * unregistering the MTD device embedded in @nand.
 *
 * Return: 0 in case of success, a negative error code otherwise.
 */
static inline int nanddev_unregister(struct nand_device *nand)
{
	return mtd_device_unregister(&nand->mtd);
}

/**
 * nanddev_set_of_node() - Attach a DT node to a NAND device
 * @nand: NAND device
 * @np: DT node
 *
 * Attach a DT node to a NAND device.
 */
static inline void nanddev_set_of_node(struct nand_device *nand,
				       struct device_node *np)
{
	mtd_set_of_node(&nand->mtd, np);
}

/**
 * nanddev_get_of_node() - Retrieve the DT node attached to a NAND device
 * @nand: NAND device
 *
 * Return: the DT node attached to @nand.
 */
static inline struct device_node *nanddev_get_of_node(struct nand_device *nand)
{
	return mtd_get_of_node(&nand->mtd);
}

/**
 * nanddev_offs_to_pos() - Convert an absolute NAND offset into a NAND position
 * @nand: NAND device
 * @offs: absolute NAND offset (usually passed by the MTD layer)
 * @pos: a NAND position object to fill in
 *
 * Converts @offs into a nand_pos representation.
 *
 * Return: the offset within the NAND page pointed by @pos.
 */
static inline unsigned int nanddev_offs_to_pos(struct nand_device *nand,
					       loff_t offs,
					       struct nand_pos *pos)
{
	unsigned int pageoffs;
	u64 tmp = offs;

	pageoffs = do_div(tmp, nand->memorg.pagesize);
	pos->page = do_div(tmp, nand->memorg.pages_per_eraseblock);
	pos->eraseblock = do_div(tmp, nand->memorg.eraseblocks_per_lun);
	pos->plane = pos->eraseblock % nand->memorg.planes_per_lun;
	pos->lun = do_div(tmp, nand->memorg.luns_per_target);
	pos->target = tmp;

	return pageoffs;
}

/**
 * nanddev_pos_cmp() - Compare two NAND positions
 * @a: First NAND position
 * @b: Second NAND position
 *
 * Compares two NAND positions.
 *
 * Return: -1 if @a < @b, 0 if @a == @b and 1 if @a > @b.
 */
static inline int nanddev_pos_cmp(const struct nand_pos *a,
				  const struct nand_pos *b)
{
	if (a->target != b->target)
		return a->target < b->target ? -1 : 1;

	if (a->lun != b->lun)
		return a->lun < b->lun ? -1 : 1;

	if (a->eraseblock != b->eraseblock)
		return a->eraseblock < b->eraseblock ? -1 : 1;

	if (a->page != b->page)
		return a->page < b->page ? -1 : 1;

	return 0;
}

/**
 * nanddev_pos_to_offs() - Convert a NAND position into an absolute offset
 * @nand: NAND device
 * @pos: the NAND position to convert
 *
 * Converts @pos NAND position into an absolute offset.
 *
 * Return: the absolute offset. Note that @pos points to the beginning of a
 *	   page, if one wants to point to a specific offset within this page
 *	   the returned offset has to be adjusted manually.
 */
static inline loff_t nanddev_pos_to_offs(struct nand_device *nand,
					 const struct nand_pos *pos)
{
	unsigned int npages;

	npages = pos->page +
		 ((pos->eraseblock +
		   (pos->lun +
		    (pos->target * nand->memorg.luns_per_target)) *
		   nand->memorg.eraseblocks_per_lun) *
		  nand->memorg.pages_per_eraseblock);

	return (loff_t)npages * nand->memorg.pagesize;
}

/**
 * nanddev_pos_to_row() - Extract a row address from a NAND position
 * @nand: NAND device
 * @pos: the position to convert
 *
 * Converts a NAND position into a row address that can then be passed to the
 * device.
 *
 * Return: the row address extracted from @pos.
 */
static inline unsigned int nanddev_pos_to_row(struct nand_device *nand,
					      const struct nand_pos *pos)
{
	return (pos->lun << nand->rowconv.lun_addr_shift) |
	       (pos->eraseblock << nand->rowconv.eraseblock_addr_shift) |
	       pos->page;
}

/**
 * nanddev_pos_next_target() - Move a position to the next target/die
 * @nand: NAND device
 * @pos: the position to update
 *
 * Updates @pos to point to the start of the next target/die. Useful when you
 * want to iterate over all targets/dies of a NAND device.
 */
static inline void nanddev_pos_next_target(struct nand_device *nand,
					   struct nand_pos *pos)
{
	pos->page = 0;
	pos->plane = 0;
	pos->eraseblock = 0;
	pos->lun = 0;
	pos->target++;
}

/**
 * nanddev_pos_next_lun() - Move a position to the next LUN
 * @nand: NAND device
 * @pos: the position to update
 *
 * Updates @pos to point to the start of the next LUN. Useful when you want to
 * iterate over all LUNs of a NAND device.
 */
static inline void nanddev_pos_next_lun(struct nand_device *nand,
					struct nand_pos *pos)
{
	if (pos->lun >= nand->memorg.luns_per_target - 1)
		return nanddev_pos_next_target(nand, pos);

	pos->lun++;
	pos->page = 0;
	pos->plane = 0;
	pos->eraseblock = 0;
}

/**
 * nanddev_pos_next_eraseblock() - Move a position to the next eraseblock
 * @nand: NAND device
 * @pos: the position to update
 *
 * Updates @pos to point to the start of the next eraseblock. Useful when you
 * want to iterate over all eraseblocks of a NAND device.
 */
static inline void nanddev_pos_next_eraseblock(struct nand_device *nand,
					       struct nand_pos *pos)
{
	if (pos->eraseblock >= nand->memorg.eraseblocks_per_lun - 1)
		return nanddev_pos_next_lun(nand, pos);

	pos->eraseblock++;
	pos->page = 0;
	pos->plane = pos->eraseblock % nand->memorg.planes_per_lun;
}

/**
 * nanddev_pos_next_page() - Move a position to the next page
 * @nand: NAND device
 * @pos: the position to update
 *
 * Updates @pos to point to the start of the next page. Useful when you want to
 * iterate over all pages of a NAND device.
 */
static inline void nanddev_pos_next_page(struct nand_device *nand,
					 struct nand_pos *pos)
{
	if (pos->page >= nand->memorg.pages_per_eraseblock - 1)
		return nanddev_pos_next_eraseblock(nand, pos);

	pos->page++;
}

/**
 * nand_io_iter_init - Initialize a NAND I/O iterator
 * @nand: NAND device
 * @offs: absolute offset
 * @req: MTD request
 * @iter: NAND I/O iterator
 *
 * Initializes a NAND iterator based on the information passed by the MTD
 * layer.
 */
static inline void nanddev_io_iter_init(struct nand_device *nand,
					enum nand_page_io_req_type reqtype,
					loff_t offs, struct mtd_oob_ops *req,
					struct nand_io_iter *iter)
{
	struct mtd_info *mtd = nanddev_to_mtd(nand);

	iter->req.type = reqtype;
	iter->req.mode = req->mode;
	iter->req.dataoffs = nanddev_offs_to_pos(nand, offs, &iter->req.pos);
	iter->req.ooboffs = req->ooboffs;
	iter->oobbytes_per_page = mtd_oobavail(mtd, req);
	iter->dataleft = req->len;
	iter->oobleft = req->ooblen;
	iter->req.databuf.in = req->datbuf;
	iter->req.datalen = min_t(unsigned int,
				  nand->memorg.pagesize - iter->req.dataoffs,
				  iter->dataleft);
	iter->req.oobbuf.in = req->oobbuf;
	iter->req.ooblen = min_t(unsigned int,
				 iter->oobbytes_per_page - iter->req.ooboffs,
				 iter->oobleft);
}

/**
 * nand_io_iter_next_page - Move to the next page
 * @nand: NAND device
 * @iter: NAND I/O iterator
 *
 * Updates the @iter to point to the next page.
 */
static inline void nanddev_io_iter_next_page(struct nand_device *nand,
					     struct nand_io_iter *iter)
{
	nanddev_pos_next_page(nand, &iter->req.pos);
	iter->dataleft -= iter->req.datalen;
	iter->req.databuf.in += iter->req.datalen;
	iter->oobleft -= iter->req.ooblen;
	iter->req.oobbuf.in += iter->req.ooblen;
	iter->req.dataoffs = 0;
	iter->req.ooboffs = 0;
	iter->req.datalen = min_t(unsigned int, nand->memorg.pagesize,
				  iter->dataleft);
	iter->req.ooblen = min_t(unsigned int, iter->oobbytes_per_page,
				 iter->oobleft);
}

/**
 * nand_io_iter_end - Should end iteration or not
 * @nand: NAND device
 * @iter: NAND I/O iterator
 *
 * Check whether @iter has reached the end of the NAND portion it was asked to
 * iterate on or not.
 *
 * Return: true if @iter has reached the end of the iteration request, false
 *	   otherwise.
 */
static inline bool nanddev_io_iter_end(struct nand_device *nand,
				       const struct nand_io_iter *iter)
{
	if (iter->dataleft || iter->oobleft)
		return false;

	return true;
}

/**
 * nand_io_for_each_page - Iterate over all NAND pages contained in an MTD I/O
 *			   request
 * @nand: NAND device
 * @start: start address to read/write from
 * @req: MTD I/O request
 * @iter: NAND I/O iterator
 *
 * Should be used for iterate over pages that are contained in an MTD request.
 */
#define nanddev_io_for_each_page(nand, type, start, req, iter)		\
	for (nanddev_io_iter_init(nand, type, start, req, iter);	\
	     !nanddev_io_iter_end(nand, iter);				\
	     nanddev_io_iter_next_page(nand, iter))

bool nanddev_isbad(struct nand_device *nand, const struct nand_pos *pos);
bool nanddev_isreserved(struct nand_device *nand, const struct nand_pos *pos);
int nanddev_markbad(struct nand_device *nand, const struct nand_pos *pos);

/* ECC related functions */
int nanddev_ecc_engine_init(struct nand_device *nand);
void nanddev_ecc_engine_cleanup(struct nand_device *nand);

static inline void *nand_to_ecc_ctx(struct nand_device *nand)
{
	return nand->ecc.ctx.priv;
}

/* BBT related functions */
enum nand_bbt_block_status {
	NAND_BBT_BLOCK_STATUS_UNKNOWN,
	NAND_BBT_BLOCK_GOOD,
	NAND_BBT_BLOCK_WORN,
	NAND_BBT_BLOCK_RESERVED,
	NAND_BBT_BLOCK_FACTORY_BAD,
	NAND_BBT_BLOCK_NUM_STATUS,
};

int nanddev_bbt_init(struct nand_device *nand);
void nanddev_bbt_cleanup(struct nand_device *nand);
int nanddev_bbt_update(struct nand_device *nand);
int nanddev_bbt_get_block_status(const struct nand_device *nand,
				 unsigned int entry);
int nanddev_bbt_set_block_status(struct nand_device *nand, unsigned int entry,
				 enum nand_bbt_block_status status);
int nanddev_bbt_markbad(struct nand_device *nand, unsigned int block);

/**
 * nanddev_bbt_pos_to_entry() - Convert a NAND position into a BBT entry
 * @nand: NAND device
 * @pos: the NAND position we want to get BBT entry for
 *
 * Return the BBT entry used to store information about the eraseblock pointed
 * by @pos.
 *
 * Return: the BBT entry storing information about eraseblock pointed by @pos.
 */
static inline unsigned int nanddev_bbt_pos_to_entry(struct nand_device *nand,
						    const struct nand_pos *pos)
{
	return pos->eraseblock +
	       ((pos->lun + (pos->target * nand->memorg.luns_per_target)) *
		nand->memorg.eraseblocks_per_lun);
}

/**
 * nanddev_bbt_is_initialized() - Check if the BBT has been initialized
 * @nand: NAND device
 *
 * Return: true if the BBT has been initialized, false otherwise.
 */
static inline bool nanddev_bbt_is_initialized(struct nand_device *nand)
{
	return !!nand->bbt.cache;
}

/* MTD -> NAND helper functions. */
int nanddev_mtd_erase(struct mtd_info *mtd, struct erase_info *einfo);
int nanddev_mtd_max_bad_blocks(struct mtd_info *mtd, loff_t offs, size_t len);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_MTD_NAND_H */
