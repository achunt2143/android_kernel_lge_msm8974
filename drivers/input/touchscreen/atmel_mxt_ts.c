<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Atmel maXTouch Touchscreen driver
 *
 * Copyright (C) 2010 Samsung Electronics Co.Ltd
<<<<<<< HEAD
 * Author: Joonyoung Shim <jy0922.shim@samsung.com>
 * Copyright (c) 2011-2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/i2c/atmel_mxt_ts.h>
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <linux/of_gpio.h>

#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>

#elif defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
/* Early-suspend level */
#define MXT_SUSPEND_LEVEL 1
#endif

#if defined(CONFIG_SECURE_TOUCH)
#include <linux/completion.h>
#include <linux/pm_runtime.h>
#include <linux/errno.h>
#include <asm/system.h>
#include <linux/atomic.h>
#endif

/* Family ID */
#define MXT224_ID	0x80
#define MXT224E_ID	0x81
#define MXT336S_ID	0x82
#define MXT1386_ID	0xA0
#define MXT1664S_ID	0xA2

/* Version */
#define MXT_VER_20		20
#define MXT_VER_21		21
#define MXT_VER_22		22

/* I2C slave address pairs */
struct mxt_address_pair {
	int bootloader;
	int application;
};

static const struct mxt_address_pair mxt_slave_addresses[] = {
	{ 0x24, 0x4a },
	{ 0x25, 0x4b },
	{ 0x25, 0x4b },
	{ 0x26, 0x4c },
	{ 0x27, 0x4d },
	{ 0x34, 0x5a },
	{ 0x35, 0x5b },
	{ 0 },
};

enum mxt_device_state { INIT, APPMODE, BOOTLOADER };

/* Firmware */
#define MXT_FW_NAME		"maxtouch.fw"

/* Firmware frame size including frame data and CRC */
#define MXT_SINGLE_FW_MAX_FRAME_SIZE	278
#define MXT_CHIPSET_FW_MAX_FRAME_SIZE	534

/* Registers */
#define MXT_FAMILY_ID		0x00
#define MXT_VARIANT_ID		0x01
#define MXT_VERSION		0x02
#define MXT_BUILD		0x03
#define MXT_MATRIX_X_SIZE	0x04
#define MXT_MATRIX_Y_SIZE	0x05
#define MXT_OBJECT_NUM		0x06
#define MXT_OBJECT_START	0x07

#define MXT_OBJECT_SIZE		6
=======
 * Copyright (C) 2011-2014 Atmel Corporation
 * Copyright (C) 2012 Google, Inc.
 * Copyright (C) 2016 Zodiac Inflight Innovations
 *
 * Author: Joonyoung Shim <jy0922.shim@samsung.com>
 */

#include <linux/acpi.h>
#include <linux/dmi.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/i2c.h>
#include <linux/input/mt.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio/consumer.h>
#include <asm/unaligned.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-v4l2.h>
#include <media/videobuf2-vmalloc.h>
#include <dt-bindings/input/atmel-maxtouch.h>

/* Firmware files */
#define MXT_FW_NAME		"maxtouch.fw"
#define MXT_CFG_NAME		"maxtouch.cfg"
#define MXT_CFG_MAGIC		"OBP_RAW V1"

/* Registers */
#define MXT_OBJECT_START	0x07
#define MXT_OBJECT_SIZE		6
#define MXT_INFO_CHECKSUM_SIZE	3
#define MXT_MAX_BLOCK_WRITE	256
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Object types */
#define MXT_DEBUG_DIAGNOSTIC_T37	37
#define MXT_GEN_MESSAGE_T5		5
#define MXT_GEN_COMMAND_T6		6
#define MXT_GEN_POWER_T7		7
#define MXT_GEN_ACQUIRE_T8		8
#define MXT_GEN_DATASOURCE_T53		53
#define MXT_TOUCH_MULTI_T9		9
#define MXT_TOUCH_KEYARRAY_T15		15
#define MXT_TOUCH_PROXIMITY_T23		23
#define MXT_TOUCH_PROXKEY_T52		52
<<<<<<< HEAD
#define MXT_PROCI_GRIPFACE_T20		20
#define MXT_PROCG_NOISE_T22		22
#define MXT_PROCG_NOISE_T62		62
=======
#define MXT_TOUCH_PTC_KEYS_T97		97
#define MXT_PROCI_GRIPFACE_T20		20
#define MXT_PROCG_NOISE_T22		22
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MXT_PROCI_ONETOUCH_T24		24
#define MXT_PROCI_TWOTOUCH_T27		27
#define MXT_PROCI_GRIP_T40		40
#define MXT_PROCI_PALM_T41		41
#define MXT_PROCI_TOUCHSUPPRESSION_T42	42
#define MXT_PROCI_STYLUS_T47		47
<<<<<<< HEAD
#define MXT_PROCI_ADAPTIVETHRESHOLD_T55 55
#define MXT_PROCI_SHIELDLESS_T56	56
#define MXT_PROCI_EXTRATSDATA_T57	57
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MXT_PROCG_NOISESUPPRESSION_T48	48
#define MXT_SPT_COMMSCONFIG_T18		18
#define MXT_SPT_GPIOPWM_T19		19
#define MXT_SPT_SELFTEST_T25		25
#define MXT_SPT_CTECONFIG_T28		28
#define MXT_SPT_USERDATA_T38		38
#define MXT_SPT_DIGITIZER_T43		43
#define MXT_SPT_MESSAGECOUNT_T44	44
#define MXT_SPT_CTECONFIG_T46		46
<<<<<<< HEAD
#define MXT_SPT_EXTRANOISESUPCTRLS_T58	58
#define MXT_SPT_TIMER_T61		61
=======
#define MXT_SPT_DYNAMICCONFIGURATIONCONTAINER_T71 71
#define MXT_TOUCH_MULTITOUCHSCREEN_T100 100

/* MXT_GEN_MESSAGE_T5 object */
#define MXT_RPTID_NOMSG		0xff
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* MXT_GEN_COMMAND_T6 field */
#define MXT_COMMAND_RESET	0
#define MXT_COMMAND_BACKUPNV	1
#define MXT_COMMAND_CALIBRATE	2
#define MXT_COMMAND_REPORTALL	3
#define MXT_COMMAND_DIAGNOSTIC	5

<<<<<<< HEAD
/* MXT_GEN_POWER_T7 field */
#define MXT_POWER_IDLEACQINT	0
#define MXT_POWER_ACTVACQINT	1
#define MXT_POWER_ACTV2IDLETO	2

/* MXT_GEN_ACQUIRE_T8 field */
#define MXT_ACQUIRE_CHRGTIME	0
#define MXT_ACQUIRE_TCHDRIFT	2
#define MXT_ACQUIRE_DRIFTST	3
#define MXT_ACQUIRE_TCHAUTOCAL	4
#define MXT_ACQUIRE_SYNC	5
#define MXT_ACQUIRE_ATCHCALST	6
#define MXT_ACQUIRE_ATCHCALSTHR	7

/* MXT_TOUCH_MULT_T9 field */
#define MXT_TOUCH_CTRL		0
#define MXT_TOUCH_XORIGIN	1
#define MXT_TOUCH_YORIGIN	2
#define MXT_TOUCH_XSIZE		3
#define MXT_TOUCH_YSIZE		4
#define MXT_TOUCH_BLEN		6
#define MXT_TOUCH_TCHTHR	7
#define MXT_TOUCH_TCHDI		8
#define MXT_TOUCH_ORIENT	9
#define MXT_TOUCH_MOVHYSTI	11
#define MXT_TOUCH_MOVHYSTN	12
#define MXT_TOUCH_NUMTOUCH	14
#define MXT_TOUCH_MRGHYST	15
#define MXT_TOUCH_MRGTHR	16
#define MXT_TOUCH_AMPHYST	17
#define MXT_TOUCH_XRANGE_LSB	18
#define MXT_TOUCH_XRANGE_MSB	19
#define MXT_TOUCH_YRANGE_LSB	20
#define MXT_TOUCH_YRANGE_MSB	21
#define MXT_TOUCH_XLOCLIP	22
#define MXT_TOUCH_XHICLIP	23
#define MXT_TOUCH_YLOCLIP	24
#define MXT_TOUCH_YHICLIP	25
#define MXT_TOUCH_XEDGECTRL	26
#define MXT_TOUCH_XEDGEDIST	27
#define MXT_TOUCH_YEDGECTRL	28
#define MXT_TOUCH_YEDGEDIST	29
#define MXT_TOUCH_JUMPLIMIT	30

/* MXT_PROCI_GRIPFACE_T20 field */
#define MXT_GRIPFACE_CTRL	0
#define MXT_GRIPFACE_XLOGRIP	1
#define MXT_GRIPFACE_XHIGRIP	2
#define MXT_GRIPFACE_YLOGRIP	3
#define MXT_GRIPFACE_YHIGRIP	4
#define MXT_GRIPFACE_MAXTCHS	5
#define MXT_GRIPFACE_SZTHR1	7
#define MXT_GRIPFACE_SZTHR2	8
#define MXT_GRIPFACE_SHPTHR1	9
#define MXT_GRIPFACE_SHPTHR2	10
#define MXT_GRIPFACE_SUPEXTTO	11

/* MXT_PROCI_NOISE field */
#define MXT_NOISE_CTRL		0
#define MXT_NOISE_OUTFLEN	1
#define MXT_NOISE_GCAFUL_LSB	3
#define MXT_NOISE_GCAFUL_MSB	4
#define MXT_NOISE_GCAFLL_LSB	5
#define MXT_NOISE_GCAFLL_MSB	6
#define MXT_NOISE_ACTVGCAFVALID	7
#define MXT_NOISE_NOISETHR	8
#define MXT_NOISE_FREQHOPSCALE	10
#define MXT_NOISE_FREQ0		11
#define MXT_NOISE_FREQ1		12
#define MXT_NOISE_FREQ2		13
#define MXT_NOISE_FREQ3		14
#define MXT_NOISE_FREQ4		15
#define MXT_NOISE_IDLEGCAFVALID	16
=======
/* Define for T6 status byte */
#define MXT_T6_STATUS_RESET	BIT(7)
#define MXT_T6_STATUS_OFL	BIT(6)
#define MXT_T6_STATUS_SIGERR	BIT(5)
#define MXT_T6_STATUS_CAL	BIT(4)
#define MXT_T6_STATUS_CFGERR	BIT(3)
#define MXT_T6_STATUS_COMSERR	BIT(2)

/* MXT_GEN_POWER_T7 field */
struct t7_config {
	u8 idle;
	u8 active;
} __packed;

#define MXT_POWER_CFG_RUN		0
#define MXT_POWER_CFG_DEEPSLEEP		1

/* MXT_TOUCH_MULTI_T9 field */
#define MXT_T9_CTRL		0
#define MXT_T9_XSIZE		3
#define MXT_T9_YSIZE		4
#define MXT_T9_ORIENT		9
#define MXT_T9_RANGE		18

/* MXT_TOUCH_MULTI_T9 status */
#define MXT_T9_UNGRIP		BIT(0)
#define MXT_T9_SUPPRESS		BIT(1)
#define MXT_T9_AMP		BIT(2)
#define MXT_T9_VECTOR		BIT(3)
#define MXT_T9_MOVE		BIT(4)
#define MXT_T9_RELEASE		BIT(5)
#define MXT_T9_PRESS		BIT(6)
#define MXT_T9_DETECT		BIT(7)

struct t9_range {
	__le16 x;
	__le16 y;
} __packed;

/* MXT_TOUCH_MULTI_T9 orient */
#define MXT_T9_ORIENT_SWITCH	BIT(0)
#define MXT_T9_ORIENT_INVERTX	BIT(1)
#define MXT_T9_ORIENT_INVERTY	BIT(2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* MXT_SPT_COMMSCONFIG_T18 */
#define MXT_COMMS_CTRL		0
#define MXT_COMMS_CMD		1
<<<<<<< HEAD

/* MXT_SPT_CTECONFIG_T28 field */
#define MXT_CTE_CTRL		0
#define MXT_CTE_CMD		1
#define MXT_CTE_MODE		2
#define MXT_CTE_IDLEGCAFDEPTH	3
#define MXT_CTE_ACTVGCAFDEPTH	4
#define MXT_CTE_VOLTAGE		5

#define MXT_VOLTAGE_DEFAULT	2700000
#define MXT_VOLTAGE_STEP	10000

/* Analog voltage @2.7 V */
#define MXT_VTG_MIN_UV		2700000
#define MXT_VTG_MAX_UV		3300000
#define MXT_ACTIVE_LOAD_UA	15000
#define MXT_LPM_LOAD_UA		10
/* Digital voltage @1.8 V */
#define MXT_VTG_DIG_MIN_UV	1800000
#define MXT_VTG_DIG_MAX_UV	1800000
#define MXT_ACTIVE_LOAD_DIG_UA	10000
#define MXT_LPM_LOAD_DIG_UA	10

#define MXT_I2C_VTG_MIN_UV	1800000
#define MXT_I2C_VTG_MAX_UV	1800000
#define MXT_I2C_LOAD_UA		10000
#define MXT_I2C_LPM_LOAD_UA	10

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE		0xa5
#define MXT_BACKUP_VALUE	0x55
#define MXT_BACKUP_TIME		25	/* msec */

/* Software reset delay */
#define MXT224_RESET_TIME	64	/* msec */
#define MXT224E_RESET_TIME	21	/* msec */
#define MXT336S_RESET_TIME	25	/* msec */
#define MXT1386_RESET_TIME	250	/* msec */
#define MXT1386E_RESET_TIME	229	/* msec */
#define MXT1664S_RESET_TIME	280	/* msec */
#define MXT_RESET_TIME		250	/* msec */
#define MXT_RESET_NOCHGREAD	400	/* msec */

/* Power on delay */
#define MXT224_POWER_ON_TIME	40	/* msec */
#define MXT224E_POWER_ON_TIME	21	/* msec */
#define MXT336S_POWER_ON_TIME	25	/* msec */
#define MXT1386_POWER_ON_TIME	90	/* msec */
#define MXT1386E_POWER_ON_TIME	81	/* msec */
#define MXT1664S_POWER_ON_TIME	65	/* msec */
#define MXT_POWER_ON_TIME	100	/* msec */

#define MXT_FWRESET_TIME	1000	/* msec */

#define MXT_WAKE_TIME		25
=======
#define MXT_COMMS_RETRIGEN	BIT(6)

/* MXT_DEBUG_DIAGNOSTIC_T37 */
#define MXT_DIAGNOSTIC_PAGEUP	0x01
#define MXT_DIAGNOSTIC_DELTAS	0x10
#define MXT_DIAGNOSTIC_REFS	0x11
#define MXT_DIAGNOSTIC_SIZE	128

#define MXT_FAMILY_1386			160
#define MXT1386_COLUMNS			3
#define MXT1386_PAGES_PER_COLUMN	8

struct t37_debug {
#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT_T37
	u8 mode;
	u8 page;
	u8 data[MXT_DIAGNOSTIC_SIZE];
#endif
};

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE		0xa5
#define MXT_RESET_VALUE		0x01
#define MXT_BACKUP_VALUE	0x55

/* T100 Multiple Touch Touchscreen */
#define MXT_T100_CTRL		0
#define MXT_T100_CFG1		1
#define MXT_T100_TCHAUX		3
#define MXT_T100_XSIZE		9
#define MXT_T100_XRANGE		13
#define MXT_T100_YSIZE		20
#define MXT_T100_YRANGE		24

#define MXT_T100_CFG_SWITCHXY	BIT(5)
#define MXT_T100_CFG_INVERTY	BIT(6)
#define MXT_T100_CFG_INVERTX	BIT(7)

#define MXT_T100_TCHAUX_VECT	BIT(0)
#define MXT_T100_TCHAUX_AMPL	BIT(1)
#define MXT_T100_TCHAUX_AREA	BIT(2)

#define MXT_T100_DETECT		BIT(7)
#define MXT_T100_TYPE_MASK	0x70

enum t100_type {
	MXT_T100_TYPE_FINGER		= 1,
	MXT_T100_TYPE_PASSIVE_STYLUS	= 2,
	MXT_T100_TYPE_HOVERING_FINGER	= 4,
	MXT_T100_TYPE_GLOVE		= 5,
	MXT_T100_TYPE_LARGE_TOUCH	= 6,
};

#define MXT_DISTANCE_ACTIVE_TOUCH	0
#define MXT_DISTANCE_HOVERING		1

#define MXT_TOUCH_MAJOR_DEFAULT		1
#define MXT_PRESSURE_DEFAULT		1

/* Delay times */
#define MXT_BACKUP_TIME		50	/* msec */
#define MXT_RESET_GPIO_TIME	20	/* msec */
#define MXT_RESET_INVALID_CHG	100	/* msec */
#define MXT_RESET_TIME		200	/* msec */
#define MXT_RESET_TIMEOUT	3000	/* msec */
#define MXT_CRC_TIMEOUT		1000	/* msec */
#define MXT_FW_RESET_TIME	3000	/* msec */
#define MXT_FW_CHG_TIMEOUT	300	/* msec */
#define MXT_WAKEUP_TIME		25	/* msec */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Command to unlock bootloader */
#define MXT_UNLOCK_CMD_MSB	0xaa
#define MXT_UNLOCK_CMD_LSB	0xdc

/* Bootloader mode status */
#define MXT_WAITING_BOOTLOAD_CMD	0xc0	/* valid 7 6 bit only */
#define MXT_WAITING_FRAME_DATA	0x80	/* valid 7 6 bit only */
#define MXT_FRAME_CRC_CHECK	0x02
#define MXT_FRAME_CRC_FAIL	0x03
#define MXT_FRAME_CRC_PASS	0x04
#define MXT_APP_CRC_FAIL	0x40	/* valid 7 8 bit only */
#define MXT_BOOT_STATUS_MASK	0x3f
<<<<<<< HEAD
#define MXT_BOOT_EXTENDED_ID	(1 << 5)
#define MXT_BOOT_ID_MASK	0x1f

/* Touch status */
#define MXT_SUPPRESS		(1 << 1)
#define MXT_AMP			(1 << 2)
#define MXT_VECTOR		(1 << 3)
#define MXT_MOVE		(1 << 4)
#define MXT_RELEASE		(1 << 5)
#define MXT_PRESS		(1 << 6)
#define MXT_DETECT		(1 << 7)

/* Touch orient bits */
#define MXT_XY_SWITCH		(1 << 0)
#define MXT_X_INVERT		(1 << 1)
#define MXT_Y_INVERT		(1 << 2)

/* Touch suppression */
#define MXT_TCHSUP_ACTIVE      (1 << 0)

/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

#define MXT_MAX_FINGER		10

#define T7_DATA_SIZE		3
#define MXT_MAX_RW_TRIES	3
#define MXT_BLOCK_SIZE		256
#define MXT_CFG_VERSION_LEN	3
#define MXT_CFG_VERSION_EQUAL	0
#define MXT_CFG_VERSION_LESS	1
#define MXT_CFG_VERSION_GREATER	2

#define MXT_COORDS_ARR_SIZE	4

#define MXT_DEBUGFS_DIR		"atmel_mxt_ts"
#define MXT_DEBUGFS_FILE	"object"
=======
#define MXT_BOOT_EXTENDED_ID	BIT(5)
#define MXT_BOOT_ID_MASK	0x1f

/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

#define MXT_PIXELS_PER_MM	20
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct mxt_info {
	u8 family_id;
	u8 variant_id;
	u8 version;
	u8 build;
	u8 matrix_xsize;
	u8 matrix_ysize;
	u8 object_num;
};

struct mxt_object {
	u8 type;
	u16 start_address;
<<<<<<< HEAD
	u8 size;
	u8 instances;
	u8 num_report_ids;

	/* to map object and message */
	u8 max_reportid;
};

struct mxt_message {
	u8 reportid;
	u8 message[7];
	u8 checksum;
};

struct mxt_finger {
	int status;
	int x;
	int y;
	int area;
	int pressure;
=======
	u8 size_minus_one;
	u8 instances_minus_one;
	u8 num_report_ids;
} __packed;

struct mxt_dbg {
	u16 t37_address;
	u16 diag_cmd_address;
	struct t37_debug *t37_buf;
	unsigned int t37_pages;
	unsigned int t37_nodes;

	struct v4l2_device v4l2;
	struct v4l2_pix_format format;
	struct video_device vdev;
	struct vb2_queue queue;
	struct mutex lock;
	int input;
};

enum v4l_dbg_inputs {
	MXT_V4L_INPUT_DELTAS,
	MXT_V4L_INPUT_REFS,
	MXT_V4L_INPUT_MAX,
};

enum mxt_suspend_mode {
	MXT_SUSPEND_DEEP_SLEEP	= 0,
	MXT_SUSPEND_T9_CTRL	= 1,
};

/* Config update context */
struct mxt_cfg {
	u8 *raw;
	size_t raw_size;
	off_t raw_pos;

	u8 *mem;
	size_t mem_size;
	int start_ofs;

	struct mxt_info info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Each client has this additional data */
struct mxt_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
<<<<<<< HEAD
	const struct mxt_platform_data *pdata;
	const struct mxt_config_info *config_info;
	enum mxt_device_state state;
	struct mxt_object *object_table;
	struct mxt_info info;
	struct mxt_finger finger[MXT_MAX_FINGER];
	unsigned int irq;
	struct regulator *vcc_ana;
	struct regulator *vcc_dig;
	struct regulator *vcc_i2c;
	struct mxt_address_pair addr_pair;
#if defined(CONFIG_FB)
	struct notifier_block fb_notif;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif

	u8 t7_data[T7_DATA_SIZE];
	u16 t7_start_addr;
	u32 keyarray_old;
	u32 keyarray_new;
	u8 t9_max_reportid;
	u8 t9_min_reportid;
	u8 t15_max_reportid;
	u8 t15_min_reportid;
	u8 t42_max_reportid;
	u8 t42_min_reportid;
	u8 cfg_version[MXT_CFG_VERSION_LEN];
	int cfg_version_idx;
	int t38_start_addr;
	bool update_cfg;
	const char *fw_name;
	bool no_force_update;
	bool lpm_support;
	bool dev_sleep;

#if defined(CONFIG_SECURE_TOUCH)
	atomic_t st_enabled;
	atomic_t st_pending_irqs;
	struct completion st_powerdown;
#endif
};

