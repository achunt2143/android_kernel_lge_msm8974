<<<<<<< HEAD
/*
 * Copyright 2010 Michael Ellerman, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2010 Michael Ellerman, IBM Corp.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/jump_label.h>
#include <asm/code-patching.h>
<<<<<<< HEAD

#ifdef HAVE_JUMP_LABEL
void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	u32 *addr = (u32 *)(unsigned long)entry->code;

	if (type == JUMP_LABEL_ENABLE)
		patch_branch(addr, entry->target, 0);
	else
		patch_instruction(addr, PPC_INST_NOP);
}
#endif
=======
#include <asm/inst.h>

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	u32 *addr = (u32 *)jump_entry_code(entry);

	if (type == JUMP_LABEL_JMP)
		patch_branch(addr, jump_entry_target(entry), 0);
	else
		patch_instruction(addr, ppc_inst(PPC_RAW_NOP()));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
