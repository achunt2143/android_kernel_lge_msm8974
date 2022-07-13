<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/cpu.h>

#include <linux/jump_label.h>
#include <linux/memory.h>

#include <asm/cacheflush.h>

<<<<<<< HEAD
#ifdef HAVE_JUMP_LABEL

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	u32 val;
	u32 *insn = (u32 *) (unsigned long) entry->code;

	if (type == JUMP_LABEL_ENABLE) {
		s32 off = (s32)entry->target - (s32)entry->code;

#ifdef CONFIG_SPARC64
		/* ba,pt %xcc, . + (off << 2) */
		val = 0x10680000 | ((u32) off >> 2);
#else
		/* ba . + (off << 2) */
		val = 0x10800000 | ((u32) off >> 2);
#endif
=======
void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	u32 *insn = (u32 *) (unsigned long) entry->code;
	u32 val;

	if (type == JUMP_LABEL_JMP) {
		s32 off = (s32)entry->target - (s32)entry->code;
		bool use_v9_branch = false;

		BUG_ON(off & 3);

#ifdef CONFIG_SPARC64
		if (off <= 0xfffff && off >= -0x100000)
			use_v9_branch = true;
#endif
		if (use_v9_branch) {
			/* WDISP19 - target is . + immed << 2 */
			/* ba,pt %xcc, . + off */
			val = 0x10680000 | (((u32) off >> 2) & 0x7ffff);
		} else {
			/* WDISP22 - target is . + immed << 2 */
			BUG_ON(off > 0x7fffff);
			BUG_ON(off < -0x800000);
			/* ba . + off */
			val = 0x10800000 | (((u32) off >> 2) & 0x3fffff);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		val = 0x01000000;
	}

<<<<<<< HEAD
	get_online_cpus();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&text_mutex);
	*insn = val;
	flushi(insn);
	mutex_unlock(&text_mutex);
<<<<<<< HEAD
	put_online_cpus();
}

#endif
=======
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