static struct dentry *debug_base;
=======
	char phys[64];		/* device physical location */
	struct mxt_object *object_table;
	struct mxt_info *info;
	void *raw_info_block;
	unsigned int irq;
	unsigned int max_x;
	unsigned int max_y;
	bool invertx;
	bool inverty;
	bool xy_switch;
	u8 xsize;
	u8 ysize;
	bool in_bootloader;
	u16 mem_size;
	u8 t100_aux_ampl;
	u8 t100_aux_area;
	u8 t100_aux_vect;
	u8 max_reportid;
	u32 config_crc;
	u32 info_crc;
	u8 bootloader_addr;
	u8 *msg_buf;
	u8 t6_status;
	bool update_input;
	u8 last_message_count;
	u8 num_touchids;
	u8 multitouch;
	struct t7_config t7_cfg;
	struct mxt_dbg dbg;
	struct regulator_bulk_data regulators[2];
	struct gpio_desc *reset_gpio;
	struct gpio_desc *wake_gpio;
	bool use_retrigen_workaround;

	/* Cached parameters from object table */
	u16 T5_address;
	u8 T5_msg_size;
	u8 T6_reportid;
	u16 T6_address;
	u16 T7_address;
	u16 T71_address;
	u8 T9_reportid_min;
	u8 T9_reportid_max;
	u8 T15_reportid_min;
	u8 T15_reportid_max;
	u16 T18_address;
	u8 T19_reportid;
	u16 T44_address;
	u8 T97_reportid_min;
	u8 T97_reportid_max;
	u8 T100_reportid_min;
	u8 T100_reportid_max;

	/* for fw update in bootloader */
	struct completion bl_completion;

	/* for reset handling */
	struct completion reset_completion;

	/* for config update handling */
	struct completion crc_completion;

	u32 *t19_keymap;
	unsigned int t19_num_keys;

	u32 *t15_keymap;
	unsigned int t15_num_keys;

	enum mxt_suspend_mode suspend_mode;

	u32 wakeup_method;
};

struct mxt_vb2_buffer {
	struct vb2_buffer	vb;
	struct list_head	list;
};

static size_t mxt_obj_size(const struct mxt_object *obj)
{
	return obj->size_minus_one + 1;
}

