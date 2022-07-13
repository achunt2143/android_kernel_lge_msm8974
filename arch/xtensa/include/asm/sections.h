<<<<<<< HEAD
/*
 * include/asm-xtensa/sections.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _XTENSA_SECTIONS_H
#define _XTENSA_SECTIONS_H

#include <asm-generic/sections.h>

<<<<<<< HEAD
#endif	/* _XTENSA_SECTIONS_H */
=======
#ifdef CONFIG_VECTORS_ADDR
extern char _WindowVectors_text_start[];
extern char _WindowVectors_text_end[];
extern char _DebugInterruptVector_text_start[];
extern char _DebugInterruptVector_text_end[];
extern char _KernelExceptionVector_text_start[];
extern char _KernelExceptionVector_text_end[];
extern char _UserExceptionVector_text_start[];
extern char _UserExceptionVector_text_end[];
extern char _DoubleExceptionVector_text_start[];
extern char _DoubleExceptionVector_text_end[];
extern char _exception_text_start[];
extern char _exception_text_end[];
extern char _Level2InterruptVector_text_start[];
extern char _Level2InterruptVector_text_end[];
extern char _Level3InterruptVector_text_start[];
extern char _Level3InterruptVector_text_end[];
extern char _Level4InterruptVector_text_start[];
extern char _Level4InterruptVector_text_end[];
extern char _Level5InterruptVector_text_start[];
extern char _Level5InterruptVector_text_end[];
extern char _Level6InterruptVector_text_start[];
extern char _Level6InterruptVector_text_end[];
#endif
#ifdef CONFIG_SECONDARY_RESET_VECTOR
extern char _SecondaryResetVector_text_start[];
extern char _SecondaryResetVector_text_end[];
#endif
#ifdef CONFIG_XIP_KERNEL
#ifdef CONFIG_VECTORS_ADDR
extern char _xip_text_start[];
extern char _xip_text_end[];
#endif
extern char _xip_start[];
extern char _xip_end[];
#endif

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
