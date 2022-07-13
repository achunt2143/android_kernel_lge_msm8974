<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Miscellaneous Mac68K-specific stuff
 */

#include <linux/types.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/miscdevice.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/mm.h>

#include <linux/adb.h>
#include <linux/cuda.h>
#include <linux/pmu.h>

<<<<<<< HEAD
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/rtc.h>
#include <asm/segment.h>
=======
#include <linux/uaccess.h>
#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/setup.h>
#include <asm/macintosh.h>
#include <asm/mac_via.h>
#include <asm/mac_oss.h>

<<<<<<< HEAD
#define BOOTINFO_COMPAT_1_0
#include <asm/bootinfo.h>
#include <asm/machdep.h>

/* Offset between Unix time (1970-based) and Mac time (1904-based) */
=======
#include <asm/machdep.h>

#include "mac.h"

/*
 * Offset between Unix time (1970-based) and Mac time (1904-based). Cuda and PMU
 * times wrap in 2040. If we need to handle later times, the read_time functions
 * need to be changed to interpret wrapped times as post-2040.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RTC_OFFSET 2082844800

static void (*rom_reset)(void);

<<<<<<< HEAD
#ifdef CONFIG_ADB_CUDA
static long cuda_read_time(void)
{
	struct adb_request req;
	long time;

	if (cuda_request(&req, NULL, 2, CUDA_PACKET, CUDA_GET_TIME) < 0)
		return 0;
	while (!req.complete)
		cuda_poll();

	time = (req.reply[3] << 24) | (req.reply[4] << 16)
		| (req.reply[5] << 8) | req.reply[6];
	return time - RTC_OFFSET;
}

static void cuda_write_time(long data)
{
	struct adb_request req;
	data += RTC_OFFSET;
	if (cuda_request(&req, NULL, 6, CUDA_PACKET, CUDA_SET_TIME,
			(data >> 24) & 0xFF, (data >> 16) & 0xFF,
			(data >> 8) & 0xFF, data & 0xFF) < 0)
		return;
	while (!req.complete)
		cuda_poll();
}

static __u8 cuda_read_pram(int offset)
{
	struct adb_request req;
	if (cuda_request(&req, NULL, 4, CUDA_PACKET, CUDA_GET_PRAM,
			(offset >> 8) & 0xFF, offset & 0xFF) < 0)
=======
#if IS_ENABLED(CONFIG_NVRAM)
#ifdef CONFIG_ADB_CUDA
static unsigned char cuda_pram_read_byte(int offset)
{
	struct adb_request req;

	if (cuda_request(&req, NULL, 4, CUDA_PACKET, CUDA_GET_PRAM,
			 (offset >> 8) & 0xFF, offset & 0xFF) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	while (!req.complete)
		cuda_poll();
	return req.reply[3];
}

<<<<<<< HEAD
static void cuda_write_pram(int offset, __u8 data)
{
	struct adb_request req;
	if (cuda_request(&req, NULL, 5, CUDA_PACKET, CUDA_SET_PRAM,
			(offset >> 8) & 0xFF, offset & 0xFF, data) < 0)
=======
static void cuda_pram_write_byte(unsigned char data, int offset)
{
	struct adb_request req;

	if (cuda_request(&req, NULL, 5, CUDA_PACKET, CUDA_SET_PRAM,
			 (offset >> 8) & 0xFF, offset & 0xFF, data) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	while (!req.complete)
		cuda_poll();
}
<<<<<<< HEAD
#else
#define cuda_read_time() 0
#define cuda_write_time(n)
#define cuda_read_pram NULL
#define cuda_write_pram NULL
#endif

#ifdef CONFIG_ADB_PMU68K
static long pmu_read_time(void)
{
	struct adb_request req;
	long time;

	if (pmu_request(&req, NULL, 1, PMU_READ_RTC) < 0)
		return 0;
	while (!req.complete)
		pmu_poll();

	time = (req.reply[1] << 24) | (req.reply[2] << 16)
		| (req.reply[3] << 8) | req.reply[4];
	return time - RTC_OFFSET;
}

static void pmu_write_time(long data)
{
	struct adb_request req;
	data += RTC_OFFSET;
	if (pmu_request(&req, NULL, 5, PMU_SET_RTC,
			(data >> 24) & 0xFF, (data >> 16) & 0xFF,
			(data >> 8) & 0xFF, data & 0xFF) < 0)
		return;
	while (!req.complete)
		pmu_poll();
}

static __u8 pmu_read_pram(int offset)
{
	struct adb_request req;
	if (pmu_request(&req, NULL, 3, PMU_READ_NVRAM,
			(offset >> 8) & 0xFF, offset & 0xFF) < 0)
		return 0;
	while (!req.complete)
		pmu_poll();
	return req.reply[3];
}

static void pmu_write_pram(int offset, __u8 data)
{
	struct adb_request req;
	if (pmu_request(&req, NULL, 4, PMU_WRITE_NVRAM,
			(offset >> 8) & 0xFF, offset & 0xFF, data) < 0)
		return;
	while (!req.complete)
		pmu_poll();
}
#else
#define pmu_read_time() 0
#define pmu_write_time(n)
#define pmu_read_pram NULL
#define pmu_write_pram NULL
#endif

#if 0 /* def CONFIG_ADB_MACIISI */
extern int maciisi_request(struct adb_request *req,
			void (*done)(struct adb_request *), int nbytes, ...);

