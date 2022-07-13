<<<<<<< HEAD
/* 
 * File...........: linux/include/asm-s390x/idals.h
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 *		    Martin Schwidefsky <schwidefsky@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 2000a
 
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 *		    Martin Schwidefsky <schwidefsky@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * Copyright IBM Corp. 2000
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * History of changes
 * 07/24/00 new file
 * 05/04/02 code restructuring.
 */

#ifndef _S390_IDALS_H
#define _S390_IDALS_H

#include <linux/errno.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <asm/cio.h>
#include <asm/uaccess.h>

#ifdef __s390x__
#define IDA_SIZE_LOG 12 /* 11 for 2k , 12 for 4k */
#else
#define IDA_SIZE_LOG 11 /* 11 for 2k , 12 for 4k */
#endif
#define IDA_BLOCK_SIZE (1L<<IDA_SIZE_LOG)
=======
#include <linux/uaccess.h>
#include <asm/dma-types.h>
#include <asm/cio.h>

#define IDA_SIZE_SHIFT		12
#define IDA_BLOCK_SIZE		(1UL << IDA_SIZE_SHIFT)

#define IDA_2K_SIZE_SHIFT	11
#define IDA_2K_BLOCK_SIZE	(1UL << IDA_2K_SIZE_SHIFT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Test if an address/length pair needs an idal list.
 */
<<<<<<< HEAD
static inline int
idal_is_needed(void *vaddr, unsigned int length)
{
#ifdef __s390x__
	return ((__pa(vaddr) + length - 1) >> 31) != 0;
#else
	return 0;
#endif
}

