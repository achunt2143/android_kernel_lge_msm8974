<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/arch/m68k/sun3/sun3dvma.c
 *
 * Copyright (C) 2000 Sam Creasey
 *
 * Contains common routines for sun3/sun3x DVMA management.
 */

<<<<<<< HEAD
=======
#include <linux/memblock.h>
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/list.h>

#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/dvma.h>

#undef DVMA_DEBUG

<<<<<<< HEAD
#ifdef CONFIG_SUN3X
extern void dvma_unmap_iommu(unsigned long baddr, int len);
#else
static inline void dvma_unmap_iommu(unsigned long a, int b)
{
}
#endif

#ifdef CONFIG_SUN3
extern void sun3_dvma_init(void);
#endif

static unsigned long iommu_use[IOMMU_TOTAL_ENTRIES];
=======
static unsigned long *iommu_use;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define dvma_index(baddr) ((baddr - DVMA_START) >> DVMA_PAGE_SHIFT)

#define dvma_entry_use(baddr)		(iommu_use[dvma_index(baddr)])

struct hole {
	unsigned long start;
	unsigned long end;
	unsigned long size;
	struct list_head list;
};

static struct list_head hole_list;
static struct list_head hole_cache;
static struct hole initholes[64];

#ifdef DVMA_DEBUG

static unsigned long dvma_allocs;
static unsigned long dvma_frees;
static unsigned long long dvma_alloc_bytes;
static unsigned long long dvma_free_bytes;

