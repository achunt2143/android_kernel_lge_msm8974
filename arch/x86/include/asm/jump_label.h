<<<<<<< HEAD
#ifndef _ASM_X86_JUMP_LABEL_H
#define _ASM_X86_JUMP_LABEL_H

#ifdef __KERNEL__

#include <linux/types.h>
#include <asm/nops.h>
#include <asm/asm.h>

#define JUMP_LABEL_NOP_SIZE 5

#define STATIC_KEY_INITIAL_NOP ".byte 0xe9 \n\t .long 0\n\t"

static __always_inline bool arch_static_branch(struct static_key *key)
{
	asm_volatile_goto("1:"
		STATIC_KEY_INITIAL_NOP
		".pushsection __jump_table,  \"aw\" \n\t"
		_ASM_ALIGN "\n\t"
		_ASM_PTR "1b, %l[l_yes], %c0 \n\t"
		".popsection \n\t"
		: :  "i" (key) : : l_yes);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_JUMP_LABEL_H
#define _ASM_X86_JUMP_LABEL_H

#define HAVE_JUMP_LABEL_BATCH

#include <asm/asm.h>
#include <asm/nops.h>

#ifndef __ASSEMBLY__

#include <linux/stringify.h>
#include <linux/types.h>

#define JUMP_TABLE_ENTRY				\
	".pushsection __jump_table,  \"aw\" \n\t"	\
	_ASM_ALIGN "\n\t"				\
	".long 1b - . \n\t"				\
	".long %l[l_yes] - . \n\t"			\
	_ASM_PTR "%c0 + %c1 - .\n\t"			\
	".popsection \n\t"

#ifdef CONFIG_HAVE_JUMP_LABEL_HACK

static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
{
	asm goto("1:"
		"jmp %l[l_yes] # objtool NOPs this \n\t"
		JUMP_TABLE_ENTRY
		: :  "i" (key), "i" (2 | branch) : : l_yes);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return false;
l_yes:
	return true;
}

<<<<<<< HEAD
#endif /* __KERNEL__ */

#ifdef CONFIG_X86_64
typedef u64 jump_label_t;
#else
typedef u32 jump_label_t;
#endif

struct jump_entry {
	jump_label_t code;
	jump_label_t target;
	jump_label_t key;
};
=======
#else /* !CONFIG_HAVE_JUMP_LABEL_HACK */

static __always_inline bool arch_static_branch(struct static_key * const key, const bool branch)
{
	asm goto("1:"
		".byte " __stringify(BYTES_NOP5) "\n\t"
		JUMP_TABLE_ENTRY
		: :  "i" (key), "i" (branch) : : l_yes);

	return false;
l_yes:
	return true;
}

#endif /* CONFIG_HAVE_JUMP_LABEL_HACK */

static __always_inline bool arch_static_branch_jump(struct static_key * const key, const bool branch)
{
	asm goto("1:"
		"jmp %l[l_yes]\n\t"
		JUMP_TABLE_ENTRY
		: :  "i" (key), "i" (branch) : : l_yes);

	return false;
l_yes:
	return true;
}

extern int arch_jump_entry_size(struct jump_entry *entry);

#endif	/* __ASSEMBLY__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
