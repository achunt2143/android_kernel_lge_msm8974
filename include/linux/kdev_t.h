<<<<<<< HEAD
#ifndef _LINUX_KDEV_T_H
#define _LINUX_KDEV_T_H
#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KDEV_T_H
#define _LINUX_KDEV_T_H

#include <uapi/linux/kdev_t.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))

#define print_dev_t(buffer, dev)					\
	sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))

#define format_dev_t(buffer, dev)					\
	({								\
		sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));	\
		buffer;							\
	})

/* acceptable for old filesystems */
<<<<<<< HEAD
static inline int old_valid_dev(dev_t dev)
=======
static __always_inline bool old_valid_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return MAJOR(dev) < 256 && MINOR(dev) < 256;
}

<<<<<<< HEAD
static inline u16 old_encode_dev(dev_t dev)
=======
static __always_inline u16 old_encode_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (MAJOR(dev) << 8) | MINOR(dev);
}

<<<<<<< HEAD
static inline dev_t old_decode_dev(u16 val)
=======
static __always_inline dev_t old_decode_dev(u16 val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return MKDEV((val >> 8) & 255, val & 255);
}

<<<<<<< HEAD
static inline int new_valid_dev(dev_t dev)
{
	return 1;
}

static inline u32 new_encode_dev(dev_t dev)
=======
static __always_inline u32 new_encode_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned major = MAJOR(dev);
	unsigned minor = MINOR(dev);
	return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}

<<<<<<< HEAD
static inline dev_t new_decode_dev(u32 dev)
=======
static __always_inline dev_t new_decode_dev(u32 dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned major = (dev & 0xfff00) >> 8;
	unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
	return MKDEV(major, minor);
}

<<<<<<< HEAD
static inline int huge_valid_dev(dev_t dev)
{
	return 1;
}

static inline u64 huge_encode_dev(dev_t dev)
=======
static __always_inline u64 huge_encode_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return new_encode_dev(dev);
}

<<<<<<< HEAD
static inline dev_t huge_decode_dev(u64 dev)
=======
static __always_inline dev_t huge_decode_dev(u64 dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return new_decode_dev(dev);
}

<<<<<<< HEAD
static inline int sysv_valid_dev(dev_t dev)
=======
static __always_inline int sysv_valid_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return MAJOR(dev) < (1<<14) && MINOR(dev) < (1<<18);
}

<<<<<<< HEAD
static inline u32 sysv_encode_dev(dev_t dev)
=======
static __always_inline u32 sysv_encode_dev(dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return MINOR(dev) | (MAJOR(dev) << 18);
}

<<<<<<< HEAD
static inline unsigned sysv_major(u32 dev)
=======
static __always_inline unsigned sysv_major(u32 dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (dev >> 18) & 0x3fff;
}

<<<<<<< HEAD
static inline unsigned sysv_minor(u32 dev)
=======
static __always_inline unsigned sysv_minor(u32 dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return dev & 0x3ffff;
}

<<<<<<< HEAD
#else /* __KERNEL__ */

/*
Some programs want their definitions of MAJOR and MINOR and MKDEV
from the kernel sources. These must be the externally visible ones.
*/
#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))
#endif /* __KERNEL__ */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
