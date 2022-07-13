<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Memory subsystem support
 *
 * Written by Matt Tolentino <matthew.e.tolentino@intel.com>
 *            Dave Hansen <haveblue@us.ibm.com>
 *
 * This file provides the necessary infrastructure to represent
 * a SPARSEMEM-memory-model system's physical memory in /sysfs.
 * All arch-independent code that assumes MEMORY_HOTPLUG requires
 * SPARSEMEM should be contained here, or in mm/memory_hotplug.c.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/topology.h>
#include <linux/capability.h>
#include <linux/device.h>
#include <linux/memory.h>
<<<<<<< HEAD
#include <linux/kobject.h>
#include <linux/memory_hotplug.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/stat.h>
#include <linux/slab.h>

#include <linux/atomic.h>
#include <asm/uaccess.h>

static DEFINE_MUTEX(mem_sysfs_mutex);

#define MEMORY_CLASS_NAME	"memory"

static int sections_per_block;

static inline int base_memory_block_id(int section_nr)
=======
#include <linux/memory_hotplug.h>
#include <linux/mm.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/xarray.h>

#include <linux/atomic.h>
#include <linux/uaccess.h>

#define MEMORY_CLASS_NAME	"memory"

static const char *const online_type_to_str[] = {
	[MMOP_OFFLINE] = "offline",
	[MMOP_ONLINE] = "online",
	[MMOP_ONLINE_KERNEL] = "online_kernel",
	[MMOP_ONLINE_MOVABLE] = "online_movable",
};

int mhp_online_type_from_str(const char *str)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(online_type_to_str); i++) {
		if (sysfs_streq(str, online_type_to_str[i]))
			return i;
	}
	return -EINVAL;
}

#define to_memory_block(dev) container_of(dev, struct memory_block, dev)

static int sections_per_block;

static inline unsigned long memory_block_id(unsigned long section_nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return section_nr / sections_per_block;
}

<<<<<<< HEAD
static struct bus_type memory_subsys = {
	.name = MEMORY_CLASS_NAME,
	.dev_name = MEMORY_CLASS_NAME,
};

=======
static inline unsigned long pfn_to_block_id(unsigned long pfn)
{
	return memory_block_id(pfn_to_section_nr(pfn));
}

static inline unsigned long phys_to_block_id(unsigned long phys)
{
	return pfn_to_block_id(PFN_DOWN(phys));
}

static int memory_subsys_online(struct device *dev);
static int memory_subsys_offline(struct device *dev);

static const struct bus_type memory_subsys = {
	.name = MEMORY_CLASS_NAME,
	.dev_name = MEMORY_CLASS_NAME,
	.online = memory_subsys_online,
	.offline = memory_subsys_offline,
};

/*
 * Memory blocks are cached in a local radix tree to avoid
 * a costly linear search for the corresponding device on
 * the subsystem bus.
 */
static DEFINE_XARRAY(memory_blocks);

/*
 * Memory groups, indexed by memory group id (mgid).
 */
static DEFINE_XARRAY_FLAGS(memory_groups, XA_FLAGS_ALLOC);
#define MEMORY_GROUP_MARK_DYNAMIC	XA_MARK_1

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static BLOCKING_NOTIFIER_HEAD(memory_chain);

int register_memory_notifier(struct notifier_block *nb)
{
<<<<<<< HEAD
        return blocking_notifier_chain_register(&memory_chain, nb);
=======
	return blocking_notifier_chain_register(&memory_chain, nb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(register_memory_notifier);

void unregister_memory_notifier(struct notifier_block *nb)
{
<<<<<<< HEAD
        blocking_notifier_chain_unregister(&memory_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_notifier);

static ATOMIC_NOTIFIER_HEAD(memory_isolate_chain);

int register_memory_isolate_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(register_memory_isolate_notifier);

void unregister_memory_isolate_notifier(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_isolate_notifier);

/*
 * register_memory - Setup a sysfs device for a memory block
 */
static
int register_memory(struct memory_block *memory)
{
	int error;

	memory->dev.bus = &memory_subsys;
	memory->dev.id = memory->start_section_nr / sections_per_block;

	error = device_register(&memory->dev);
	return error;
}

static void
unregister_memory(struct memory_block *memory)
{
	BUG_ON(memory->dev.bus != &memory_subsys);

	/* drop the ref. we got in remove_memory_block() */
	kobject_put(&memory->dev.kobj);
	device_unregister(&memory->dev);
=======
	blocking_notifier_chain_unregister(&memory_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_notifier);

static void memory_block_release(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);
	/* Verify that the altmap is freed */
	WARN_ON(mem->altmap);
	kfree(mem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

unsigned long __weak memory_block_size_bytes(void)
{
	return MIN_MEMORY_BLOCK_SIZE;
}
<<<<<<< HEAD

static unsigned long get_memory_block_size(void)
{
	unsigned long block_sz;

	block_sz = memory_block_size_bytes();

	/* Validate blk_sz is a power of 2 and not less than section size */
	if ((block_sz & (block_sz - 1)) || (block_sz < MIN_MEMORY_BLOCK_SIZE)) {
		WARN_ON(1);
		block_sz = MIN_MEMORY_BLOCK_SIZE;
	}

	return block_sz;
}

/*
 * use this as the physical section index that this memsection
 * uses.
 */

static ssize_t show_mem_start_phys_index(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
	unsigned long phys_index;

	phys_index = mem->start_section_nr / sections_per_block;
	return sprintf(buf, "%08lx\n", phys_index);
}

static ssize_t show_mem_end_phys_index(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
	unsigned long phys_index;

	phys_index = mem->end_section_nr / sections_per_block;
	return sprintf(buf, "%08lx\n", phys_index);
}

/*
 * Show whether the section of memory is likely to be hot-removable
 */
static ssize_t show_mem_removable(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	unsigned long i, pfn;
	int ret = 1;
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);

	for (i = 0; i < sections_per_block; i++) {
		if (!present_section_nr(mem->start_section_nr + i))
			continue;
		pfn = section_nr_to_pfn(mem->start_section_nr + i);
		ret &= is_mem_section_removable(pfn, PAGES_PER_SECTION);
	}

	return sprintf(buf, "%d\n", ret);
=======
EXPORT_SYMBOL_GPL(memory_block_size_bytes);

/* Show the memory block ID, relative to the memory block size */
static ssize_t phys_index_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);

	return sysfs_emit(buf, "%08lx\n", memory_block_id(mem->start_section_nr));
}

/*
 * Legacy interface that we cannot remove. Always indicate "removable"
 * with CONFIG_MEMORY_HOTREMOVE - bad heuristic.
 */
static ssize_t removable_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	return sysfs_emit(buf, "%d\n", (int)IS_ENABLED(CONFIG_MEMORY_HOTREMOVE));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * online, offline, going offline, etc.
 */
<<<<<<< HEAD
static ssize_t show_mem_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
	ssize_t len = 0;
=======
static ssize_t state_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	const char *output;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We can probably put these states in a nice little array
	 * so that they're not open-coded
	 */
	switch (mem->state) {
<<<<<<< HEAD
		case MEM_ONLINE:
			len = sprintf(buf, "online\n");
			break;
		case MEM_OFFLINE:
			len = sprintf(buf, "offline\n");
			break;
		case MEM_GOING_OFFLINE:
			len = sprintf(buf, "going-offline\n");
			break;
		default:
			len = sprintf(buf, "ERROR-UNKNOWN-%ld\n",
					mem->state);
			WARN_ON(1);
			break;
	}

	return len;
=======
	case MEM_ONLINE:
		output = "online";
		break;
	case MEM_OFFLINE:
		output = "offline";
		break;
	case MEM_GOING_OFFLINE:
		output = "going-offline";
		break;
	default:
		WARN_ON(1);
		return sysfs_emit(buf, "ERROR-UNKNOWN-%ld\n", mem->state);
	}

	return sysfs_emit(buf, "%s\n", output);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int memory_notify(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&memory_chain, val, v);
}

<<<<<<< HEAD
int memory_isolate_notify(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&memory_isolate_chain, val, v);
}