static long maciisi_read_time(void)
{
	struct adb_request req;
	long time;

	if (maciisi_request(&req, NULL, 2, CUDA_PACKET, CUDA_GET_TIME))
		return 0;

	time = (req.reply[3] << 24) | (req.reply[4] << 16)
		| (req.reply[5] << 8) | req.reply[6];
	return time - RTC_OFFSET;
}

static void maciisi_write_time(long data)
{
	struct adb_request req;
	data += RTC_OFFSET;
	maciisi_request(&req, NULL, 6, CUDA_PACKET, CUDA_SET_TIME,
			(data >> 24) & 0xFF, (data >> 16) & 0xFF,
			(data >> 8) & 0xFF, data & 0xFF);
}

static __u8 maciisi_read_pram(int offset)
{
	struct adb_request req;
	if (maciisi_request(&req, NULL, 4, CUDA_PACKET, CUDA_GET_PRAM,
			(offset >> 8) & 0xFF, offset & 0xFF))
		return 0;
	return req.reply[3];
}

static void maciisi_write_pram(int offset, __u8 data)
{
	struct adb_request req;
	maciisi_request(&req, NULL, 5, CUDA_PACKET, CUDA_SET_PRAM,
			(offset >> 8) & 0xFF, offset & 0xFF, data);
}
#else
#define maciisi_read_time() 0
#define maciisi_write_time(n)
#define maciisi_read_pram NULL
#define maciisi_write_pram NULL
#endif
=======
#endif /* CONFIG_ADB_CUDA */

#ifdef CONFIG_ADB_PMU
static unsigned char pmu_pram_read_byte(int offset)
{
	struct adb_request req;

	if (pmu_request(&req, NULL, 3, PMU_READ_XPRAM,
	                offset & 0xFF, 1) < 0)
		return 0;
	pmu_wait_complete(&req);

	return req.reply[0];
}

static void pmu_pram_write_byte(unsigned char data, int offset)
{
	struct adb_request req;

	if (pmu_request(&req, NULL, 4, PMU_WRITE_XPRAM,
	                offset & 0xFF, 1, data) < 0)
		return;
	pmu_wait_complete(&req);
}
#endif /* CONFIG_ADB_PMU */
#endif /* CONFIG_NVRAM */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * VIA PRAM/RTC access routines
 *
 * Must be called with interrupts disabled and
 * the RTC should be enabled.
 */

