<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Jump label s390 support
 *
 * Copyright IBM Corp. 2011
 * Author(s): Jan Glauber <jang@linux.vnet.ibm.com>
 */
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/stop_machine.h>
#include <linux/jump_label.h>
#include <asm/ipl.h>

#ifdef HAVE_JUMP_LABEL

=======
#include <linux/uaccess.h>
#include <linux/jump_label.h>
#include <linux/module.h>
#include <asm/text-patching.h>
#include <asm/ipl.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct insn {
	u16 opcode;
	s32 offset;
} __packed;

<<<<<<< HEAD
struct insn_args {
	struct jump_entry *entry;
	enum jump_label_type type;
};

static void __jump_label_transform(struct jump_entry *entry,
				   enum jump_label_type type)
{
	struct insn insn;
	int rc;

	if (type == JUMP_LABEL_ENABLE) {
		/* brcl 15,offset */
		insn.opcode = 0xc0f4;
		insn.offset = (entry->target - entry->code) >> 1;
	} else {
		/* brcl 0,0 */
		insn.opcode = 0xc004;
		insn.offset = 0;
	}

	rc = probe_kernel_write((void *)entry->code, &insn, JUMP_LABEL_NOP_SIZE);
	WARN_ON_ONCE(rc < 0);
}

static int __sm_arch_jump_label_transform(void *data)
{
	struct insn_args *args = data;

	__jump_label_transform(args->entry, args->type);
	return 0;
=======
static void jump_label_make_nop(struct jump_entry *entry, struct insn *insn)
{
	/* brcl 0,offset */
	insn->opcode = 0xc004;
	insn->offset = (jump_entry_target(entry) - jump_entry_code(entry)) >> 1;
}

static void jump_label_make_branch(struct jump_entry *entry, struct insn *insn)
{
	/* brcl 15,offset */
	insn->opcode = 0xc0f4;
	insn->offset = (jump_entry_target(entry) - jump_entry_code(entry)) >> 1;
}

static void jump_label_bug(struct jump_entry *entry, struct insn *expected,
			   struct insn *new)
{
	unsigned char *ipc = (unsigned char *)jump_entry_code(entry);
	unsigned char *ipe = (unsigned char *)expected;
	unsigned char *ipn = (unsigned char *)new;

	pr_emerg("Jump label code mismatch at %pS [%px]\n", ipc, ipc);
	pr_emerg("Found:    %6ph\n", ipc);
	pr_emerg("Expected: %6ph\n", ipe);
	pr_emerg("New:      %6ph\n", ipn);
	panic("Corrupted kernel text");
}

static void jump_label_transform(struct jump_entry *entry,
				 enum jump_label_type type)
{
	void *code = (void *)jump_entry_code(entry);
	struct insn old, new;

	if (type == JUMP_LABEL_JMP) {
		jump_label_make_nop(entry, &old);
		jump_label_make_branch(entry, &new);
	} else {
		jump_label_make_branch(entry, &old);
		jump_label_make_nop(entry, &new);
	}
	if (memcmp(code, &old, sizeof(old)))
		jump_label_bug(entry, &old, &new);
	s390_kernel_write(code, &new, sizeof(new));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
<<<<<<< HEAD
	struct insn_args args;

	args.entry = entry;
	args.type = type;

	stop_machine(__sm_arch_jump_label_transform, &args, NULL);
}

void arch_jump_label_transform_static(struct jump_entry *entry,
				      enum jump_label_type type)
{
	__jump_label_transform(entry, type);
}

#endif
=======
	jump_label_transform(entry, type);
	text_poke_sync();
}

bool arch_jump_label_transform_queue(struct jump_entry *entry,
				     enum jump_label_type type)
{
	jump_label_transform(entry, type);
	return true;
}

void arch_jump_label_transform_apply(void)
{
	text_poke_sync();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
