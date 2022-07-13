<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef LINUX_BCMA_SOC_H_
#define LINUX_BCMA_SOC_H_

#include <linux/bcma/bcma.h>

struct bcma_soc {
	struct bcma_bus bus;
<<<<<<< HEAD
	struct bcma_device core_cc;
	struct bcma_device core_mips;
};

int __init bcma_host_soc_register(struct bcma_soc *soc);
=======
	struct device *dev;
};

int __init bcma_host_soc_register(struct bcma_soc *soc);
int __init bcma_host_soc_init(struct bcma_soc *soc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int bcma_bus_register(struct bcma_bus *bus);

#endif /* LINUX_BCMA_SOC_H_ */
