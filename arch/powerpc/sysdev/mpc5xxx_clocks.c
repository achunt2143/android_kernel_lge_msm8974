<<<<<<< HEAD
/**
 * 	mpc5xxx_get_bus_frequency - Find the bus frequency for a device
 * 	@node:	device node
 *
 * 	Returns bus frequency (IPS on MPC512x, IPB on MPC52xx),
 * 	or 0 if the bus frequency cannot be found.
 */

#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/export.h>

unsigned int
mpc5xxx_get_bus_frequency(struct device_node *node)
{
	struct device_node *np;
	const unsigned int *p_bus_freq = NULL;

	of_node_get(node);
	while (node) {
		p_bus_freq = of_get_property(node, "bus-frequency", NULL);
		if (p_bus_freq)
			break;

		np = of_get_parent(node);
		of_node_put(node);
		node = np;
	}
	if (node)
		of_node_put(node);

	return p_bus_freq ? *p_bus_freq : 0;
}
EXPORT_SYMBOL(mpc5xxx_get_bus_frequency);
=======
// SPDX-License-Identifier: GPL-2.0

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/property.h>

#include <asm/mpc5xxx.h>

/**
 * mpc5xxx_fwnode_get_bus_frequency - Find the bus frequency for a firmware node
 * @fwnode:	firmware node
 *
 * Returns bus frequency (IPS on MPC512x, IPB on MPC52xx),
 * or 0 if the bus frequency cannot be found.
 */
unsigned long mpc5xxx_fwnode_get_bus_frequency(struct fwnode_handle *fwnode)
{
	struct fwnode_handle *parent;
	u32 bus_freq;
	int ret;

	ret = fwnode_property_read_u32(fwnode, "bus-frequency", &bus_freq);
	if (!ret)
		return bus_freq;

	fwnode_for_each_parent_node(fwnode, parent) {
		ret = fwnode_property_read_u32(parent, "bus-frequency", &bus_freq);
		if (!ret) {
			fwnode_handle_put(parent);
			return bus_freq;
		}
	}

	return 0;
}
EXPORT_SYMBOL(mpc5xxx_fwnode_get_bus_frequency);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
