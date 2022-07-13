<<<<<<< HEAD
#ifndef __WIRELESS_SYSFS_H
#define __WIRELESS_SYSFS_H

extern int wiphy_sysfs_init(void);
extern void wiphy_sysfs_exit(void);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __WIRELESS_SYSFS_H
#define __WIRELESS_SYSFS_H

int wiphy_sysfs_init(void);
void wiphy_sysfs_exit(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct class ieee80211_class;

#endif /* __WIRELESS_SYSFS_H */
