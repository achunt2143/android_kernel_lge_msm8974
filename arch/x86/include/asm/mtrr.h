/*  Generic MTRR (Memory Type Range Register) ioctls.

    Copyright (C) 1997-1999  Richard Gooch

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Richard Gooch may be reached by email at  rgooch@atnf.csiro.au
    The postal address is:
      Richard Gooch, c/o ATNF, P. O. Box 76, Epping, N.S.W., 2121, Australia.
*/
#ifndef _ASM_X86_MTRR_H
#define _ASM_X86_MTRR_H

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/errno.h>

#define	MTRR_IOCTL_BASE	'M'

/* Warning: this structure has a different order from i386
   on x86-64. The 32bit emulation code takes care of that.
   But you need to use this for 64bit, otherwise your X server
   will break. */

#ifdef __i386__
struct mtrr_sentry {
    unsigned long base;    /*  Base address     */
    unsigned int size;    /*  Size of region   */
    unsigned int type;     /*  Type of region   */
};

struct mtrr_gentry {
    unsigned int regnum;   /*  Register number  */
    unsigned long base;    /*  Base address     */
    unsigned int size;    /*  Size of region   */
    unsigned int type;     /*  Type of region   */
};

#else /* __i386__ */

struct mtrr_sentry {
	__u64 base;		/*  Base address     */
	__u32 size;		/*  Size of region   */
	__u32 type;		/*  Type of region   */
};

struct mtrr_gentry {
	__u64 base;		/*  Base address     */
	__u32 size;		/*  Size of region   */
	__u32 regnum;		/*  Register number  */
	__u32 type;		/*  Type of region   */
	__u32 _pad;		/*  Unused	     */
};

#endif /* !__i386__ */

struct mtrr_var_range {
	__u32 base_lo;
	__u32 base_hi;
	__u32 mask_lo;
	__u32 mask_hi;
};

/* In the Intel processor's MTRR interface, the MTRR type is always held in
   an 8 bit field: */
typedef __u8 mtrr_type;

#define MTRR_NUM_FIXED_RANGES 88
#define MTRR_MAX_VAR_RANGES 256
=======
#include <linux/bits.h>
#include <uapi/asm/mtrr.h>

/* Defines for hardware MTRR registers. */
#define MTRR_CAP_VCNT		GENMASK(7, 0)
#define MTRR_CAP_FIX		BIT_MASK(8)
#define MTRR_CAP_WC		BIT_MASK(10)

#define MTRR_DEF_TYPE_TYPE	GENMASK(7, 0)
#define MTRR_DEF_TYPE_FE	BIT_MASK(10)
#define MTRR_DEF_TYPE_E		BIT_MASK(11)

#define MTRR_DEF_TYPE_ENABLE	(MTRR_DEF_TYPE_FE | MTRR_DEF_TYPE_E)
#define MTRR_DEF_TYPE_DISABLE	~(MTRR_DEF_TYPE_TYPE | MTRR_DEF_TYPE_ENABLE)

#define MTRR_PHYSBASE_TYPE	GENMASK(7, 0)
#define MTRR_PHYSBASE_RSVD	GENMASK(11, 8)

#define MTRR_PHYSMASK_RSVD	GENMASK(10, 0)
#define MTRR_PHYSMASK_V		BIT_MASK(11)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct mtrr_state_type {
	struct mtrr_var_range var_ranges[MTRR_MAX_VAR_RANGES];
	mtrr_type fixed_ranges[MTRR_NUM_FIXED_RANGES];
	unsigned char enabled;
<<<<<<< HEAD
	unsigned char have_fixed;
	mtrr_type def_type;
};

#define MTRRphysBase_MSR(reg) (0x200 + 2 * (reg))
#define MTRRphysMask_MSR(reg) (0x200 + 2 * (reg) + 1)