/*
 * The probe routines leave the pages reserved, just as the bootmem code does.
 * Make sure they're still that way.
 */
static bool pages_correctly_reserved(unsigned long start_pfn,
					unsigned long nr_pages)
{
	int i, j;
	struct page *page;
	unsigned long pfn = start_pfn;

	/*
	 * memmap between sections is not contiguous except with
	 * SPARSEMEM_VMEMMAP. We lookup the page once per section
	 * and assume memmap is contiguous within each section
	 */
	for (i = 0; i < sections_per_block; i++, pfn += PAGES_PER_SECTION) {
		if (WARN_ON_ONCE(!pfn_valid(pfn)))
			return false;
		page = pfn_to_page(pfn);

		for (j = 0; j < PAGES_PER_SECTION; j++) {
			if (PageReserved(page + j))
				continue;

			printk(KERN_WARNING "section number %ld page number %d "
				"not reserved, was it already online?\n",
				pfn_to_section_nr(pfn), j);

			return false;
		}
	}

	return true;
=======
#if defined(CONFIG_MEMORY_FAILURE) && defined(CONFIG_MEMORY_HOTPLUG)
static unsigned long memblk_nr_poison(struct memory_block *mem);
#else
static inline unsigned long memblk_nr_poison(struct memory_block *mem)
{
	return 0;
}
#endif

/*
 * Must acquire mem_hotplug_lock in write mode.
 */
static int memory_block_online(struct memory_block *mem)
{
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	unsigned long nr_vmemmap_pages = 0;
	struct memory_notify arg;
	struct zone *zone;
	int ret;

	if (memblk_nr_poison(mem))
		return -EHWPOISON;

	zone = zone_for_pfn_range(mem->online_type, mem->nid, mem->group,
				  start_pfn, nr_pages);

	/*
	 * Although vmemmap pages have a different lifecycle than the pages
	 * they describe (they remain until the memory is unplugged), doing
	 * their initialization and accounting at memory onlining/offlining
	 * stage helps to keep accounting easier to follow - e.g vmemmaps
	 * belong to the same zone as the memory they backed.
	 */
	if (mem->altmap)
		nr_vmemmap_pages = mem->altmap->free;

	arg.altmap_start_pfn = start_pfn;
	arg.altmap_nr_pages = nr_vmemmap_pages;
	arg.start_pfn = start_pfn + nr_vmemmap_pages;
	arg.nr_pages = nr_pages - nr_vmemmap_pages;
	mem_hotplug_begin();
	ret = memory_notify(MEM_PREPARE_ONLINE, &arg);
	ret = notifier_to_errno(ret);
	if (ret)
		goto out_notifier;

	if (nr_vmemmap_pages) {
		ret = mhp_init_memmap_on_memory(start_pfn, nr_vmemmap_pages,
						zone, mem->altmap->inaccessible);
		if (ret)
			goto out;
	}

	ret = online_pages(start_pfn + nr_vmemmap_pages,
			   nr_pages - nr_vmemmap_pages, zone, mem->group);
	if (ret) {
		if (nr_vmemmap_pages)
			mhp_deinit_memmap_on_memory(start_pfn, nr_vmemmap_pages);
		goto out;
	}

	/*
	 * Account once onlining succeeded. If the zone was unpopulated, it is
	 * now already properly populated.
	 */
	if (nr_vmemmap_pages)
		adjust_present_page_count(pfn_to_page(start_pfn), mem->group,
					  nr_vmemmap_pages);

	mem->zone = zone;
	mem_hotplug_done();
	return ret;
out:
	memory_notify(MEM_FINISH_OFFLINE, &arg);
out_notifier:
	mem_hotplug_done();
	return ret;
}

/*
 * Must acquire mem_hotplug_lock in write mode.
 */
static int memory_block_offline(struct memory_block *mem)
{
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	unsigned long nr_vmemmap_pages = 0;
	struct memory_notify arg;
	int ret;

	if (!mem->zone)
		return -EINVAL;

	/*
	 * Unaccount before offlining, such that unpopulated zone and kthreads
	 * can properly be torn down in offline_pages().
	 */
	if (mem->altmap)
		nr_vmemmap_pages = mem->altmap->free;

	mem_hotplug_begin();
	if (nr_vmemmap_pages)
		adjust_present_page_count(pfn_to_page(start_pfn), mem->group,
					  -nr_vmemmap_pages);

	ret = offline_pages(start_pfn + nr_vmemmap_pages,
			    nr_pages - nr_vmemmap_pages, mem->zone, mem->group);
	if (ret) {
		/* offline_pages() failed. Account back. */
		if (nr_vmemmap_pages)
			adjust_present_page_count(pfn_to_page(start_pfn),
						  mem->group, nr_vmemmap_pages);
		goto out;
	}

	if (nr_vmemmap_pages)
		mhp_deinit_memmap_on_memory(start_pfn, nr_vmemmap_pages);

	mem->zone = NULL;
	arg.altmap_start_pfn = start_pfn;
	arg.altmap_nr_pages = nr_vmemmap_pages;
	arg.start_pfn = start_pfn + nr_vmemmap_pages;
	arg.nr_pages = nr_pages - nr_vmemmap_pages;
	memory_notify(MEM_FINISH_OFFLINE, &arg);
out:
	mem_hotplug_done();
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * MEMORY_HOTPLUG depends on SPARSEMEM in mm/Kconfig, so it is
 * OK to have direct references to sparsemem variables in here.
 */
static int
<<<<<<< HEAD
memory_block_action(unsigned long phys_index, unsigned long action)
{
	unsigned long start_pfn, start_paddr;
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	struct page *first_page;
	int ret;

	first_page = pfn_to_page(phys_index << PFN_SECTION_SHIFT);

	switch (action) {
		case MEM_ONLINE:
			start_pfn = page_to_pfn(first_page);

			if (!pages_correctly_reserved(start_pfn, nr_pages))
				return -EBUSY;

			ret = online_pages(start_pfn, nr_pages);
			break;
		case MEM_OFFLINE:
			start_paddr = page_to_pfn(first_page) << PAGE_SHIFT;
			ret = remove_memory(start_paddr,
					    nr_pages << PAGE_SHIFT);
			break;
		default:
			WARN(1, KERN_WARNING "%s(%ld, %ld) unknown action: "
			     "%ld\n", __func__, phys_index, action, action);
			ret = -EINVAL;
=======
memory_block_action(struct memory_block *mem, unsigned long action)
{
	int ret;

	switch (action) {
	case MEM_ONLINE:
		ret = memory_block_online(mem);
		break;
	case MEM_OFFLINE:
		ret = memory_block_offline(mem);
		break;
	default:
		WARN(1, KERN_WARNING "%s(%ld, %ld) unknown action: "
		     "%ld\n", __func__, mem->start_section_nr, action, action);
		ret = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

static int memory_block_change_state(struct memory_block *mem,
		unsigned long to_state, unsigned long from_state_req)
{
	int ret = 0;

<<<<<<< HEAD
	mutex_lock(&mem->state_mutex);

	if (mem->state != from_state_req) {
		ret = -EINVAL;
		goto out;
	}
=======
	if (mem->state != from_state_req)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (to_state == MEM_OFFLINE)
		mem->state = MEM_GOING_OFFLINE;

<<<<<<< HEAD
	ret = memory_block_action(mem->start_section_nr, to_state);

	if (ret) {
		mem->state = from_state_req;
		goto out;
	}

	mem->state = to_state;
	switch (mem->state) {
	case MEM_OFFLINE:
		kobject_uevent(&mem->dev.kobj, KOBJ_OFFLINE);
		break;
	case MEM_ONLINE:
		kobject_uevent(&mem->dev.kobj, KOBJ_ONLINE);
		break;
	default:
		break;
	}
out:
	mutex_unlock(&mem->state_mutex);
	return ret;
}

static ssize_t
store_mem_state(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct memory_block *mem;
	int ret = -EINVAL;

	mem = container_of(dev, struct memory_block, dev);

	if (!strncmp(buf, "online", min((int)count, 6)))
		ret = memory_block_change_state(mem, MEM_ONLINE, MEM_OFFLINE);
	else if(!strncmp(buf, "offline", min((int)count, 7)))
		ret = memory_block_change_state(mem, MEM_OFFLINE, MEM_ONLINE);

	if (ret)
		return ret;
=======
	ret = memory_block_action(mem, to_state);
	mem->state = ret ? from_state_req : to_state;

	return ret;
}

/* The device lock serializes operations on memory_subsys_[online|offline] */
static int memory_subsys_online(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);
	int ret;

	if (mem->state == MEM_ONLINE)
		return 0;

	/*
	 * When called via device_online() without configuring the online_type,
	 * we want to default to MMOP_ONLINE.
	 */
	if (mem->online_type == MMOP_OFFLINE)
		mem->online_type = MMOP_ONLINE;

	ret = memory_block_change_state(mem, MEM_ONLINE, MEM_OFFLINE);
	mem->online_type = MMOP_OFFLINE;

	return ret;
}

static int memory_subsys_offline(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);

	if (mem->state == MEM_OFFLINE)
		return 0;

	return memory_block_change_state(mem, MEM_OFFLINE, MEM_ONLINE);
}

static ssize_t state_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	const int online_type = mhp_online_type_from_str(buf);
	struct memory_block *mem = to_memory_block(dev);
	int ret;

	if (online_type < 0)
		return -EINVAL;

	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

	switch (online_type) {
	case MMOP_ONLINE_KERNEL:
	case MMOP_ONLINE_MOVABLE:
	case MMOP_ONLINE:
		/* mem->online_type is protected by device_hotplug_lock */
		mem->online_type = online_type;
		ret = device_online(&mem->dev);
		break;
	case MMOP_OFFLINE:
		ret = device_offline(&mem->dev);
		break;
	default:
		ret = -EINVAL; /* should never happen */
	}

	unlock_device_hotplug();

	if (ret < 0)
		return ret;
	if (ret)
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

/*
<<<<<<< HEAD
 * phys_device is a bad name for this.  What I really want
 * is a way to differentiate between memory ranges that
 * are part of physical devices that constitute
 * a complete removable unit or fru.
 * i.e. do these ranges belong to the same physical device,
 * s.t. if I offline all of these sections I can then
 * remove the physical device?
 */
static ssize_t show_phys_device(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
	return sprintf(buf, "%d\n", mem->phys_device);
}

static DEVICE_ATTR(phys_index, 0444, show_mem_start_phys_index, NULL);
static DEVICE_ATTR(end_phys_index, 0444, show_mem_end_phys_index, NULL);
static DEVICE_ATTR(state, 0644, show_mem_state, store_mem_state);
static DEVICE_ATTR(phys_device, 0444, show_phys_device, NULL);
static DEVICE_ATTR(removable, 0444, show_mem_removable, NULL);

#define mem_create_simple_file(mem, attr_name)	\
	device_create_file(&mem->dev, &dev_attr_##attr_name)
#define mem_remove_simple_file(mem, attr_name)	\
	device_remove_file(&mem->dev, &dev_attr_##attr_name)

/*
 * Block size attribute stuff
 */
static ssize_t
print_block_size(struct device *dev, struct device_attribute *attr,
		 char *buf)
{
	return sprintf(buf, "%lx\n", get_memory_block_size());
}

static DEVICE_ATTR(block_size_bytes, 0444, print_block_size, NULL);

static int block_size_init(void)
{
	return device_create_file(memory_subsys.dev_root,
				  &dev_attr_block_size_bytes);
}

=======
 * Legacy interface that we cannot remove: s390x exposes the storage increment
 * covered by a memory block, allowing for identifying which memory blocks
 * comprise a storage increment. Since a memory block spans complete
 * storage increments nowadays, this interface is basically unused. Other
 * archs never exposed != 0.
 */
static ssize_t phys_device_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);

	return sysfs_emit(buf, "%d\n",
			  arch_get_memory_phys_device(start_pfn));
}

#ifdef CONFIG_MEMORY_HOTREMOVE
static int print_allowed_zone(char *buf, int len, int nid,
			      struct memory_group *group,
			      unsigned long start_pfn, unsigned long nr_pages,
			      int online_type, struct zone *default_zone)
{
	struct zone *zone;

	zone = zone_for_pfn_range(online_type, nid, group, start_pfn, nr_pages);
	if (zone == default_zone)
		return 0;

	return sysfs_emit_at(buf, len, " %s", zone->name);
}

static ssize_t valid_zones_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	struct memory_group *group = mem->group;
	struct zone *default_zone;
	int nid = mem->nid;
	int len = 0;

	/*
	 * Check the existing zone. Make sure that we do that only on the
	 * online nodes otherwise the page_zone is not reliable
	 */
	if (mem->state == MEM_ONLINE) {
		/*
		 * If !mem->zone, the memory block spans multiple zones and
		 * cannot get offlined.
		 */
		default_zone = mem->zone;
		if (!default_zone)
			return sysfs_emit(buf, "%s\n", "none");
		len += sysfs_emit_at(buf, len, "%s", default_zone->name);
		goto out;
	}

	default_zone = zone_for_pfn_range(MMOP_ONLINE, nid, group,
					  start_pfn, nr_pages);

	len += sysfs_emit_at(buf, len, "%s", default_zone->name);
	len += print_allowed_zone(buf, len, nid, group, start_pfn, nr_pages,
				  MMOP_ONLINE_KERNEL, default_zone);
	len += print_allowed_zone(buf, len, nid, group, start_pfn, nr_pages,
				  MMOP_ONLINE_MOVABLE, default_zone);
out:
	len += sysfs_emit_at(buf, len, "\n");
	return len;
}
static DEVICE_ATTR_RO(valid_zones);
#endif

static DEVICE_ATTR_RO(phys_index);
static DEVICE_ATTR_RW(state);
static DEVICE_ATTR_RO(phys_device);
static DEVICE_ATTR_RO(removable);

/*
 * Show the memory block size (shared by all memory blocks).
 */
static ssize_t block_size_bytes_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lx\n", memory_block_size_bytes());
}

static DEVICE_ATTR_RO(block_size_bytes);

/*
 * Memory auto online policy.
 */

static ssize_t auto_online_blocks_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%s\n",
			  online_type_to_str[mhp_default_online_type]);
}

