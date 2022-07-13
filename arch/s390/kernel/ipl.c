<<<<<<< HEAD
/*
 *  arch/s390/kernel/ipl.c
 *    ipl/reipl/dump support for Linux on s390.
 *
 *    Copyright IBM Corp. 2005,2012
 *    Author(s): Michael Holzheu <holzheu@de.ibm.com>
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *    ipl/reipl/dump support for Linux on s390.
 *
 *    Copyright IBM Corp. 2005, 2012
 *    Author(s): Michael Holzheu <holzheu@de.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *		 Volker Sameske <sameske@de.ibm.com>
 */

#include <linux/types.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
=======
#include <linux/export.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/kstrtox.h>
#include <linux/panic_notifier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/reboot.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/crash_dump.h>
#include <linux/debug_locks.h>
<<<<<<< HEAD
=======
#include <asm/asm-extable.h>
#include <asm/diag.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/ipl.h>
#include <asm/smp.h>
#include <asm/setup.h>
#include <asm/cpcmd.h>
<<<<<<< HEAD
#include <asm/cio.h>
#include <asm/ebcdic.h>
#include <asm/reset.h>
#include <asm/sclp.h>
#include <asm/checksum.h>
#include <asm/debug.h>
#include <asm/os_info.h>
=======
#include <asm/ebcdic.h>
#include <asm/sclp.h>
#include <asm/checksum.h>
#include <asm/debug.h>
#include <asm/abs_lowcore.h>
#include <asm/os_info.h>
#include <asm/sections.h>
#include <asm/boot_data.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "entry.h"

#define IPL_PARM_BLOCK_VERSION 0

#define IPL_UNKNOWN_STR		"unknown"
#define IPL_CCW_STR		"ccw"
<<<<<<< HEAD
#define IPL_FCP_STR		"fcp"
#define IPL_FCP_DUMP_STR	"fcp_dump"
#define IPL_NSS_STR		"nss"

#define DUMP_CCW_STR		"ccw"
#define DUMP_FCP_STR		"fcp"
=======
#define IPL_ECKD_STR		"eckd"
#define IPL_ECKD_DUMP_STR	"eckd_dump"
#define IPL_FCP_STR		"fcp"
#define IPL_FCP_DUMP_STR	"fcp_dump"
#define IPL_NVME_STR		"nvme"
#define IPL_NVME_DUMP_STR	"nvme_dump"
#define IPL_NSS_STR		"nss"

#define DUMP_CCW_STR		"ccw"
#define DUMP_ECKD_STR		"eckd"
#define DUMP_FCP_STR		"fcp"
#define DUMP_NVME_STR		"nvme"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DUMP_NONE_STR		"none"

/*
 * Four shutdown trigger types are supported:
 * - panic
 * - halt
 * - power off
 * - reipl
 * - restart
 */
#define ON_PANIC_STR		"on_panic"
#define ON_HALT_STR		"on_halt"
#define ON_POFF_STR		"on_poff"
#define ON_REIPL_STR		"on_reboot"
#define ON_RESTART_STR		"on_restart"

struct shutdown_action;
struct shutdown_trigger {
	char *name;
	struct shutdown_action *action;
};

/*
 * The following shutdown action types are supported:
 */
#define SHUTDOWN_ACTION_IPL_STR		"ipl"
#define SHUTDOWN_ACTION_REIPL_STR	"reipl"
#define SHUTDOWN_ACTION_DUMP_STR	"dump"
#define SHUTDOWN_ACTION_VMCMD_STR	"vmcmd"
#define SHUTDOWN_ACTION_STOP_STR	"stop"
#define SHUTDOWN_ACTION_DUMP_REIPL_STR	"dump_reipl"

struct shutdown_action {
	char *name;
	void (*fn) (struct shutdown_trigger *trigger);
	int (*init) (void);
	int init_rc;
};

static char *ipl_type_str(enum ipl_type type)
{
	switch (type) {
	case IPL_TYPE_CCW:
		return IPL_CCW_STR;
<<<<<<< HEAD
=======
	case IPL_TYPE_ECKD:
		return IPL_ECKD_STR;
	case IPL_TYPE_ECKD_DUMP:
		return IPL_ECKD_DUMP_STR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPL_TYPE_FCP:
		return IPL_FCP_STR;
	case IPL_TYPE_FCP_DUMP:
		return IPL_FCP_DUMP_STR;
	case IPL_TYPE_NSS:
		return IPL_NSS_STR;
<<<<<<< HEAD
=======
	case IPL_TYPE_NVME:
		return IPL_NVME_STR;
	case IPL_TYPE_NVME_DUMP:
		return IPL_NVME_DUMP_STR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPL_TYPE_UNKNOWN:
	default:
		return IPL_UNKNOWN_STR;
	}
}

