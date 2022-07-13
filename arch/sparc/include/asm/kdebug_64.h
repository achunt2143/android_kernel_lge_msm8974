<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SPARC64_KDEBUG_H
#define _SPARC64_KDEBUG_H

struct pt_regs;

<<<<<<< HEAD
extern void bad_trap(struct pt_regs *, long);
=======
void bad_trap(struct pt_regs *, long);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Grossly misnamed. */
enum die_val {
	DIE_OOPS = 1,
	DIE_DEBUG,	/* ta 0x70 */
	DIE_DEBUG_2,	/* ta 0x71 */
<<<<<<< HEAD
=======
	DIE_BPT,	/* ta 0x73 */
	DIE_SSTEP,	/* ta 0x74 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	DIE_DIE,
	DIE_TRAP,
	DIE_TRAP_TL1,
	DIE_CALL,
	DIE_NMI,
	DIE_NMIWATCHDOG,
};

#endif
