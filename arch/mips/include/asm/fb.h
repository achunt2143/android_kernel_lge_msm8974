#ifndef _ASM_FB_H_
#define _ASM_FB_H_

<<<<<<< HEAD
#include <linux/fb.h>
#include <linux/fs.h>
#include <asm/page.h>

static inline void fb_pgprotect(struct file *file, struct vm_area_struct *vma,
				unsigned long off)
{
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
}

static inline int fb_is_primary_device(struct fb_info *info)
{
	return 0;
}
=======
#include <asm/page.h>

static inline pgprot_t pgprot_framebuffer(pgprot_t prot,
					  unsigned long vm_start, unsigned long vm_end,
					  unsigned long offset)
{
	return pgprot_noncached(prot);
}
#define pgprot_framebuffer pgprot_framebuffer

/*
 * MIPS doesn't define __raw_ I/O macros, so the helpers
 * in <asm-generic/fb.h> don't generate fb_readq() and
 * fb_write(). We have to provide them here.
 *
 * TODO: Convert MIPS to generic I/O. The helpers below can
 *       then be removed.
 */
#ifdef CONFIG_64BIT
static inline u64 fb_readq(const volatile void __iomem *addr)
{
	return __raw_readq(addr);
}
#define fb_readq fb_readq

static inline void fb_writeq(u64 b, volatile void __iomem *addr)
{
	__raw_writeq(b, addr);
}
#define fb_writeq fb_writeq
#endif

#include <asm-generic/fb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_FB_H_ */