static size_t mxt_obj_instances(const struct mxt_object *obj)
{
	return obj->instances_minus_one + 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static bool mxt_object_readable(unsigned int type)
{
	switch (type) {
<<<<<<< HEAD
	case MXT_GEN_MESSAGE_T5:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case MXT_GEN_COMMAND_T6:
	case MXT_GEN_POWER_T7:
	case MXT_GEN_ACQUIRE_T8:
	case MXT_GEN_DATASOURCE_T53:
	case MXT_TOUCH_MULTI_T9:
	case MXT_TOUCH_KEYARRAY_T15:
	case MXT_TOUCH_PROXIMITY_T23:
	case MXT_TOUCH_PROXKEY_T52:
<<<<<<< HEAD
	case MXT_PROCI_GRIPFACE_T20:
	case MXT_PROCG_NOISE_T22:
	case MXT_PROCG_NOISE_T62:
=======
	case MXT_TOUCH_PTC_KEYS_T97:
	case MXT_TOUCH_MULTITOUCHSCREEN_T100:
	case MXT_PROCI_GRIPFACE_T20:
	case MXT_PROCG_NOISE_T22:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case MXT_PROCI_ONETOUCH_T24:
	case MXT_PROCI_TWOTOUCH_T27:
	case MXT_PROCI_GRIP_T40:
	case MXT_PROCI_PALM_T41:
	case MXT_PROCI_TOUCHSUPPRESSION_T42:
	case MXT_PROCI_STYLUS_T47:
<<<<<<< HEAD
	case MXT_PROCI_SHIELDLESS_T56:
	case MXT_PROCI_EXTRATSDATA_T57:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case MXT_PROCG_NOISESUPPRESSION_T48:
	case MXT_SPT_COMMSCONFIG_T18:
	case MXT_SPT_GPIOPWM_T19:
	case MXT_SPT_SELFTEST_T25:
	case MXT_SPT_CTECONFIG_T28:
	case MXT_SPT_USERDATA_T38:
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
<<<<<<< HEAD
	case MXT_SPT_EXTRANOISESUPCTRLS_T58:
	case MXT_SPT_TIMER_T61:
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
=======
	case MXT_SPT_DYNAMICCONFIGURATIONCONTAINER_T71:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true;
	default:
		return false;
	}
}

<<<<<<< HEAD
static bool mxt_object_writable(unsigned int type)
{
	switch (type) {
	case MXT_GEN_COMMAND_T6:
	case MXT_GEN_POWER_T7:
	case MXT_GEN_ACQUIRE_T8:
	case MXT_TOUCH_MULTI_T9:
	case MXT_TOUCH_KEYARRAY_T15:
	case MXT_TOUCH_PROXIMITY_T23:
	case MXT_TOUCH_PROXKEY_T52:
	case MXT_PROCI_GRIPFACE_T20:
	case MXT_PROCG_NOISE_T22:
	case MXT_PROCG_NOISE_T62:
	case MXT_PROCI_ONETOUCH_T24:
	case MXT_PROCI_TWOTOUCH_T27:
	case MXT_PROCI_GRIP_T40:
	case MXT_PROCI_PALM_T41:
	case MXT_PROCI_TOUCHSUPPRESSION_T42:
	case MXT_PROCI_STYLUS_T47:
	case MXT_PROCI_SHIELDLESS_T56:
	case MXT_PROCI_EXTRATSDATA_T57:
	case MXT_PROCG_NOISESUPPRESSION_T48:
	case MXT_SPT_COMMSCONFIG_T18:
	case MXT_SPT_GPIOPWM_T19:
	case MXT_SPT_SELFTEST_T25:
	case MXT_SPT_CTECONFIG_T28:
	case MXT_SPT_USERDATA_T38:
	case MXT_SPT_DIGITIZER_T43:
	case MXT_SPT_CTECONFIG_T46:
	case MXT_SPT_EXTRANOISESUPCTRLS_T58:
	case MXT_SPT_TIMER_T61:
	case MXT_PROCI_ADAPTIVETHRESHOLD_T55:
		return true;
	default:
		return false;
	}
}

static void mxt_dump_message(struct device *dev,
				  struct mxt_message *message)
{
	dev_dbg(dev, "reportid:\t0x%x\n", message->reportid);
	dev_dbg(dev, "message1:\t0x%x\n", message->message[0]);
	dev_dbg(dev, "message2:\t0x%x\n", message->message[1]);
	dev_dbg(dev, "message3:\t0x%x\n", message->message[2]);
	dev_dbg(dev, "message4:\t0x%x\n", message->message[3]);
	dev_dbg(dev, "message5:\t0x%x\n", message->message[4]);
	dev_dbg(dev, "message6:\t0x%x\n", message->message[5]);
	dev_dbg(dev, "message7:\t0x%x\n", message->message[6]);
	dev_dbg(dev, "checksum:\t0x%x\n", message->checksum);
}

static int mxt_lookup_bootloader_address(struct mxt_data *data)
{
	int i;

	for (i = 0; mxt_slave_addresses[i].application != 0;  i++) {
		if (mxt_slave_addresses[i].application ==
				data->client->addr) {
			data->addr_pair.bootloader =
				mxt_slave_addresses[i].bootloader;
			return 0;
		}
	}

	dev_err(&data->client->dev, "Address 0x%02x not found in address table",
			data->client->addr);
	return -EINVAL;

};

static int mxt_switch_to_bootloader_address(struct mxt_data *data)
{
	struct i2c_client *client = data->client;

	if (data->state == BOOTLOADER) {
		dev_err(&client->dev, "Already in BOOTLOADER state\n");
		return -EINVAL;
	}

	dev_info(&client->dev, "Changing to bootloader address: 0x%02x -> 0x%02x",
			client->addr, data->addr_pair.bootloader);

	client->addr = data->addr_pair.bootloader;
	data->state = BOOTLOADER;
	return 0;
}

static int mxt_switch_to_appmode_address(struct mxt_data *data)
{
	struct i2c_client *client = data->client;

	if (data->state == APPMODE) {
		dev_err(&client->dev, "Already in APPMODE state\n");
		return -EINVAL;
	}

	dev_info(&client->dev, "Changing to application mode address: " \
			"0x%02x -> 0x%02x", client->addr,
			data->addr_pair.application);

	client->addr = data->addr_pair.application;
	data->state = APPMODE;
	return 0;
}

static int mxt_get_bootloader_version(struct i2c_client *client, u8 val)
{
	u8 buf[3];

	if (val | MXT_BOOT_EXTENDED_ID)	{
		dev_dbg(&client->dev,
				"Retrieving extended mode ID information");

		if (i2c_master_recv(client, &buf[0], 3) != 3) {
			dev_err(&client->dev, "%s: i2c recv failed\n",
								__func__);
			return -EIO;
		}

		dev_info(&client->dev, "Bootloader ID:%d Version:%d",
			buf[1], buf[2]);

		return buf[0];
	} else {
		dev_info(&client->dev, "Bootloader ID:%d",
			val & MXT_BOOT_ID_MASK);
=======
static void mxt_dump_message(struct mxt_data *data, u8 *message)
{
	dev_dbg(&data->client->dev, "message: %*ph\n",
		data->T5_msg_size, message);
}

static int mxt_wait_for_completion(struct mxt_data *data,
				   struct completion *comp,
				   unsigned int timeout_ms)
{
	struct device *dev = &data->client->dev;
	unsigned long timeout = msecs_to_jiffies(timeout_ms);
	long ret;

	ret = wait_for_completion_interruptible_timeout(comp, timeout);
	if (ret < 0) {
		return ret;
	} else if (ret == 0) {
		dev_err(dev, "Wait for completion timed out.\n");
		return -ETIMEDOUT;
	}
	return 0;
}

static int mxt_bootloader_read(struct mxt_data *data,
			       u8 *val, unsigned int count)
{
	int ret;
	struct i2c_msg msg;

	msg.addr = data->bootloader_addr;
	msg.flags = data->client->flags & I2C_M_TEN;
	msg.flags |= I2C_M_RD;
	msg.len = count;
	msg.buf = val;

	ret = i2c_transfer(data->client->adapter, &msg, 1);
	if (ret == 1) {
		ret = 0;
	} else {
		ret = ret < 0 ? ret : -EIO;
		dev_err(&data->client->dev, "%s: i2c recv failed (%d)\n",
			__func__, ret);
	}

	return ret;
}

static int mxt_bootloader_write(struct mxt_data *data,
				const u8 * const val, unsigned int count)
{
	int ret;
	struct i2c_msg msg;

	msg.addr = data->bootloader_addr;
	msg.flags = data->client->flags & I2C_M_TEN;
	msg.len = count;
	msg.buf = (u8 *)val;

	ret = i2c_transfer(data->client->adapter, &msg, 1);
	if (ret == 1) {
		ret = 0;
	} else {
		ret = ret < 0 ? ret : -EIO;
		dev_err(&data->client->dev, "%s: i2c send failed (%d)\n",
			__func__, ret);
	}

	return ret;
}

static int mxt_lookup_bootloader_address(struct mxt_data *data, bool retry)
{
	u8 appmode = data->client->addr;
	u8 bootloader;
	u8 family_id = data->info ? data->info->family_id : 0;

	switch (appmode) {
	case 0x4a:
	case 0x4b:
		/* Chips after 1664S use different scheme */
		if (retry || family_id >= 0xa2) {
			bootloader = appmode - 0x24;
			break;
		}
		fallthrough;	/* for normal case */
	case 0x4c:
	case 0x4d:
	case 0x5a:
	case 0x5b:
		bootloader = appmode - 0x26;
		break;

	default:
		dev_err(&data->client->dev,
			"Appmode i2c address 0x%02x not found\n",
			appmode);
		return -EINVAL;
	}

	data->bootloader_addr = bootloader;
	return 0;
}

static int mxt_probe_bootloader(struct mxt_data *data, bool alt_address)
{
	struct device *dev = &data->client->dev;
	int error;
	u8 val;
	bool crc_failure;

	error = mxt_lookup_bootloader_address(data, alt_address);
	if (error)
		return error;

	error = mxt_bootloader_read(data, &val, 1);
	if (error)
		return error;

	/* Check app crc fail mode */
	crc_failure = (val & ~MXT_BOOT_STATUS_MASK) == MXT_APP_CRC_FAIL;

	dev_err(dev, "Detected bootloader, status:%02X%s\n",
			val, crc_failure ? ", APP_CRC_FAIL" : "");

	return 0;
}

static u8 mxt_get_bootloader_version(struct mxt_data *data, u8 val)
{
	struct device *dev = &data->client->dev;
	u8 buf[3];

	if (val & MXT_BOOT_EXTENDED_ID) {
		if (mxt_bootloader_read(data, &buf[0], 3) != 0) {
			dev_err(dev, "%s: i2c failure\n", __func__);
			return val;
		}

		dev_dbg(dev, "Bootloader ID:%d Version:%d\n", buf[1], buf[2]);

		return buf[0];
	} else {
		dev_dbg(dev, "Bootloader ID:%d\n", val & MXT_BOOT_ID_MASK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		return val;
	}
}

<<<<<<< HEAD
static int mxt_get_bootloader_id(struct i2c_client *client)
{
	u8 val;
	u8 buf[3];

	if (i2c_master_recv(client, &val, 1) != 1) {
		dev_err(&client->dev, "%s: i2c recv failed\n", __func__);
		return -EIO;
	}

	if (val | MXT_BOOT_EXTENDED_ID)	{
		if (i2c_master_recv(client, &buf[0], 3) != 3) {
			dev_err(&client->dev, "%s: i2c recv failed\n",
								__func__);
			return -EIO;
		}
		return buf[1];
	} else {
		dev_info(&client->dev, "Bootloader ID:%d",
			val & MXT_BOOT_ID_MASK);

		return val & MXT_BOOT_ID_MASK;
	}
}

static int mxt_check_bootloader(struct i2c_client *client,
				unsigned int state)
{
	u8 val;

recheck:
	if (i2c_master_recv(client, &val, 1) != 1) {
		dev_err(&client->dev, "%s: i2c recv failed\n", __func__);
		return -EIO;
	}

	switch (state) {
	case MXT_WAITING_BOOTLOAD_CMD:
		val = mxt_get_bootloader_version(client, val);
		val &= ~MXT_BOOT_STATUS_MASK;
		break;
=======
static int mxt_check_bootloader(struct mxt_data *data, unsigned int state,
				bool wait)
{
	struct device *dev = &data->client->dev;
	u8 val;
	int ret;

recheck:
	if (wait) {
		/*
		 * In application update mode, the interrupt
		 * line signals state transitions. We must wait for the
		 * CHG assertion before reading the status byte.
		 * Once the status byte has been read, the line is deasserted.
		 */
		ret = mxt_wait_for_completion(data, &data->bl_completion,
					      MXT_FW_CHG_TIMEOUT);
		if (ret) {
			/*
			 * TODO: handle -ERESTARTSYS better by terminating
			 * fw update process before returning to userspace
			 * by writing length 0x000 to device (iff we are in
			 * WAITING_FRAME_DATA state).
			 */
			dev_err(dev, "Update wait error %d\n", ret);
			return ret;
		}
	}

	ret = mxt_bootloader_read(data, &val, 1);
	if (ret)
		return ret;

	if (state == MXT_WAITING_BOOTLOAD_CMD)
		val = mxt_get_bootloader_version(data, val);

	switch (state) {
	case MXT_WAITING_BOOTLOAD_CMD:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case MXT_WAITING_FRAME_DATA:
	case MXT_APP_CRC_FAIL:
		val &= ~MXT_BOOT_STATUS_MASK;
		break;
	case MXT_FRAME_CRC_PASS:
<<<<<<< HEAD
		if (val == MXT_FRAME_CRC_CHECK)
			goto recheck;
		if (val == MXT_FRAME_CRC_FAIL) {
			dev_err(&client->dev, "Bootloader CRC fail\n");
=======
		if (val == MXT_FRAME_CRC_CHECK) {
			goto recheck;
		} else if (val == MXT_FRAME_CRC_FAIL) {
			dev_err(dev, "Bootloader CRC fail\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		break;
	default:
		return -EINVAL;
	}

	if (val != state) {
<<<<<<< HEAD
		dev_err(&client->dev, "Invalid bootloader mode state %X\n",
			val);
=======
		dev_err(dev, "Invalid bootloader state %02X != %02X\n",
			val, state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int mxt_unlock_bootloader(struct i2c_client *client)
{
	u8 buf[2];

	buf[0] = MXT_UNLOCK_CMD_LSB;
	buf[1] = MXT_UNLOCK_CMD_MSB;

	if (i2c_master_send(client, buf, 2) != 2) {
		dev_err(&client->dev, "%s: i2c send failed\n", __func__);
		return -EIO;
	}

	return 0;
}

static int mxt_fw_write(struct i2c_client *client,
			const u8 *data, unsigned int frame_size)
{
	if (i2c_master_send(client, data, frame_size) != frame_size) {
		dev_err(&client->dev, "%s: i2c send failed\n", __func__);
		return -EIO;
	}

	return 0;
=======
static int mxt_send_bootloader_cmd(struct mxt_data *data, bool unlock)
{
	u8 buf[2];

	if (unlock) {
		buf[0] = MXT_UNLOCK_CMD_LSB;
		buf[1] = MXT_UNLOCK_CMD_MSB;
	} else {
		buf[0] = 0x01;
		buf[1] = 0x01;
	}

	return mxt_bootloader_write(data, buf, sizeof(buf));
}

static bool mxt_wakeup_toggle(struct i2c_client *client,
			      bool wake_up, bool in_i2c)
{
	struct mxt_data *data = i2c_get_clientdata(client);

	switch (data->wakeup_method) {
	case ATMEL_MXT_WAKEUP_I2C_SCL:
		if (!in_i2c)
			return false;
		break;

	case ATMEL_MXT_WAKEUP_GPIO:
		if (in_i2c)
			return false;

		gpiod_set_value(data->wake_gpio, wake_up);
		break;

	default:
		return false;
	}

	if (wake_up) {
		dev_dbg(&client->dev, "waking up controller\n");

		msleep(MXT_WAKEUP_TIME);
	}

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __mxt_read_reg(struct i2c_client *client,
			       u16 reg, u16 len, void *val)
{
	struct i2c_msg xfer[2];
<<<<<<< HEAD
	u8 buf[2];
	int i = 0;
=======
	bool retried = false;
	u8 buf[2];
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;

	/* Write register */
	xfer[0].addr = client->addr;
	xfer[0].flags = 0;
	xfer[0].len = 2;
	xfer[0].buf = buf;

	/* Read data */
	xfer[1].addr = client->addr;
	xfer[1].flags = I2C_M_RD;
	xfer[1].len = len;
	xfer[1].buf = val;

<<<<<<< HEAD
	do {
		if (i2c_transfer(client->adapter, xfer, 2) == 2)
			return 0;
		msleep(MXT_WAKE_TIME);
	} while (++i < MXT_MAX_RW_TRIES);

	dev_err(&client->dev, "%s: i2c transfer failed\n", __func__);
	return -EIO;
}

static int mxt_read_reg(struct i2c_client *client, u16 reg, u8 *val)
{
	return __mxt_read_reg(client, reg, 1, val);
}

static int __mxt_write_reg(struct i2c_client *client,
		    u16 addr, u16 length, u8 *value)
{
	u8 buf[MXT_BLOCK_SIZE + 2];
	int i, tries = 0;

	if (length > MXT_BLOCK_SIZE)
		return -EINVAL;

	buf[0] = addr & 0xff;
	buf[1] = (addr >> 8) & 0xff;
	for (i = 0; i < length; i++)
		buf[i + 2] = *value++;

	do {
		if (i2c_master_send(client, buf, length + 2) == (length + 2))
			return 0;
		msleep(MXT_WAKE_TIME);
	} while (++tries < MXT_MAX_RW_TRIES);

	dev_err(&client->dev, "%s: i2c send failed\n", __func__);
	return -EIO;
=======
retry:
	ret = i2c_transfer(client->adapter, xfer, 2);
	if (ret == 2) {
		ret = 0;
	} else if (!retried && mxt_wakeup_toggle(client, true, true)) {
		retried = true;
		goto retry;
	} else {
		if (ret >= 0)
			ret = -EIO;
		dev_err(&client->dev, "%s: i2c transfer failed (%d)\n",
			__func__, ret);
	}

	return ret;
}

static int __mxt_write_reg(struct i2c_client *client, u16 reg, u16 len,
			   const void *val)
{
	bool retried = false;
	u8 *buf;
	size_t count;
	int ret;

	count = len + 2;
	buf = kmalloc(count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;
	memcpy(&buf[2], val, len);

retry:
	ret = i2c_master_send(client, buf, count);
	if (ret == count) {
		ret = 0;
	} else if (!retried && mxt_wakeup_toggle(client, true, true)) {
		retried = true;
		goto retry;
	} else {
		if (ret >= 0)
			ret = -EIO;
		dev_err(&client->dev, "%s: i2c send failed (%d)\n",
			__func__, ret);
	}

	kfree(buf);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxt_write_reg(struct i2c_client *client, u16 reg, u8 val)
{
	return __mxt_write_reg(client, reg, 1, &val);
}

<<<<<<< HEAD
static int mxt_read_object_table(struct i2c_client *client,
				      u16 reg, u8 *object_buf)
{
	return __mxt_read_reg(client, reg, MXT_OBJECT_SIZE,
				   object_buf);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mxt_object *
mxt_get_object(struct mxt_data *data, u8 type)
{
	struct mxt_object *object;
	int i;

<<<<<<< HEAD
	for (i = 0; i < data->info.object_num; i++) {
=======
	for (i = 0; i < data->info->object_num; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		object = data->object_table + i;
		if (object->type == type)
			return object;
	}

<<<<<<< HEAD
	dev_err(&data->client->dev, "Invalid object type\n");
	return NULL;
}

static int mxt_read_message(struct mxt_data *data,
				 struct mxt_message *message)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, MXT_GEN_MESSAGE_T5);
	if (!object)
		return -EINVAL;

	reg = object->start_address;
	return __mxt_read_reg(data->client, reg,
			sizeof(struct mxt_message), message);
}

static int mxt_read_object(struct mxt_data *data,
				u8 type, u8 offset, u8 *val)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, type);
	if (!object)
		return -EINVAL;

	reg = object->start_address;
	return __mxt_read_reg(data->client, reg + offset, 1, val);
}

static int mxt_get_object_address(struct device *dev, u8 type)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	u8 obj_num, obj_buf[MXT_OBJECT_SIZE];
	u16 reg;
	int i, error;

	error = mxt_read_reg(data->client, MXT_OBJECT_NUM, &obj_num);

	if (error) {
		dev_err(dev, "reading number of objects failed\n");
		return -EINVAL;
	}

	for (i = 0; i < obj_num; i++) {
		reg = MXT_OBJECT_START + MXT_OBJECT_SIZE * i;
		error = mxt_read_object_table(data->client,
						reg, obj_buf);
		if (error)
			return error;

		if (obj_buf[0] == type)
			return obj_buf[2] << 8 | obj_buf[1];
	}
	/* If control reaches here, i = obj_num and object not found */
	dev_err(dev, "Requested object %d not found.\n", type);
	return -EINVAL;

=======
	dev_warn(&data->client->dev, "Invalid object type T%u\n", type);
	return NULL;
}

static void mxt_proc_t6_messages(struct mxt_data *data, u8 *msg)
{
	struct device *dev = &data->client->dev;
	u8 status = msg[1];
	u32 crc = msg[2] | (msg[3] << 8) | (msg[4] << 16);

	if (crc != data->config_crc) {
		data->config_crc = crc;
		dev_dbg(dev, "T6 Config Checksum: 0x%06X\n", crc);
	}

	complete(&data->crc_completion);

	/* Detect reset */
	if (status & MXT_T6_STATUS_RESET)
		complete(&data->reset_completion);

	/* Output debug if status has changed */
	if (status != data->t6_status)
		dev_dbg(dev, "T6 Status 0x%02X%s%s%s%s%s%s%s\n",
			status,
			status == 0 ? " OK" : "",
			status & MXT_T6_STATUS_RESET ? " RESET" : "",
			status & MXT_T6_STATUS_OFL ? " OFL" : "",
			status & MXT_T6_STATUS_SIGERR ? " SIGERR" : "",
			status & MXT_T6_STATUS_CAL ? " CAL" : "",
			status & MXT_T6_STATUS_CFGERR ? " CFGERR" : "",
			status & MXT_T6_STATUS_COMSERR ? " COMSERR" : "");

	/* Save current status */
	data->t6_status = status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxt_write_object(struct mxt_data *data,
				 u8 type, u8 offset, u8 val)
{
	struct mxt_object *object;
	u16 reg;

	object = mxt_get_object(data, type);
<<<<<<< HEAD
	if (!object)
=======
	if (!object || offset >= mxt_obj_size(object))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	reg = object->start_address;
	return mxt_write_reg(data->client, reg + offset, val);
}

<<<<<<< HEAD
static void mxt_input_report(struct mxt_data *data, int single_id)
{
	struct mxt_finger *finger = data->finger;
	struct input_dev *input_dev = data->input_dev;
	int status = finger[single_id].status;
	int finger_num = 0;
	int id;

	for (id = 0; id < MXT_MAX_FINGER; id++) {
		if (!finger[id].status)
			continue;

		input_mt_slot(input_dev, id);
		/* Firmware reports min/max values when the touch is
		 * outside screen area. Send a release event in
		 * such cases to avoid unwanted touches.
		 */
		if (finger[id].x <= data->pdata->panel_minx ||
				finger[id].x >= data->pdata->panel_maxx ||
				finger[id].y <= data->pdata->panel_miny ||
				finger[id].y >= data->pdata->panel_maxy) {
			finger[id].status = MXT_RELEASE;
		}

		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER,
				finger[id].status != MXT_RELEASE);

		if (finger[id].status != MXT_RELEASE) {
			finger_num++;
			input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR,
					finger[id].area);
			input_report_abs(input_dev, ABS_MT_POSITION_X,
					finger[id].x);
			input_report_abs(input_dev, ABS_MT_POSITION_Y,
					finger[id].y);
			input_report_abs(input_dev, ABS_MT_PRESSURE,
					 finger[id].pressure);
		} else {
			finger[id].status = 0;
		}
	}

	input_report_key(input_dev, BTN_TOUCH, finger_num > 0);

	if (finger[single_id].x <= data->pdata->panel_minx ||
		finger[single_id].x >= data->pdata->panel_maxx ||
		finger[single_id].y <= data->pdata->panel_miny ||
		finger[single_id].y >= data->pdata->panel_maxy) {
		status = MXT_RELEASE;
	}

	if ((status != MXT_RELEASE) && status) {
		input_report_abs(input_dev, ABS_X, finger[single_id].x);
		input_report_abs(input_dev, ABS_Y, finger[single_id].y);
		input_report_abs(input_dev,
			ABS_PRESSURE, finger[single_id].pressure);
	}

	input_sync(input_dev);
}

static void mxt_release_all(struct mxt_data *data)
{
	int id;

	for (id = 0; id < MXT_MAX_FINGER; id++)
		if (data->finger[id].status)
			data->finger[id].status = MXT_RELEASE;

	mxt_input_report(data, 0);
}

static void mxt_input_touchevent(struct mxt_data *data,
				      struct mxt_message *message, int id)
{
	struct mxt_finger *finger = data->finger;
	struct device *dev = &data->client->dev;
	u8 status = message->message[0];
	int x;
	int y;
	int area;
	int pressure;

	if (status & MXT_SUPPRESS) {
		mxt_release_all(data);
		return;
	}
	/* Check the touch is present on the screen */
	if (!(status & MXT_DETECT)) {
		if (status & MXT_RELEASE) {
			dev_dbg(dev, "[%d] released\n", id);

			finger[id].status = MXT_RELEASE;
			mxt_input_report(data, id);
		}
		return;
	}

	/* Check only AMP detection */
	if (!(status & (MXT_PRESS | MXT_MOVE)))
		return;

	x = (message->message[1] << 4) | ((message->message[3] >> 4) & 0xf);
	y = (message->message[2] << 4) | ((message->message[3] & 0xf));
	if (data->pdata->panel_maxx < 1024)
		x = x >> 2;
	if (data->pdata->panel_maxy < 1024)
		y = y >> 2;

	area = message->message[4];
	pressure = message->message[5];

	dev_dbg(dev, "[%d] %s x: %d, y: %d, area: %d\n", id,
		status & MXT_MOVE ? "moved" : "pressed",
		x, y, area);

	finger[id].status = status & MXT_MOVE ?
				MXT_MOVE : MXT_PRESS;
	finger[id].x = x;
	finger[id].y = y;
	finger[id].area = area;
	finger[id].pressure = pressure;

	mxt_input_report(data, id);
}

static void mxt_handle_key_array(struct mxt_data *data,
				struct mxt_message *message)
{
	u32 keys_changed;
	int i;

	if (!data->pdata->key_codes) {
		dev_err(&data->client->dev, "keyarray is not supported\n");
		return;
	}

	data->keyarray_new = message->message[1] |
				(message->message[2] << 8) |
				(message->message[3] << 16) |
				(message->message[4] << 24);

	keys_changed = data->keyarray_old ^ data->keyarray_new;

	if (!keys_changed) {
		dev_dbg(&data->client->dev, "no keys changed\n");
		return;
	}

	for (i = 0; i < MXT_KEYARRAY_MAX_KEYS; i++) {
		if (!(keys_changed & (1 << i)))
			continue;

		input_report_key(data->input_dev, data->pdata->key_codes[i],
					(data->keyarray_new & (1 << i)));
		input_sync(data->input_dev);
	}

	data->keyarray_old = data->keyarray_new;
}

static void mxt_handle_touch_suppression(struct mxt_data *data, u8 status)
{
	dev_dbg(&data->client->dev, "touch suppression\n");
	/* release all touches */
	if (status & MXT_TCHSUP_ACTIVE)
		mxt_release_all(data);
}

#if defined(CONFIG_SECURE_TOUCH)
static void mxt_secure_touch_notify(struct mxt_data *data)
{
	sysfs_notify(&data->client->dev.kobj, NULL, "secure_touch");
}

static irqreturn_t mxt_filter_interrupt(struct mxt_data *data)
{
	if (atomic_read(&data->st_enabled)) {
		if (atomic_cmpxchg(&data->st_pending_irqs, 0, 1) == 0)
			mxt_secure_touch_notify(data);
		return IRQ_HANDLED;
	}
	return IRQ_NONE;
}
#else
static irqreturn_t mxt_filter_interrupt(struct mxt_data *data)
{
	return IRQ_NONE;
}
#endif
=======
static void mxt_input_button(struct mxt_data *data, u8 *message)
{
	struct input_dev *input = data->input_dev;
	int i;

	for (i = 0; i < data->t19_num_keys; i++) {
		if (data->t19_keymap[i] == KEY_RESERVED)
			continue;

		/* Active-low switch */
		input_report_key(input, data->t19_keymap[i],
				 !(message[1] & BIT(i)));
	}
}

static void mxt_input_sync(struct mxt_data *data)
{
	input_mt_report_pointer_emulation(data->input_dev,
					  data->t19_num_keys);
	input_sync(data->input_dev);
}

static void mxt_proc_t9_message(struct mxt_data *data, u8 *message)
{
	struct device *dev = &data->client->dev;
	struct input_dev *input_dev = data->input_dev;
	int id;
	u8 status;
	int x;
	int y;
	int area;
	int amplitude;

	id = message[0] - data->T9_reportid_min;
	status = message[1];
	x = (message[2] << 4) | ((message[4] >> 4) & 0xf);
	y = (message[3] << 4) | ((message[4] & 0xf));

	/* Handle 10/12 bit switching */
	if (data->max_x < 1024)
		x >>= 2;
	if (data->max_y < 1024)
		y >>= 2;

	area = message[5];
	amplitude = message[6];

	dev_dbg(dev,
		"[%u] %c%c%c%c%c%c%c%c x: %5u y: %5u area: %3u amp: %3u\n",
		id,
		(status & MXT_T9_DETECT) ? 'D' : '.',
		(status & MXT_T9_PRESS) ? 'P' : '.',
		(status & MXT_T9_RELEASE) ? 'R' : '.',
		(status & MXT_T9_MOVE) ? 'M' : '.',
		(status & MXT_T9_VECTOR) ? 'V' : '.',
		(status & MXT_T9_AMP) ? 'A' : '.',
		(status & MXT_T9_SUPPRESS) ? 'S' : '.',
		(status & MXT_T9_UNGRIP) ? 'U' : '.',
		x, y, area, amplitude);

	input_mt_slot(input_dev, id);

	if (status & MXT_T9_DETECT) {
		/*
		 * Multiple bits may be set if the host is slow to read
		 * the status messages, indicating all the events that
		 * have happened.
		 */
		if (status & MXT_T9_RELEASE) {
			input_mt_report_slot_inactive(input_dev);
			mxt_input_sync(data);
		}

		/* if active, pressure must be non-zero */
		if (!amplitude)
			amplitude = MXT_PRESSURE_DEFAULT;

		/* Touch active */
		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 1);
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_PRESSURE, amplitude);
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, area);
	} else {
		/* Touch no longer active, close out slot */
		input_mt_report_slot_inactive(input_dev);
	}

	data->update_input = true;
}

static void mxt_proc_t15_messages(struct mxt_data *data, u8 *message)
{
	struct input_dev *input_dev = data->input_dev;
	unsigned long keystates = get_unaligned_le32(&message[2]);
	int key;

	for (key = 0; key < data->t15_num_keys; key++)
		input_report_key(input_dev, data->t15_keymap[key],
				 keystates & BIT(key));

	data->update_input = true;
}

static void mxt_proc_t97_messages(struct mxt_data *data, u8 *message)
{
	mxt_proc_t15_messages(data, message);
}

static void mxt_proc_t100_message(struct mxt_data *data, u8 *message)
{
	struct device *dev = &data->client->dev;
	struct input_dev *input_dev = data->input_dev;
	int id;
	u8 status;
	u8 type = 0;
	u16 x;
	u16 y;
	int distance = 0;
	int tool = 0;
	u8 major = 0;
	u8 pressure = 0;
	u8 orientation = 0;

	id = message[0] - data->T100_reportid_min - 2;

	/* ignore SCRSTATUS events */
	if (id < 0)
		return;

	status = message[1];
	x = get_unaligned_le16(&message[2]);
	y = get_unaligned_le16(&message[4]);

	if (status & MXT_T100_DETECT) {
		type = (status & MXT_T100_TYPE_MASK) >> 4;

		switch (type) {
		case MXT_T100_TYPE_HOVERING_FINGER:
			tool = MT_TOOL_FINGER;
			distance = MXT_DISTANCE_HOVERING;

			if (data->t100_aux_vect)
				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_FINGER:
		case MXT_T100_TYPE_GLOVE:
			tool = MT_TOOL_FINGER;
			distance = MXT_DISTANCE_ACTIVE_TOUCH;

			if (data->t100_aux_area)
				major = message[data->t100_aux_area];

			if (data->t100_aux_ampl)
				pressure = message[data->t100_aux_ampl];

			if (data->t100_aux_vect)
				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_PASSIVE_STYLUS:
			tool = MT_TOOL_PEN;

			/*
			 * Passive stylus is reported with size zero so
			 * hardcode.
			 */
			major = MXT_TOUCH_MAJOR_DEFAULT;

			if (data->t100_aux_ampl)
				pressure = message[data->t100_aux_ampl];

			break;

		case MXT_T100_TYPE_LARGE_TOUCH:
			/* Ignore suppressed touch */
			break;

		default:
			dev_dbg(dev, "Unexpected T100 type\n");
			return;
		}
	}

	/*
	 * Values reported should be non-zero if tool is touching the
	 * device
	 */
	if (!pressure && type != MXT_T100_TYPE_HOVERING_FINGER)
		pressure = MXT_PRESSURE_DEFAULT;

	input_mt_slot(input_dev, id);

	if (status & MXT_T100_DETECT) {
		dev_dbg(dev, "[%u] type:%u x:%u y:%u a:%02X p:%02X v:%02X\n",
			id, type, x, y, major, pressure, orientation);

		input_mt_report_slot_state(input_dev, tool, 1);
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, major);
		input_report_abs(input_dev, ABS_MT_PRESSURE, pressure);
		input_report_abs(input_dev, ABS_MT_DISTANCE, distance);
		input_report_abs(input_dev, ABS_MT_ORIENTATION, orientation);
	} else {
		dev_dbg(dev, "[%u] release\n", id);

		/* close out slot */
		input_mt_report_slot_inactive(input_dev);
	}

	data->update_input = true;
}

static int mxt_proc_message(struct mxt_data *data, u8 *message)
{
	u8 report_id = message[0];

	if (report_id == MXT_RPTID_NOMSG)
		return 0;

	if (report_id == data->T6_reportid) {
		mxt_proc_t6_messages(data, message);
	} else if (!data->input_dev) {
		/*
		 * Do not report events if input device
		 * is not yet registered.
		 */
		mxt_dump_message(data, message);
	} else if (report_id >= data->T9_reportid_min &&
		   report_id <= data->T9_reportid_max) {
		mxt_proc_t9_message(data, message);
	} else if (report_id >= data->T15_reportid_min &&
		   report_id <= data->T15_reportid_max) {
		mxt_proc_t15_messages(data, message);
	} else if (report_id >= data->T97_reportid_min &&
		   report_id <= data->T97_reportid_max) {
		mxt_proc_t97_messages(data, message);
	} else if (report_id >= data->T100_reportid_min &&
		   report_id <= data->T100_reportid_max) {
		mxt_proc_t100_message(data, message);
	} else if (report_id == data->T19_reportid) {
		mxt_input_button(data, message);
		data->update_input = true;
	} else {
		mxt_dump_message(data, message);
	}

	return 1;
}

static int mxt_read_and_process_messages(struct mxt_data *data, u8 count)
{
	struct device *dev = &data->client->dev;
	int ret;
	int i;
	u8 num_valid = 0;

	/* Safety check for msg_buf */
	if (count > data->max_reportid)
		return -EINVAL;

	/* Process remaining messages if necessary */
	ret = __mxt_read_reg(data->client, data->T5_address,
				data->T5_msg_size * count, data->msg_buf);
	if (ret) {
		dev_err(dev, "Failed to read %u messages (%d)\n", count, ret);
		return ret;
	}

	for (i = 0;  i < count; i++) {
		ret = mxt_proc_message(data,
			data->msg_buf + data->T5_msg_size * i);

		if (ret == 1)
			num_valid++;
	}

	/* return number of messages read */
	return num_valid;
}

static irqreturn_t mxt_process_messages_t44(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	int ret;
	u8 count, num_left;

	/* Read T44 and T5 together */
	ret = __mxt_read_reg(data->client, data->T44_address,
		data->T5_msg_size + 1, data->msg_buf);
	if (ret) {
		dev_err(dev, "Failed to read T44 and T5 (%d)\n", ret);
		return IRQ_NONE;
	}

	count = data->msg_buf[0];

	/*
	 * This condition may be caused by the CHG line being configured in
	 * Mode 0. It results in unnecessary I2C operations but it is benign.
	 */
	if (count == 0)
		return IRQ_NONE;

	if (count > data->max_reportid) {
		dev_warn(dev, "T44 count %d exceeded max report id\n", count);
		count = data->max_reportid;
	}

	/* Process first message */
	ret = mxt_proc_message(data, data->msg_buf + 1);
	if (ret < 0) {
		dev_warn(dev, "Unexpected invalid message\n");
		return IRQ_NONE;
	}

	num_left = count - 1;

	/* Process remaining messages if necessary */
	if (num_left) {
		ret = mxt_read_and_process_messages(data, num_left);
		if (ret < 0)
			goto end;
		else if (ret != num_left)
			dev_warn(dev, "Unexpected invalid message\n");
	}

end:
	if (data->update_input) {
		mxt_input_sync(data);
		data->update_input = false;
	}

	return IRQ_HANDLED;
}

static int mxt_process_messages_until_invalid(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	int count, read;
	u8 tries = 2;

	count = data->max_reportid;

	/* Read messages until we force an invalid */
	do {
		read = mxt_read_and_process_messages(data, count);
		if (read < count)
			return 0;
	} while (--tries);

	if (data->update_input) {
		mxt_input_sync(data);
		data->update_input = false;
	}

	dev_err(dev, "CHG pin isn't cleared\n");
	return -EBUSY;
}

static irqreturn_t mxt_process_messages(struct mxt_data *data)
{
	int total_handled, num_handled;
	u8 count = data->last_message_count;

	if (count < 1 || count > data->max_reportid)
		count = 1;

	/* include final invalid message */
	total_handled = mxt_read_and_process_messages(data, count + 1);
	if (total_handled < 0)
		return IRQ_NONE;
	/* if there were invalid messages, then we are done */
	else if (total_handled <= count)
		goto update_count;

	/* keep reading two msgs until one is invalid or reportid limit */
	do {
		num_handled = mxt_read_and_process_messages(data, 2);
		if (num_handled < 0)
			return IRQ_NONE;

		total_handled += num_handled;

		if (num_handled < 2)
			break;
	} while (total_handled < data->num_touchids);

update_count:
	data->last_message_count = total_handled;

	if (data->update_input) {
		mxt_input_sync(data);
		data->update_input = false;
	}

	return IRQ_HANDLED;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static irqreturn_t mxt_interrupt(int irq, void *dev_id)
{
	struct mxt_data *data = dev_id;
<<<<<<< HEAD
	struct mxt_message message;
	struct device *dev = &data->client->dev;
	int id;
	u8 reportid;

	if (data->state != APPMODE) {
		dev_err(dev, "Ignoring IRQ - not in APPMODE state\n");
		return IRQ_HANDLED;
	}

	if (IRQ_HANDLED == mxt_filter_interrupt(data))
		goto end;

	do {
		if (mxt_read_message(data, &message)) {
			dev_err(dev, "Failed to read message\n");
			goto end;
		}
		reportid = message.reportid;

		if (!reportid) {
			dev_dbg(dev, "Report id 0 is reserved\n");
			continue;
		}

		id = reportid - data->t9_min_reportid;

		 /* check whether report id is part of T9, T15 or T42 */
		if (reportid >= data->t9_min_reportid &&
					reportid <= data->t9_max_reportid)
			mxt_input_touchevent(data, &message, id);
		else if (reportid >= data->t15_min_reportid &&
					reportid <= data->t15_max_reportid)
			mxt_handle_key_array(data, &message);
		else if (reportid >= data->t42_min_reportid &&
				reportid <= data->t42_max_reportid)
			mxt_handle_touch_suppression(data,
					message.message[0]);
		else
			mxt_dump_message(dev, &message);
	} while (reportid != 0xff);

end:
	return IRQ_HANDLED;
}

static int mxt_check_reg_init(struct mxt_data *data)
{
	const struct mxt_config_info *config_info = data->config_info;
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int index = 0;
	int i, j, config_offset;

	if (!config_info) {
		dev_dbg(dev, "No cfg data defined, skipping reg init\n");
		return 0;
	}

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		if (!mxt_object_writable(object->type))
			continue;

		for (j = 0; j < object->size + 1; j++) {
			config_offset = index + j;
			if (config_offset > config_info->config_length) {
				dev_err(dev, "Not enough config data!\n");
				return -EINVAL;
			}
			mxt_write_object(data, object->type, j,
					 config_info->config[config_offset]);
		}
		index += object->size + 1;
	}

	return 0;
}

static int mxt_make_highchg(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	struct mxt_message message;
	int count = 10;
	int error;

	/* Read dummy message to make high CHG pin */
	do {
		error = mxt_read_message(data, &message);
		if (error)
			return error;
	} while (message.reportid != 0xff && --count);

	if (!count) {
		dev_err(dev, "CHG pin isn't cleared\n");
		return -EBUSY;
	}

	return 0;
}

static int mxt_get_info(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_info *info = &data->info;
	int error;
	u8 val;

	error = mxt_read_reg(client, MXT_FAMILY_ID, &val);
	if (error)
		return error;
	info->family_id = val;

	error = mxt_read_reg(client, MXT_VARIANT_ID, &val);
	if (error)
		return error;
	info->variant_id = val;

	error = mxt_read_reg(client, MXT_VERSION, &val);
	if (error)
		return error;
	info->version = val;

	error = mxt_read_reg(client, MXT_BUILD, &val);
	if (error)
		return error;
	info->build = val;

	error = mxt_read_reg(client, MXT_OBJECT_NUM, &val);
	if (error)
		return error;
	info->object_num = val;

	return 0;
}

static int mxt_get_object_table(struct mxt_data *data)
{
	int error;
	int i;
	u16 reg;
	u8 reportid = 0;
	u8 buf[MXT_OBJECT_SIZE];
	bool found_t38 = false;

	for (i = 0; i < data->info.object_num; i++) {
		struct mxt_object *object = data->object_table + i;

		reg = MXT_OBJECT_START + MXT_OBJECT_SIZE * i;
		error = mxt_read_object_table(data->client, reg, buf);
		if (error)
			return error;

		object->type = buf[0];
		object->start_address = (buf[2] << 8) | buf[1];
		object->size = buf[3];
		object->instances = buf[4];
		object->num_report_ids = buf[5];

		if (object->num_report_ids) {
			reportid += object->num_report_ids *
					(object->instances + 1);
			object->max_reportid = reportid;
		}

		/* Calculate index for config major version in config array.
		 * Major version is the first byte in object T38.
		 */
		if (object->type == MXT_SPT_USERDATA_T38) {
			data->t38_start_addr = object->start_address;
			found_t38 = true;
		}
		if (!found_t38 && mxt_object_writable(object->type))
			data->cfg_version_idx += object->size + 1;
	}

	return 0;
}

static int compare_versions(const u8 *v1, const u8 *v2)
{
	int i;

	if (!v1 || !v2)
		return -EINVAL;

	/* The major version number stays the same across different versions for
	 * a particular controller on a target. The minor and sub-minor version
	 * numbers indicate which version is newer.
	 */
	if (v1[0] != v2[0])
		return -EINVAL;

	for (i = 1; i < MXT_CFG_VERSION_LEN; i++) {
		if (v1[i] > v2[i])
			return MXT_CFG_VERSION_LESS;	/* v2 is older */

		if (v1[i] < v2[i])
			return MXT_CFG_VERSION_GREATER;	/* v2 is newer */
	}

	return MXT_CFG_VERSION_EQUAL;	/* v1 and v2 are equal */
}

static void mxt_check_config_version(struct mxt_data *data,
			const struct mxt_config_info *cfg_info,
			bool match_major,
			const u8 **cfg_version_found,
			bool *found_cfg_major_match)
{
	const u8 *cfg_version;
	int result = -EINVAL;

	cfg_version = cfg_info->config + data->cfg_version_idx;

	if (*cfg_version_found)
		result = compare_versions(*cfg_version_found, cfg_version);

	if (match_major) {
		if (result >= MXT_CFG_VERSION_EQUAL)
			*found_cfg_major_match = true;

		if (result == MXT_CFG_VERSION_EQUAL ||
			result == MXT_CFG_VERSION_GREATER) {
			data->config_info = cfg_info;
			data->fw_name = cfg_info->fw_name;
			*cfg_version_found = cfg_version;
		}

		if (result == MXT_CFG_VERSION_GREATER)
			data->update_cfg = true;
	} else if (!*cfg_version_found || result == MXT_CFG_VERSION_GREATER) {
		data->config_info = cfg_info;
		data->fw_name = cfg_info->fw_name;
		data->update_cfg = true;
		*cfg_version_found = cfg_version;
	}
}

/* If the controller's config version has a non-zero major number, call this
 * function with match_major = true to look for the latest config present in
 * the pdata based on matching family id, variant id, f/w version, build, and
 * config major number. If the controller is programmed with wrong config data
 * previously, call this function with match_major = false to look for latest
 * config based on based on matching family id, variant id, f/w version and
 * build only.
 */
static int mxt_search_config_array(struct mxt_data *data, bool match_major)
{

	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const struct mxt_info *info = &data->info;
	const u8 *cfg_version_found;
	bool found_cfg_major_match = false;
	int i;

	cfg_version_found = match_major ? data->cfg_version : NULL;

	for (i = 0; i < pdata->config_array_size; i++) {

		cfg_info = &pdata->config_array[i];

		if (!cfg_info->config || !cfg_info->config_length)
			continue;

		if (info->family_id == cfg_info->family_id &&
			info->variant_id == cfg_info->variant_id &&
			info->version == cfg_info->version &&
			info->build == cfg_info->build) {

			mxt_check_config_version(data, cfg_info, match_major,
				&cfg_version_found, &found_cfg_major_match);
		}
	}

	if (data->config_info || found_cfg_major_match)
		return 0;

	data->config_info = NULL;
	data->fw_name = NULL;

	return -EINVAL;
}

static int mxt_get_config(struct mxt_data *data)
{
	const struct mxt_platform_data *pdata = data->pdata;
	struct device *dev = &data->client->dev;
	struct mxt_object *object;
	int error;

	if (!pdata->config_array || !pdata->config_array_size) {
		dev_dbg(dev, "No cfg data provided by platform data\n");
		return 0;
	}

	/* Get current config version */
	object = mxt_get_object(data, MXT_SPT_USERDATA_T38);
	if (!object) {
		dev_err(dev, "Unable to obtain USERDATA object\n");
		return -EINVAL;
	}

	error = __mxt_read_reg(data->client, object->start_address,
				sizeof(data->cfg_version), data->cfg_version);
	if (error) {
		dev_err(dev, "Unable to read config version\n");
		return error;
	}
	dev_info(dev, "Current config version on the controller is %d.%d.%d\n",
			data->cfg_version[0], data->cfg_version[1],
			data->cfg_version[2]);

	/* configuration update requires major match */
	error = mxt_search_config_array(data, true);

	/* if no_force_update is false , try again with false
	as the second parameter to mxt_search_config_array */
	if (error && (data->no_force_update == false))
		error = mxt_search_config_array(data, false);

	if (error) {
		dev_err(dev,
			"Unable to find matching config in pdata\n");
		return error;
	}

	return 0;
}

static void mxt_power_on_delay(struct mxt_data *data)
{
	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	u32 delay = 0;
	int i;

	for (i = 0; i < pdata->config_array_size; i++) {
		cfg_info = &pdata->config_array[i];

		switch (cfg_info->family_id) {
		case MXT224_ID:
			delay = max_t(u32, delay, MXT224_POWER_ON_TIME);
			break;
		case MXT224E_ID:
			delay = max_t(u32, delay, MXT224E_POWER_ON_TIME);
			break;
		case MXT336S_ID:
			delay = max_t(u32, delay, MXT336S_POWER_ON_TIME);
			break;
		case MXT1386_ID:
			delay = max_t(u32, delay,
					max_t(u32, MXT1386_POWER_ON_TIME,
						MXT1386E_POWER_ON_TIME));
			break;
		case MXT1664S_ID:
			delay = max_t(u32, delay, MXT1664S_POWER_ON_TIME);
			break;
		default:
			delay = max_t(u32, delay, MXT_POWER_ON_TIME);
		}
	}

	msleep(delay);
}

static void mxt_reset_delay(struct mxt_data *data)
{
	struct mxt_info *info = &data->info;

	switch (info->family_id) {
	case MXT224_ID:
		msleep(MXT224_RESET_TIME);
		break;
	case MXT224E_ID:
		msleep(MXT224E_RESET_TIME);
		break;
	case MXT336S_ID:
		msleep(MXT336S_RESET_TIME);
		break;
	case MXT1386_ID:
		msleep(max_t(u32, MXT1386_RESET_TIME, MXT1386E_RESET_TIME));
		break;
	case MXT1664S_ID:
		msleep(MXT1664S_RESET_TIME);
		break;
	default:
		msleep(MXT_RESET_TIME);
	}
}

static int mxt_backup_nv(struct mxt_data *data)
{
	int error;
	u8 command_register;
	int timeout_counter = 0;

	/* Backup to memory */
	mxt_write_object(data, MXT_GEN_COMMAND_T6,
			MXT_COMMAND_BACKUPNV,
			MXT_BACKUP_VALUE);
	msleep(MXT_BACKUP_TIME);

	do {
		error = mxt_read_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_BACKUPNV,
					&command_register);
		if (error)
			return error;

		usleep_range(1000, 2000);

	} while ((command_register != 0) && (++timeout_counter <= 100));

	if (timeout_counter > 100) {
		dev_err(&data->client->dev, "No response after backup!\n");
		return -EIO;
	}

	/* Soft reset */
	mxt_write_object(data, MXT_GEN_COMMAND_T6, MXT_COMMAND_RESET, 1);

	mxt_reset_delay(data);
=======

	if (data->in_bootloader) {
		/* bootloader state transition completion */
		complete(&data->bl_completion);
		return IRQ_HANDLED;
	}

	if (!data->object_table)
		return IRQ_HANDLED;

	if (data->T44_address) {
		return mxt_process_messages_t44(data);
	} else {
		return mxt_process_messages(data);
	}
}

static int mxt_t6_command(struct mxt_data *data, u16 cmd_offset,
			  u8 value, bool wait)
{
	u16 reg;
	u8 command_register;
	int timeout_counter = 0;
	int ret;

	reg = data->T6_address + cmd_offset;

	ret = mxt_write_reg(data->client, reg, value);
	if (ret)
		return ret;

	if (!wait)
		return 0;

	do {
		msleep(20);
		ret = __mxt_read_reg(data->client, reg, 1, &command_register);
		if (ret)
			return ret;
	} while (command_register != 0 && timeout_counter++ <= 100);

	if (timeout_counter > 100) {
		dev_err(&data->client->dev, "Command failed!\n");
		return -EIO;
	}

	return 0;
}

static int mxt_acquire_irq(struct mxt_data *data)
{
	int error;

	enable_irq(data->irq);

	if (data->use_retrigen_workaround) {
		error = mxt_process_messages_until_invalid(data);
		if (error)
			return error;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int mxt_save_objects(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	struct mxt_object *t7_object;
	struct mxt_object *t9_object;
	struct mxt_object *t15_object;
	struct mxt_object *t42_object;
	int error;

	/* Store T7 and T9 locally, used in suspend/resume operations */
	t7_object = mxt_get_object(data, MXT_GEN_POWER_T7);
	if (!t7_object) {
		dev_err(&client->dev, "Failed to get T7 object\n");
		return -EINVAL;
	}

	data->t7_start_addr = t7_object->start_address;
	error = __mxt_read_reg(client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&client->dev,
			"Failed to save current power state\n");
		return error;
	}

	/* Store T9, T15's min and max report ids */
	t9_object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
	if (!t9_object) {
		dev_err(&client->dev, "Failed to get T9 object\n");
		return -EINVAL;
	}
	data->t9_max_reportid = t9_object->max_reportid;
	data->t9_min_reportid = t9_object->max_reportid -
					(t9_object->num_report_ids *
					(t9_object->instances + 1)) + 1;

	if (data->pdata->key_codes) {
		t15_object = mxt_get_object(data, MXT_TOUCH_KEYARRAY_T15);
		if (!t15_object)
			dev_dbg(&client->dev, "T15 object is not available\n");
		else {
			data->t15_max_reportid = t15_object->max_reportid;
			data->t15_min_reportid = t15_object->max_reportid -
					(t15_object->num_report_ids *
					(t15_object->instances + 1)) + 1;
		}
	}

	/* Store T42 min and max report ids */
	t42_object = mxt_get_object(data, MXT_PROCI_TOUCHSUPPRESSION_T42);
	if (!t42_object)
		dev_dbg(&client->dev, "T42 object is not available\n");
	else {
		data->t42_max_reportid = t42_object->max_reportid;
		data->t42_min_reportid = t42_object->max_reportid -
					(t42_object->num_report_ids *
					(t42_object->instances + 1)) + 1;
=======
static int mxt_soft_reset(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	int ret = 0;

	dev_info(dev, "Resetting device\n");

	disable_irq(data->irq);

	reinit_completion(&data->reset_completion);

	ret = mxt_t6_command(data, MXT_COMMAND_RESET, MXT_RESET_VALUE, false);
	if (ret)
		return ret;

	/* Ignore CHG line for 100ms after reset */
	msleep(MXT_RESET_INVALID_CHG);

	mxt_acquire_irq(data);

	ret = mxt_wait_for_completion(data, &data->reset_completion,
				      MXT_RESET_TIMEOUT);
	if (ret)
		return ret;

	return 0;
}

static void mxt_update_crc(struct mxt_data *data, u8 cmd, u8 value)
{
	/*
	 * On failure, CRC is set to 0 and config will always be
	 * downloaded.
	 */
	data->config_crc = 0;
	reinit_completion(&data->crc_completion);

	mxt_t6_command(data, cmd, value, true);

	/*
	 * Wait for crc message. On failure, CRC is set to 0 and config will
	 * always be downloaded.
	 */
	mxt_wait_for_completion(data, &data->crc_completion, MXT_CRC_TIMEOUT);
}

static void mxt_calc_crc24(u32 *crc, u8 firstbyte, u8 secondbyte)
{
	static const unsigned int crcpoly = 0x80001B;
	u32 result;
	u32 data_word;

	data_word = (secondbyte << 8) | firstbyte;
	result = ((*crc << 1) ^ data_word);

	if (result & 0x1000000)
		result ^= crcpoly;

	*crc = result;
}

static u32 mxt_calculate_crc(u8 *base, off_t start_off, off_t end_off)
{
	u32 crc = 0;
	u8 *ptr = base + start_off;
	u8 *last_val = base + end_off - 1;

	if (end_off < start_off)
		return -EINVAL;

	while (ptr < last_val) {
		mxt_calc_crc24(&crc, *ptr, *(ptr + 1));
		ptr += 2;
	}

	/* if len is odd, fill the last byte with 0 */
	if (ptr == last_val)
		mxt_calc_crc24(&crc, *ptr, 0);

	/* Mask to 24-bit */
	crc &= 0x00FFFFFF;

	return crc;
}

static int mxt_check_retrigen(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	int val;
	struct irq_data *irqd;

	data->use_retrigen_workaround = false;

	irqd = irq_get_irq_data(data->irq);
	if (!irqd)
		return -EINVAL;

	if (irqd_is_level_type(irqd))
		return 0;

	if (data->T18_address) {
		error = __mxt_read_reg(client,
				       data->T18_address + MXT_COMMS_CTRL,
				       1, &val);
		if (error)
			return error;

		if (val & MXT_COMMS_RETRIGEN)
			return 0;
	}

	dev_warn(&client->dev, "Enabling RETRIGEN workaround\n");
	data->use_retrigen_workaround = true;
	return 0;
}

static int mxt_prepare_cfg_mem(struct mxt_data *data, struct mxt_cfg *cfg)
{
	struct device *dev = &data->client->dev;
	struct mxt_object *object;
	unsigned int type, instance, size, byte_offset;
	int offset;
	int ret;
	int i;
	u16 reg;
	u8 val;

	while (cfg->raw_pos < cfg->raw_size) {
		/* Read type, instance, length */
		ret = sscanf(cfg->raw + cfg->raw_pos, "%x %x %x%n",
			     &type, &instance, &size, &offset);
		if (ret == 0) {
			/* EOF */
			break;
		} else if (ret != 3) {
			dev_err(dev, "Bad format: failed to parse object\n");
			return -EINVAL;
		}
		cfg->raw_pos += offset;

		object = mxt_get_object(data, type);
		if (!object) {
			/* Skip object */
			for (i = 0; i < size; i++) {
				ret = sscanf(cfg->raw + cfg->raw_pos, "%hhx%n",
					     &val, &offset);
				if (ret != 1) {
					dev_err(dev, "Bad format in T%d at %d\n",
						type, i);
					return -EINVAL;
				}
				cfg->raw_pos += offset;
			}
			continue;
		}

		if (size > mxt_obj_size(object)) {
			/*
			 * Either we are in fallback mode due to wrong
			 * config or config from a later fw version,
			 * or the file is corrupt or hand-edited.
			 */
			dev_warn(dev, "Discarding %zu byte(s) in T%u\n",
				 size - mxt_obj_size(object), type);
		} else if (mxt_obj_size(object) > size) {
			/*
			 * If firmware is upgraded, new bytes may be added to
			 * end of objects. It is generally forward compatible
			 * to zero these bytes - previous behaviour will be
			 * retained. However this does invalidate the CRC and
			 * will force fallback mode until the configuration is
			 * updated. We warn here but do nothing else - the
			 * malloc has zeroed the entire configuration.
			 */
			dev_warn(dev, "Zeroing %zu byte(s) in T%d\n",
				 mxt_obj_size(object) - size, type);
		}

		if (instance >= mxt_obj_instances(object)) {
			dev_err(dev, "Object instances exceeded!\n");
			return -EINVAL;
		}

		reg = object->start_address + mxt_obj_size(object) * instance;

		for (i = 0; i < size; i++) {
			ret = sscanf(cfg->raw + cfg->raw_pos, "%hhx%n",
				     &val,
				     &offset);
			if (ret != 1) {
				dev_err(dev, "Bad format in T%d at %d\n",
					type, i);
				return -EINVAL;
			}
			cfg->raw_pos += offset;

			if (i > mxt_obj_size(object))
				continue;

			byte_offset = reg + i - cfg->start_ofs;

			if (byte_offset >= 0 && byte_offset < cfg->mem_size) {
				*(cfg->mem + byte_offset) = val;
			} else {
				dev_err(dev, "Bad object: reg:%d, T%d, ofs=%d\n",
					reg, object->type, byte_offset);
				return -EINVAL;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int mxt_update_cfg(struct mxt_data *data)
{
	int error;
	const u8 *cfg_ver;

	/* Get config data from platform data */
	error = mxt_get_config(data);
	if (error)
		dev_dbg(&data->client->dev, "Config info not found.\n");

	/* Check register init values */
	if (data->config_info && data->config_info->config) {
		if (data->update_cfg) {
			error = mxt_check_reg_init(data);
			if (error) {
				dev_err(&data->client->dev,
					"Failed to check reg init value\n");
				return error;
			}

			error = mxt_backup_nv(data);
			if (error) {
				dev_err(&data->client->dev, "Failed to back up NV\n");
				return error;
			}

			cfg_ver = data->config_info->config +
						data->cfg_version_idx;
			dev_info(&data->client->dev,
				"Config updated from %d.%d.%d to %d.%d.%d\n",
				data->cfg_version[0], data->cfg_version[1],
				data->cfg_version[2],
				cfg_ver[0], cfg_ver[1], cfg_ver[2]);

			memcpy(data->cfg_version, cfg_ver, MXT_CFG_VERSION_LEN);
		}
	} else {
		dev_info(&data->client->dev,
			"No cfg data defined, skipping check reg init\n");
	}

	error = mxt_save_objects(data);
	if (error)
		return error;
=======
static int mxt_upload_cfg_mem(struct mxt_data *data, struct mxt_cfg *cfg)
{
	unsigned int byte_offset = 0;
	int error;

	/* Write configuration as blocks */
	while (byte_offset < cfg->mem_size) {
		unsigned int size = cfg->mem_size - byte_offset;

		if (size > MXT_MAX_BLOCK_WRITE)
			size = MXT_MAX_BLOCK_WRITE;

		error = __mxt_write_reg(data->client,
					cfg->start_ofs + byte_offset,
					size, cfg->mem + byte_offset);
		if (error) {
			dev_err(&data->client->dev,
				"Config write error, ret=%d\n", error);
			return error;
		}

		byte_offset += size;
	}

	return 0;
}

static int mxt_init_t7_power_cfg(struct mxt_data *data);

/*
 * mxt_update_cfg - download configuration to chip
 *
 * Atmel Raw Config File Format
 *
 * The first four lines of the raw config file contain:
 *  1) Version
 *  2) Chip ID Information (first 7 bytes of device memory)
 *  3) Chip Information Block 24-bit CRC Checksum
 *  4) Chip Configuration 24-bit CRC Checksum
 *
 * The rest of the file consists of one line per object instance:
 *   <TYPE> <INSTANCE> <SIZE> <CONTENTS>
 *
 *   <TYPE> - 2-byte object type as hex
 *   <INSTANCE> - 2-byte object instance number as hex
 *   <SIZE> - 2-byte object size as hex
 *   <CONTENTS> - array of <SIZE> 1-byte hex values
 */
static int mxt_update_cfg(struct mxt_data *data, const struct firmware *fw)
{
	struct device *dev = &data->client->dev;
	struct mxt_cfg cfg;
	int ret;
	int offset;
	int i;
	u32 info_crc, config_crc, calculated_crc;
	u16 crc_start = 0;

	/* Make zero terminated copy of the OBP_RAW file */
	cfg.raw = kmemdup_nul(fw->data, fw->size, GFP_KERNEL);
	if (!cfg.raw)
		return -ENOMEM;

	cfg.raw_size = fw->size;

	mxt_update_crc(data, MXT_COMMAND_REPORTALL, 1);

	if (strncmp(cfg.raw, MXT_CFG_MAGIC, strlen(MXT_CFG_MAGIC))) {
		dev_err(dev, "Unrecognised config file\n");
		ret = -EINVAL;
		goto release_raw;
	}

	cfg.raw_pos = strlen(MXT_CFG_MAGIC);

	/* Load information block and check */
	for (i = 0; i < sizeof(struct mxt_info); i++) {
		ret = sscanf(cfg.raw + cfg.raw_pos, "%hhx%n",
			     (unsigned char *)&cfg.info + i,
			     &offset);
		if (ret != 1) {
			dev_err(dev, "Bad format\n");
			ret = -EINVAL;
			goto release_raw;
		}

		cfg.raw_pos += offset;
	}

	if (cfg.info.family_id != data->info->family_id) {
		dev_err(dev, "Family ID mismatch!\n");
		ret = -EINVAL;
		goto release_raw;
	}

	if (cfg.info.variant_id != data->info->variant_id) {
		dev_err(dev, "Variant ID mismatch!\n");
		ret = -EINVAL;
		goto release_raw;
	}

	/* Read CRCs */
	ret = sscanf(cfg.raw + cfg.raw_pos, "%x%n", &info_crc, &offset);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Info CRC\n");
		ret = -EINVAL;
		goto release_raw;
	}
	cfg.raw_pos += offset;

	ret = sscanf(cfg.raw + cfg.raw_pos, "%x%n", &config_crc, &offset);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Config CRC\n");
		ret = -EINVAL;
		goto release_raw;
	}
	cfg.raw_pos += offset;

	/*
	 * The Info Block CRC is calculated over mxt_info and the object
	 * table. If it does not match then we are trying to load the
	 * configuration from a different chip or firmware version, so
	 * the configuration CRC is invalid anyway.
	 */
	if (info_crc == data->info_crc) {
		if (config_crc == 0 || data->config_crc == 0) {
			dev_info(dev, "CRC zero, attempting to apply config\n");
		} else if (config_crc == data->config_crc) {
			dev_dbg(dev, "Config CRC 0x%06X: OK\n",
				 data->config_crc);
			ret = 0;
			goto release_raw;
		} else {
			dev_info(dev, "Config CRC 0x%06X: does not match file 0x%06X\n",
				 data->config_crc, config_crc);
		}
	} else {
		dev_warn(dev,
			 "Warning: Info CRC error - device=0x%06X file=0x%06X\n",
			 data->info_crc, info_crc);
	}

	/* Malloc memory to store configuration */
	cfg.start_ofs = MXT_OBJECT_START +
			data->info->object_num * sizeof(struct mxt_object) +
			MXT_INFO_CHECKSUM_SIZE;
	cfg.mem_size = data->mem_size - cfg.start_ofs;
	cfg.mem = kzalloc(cfg.mem_size, GFP_KERNEL);
	if (!cfg.mem) {
		ret = -ENOMEM;
		goto release_raw;
	}

	ret = mxt_prepare_cfg_mem(data, &cfg);
	if (ret)
		goto release_mem;

	/* Calculate crc of the received configs (not the raw config file) */
	if (data->T71_address)
		crc_start = data->T71_address;
	else if (data->T7_address)
		crc_start = data->T7_address;
	else
		dev_warn(dev, "Could not find CRC start\n");

	if (crc_start > cfg.start_ofs) {
		calculated_crc = mxt_calculate_crc(cfg.mem,
						   crc_start - cfg.start_ofs,
						   cfg.mem_size);

		if (config_crc > 0 && config_crc != calculated_crc)
			dev_warn(dev, "Config CRC in file inconsistent, calculated=%06X, file=%06X\n",
				 calculated_crc, config_crc);
	}

	ret = mxt_upload_cfg_mem(data, &cfg);
	if (ret)
		goto release_mem;

	mxt_update_crc(data, MXT_COMMAND_BACKUPNV, MXT_BACKUP_VALUE);

	ret = mxt_check_retrigen(data);
	if (ret)
		goto release_mem;

	ret = mxt_soft_reset(data);
	if (ret)
		goto release_mem;

	dev_info(dev, "Config successfully updated\n");

	/* T7 config may have changed */
	mxt_init_t7_power_cfg(data);

release_mem:
	kfree(cfg.mem);
release_raw:
	kfree(cfg.raw);
	return ret;
}

static void mxt_free_input_device(struct mxt_data *data)
{
	if (data->input_dev) {
		input_unregister_device(data->input_dev);
		data->input_dev = NULL;
	}
}

static void mxt_free_object_table(struct mxt_data *data)
{
#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT_T37
	video_unregister_device(&data->dbg.vdev);
	v4l2_device_unregister(&data->dbg.v4l2);
#endif
	data->object_table = NULL;
	data->info = NULL;
	kfree(data->raw_info_block);
	data->raw_info_block = NULL;
	kfree(data->msg_buf);
	data->msg_buf = NULL;
	data->T5_address = 0;
	data->T5_msg_size = 0;
	data->T6_reportid = 0;
	data->T7_address = 0;
	data->T71_address = 0;
	data->T9_reportid_min = 0;
	data->T9_reportid_max = 0;
	data->T15_reportid_min = 0;
	data->T15_reportid_max = 0;
	data->T18_address = 0;
	data->T19_reportid = 0;
	data->T44_address = 0;
	data->T97_reportid_min = 0;
	data->T97_reportid_max = 0;
	data->T100_reportid_min = 0;
	data->T100_reportid_max = 0;
	data->max_reportid = 0;
}

static int mxt_parse_object_table(struct mxt_data *data,
				  struct mxt_object *object_table)
{
	struct i2c_client *client = data->client;
	int i;
	u8 reportid;
	u16 end_address;

	/* Valid Report IDs start counting from 1 */
	reportid = 1;
	data->mem_size = 0;
	for (i = 0; i < data->info->object_num; i++) {
		struct mxt_object *object = object_table + i;
		u8 min_id, max_id;

		le16_to_cpus(&object->start_address);

		if (object->num_report_ids) {
			min_id = reportid;
			reportid += object->num_report_ids *
					mxt_obj_instances(object);
			max_id = reportid - 1;
		} else {
			min_id = 0;
			max_id = 0;
		}

		dev_dbg(&data->client->dev,
			"T%u Start:%u Size:%zu Instances:%zu Report IDs:%u-%u\n",
			object->type, object->start_address,
			mxt_obj_size(object), mxt_obj_instances(object),
			min_id, max_id);

		switch (object->type) {
		case MXT_GEN_MESSAGE_T5:
			if (data->info->family_id == 0x80 &&
			    data->info->version < 0x20) {
				/*
				 * On mXT224 firmware versions prior to V2.0
				 * read and discard unused CRC byte otherwise
				 * DMA reads are misaligned.
				 */
				data->T5_msg_size = mxt_obj_size(object);
			} else {
				/* CRC not enabled, so skip last byte */
				data->T5_msg_size = mxt_obj_size(object) - 1;
			}
			data->T5_address = object->start_address;
			break;
		case MXT_GEN_COMMAND_T6:
			data->T6_reportid = min_id;
			data->T6_address = object->start_address;
			break;
		case MXT_GEN_POWER_T7:
			data->T7_address = object->start_address;
			break;
		case MXT_SPT_DYNAMICCONFIGURATIONCONTAINER_T71:
			data->T71_address = object->start_address;
			break;
		case MXT_TOUCH_MULTI_T9:
			data->multitouch = MXT_TOUCH_MULTI_T9;
			/* Only handle messages from first T9 instance */
			data->T9_reportid_min = min_id;
			data->T9_reportid_max = min_id +
						object->num_report_ids - 1;
			data->num_touchids = object->num_report_ids;
			break;
		case MXT_TOUCH_KEYARRAY_T15:
			data->T15_reportid_min = min_id;
			data->T15_reportid_max = max_id;
			break;
		case MXT_SPT_COMMSCONFIG_T18:
			data->T18_address = object->start_address;
			break;
		case MXT_SPT_MESSAGECOUNT_T44:
			data->T44_address = object->start_address;
			break;
		case MXT_SPT_GPIOPWM_T19:
			data->T19_reportid = min_id;
			break;
		case MXT_TOUCH_PTC_KEYS_T97:
			data->T97_reportid_min = min_id;
			data->T97_reportid_max = max_id;
			break;
		case MXT_TOUCH_MULTITOUCHSCREEN_T100:
			data->multitouch = MXT_TOUCH_MULTITOUCHSCREEN_T100;
			data->T100_reportid_min = min_id;
			data->T100_reportid_max = max_id;
			/* first two report IDs reserved */
			data->num_touchids = object->num_report_ids - 2;
			break;
		}

		end_address = object->start_address
			+ mxt_obj_size(object) * mxt_obj_instances(object) - 1;

		if (end_address >= data->mem_size)
			data->mem_size = end_address + 1;
	}

	/* Store maximum reportid */
	data->max_reportid = reportid;

	/* If T44 exists, T5 position has to be directly after */
	if (data->T44_address && (data->T5_address != data->T44_address + 1)) {
		dev_err(&client->dev, "Invalid T44 position\n");
		return -EINVAL;
	}

	data->msg_buf = kcalloc(data->max_reportid,
				data->T5_msg_size, GFP_KERNEL);
	if (!data->msg_buf)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

=======
static int mxt_read_info_block(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	size_t size;
	void *id_buf, *buf;
	uint8_t num_objects;
	u32 calculated_crc;
	u8 *crc_ptr;

	/* If info block already allocated, free it */
	if (data->raw_info_block)
		mxt_free_object_table(data);

	/* Read 7-byte ID information block starting at address 0 */
	size = sizeof(struct mxt_info);
	id_buf = kzalloc(size, GFP_KERNEL);
	if (!id_buf)
		return -ENOMEM;

	error = __mxt_read_reg(client, 0, size, id_buf);
	if (error)
		goto err_free_mem;

	/* Resize buffer to give space for rest of info block */
	num_objects = ((struct mxt_info *)id_buf)->object_num;
	size += (num_objects * sizeof(struct mxt_object))
		+ MXT_INFO_CHECKSUM_SIZE;

	buf = krealloc(id_buf, size, GFP_KERNEL);
	if (!buf) {
		error = -ENOMEM;
		goto err_free_mem;
	}
	id_buf = buf;

	/* Read rest of info block */
	error = __mxt_read_reg(client, MXT_OBJECT_START,
			       size - MXT_OBJECT_START,
			       id_buf + MXT_OBJECT_START);
	if (error)
		goto err_free_mem;

	/* Extract & calculate checksum */
	crc_ptr = id_buf + size - MXT_INFO_CHECKSUM_SIZE;
	data->info_crc = crc_ptr[0] | (crc_ptr[1] << 8) | (crc_ptr[2] << 16);

	calculated_crc = mxt_calculate_crc(id_buf, 0,
					   size - MXT_INFO_CHECKSUM_SIZE);

	/*
	 * CRC mismatch can be caused by data corruption due to I2C comms
	 * issue or else device is not using Object Based Protocol (eg i2c-hid)
	 */
	if ((data->info_crc == 0) || (data->info_crc != calculated_crc)) {
		dev_err(&client->dev,
			"Info Block CRC error calculated=0x%06X read=0x%06X\n",
			calculated_crc, data->info_crc);
		error = -EIO;
		goto err_free_mem;
	}

	data->raw_info_block = id_buf;
	data->info = (struct mxt_info *)id_buf;

	dev_info(&client->dev,
		 "Family: %u Variant: %u Firmware V%u.%u.%02X Objects: %u\n",
		 data->info->family_id, data->info->variant_id,
		 data->info->version >> 4, data->info->version & 0xf,
		 data->info->build, data->info->object_num);

	/* Parse object table information */
	error = mxt_parse_object_table(data, id_buf + MXT_OBJECT_START);
	if (error) {
		dev_err(&client->dev, "Error %d parsing object table\n", error);
		mxt_free_object_table(data);
		return error;
	}

	data->object_table = (struct mxt_object *)(id_buf + MXT_OBJECT_START);

	return 0;

err_free_mem:
	kfree(id_buf);
	return error;
}

static int mxt_read_t9_resolution(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	struct t9_range range;
	unsigned char orient;
	struct mxt_object *object;

	object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
	if (!object)
		return -EINVAL;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_XSIZE,
			       sizeof(data->xsize), &data->xsize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_YSIZE,
			       sizeof(data->ysize), &data->ysize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_RANGE,
			       sizeof(range), &range);
	if (error)
		return error;

	data->max_x = get_unaligned_le16(&range.x);
	data->max_y = get_unaligned_le16(&range.y);

	error =  __mxt_read_reg(client,
				object->start_address + MXT_T9_ORIENT,
				1, &orient);
	if (error)
		return error;

	data->xy_switch = orient & MXT_T9_ORIENT_SWITCH;
	data->invertx = orient & MXT_T9_ORIENT_INVERTX;
	data->inverty = orient & MXT_T9_ORIENT_INVERTY;

	return 0;
}

static int mxt_read_t100_config(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	struct mxt_object *object;
	u16 range_x, range_y;
	u8 cfg, tchaux;
	u8 aux;

	object = mxt_get_object(data, MXT_TOUCH_MULTITOUCHSCREEN_T100);
	if (!object)
		return -EINVAL;

	/* read touchscreen dimensions */
	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_XRANGE,
			       sizeof(range_x), &range_x);
	if (error)
		return error;

	data->max_x = get_unaligned_le16(&range_x);

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_YRANGE,
			       sizeof(range_y), &range_y);
	if (error)
		return error;

	data->max_y = get_unaligned_le16(&range_y);

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_XSIZE,
			       sizeof(data->xsize), &data->xsize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_YSIZE,
			       sizeof(data->ysize), &data->ysize);
	if (error)
		return error;

	/* read orientation config */
	error =  __mxt_read_reg(client,
				object->start_address + MXT_T100_CFG1,
				1, &cfg);
	if (error)
		return error;

	data->xy_switch = cfg & MXT_T100_CFG_SWITCHXY;
	data->invertx = cfg & MXT_T100_CFG_INVERTX;
	data->inverty = cfg & MXT_T100_CFG_INVERTY;

	/* allocate aux bytes */
	error =  __mxt_read_reg(client,
				object->start_address + MXT_T100_TCHAUX,
				1, &tchaux);
	if (error)
		return error;

	aux = 6;

	if (tchaux & MXT_T100_TCHAUX_VECT)
		data->t100_aux_vect = aux++;

	if (tchaux & MXT_T100_TCHAUX_AMPL)
		data->t100_aux_ampl = aux++;

	if (tchaux & MXT_T100_TCHAUX_AREA)
		data->t100_aux_area = aux++;

	dev_dbg(&client->dev,
		"T100 aux mappings vect:%u ampl:%u area:%u\n",
		data->t100_aux_vect, data->t100_aux_ampl, data->t100_aux_area);

	return 0;
}

static int mxt_input_open(struct input_dev *dev);
static void mxt_input_close(struct input_dev *dev);

static void mxt_set_up_as_touchpad(struct input_dev *input_dev,
				   struct mxt_data *data)
{
	int i;

	input_dev->name = "Atmel maXTouch Touchpad";

	__set_bit(INPUT_PROP_BUTTONPAD, input_dev->propbit);

	input_abs_set_res(input_dev, ABS_X, MXT_PIXELS_PER_MM);
	input_abs_set_res(input_dev, ABS_Y, MXT_PIXELS_PER_MM);
	input_abs_set_res(input_dev, ABS_MT_POSITION_X,
			  MXT_PIXELS_PER_MM);
	input_abs_set_res(input_dev, ABS_MT_POSITION_Y,
			  MXT_PIXELS_PER_MM);

	for (i = 0; i < data->t19_num_keys; i++)
		if (data->t19_keymap[i] != KEY_RESERVED)
			input_set_capability(input_dev, EV_KEY,
					     data->t19_keymap[i]);
}

static int mxt_initialize_input_device(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	struct input_dev *input_dev;
	int error;
	unsigned int num_mt_slots;
	unsigned int mt_flags = 0;
	int i;

	switch (data->multitouch) {
	case MXT_TOUCH_MULTI_T9:
		num_mt_slots = data->T9_reportid_max - data->T9_reportid_min + 1;
		error = mxt_read_t9_resolution(data);
		if (error)
			dev_warn(dev, "Failed to initialize T9 resolution\n");
		break;

	case MXT_TOUCH_MULTITOUCHSCREEN_T100:
		num_mt_slots = data->num_touchids;
		error = mxt_read_t100_config(data);
		if (error)
			dev_warn(dev, "Failed to read T100 config\n");
		break;

	default:
		dev_err(dev, "Invalid multitouch object\n");
		return -EINVAL;
	}

	/* Handle default values and orientation switch */
	if (data->max_x == 0)
		data->max_x = 1023;

	if (data->max_y == 0)
		data->max_y = 1023;

	if (data->xy_switch)
		swap(data->max_x, data->max_y);

	dev_info(dev, "Touchscreen size X%uY%u\n", data->max_x, data->max_y);

	/* Register input device */
	input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_dev->name = "Atmel maXTouch Touchscreen";
	input_dev->phys = data->phys;
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = dev;
	input_dev->open = mxt_input_open;
	input_dev->close = mxt_input_close;

	input_dev->keycode = data->t15_keymap;
	input_dev->keycodemax = data->t15_num_keys;
	input_dev->keycodesize = sizeof(data->t15_keymap[0]);

	input_set_capability(input_dev, EV_KEY, BTN_TOUCH);

	/* For single touch */
	input_set_abs_params(input_dev, ABS_X, 0, data->max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, data->max_y, 0, 0);

	if (data->multitouch == MXT_TOUCH_MULTI_T9 ||
	    (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
	     data->t100_aux_ampl)) {
		input_set_abs_params(input_dev, ABS_PRESSURE, 0, 255, 0, 0);
	}

	/* If device has buttons we assume it is a touchpad */
	if (data->t19_num_keys) {
		mxt_set_up_as_touchpad(input_dev, data);
		mt_flags |= INPUT_MT_POINTER;
	} else {
		mt_flags |= INPUT_MT_DIRECT;
	}

	/* For multi touch */
	error = input_mt_init_slots(input_dev, num_mt_slots, mt_flags);
	if (error) {
		dev_err(dev, "Error %d initialising slots\n", error);
		goto err_free_mem;
	}

	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100) {
		input_set_abs_params(input_dev, ABS_MT_TOOL_TYPE,
				     0, MT_TOOL_MAX, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_DISTANCE,
				     MXT_DISTANCE_ACTIVE_TOUCH,
				     MXT_DISTANCE_HOVERING,
				     0, 0);
	}

	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			     0, data->max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			     0, data->max_y, 0, 0);

	if (data->multitouch == MXT_TOUCH_MULTI_T9 ||
	    (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
	     data->t100_aux_area)) {
		input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
				     0, MXT_MAX_AREA, 0, 0);
	}

	if (data->multitouch == MXT_TOUCH_MULTI_T9 ||
	    (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
	     data->t100_aux_ampl)) {
		input_set_abs_params(input_dev, ABS_MT_PRESSURE,
				     0, 255, 0, 0);
	}

	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
	    data->t100_aux_vect) {
		input_set_abs_params(input_dev, ABS_MT_ORIENTATION,
				     0, 255, 0, 0);
	}

	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
	    data->t100_aux_vect) {
		input_set_abs_params(input_dev, ABS_MT_ORIENTATION,
				     0, 255, 0, 0);
	}

	/* For T15 and T97 Key Array */
	if (data->T15_reportid_min || data->T97_reportid_min) {
		for (i = 0; i < data->t15_num_keys; i++)
			input_set_capability(input_dev,
					     EV_KEY, data->t15_keymap[i]);
	}

	input_set_drvdata(input_dev, data);

	error = input_register_device(input_dev);
	if (error) {
		dev_err(dev, "Error %d registering input device\n", error);
		goto err_free_mem;
	}

	data->input_dev = input_dev;

	return 0;

err_free_mem:
	input_free_device(input_dev);
	return error;
}

static int mxt_configure_objects(struct mxt_data *data,
				 const struct firmware *cfg);

static void mxt_config_cb(const struct firmware *cfg, void *ctx)
{
	mxt_configure_objects(ctx, cfg);
	release_firmware(cfg);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int mxt_initialize(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
<<<<<<< HEAD
	struct mxt_info *info = &data->info;
	int error;
	u8 val;

	error = mxt_get_info(data);
	if (error) {
		/* Try bootloader mode */
		error = mxt_switch_to_bootloader_address(data);
		if (error)
			return error;

		error = mxt_check_bootloader(client, MXT_APP_CRC_FAIL);
		if (error)
			return error;

		dev_err(&client->dev, "Application CRC failure\n");
		data->state = BOOTLOADER;

		return 0;
	}

	dev_info(&client->dev,
			"Family ID: %d Variant ID: %d Version: %d.%d "
			"Build: 0x%02X Object Num: %d\n",
			info->family_id, info->variant_id,
			info->version >> 4, info->version & 0xf,
			info->build, info->object_num);

	data->state = APPMODE;

	data->object_table = kcalloc(info->object_num,
				     sizeof(struct mxt_object),
				     GFP_KERNEL);
	if (!data->object_table) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error)
		goto free_object_table;

	error = mxt_update_cfg(data);
	if (error)
		goto free_object_table;
	/* Update matrix size at info struct */
	error = mxt_read_reg(client, MXT_MATRIX_X_SIZE, &val);
	if (error)
		goto free_object_table;
	info->matrix_xsize = val;

	error = mxt_read_reg(client, MXT_MATRIX_Y_SIZE, &val);
	if (error)
		goto free_object_table;
	info->matrix_ysize = val;

	dev_info(&client->dev,
			"Matrix X Size: %d Matrix Y Size: %d\n",
			info->matrix_xsize, info->matrix_ysize);

	return 0;

free_object_table:
	kfree(data->object_table);
	return error;
}

static ssize_t mxt_object_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
=======
	int recovery_attempts = 0;
	int error;

	while (1) {
		error = mxt_read_info_block(data);
		if (!error)
			break;

		/* Check bootloader state */
		error = mxt_probe_bootloader(data, false);
		if (error) {
			dev_info(&client->dev, "Trying alternate bootloader address\n");
			error = mxt_probe_bootloader(data, true);
			if (error) {
				/* Chip is not in appmode or bootloader mode */
				return error;
			}
		}

		/* OK, we are in bootloader, see if we can recover */
		if (++recovery_attempts > 1) {
			dev_err(&client->dev, "Could not recover from bootloader mode\n");
			/*
			 * We can reflash from this state, so do not
			 * abort initialization.
			 */
			data->in_bootloader = true;
			return 0;
		}

		/* Attempt to exit bootloader into app mode */
		mxt_send_bootloader_cmd(data, false);
		msleep(MXT_FW_RESET_TIME);
	}

	error = mxt_check_retrigen(data);
	if (error)
		return error;

	error = mxt_acquire_irq(data);
	if (error)
		return error;

	error = request_firmware_nowait(THIS_MODULE, true, MXT_CFG_NAME,
					&client->dev, GFP_KERNEL, data,
					mxt_config_cb);
	if (error) {
		dev_err(&client->dev, "Failed to invoke firmware loader: %d\n",
			error);
		return error;
	}

	return 0;
}

static int mxt_set_t7_power_cfg(struct mxt_data *data, u8 sleep)
{
	struct device *dev = &data->client->dev;
	int error;
	struct t7_config *new_config;
	struct t7_config deepsleep = { .active = 0, .idle = 0 };

	if (sleep == MXT_POWER_CFG_DEEPSLEEP)
		new_config = &deepsleep;
	else
		new_config = &data->t7_cfg;

	error = __mxt_write_reg(data->client, data->T7_address,
				sizeof(data->t7_cfg), new_config);
	if (error)
		return error;

	dev_dbg(dev, "Set T7 ACTV:%d IDLE:%d\n",
		new_config->active, new_config->idle);

	return 0;
}

static int mxt_init_t7_power_cfg(struct mxt_data *data)
{
	struct device *dev = &data->client->dev;
	int error;
	bool retry = false;

recheck:
	error = __mxt_read_reg(data->client, data->T7_address,
				sizeof(data->t7_cfg), &data->t7_cfg);
	if (error)
		return error;

	if (data->t7_cfg.active == 0 || data->t7_cfg.idle == 0) {
		if (!retry) {
			dev_dbg(dev, "T7 cfg zero, resetting\n");
			mxt_soft_reset(data);
			retry = true;
			goto recheck;
		} else {
			dev_dbg(dev, "T7 cfg zero after reset, overriding\n");
			data->t7_cfg.active = 20;
			data->t7_cfg.idle = 100;
			return mxt_set_t7_power_cfg(data, MXT_POWER_CFG_RUN);
		}
	}

	dev_dbg(dev, "Initialized power cfg: ACTV %d, IDLE %d\n",
		data->t7_cfg.active, data->t7_cfg.idle);
	return 0;
}

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT_T37
static const struct v4l2_file_operations mxt_video_fops = {
	.owner = THIS_MODULE,
	.open = v4l2_fh_open,
	.release = vb2_fop_release,
	.unlocked_ioctl = video_ioctl2,
	.read = vb2_fop_read,
	.mmap = vb2_fop_mmap,
	.poll = vb2_fop_poll,
};

static u16 mxt_get_debug_value(struct mxt_data *data, unsigned int x,
			       unsigned int y)
{
	struct mxt_info *info = data->info;
	struct mxt_dbg *dbg = &data->dbg;
	unsigned int ofs, page;
	unsigned int col = 0;
	unsigned int col_width;

	if (info->family_id == MXT_FAMILY_1386) {
		col_width = info->matrix_ysize / MXT1386_COLUMNS;
		col = y / col_width;
		y = y % col_width;
	} else {
		col_width = info->matrix_ysize;
	}

	ofs = (y + (x * col_width)) * sizeof(u16);
	page = ofs / MXT_DIAGNOSTIC_SIZE;
	ofs %= MXT_DIAGNOSTIC_SIZE;

	if (info->family_id == MXT_FAMILY_1386)
		page += col * MXT1386_PAGES_PER_COLUMN;

	return get_unaligned_le16(&dbg->t37_buf[page].data[ofs]);
}

static int mxt_convert_debug_pages(struct mxt_data *data, u16 *outbuf)
{
	struct mxt_dbg *dbg = &data->dbg;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int i, rx, ry;

	for (i = 0; i < dbg->t37_nodes; i++) {
		/* Handle orientation */
		rx = data->xy_switch ? y : x;
		ry = data->xy_switch ? x : y;
		rx = data->invertx ? (data->xsize - 1 - rx) : rx;
		ry = data->inverty ? (data->ysize - 1 - ry) : ry;

		outbuf[i] = mxt_get_debug_value(data, rx, ry);

		/* Next value */
		if (++x >= (data->xy_switch ? data->ysize : data->xsize)) {
			x = 0;
			y++;
		}
	}

	return 0;
}

static int mxt_read_diagnostic_debug(struct mxt_data *data, u8 mode,
				     u16 *outbuf)
{
	struct mxt_dbg *dbg = &data->dbg;
	int retries = 0;
	int page;
	int ret;
	u8 cmd = mode;
	struct t37_debug *p;
	u8 cmd_poll;

	for (page = 0; page < dbg->t37_pages; page++) {
		p = dbg->t37_buf + page;

		ret = mxt_write_reg(data->client, dbg->diag_cmd_address,
				    cmd);
		if (ret)
			return ret;

		retries = 0;
		msleep(20);
wait_cmd:
		/* Read back command byte */
		ret = __mxt_read_reg(data->client, dbg->diag_cmd_address,
				     sizeof(cmd_poll), &cmd_poll);
		if (ret)
			return ret;

		/* Field is cleared once the command has been processed */
		if (cmd_poll) {
			if (retries++ > 100)
				return -EINVAL;

			msleep(20);
			goto wait_cmd;
		}

		/* Read T37 page */
		ret = __mxt_read_reg(data->client, dbg->t37_address,
				     sizeof(struct t37_debug), p);
		if (ret)
			return ret;

		if (p->mode != mode || p->page != page) {
			dev_err(&data->client->dev, "T37 page mismatch\n");
			return -EINVAL;
		}

		dev_dbg(&data->client->dev, "%s page:%d retries:%d\n",
			__func__, page, retries);

		/* For remaining pages, write PAGEUP rather than mode */
		cmd = MXT_DIAGNOSTIC_PAGEUP;
	}

	return mxt_convert_debug_pages(data, outbuf);
}

static int mxt_queue_setup(struct vb2_queue *q,
		       unsigned int *nbuffers, unsigned int *nplanes,
		       unsigned int sizes[], struct device *alloc_devs[])
{
	struct mxt_data *data = q->drv_priv;
	size_t size = data->dbg.t37_nodes * sizeof(u16);

	if (*nplanes)
		return sizes[0] < size ? -EINVAL : 0;

	*nplanes = 1;
	sizes[0] = size;

	return 0;
}

static void mxt_buffer_queue(struct vb2_buffer *vb)
{
	struct mxt_data *data = vb2_get_drv_priv(vb->vb2_queue);
	u16 *ptr;
	int ret;
	u8 mode;

	ptr = vb2_plane_vaddr(vb, 0);
	if (!ptr) {
		dev_err(&data->client->dev, "Error acquiring frame ptr\n");
		goto fault;
	}

	switch (data->dbg.input) {
	case MXT_V4L_INPUT_DELTAS:
	default:
		mode = MXT_DIAGNOSTIC_DELTAS;
		break;

	case MXT_V4L_INPUT_REFS:
		mode = MXT_DIAGNOSTIC_REFS;
		break;
	}

	ret = mxt_read_diagnostic_debug(data, mode, ptr);
	if (ret)
		goto fault;

	vb2_set_plane_payload(vb, 0, data->dbg.t37_nodes * sizeof(u16));
	vb2_buffer_done(vb, VB2_BUF_STATE_DONE);
	return;

fault:
	vb2_buffer_done(vb, VB2_BUF_STATE_ERROR);
}

/* V4L2 structures */
static const struct vb2_ops mxt_queue_ops = {
	.queue_setup		= mxt_queue_setup,
	.buf_queue		= mxt_buffer_queue,
	.wait_prepare		= vb2_ops_wait_prepare,
	.wait_finish		= vb2_ops_wait_finish,
};

static const struct vb2_queue mxt_queue = {
	.type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
	.io_modes = VB2_MMAP | VB2_USERPTR | VB2_DMABUF | VB2_READ,
	.buf_struct_size = sizeof(struct mxt_vb2_buffer),
	.ops = &mxt_queue_ops,
	.mem_ops = &vb2_vmalloc_memops,
	.timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC,
	.min_queued_buffers = 1,
};

static int mxt_vidioc_querycap(struct file *file, void *priv,
				 struct v4l2_capability *cap)
{
	struct mxt_data *data = video_drvdata(file);

	strscpy(cap->driver, "atmel_mxt_ts", sizeof(cap->driver));
	strscpy(cap->card, "atmel_mxt_ts touch", sizeof(cap->card));
	snprintf(cap->bus_info, sizeof(cap->bus_info),
		 "I2C:%s", dev_name(&data->client->dev));
	return 0;
}

static int mxt_vidioc_enum_input(struct file *file, void *priv,
				   struct v4l2_input *i)
{
	if (i->index >= MXT_V4L_INPUT_MAX)
		return -EINVAL;

	i->type = V4L2_INPUT_TYPE_TOUCH;

	switch (i->index) {
	case MXT_V4L_INPUT_REFS:
		strscpy(i->name, "Mutual Capacitance References",
			sizeof(i->name));
		break;
	case MXT_V4L_INPUT_DELTAS:
		strscpy(i->name, "Mutual Capacitance Deltas", sizeof(i->name));
		break;
	}

	return 0;
}

static int mxt_set_input(struct mxt_data *data, unsigned int i)
{
	struct v4l2_pix_format *f = &data->dbg.format;

	if (i >= MXT_V4L_INPUT_MAX)
		return -EINVAL;

	if (i == MXT_V4L_INPUT_DELTAS)
		f->pixelformat = V4L2_TCH_FMT_DELTA_TD16;
	else
		f->pixelformat = V4L2_TCH_FMT_TU16;

	f->width = data->xy_switch ? data->ysize : data->xsize;
	f->height = data->xy_switch ? data->xsize : data->ysize;
	f->field = V4L2_FIELD_NONE;
	f->colorspace = V4L2_COLORSPACE_RAW;
	f->bytesperline = f->width * sizeof(u16);
	f->sizeimage = f->width * f->height * sizeof(u16);

	data->dbg.input = i;

	return 0;
}

static int mxt_vidioc_s_input(struct file *file, void *priv, unsigned int i)
{
	return mxt_set_input(video_drvdata(file), i);
}

static int mxt_vidioc_g_input(struct file *file, void *priv, unsigned int *i)
{
	struct mxt_data *data = video_drvdata(file);

	*i = data->dbg.input;

	return 0;
}

static int mxt_vidioc_fmt(struct file *file, void *priv, struct v4l2_format *f)
{
	struct mxt_data *data = video_drvdata(file);

	f->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	f->fmt.pix = data->dbg.format;

	return 0;
}

static int mxt_vidioc_enum_fmt(struct file *file, void *priv,
				 struct v4l2_fmtdesc *fmt)
{
	if (fmt->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	switch (fmt->index) {
	case 0:
		fmt->pixelformat = V4L2_TCH_FMT_TU16;
		break;

	case 1:
		fmt->pixelformat = V4L2_TCH_FMT_DELTA_TD16;
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int mxt_vidioc_g_parm(struct file *file, void *fh,
			     struct v4l2_streamparm *a)
{
	if (a->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	a->parm.capture.readbuffers = 1;
	a->parm.capture.timeperframe.numerator = 1;
	a->parm.capture.timeperframe.denominator = 10;
	return 0;
}

static const struct v4l2_ioctl_ops mxt_video_ioctl_ops = {
	.vidioc_querycap        = mxt_vidioc_querycap,

	.vidioc_enum_fmt_vid_cap = mxt_vidioc_enum_fmt,
	.vidioc_s_fmt_vid_cap   = mxt_vidioc_fmt,
	.vidioc_g_fmt_vid_cap   = mxt_vidioc_fmt,
	.vidioc_try_fmt_vid_cap	= mxt_vidioc_fmt,
	.vidioc_g_parm		= mxt_vidioc_g_parm,

	.vidioc_enum_input      = mxt_vidioc_enum_input,
	.vidioc_g_input         = mxt_vidioc_g_input,
	.vidioc_s_input         = mxt_vidioc_s_input,

	.vidioc_reqbufs         = vb2_ioctl_reqbufs,
	.vidioc_create_bufs     = vb2_ioctl_create_bufs,
	.vidioc_querybuf        = vb2_ioctl_querybuf,
	.vidioc_qbuf            = vb2_ioctl_qbuf,
	.vidioc_dqbuf           = vb2_ioctl_dqbuf,
	.vidioc_expbuf          = vb2_ioctl_expbuf,

	.vidioc_streamon        = vb2_ioctl_streamon,
	.vidioc_streamoff       = vb2_ioctl_streamoff,
};

static const struct video_device mxt_video_device = {
	.name = "Atmel maxTouch",
	.fops = &mxt_video_fops,
	.ioctl_ops = &mxt_video_ioctl_ops,
	.release = video_device_release_empty,
	.device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_TOUCH |
		       V4L2_CAP_READWRITE | V4L2_CAP_STREAMING,
};

static void mxt_debug_init(struct mxt_data *data)
{
	struct mxt_info *info = data->info;
	struct mxt_dbg *dbg = &data->dbg;
	struct mxt_object *object;
	int error;

	object = mxt_get_object(data, MXT_GEN_COMMAND_T6);
	if (!object)
		goto error;

	dbg->diag_cmd_address = object->start_address + MXT_COMMAND_DIAGNOSTIC;

	object = mxt_get_object(data, MXT_DEBUG_DIAGNOSTIC_T37);
	if (!object)
		goto error;

	if (mxt_obj_size(object) != sizeof(struct t37_debug)) {
		dev_warn(&data->client->dev, "Bad T37 size");
		goto error;
	}

	dbg->t37_address = object->start_address;

	/* Calculate size of data and allocate buffer */
	dbg->t37_nodes = data->xsize * data->ysize;

	if (info->family_id == MXT_FAMILY_1386)
		dbg->t37_pages = MXT1386_COLUMNS * MXT1386_PAGES_PER_COLUMN;
	else
		dbg->t37_pages = DIV_ROUND_UP(data->xsize *
					      info->matrix_ysize *
					      sizeof(u16),
					      sizeof(dbg->t37_buf->data));

	dbg->t37_buf = devm_kmalloc_array(&data->client->dev, dbg->t37_pages,
					  sizeof(struct t37_debug), GFP_KERNEL);
	if (!dbg->t37_buf)
		goto error;

	/* init channel to zero */
	mxt_set_input(data, 0);

	/* register video device */
	snprintf(dbg->v4l2.name, sizeof(dbg->v4l2.name), "%s", "atmel_mxt_ts");
	error = v4l2_device_register(&data->client->dev, &dbg->v4l2);
	if (error)
		goto error;

	/* initialize the queue */
	mutex_init(&dbg->lock);
	dbg->queue = mxt_queue;
	dbg->queue.drv_priv = data;
	dbg->queue.lock = &dbg->lock;
	dbg->queue.dev = &data->client->dev;

	error = vb2_queue_init(&dbg->queue);
	if (error)
		goto error_unreg_v4l2;

	dbg->vdev = mxt_video_device;
	dbg->vdev.v4l2_dev = &dbg->v4l2;
	dbg->vdev.lock = &dbg->lock;
	dbg->vdev.vfl_dir = VFL_DIR_RX;
	dbg->vdev.queue = &dbg->queue;
	video_set_drvdata(&dbg->vdev, data);

	error = video_register_device(&dbg->vdev, VFL_TYPE_TOUCH, -1);
	if (error)
		goto error_unreg_v4l2;

	return;

error_unreg_v4l2:
	v4l2_device_unregister(&dbg->v4l2);
error:
	dev_warn(&data->client->dev, "Error initializing T37\n");
}
#else
static void mxt_debug_init(struct mxt_data *data)
{
}
#endif

static int mxt_configure_objects(struct mxt_data *data,
				 const struct firmware *cfg)
{
	struct device *dev = &data->client->dev;
	int error;

	error = mxt_init_t7_power_cfg(data);
	if (error) {
		dev_err(dev, "Failed to initialize power cfg\n");
		return error;
	}

	if (cfg) {
		error = mxt_update_cfg(data, cfg);
		if (error)
			dev_warn(dev, "Error %d updating config\n", error);
	}

	if (data->multitouch) {
		error = mxt_initialize_input_device(data);
		if (error)
			return error;
	} else {
		dev_warn(dev, "No touch object detected\n");
	}

	mxt_debug_init(data);

	return 0;
}

/* Firmware Version is returned as Major.Minor.Build */
static ssize_t mxt_fw_version_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_info *info = data->info;
	return sysfs_emit(buf, "%u.%u.%02X\n",
			  info->version >> 4, info->version & 0xf, info->build);
}

/* Hardware Version is returned as FamilyID.VariantID */
static ssize_t mxt_hw_version_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_info *info = data->info;
	return sysfs_emit(buf, "%u.%u\n", info->family_id, info->variant_id);
}

static ssize_t mxt_show_instance(char *buf, int count,
				 struct mxt_object *object, int instance,
				 const u8 *val)
{
	int i;

	if (mxt_obj_instances(object) > 1)
		count += sysfs_emit_at(buf, count, "Instance %u\n", instance);

	for (i = 0; i < mxt_obj_size(object); i++)
		count += sysfs_emit_at(buf, count, "\t[%2u]: %02x (%d)\n",
				       i, val[i], val[i]);
	count += sysfs_emit_at(buf, count, "\n");

	return count;
}

static ssize_t mxt_object_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct mxt_object *object;
	int count = 0;
	int i, j;
	int error;
<<<<<<< HEAD
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;

		count += snprintf(buf + count, PAGE_SIZE - count,
				"Object[%d] (Type %d)\n",
				i + 1, object->type);
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;

		if (!mxt_object_readable(object->type)) {
			count += snprintf(buf + count, PAGE_SIZE - count,
					"\n");
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
			continue;
		}

		for (j = 0; j < object->size + 1; j++) {
			error = mxt_read_object(data,
						object->type, j, &val);
			if (error)
				return error;

			count += snprintf(buf + count, PAGE_SIZE - count,
					"\t[%2d]: %02x (%d)\n", j, val, val);
			if (count >= PAGE_SIZE)
				return PAGE_SIZE - 1;
		}

		count += snprintf(buf + count, PAGE_SIZE - count, "\n");
		if (count >= PAGE_SIZE)
			return PAGE_SIZE - 1;
	}

	return count;
}

static int strtobyte(const char *data, u8 *value)
{
	char str[3];

	str[0] = data[0];
	str[1] = data[1];
	str[2] = '\0';

	return kstrtou8(str, 16, value);
=======
	u8 *obuf;

	/* Pre-allocate buffer large enough to hold max sized object. */
	obuf = kmalloc(256, GFP_KERNEL);
	if (!obuf)
		return -ENOMEM;

	error = 0;
	for (i = 0; i < data->info->object_num; i++) {
		object = data->object_table + i;

		if (!mxt_object_readable(object->type))
			continue;

		count += sysfs_emit_at(buf, count, "T%u:\n", object->type);

		for (j = 0; j < mxt_obj_instances(object); j++) {
			u16 size = mxt_obj_size(object);
			u16 addr = object->start_address + j * size;

			error = __mxt_read_reg(data->client, addr, size, obuf);
			if (error)
				goto done;

			count = mxt_show_instance(buf, count, object, j, obuf);
		}
	}

done:
	kfree(obuf);
	return error ?: count;
}

static int mxt_check_firmware_format(struct device *dev,
				     const struct firmware *fw)
{
	unsigned int pos = 0;
	char c;

	while (pos < fw->size) {
		c = *(fw->data + pos);

		if (c < '0' || (c > '9' && c < 'A') || c > 'F')
			return 0;

		pos++;
	}

	/*
	 * To convert file try:
	 * xxd -r -p mXTXXX__APP_VX-X-XX.enc > maxtouch.fw
	 */
	dev_err(dev, "Aborting: firmware file must be in binary format\n");

	return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxt_load_fw(struct device *dev, const char *fn)
{
	struct mxt_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct i2c_client *client = data->client;
	const struct firmware *fw = NULL;
	unsigned int frame_size;
	unsigned int retry = 0;
	unsigned int pos = 0;
	int ret, i, max_frame_size;
	u8 *frame;

	switch (data->info.family_id) {
	case MXT224_ID:
	case MXT224E_ID:
	case MXT336S_ID:
		max_frame_size = MXT_SINGLE_FW_MAX_FRAME_SIZE;
		break;
	case MXT1386_ID:
	case MXT1664S_ID:
		max_frame_size = MXT_CHIPSET_FW_MAX_FRAME_SIZE;
		break;
	default:
		return -EINVAL;
	}

	frame = kmalloc(max_frame_size, GFP_KERNEL);
	if (!frame) {
		dev_err(dev, "Unable to allocate memory for frame data\n");
		return -ENOMEM;
	}

	ret = request_firmware(&fw, fn, dev);
	if (ret < 0 || !fw) {
		dev_err(dev, "Unable to open firmware %s\n", fn);
		goto free_frame;
	}

	if (data->state != BOOTLOADER) {
		/* Change to the bootloader mode */
		mxt_write_object(data, MXT_GEN_COMMAND_T6,
				MXT_COMMAND_RESET, MXT_BOOT_VALUE);
		mxt_reset_delay(data);

		ret = mxt_switch_to_bootloader_address(data);
		if (ret)
			goto release_firmware;
	}

	ret = mxt_check_bootloader(client, MXT_WAITING_BOOTLOAD_CMD);
	if (ret) {
		/* Bootloader may still be unlocked from previous update
		 * attempt */
		ret = mxt_check_bootloader(client,
			MXT_WAITING_FRAME_DATA);

		if (ret)
			goto return_to_app_mode;
	} else {
		dev_info(dev, "Unlocking bootloader\n");
		/* Unlock bootloader */
		mxt_unlock_bootloader(client);
	}

	while (pos < fw->size) {
		ret = mxt_check_bootloader(client,
						MXT_WAITING_FRAME_DATA);
		if (ret)
			goto release_firmware;

		/* Get frame length MSB */
		ret = strtobyte(fw->data + pos, frame);
		if (ret)
			goto release_firmware;

		/* Get frame length LSB */
		ret = strtobyte(fw->data + pos + 2, frame + 1);
		if (ret)
			goto release_firmware;

		frame_size = ((*frame << 8) | *(frame + 1));

		/* We should add 2 at frame size as the the firmware data is not
		 * included the CRC bytes.
		 */
		frame_size += 2;

		if (frame_size > max_frame_size) {
			dev_err(dev, "Invalid frame size - %d\n", frame_size);
			ret = -EINVAL;
			goto release_firmware;
		}

		/* Convert frame data and CRC from hex to binary */
		for (i = 2; i < frame_size; i++) {
			ret = strtobyte(fw->data + pos + i * 2, frame + i);
			if (ret)
				goto release_firmware;
		}

		/* Write one frame to device */
		mxt_fw_write(client, frame, frame_size);

		ret = mxt_check_bootloader(client,
						MXT_FRAME_CRC_PASS);
=======
	const struct firmware *fw = NULL;
	unsigned int frame_size;
	unsigned int pos = 0;
	unsigned int retry = 0;
	unsigned int frame = 0;
	int ret;

	ret = request_firmware(&fw, fn, dev);
	if (ret) {
		dev_err(dev, "Unable to open firmware %s\n", fn);
		return ret;
	}

	/* Check for incorrect enc file */
	ret = mxt_check_firmware_format(dev, fw);
	if (ret)
		goto release_firmware;

	if (!data->in_bootloader) {
		/* Change to the bootloader mode */
		data->in_bootloader = true;

		ret = mxt_t6_command(data, MXT_COMMAND_RESET,
				     MXT_BOOT_VALUE, false);
		if (ret)
			goto release_firmware;

		msleep(MXT_RESET_TIME);

		/* Do not need to scan since we know family ID */
		ret = mxt_lookup_bootloader_address(data, 0);
		if (ret)
			goto release_firmware;

		mxt_free_input_device(data);
		mxt_free_object_table(data);
	} else {
		enable_irq(data->irq);
	}

	reinit_completion(&data->bl_completion);

	ret = mxt_check_bootloader(data, MXT_WAITING_BOOTLOAD_CMD, false);
	if (ret) {
		/* Bootloader may still be unlocked from previous attempt */
		ret = mxt_check_bootloader(data, MXT_WAITING_FRAME_DATA, false);
		if (ret)
			goto disable_irq;
	} else {
		dev_info(dev, "Unlocking bootloader\n");

		/* Unlock bootloader */
		ret = mxt_send_bootloader_cmd(data, true);
		if (ret)
			goto disable_irq;
	}

	while (pos < fw->size) {
		ret = mxt_check_bootloader(data, MXT_WAITING_FRAME_DATA, true);
		if (ret)
			goto disable_irq;

		frame_size = ((*(fw->data + pos) << 8) | *(fw->data + pos + 1));

		/* Take account of CRC bytes */
		frame_size += 2;

		/* Write one frame to device */
		ret = mxt_bootloader_write(data, fw->data + pos, frame_size);
		if (ret)
			goto disable_irq;

		ret = mxt_check_bootloader(data, MXT_FRAME_CRC_PASS, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret) {
			retry++;

			/* Back off by 20ms per retry */
			msleep(retry * 20);

<<<<<<< HEAD
			if (retry > 20)
				goto release_firmware;
		} else {
			retry = 0;
			pos += frame_size * 2;
			dev_dbg(dev, "Updated %d/%zd bytes\n", pos, fw->size);
		}
	}

return_to_app_mode:
	mxt_switch_to_appmode_address(data);
release_firmware:
	release_firmware(fw);
free_frame:
	kfree(frame);

	return ret;
}

static const char *
mxt_search_fw_name(struct mxt_data *data, u8 bootldr_id)
{
	const struct mxt_platform_data *pdata = data->pdata;
	const struct mxt_config_info *cfg_info;
	const char *fw_name = NULL;
	int i;

	for (i = 0; i < pdata->config_array_size; i++) {
		cfg_info = &pdata->config_array[i];
		if (bootldr_id == cfg_info->bootldr_id && cfg_info->fw_name) {
			data->config_info = cfg_info;
			data->info.family_id = cfg_info->family_id;
			fw_name = cfg_info->fw_name;
		}
	}

	return fw_name;
}

static ssize_t mxt_force_cfg_update_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	int flag = buf[0]-'0';
	int error;
	data->no_force_update = !flag;

	if (data->state == APPMODE) {
		disable_irq(data->irq);
		error = mxt_update_cfg(data);
		enable_irq(data->irq);
		if (error)
			return error;
	} else {
		dev_err(dev,
		"Not in APPMODE, Unable to force cfg update\n");
		return -EINVAL;
	}

	return count;
}

=======
			if (retry > 20) {
				dev_err(dev, "Retry count exceeded\n");
				goto disable_irq;
			}
		} else {
			retry = 0;
			pos += frame_size;
			frame++;
		}

		if (frame % 50 == 0)
			dev_dbg(dev, "Sent %d frames, %d/%zd bytes\n",
				frame, pos, fw->size);
	}

	/* Wait for flash. */
	ret = mxt_wait_for_completion(data, &data->bl_completion,
				      MXT_FW_RESET_TIME);
	if (ret)
		goto disable_irq;

	dev_dbg(dev, "Sent %d frames, %d bytes\n", frame, pos);

	/*
	 * Wait for device to reset. Some bootloader versions do not assert
	 * the CHG line after bootloading has finished, so ignore potential
	 * errors.
	 */
	mxt_wait_for_completion(data, &data->bl_completion, MXT_FW_RESET_TIME);

	data->in_bootloader = false;

disable_irq:
	disable_irq(data->irq);
release_firmware:
	release_firmware(fw);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t mxt_update_fw_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct mxt_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	int error, address;
	const char *fw_name;
	u8 bootldr_id;
	u8 cfg_version[MXT_CFG_VERSION_LEN] = {0};

	data->no_force_update = false;
	/* If fw_name is set, then the existing firmware has an upgrade */
	if (!data->fw_name) {
		/*
		 * If the device boots up in the bootloader mode, check if
		 * there is a firmware to upgrade.
		 */
		if (data->state == BOOTLOADER) {
			bootldr_id = mxt_get_bootloader_id(data->client);
			if (bootldr_id <= 0) {
				dev_err(dev,
					"Unable to retrieve bootloader id\n");
				return -EINVAL;
			}
			fw_name = mxt_search_fw_name(data, bootldr_id);
			if (fw_name == NULL) {
				dev_err(dev,
				"Unable to find fw from bootloader id\n");
				return -EINVAL;
			}
		} else {
			/* In APPMODE, if the f/w name does not exist, quit */
			dev_err(dev,
			"Firmware name not specified in platform data\n");
			return -EINVAL;
		}
	} else {
		fw_name = data->fw_name;
	}

	dev_info(dev, "Upgrading the firmware file to %s\n", fw_name);

	disable_irq(data->irq);

	error = mxt_load_fw(dev, fw_name);
=======
	int error;

	error = mxt_load_fw(dev, MXT_FW_NAME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error) {
		dev_err(dev, "The firmware update failed(%d)\n", error);
		count = error;
	} else {
		dev_info(dev, "The firmware update succeeded\n");

<<<<<<< HEAD
		/* Wait for reset */
		msleep(MXT_FWRESET_TIME);

		data->state = INIT;
		kfree(data->object_table);
		data->object_table = NULL;
		data->cfg_version_idx = 0;
		data->config_info = NULL;
		data->update_cfg = false;

		/* T38 object address might have changed, read it from
		   touch controller */
		address = mxt_get_object_address(dev, MXT_SPT_USERDATA_T38);
		if (address < 0) {
			dev_err(dev, "T38 required for touch operation\n");
			return -EINVAL;
		}

		data->t38_start_addr = address;

		error = __mxt_write_reg(data->client, data->t38_start_addr,
				sizeof(cfg_version), cfg_version);
		if (error)
			dev_err(dev,
			"Unable to zero out config version after fw upgrade\n");

		mxt_initialize(data);
	}

	if (data->state == APPMODE) {
		enable_irq(data->irq);

		error = mxt_make_highchg(data);
=======
		error = mxt_initialize(data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error)
			return error;
	}

	return count;
}

<<<<<<< HEAD
#if defined(CONFIG_SECURE_TOUCH)

static ssize_t mxt_secure_touch_enable_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	return scnprintf(buf, PAGE_SIZE, "%d", atomic_read(&data->st_enabled));
}
/*
 * Accept only "0" and "1" valid values.
 * "0" will reset the st_enabled flag, then wake up the reading process.
 * The bus driver is notified via pm_runtime that it is not required to stay
 * awake anymore.
 * It will also make sure the queue of events is emptied in the controller,
 * in case a touch happened in between the secure touch being disabled and
 * the local ISR being ungated.
 * "1" will set the st_enabled flag and clear the st_pending_irqs flag.
 * The bus driver is requested via pm_runtime to stay awake.
 */
static ssize_t mxt_secure_touch_enable_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	struct device *adapter = data->client->adapter->dev.parent;
	unsigned long value;
	int err = 0;

	if (count > 2)
		return -EINVAL;

	err = kstrtoul(buf, 10, &value);
	if (err != 0)
		return err;

	err = count;

	switch (value) {
	case 0:
		if (atomic_read(&data->st_enabled) == 0)
			break;

		pm_runtime_put(adapter);
		atomic_set(&data->st_enabled, 0);
		mxt_secure_touch_notify(data);
		mxt_interrupt(data->client->irq, data);
		complete(&data->st_powerdown);
		break;
	case 1:
		if (atomic_read(&data->st_enabled)) {
			err = -EBUSY;
			break;
		}

		if (pm_runtime_get_sync(adapter) < 0) {
			dev_err(&data->client->dev, "pm_runtime_get failed\n");
			err = -EIO;
			break;
		}
		INIT_COMPLETION(data->st_powerdown);
		atomic_set(&data->st_enabled, 1);
		synchronize_irq(data->client->irq);
		atomic_set(&data->st_pending_irqs, 0);
		break;
	default:
		dev_err(&data->client->dev, "unsupported value: %lu\n", value);
		err = -EINVAL;
		break;
	}

	return err;
}

