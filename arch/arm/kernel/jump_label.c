<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/jump_label.h>

#include "insn.h"
#include "patch.h"

#ifdef HAVE_JUMP_LABEL
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/jump_label.h>
#include <asm/patch.h>
#include <asm/insn.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __arch_jump_label_transform(struct jump_entry *entry,
					enum jump_label_type type,
					bool is_static)
{
	void *addr = (void *)entry->code;
	unsigned int insn;

<<<<<<< HEAD
	if (type == JUMP_LABEL_ENABLE)
=======
	if (type == JUMP_LABEL_JMP)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		insn = arm_gen_branch(entry->code, entry->target);
	else
		insn = arm_gen_nop();

	if (is_static)
<<<<<<< HEAD
		__patch_text(addr, insn);
=======
		__patch_text_early(addr, insn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		patch_text(addr, insn);
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	__arch_jump_label_transform(entry, type, false);
}
<<<<<<< HEAD

void arch_jump_label_transform_static(struct jump_entry *entry,
				      enum jump_label_type type)
{
	__arch_jump_label_transform(entry, type, true);
}

#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