<<<<<<< HEAD
static __u8 via_pram_readbyte(void)
{
	int	i,reg;
	__u8	data;
=======
static __u8 via_rtc_recv(void)
{
	int i, reg;
	__u8 data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reg = via1[vBufB] & ~VIA1B_vRTCClk;

	/* Set the RTC data line to be an input. */

	via1[vDirB] &= ~VIA1B_vRTCData;

	/* The bits of the byte come out in MSB order */

	data = 0;
	for (i = 0 ; i < 8 ; i++) {
		via1[vBufB] = reg;
		via1[vBufB] = reg | VIA1B_vRTCClk;
		data = (data << 1) | (via1[vBufB] & VIA1B_vRTCData);
	}

	/* Return RTC data line to output state */

	via1[vDirB] |= VIA1B_vRTCData;

	return data;
}

<<<<<<< HEAD
static void via_pram_writebyte(__u8 data)
{
	int	i,reg,bit;

	reg = via1[vBufB] & ~(VIA1B_vRTCClk | VIA1B_vRTCData);

	/* The bits of the byte go in in MSB order */
=======
static void via_rtc_send(__u8 data)
{
	int i, reg, bit;

	reg = via1[vBufB] & ~(VIA1B_vRTCClk | VIA1B_vRTCData);

	/* The bits of the byte go into the RTC in MSB order */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0 ; i < 8 ; i++) {
		bit = data & 0x80? 1 : 0;
		data <<= 1;
		via1[vBufB] = reg | bit;
		via1[vBufB] = reg | bit | VIA1B_vRTCClk;
	}
}

/*
<<<<<<< HEAD
=======
 * These values can be found in Inside Macintosh vol. III ch. 2
 * which has a description of the RTC chip in the original Mac.
 */

#define RTC_FLG_READ            BIT(7)
#define RTC_FLG_WRITE_PROTECT   BIT(7)
#define RTC_CMD_READ(r)         (RTC_FLG_READ | (r << 2))
#define RTC_CMD_WRITE(r)        (r << 2)
#define RTC_REG_SECONDS_0       0
#define RTC_REG_SECONDS_1       1
#define RTC_REG_SECONDS_2       2
#define RTC_REG_SECONDS_3       3
#define RTC_REG_WRITE_PROTECT   13

/*
 * Inside Mac has no information about two-byte RTC commands but
 * the MAME/MESS source code has the essentials.
 */

#define RTC_REG_XPRAM           14
#define RTC_CMD_XPRAM_READ      (RTC_CMD_READ(RTC_REG_XPRAM) << 8)
#define RTC_CMD_XPRAM_WRITE     (RTC_CMD_WRITE(RTC_REG_XPRAM) << 8)
#define RTC_CMD_XPRAM_ARG(a)    (((a & 0xE0) << 3) | ((a & 0x1F) << 2))

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Execute a VIA PRAM/RTC command. For read commands
 * data should point to a one-byte buffer for the
 * resulting data. For write commands it should point
 * to the data byte to for the command.
 *
 * This function disables all interrupts while running.
 */

<<<<<<< HEAD
static void via_pram_command(int command, __u8 *data)
{
	unsigned long flags;
	int	is_read;

	local_irq_save(flags);

=======
static void via_rtc_command(int command, __u8 *data)
{
	unsigned long flags;
	int is_read;

	local_irq_save(flags);

	/* The least significant bits must be 0b01 according to Inside Mac */

	command = (command & ~3) | 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Enable the RTC and make sure the strobe line is high */

	via1[vBufB] = (via1[vBufB] | VIA1B_vRTCClk) & ~VIA1B_vRTCEnb;

	if (command & 0xFF00) {		/* extended (two-byte) command */
<<<<<<< HEAD
		via_pram_writebyte((command & 0xFF00) >> 8);
		via_pram_writebyte(command & 0xFF);
		is_read = command & 0x8000;
	} else {			/* one-byte command */
		via_pram_writebyte(command);
		is_read = command & 0x80;
	}
	if (is_read) {
		*data = via_pram_readbyte();
	} else {
		via_pram_writebyte(*data);
=======
		via_rtc_send((command & 0xFF00) >> 8);
		via_rtc_send(command & 0xFF);
		is_read = command & (RTC_FLG_READ << 8);
	} else {			/* one-byte command */
		via_rtc_send(command);
		is_read = command & RTC_FLG_READ;
	}
	if (is_read) {
		*data = via_rtc_recv();
	} else {
		via_rtc_send(*data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* All done, disable the RTC */

	via1[vBufB] |= VIA1B_vRTCEnb;

	local_irq_restore(flags);
}

<<<<<<< HEAD
static __u8 via_read_pram(int offset)
{
	return 0;
}

static void via_write_pram(int offset, __u8 data)
{
}
=======
#if IS_ENABLED(CONFIG_NVRAM)
static unsigned char via_pram_read_byte(int offset)
{
	unsigned char temp;

	via_rtc_command(RTC_CMD_XPRAM_READ | RTC_CMD_XPRAM_ARG(offset), &temp);

	return temp;
}

static void via_pram_write_byte(unsigned char data, int offset)
{
	unsigned char temp;

	temp = 0x55;
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_WRITE_PROTECT), &temp);

	temp = data;
	via_rtc_command(RTC_CMD_XPRAM_WRITE | RTC_CMD_XPRAM_ARG(offset), &temp);

	temp = 0x55 | RTC_FLG_WRITE_PROTECT;
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_WRITE_PROTECT), &temp);
}
#endif /* CONFIG_NVRAM */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Return the current time in seconds since January 1, 1904.
 *
 * This only works on machines with the VIA-based PRAM/RTC, which
 * is basically any machine with Mac II-style ADB.
 */

