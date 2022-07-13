<<<<<<< HEAD
#ifdef CONFIG_RTC_INTF_DEV

extern void __init rtc_dev_init(void);
extern void __exit rtc_dev_exit(void);
extern void rtc_dev_prepare(struct rtc_device *rtc);
extern void rtc_dev_add_device(struct rtc_device *rtc);
extern void rtc_dev_del_device(struct rtc_device *rtc);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifdef CONFIG_RTC_INTF_DEV

extern void __init rtc_dev_init(void);
extern void rtc_dev_prepare(struct rtc_device *rtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else

static inline void rtc_dev_init(void)
{
}

<<<<<<< HEAD
static inline void rtc_dev_exit(void)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void rtc_dev_prepare(struct rtc_device *rtc)
{
}

<<<<<<< HEAD
static inline void rtc_dev_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_dev_del_device(struct rtc_device *rtc)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_RTC_INTF_PROC

extern void rtc_proc_add_device(struct rtc_device *rtc);
extern void rtc_proc_del_device(struct rtc_device *rtc);

#else

static inline void rtc_proc_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_proc_del_device(struct rtc_device *rtc)
{
}

#endif

#ifdef CONFIG_RTC_INTF_SYSFS
<<<<<<< HEAD

extern void __init rtc_sysfs_init(struct class *);
extern void rtc_sysfs_add_device(struct rtc_device *rtc);
extern void rtc_sysfs_del_device(struct rtc_device *rtc);

#else

static inline void rtc_sysfs_init(struct class *rtc)
{
}

static inline void rtc_sysfs_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_sysfs_del_device(struct rtc_device *rtc)
{
}

=======
const struct attribute_group **rtc_get_dev_attribute_groups(void);
#else
static inline const struct attribute_group **rtc_get_dev_attribute_groups(void)
{
	return NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
