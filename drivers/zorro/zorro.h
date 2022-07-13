<<<<<<< HEAD

extern void zorro_name_device(struct zorro_dev *z);
extern int zorro_create_sysfs_dev_files(struct zorro_dev *z);

=======
/* SPDX-License-Identifier: GPL-2.0 */

    /*
     *  Zorro bus
     */

extern const struct bus_type zorro_bus_type;


#ifdef CONFIG_ZORRO_NAMES
extern void zorro_name_device(struct zorro_dev *z);
#else
static inline void zorro_name_device(struct zorro_dev *dev) { }
#endif

extern const struct attribute_group *zorro_device_attribute_groups[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