/*  These are the various ioctls  */
#define MTRRIOC_ADD_ENTRY        _IOW(MTRR_IOCTL_BASE,  0, struct mtrr_sentry)
#define MTRRIOC_SET_ENTRY        _IOW(MTRR_IOCTL_BASE,  1, struct mtrr_sentry)
#define MTRRIOC_DEL_ENTRY        _IOW(MTRR_IOCTL_BASE,  2, struct mtrr_sentry)
#define MTRRIOC_GET_ENTRY        _IOWR(MTRR_IOCTL_BASE, 3, struct mtrr_gentry)
#define MTRRIOC_KILL_ENTRY       _IOW(MTRR_IOCTL_BASE,  4, struct mtrr_sentry)
#define MTRRIOC_ADD_PAGE_ENTRY   _IOW(MTRR_IOCTL_BASE,  5, struct mtrr_sentry)
#define MTRRIOC_SET_PAGE_ENTRY   _IOW(MTRR_IOCTL_BASE,  6, struct mtrr_sentry)
#define MTRRIOC_DEL_PAGE_ENTRY   _IOW(MTRR_IOCTL_BASE,  7, struct mtrr_sentry)
#define MTRRIOC_GET_PAGE_ENTRY   _IOWR(MTRR_IOCTL_BASE, 8, struct mtrr_gentry)
#define MTRRIOC_KILL_PAGE_ENTRY  _IOW(MTRR_IOCTL_BASE,  9, struct mtrr_sentry)

/*  These are the region types  */
#define MTRR_TYPE_UNCACHABLE 0
#define MTRR_TYPE_WRCOMB     1
/*#define MTRR_TYPE_         2*/
/*#define MTRR_TYPE_         3*/
#define MTRR_TYPE_WRTHROUGH  4
#define MTRR_TYPE_WRPROT     5
#define MTRR_TYPE_WRBACK     6
#define MTRR_NUM_TYPES       7

#ifdef __KERNEL__

/*  The following functions are for use by other drivers  */
# ifdef CONFIG_MTRR
extern u8 mtrr_type_lookup(u64 addr, u64 end);
=======
	bool have_fixed;
	mtrr_type def_type;
};

/*
 * The following functions are for use by other drivers that cannot use
 * arch_phys_wc_add and arch_phys_wc_del.
 */
# ifdef CONFIG_MTRR
void mtrr_bp_init(void);
void mtrr_overwrite_state(struct mtrr_var_range *var, unsigned int num_var,
			  mtrr_type def_type);
extern u8 mtrr_type_lookup(u64 addr, u64 end, u8 *uniform);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void mtrr_save_fixed_ranges(void *);
extern void mtrr_save_state(void);
extern int mtrr_add(unsigned long base, unsigned long size,
		    unsigned int type, bool increment);
extern int mtrr_add_page(unsigned long base, unsigned long size,
			 unsigned int type, bool increment);