static ssize_t mxt_secure_touch_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct mxt_data *data = dev_get_drvdata(dev);
	int val = 0;

	if (atomic_read(&data->st_enabled) == 0)
		return -EBADF;

	if (atomic_cmpxchg(&data->st_pending_irqs, -1, 0) == -1)
		return -EINVAL;

	if (atomic_cmpxchg(&data->st_pending_irqs, 1, 0) == 1)
		val = 1;

	return scnprintf(buf, PAGE_SIZE, "%u", val);
}

static DEVICE_ATTR(secure_touch_enable, S_IRUGO | S_IWUSR | S_IWGRP ,
			 mxt_secure_touch_enable_show,
			 mxt_secure_touch_enable_store);
static DEVICE_ATTR(secure_touch, S_IRUGO, mxt_secure_touch_show, NULL);
#endif

static DEVICE_ATTR(object, S_IRUGO, mxt_object_show, NULL);
static DEVICE_ATTR(update_fw, S_IWUSR | S_IWGRP , NULL, mxt_update_fw_store);
static DEVICE_ATTR(force_cfg_update, S_IWUSR | S_IWGRP ,
			 NULL,
			 mxt_force_cfg_update_store);

static struct attribute *mxt_attrs[] = {
	&dev_attr_object.attr,
	&dev_attr_update_fw.attr,
	&dev_attr_force_cfg_update.attr,
#if defined(CONFIG_SECURE_TOUCH)
	&dev_attr_secure_touch_enable.attr,
	&dev_attr_secure_touch.attr,
#endif
=======
static DEVICE_ATTR(fw_version, S_IRUGO, mxt_fw_version_show, NULL);
static DEVICE_ATTR(hw_version, S_IRUGO, mxt_hw_version_show, NULL);
static DEVICE_ATTR(object, S_IRUGO, mxt_object_show, NULL);
static DEVICE_ATTR(update_fw, S_IWUSR, NULL, mxt_update_fw_store);

static struct attribute *mxt_attrs[] = {
	&dev_attr_fw_version.attr,
	&dev_attr_hw_version.attr,
	&dev_attr_object.attr,
	&dev_attr_update_fw.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL
};

static const struct attribute_group mxt_attr_group = {
	.attrs = mxt_attrs,
};

<<<<<<< HEAD

#if defined(CONFIG_SECURE_TOUCH)
static void mxt_secure_touch_stop(struct mxt_data *data, int blocking)
{
	if (atomic_read(&data->st_enabled)) {
		atomic_set(&data->st_pending_irqs, -1);
		mxt_secure_touch_notify(data);
		if (blocking)
			wait_for_completion_interruptible(&data->st_powerdown);
	}
}
#else
static void mxt_secure_touch_stop(struct mxt_data *data, int blocking)
{
}
#endif

static int mxt_start(struct mxt_data *data)
{
	int error;
	mxt_secure_touch_stop(data, 1);

	/* restore the old power state values and reenable touch */
	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, data->t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to restore old power state\n");
		return error;
	}

