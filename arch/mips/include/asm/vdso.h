<<<<<<< HEAD
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2009 Cavium Networks
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2015 Imagination Technologies
 * Author: Alex Smith <alex.smith@imgtec.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ASM_VDSO_H
#define __ASM_VDSO_H

<<<<<<< HEAD
#include <linux/types.h>


#ifdef CONFIG_32BIT
struct mips_vdso {
	u32 signal_trampoline[2];
	u32 rt_signal_trampoline[2];
};
#else  /* !CONFIG_32BIT */
struct mips_vdso {
	u32 o32_signal_trampoline[2];
	u32 o32_rt_signal_trampoline[2];
	u32 rt_signal_trampoline[2];
	u32 n32_rt_signal_trampoline[2];
};
#endif /* CONFIG_32BIT */
=======
#include <linux/mm_types.h>
#include <vdso/datapage.h>

#include <asm/barrier.h>

/**
 * struct mips_vdso_image - Details of a VDSO image.
 * @data: Pointer to VDSO image data (page-aligned).
 * @size: Size of the VDSO image data (page-aligned).
 * @off_sigreturn: Offset of the sigreturn() trampoline.
 * @off_rt_sigreturn: Offset of the rt_sigreturn() trampoline.
 * @mapping: Special mapping structure.
 *
 * This structure contains details of a VDSO image, including the image data
 * and offsets of certain symbols required by the kernel. It is generated as
 * part of the VDSO build process, aside from the mapping page array, which is
 * populated at runtime.
 */
struct mips_vdso_image {
	void *data;
	unsigned long size;

	unsigned long off_sigreturn;
	unsigned long off_rt_sigreturn;

	struct vm_special_mapping mapping;
};

/*
 * The following structures are auto-generated as part of the build for each
 * ABI by genvdso, see arch/mips/vdso/Makefile.
 */

extern struct mips_vdso_image vdso_image;

#ifdef CONFIG_MIPS32_O32
extern struct mips_vdso_image vdso_image_o32;
#endif

#ifdef CONFIG_MIPS32_N32
extern struct mips_vdso_image vdso_image_n32;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_VDSO_H */
