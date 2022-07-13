<<<<<<< HEAD
#ifndef _M68KNOMMU_IO_H
#define _M68KNOMMU_IO_H

#ifdef __KERNEL__

#include <asm/virtconvert.h>

/*
 * These are for ISA/PCI shared memory _only_ and should never be used
 * on any other type of memory, including Zorro memory. They are meant to
 * access the bus in the bus byte order which is little-endian!.
 *
 * readX/writeX() are used to access memory mapped devices. On some
 * architectures the memory mapped IO stuff needs to be accessed
 * differently. On the m68k architecture, we just read/write the
 * memory location directly.
 */
/* ++roman: The assignments to temp. vars avoid that gcc sometimes generates
 * two accesses to memory, which may be undesirable for some devices.
 */

/*
 * swap functions are sometimes needed to interface little-endian hardware
 */
static inline unsigned short _swapw(volatile unsigned short v)
{
    return ((v << 8) | (v >> 8));
}

static inline unsigned int _swapl(volatile unsigned long v)
{
    return ((v << 24) | ((v & 0xff00) << 8) | ((v & 0xff0000) >> 8) | (v >> 24));
}

#define readb(addr) \
    ({ unsigned char __v = (*(volatile unsigned char *) (addr)); __v; })
#define readw(addr) \
    ({ unsigned short __v = (*(volatile unsigned short *) (addr)); __v; })
#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

#define readb_relaxed(addr) readb(addr)
#define readw_relaxed(addr) readw(addr)
#define readl_relaxed(addr) readl(addr)

#define writeb(b,addr) (void)((*(volatile unsigned char *) (addr)) = (b))
#define writew(b,addr) (void)((*(volatile unsigned short *) (addr)) = (b))
#define writel(b,addr) (void)((*(volatile unsigned int *) (addr)) = (b))

#define __raw_readb readb
#define __raw_readw readw
#define __raw_readl readl
#define __raw_writeb writeb
#define __raw_writew writew
#define __raw_writel writel

static inline void io_outsb(unsigned int addr, void *buf, int len)
{
	volatile unsigned char *ap = (volatile unsigned char *) addr;
	unsigned char *bp = (unsigned char *) buf;
	while (len--)
		*ap = *bp++;
}

static inline void io_outsw(unsigned int addr, void *buf, int len)
{
	volatile unsigned short *ap = (volatile unsigned short *) addr;
	unsigned short *bp = (unsigned short *) buf;
	while (len--)
		*ap = _swapw(*bp++);
}

static inline void io_outsl(unsigned int addr, void *buf, int len)
{
	volatile unsigned int *ap = (volatile unsigned int *) addr;
	unsigned int *bp = (unsigned int *) buf;
	while (len--)
		*ap = _swapl(*bp++);
}

static inline void io_insb(unsigned int addr, void *buf, int len)
{
	volatile unsigned char *ap = (volatile unsigned char *) addr;
	unsigned char *bp = (unsigned char *) buf;
	while (len--)
		*bp++ = *ap;
}

static inline void io_insw(unsigned int addr, void *buf, int len)
{
	volatile unsigned short *ap = (volatile unsigned short *) addr;
	unsigned short *bp = (unsigned short *) buf;
	while (len--)
		*bp++ = _swapw(*ap);
}

static inline void io_insl(unsigned int addr, void *buf, int len)
{
	volatile unsigned int *ap = (volatile unsigned int *) addr;
	unsigned int *bp = (unsigned int *) buf;
	while (len--)
		*bp++ = _swapl(*ap);
}

#define mmiowb()

/*
 *	make the short names macros so specific devices
 *	can override them as required
 */

#define memset_io(a,b,c)	memset((void *)(a),(b),(c))
#define memcpy_fromio(a,b,c)	memcpy((a),(void *)(b),(c))
#define memcpy_toio(a,b,c)	memcpy((void *)(a),(b),(c))

#define inb(addr)    readb(addr)
#define inw(addr)    readw(addr)
#define inl(addr)    readl(addr)
#define outb(x,addr) ((void) writeb(x,addr))
#define outw(x,addr) ((void) writew(x,addr))
#define outl(x,addr) ((void) writel(x,addr))

#define inb_p(addr)    inb(addr)
#define inw_p(addr)    inw(addr)
#define inl_p(addr)    inl(addr)
#define outb_p(x,addr) outb(x,addr)
#define outw_p(x,addr) outw(x,addr)
#define outl_p(x,addr) outl(x,addr)

#define outsb(a,b,l) io_outsb(a,b,l)
#define outsw(a,b,l) io_outsw(a,b,l)
#define outsl(a,b,l) io_outsl(a,b,l)

#define insb(a,b,l) io_insb(a,b,l)
#define insw(a,b,l) io_insw(a,b,l)
#define insl(a,b,l) io_insl(a,b,l)

#define IO_SPACE_LIMIT 0xffffffff


/* Values for nocacheflag and cmode */
#define IOMAP_FULL_CACHING		0
#define IOMAP_NOCACHE_SER		1
#define IOMAP_NOCACHE_NONSER		2
#define IOMAP_WRITETHROUGH		3