	return 0;
}

static int mxt_stop(struct mxt_data *data)
{
	int error;
	u8 t7_data[T7_DATA_SIZE] = {0};
	mxt_secure_touch_stop(data, 1);

	error = __mxt_write_reg(data->client, data->t7_start_addr,
				T7_DATA_SIZE, t7_data);
	if (error < 0) {
		dev_err(&data->client->dev,
			"failed to configure deep sleep mode\n");
		return error;
	}

	return 0;
=======
static void mxt_start(struct mxt_data *data)
{
	mxt_wakeup_toggle(data->client, true, false);

	switch (data->suspend_mode) {
	case MXT_SUSPEND_T9_CTRL:
		mxt_soft_reset(data);

		/* Touch enable */
		/* 0x83 = SCANEN | RPTEN | ENABLE */
		mxt_write_object(data,
				MXT_TOUCH_MULTI_T9, MXT_T9_CTRL, 0x83);
		break;

	case MXT_SUSPEND_DEEP_SLEEP:
	default:
		mxt_set_t7_power_cfg(data, MXT_POWER_CFG_RUN);

		/* Recalibrate since chip has been in deep sleep */
		mxt_t6_command(data, MXT_COMMAND_CALIBRATE, 1, false);
		break;
	}
}

static void mxt_stop(struct mxt_data *data)
{
	switch (data->suspend_mode) {
	case MXT_SUSPEND_T9_CTRL:
		/* Touch disable */
		mxt_write_object(data,
				MXT_TOUCH_MULTI_T9, MXT_T9_CTRL, 0);
		break;

	case MXT_SUSPEND_DEEP_SLEEP:
	default:
		mxt_set_t7_power_cfg(data, MXT_POWER_CFG_DEEPSLEEP);
		break;
	}

	mxt_wakeup_toggle(data->client, false, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mxt_input_open(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
<<<<<<< HEAD
	int error;

	if (data->state == APPMODE) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(&data->client->dev, "mxt_start failed in input_open\n");
			return error;
		}
	}
=======

	mxt_start(data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void mxt_input_close(struct input_dev *dev)
{
	struct mxt_data *data = input_get_drvdata(dev);
<<<<<<< HEAD
	int error;

	if (data->state == APPMODE) {
		error = mxt_stop(data);
		if (error < 0)
			dev_err(&data->client->dev, "mxt_stop failed in input_close\n");
	}
}

static int reg_set_optimum_mode_check(struct regulator *reg, int load_uA)
{
	return (regulator_count_voltages(reg) > 0) ?
		regulator_set_optimum_mode(reg, load_uA) : 0;
}

static int mxt_power_on(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto power_off;

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		return rc;
	}

	rc = regulator_enable(data->vcc_ana);
	if (rc) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana enable failed rc=%d\n", rc);
		goto error_reg_en_vcc_ana;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n",
				rc);
			goto error_reg_opt_vcc_dig;
		}

		rc = regulator_enable(data->vcc_dig);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_dig;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto error_reg_opt_i2c;
		}

		rc = regulator_enable(data->vcc_i2c);
		if (rc) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c enable failed rc=%d\n", rc);
			goto error_reg_en_vcc_i2c;
		}
	}

	msleep(130);

	return 0;

