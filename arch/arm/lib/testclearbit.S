<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/lib/testclearbit.S
 *
 *  Copyright (C) 1995-1996 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include "bitops.h"
                .text

testop	_test_and_clear_bit, bicne, strne
<<<<<<< HEAD
=======

#if __LINUX_ARM_ARCH__ >= 6
sync_testop	_sync_test_and_clear_bit, bicne, strne
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