<<<<<<< HEAD
static long via_read_time(void)
{
	union {
		__u8 cdata[4];
		long idata;
	} result, last_result;
	int count = 1;

	via_pram_command(0x81, &last_result.cdata[3]);
	via_pram_command(0x85, &last_result.cdata[2]);
	via_pram_command(0x89, &last_result.cdata[1]);
	via_pram_command(0x8D, &last_result.cdata[0]);
=======
static time64_t via_read_time(void)
{
	union {
		__u8 cdata[4];
		__u32 idata;
	} result, last_result;
	int count = 1;

	via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_0), &last_result.cdata[3]);
	via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_1), &last_result.cdata[2]);
	via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_2), &last_result.cdata[1]);
	via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_3), &last_result.cdata[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The NetBSD guys say to loop until you get the same reading
	 * twice in a row.
	 */

	while (1) {
<<<<<<< HEAD
		via_pram_command(0x81, &result.cdata[3]);
		via_pram_command(0x85, &result.cdata[2]);
		via_pram_command(0x89, &result.cdata[1]);
		via_pram_command(0x8D, &result.cdata[0]);

		if (result.idata == last_result.idata)
			return result.idata - RTC_OFFSET;
=======
		via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_0),
		                &result.cdata[3]);
		via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_1),
		                &result.cdata[2]);
		via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_2),
		                &result.cdata[1]);
		via_rtc_command(RTC_CMD_READ(RTC_REG_SECONDS_3),
		                &result.cdata[0]);

		if (result.idata == last_result.idata)
			return (time64_t)result.idata - RTC_OFFSET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (++count > 10)
			break;

		last_result.idata = result.idata;
	}

<<<<<<< HEAD
	pr_err("via_read_time: failed to read a stable value; "
	       "got 0x%08lx then 0x%08lx\n",
	       last_result.idata, result.idata);