static ssize_t auto_online_blocks_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	const int online_type = mhp_online_type_from_str(buf);

	if (online_type < 0)
		return -EINVAL;

	mhp_default_online_type = online_type;
	return count;
}

static DEVICE_ATTR_RW(auto_online_blocks);

#ifdef CONFIG_CRASH_HOTPLUG
#include <linux/kexec.h>
static ssize_t crash_hotplug_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", crash_hotplug_memory_support());
}
static DEVICE_ATTR_RO(crash_hotplug);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Some architectures will have custom drivers to do this, and
 * will not need to do it from userspace.  The fake hot-add code
 * as well as ppc64 will do all of their discovery in userspace
 * and will require this interface.
 */
#ifdef CONFIG_ARCH_MEMORY_PROBE
<<<<<<< HEAD
static ssize_t
memory_probe_store(struct device *dev, struct device_attribute *attr,
		   const char *buf, size_t count)
{
	u64 phys_addr;
	int nid;
	int i, ret;
	unsigned long pages_per_block = PAGES_PER_SECTION * sections_per_block;

	phys_addr = simple_strtoull(buf, NULL, 0);
=======
static ssize_t probe_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	u64 phys_addr;
	int nid, ret;
	unsigned long pages_per_block = PAGES_PER_SECTION * sections_per_block;

	ret = kstrtoull(buf, 0, &phys_addr);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (phys_addr & ((pages_per_block << PAGE_SHIFT) - 1))
		return -EINVAL;

<<<<<<< HEAD
	for (i = 0; i < sections_per_block; i++) {
		nid = memory_add_physaddr_to_nid(phys_addr);
		ret = add_memory(nid, phys_addr,
				 PAGES_PER_SECTION << PAGE_SHIFT);
		if (ret)
			goto out;

		phys_addr += MIN_MEMORY_BLOCK_SIZE;
	}

	ret = count;
out:
	return ret;
}
static DEVICE_ATTR(probe, S_IWUSR, NULL, memory_probe_store);

