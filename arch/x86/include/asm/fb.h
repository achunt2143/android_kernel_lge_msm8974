<<<<<<< HEAD
#ifndef _ASM_X86_FB_H
#define _ASM_X86_FB_H

#include <linux/fb.h>
#include <linux/fs.h>
#include <asm/page.h>

static inline void fb_pgprotect(struct file *file, struct vm_area_struct *vma,
				unsigned long off)
{
	if (boot_cpu_data.x86 > 3)
		pgprot_val(vma->vm_page_prot) |= _PAGE_PCD;
}

extern int fb_is_primary_device(struct fb_info *info);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_FB_H
#define _ASM_X86_FB_H

#include <asm/page.h>

struct fb_info;

pgprot_t pgprot_framebuffer(pgprot_t prot,
			    unsigned long vm_start, unsigned long vm_end,
			    unsigned long offset);
#define pgprot_framebuffer pgprot_framebuffer

int fb_is_primary_device(struct fb_info *info);
#define fb_is_primary_device fb_is_primary_device

#include <asm-generic/fb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_FB_H */
