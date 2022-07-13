<<<<<<< HEAD
#ifndef __PHY_FIXED_H
#define __PHY_FIXED_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PHY_FIXED_H
#define __PHY_FIXED_H

#include <linux/types.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct fixed_phy_status {
	int link;
	int speed;
	int duplex;
	int pause;
	int asym_pause;
};

<<<<<<< HEAD
#ifdef CONFIG_FIXED_PHY
extern int fixed_phy_add(unsigned int irq, int phy_id,
			 struct fixed_phy_status *status);
=======
struct device_node;
struct gpio_desc;
struct net_device;

#if IS_ENABLED(CONFIG_FIXED_PHY)
extern int fixed_phy_change_carrier(struct net_device *dev, bool new_carrier);
extern int fixed_phy_add(unsigned int irq, int phy_id,
			 struct fixed_phy_status *status);
extern struct phy_device *fixed_phy_register(unsigned int irq,
					     struct fixed_phy_status *status,
					     struct device_node *np);

extern struct phy_device *
fixed_phy_register_with_gpiod(unsigned int irq,
			      struct fixed_phy_status *status,
			      struct gpio_desc *gpiod);

extern void fixed_phy_unregister(struct phy_device *phydev);
extern int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline int fixed_phy_add(unsigned int irq, int phy_id,
				struct fixed_phy_status *status)
{
	return -ENODEV;
}
<<<<<<< HEAD
#endif /* CONFIG_FIXED_PHY */

/*
 * This function issued only by fixed_phy-aware drivers, no need
 * protect it with #ifdef
 */
extern int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *));

=======
static inline struct phy_device *fixed_phy_register(unsigned int irq,
						struct fixed_phy_status *status,
						struct device_node *np)
{
	return ERR_PTR(-ENODEV);
}

static inline struct phy_device *
fixed_phy_register_with_gpiod(unsigned int irq,
			      struct fixed_phy_status *status,
			      struct gpio_desc *gpiod)
{
	return ERR_PTR(-ENODEV);
}

static inline void fixed_phy_unregister(struct phy_device *phydev)
{
}
static inline int fixed_phy_set_link_update(struct phy_device *phydev,
			int (*link_update)(struct net_device *,
					   struct fixed_phy_status *))
{
	return -ENODEV;
}
static inline int fixed_phy_change_carrier(struct net_device *dev, bool new_carrier)
{
	return -EINVAL;
}
#endif /* CONFIG_FIXED_PHY */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __PHY_FIXED_H */