=======
	pr_err("%s: failed to read a stable value; got 0x%08x then 0x%08x\n",
	       __func__, last_result.idata, result.idata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * Set the current time to a number of seconds since January 1, 1904.
 *
 * This only works on machines with the VIA-based PRAM/RTC, which
 * is basically any machine with Mac II-style ADB.
 */

<<<<<<< HEAD
static void via_write_time(long time)
{
	union {
		__u8  cdata[4];
		long  idata;
	} data;
	__u8	temp;
=======
static void via_set_rtc_time(struct rtc_time *tm)
{
	union {
		__u8 cdata[4];
		__u32 idata;
	} data;
	__u8 temp;
	time64_t time;

	time = mktime64(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	                tm->tm_hour, tm->tm_min, tm->tm_sec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Clear the write protect bit */

	temp = 0x55;
<<<<<<< HEAD
	via_pram_command(0x35, &temp);

	data.idata = time + RTC_OFFSET;
	via_pram_command(0x01, &data.cdata[3]);
	via_pram_command(0x05, &data.cdata[2]);
	via_pram_command(0x09, &data.cdata[1]);
	via_pram_command(0x0D, &data.cdata[0]);

	/* Set the write protect bit */

	temp = 0xD5;
	via_pram_command(0x35, &temp);
=======
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_WRITE_PROTECT), &temp);

	data.idata = lower_32_bits(time + RTC_OFFSET);
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_SECONDS_0), &data.cdata[3]);
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_SECONDS_1), &data.cdata[2]);
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_SECONDS_2), &data.cdata[1]);
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_SECONDS_3), &data.cdata[0]);

	/* Set the write protect bit */

	temp = 0x55 | RTC_FLG_WRITE_PROTECT;
	via_rtc_command(RTC_CMD_WRITE(RTC_REG_WRITE_PROTECT), &temp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void via_shutdown(void)
{
	if (rbv_present) {
		via2[rBufB] &= ~0x04;
	} else {
		/* Direction of vDirB is output */
		via2[vDirB] |= 0x04;
		/* Send a value of 0 on that line */
		via2[vBufB] &= ~0x04;
		mdelay(1000);
	}
}

<<<<<<< HEAD
/*
 * FIXME: not sure how this is supposed to work exactly...
 */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void oss_shutdown(void)
{
	oss->rom_ctrl = OSS_POWEROFF;
}

#ifdef CONFIG_ADB_CUDA
<<<<<<< HEAD

static void cuda_restart(void)
{
	struct adb_request req;
=======
static void cuda_restart(void)
{
	struct adb_request req;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cuda_request(&req, NULL, 2, CUDA_PACKET, CUDA_RESET_SYSTEM) < 0)
		return;
	while (!req.complete)
		cuda_poll();
}

static void cuda_shutdown(void)
{
	struct adb_request req;
<<<<<<< HEAD
	if (cuda_request(&req, NULL, 2, CUDA_PACKET, CUDA_POWERDOWN) < 0)
		return;
	while (!req.complete)
		cuda_poll();
}

#endif /* CONFIG_ADB_CUDA */

#ifdef CONFIG_ADB_PMU68K

void pmu_restart(void)
{
	struct adb_request req;
	if (pmu_request(&req, NULL,
			2, PMU_SET_INTR_MASK, PMU_INT_ADB|PMU_INT_TICK) < 0)
		return;
	while (!req.complete)
		pmu_poll();
	if (pmu_request(&req, NULL, 1, PMU_RESET) < 0)
		return;
	while (!req.complete)
		pmu_poll();
}

void pmu_shutdown(void)
{
	struct adb_request req;
	if (pmu_request(&req, NULL,
			2, PMU_SET_INTR_MASK, PMU_INT_ADB|PMU_INT_TICK) < 0)
		return;
	while (!req.complete)
		pmu_poll();
	if (pmu_request(&req, NULL, 5, PMU_SHUTDOWN, 'M', 'A', 'T', 'T') < 0)
		return;
	while (!req.complete)
		pmu_poll();
}

#endif

=======

	if (cuda_request(&req, NULL, 2, CUDA_PACKET, CUDA_POWERDOWN) < 0)
		return;

	/* Avoid infinite polling loop when PSU is not under Cuda control */
	switch (macintosh_config->ident) {
	case MAC_MODEL_C660:
	case MAC_MODEL_Q605:
	case MAC_MODEL_Q605_ACC:
	case MAC_MODEL_P475:
	case MAC_MODEL_P475F:
		return;
	}

	while (!req.complete)
		cuda_poll();
}
#endif /* CONFIG_ADB_CUDA */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *-------------------------------------------------------------------
 * Below this point are the generic routines; they'll dispatch to the
 * correct routine for the hardware on which we're running.
 *-------------------------------------------------------------------
 */

<<<<<<< HEAD
void mac_pram_read(int offset, __u8 *buffer, int len)
{
	__u8 (*func)(int);
	int i;

	switch(macintosh_config->adb_type) {
	case MAC_ADB_IISI:
		func = maciisi_read_pram; break;
	case MAC_ADB_PB1:
	case MAC_ADB_PB2:
		func = pmu_read_pram; break;
	case MAC_ADB_CUDA:
		func = cuda_read_pram; break;
	default:
		func = via_read_pram;
	}
	if (!func)
		return;
	for (i = 0 ; i < len ; i++) {
		buffer[i] = (*func)(offset++);
	}
}

void mac_pram_write(int offset, __u8 *buffer, int len)
{
	void (*func)(int, __u8);
	int i;

	switch(macintosh_config->adb_type) {
	case MAC_ADB_IISI:
		func = maciisi_write_pram; break;
	case MAC_ADB_PB1:
	case MAC_ADB_PB2:
		func = pmu_write_pram; break;
	case MAC_ADB_CUDA:
		func = cuda_write_pram; break;
	default:
		func = via_write_pram;
	}
	if (!func)
		return;
	for (i = 0 ; i < len ; i++) {
		(*func)(offset++, buffer[i]);
	}
}

