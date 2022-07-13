<<<<<<< HEAD
#define memset MEMSET /* don't hide glibc's memset() */
#define altinstr_replacement text
#define globl p2align 4; .globl
#define Lmemset_c globl memset_c; memset_c
#define Lmemset_c_e globl memset_c_e; memset_c_e
#include "../../../arch/x86/lib/memset_64.S"
=======
/* SPDX-License-Identifier: GPL-2.0 */
// memset_orig is being defined as SYM_L_LOCAL but we need it
#define SYM_FUNC_START_LOCAL(name)                      \
        SYM_START(name, SYM_L_GLOBAL, SYM_A_ALIGN)
#define memset MEMSET /* don't hide glibc's memset() */
#define altinstr_replacement text
#define globl p2align 4; .globl
#include "../../arch/x86/lib/memset_64.S"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * We need to provide note.GNU-stack section, saying that we want
 * NOT executable stack. Otherwise the final linking will assume that
 * the ELF stack should not be restricted at all and set it RWX.
 */
.section .note.GNU-stack,"",@progbits