extern int mtrr_del(int reg, unsigned long base, unsigned long size);
extern int mtrr_del_page(int reg, unsigned long base, unsigned long size);
<<<<<<< HEAD
extern void mtrr_centaur_report_mcr(int mcr, u32 lo, u32 hi);
extern void mtrr_ap_init(void);
extern void mtrr_bp_init(void);
extern void set_mtrr_aps_delayed_init(void);
extern void mtrr_aps_init(void);
extern void mtrr_bp_restore(void);
extern int mtrr_trim_uncached_memory(unsigned long end_pfn);
extern int amd_special_default_mtrr(void);
#  else
static inline u8 mtrr_type_lookup(u64 addr, u64 end)
{
	/*
	 * Return no-MTRRs:
	 */
	return 0xff;
=======
extern void mtrr_bp_restore(void);
extern int mtrr_trim_uncached_memory(unsigned long end_pfn);
extern int amd_special_default_mtrr(void);
void mtrr_disable(void);
void mtrr_enable(void);
void mtrr_generic_set_state(void);
#  else
static inline void mtrr_overwrite_state(struct mtrr_var_range *var,
					unsigned int num_var,
					mtrr_type def_type)
{
}

static inline u8 mtrr_type_lookup(u64 addr, u64 end, u8 *uniform)
{
	/*
	 * Return the default MTRR type, without any known other types in
	 * that range.
	 */
	*uniform = 1;

	return MTRR_TYPE_UNCACHABLE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#define mtrr_save_fixed_ranges(arg) do {} while (0)
#define mtrr_save_state() do {} while (0)
static inline int mtrr_add(unsigned long base, unsigned long size,
			   unsigned int type, bool increment)
{
    return -ENODEV;
}
static inline int mtrr_add_page(unsigned long base, unsigned long size,
				unsigned int type, bool increment)
{
    return -ENODEV;
}
static inline int mtrr_del(int reg, unsigned long base, unsigned long size)
{
    return -ENODEV;
}
static inline int mtrr_del_page(int reg, unsigned long base, unsigned long size)
{
    return -ENODEV;
}
static inline int mtrr_trim_uncached_memory(unsigned long end_pfn)
{
	return 0;
}
<<<<<<< HEAD
static inline void mtrr_centaur_report_mcr(int mcr, u32 lo, u32 hi)
{
}

#define mtrr_ap_init() do {} while (0)
#define mtrr_bp_init() do {} while (0)
#define set_mtrr_aps_delayed_init() do {} while (0)
#define mtrr_aps_init() do {} while (0)
#define mtrr_bp_restore() do {} while (0)
=======
#define mtrr_bp_init() do {} while (0)
#define mtrr_bp_restore() do {} while (0)
#define mtrr_disable() do {} while (0)
#define mtrr_enable() do {} while (0)
#define mtrr_generic_set_state() do {} while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#  endif

#ifdef CONFIG_COMPAT
#include <linux/compat.h>

struct mtrr_sentry32 {
    compat_ulong_t base;    /*  Base address     */
    compat_uint_t size;    /*  Size of region   */
    compat_uint_t type;     /*  Type of region   */
};

struct mtrr_gentry32 {
    compat_ulong_t regnum;   /*  Register number  */
    compat_uint_t base;    /*  Base address     */
    compat_uint_t size;    /*  Size of region   */
    compat_uint_t type;     /*  Type of region   */
};

#define MTRR_IOCTL_BASE 'M'

#define MTRRIOC32_ADD_ENTRY      _IOW(MTRR_IOCTL_BASE,  0, struct mtrr_sentry32)
#define MTRRIOC32_SET_ENTRY      _IOW(MTRR_IOCTL_BASE,  1, struct mtrr_sentry32)
#define MTRRIOC32_DEL_ENTRY      _IOW(MTRR_IOCTL_BASE,  2, struct mtrr_sentry32)
#define MTRRIOC32_GET_ENTRY      _IOWR(MTRR_IOCTL_BASE, 3, struct mtrr_gentry32)
#define MTRRIOC32_KILL_ENTRY     _IOW(MTRR_IOCTL_BASE,  4, struct mtrr_sentry32)
#define MTRRIOC32_ADD_PAGE_ENTRY _IOW(MTRR_IOCTL_BASE,  5, struct mtrr_sentry32)
#define MTRRIOC32_SET_PAGE_ENTRY _IOW(MTRR_IOCTL_BASE,  6, struct mtrr_sentry32)
#define MTRRIOC32_DEL_PAGE_ENTRY _IOW(MTRR_IOCTL_BASE,  7, struct mtrr_sentry32)
#define MTRRIOC32_GET_PAGE_ENTRY _IOWR(MTRR_IOCTL_BASE, 8, struct mtrr_gentry32)
#define MTRRIOC32_KILL_PAGE_ENTRY		\
				 _IOW(MTRR_IOCTL_BASE,  9, struct mtrr_sentry32)
#endif /* CONFIG_COMPAT */

<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
/* Bit fields for enabled in struct mtrr_state_type */
#define MTRR_STATE_SHIFT		10
#define MTRR_STATE_MTRR_FIXED_ENABLED	(MTRR_DEF_TYPE_FE >> MTRR_STATE_SHIFT)
#define MTRR_STATE_MTRR_ENABLED		(MTRR_DEF_TYPE_E >> MTRR_STATE_SHIFT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_MTRR_H */
