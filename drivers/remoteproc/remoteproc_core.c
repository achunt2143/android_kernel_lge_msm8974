<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Remote Processor Framework
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 *
 * Ohad Ben-Cohen <ohad@wizery.com>
 * Brian Swetland <swetland@google.com>
 * Mark Grosen <mgrosen@ti.com>
 * Fernando Guzman Lugo <fernando.lugo@ti.com>
 * Suman Anna <s-anna@ti.com>
 * Robert Tivy <rtivy@ti.com>
 * Armando Uribe De Leon <x0095078@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt)    "%s: " fmt, __func__

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
=======
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/panic_notifier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/dma-mapping.h>
#include <linux/firmware.h>
#include <linux/string.h>
#include <linux/debugfs.h>
<<<<<<< HEAD
#include <linux/remoteproc.h>
#include <linux/iommu.h>
#include <linux/klist.h>
#include <linux/elf.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_ring.h>
#include <asm/byteorder.h>

#include "remoteproc_internal.h"

static void klist_rproc_get(struct klist_node *n);
static void klist_rproc_put(struct klist_node *n);

/*
 * klist of the available remote processors.
 *
 * We need this in order to support name-based lookups (needed by the
 * rproc_get_by_name()).
 *
 * That said, we don't use rproc_get_by_name() at this point.
 * The use cases that do require its existence should be
 * scrutinized, and hopefully migrated to rproc_boot() using device-based
 * binding.
 *
 * If/when this materializes, we could drop the klist (and the by_name
 * API).
 */
static DEFINE_KLIST(rprocs, klist_rproc_get, klist_rproc_put);

typedef int (*rproc_handle_resources_t)(struct rproc *rproc,
				struct resource_table *table, int len);
typedef int (*rproc_handle_resource_t)(struct rproc *rproc, void *, int avail);
=======
#include <linux/rculist.h>
#include <linux/remoteproc.h>
#include <linux/iommu.h>
#include <linux/idr.h>
#include <linux/elf.h>
#include <linux/crc32.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_ring.h>
#include <asm/byteorder.h>
#include <linux/platform_device.h>

#include "remoteproc_internal.h"

#define HIGH_BITS_MASK 0xFFFFFFFF00000000ULL

static DEFINE_MUTEX(rproc_list_mutex);
static LIST_HEAD(rproc_list);
static struct notifier_block rproc_panic_nb;

typedef int (*rproc_handle_resource_t)(struct rproc *rproc,
				 void *, int offset, int avail);

static int rproc_alloc_carveout(struct rproc *rproc,
				struct rproc_mem_entry *mem);
static int rproc_release_carveout(struct rproc *rproc,
				  struct rproc_mem_entry *mem);

/* Unique indices for remoteproc devices */
static DEFINE_IDA(rproc_dev_index);
static struct workqueue_struct *rproc_recovery_wq;

static const char * const rproc_crash_names[] = {
	[RPROC_MMUFAULT]	= "mmufault",
	[RPROC_WATCHDOG]	= "watchdog",
	[RPROC_FATAL_ERROR]	= "fatal error",
};

