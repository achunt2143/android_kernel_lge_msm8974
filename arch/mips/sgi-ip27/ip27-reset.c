/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Reset an IP27.
 *
 * Copyright (C) 1997, 1998, 1999, 2000, 06 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
<<<<<<< HEAD
=======
#include <linux/compiler.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/smp.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/sgialib.h>
#include <asm/sn/addrs.h>
<<<<<<< HEAD
#include <asm/sn/arch.h>
#include <asm/sn/gda.h>
#include <asm/sn/sn0/hub.h>

void machine_restart(char *command) __attribute__((noreturn));
void machine_halt(void) __attribute__((noreturn));
void machine_power_off(void) __attribute__((noreturn));

#define noreturn while(1);				/* Silence gcc.  */
=======
#include <asm/sn/agent.h>
#include <asm/sn/arch.h>
#include <asm/sn/gda.h>

#include "ip27-common.h"

void machine_restart(char *command) __noreturn;
void machine_halt(void) __noreturn;
void machine_power_off(void) __noreturn;

#define noreturn while(1);				/* Silence gcc.	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* XXX How to pass the reboot command to the firmware??? */
static void ip27_machine_restart(char *command)
{
#if 0
	int i;
#endif

	printk("Reboot started from CPU %d\n", smp_processor_id());
#ifdef CONFIG_SMP
	smp_send_stop();
#endif
#if 0
	for_each_online_node(i)
<<<<<<< HEAD
		REMOTE_HUB_S(COMPACT_TO_NASID_NODEID(i), PROMOP_REG,
							PROMOP_REBOOT);
=======
		REMOTE_HUB_S(i, PROMOP_REG, PROMOP_REBOOT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
#endif
	noreturn;
}

static void ip27_machine_halt(void)
{
	int i;

#ifdef CONFIG_SMP
	smp_send_stop();
#endif
	for_each_online_node(i)
<<<<<<< HEAD
		REMOTE_HUB_S(COMPACT_TO_NASID_NODEID(i), PROMOP_REG,
							PROMOP_RESTART);
=======
		REMOTE_HUB_S(i, PROMOP_REG, PROMOP_RESTART);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
	noreturn;
}

static void ip27_machine_power_off(void)
{
	/* To do ...  */
	noreturn;
}

void ip27_reboot_setup(void)
{
	_machine_restart = ip27_machine_restart;
	_machine_halt = ip27_machine_halt;
	pm_power_off = ip27_machine_power_off;
}
