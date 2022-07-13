<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pm.h>
#include <linux/kexec.h>
#include <linux/kernel.h>
#include <linux/reboot.h>
#include <linux/module.h>
<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
#include <asm/watchdog.h>
#endif
=======
#include <asm/watchdog.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/addrspace.h>
#include <asm/reboot.h>
#include <asm/tlbflush.h>
#include <asm/traps.h>

void (*pm_power_off)(void);
EXPORT_SYMBOL(pm_power_off);

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void watchdog_trigger_immediate(void)
{
	sh_wdt_write_cnt(0xFF);
	sh_wdt_write_csr(0xC2);
}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void native_machine_restart(char * __unused)
{
	local_irq_disable();

	/* Destroy all of the TLBs in preparation for reset by MMU */
	__flush_tlb_global();

	/* Address error with SR.BL=1 first. */
	trigger_address_error();

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
	/* If that fails or is unsupported, go for the watchdog next. */
	watchdog_trigger_immediate();
#endif
=======
	/* If that fails or is unsupported, go for the watchdog next. */
	watchdog_trigger_immediate();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Give up and sleep.
	 */
	while (1)
		cpu_sleep();
}

static void native_machine_shutdown(void)
{
	smp_send_stop();
}

static void native_machine_power_off(void)
{
<<<<<<< HEAD
	if (pm_power_off)
		pm_power_off();
=======
	do_kernel_power_off();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void native_machine_halt(void)
{
	/* stop other cpus */
	machine_shutdown();

	/* stop this cpu */
	stop_this_cpu(NULL);
}

struct machine_ops machine_ops = {
	.power_off	= native_machine_power_off,
	.shutdown	= native_machine_shutdown,
	.restart	= native_machine_restart,
	.halt		= native_machine_halt,
<<<<<<< HEAD
#ifdef CONFIG_KEXEC
=======
#ifdef CONFIG_KEXEC_CORE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.crash_shutdown = native_machine_crash_shutdown,
#endif
};

void machine_power_off(void)
{
	machine_ops.power_off();
}

void machine_shutdown(void)
{
	machine_ops.shutdown();
}

void machine_restart(char *cmd)
{
	machine_ops.restart(cmd);
}

void machine_halt(void)
{
	machine_ops.halt();
}

<<<<<<< HEAD
#ifdef CONFIG_KEXEC
=======
#ifdef CONFIG_KEXEC_CORE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void machine_crash_shutdown(struct pt_regs *regs)
{
	machine_ops.crash_shutdown(regs);
}
#endif