/* translate rproc_crash_type to string */
static const char *rproc_crash_to_string(enum rproc_crash_type type)
{
	if (type < ARRAY_SIZE(rproc_crash_names))
		return rproc_crash_names[type];
	return "unknown";
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the IOMMU fault handler we register with the IOMMU API
 * (when relevant; not all remote processors access memory through
 * an IOMMU).
 *
 * IOMMU core will invoke this handler whenever the remote processor
 * will try to access an unmapped device address.
<<<<<<< HEAD
 *
 * Currently this is mostly a stub, but it will be later used to trigger
 * the recovery of the remote processor.
 */
static int rproc_iommu_fault(struct iommu_domain *domain, struct device *dev,
		unsigned long iova, int flags, void *token)
{
	dev_err(dev, "iommu fault: da 0x%lx flags 0x%x\n", iova, flags);

	/*
	 * Let the iommu core know we're not really handling this fault;
	 * we just plan to use this as a recovery trigger.
=======
 */
static int rproc_iommu_fault(struct iommu_domain *domain, struct device *dev,
			     unsigned long iova, int flags, void *token)
{
	struct rproc *rproc = token;

	dev_err(dev, "iommu fault: da 0x%lx flags 0x%x\n", iova, flags);

	rproc_report_crash(rproc, RPROC_MMUFAULT);

	/*
	 * Let the iommu core know we're not really handling this fault;
	 * we just used it as a recovery trigger.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	return -ENOSYS;
}

static int rproc_enable_iommu(struct rproc *rproc)
{
	struct iommu_domain *domain;
<<<<<<< HEAD
	struct device *dev = rproc->dev;
	int ret;

	/*
	 * We currently use iommu_present() to decide if an IOMMU
	 * setup is needed.
	 *
	 * This works for simple cases, but will easily fail with
	 * platforms that do have an IOMMU, but not for this specific
	 * rproc.
	 *
	 * This will be easily solved by introducing hw capabilities
	 * that will be set by the remoteproc driver.
	 */
	if (!iommu_present(dev->bus)) {
		dev_dbg(dev, "iommu not found\n");
=======
	struct device *dev = rproc->dev.parent;
	int ret;

	if (!rproc->has_iommu) {
		dev_dbg(dev, "iommu not present\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	domain = iommu_domain_alloc(dev->bus);
	if (!domain) {
		dev_err(dev, "can't alloc iommu domain\n");
		return -ENOMEM;
	}

	iommu_set_fault_handler(domain, rproc_iommu_fault, rproc);

	ret = iommu_attach_device(domain, dev);
	if (ret) {
		dev_err(dev, "can't attach iommu device: %d\n", ret);
		goto free_domain;
	}

	rproc->domain = domain;

	return 0;

free_domain:
	iommu_domain_free(domain);
	return ret;
}

static void rproc_disable_iommu(struct rproc *rproc)
{
	struct iommu_domain *domain = rproc->domain;
<<<<<<< HEAD
	struct device *dev = rproc->dev;
=======
	struct device *dev = rproc->dev.parent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!domain)
		return;

	iommu_detach_device(domain, dev);
	iommu_domain_free(domain);
<<<<<<< HEAD

	return;
}

/*
 * Some remote processors will ask us to allocate them physically contiguous
 * memory regions (which we call "carveouts"), and map them to specific
 * device addresses (which are hardcoded in the firmware).
=======
}

phys_addr_t rproc_va_to_pa(void *cpu_addr)
{
	/*
	 * Return physical address according to virtual address location
	 * - in vmalloc: if region ioremapped or defined as dma_alloc_coherent
	 * - in kernel: if region allocated in generic dma memory pool
	 */
	if (is_vmalloc_addr(cpu_addr)) {
		return page_to_phys(vmalloc_to_page(cpu_addr)) +
				    offset_in_page(cpu_addr);
	}

	WARN_ON(!virt_addr_valid(cpu_addr));
	return virt_to_phys(cpu_addr);
}
EXPORT_SYMBOL(rproc_va_to_pa);

/**
 * rproc_da_to_va() - lookup the kernel virtual address for a remoteproc address
 * @rproc: handle of a remote processor
 * @da: remoteproc device address to translate
 * @len: length of the memory region @da is pointing to
 * @is_iomem: optional pointer filled in to indicate if @da is iomapped memory
 *
 * Some remote processors will ask us to allocate them physically contiguous
 * memory regions (which we call "carveouts"), and map them to specific
 * device addresses (which are hardcoded in the firmware). They may also have
 * dedicated memory regions internal to the processors, and use them either
 * exclusively or alongside carveouts.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * They may then ask us to copy objects into specific device addresses (e.g.
 * code/data sections) or expose us certain symbols in other device address
 * (e.g. their trace buffer).
 *
<<<<<<< HEAD
 * This function is an internal helper with which we can go over the allocated
 * carveouts and translate specific device address to kernel virtual addresses
 * so we can access the referenced memory.
 *
 * Note: phys_to_virt(iommu_iova_to_phys(rproc->domain, da)) will work too,
 * but only on kernel direct mapped RAM memory. Instead, we're just using
 * here the output of the DMA API, which should be more correct.
 */
static void *rproc_da_to_va(struct rproc *rproc, u64 da, int len)
=======
 * This function is a helper function with which we can go over the allocated
 * carveouts and translate specific device addresses to kernel virtual addresses
 * so we can access the referenced memory. This function also allows to perform
 * translations on the internal remoteproc memory regions through a platform
 * implementation specific da_to_va ops, if present.
 *
 * Note: phys_to_virt(iommu_iova_to_phys(rproc->domain, da)) will work too,
 * but only on kernel direct mapped RAM memory. Instead, we're just using
 * here the output of the DMA API for the carveouts, which should be more
 * correct.
 *
 * Return: a valid kernel address on success or NULL on failure
 */
void *rproc_da_to_va(struct rproc *rproc, u64 da, size_t len, bool *is_iomem)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rproc_mem_entry *carveout;
	void *ptr = NULL;

<<<<<<< HEAD
	list_for_each_entry(carveout, &rproc->carveouts, node) {
		int offset = da - carveout->da;

=======
	if (rproc->ops->da_to_va) {
		ptr = rproc->ops->da_to_va(rproc, da, len, is_iomem);
		if (ptr)
			goto out;
	}

	list_for_each_entry(carveout, &rproc->carveouts, node) {
		int offset = da - carveout->da;

		/*  Verify that carveout is allocated */
		if (!carveout->va)
			continue;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* try next carveout if da is too small */
		if (offset < 0)
			continue;

		/* try next carveout if da is too large */
		if (offset + len > carveout->len)
			continue;

		ptr = carveout->va + offset;

<<<<<<< HEAD
		break;
	}

	return ptr;
}

/**
 * rproc_load_segments() - load firmware segments to memory
 * @rproc: remote processor which will be booted using these fw segments
 * @elf_data: the content of the ELF firmware image
 * @len: firmware size (in bytes)
 *
 * This function loads the firmware segments to memory, where the remote
 * processor expects them.
 *
 * Some remote processors will expect their code and data to be placed
 * in specific device addresses, and can't have them dynamically assigned.
 *
 * We currently support only those kind of remote processors, and expect
 * the program header's paddr member to contain those addresses. We then go
 * through the physically contiguous "carveout" memory regions which we
 * allocated (and mapped) earlier on behalf of the remote processor,
 * and "translate" device address to kernel addresses, so we can copy the
 * segments where they are expected.
 *
 * Currently we only support remote processors that required carveout
 * allocations and got them mapped onto their iommus. Some processors
 * might be different: they might not have iommus, and would prefer to
 * directly allocate memory for every segment/resource. This is not yet
 * supported, though.
 */
static int
rproc_load_segments(struct rproc *rproc, const u8 *elf_data, size_t len)
{
	struct device *dev = rproc->dev;
	struct elf32_hdr *ehdr;
	struct elf32_phdr *phdr;
	int i, ret = 0;

	ehdr = (struct elf32_hdr *)elf_data;
	phdr = (struct elf32_phdr *)(elf_data + ehdr->e_phoff);

	/* go through the available ELF segments */
	for (i = 0; i < ehdr->e_phnum; i++, phdr++) {
		u32 da = phdr->p_paddr;
		u32 memsz = phdr->p_memsz;
		u32 filesz = phdr->p_filesz;
		u32 offset = phdr->p_offset;
		void *ptr;

		if (phdr->p_type != PT_LOAD)
			continue;

		dev_dbg(dev, "phdr: type %d da 0x%x memsz 0x%x filesz 0x%x\n",
					phdr->p_type, da, memsz, filesz);

		if (filesz > memsz) {
			dev_err(dev, "bad phdr filesz 0x%x memsz 0x%x\n",
							filesz, memsz);
			ret = -EINVAL;
			break;
		}

		if (offset + filesz > len) {
			dev_err(dev, "truncated fw: need 0x%x avail 0x%zx\n",
					offset + filesz, len);
			ret = -EINVAL;
			break;
		}

		/* grab the kernel address for this device address */
		ptr = rproc_da_to_va(rproc, da, memsz);
		if (!ptr) {
			dev_err(dev, "bad phdr da 0x%x mem 0x%x\n", da, memsz);
			ret = -EINVAL;
			break;
		}

		/* put the segment where the remote processor expects it */
		if (phdr->p_filesz)
			memcpy(ptr, elf_data + phdr->p_offset, filesz);

		/*
		 * Zero out remaining memory for this segment.
		 *
		 * This isn't strictly required since dma_alloc_coherent already
		 * did this for us. albeit harmless, we may consider removing
		 * this.
		 */
		if (memsz > filesz)
			memset(ptr + filesz, 0, memsz - filesz);
	}

	return ret;
}

static int
__rproc_handle_vring(struct rproc_vdev *rvdev, struct fw_rsc_vdev *rsc, int i)
{
	struct rproc *rproc = rvdev->rproc;
	struct device *dev = rproc->dev;
	struct fw_rsc_vdev_vring *vring = &rsc->vring[i];
	dma_addr_t dma;
	void *va;
	int ret, size, notifyid;

	dev_dbg(dev, "vdev rsc: vring%d: da %x, qsz %d, align %d\n",
				i, vring->da, vring->num, vring->align);

	/* make sure reserved bytes are zeroes */
	if (vring->reserved) {
		dev_err(dev, "vring rsc has non zero reserved bytes\n");
		return -EINVAL;
	}

	/* verify queue size and vring alignment are sane */
	if (!vring->num || !vring->align) {
		dev_err(dev, "invalid qsz (%d) or alignment (%d)\n",
						vring->num, vring->align);
		return -EINVAL;
	}

	/* actual size of vring (in bytes) */
	size = PAGE_ALIGN(vring_size(vring->num, vring->align));

	if (!idr_pre_get(&rproc->notifyids, GFP_KERNEL)) {
		dev_err(dev, "idr_pre_get failed\n");
		return -ENOMEM;
	}

	/*
	 * Allocate non-cacheable memory for the vring. In the future
	 * this call will also configure the IOMMU for us
	 */
	va = dma_alloc_coherent(dev, size, &dma, GFP_KERNEL);
	if (!va) {
		dev_err(dev, "dma_alloc_coherent failed\n");
		return -EINVAL;
	}

	/* assign an rproc-wide unique index for this vring */
	/* TODO: assign a notifyid for rvdev updates as well */
	ret = idr_get_new(&rproc->notifyids, &rvdev->vring[i], &notifyid);
	if (ret) {
		dev_err(dev, "idr_get_new failed: %d\n", ret);
		dma_free_coherent(dev, size, va, dma);
		return ret;
	}

	/* let the rproc know the da and notifyid of this vring */
	/* TODO: expose this to remote processor */
	vring->da = dma;
	vring->notifyid = notifyid;

	dev_dbg(dev, "vring%d: va %p dma %x size %x idr %d\n", i, va,
					dma, size, notifyid);

	rvdev->vring[i].len = vring->num;
	rvdev->vring[i].align = vring->align;
	rvdev->vring[i].va = va;
	rvdev->vring[i].dma = dma;
	rvdev->vring[i].notifyid = notifyid;
	rvdev->vring[i].rvdev = rvdev;

	return 0;
}

static void __rproc_free_vrings(struct rproc_vdev *rvdev, int i)
{
	struct rproc *rproc = rvdev->rproc;

	for (i--; i >= 0; i--) {
		struct rproc_vring *rvring = &rvdev->vring[i];
		int size = PAGE_ALIGN(vring_size(rvring->len, rvring->align));

		dma_free_coherent(rproc->dev, size, rvring->va, rvring->dma);
		idr_remove(&rproc->notifyids, rvring->notifyid);
	}
}

/**
 * rproc_handle_vdev() - handle a vdev fw resource
 * @rproc: the remote processor
 * @rsc: the vring resource descriptor
=======
		if (is_iomem)
			*is_iomem = carveout->is_iomem;

		break;
	}

out:
	return ptr;
}
EXPORT_SYMBOL(rproc_da_to_va);

/**
 * rproc_find_carveout_by_name() - lookup the carveout region by a name
 * @rproc: handle of a remote processor
 * @name: carveout name to find (format string)
 * @...: optional parameters matching @name string
 *
 * Platform driver has the capability to register some pre-allacoted carveout
 * (physically contiguous memory regions) before rproc firmware loading and
 * associated resource table analysis. These regions may be dedicated memory
 * regions internal to the coprocessor or specified DDR region with specific
 * attributes
 *
 * This function is a helper function with which we can go over the
 * allocated carveouts and return associated region characteristics like
 * coprocessor address, length or processor virtual address.
 *
 * Return: a valid pointer on carveout entry on success or NULL on failure.
 */
__printf(2, 3)
struct rproc_mem_entry *
rproc_find_carveout_by_name(struct rproc *rproc, const char *name, ...)
{
	va_list args;
	char _name[32];
	struct rproc_mem_entry *carveout, *mem = NULL;

	if (!name)
		return NULL;

	va_start(args, name);
	vsnprintf(_name, sizeof(_name), name, args);
	va_end(args);

	list_for_each_entry(carveout, &rproc->carveouts, node) {
		/* Compare carveout and requested names */
		if (!strcmp(carveout->name, _name)) {
			mem = carveout;
			break;
		}
	}

	return mem;
}

/**
 * rproc_check_carveout_da() - Check specified carveout da configuration
 * @rproc: handle of a remote processor
 * @mem: pointer on carveout to check
 * @da: area device address
 * @len: associated area size
 *
 * This function is a helper function to verify requested device area (couple
 * da, len) is part of specified carveout.
 * If da is not set (defined as FW_RSC_ADDR_ANY), only requested length is
 * checked.
 *
 * Return: 0 if carveout matches request else error
 */
static int rproc_check_carveout_da(struct rproc *rproc,
				   struct rproc_mem_entry *mem, u32 da, u32 len)
{
	struct device *dev = &rproc->dev;
	int delta;

	/* Check requested resource length */
	if (len > mem->len) {
		dev_err(dev, "Registered carveout doesn't fit len request\n");
		return -EINVAL;
	}

	if (da != FW_RSC_ADDR_ANY && mem->da == FW_RSC_ADDR_ANY) {
		/* Address doesn't match registered carveout configuration */
		return -EINVAL;
	} else if (da != FW_RSC_ADDR_ANY && mem->da != FW_RSC_ADDR_ANY) {
		delta = da - mem->da;

		/* Check requested resource belongs to registered carveout */
		if (delta < 0) {
			dev_err(dev,
				"Registered carveout doesn't fit da request\n");
			return -EINVAL;
		}

		if (delta + len > mem->len) {
			dev_err(dev,
				"Registered carveout doesn't fit len request\n");
			return -EINVAL;
		}
	}

	return 0;
}

int rproc_alloc_vring(struct rproc_vdev *rvdev, int i)
{
	struct rproc *rproc = rvdev->rproc;
	struct device *dev = &rproc->dev;
	struct rproc_vring *rvring = &rvdev->vring[i];
	struct fw_rsc_vdev *rsc;
	int ret, notifyid;
	struct rproc_mem_entry *mem;
	size_t size;

	/* actual size of vring (in bytes) */
	size = PAGE_ALIGN(vring_size(rvring->num, rvring->align));

	rsc = (void *)rproc->table_ptr + rvdev->rsc_offset;

	/* Search for pre-registered carveout */
	mem = rproc_find_carveout_by_name(rproc, "vdev%dvring%d", rvdev->index,
					  i);
	if (mem) {
		if (rproc_check_carveout_da(rproc, mem, rsc->vring[i].da, size))
			return -ENOMEM;
	} else {
		/* Register carveout in list */
		mem = rproc_mem_entry_init(dev, NULL, 0,
					   size, rsc->vring[i].da,
					   rproc_alloc_carveout,
					   rproc_release_carveout,
					   "vdev%dvring%d",
					   rvdev->index, i);
		if (!mem) {
			dev_err(dev, "Can't allocate memory entry structure\n");
			return -ENOMEM;
		}

		rproc_add_carveout(rproc, mem);
	}

	/*
	 * Assign an rproc-wide unique index for this vring
	 * TODO: assign a notifyid for rvdev updates as well
	 * TODO: support predefined notifyids (via resource table)
	 */
	ret = idr_alloc(&rproc->notifyids, rvring, 0, 0, GFP_KERNEL);
	if (ret < 0) {
		dev_err(dev, "idr_alloc failed: %d\n", ret);
		return ret;
	}
	notifyid = ret;

	/* Potentially bump max_notifyid */
	if (notifyid > rproc->max_notifyid)
		rproc->max_notifyid = notifyid;

	rvring->notifyid = notifyid;

	/* Let the rproc know the notifyid of this vring.*/
	rsc->vring[i].notifyid = notifyid;
	return 0;
}

int
rproc_parse_vring(struct rproc_vdev *rvdev, struct fw_rsc_vdev *rsc, int i)
{
	struct rproc *rproc = rvdev->rproc;
	struct device *dev = &rproc->dev;
	struct fw_rsc_vdev_vring *vring = &rsc->vring[i];
	struct rproc_vring *rvring = &rvdev->vring[i];

	dev_dbg(dev, "vdev rsc: vring%d: da 0x%x, qsz %d, align %d\n",
		i, vring->da, vring->num, vring->align);

	/* verify queue size and vring alignment are sane */
	if (!vring->num || !vring->align) {
		dev_err(dev, "invalid qsz (%d) or alignment (%d)\n",
			vring->num, vring->align);
		return -EINVAL;
	}

	rvring->num = vring->num;
	rvring->align = vring->align;
	rvring->rvdev = rvdev;

	return 0;
}

void rproc_free_vring(struct rproc_vring *rvring)
{
	struct rproc *rproc = rvring->rvdev->rproc;
	int idx = rvring - rvring->rvdev->vring;
	struct fw_rsc_vdev *rsc;

	idr_remove(&rproc->notifyids, rvring->notifyid);

	/*
	 * At this point rproc_stop() has been called and the installed resource
	 * table in the remote processor memory may no longer be accessible. As
	 * such and as per rproc_stop(), rproc->table_ptr points to the cached
	 * resource table (rproc->cached_table).  The cached resource table is
	 * only available when a remote processor has been booted by the
	 * remoteproc core, otherwise it is NULL.
	 *
	 * Based on the above, reset the virtio device section in the cached
	 * resource table only if there is one to work with.
	 */
	if (rproc->table_ptr) {
		rsc = (void *)rproc->table_ptr + rvring->rvdev->rsc_offset;
		rsc->vring[idx].da = 0;
		rsc->vring[idx].notifyid = -1;
	}
}

void rproc_add_rvdev(struct rproc *rproc, struct rproc_vdev *rvdev)
{
	if (rvdev && rproc)
		list_add_tail(&rvdev->node, &rproc->rvdevs);
}

void rproc_remove_rvdev(struct rproc_vdev *rvdev)
{
	if (rvdev)
		list_del(&rvdev->node);
}
/**
 * rproc_handle_vdev() - handle a vdev fw resource
 * @rproc: the remote processor
 * @ptr: the vring resource descriptor
 * @offset: offset of the resource entry
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @avail: size of available data (for sanity checking the image)
 *
 * This resource entry requests the host to statically register a virtio
 * device (vdev), and setup everything needed to support it. It contains
 * everything needed to make it possible: the virtio device id, virtio
 * device features, vrings information, virtio config space, etc...
 *
 * Before registering the vdev, the vrings are allocated from non-cacheable
 * physically contiguous memory. Currently we only support two vrings per
 * remote processor (temporary limitation). We might also want to consider
 * doing the vring allocation only later when ->find_vqs() is invoked, and
 * then release them upon ->del_vqs().
 *
 * Note: @da is currently not really handled correctly: we dynamically
 * allocate it using the DMA API, ignoring requested hard coded addresses,
 * and we don't take care of any required IOMMU programming. This is all
 * going to be taken care of when the generic iommu-based DMA API will be
 * merged. Meanwhile, statically-addressed iommu-based firmware images should
 * use RSC_DEVMEM resource entries to map their required @da to the physical
 * address of their base CMA region (ouch, hacky!).
 *
<<<<<<< HEAD
 * Returns 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_vdev(struct rproc *rproc, struct fw_rsc_vdev *rsc,
								int avail)
{
	struct device *dev = rproc->dev;
	struct rproc_vdev *rvdev;
	int i, ret;

	/* make sure resource isn't truncated */
	if (sizeof(*rsc) + rsc->num_of_vrings * sizeof(struct fw_rsc_vdev_vring)
			+ rsc->config_len > avail) {
		dev_err(rproc->dev, "vdev rsc is truncated\n");
=======
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_vdev(struct rproc *rproc, void *ptr,
			     int offset, int avail)
{
	struct fw_rsc_vdev *rsc = ptr;
	struct device *dev = &rproc->dev;
	struct rproc_vdev *rvdev;
	size_t rsc_size;
	struct rproc_vdev_data rvdev_data;
	struct platform_device *pdev;

	/* make sure resource isn't truncated */
	rsc_size = struct_size(rsc, vring, rsc->num_of_vrings);
	if (size_add(rsc_size, rsc->config_len) > avail) {
		dev_err(dev, "vdev rsc is truncated\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	/* make sure reserved bytes are zeroes */
	if (rsc->reserved[0] || rsc->reserved[1]) {
		dev_err(dev, "vdev rsc has non zero reserved bytes\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	dev_dbg(dev, "vdev rsc: id %d, dfeatures %x, cfg len %d, %d vrings\n",
=======
	dev_dbg(dev, "vdev rsc: id %d, dfeatures 0x%x, cfg len %d, %d vrings\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rsc->id, rsc->dfeatures, rsc->config_len, rsc->num_of_vrings);

	/* we currently support only two vrings per rvdev */
	if (rsc->num_of_vrings > ARRAY_SIZE(rvdev->vring)) {
		dev_err(dev, "too many vrings: %d\n", rsc->num_of_vrings);
		return -EINVAL;
	}

<<<<<<< HEAD
	rvdev = kzalloc(sizeof(struct rproc_vdev), GFP_KERNEL);
	if (!rvdev)
		return -ENOMEM;

	rvdev->rproc = rproc;

	/* allocate the vrings */
	for (i = 0; i < rsc->num_of_vrings; i++) {
		ret = __rproc_handle_vring(rvdev, rsc, i);
		if (ret)
			goto free_vrings;
	}

	/* remember the device features */
	rvdev->dfeatures = rsc->dfeatures;

	list_add_tail(&rvdev->node, &rproc->rvdevs);

	/* it is now safe to add the virtio device */
	ret = rproc_add_virtio_dev(rvdev, rsc->id);
	if (ret)
		goto free_vrings;

	return 0;

free_vrings:
	__rproc_free_vrings(rvdev, i);
	kfree(rvdev);
	return ret;
=======
	rvdev_data.id = rsc->id;
	rvdev_data.index = rproc->nb_vdev++;
	rvdev_data.rsc_offset = offset;
	rvdev_data.rsc = rsc;

	/*
	 * When there is more than one remote processor, rproc->nb_vdev number is
	 * same for each separate instances of "rproc". If rvdev_data.index is used
	 * as device id, then we get duplication in sysfs, so need to use
	 * PLATFORM_DEVID_AUTO to auto select device id.
	 */
	pdev = platform_device_register_data(dev, "rproc-virtio", PLATFORM_DEVID_AUTO, &rvdev_data,
					     sizeof(rvdev_data));
	if (IS_ERR(pdev)) {
		dev_err(dev, "failed to create rproc-virtio device\n");
		return PTR_ERR(pdev);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rproc_handle_trace() - handle a shared trace buffer resource
 * @rproc: the remote processor
<<<<<<< HEAD
 * @rsc: the trace resource descriptor
=======
 * @ptr: the trace resource descriptor
 * @offset: offset of the resource entry
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @avail: size of available data (for sanity checking the image)
 *
 * In case the remote processor dumps trace logs into memory,
 * export it via debugfs.
 *
 * Currently, the 'da' member of @rsc should contain the device address
 * where the remote processor is dumping the traces. Later we could also
 * support dynamically allocating this address using the generic
 * DMA API (but currently there isn't a use case for that).
 *
<<<<<<< HEAD
 * Returns 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_trace(struct rproc *rproc, struct fw_rsc_trace *rsc,
								int avail)
{
	struct rproc_mem_entry *trace;
	struct device *dev = rproc->dev;
	void *ptr;
	char name[15];

	if (sizeof(*rsc) > avail) {
		dev_err(rproc->dev, "trace rsc is truncated\n");
=======
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_trace(struct rproc *rproc, void *ptr,
			      int offset, int avail)
{
	struct fw_rsc_trace *rsc = ptr;
	struct rproc_debug_trace *trace;
	struct device *dev = &rproc->dev;
	char name[15];

	if (sizeof(*rsc) > avail) {
		dev_err(dev, "trace rsc is truncated\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	/* make sure reserved bytes are zeroes */
	if (rsc->reserved) {
		dev_err(dev, "trace rsc has non zero reserved bytes\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	/* what's the kernel address of this resource ? */
	ptr = rproc_da_to_va(rproc, rsc->da, rsc->len);
	if (!ptr) {
		dev_err(dev, "erroneous trace resource entry\n");
		return -EINVAL;
	}

	trace = kzalloc(sizeof(*trace), GFP_KERNEL);
	if (!trace) {
		dev_err(dev, "kzalloc trace failed\n");
		return -ENOMEM;
	}

	/* set the trace buffer dma properties */
	trace->len = rsc->len;
	trace->va = ptr;
=======
	trace = kzalloc(sizeof(*trace), GFP_KERNEL);
	if (!trace)
		return -ENOMEM;

	/* set the trace buffer dma properties */
	trace->trace_mem.len = rsc->len;
	trace->trace_mem.da = rsc->da;

	/* set pointer on rproc device */
	trace->rproc = rproc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* make sure snprintf always null terminates, even if truncating */
	snprintf(name, sizeof(name), "trace%d", rproc->num_traces);

	/* create the debugfs entry */
<<<<<<< HEAD
	trace->priv = rproc_create_trace_file(name, rproc, trace);
	if (!trace->priv) {
		trace->va = NULL;
		kfree(trace);
		return -EINVAL;
	}
=======
	trace->tfile = rproc_create_trace_file(name, rproc, trace);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_add_tail(&trace->node, &rproc->traces);

	rproc->num_traces++;

<<<<<<< HEAD
	dev_dbg(dev, "%s added: va %p, da 0x%x, len 0x%x\n", name, ptr,
						rsc->da, rsc->len);
=======
	dev_dbg(dev, "%s added: da 0x%x, len 0x%x\n",
		name, rsc->da, rsc->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/**
 * rproc_handle_devmem() - handle devmem resource entry
 * @rproc: remote processor handle
<<<<<<< HEAD
 * @rsc: the devmem resource entry
=======
 * @ptr: the devmem resource entry
 * @offset: offset of the resource entry
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @avail: size of available data (for sanity checking the image)
 *
 * Remote processors commonly need to access certain on-chip peripherals.
 *
 * Some of these remote processors access memory via an iommu device,
 * and might require us to configure their iommu before they can access
 * the on-chip peripherals they need.
 *
 * This resource entry is a request to map such a peripheral device.
 *
 * These devmem entries will contain the physical address of the device in
 * the 'pa' member. If a specific device address is expected, then 'da' will
 * contain it (currently this is the only use case supported). 'len' will
 * contain the size of the physical region we need to map.
 *
 * Currently we just "trust" those devmem entries to contain valid physical
 * addresses, but this is going to change: we want the implementations to
 * tell us ranges of physical addresses the firmware is allowed to request,
 * and not allow firmwares to request access to physical addresses that
 * are outside those ranges.
<<<<<<< HEAD
 */
static int rproc_handle_devmem(struct rproc *rproc, struct fw_rsc_devmem *rsc,
								int avail)
{
	struct rproc_mem_entry *mapping;
=======
 *
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_devmem(struct rproc *rproc, void *ptr,
			       int offset, int avail)
{
	struct fw_rsc_devmem *rsc = ptr;
	struct rproc_mem_entry *mapping;
	struct device *dev = &rproc->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	/* no point in handling this resource without a valid iommu domain */
	if (!rproc->domain)
		return -EINVAL;

	if (sizeof(*rsc) > avail) {
<<<<<<< HEAD
		dev_err(rproc->dev, "devmem rsc is truncated\n");
=======
		dev_err(dev, "devmem rsc is truncated\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	/* make sure reserved bytes are zeroes */
	if (rsc->reserved) {
<<<<<<< HEAD
		dev_err(rproc->dev, "devmem rsc has non zero reserved bytes\n");
=======
		dev_err(dev, "devmem rsc has non zero reserved bytes\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	mapping = kzalloc(sizeof(*mapping), GFP_KERNEL);
<<<<<<< HEAD
	if (!mapping) {
		dev_err(rproc->dev, "kzalloc mapping failed\n");
		return -ENOMEM;
	}

	ret = iommu_map(rproc->domain, rsc->da, rsc->pa, rsc->len, rsc->flags);
	if (ret) {
		dev_err(rproc->dev, "failed to map devmem: %d\n", ret);
=======
	if (!mapping)
		return -ENOMEM;

	ret = iommu_map(rproc->domain, rsc->da, rsc->pa, rsc->len, rsc->flags,
			GFP_KERNEL);
	if (ret) {
		dev_err(dev, "failed to map devmem: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/*
	 * We'll need this info later when we'll want to unmap everything
	 * (e.g. on shutdown).
	 *
	 * We can't trust the remote processor not to change the resource
	 * table, so we must maintain this info independently.
	 */
	mapping->da = rsc->da;
	mapping->len = rsc->len;
	list_add_tail(&mapping->node, &rproc->mappings);

<<<<<<< HEAD
	dev_dbg(rproc->dev, "mapped devmem pa 0x%x, da 0x%x, len 0x%x\n",
					rsc->pa, rsc->da, rsc->len);
=======
	dev_dbg(dev, "mapped devmem pa 0x%x, da 0x%x, len 0x%x\n",
		rsc->pa, rsc->da, rsc->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

out:
	kfree(mapping);
	return ret;
}

/**
<<<<<<< HEAD
 * rproc_handle_carveout() - handle phys contig memory allocation requests
 * @rproc: rproc handle
 * @rsc: the resource entry
 * @avail: size of available data (for image validation)
 *
 * This function will handle firmware requests for allocation of physically
 * contiguous memory regions.
 *
 * These request entries should come first in the firmware's resource table,
 * as other firmware entries might request placing other data objects inside
 * these memory regions (e.g. data/code segments, trace resource entries, ...).
 *
 * Allocating memory this way helps utilizing the reserved physical memory
 * (e.g. CMA) more efficiently, and also minimizes the number of TLB entries
 * needed to map it (in case @rproc is using an IOMMU). Reducing the TLB
 * pressure is important; it may have a substantial impact on performance.
 */
static int rproc_handle_carveout(struct rproc *rproc,
				struct fw_rsc_carveout *rsc, int avail)
{
	struct rproc_mem_entry *carveout, *mapping;
	struct device *dev = rproc->dev;
=======
 * rproc_alloc_carveout() - allocated specified carveout
 * @rproc: rproc handle
 * @mem: the memory entry to allocate
 *
 * This function allocate specified memory entry @mem using
 * dma_alloc_coherent() as default allocator
 *
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_alloc_carveout(struct rproc *rproc,
				struct rproc_mem_entry *mem)
{
	struct rproc_mem_entry *mapping = NULL;
	struct device *dev = &rproc->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dma_addr_t dma;
	void *va;
	int ret;

<<<<<<< HEAD
	if (sizeof(*rsc) > avail) {
		dev_err(rproc->dev, "carveout rsc is truncated\n");
		return -EINVAL;
	}

	/* make sure reserved bytes are zeroes */
	if (rsc->reserved) {
		dev_err(dev, "carveout rsc has non zero reserved bytes\n");
		return -EINVAL;
	}

	dev_dbg(dev, "carveout rsc: da %x, pa %x, len %x, flags %x\n",
			rsc->da, rsc->pa, rsc->len, rsc->flags);

	carveout = kzalloc(sizeof(*carveout), GFP_KERNEL);
	if (!carveout) {
		dev_err(dev, "kzalloc carveout failed\n");
		return -ENOMEM;
	}

	va = dma_alloc_coherent(dev, rsc->len, &dma, GFP_KERNEL);
	if (!va) {
		dev_err(dev, "failed to dma alloc carveout: %d\n", rsc->len);
		ret = -ENOMEM;
		goto free_carv;
	}

	dev_dbg(dev, "carveout va %p, dma %x, len 0x%x\n", va, dma, rsc->len);
=======
	va = dma_alloc_coherent(dev->parent, mem->len, &dma, GFP_KERNEL);
	if (!va) {
		dev_err(dev->parent,
			"failed to allocate dma memory: len 0x%zx\n",
			mem->len);
		return -ENOMEM;
	}

	dev_dbg(dev, "carveout va %pK, dma %pad, len 0x%zx\n",
		va, &dma, mem->len);

	if (mem->da != FW_RSC_ADDR_ANY && !rproc->domain) {
		/*
		 * Check requested da is equal to dma address
		 * and print a warn message in case of missalignment.
		 * Don't stop rproc_start sequence as coprocessor may
		 * build pa to da translation on its side.
		 */
		if (mem->da != (u32)dma)
			dev_warn(dev->parent,
				 "Allocated carveout doesn't fit device address request\n");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Ok, this is non-standard.
	 *
	 * Sometimes we can't rely on the generic iommu-based DMA API
	 * to dynamically allocate the device address and then set the IOMMU
	 * tables accordingly, because some remote processors might
	 * _require_ us to use hard coded device addresses that their
	 * firmware was compiled with.
	 *
	 * In this case, we must use the IOMMU API directly and map
	 * the memory to the device address as expected by the remote
	 * processor.
	 *
	 * Obviously such remote processor devices should not be configured
	 * to use the iommu-based DMA API: we expect 'dma' to contain the
	 * physical address in this case.
	 */
<<<<<<< HEAD
	if (rproc->domain) {
		mapping = kzalloc(sizeof(*mapping), GFP_KERNEL);
		if (!mapping) {
			dev_err(dev, "kzalloc mapping failed\n");
=======
	if (mem->da != FW_RSC_ADDR_ANY && rproc->domain) {
		mapping = kzalloc(sizeof(*mapping), GFP_KERNEL);
		if (!mapping) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -ENOMEM;
			goto dma_free;
		}

<<<<<<< HEAD
		ret = iommu_map(rproc->domain, rsc->da, dma, rsc->len,
								rsc->flags);
=======
		ret = iommu_map(rproc->domain, mem->da, dma, mem->len,
				mem->flags, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret) {
			dev_err(dev, "iommu_map failed: %d\n", ret);
			goto free_mapping;
		}

		/*
		 * We'll need this info later when we'll want to unmap
		 * everything (e.g. on shutdown).
		 *
		 * We can't trust the remote processor not to change the
		 * resource table, so we must maintain this info independently.
		 */
<<<<<<< HEAD
		mapping->da = rsc->da;
		mapping->len = rsc->len;
		list_add_tail(&mapping->node, &rproc->mappings);

		dev_dbg(dev, "carveout mapped 0x%x to 0x%x\n", rsc->da, dma);

		/*
		 * Some remote processors might need to know the pa
		 * even though they are behind an IOMMU. E.g., OMAP4's
		 * remote M3 processor needs this so it can control
		 * on-chip hardware accelerators that are not behind
		 * the IOMMU, and therefor must know the pa.
		 *
		 * Generally we don't want to expose physical addresses
		 * if we don't have to (remote processors are generally
		 * _not_ trusted), so we might want to do this only for
		 * remote processor that _must_ have this (e.g. OMAP4's
		 * dual M3 subsystem).
		 */
		rsc->pa = dma;
	}

	carveout->va = va;
	carveout->len = rsc->len;
	carveout->dma = dma;
	carveout->da = rsc->da;

	list_add_tail(&carveout->node, &rproc->carveouts);
=======
		mapping->da = mem->da;
		mapping->len = mem->len;
		list_add_tail(&mapping->node, &rproc->mappings);

		dev_dbg(dev, "carveout mapped 0x%x to %pad\n",
			mem->da, &dma);
	}

	if (mem->da == FW_RSC_ADDR_ANY) {
		/* Update device address as undefined by requester */
		if ((u64)dma & HIGH_BITS_MASK)
			dev_warn(dev, "DMA address cast in 32bit to fit resource table format\n");

		mem->da = (u32)dma;
	}

	mem->dma = dma;
	mem->va = va;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

free_mapping:
	kfree(mapping);
dma_free:
<<<<<<< HEAD
	dma_free_coherent(dev, rsc->len, va, dma);
free_carv:
	kfree(carveout);
	return ret;
}

=======
	dma_free_coherent(dev->parent, mem->len, va, dma);
	return ret;
}

/**
 * rproc_release_carveout() - release acquired carveout
 * @rproc: rproc handle
 * @mem: the memory entry to release
 *
 * This function releases specified memory entry @mem allocated via
 * rproc_alloc_carveout() function by @rproc.
 *
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_release_carveout(struct rproc *rproc,
				  struct rproc_mem_entry *mem)
{
	struct device *dev = &rproc->dev;

	/* clean up carveout allocations */
	dma_free_coherent(dev->parent, mem->len, mem->va, mem->dma);
	return 0;
}

/**
 * rproc_handle_carveout() - handle phys contig memory allocation requests
 * @rproc: rproc handle
 * @ptr: the resource entry
 * @offset: offset of the resource entry
 * @avail: size of available data (for image validation)
 *
 * This function will handle firmware requests for allocation of physically
 * contiguous memory regions.
 *
 * These request entries should come first in the firmware's resource table,
 * as other firmware entries might request placing other data objects inside
 * these memory regions (e.g. data/code segments, trace resource entries, ...).
 *
 * Allocating memory this way helps utilizing the reserved physical memory
 * (e.g. CMA) more efficiently, and also minimizes the number of TLB entries
 * needed to map it (in case @rproc is using an IOMMU). Reducing the TLB
 * pressure is important; it may have a substantial impact on performance.
 *
 * Return: 0 on success, or an appropriate error code otherwise
 */
static int rproc_handle_carveout(struct rproc *rproc,
				 void *ptr, int offset, int avail)
{
	struct fw_rsc_carveout *rsc = ptr;
	struct rproc_mem_entry *carveout;
	struct device *dev = &rproc->dev;

	if (sizeof(*rsc) > avail) {
		dev_err(dev, "carveout rsc is truncated\n");
		return -EINVAL;
	}

	/* make sure reserved bytes are zeroes */
	if (rsc->reserved) {
		dev_err(dev, "carveout rsc has non zero reserved bytes\n");
		return -EINVAL;
	}

	dev_dbg(dev, "carveout rsc: name: %s, da 0x%x, pa 0x%x, len 0x%x, flags 0x%x\n",
		rsc->name, rsc->da, rsc->pa, rsc->len, rsc->flags);

	/*
	 * Check carveout rsc already part of a registered carveout,
	 * Search by name, then check the da and length
	 */
	carveout = rproc_find_carveout_by_name(rproc, rsc->name);

	if (carveout) {
		if (carveout->rsc_offset != FW_RSC_ADDR_ANY) {
			dev_err(dev,
				"Carveout already associated to resource table\n");
			return -ENOMEM;
		}

		if (rproc_check_carveout_da(rproc, carveout, rsc->da, rsc->len))
			return -ENOMEM;

		/* Update memory carveout with resource table info */
		carveout->rsc_offset = offset;
		carveout->flags = rsc->flags;

		return 0;
	}

	/* Register carveout in list */
	carveout = rproc_mem_entry_init(dev, NULL, 0, rsc->len, rsc->da,
					rproc_alloc_carveout,
					rproc_release_carveout, rsc->name);
	if (!carveout) {
		dev_err(dev, "Can't allocate memory entry structure\n");
		return -ENOMEM;
	}

	carveout->flags = rsc->flags;
	carveout->rsc_offset = offset;
	rproc_add_carveout(rproc, carveout);

	return 0;
}

/**
 * rproc_add_carveout() - register an allocated carveout region
 * @rproc: rproc handle
 * @mem: memory entry to register
 *
 * This function registers specified memory entry in @rproc carveouts list.
 * Specified carveout should have been allocated before registering.
 */
void rproc_add_carveout(struct rproc *rproc, struct rproc_mem_entry *mem)
{
	list_add_tail(&mem->node, &rproc->carveouts);
}
EXPORT_SYMBOL(rproc_add_carveout);

/**
 * rproc_mem_entry_init() - allocate and initialize rproc_mem_entry struct
 * @dev: pointer on device struct
 * @va: virtual address
 * @dma: dma address
 * @len: memory carveout length
 * @da: device address
 * @alloc: memory carveout allocation function
 * @release: memory carveout release function
 * @name: carveout name
 *
 * This function allocates a rproc_mem_entry struct and fill it with parameters
 * provided by client.
 *
 * Return: a valid pointer on success, or NULL on failure
 */
__printf(8, 9)
struct rproc_mem_entry *
rproc_mem_entry_init(struct device *dev,
		     void *va, dma_addr_t dma, size_t len, u32 da,
		     int (*alloc)(struct rproc *, struct rproc_mem_entry *),
		     int (*release)(struct rproc *, struct rproc_mem_entry *),
		     const char *name, ...)
{
	struct rproc_mem_entry *mem;
	va_list args;

	mem = kzalloc(sizeof(*mem), GFP_KERNEL);
	if (!mem)
		return mem;

	mem->va = va;
	mem->dma = dma;
	mem->da = da;
	mem->len = len;
	mem->alloc = alloc;
	mem->release = release;
	mem->rsc_offset = FW_RSC_ADDR_ANY;
	mem->of_resm_idx = -1;

	va_start(args, name);
	vsnprintf(mem->name, sizeof(mem->name), name, args);
	va_end(args);

	return mem;
}
EXPORT_SYMBOL(rproc_mem_entry_init);

/**
 * rproc_of_resm_mem_entry_init() - allocate and initialize rproc_mem_entry struct
 * from a reserved memory phandle
 * @dev: pointer on device struct
 * @of_resm_idx: reserved memory phandle index in "memory-region"
 * @len: memory carveout length
 * @da: device address
 * @name: carveout name
 *
 * This function allocates a rproc_mem_entry struct and fill it with parameters
 * provided by client.
 *
 * Return: a valid pointer on success, or NULL on failure
 */
__printf(5, 6)
struct rproc_mem_entry *
rproc_of_resm_mem_entry_init(struct device *dev, u32 of_resm_idx, size_t len,
			     u32 da, const char *name, ...)
{
	struct rproc_mem_entry *mem;
	va_list args;

	mem = kzalloc(sizeof(*mem), GFP_KERNEL);
	if (!mem)
		return mem;

	mem->da = da;
	mem->len = len;
	mem->rsc_offset = FW_RSC_ADDR_ANY;
	mem->of_resm_idx = of_resm_idx;

	va_start(args, name);
	vsnprintf(mem->name, sizeof(mem->name), name, args);
	va_end(args);

	return mem;
}
EXPORT_SYMBOL(rproc_of_resm_mem_entry_init);

/**
 * rproc_of_parse_firmware() - parse and return the firmware-name
 * @dev: pointer on device struct representing a rproc
 * @index: index to use for the firmware-name retrieval
 * @fw_name: pointer to a character string, in which the firmware
 *           name is returned on success and unmodified otherwise.
 *
 * This is an OF helper function that parses a device's DT node for
 * the "firmware-name" property and returns the firmware name pointer
 * in @fw_name on success.
 *
 * Return: 0 on success, or an appropriate failure.
 */
int rproc_of_parse_firmware(struct device *dev, int index, const char **fw_name)
{
	int ret;

	ret = of_property_read_string_index(dev->of_node, "firmware-name",
					    index, fw_name);
	return ret ? ret : 0;
}
EXPORT_SYMBOL(rproc_of_parse_firmware);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * A lookup table for resource handlers. The indices are defined in
 * enum fw_resource_type.
 */
<<<<<<< HEAD
static rproc_handle_resource_t rproc_handle_rsc[] = {
	[RSC_CARVEOUT] = (rproc_handle_resource_t)rproc_handle_carveout,
	[RSC_DEVMEM] = (rproc_handle_resource_t)rproc_handle_devmem,
	[RSC_TRACE] = (rproc_handle_resource_t)rproc_handle_trace,
	[RSC_VDEV] = NULL, /* VDEVs were handled upon registrarion */
};

/* handle firmware resource entries before booting the remote processor */
static int
rproc_handle_boot_rsc(struct rproc *rproc, struct resource_table *table, int len)
{
	struct device *dev = rproc->dev;
	rproc_handle_resource_t handler;
	int ret = 0, i;

	for (i = 0; i < table->num; i++) {
		int offset = table->offset[i];
		struct fw_rsc_hdr *hdr = (void *)table + offset;
		int avail = len - offset - sizeof(*hdr);
=======
static rproc_handle_resource_t rproc_loading_handlers[RSC_LAST] = {
	[RSC_CARVEOUT] = rproc_handle_carveout,
	[RSC_DEVMEM] = rproc_handle_devmem,
	[RSC_TRACE] = rproc_handle_trace,
	[RSC_VDEV] = rproc_handle_vdev,
};

/* handle firmware resource entries before booting the remote processor */
static int rproc_handle_resources(struct rproc *rproc,
				  rproc_handle_resource_t handlers[RSC_LAST])
{
	struct device *dev = &rproc->dev;
	rproc_handle_resource_t handler;
	int ret = 0, i;

	if (!rproc->table_ptr)
		return 0;

	for (i = 0; i < rproc->table_ptr->num; i++) {
		int offset = rproc->table_ptr->offset[i];
		struct fw_rsc_hdr *hdr = (void *)rproc->table_ptr + offset;
		int avail = rproc->table_sz - offset - sizeof(*hdr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		void *rsc = (void *)hdr + sizeof(*hdr);

		/* make sure table isn't truncated */
		if (avail < 0) {
			dev_err(dev, "rsc table is truncated\n");
			return -EINVAL;
		}

		dev_dbg(dev, "rsc: type %d\n", hdr->type);

<<<<<<< HEAD
=======
		if (hdr->type >= RSC_VENDOR_START &&
		    hdr->type <= RSC_VENDOR_END) {
			ret = rproc_handle_rsc(rproc, hdr->type, rsc,
					       offset + sizeof(*hdr), avail);
			if (ret == RSC_HANDLED)
				continue;
			else if (ret < 0)
				break;

			dev_warn(dev, "unsupported vendor resource %d\n",
				 hdr->type);
			continue;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (hdr->type >= RSC_LAST) {
			dev_warn(dev, "unsupported resource %d\n", hdr->type);
			continue;
		}

<<<<<<< HEAD
		handler = rproc_handle_rsc[hdr->type];
		if (!handler)
			continue;

		ret = handler(rproc, rsc, avail);
=======
		handler = handlers[hdr->type];
		if (!handler)
			continue;

		ret = handler(rproc, rsc, offset + sizeof(*hdr), avail);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			break;
	}

	return ret;
}

<<<<<<< HEAD
/* handle firmware resource entries while registering the remote processor */
static int
rproc_handle_virtio_rsc(struct rproc *rproc, struct resource_table *table, int len)
{
	struct device *dev = rproc->dev;
	int ret = 0, i;

	for (i = 0; i < table->num; i++) {
		int offset = table->offset[i];
		struct fw_rsc_hdr *hdr = (void *)table + offset;
		int avail = len - offset - sizeof(*hdr);
		struct fw_rsc_vdev *vrsc;

		/* make sure table isn't truncated */
		if (avail < 0) {
			dev_err(dev, "rsc table is truncated\n");
			return -EINVAL;
		}

		dev_dbg(dev, "%s: rsc type %d\n", __func__, hdr->type);

		if (hdr->type != RSC_VDEV)
			continue;

		vrsc = (struct fw_rsc_vdev *)hdr->data;

		ret = rproc_handle_vdev(rproc, vrsc, avail);
		if (ret)
			break;
=======
static int rproc_prepare_subdevices(struct rproc *rproc)
{
	struct rproc_subdev *subdev;
	int ret;

	list_for_each_entry(subdev, &rproc->subdevs, node) {
		if (subdev->prepare) {
			ret = subdev->prepare(subdev);
			if (ret)
				goto unroll_preparation;
		}
	}

	return 0;

unroll_preparation:
	list_for_each_entry_continue_reverse(subdev, &rproc->subdevs, node) {
		if (subdev->unprepare)
			subdev->unprepare(subdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

<<<<<<< HEAD
/**
 * rproc_find_rsc_table() - find the resource table
 * @rproc: the rproc handle
 * @elf_data: the content of the ELF firmware image
 * @len: firmware size (in bytes)
 * @tablesz: place holder for providing back the table size
 *
 * This function finds the resource table inside the remote processor's
 * firmware. It is used both upon the registration of @rproc (in order
 * to look for and register the supported virito devices), and when the
 * @rproc is booted.
 *
 * Returns the pointer to the resource table if it is found, and write its
 * size into @tablesz. If a valid table isn't found, NULL is returned
 * (and @tablesz isn't set).
 */
static struct resource_table *
rproc_find_rsc_table(struct rproc *rproc, const u8 *elf_data, size_t len,
							int *tablesz)
{
	struct elf32_hdr *ehdr;
	struct elf32_shdr *shdr;
	const char *name_table;
	struct device *dev = rproc->dev;
	struct resource_table *table = NULL;
	int i;

	ehdr = (struct elf32_hdr *)elf_data;
	shdr = (struct elf32_shdr *)(elf_data + ehdr->e_shoff);
	name_table = elf_data + shdr[ehdr->e_shstrndx].sh_offset;

	/* look for the resource table and handle it */
	for (i = 0; i < ehdr->e_shnum; i++, shdr++) {
		int size = shdr->sh_size;
		int offset = shdr->sh_offset;

		if (strcmp(name_table + shdr->sh_name, ".resource_table"))
			continue;

		table = (struct resource_table *)(elf_data + offset);

		/* make sure we have the entire table */
		if (offset + size > len) {
			dev_err(dev, "resource table truncated\n");
			return NULL;
		}

		/* make sure table has at least the header */
		if (sizeof(struct resource_table) > size) {
			dev_err(dev, "header-less resource table\n");
			return NULL;
		}

		/* we don't support any version beyond the first */
		if (table->ver != 1) {
			dev_err(dev, "unsupported fw ver: %d\n", table->ver);
			return NULL;
		}

		/* make sure reserved bytes are zeroes */
		if (table->reserved[0] || table->reserved[1]) {
			dev_err(dev, "non zero reserved bytes\n");
			return NULL;
		}

		/* make sure the offsets array isn't truncated */
		if (table->num * sizeof(table->offset[0]) +
				sizeof(struct resource_table) > size) {
			dev_err(dev, "resource table incomplete\n");
			return NULL;
		}

		*tablesz = shdr->sh_size;
		break;
	}

	return table;
}

=======
static int rproc_start_subdevices(struct rproc *rproc)
{
	struct rproc_subdev *subdev;
	int ret;

	list_for_each_entry(subdev, &rproc->subdevs, node) {
		if (subdev->start) {
			ret = subdev->start(subdev);
			if (ret)
				goto unroll_registration;
		}
	}

	return 0;

unroll_registration:
	list_for_each_entry_continue_reverse(subdev, &rproc->subdevs, node) {
		if (subdev->stop)
			subdev->stop(subdev, true);
	}

	return ret;
}

static void rproc_stop_subdevices(struct rproc *rproc, bool crashed)
{
	struct rproc_subdev *subdev;

	list_for_each_entry_reverse(subdev, &rproc->subdevs, node) {
		if (subdev->stop)
			subdev->stop(subdev, crashed);
	}
}

static void rproc_unprepare_subdevices(struct rproc *rproc)
{
	struct rproc_subdev *subdev;

	list_for_each_entry_reverse(subdev, &rproc->subdevs, node) {
		if (subdev->unprepare)
			subdev->unprepare(subdev);
	}
}

/**
 * rproc_alloc_registered_carveouts() - allocate all carveouts registered
 * in the list
 * @rproc: the remote processor handle
 *
 * This function parses registered carveout list, performs allocation
 * if alloc() ops registered and updates resource table information
 * if rsc_offset set.
 *
 * Return: 0 on success
 */
static int rproc_alloc_registered_carveouts(struct rproc *rproc)
{
	struct rproc_mem_entry *entry, *tmp;
	struct fw_rsc_carveout *rsc;
	struct device *dev = &rproc->dev;
	u64 pa;
	int ret;

	list_for_each_entry_safe(entry, tmp, &rproc->carveouts, node) {
		if (entry->alloc) {
			ret = entry->alloc(rproc, entry);
			if (ret) {
				dev_err(dev, "Unable to allocate carveout %s: %d\n",
					entry->name, ret);
				return -ENOMEM;
			}
		}

		if (entry->rsc_offset != FW_RSC_ADDR_ANY) {
			/* update resource table */
			rsc = (void *)rproc->table_ptr + entry->rsc_offset;

			/*
			 * Some remote processors might need to know the pa
			 * even though they are behind an IOMMU. E.g., OMAP4's
			 * remote M3 processor needs this so it can control
			 * on-chip hardware accelerators that are not behind
			 * the IOMMU, and therefor must know the pa.
			 *
			 * Generally we don't want to expose physical addresses
			 * if we don't have to (remote processors are generally
			 * _not_ trusted), so we might want to do this only for
			 * remote processor that _must_ have this (e.g. OMAP4's
			 * dual M3 subsystem).
			 *
			 * Non-IOMMU processors might also want to have this info.
			 * In this case, the device address and the physical address
			 * are the same.
			 */

			/* Use va if defined else dma to generate pa */
			if (entry->va)
				pa = (u64)rproc_va_to_pa(entry->va);
			else
				pa = (u64)entry->dma;

			if (((u64)pa) & HIGH_BITS_MASK)
				dev_warn(dev,
					 "Physical address cast in 32bit to fit resource table format\n");

			rsc->pa = (u32)pa;
			rsc->da = entry->da;
			rsc->len = entry->len;
		}
	}

	return 0;
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * rproc_resource_cleanup() - clean up and free all acquired resources
 * @rproc: rproc handle
 *
 * This function will free all resources acquired for @rproc, and it
 * is called whenever @rproc either shuts down or fails to boot.
 */
<<<<<<< HEAD
static void rproc_resource_cleanup(struct rproc *rproc)
{
	struct rproc_mem_entry *entry, *tmp;
	struct device *dev = rproc->dev;

	/* clean up debugfs trace entries */
	list_for_each_entry_safe(entry, tmp, &rproc->traces, node) {
		rproc_remove_trace_file(entry->priv);
		rproc->num_traces--;
		list_del(&entry->node);
		kfree(entry);
	}

	/* clean up carveout allocations */
	list_for_each_entry_safe(entry, tmp, &rproc->carveouts, node) {
		dma_free_coherent(dev, entry->len, entry->va, entry->dma);
		list_del(&entry->node);
		kfree(entry);
=======
void rproc_resource_cleanup(struct rproc *rproc)
{
	struct rproc_mem_entry *entry, *tmp;
	struct rproc_debug_trace *trace, *ttmp;
	struct rproc_vdev *rvdev, *rvtmp;
	struct device *dev = &rproc->dev;

	/* clean up debugfs trace entries */
	list_for_each_entry_safe(trace, ttmp, &rproc->traces, node) {
		rproc_remove_trace_file(trace->tfile);
		rproc->num_traces--;
		list_del(&trace->node);
		kfree(trace);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* clean up iommu mapping entries */
	list_for_each_entry_safe(entry, tmp, &rproc->mappings, node) {
		size_t unmapped;

		unmapped = iommu_unmap(rproc->domain, entry->da, entry->len);
		if (unmapped != entry->len) {
			/* nothing much to do besides complaining */
<<<<<<< HEAD
			dev_err(dev, "failed to unmap %u/%zu\n", entry->len,
								unmapped);
=======
			dev_err(dev, "failed to unmap %zx/%zu\n", entry->len,
				unmapped);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		list_del(&entry->node);
		kfree(entry);
	}
<<<<<<< HEAD
}

/* make sure this fw image is sane */
static int rproc_fw_sanity_check(struct rproc *rproc, const struct firmware *fw)
{
	const char *name = rproc->firmware;
	struct device *dev = rproc->dev;
	struct elf32_hdr *ehdr;
	char class;

	if (!fw) {
		dev_err(dev, "failed to load %s\n", name);
		return -EINVAL;
	}

	if (fw->size < sizeof(struct elf32_hdr)) {
		dev_err(dev, "Image is too small\n");
		return -EINVAL;
	}

	ehdr = (struct elf32_hdr *)fw->data;

	/* We only support ELF32 at this point */
	class = ehdr->e_ident[EI_CLASS];
	if (class != ELFCLASS32) {
		dev_err(dev, "Unsupported class: %d\n", class);
		return -EINVAL;
	}

	/* We assume the firmware has the same endianess as the host */
# ifdef __LITTLE_ENDIAN
	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
# else /* BIG ENDIAN */
	if (ehdr->e_ident[EI_DATA] != ELFDATA2MSB) {
# endif
		dev_err(dev, "Unsupported firmware endianess\n");
		return -EINVAL;
	}

	if (fw->size < ehdr->e_shoff + sizeof(struct elf32_shdr)) {
		dev_err(dev, "Image is too small\n");
		return -EINVAL;
	}

	if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) {
		dev_err(dev, "Image is corrupted (bad magic)\n");
		return -EINVAL;
	}

	if (ehdr->e_phnum == 0) {
		dev_err(dev, "No loadable segments\n");
		return -EINVAL;
	}

	if (ehdr->e_phoff > fw->size) {
		dev_err(dev, "Firmware size is too small\n");
		return -EINVAL;
	}

	return 0;
=======

	/* clean up carveout allocations */
	list_for_each_entry_safe(entry, tmp, &rproc->carveouts, node) {
		if (entry->release)
			entry->release(rproc, entry);
		list_del(&entry->node);
		kfree(entry);
	}

	/* clean up remote vdev entries */
	list_for_each_entry_safe(rvdev, rvtmp, &rproc->rvdevs, node)
		platform_device_unregister(rvdev->pdev);

	rproc_coredump_cleanup(rproc);
}
EXPORT_SYMBOL(rproc_resource_cleanup);

static int rproc_start(struct rproc *rproc, const struct firmware *fw)
{
	struct resource_table *loaded_table;
	struct device *dev = &rproc->dev;
	int ret;

	/* load the ELF segments to memory */
	ret = rproc_load_segments(rproc, fw);
	if (ret) {
		dev_err(dev, "Failed to load program segments: %d\n", ret);
		return ret;
	}

	/*
	 * The starting device has been given the rproc->cached_table as the
	 * resource table. The address of the vring along with the other
	 * allocated resources (carveouts etc) is stored in cached_table.
	 * In order to pass this information to the remote device we must copy
	 * this information to device memory. We also update the table_ptr so
	 * that any subsequent changes will be applied to the loaded version.
	 */
	loaded_table = rproc_find_loaded_rsc_table(rproc, fw);
	if (loaded_table) {
		memcpy(loaded_table, rproc->cached_table, rproc->table_sz);
		rproc->table_ptr = loaded_table;
	}

	ret = rproc_prepare_subdevices(rproc);
	if (ret) {
		dev_err(dev, "failed to prepare subdevices for %s: %d\n",
			rproc->name, ret);
		goto reset_table_ptr;
	}

	/* power up the remote processor */
	ret = rproc->ops->start(rproc);
	if (ret) {
		dev_err(dev, "can't start rproc %s: %d\n", rproc->name, ret);
		goto unprepare_subdevices;
	}

	/* Start any subdevices for the remote processor */
	ret = rproc_start_subdevices(rproc);
	if (ret) {
		dev_err(dev, "failed to probe subdevices for %s: %d\n",
			rproc->name, ret);
		goto stop_rproc;
	}

	rproc->state = RPROC_RUNNING;

	dev_info(dev, "remote processor %s is now up\n", rproc->name);

	return 0;

stop_rproc:
	rproc->ops->stop(rproc);
unprepare_subdevices:
	rproc_unprepare_subdevices(rproc);
reset_table_ptr:
	rproc->table_ptr = rproc->cached_table;

	return ret;
}

static int __rproc_attach(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	ret = rproc_prepare_subdevices(rproc);
	if (ret) {
		dev_err(dev, "failed to prepare subdevices for %s: %d\n",
			rproc->name, ret);
		goto out;
	}

	/* Attach to the remote processor */
	ret = rproc_attach_device(rproc);
	if (ret) {
		dev_err(dev, "can't attach to rproc %s: %d\n",
			rproc->name, ret);
		goto unprepare_subdevices;
	}

	/* Start any subdevices for the remote processor */
	ret = rproc_start_subdevices(rproc);
	if (ret) {
		dev_err(dev, "failed to probe subdevices for %s: %d\n",
			rproc->name, ret);
		goto stop_rproc;
	}

	rproc->state = RPROC_ATTACHED;

	dev_info(dev, "remote processor %s is now attached\n", rproc->name);

	return 0;

stop_rproc:
	rproc->ops->stop(rproc);
unprepare_subdevices:
	rproc_unprepare_subdevices(rproc);
out:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * take a firmware and boot a remote processor with it.
 */
static int rproc_fw_boot(struct rproc *rproc, const struct firmware *fw)
{
<<<<<<< HEAD
	struct device *dev = rproc->dev;
	const char *name = rproc->firmware;
	struct elf32_hdr *ehdr;
	struct resource_table *table;
	int ret, tablesz;
=======
	struct device *dev = &rproc->dev;
	const char *name = rproc->firmware;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = rproc_fw_sanity_check(rproc, fw);
	if (ret)
		return ret;

<<<<<<< HEAD
	ehdr = (struct elf32_hdr *)fw->data;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_info(dev, "Booting fw image %s, size %zd\n", name, fw->size);

	/*
	 * if enabling an IOMMU isn't relevant for this rproc, this is
	 * just a nop
	 */
	ret = rproc_enable_iommu(rproc);
	if (ret) {
		dev_err(dev, "can't enable iommu: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	/*
	 * The ELF entry point is the rproc's boot addr (though this is not
	 * a configurable property of all remote processors: some will always
	 * boot at a specific hardcoded address).
	 */
	rproc->bootaddr = ehdr->e_entry;

	/* look for the resource table */
	table = rproc_find_rsc_table(rproc, fw->data, fw->size, &tablesz);
	if (!table) {
		ret = -EINVAL;
		goto clean_up;
	}

	/* handle fw resources which are required to boot rproc */
	ret = rproc_handle_boot_rsc(rproc, table, tablesz);
	if (ret) {
		dev_err(dev, "Failed to process resources: %d\n", ret);
		goto clean_up;
	}

	/* load the ELF segments to memory */
	ret = rproc_load_segments(rproc, fw->data, fw->size);
	if (ret) {
		dev_err(dev, "Failed to load program segments: %d\n", ret);
		goto clean_up;
	}

	/* power up the remote processor */
	ret = rproc->ops->start(rproc);
	if (ret) {
		dev_err(dev, "can't start rproc %s: %d\n", rproc->name, ret);
		goto clean_up;
	}

	rproc->state = RPROC_RUNNING;

	dev_info(dev, "remote processor %s is now up\n", rproc->name);

	return 0;

clean_up:
	rproc_resource_cleanup(rproc);
=======
	/* Prepare rproc for firmware loading if needed */
	ret = rproc_prepare_device(rproc);
	if (ret) {
		dev_err(dev, "can't prepare rproc %s: %d\n", rproc->name, ret);
		goto disable_iommu;
	}

	rproc->bootaddr = rproc_get_boot_addr(rproc, fw);

	/* Load resource table, core dump segment list etc from the firmware */
	ret = rproc_parse_fw(rproc, fw);
	if (ret)
		goto unprepare_rproc;

	/* reset max_notifyid */
	rproc->max_notifyid = -1;

	/* reset handled vdev */
	rproc->nb_vdev = 0;

	/* handle fw resources which are required to boot rproc */
	ret = rproc_handle_resources(rproc, rproc_loading_handlers);
	if (ret) {
		dev_err(dev, "Failed to process resources: %d\n", ret);
		goto clean_up_resources;
	}

	/* Allocate carveout resources associated to rproc */
	ret = rproc_alloc_registered_carveouts(rproc);
	if (ret) {
		dev_err(dev, "Failed to allocate associated carveouts: %d\n",
			ret);
		goto clean_up_resources;
	}

	ret = rproc_start(rproc, fw);
	if (ret)
		goto clean_up_resources;

	return 0;

clean_up_resources:
	rproc_resource_cleanup(rproc);
	kfree(rproc->cached_table);
	rproc->cached_table = NULL;
	rproc->table_ptr = NULL;
unprepare_rproc:
	/* release HW resources if needed */
	rproc_unprepare_device(rproc);
disable_iommu:
	rproc_disable_iommu(rproc);
	return ret;
}

static int rproc_set_rsc_table(struct rproc *rproc)
{
	struct resource_table *table_ptr;
	struct device *dev = &rproc->dev;
	size_t table_sz;
	int ret;

	table_ptr = rproc_get_loaded_rsc_table(rproc, &table_sz);
	if (!table_ptr) {
		/* Not having a resource table is acceptable */
		return 0;
	}

	if (IS_ERR(table_ptr)) {
		ret = PTR_ERR(table_ptr);
		dev_err(dev, "can't load resource table: %d\n", ret);
		return ret;
	}

	/*
	 * If it is possible to detach the remote processor, keep an untouched
	 * copy of the resource table.  That way we can start fresh again when
	 * the remote processor is re-attached, that is:
	 *
	 *      DETACHED -> ATTACHED -> DETACHED -> ATTACHED
	 *
	 * Free'd in rproc_reset_rsc_table_on_detach() and
	 * rproc_reset_rsc_table_on_stop().
	 */
	if (rproc->ops->detach) {
		rproc->clean_table = kmemdup(table_ptr, table_sz, GFP_KERNEL);
		if (!rproc->clean_table)
			return -ENOMEM;
	} else {
		rproc->clean_table = NULL;
	}

	rproc->cached_table = NULL;
	rproc->table_ptr = table_ptr;
	rproc->table_sz = table_sz;

	return 0;
}

static int rproc_reset_rsc_table_on_detach(struct rproc *rproc)
{
	struct resource_table *table_ptr;

	/* A resource table was never retrieved, nothing to do here */
	if (!rproc->table_ptr)
		return 0;

	/*
	 * If we made it to this point a clean_table _must_ have been
	 * allocated in rproc_set_rsc_table().  If one isn't present
	 * something went really wrong and we must complain.
	 */
	if (WARN_ON(!rproc->clean_table))
		return -EINVAL;

	/* Remember where the external entity installed the resource table */
	table_ptr = rproc->table_ptr;

	/*
	 * If we made it here the remote processor was started by another
	 * entity and a cache table doesn't exist.  As such make a copy of
	 * the resource table currently used by the remote processor and
	 * use that for the rest of the shutdown process.  The memory
	 * allocated here is free'd in rproc_detach().
	 */
	rproc->cached_table = kmemdup(rproc->table_ptr,
				      rproc->table_sz, GFP_KERNEL);
	if (!rproc->cached_table)
		return -ENOMEM;

	/*
	 * Use a copy of the resource table for the remainder of the
	 * shutdown process.
	 */
	rproc->table_ptr = rproc->cached_table;

	/*
	 * Reset the memory area where the firmware loaded the resource table
	 * to its original value.  That way when we re-attach the remote
	 * processor the resource table is clean and ready to be used again.
	 */
	memcpy(table_ptr, rproc->clean_table, rproc->table_sz);

	/*
	 * The clean resource table is no longer needed.  Allocated in
	 * rproc_set_rsc_table().
	 */
	kfree(rproc->clean_table);

	return 0;
}

static int rproc_reset_rsc_table_on_stop(struct rproc *rproc)
{
	/* A resource table was never retrieved, nothing to do here */
	if (!rproc->table_ptr)
		return 0;

	/*
	 * If a cache table exists the remote processor was started by
	 * the remoteproc core.  That cache table should be used for
	 * the rest of the shutdown process.
	 */
	if (rproc->cached_table)
		goto out;

	/*
	 * If we made it here the remote processor was started by another
	 * entity and a cache table doesn't exist.  As such make a copy of
	 * the resource table currently used by the remote processor and
	 * use that for the rest of the shutdown process.  The memory
	 * allocated here is free'd in rproc_shutdown().
	 */
	rproc->cached_table = kmemdup(rproc->table_ptr,
				      rproc->table_sz, GFP_KERNEL);
	if (!rproc->cached_table)
		return -ENOMEM;

	/*
	 * Since the remote processor is being switched off the clean table
	 * won't be needed.  Allocated in rproc_set_rsc_table().
	 */
	kfree(rproc->clean_table);

out:
	/*
	 * Use a copy of the resource table for the remainder of the
	 * shutdown process.
	 */
	rproc->table_ptr = rproc->cached_table;
	return 0;
}

/*
 * Attach to remote processor - similar to rproc_fw_boot() but without
 * the steps that deal with the firmware image.
 */
static int rproc_attach(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	/*
	 * if enabling an IOMMU isn't relevant for this rproc, this is
	 * just a nop
	 */
	ret = rproc_enable_iommu(rproc);
	if (ret) {
		dev_err(dev, "can't enable iommu: %d\n", ret);
		return ret;
	}

	/* Do anything that is needed to boot the remote processor */
	ret = rproc_prepare_device(rproc);
	if (ret) {
		dev_err(dev, "can't prepare rproc %s: %d\n", rproc->name, ret);
		goto disable_iommu;
	}

	ret = rproc_set_rsc_table(rproc);
	if (ret) {
		dev_err(dev, "can't load resource table: %d\n", ret);
		goto unprepare_device;
	}

	/* reset max_notifyid */
	rproc->max_notifyid = -1;

	/* reset handled vdev */
	rproc->nb_vdev = 0;

	/*
	 * Handle firmware resources required to attach to a remote processor.
	 * Because we are attaching rather than booting the remote processor,
	 * we expect the platform driver to properly set rproc->table_ptr.
	 */
	ret = rproc_handle_resources(rproc, rproc_loading_handlers);
	if (ret) {
		dev_err(dev, "Failed to process resources: %d\n", ret);
		goto unprepare_device;
	}

	/* Allocate carveout resources associated to rproc */
	ret = rproc_alloc_registered_carveouts(rproc);
	if (ret) {
		dev_err(dev, "Failed to allocate associated carveouts: %d\n",
			ret);
		goto clean_up_resources;
	}

	ret = __rproc_attach(rproc);
	if (ret)
		goto clean_up_resources;

	return 0;

clean_up_resources:
	rproc_resource_cleanup(rproc);
unprepare_device:
	/* release HW resources if needed */
	rproc_unprepare_device(rproc);
disable_iommu:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rproc_disable_iommu(rproc);
	return ret;
}

/*
<<<<<<< HEAD
 * take a firmware and look for virtio devices to register.
=======
 * take a firmware and boot it up.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Note: this function is called asynchronously upon registration of the
 * remote processor (so we must wait until it completes before we try
 * to unregister the device. one other option is just to use kref here,
 * that might be cleaner).
 */
<<<<<<< HEAD
static void rproc_fw_config_virtio(const struct firmware *fw, void *context)
{
	struct rproc *rproc = context;
	struct resource_table *table;
	int ret, tablesz;

	if (rproc_fw_sanity_check(rproc, fw) < 0)
		goto out;

	/* look for the resource table */
	table = rproc_find_rsc_table(rproc, fw->data, fw->size, &tablesz);
	if (!table)
		goto out;

	/* look for virtio devices and register them */
	ret = rproc_handle_virtio_rsc(rproc, table, tablesz);
	if (ret)
		goto out;

out:
	if (fw)
		release_firmware(fw);
	/* allow rproc_unregister() contexts, if any, to proceed */
	complete_all(&rproc->firmware_loading_complete);
=======
static void rproc_auto_boot_callback(const struct firmware *fw, void *context)
{
	struct rproc *rproc = context;

	rproc_boot(rproc);

	release_firmware(fw);
}

static int rproc_trigger_auto_boot(struct rproc *rproc)
{
	int ret;

	/*
	 * Since the remote processor is in a detached state, it has already
	 * been booted by another entity.  As such there is no point in waiting
	 * for a firmware image to be loaded, we can simply initiate the process
	 * of attaching to it immediately.
	 */
	if (rproc->state == RPROC_DETACHED)
		return rproc_boot(rproc);

	/*
	 * We're initiating an asynchronous firmware loading, so we can
	 * be built-in kernel code, without hanging the boot process.
	 */
	ret = request_firmware_nowait(THIS_MODULE, FW_ACTION_UEVENT,
				      rproc->firmware, &rproc->dev, GFP_KERNEL,
				      rproc, rproc_auto_boot_callback);
	if (ret < 0)
		dev_err(&rproc->dev, "request_firmware_nowait err: %d\n", ret);

	return ret;
}

static int rproc_stop(struct rproc *rproc, bool crashed)
{
	struct device *dev = &rproc->dev;
	int ret;

	/* No need to continue if a stop() operation has not been provided */
	if (!rproc->ops->stop)
		return -EINVAL;

	/* Stop any subdevices for the remote processor */
	rproc_stop_subdevices(rproc, crashed);

	/* the installed resource table is no longer accessible */
	ret = rproc_reset_rsc_table_on_stop(rproc);
	if (ret) {
		dev_err(dev, "can't reset resource table: %d\n", ret);
		return ret;
	}


	/* power off the remote processor */
	ret = rproc->ops->stop(rproc);
	if (ret) {
		dev_err(dev, "can't stop rproc: %d\n", ret);
		return ret;
	}

	rproc_unprepare_subdevices(rproc);

	rproc->state = RPROC_OFFLINE;

	dev_info(dev, "stopped remote processor %s\n", rproc->name);

	return 0;
}

/*
 * __rproc_detach(): Does the opposite of __rproc_attach()
 */
static int __rproc_detach(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	/* No need to continue if a detach() operation has not been provided */
	if (!rproc->ops->detach)
		return -EINVAL;

	/* Stop any subdevices for the remote processor */
	rproc_stop_subdevices(rproc, false);

	/* the installed resource table is no longer accessible */
	ret = rproc_reset_rsc_table_on_detach(rproc);
	if (ret) {
		dev_err(dev, "can't reset resource table: %d\n", ret);
		return ret;
	}

	/* Tell the remote processor the core isn't available anymore */
	ret = rproc->ops->detach(rproc);
	if (ret) {
		dev_err(dev, "can't detach from rproc: %d\n", ret);
		return ret;
	}

	rproc_unprepare_subdevices(rproc);

	rproc->state = RPROC_DETACHED;

	dev_info(dev, "detached remote processor %s\n", rproc->name);

	return 0;
}

static int rproc_attach_recovery(struct rproc *rproc)
{
	int ret;

	ret = __rproc_detach(rproc);
	if (ret)
		return ret;

	return __rproc_attach(rproc);
}

static int rproc_boot_recovery(struct rproc *rproc)
{
	const struct firmware *firmware_p;
	struct device *dev = &rproc->dev;
	int ret;

	ret = rproc_stop(rproc, true);
	if (ret)
		return ret;

	/* generate coredump */
	rproc->ops->coredump(rproc);

	/* load firmware */
	ret = request_firmware(&firmware_p, rproc->firmware, dev);
	if (ret < 0) {
		dev_err(dev, "request_firmware failed: %d\n", ret);
		return ret;
	}

	/* boot the remote processor up again */
	ret = rproc_start(rproc, firmware_p);

	release_firmware(firmware_p);

	return ret;
}

/**
 * rproc_trigger_recovery() - recover a remoteproc
 * @rproc: the remote processor
 *
 * The recovery is done by resetting all the virtio devices, that way all the
 * rpmsg drivers will be reseted along with the remote processor making the
 * remoteproc functional again.
 *
 * This function can sleep, so it cannot be called from atomic context.
 *
 * Return: 0 on success or a negative value upon failure
 */
int rproc_trigger_recovery(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	ret = mutex_lock_interruptible(&rproc->lock);
	if (ret)
		return ret;

	/* State could have changed before we got the mutex */
	if (rproc->state != RPROC_CRASHED)
		goto unlock_mutex;

	dev_err(dev, "recovering %s\n", rproc->name);

	if (rproc_has_feature(rproc, RPROC_FEAT_ATTACH_ON_RECOVERY))
		ret = rproc_attach_recovery(rproc);
	else
		ret = rproc_boot_recovery(rproc);

unlock_mutex:
	mutex_unlock(&rproc->lock);
	return ret;
}

/**
 * rproc_crash_handler_work() - handle a crash
 * @work: work treating the crash
 *
 * This function needs to handle everything related to a crash, like cpu
 * registers and stack dump, information to help to debug the fatal error, etc.
 */
static void rproc_crash_handler_work(struct work_struct *work)
{
	struct rproc *rproc = container_of(work, struct rproc, crash_handler);
	struct device *dev = &rproc->dev;

	dev_dbg(dev, "enter %s\n", __func__);

	mutex_lock(&rproc->lock);

	if (rproc->state == RPROC_CRASHED) {
		/* handle only the first crash detected */
		mutex_unlock(&rproc->lock);
		return;
	}

	if (rproc->state == RPROC_OFFLINE) {
		/* Don't recover if the remote processor was stopped */
		mutex_unlock(&rproc->lock);
		goto out;
	}

	rproc->state = RPROC_CRASHED;
	dev_err(dev, "handling crash #%u in %s\n", ++rproc->crash_cnt,
		rproc->name);

	mutex_unlock(&rproc->lock);

	if (!rproc->recovery_disabled)
		rproc_trigger_recovery(rproc);

out:
	pm_relax(rproc->dev.parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rproc_boot() - boot a remote processor
 * @rproc: handle of a remote processor
 *
 * Boot a remote processor (i.e. load its firmware, power it on, ...).
 *
 * If the remote processor is already powered on, this function immediately
 * returns (successfully).
 *
<<<<<<< HEAD
 * Returns 0 on success, and an appropriate error value otherwise.
=======
 * Return: 0 on success, and an appropriate error value otherwise
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int rproc_boot(struct rproc *rproc)
{
	const struct firmware *firmware_p;
	struct device *dev;
	int ret;

	if (!rproc) {
		pr_err("invalid rproc handle\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	dev = rproc->dev;
=======
	dev = &rproc->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mutex_lock_interruptible(&rproc->lock);
	if (ret) {
		dev_err(dev, "can't lock rproc %s: %d\n", rproc->name, ret);
		return ret;
	}

<<<<<<< HEAD
	/* loading a firmware is required */
	if (!rproc->firmware) {
		dev_err(dev, "%s: no firmware to load\n", __func__);
		ret = -EINVAL;
		goto unlock_mutex;
	}

	/* prevent underlying implementation from being removed */
	if (!try_module_get(dev->driver->owner)) {
		dev_err(dev, "%s: can't get owner\n", __func__);
		ret = -EINVAL;
		goto unlock_mutex;
	}

	/* skip the boot process if rproc is already powered up */
=======
	if (rproc->state == RPROC_DELETED) {
		ret = -ENODEV;
		dev_err(dev, "can't boot deleted rproc %s\n", rproc->name);
		goto unlock_mutex;
	}

	/* skip the boot or attach process if rproc is already powered up */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (atomic_inc_return(&rproc->power) > 1) {
		ret = 0;
		goto unlock_mutex;
	}

<<<<<<< HEAD
	dev_info(dev, "powering up %s\n", rproc->name);

	/* load firmware */
	ret = request_firmware(&firmware_p, rproc->firmware, dev);
	if (ret < 0) {
		dev_err(dev, "request_firmware failed: %d\n", ret);
		goto downref_rproc;
	}

	ret = rproc_fw_boot(rproc, firmware_p);

	release_firmware(firmware_p);

downref_rproc:
	if (ret) {
		module_put(dev->driver->owner);
		atomic_dec(&rproc->power);
	}
=======
	if (rproc->state == RPROC_DETACHED) {
		dev_info(dev, "attaching to %s\n", rproc->name);

		ret = rproc_attach(rproc);
	} else {
		dev_info(dev, "powering up %s\n", rproc->name);

		/* load firmware */
		ret = request_firmware(&firmware_p, rproc->firmware, dev);
		if (ret < 0) {
			dev_err(dev, "request_firmware failed: %d\n", ret);
			goto downref_rproc;
		}

		ret = rproc_fw_boot(rproc, firmware_p);

		release_firmware(firmware_p);
	}

downref_rproc:
	if (ret)
		atomic_dec(&rproc->power);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
unlock_mutex:
	mutex_unlock(&rproc->lock);
	return ret;
}
EXPORT_SYMBOL(rproc_boot);

/**
 * rproc_shutdown() - power off the remote processor
 * @rproc: the remote processor
 *
 * Power off a remote processor (previously booted with rproc_boot()).
 *
 * In case @rproc is still being used by an additional user(s), then
 * this function will just decrement the power refcount and exit,
 * without really powering off the device.
 *
 * Every call to rproc_boot() must (eventually) be accompanied by a call
 * to rproc_shutdown(). Calling rproc_shutdown() redundantly is a bug.
 *
 * Notes:
 * - we're not decrementing the rproc's refcount, only the power refcount.
 *   which means that the @rproc handle stays valid even after rproc_shutdown()
 *   returns, and users can still use it with a subsequent rproc_boot(), if
 *   needed.
<<<<<<< HEAD
 * - don't call rproc_shutdown() to unroll rproc_get_by_name(), exactly
 *   because rproc_shutdown() _does not_ decrement the refcount of @rproc.
 *   To decrement the refcount of @rproc, use rproc_put() (but _only_ if
 *   you acquired @rproc using rproc_get_by_name()).
 */
void rproc_shutdown(struct rproc *rproc)
{
	struct device *dev = rproc->dev;
	int ret;
=======
 *
 * Return: 0 on success, and an appropriate error value otherwise
 */
int rproc_shutdown(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mutex_lock_interruptible(&rproc->lock);
	if (ret) {
		dev_err(dev, "can't lock rproc %s: %d\n", rproc->name, ret);
<<<<<<< HEAD
		return;
=======
		return ret;
	}

	if (rproc->state != RPROC_RUNNING &&
	    rproc->state != RPROC_ATTACHED) {
		ret = -EINVAL;
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* if the remote proc is still needed, bail out */
	if (!atomic_dec_and_test(&rproc->power))
		goto out;

<<<<<<< HEAD
	/* power off the remote processor */
	ret = rproc->ops->stop(rproc);
	if (ret) {
		atomic_inc(&rproc->power);
		dev_err(dev, "can't stop rproc: %d\n", ret);
=======
	ret = rproc_stop(rproc, false);
	if (ret) {
		atomic_inc(&rproc->power);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* clean up all acquired resources */
	rproc_resource_cleanup(rproc);

<<<<<<< HEAD
	rproc_disable_iommu(rproc);

	rproc->state = RPROC_OFFLINE;

	dev_info(dev, "stopped remote processor %s\n", rproc->name);

out:
	mutex_unlock(&rproc->lock);
	if (!ret)
		module_put(dev->driver->owner);
=======
	/* release HW resources if needed */
	rproc_unprepare_device(rproc);

	rproc_disable_iommu(rproc);

	/* Free the copy of the resource table */
	kfree(rproc->cached_table);
	rproc->cached_table = NULL;
	rproc->table_ptr = NULL;
out:
	mutex_unlock(&rproc->lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rproc_shutdown);

/**
<<<<<<< HEAD
 * rproc_release() - completely deletes the existence of a remote processor
 * @kref: the rproc's kref
 *
 * This function should _never_ be called directly.
 *
 * The only reasonable location to use it is as an argument when kref_put'ing
 * @rproc's refcount.
 *
 * This way it will be called when no one holds a valid pointer to this @rproc
 * anymore (and obviously after it is removed from the rprocs klist).
 *
 * Note: this function is not static because rproc_vdev_release() needs it when
 * it decrements @rproc's refcount.
 */
void rproc_release(struct kref *kref)
{
	struct rproc *rproc = container_of(kref, struct rproc, refcount);
	struct rproc_vdev *rvdev, *rvtmp;

	dev_info(rproc->dev, "removing %s\n", rproc->name);

	rproc_delete_debug_dir(rproc);

	/* clean up remote vdev entries */
	list_for_each_entry_safe(rvdev, rvtmp, &rproc->rvdevs, node) {
		__rproc_free_vrings(rvdev, RVDEV_NUM_VRINGS);
		list_del(&rvdev->node);
	}

	/*
	 * At this point no one holds a reference to rproc anymore,
	 * so we can directly unroll rproc_alloc()
	 */
	rproc_free(rproc);
}

/* will be called when an rproc is added to the rprocs klist */
static void klist_rproc_get(struct klist_node *n)
{
	struct rproc *rproc = container_of(n, struct rproc, node);

	kref_get(&rproc->refcount);
}

/* will be called when an rproc is removed from the rprocs klist */
static void klist_rproc_put(struct klist_node *n)
{
	struct rproc *rproc = container_of(n, struct rproc, node);

	kref_put(&rproc->refcount, rproc_release);
}

static struct rproc *next_rproc(struct klist_iter *i)
{
	struct klist_node *n;

	n = klist_next(i);
	if (!n)
		return NULL;

	return container_of(n, struct rproc, node);
}

/**
 * rproc_get_by_name() - find a remote processor by name and boot it
 * @name: name of the remote processor
 *
 * Finds an rproc handle using the remote processor's name, and then
 * boot it. If it's already powered on, then just immediately return
 * (successfully).
 *
 * Returns the rproc handle on success, and NULL on failure.
=======
 * rproc_detach() - Detach the remote processor from the
 * remoteproc core
 *
 * @rproc: the remote processor
 *
 * Detach a remote processor (previously attached to with rproc_attach()).
 *
 * In case @rproc is still being used by an additional user(s), then
 * this function will just decrement the power refcount and exit,
 * without disconnecting the device.
 *
 * Function rproc_detach() calls __rproc_detach() in order to let a remote
 * processor know that services provided by the application processor are
 * no longer available.  From there it should be possible to remove the
 * platform driver and even power cycle the application processor (if the HW
 * supports it) without needing to switch off the remote processor.
 *
 * Return: 0 on success, and an appropriate error value otherwise
 */
int rproc_detach(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	ret = mutex_lock_interruptible(&rproc->lock);
	if (ret) {
		dev_err(dev, "can't lock rproc %s: %d\n", rproc->name, ret);
		return ret;
	}

	if (rproc->state != RPROC_ATTACHED) {
		ret = -EINVAL;
		goto out;
	}

	/* if the remote proc is still needed, bail out */
	if (!atomic_dec_and_test(&rproc->power)) {
		ret = 0;
		goto out;
	}

	ret = __rproc_detach(rproc);
	if (ret) {
		atomic_inc(&rproc->power);
		goto out;
	}

	/* clean up all acquired resources */
	rproc_resource_cleanup(rproc);

	/* release HW resources if needed */
	rproc_unprepare_device(rproc);

	rproc_disable_iommu(rproc);

	/* Free the copy of the resource table */
	kfree(rproc->cached_table);
	rproc->cached_table = NULL;
	rproc->table_ptr = NULL;
out:
	mutex_unlock(&rproc->lock);
	return ret;
}
EXPORT_SYMBOL(rproc_detach);

/**
 * rproc_get_by_phandle() - find a remote processor by phandle
 * @phandle: phandle to the rproc
 *
 * Finds an rproc handle using the remote processor's phandle, and then
 * return a handle to the rproc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function increments the remote processor's refcount, so always
 * use rproc_put() to decrement it back once rproc isn't needed anymore.
 *
<<<<<<< HEAD
 * Note: currently this function (and its counterpart rproc_put()) are not
 * being used. We need to scrutinize the use cases
 * that still need them, and see if we can migrate them to use the non
 * name-based boot/shutdown interface.
 */
struct rproc *rproc_get_by_name(const char *name)
{
	struct rproc *rproc;
	struct klist_iter i;
	int ret;

	/* find the remote processor, and upref its refcount */
	klist_iter_init(&rprocs, &i);
	while ((rproc = next_rproc(&i)) != NULL)
		if (!strcmp(rproc->name, name)) {
			kref_get(&rproc->refcount);
			break;
		}
	klist_iter_exit(&i);

	/* can't find this rproc ? */
	if (!rproc) {
		pr_err("can't find remote processor %s\n", name);
		return NULL;
	}

	ret = rproc_boot(rproc);
	if (ret < 0) {
		kref_put(&rproc->refcount, rproc_release);
		return NULL;
	}

	return rproc;
}
EXPORT_SYMBOL(rproc_get_by_name);

/**
 * rproc_put() - decrement the refcount of a remote processor, and shut it down
 * @rproc: the remote processor
 *
 * This function tries to shutdown @rproc, and it then decrements its
 * refcount.
 *
 * After this function returns, @rproc may _not_ be used anymore, and its
 * handle should be considered invalid.
 *
 * This function should be called _iff_ the @rproc handle was grabbed by
 * calling rproc_get_by_name().
 */
void rproc_put(struct rproc *rproc)
{
	/* try to power off the remote processor */
	rproc_shutdown(rproc);

	/* downref rproc's refcount */
	kref_put(&rproc->refcount, rproc_release);
}
EXPORT_SYMBOL(rproc_put);

/**
 * rproc_register() - register a remote processor
=======
 * Return: rproc handle on success, and NULL on failure
 */
#ifdef CONFIG_OF
struct rproc *rproc_get_by_phandle(phandle phandle)
{
	struct rproc *rproc = NULL, *r;
	struct device_driver *driver;
	struct device_node *np;

	np = of_find_node_by_phandle(phandle);
	if (!np)
		return NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(r, &rproc_list, node) {
		if (r->dev.parent && device_match_of_node(r->dev.parent, np)) {
			/* prevent underlying implementation from being removed */

			/*
			 * If the remoteproc's parent has a driver, the
			 * remoteproc is not part of a cluster and we can use
			 * that driver.
			 */
			driver = r->dev.parent->driver;

			/*
			 * If the remoteproc's parent does not have a driver,
			 * look for the driver associated with the cluster.
			 */
			if (!driver) {
				if (r->dev.parent->parent)
					driver = r->dev.parent->parent->driver;
				if (!driver)
					break;
			}

			if (!try_module_get(driver->owner)) {
				dev_err(&r->dev, "can't get owner\n");
				break;
			}

			rproc = r;
			get_device(&rproc->dev);
			break;
		}
	}
	rcu_read_unlock();

	of_node_put(np);

	return rproc;
}
#else
struct rproc *rproc_get_by_phandle(phandle phandle)
{
	return NULL;
}
#endif
EXPORT_SYMBOL(rproc_get_by_phandle);

/**
 * rproc_set_firmware() - assign a new firmware
 * @rproc: rproc handle to which the new firmware is being assigned
 * @fw_name: new firmware name to be assigned
 *
 * This function allows remoteproc drivers or clients to configure a custom
 * firmware name that is different from the default name used during remoteproc
 * registration. The function does not trigger a remote processor boot,
 * only sets the firmware name used for a subsequent boot. This function
 * should also be called only when the remote processor is offline.
 *
 * This allows either the userspace to configure a different name through
 * sysfs or a kernel-level remoteproc or a remoteproc client driver to set
 * a specific firmware when it is controlling the boot and shutdown of the
 * remote processor.
 *
 * Return: 0 on success or a negative value upon failure
 */
int rproc_set_firmware(struct rproc *rproc, const char *fw_name)
{
	struct device *dev;
	int ret, len;
	char *p;

	if (!rproc || !fw_name)
		return -EINVAL;

	dev = rproc->dev.parent;

	ret = mutex_lock_interruptible(&rproc->lock);
	if (ret) {
		dev_err(dev, "can't lock rproc %s: %d\n", rproc->name, ret);
		return -EINVAL;
	}

	if (rproc->state != RPROC_OFFLINE) {
		dev_err(dev, "can't change firmware while running\n");
		ret = -EBUSY;
		goto out;
	}

	len = strcspn(fw_name, "\n");
	if (!len) {
		dev_err(dev, "can't provide empty string for firmware name\n");
		ret = -EINVAL;
		goto out;
	}

	p = kstrndup(fw_name, len, GFP_KERNEL);
	if (!p) {
		ret = -ENOMEM;
		goto out;
	}

	kfree_const(rproc->firmware);
	rproc->firmware = p;

out:
	mutex_unlock(&rproc->lock);
	return ret;
}
EXPORT_SYMBOL(rproc_set_firmware);

static int rproc_validate(struct rproc *rproc)
{
	switch (rproc->state) {
	case RPROC_OFFLINE:
		/*
		 * An offline processor without a start()
		 * function makes no sense.
		 */
		if (!rproc->ops->start)
			return -EINVAL;
		break;
	case RPROC_DETACHED:
		/*
		 * A remote processor in a detached state without an
		 * attach() function makes not sense.
		 */
		if (!rproc->ops->attach)
			return -EINVAL;
		/*
		 * When attaching to a remote processor the device memory
		 * is already available and as such there is no need to have a
		 * cached table.
		 */
		if (rproc->cached_table)
			return -EINVAL;
		break;
	default:
		/*
		 * When adding a remote processor, the state of the device
		 * can be offline or detached, nothing else.
		 */
		return -EINVAL;
	}

	return 0;
}

/**
 * rproc_add() - register a remote processor
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @rproc: the remote processor handle to register
 *
 * Registers @rproc with the remoteproc framework, after it has been
 * allocated with rproc_alloc().
 *
 * This is called by the platform-specific rproc implementation, whenever
 * a new remote processor device is probed.
 *
<<<<<<< HEAD
 * Returns 0 on success and an appropriate error code otherwise.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Note: this function initiates an asynchronous firmware loading
 * context, which will look for virtio devices supported by the rproc's
 * firmware.
 *
 * If found, those virtio devices will be created and added, so as a result
 * of registering this remote processor, additional virtio drivers might be
 * probed.
<<<<<<< HEAD
 */
int rproc_register(struct rproc *rproc)
{
	struct device *dev = rproc->dev;
	int ret = 0;

	/* expose to rproc_get_by_name users */
	klist_add_tail(&rproc->node, &rprocs);

	dev_info(rproc->dev, "%s is available\n", rproc->name);

	dev_info(dev, "Note: remoteproc is still under development and considered experimental.\n");
	dev_info(dev, "THE BINARY FORMAT IS NOT YET FINALIZED, and backward compatibility isn't yet guaranteed.\n");
=======
 *
 * Return: 0 on success and an appropriate error code otherwise
 */
int rproc_add(struct rproc *rproc)
{
	struct device *dev = &rproc->dev;
	int ret;

	ret = rproc_validate(rproc);
	if (ret < 0)
		return ret;

	/* add char device for this remoteproc */
	ret = rproc_char_device_add(rproc);
	if (ret < 0)
		return ret;

	ret = device_add(dev);
	if (ret < 0) {
		put_device(dev);
		goto rproc_remove_cdev;
	}

	dev_info(dev, "%s is available\n", rproc->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* create debugfs entries */
	rproc_create_debug_dir(rproc);

<<<<<<< HEAD
	/* rproc_unregister() calls must wait until async loader completes */
	init_completion(&rproc->firmware_loading_complete);

	/*
	 * We must retrieve early virtio configuration info from
	 * the firmware (e.g. whether to register a virtio device,
	 * what virtio features does it support, ...).
	 *
	 * We're initiating an asynchronous firmware loading, so we can
	 * be built-in kernel code, without hanging the boot process.
	 */
	ret = request_firmware_nowait(THIS_MODULE, FW_ACTION_HOTPLUG,
					rproc->firmware, dev, GFP_KERNEL,
					rproc, rproc_fw_config_virtio);
	if (ret < 0) {
		dev_err(dev, "request_firmware_nowait failed: %d\n", ret);
		complete_all(&rproc->firmware_loading_complete);
		klist_remove(&rproc->node);
	}

	return ret;
}
EXPORT_SYMBOL(rproc_register);
=======
	/* if rproc is marked always-on, request it to boot */
	if (rproc->auto_boot) {
		ret = rproc_trigger_auto_boot(rproc);
		if (ret < 0)
			goto rproc_remove_dev;
	}

	/* expose to rproc_get_by_phandle users */
	mutex_lock(&rproc_list_mutex);
	list_add_rcu(&rproc->node, &rproc_list);
	mutex_unlock(&rproc_list_mutex);

	return 0;

rproc_remove_dev:
	rproc_delete_debug_dir(rproc);
	device_del(dev);
rproc_remove_cdev:
	rproc_char_device_remove(rproc);
	return ret;
}
EXPORT_SYMBOL(rproc_add);

static void devm_rproc_remove(void *rproc)
{
	rproc_del(rproc);
}

/**
 * devm_rproc_add() - resource managed rproc_add()
 * @dev: the underlying device
 * @rproc: the remote processor handle to register
 *
 * This function performs like rproc_add() but the registered rproc device will
 * automatically be removed on driver detach.
 *
 * Return: 0 on success, negative errno on failure
 */
int devm_rproc_add(struct device *dev, struct rproc *rproc)
{
	int err;

	err = rproc_add(rproc);
	if (err)
		return err;

	return devm_add_action_or_reset(dev, devm_rproc_remove, rproc);
}
EXPORT_SYMBOL(devm_rproc_add);

/**
 * rproc_type_release() - release a remote processor instance
 * @dev: the rproc's device
 *
 * This function should _never_ be called directly.
 *
 * It will be called by the driver core when no one holds a valid pointer
 * to @dev anymore.
 */
static void rproc_type_release(struct device *dev)
{
	struct rproc *rproc = container_of(dev, struct rproc, dev);

	dev_info(&rproc->dev, "releasing %s\n", rproc->name);

	idr_destroy(&rproc->notifyids);

	if (rproc->index >= 0)
		ida_free(&rproc_dev_index, rproc->index);

	kfree_const(rproc->firmware);
	kfree_const(rproc->name);
	kfree(rproc->ops);
	kfree(rproc);
}

static const struct device_type rproc_type = {
	.name		= "remoteproc",
	.release	= rproc_type_release,
};

static int rproc_alloc_firmware(struct rproc *rproc,
				const char *name, const char *firmware)
{
	const char *p;

	/*
	 * Allocate a firmware name if the caller gave us one to work
	 * with.  Otherwise construct a new one using a default pattern.
	 */
	if (firmware)
		p = kstrdup_const(firmware, GFP_KERNEL);
	else
		p = kasprintf(GFP_KERNEL, "rproc-%s-fw", name);

	if (!p)
		return -ENOMEM;

	rproc->firmware = p;

	return 0;
}

static int rproc_alloc_ops(struct rproc *rproc, const struct rproc_ops *ops)
{
	rproc->ops = kmemdup(ops, sizeof(*ops), GFP_KERNEL);
	if (!rproc->ops)
		return -ENOMEM;

	/* Default to rproc_coredump if no coredump function is specified */
	if (!rproc->ops->coredump)
		rproc->ops->coredump = rproc_coredump;

	if (rproc->ops->load)
		return 0;

	/* Default to ELF loader if no load function is specified */
	rproc->ops->load = rproc_elf_load_segments;
	rproc->ops->parse_fw = rproc_elf_load_rsc_table;
	rproc->ops->find_loaded_rsc_table = rproc_elf_find_loaded_rsc_table;
	rproc->ops->sanity_check = rproc_elf_sanity_check;
	rproc->ops->get_boot_addr = rproc_elf_get_boot_addr;

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * rproc_alloc() - allocate a remote processor handle
 * @dev: the underlying device
 * @name: name of this remote processor
 * @ops: platform-specific handlers (mainly start/stop)
<<<<<<< HEAD
 * @firmware: name of firmware file to load
 * @len: length of private data needed by the rproc driver (in bytes)
 *
 * Allocates a new remote processor handle, but does not register
 * it yet.
=======
 * @firmware: name of firmware file to load, can be NULL
 * @len: length of private data needed by the rproc driver (in bytes)
 *
 * Allocates a new remote processor handle, but does not register
 * it yet. if @firmware is NULL, a default name is used.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function should be used by rproc implementations during initialization
 * of the remote processor.
 *
 * After creating an rproc handle using this function, and when ready,
<<<<<<< HEAD
 * implementations should then call rproc_register() to complete
 * the registration of the remote processor.
 *
 * On success the new rproc is returned, and on failure, NULL.
 *
 * Note: _never_ directly deallocate @rproc, even if it was not registered
 * yet. Instead, if you just need to unroll rproc_alloc(), use rproc_free().
 */
struct rproc *rproc_alloc(struct device *dev, const char *name,
				const struct rproc_ops *ops,
				const char *firmware, int len)
=======
 * implementations should then call rproc_add() to complete
 * the registration of the remote processor.
 *
 * Note: _never_ directly deallocate @rproc, even if it was not registered
 * yet. Instead, when you need to unroll rproc_alloc(), use rproc_free().
 *
 * Return: new rproc pointer on success, and NULL on failure
 */
struct rproc *rproc_alloc(struct device *dev, const char *name,
			  const struct rproc_ops *ops,
			  const char *firmware, int len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rproc *rproc;

	if (!dev || !name || !ops)
		return NULL;

	rproc = kzalloc(sizeof(struct rproc) + len, GFP_KERNEL);
<<<<<<< HEAD
	if (!rproc) {
		dev_err(dev, "%s: kzalloc failed\n", __func__);
		return NULL;
	}

	rproc->dev = dev;
	rproc->name = name;
	rproc->ops = ops;
	rproc->firmware = firmware;
	rproc->priv = &rproc[1];

	atomic_set(&rproc->power, 0);

	kref_init(&rproc->refcount);

	mutex_init(&rproc->lock);

	idr_init(&rproc->notifyids);

=======
	if (!rproc)
		return NULL;

	rproc->priv = &rproc[1];
	rproc->auto_boot = true;
	rproc->elf_class = ELFCLASSNONE;
	rproc->elf_machine = EM_NONE;

	device_initialize(&rproc->dev);
	rproc->dev.parent = dev;
	rproc->dev.type = &rproc_type;
	rproc->dev.class = &rproc_class;
	rproc->dev.driver_data = rproc;
	idr_init(&rproc->notifyids);

	rproc->name = kstrdup_const(name, GFP_KERNEL);
	if (!rproc->name)
		goto put_device;

	if (rproc_alloc_firmware(rproc, name, firmware))
		goto put_device;

	if (rproc_alloc_ops(rproc, ops))
		goto put_device;

	/* Assign a unique device index and name */
	rproc->index = ida_alloc(&rproc_dev_index, GFP_KERNEL);
	if (rproc->index < 0) {
		dev_err(dev, "ida_alloc failed: %d\n", rproc->index);
		goto put_device;
	}

	dev_set_name(&rproc->dev, "remoteproc%d", rproc->index);

	atomic_set(&rproc->power, 0);

	mutex_init(&rproc->lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&rproc->carveouts);
	INIT_LIST_HEAD(&rproc->mappings);
	INIT_LIST_HEAD(&rproc->traces);
	INIT_LIST_HEAD(&rproc->rvdevs);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&rproc->subdevs);
	INIT_LIST_HEAD(&rproc->dump_segments);

	INIT_WORK(&rproc->crash_handler, rproc_crash_handler_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rproc->state = RPROC_OFFLINE;

	return rproc;
<<<<<<< HEAD
=======

put_device:
	put_device(&rproc->dev);
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rproc_alloc);

/**
<<<<<<< HEAD
 * rproc_free() - free an rproc handle that was allocated by rproc_alloc
 * @rproc: the remote processor handle
 *
 * This function should _only_ be used if @rproc was only allocated,
 * but not registered yet.
 *
 * If @rproc was already successfully registered (by calling rproc_register()),
 * then use rproc_unregister() instead.
 */
void rproc_free(struct rproc *rproc)
{
	idr_remove_all(&rproc->notifyids);
	idr_destroy(&rproc->notifyids);

	kfree(rproc);
=======
 * rproc_free() - unroll rproc_alloc()
 * @rproc: the remote processor handle
 *
 * This function decrements the rproc dev refcount.
 *
 * If no one holds any reference to rproc anymore, then its refcount would
 * now drop to zero, and it would be freed.
 */
void rproc_free(struct rproc *rproc)
{
	put_device(&rproc->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rproc_free);

/**
<<<<<<< HEAD
 * rproc_unregister() - unregister a remote processor
 * @rproc: rproc handle to unregister
 *
 * Unregisters a remote processor, and decrements its refcount.
 * If its refcount drops to zero, then @rproc will be freed. If not,
 * it will be freed later once the last reference is dropped.
 *
 * This function should be called when the platform specific rproc
 * implementation decides to remove the rproc device. it should
 * _only_ be called if a previous invocation of rproc_register()
 * has completed successfully.
 *
 * After rproc_unregister() returns, @rproc is _not_ valid anymore and
 * it shouldn't be used. More specifically, don't call rproc_free()
 * or try to directly free @rproc after rproc_unregister() returns;
 * none of these are needed, and calling them is a bug.
 *
 * Returns 0 on success and -EINVAL if @rproc isn't valid.
 */
int rproc_unregister(struct rproc *rproc)
{
	struct rproc_vdev *rvdev;

	if (!rproc)
		return -EINVAL;

	/* if rproc is just being registered, wait */
	wait_for_completion(&rproc->firmware_loading_complete);

	/* clean up remote vdev entries */
	list_for_each_entry(rvdev, &rproc->rvdevs, node)
		rproc_remove_virtio_dev(rvdev);

	/* the rproc is downref'ed as soon as it's removed from the klist */
	klist_del(&rproc->node);

	/* the rproc will only be released after its refcount drops to zero */
	kref_put(&rproc->refcount, rproc_release);

	return 0;
}
EXPORT_SYMBOL(rproc_unregister);

static int __init remoteproc_init(void)
{
	rproc_init_debugfs();
	return 0;
}
module_init(remoteproc_init);

static void __exit remoteproc_exit(void)
{
	rproc_exit_debugfs();
}
module_exit(remoteproc_exit);

MODULE_LICENSE("GPL v2");
=======
 * rproc_put() - release rproc reference
 * @rproc: the remote processor handle
 *
 * This function decrements the rproc dev refcount.
 *
 * If no one holds any reference to rproc anymore, then its refcount would
 * now drop to zero, and it would be freed.
 */
void rproc_put(struct rproc *rproc)
{
	if (rproc->dev.parent->driver)
		module_put(rproc->dev.parent->driver->owner);
	else
		module_put(rproc->dev.parent->parent->driver->owner);

	put_device(&rproc->dev);
}
EXPORT_SYMBOL(rproc_put);

/**
 * rproc_del() - unregister a remote processor
 * @rproc: rproc handle to unregister
 *
 * This function should be called when the platform specific rproc
 * implementation decides to remove the rproc device. it should
 * _only_ be called if a previous invocation of rproc_add()
 * has completed successfully.
 *
 * After rproc_del() returns, @rproc isn't freed yet, because
 * of the outstanding reference created by rproc_alloc. To decrement that
 * one last refcount, one still needs to call rproc_free().
 *
 * Return: 0 on success and -EINVAL if @rproc isn't valid
 */
int rproc_del(struct rproc *rproc)
{
	if (!rproc)
		return -EINVAL;

	/* TODO: make sure this works with rproc->power > 1 */
	rproc_shutdown(rproc);

	mutex_lock(&rproc->lock);
	rproc->state = RPROC_DELETED;
	mutex_unlock(&rproc->lock);

	rproc_delete_debug_dir(rproc);

	/* the rproc is downref'ed as soon as it's removed from the klist */
	mutex_lock(&rproc_list_mutex);
	list_del_rcu(&rproc->node);
	mutex_unlock(&rproc_list_mutex);

	/* Ensure that no readers of rproc_list are still active */
	synchronize_rcu();

	device_del(&rproc->dev);
	rproc_char_device_remove(rproc);

	return 0;
}
EXPORT_SYMBOL(rproc_del);

static void devm_rproc_free(struct device *dev, void *res)
{
	rproc_free(*(struct rproc **)res);
}

/**
 * devm_rproc_alloc() - resource managed rproc_alloc()
 * @dev: the underlying device
 * @name: name of this remote processor
 * @ops: platform-specific handlers (mainly start/stop)
 * @firmware: name of firmware file to load, can be NULL
 * @len: length of private data needed by the rproc driver (in bytes)
 *
 * This function performs like rproc_alloc() but the acquired rproc device will
 * automatically be released on driver detach.
 *
 * Return: new rproc instance, or NULL on failure
 */
struct rproc *devm_rproc_alloc(struct device *dev, const char *name,
			       const struct rproc_ops *ops,
			       const char *firmware, int len)
{
	struct rproc **ptr, *rproc;

	ptr = devres_alloc(devm_rproc_free, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return NULL;

	rproc = rproc_alloc(dev, name, ops, firmware, len);
	if (rproc) {
		*ptr = rproc;
		devres_add(dev, ptr);
	} else {
		devres_free(ptr);
	}

	return rproc;
}
EXPORT_SYMBOL(devm_rproc_alloc);

/**
 * rproc_add_subdev() - add a subdevice to a remoteproc
 * @rproc: rproc handle to add the subdevice to
 * @subdev: subdev handle to register
 *
 * Caller is responsible for populating optional subdevice function pointers.
 */
void rproc_add_subdev(struct rproc *rproc, struct rproc_subdev *subdev)
{
	list_add_tail(&subdev->node, &rproc->subdevs);
}
EXPORT_SYMBOL(rproc_add_subdev);

/**
 * rproc_remove_subdev() - remove a subdevice from a remoteproc
 * @rproc: rproc handle to remove the subdevice from
 * @subdev: subdev handle, previously registered with rproc_add_subdev()
 */
void rproc_remove_subdev(struct rproc *rproc, struct rproc_subdev *subdev)
{
	list_del(&subdev->node);
}
EXPORT_SYMBOL(rproc_remove_subdev);

/**
 * rproc_get_by_child() - acquire rproc handle of @dev's ancestor
 * @dev:	child device to find ancestor of
 *
 * Return: the ancestor rproc instance, or NULL if not found
 */
struct rproc *rproc_get_by_child(struct device *dev)
{
	for (dev = dev->parent; dev; dev = dev->parent) {
		if (dev->type == &rproc_type)
			return dev->driver_data;
	}

	return NULL;
}
EXPORT_SYMBOL(rproc_get_by_child);

/**
 * rproc_report_crash() - rproc crash reporter function
 * @rproc: remote processor
 * @type: crash type
 *
 * This function must be called every time a crash is detected by the low-level
 * drivers implementing a specific remoteproc. This should not be called from a
 * non-remoteproc driver.
 *
 * This function can be called from atomic/interrupt context.
 */
void rproc_report_crash(struct rproc *rproc, enum rproc_crash_type type)
{
	if (!rproc) {
		pr_err("NULL rproc pointer\n");
		return;
	}

	/* Prevent suspend while the remoteproc is being recovered */
	pm_stay_awake(rproc->dev.parent);

	dev_err(&rproc->dev, "crash detected in %s: type %s\n",
		rproc->name, rproc_crash_to_string(type));

	queue_work(rproc_recovery_wq, &rproc->crash_handler);
}
EXPORT_SYMBOL(rproc_report_crash);

static int rproc_panic_handler(struct notifier_block *nb, unsigned long event,
			       void *ptr)
{
	unsigned int longest = 0;
	struct rproc *rproc;
	unsigned int d;

	rcu_read_lock();
	list_for_each_entry_rcu(rproc, &rproc_list, node) {
		if (!rproc->ops->panic)
			continue;

		if (rproc->state != RPROC_RUNNING &&
		    rproc->state != RPROC_ATTACHED)
			continue;

		d = rproc->ops->panic(rproc);
		longest = max(longest, d);
	}
	rcu_read_unlock();

	/*
	 * Delay for the longest requested duration before returning. This can
	 * be used by the remoteproc drivers to give the remote processor time
	 * to perform any requested operations (such as flush caches), when
	 * it's not possible to signal the Linux side due to the panic.
	 */
	mdelay(longest);

	return NOTIFY_DONE;
}

static void __init rproc_init_panic(void)
{
	rproc_panic_nb.notifier_call = rproc_panic_handler;
	atomic_notifier_chain_register(&panic_notifier_list, &rproc_panic_nb);
}

static void __exit rproc_exit_panic(void)
{
	atomic_notifier_chain_unregister(&panic_notifier_list, &rproc_panic_nb);
}

static int __init remoteproc_init(void)
{
	rproc_recovery_wq = alloc_workqueue("rproc_recovery_wq",
						WQ_UNBOUND | WQ_FREEZABLE, 0);
	if (!rproc_recovery_wq) {
		pr_err("remoteproc: creation of rproc_recovery_wq failed\n");
		return -ENOMEM;
	}

	rproc_init_sysfs();
	rproc_init_debugfs();
	rproc_init_cdev();
	rproc_init_panic();

	return 0;
}
subsys_initcall(remoteproc_init);

static void __exit remoteproc_exit(void)
{
	ida_destroy(&rproc_dev_index);

	if (!rproc_recovery_wq)
		return;

	rproc_exit_panic();
	rproc_exit_debugfs();
	rproc_exit_sysfs();
	destroy_workqueue(rproc_recovery_wq);
}
module_exit(remoteproc_exit);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("Generic Remote Processor Framework");