static int memory_probe_init(void)
{
	return device_create_file(memory_subsys.dev_root, &dev_attr_probe);
}
#else
static inline int memory_probe_init(void)
{
	return 0;
}
=======
	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

	nid = memory_add_physaddr_to_nid(phys_addr);
	ret = __add_memory(nid, phys_addr,
			   MIN_MEMORY_BLOCK_SIZE * sections_per_block,
			   MHP_NONE);

	if (ret)
		goto out;

	ret = count;
out:
	unlock_device_hotplug();
	return ret;
}

static DEVICE_ATTR_WO(probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_MEMORY_FAILURE
/*
 * Support for offlining pages of memory
 */

/* Soft offline a page */
<<<<<<< HEAD
static ssize_t
store_soft_offline_page(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
=======
static ssize_t soft_offline_page_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
<<<<<<< HEAD
	if (strict_strtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	if (!pfn_valid(pfn))
		return -ENXIO;
	ret = soft_offline_page(pfn_to_page(pfn), 0);
=======
	if (kstrtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	ret = soft_offline_page(pfn, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret == 0 ? count : ret;
}

/* Forcibly offline a page, including killing processes. */
<<<<<<< HEAD
static ssize_t
store_hard_offline_page(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
=======
static ssize_t hard_offline_page_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
<<<<<<< HEAD
	if (strict_strtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	ret = memory_failure(pfn, 0, 0);
	return ret ? ret : count;
}

static DEVICE_ATTR(soft_offline_page, 0644, NULL, store_soft_offline_page);
static DEVICE_ATTR(hard_offline_page, 0644, NULL, store_hard_offline_page);

static __init int memory_fail_init(void)
{
	int err;

	err = device_create_file(memory_subsys.dev_root,
				&dev_attr_soft_offline_page);
	if (!err)
		err = device_create_file(memory_subsys.dev_root,
				&dev_attr_hard_offline_page);
	return err;
}
#else
static inline int memory_fail_init(void)
{
	return 0;
}
#endif

/*
 * Note that phys_device is optional.  It is here to allow for
 * differentiation between which *physical* devices each
 * section belongs to...
 */
=======
	if (kstrtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	ret = memory_failure(pfn, MF_SW_SIMULATED);
	if (ret == -EOPNOTSUPP)
		ret = 0;
	return ret ? ret : count;
}

static DEVICE_ATTR_WO(soft_offline_page);
static DEVICE_ATTR_WO(hard_offline_page);
#endif

/* See phys_device_show(). */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __weak arch_get_memory_phys_device(unsigned long start_pfn)
{
	return 0;
}

/*
<<<<<<< HEAD
 * A reference for the returned object is held and the reference for the
 * hinted object is released.
 */
struct memory_block *find_memory_block_hinted(struct mem_section *section,
					      struct memory_block *hint)
{
	int block_id = base_memory_block_id(__section_nr(section));
	struct device *hintdev = hint ? &hint->dev : NULL;
	struct device *dev;

	dev = subsys_find_device_by_id(&memory_subsys, block_id, hintdev);
	if (hint)
		put_device(&hint->dev);
	if (!dev)
		return NULL;
	return container_of(dev, struct memory_block, dev);
}

/*
 * For now, we have a linear search to go find the appropriate
 * memory_block corresponding to a particular phys_index. If
 * this gets to be a real problem, we can always use a radix
 * tree or something here.
 *
 * This could be made generic for all device subsystems.
 */
struct memory_block *find_memory_block(struct mem_section *section)
{
	return find_memory_block_hinted(section, NULL);
}

static int init_memory_block(struct memory_block **memory,
			     struct mem_section *section, unsigned long state)
{
	struct memory_block *mem;
	unsigned long start_pfn;
	int scn_nr;
	int ret = 0;

=======
 * A reference for the returned memory block device is acquired.
 *
 * Called under device_hotplug_lock.
 */
static struct memory_block *find_memory_block_by_id(unsigned long block_id)
{
	struct memory_block *mem;

	mem = xa_load(&memory_blocks, block_id);
	if (mem)
		get_device(&mem->dev);
	return mem;
}

/*
 * Called under device_hotplug_lock.
 */
struct memory_block *find_memory_block(unsigned long section_nr)
{
	unsigned long block_id = memory_block_id(section_nr);

	return find_memory_block_by_id(block_id);
}

static struct attribute *memory_memblk_attrs[] = {
	&dev_attr_phys_index.attr,
	&dev_attr_state.attr,
	&dev_attr_phys_device.attr,
	&dev_attr_removable.attr,
#ifdef CONFIG_MEMORY_HOTREMOVE
	&dev_attr_valid_zones.attr,
#endif
	NULL
};

static const struct attribute_group memory_memblk_attr_group = {
	.attrs = memory_memblk_attrs,
};

static const struct attribute_group *memory_memblk_attr_groups[] = {
	&memory_memblk_attr_group,
	NULL,
};

static int __add_memory_block(struct memory_block *memory)
{
	int ret;

	memory->dev.bus = &memory_subsys;
	memory->dev.id = memory->start_section_nr / sections_per_block;
	memory->dev.release = memory_block_release;
	memory->dev.groups = memory_memblk_attr_groups;
	memory->dev.offline = memory->state == MEM_OFFLINE;

	ret = device_register(&memory->dev);
	if (ret) {
		put_device(&memory->dev);
		return ret;
	}
	ret = xa_err(xa_store(&memory_blocks, memory->dev.id, memory,
			      GFP_KERNEL));
	if (ret)
		device_unregister(&memory->dev);

	return ret;
}

static struct zone *early_node_zone_for_memory_block(struct memory_block *mem,
						     int nid)
{
	const unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	const unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	struct zone *zone, *matching_zone = NULL;
	pg_data_t *pgdat = NODE_DATA(nid);
	int i;

	/*
	 * This logic only works for early memory, when the applicable zones
	 * already span the memory block. We don't expect overlapping zones on
	 * a single node for early memory. So if we're told that some PFNs
	 * of a node fall into this memory block, we can assume that all node
	 * zones that intersect with the memory block are actually applicable.
	 * No need to look at the memmap.
	 */
	for (i = 0; i < MAX_NR_ZONES; i++) {
		zone = pgdat->node_zones + i;
		if (!populated_zone(zone))
			continue;
		if (!zone_intersects(zone, start_pfn, nr_pages))
			continue;
		if (!matching_zone) {
			matching_zone = zone;
			continue;
		}
		/* Spans multiple zones ... */
		matching_zone = NULL;
		break;
	}
	return matching_zone;
}

#ifdef CONFIG_NUMA
/**
 * memory_block_add_nid() - Indicate that system RAM falling into this memory
 *			    block device (partially) belongs to the given node.
 * @mem: The memory block device.
 * @nid: The node id.
 * @context: The memory initialization context.
 *
 * Indicate that system RAM falling into this memory block (partially) belongs
 * to the given node. If the context indicates ("early") that we are adding the
 * node during node device subsystem initialization, this will also properly
 * set/adjust mem->zone based on the zone ranges of the given node.
 */
void memory_block_add_nid(struct memory_block *mem, int nid,
			  enum meminit_context context)
{
	if (context == MEMINIT_EARLY && mem->nid != nid) {
		/*
		 * For early memory we have to determine the zone when setting
		 * the node id and handle multiple nodes spanning a single
		 * memory block by indicate via zone == NULL that we're not
		 * dealing with a single zone. So if we're setting the node id
		 * the first time, determine if there is a single zone. If we're
		 * setting the node id a second time to a different node,
		 * invalidate the single detected zone.
		 */
		if (mem->nid == NUMA_NO_NODE)
			mem->zone = early_node_zone_for_memory_block(mem, nid);
		else
			mem->zone = NULL;
	}

	/*
	 * If this memory block spans multiple nodes, we only indicate
	 * the last processed node. If we span multiple nodes (not applicable
	 * to hotplugged memory), zone == NULL will prohibit memory offlining
	 * and consequently unplug.
	 */
	mem->nid = nid;
}
#endif

static int add_memory_block(unsigned long block_id, unsigned long state,
			    struct vmem_altmap *altmap,
			    struct memory_group *group)
{
	struct memory_block *mem;
	int ret = 0;

	mem = find_memory_block_by_id(block_id);
	if (mem) {
		put_device(&mem->dev);
		return -EEXIST;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mem = kzalloc(sizeof(*mem), GFP_KERNEL);
	if (!mem)
		return -ENOMEM;

<<<<<<< HEAD
	scn_nr = __section_nr(section);
	mem->start_section_nr =
			base_memory_block_id(scn_nr) * sections_per_block;
	mem->end_section_nr = mem->start_section_nr + sections_per_block - 1;
	mem->state = state;
	mem->section_count++;
	mutex_init(&mem->state_mutex);
	start_pfn = section_nr_to_pfn(mem->start_section_nr);
	mem->phys_device = arch_get_memory_phys_device(start_pfn);

	ret = register_memory(mem);
	if (!ret)
		ret = mem_create_simple_file(mem, phys_index);
	if (!ret)
		ret = mem_create_simple_file(mem, end_phys_index);
	if (!ret)
		ret = mem_create_simple_file(mem, state);
	if (!ret)
		ret = mem_create_simple_file(mem, phys_device);
	if (!ret)
		ret = mem_create_simple_file(mem, removable);

	*memory = mem;
	return ret;
}

static int add_memory_section(int nid, struct mem_section *section,
			struct memory_block **mem_p,
			unsigned long state, enum mem_add_context context)
{
	struct memory_block *mem = NULL;
	int scn_nr = __section_nr(section);
	int ret = 0;

	mutex_lock(&mem_sysfs_mutex);

	if (context == BOOT) {
		/* same memory block ? */
		if (mem_p && *mem_p)
			if (scn_nr >= (*mem_p)->start_section_nr &&
			    scn_nr <= (*mem_p)->end_section_nr) {
				mem = *mem_p;
				kobject_get(&mem->dev.kobj);
			}
	} else
		mem = find_memory_block(section);

	if (mem) {
		mem->section_count++;
		kobject_put(&mem->dev.kobj);
	} else {
		ret = init_memory_block(&mem, section, state);
		/* store memory_block pointer for next loop */
		if (!ret && context == BOOT)
			if (mem_p)
				*mem_p = mem;
	}

	if (!ret) {
		if (context == HOTPLUG &&
		    mem->section_count == sections_per_block)
			ret = register_mem_sect_under_node(mem, nid);
	}

	mutex_unlock(&mem_sysfs_mutex);
	return ret;
}

int remove_memory_block(unsigned long node_id, struct mem_section *section,
		int phys_device)
{
	struct memory_block *mem;

	mutex_lock(&mem_sysfs_mutex);
	mem = find_memory_block(section);
	unregister_mem_sect_under_nodes(mem, __section_nr(section));

	mem->section_count--;
	if (mem->section_count == 0) {
		mem_remove_simple_file(mem, phys_index);
		mem_remove_simple_file(mem, end_phys_index);
		mem_remove_simple_file(mem, state);
		mem_remove_simple_file(mem, phys_device);
		mem_remove_simple_file(mem, removable);
		unregister_memory(mem);
		kfree(mem);
	} else
		kobject_put(&mem->dev.kobj);

	mutex_unlock(&mem_sysfs_mutex);
	return 0;
}

/*
 * need an interface for the VM to add new memory regions,
 * but without onlining it.
 */
int register_new_memory(int nid, struct mem_section *section)
{
	return add_memory_section(nid, section, NULL, MEM_OFFLINE, HOTPLUG);
}

int unregister_memory_section(struct mem_section *section)
{
	if (!present_section(section))
		return -EINVAL;

	return remove_memory_block(0, section, 0);
}

/*
 * Initialize the sysfs support for memory devices...
 */
int __init memory_dev_init(void)
{
	unsigned int i;
	int ret;
	int err;
	unsigned long block_sz;
	struct memory_block *mem = NULL;

	ret = subsys_system_register(&memory_subsys, NULL);
	if (ret)
		goto out;

	block_sz = get_memory_block_size();
	sections_per_block = block_sz / MIN_MEMORY_BLOCK_SIZE;

=======
	mem->start_section_nr = block_id * sections_per_block;
	mem->state = state;
	mem->nid = NUMA_NO_NODE;
	mem->altmap = altmap;
	INIT_LIST_HEAD(&mem->group_next);

#ifndef CONFIG_NUMA
	if (state == MEM_ONLINE)
		/*
		 * MEM_ONLINE at this point implies early memory. With NUMA,
		 * we'll determine the zone when setting the node id via
		 * memory_block_add_nid(). Memory hotplug updated the zone
		 * manually when memory onlining/offlining succeeds.
		 */
		mem->zone = early_node_zone_for_memory_block(mem, NUMA_NO_NODE);
#endif /* CONFIG_NUMA */

	ret = __add_memory_block(mem);
	if (ret)
		return ret;

	if (group) {
		mem->group = group;
		list_add(&mem->group_next, &group->memory_blocks);
	}

	return 0;
}

static int __init add_boot_memory_block(unsigned long base_section_nr)
{
	int section_count = 0;
	unsigned long nr;

	for (nr = base_section_nr; nr < base_section_nr + sections_per_block;
	     nr++)
		if (present_section_nr(nr))
			section_count++;

	if (section_count == 0)
		return 0;
	return add_memory_block(memory_block_id(base_section_nr),
				MEM_ONLINE, NULL,  NULL);
}

static int add_hotplug_memory_block(unsigned long block_id,
				    struct vmem_altmap *altmap,
				    struct memory_group *group)
{
	return add_memory_block(block_id, MEM_OFFLINE, altmap, group);
}

static void remove_memory_block(struct memory_block *memory)
{
	if (WARN_ON_ONCE(memory->dev.bus != &memory_subsys))
		return;

	WARN_ON(xa_erase(&memory_blocks, memory->dev.id) == NULL);

	if (memory->group) {
		list_del(&memory->group_next);
		memory->group = NULL;
	}

	/* drop the ref. we got via find_memory_block() */
	put_device(&memory->dev);
	device_unregister(&memory->dev);
}

/*
 * Create memory block devices for the given memory area. Start and size
 * have to be aligned to memory block granularity. Memory block devices
 * will be initialized as offline.
 *
 * Called under device_hotplug_lock.
 */
int create_memory_block_devices(unsigned long start, unsigned long size,
				struct vmem_altmap *altmap,
				struct memory_group *group)
{
	const unsigned long start_block_id = pfn_to_block_id(PFN_DOWN(start));
	unsigned long end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
	struct memory_block *mem;
	unsigned long block_id;
	int ret = 0;

	if (WARN_ON_ONCE(!IS_ALIGNED(start, memory_block_size_bytes()) ||
			 !IS_ALIGNED(size, memory_block_size_bytes())))
		return -EINVAL;

	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		ret = add_hotplug_memory_block(block_id, altmap, group);
		if (ret)
			break;
	}
	if (ret) {
		end_block_id = block_id;
		for (block_id = start_block_id; block_id != end_block_id;
		     block_id++) {
			mem = find_memory_block_by_id(block_id);
			if (WARN_ON_ONCE(!mem))
				continue;
			remove_memory_block(mem);
		}
	}
	return ret;
}

/*
 * Remove memory block devices for the given memory area. Start and size
 * have to be aligned to memory block granularity. Memory block devices
 * have to be offline.
 *
 * Called under device_hotplug_lock.
 */
void remove_memory_block_devices(unsigned long start, unsigned long size)
{
	const unsigned long start_block_id = pfn_to_block_id(PFN_DOWN(start));
	const unsigned long end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
	struct memory_block *mem;
	unsigned long block_id;

	if (WARN_ON_ONCE(!IS_ALIGNED(start, memory_block_size_bytes()) ||
			 !IS_ALIGNED(size, memory_block_size_bytes())))
		return;

	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		mem = find_memory_block_by_id(block_id);
		if (WARN_ON_ONCE(!mem))
			continue;
		num_poisoned_pages_sub(-1UL, memblk_nr_poison(mem));
		unregister_memory_block_under_nodes(mem);
		remove_memory_block(mem);
	}
}

static struct attribute *memory_root_attrs[] = {
#ifdef CONFIG_ARCH_MEMORY_PROBE
	&dev_attr_probe.attr,
#endif

#ifdef CONFIG_MEMORY_FAILURE
	&dev_attr_soft_offline_page.attr,
	&dev_attr_hard_offline_page.attr,
#endif

	&dev_attr_block_size_bytes.attr,
	&dev_attr_auto_online_blocks.attr,
#ifdef CONFIG_CRASH_HOTPLUG
	&dev_attr_crash_hotplug.attr,
#endif
	NULL
};

static const struct attribute_group memory_root_attr_group = {
	.attrs = memory_root_attrs,
};

static const struct attribute_group *memory_root_attr_groups[] = {
	&memory_root_attr_group,
	NULL,
};

/*
 * Initialize the sysfs support for memory devices. At the time this function
 * is called, we cannot have concurrent creation/deletion of memory block
 * devices, the device_hotplug_lock is not needed.
 */
void __init memory_dev_init(void)
{
	int ret;
	unsigned long block_sz, nr;

	/* Validate the configured memory block size */
	block_sz = memory_block_size_bytes();
	if (!is_power_of_2(block_sz) || block_sz < MIN_MEMORY_BLOCK_SIZE)
		panic("Memory block size not suitable: 0x%lx\n", block_sz);
	sections_per_block = block_sz / MIN_MEMORY_BLOCK_SIZE;

	ret = subsys_system_register(&memory_subsys, memory_root_attr_groups);
	if (ret)
		panic("%s() failed to register subsystem: %d\n", __func__, ret);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Create entries for memory sections that were found
	 * during boot and have been initialized
	 */
<<<<<<< HEAD
	for (i = 0; i < NR_MEM_SECTIONS; i++) {
		if (!present_section_nr(i))
			continue;
		/* don't need to reuse memory_block if only one per block */
		err = add_memory_section(0, __nr_to_section(i),
				 (sections_per_block == 1) ? NULL : &mem,
					 MEM_ONLINE,
					 BOOT);
		if (!ret)
			ret = err;
	}

	err = memory_probe_init();
	if (!ret)
		ret = err;
	err = memory_fail_init();
	if (!ret)
		ret = err;
	err = block_size_init();
	if (!ret)
		ret = err;
out:
	if (ret)
		printk(KERN_ERR "%s() failed: %d\n", __func__, ret);
	return ret;
}
=======
	for (nr = 0; nr <= __highest_present_section_nr;
	     nr += sections_per_block) {
		ret = add_boot_memory_block(nr);
		if (ret)
			panic("%s() failed to add memory block: %d\n", __func__,
			      ret);
	}
}

/**
 * walk_memory_blocks - walk through all present memory blocks overlapped
 *			by the range [start, start + size)
 *
 * @start: start address of the memory range
 * @size: size of the memory range
 * @arg: argument passed to func
 * @func: callback for each memory section walked
 *
 * This function walks through all present memory blocks overlapped by the
 * range [start, start + size), calling func on each memory block.
 *
 * In case func() returns an error, walking is aborted and the error is
 * returned.
 *
 * Called under device_hotplug_lock.
 */
int walk_memory_blocks(unsigned long start, unsigned long size,
		       void *arg, walk_memory_blocks_func_t func)
{
	const unsigned long start_block_id = phys_to_block_id(start);
	const unsigned long end_block_id = phys_to_block_id(start + size - 1);
	struct memory_block *mem;
	unsigned long block_id;
	int ret = 0;

	if (!size)
		return 0;

	for (block_id = start_block_id; block_id <= end_block_id; block_id++) {
		mem = find_memory_block_by_id(block_id);
		if (!mem)
			continue;

		ret = func(mem, arg);
		put_device(&mem->dev);
		if (ret)
			break;
	}
	return ret;
}

struct for_each_memory_block_cb_data {
	walk_memory_blocks_func_t func;
	void *arg;
};

static int for_each_memory_block_cb(struct device *dev, void *data)
{
	struct memory_block *mem = to_memory_block(dev);
	struct for_each_memory_block_cb_data *cb_data = data;

	return cb_data->func(mem, cb_data->arg);
}

/**
 * for_each_memory_block - walk through all present memory blocks
 *
 * @arg: argument passed to func
 * @func: callback for each memory block walked
 *
 * This function walks through all present memory blocks, calling func on
 * each memory block.
 *
 * In case func() returns an error, walking is aborted and the error is
 * returned.
 */
int for_each_memory_block(void *arg, walk_memory_blocks_func_t func)
{
	struct for_each_memory_block_cb_data cb_data = {
		.func = func,
		.arg = arg,
	};

	return bus_for_each_dev(&memory_subsys, NULL, &cb_data,
				for_each_memory_block_cb);
}

/*
 * This is an internal helper to unify allocation and initialization of
 * memory groups. Note that the passed memory group will be copied to a
 * dynamically allocated memory group. After this call, the passed
 * memory group should no longer be used.
 */
static int memory_group_register(struct memory_group group)
{
	struct memory_group *new_group;
	uint32_t mgid;
	int ret;

	if (!node_possible(group.nid))
		return -EINVAL;

	new_group = kzalloc(sizeof(group), GFP_KERNEL);
	if (!new_group)
		return -ENOMEM;
	*new_group = group;
	INIT_LIST_HEAD(&new_group->memory_blocks);

	ret = xa_alloc(&memory_groups, &mgid, new_group, xa_limit_31b,
		       GFP_KERNEL);
	if (ret) {
		kfree(new_group);
		return ret;
	} else if (group.is_dynamic) {
		xa_set_mark(&memory_groups, mgid, MEMORY_GROUP_MARK_DYNAMIC);
	}
	return mgid;
}

/**
 * memory_group_register_static() - Register a static memory group.
 * @nid: The node id.
 * @max_pages: The maximum number of pages we'll have in this static memory
 *	       group.
 *
 * Register a new static memory group and return the memory group id.
 * All memory in the group belongs to a single unit, such as a DIMM. All
 * memory belonging to a static memory group is added in one go to be removed
 * in one go -- it's static.
 *
 * Returns an error if out of memory, if the node id is invalid, if no new
 * memory groups can be registered, or if max_pages is invalid (0). Otherwise,
 * returns the new memory group id.
 */
int memory_group_register_static(int nid, unsigned long max_pages)
{
	struct memory_group group = {
		.nid = nid,
		.s = {
			.max_pages = max_pages,
		},
	};

	if (!max_pages)
		return -EINVAL;
	return memory_group_register(group);
}
EXPORT_SYMBOL_GPL(memory_group_register_static);

/**
 * memory_group_register_dynamic() - Register a dynamic memory group.
 * @nid: The node id.
 * @unit_pages: Unit in pages in which is memory added/removed in this dynamic
 *		memory group.
 *
 * Register a new dynamic memory group and return the memory group id.
 * Memory within a dynamic memory group is added/removed dynamically
 * in unit_pages.
 *
 * Returns an error if out of memory, if the node id is invalid, if no new
 * memory groups can be registered, or if unit_pages is invalid (0, not a
 * power of two, smaller than a single memory block). Otherwise, returns the
 * new memory group id.
 */
int memory_group_register_dynamic(int nid, unsigned long unit_pages)
{
	struct memory_group group = {
		.nid = nid,
		.is_dynamic = true,
		.d = {
			.unit_pages = unit_pages,
		},
	};

	if (!unit_pages || !is_power_of_2(unit_pages) ||
	    unit_pages < PHYS_PFN(memory_block_size_bytes()))
		return -EINVAL;
	return memory_group_register(group);
}
EXPORT_SYMBOL_GPL(memory_group_register_dynamic);

/**
 * memory_group_unregister() - Unregister a memory group.
 * @mgid: the memory group id
 *
 * Unregister a memory group. If any memory block still belongs to this
 * memory group, unregistering will fail.
 *
 * Returns -EINVAL if the memory group id is invalid, returns -EBUSY if some
 * memory blocks still belong to this memory group and returns 0 if
 * unregistering succeeded.
 */
int memory_group_unregister(int mgid)
{
	struct memory_group *group;

	if (mgid < 0)
		return -EINVAL;

	group = xa_load(&memory_groups, mgid);
	if (!group)
		return -EINVAL;
	if (!list_empty(&group->memory_blocks))
		return -EBUSY;
	xa_erase(&memory_groups, mgid);
	kfree(group);
	return 0;
}
EXPORT_SYMBOL_GPL(memory_group_unregister);

/*
 * This is an internal helper only to be used in core memory hotplug code to
 * lookup a memory group. We don't care about locking, as we don't expect a
 * memory group to get unregistered while adding memory to it -- because
 * the group and the memory is managed by the same driver.
 */
struct memory_group *memory_group_find_by_id(int mgid)
{
	return xa_load(&memory_groups, mgid);
}

/*
 * This is an internal helper only to be used in core memory hotplug code to
 * walk all dynamic memory groups excluding a given memory group, either
 * belonging to a specific node, or belonging to any node.
 */
int walk_dynamic_memory_groups(int nid, walk_memory_groups_func_t func,
			       struct memory_group *excluded, void *arg)
{
	struct memory_group *group;
	unsigned long index;
	int ret = 0;

	xa_for_each_marked(&memory_groups, index, group,
			   MEMORY_GROUP_MARK_DYNAMIC) {
		if (group == excluded)
			continue;
#ifdef CONFIG_NUMA
		if (nid != NUMA_NO_NODE && group->nid != nid)
			continue;
#endif /* CONFIG_NUMA */
		ret = func(group, arg);
		if (ret)
			break;
	}
	return ret;
}

#if defined(CONFIG_MEMORY_FAILURE) && defined(CONFIG_MEMORY_HOTPLUG)
void memblk_nr_poison_inc(unsigned long pfn)
{
	const unsigned long block_id = pfn_to_block_id(pfn);
	struct memory_block *mem = find_memory_block_by_id(block_id);

	if (mem)
		atomic_long_inc(&mem->nr_hwpoison);
}

void memblk_nr_poison_sub(unsigned long pfn, long i)
{
	const unsigned long block_id = pfn_to_block_id(pfn);
	struct memory_block *mem = find_memory_block_by_id(block_id);

	if (mem)
		atomic_long_sub(i, &mem->nr_hwpoison);
}

static unsigned long memblk_nr_poison(struct memory_block *mem)
{
	return atomic_long_read(&mem->nr_hwpoison);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