void mac_poweroff(void)
{
	/*
	 * MAC_ADB_IISI may need to be moved up here if it doesn't actually
	 * work using the ADB packet method.  --David Kilzer
	 */

=======
#if IS_ENABLED(CONFIG_NVRAM)
unsigned char mac_pram_read_byte(int addr)
{
	switch (macintosh_config->adb_type) {
	case MAC_ADB_IOP:
	case MAC_ADB_II:
	case MAC_ADB_PB1:
		return via_pram_read_byte(addr);
#ifdef CONFIG_ADB_CUDA
	case MAC_ADB_EGRET:
	case MAC_ADB_CUDA:
		return cuda_pram_read_byte(addr);
#endif
#ifdef CONFIG_ADB_PMU
	case MAC_ADB_PB2:
		return pmu_pram_read_byte(addr);
#endif
	default:
		return 0xFF;
	}
}

void mac_pram_write_byte(unsigned char val, int addr)
{
	switch (macintosh_config->adb_type) {
	case MAC_ADB_IOP:
	case MAC_ADB_II:
	case MAC_ADB_PB1:
		via_pram_write_byte(val, addr);
		break;
#ifdef CONFIG_ADB_CUDA
	case MAC_ADB_EGRET:
	case MAC_ADB_CUDA:
		cuda_pram_write_byte(val, addr);
		break;
#endif
#ifdef CONFIG_ADB_PMU
	case MAC_ADB_PB2:
		pmu_pram_write_byte(val, addr);
		break;
#endif
	default:
		break;
	}
}

ssize_t mac_pram_get_size(void)
{
	return 256;
}
#endif /* CONFIG_NVRAM */

void mac_poweroff(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (oss_present) {
		oss_shutdown();
	} else if (macintosh_config->adb_type == MAC_ADB_II) {
		via_shutdown();
#ifdef CONFIG_ADB_CUDA
<<<<<<< HEAD
	} else if (macintosh_config->adb_type == MAC_ADB_CUDA) {
		cuda_shutdown();
#endif
#ifdef CONFIG_ADB_PMU68K
	} else if (macintosh_config->adb_type == MAC_ADB_PB1
		|| macintosh_config->adb_type == MAC_ADB_PB2) {
		pmu_shutdown();
#endif
	}
	local_irq_enable();
	printk("It is now safe to turn off your Macintosh.\n");
=======
	} else if (macintosh_config->adb_type == MAC_ADB_EGRET ||
	           macintosh_config->adb_type == MAC_ADB_CUDA) {
		cuda_shutdown();
#endif
#ifdef CONFIG_ADB_PMU
	} else if (macintosh_config->adb_type == MAC_ADB_PB2) {
		pmu_shutdown();
#endif
	}

	pr_crit("It is now safe to turn off your Macintosh.\n");
	local_irq_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while(1);
}

