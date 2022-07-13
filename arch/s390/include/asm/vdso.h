<<<<<<< HEAD
#ifndef __S390_VDSO_H__
#define __S390_VDSO_H__

#ifdef __KERNEL__

/* Default link addresses for the vDSOs */
#define VDSO32_LBASE	0
#define VDSO64_LBASE	0

#define VDSO_VERSION_STRING	LINUX_2.6.29

#ifndef __ASSEMBLY__

/*
 * Note about the vdso_data and vdso_per_cpu_data structures:
 *
 * NEVER USE THEM IN USERSPACE CODE DIRECTLY. The layout of the
 * structure is supposed to be known only to the function in the vdso
 * itself and may change without notice.
 */

struct vdso_data {
	__u64 tb_update_count;		/* Timebase atomicity ctr	0x00 */
	__u64 xtime_tod_stamp;		/* TOD clock for xtime		0x08 */
	__u64 xtime_clock_sec;		/* Kernel time			0x10 */
	__u64 xtime_clock_nsec;		/*				0x18 */
	__u64 wtom_clock_sec;		/* Wall to monotonic clock	0x20 */
	__u64 wtom_clock_nsec;		/*				0x28 */
	__u32 tz_minuteswest;		/* Minutes west of Greenwich	0x30 */
	__u32 tz_dsttime;		/* Type of dst correction	0x34 */
	__u32 ectg_available;
	__u32 ntp_mult;			/* NTP adjusted multiplier	0x3C */
};

struct vdso_per_cpu_data {
	__u64 ectg_timer_base;
	__u64 ectg_user_time;
};

extern struct vdso_data *vdso_data;

#ifdef CONFIG_64BIT
int vdso_alloc_per_cpu(struct _lowcore *lowcore);
void vdso_free_per_cpu(struct _lowcore *lowcore);
#endif

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __S390_VDSO_H__
#define __S390_VDSO_H__

#include <vdso/datapage.h>

#ifndef __ASSEMBLY__

#include <generated/vdso64-offsets.h>
#ifdef CONFIG_COMPAT
#include <generated/vdso32-offsets.h>
#endif

#define VDSO64_SYMBOL(tsk, name) ((tsk)->mm->context.vdso_base + (vdso64_offset_##name))
#ifdef CONFIG_COMPAT
#define VDSO32_SYMBOL(tsk, name) ((tsk)->mm->context.vdso_base + (vdso32_offset_##name))
#else
#define VDSO32_SYMBOL(tsk, name) (-1UL)
#endif

extern struct vdso_data *vdso_data;

int vdso_getcpu_init(void);

#endif /* __ASSEMBLY__ */

/* Default link address for the vDSO */
#define VDSO_LBASE	0

#define __VVAR_PAGES	2

#define VDSO_VERSION_STRING	LINUX_2.6.29
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __S390_VDSO_H__ */