error_reg_en_vcc_i2c:
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
error_reg_opt_i2c:
	if (data->pdata->digital_pwr_regulator)
		regulator_disable(data->vcc_dig);
error_reg_en_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig, 0);
error_reg_opt_vcc_dig:
	regulator_disable(data->vcc_ana);
error_reg_en_vcc_ana:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	return rc;

power_off:
	reg_set_optimum_mode_check(data->vcc_ana, 0);
	regulator_disable(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		reg_set_optimum_mode_check(data->vcc_dig, 0);
		regulator_disable(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		reg_set_optimum_mode_check(data->vcc_i2c, 0);
		regulator_disable(data->vcc_i2c);
	}

	msleep(50);
	return 0;
}

static int mxt_regulator_configure(struct mxt_data *data, bool on)
{
	int rc;

	if (on == false)
		goto hw_shutdown;

	data->vcc_ana = regulator_get(&data->client->dev, "vdd_ana");
	if (IS_ERR(data->vcc_ana)) {
		rc = PTR_ERR(data->vcc_ana);
		dev_err(&data->client->dev,
			"Regulator get failed vcc_ana rc=%d\n", rc);
		return rc;
	}

	if (regulator_count_voltages(data->vcc_ana) > 0) {
		rc = regulator_set_voltage(data->vcc_ana, MXT_VTG_MIN_UV,
							MXT_VTG_MAX_UV);
		if (rc) {
			dev_err(&data->client->dev,
				"regulator set_vtg failed rc=%d\n", rc);
			goto error_set_vtg_vcc_ana;
		}
	}
	if (data->pdata->digital_pwr_regulator) {
		data->vcc_dig = regulator_get(&data->client->dev, "vdd_dig");
		if (IS_ERR(data->vcc_dig)) {
			rc = PTR_ERR(data->vcc_dig);
			dev_err(&data->client->dev,
				"Regulator get dig failed rc=%d\n", rc);
			goto error_get_vtg_vcc_dig;
		}

		if (regulator_count_voltages(data->vcc_dig) > 0) {
			rc = regulator_set_voltage(data->vcc_dig,
				MXT_VTG_DIG_MIN_UV, MXT_VTG_DIG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_vcc_dig;
			}
		}
	}
	if (data->pdata->i2c_pull_up) {
		data->vcc_i2c = regulator_get(&data->client->dev, "vcc_i2c");
		if (IS_ERR(data->vcc_i2c)) {
			rc = PTR_ERR(data->vcc_i2c);
			dev_err(&data->client->dev,
				"Regulator get failed rc=%d\n",	rc);
			goto error_get_vtg_i2c;
		}
		if (regulator_count_voltages(data->vcc_i2c) > 0) {
			rc = regulator_set_voltage(data->vcc_i2c,
				MXT_I2C_VTG_MIN_UV, MXT_I2C_VTG_MAX_UV);
			if (rc) {
				dev_err(&data->client->dev,
					"regulator set_vtg failed rc=%d\n", rc);
				goto error_set_vtg_i2c;
			}
		}
	}

	return 0;

error_set_vtg_i2c:
	regulator_put(data->vcc_i2c);
error_get_vtg_i2c:
	if (data->pdata->digital_pwr_regulator)
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
				MXT_VTG_DIG_MAX_UV);