void mac_reset(void)
{
<<<<<<< HEAD
	if (macintosh_config->adb_type == MAC_ADB_II) {
		unsigned long flags;

=======
	if (macintosh_config->adb_type == MAC_ADB_II &&
	    macintosh_config->ident != MAC_MODEL_SE30) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* need ROMBASE in booter */
		/* indeed, plus need to MAP THE ROM !! */

		if (mac_bi_data.rombase == 0)
			mac_bi_data.rombase = 0x40800000;

		/* works on some */
		rom_reset = (void *) (mac_bi_data.rombase + 0xa);

<<<<<<< HEAD
		if (macintosh_config->ident == MAC_MODEL_SE30) {
			/*
			 * MSch: Machines known to crash on ROM reset ...
			 */
		} else {
			local_irq_save(flags);

			rom_reset();

			local_irq_restore(flags);
		}
#ifdef CONFIG_ADB_CUDA
	} else if (macintosh_config->adb_type == MAC_ADB_CUDA) {
		cuda_restart();
#endif
#ifdef CONFIG_ADB_PMU68K
	} else if (macintosh_config->adb_type == MAC_ADB_PB1
		|| macintosh_config->adb_type == MAC_ADB_PB2) {
=======
		local_irq_disable();
		rom_reset();
#ifdef CONFIG_ADB_CUDA
	} else if (macintosh_config->adb_type == MAC_ADB_EGRET ||
	           macintosh_config->adb_type == MAC_ADB_CUDA) {
		cuda_restart();
#endif
#ifdef CONFIG_ADB_PMU
	} else if (macintosh_config->adb_type == MAC_ADB_PB2) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pmu_restart();
#endif
	} else if (CPU_IS_030) {

		/* 030-specific reset routine.  The idea is general, but the
		 * specific registers to reset are '030-specific.  Until I
		 * have a non-030 machine, I can't test anything else.
		 *  -- C. Scott Ananian <cananian@alumni.princeton.edu>
		 */

		unsigned long rombase = 0x40000000;

		/* make a 1-to-1 mapping, using the transparent tran. reg. */
		unsigned long virt = (unsigned long) mac_reset;
		unsigned long phys = virt_to_phys(mac_reset);
		unsigned long addr = (phys&0xFF000000)|0x8777;
		unsigned long offset = phys-virt;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		local_irq_disable(); /* lets not screw this up, ok? */
		__asm__ __volatile__(".chip 68030\n\t"
				     "pmove %0,%/tt0\n\t"
				     ".chip 68k"
				     : : "m" (addr));
		/* Now jump to physical address so we can disable MMU */
		__asm__ __volatile__(
<<<<<<< HEAD
                    ".chip 68030\n\t"
=======
		    ".chip 68030\n\t"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "lea %/pc@(1f),%/a0\n\t"
		    "addl %0,%/a0\n\t"/* fixup target address and stack ptr */
		    "addl %0,%/sp\n\t"
		    "pflusha\n\t"
		    "jmp %/a0@\n\t" /* jump into physical memory */
		    "0:.long 0\n\t" /* a constant zero. */
		    /* OK.  Now reset everything and jump to reset vector. */
		    "1:\n\t"
		    "lea %/pc@(0b),%/a0\n\t"
		    "pmove %/a0@, %/tc\n\t" /* disable mmu */
		    "pmove %/a0@, %/tt0\n\t" /* disable tt0 */
		    "pmove %/a0@, %/tt1\n\t" /* disable tt1 */
		    "movel #0, %/a0\n\t"
		    "movec %/a0, %/vbr\n\t" /* clear vector base register */
		    "movec %/a0, %/cacr\n\t" /* disable caches */
		    "movel #0x0808,%/a0\n\t"
		    "movec %/a0, %/cacr\n\t" /* flush i&d caches */
		    "movew #0x2700,%/sr\n\t" /* set up status register */
		    "movel %1@(0x0),%/a0\n\t"/* load interrupt stack pointer */
		    "movec %/a0, %/isp\n\t"
		    "movel %1@(0x4),%/a0\n\t" /* load reset vector */
		    "reset\n\t" /* reset external devices */
		    "jmp %/a0@\n\t" /* jump to the reset vector */
		    ".chip 68k"
		    : : "r" (offset), "a" (rombase) : "a0");
	}

	/* should never get here */
<<<<<<< HEAD
	local_irq_enable();
	printk ("Restart failed.  Please restart manually.\n");
=======
	pr_crit("Restart failed. Please restart manually.\n");
	local_irq_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while(1);
}