enum dump_type {
	DUMP_TYPE_NONE	= 1,
	DUMP_TYPE_CCW	= 2,
	DUMP_TYPE_FCP	= 4,
<<<<<<< HEAD
=======
	DUMP_TYPE_NVME	= 8,
	DUMP_TYPE_ECKD	= 16,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static char *dump_type_str(enum dump_type type)
{
	switch (type) {
	case DUMP_TYPE_NONE:
		return DUMP_NONE_STR;
	case DUMP_TYPE_CCW:
		return DUMP_CCW_STR;
<<<<<<< HEAD
	case DUMP_TYPE_FCP:
		return DUMP_FCP_STR;
=======
	case DUMP_TYPE_ECKD:
		return DUMP_ECKD_STR;
	case DUMP_TYPE_FCP:
		return DUMP_FCP_STR;
	case DUMP_TYPE_NVME:
		return DUMP_NVME_STR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return NULL;
	}
}

<<<<<<< HEAD
/*
 * Must be in data section since the bss section
 * is not cleared when these are accessed.
 */
static u16 ipl_devno __attribute__((__section__(".data"))) = 0;
u32 ipl_flags __attribute__((__section__(".data"))) = 0;

enum ipl_method {
	REIPL_METHOD_CCW_CIO,
	REIPL_METHOD_CCW_DIAG,
	REIPL_METHOD_CCW_VM,
	REIPL_METHOD_FCP_RO_DIAG,
	REIPL_METHOD_FCP_RW_DIAG,
	REIPL_METHOD_FCP_RO_VM,
	REIPL_METHOD_FCP_DUMP,
	REIPL_METHOD_NSS,
	REIPL_METHOD_NSS_DIAG,
	REIPL_METHOD_DEFAULT,
};

enum dump_method {
	DUMP_METHOD_NONE,
	DUMP_METHOD_CCW_CIO,
	DUMP_METHOD_CCW_DIAG,
	DUMP_METHOD_CCW_VM,
	DUMP_METHOD_FCP_DIAG,
};

static int diag308_set_works = 0;

static struct ipl_parameter_block ipl_block;
=======
int __bootdata_preserved(ipl_block_valid);
struct ipl_parameter_block __bootdata_preserved(ipl_block);
int __bootdata_preserved(ipl_secure_flag);

unsigned long __bootdata_preserved(ipl_cert_list_addr);
unsigned long __bootdata_preserved(ipl_cert_list_size);

unsigned long __bootdata(early_ipl_comp_list_addr);
unsigned long __bootdata(early_ipl_comp_list_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int reipl_capabilities = IPL_TYPE_UNKNOWN;

static enum ipl_type reipl_type = IPL_TYPE_UNKNOWN;
<<<<<<< HEAD
static enum ipl_method reipl_method = REIPL_METHOD_DEFAULT;
static struct ipl_parameter_block *reipl_block_fcp;
static struct ipl_parameter_block *reipl_block_ccw;
=======
static struct ipl_parameter_block *reipl_block_fcp;
static struct ipl_parameter_block *reipl_block_nvme;
static struct ipl_parameter_block *reipl_block_ccw;
static struct ipl_parameter_block *reipl_block_eckd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ipl_parameter_block *reipl_block_nss;
static struct ipl_parameter_block *reipl_block_actual;

static int dump_capabilities = DUMP_TYPE_NONE;
static enum dump_type dump_type = DUMP_TYPE_NONE;
<<<<<<< HEAD
static enum dump_method dump_method = DUMP_METHOD_NONE;
static struct ipl_parameter_block *dump_block_fcp;
static struct ipl_parameter_block *dump_block_ccw;

static struct sclp_ipl_info sclp_ipl_info;

int diag308(unsigned long subcode, void *addr)
{
	register unsigned long _addr asm("0") = (unsigned long) addr;
	register unsigned long _rc asm("1") = 0;

	asm volatile(
		"	diag	%0,%2,0x308\n"
		"0:\n"
		EX_TABLE(0b,0b)
		: "+d" (_addr), "+d" (_rc)
		: "d" (subcode) : "cc", "memory");
	return _rc;
=======
static struct ipl_parameter_block *dump_block_fcp;
static struct ipl_parameter_block *dump_block_nvme;
static struct ipl_parameter_block *dump_block_ccw;
static struct ipl_parameter_block *dump_block_eckd;

static struct sclp_ipl_info sclp_ipl_info;

static bool reipl_nvme_clear;
static bool reipl_fcp_clear;
static bool reipl_ccw_clear;
static bool reipl_eckd_clear;

static unsigned long os_info_flags;

static inline int __diag308(unsigned long subcode, unsigned long addr)
{
	union register_pair r1;

	r1.even = addr;
	r1.odd	= 0;
	asm volatile(
		"	diag	%[r1],%[subcode],0x308\n"
		"0:	nopr	%%r7\n"
		EX_TABLE(0b,0b)
		: [r1] "+&d" (r1.pair)
		: [subcode] "d" (subcode)
		: "cc", "memory");
	return r1.odd;
}

int diag308(unsigned long subcode, void *addr)
{
	diag_stat_inc(DIAG_STAT_X308);
	return __diag308(subcode, addr ? virt_to_phys(addr) : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(diag308);

/* SYSFS */

<<<<<<< HEAD
#define DEFINE_IPL_ATTR_RO(_prefix, _name, _format, _value)		\
=======
#define IPL_ATTR_SHOW_FN(_prefix, _name, _format, args...)		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sys_##_prefix##_##_name##_show(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		char *page)						\
{									\
<<<<<<< HEAD
	return sprintf(page, _format, _value);				\
}									\
static struct kobj_attribute sys_##_prefix##_##_name##_attr =		\
	__ATTR(_name, S_IRUGO, sys_##_prefix##_##_name##_show, NULL);

#define DEFINE_IPL_ATTR_RW(_prefix, _name, _fmt_out, _fmt_in, _value)	\
static ssize_t sys_##_prefix##_##_name##_show(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		char *page)						\
{									\
	return sprintf(page, _fmt_out,					\
			(unsigned long long) _value);			\
}									\
=======
	return scnprintf(page, PAGE_SIZE, _format, ##args);		\
}

#define IPL_ATTR_CCW_STORE_FN(_prefix, _name, _ipl_blk)			\
static ssize_t sys_##_prefix##_##_name##_store(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		const char *buf, size_t len)				\
{									\
	unsigned long long ssid, devno;					\
									\
	if (sscanf(buf, "0.%llx.%llx\n", &ssid, &devno) != 2)		\
		return -EINVAL;						\
									\
	if (ssid > __MAX_SSID || devno > __MAX_SUBCHANNEL)		\
		return -EINVAL;						\
									\
	_ipl_blk.ssid = ssid;						\
	_ipl_blk.devno = devno;						\
	return len;							\
}

#define DEFINE_IPL_CCW_ATTR_RW(_prefix, _name, _ipl_blk)		\
IPL_ATTR_SHOW_FN(_prefix, _name, "0.%x.%04x\n",				\
		 _ipl_blk.ssid, _ipl_blk.devno);			\
IPL_ATTR_CCW_STORE_FN(_prefix, _name, _ipl_blk);			\
static struct kobj_attribute sys_##_prefix##_##_name##_attr =		\
	__ATTR(_name, 0644,						\
	       sys_##_prefix##_##_name##_show,				\
	       sys_##_prefix##_##_name##_store)				\

#define DEFINE_IPL_ATTR_RO(_prefix, _name, _format, _value)		\
IPL_ATTR_SHOW_FN(_prefix, _name, _format, _value)			\
static struct kobj_attribute sys_##_prefix##_##_name##_attr =		\
	__ATTR(_name, 0444, sys_##_prefix##_##_name##_show, NULL)

#define DEFINE_IPL_ATTR_RW(_prefix, _name, _fmt_out, _fmt_in, _value)	\
IPL_ATTR_SHOW_FN(_prefix, _name, _fmt_out, (unsigned long long) _value)	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sys_##_prefix##_##_name##_store(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		const char *buf, size_t len)				\
{									\
	unsigned long long value;					\
	if (sscanf(buf, _fmt_in, &value) != 1)				\
		return -EINVAL;						\
	_value = value;							\
	return len;							\
}									\
static struct kobj_attribute sys_##_prefix##_##_name##_attr =		\
<<<<<<< HEAD
	__ATTR(_name,(S_IRUGO | S_IWUSR),				\
			sys_##_prefix##_##_name##_show,			\
			sys_##_prefix##_##_name##_store);

#define DEFINE_IPL_ATTR_STR_RW(_prefix, _name, _fmt_out, _fmt_in, _value)\
static ssize_t sys_##_prefix##_##_name##_show(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		char *page)						\
{									\
	return sprintf(page, _fmt_out, _value);				\
}									\
=======
	__ATTR(_name, 0644,						\
			sys_##_prefix##_##_name##_show,			\
			sys_##_prefix##_##_name##_store)

#define DEFINE_IPL_ATTR_STR_RW(_prefix, _name, _fmt_out, _fmt_in, _value)\
IPL_ATTR_SHOW_FN(_prefix, _name, _fmt_out, _value)			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sys_##_prefix##_##_name##_store(struct kobject *kobj,	\
		struct kobj_attribute *attr,				\
		const char *buf, size_t len)				\
{									\
<<<<<<< HEAD
	strncpy(_value, buf, sizeof(_value) - 1);			\
=======
	strscpy(_value, buf, sizeof(_value));				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	strim(_value);							\
	return len;							\
}									\
static struct kobj_attribute sys_##_prefix##_##_name##_attr =		\
<<<<<<< HEAD
	__ATTR(_name,(S_IRUGO | S_IWUSR),				\
			sys_##_prefix##_##_name##_show,			\
			sys_##_prefix##_##_name##_store);

static void make_attrs_ro(struct attribute **attrs)
{
	while (*attrs) {
		(*attrs)->mode = S_IRUGO;
		attrs++;
	}
}
=======
	__ATTR(_name, 0644,						\
			sys_##_prefix##_##_name##_show,			\
			sys_##_prefix##_##_name##_store)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * ipl section
 */

static __init enum ipl_type get_ipl_type(void)
{
<<<<<<< HEAD
	struct ipl_parameter_block *ipl = IPL_PARMBLOCK_START;

	if (ipl_flags & IPL_NSS_VALID)
		return IPL_TYPE_NSS;
	if (!(ipl_flags & IPL_DEVNO_VALID))
		return IPL_TYPE_UNKNOWN;
	if (!(ipl_flags & IPL_PARMBLOCK_VALID))
		return IPL_TYPE_CCW;
	if (ipl->hdr.version > IPL_MAX_SUPPORTED_VERSION)
		return IPL_TYPE_UNKNOWN;
	if (ipl->hdr.pbt != DIAG308_IPL_TYPE_FCP)
		return IPL_TYPE_UNKNOWN;
	if (ipl->ipl_info.fcp.opt == DIAG308_IPL_OPT_DUMP)
		return IPL_TYPE_FCP_DUMP;
	return IPL_TYPE_FCP;
=======
	if (!ipl_block_valid)
		return IPL_TYPE_UNKNOWN;

	switch (ipl_block.pb0_hdr.pbt) {
	case IPL_PBT_CCW:
		return IPL_TYPE_CCW;
	case IPL_PBT_FCP:
		if (ipl_block.fcp.opt == IPL_PB0_FCP_OPT_DUMP)
			return IPL_TYPE_FCP_DUMP;
		else
			return IPL_TYPE_FCP;
	case IPL_PBT_NVME:
		if (ipl_block.nvme.opt == IPL_PB0_NVME_OPT_DUMP)
			return IPL_TYPE_NVME_DUMP;
		else
			return IPL_TYPE_NVME;
	case IPL_PBT_ECKD:
		if (ipl_block.eckd.opt == IPL_PB0_ECKD_OPT_DUMP)
			return IPL_TYPE_ECKD_DUMP;
		else
			return IPL_TYPE_ECKD;
	}
	return IPL_TYPE_UNKNOWN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct ipl_info ipl_info;
EXPORT_SYMBOL_GPL(ipl_info);

static ssize_t ipl_type_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *page)
{
	return sprintf(page, "%s\n", ipl_type_str(ipl_info.type));
}

static struct kobj_attribute sys_ipl_type_attr = __ATTR_RO(ipl_type);

<<<<<<< HEAD
/* VM IPL PARM routines */
static size_t reipl_get_ascii_vmparm(char *dest, size_t size,
				     const struct ipl_parameter_block *ipb)
{
	int i;
	size_t len;
	char has_lowercase = 0;

	len = 0;
	if ((ipb->ipl_info.ccw.vm_flags & DIAG308_VM_FLAGS_VP_VALID) &&
	    (ipb->ipl_info.ccw.vm_parm_len > 0)) {

		len = min_t(size_t, size - 1, ipb->ipl_info.ccw.vm_parm_len);
		memcpy(dest, ipb->ipl_info.ccw.vm_parm, len);
		/* If at least one character is lowercase, we assume mixed
		 * case; otherwise we convert everything to lowercase.
		 */
		for (i = 0; i < len; i++)
			if ((dest[i] > 0x80 && dest[i] < 0x8a) || /* a-i */
			    (dest[i] > 0x90 && dest[i] < 0x9a) || /* j-r */
			    (dest[i] > 0xa1 && dest[i] < 0xaa)) { /* s-z */
				has_lowercase = 1;
				break;
			}
		if (!has_lowercase)
			EBC_TOLOWER(dest, len);
		EBCASC(dest, len);
	}
	dest[len] = 0;

	return len;
}

size_t append_ipl_vmparm(char *dest, size_t size)
{
	size_t rc;

	rc = 0;
	if (diag308_set_works && (ipl_block.hdr.pbt == DIAG308_IPL_TYPE_CCW))
		rc = reipl_get_ascii_vmparm(dest, size, &ipl_block);
	else
		dest[0] = 0;
	return rc;
}

=======
static ssize_t ipl_secure_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%i\n", !!ipl_secure_flag);
}

static struct kobj_attribute sys_ipl_secure_attr =
	__ATTR(secure, 0444, ipl_secure_show, NULL);

static ssize_t ipl_has_secure_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%i\n", !!sclp.has_sipl);
}

static struct kobj_attribute sys_ipl_has_secure_attr =
	__ATTR(has_secure, 0444, ipl_has_secure_show, NULL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t ipl_vm_parm_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *page)
{
	char parm[DIAG308_VMPARM_SIZE + 1] = {};

<<<<<<< HEAD
	append_ipl_vmparm(parm, sizeof(parm));
	return sprintf(page, "%s\n", parm);
}

static size_t scpdata_length(const char* buf, size_t count)
{
	while (count) {
		if (buf[count - 1] != '\0' && buf[count - 1] != ' ')
			break;
		count--;
	}
	return count;
}

static size_t reipl_append_ascii_scpdata(char *dest, size_t size,
					 const struct ipl_parameter_block *ipb)
{
	size_t count;
	size_t i;
	int has_lowercase;

	count = min(size - 1, scpdata_length(ipb->ipl_info.fcp.scp_data,
					     ipb->ipl_info.fcp.scp_data_len));
	if (!count)
		goto out;

	has_lowercase = 0;
	for (i = 0; i < count; i++) {
		if (!isascii(ipb->ipl_info.fcp.scp_data[i])) {
			count = 0;
			goto out;
		}
		if (!has_lowercase && islower(ipb->ipl_info.fcp.scp_data[i]))
			has_lowercase = 1;
	}

	if (has_lowercase)
		memcpy(dest, ipb->ipl_info.fcp.scp_data, count);
	else
		for (i = 0; i < count; i++)
			dest[i] = tolower(ipb->ipl_info.fcp.scp_data[i]);
out:
	dest[count] = '\0';
	return count;
}

size_t append_ipl_scpdata(char *dest, size_t len)
{
	size_t rc;

	rc = 0;
	if (ipl_block.hdr.pbt == DIAG308_IPL_TYPE_FCP)
		rc = reipl_append_ascii_scpdata(dest, len, &ipl_block);
	else
		dest[0] = 0;
	return rc;
}


static struct kobj_attribute sys_ipl_vm_parm_attr =
	__ATTR(parm, S_IRUGO, ipl_vm_parm_show, NULL);
=======
	if (ipl_block_valid && (ipl_block.pb0_hdr.pbt == IPL_PBT_CCW))
		ipl_block_get_ascii_vmparm(parm, sizeof(parm), &ipl_block);
	return sprintf(page, "%s\n", parm);
}

static struct kobj_attribute sys_ipl_vm_parm_attr =
	__ATTR(parm, 0444, ipl_vm_parm_show, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t sys_ipl_device_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *page)
{
<<<<<<< HEAD
	struct ipl_parameter_block *ipl = IPL_PARMBLOCK_START;

	switch (ipl_info.type) {
	case IPL_TYPE_CCW:
		return sprintf(page, "0.0.%04x\n", ipl_devno);
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		return sprintf(page, "0.0.%04x\n", ipl->ipl_info.fcp.devno);
=======
	switch (ipl_info.type) {
	case IPL_TYPE_CCW:
		return sprintf(page, "0.%x.%04x\n", ipl_block.ccw.ssid,
			       ipl_block.ccw.devno);
	case IPL_TYPE_ECKD:
	case IPL_TYPE_ECKD_DUMP:
		return sprintf(page, "0.%x.%04x\n", ipl_block.eckd.ssid,
			       ipl_block.eckd.devno);
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		return sprintf(page, "0.0.%04x\n", ipl_block.fcp.devno);
	case IPL_TYPE_NVME:
	case IPL_TYPE_NVME_DUMP:
		return sprintf(page, "%08ux\n", ipl_block.nvme.fid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return 0;
	}
}

static struct kobj_attribute sys_ipl_device_attr =
<<<<<<< HEAD
	__ATTR(device, S_IRUGO, sys_ipl_device_show, NULL);
=======
	__ATTR(device, 0444, sys_ipl_device_show, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t ipl_parameter_read(struct file *filp, struct kobject *kobj,
				  struct bin_attribute *attr, char *buf,
				  loff_t off, size_t count)
{
<<<<<<< HEAD
	return memory_read_from_buffer(buf, count, &off, IPL_PARMBLOCK_START,
					IPL_PARMBLOCK_SIZE);
}

static struct bin_attribute ipl_parameter_attr = {
	.attr = {
		.name = "binary_parameter",
		.mode = S_IRUGO,
	},
	.size = PAGE_SIZE,
	.read = &ipl_parameter_read,
};
=======
	return memory_read_from_buffer(buf, count, &off, &ipl_block,
				       ipl_block.hdr.len);
}
static struct bin_attribute ipl_parameter_attr =
	__BIN_ATTR(binary_parameter, 0444, ipl_parameter_read, NULL,
		   PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t ipl_scp_data_read(struct file *filp, struct kobject *kobj,
				 struct bin_attribute *attr, char *buf,
				 loff_t off, size_t count)
{
<<<<<<< HEAD
	unsigned int size = IPL_PARMBLOCK_START->ipl_info.fcp.scp_data_len;
	void *scp_data = &IPL_PARMBLOCK_START->ipl_info.fcp.scp_data;
=======
	unsigned int size = ipl_block.fcp.scp_data_len;
	void *scp_data = &ipl_block.fcp.scp_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

<<<<<<< HEAD
static struct bin_attribute ipl_scp_data_attr = {
	.attr = {
		.name = "scp_data",
		.mode = S_IRUGO,
	},
	.size = PAGE_SIZE,
	.read = ipl_scp_data_read,
=======
static ssize_t ipl_nvme_scp_data_read(struct file *filp, struct kobject *kobj,
				 struct bin_attribute *attr, char *buf,
				 loff_t off, size_t count)
{
	unsigned int size = ipl_block.nvme.scp_data_len;
	void *scp_data = &ipl_block.nvme.scp_data;

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

static ssize_t ipl_eckd_scp_data_read(struct file *filp, struct kobject *kobj,
				      struct bin_attribute *attr, char *buf,
				      loff_t off, size_t count)
{
	unsigned int size = ipl_block.eckd.scp_data_len;
	void *scp_data = &ipl_block.eckd.scp_data;

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

static struct bin_attribute ipl_scp_data_attr =
	__BIN_ATTR(scp_data, 0444, ipl_scp_data_read, NULL, PAGE_SIZE);

static struct bin_attribute ipl_nvme_scp_data_attr =
	__BIN_ATTR(scp_data, 0444, ipl_nvme_scp_data_read, NULL, PAGE_SIZE);

static struct bin_attribute ipl_eckd_scp_data_attr =
	__BIN_ATTR(scp_data, 0444, ipl_eckd_scp_data_read, NULL, PAGE_SIZE);

static struct bin_attribute *ipl_fcp_bin_attrs[] = {
	&ipl_parameter_attr,
	&ipl_scp_data_attr,
	NULL,
};

static struct bin_attribute *ipl_nvme_bin_attrs[] = {
	&ipl_parameter_attr,
	&ipl_nvme_scp_data_attr,
	NULL,
};

static struct bin_attribute *ipl_eckd_bin_attrs[] = {
	&ipl_parameter_attr,
	&ipl_eckd_scp_data_attr,
	NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* FCP ipl device attributes */

<<<<<<< HEAD
DEFINE_IPL_ATTR_RO(ipl_fcp, wwpn, "0x%016llx\n", (unsigned long long)
		   IPL_PARMBLOCK_START->ipl_info.fcp.wwpn);
DEFINE_IPL_ATTR_RO(ipl_fcp, lun, "0x%016llx\n", (unsigned long long)
		   IPL_PARMBLOCK_START->ipl_info.fcp.lun);
DEFINE_IPL_ATTR_RO(ipl_fcp, bootprog, "%lld\n", (unsigned long long)
		   IPL_PARMBLOCK_START->ipl_info.fcp.bootprog);
DEFINE_IPL_ATTR_RO(ipl_fcp, br_lba, "%lld\n", (unsigned long long)
		   IPL_PARMBLOCK_START->ipl_info.fcp.br_lba);

static struct attribute *ipl_fcp_attrs[] = {
	&sys_ipl_type_attr.attr,
	&sys_ipl_device_attr.attr,
	&sys_ipl_fcp_wwpn_attr.attr,
	&sys_ipl_fcp_lun_attr.attr,
	&sys_ipl_fcp_bootprog_attr.attr,
	&sys_ipl_fcp_br_lba_attr.attr,
	NULL,
};

static struct attribute_group ipl_fcp_attr_group = {
	.attrs = ipl_fcp_attrs,
};

/* CCW ipl device attributes */
=======
DEFINE_IPL_ATTR_RO(ipl_fcp, wwpn, "0x%016llx\n",
		   (unsigned long long)ipl_block.fcp.wwpn);
DEFINE_IPL_ATTR_RO(ipl_fcp, lun, "0x%016llx\n",
		   (unsigned long long)ipl_block.fcp.lun);
DEFINE_IPL_ATTR_RO(ipl_fcp, bootprog, "%lld\n",
		   (unsigned long long)ipl_block.fcp.bootprog);
DEFINE_IPL_ATTR_RO(ipl_fcp, br_lba, "%lld\n",
		   (unsigned long long)ipl_block.fcp.br_lba);

/* NVMe ipl device attributes */
DEFINE_IPL_ATTR_RO(ipl_nvme, fid, "0x%08llx\n",
		   (unsigned long long)ipl_block.nvme.fid);
DEFINE_IPL_ATTR_RO(ipl_nvme, nsid, "0x%08llx\n",
		   (unsigned long long)ipl_block.nvme.nsid);
DEFINE_IPL_ATTR_RO(ipl_nvme, bootprog, "%lld\n",
		   (unsigned long long)ipl_block.nvme.bootprog);
DEFINE_IPL_ATTR_RO(ipl_nvme, br_lba, "%lld\n",
		   (unsigned long long)ipl_block.nvme.br_lba);

/* ECKD ipl device attributes */
DEFINE_IPL_ATTR_RO(ipl_eckd, bootprog, "%lld\n",
		   (unsigned long long)ipl_block.eckd.bootprog);

#define IPL_ATTR_BR_CHR_SHOW_FN(_name, _ipb)				\
static ssize_t eckd_##_name##_br_chr_show(struct kobject *kobj,		\
					  struct kobj_attribute *attr,	\
					  char *buf)			\
{									\
	struct ipl_pb0_eckd *ipb = &(_ipb);				\
									\
	if (!ipb->br_chr.cyl &&						\
	    !ipb->br_chr.head &&					\
	    !ipb->br_chr.record)					\
		return sprintf(buf, "auto\n");				\
									\
	return sprintf(buf, "0x%x,0x%x,0x%x\n",				\
			ipb->br_chr.cyl,				\
			ipb->br_chr.head,				\
			ipb->br_chr.record);				\
}

#define IPL_ATTR_BR_CHR_STORE_FN(_name, _ipb)				\
static ssize_t eckd_##_name##_br_chr_store(struct kobject *kobj,	\
					   struct kobj_attribute *attr,	\
					   const char *buf, size_t len)	\
{									\
	struct ipl_pb0_eckd *ipb = &(_ipb);				\
	unsigned long args[3] = { 0 };					\
	char *p, *p1, *tmp = NULL;					\
	int i, rc;							\
									\
	if (!strncmp(buf, "auto", 4))					\
		goto out;						\
									\
	tmp = kstrdup(buf, GFP_KERNEL);					\
	p = tmp;							\
	for (i = 0; i < 3; i++) {					\
		p1 = strsep(&p, ", ");					\
		if (!p1) {						\
			rc = -EINVAL;					\
			goto err;					\
		}							\
		rc = kstrtoul(p1, 0, args + i);				\
		if (rc)							\
			goto err;					\
	}								\
									\
	rc = -EINVAL;							\
	if (i != 3)							\
		goto err;						\
									\
	if ((args[0] || args[1]) && !args[2])				\
		goto err;						\
									\
	if (args[0] > UINT_MAX || args[1] > 255 || args[2] > 255)	\
		goto err;						\
									\
out:									\
	ipb->br_chr.cyl = args[0];					\
	ipb->br_chr.head = args[1];					\
	ipb->br_chr.record = args[2];					\
	rc = len;							\
err:									\
	kfree(tmp);							\
	return rc;							\
}

IPL_ATTR_BR_CHR_SHOW_FN(ipl, ipl_block.eckd);
static struct kobj_attribute sys_ipl_eckd_br_chr_attr =
	__ATTR(br_chr, 0644, eckd_ipl_br_chr_show, NULL);

IPL_ATTR_BR_CHR_SHOW_FN(reipl, reipl_block_eckd->eckd);
IPL_ATTR_BR_CHR_STORE_FN(reipl, reipl_block_eckd->eckd);

static struct kobj_attribute sys_reipl_eckd_br_chr_attr =
	__ATTR(br_chr, 0644, eckd_reipl_br_chr_show, eckd_reipl_br_chr_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t ipl_ccw_loadparm_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *page)
{
	char loadparm[LOADPARM_LEN + 1] = {};

	if (!sclp_ipl_info.is_valid)
		return sprintf(page, "#unknown#\n");
	memcpy(loadparm, &sclp_ipl_info.loadparm, LOADPARM_LEN);
	EBCASC(loadparm, LOADPARM_LEN);
	strim(loadparm);
	return sprintf(page, "%s\n", loadparm);
}

static struct kobj_attribute sys_ipl_ccw_loadparm_attr =
	__ATTR(loadparm, 0444, ipl_ccw_loadparm_show, NULL);

<<<<<<< HEAD
static struct attribute *ipl_ccw_attrs_vm[] = {
	&sys_ipl_type_attr.attr,
=======
static struct attribute *ipl_fcp_attrs[] = {
	&sys_ipl_device_attr.attr,
	&sys_ipl_fcp_wwpn_attr.attr,
	&sys_ipl_fcp_lun_attr.attr,
	&sys_ipl_fcp_bootprog_attr.attr,
	&sys_ipl_fcp_br_lba_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	NULL,
};

static struct attribute_group ipl_fcp_attr_group = {
	.attrs = ipl_fcp_attrs,
	.bin_attrs = ipl_fcp_bin_attrs,
};

static struct attribute *ipl_nvme_attrs[] = {
	&sys_ipl_nvme_fid_attr.attr,
	&sys_ipl_nvme_nsid_attr.attr,
	&sys_ipl_nvme_bootprog_attr.attr,
	&sys_ipl_nvme_br_lba_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	NULL,
};

static struct attribute_group ipl_nvme_attr_group = {
	.attrs = ipl_nvme_attrs,
	.bin_attrs = ipl_nvme_bin_attrs,
};

static struct attribute *ipl_eckd_attrs[] = {
	&sys_ipl_eckd_bootprog_attr.attr,
	&sys_ipl_eckd_br_chr_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	&sys_ipl_device_attr.attr,
	NULL,
};

static struct attribute_group ipl_eckd_attr_group = {
	.attrs = ipl_eckd_attrs,
	.bin_attrs = ipl_eckd_bin_attrs,
};

/* CCW ipl device attributes */

static struct attribute *ipl_ccw_attrs_vm[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&sys_ipl_device_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	&sys_ipl_vm_parm_attr.attr,
	NULL,
};

static struct attribute *ipl_ccw_attrs_lpar[] = {
<<<<<<< HEAD
	&sys_ipl_type_attr.attr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&sys_ipl_device_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	NULL,
};

static struct attribute_group ipl_ccw_attr_group_vm = {
	.attrs = ipl_ccw_attrs_vm,
};

static struct attribute_group ipl_ccw_attr_group_lpar = {
	.attrs = ipl_ccw_attrs_lpar
};

<<<<<<< HEAD
/* NSS ipl device attributes */

DEFINE_IPL_ATTR_RO(ipl_nss, name, "%s\n", kernel_nss_name);

static struct attribute *ipl_nss_attrs[] = {
	&sys_ipl_type_attr.attr,
	&sys_ipl_nss_name_attr.attr,
	&sys_ipl_ccw_loadparm_attr.attr,
	&sys_ipl_vm_parm_attr.attr,
	NULL,
};

static struct attribute_group ipl_nss_attr_group = {
	.attrs = ipl_nss_attrs,
};

/* UNKNOWN ipl device attributes */

static struct attribute *ipl_unknown_attrs[] = {
	&sys_ipl_type_attr.attr,
	NULL,
};

static struct attribute_group ipl_unknown_attr_group = {
	.attrs = ipl_unknown_attrs,
=======
static struct attribute *ipl_common_attrs[] = {
	&sys_ipl_type_attr.attr,
	&sys_ipl_secure_attr.attr,
	&sys_ipl_has_secure_attr.attr,
	NULL,
};

static struct attribute_group ipl_common_attr_group = {
	.attrs = ipl_common_attrs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct kset *ipl_kset;

<<<<<<< HEAD
static int __init ipl_register_fcp_files(void)
{
	int rc;

	rc = sysfs_create_group(&ipl_kset->kobj, &ipl_fcp_attr_group);
	if (rc)
		goto out;
	rc = sysfs_create_bin_file(&ipl_kset->kobj, &ipl_parameter_attr);
	if (rc)
		goto out_ipl_parm;
	rc = sysfs_create_bin_file(&ipl_kset->kobj, &ipl_scp_data_attr);
	if (!rc)
		goto out;

	sysfs_remove_bin_file(&ipl_kset->kobj, &ipl_parameter_attr);

out_ipl_parm:
	sysfs_remove_group(&ipl_kset->kobj, &ipl_fcp_attr_group);
out:
	return rc;
}

static void __ipl_run(void *unused)
{
	diag308(DIAG308_IPL, NULL);
	if (MACHINE_IS_VM)
		__cpcmd("IPL", NULL, 0, NULL);
	else if (ipl_info.type == IPL_TYPE_CCW)
		reipl_ccw_dev(&ipl_info.data.ccw.dev_id);
=======
static void __ipl_run(void *unused)
{
	diag308(DIAG308_LOAD_CLEAR, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ipl_run(struct shutdown_trigger *trigger)
{
	smp_call_ipl_cpu(__ipl_run, NULL);
}

static int __init ipl_init(void)
{
	int rc;

	ipl_kset = kset_create_and_add("ipl", NULL, firmware_kobj);
	if (!ipl_kset) {
		rc = -ENOMEM;
		goto out;
	}
<<<<<<< HEAD
=======
	rc = sysfs_create_group(&ipl_kset->kobj, &ipl_common_attr_group);
	if (rc)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (ipl_info.type) {
	case IPL_TYPE_CCW:
		if (MACHINE_IS_VM)
			rc = sysfs_create_group(&ipl_kset->kobj,
						&ipl_ccw_attr_group_vm);
		else
			rc = sysfs_create_group(&ipl_kset->kobj,
						&ipl_ccw_attr_group_lpar);
		break;
<<<<<<< HEAD
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		rc = ipl_register_fcp_files();
		break;
	case IPL_TYPE_NSS:
		rc = sysfs_create_group(&ipl_kset->kobj, &ipl_nss_attr_group);
		break;
	default:
		rc = sysfs_create_group(&ipl_kset->kobj,
					&ipl_unknown_attr_group);
=======
	case IPL_TYPE_ECKD:
	case IPL_TYPE_ECKD_DUMP:
		rc = sysfs_create_group(&ipl_kset->kobj, &ipl_eckd_attr_group);
		break;
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		rc = sysfs_create_group(&ipl_kset->kobj, &ipl_fcp_attr_group);
		break;
	case IPL_TYPE_NVME:
	case IPL_TYPE_NVME_DUMP:
		rc = sysfs_create_group(&ipl_kset->kobj, &ipl_nvme_attr_group);
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
out:
	if (rc)
		panic("ipl_init failed: rc = %i\n", rc);

	return 0;
}

static struct shutdown_action __refdata ipl_action = {
	.name	= SHUTDOWN_ACTION_IPL_STR,
	.fn	= ipl_run,
	.init	= ipl_init,
};

/*
 * reipl shutdown action: Reboot Linux on shutdown.
 */

/* VM IPL PARM attributes */
static ssize_t reipl_generic_vmparm_show(struct ipl_parameter_block *ipb,
					  char *page)
{
	char vmparm[DIAG308_VMPARM_SIZE + 1] = {};

<<<<<<< HEAD
	reipl_get_ascii_vmparm(vmparm, sizeof(vmparm), ipb);
=======
	ipl_block_get_ascii_vmparm(vmparm, sizeof(vmparm), ipb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return sprintf(page, "%s\n", vmparm);
}

static ssize_t reipl_generic_vmparm_store(struct ipl_parameter_block *ipb,
					  size_t vmparm_max,
					  const char *buf, size_t len)
{
	int i, ip_len;

	/* ignore trailing newline */
	ip_len = len;
	if ((len > 0) && (buf[len - 1] == '\n'))
		ip_len--;

	if (ip_len > vmparm_max)
		return -EINVAL;

	/* parm is used to store kernel options, check for common chars */
	for (i = 0; i < ip_len; i++)
		if (!(isalnum(buf[i]) || isascii(buf[i]) || isprint(buf[i])))
			return -EINVAL;

<<<<<<< HEAD
	memset(ipb->ipl_info.ccw.vm_parm, 0, DIAG308_VMPARM_SIZE);
	ipb->ipl_info.ccw.vm_parm_len = ip_len;
	if (ip_len > 0) {
		ipb->ipl_info.ccw.vm_flags |= DIAG308_VM_FLAGS_VP_VALID;
		memcpy(ipb->ipl_info.ccw.vm_parm, buf, ip_len);
		ASCEBC(ipb->ipl_info.ccw.vm_parm, ip_len);
	} else {
		ipb->ipl_info.ccw.vm_flags &= ~DIAG308_VM_FLAGS_VP_VALID;
=======
	memset(ipb->ccw.vm_parm, 0, DIAG308_VMPARM_SIZE);
	ipb->ccw.vm_parm_len = ip_len;
	if (ip_len > 0) {
		ipb->ccw.vm_flags |= IPL_PB0_CCW_VM_FLAG_VP;
		memcpy(ipb->ccw.vm_parm, buf, ip_len);
		ASCEBC(ipb->ccw.vm_parm, ip_len);
	} else {
		ipb->ccw.vm_flags &= ~IPL_PB0_CCW_VM_FLAG_VP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return len;
}

/* NSS wrapper */
static ssize_t reipl_nss_vmparm_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *page)
{
	return reipl_generic_vmparm_show(reipl_block_nss, page);
}

static ssize_t reipl_nss_vmparm_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t len)
{
	return reipl_generic_vmparm_store(reipl_block_nss, 56, buf, len);
}

/* CCW wrapper */
static ssize_t reipl_ccw_vmparm_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *page)
{
	return reipl_generic_vmparm_show(reipl_block_ccw, page);
}

static ssize_t reipl_ccw_vmparm_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t len)
{
	return reipl_generic_vmparm_store(reipl_block_ccw, 64, buf, len);
}

static struct kobj_attribute sys_reipl_nss_vmparm_attr =
<<<<<<< HEAD
	__ATTR(parm, S_IRUGO | S_IWUSR, reipl_nss_vmparm_show,
					reipl_nss_vmparm_store);
static struct kobj_attribute sys_reipl_ccw_vmparm_attr =
	__ATTR(parm, S_IRUGO | S_IWUSR, reipl_ccw_vmparm_show,
					reipl_ccw_vmparm_store);
=======
	__ATTR(parm, 0644, reipl_nss_vmparm_show,
	       reipl_nss_vmparm_store);
static struct kobj_attribute sys_reipl_ccw_vmparm_attr =
	__ATTR(parm, 0644, reipl_ccw_vmparm_show,
	       reipl_ccw_vmparm_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* FCP reipl device attributes */

static ssize_t reipl_fcp_scpdata_read(struct file *filp, struct kobject *kobj,
				      struct bin_attribute *attr,
				      char *buf, loff_t off, size_t count)
{
<<<<<<< HEAD
	size_t size = reipl_block_fcp->ipl_info.fcp.scp_data_len;
	void *scp_data = reipl_block_fcp->ipl_info.fcp.scp_data;
=======
	size_t size = reipl_block_fcp->fcp.scp_data_len;
	void *scp_data = reipl_block_fcp->fcp.scp_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

static ssize_t reipl_fcp_scpdata_write(struct file *filp, struct kobject *kobj,
				       struct bin_attribute *attr,
				       char *buf, loff_t off, size_t count)
{
<<<<<<< HEAD
	size_t padding;
	size_t scpdata_len;

	if (off < 0)
		return -EINVAL;

	if (off >= DIAG308_SCPDATA_SIZE)
		return -ENOSPC;

	if (count > DIAG308_SCPDATA_SIZE - off)
		count = DIAG308_SCPDATA_SIZE - off;

	memcpy(reipl_block_fcp->ipl_info.fcp.scp_data, buf + off, count);
	scpdata_len = off + count;

	if (scpdata_len % 8) {
		padding = 8 - (scpdata_len % 8);
		memset(reipl_block_fcp->ipl_info.fcp.scp_data + scpdata_len,
=======
	size_t scpdata_len = count;
	size_t padding;


	if (off)
		return -EINVAL;

	memcpy(reipl_block_fcp->fcp.scp_data, buf, count);
	if (scpdata_len % 8) {
		padding = 8 - (scpdata_len % 8);
		memset(reipl_block_fcp->fcp.scp_data + scpdata_len,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       0, padding);
		scpdata_len += padding;
	}

<<<<<<< HEAD
	reipl_block_fcp->ipl_info.fcp.scp_data_len = scpdata_len;
	reipl_block_fcp->hdr.len = IPL_PARM_BLK_FCP_LEN + scpdata_len;
	reipl_block_fcp->hdr.blk0_len = IPL_PARM_BLK0_FCP_LEN + scpdata_len;

	return count;
}

static struct bin_attribute sys_reipl_fcp_scp_data_attr = {
	.attr = {
		.name = "scp_data",
		.mode = S_IRUGO | S_IWUSR,
	},
	.size = PAGE_SIZE,
	.read = reipl_fcp_scpdata_read,
	.write = reipl_fcp_scpdata_write,
};

DEFINE_IPL_ATTR_RW(reipl_fcp, wwpn, "0x%016llx\n", "%016llx\n",
		   reipl_block_fcp->ipl_info.fcp.wwpn);
DEFINE_IPL_ATTR_RW(reipl_fcp, lun, "0x%016llx\n", "%016llx\n",
		   reipl_block_fcp->ipl_info.fcp.lun);
DEFINE_IPL_ATTR_RW(reipl_fcp, bootprog, "%lld\n", "%lld\n",
		   reipl_block_fcp->ipl_info.fcp.bootprog);
DEFINE_IPL_ATTR_RW(reipl_fcp, br_lba, "%lld\n", "%lld\n",
		   reipl_block_fcp->ipl_info.fcp.br_lba);
DEFINE_IPL_ATTR_RW(reipl_fcp, device, "0.0.%04llx\n", "0.0.%llx\n",
		   reipl_block_fcp->ipl_info.fcp.devno);

static struct attribute *reipl_fcp_attrs[] = {
	&sys_reipl_fcp_device_attr.attr,
	&sys_reipl_fcp_wwpn_attr.attr,
	&sys_reipl_fcp_lun_attr.attr,
	&sys_reipl_fcp_bootprog_attr.attr,
	&sys_reipl_fcp_br_lba_attr.attr,
	NULL,
};

static struct attribute_group reipl_fcp_attr_group = {
	.attrs = reipl_fcp_attrs,
};

/* CCW reipl device attributes */

DEFINE_IPL_ATTR_RW(reipl_ccw, device, "0.0.%04llx\n", "0.0.%llx\n",
	reipl_block_ccw->ipl_info.ccw.devno);
=======
	reipl_block_fcp->hdr.len = IPL_BP_FCP_LEN + scpdata_len;
	reipl_block_fcp->fcp.len = IPL_BP0_FCP_LEN + scpdata_len;
	reipl_block_fcp->fcp.scp_data_len = scpdata_len;

	return count;
}
static struct bin_attribute sys_reipl_fcp_scp_data_attr =
	__BIN_ATTR(scp_data, 0644, reipl_fcp_scpdata_read,
		   reipl_fcp_scpdata_write, DIAG308_SCPDATA_SIZE);

static struct bin_attribute *reipl_fcp_bin_attrs[] = {
	&sys_reipl_fcp_scp_data_attr,
	NULL,
};

DEFINE_IPL_ATTR_RW(reipl_fcp, wwpn, "0x%016llx\n", "%llx\n",
		   reipl_block_fcp->fcp.wwpn);
DEFINE_IPL_ATTR_RW(reipl_fcp, lun, "0x%016llx\n", "%llx\n",
		   reipl_block_fcp->fcp.lun);
DEFINE_IPL_ATTR_RW(reipl_fcp, bootprog, "%lld\n", "%lld\n",
		   reipl_block_fcp->fcp.bootprog);
DEFINE_IPL_ATTR_RW(reipl_fcp, br_lba, "%lld\n", "%lld\n",
		   reipl_block_fcp->fcp.br_lba);
DEFINE_IPL_ATTR_RW(reipl_fcp, device, "0.0.%04llx\n", "0.0.%llx\n",
		   reipl_block_fcp->fcp.devno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void reipl_get_ascii_loadparm(char *loadparm,
				     struct ipl_parameter_block *ibp)
{
<<<<<<< HEAD
	memcpy(loadparm, ibp->ipl_info.ccw.load_parm, LOADPARM_LEN);
=======
	memcpy(loadparm, ibp->common.loadparm, LOADPARM_LEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	EBCASC(loadparm, LOADPARM_LEN);
	loadparm[LOADPARM_LEN] = 0;
	strim(loadparm);
}

static ssize_t reipl_generic_loadparm_show(struct ipl_parameter_block *ipb,
					   char *page)
{
	char buf[LOADPARM_LEN + 1];

	reipl_get_ascii_loadparm(buf, ipb);
	return sprintf(page, "%s\n", buf);
}

static ssize_t reipl_generic_loadparm_store(struct ipl_parameter_block *ipb,
					    const char *buf, size_t len)
{
	int i, lp_len;

	/* ignore trailing newline */
	lp_len = len;
	if ((len > 0) && (buf[len - 1] == '\n'))
		lp_len--;
	/* loadparm can have max 8 characters and must not start with a blank */
	if ((lp_len > LOADPARM_LEN) || ((lp_len > 0) && (buf[0] == ' ')))
		return -EINVAL;
	/* loadparm can only contain "a-z,A-Z,0-9,SP,." */
	for (i = 0; i < lp_len; i++) {
		if (isalpha(buf[i]) || isdigit(buf[i]) || (buf[i] == ' ') ||
		    (buf[i] == '.'))
			continue;
		return -EINVAL;
	}
	/* initialize loadparm with blanks */
<<<<<<< HEAD
	memset(ipb->ipl_info.ccw.load_parm, ' ', LOADPARM_LEN);
	/* copy and convert to ebcdic */
	memcpy(ipb->ipl_info.ccw.load_parm, buf, lp_len);
	ASCEBC(ipb->ipl_info.ccw.load_parm, LOADPARM_LEN);
	return len;
}

/* NSS wrapper */
static ssize_t reipl_nss_loadparm_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *page)
{
	return reipl_generic_loadparm_show(reipl_block_nss, page);
}

static ssize_t reipl_nss_loadparm_store(struct kobject *kobj,
					struct kobj_attribute *attr,
					const char *buf, size_t len)
{
	return reipl_generic_loadparm_store(reipl_block_nss, buf, len);
}

/* CCW wrapper */
static ssize_t reipl_ccw_loadparm_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *page)
{
	return reipl_generic_loadparm_show(reipl_block_ccw, page);
}

static ssize_t reipl_ccw_loadparm_store(struct kobject *kobj,
					struct kobj_attribute *attr,
					const char *buf, size_t len)
{
	return reipl_generic_loadparm_store(reipl_block_ccw, buf, len);
}

static struct kobj_attribute sys_reipl_ccw_loadparm_attr =
	__ATTR(loadparm, S_IRUGO | S_IWUSR, reipl_ccw_loadparm_show,
					    reipl_ccw_loadparm_store);
=======
	memset(ipb->common.loadparm, ' ', LOADPARM_LEN);
	/* copy and convert to ebcdic */
	memcpy(ipb->common.loadparm, buf, lp_len);
	ASCEBC(ipb->common.loadparm, LOADPARM_LEN);
	ipb->common.flags |= IPL_PB0_FLAG_LOADPARM;
	return len;
}

#define DEFINE_GENERIC_LOADPARM(name)							\
static ssize_t reipl_##name##_loadparm_show(struct kobject *kobj,			\
					    struct kobj_attribute *attr, char *page)	\
{											\
	return reipl_generic_loadparm_show(reipl_block_##name, page);			\
}											\
static ssize_t reipl_##name##_loadparm_store(struct kobject *kobj,			\
					     struct kobj_attribute *attr,		\
					     const char *buf, size_t len)		\
{											\
	return reipl_generic_loadparm_store(reipl_block_##name, buf, len);		\
}											\
static struct kobj_attribute sys_reipl_##name##_loadparm_attr =				\
	__ATTR(loadparm, 0644, reipl_##name##_loadparm_show,				\
	       reipl_##name##_loadparm_store)

DEFINE_GENERIC_LOADPARM(fcp);
DEFINE_GENERIC_LOADPARM(nvme);
DEFINE_GENERIC_LOADPARM(ccw);
DEFINE_GENERIC_LOADPARM(nss);
DEFINE_GENERIC_LOADPARM(eckd);

static ssize_t reipl_fcp_clear_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%u\n", reipl_fcp_clear);
}

static ssize_t reipl_fcp_clear_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t len)
{
	if (kstrtobool(buf, &reipl_fcp_clear) < 0)
		return -EINVAL;
	return len;
}

static struct attribute *reipl_fcp_attrs[] = {
	&sys_reipl_fcp_device_attr.attr,
	&sys_reipl_fcp_wwpn_attr.attr,
	&sys_reipl_fcp_lun_attr.attr,
	&sys_reipl_fcp_bootprog_attr.attr,
	&sys_reipl_fcp_br_lba_attr.attr,
	&sys_reipl_fcp_loadparm_attr.attr,
	NULL,
};

static struct attribute_group reipl_fcp_attr_group = {
	.attrs = reipl_fcp_attrs,
	.bin_attrs = reipl_fcp_bin_attrs,
};

static struct kobj_attribute sys_reipl_fcp_clear_attr =
	__ATTR(clear, 0644, reipl_fcp_clear_show, reipl_fcp_clear_store);

/* NVME reipl device attributes */

static ssize_t reipl_nvme_scpdata_read(struct file *filp, struct kobject *kobj,
				      struct bin_attribute *attr,
				      char *buf, loff_t off, size_t count)
{
	size_t size = reipl_block_nvme->nvme.scp_data_len;
	void *scp_data = reipl_block_nvme->nvme.scp_data;

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

static ssize_t reipl_nvme_scpdata_write(struct file *filp, struct kobject *kobj,
				       struct bin_attribute *attr,
				       char *buf, loff_t off, size_t count)
{
	size_t scpdata_len = count;
	size_t padding;

	if (off)
		return -EINVAL;

	memcpy(reipl_block_nvme->nvme.scp_data, buf, count);
	if (scpdata_len % 8) {
		padding = 8 - (scpdata_len % 8);
		memset(reipl_block_nvme->nvme.scp_data + scpdata_len,
		       0, padding);
		scpdata_len += padding;
	}

	reipl_block_nvme->hdr.len = IPL_BP_FCP_LEN + scpdata_len;
	reipl_block_nvme->nvme.len = IPL_BP0_FCP_LEN + scpdata_len;
	reipl_block_nvme->nvme.scp_data_len = scpdata_len;

	return count;
}

static struct bin_attribute sys_reipl_nvme_scp_data_attr =
	__BIN_ATTR(scp_data, 0644, reipl_nvme_scpdata_read,
		   reipl_nvme_scpdata_write, DIAG308_SCPDATA_SIZE);

static struct bin_attribute *reipl_nvme_bin_attrs[] = {
	&sys_reipl_nvme_scp_data_attr,
	NULL,
};

DEFINE_IPL_ATTR_RW(reipl_nvme, fid, "0x%08llx\n", "%llx\n",
		   reipl_block_nvme->nvme.fid);
DEFINE_IPL_ATTR_RW(reipl_nvme, nsid, "0x%08llx\n", "%llx\n",
		   reipl_block_nvme->nvme.nsid);
DEFINE_IPL_ATTR_RW(reipl_nvme, bootprog, "%lld\n", "%lld\n",
		   reipl_block_nvme->nvme.bootprog);
DEFINE_IPL_ATTR_RW(reipl_nvme, br_lba, "%lld\n", "%lld\n",
		   reipl_block_nvme->nvme.br_lba);

static struct attribute *reipl_nvme_attrs[] = {
	&sys_reipl_nvme_fid_attr.attr,
	&sys_reipl_nvme_nsid_attr.attr,
	&sys_reipl_nvme_bootprog_attr.attr,
	&sys_reipl_nvme_br_lba_attr.attr,
	&sys_reipl_nvme_loadparm_attr.attr,
	NULL,
};

static struct attribute_group reipl_nvme_attr_group = {
	.attrs = reipl_nvme_attrs,
	.bin_attrs = reipl_nvme_bin_attrs
};

static ssize_t reipl_nvme_clear_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%u\n", reipl_nvme_clear);
}

static ssize_t reipl_nvme_clear_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t len)
{
	if (kstrtobool(buf, &reipl_nvme_clear) < 0)
		return -EINVAL;
	return len;
}

static struct kobj_attribute sys_reipl_nvme_clear_attr =
	__ATTR(clear, 0644, reipl_nvme_clear_show, reipl_nvme_clear_store);

/* CCW reipl device attributes */
DEFINE_IPL_CCW_ATTR_RW(reipl_ccw, device, reipl_block_ccw->ccw);

static ssize_t reipl_ccw_clear_show(struct kobject *kobj,
				    struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%u\n", reipl_ccw_clear);
}

static ssize_t reipl_ccw_clear_store(struct kobject *kobj,
				     struct kobj_attribute *attr,
				     const char *buf, size_t len)
{
	if (kstrtobool(buf, &reipl_ccw_clear) < 0)
		return -EINVAL;
	return len;
}

static struct kobj_attribute sys_reipl_ccw_clear_attr =
	__ATTR(clear, 0644, reipl_ccw_clear_show, reipl_ccw_clear_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *reipl_ccw_attrs_vm[] = {
	&sys_reipl_ccw_device_attr.attr,
	&sys_reipl_ccw_loadparm_attr.attr,
	&sys_reipl_ccw_vmparm_attr.attr,
<<<<<<< HEAD
=======
	&sys_reipl_ccw_clear_attr.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL,
};

static struct attribute *reipl_ccw_attrs_lpar[] = {
	&sys_reipl_ccw_device_attr.attr,
	&sys_reipl_ccw_loadparm_attr.attr,
<<<<<<< HEAD
=======
	&sys_reipl_ccw_clear_attr.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL,
};

static struct attribute_group reipl_ccw_attr_group_vm = {
	.name  = IPL_CCW_STR,
	.attrs = reipl_ccw_attrs_vm,
};

static struct attribute_group reipl_ccw_attr_group_lpar = {
	.name  = IPL_CCW_STR,
	.attrs = reipl_ccw_attrs_lpar,
};

<<<<<<< HEAD
=======
/* ECKD reipl device attributes */

static ssize_t reipl_eckd_scpdata_read(struct file *filp, struct kobject *kobj,
				       struct bin_attribute *attr,
				       char *buf, loff_t off, size_t count)
{
	size_t size = reipl_block_eckd->eckd.scp_data_len;
	void *scp_data = reipl_block_eckd->eckd.scp_data;

	return memory_read_from_buffer(buf, count, &off, scp_data, size);
}

static ssize_t reipl_eckd_scpdata_write(struct file *filp, struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf, loff_t off, size_t count)
{
	size_t scpdata_len = count;
	size_t padding;

	if (off)
		return -EINVAL;

	memcpy(reipl_block_eckd->eckd.scp_data, buf, count);
	if (scpdata_len % 8) {
		padding = 8 - (scpdata_len % 8);
		memset(reipl_block_eckd->eckd.scp_data + scpdata_len,
		       0, padding);
		scpdata_len += padding;
	}

	reipl_block_eckd->hdr.len = IPL_BP_ECKD_LEN + scpdata_len;
	reipl_block_eckd->eckd.len = IPL_BP0_ECKD_LEN + scpdata_len;
	reipl_block_eckd->eckd.scp_data_len = scpdata_len;

	return count;
}

static struct bin_attribute sys_reipl_eckd_scp_data_attr =
	__BIN_ATTR(scp_data, 0644, reipl_eckd_scpdata_read,
		   reipl_eckd_scpdata_write, DIAG308_SCPDATA_SIZE);

static struct bin_attribute *reipl_eckd_bin_attrs[] = {
	&sys_reipl_eckd_scp_data_attr,
	NULL,
};

DEFINE_IPL_CCW_ATTR_RW(reipl_eckd, device, reipl_block_eckd->eckd);
DEFINE_IPL_ATTR_RW(reipl_eckd, bootprog, "%lld\n", "%lld\n",
		   reipl_block_eckd->eckd.bootprog);

static struct attribute *reipl_eckd_attrs[] = {
	&sys_reipl_eckd_device_attr.attr,
	&sys_reipl_eckd_bootprog_attr.attr,
	&sys_reipl_eckd_br_chr_attr.attr,
	&sys_reipl_eckd_loadparm_attr.attr,
	NULL,
};

static struct attribute_group reipl_eckd_attr_group = {
	.attrs = reipl_eckd_attrs,
	.bin_attrs = reipl_eckd_bin_attrs
};

static ssize_t reipl_eckd_clear_show(struct kobject *kobj,
				     struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%u\n", reipl_eckd_clear);
}

static ssize_t reipl_eckd_clear_store(struct kobject *kobj,
				      struct kobj_attribute *attr,
				      const char *buf, size_t len)
{
	if (kstrtobool(buf, &reipl_eckd_clear) < 0)
		return -EINVAL;
	return len;
}

static struct kobj_attribute sys_reipl_eckd_clear_attr =
	__ATTR(clear, 0644, reipl_eckd_clear_show, reipl_eckd_clear_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* NSS reipl device attributes */
static void reipl_get_ascii_nss_name(char *dst,
				     struct ipl_parameter_block *ipb)
{
<<<<<<< HEAD
	memcpy(dst, ipb->ipl_info.ccw.nss_name, NSS_NAME_SIZE);
=======
	memcpy(dst, ipb->ccw.nss_name, NSS_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	EBCASC(dst, NSS_NAME_SIZE);
	dst[NSS_NAME_SIZE] = 0;
}

static ssize_t reipl_nss_name_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *page)
{
	char nss_name[NSS_NAME_SIZE + 1] = {};

	reipl_get_ascii_nss_name(nss_name, reipl_block_nss);
	return sprintf(page, "%s\n", nss_name);
}

static ssize_t reipl_nss_name_store(struct kobject *kobj,
				    struct kobj_attribute *attr,
				    const char *buf, size_t len)
{
	int nss_len;

	/* ignore trailing newline */
	nss_len = len;
	if ((len > 0) && (buf[len - 1] == '\n'))
		nss_len--;

	if (nss_len > NSS_NAME_SIZE)
		return -EINVAL;

<<<<<<< HEAD
	memset(reipl_block_nss->ipl_info.ccw.nss_name, 0x40, NSS_NAME_SIZE);
	if (nss_len > 0) {
		reipl_block_nss->ipl_info.ccw.vm_flags |=
			DIAG308_VM_FLAGS_NSS_VALID;
		memcpy(reipl_block_nss->ipl_info.ccw.nss_name, buf, nss_len);
		ASCEBC(reipl_block_nss->ipl_info.ccw.nss_name, nss_len);
		EBC_TOUPPER(reipl_block_nss->ipl_info.ccw.nss_name, nss_len);
	} else {
		reipl_block_nss->ipl_info.ccw.vm_flags &=
			~DIAG308_VM_FLAGS_NSS_VALID;
=======
	memset(reipl_block_nss->ccw.nss_name, 0x40, NSS_NAME_SIZE);
	if (nss_len > 0) {
		reipl_block_nss->ccw.vm_flags |= IPL_PB0_CCW_VM_FLAG_NSS;
		memcpy(reipl_block_nss->ccw.nss_name, buf, nss_len);
		ASCEBC(reipl_block_nss->ccw.nss_name, nss_len);
		EBC_TOUPPER(reipl_block_nss->ccw.nss_name, nss_len);
	} else {
		reipl_block_nss->ccw.vm_flags &= ~IPL_PB0_CCW_VM_FLAG_NSS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return len;
}

static struct kobj_attribute sys_reipl_nss_name_attr =
<<<<<<< HEAD
	__ATTR(name, S_IRUGO | S_IWUSR, reipl_nss_name_show,
					reipl_nss_name_store);

static struct kobj_attribute sys_reipl_nss_loadparm_attr =
	__ATTR(loadparm, S_IRUGO | S_IWUSR, reipl_nss_loadparm_show,
					    reipl_nss_loadparm_store);
=======
	__ATTR(name, 0644, reipl_nss_name_show,
	       reipl_nss_name_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *reipl_nss_attrs[] = {
	&sys_reipl_nss_name_attr.attr,
	&sys_reipl_nss_loadparm_attr.attr,
	&sys_reipl_nss_vmparm_attr.attr,
	NULL,
};

static struct attribute_group reipl_nss_attr_group = {
	.name  = IPL_NSS_STR,
	.attrs = reipl_nss_attrs,
};

<<<<<<< HEAD
static void set_reipl_block_actual(struct ipl_parameter_block *reipl_block)
{
	reipl_block_actual = reipl_block;
	os_info_entry_add(OS_INFO_REIPL_BLOCK, reipl_block_actual,
			  reipl_block->hdr.len);
=======
void set_os_info_reipl_block(void)
{
	os_info_entry_add(OS_INFO_REIPL_BLOCK, reipl_block_actual,
			  reipl_block_actual->hdr.len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* reipl type */

static int reipl_set_type(enum ipl_type type)
{
	if (!(reipl_capabilities & type))
		return -EINVAL;

	switch(type) {
	case IPL_TYPE_CCW:
<<<<<<< HEAD
		if (diag308_set_works)
			reipl_method = REIPL_METHOD_CCW_DIAG;
		else if (MACHINE_IS_VM)
			reipl_method = REIPL_METHOD_CCW_VM;
		else
			reipl_method = REIPL_METHOD_CCW_CIO;
		set_reipl_block_actual(reipl_block_ccw);
		break;
	case IPL_TYPE_FCP:
		if (diag308_set_works)
			reipl_method = REIPL_METHOD_FCP_RW_DIAG;
		else if (MACHINE_IS_VM)
			reipl_method = REIPL_METHOD_FCP_RO_VM;
		else
			reipl_method = REIPL_METHOD_FCP_RO_DIAG;
		set_reipl_block_actual(reipl_block_fcp);
		break;
	case IPL_TYPE_FCP_DUMP:
		reipl_method = REIPL_METHOD_FCP_DUMP;
		break;
	case IPL_TYPE_NSS:
		if (diag308_set_works)
			reipl_method = REIPL_METHOD_NSS_DIAG;
		else
			reipl_method = REIPL_METHOD_NSS;
		set_reipl_block_actual(reipl_block_nss);
		break;
	case IPL_TYPE_UNKNOWN:
		reipl_method = REIPL_METHOD_DEFAULT;
		break;
	default:
		BUG();
=======
		reipl_block_actual = reipl_block_ccw;
		break;
	case IPL_TYPE_ECKD:
		reipl_block_actual = reipl_block_eckd;
		break;
	case IPL_TYPE_FCP:
		reipl_block_actual = reipl_block_fcp;
		break;
	case IPL_TYPE_NVME:
		reipl_block_actual = reipl_block_nvme;
		break;
	case IPL_TYPE_NSS:
		reipl_block_actual = reipl_block_nss;
		break;
	default:
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	reipl_type = type;
	return 0;
}

static ssize_t reipl_type_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", ipl_type_str(reipl_type));
}

static ssize_t reipl_type_store(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t len)
{
	int rc = -EINVAL;

	if (strncmp(buf, IPL_CCW_STR, strlen(IPL_CCW_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_CCW);
<<<<<<< HEAD
	else if (strncmp(buf, IPL_FCP_STR, strlen(IPL_FCP_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_FCP);
=======
	else if (strncmp(buf, IPL_ECKD_STR, strlen(IPL_ECKD_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_ECKD);
	else if (strncmp(buf, IPL_FCP_STR, strlen(IPL_FCP_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_FCP);
	else if (strncmp(buf, IPL_NVME_STR, strlen(IPL_NVME_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_NVME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (strncmp(buf, IPL_NSS_STR, strlen(IPL_NSS_STR)) == 0)
		rc = reipl_set_type(IPL_TYPE_NSS);
	return (rc != 0) ? rc : len;
}

static struct kobj_attribute reipl_type_attr =
	__ATTR(reipl_type, 0644, reipl_type_show, reipl_type_store);

static struct kset *reipl_kset;
static struct kset *reipl_fcp_kset;
<<<<<<< HEAD

static void get_ipl_string(char *dst, struct ipl_parameter_block *ipb,
			   const enum ipl_method m)
{
	char loadparm[LOADPARM_LEN + 1] = {};
	char vmparm[DIAG308_VMPARM_SIZE + 1] = {};
	char nss_name[NSS_NAME_SIZE + 1] = {};
	size_t pos = 0;

	reipl_get_ascii_loadparm(loadparm, ipb);
	reipl_get_ascii_nss_name(nss_name, ipb);
	reipl_get_ascii_vmparm(vmparm, sizeof(vmparm), ipb);

	switch (m) {
	case REIPL_METHOD_CCW_VM:
		pos = sprintf(dst, "IPL %X CLEAR", ipb->ipl_info.ccw.devno);
		break;
	case REIPL_METHOD_NSS:
		pos = sprintf(dst, "IPL %s", nss_name);
		break;
	default:
		break;
	}
	if (strlen(loadparm) > 0)
		pos += sprintf(dst + pos, " LOADPARM '%s'", loadparm);
	if (strlen(vmparm) > 0)
		sprintf(dst + pos, " PARM %s", vmparm);
}

static void __reipl_run(void *unused)
{
	struct ccw_dev_id devid;
	static char buf[128];

	switch (reipl_method) {
	case REIPL_METHOD_CCW_CIO:
		devid.devno = reipl_block_ccw->ipl_info.ccw.devno;
		devid.ssid  = 0;
		reipl_ccw_dev(&devid);
		break;
	case REIPL_METHOD_CCW_VM:
		get_ipl_string(buf, reipl_block_ccw, REIPL_METHOD_CCW_VM);
		__cpcmd(buf, NULL, 0, NULL);
		break;
	case REIPL_METHOD_CCW_DIAG:
		diag308(DIAG308_SET, reipl_block_ccw);
		diag308(DIAG308_IPL, NULL);
		break;
	case REIPL_METHOD_FCP_RW_DIAG:
		diag308(DIAG308_SET, reipl_block_fcp);
		diag308(DIAG308_IPL, NULL);
		break;
	case REIPL_METHOD_FCP_RO_DIAG:
		diag308(DIAG308_IPL, NULL);
		break;
	case REIPL_METHOD_FCP_RO_VM:
		__cpcmd("IPL", NULL, 0, NULL);
		break;
	case REIPL_METHOD_NSS_DIAG:
		diag308(DIAG308_SET, reipl_block_nss);
		diag308(DIAG308_IPL, NULL);
		break;
	case REIPL_METHOD_NSS:
		get_ipl_string(buf, reipl_block_nss, REIPL_METHOD_NSS);
		__cpcmd(buf, NULL, 0, NULL);
		break;
	case REIPL_METHOD_DEFAULT:
		if (MACHINE_IS_VM)
			__cpcmd("IPL", NULL, 0, NULL);
		diag308(DIAG308_IPL, NULL);
		break;
	case REIPL_METHOD_FCP_DUMP:
		break;
	}
	disabled_wait((unsigned long) __builtin_return_address(0));
=======
static struct kset *reipl_nvme_kset;
static struct kset *reipl_eckd_kset;

static void __reipl_run(void *unused)
{
	switch (reipl_type) {
	case IPL_TYPE_CCW:
		diag308(DIAG308_SET, reipl_block_ccw);
		if (reipl_ccw_clear)
			diag308(DIAG308_LOAD_CLEAR, NULL);
		else
			diag308(DIAG308_LOAD_NORMAL_DUMP, NULL);
		break;
	case IPL_TYPE_ECKD:
		diag308(DIAG308_SET, reipl_block_eckd);
		if (reipl_eckd_clear)
			diag308(DIAG308_LOAD_CLEAR, NULL);
		else
			diag308(DIAG308_LOAD_NORMAL, NULL);
		break;
	case IPL_TYPE_FCP:
		diag308(DIAG308_SET, reipl_block_fcp);
		if (reipl_fcp_clear)
			diag308(DIAG308_LOAD_CLEAR, NULL);
		else
			diag308(DIAG308_LOAD_NORMAL, NULL);
		break;
	case IPL_TYPE_NVME:
		diag308(DIAG308_SET, reipl_block_nvme);
		if (reipl_nvme_clear)
			diag308(DIAG308_LOAD_CLEAR, NULL);
		else
			diag308(DIAG308_LOAD_NORMAL, NULL);
		break;
	case IPL_TYPE_NSS:
		diag308(DIAG308_SET, reipl_block_nss);
		diag308(DIAG308_LOAD_CLEAR, NULL);
		break;
	case IPL_TYPE_UNKNOWN:
		diag308(DIAG308_LOAD_CLEAR, NULL);
		break;
	case IPL_TYPE_FCP_DUMP:
	case IPL_TYPE_NVME_DUMP:
	case IPL_TYPE_ECKD_DUMP:
		break;
	}
	disabled_wait();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void reipl_run(struct shutdown_trigger *trigger)
{
	smp_call_ipl_cpu(__reipl_run, NULL);
}

static void reipl_block_ccw_init(struct ipl_parameter_block *ipb)
{
<<<<<<< HEAD
	ipb->hdr.len = IPL_PARM_BLK_CCW_LEN;
	ipb->hdr.version = IPL_PARM_BLOCK_VERSION;
	ipb->hdr.blk0_len = IPL_PARM_BLK0_CCW_LEN;
	ipb->hdr.pbt = DIAG308_IPL_TYPE_CCW;
=======
	ipb->hdr.len = IPL_BP_CCW_LEN;
	ipb->hdr.version = IPL_PARM_BLOCK_VERSION;
	ipb->pb0_hdr.len = IPL_BP0_CCW_LEN;
	ipb->pb0_hdr.pbt = IPL_PBT_CCW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void reipl_block_ccw_fill_parms(struct ipl_parameter_block *ipb)
{
	/* LOADPARM */
	/* check if read scp info worked and set loadparm */
	if (sclp_ipl_info.is_valid)
<<<<<<< HEAD
		memcpy(ipb->ipl_info.ccw.load_parm,
				&sclp_ipl_info.loadparm, LOADPARM_LEN);
	else
		/* read scp info failed: set empty loadparm (EBCDIC blanks) */
		memset(ipb->ipl_info.ccw.load_parm, 0x40, LOADPARM_LEN);
	ipb->hdr.flags = DIAG308_FLAGS_LP_VALID;

	/* VM PARM */
	if (MACHINE_IS_VM && diag308_set_works &&
	    (ipl_block.ipl_info.ccw.vm_flags & DIAG308_VM_FLAGS_VP_VALID)) {

		ipb->ipl_info.ccw.vm_flags |= DIAG308_VM_FLAGS_VP_VALID;
		ipb->ipl_info.ccw.vm_parm_len =
					ipl_block.ipl_info.ccw.vm_parm_len;
		memcpy(ipb->ipl_info.ccw.vm_parm,
		       ipl_block.ipl_info.ccw.vm_parm, DIAG308_VMPARM_SIZE);
=======
		memcpy(ipb->ccw.loadparm, &sclp_ipl_info.loadparm, LOADPARM_LEN);
	else
		/* read scp info failed: set empty loadparm (EBCDIC blanks) */
		memset(ipb->ccw.loadparm, 0x40, LOADPARM_LEN);
	ipb->ccw.flags = IPL_PB0_FLAG_LOADPARM;

	/* VM PARM */
	if (MACHINE_IS_VM && ipl_block_valid &&
	    (ipl_block.ccw.vm_flags & IPL_PB0_CCW_VM_FLAG_VP)) {

		ipb->ccw.vm_flags |= IPL_PB0_CCW_VM_FLAG_VP;
		ipb->ccw.vm_parm_len = ipl_block.ccw.vm_parm_len;
		memcpy(ipb->ccw.vm_parm,
		       ipl_block.ccw.vm_parm, DIAG308_VMPARM_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int __init reipl_nss_init(void)
{
	int rc;

	if (!MACHINE_IS_VM)
		return 0;

	reipl_block_nss = (void *) get_zeroed_page(GFP_KERNEL);
	if (!reipl_block_nss)
		return -ENOMEM;

<<<<<<< HEAD
	if (!diag308_set_works)
		sys_reipl_nss_vmparm_attr.attr.mode = S_IRUGO;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = sysfs_create_group(&reipl_kset->kobj, &reipl_nss_attr_group);
	if (rc)
		return rc;

	reipl_block_ccw_init(reipl_block_nss);
<<<<<<< HEAD
	if (ipl_info.type == IPL_TYPE_NSS) {
		memset(reipl_block_nss->ipl_info.ccw.nss_name,
			' ', NSS_NAME_SIZE);
		memcpy(reipl_block_nss->ipl_info.ccw.nss_name,
			kernel_nss_name, strlen(kernel_nss_name));
		ASCEBC(reipl_block_nss->ipl_info.ccw.nss_name, NSS_NAME_SIZE);
		reipl_block_nss->ipl_info.ccw.vm_flags |=
			DIAG308_VM_FLAGS_NSS_VALID;

		reipl_block_ccw_fill_parms(reipl_block_nss);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reipl_capabilities |= IPL_TYPE_NSS;
	return 0;
}

static int __init reipl_ccw_init(void)
{
	int rc;

	reipl_block_ccw = (void *) get_zeroed_page(GFP_KERNEL);
	if (!reipl_block_ccw)
		return -ENOMEM;

<<<<<<< HEAD
	if (MACHINE_IS_VM) {
		if (!diag308_set_works)
			sys_reipl_ccw_vmparm_attr.attr.mode = S_IRUGO;
		rc = sysfs_create_group(&reipl_kset->kobj,
					&reipl_ccw_attr_group_vm);
	} else {
		if(!diag308_set_works)
			sys_reipl_ccw_loadparm_attr.attr.mode = S_IRUGO;
		rc = sysfs_create_group(&reipl_kset->kobj,
					&reipl_ccw_attr_group_lpar);
	}
=======
	rc = sysfs_create_group(&reipl_kset->kobj,
				MACHINE_IS_VM ? &reipl_ccw_attr_group_vm
					      : &reipl_ccw_attr_group_lpar);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	reipl_block_ccw_init(reipl_block_ccw);
	if (ipl_info.type == IPL_TYPE_CCW) {
<<<<<<< HEAD
		reipl_block_ccw->ipl_info.ccw.devno = ipl_devno;
=======
		reipl_block_ccw->ccw.ssid = ipl_block.ccw.ssid;
		reipl_block_ccw->ccw.devno = ipl_block.ccw.devno;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		reipl_block_ccw_fill_parms(reipl_block_ccw);
	}

	reipl_capabilities |= IPL_TYPE_CCW;
	return 0;
}

static int __init reipl_fcp_init(void)
{
	int rc;

<<<<<<< HEAD
	if (!diag308_set_works) {
		if (ipl_info.type == IPL_TYPE_FCP) {
			make_attrs_ro(reipl_fcp_attrs);
			sys_reipl_fcp_scp_data_attr.attr.mode = S_IRUGO;
		} else
			return 0;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reipl_block_fcp = (void *) get_zeroed_page(GFP_KERNEL);
	if (!reipl_block_fcp)
		return -ENOMEM;

	/* sysfs: create fcp kset for mixing attr group and bin attrs */
	reipl_fcp_kset = kset_create_and_add(IPL_FCP_STR, NULL,
					     &reipl_kset->kobj);
	if (!reipl_fcp_kset) {
		free_page((unsigned long) reipl_block_fcp);
		return -ENOMEM;
	}

	rc = sysfs_create_group(&reipl_fcp_kset->kobj, &reipl_fcp_attr_group);
<<<<<<< HEAD
	if (rc) {
		kset_unregister(reipl_fcp_kset);
		free_page((unsigned long) reipl_block_fcp);
		return rc;
	}

	rc = sysfs_create_bin_file(&reipl_fcp_kset->kobj,
				   &sys_reipl_fcp_scp_data_attr);
	if (rc) {
		sysfs_remove_group(&reipl_fcp_kset->kobj, &reipl_fcp_attr_group);
		kset_unregister(reipl_fcp_kset);
		free_page((unsigned long) reipl_block_fcp);
		return rc;
	}

	if (ipl_info.type == IPL_TYPE_FCP)
		memcpy(reipl_block_fcp, IPL_PARMBLOCK_START, PAGE_SIZE);
	else {
		reipl_block_fcp->hdr.len = IPL_PARM_BLK_FCP_LEN;
		reipl_block_fcp->hdr.version = IPL_PARM_BLOCK_VERSION;
		reipl_block_fcp->hdr.blk0_len = IPL_PARM_BLK0_FCP_LEN;
		reipl_block_fcp->hdr.pbt = DIAG308_IPL_TYPE_FCP;
		reipl_block_fcp->ipl_info.fcp.opt = DIAG308_IPL_OPT_IPL;
	}
	reipl_capabilities |= IPL_TYPE_FCP;
	return 0;
=======
	if (rc)
		goto out1;

	if (test_facility(141)) {
		rc = sysfs_create_file(&reipl_fcp_kset->kobj,
				       &sys_reipl_fcp_clear_attr.attr);
		if (rc)
			goto out2;
	} else {
		reipl_fcp_clear = true;
	}

	if (ipl_info.type == IPL_TYPE_FCP) {
		memcpy(reipl_block_fcp, &ipl_block, sizeof(ipl_block));
		/*
		 * Fix loadparm: There are systems where the (SCSI) LOADPARM
		 * is invalid in the SCSI IPL parameter block, so take it
		 * always from sclp_ipl_info.
		 */
		memcpy(reipl_block_fcp->fcp.loadparm, sclp_ipl_info.loadparm,
		       LOADPARM_LEN);
	} else {
		reipl_block_fcp->hdr.len = IPL_BP_FCP_LEN;
		reipl_block_fcp->hdr.version = IPL_PARM_BLOCK_VERSION;
		reipl_block_fcp->fcp.len = IPL_BP0_FCP_LEN;
		reipl_block_fcp->fcp.pbt = IPL_PBT_FCP;
		reipl_block_fcp->fcp.opt = IPL_PB0_FCP_OPT_IPL;
	}
	reipl_capabilities |= IPL_TYPE_FCP;
	return 0;

out2:
	sysfs_remove_group(&reipl_fcp_kset->kobj, &reipl_fcp_attr_group);
out1:
	kset_unregister(reipl_fcp_kset);
	free_page((unsigned long) reipl_block_fcp);
	return rc;
}

static int __init reipl_nvme_init(void)
{
	int rc;

	reipl_block_nvme = (void *) get_zeroed_page(GFP_KERNEL);
	if (!reipl_block_nvme)
		return -ENOMEM;

	/* sysfs: create kset for mixing attr group and bin attrs */
	reipl_nvme_kset = kset_create_and_add(IPL_NVME_STR, NULL,
					     &reipl_kset->kobj);
	if (!reipl_nvme_kset) {
		free_page((unsigned long) reipl_block_nvme);
		return -ENOMEM;
	}

	rc = sysfs_create_group(&reipl_nvme_kset->kobj, &reipl_nvme_attr_group);
	if (rc)
		goto out1;

	if (test_facility(141)) {
		rc = sysfs_create_file(&reipl_nvme_kset->kobj,
				       &sys_reipl_nvme_clear_attr.attr);
		if (rc)
			goto out2;
	} else {
		reipl_nvme_clear = true;
	}

	if (ipl_info.type == IPL_TYPE_NVME) {
		memcpy(reipl_block_nvme, &ipl_block, sizeof(ipl_block));
		/*
		 * Fix loadparm: There are systems where the (SCSI) LOADPARM
		 * is invalid in the IPL parameter block, so take it
		 * always from sclp_ipl_info.
		 */
		memcpy(reipl_block_nvme->nvme.loadparm, sclp_ipl_info.loadparm,
		       LOADPARM_LEN);
	} else {
		reipl_block_nvme->hdr.len = IPL_BP_NVME_LEN;
		reipl_block_nvme->hdr.version = IPL_PARM_BLOCK_VERSION;
		reipl_block_nvme->nvme.len = IPL_BP0_NVME_LEN;
		reipl_block_nvme->nvme.pbt = IPL_PBT_NVME;
		reipl_block_nvme->nvme.opt = IPL_PB0_NVME_OPT_IPL;
	}
	reipl_capabilities |= IPL_TYPE_NVME;
	return 0;

out2:
	sysfs_remove_group(&reipl_nvme_kset->kobj, &reipl_nvme_attr_group);
out1:
	kset_unregister(reipl_nvme_kset);
	free_page((unsigned long) reipl_block_nvme);
	return rc;
}

static int __init reipl_eckd_init(void)
{
	int rc;

	if (!sclp.has_sipl_eckd)
		return 0;

	reipl_block_eckd = (void *)get_zeroed_page(GFP_KERNEL);
	if (!reipl_block_eckd)
		return -ENOMEM;

	/* sysfs: create kset for mixing attr group and bin attrs */
	reipl_eckd_kset = kset_create_and_add(IPL_ECKD_STR, NULL,
					      &reipl_kset->kobj);
	if (!reipl_eckd_kset) {
		free_page((unsigned long)reipl_block_eckd);
		return -ENOMEM;
	}

	rc = sysfs_create_group(&reipl_eckd_kset->kobj, &reipl_eckd_attr_group);
	if (rc)
		goto out1;

	if (test_facility(141)) {
		rc = sysfs_create_file(&reipl_eckd_kset->kobj,
				       &sys_reipl_eckd_clear_attr.attr);
		if (rc)
			goto out2;
	} else {
		reipl_eckd_clear = true;
	}

	if (ipl_info.type == IPL_TYPE_ECKD) {
		memcpy(reipl_block_eckd, &ipl_block, sizeof(ipl_block));
	} else {
		reipl_block_eckd->hdr.len = IPL_BP_ECKD_LEN;
		reipl_block_eckd->hdr.version = IPL_PARM_BLOCK_VERSION;
		reipl_block_eckd->eckd.len = IPL_BP0_ECKD_LEN;
		reipl_block_eckd->eckd.pbt = IPL_PBT_ECKD;
		reipl_block_eckd->eckd.opt = IPL_PB0_ECKD_OPT_IPL;
	}
	reipl_capabilities |= IPL_TYPE_ECKD;
	return 0;

out2:
	sysfs_remove_group(&reipl_eckd_kset->kobj, &reipl_eckd_attr_group);
out1:
	kset_unregister(reipl_eckd_kset);
	free_page((unsigned long)reipl_block_eckd);
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init reipl_type_init(void)
{
	enum ipl_type reipl_type = ipl_info.type;
	struct ipl_parameter_block *reipl_block;
	unsigned long size;

	reipl_block = os_info_old_entry(OS_INFO_REIPL_BLOCK, &size);
	if (!reipl_block)
		goto out;
	/*
	 * If we have an OS info reipl block, this will be used
	 */
<<<<<<< HEAD
	if (reipl_block->hdr.pbt == DIAG308_IPL_TYPE_FCP) {
		memcpy(reipl_block_fcp, reipl_block, size);
		reipl_type = IPL_TYPE_FCP;
	} else if (reipl_block->hdr.pbt == DIAG308_IPL_TYPE_CCW) {
		memcpy(reipl_block_ccw, reipl_block, size);
		reipl_type = IPL_TYPE_CCW;
=======
	if (reipl_block->pb0_hdr.pbt == IPL_PBT_FCP) {
		memcpy(reipl_block_fcp, reipl_block, size);
		reipl_type = IPL_TYPE_FCP;
	} else if (reipl_block->pb0_hdr.pbt == IPL_PBT_NVME) {
		memcpy(reipl_block_nvme, reipl_block, size);
		reipl_type = IPL_TYPE_NVME;
	} else if (reipl_block->pb0_hdr.pbt == IPL_PBT_CCW) {
		memcpy(reipl_block_ccw, reipl_block, size);
		reipl_type = IPL_TYPE_CCW;
	} else if (reipl_block->pb0_hdr.pbt == IPL_PBT_ECKD) {
		memcpy(reipl_block_eckd, reipl_block, size);
		reipl_type = IPL_TYPE_ECKD;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
out:
	return reipl_set_type(reipl_type);
}

static int __init reipl_init(void)
{
	int rc;

	reipl_kset = kset_create_and_add("reipl", NULL, firmware_kobj);
	if (!reipl_kset)
		return -ENOMEM;
	rc = sysfs_create_file(&reipl_kset->kobj, &reipl_type_attr.attr);
	if (rc) {
		kset_unregister(reipl_kset);
		return rc;
	}
	rc = reipl_ccw_init();
	if (rc)
		return rc;
<<<<<<< HEAD
	rc = reipl_fcp_init();
	if (rc)
		return rc;
=======
	rc = reipl_eckd_init();
	if (rc)
		return rc;
	rc = reipl_fcp_init();
	if (rc)
		return rc;
	rc = reipl_nvme_init();
	if (rc)
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = reipl_nss_init();
	if (rc)
		return rc;
	return reipl_type_init();
}

static struct shutdown_action __refdata reipl_action = {
	.name	= SHUTDOWN_ACTION_REIPL_STR,
	.fn	= reipl_run,
	.init	= reipl_init,
};

/*
 * dump shutdown action: Dump Linux on shutdown.
 */

/* FCP dump device attributes */

<<<<<<< HEAD
DEFINE_IPL_ATTR_RW(dump_fcp, wwpn, "0x%016llx\n", "%016llx\n",
		   dump_block_fcp->ipl_info.fcp.wwpn);
DEFINE_IPL_ATTR_RW(dump_fcp, lun, "0x%016llx\n", "%016llx\n",
		   dump_block_fcp->ipl_info.fcp.lun);
DEFINE_IPL_ATTR_RW(dump_fcp, bootprog, "%lld\n", "%lld\n",
		   dump_block_fcp->ipl_info.fcp.bootprog);
DEFINE_IPL_ATTR_RW(dump_fcp, br_lba, "%lld\n", "%lld\n",
		   dump_block_fcp->ipl_info.fcp.br_lba);
DEFINE_IPL_ATTR_RW(dump_fcp, device, "0.0.%04llx\n", "0.0.%llx\n",
		   dump_block_fcp->ipl_info.fcp.devno);
=======
DEFINE_IPL_ATTR_RW(dump_fcp, wwpn, "0x%016llx\n", "%llx\n",
		   dump_block_fcp->fcp.wwpn);
DEFINE_IPL_ATTR_RW(dump_fcp, lun, "0x%016llx\n", "%llx\n",
		   dump_block_fcp->fcp.lun);
DEFINE_IPL_ATTR_RW(dump_fcp, bootprog, "%lld\n", "%lld\n",
		   dump_block_fcp->fcp.bootprog);
DEFINE_IPL_ATTR_RW(dump_fcp, br_lba, "%lld\n", "%lld\n",
		   dump_block_fcp->fcp.br_lba);
DEFINE_IPL_ATTR_RW(dump_fcp, device, "0.0.%04llx\n", "0.0.%llx\n",
		   dump_block_fcp->fcp.devno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *dump_fcp_attrs[] = {
	&sys_dump_fcp_device_attr.attr,
	&sys_dump_fcp_wwpn_attr.attr,
	&sys_dump_fcp_lun_attr.attr,
	&sys_dump_fcp_bootprog_attr.attr,
	&sys_dump_fcp_br_lba_attr.attr,
	NULL,
};

static struct attribute_group dump_fcp_attr_group = {
	.name  = IPL_FCP_STR,
	.attrs = dump_fcp_attrs,
};

<<<<<<< HEAD
/* CCW dump device attributes */

DEFINE_IPL_ATTR_RW(dump_ccw, device, "0.0.%04llx\n", "0.0.%llx\n",
		   dump_block_ccw->ipl_info.ccw.devno);
=======
/* NVME dump device attributes */
DEFINE_IPL_ATTR_RW(dump_nvme, fid, "0x%08llx\n", "%llx\n",
		   dump_block_nvme->nvme.fid);
DEFINE_IPL_ATTR_RW(dump_nvme, nsid, "0x%08llx\n", "%llx\n",
		   dump_block_nvme->nvme.nsid);
DEFINE_IPL_ATTR_RW(dump_nvme, bootprog, "%lld\n", "%llx\n",
		   dump_block_nvme->nvme.bootprog);
DEFINE_IPL_ATTR_RW(dump_nvme, br_lba, "%lld\n", "%llx\n",
		   dump_block_nvme->nvme.br_lba);

static struct attribute *dump_nvme_attrs[] = {
	&sys_dump_nvme_fid_attr.attr,
	&sys_dump_nvme_nsid_attr.attr,
	&sys_dump_nvme_bootprog_attr.attr,
	&sys_dump_nvme_br_lba_attr.attr,
	NULL,
};

static struct attribute_group dump_nvme_attr_group = {
	.name  = IPL_NVME_STR,
	.attrs = dump_nvme_attrs,
};

/* ECKD dump device attributes */
DEFINE_IPL_CCW_ATTR_RW(dump_eckd, device, dump_block_eckd->eckd);
DEFINE_IPL_ATTR_RW(dump_eckd, bootprog, "%lld\n", "%llx\n",
		   dump_block_eckd->eckd.bootprog);

IPL_ATTR_BR_CHR_SHOW_FN(dump, dump_block_eckd->eckd);
IPL_ATTR_BR_CHR_STORE_FN(dump, dump_block_eckd->eckd);

static struct kobj_attribute sys_dump_eckd_br_chr_attr =
	__ATTR(br_chr, 0644, eckd_dump_br_chr_show, eckd_dump_br_chr_store);

static struct attribute *dump_eckd_attrs[] = {
	&sys_dump_eckd_device_attr.attr,
	&sys_dump_eckd_bootprog_attr.attr,
	&sys_dump_eckd_br_chr_attr.attr,
	NULL,
};

static struct attribute_group dump_eckd_attr_group = {
	.name  = IPL_ECKD_STR,
	.attrs = dump_eckd_attrs,
};

/* CCW dump device attributes */
DEFINE_IPL_CCW_ATTR_RW(dump_ccw, device, dump_block_ccw->ccw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *dump_ccw_attrs[] = {
	&sys_dump_ccw_device_attr.attr,
	NULL,
};

static struct attribute_group dump_ccw_attr_group = {
	.name  = IPL_CCW_STR,
	.attrs = dump_ccw_attrs,
};

/* dump type */

static int dump_set_type(enum dump_type type)
{
	if (!(dump_capabilities & type))
		return -EINVAL;
<<<<<<< HEAD
	switch (type) {
	case DUMP_TYPE_CCW:
		if (diag308_set_works)
			dump_method = DUMP_METHOD_CCW_DIAG;
		else if (MACHINE_IS_VM)
			dump_method = DUMP_METHOD_CCW_VM;
		else
			dump_method = DUMP_METHOD_CCW_CIO;
		break;
	case DUMP_TYPE_FCP:
		dump_method = DUMP_METHOD_FCP_DIAG;
		break;
	default:
		dump_method = DUMP_METHOD_NONE;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_type = type;
	return 0;
}

static ssize_t dump_type_show(struct kobject *kobj,
			      struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", dump_type_str(dump_type));
}

static ssize_t dump_type_store(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       const char *buf, size_t len)
{
	int rc = -EINVAL;

	if (strncmp(buf, DUMP_NONE_STR, strlen(DUMP_NONE_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_NONE);
	else if (strncmp(buf, DUMP_CCW_STR, strlen(DUMP_CCW_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_CCW);
<<<<<<< HEAD
	else if (strncmp(buf, DUMP_FCP_STR, strlen(DUMP_FCP_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_FCP);
=======
	else if (strncmp(buf, DUMP_ECKD_STR, strlen(DUMP_ECKD_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_ECKD);
	else if (strncmp(buf, DUMP_FCP_STR, strlen(DUMP_FCP_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_FCP);
	else if (strncmp(buf, DUMP_NVME_STR, strlen(DUMP_NVME_STR)) == 0)
		rc = dump_set_type(DUMP_TYPE_NVME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (rc != 0) ? rc : len;
}

static struct kobj_attribute dump_type_attr =
	__ATTR(dump_type, 0644, dump_type_show, dump_type_store);

static struct kset *dump_kset;

<<<<<<< HEAD
static void __dump_run(void *unused)
{
	struct ccw_dev_id devid;
	static char buf[100];

	switch (dump_method) {
	case DUMP_METHOD_CCW_CIO:
		devid.devno = dump_block_ccw->ipl_info.ccw.devno;
		devid.ssid  = 0;
		reipl_ccw_dev(&devid);
		break;
	case DUMP_METHOD_CCW_VM:
		sprintf(buf, "STORE STATUS");
		__cpcmd(buf, NULL, 0, NULL);
		sprintf(buf, "IPL %X", dump_block_ccw->ipl_info.ccw.devno);
		__cpcmd(buf, NULL, 0, NULL);
		break;
	case DUMP_METHOD_CCW_DIAG:
		diag308(DIAG308_SET, dump_block_ccw);
		diag308(DIAG308_DUMP, NULL);
		break;
	case DUMP_METHOD_FCP_DIAG:
		diag308(DIAG308_SET, dump_block_fcp);
		diag308(DIAG308_DUMP, NULL);
=======
static void diag308_dump(void *dump_block)
{
	diag308(DIAG308_SET, dump_block);
	while (1) {
		if (diag308(DIAG308_LOAD_NORMAL_DUMP, NULL) != 0x302)
			break;
		udelay(USEC_PER_SEC);
	}
}

static void __dump_run(void *unused)
{
	switch (dump_type) {
	case DUMP_TYPE_CCW:
		diag308_dump(dump_block_ccw);
		break;
	case DUMP_TYPE_ECKD:
		diag308_dump(dump_block_eckd);
		break;
	case DUMP_TYPE_FCP:
		diag308_dump(dump_block_fcp);
		break;
	case DUMP_TYPE_NVME:
		diag308_dump(dump_block_nvme);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		break;
	}
}

static void dump_run(struct shutdown_trigger *trigger)
{
<<<<<<< HEAD
	if (dump_method == DUMP_METHOD_NONE)
=======
	if (dump_type == DUMP_TYPE_NONE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	smp_send_stop();
	smp_call_ipl_cpu(__dump_run, NULL);
}

static int __init dump_ccw_init(void)
{
	int rc;

	dump_block_ccw = (void *) get_zeroed_page(GFP_KERNEL);
	if (!dump_block_ccw)
		return -ENOMEM;
	rc = sysfs_create_group(&dump_kset->kobj, &dump_ccw_attr_group);
	if (rc) {
		free_page((unsigned long)dump_block_ccw);
		return rc;
	}
<<<<<<< HEAD
	dump_block_ccw->hdr.len = IPL_PARM_BLK_CCW_LEN;
	dump_block_ccw->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_ccw->hdr.blk0_len = IPL_PARM_BLK0_CCW_LEN;
	dump_block_ccw->hdr.pbt = DIAG308_IPL_TYPE_CCW;
=======
	dump_block_ccw->hdr.len = IPL_BP_CCW_LEN;
	dump_block_ccw->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_ccw->ccw.len = IPL_BP0_CCW_LEN;
	dump_block_ccw->ccw.pbt = IPL_PBT_CCW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_capabilities |= DUMP_TYPE_CCW;
	return 0;
}

static int __init dump_fcp_init(void)
{
	int rc;

	if (!sclp_ipl_info.has_dump)
		return 0; /* LDIPL DUMP is not installed */
<<<<<<< HEAD
	if (!diag308_set_works)
		return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_block_fcp = (void *) get_zeroed_page(GFP_KERNEL);
	if (!dump_block_fcp)
		return -ENOMEM;
	rc = sysfs_create_group(&dump_kset->kobj, &dump_fcp_attr_group);
	if (rc) {
		free_page((unsigned long)dump_block_fcp);
		return rc;
	}
<<<<<<< HEAD
	dump_block_fcp->hdr.len = IPL_PARM_BLK_FCP_LEN;
	dump_block_fcp->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_fcp->hdr.blk0_len = IPL_PARM_BLK0_FCP_LEN;
	dump_block_fcp->hdr.pbt = DIAG308_IPL_TYPE_FCP;
	dump_block_fcp->ipl_info.fcp.opt = DIAG308_IPL_OPT_DUMP;
=======
	dump_block_fcp->hdr.len = IPL_BP_FCP_LEN;
	dump_block_fcp->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_fcp->fcp.len = IPL_BP0_FCP_LEN;
	dump_block_fcp->fcp.pbt = IPL_PBT_FCP;
	dump_block_fcp->fcp.opt = IPL_PB0_FCP_OPT_DUMP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_capabilities |= DUMP_TYPE_FCP;
	return 0;
}

<<<<<<< HEAD
=======
static int __init dump_nvme_init(void)
{
	int rc;

	if (!sclp_ipl_info.has_dump)
		return 0; /* LDIPL DUMP is not installed */
	dump_block_nvme = (void *) get_zeroed_page(GFP_KERNEL);
	if (!dump_block_nvme)
		return -ENOMEM;
	rc = sysfs_create_group(&dump_kset->kobj, &dump_nvme_attr_group);
	if (rc) {
		free_page((unsigned long)dump_block_nvme);
		return rc;
	}
	dump_block_nvme->hdr.len = IPL_BP_NVME_LEN;
	dump_block_nvme->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_nvme->fcp.len = IPL_BP0_NVME_LEN;
	dump_block_nvme->fcp.pbt = IPL_PBT_NVME;
	dump_block_nvme->fcp.opt = IPL_PB0_NVME_OPT_DUMP;
	dump_capabilities |= DUMP_TYPE_NVME;
	return 0;
}

static int __init dump_eckd_init(void)
{
	int rc;

	if (!sclp_ipl_info.has_dump || !sclp.has_sipl_eckd)
		return 0; /* LDIPL DUMP is not installed */
	dump_block_eckd = (void *)get_zeroed_page(GFP_KERNEL);
	if (!dump_block_eckd)
		return -ENOMEM;
	rc = sysfs_create_group(&dump_kset->kobj, &dump_eckd_attr_group);
	if (rc) {
		free_page((unsigned long)dump_block_eckd);
		return rc;
	}
	dump_block_eckd->hdr.len = IPL_BP_ECKD_LEN;
	dump_block_eckd->hdr.version = IPL_PARM_BLOCK_VERSION;
	dump_block_eckd->eckd.len = IPL_BP0_ECKD_LEN;
	dump_block_eckd->eckd.pbt = IPL_PBT_ECKD;
	dump_block_eckd->eckd.opt = IPL_PB0_ECKD_OPT_DUMP;
	dump_capabilities |= DUMP_TYPE_ECKD;
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init dump_init(void)
{
	int rc;

	dump_kset = kset_create_and_add("dump", NULL, firmware_kobj);
	if (!dump_kset)
		return -ENOMEM;
	rc = sysfs_create_file(&dump_kset->kobj, &dump_type_attr.attr);
	if (rc) {
		kset_unregister(dump_kset);
		return rc;
	}
	rc = dump_ccw_init();
	if (rc)
		return rc;
<<<<<<< HEAD
	rc = dump_fcp_init();
	if (rc)
		return rc;
=======
	rc = dump_eckd_init();
	if (rc)
		return rc;
	rc = dump_fcp_init();
	if (rc)
		return rc;
	rc = dump_nvme_init();
	if (rc)
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_set_type(DUMP_TYPE_NONE);
	return 0;
}

static struct shutdown_action __refdata dump_action = {
	.name	= SHUTDOWN_ACTION_DUMP_STR,
	.fn	= dump_run,
	.init	= dump_init,
};

static void dump_reipl_run(struct shutdown_trigger *trigger)
{
<<<<<<< HEAD
	u32 csum;

	csum = csum_partial(reipl_block_actual, reipl_block_actual->hdr.len, 0);
	copy_to_absolute_zero(&S390_lowcore.ipib_checksum, &csum, sizeof(csum));
	copy_to_absolute_zero(&S390_lowcore.ipib, &reipl_block_actual,
			      sizeof(reipl_block_actual));
	dump_run(trigger);
}

static int __init dump_reipl_init(void)
{
	if (!diag308_set_works)
		return -EOPNOTSUPP;
	else
		return 0;
}

static struct shutdown_action __refdata dump_reipl_action = {
	.name	= SHUTDOWN_ACTION_DUMP_REIPL_STR,
	.fn	= dump_reipl_run,
	.init	= dump_reipl_init,
=======
	struct lowcore *abs_lc;
	unsigned int csum;

	/*
	 * Set REIPL_CLEAR flag in os_info flags entry indicating
	 * 'clear' sysfs attribute has been set on the panicked system
	 * for specified reipl type.
	 * Always set for IPL_TYPE_NSS and IPL_TYPE_UNKNOWN.
	 */
	if ((reipl_type == IPL_TYPE_CCW && reipl_ccw_clear) ||
	    (reipl_type == IPL_TYPE_ECKD && reipl_eckd_clear) ||
	    (reipl_type == IPL_TYPE_FCP && reipl_fcp_clear) ||
	    (reipl_type == IPL_TYPE_NVME && reipl_nvme_clear) ||
	    reipl_type == IPL_TYPE_NSS ||
	    reipl_type == IPL_TYPE_UNKNOWN)
		os_info_flags |= OS_INFO_FLAG_REIPL_CLEAR;
	os_info_entry_add(OS_INFO_FLAGS_ENTRY, &os_info_flags, sizeof(os_info_flags));
	csum = (__force unsigned int)cksm(reipl_block_actual, reipl_block_actual->hdr.len, 0);
	abs_lc = get_abs_lowcore();
	abs_lc->ipib = __pa(reipl_block_actual);
	abs_lc->ipib_checksum = csum;
	put_abs_lowcore(abs_lc);
	dump_run(trigger);
}

static struct shutdown_action __refdata dump_reipl_action = {
	.name	= SHUTDOWN_ACTION_DUMP_REIPL_STR,
	.fn	= dump_reipl_run,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * vmcmd shutdown action: Trigger vm command on shutdown.
 */

static char vmcmd_on_reboot[128];
static char vmcmd_on_panic[128];
static char vmcmd_on_halt[128];
static char vmcmd_on_poff[128];
static char vmcmd_on_restart[128];

DEFINE_IPL_ATTR_STR_RW(vmcmd, on_reboot, "%s\n", "%s\n", vmcmd_on_reboot);
DEFINE_IPL_ATTR_STR_RW(vmcmd, on_panic, "%s\n", "%s\n", vmcmd_on_panic);
DEFINE_IPL_ATTR_STR_RW(vmcmd, on_halt, "%s\n", "%s\n", vmcmd_on_halt);
DEFINE_IPL_ATTR_STR_RW(vmcmd, on_poff, "%s\n", "%s\n", vmcmd_on_poff);
DEFINE_IPL_ATTR_STR_RW(vmcmd, on_restart, "%s\n", "%s\n", vmcmd_on_restart);

static struct attribute *vmcmd_attrs[] = {
	&sys_vmcmd_on_reboot_attr.attr,
	&sys_vmcmd_on_panic_attr.attr,
	&sys_vmcmd_on_halt_attr.attr,
	&sys_vmcmd_on_poff_attr.attr,
	&sys_vmcmd_on_restart_attr.attr,
	NULL,
};

static struct attribute_group vmcmd_attr_group = {
	.attrs = vmcmd_attrs,
};

static struct kset *vmcmd_kset;

static void vmcmd_run(struct shutdown_trigger *trigger)
{
<<<<<<< HEAD
	char *cmd, *next_cmd;
=======
	char *cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (strcmp(trigger->name, ON_REIPL_STR) == 0)
		cmd = vmcmd_on_reboot;
	else if (strcmp(trigger->name, ON_PANIC_STR) == 0)
		cmd = vmcmd_on_panic;
	else if (strcmp(trigger->name, ON_HALT_STR) == 0)
		cmd = vmcmd_on_halt;
	else if (strcmp(trigger->name, ON_POFF_STR) == 0)
		cmd = vmcmd_on_poff;
	else if (strcmp(trigger->name, ON_RESTART_STR) == 0)
		cmd = vmcmd_on_restart;
	else
		return;

	if (strlen(cmd) == 0)
		return;
<<<<<<< HEAD
	do {
		next_cmd = strchr(cmd, '\n');
		if (next_cmd) {
			next_cmd[0] = 0;
			next_cmd += 1;
		}
		__cpcmd(cmd, NULL, 0, NULL);
		cmd = next_cmd;
	} while (cmd != NULL);
=======
	__cpcmd(cmd, NULL, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int vmcmd_init(void)
{
	if (!MACHINE_IS_VM)
		return -EOPNOTSUPP;
	vmcmd_kset = kset_create_and_add("vmcmd", NULL, firmware_kobj);
	if (!vmcmd_kset)
		return -ENOMEM;
	return sysfs_create_group(&vmcmd_kset->kobj, &vmcmd_attr_group);
}

static struct shutdown_action vmcmd_action = {SHUTDOWN_ACTION_VMCMD_STR,
					      vmcmd_run, vmcmd_init};

/*
 * stop shutdown action: Stop Linux on shutdown.
 */

static void stop_run(struct shutdown_trigger *trigger)
{
	if (strcmp(trigger->name, ON_PANIC_STR) == 0 ||
	    strcmp(trigger->name, ON_RESTART_STR) == 0)
<<<<<<< HEAD
		disabled_wait((unsigned long) __builtin_return_address(0));
=======
		disabled_wait();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	smp_stop_cpu();
}

static struct shutdown_action stop_action = {SHUTDOWN_ACTION_STOP_STR,
					     stop_run, NULL};

/* action list */

static struct shutdown_action *shutdown_actions_list[] = {
	&ipl_action, &reipl_action, &dump_reipl_action, &dump_action,
	&vmcmd_action, &stop_action};
#define SHUTDOWN_ACTIONS_COUNT (sizeof(shutdown_actions_list) / sizeof(void *))

/*
 * Trigger section
 */

static struct kset *shutdown_actions_kset;

static int set_trigger(const char *buf, struct shutdown_trigger *trigger,
		       size_t len)
{
	int i;

	for (i = 0; i < SHUTDOWN_ACTIONS_COUNT; i++) {
		if (sysfs_streq(buf, shutdown_actions_list[i]->name)) {
			if (shutdown_actions_list[i]->init_rc) {
				return shutdown_actions_list[i]->init_rc;
			} else {
				trigger->action = shutdown_actions_list[i];
				return len;
			}
		}
	}
	return -EINVAL;
}

/* on reipl */

static struct shutdown_trigger on_reboot_trigger = {ON_REIPL_STR,
						    &reipl_action};

static ssize_t on_reboot_show(struct kobject *kobj,
			      struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", on_reboot_trigger.action->name);
}

static ssize_t on_reboot_store(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       const char *buf, size_t len)
{
	return set_trigger(buf, &on_reboot_trigger, len);
}
<<<<<<< HEAD

static struct kobj_attribute on_reboot_attr =
	__ATTR(on_reboot, 0644, on_reboot_show, on_reboot_store);
=======
static struct kobj_attribute on_reboot_attr = __ATTR_RW(on_reboot);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void do_machine_restart(char *__unused)
{
	smp_send_stop();
	on_reboot_trigger.action->fn(&on_reboot_trigger);
	reipl_run(NULL);
}
void (*_machine_restart)(char *command) = do_machine_restart;

/* on panic */

static struct shutdown_trigger on_panic_trigger = {ON_PANIC_STR, &stop_action};

static ssize_t on_panic_show(struct kobject *kobj,
			     struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", on_panic_trigger.action->name);
}

static ssize_t on_panic_store(struct kobject *kobj,
			      struct kobj_attribute *attr,
			      const char *buf, size_t len)
{
	return set_trigger(buf, &on_panic_trigger, len);
}
<<<<<<< HEAD

static struct kobj_attribute on_panic_attr =
	__ATTR(on_panic, 0644, on_panic_show, on_panic_store);
=======
static struct kobj_attribute on_panic_attr = __ATTR_RW(on_panic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void do_panic(void)
{
	lgr_info_log();
	on_panic_trigger.action->fn(&on_panic_trigger);
	stop_run(&on_panic_trigger);
}

/* on restart */

static struct shutdown_trigger on_restart_trigger = {ON_RESTART_STR,
	&stop_action};

static ssize_t on_restart_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", on_restart_trigger.action->name);
}

static ssize_t on_restart_store(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t len)
{
	return set_trigger(buf, &on_restart_trigger, len);
}
<<<<<<< HEAD

static struct kobj_attribute on_restart_attr =
	__ATTR(on_restart, 0644, on_restart_show, on_restart_store);
=======
static struct kobj_attribute on_restart_attr = __ATTR_RW(on_restart);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __do_restart(void *ignore)
{
	smp_send_stop();
#ifdef CONFIG_CRASH_DUMP
	crash_kexec(NULL);
#endif
	on_restart_trigger.action->fn(&on_restart_trigger);
	stop_run(&on_restart_trigger);
}

<<<<<<< HEAD
void do_restart(void)
=======
void do_restart(void *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	tracing_off();
	debug_locks_off();
	lgr_info_log();
<<<<<<< HEAD
	smp_call_online_cpu(__do_restart, NULL);
=======
	smp_call_online_cpu(__do_restart, arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* on halt */

static struct shutdown_trigger on_halt_trigger = {ON_HALT_STR, &stop_action};

static ssize_t on_halt_show(struct kobject *kobj,
			    struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", on_halt_trigger.action->name);
}

static ssize_t on_halt_store(struct kobject *kobj,
			     struct kobj_attribute *attr,
			     const char *buf, size_t len)
{
	return set_trigger(buf, &on_halt_trigger, len);
}
<<<<<<< HEAD

static struct kobj_attribute on_halt_attr =
	__ATTR(on_halt, 0644, on_halt_show, on_halt_store);

=======
static struct kobj_attribute on_halt_attr = __ATTR_RW(on_halt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void do_machine_halt(void)
{
	smp_send_stop();
	on_halt_trigger.action->fn(&on_halt_trigger);
	stop_run(&on_halt_trigger);
}
void (*_machine_halt)(void) = do_machine_halt;

/* on power off */

static struct shutdown_trigger on_poff_trigger = {ON_POFF_STR, &stop_action};

static ssize_t on_poff_show(struct kobject *kobj,
			    struct kobj_attribute *attr, char *page)
{
	return sprintf(page, "%s\n", on_poff_trigger.action->name);
}

static ssize_t on_poff_store(struct kobject *kobj,
			     struct kobj_attribute *attr,
			     const char *buf, size_t len)
{
	return set_trigger(buf, &on_poff_trigger, len);
}
<<<<<<< HEAD

static struct kobj_attribute on_poff_attr =
	__ATTR(on_poff, 0644, on_poff_show, on_poff_store);

=======
static struct kobj_attribute on_poff_attr = __ATTR_RW(on_poff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void do_machine_power_off(void)
{
	smp_send_stop();
	on_poff_trigger.action->fn(&on_poff_trigger);
	stop_run(&on_poff_trigger);
}
void (*_machine_power_off)(void) = do_machine_power_off;

<<<<<<< HEAD
=======
static struct attribute *shutdown_action_attrs[] = {
	&on_restart_attr.attr,
	&on_reboot_attr.attr,
	&on_panic_attr.attr,
	&on_halt_attr.attr,
	&on_poff_attr.attr,
	NULL,
};

static struct attribute_group shutdown_action_attr_group = {
	.attrs = shutdown_action_attrs,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __init shutdown_triggers_init(void)
{
	shutdown_actions_kset = kset_create_and_add("shutdown_actions", NULL,
						    firmware_kobj);
	if (!shutdown_actions_kset)
		goto fail;
<<<<<<< HEAD
	if (sysfs_create_file(&shutdown_actions_kset->kobj,
			      &on_reboot_attr.attr))
		goto fail;
	if (sysfs_create_file(&shutdown_actions_kset->kobj,
			      &on_panic_attr.attr))
		goto fail;
	if (sysfs_create_file(&shutdown_actions_kset->kobj,
			      &on_halt_attr.attr))
		goto fail;
	if (sysfs_create_file(&shutdown_actions_kset->kobj,
			      &on_poff_attr.attr))
		goto fail;
	if (sysfs_create_file(&shutdown_actions_kset->kobj,
			      &on_restart_attr.attr))
=======
	if (sysfs_create_group(&shutdown_actions_kset->kobj,
			       &shutdown_action_attr_group))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	return;
fail:
	panic("shutdown_triggers_init failed\n");
}

static void __init shutdown_actions_init(void)
{
	int i;

	for (i = 0; i < SHUTDOWN_ACTIONS_COUNT; i++) {
		if (!shutdown_actions_list[i]->init)
			continue;
		shutdown_actions_list[i]->init_rc =
			shutdown_actions_list[i]->init();
	}
}

static int __init s390_ipl_init(void)
{
<<<<<<< HEAD
	sclp_get_ipl_info(&sclp_ipl_info);
=======
	char str[8] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40};

	sclp_early_get_ipl_info(&sclp_ipl_info);
	/*
	 * Fix loadparm: There are systems where the (SCSI) LOADPARM
	 * returned by read SCP info is invalid (contains EBCDIC blanks)
	 * when the system has been booted via diag308. In that case we use
	 * the value from diag308, if available.
	 *
	 * There are also systems where diag308 store does not work in
	 * case the system is booted from HMC. Fortunately in this case
	 * READ SCP info provides the correct value.
	 */
	if (memcmp(sclp_ipl_info.loadparm, str, sizeof(str)) == 0 && ipl_block_valid)
		memcpy(sclp_ipl_info.loadparm, ipl_block.ccw.loadparm, LOADPARM_LEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	shutdown_actions_init();
	shutdown_triggers_init();
	return 0;
}

__initcall(s390_ipl_init);

static void __init strncpy_skip_quote(char *dst, char *src, int n)
{
	int sx, dx;

	dx = 0;
	for (sx = 0; src[sx] != 0; sx++) {
		if (src[sx] == '"')
			continue;
		dst[dx++] = src[sx];
		if (dx >= n)
			break;
	}
}

static int __init vmcmd_on_reboot_setup(char *str)
{
	if (!MACHINE_IS_VM)
		return 1;
	strncpy_skip_quote(vmcmd_on_reboot, str, 127);
	vmcmd_on_reboot[127] = 0;
	on_reboot_trigger.action = &vmcmd_action;
	return 1;
}
__setup("vmreboot=", vmcmd_on_reboot_setup);

static int __init vmcmd_on_panic_setup(char *str)
{
	if (!MACHINE_IS_VM)
		return 1;
	strncpy_skip_quote(vmcmd_on_panic, str, 127);
	vmcmd_on_panic[127] = 0;
	on_panic_trigger.action = &vmcmd_action;
	return 1;
}
__setup("vmpanic=", vmcmd_on_panic_setup);

static int __init vmcmd_on_halt_setup(char *str)
{
	if (!MACHINE_IS_VM)
		return 1;
	strncpy_skip_quote(vmcmd_on_halt, str, 127);
	vmcmd_on_halt[127] = 0;
	on_halt_trigger.action = &vmcmd_action;
	return 1;
}
__setup("vmhalt=", vmcmd_on_halt_setup);

static int __init vmcmd_on_poff_setup(char *str)
{
	if (!MACHINE_IS_VM)
		return 1;
	strncpy_skip_quote(vmcmd_on_poff, str, 127);
	vmcmd_on_poff[127] = 0;
	on_poff_trigger.action = &vmcmd_action;
	return 1;
}
__setup("vmpoff=", vmcmd_on_poff_setup);

static int on_panic_notify(struct notifier_block *self,
			   unsigned long event, void *data)
{
	do_panic();
	return NOTIFY_OK;
}

static struct notifier_block on_panic_nb = {
	.notifier_call = on_panic_notify,
	.priority = INT_MIN,
};

void __init setup_ipl(void)
{
<<<<<<< HEAD
	ipl_info.type = get_ipl_type();
	switch (ipl_info.type) {
	case IPL_TYPE_CCW:
		ipl_info.data.ccw.dev_id.devno = ipl_devno;
		ipl_info.data.ccw.dev_id.ssid = 0;
		break;
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		ipl_info.data.fcp.dev_id.devno =
			IPL_PARMBLOCK_START->ipl_info.fcp.devno;
		ipl_info.data.fcp.dev_id.ssid = 0;
		ipl_info.data.fcp.wwpn = IPL_PARMBLOCK_START->ipl_info.fcp.wwpn;
		ipl_info.data.fcp.lun = IPL_PARMBLOCK_START->ipl_info.fcp.lun;
		break;
	case IPL_TYPE_NSS:
		strncpy(ipl_info.data.nss.name, kernel_nss_name,
			sizeof(ipl_info.data.nss.name));
		break;
=======
	BUILD_BUG_ON(sizeof(struct ipl_parameter_block) != PAGE_SIZE);

	ipl_info.type = get_ipl_type();
	switch (ipl_info.type) {
	case IPL_TYPE_CCW:
		ipl_info.data.ccw.dev_id.ssid = ipl_block.ccw.ssid;
		ipl_info.data.ccw.dev_id.devno = ipl_block.ccw.devno;
		break;
	case IPL_TYPE_ECKD:
	case IPL_TYPE_ECKD_DUMP:
		ipl_info.data.eckd.dev_id.ssid = ipl_block.eckd.ssid;
		ipl_info.data.eckd.dev_id.devno = ipl_block.eckd.devno;
		break;
	case IPL_TYPE_FCP:
	case IPL_TYPE_FCP_DUMP:
		ipl_info.data.fcp.dev_id.ssid = 0;
		ipl_info.data.fcp.dev_id.devno = ipl_block.fcp.devno;
		ipl_info.data.fcp.wwpn = ipl_block.fcp.wwpn;
		ipl_info.data.fcp.lun = ipl_block.fcp.lun;
		break;
	case IPL_TYPE_NVME:
	case IPL_TYPE_NVME_DUMP:
		ipl_info.data.nvme.fid = ipl_block.nvme.fid;
		ipl_info.data.nvme.nsid = ipl_block.nvme.nsid;
		break;
	case IPL_TYPE_NSS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPL_TYPE_UNKNOWN:
		/* We have no info to copy */
		break;
	}
	atomic_notifier_chain_register(&panic_notifier_list, &on_panic_nb);
}

<<<<<<< HEAD
void __init ipl_update_parameters(void)
{
	int rc;

	rc = diag308(DIAG308_STORE, &ipl_block);
	if ((rc == DIAG308_RC_OK) || (rc == DIAG308_RC_NOCONFIG))
		diag308_set_works = 1;
}

void __init ipl_save_parameters(void)
{
	struct cio_iplinfo iplinfo;
	void *src, *dst;

	if (cio_get_iplinfo(&iplinfo))
		return;

	ipl_devno = iplinfo.devno;
	ipl_flags |= IPL_DEVNO_VALID;
	if (!iplinfo.is_qdio)
		return;
	ipl_flags |= IPL_PARMBLOCK_VALID;
	src = (void *)(unsigned long)S390_lowcore.ipl_parmblock_ptr;
	dst = (void *)IPL_PARMBLOCK_ORIGIN;
	memmove(dst, src, PAGE_SIZE);
	S390_lowcore.ipl_parmblock_ptr = IPL_PARMBLOCK_ORIGIN;
}

static LIST_HEAD(rcall);
static DEFINE_MUTEX(rcall_mutex);

void register_reset_call(struct reset_call *reset)
{
	mutex_lock(&rcall_mutex);
	list_add(&reset->list, &rcall);
	mutex_unlock(&rcall_mutex);
}
EXPORT_SYMBOL_GPL(register_reset_call);

void unregister_reset_call(struct reset_call *reset)
{
	mutex_lock(&rcall_mutex);
	list_del(&reset->list);
	mutex_unlock(&rcall_mutex);
}
EXPORT_SYMBOL_GPL(unregister_reset_call);

static void do_reset_calls(void)
{
	struct reset_call *reset;

#ifdef CONFIG_64BIT
	if (diag308_set_works) {
		diag308_reset();
		return;
	}
#endif
	list_for_each_entry(reset, &rcall, list)
		reset->fn();
}

u32 dump_prefix_page;

void s390_reset_system(void (*func)(void *), void *data)
{
	struct _lowcore *lc;

	lc = (struct _lowcore *)(unsigned long) store_prefix();

	/* Stack for interrupt/machine check handler */
	lc->panic_stack = S390_lowcore.panic_stack;

	/* Save prefix page address for dump case */
	dump_prefix_page = (u32)(unsigned long) lc;

=======
void s390_reset_system(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Disable prefixing */
	set_prefix(0);

	/* Disable lowcore protection */
<<<<<<< HEAD
	__ctl_clear_bit(0,28);

	/* Set new machine check handler */
	S390_lowcore.mcck_new_psw.mask = psw_kernel_bits | PSW_MASK_DAT;
	S390_lowcore.mcck_new_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) s390_base_mcck_handler;

	/* Set new program check handler */
	S390_lowcore.program_new_psw.mask = psw_kernel_bits | PSW_MASK_DAT;
	S390_lowcore.program_new_psw.addr =
		PSW_ADDR_AMODE | (unsigned long) s390_base_pgm_handler;

	/* Store status at absolute zero */
	store_status();

	do_reset_calls();
	if (func)
		func(data);
}
=======
	local_ctl_clear_bit(0, CR0_LOW_ADDRESS_PROTECTION_BIT);
	diag_amode31_ops.diag308_reset();
}

#ifdef CONFIG_KEXEC_FILE

int ipl_report_add_component(struct ipl_report *report, struct kexec_buf *kbuf,
			     unsigned char flags, unsigned short cert)
{
	struct ipl_report_component *comp;

	comp = vzalloc(sizeof(*comp));
	if (!comp)
		return -ENOMEM;
	list_add_tail(&comp->list, &report->components);

	comp->entry.addr = kbuf->mem;
	comp->entry.len = kbuf->memsz;
	comp->entry.flags = flags;
	comp->entry.certificate_index = cert;

	report->size += sizeof(comp->entry);

	return 0;
}

int ipl_report_add_certificate(struct ipl_report *report, void *key,
			       unsigned long addr, unsigned long len)
{
	struct ipl_report_certificate *cert;

	cert = vzalloc(sizeof(*cert));
	if (!cert)
		return -ENOMEM;
	list_add_tail(&cert->list, &report->certificates);

	cert->entry.addr = addr;
	cert->entry.len = len;
	cert->key = key;

	report->size += sizeof(cert->entry);
	report->size += cert->entry.len;

	return 0;
}

struct ipl_report *ipl_report_init(struct ipl_parameter_block *ipib)
{
	struct ipl_report *report;

	report = vzalloc(sizeof(*report));
	if (!report)
		return ERR_PTR(-ENOMEM);

	report->ipib = ipib;
	INIT_LIST_HEAD(&report->components);
	INIT_LIST_HEAD(&report->certificates);

	report->size = ALIGN(ipib->hdr.len, 8);
	report->size += sizeof(struct ipl_rl_hdr);
	report->size += sizeof(struct ipl_rb_components);
	report->size += sizeof(struct ipl_rb_certificates);

	return report;
}

void *ipl_report_finish(struct ipl_report *report)
{
	struct ipl_report_certificate *cert;
	struct ipl_report_component *comp;
	struct ipl_rb_certificates *certs;
	struct ipl_parameter_block *ipib;
	struct ipl_rb_components *comps;
	struct ipl_rl_hdr *rl_hdr;
	void *buf, *ptr;

	buf = vzalloc(report->size);
	if (!buf)
		goto out;
	ptr = buf;

	memcpy(ptr, report->ipib, report->ipib->hdr.len);
	ipib = ptr;
	if (ipl_secure_flag)
		ipib->hdr.flags |= IPL_PL_FLAG_SIPL;
	ipib->hdr.flags |= IPL_PL_FLAG_IPLSR;
	ptr += report->ipib->hdr.len;
	ptr = PTR_ALIGN(ptr, 8);

	rl_hdr = ptr;
	ptr += sizeof(*rl_hdr);

	comps = ptr;
	comps->rbt = IPL_RBT_COMPONENTS;
	ptr += sizeof(*comps);
	list_for_each_entry(comp, &report->components, list) {
		memcpy(ptr, &comp->entry, sizeof(comp->entry));
		ptr += sizeof(comp->entry);
	}
	comps->len = ptr - (void *)comps;

	certs = ptr;
	certs->rbt = IPL_RBT_CERTIFICATES;
	ptr += sizeof(*certs);
	list_for_each_entry(cert, &report->certificates, list) {
		memcpy(ptr, &cert->entry, sizeof(cert->entry));
		ptr += sizeof(cert->entry);
	}
	certs->len = ptr - (void *)certs;
	rl_hdr->len = ptr - (void *)rl_hdr;

	list_for_each_entry(cert, &report->certificates, list) {
		memcpy(ptr, cert->key, cert->entry.len);
		ptr += cert->entry.len;
	}

	BUG_ON(ptr > buf + report->size);
out:
	return buf;
}

int ipl_report_free(struct ipl_report *report)
{
	struct ipl_report_component *comp, *ncomp;
	struct ipl_report_certificate *cert, *ncert;

	list_for_each_entry_safe(comp, ncomp, &report->components, list)
		vfree(comp);

	list_for_each_entry_safe(cert, ncert, &report->certificates, list)
		vfree(cert);

	vfree(report);

	return 0;
}

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
