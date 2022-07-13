<<<<<<< HEAD

MEMSET_FN(__memset,
	"x86-64-unrolled",
	"unrolled memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(memset_c,
	"x86-64-stosq",
	"movsq-based memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(memset_c_e,
	"x86-64-stosb",
	"movsb-based memset() in arch/x86/lib/memset_64.S")
=======
/* SPDX-License-Identifier: GPL-2.0 */

MEMSET_FN(memset_orig,
	"x86-64-unrolled",
	"unrolled memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(__memset,
	"x86-64-stosq",
	"movsq-based memset() in arch/x86/lib/memset_64.S")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