=======
static inline bool idal_is_needed(void *vaddr, unsigned int length)
{
	dma64_t paddr = virt_to_dma64(vaddr);

	return (((__force unsigned long)(paddr) + length - 1) >> 31) != 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Return the number of idal words needed for an address/length pair.
 */
static inline unsigned int idal_nr_words(void *vaddr, unsigned int length)
{
<<<<<<< HEAD
	return ((__pa(vaddr) & (IDA_BLOCK_SIZE-1)) + length +
		(IDA_BLOCK_SIZE-1)) >> IDA_SIZE_LOG;
=======
	unsigned int cidaw;

	cidaw = (unsigned long)vaddr & (IDA_BLOCK_SIZE - 1);
	cidaw += length + IDA_BLOCK_SIZE - 1;
	cidaw >>= IDA_SIZE_SHIFT;
	return cidaw;
}

/*
 * Return the number of 2K IDA words needed for an address/length pair.
 */
static inline unsigned int idal_2k_nr_words(void *vaddr, unsigned int length)
{
	unsigned int cidaw;

	cidaw = (unsigned long)vaddr & (IDA_2K_BLOCK_SIZE - 1);
	cidaw += length + IDA_2K_BLOCK_SIZE - 1;
	cidaw >>= IDA_2K_SIZE_SHIFT;
	return cidaw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Create the list of idal words for an address/length pair.
 */
<<<<<<< HEAD
static inline unsigned long *idal_create_words(unsigned long *idaws,
					       void *vaddr, unsigned int length)
{
	unsigned long paddr;
	unsigned int cidaw;

	paddr = __pa(vaddr);
	cidaw = ((paddr & (IDA_BLOCK_SIZE-1)) + length + 
		 (IDA_BLOCK_SIZE-1)) >> IDA_SIZE_LOG;
	*idaws++ = paddr;
	paddr &= -IDA_BLOCK_SIZE;
	while (--cidaw > 0) {
		paddr += IDA_BLOCK_SIZE;
=======
static inline dma64_t *idal_create_words(dma64_t *idaws, void *vaddr, unsigned int length)
{
	dma64_t paddr = virt_to_dma64(vaddr);
	unsigned int cidaw;

	*idaws++ = paddr;
	cidaw = idal_nr_words(vaddr, length);
	paddr = dma64_and(paddr, -IDA_BLOCK_SIZE);
	while (--cidaw > 0) {
		paddr = dma64_add(paddr, IDA_BLOCK_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*idaws++ = paddr;
	}
	return idaws;
}

/*
 * Sets the address of the data in CCW.
 * If necessary it allocates an IDAL and sets the appropriate flags.
 */
<<<<<<< HEAD
static inline int
set_normalized_cda(struct ccw1 * ccw, void *vaddr)
{
#ifdef __s390x__
	unsigned int nridaws;
	unsigned long *idal;
=======
static inline int set_normalized_cda(struct ccw1 *ccw, void *vaddr)
{
	unsigned int nridaws;
	dma64_t *idal;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ccw->flags & CCW_FLAG_IDA)
		return -EINVAL;
	nridaws = idal_nr_words(vaddr, ccw->count);
	if (nridaws > 0) {
<<<<<<< HEAD
		idal = kmalloc(nridaws * sizeof(unsigned long),
			       GFP_ATOMIC | GFP_DMA );
		if (idal == NULL)
=======
		idal = kcalloc(nridaws, sizeof(*idal), GFP_ATOMIC | GFP_DMA);
		if (!idal)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENOMEM;
		idal_create_words(idal, vaddr, ccw->count);
		ccw->flags |= CCW_FLAG_IDA;
		vaddr = idal;
	}
<<<<<<< HEAD
#endif
	ccw->cda = (__u32)(unsigned long) vaddr;
=======
	ccw->cda = virt_to_dma32(vaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Releases any allocated IDAL related to the CCW.
 */
<<<<<<< HEAD
static inline void
clear_normalized_cda(struct ccw1 * ccw)
{
#ifdef __s390x__
	if (ccw->flags & CCW_FLAG_IDA) {
		kfree((void *)(unsigned long) ccw->cda);
		ccw->flags &= ~CCW_FLAG_IDA;
	}
#endif
=======
static inline void clear_normalized_cda(struct ccw1 *ccw)
{
	if (ccw->flags & CCW_FLAG_IDA) {
		kfree(dma32_to_virt(ccw->cda));
		ccw->flags &= ~CCW_FLAG_IDA;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ccw->cda = 0;
}

/*
 * Idal buffer extension
 */
struct idal_buffer {
	size_t size;
	size_t page_order;
<<<<<<< HEAD
	void *data[0];
=======
	dma64_t data[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Allocate an idal buffer
 */
<<<<<<< HEAD
static inline struct idal_buffer *
idal_buffer_alloc(size_t size, int page_order)
{
	struct idal_buffer *ib;
	int nr_chunks, nr_ptrs, i;

	nr_ptrs = (size + IDA_BLOCK_SIZE - 1) >> IDA_SIZE_LOG;
	nr_chunks = (4096 << page_order) >> IDA_SIZE_LOG;
	ib = kmalloc(sizeof(struct idal_buffer) + nr_ptrs*sizeof(void *),
		     GFP_DMA | GFP_KERNEL);
	if (ib == NULL)
=======
static inline struct idal_buffer *idal_buffer_alloc(size_t size, int page_order)
{
	int nr_chunks, nr_ptrs, i;
	struct idal_buffer *ib;
	void *vaddr;

	nr_ptrs = (size + IDA_BLOCK_SIZE - 1) >> IDA_SIZE_SHIFT;
	nr_chunks = (PAGE_SIZE << page_order) >> IDA_SIZE_SHIFT;
	ib = kmalloc(struct_size(ib, data, nr_ptrs), GFP_DMA | GFP_KERNEL);
	if (!ib)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-ENOMEM);
	ib->size = size;
	ib->page_order = page_order;
	for (i = 0; i < nr_ptrs; i++) {
<<<<<<< HEAD
		if ((i & (nr_chunks - 1)) != 0) {
			ib->data[i] = ib->data[i-1] + IDA_BLOCK_SIZE;
			continue;
		}
		ib->data[i] = (void *)
			__get_free_pages(GFP_KERNEL, page_order);
		if (ib->data[i] != NULL)
			continue;
		// Not enough memory
		while (i >= nr_chunks) {
			i -= nr_chunks;
			free_pages((unsigned long) ib->data[i],
				   ib->page_order);
		}
		kfree(ib);
		return ERR_PTR(-ENOMEM);
	}
	return ib;
=======
		if (i & (nr_chunks - 1)) {
			ib->data[i] = dma64_add(ib->data[i - 1], IDA_BLOCK_SIZE);
			continue;
		}
		vaddr = (void *)__get_free_pages(GFP_KERNEL, page_order);
		if (!vaddr)
			goto error;
		ib->data[i] = virt_to_dma64(vaddr);
	}
	return ib;
error:
	while (i >= nr_chunks) {
		i -= nr_chunks;
		vaddr = dma64_to_virt(ib->data[i]);
		free_pages((unsigned long)vaddr, ib->page_order);
	}
	kfree(ib);
	return ERR_PTR(-ENOMEM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Free an idal buffer.
 */
<<<<<<< HEAD
static inline void
idal_buffer_free(struct idal_buffer *ib)
{
	int nr_chunks, nr_ptrs, i;

	nr_ptrs = (ib->size + IDA_BLOCK_SIZE - 1) >> IDA_SIZE_LOG;
	nr_chunks = (4096 << ib->page_order) >> IDA_SIZE_LOG;
	for (i = 0; i < nr_ptrs; i += nr_chunks)
		free_pages((unsigned long) ib->data[i], ib->page_order);
=======
static inline void idal_buffer_free(struct idal_buffer *ib)
{
	int nr_chunks, nr_ptrs, i;
	void *vaddr;

	nr_ptrs = (ib->size + IDA_BLOCK_SIZE - 1) >> IDA_SIZE_SHIFT;
	nr_chunks = (PAGE_SIZE << ib->page_order) >> IDA_SIZE_SHIFT;
	for (i = 0; i < nr_ptrs; i += nr_chunks) {
		vaddr = dma64_to_virt(ib->data[i]);
		free_pages((unsigned long)vaddr, ib->page_order);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(ib);
}

/*
 * Test if a idal list is really needed.
 */
<<<<<<< HEAD
static inline int
__idal_buffer_is_needed(struct idal_buffer *ib)
{
#ifdef __s390x__
	return ib->size > (4096ul << ib->page_order) ||
		idal_is_needed(ib->data[0], ib->size);
#else
	return ib->size > (4096ul << ib->page_order);
#endif
=======
static inline bool __idal_buffer_is_needed(struct idal_buffer *ib)
{
	if (ib->size > (PAGE_SIZE << ib->page_order))
		return true;
	return idal_is_needed(dma64_to_virt(ib->data[0]), ib->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set channel data address to idal buffer.
 */
<<<<<<< HEAD
static inline void
idal_buffer_set_cda(struct idal_buffer *ib, struct ccw1 *ccw)
{
	if (__idal_buffer_is_needed(ib)) {
		// setup idals;
		ccw->cda = (u32)(addr_t) ib->data;
		ccw->flags |= CCW_FLAG_IDA;
	} else
		// we do not need idals - use direct addressing
		ccw->cda = (u32)(addr_t) ib->data[0];
=======
static inline void idal_buffer_set_cda(struct idal_buffer *ib, struct ccw1 *ccw)
{
	void *vaddr;

	if (__idal_buffer_is_needed(ib)) {
		/* Setup idals */
		ccw->cda = virt_to_dma32(ib->data);
		ccw->flags |= CCW_FLAG_IDA;
	} else {
		/*
		 * No idals needed - use direct addressing. Convert from
		 * dma64_t to virt and then to dma32_t only because of type
		 * checking. The physical address is known to be below 2GB.
		 */
		vaddr = dma64_to_virt(ib->data[0]);
		ccw->cda = virt_to_dma32(vaddr);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ccw->count = ib->size;
}

/*
 * Copy count bytes from an idal buffer to user memory
 */
<<<<<<< HEAD
static inline size_t
idal_buffer_to_user(struct idal_buffer *ib, void __user *to, size_t count)
{
	size_t left;
=======
static inline size_t idal_buffer_to_user(struct idal_buffer *ib, void __user *to, size_t count)
{
	size_t left;
	void *vaddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	BUG_ON(count > ib->size);
	for (i = 0; count > IDA_BLOCK_SIZE; i++) {
<<<<<<< HEAD
		left = copy_to_user(to, ib->data[i], IDA_BLOCK_SIZE);
		if (left)
			return left + count - IDA_BLOCK_SIZE;
		to = (void __user *) to + IDA_BLOCK_SIZE;
		count -= IDA_BLOCK_SIZE;
	}
	return copy_to_user(to, ib->data[i], count);
=======
		vaddr = dma64_to_virt(ib->data[i]);
		left = copy_to_user(to, vaddr, IDA_BLOCK_SIZE);
		if (left)
			return left + count - IDA_BLOCK_SIZE;
		to = (void __user *)to + IDA_BLOCK_SIZE;
		count -= IDA_BLOCK_SIZE;
	}
	vaddr = dma64_to_virt(ib->data[i]);
	return copy_to_user(to, vaddr, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Copy count bytes from user memory to an idal buffer
 */
<<<<<<< HEAD
static inline size_t
idal_buffer_from_user(struct idal_buffer *ib, const void __user *from, size_t count)
{
	size_t left;
=======
static inline size_t idal_buffer_from_user(struct idal_buffer *ib, const void __user *from, size_t count)
{
	size_t left;
	void *vaddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	BUG_ON(count > ib->size);
	for (i = 0; count > IDA_BLOCK_SIZE; i++) {
<<<<<<< HEAD
		left = copy_from_user(ib->data[i], from, IDA_BLOCK_SIZE);
		if (left)
			return left + count - IDA_BLOCK_SIZE;
		from = (void __user *) from + IDA_BLOCK_SIZE;
		count -= IDA_BLOCK_SIZE;
	}
	return copy_from_user(ib->data[i], from, count);
=======
		vaddr = dma64_to_virt(ib->data[i]);
		left = copy_from_user(vaddr, from, IDA_BLOCK_SIZE);
		if (left)
			return left + count - IDA_BLOCK_SIZE;
		from = (void __user *)from + IDA_BLOCK_SIZE;
		count -= IDA_BLOCK_SIZE;
	}
	vaddr = dma64_to_virt(ib->data[i]);
	return copy_from_user(vaddr, from, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif
