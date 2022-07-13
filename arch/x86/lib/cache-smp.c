<<<<<<< HEAD
#include <linux/smp.h>
#include <linux/module.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <asm/paravirt.h>
#include <linux/smp.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __wbinvd(void *dummy)
{
	wbinvd();
}

void wbinvd_on_cpu(int cpu)
{
	smp_call_function_single(cpu, __wbinvd, NULL, 1);
}
EXPORT_SYMBOL(wbinvd_on_cpu);

int wbinvd_on_all_cpus(void)
{
<<<<<<< HEAD
	return on_each_cpu(__wbinvd, NULL, 1);
=======
	on_each_cpu(__wbinvd, NULL, 1);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(wbinvd_on_all_cpus);
