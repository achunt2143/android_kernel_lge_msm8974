<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * NSC/Cyrix CPU indexed register access. Must be inlined instead of
 * macros to ensure correct access ordering
 * Access order is always 0x22 (=offset), 0x23 (=value)
<<<<<<< HEAD
 *
 * When using the old macros a line like
 *   setCx86(CX86_CCR2, getCx86(CX86_CCR2) | 0x88);
 * gets expanded to:
 *  do {
 *    outb((CX86_CCR2), 0x22);
 *    outb((({
 *        outb((CX86_CCR2), 0x22);
 *        inb(0x23);
 *    }) | 0x88), 0x23);
 *  } while (0);
 *
 * which in fact violates the access order (= 0x22, 0x22, 0x23, 0x23).
 */

static inline u8 getCx86(u8 reg)
{
	outb(reg, 0x22);
	return inb(0x23);
=======
 */

#include <asm/pc-conf-reg.h>

static inline u8 getCx86(u8 reg)
{
	return pc_conf_get(reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void setCx86(u8 reg, u8 data)
{
<<<<<<< HEAD
	outb(reg, 0x22);
	outb(data, 0x23);
}

#define getCx86_old(reg) ({ outb((reg), 0x22); inb(0x23); })

#define setCx86_old(reg, data) do { \
	outb((reg), 0x22); \
	outb((data), 0x23); \
} while (0)

=======
	pc_conf_set(reg, data);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