/*
 * This function translates seconds since 1970 into a proper date.
 *
 * Algorithm cribbed from glibc2.1, __offtime().
<<<<<<< HEAD
=======
 *
 * This is roughly same as rtc_time64_to_tm(), which we should probably
 * use here, but it's only available when CONFIG_RTC_LIB is enabled.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define SECS_PER_MINUTE (60)
#define SECS_PER_HOUR  (SECS_PER_MINUTE * 60)
#define SECS_PER_DAY   (SECS_PER_HOUR * 24)

<<<<<<< HEAD
static void unmktime(unsigned long time, long offset,
=======
static void unmktime(time64_t time, long offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     int *yearp, int *monp, int *dayp,
		     int *hourp, int *minp, int *secp)
{
        /* How many days come before each month (0-12).  */
	static const unsigned short int __mon_yday[2][13] =
	{
		/* Normal years.  */
		{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
		/* Leap years.  */
		{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
	};
<<<<<<< HEAD
	long int days, rem, y, wday, yday;
	const unsigned short int *ip;

	days = time / SECS_PER_DAY;
	rem = time % SECS_PER_DAY;
=======
	int days, rem, y, wday;
	const unsigned short int *ip;

	days = div_u64_rem(time, SECS_PER_DAY, &rem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rem += offset;
	while (rem < 0) {
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY) {
		rem -= SECS_PER_DAY;
		++days;
	}
	*hourp = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	*minp = rem / SECS_PER_MINUTE;
	*secp = rem % SECS_PER_MINUTE;
	/* January 1, 1970 was a Thursday. */
	wday = (4 + days) % 7; /* Day in the week. Not currently used */
	if (wday < 0) wday += 7;
	y = 1970;

#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))
#define __isleap(year)	\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

	while (days < 0 || days >= (__isleap (y) ? 366 : 365))
	{
		/* Guess a corrected year, assuming 365 days per year.  */
		long int yg = y + days / 365 - (days % 365 < 0);

		/* Adjust DAYS and Y to match the guessed year.  */
<<<<<<< HEAD
		days -= ((yg - y) * 365
			 + LEAPS_THRU_END_OF (yg - 1)
			 - LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}
	*yearp = y - 1900;
	yday = days; /* day in the year.  Not currently used. */
=======
		days -= (yg - y) * 365 +
			LEAPS_THRU_END_OF(yg - 1) - LEAPS_THRU_END_OF(y - 1);
		y = yg;
	}
	*yearp = y - 1900;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	*monp = y;
	*dayp = days + 1; /* day in the month */
	return;
}

/*
 * Read/write the hardware clock.
 */

int mac_hwclk(int op, struct rtc_time *t)
{
<<<<<<< HEAD
	unsigned long now;

	if (!op) { /* read */
		switch (macintosh_config->adb_type) {
		case MAC_ADB_II:
		case MAC_ADB_IOP:
			now = via_read_time();
			break;
		case MAC_ADB_IISI:
			now = maciisi_read_time();
			break;
		case MAC_ADB_PB1:
		case MAC_ADB_PB2:
			now = pmu_read_time();
			break;
		case MAC_ADB_CUDA:
			now = cuda_read_time();
			break;
=======
	time64_t now;

	if (!op) { /* read */
		switch (macintosh_config->adb_type) {
		case MAC_ADB_IOP:
		case MAC_ADB_II:
		case MAC_ADB_PB1:
			now = via_read_time();
			break;
#ifdef CONFIG_ADB_CUDA
		case MAC_ADB_EGRET:
		case MAC_ADB_CUDA:
			now = cuda_get_time();
			break;
#endif
#ifdef CONFIG_ADB_PMU
		case MAC_ADB_PB2:
			now = pmu_get_time();
			break;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			now = 0;
		}

		t->tm_wday = 0;
		unmktime(now, 0,
			 &t->tm_year, &t->tm_mon, &t->tm_mday,
			 &t->tm_hour, &t->tm_min, &t->tm_sec);
<<<<<<< HEAD
#if 0
		printk("mac_hwclk: read %04d-%02d-%-2d %02d:%02d:%02d\n",
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);
#endif
	} else { /* write */
#if 0
		printk("mac_hwclk: tried to write %04d-%02d-%-2d %02d:%02d:%02d\n",
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);
#endif

		now = mktime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			     t->tm_hour, t->tm_min, t->tm_sec);

		switch (macintosh_config->adb_type) {
		case MAC_ADB_II:
		case MAC_ADB_IOP:
			via_write_time(now);
			break;
		case MAC_ADB_CUDA:
			cuda_write_time(now);
			break;
		case MAC_ADB_PB1:
		case MAC_ADB_PB2:
			pmu_write_time(now);
			break;
		case MAC_ADB_IISI:
			maciisi_write_time(now);
=======
		pr_debug("%s: read %ptR\n", __func__, t);
	} else { /* write */
		pr_debug("%s: tried to write %ptR\n", __func__, t);

		switch (macintosh_config->adb_type) {
		case MAC_ADB_IOP:
		case MAC_ADB_II:
		case MAC_ADB_PB1:
			via_set_rtc_time(t);
			break;
#ifdef CONFIG_ADB_CUDA
		case MAC_ADB_EGRET:
		case MAC_ADB_CUDA:
			cuda_set_rtc_time(t);
			break;
#endif
#ifdef CONFIG_ADB_PMU
		case MAC_ADB_PB2:
			pmu_set_rtc_time(t);
			break;
#endif
		default:
			return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	return 0;
}
<<<<<<< HEAD

/*
 * Set minutes/seconds in the hardware clock
 */

int mac_set_clock_mmss (unsigned long nowtime)
{
	struct rtc_time now;

	mac_hwclk(0, &now);
	now.tm_sec = nowtime % 60;
	now.tm_min = (nowtime / 60) % 60;
	mac_hwclk(1, &now);

	return 0;
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