error_set_vtg_vcc_dig:
	if (data->pdata->digital_pwr_regulator)
		regulator_put(data->vcc_dig);
error_get_vtg_vcc_dig:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
error_set_vtg_vcc_ana:
	regulator_put(data->vcc_ana);
	return rc;

hw_shutdown:
	if (regulator_count_voltages(data->vcc_ana) > 0)
		regulator_set_voltage(data->vcc_ana, 0, MXT_VTG_MAX_UV);
	regulator_put(data->vcc_ana);
	if (data->pdata->digital_pwr_regulator) {
		if (regulator_count_voltages(data->vcc_dig) > 0)
			regulator_set_voltage(data->vcc_dig, 0,
						MXT_VTG_DIG_MAX_UV);
		regulator_put(data->vcc_dig);
	}
	if (data->pdata->i2c_pull_up) {
		if (regulator_count_voltages(data->vcc_i2c) > 0)
			regulator_set_voltage(data->vcc_i2c, 0,
						MXT_I2C_VTG_MAX_UV);
		regulator_put(data->vcc_i2c);
	}
	return 0;
}

#ifdef CONFIG_PM
static int mxt_regulator_lpm(struct mxt_data *data, bool on)
{

	int rc;

	if (on == false)
		goto regulator_hpm;

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_lpm;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
						MXT_LPM_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c,
						MXT_I2C_LPM_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_lpm;
		}
	}

	return 0;

regulator_hpm:

	rc = reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (rc < 0) {
		dev_err(&data->client->dev,
			"Regulator vcc_ana set_opt failed rc=%d\n", rc);
		goto fail_regulator_hpm;
	}

	if (data->pdata->digital_pwr_regulator) {
		rc = reg_set_optimum_mode_check(data->vcc_dig,
						 MXT_ACTIVE_LOAD_DIG_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_dig set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	if (data->pdata->i2c_pull_up) {
		rc = reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);
		if (rc < 0) {
			dev_err(&data->client->dev,
				"Regulator vcc_i2c set_opt failed rc=%d\n", rc);
			goto fail_regulator_hpm;
		}
	}

	return 0;

fail_regulator_lpm:
	reg_set_optimum_mode_check(data->vcc_ana, MXT_ACTIVE_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig,
					MXT_ACTIVE_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LOAD_UA);

	return rc;

fail_regulator_hpm:
	reg_set_optimum_mode_check(data->vcc_ana, MXT_LPM_LOAD_UA);
	if (data->pdata->digital_pwr_regulator)
		reg_set_optimum_mode_check(data->vcc_dig, MXT_LPM_LOAD_DIG_UA);
	if (data->pdata->i2c_pull_up)
		reg_set_optimum_mode_check(data->vcc_i2c, MXT_I2C_LPM_LOAD_UA);

	return rc;
}

static int mxt_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	if (data->dev_sleep) {
		dev_dbg(dev, "Device already in sleep\n");
		return 0;
	}

	disable_irq(data->irq);

	mutex_lock(&input_dev->mutex);
	if (input_dev->users) {
		error = mxt_stop(data);
		if (error < 0) {
			dev_err(dev, "mxt_stop failed in suspend\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}
	}

	mutex_unlock(&input_dev->mutex);
	mxt_release_all(data);

	/* put regulators in low power mode */
	if (data->lpm_support) {
		error = mxt_regulator_lpm(data, true);
		if (error < 0) {
			dev_err(dev, "failed to enter low power mode\n");
			return error;
		}
	} else {
		error = mxt_power_on(data, false);
		if (error < 0) {
			dev_err(dev, "failed to disable regulators\n");
			return error;
		}
	}

	data->dev_sleep = true;
	return 0;
}

static int mxt_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;
	int error;

	if (!data->dev_sleep) {
		dev_dbg(dev, "Device already in resume\n");
		return 0;
	}

	/* put regulators back in active power mode */
	if (data->lpm_support) {
		error = mxt_regulator_lpm(data, false);
		if (error < 0) {
			dev_err(dev, "failed to enter high power mode\n");
			return error;
		}
	} else {
		error = mxt_power_on(data, true);
		if (error < 0) {
			dev_err(dev, "failed to enable regulators\n");
			return error;
		}
		mxt_power_on_delay(data);
	}

	mxt_write_object(data, MXT_GEN_COMMAND_T6, MXT_COMMAND_RESET, 1);
	mxt_reset_delay(data);

	mutex_lock(&input_dev->mutex);

	if (input_dev->users) {
		error = mxt_start(data);
		if (error < 0) {
			dev_err(dev, "mxt_start failed in resume\n");
			mutex_unlock(&input_dev->mutex);
			return error;
		}
	}

	/* calibrate */
	if (data->pdata->need_calibration) {
		mxt_secure_touch_stop(data, 1);
		error = mxt_write_object(data, MXT_GEN_COMMAND_T6,
					MXT_COMMAND_CALIBRATE, 1);
		if (error < 0)
			dev_dbg(dev, "sending calibration command failed\n");
	}

	mutex_unlock(&input_dev->mutex);

	enable_irq(data->irq);

	data->dev_sleep = false;
	return 0;
}

static const struct dev_pm_ops mxt_pm_ops = {
#if (!defined(CONFIG_FB) && !defined(CONFIG_HAS_EARLYSUSPEND))
	.suspend	= mxt_suspend,
	.resume		= mxt_resume,
#endif
};
#else
static int mxt_suspend(struct device *dev)
{
	return 0;
};
static int mxt_resume(struct device *dev)
{
	return 0;
};
#endif

static int mxt_debugfs_object_show(struct seq_file *m, void *v)
{
	struct mxt_data *data = m->private;
	struct mxt_object *object;
	struct device *dev = &data->client->dev;
	int i, j, k;
	int error;
	int obj_size;
	u8 val;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;
		obj_size = object->size + 1;

		seq_printf(m, "Object[%d] (Type %d)\n", i + 1, object->type);

		for (j = 0; j < object->instances + 1; j++) {
			seq_printf(m, "[Instance %d]\n", j);

			for (k = 0; k < obj_size; k++) {
				error = mxt_read_object(data, object->type,
							j * obj_size + k, &val);
				if (error) {
					dev_err(dev,
						"Failed to read object %d "
						"instance %d at offset %d\n",
						object->type, j, k);
					return error;
				}

				seq_printf(m, "Byte %d: 0x%02x (%d)\n",
						k, val, val);
			}
		}
=======

	mxt_stop(data);
}