static inline void *__ioremap(unsigned long physaddr, unsigned long size, int cacheflag)
{
	return (void *) physaddr;
}
static inline void *ioremap(unsigned long physaddr, unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_NOCACHE_SER);
}
static inline void *ioremap_nocache(unsigned long physaddr, unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_NOCACHE_SER);
}
static inline void *ioremap_writethrough(unsigned long physaddr, unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_WRITETHROUGH);
}
static inline void *ioremap_fullcache(unsigned long physaddr, unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_FULL_CACHING);
}

#define	iounmap(addr)	do { } while(0)

/*
 * Convert a physical pointer to a virtual kernel pointer for /dev/mem
 * access
 */
#define xlate_dev_mem_ptr(p)	__va(p)

/*
 * Convert a virtual cached pointer to an uncached pointer
 */
#define xlate_dev_kmem_ptr(p)	p

#endif /* __KERNEL__ */

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _M68KNOMMU_IO_H
#define _M68KNOMMU_IO_H

/*
 * Convert a physical memory address into a IO memory address.
 * For us this is trivially a type cast.
 */
#define iomem(a)	((void __iomem *) (a))

/*
 * The non-MMU m68k and ColdFire IO and memory mapped hardware access
 * functions have always worked in CPU native endian. We need to define
 * that behavior here first before we include asm-generic/io.h.
 */
#define __raw_readb(addr) \
    ({ u8 __v = (*(__force volatile u8 *) (addr)); __v; })
#define __raw_readw(addr) \
    ({ u16 __v = (*(__force volatile u16 *) (addr)); __v; })
#define __raw_readl(addr) \
    ({ u32 __v = (*(__force volatile u32 *) (addr)); __v; })

#define __raw_writeb(b, addr) (void)((*(__force volatile u8 *) (addr)) = (b))
#define __raw_writew(b, addr) (void)((*(__force volatile u16 *) (addr)) = (b))
#define __raw_writel(b, addr) (void)((*(__force volatile u32 *) (addr)) = (b))

#if defined(CONFIG_COLDFIRE)
/*
 * For ColdFire platforms we may need to do some extra checks for what
 * type of address range we are accessing. Include the ColdFire platform
 * definitions so we can figure out if need to do something special.
 */
#include <asm/byteorder.h>
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#endif /* CONFIG_COLDFIRE */

#if defined(IOMEMBASE)
/*
 * The ColdFire SoC internal peripherals are mapped into virtual address
 * space using the ACR registers of the cache control unit. This means we
 * are using a 1:1 physical:virtual mapping for them. We can quickly
 * determine if we are accessing an internal peripheral device given the
 * physical or vitrual address using the same range check. This check logic
 * applies just the same of there is no MMU but something like a PCI bus
 * is present.
 */
static int __cf_internalio(unsigned long addr)
{
	return (addr >= IOMEMBASE) && (addr <= IOMEMBASE + IOMEMSIZE - 1);
}

static int cf_internalio(const volatile void __iomem *addr)
{
	return __cf_internalio((unsigned long) addr);
}

/*
 * We need to treat built-in peripherals and bus based address ranges
 * differently. Local built-in peripherals (and the ColdFire SoC parts
 * have quite a lot of them) are always native endian - which is big
 * endian on m68k/ColdFire. Bus based address ranges, like the PCI bus,
 * are accessed little endian - so we need to byte swap those.
 */
#define readw readw
static inline u16 readw(const volatile void __iomem *addr)
{
	if (cf_internalio(addr))
		return __raw_readw(addr);
	return swab16(__raw_readw(addr));
}

#define readl readl
static inline u32 readl(const volatile void __iomem *addr)
{
	if (cf_internalio(addr))
		return __raw_readl(addr);
	return swab32(__raw_readl(addr));
}

#define writew writew
static inline void writew(u16 value, volatile void __iomem *addr)
{
	if (cf_internalio(addr))
		__raw_writew(value, addr);
	else
		__raw_writew(swab16(value), addr);
}

#define writel writel
static inline void writel(u32 value, volatile void __iomem *addr)
{
	if (cf_internalio(addr))
		__raw_writel(value, addr);
	else
		__raw_writel(swab32(value), addr);
}

#else

#define readb __raw_readb
#define readw __raw_readw
#define readl __raw_readl
#define writeb __raw_writeb
#define writew __raw_writew
#define writel __raw_writel

#endif /* IOMEMBASE */

#if defined(CONFIG_PCI)
/*
 * Support for PCI bus access uses the asm-generic access functions.
 * We need to supply the base address and masks for the normal memory
 * and IO address space mappings.
 */
#define PCI_MEM_PA	0xf0000000		/* Host physical address */
#define PCI_MEM_BA	0xf0000000		/* Bus physical address */
#define PCI_MEM_SIZE	0x08000000		/* 128 MB */
#define PCI_MEM_MASK	(PCI_MEM_SIZE - 1)

#define PCI_IO_PA	0xf8000000		/* Host physical address */
#define PCI_IO_BA	0x00000000		/* Bus physical address */
#define PCI_IO_SIZE	0x00010000		/* 64k */
#define PCI_IO_MASK	(PCI_IO_SIZE - 1)

#define HAVE_ARCH_PIO_SIZE
#define PIO_OFFSET	0
#define PIO_MASK	0xffff
#define PIO_RESERVED	0x10000
#define PCI_IOBASE	((void __iomem *) PCI_IO_PA)
#define PCI_SPACE_LIMIT	PCI_IO_MASK
#endif /* CONFIG_PCI */

#include <asm/kmap.h>
#include <asm/virtconvert.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _M68KNOMMU_IO_H */
