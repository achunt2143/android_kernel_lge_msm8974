<<<<<<< HEAD
#ifndef _SPARC_FB_H_
#define _SPARC_FB_H_
#include <linux/console.h>
#include <linux/fb.h>
#include <linux/fs.h>
#include <asm/page.h>
#include <asm/prom.h>

static inline void fb_pgprotect(struct file *file, struct vm_area_struct *vma,
				unsigned long off)
{
#ifdef CONFIG_SPARC64
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#endif
}

static inline int fb_is_primary_device(struct fb_info *info)
{
	struct device *dev = info->device;
	struct device_node *node;

	if (console_set_on_cmdline)
		return 0;

	node = dev->of_node;
	if (node &&
	    node == of_console_device)
		return 1;

	return 0;
}
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SPARC_FB_H_
#define _SPARC_FB_H_

#include <linux/io.h>

#include <asm/page.h>

struct fb_info;

#ifdef CONFIG_SPARC32
static inline pgprot_t pgprot_framebuffer(pgprot_t prot,
					  unsigned long vm_start, unsigned long vm_end,
					  unsigned long offset)
{
	return prot;
}
#define pgprot_framebuffer pgprot_framebuffer
#endif

int fb_is_primary_device(struct fb_info *info);
#define fb_is_primary_device fb_is_primary_device

static inline void fb_memcpy_fromio(void *to, const volatile void __iomem *from, size_t n)
{
	sbus_memcpy_fromio(to, from, n);
}
#define fb_memcpy_fromio fb_memcpy_fromio

static inline void fb_memcpy_toio(volatile void __iomem *to, const void *from, size_t n)
{
	sbus_memcpy_toio(to, from, n);
}
#define fb_memcpy_toio fb_memcpy_toio

static inline void fb_memset_io(volatile void __iomem *addr, int c, size_t n)
{
	sbus_memset_io(addr, c, n);
}
#define fb_memset fb_memset_io

#include <asm-generic/fb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _SPARC_FB_H_ */