static void print_use(void)
{

	int i;
	int j = 0;

<<<<<<< HEAD
	printk("dvma entry usage:\n");
=======
	pr_info("dvma entry usage:\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for(i = 0; i < IOMMU_TOTAL_ENTRIES; i++) {
		if(!iommu_use[i])
			continue;

		j++;

<<<<<<< HEAD
		printk("dvma entry: %08lx len %08lx\n",
		       ( i << DVMA_PAGE_SHIFT) + DVMA_START,
		       iommu_use[i]);
	}

	printk("%d entries in use total\n", j);

	printk("allocation/free calls: %lu/%lu\n", dvma_allocs, dvma_frees);
	printk("allocation/free bytes: %Lx/%Lx\n", dvma_alloc_bytes,
	       dvma_free_bytes);
=======
		pr_info("dvma entry: %08x len %08lx\n",
			(i << DVMA_PAGE_SHIFT) + DVMA_START, iommu_use[i]);
	}

	pr_info("%d entries in use total\n", j);

	pr_info("allocation/free calls: %lu/%lu\n", dvma_allocs, dvma_frees);
	pr_info("allocation/free bytes: %Lx/%Lx\n", dvma_alloc_bytes,
		dvma_free_bytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void print_holes(struct list_head *holes)
{

	struct list_head *cur;
	struct hole *hole;

<<<<<<< HEAD
	printk("listing dvma holes\n");
=======
	pr_info("listing dvma holes\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each(cur, holes) {
		hole = list_entry(cur, struct hole, list);

		if((hole->start == 0) && (hole->end == 0) && (hole->size == 0))
			continue;

<<<<<<< HEAD
		printk("hole: start %08lx end %08lx size %08lx\n", hole->start, hole->end, hole->size);
	}

	printk("end of hole listing...\n");

=======
		pr_info("hole: start %08lx end %08lx size %08lx\n",
			hole->start, hole->end, hole->size);
	}

	pr_info("end of hole listing...\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* DVMA_DEBUG */

static inline int refill(void)
{

	struct hole *hole;
	struct hole *prev = NULL;
	struct list_head *cur;
	int ret = 0;

	list_for_each(cur, &hole_list) {
		hole = list_entry(cur, struct hole, list);

		if(!prev) {
			prev = hole;
			continue;
		}

		if(hole->end == prev->start) {
			hole->size += prev->size;
			hole->end = prev->end;
			list_move(&(prev->list), &hole_cache);
			ret++;
		}

	}

	return ret;
}

static inline struct hole *rmcache(void)
{
	struct hole *ret;

	if(list_empty(&hole_cache)) {
		if(!refill()) {
<<<<<<< HEAD
			printk("out of dvma hole cache!\n");
=======
			pr_crit("out of dvma hole cache!\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			BUG();
		}
	}

	ret = list_entry(hole_cache.next, struct hole, list);
	list_del(&(ret->list));

	return ret;

}

static inline unsigned long get_baddr(int len, unsigned long align)
{

	struct list_head *cur;
	struct hole *hole;

	if(list_empty(&hole_list)) {
#ifdef DVMA_DEBUG
<<<<<<< HEAD
		printk("out of dvma holes! (printing hole cache)\n");
=======
		pr_crit("out of dvma holes! (printing hole cache)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		print_holes(&hole_cache);
		print_use();
#endif
		BUG();
	}

	list_for_each(cur, &hole_list) {
		unsigned long newlen;

		hole = list_entry(cur, struct hole, list);

		if(align > DVMA_PAGE_SIZE)
			newlen = len + ((hole->end - len) & (align-1));
		else
			newlen = len;

		if(hole->size > newlen) {
			hole->end -= newlen;
			hole->size -= newlen;
			dvma_entry_use(hole->end) = newlen;
#ifdef DVMA_DEBUG
			dvma_allocs++;
			dvma_alloc_bytes += newlen;
#endif
			return hole->end;
		} else if(hole->size == newlen) {
			list_move(&(hole->list), &hole_cache);
			dvma_entry_use(hole->start) = newlen;
#ifdef DVMA_DEBUG
			dvma_allocs++;
			dvma_alloc_bytes += newlen;
#endif
			return hole->start;
		}

	}

<<<<<<< HEAD
	printk("unable to find dvma hole!\n");
=======
	pr_crit("unable to find dvma hole!\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG();
	return 0;
}

static inline int free_baddr(unsigned long baddr)
{

	unsigned long len;
	struct hole *hole;
	struct list_head *cur;
<<<<<<< HEAD
	unsigned long orig_baddr;

	orig_baddr = baddr;
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	len = dvma_entry_use(baddr);
	dvma_entry_use(baddr) = 0;
	baddr &= DVMA_PAGE_MASK;
	dvma_unmap_iommu(baddr, len);

#ifdef DVMA_DEBUG
	dvma_frees++;
	dvma_free_bytes += len;
#endif

	list_for_each(cur, &hole_list) {
		hole = list_entry(cur, struct hole, list);

		if(hole->end == baddr) {
			hole->end += len;
			hole->size += len;
			return 0;
		} else if(hole->start == (baddr + len)) {
			hole->start = baddr;
			hole->size += len;
			return 0;
		}

	}

	hole = rmcache();

	hole->start = baddr;
	hole->end = baddr + len;
	hole->size = len;

//	list_add_tail(&(hole->list), cur);
	list_add(&(hole->list), cur);

	return 0;

}

<<<<<<< HEAD
void dvma_init(void)
=======
void __init dvma_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{

	struct hole *hole;
	int i;

	INIT_LIST_HEAD(&hole_list);
	INIT_LIST_HEAD(&hole_cache);

	/* prepare the hole cache */
	for(i = 0; i < 64; i++)
		list_add(&(initholes[i].list), &hole_cache);

	hole = rmcache();
	hole->start = DVMA_START;
	hole->end = DVMA_END;
	hole->size = DVMA_SIZE;

	list_add(&(hole->list), &hole_list);

<<<<<<< HEAD
	memset(iommu_use, 0, sizeof(iommu_use));

	dvma_unmap_iommu(DVMA_START, DVMA_SIZE);

#ifdef CONFIG_SUN3
	sun3_dvma_init();
#endif

}

inline unsigned long dvma_map_align(unsigned long kaddr, int len, int align)
=======
	iommu_use = memblock_alloc(IOMMU_TOTAL_ENTRIES * sizeof(unsigned long),
				   SMP_CACHE_BYTES);
	if (!iommu_use)
		panic("%s: Failed to allocate %zu bytes\n", __func__,
		      IOMMU_TOTAL_ENTRIES * sizeof(unsigned long));

	dvma_unmap_iommu(DVMA_START, DVMA_SIZE);

	sun3_dvma_init();
}

unsigned long dvma_map_align(unsigned long kaddr, int len, int align)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{

	unsigned long baddr;
	unsigned long off;

	if(!len)
		len = 0x800;

	if(!kaddr || !len) {
<<<<<<< HEAD
//		printk("error: kaddr %lx len %x\n", kaddr, len);
=======
//		pr_err("error: kaddr %lx len %x\n", kaddr, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
//		*(int *)4 = 0;
		return 0;
	}

<<<<<<< HEAD
#ifdef DEBUG
	printk("dvma_map request %08lx bytes from %08lx\n",
	       len, kaddr);
#endif
=======
	pr_debug("dvma_map request %08x bytes from %08lx\n", len, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	off = kaddr & ~DVMA_PAGE_MASK;
	kaddr &= PAGE_MASK;
	len += off;
	len = ((len + (DVMA_PAGE_SIZE-1)) & DVMA_PAGE_MASK);

	if(align == 0)
		align = DVMA_PAGE_SIZE;
	else
		align = ((align + (DVMA_PAGE_SIZE-1)) & DVMA_PAGE_MASK);

	baddr = get_baddr(len, align);
<<<<<<< HEAD
//	printk("using baddr %lx\n", baddr);
=======
//	pr_info("using baddr %lx\n", baddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if(!dvma_map_iommu(kaddr, baddr, len))
		return (baddr + off);

<<<<<<< HEAD
	printk("dvma_map failed kaddr %lx baddr %lx len %x\n", kaddr, baddr, len);
=======
	pr_crit("dvma_map failed kaddr %lx baddr %lx len %x\n", kaddr, baddr,
	len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG();
	return 0;
}
EXPORT_SYMBOL(dvma_map_align);

void dvma_unmap(void *baddr)
{
	unsigned long addr;

	addr = (unsigned long)baddr;
	/* check if this is a vme mapping */
	if(!(addr & 0x00f00000))
		addr |= 0xf00000;

	free_baddr(addr);

	return;

}
EXPORT_SYMBOL(dvma_unmap);

void *dvma_malloc_align(unsigned long len, unsigned long align)
{
	unsigned long kaddr;
	unsigned long baddr;
	unsigned long vaddr;

	if(!len)
		return NULL;

<<<<<<< HEAD
#ifdef DEBUG
	printk("dvma_malloc request %lx bytes\n", len);
#endif
=======
	pr_debug("dvma_malloc request %lx bytes\n", len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	len = ((len + (DVMA_PAGE_SIZE-1)) & DVMA_PAGE_MASK);

        if((kaddr = __get_free_pages(GFP_ATOMIC, get_order(len))) == 0)
		return NULL;

	if((baddr = (unsigned long)dvma_map_align(kaddr, len, align)) == 0) {
		free_pages(kaddr, get_order(len));
		return NULL;
	}

	vaddr = dvma_btov(baddr);

	if(dvma_map_cpu(kaddr, vaddr, len) < 0) {
		dvma_unmap((void *)baddr);
		free_pages(kaddr, get_order(len));
		return NULL;
	}

<<<<<<< HEAD
#ifdef DEBUG
	printk("mapped %08lx bytes %08lx kern -> %08lx bus\n",
	       len, kaddr, baddr);
#endif
=======
	pr_debug("mapped %08lx bytes %08lx kern -> %08lx bus\n", len, kaddr,
		 baddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (void *)vaddr;

}
EXPORT_SYMBOL(dvma_malloc_align);

void dvma_free(void *vaddr)
{

	return;

}
EXPORT_SYMBOL(dvma_free);