static int mxt_parse_device_properties(struct mxt_data *data)
{
	static const char keymap_property[] = "linux,gpio-keymap";
	static const char buttons_property[] = "linux,keycodes";
	struct device *dev = &data->client->dev;
	u32 *keymap;
	u32 *buttonmap;
	int n_keys;
	int error;

	if (device_property_present(dev, keymap_property)) {
		n_keys = device_property_count_u32(dev, keymap_property);
		if (n_keys <= 0) {
			error = n_keys < 0 ? n_keys : -EINVAL;
			dev_err(dev, "invalid/malformed '%s' property: %d\n",
				keymap_property, error);
			return error;
		}

		keymap = devm_kmalloc_array(dev, n_keys, sizeof(*keymap),
					    GFP_KERNEL);
		if (!keymap)
			return -ENOMEM;

		error = device_property_read_u32_array(dev, keymap_property,
						       keymap, n_keys);
		if (error) {
			dev_err(dev, "failed to parse '%s' property: %d\n",
				keymap_property, error);
			return error;
		}

		data->t19_keymap = keymap;
		data->t19_num_keys = n_keys;
	}

	if (device_property_present(dev, buttons_property)) {
		n_keys = device_property_count_u32(dev, buttons_property);
		if (n_keys <= 0) {
			error = n_keys < 0 ? n_keys : -EINVAL;
			dev_err(dev, "invalid/malformed '%s' property: %d\n",
				buttons_property, error);
			return error;
		}

		buttonmap = devm_kmalloc_array(dev, n_keys, sizeof(*buttonmap),
					       GFP_KERNEL);
		if (!buttonmap)
			return -ENOMEM;

		error = device_property_read_u32_array(dev, buttons_property,
						       buttonmap, n_keys);
		if (error) {
			dev_err(dev, "failed to parse '%s' property: %d\n",
				buttons_property, error);
			return error;
		}

		data->t15_keymap = buttonmap;
		data->t15_num_keys = n_keys;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int mxt_debugfs_object_open(struct inode *inode, struct file *file)
{
	return single_open(file, mxt_debugfs_object_show, inode->i_private);
}

static const struct file_operations mxt_object_fops = {
	.owner		= THIS_MODULE,
	.open		= mxt_debugfs_object_open,
	.read		= seq_read,
	.release	= single_release,
};

static void __devinit mxt_debugfs_init(struct mxt_data *data)
{
	debug_base = debugfs_create_dir(MXT_DEBUGFS_DIR, NULL);
	if (IS_ERR_OR_NULL(debug_base))
		pr_err("atmel_mxt_ts: Failed to create debugfs dir\n");
	if (IS_ERR_OR_NULL(debugfs_create_file(MXT_DEBUGFS_FILE,
					       0444,
					       debug_base,
					       data,
					       &mxt_object_fops))) {
		pr_err("atmel_mxt_ts: Failed to create object file\n");
		debugfs_remove_recursive(debug_base);
	}
}

#ifdef CONFIG_OF
static int mxt_get_dt_coords(struct device *dev, char *name,
				struct mxt_platform_data *pdata)
{
	u32 coords[MXT_COORDS_ARR_SIZE];
	struct property *prop;
	struct device_node *np = dev->of_node;
	int coords_size, rc;

	prop = of_find_property(np, name, NULL);
	if (!prop)
		return -EINVAL;
	if (!prop->value)
		return -ENODATA;

	coords_size = prop->length / sizeof(u32);
	if (coords_size != MXT_COORDS_ARR_SIZE) {
		dev_err(dev, "invalid %s\n", name);
		return -EINVAL;
	}

	rc = of_property_read_u32_array(np, name, coords, coords_size);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read %s\n", name);
		return rc;
	}

	if (strncmp(name, "atmel,panel-coords",
			sizeof("atmel,panel-coords")) == 0) {
		pdata->panel_minx = coords[0];
		pdata->panel_miny = coords[1];
		pdata->panel_maxx = coords[2];
		pdata->panel_maxy = coords[3];
	} else if (strncmp(name, "atmel,display-coords",
			sizeof("atmel,display-coords")) == 0) {
		pdata->disp_minx = coords[0];
		pdata->disp_miny = coords[1];
		pdata->disp_maxx = coords[2];
		pdata->disp_maxy = coords[3];
	} else {
		dev_err(dev, "unsupported property %s\n", name);
		return -EINVAL;
	}

	return 0;
}

static int mxt_parse_config(struct device *dev, struct device_node *np,
				struct mxt_config_info *info)
{
	struct property *prop;
	u8 *temp_cfg;

	prop = of_find_property(np, "atmel,config", &info->config_length);
	if (!prop) {
		dev_err(dev, "Looking up %s property in node %s failed",
			"atmel,config", np->full_name);
		return -ENODEV;
	} else if (!info->config_length) {
		dev_err(dev, "Invalid length of configuration data\n");
		return -EINVAL;
	}

	temp_cfg = devm_kzalloc(dev,
			info->config_length * sizeof(u8), GFP_KERNEL);
	if (!temp_cfg) {
		dev_err(dev, "Unable to allocate memory to store cfg\n");
		return -ENOMEM;
	}

	memcpy(temp_cfg, prop->value, info->config_length);
	info->config = temp_cfg;

	return 0;
}

static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	int rc;
	struct mxt_config_info *info;
	struct device_node *temp, *np = dev->of_node;
	struct property *prop;
	u32 temp_val;

	rc = mxt_get_dt_coords(dev, "atmel,panel-coords", pdata);
	if (rc)
		return rc;

	rc = mxt_get_dt_coords(dev, "atmel,display-coords", pdata);
	if (rc)
		return rc;

	/* regulator info */
	pdata->i2c_pull_up = of_property_read_bool(np, "atmel,i2c-pull-up");
	pdata->digital_pwr_regulator = of_property_read_bool(np,
						"atmel,dig-reg-support");

	pdata->no_force_update = of_property_read_bool(np,
						"atmel,no-force-update");

	pdata->no_lpm_support = of_property_read_bool(np,
					"atmel,no-lpm-support");

	/* reset, irq gpio info */
	pdata->reset_gpio = of_get_named_gpio_flags(np, "atmel,reset-gpio",
				0, &pdata->reset_gpio_flags);
	pdata->irq_gpio = of_get_named_gpio_flags(np, "atmel,irq-gpio",
				0, &pdata->irq_gpio_flags);

	/* keycodes for keyarray object*/
	prop = of_find_property(np, "atmel,key-codes", NULL);
	if (prop) {
		pdata->key_codes = devm_kzalloc(dev,
				sizeof(int) * MXT_KEYARRAY_MAX_KEYS,
				GFP_KERNEL);
		if (!pdata->key_codes)
			return -ENOMEM;
		if ((prop->length/sizeof(u32)) == MXT_KEYARRAY_MAX_KEYS) {
			rc = of_property_read_u32_array(np, "atmel,key-codes",
				pdata->key_codes, MXT_KEYARRAY_MAX_KEYS);
			if (rc) {
				dev_err(dev, "Unable to read key codes\n");
				return rc;
			}
		} else
			return -EINVAL;
	}

	/* need calibration during wakeup? */
	pdata->need_calibration = of_property_read_bool(np,
					"atmel,need-calibration");
	/* config array size */
	pdata->config_array_size = 0;
	temp = NULL;
	while ((temp = of_get_next_child(np, temp)))
		pdata->config_array_size++;

	if (!pdata->config_array_size)
		return 0;

	info = devm_kzalloc(dev, pdata->config_array_size *
				sizeof(struct mxt_config_info), GFP_KERNEL);
	if (!info) {
		dev_err(dev, "Unable to allocate memory\n");
		return -ENOMEM;
	}

	rc = of_property_read_u32(np, "atmel,bl-addr", &temp_val);
	if (rc && (rc != -EINVAL))
		dev_err(dev, "Unable to read bootloader address\n");
	else if (rc != -EINVAL)
		pdata->bl_addr = (u8) temp_val;

	pdata->config_array  = info;

	for_each_child_of_node(np, temp) {
		rc = of_property_read_string(temp, "atmel,fw-name",
			&info->fw_name);
		if (rc && (rc != -EINVAL)) {
			dev_err(dev, "Unable to read fw name\n");
			return rc;
		}

		rc = of_property_read_u32(temp, "atmel,family-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read family id\n");
			return rc;
		} else
			info->family_id = (u8) temp_val;

		rc  = of_property_read_u32(temp, "atmel,variant-id", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read variant id\n");
			return rc;
		} else
			info->variant_id = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,version", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read controller version\n");
			return rc;
		} else
			info->version = (u8) temp_val;

		rc = of_property_read_u32(temp, "atmel,build", &temp_val);
		if (rc) {
			dev_err(dev, "Unable to read build id\n");
			return rc;
		} else
			info->build = (u8) temp_val;

		rc = of_property_read_u32(temp,
					"atmel,bootldr-id", &temp_val);
		if (rc && (rc != -EINVAL))
			dev_err(dev, "Unable to read bootldr-id\n");
		else if (rc != -EINVAL)
			info->bootldr_id = (u8) temp_val;

		rc = mxt_parse_config(dev, temp, info);
		if (rc) {
			dev_err(dev, "Unable to parse config data\n");
			return rc;
		}
		info++;
	}

	return 0;
}
#else
static int mxt_parse_dt(struct device *dev, struct mxt_platform_data *pdata)
{
	return -ENODEV;
}
#endif

#if defined(CONFIG_FB)
static int fb_notifier_callback(struct notifier_block *self,
				 unsigned long event, void *data)
{
	struct fb_event *evdata = data;
	int *blank;
	struct mxt_data *mxt_dev_data =
		container_of(self, struct mxt_data, fb_notif);

	if (evdata && evdata->data && mxt_dev_data && mxt_dev_data->client) {
		if (event == FB_EVENT_BLANK) {
			blank = evdata->data;
			if (*blank == FB_BLANK_UNBLANK)
				mxt_resume(&mxt_dev_data->client->dev);
			else if (*blank == FB_BLANK_POWERDOWN)
				mxt_suspend(&mxt_dev_data->client->dev);
		}
	}

	return 0;
}
#elif defined(CONFIG_HAS_EARLYSUSPEND)
static void mxt_early_suspend(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data,
						early_suspend);
	mxt_suspend(&data->client->dev);
}

static void mxt_late_resume(struct early_suspend *h)
{
	struct mxt_data *data = container_of(h, struct mxt_data,
						early_suspend);
	mxt_resume(&data->client->dev);
}

#endif

#if defined(CONFIG_SECURE_TOUCH)
static void __devinit mxt_secure_touch_init(struct mxt_data *data)
{
	init_completion(&data->st_powerdown);
}
#else
static void __devinit mxt_secure_touch_init(struct mxt_data *data)
{
}
#endif

static int __devinit mxt_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct mxt_platform_data *pdata;
	struct mxt_data *data;
	struct input_dev *input_dev;
	int error, i;

	if (client->dev.of_node) {
		pdata = devm_kzalloc(&client->dev,
			sizeof(struct mxt_platform_data), GFP_KERNEL);
		if (!pdata) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}

		error = mxt_parse_dt(&client->dev, pdata);
		if (error)
			return error;
	} else
		pdata = client->dev.platform_data;

	if (!pdata)
		return -EINVAL;

	data = kzalloc(sizeof(struct mxt_data), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!data || !input_dev) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	data->state = INIT;
	input_dev->name = "atmel_mxt_ts";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;
	input_dev->open = mxt_input_open;
	input_dev->close = mxt_input_close;

	data->client = client;
	data->input_dev = input_dev;
	data->pdata = pdata;
	data->no_force_update = pdata->no_force_update;
	data->lpm_support = !pdata->no_lpm_support;
	data->dev_sleep = false;

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);

	/* For single touch */
	input_set_abs_params(input_dev, ABS_X,
			pdata->disp_minx, pdata->disp_maxx, 0, 0);
	input_set_abs_params(input_dev, ABS_Y,
			pdata->disp_miny, pdata->disp_maxy, 0, 0);
	input_set_abs_params(input_dev, ABS_PRESSURE,
			     0, 255, 0, 0);

	/* For multi touch */
	input_mt_init_slots(input_dev, MXT_MAX_FINGER);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
			     0, MXT_MAX_AREA, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
			pdata->disp_minx, pdata->disp_maxx, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
			pdata->disp_miny, pdata->disp_maxy, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE,
			     0, 255, 0, 0);

	/* set key array supported keys */
	if (pdata->key_codes) {
		for (i = 0; i < MXT_KEYARRAY_MAX_KEYS; i++) {
			if (pdata->key_codes[i])
				input_set_capability(input_dev, EV_KEY,
							pdata->key_codes[i]);
		}
	}

	input_set_drvdata(input_dev, data);
	i2c_set_clientdata(client, data);

	if (pdata->init_hw)
		error = pdata->init_hw(true);
	else
		error = mxt_regulator_configure(data, true);
	if (error) {
		dev_err(&client->dev, "Failed to intialize hardware\n");
		goto err_free_mem;
	}

	if (gpio_is_valid(pdata->reset_gpio)) {
		/* configure touchscreen reset out gpio */
		error = gpio_request(pdata->reset_gpio, "mxt_reset_gpio");
		if (error) {
			dev_err(&client->dev, "unable to request gpio [%d]\n",
						pdata->reset_gpio);
			goto err_regulator_on;
		}

		error = gpio_direction_output(pdata->reset_gpio, 0);
		if (error) {
			dev_err(&client->dev,
				"unable to set direction for gpio [%d]\n",
				pdata->reset_gpio);
			goto err_reset_gpio_req;
		}
		mxt_reset_delay(data);
	}

	if (pdata->power_on)
		error = pdata->power_on(true);
	else
		error = mxt_power_on(data, true);
	if (error) {
		dev_err(&client->dev, "Failed to power on hardware\n");
		goto err_reset_gpio_req;
	}

	if (gpio_is_valid(pdata->irq_gpio)) {
		/* configure touchscreen irq gpio */
		error = gpio_request(pdata->irq_gpio, "mxt_irq_gpio");
		if (error) {
			dev_err(&client->dev, "unable to request gpio [%d]\n",
						pdata->irq_gpio);
			goto err_power_on;
		}
		error = gpio_direction_input(pdata->irq_gpio);
		if (error) {
			dev_err(&client->dev,
				"unable to set direction for gpio [%d]\n",
				pdata->irq_gpio);
			goto err_irq_gpio_req;
		}
		data->irq = client->irq = gpio_to_irq(pdata->irq_gpio);
	} else {
		dev_err(&client->dev, "irq gpio not provided\n");
		goto err_power_on;
	}

	if (gpio_is_valid(pdata->reset_gpio)) {
		error = gpio_direction_output(pdata->reset_gpio, 1);
		if (error) {
			dev_err(&client->dev,
				"unable to set direction for gpio [%d]\n",
				pdata->reset_gpio);
				goto err_irq_gpio_req;
		}
	}

	mxt_power_on_delay(data);

	data->addr_pair.application = data->client->addr;

	if (pdata->bl_addr)
		data->addr_pair.bootloader = pdata->bl_addr;
	else
		mxt_lookup_bootloader_address(data);

	error = mxt_initialize(data);
	if (error)
		goto err_irq_gpio_req;

	error = request_threaded_irq(client->irq, NULL, mxt_interrupt,
			pdata->irqflags, client->dev.driver->name, data);
	if (error) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		goto err_free_object;
	}

	if (data->state == APPMODE) {
		error = mxt_make_highchg(data);
		if (error) {
			dev_err(&client->dev, "Failed to make high CHG\n");
			goto err_free_irq;
		}
	}

	error = input_register_device(input_dev);
	if (error)
		goto err_free_irq;

	error = sysfs_create_group(&client->dev.kobj, &mxt_attr_group);
	if (error)
		goto err_unregister_device;

#if defined(CONFIG_FB)
	data->fb_notif.notifier_call = fb_notifier_callback;

	error = fb_register_client(&data->fb_notif);

	if (error)
		dev_err(&client->dev, "Unable to register fb_notifier: %d\n",
			error);
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN +
						MXT_SUSPEND_LEVEL;
	data->early_suspend.suspend = mxt_early_suspend;
	data->early_suspend.resume = mxt_late_resume;
	register_early_suspend(&data->early_suspend);
#endif

	mxt_debugfs_init(data);

	mxt_secure_touch_init(data);

	return 0;

err_unregister_device:
	input_unregister_device(input_dev);
	input_dev = NULL;
err_free_irq:
	free_irq(client->irq, data);
err_free_object:
	kfree(data->object_table);
err_irq_gpio_req:
	if (gpio_is_valid(pdata->irq_gpio))
		gpio_free(pdata->irq_gpio);
err_power_on:
	if (pdata->power_on)
		pdata->power_on(false);
	else
		mxt_power_on(data, false);
err_reset_gpio_req:
	if (gpio_is_valid(pdata->reset_gpio))
		gpio_free(pdata->reset_gpio);
err_regulator_on:
	if (pdata->init_hw)
		pdata->init_hw(false);
	else
		mxt_regulator_configure(data, false);
err_free_mem:
	input_free_device(input_dev);
	kfree(data);
	return error;
}

static int __devexit mxt_remove(struct i2c_client *client)
{
	struct mxt_data *data = i2c_get_clientdata(client);

	sysfs_remove_group(&client->dev.kobj, &mxt_attr_group);
	free_irq(data->irq, data);
	input_unregister_device(data->input_dev);
#if defined(CONFIG_FB)
	if (fb_unregister_client(&data->fb_notif))
		dev_err(&client->dev, "Error occurred while unregistering fb_notifier.\n");
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	unregister_early_suspend(&data->early_suspend);
#endif

	if (data->pdata->power_on)
		data->pdata->power_on(false);
	else
		mxt_power_on(data, false);

	if (data->pdata->init_hw)
		data->pdata->init_hw(false);
	else
		mxt_regulator_configure(data, false);

	if (gpio_is_valid(data->pdata->reset_gpio))
		gpio_free(data->pdata->reset_gpio);

	if (gpio_is_valid(data->pdata->irq_gpio))
		gpio_free(data->pdata->irq_gpio);

	kfree(data->object_table);
	kfree(data);

	debugfs_remove_recursive(debug_base);
=======
static const struct dmi_system_id chromebook_T9_suspend_dmi[] = {
	{
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "GOOGLE"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Link"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_PRODUCT_NAME, "Peppy"),
		},
	},
	{ }
};

static int mxt_probe(struct i2c_client *client)
{
	struct mxt_data *data;
	int error;

	/*
	 * Ignore devices that do not have device properties attached to
	 * them, as we need help determining whether we are dealing with
	 * touch screen or touchpad.
	 *
	 * So far on x86 the only users of Atmel touch controllers are
	 * Chromebooks, and chromeos_laptop driver will ensure that
	 * necessary properties are provided (if firmware does not do that).
	 */
	if (!device_property_present(&client->dev, "compatible"))
		return -ENXIO;

	/*
	 * Ignore ACPI devices representing bootloader mode.
	 *
	 * This is a bit of a hack: Google Chromebook BIOS creates ACPI
	 * devices for both application and bootloader modes, but we are
	 * interested in application mode only (if device is in bootloader
	 * mode we'll end up switching into application anyway). So far
	 * application mode addresses were all above 0x40, so we'll use it
	 * as a threshold.
	 */
	if (ACPI_COMPANION(&client->dev) && client->addr < 0x40)
		return -ENXIO;

	data = devm_kzalloc(&client->dev, sizeof(struct mxt_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	snprintf(data->phys, sizeof(data->phys), "i2c-%u-%04x/input0",
		 client->adapter->nr, client->addr);

	data->client = client;
	data->irq = client->irq;
	i2c_set_clientdata(client, data);

	init_completion(&data->bl_completion);
	init_completion(&data->reset_completion);
	init_completion(&data->crc_completion);

	data->suspend_mode = dmi_check_system(chromebook_T9_suspend_dmi) ?
		MXT_SUSPEND_T9_CTRL : MXT_SUSPEND_DEEP_SLEEP;

	error = mxt_parse_device_properties(data);
	if (error)
		return error;

	/*
	 * VDDA is the analog voltage supply 2.57..3.47 V
	 * VDD  is the digital voltage supply 1.71..3.47 V
	 */
	data->regulators[0].supply = "vdda";
	data->regulators[1].supply = "vdd";
	error = devm_regulator_bulk_get(&client->dev, ARRAY_SIZE(data->regulators),
					data->regulators);
	if (error) {
		if (error != -EPROBE_DEFER)
			dev_err(&client->dev, "Failed to get regulators %d\n",
				error);
		return error;
	}

	/* Request the RESET line as asserted so we go into reset */
	data->reset_gpio = devm_gpiod_get_optional(&client->dev,
						   "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(data->reset_gpio)) {
		error = PTR_ERR(data->reset_gpio);
		dev_err(&client->dev, "Failed to get reset gpio: %d\n", error);
		return error;
	}

	/* Request the WAKE line as asserted so we go out of sleep */
	data->wake_gpio = devm_gpiod_get_optional(&client->dev,
						  "wake", GPIOD_OUT_HIGH);
	if (IS_ERR(data->wake_gpio)) {
		error = PTR_ERR(data->wake_gpio);
		dev_err(&client->dev, "Failed to get wake gpio: %d\n", error);
		return error;
	}

	error = devm_request_threaded_irq(&client->dev, client->irq,
					  NULL, mxt_interrupt,
					  IRQF_ONESHOT | IRQF_NO_AUTOEN,
					  client->name, data);
	if (error) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		return error;
	}

	error = regulator_bulk_enable(ARRAY_SIZE(data->regulators),
				      data->regulators);
	if (error) {
		dev_err(&client->dev, "failed to enable regulators: %d\n",
			error);
		return error;
	}
	/*
	 * The device takes 40ms to come up after power-on according
	 * to the mXT224 datasheet, page 13.
	 */
	msleep(MXT_BACKUP_TIME);

	if (data->reset_gpio) {
		/* Wait a while and then de-assert the RESET GPIO line */
		msleep(MXT_RESET_GPIO_TIME);
		gpiod_set_value(data->reset_gpio, 0);
		msleep(MXT_RESET_INVALID_CHG);
	}

	/*
	 * Controllers like mXT1386 have a dedicated WAKE line that could be
	 * connected to a GPIO or to I2C SCL pin, or permanently asserted low.
	 *
	 * This WAKE line is used for waking controller from a deep-sleep and
	 * it needs to be asserted low for 25 milliseconds before I2C transfers
	 * could be accepted by controller if it was in a deep-sleep mode.
	 * Controller will go into sleep automatically after 2 seconds of
	 * inactivity if WAKE line is deasserted and deep sleep is activated.
	 *
	 * If WAKE line is connected to I2C SCL pin, then the first I2C transfer
	 * will get an instant NAK and transfer needs to be retried after 25ms.
	 *
	 * If WAKE line is connected to a GPIO line, the line must be asserted
	 * 25ms before the host attempts to communicate with the controller.
	 */
	device_property_read_u32(&client->dev, "atmel,wakeup-method",
				 &data->wakeup_method);

	error = mxt_initialize(data);
	if (error)
		goto err_disable_regulators;

	error = sysfs_create_group(&client->dev.kobj, &mxt_attr_group);
	if (error) {
		dev_err(&client->dev, "Failure %d creating sysfs group\n",
			error);
		goto err_free_object;
	}

	return 0;

err_free_object:
	mxt_free_input_device(data);
	mxt_free_object_table(data);
err_disable_regulators:
	regulator_bulk_disable(ARRAY_SIZE(data->regulators),
			       data->regulators);
	return error;
}

static void mxt_remove(struct i2c_client *client)
{
	struct mxt_data *data = i2c_get_clientdata(client);

	disable_irq(data->irq);
	sysfs_remove_group(&client->dev.kobj, &mxt_attr_group);
	mxt_free_input_device(data);
	mxt_free_object_table(data);
	regulator_bulk_disable(ARRAY_SIZE(data->regulators),
			       data->regulators);
}

static int mxt_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;

	if (!input_dev)
		return 0;

	mutex_lock(&input_dev->mutex);

	if (input_device_enabled(input_dev))
		mxt_stop(data);

	mutex_unlock(&input_dev->mutex);

	disable_irq(data->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct i2c_device_id mxt_id[] = {
	{ "qt602240_ts", 0 },
	{ "atmel_mxt_ts", 0 },
=======
static int mxt_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mxt_data *data = i2c_get_clientdata(client);
	struct input_dev *input_dev = data->input_dev;

	if (!input_dev)
		return 0;

	enable_irq(data->irq);

	mutex_lock(&input_dev->mutex);

	if (input_device_enabled(input_dev))
		mxt_start(data);

	mutex_unlock(&input_dev->mutex);

	return 0;
}

static DEFINE_SIMPLE_DEV_PM_OPS(mxt_pm_ops, mxt_suspend, mxt_resume);

static const struct of_device_id mxt_of_match[] = {
	{ .compatible = "atmel,maxtouch", },
	/* Compatibles listed below are deprecated */
	{ .compatible = "atmel,qt602240_ts", },
	{ .compatible = "atmel,atmel_mxt_ts", },
	{ .compatible = "atmel,atmel_mxt_tp", },
	{ .compatible = "atmel,mXT224", },
	{},
};
MODULE_DEVICE_TABLE(of, mxt_of_match);

#ifdef CONFIG_ACPI
static const struct acpi_device_id mxt_acpi_id[] = {
	{ "ATML0000", 0 },	/* Touchpad */
	{ "ATML0001", 0 },	/* Touchscreen */
	{ }
};
MODULE_DEVICE_TABLE(acpi, mxt_acpi_id);
#endif

static const struct i2c_device_id mxt_id[] = {
	{ "qt602240_ts", 0 },
	{ "atmel_mxt_ts", 0 },
	{ "atmel_mxt_tp", 0 },
	{ "maxtouch", 0 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ "mXT224", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mxt_id);
<<<<<<< HEAD
#ifdef CONFIG_OF
static struct of_device_id mxt_match_table[] = {
	{ .compatible = "atmel,mxt-ts",},
	{ },
};
#else
#define mxt_match_table NULL
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct i2c_driver mxt_driver = {
	.driver = {
		.name	= "atmel_mxt_ts",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.of_match_table = mxt_match_table,
#ifdef CONFIG_PM
		.pm	= &mxt_pm_ops,
#endif
	},
	.probe		= mxt_probe,
	.remove		= __devexit_p(mxt_remove),
=======
		.of_match_table = mxt_of_match,
		.acpi_match_table = ACPI_PTR(mxt_acpi_id),
		.pm	= pm_sleep_ptr(&mxt_pm_ops),
	},
	.probe		= mxt_probe,
	.remove		= mxt_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= mxt_id,
};

module_i2c_driver(mxt_driver);

/* Module information */
MODULE_AUTHOR("Joonyoung Shim <jy0922.shim@samsung.com>");
MODULE_DESCRIPTION("Atmel maXTouch Touchscreen driver");
MODULE_LICENSE("GPL");
