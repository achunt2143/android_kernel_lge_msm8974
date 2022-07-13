<<<<<<< HEAD
/*
 * Copyright (C) 2004 IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2004 IBM Corporation
 * Copyright (C) 2015 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Authors:
 * Leendert van Doorn <leendert@watson.ibm.com>
 * Dave Safford <safford@watson.ibm.com>
 * Reiner Sailer <sailer@watson.ibm.com>
 * Kylene Hall <kjhall@us.ibm.com>
 *
 * Maintained by: <tpmdd-devel@lists.sourceforge.net>
 *
 * Device driver for TCG/TCPA TPM (trusted platform module).
<<<<<<< HEAD
 * Specifications at www.trustedcomputinggroup.org	 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 * 
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/tpm.h>

enum tpm_timeout {
	TPM_TIMEOUT = 5,	/* msecs */
=======
 * Specifications at www.trustedcomputinggroup.org
 */

#ifndef __TPM_H__
#define __TPM_H__

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/tpm.h>
#include <linux/tpm_eventlog.h>

#ifdef CONFIG_X86
#include <asm/intel-family.h>
#endif

#define TPM_MINOR		224	/* officially assigned */
#define TPM_BUFSIZE		4096
#define TPM_NUM_DEVICES		65536
#define TPM_RETRY		50

