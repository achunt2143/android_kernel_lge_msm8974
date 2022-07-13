<<<<<<< HEAD
#ifndef _ASM_FB_H_
#define _ASM_FB_H_

#include <linux/fb.h>
#include <linux/fs.h>
#include <asm/page.h>
#include <asm/setup.h>

#ifdef CONFIG_MMU
#ifdef CONFIG_SUN3
static inline void fb_pgprotect(struct file *file, struct vm_area_struct *vma,
				unsigned long off)
{
	pgprot_val(vma->vm_page_prot) |= SUN3_PAGE_NOCACHE;
}
#else
static inline void fb_pgprotect(struct file *file, struct vm_area_struct *vma,
				unsigned long off)
{
	if (CPU_IS_020_OR_030)
		pgprot_val(vma->vm_page_prot) |= _PAGE_NOCACHE030;
	if (CPU_IS_040_OR_060) {
		pgprot_val(vma->vm_page_prot) &= _CACHEMASK040;
		/* Use no-cache mode, serialized */
		pgprot_val(vma->vm_page_prot) |= _PAGE_NOCACHE_S;
	}
}
#endif /* CONFIG_SUN3 */
#else
#define fb_pgprotect(...) do {} while (0)
#endif /* CONFIG_MMU */

static inline int fb_is_primary_device(struct fb_info *info)
{
	return 0;
}
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_FB_H_
#define _ASM_FB_H_

#include <asm/page.h>
#include <asm/setup.h>

static inline pgprot_t pgprot_framebuffer(pgprot_t prot,
					  unsigned long vm_start, unsigned long vm_end,
					  unsigned long offset)
{
#ifdef CONFIG_MMU
#ifdef CONFIG_SUN3
	pgprot_val(prot) |= SUN3_PAGE_NOCACHE;
#else
	if (CPU_IS_020_OR_030)
		pgprot_val(prot) |= _PAGE_NOCACHE030;
	if (CPU_IS_040_OR_060) {
		pgprot_val(prot) &= _CACHEMASK040;
		/* Use no-cache mode, serialized */
		pgprot_val(prot) |= _PAGE_NOCACHE_S;
	}
#endif /* CONFIG_SUN3 */
#endif /* CONFIG_MMU */

	return prot;
}
#define pgprot_framebuffer pgprot_framebuffer

#include <asm-generic/fb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_FB_H_ */
