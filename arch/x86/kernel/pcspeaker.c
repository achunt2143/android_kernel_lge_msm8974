<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/init.h>

static __init int add_pcspkr(void)
{
	struct platform_device *pd;

	pd = platform_device_register_simple("pcspkr", -1, NULL, 0);

<<<<<<< HEAD
	return IS_ERR(pd) ? PTR_ERR(pd) : 0;
=======
	return PTR_ERR_OR_ZERO(pd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
device_initcall(add_pcspkr);