enum tpm_timeout {
	TPM_TIMEOUT = 5,	/* msecs */
	TPM_TIMEOUT_RETRY = 100, /* msecs */
	TPM_TIMEOUT_RANGE_US = 300,	/* usecs */
	TPM_TIMEOUT_POLL = 1,	/* msecs */
	TPM_TIMEOUT_USECS_MIN = 100,      /* usecs */
	TPM_TIMEOUT_USECS_MAX = 500      /* usecs */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* TPM addresses */
enum tpm_addr {
	TPM_SUPERIO_ADDR = 0x2E,
	TPM_ADDR = 0x4E,
};

<<<<<<< HEAD
#define TPM_WARN_DOING_SELFTEST 0x802
#define TPM_ERR_DEACTIVATED     0x6
#define TPM_ERR_DISABLED        0x7

#define TPM_HEADER_SIZE		10
extern ssize_t tpm_show_pubek(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_pcrs(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_caps(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_caps_1_2(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_store_cancel(struct device *, struct device_attribute *attr,
				const char *, size_t);
extern ssize_t tpm_show_enabled(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_active(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_owned(struct device *, struct device_attribute *attr,
				char *);
extern ssize_t tpm_show_temp_deactivated(struct device *,
					 struct device_attribute *attr, char *);
extern ssize_t tpm_show_durations(struct device *,
				  struct device_attribute *attr, char *);
extern ssize_t tpm_show_timeouts(struct device *,
				 struct device_attribute *attr, char *);

struct tpm_chip;

struct tpm_vendor_specific {
	const u8 req_complete_mask;
	const u8 req_complete_val;
	const u8 req_canceled;
	void __iomem *iobase;		/* ioremapped address */
	unsigned long base;		/* TPM base address */

	int irq;
	int probed_irq;

	int region_size;
	int have_region;

	int (*recv) (struct tpm_chip *, u8 *, size_t);
	int (*send) (struct tpm_chip *, u8 *, size_t);
	void (*cancel) (struct tpm_chip *);
	u8 (*status) (struct tpm_chip *);
	void (*release) (struct device *);
	struct miscdevice miscdev;
	struct attribute_group *attr_group;
	struct list_head list;
	int locality;
	unsigned long timeout_a, timeout_b, timeout_c, timeout_d; /* jiffies */
	bool timeout_adjusted;
	unsigned long duration[3]; /* jiffies */
	bool duration_adjusted;

	wait_queue_head_t read_queue;
	wait_queue_head_t int_queue;
};

#define TPM_VID_INTEL    0x8086

struct tpm_chip {
	struct device *dev;	/* Device stuff */

	int dev_num;		/* /dev/tpm# */
	unsigned long is_open;	/* only one allowed */
	int time_expired;

	/* Data passed to and from the tpm via the read/write calls */
	u8 *data_buffer;
	atomic_t data_pending;
	struct mutex buffer_mutex;

	struct timer_list user_read_timer;	/* user needs to claim result */
	struct work_struct work;
	struct mutex tpm_mutex;	/* tpm is processing */

	struct tpm_vendor_specific vendor;

	struct dentry **bios_dir;

	struct list_head list;
	void (*release) (struct device *);
};

#define to_tpm_chip(n) container_of(n, struct tpm_chip, vendor)

static inline void tpm_chip_put(struct tpm_chip *chip)
{
	module_put(chip->dev->driver->owner);
}

static inline int tpm_read_index(int base, int index)
{
	outb(index, base);
	return inb(base+1) & 0xFF;
}

static inline void tpm_write_index(int base, int index, int value)
{
	outb(index, base);
	outb(value & 0xFF, base+1);
}
struct tpm_input_header {
	__be16	tag;
	__be32	length;
	__be32	ordinal;
}__attribute__((packed));

struct tpm_output_header {
	__be16	tag;
	__be32	length;
	__be32	return_code;
}__attribute__((packed));
=======
#define TPM_WARN_RETRY          0x800
#define TPM_WARN_DOING_SELFTEST 0x802
#define TPM_ERR_DEACTIVATED     0x6
#define TPM_ERR_DISABLED        0x7
#define TPM_ERR_FAILEDSELFTEST  0x1C
#define TPM_ERR_INVALID_POSTINIT 38

#define TPM_TAG_RQU_COMMAND 193

/* TPM2 specific constants. */
#define TPM2_SPACE_BUFFER_SIZE		16384 /* 16 kB */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct	stclear_flags_t {
	__be16	tag;
	u8	deactivated;
	u8	disableForceClear;
	u8	physicalPresence;
	u8	physicalPresenceLock;
	u8	bGlobalLock;
<<<<<<< HEAD
}__attribute__((packed));

struct	tpm_version_t {
	u8	Major;
	u8	Minor;
	u8	revMajor;
	u8	revMinor;
}__attribute__((packed));

struct	tpm_version_1_2_t {
	__be16	tag;
	u8	Major;
	u8	Minor;
	u8	revMajor;
	u8	revMinor;
}__attribute__((packed));
=======
} __packed;

struct tpm1_version {
	u8 major;
	u8 minor;
	u8 rev_major;
	u8 rev_minor;
} __packed;

struct tpm1_version2 {
	__be16 tag;
	struct tpm1_version version;
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct	timeout_t {
	__be32	a;
	__be32	b;
	__be32	c;
	__be32	d;
<<<<<<< HEAD
}__attribute__((packed));
=======
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct duration_t {
	__be32	tpm_short;
	__be32	tpm_medium;
	__be32	tpm_long;
<<<<<<< HEAD
}__attribute__((packed));
=======
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct permanent_flags_t {
	__be16	tag;
	u8	disable;
	u8	ownership;
	u8	deactivated;
	u8	readPubek;
	u8	disableOwnerClear;
	u8	allowMaintenance;
	u8	physicalPresenceLifetimeLock;
	u8	physicalPresenceHWEnable;
	u8	physicalPresenceCMDEnable;
	u8	CEKPUsed;
	u8	TPMpost;
	u8	TPMpostLock;
	u8	FIPS;
	u8	operator;
	u8	enableRevokeEK;
	u8	nvLocked;
	u8	readSRKPub;
	u8	tpmEstablished;
	u8	maintenanceDone;
	u8	disableFullDALogicInfo;
<<<<<<< HEAD
}__attribute__((packed));
=======
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef union {
	struct	permanent_flags_t perm_flags;
	struct	stclear_flags_t	stclear_flags;
<<<<<<< HEAD
	bool	owned;
	__be32	num_pcrs;
	struct	tpm_version_t	tpm_version;
	struct	tpm_version_1_2_t tpm_version_1_2;
=======
	__u8	owned;
	__be32	num_pcrs;
	struct tpm1_version version1;
	struct tpm1_version2 version2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32	manufacturer_id;
	struct timeout_t  timeout;
	struct duration_t duration;
} cap_t;

<<<<<<< HEAD
struct	tpm_getcap_params_in {
	__be32	cap;
	__be32	subcap_size;
	__be32	subcap;
}__attribute__((packed));

struct	tpm_getcap_params_out {
	__be32	cap_size;
	cap_t	cap;
}__attribute__((packed));

struct	tpm_readpubek_params_out {
	u8	algorithm[4];
	u8	encscheme[2];
	u8	sigscheme[2];
	__be32	paramsize;
	u8	parameters[12]; /*assuming RSA*/
	__be32	keysize;
	u8	modulus[256];
	u8	checksum[20];
}__attribute__((packed));

typedef union {
	struct	tpm_input_header in;
	struct	tpm_output_header out;
} tpm_cmd_header;

#define TPM_DIGEST_SIZE 20
struct tpm_pcrread_out {
	u8	pcr_result[TPM_DIGEST_SIZE];
}__attribute__((packed));

struct tpm_pcrread_in {
	__be32	pcr_idx;
}__attribute__((packed));

struct tpm_pcrextend_in {
	__be32	pcr_idx;
	u8	hash[TPM_DIGEST_SIZE];
}__attribute__((packed));

typedef union {
	struct	tpm_getcap_params_out getcap_out;
	struct	tpm_readpubek_params_out readpubek_out;
	u8	readpubek_out_buffer[sizeof(struct tpm_readpubek_params_out)];
	struct	tpm_getcap_params_in getcap_in;
	struct	tpm_pcrread_in	pcrread_in;
	struct	tpm_pcrread_out	pcrread_out;
	struct	tpm_pcrextend_in pcrextend_in;
} tpm_cmd_params;

struct tpm_cmd_t {
	tpm_cmd_header	header;
	tpm_cmd_params	params;
}__attribute__((packed));

ssize_t	tpm_getcap(struct device *, __be32, cap_t *, const char *);

extern int tpm_get_timeouts(struct tpm_chip *);
extern void tpm_gen_interrupt(struct tpm_chip *);
extern int tpm_do_selftest(struct tpm_chip *);
extern unsigned long tpm_calc_ordinal_duration(struct tpm_chip *, u32);
extern struct tpm_chip* tpm_register_hardware(struct device *,
				 const struct tpm_vendor_specific *);
extern int tpm_open(struct inode *, struct file *);
extern int tpm_release(struct inode *, struct file *);
extern void tpm_dev_vendor_release(struct tpm_chip *);
extern ssize_t tpm_write(struct file *, const char __user *, size_t,
			 loff_t *);
extern ssize_t tpm_read(struct file *, char __user *, size_t, loff_t *);
extern void tpm_remove_hardware(struct device *);
extern int tpm_pm_suspend(struct device *, pm_message_t);
extern int tpm_pm_resume(struct device *);
extern int wait_for_tpm_stat(struct tpm_chip *, u8, unsigned long,
			     wait_queue_head_t *);
#ifdef CONFIG_ACPI
extern struct dentry ** tpm_bios_log_setup(char *);
extern void tpm_bios_log_teardown(struct dentry **);
#else
static inline struct dentry ** tpm_bios_log_setup(char *name)
{
	return NULL;
}
static inline void tpm_bios_log_teardown(struct dentry **dir)
{
}
#endif
=======
enum tpm_capabilities {
	TPM_CAP_FLAG = 4,
	TPM_CAP_PROP = 5,
	TPM_CAP_VERSION_1_1 = 0x06,
	TPM_CAP_VERSION_1_2 = 0x1A,
};

enum tpm_sub_capabilities {
	TPM_CAP_PROP_PCR = 0x101,
	TPM_CAP_PROP_MANUFACTURER = 0x103,
	TPM_CAP_FLAG_PERM = 0x108,
	TPM_CAP_FLAG_VOL = 0x109,
	TPM_CAP_PROP_OWNER = 0x111,
	TPM_CAP_PROP_TIS_TIMEOUT = 0x115,
	TPM_CAP_PROP_TIS_DURATION = 0x120,
};

enum tpm2_pt_props {
	TPM2_PT_NONE = 0x00000000,
	TPM2_PT_GROUP = 0x00000100,
	TPM2_PT_FIXED = TPM2_PT_GROUP * 1,
	TPM2_PT_FAMILY_INDICATOR = TPM2_PT_FIXED + 0,
	TPM2_PT_LEVEL = TPM2_PT_FIXED + 1,
	TPM2_PT_REVISION = TPM2_PT_FIXED + 2,
	TPM2_PT_DAY_OF_YEAR = TPM2_PT_FIXED + 3,
	TPM2_PT_YEAR = TPM2_PT_FIXED + 4,
	TPM2_PT_MANUFACTURER = TPM2_PT_FIXED + 5,
	TPM2_PT_VENDOR_STRING_1 = TPM2_PT_FIXED + 6,
	TPM2_PT_VENDOR_STRING_2 = TPM2_PT_FIXED + 7,
	TPM2_PT_VENDOR_STRING_3 = TPM2_PT_FIXED + 8,
	TPM2_PT_VENDOR_STRING_4 = TPM2_PT_FIXED + 9,
	TPM2_PT_VENDOR_TPM_TYPE = TPM2_PT_FIXED + 10,
	TPM2_PT_FIRMWARE_VERSION_1 = TPM2_PT_FIXED + 11,
	TPM2_PT_FIRMWARE_VERSION_2 = TPM2_PT_FIXED + 12,
	TPM2_PT_INPUT_BUFFER = TPM2_PT_FIXED + 13,
	TPM2_PT_HR_TRANSIENT_MIN = TPM2_PT_FIXED + 14,
	TPM2_PT_HR_PERSISTENT_MIN = TPM2_PT_FIXED + 15,
	TPM2_PT_HR_LOADED_MIN = TPM2_PT_FIXED + 16,
	TPM2_PT_ACTIVE_SESSIONS_MAX = TPM2_PT_FIXED + 17,
	TPM2_PT_PCR_COUNT = TPM2_PT_FIXED + 18,
	TPM2_PT_PCR_SELECT_MIN = TPM2_PT_FIXED + 19,
	TPM2_PT_CONTEXT_GAP_MAX = TPM2_PT_FIXED + 20,
	TPM2_PT_NV_COUNTERS_MAX = TPM2_PT_FIXED + 22,
	TPM2_PT_NV_INDEX_MAX = TPM2_PT_FIXED + 23,
	TPM2_PT_MEMORY = TPM2_PT_FIXED + 24,
	TPM2_PT_CLOCK_UPDATE = TPM2_PT_FIXED + 25,
	TPM2_PT_CONTEXT_HASH = TPM2_PT_FIXED + 26,
	TPM2_PT_CONTEXT_SYM = TPM2_PT_FIXED + 27,
	TPM2_PT_CONTEXT_SYM_SIZE = TPM2_PT_FIXED + 28,
	TPM2_PT_ORDERLY_COUNT = TPM2_PT_FIXED + 29,
	TPM2_PT_MAX_COMMAND_SIZE = TPM2_PT_FIXED + 30,
	TPM2_PT_MAX_RESPONSE_SIZE = TPM2_PT_FIXED + 31,
	TPM2_PT_MAX_DIGEST = TPM2_PT_FIXED + 32,
	TPM2_PT_MAX_OBJECT_CONTEXT = TPM2_PT_FIXED + 33,
	TPM2_PT_MAX_SESSION_CONTEXT = TPM2_PT_FIXED + 34,
	TPM2_PT_PS_FAMILY_INDICATOR = TPM2_PT_FIXED + 35,
	TPM2_PT_PS_LEVEL = TPM2_PT_FIXED + 36,
	TPM2_PT_PS_REVISION = TPM2_PT_FIXED + 37,
	TPM2_PT_PS_DAY_OF_YEAR = TPM2_PT_FIXED + 38,
	TPM2_PT_PS_YEAR = TPM2_PT_FIXED + 39,
	TPM2_PT_SPLIT_MAX = TPM2_PT_FIXED + 40,
	TPM2_PT_TOTAL_COMMANDS = TPM2_PT_FIXED + 41,
	TPM2_PT_LIBRARY_COMMANDS = TPM2_PT_FIXED + 42,
	TPM2_PT_VENDOR_COMMANDS = TPM2_PT_FIXED + 43,
	TPM2_PT_NV_BUFFER_MAX = TPM2_PT_FIXED + 44,
	TPM2_PT_MODES = TPM2_PT_FIXED + 45,
	TPM2_PT_MAX_CAP_BUFFER = TPM2_PT_FIXED + 46,
	TPM2_PT_VAR = TPM2_PT_GROUP * 2,
	TPM2_PT_PERMANENT = TPM2_PT_VAR + 0,
	TPM2_PT_STARTUP_CLEAR = TPM2_PT_VAR + 1,
	TPM2_PT_HR_NV_INDEX = TPM2_PT_VAR + 2,
	TPM2_PT_HR_LOADED = TPM2_PT_VAR + 3,
	TPM2_PT_HR_LOADED_AVAIL = TPM2_PT_VAR + 4,
	TPM2_PT_HR_ACTIVE = TPM2_PT_VAR + 5,
	TPM2_PT_HR_ACTIVE_AVAIL = TPM2_PT_VAR + 6,
	TPM2_PT_HR_TRANSIENT_AVAIL = TPM2_PT_VAR + 7,
	TPM2_PT_HR_PERSISTENT = TPM2_PT_VAR + 8,
	TPM2_PT_HR_PERSISTENT_AVAIL = TPM2_PT_VAR + 9,
	TPM2_PT_NV_COUNTERS = TPM2_PT_VAR + 10,
	TPM2_PT_NV_COUNTERS_AVAIL = TPM2_PT_VAR + 11,
	TPM2_PT_ALGORITHM_SET = TPM2_PT_VAR + 12,
	TPM2_PT_LOADED_CURVES = TPM2_PT_VAR + 13,
	TPM2_PT_LOCKOUT_COUNTER = TPM2_PT_VAR + 14,
	TPM2_PT_MAX_AUTH_FAIL = TPM2_PT_VAR + 15,
	TPM2_PT_LOCKOUT_INTERVAL = TPM2_PT_VAR + 16,
	TPM2_PT_LOCKOUT_RECOVERY = TPM2_PT_VAR + 17,
	TPM2_PT_NV_WRITE_RECOVERY = TPM2_PT_VAR + 18,
	TPM2_PT_AUDIT_COUNTER_0 = TPM2_PT_VAR + 19,
	TPM2_PT_AUDIT_COUNTER_1 = TPM2_PT_VAR + 20,
};

/* 128 bytes is an arbitrary cap. This could be as large as TPM_BUFSIZE - 18
 * bytes, but 128 is still a relatively large number of random bytes and
 * anything much bigger causes users of struct tpm_cmd_t to start getting
 * compiler warnings about stack frame size. */
#define TPM_MAX_RNG_DATA	128

extern const struct class tpm_class;
extern const struct class tpmrm_class;
extern dev_t tpm_devt;
extern const struct file_operations tpm_fops;
extern const struct file_operations tpmrm_fops;
extern struct idr dev_nums_idr;

ssize_t tpm_transmit(struct tpm_chip *chip, u8 *buf, size_t bufsiz);
int tpm_get_timeouts(struct tpm_chip *);
int tpm_auto_startup(struct tpm_chip *chip);

int tpm1_pm_suspend(struct tpm_chip *chip, u32 tpm_suspend_pcr);
int tpm1_auto_startup(struct tpm_chip *chip);
int tpm1_do_selftest(struct tpm_chip *chip);
int tpm1_get_timeouts(struct tpm_chip *chip);
unsigned long tpm1_calc_ordinal_duration(struct tpm_chip *chip, u32 ordinal);
int tpm1_pcr_extend(struct tpm_chip *chip, u32 pcr_idx, const u8 *hash,
		    const char *log_msg);
int tpm1_pcr_read(struct tpm_chip *chip, u32 pcr_idx, u8 *res_buf);
ssize_t tpm1_getcap(struct tpm_chip *chip, u32 subcap_id, cap_t *cap,
		    const char *desc, size_t min_cap_length);
int tpm1_get_random(struct tpm_chip *chip, u8 *out, size_t max);
int tpm1_get_pcr_allocation(struct tpm_chip *chip);
unsigned long tpm_calc_ordinal_duration(struct tpm_chip *chip, u32 ordinal);
int tpm_pm_suspend(struct device *dev);
int tpm_pm_resume(struct device *dev);
int tpm_class_shutdown(struct device *dev);

static inline void tpm_msleep(unsigned int delay_msec)
{
	usleep_range((delay_msec * 1000) - TPM_TIMEOUT_RANGE_US,
		     delay_msec * 1000);
};

int tpm_chip_bootstrap(struct tpm_chip *chip);
int tpm_chip_start(struct tpm_chip *chip);
void tpm_chip_stop(struct tpm_chip *chip);
struct tpm_chip *tpm_find_get_ops(struct tpm_chip *chip);

struct tpm_chip *tpm_chip_alloc(struct device *dev,
				const struct tpm_class_ops *ops);
struct tpm_chip *tpmm_chip_alloc(struct device *pdev,
				 const struct tpm_class_ops *ops);
int tpm_chip_register(struct tpm_chip *chip);
void tpm_chip_unregister(struct tpm_chip *chip);

void tpm_sysfs_add_device(struct tpm_chip *chip);


#ifdef CONFIG_ACPI
extern void tpm_add_ppi(struct tpm_chip *chip);
#else
static inline void tpm_add_ppi(struct tpm_chip *chip)
{
}
#endif

int tpm2_get_timeouts(struct tpm_chip *chip);
int tpm2_pcr_read(struct tpm_chip *chip, u32 pcr_idx,
		  struct tpm_digest *digest, u16 *digest_size_ptr);
int tpm2_pcr_extend(struct tpm_chip *chip, u32 pcr_idx,
		    struct tpm_digest *digests);
int tpm2_get_random(struct tpm_chip *chip, u8 *dest, size_t max);
ssize_t tpm2_get_tpm_pt(struct tpm_chip *chip, u32 property_id,
			u32 *value, const char *desc);

ssize_t tpm2_get_pcr_allocation(struct tpm_chip *chip);
int tpm2_auto_startup(struct tpm_chip *chip);
void tpm2_shutdown(struct tpm_chip *chip, u16 shutdown_type);
unsigned long tpm2_calc_ordinal_duration(struct tpm_chip *chip, u32 ordinal);
int tpm2_probe(struct tpm_chip *chip);
int tpm2_get_cc_attrs_tbl(struct tpm_chip *chip);
int tpm2_find_cc(struct tpm_chip *chip, u32 cc);
int tpm2_init_space(struct tpm_space *space, unsigned int buf_size);
void tpm2_del_space(struct tpm_chip *chip, struct tpm_space *space);
void tpm2_flush_space(struct tpm_chip *chip);
int tpm2_prepare_space(struct tpm_chip *chip, struct tpm_space *space, u8 *cmd,
		       size_t cmdsiz);
int tpm2_commit_space(struct tpm_chip *chip, struct tpm_space *space, void *buf,
		      size_t *bufsiz);
int tpm_devs_add(struct tpm_chip *chip);
void tpm_devs_remove(struct tpm_chip *chip);

void tpm_bios_log_setup(struct tpm_chip *chip);
void tpm_bios_log_teardown(struct tpm_chip *chip);
int tpm_dev_common_init(void);
void tpm_dev_common_exit(void);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
