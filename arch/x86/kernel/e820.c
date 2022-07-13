<<<<<<< HEAD
/*
 * Handle the memory map.
 * The functions here do the job until bootmem takes over.
 *
 *  Getting sanitize_e820_map() in sync with i386 version by applying change:
 *  -  Provisions for empty E820 memory regions (reported by certain BIOSes).
 *     Alex Achenbach <xela@slit.de>, December 2002.
 *  Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/crash_dump.h>
#include <linux/export.h>
#include <linux/bootmem.h>
#include <linux/pfn.h>
#include <linux/suspend.h>
#include <linux/acpi.h>
#include <linux/firmware-map.h>
#include <linux/memblock.h>
#include <linux/sort.h>

#include <asm/e820.h>
#include <asm/proto.h>
#include <asm/setup.h>

/*
 * The e820 map is the map that gets modified e.g. with command line parameters
 * and that is also registered with modifications in the kernel resource tree
 * with the iomem_resource as parent.
 *
 * The e820_saved is directly saved after the BIOS-provided memory map is
 * copied. It doesn't get modified afterwards. It's registered for the
 * /sys/firmware/memmap interface.
 *
 * That memory map is not modified and is used as base for kexec. The kexec'd
 * kernel should get the same memory map as the firmware provides. Then the
 * user can e.g. boot the original kernel with mem=1G while still booting the
 * next kernel with full memory.
 */
struct e820map e820;
struct e820map e820_saved;
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Low level x86 E820 memory map handling functions.
 *
 * The firmware and bootloader passes us the "E820 table", which is the primary
 * physical memory layout description available about x86 systems.
 *
 * The kernel takes the E820 memory layout and optionally modifies it with
 * quirks and other tweaks, and feeds that into the generic Linux memory
 * allocation code routines via a platform independent interface (memblock, etc.).
 */
#include <linux/crash_dump.h>
#include <linux/memblock.h>
#include <linux/suspend.h>
#include <linux/acpi.h>
#include <linux/firmware-map.h>
#include <linux/sort.h>
#include <linux/memory_hotplug.h>

#include <asm/e820/api.h>
#include <asm/setup.h>

/*
 * We organize the E820 table into three main data structures:
 *
 * - 'e820_table_firmware': the original firmware version passed to us by the
 *   bootloader - not modified by the kernel. It is composed of two parts:
 *   the first 128 E820 memory entries in boot_params.e820_table and the remaining
 *   (if any) entries of the SETUP_E820_EXT nodes. We use this to:
 *
 *       - inform the user about the firmware's notion of memory layout
 *         via /sys/firmware/memmap
 *
 *       - the hibernation code uses it to generate a kernel-independent CRC32
 *         checksum of the physical memory layout of a system.
 *
 * - 'e820_table_kexec': a slightly modified (by the kernel) firmware version
 *   passed to us by the bootloader - the major difference between
 *   e820_table_firmware[] and this one is that, the latter marks the setup_data
 *   list created by the EFI boot stub as reserved, so that kexec can reuse the
 *   setup_data information in the second kernel. Besides, e820_table_kexec[]
 *   might also be modified by the kexec itself to fake a mptable.
 *   We use this to:
 *
 *       - kexec, which is a bootloader in disguise, uses the original E820
 *         layout to pass to the kexec-ed kernel. This way the original kernel
 *         can have a restricted E820 map while the kexec()-ed kexec-kernel
 *         can have access to full memory - etc.
 *
 * - 'e820_table': this is the main E820 table that is massaged by the
 *   low level x86 platform code, or modified by boot parameters, before
 *   passed on to higher level MM layers.
 *
 * Once the E820 map has been converted to the standard Linux memory layout
 * information its role stops - modifying it has no effect and does not get
 * re-propagated. So its main role is a temporary bootstrap storage of firmware
 * specific memory layout data during early bootup.
 */
static struct e820_table e820_table_init		__initdata;
static struct e820_table e820_table_kexec_init		__initdata;
static struct e820_table e820_table_firmware_init	__initdata;

struct e820_table *e820_table __refdata			= &e820_table_init;
struct e820_table *e820_table_kexec __refdata		= &e820_table_kexec_init;
struct e820_table *e820_table_firmware __refdata	= &e820_table_firmware_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* For PCI or other memory-mapped resources */
unsigned long pci_mem_start = 0xaeedbabe;
#ifdef CONFIG_PCI
EXPORT_SYMBOL(pci_mem_start);
#endif

/*
 * This function checks if any part of the range <start,end> is mapped
 * with type.
 */
<<<<<<< HEAD
int
e820_any_mapped(u64 start, u64 end, unsigned type)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];

		if (type && ei->type != type)
			continue;
		if (ei->addr >= end || ei->addr + ei->size <= start)
			continue;
		return 1;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(e820_any_mapped);

/*
 * This function checks if the entire range <start,end> is mapped with type.
 *
 * Note: this function only works correct if the e820 table is sorted and
 * not-overlapping, which is the case
 */
int __init e820_all_mapped(u64 start, u64 end, unsigned type)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];

		if (type && ei->type != type)
			continue;
		/* is the region (part) in overlap with the current region ?*/
		if (ei->addr >= end || ei->addr + ei->size <= start)
			continue;

		/* if the region is at the beginning of <start,end> we move
		 * start to the end of the region since it's ok until there
		 */
		if (ei->addr <= start)
			start = ei->addr + ei->size;
		/*
		 * if start is now at or beyond end, we're done, full
		 * coverage
		 */
		if (start >= end)
			return 1;
	}
	return 0;
}

/*
 * Add a memory region to the kernel e820 map.
 */
static void __init __e820_add_region(struct e820map *e820x, u64 start, u64 size,
					 int type)
{
	int x = e820x->nr_map;

	if (x >= ARRAY_SIZE(e820x->map)) {
		printk(KERN_ERR "Ooops! Too many entries in the memory map!\n");
		return;
	}

	e820x->map[x].addr = start;
	e820x->map[x].size = size;
	e820x->map[x].type = type;
	e820x->nr_map++;
}

void __init e820_add_region(u64 start, u64 size, int type)
{
	__e820_add_region(&e820, start, size, type);
}

static void __init e820_print_type(u32 type)
{
	switch (type) {
	case E820_RAM:
	case E820_RESERVED_KERN:
		printk(KERN_CONT "(usable)");
		break;
	case E820_RESERVED:
		printk(KERN_CONT "(reserved)");
		break;
	case E820_ACPI:
		printk(KERN_CONT "(ACPI data)");
		break;
	case E820_NVS:
		printk(KERN_CONT "(ACPI NVS)");
		break;
	case E820_UNUSABLE:
		printk(KERN_CONT "(unusable)");
		break;
	default:
		printk(KERN_CONT "type %u", type);
		break;
	}
}

void __init e820_print_map(char *who)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		printk(KERN_INFO " %s: %016Lx - %016Lx ", who,
		       (unsigned long long) e820.map[i].addr,
		       (unsigned long long)
		       (e820.map[i].addr + e820.map[i].size));
		e820_print_type(e820.map[i].type);
		printk(KERN_CONT "\n");
=======
static bool _e820__mapped_any(struct e820_table *table,
			      u64 start, u64 end, enum e820_type type)
{
	int i;

	for (i = 0; i < table->nr_entries; i++) {
		struct e820_entry *entry = &table->entries[i];

		if (type && entry->type != type)
			continue;
		if (entry->addr >= end || entry->addr + entry->size <= start)
			continue;
		return true;
	}
	return false;
}

bool e820__mapped_raw_any(u64 start, u64 end, enum e820_type type)
{
	return _e820__mapped_any(e820_table_firmware, start, end, type);
}
EXPORT_SYMBOL_GPL(e820__mapped_raw_any);

bool e820__mapped_any(u64 start, u64 end, enum e820_type type)
{
	return _e820__mapped_any(e820_table, start, end, type);
}
EXPORT_SYMBOL_GPL(e820__mapped_any);

/*
 * This function checks if the entire <start,end> range is mapped with 'type'.
 *
 * Note: this function only works correctly once the E820 table is sorted and
 * not-overlapping (at least for the range specified), which is the case normally.
 */
static struct e820_entry *__e820__mapped_all(u64 start, u64 end,
					     enum e820_type type)
{
	int i;

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];

		if (type && entry->type != type)
			continue;

		/* Is the region (part) in overlap with the current region? */
		if (entry->addr >= end || entry->addr + entry->size <= start)
			continue;

		/*
		 * If the region is at the beginning of <start,end> we move
		 * 'start' to the end of the region since it's ok until there
		 */
		if (entry->addr <= start)
			start = entry->addr + entry->size;

		/*
		 * If 'start' is now at or beyond 'end', we're done, full
		 * coverage of the desired range exists:
		 */
		if (start >= end)
			return entry;
	}

	return NULL;
}

/*
 * This function checks if the entire range <start,end> is mapped with type.
 */
bool __init e820__mapped_all(u64 start, u64 end, enum e820_type type)
{
	return __e820__mapped_all(start, end, type);
}

/*
 * This function returns the type associated with the range <start,end>.
 */
int e820__get_entry_type(u64 start, u64 end)
{
	struct e820_entry *entry = __e820__mapped_all(start, end, 0);

	return entry ? entry->type : -EINVAL;
}

/*
 * Add a memory region to the kernel E820 map.
 */
static void __init __e820__range_add(struct e820_table *table, u64 start, u64 size, enum e820_type type)
{
	int x = table->nr_entries;

	if (x >= ARRAY_SIZE(table->entries)) {
		pr_err("too many entries; ignoring [mem %#010llx-%#010llx]\n",
		       start, start + size - 1);
		return;
	}

	table->entries[x].addr = start;
	table->entries[x].size = size;
	table->entries[x].type = type;
	table->nr_entries++;
}

void __init e820__range_add(u64 start, u64 size, enum e820_type type)
{
	__e820__range_add(e820_table, start, size, type);
}

static void __init e820_print_type(enum e820_type type)
{
	switch (type) {
	case E820_TYPE_RAM:		/* Fall through: */
	case E820_TYPE_RESERVED_KERN:	pr_cont("usable");			break;
	case E820_TYPE_RESERVED:	pr_cont("reserved");			break;
	case E820_TYPE_SOFT_RESERVED:	pr_cont("soft reserved");		break;
	case E820_TYPE_ACPI:		pr_cont("ACPI data");			break;
	case E820_TYPE_NVS:		pr_cont("ACPI NVS");			break;
	case E820_TYPE_UNUSABLE:	pr_cont("unusable");			break;
	case E820_TYPE_PMEM:		/* Fall through: */
	case E820_TYPE_PRAM:		pr_cont("persistent (type %u)", type);	break;
	default:			pr_cont("type %u", type);		break;
	}
}

void __init e820__print_table(char *who)
{
	int i;

	for (i = 0; i < e820_table->nr_entries; i++) {
		pr_info("%s: [mem %#018Lx-%#018Lx] ",
			who,
			e820_table->entries[i].addr,
			e820_table->entries[i].addr + e820_table->entries[i].size - 1);

		e820_print_type(e820_table->entries[i].type);
		pr_cont("\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
 * Sanitize the BIOS e820 map.
 *
 * Some e820 responses include overlapping entries. The following
 * replaces the original e820 map with a new one, removing overlaps,
 * and resolving conflicting memory types in favor of highest
 * numbered type.
 *
 * The input parameter biosmap points to an array of 'struct
 * e820entry' which on entry has elements in the range [0, *pnr_map)
 * valid, and which has space for up to max_nr_map entries.
 * On return, the resulting sanitized e820 map entries will be in
 * overwritten in the same location, starting at biosmap.
 *
 * The integer pointed to by pnr_map must be valid on entry (the
 * current number of valid entries located at biosmap) and will
 * be updated on return, with the new number of valid entries
 * (something no more than max_nr_map.)
 *
 * The return value from sanitize_e820_map() is zero if it
=======
 * Sanitize an E820 map.
 *
 * Some E820 layouts include overlapping entries. The following
 * replaces the original E820 map with a new one, removing overlaps,
 * and resolving conflicting memory types in favor of highest
 * numbered type.
 *
 * The input parameter 'entries' points to an array of 'struct
 * e820_entry' which on entry has elements in the range [0, *nr_entries)
 * valid, and which has space for up to max_nr_entries entries.
 * On return, the resulting sanitized E820 map entries will be in
 * overwritten in the same location, starting at 'entries'.
 *
 * The integer pointed to by nr_entries must be valid on entry (the
 * current number of valid entries located at 'entries'). If the
 * sanitizing succeeds the *nr_entries will be updated with the new
 * number of valid entries (something no more than max_nr_entries).
 *
 * The return value from e820__update_table() is zero if it
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * successfully 'sanitized' the map entries passed in, and is -1
 * if it did nothing, which can happen if either of (1) it was
 * only passed one map entry, or (2) any of the input map entries
 * were invalid (start + size < start, meaning that the size was
 * so big the described memory range wrapped around through zero.)
 *
 *	Visually we're performing the following
 *	(1,2,3,4 = memory types)...
 *
 *	Sample memory map (w/overlaps):
 *	   ____22__________________
 *	   ______________________4_
 *	   ____1111________________
 *	   _44_____________________
 *	   11111111________________
 *	   ____________________33__
 *	   ___________44___________
 *	   __________33333_________
 *	   ______________22________
 *	   ___________________2222_
 *	   _________111111111______
 *	   _____________________11_
 *	   _________________4______
 *
 *	Sanitized equivalent (no overlap):
 *	   1_______________________
 *	   _44_____________________
 *	   ___1____________________
 *	   ____22__________________
 *	   ______11________________
 *	   _________1______________
 *	   __________3_____________
 *	   ___________44___________
 *	   _____________33_________
 *	   _______________2________
 *	   ________________1_______
 *	   _________________4______
 *	   ___________________2____
 *	   ____________________33__
 *	   ______________________4_
 */
struct change_member {
<<<<<<< HEAD
	struct e820entry *pbios; /* pointer to original bios entry */
	unsigned long long addr; /* address for this change point */
};

=======
	/* Pointer to the original entry: */
	struct e820_entry	*entry;
	/* Address for this change point: */
	unsigned long long	addr;
};

static struct change_member	change_point_list[2*E820_MAX_ENTRIES]	__initdata;
static struct change_member	*change_point[2*E820_MAX_ENTRIES]	__initdata;
static struct e820_entry	*overlap_list[E820_MAX_ENTRIES]		__initdata;
static struct e820_entry	new_entries[E820_MAX_ENTRIES]		__initdata;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init cpcompare(const void *a, const void *b)
{
	struct change_member * const *app = a, * const *bpp = b;
	const struct change_member *ap = *app, *bp = *bpp;

	/*
	 * Inputs are pointers to two elements of change_point[].  If their
<<<<<<< HEAD
	 * addresses are unequal, their difference dominates.  If the addresses
=======
	 * addresses are not equal, their difference dominates.  If the addresses
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * are equal, then consider one that represents the end of its region
	 * to be greater than one that does not.
	 */
	if (ap->addr != bp->addr)
		return ap->addr > bp->addr ? 1 : -1;

<<<<<<< HEAD
	return (ap->addr != ap->pbios->addr) - (bp->addr != bp->pbios->addr);
}

int __init sanitize_e820_map(struct e820entry *biosmap, int max_nr_map,
			     u32 *pnr_map)
{
	static struct change_member change_point_list[2*E820_X_MAX] __initdata;
	static struct change_member *change_point[2*E820_X_MAX] __initdata;
	static struct e820entry *overlap_list[E820_X_MAX] __initdata;
	static struct e820entry new_bios[E820_X_MAX] __initdata;
	unsigned long current_type, last_type;
	unsigned long long last_addr;
	int chgidx;
	int overlap_entries;
	int new_bios_entry;
	int old_nr, new_nr, chg_nr;
	int i;

	/* if there's only one memory region, don't bother */
	if (*pnr_map < 2)
		return -1;

	old_nr = *pnr_map;
	BUG_ON(old_nr > max_nr_map);

	/* bail out if we find any unreasonable addresses in bios map */
	for (i = 0; i < old_nr; i++)
		if (biosmap[i].addr + biosmap[i].size < biosmap[i].addr)
			return -1;

	/* create pointers for initial change-point information (for sorting) */
	for (i = 0; i < 2 * old_nr; i++)
		change_point[i] = &change_point_list[i];

	/* record all known change-points (starting and ending addresses),
	   omitting those that are for empty memory regions */
	chgidx = 0;
	for (i = 0; i < old_nr; i++)	{
		if (biosmap[i].size != 0) {
			change_point[chgidx]->addr = biosmap[i].addr;
			change_point[chgidx++]->pbios = &biosmap[i];
			change_point[chgidx]->addr = biosmap[i].addr +
				biosmap[i].size;
			change_point[chgidx++]->pbios = &biosmap[i];
		}
	}
	chg_nr = chgidx;

	/* sort change-point list by memory addresses (low -> high) */
	sort(change_point, chg_nr, sizeof *change_point, cpcompare, NULL);

	/* create a new bios memory map, removing overlaps */
	overlap_entries = 0;	 /* number of entries in the overlap table */
	new_bios_entry = 0;	 /* index for creating new bios map entries */
	last_type = 0;		 /* start with undefined memory type */
	last_addr = 0;		 /* start with 0 as last starting address */

	/* loop through change-points, determining affect on the new bios map */
	for (chgidx = 0; chgidx < chg_nr; chgidx++) {
		/* keep track of all overlapping bios entries */
		if (change_point[chgidx]->addr ==
		    change_point[chgidx]->pbios->addr) {
			/*
			 * add map entry to overlap list (> 1 entry
			 * implies an overlap)
			 */
			overlap_list[overlap_entries++] =
				change_point[chgidx]->pbios;
		} else {
			/*
			 * remove entry from list (order independent,
			 * so swap with last)
			 */
			for (i = 0; i < overlap_entries; i++) {
				if (overlap_list[i] ==
				    change_point[chgidx]->pbios)
					overlap_list[i] =
						overlap_list[overlap_entries-1];
=======
	return (ap->addr != ap->entry->addr) - (bp->addr != bp->entry->addr);
}

static bool e820_nomerge(enum e820_type type)
{
	/*
	 * These types may indicate distinct platform ranges aligned to
	 * numa node, protection domain, performance domain, or other
	 * boundaries. Do not merge them.
	 */
	if (type == E820_TYPE_PRAM)
		return true;
	if (type == E820_TYPE_SOFT_RESERVED)
		return true;
	return false;
}

int __init e820__update_table(struct e820_table *table)
{
	struct e820_entry *entries = table->entries;
	u32 max_nr_entries = ARRAY_SIZE(table->entries);
	enum e820_type current_type, last_type;
	unsigned long long last_addr;
	u32 new_nr_entries, overlap_entries;
	u32 i, chg_idx, chg_nr;

	/* If there's only one memory region, don't bother: */
	if (table->nr_entries < 2)
		return -1;

	BUG_ON(table->nr_entries > max_nr_entries);

	/* Bail out if we find any unreasonable addresses in the map: */
	for (i = 0; i < table->nr_entries; i++) {
		if (entries[i].addr + entries[i].size < entries[i].addr)
			return -1;
	}

	/* Create pointers for initial change-point information (for sorting): */
	for (i = 0; i < 2 * table->nr_entries; i++)
		change_point[i] = &change_point_list[i];

	/*
	 * Record all known change-points (starting and ending addresses),
	 * omitting empty memory regions:
	 */
	chg_idx = 0;
	for (i = 0; i < table->nr_entries; i++)	{
		if (entries[i].size != 0) {
			change_point[chg_idx]->addr	= entries[i].addr;
			change_point[chg_idx++]->entry	= &entries[i];
			change_point[chg_idx]->addr	= entries[i].addr + entries[i].size;
			change_point[chg_idx++]->entry	= &entries[i];
		}
	}
	chg_nr = chg_idx;

	/* Sort change-point list by memory addresses (low -> high): */
	sort(change_point, chg_nr, sizeof(*change_point), cpcompare, NULL);

	/* Create a new memory map, removing overlaps: */
	overlap_entries = 0;	 /* Number of entries in the overlap table */
	new_nr_entries = 0;	 /* Index for creating new map entries */
	last_type = 0;		 /* Start with undefined memory type */
	last_addr = 0;		 /* Start with 0 as last starting address */

	/* Loop through change-points, determining effect on the new map: */
	for (chg_idx = 0; chg_idx < chg_nr; chg_idx++) {
		/* Keep track of all overlapping entries */
		if (change_point[chg_idx]->addr == change_point[chg_idx]->entry->addr) {
			/* Add map entry to overlap list (> 1 entry implies an overlap) */
			overlap_list[overlap_entries++] = change_point[chg_idx]->entry;
		} else {
			/* Remove entry from list (order independent, so swap with last): */
			for (i = 0; i < overlap_entries; i++) {
				if (overlap_list[i] == change_point[chg_idx]->entry)
					overlap_list[i] = overlap_list[overlap_entries-1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			overlap_entries--;
		}
		/*
<<<<<<< HEAD
		 * if there are overlapping entries, decide which
=======
		 * If there are overlapping entries, decide which
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * "type" to use (larger value takes precedence --
		 * 1=usable, 2,3,4,4+=unusable)
		 */
		current_type = 0;
<<<<<<< HEAD
		for (i = 0; i < overlap_entries; i++)
			if (overlap_list[i]->type > current_type)
				current_type = overlap_list[i]->type;
		/*
		 * continue building up new bios map based on this
		 * information
		 */
		if (current_type != last_type)	{
			if (last_type != 0)	 {
				new_bios[new_bios_entry].size =
					change_point[chgidx]->addr - last_addr;
				/*
				 * move forward only if the new size
				 * was non-zero
				 */
				if (new_bios[new_bios_entry].size != 0)
					/*
					 * no more space left for new
					 * bios entries ?
					 */
					if (++new_bios_entry >= max_nr_map)
						break;
			}
			if (current_type != 0)	{
				new_bios[new_bios_entry].addr =
					change_point[chgidx]->addr;
				new_bios[new_bios_entry].type = current_type;
				last_addr = change_point[chgidx]->addr;
=======
		for (i = 0; i < overlap_entries; i++) {
			if (overlap_list[i]->type > current_type)
				current_type = overlap_list[i]->type;
		}

		/* Continue building up new map based on this information: */
		if (current_type != last_type || e820_nomerge(current_type)) {
			if (last_type) {
				new_entries[new_nr_entries].size = change_point[chg_idx]->addr - last_addr;
				/* Move forward only if the new size was non-zero: */
				if (new_entries[new_nr_entries].size != 0)
					/* No more space left for new entries? */
					if (++new_nr_entries >= max_nr_entries)
						break;
			}
			if (current_type) {
				new_entries[new_nr_entries].addr = change_point[chg_idx]->addr;
				new_entries[new_nr_entries].type = current_type;
				last_addr = change_point[chg_idx]->addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			last_type = current_type;
		}
	}
<<<<<<< HEAD
	/* retain count for new bios entries */
	new_nr = new_bios_entry;

	/* copy new bios mapping into original location */
	memcpy(biosmap, new_bios, new_nr * sizeof(struct e820entry));
	*pnr_map = new_nr;
=======

	/* Copy the new entries into the original location: */
	memcpy(entries, new_entries, new_nr_entries*sizeof(*entries));
	table->nr_entries = new_nr_entries;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int __init __append_e820_map(struct e820entry *biosmap, int nr_map)
{
	while (nr_map) {
		u64 start = biosmap->addr;
		u64 size = biosmap->size;
		u64 end = start + size;
		u32 type = biosmap->type;

		/* Overflow in 64 bits? Ignore the memory map. */
		if (start > end)
			return -1;

		e820_add_region(start, size, type);

		biosmap++;
		nr_map--;
=======
static int __init __append_e820_table(struct boot_e820_entry *entries, u32 nr_entries)
{
	struct boot_e820_entry *entry = entries;

	while (nr_entries) {
		u64 start = entry->addr;
		u64 size = entry->size;
		u64 end = start + size - 1;
		u32 type = entry->type;

		/* Ignore the entry on 64-bit overflow: */
		if (start > end && likely(size))
			return -1;

		e820__range_add(start, size, type);

		entry++;
		nr_entries--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

/*
<<<<<<< HEAD
 * Copy the BIOS e820 map into a safe place.
=======
 * Copy the BIOS E820 map into a safe place.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Sanity-check it while we're at it..
 *
 * If we're lucky and live on a modern system, the setup code
 * will have given us a memory map that we can use to properly
 * set up memory.  If we aren't, we'll fake a memory map.
 */
<<<<<<< HEAD
static int __init append_e820_map(struct e820entry *biosmap, int nr_map)
{
	/* Only one memory region (or negative)? Ignore it */
	if (nr_map < 2)
		return -1;

	return __append_e820_map(biosmap, nr_map);
}

static u64 __init __e820_update_range(struct e820map *e820x, u64 start,
					u64 size, unsigned old_type,
					unsigned new_type)
=======
static int __init append_e820_table(struct boot_e820_entry *entries, u32 nr_entries)
{
	/* Only one memory region (or negative)? Ignore it */
	if (nr_entries < 2)
		return -1;

	return __append_e820_table(entries, nr_entries);
}

static u64 __init
__e820__range_update(struct e820_table *table, u64 start, u64 size, enum e820_type old_type, enum e820_type new_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u64 end;
	unsigned int i;
	u64 real_updated_size = 0;

	BUG_ON(old_type == new_type);

	if (size > (ULLONG_MAX - start))
		size = ULLONG_MAX - start;

	end = start + size;
<<<<<<< HEAD
	printk(KERN_DEBUG "e820 update range: %016Lx - %016Lx ",
		       (unsigned long long) start,
		       (unsigned long long) end);
	e820_print_type(old_type);
	printk(KERN_CONT " ==> ");
	e820_print_type(new_type);
	printk(KERN_CONT "\n");

	for (i = 0; i < e820x->nr_map; i++) {
		struct e820entry *ei = &e820x->map[i];
		u64 final_start, final_end;
		u64 ei_end;

		if (ei->type != old_type)
			continue;

		ei_end = ei->addr + ei->size;
		/* totally covered by new range? */
		if (ei->addr >= start && ei_end <= end) {
			ei->type = new_type;
			real_updated_size += ei->size;
			continue;
		}

		/* new range is totally covered? */
		if (ei->addr < start && ei_end > end) {
			__e820_add_region(e820x, start, size, new_type);
			__e820_add_region(e820x, end, ei_end - end, ei->type);
			ei->size = start - ei->addr;
=======
	printk(KERN_DEBUG "e820: update [mem %#010Lx-%#010Lx] ", start, end - 1);
	e820_print_type(old_type);
	pr_cont(" ==> ");
	e820_print_type(new_type);
	pr_cont("\n");

	for (i = 0; i < table->nr_entries; i++) {
		struct e820_entry *entry = &table->entries[i];
		u64 final_start, final_end;
		u64 entry_end;

		if (entry->type != old_type)
			continue;

		entry_end = entry->addr + entry->size;

		/* Completely covered by new range? */
		if (entry->addr >= start && entry_end <= end) {
			entry->type = new_type;
			real_updated_size += entry->size;
			continue;
		}

		/* New range is completely covered? */
		if (entry->addr < start && entry_end > end) {
			__e820__range_add(table, start, size, new_type);
			__e820__range_add(table, end, entry_end - end, entry->type);
			entry->size = start - entry->addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			real_updated_size += size;
			continue;
		}

<<<<<<< HEAD
		/* partially covered */
		final_start = max(start, ei->addr);
		final_end = min(end, ei_end);
		if (final_start >= final_end)
			continue;

		__e820_add_region(e820x, final_start, final_end - final_start,
				  new_type);
=======
		/* Partially covered: */
		final_start = max(start, entry->addr);
		final_end = min(end, entry_end);
		if (final_start >= final_end)
			continue;

		__e820__range_add(table, final_start, final_end - final_start, new_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		real_updated_size += final_end - final_start;

		/*
<<<<<<< HEAD
		 * left range could be head or tail, so need to update
		 * size at first.
		 */
		ei->size -= final_end - final_start;
		if (ei->addr < final_start)
			continue;
		ei->addr = final_end;
=======
		 * Left range could be head or tail, so need to update
		 * its size first:
		 */
		entry->size -= final_end - final_start;
		if (entry->addr < final_start)
			continue;

		entry->addr = final_end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return real_updated_size;
}

<<<<<<< HEAD
u64 __init e820_update_range(u64 start, u64 size, unsigned old_type,
			     unsigned new_type)
{
	return __e820_update_range(&e820, start, size, old_type, new_type);
}

static u64 __init e820_update_range_saved(u64 start, u64 size,
					  unsigned old_type, unsigned new_type)
{
	return __e820_update_range(&e820_saved, start, size, old_type,
				     new_type);
}

/* make e820 not cover the range */
u64 __init e820_remove_range(u64 start, u64 size, unsigned old_type,
			     int checktype)
=======
u64 __init e820__range_update(u64 start, u64 size, enum e820_type old_type, enum e820_type new_type)
{
	return __e820__range_update(e820_table, start, size, old_type, new_type);
}

u64 __init e820__range_update_table(struct e820_table *t, u64 start, u64 size,
				    enum e820_type old_type, enum e820_type new_type)
{
	return __e820__range_update(t, start, size, old_type, new_type);
}

/* Remove a range of memory from the E820 table: */
u64 __init e820__range_remove(u64 start, u64 size, enum e820_type old_type, bool check_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	u64 end;
	u64 real_removed_size = 0;

	if (size > (ULLONG_MAX - start))
		size = ULLONG_MAX - start;

	end = start + size;
<<<<<<< HEAD
	printk(KERN_DEBUG "e820 remove range: %016Lx - %016Lx ",
		       (unsigned long long) start,
		       (unsigned long long) end);
	if (checktype)
		e820_print_type(old_type);
	printk(KERN_CONT "\n");

	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];
		u64 final_start, final_end;
		u64 ei_end;

		if (checktype && ei->type != old_type)
			continue;

		ei_end = ei->addr + ei->size;
		/* totally covered? */
		if (ei->addr >= start && ei_end <= end) {
			real_removed_size += ei->size;
			memset(ei, 0, sizeof(struct e820entry));
			continue;
		}

		/* new range is totally covered? */
		if (ei->addr < start && ei_end > end) {
			e820_add_region(end, ei_end - end, ei->type);
			ei->size = start - ei->addr;
=======
	printk(KERN_DEBUG "e820: remove [mem %#010Lx-%#010Lx] ", start, end - 1);
	if (check_type)
		e820_print_type(old_type);
	pr_cont("\n");

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];
		u64 final_start, final_end;
		u64 entry_end;

		if (check_type && entry->type != old_type)
			continue;

		entry_end = entry->addr + entry->size;

		/* Completely covered? */
		if (entry->addr >= start && entry_end <= end) {
			real_removed_size += entry->size;
			memset(entry, 0, sizeof(*entry));
			continue;
		}

		/* Is the new range completely covered? */
		if (entry->addr < start && entry_end > end) {
			e820__range_add(end, entry_end - end, entry->type);
			entry->size = start - entry->addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			real_removed_size += size;
			continue;
		}

<<<<<<< HEAD
		/* partially covered */
		final_start = max(start, ei->addr);
		final_end = min(end, ei_end);
		if (final_start >= final_end)
			continue;
		real_removed_size += final_end - final_start;

		/*
		 * left range could be head or tail, so need to update
		 * size at first.
		 */
		ei->size -= final_end - final_start;
		if (ei->addr < final_start)
			continue;
		ei->addr = final_end;
=======
		/* Partially covered: */
		final_start = max(start, entry->addr);
		final_end = min(end, entry_end);
		if (final_start >= final_end)
			continue;

		real_removed_size += final_end - final_start;

		/*
		 * Left range could be head or tail, so need to update
		 * the size first:
		 */
		entry->size -= final_end - final_start;
		if (entry->addr < final_start)
			continue;

		entry->addr = final_end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return real_removed_size;
}

<<<<<<< HEAD
void __init update_e820(void)
{
	u32 nr_map;

	nr_map = e820.nr_map;
	if (sanitize_e820_map(e820.map, ARRAY_SIZE(e820.map), &nr_map))
		return;
	e820.nr_map = nr_map;
	printk(KERN_INFO "modified physical RAM map:\n");
	e820_print_map("modified");
}
static void __init update_e820_saved(void)
{
	u32 nr_map;

	nr_map = e820_saved.nr_map;
	if (sanitize_e820_map(e820_saved.map, ARRAY_SIZE(e820_saved.map), &nr_map))
		return;
	e820_saved.nr_map = nr_map;
}
#define MAX_GAP_END 0x100000000ull
/*
 * Search for a gap in the e820 memory space from start_addr to end_addr.
 */
__init int e820_search_gap(unsigned long *gapstart, unsigned long *gapsize,
		unsigned long start_addr, unsigned long long end_addr)
{
	unsigned long long last;
	int i = e820.nr_map;
	int found = 0;

	last = (end_addr && end_addr < MAX_GAP_END) ? end_addr : MAX_GAP_END;

	while (--i >= 0) {
		unsigned long long start = e820.map[i].addr;
		unsigned long long end = start + e820.map[i].size;

		if (end < start_addr)
			continue;

		/*
		 * Since "last" is at most 4GB, we know we'll
		 * fit in 32 bits if this condition is true
=======
void __init e820__update_table_print(void)
{
	if (e820__update_table(e820_table))
		return;

	pr_info("modified physical RAM map:\n");
	e820__print_table("modified");
}

static void __init e820__update_table_kexec(void)
{
	e820__update_table(e820_table_kexec);
}

#define MAX_GAP_END 0x100000000ull

/*
 * Search for a gap in the E820 memory space from 0 to MAX_GAP_END (4GB).
 */
static int __init e820_search_gap(unsigned long *gapstart, unsigned long *gapsize)
{
	unsigned long long last = MAX_GAP_END;
	int i = e820_table->nr_entries;
	int found = 0;

	while (--i >= 0) {
		unsigned long long start = e820_table->entries[i].addr;
		unsigned long long end = start + e820_table->entries[i].size;

		/*
		 * Since "last" is at most 4GB, we know we'll
		 * fit in 32 bits if this condition is true:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		if (last > end) {
			unsigned long gap = last - end;

			if (gap >= *gapsize) {
				*gapsize = gap;
				*gapstart = end;
				found = 1;
			}
		}
		if (start < last)
			last = start;
	}
	return found;
}

/*
<<<<<<< HEAD
 * Search for the biggest gap in the low 32 bits of the e820
 * memory space.  We pass this space to PCI to assign MMIO resources
 * for hotplug or unconfigured devices in.
 * Hopefully the BIOS let enough space left.
 */
__init void e820_setup_gap(void)
=======
 * Search for the biggest gap in the low 32 bits of the E820
 * memory space. We pass this space to the PCI subsystem, so
 * that it can assign MMIO resources for hotplug or
 * unconfigured devices in.
 *
 * Hopefully the BIOS let enough space left.
 */
__init void e820__setup_pci_gap(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long gapstart, gapsize;
	int found;

<<<<<<< HEAD
	gapstart = 0x10000000;
	gapsize = 0x400000;
	found  = e820_search_gap(&gapstart, &gapsize, 0, MAX_GAP_END);

#ifdef CONFIG_X86_64
	if (!found) {
		gapstart = (max_pfn << PAGE_SHIFT) + 1024*1024;
		printk(KERN_ERR
	"PCI: Warning: Cannot find a gap in the 32bit address range\n"
	"PCI: Unassigned devices with 32bit resource registers may break!\n");
	}
#endif

	/*
	 * e820_reserve_resources_late protect stolen RAM already
	 */
	pci_mem_start = gapstart;

	printk(KERN_INFO
	       "Allocating PCI resources starting at %lx (gap: %lx:%lx)\n",
	       pci_mem_start, gapstart, gapsize);
}

/**
 * Because of the size limitation of struct boot_params, only first
 * 128 E820 memory entries are passed to kernel via
 * boot_params.e820_map, others are passed via SETUP_E820_EXT node of
 * linked list of struct setup_data, which is parsed here.
 */
void __init parse_e820_ext(struct setup_data *sdata)
{
	int entries;
	struct e820entry *extmap;

	entries = sdata->len / sizeof(struct e820entry);
	extmap = (struct e820entry *)(sdata->data);
	__append_e820_map(extmap, entries);
	sanitize_e820_map(e820.map, ARRAY_SIZE(e820.map), &e820.nr_map);
	printk(KERN_INFO "extended physical RAM map:\n");
	e820_print_map("extended");
}

#if defined(CONFIG_X86_64) || \
	(defined(CONFIG_X86_32) && defined(CONFIG_HIBERNATION))
/**
 * Find the ranges of physical addresses that do not correspond to
 * e820 RAM areas and mark the corresponding pages as nosave for
 * hibernation (32 bit) or software suspend and suspend to RAM (64 bit).
 *
 * This function requires the e820 map to be sorted and without any
 * overlapping entries and assumes the first e820 area to be RAM.
 */
void __init e820_mark_nosave_regions(unsigned long limit_pfn)
{
	int i;
	unsigned long pfn;

	pfn = PFN_DOWN(e820.map[0].addr + e820.map[0].size);
	for (i = 1; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];

		if (pfn < PFN_UP(ei->addr))
			register_nosave_region(pfn, PFN_UP(ei->addr));

		pfn = PFN_DOWN(ei->addr + ei->size);
		if (ei->type != E820_RAM && ei->type != E820_RESERVED_KERN)
			register_nosave_region(PFN_UP(ei->addr), pfn);
=======
	gapsize = 0x400000;
	found  = e820_search_gap(&gapstart, &gapsize);

	if (!found) {
#ifdef CONFIG_X86_64
		gapstart = (max_pfn << PAGE_SHIFT) + 1024*1024;
		pr_err("Cannot find an available gap in the 32-bit address range\n");
		pr_err("PCI devices with unassigned 32-bit BARs may not work!\n");
#else
		gapstart = 0x10000000;
#endif
	}

	/*
	 * e820__reserve_resources_late() protects stolen RAM already:
	 */
	pci_mem_start = gapstart;

	pr_info("[mem %#010lx-%#010lx] available for PCI devices\n",
		gapstart, gapstart + gapsize - 1);
}

/*
 * Called late during init, in free_initmem().
 *
 * Initial e820_table and e820_table_kexec are largish __initdata arrays.
 *
 * Copy them to a (usually much smaller) dynamically allocated area that is
 * sized precisely after the number of e820 entries.
 *
 * This is done after we've performed all the fixes and tweaks to the tables.
 * All functions which modify them are __init functions, which won't exist
 * after free_initmem().
 */
__init void e820__reallocate_tables(void)
{
	struct e820_table *n;
	int size;

	size = offsetof(struct e820_table, entries) + sizeof(struct e820_entry)*e820_table->nr_entries;
	n = kmemdup(e820_table, size, GFP_KERNEL);
	BUG_ON(!n);
	e820_table = n;

	size = offsetof(struct e820_table, entries) + sizeof(struct e820_entry)*e820_table_kexec->nr_entries;
	n = kmemdup(e820_table_kexec, size, GFP_KERNEL);
	BUG_ON(!n);
	e820_table_kexec = n;

	size = offsetof(struct e820_table, entries) + sizeof(struct e820_entry)*e820_table_firmware->nr_entries;
	n = kmemdup(e820_table_firmware, size, GFP_KERNEL);
	BUG_ON(!n);
	e820_table_firmware = n;
}

/*
 * Because of the small fixed size of struct boot_params, only the first
 * 128 E820 memory entries are passed to the kernel via boot_params.e820_table,
 * the remaining (if any) entries are passed via the SETUP_E820_EXT node of
 * struct setup_data, which is parsed here.
 */
void __init e820__memory_setup_extended(u64 phys_addr, u32 data_len)
{
	int entries;
	struct boot_e820_entry *extmap;
	struct setup_data *sdata;

	sdata = early_memremap(phys_addr, data_len);
	entries = sdata->len / sizeof(*extmap);
	extmap = (struct boot_e820_entry *)(sdata->data);

	__append_e820_table(extmap, entries);
	e820__update_table(e820_table);

	memcpy(e820_table_kexec, e820_table, sizeof(*e820_table_kexec));
	memcpy(e820_table_firmware, e820_table, sizeof(*e820_table_firmware));

	early_memunmap(sdata, data_len);
	pr_info("extended physical RAM map:\n");
	e820__print_table("extended");
}

/*
 * Find the ranges of physical addresses that do not correspond to
 * E820 RAM areas and register the corresponding pages as 'nosave' for
 * hibernation (32-bit) or software suspend and suspend to RAM (64-bit).
 *
 * This function requires the E820 map to be sorted and without any
 * overlapping entries.
 */
void __init e820__register_nosave_regions(unsigned long limit_pfn)
{
	int i;
	unsigned long pfn = 0;

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];

		if (pfn < PFN_UP(entry->addr))
			register_nosave_region(pfn, PFN_UP(entry->addr));

		pfn = PFN_DOWN(entry->addr + entry->size);

		if (entry->type != E820_TYPE_RAM && entry->type != E820_TYPE_RESERVED_KERN)
			register_nosave_region(PFN_UP(entry->addr), pfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (pfn >= limit_pfn)
			break;
	}
}
<<<<<<< HEAD
#endif

#ifdef CONFIG_ACPI
/**
 * Mark ACPI NVS memory region, so that we can save/restore it during
 * hibernation and the subsequent resume.
 */
static int __init e820_mark_nvs_memory(void)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];

		if (ei->type == E820_NVS)
			acpi_nvs_register(ei->addr, ei->size);
=======

#ifdef CONFIG_ACPI
/*
 * Register ACPI NVS memory regions, so that we can save/restore them during
 * hibernation and the subsequent resume:
 */
static int __init e820__register_nvs_regions(void)
{
	int i;

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];

		if (entry->type == E820_TYPE_NVS)
			acpi_nvs_register(entry->addr, entry->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD
core_initcall(e820_mark_nvs_memory);
#endif

/*
 * pre allocated 4k and reserved it in memblock and e820_saved
 */
u64 __init early_reserve_e820(u64 size, u64 align)
{
	u64 addr;

	addr = __memblock_alloc_base(size, align, MEMBLOCK_ALLOC_ACCESSIBLE);
	if (addr) {
		e820_update_range_saved(addr, size, E820_RAM, E820_RESERVED);
		printk(KERN_INFO "update e820_saved for early_reserve_e820\n");
		update_e820_saved();
=======
core_initcall(e820__register_nvs_regions);
#endif

/*
 * Allocate the requested number of bytes with the requested alignment
 * and return (the physical address) to the caller. Also register this
 * range in the 'kexec' E820 table as a reserved range.
 *
 * This allows kexec to fake a new mptable, as if it came from the real
 * system.
 */
u64 __init e820__memblock_alloc_reserved(u64 size, u64 align)
{
	u64 addr;

	addr = memblock_phys_alloc(size, align);
	if (addr) {
		e820__range_update_table(e820_table_kexec, addr, size, E820_TYPE_RAM, E820_TYPE_RESERVED);
		pr_info("update e820_table_kexec for e820__memblock_alloc_reserved()\n");
		e820__update_table_kexec();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return addr;
}

#ifdef CONFIG_X86_32
# ifdef CONFIG_X86_PAE
#  define MAX_ARCH_PFN		(1ULL<<(36-PAGE_SHIFT))
# else
#  define MAX_ARCH_PFN		(1ULL<<(32-PAGE_SHIFT))
# endif
#else /* CONFIG_X86_32 */
# define MAX_ARCH_PFN MAXMEM>>PAGE_SHIFT
#endif

/*
 * Find the highest page frame number we have available
 */
<<<<<<< HEAD
static unsigned long __init e820_end_pfn(unsigned long limit_pfn, unsigned type)
=======
static unsigned long __init e820_end_pfn(unsigned long limit_pfn, enum e820_type type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	unsigned long last_pfn = 0;
	unsigned long max_arch_pfn = MAX_ARCH_PFN;

<<<<<<< HEAD
	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];
		unsigned long start_pfn;
		unsigned long end_pfn;

		if (ei->type != type)
			continue;

		start_pfn = ei->addr >> PAGE_SHIFT;
		end_pfn = (ei->addr + ei->size) >> PAGE_SHIFT;
=======
	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];
		unsigned long start_pfn;
		unsigned long end_pfn;

		if (entry->type != type)
			continue;

		start_pfn = entry->addr >> PAGE_SHIFT;
		end_pfn = (entry->addr + entry->size) >> PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (start_pfn >= limit_pfn)
			continue;
		if (end_pfn > limit_pfn) {
			last_pfn = limit_pfn;
			break;
		}
		if (end_pfn > last_pfn)
			last_pfn = end_pfn;
	}

	if (last_pfn > max_arch_pfn)
		last_pfn = max_arch_pfn;

<<<<<<< HEAD
	printk(KERN_INFO "last_pfn = %#lx max_arch_pfn = %#lx\n",
			 last_pfn, max_arch_pfn);
	return last_pfn;
}
unsigned long __init e820_end_of_ram_pfn(void)
{
	return e820_end_pfn(MAX_ARCH_PFN, E820_RAM);
}

unsigned long __init e820_end_of_low_ram_pfn(void)
{
	return e820_end_pfn(1UL<<(32 - PAGE_SHIFT), E820_RAM);
}

static void early_panic(char *msg)
=======
	pr_info("last_pfn = %#lx max_arch_pfn = %#lx\n",
		last_pfn, max_arch_pfn);
	return last_pfn;
}

unsigned long __init e820__end_of_ram_pfn(void)
{
	return e820_end_pfn(MAX_ARCH_PFN, E820_TYPE_RAM);
}

unsigned long __init e820__end_of_low_ram_pfn(void)
{
	return e820_end_pfn(1UL << (32 - PAGE_SHIFT), E820_TYPE_RAM);
}

static void __init early_panic(char *msg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	early_printk(msg);
	panic(msg);
}

static int userdef __initdata;

<<<<<<< HEAD
/* "mem=nopentium" disables the 4MB page tables. */
=======
/* The "mem=nopentium" boot option disables 4MB page tables on 32-bit kernels: */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init parse_memopt(char *p)
{
	u64 mem_size;

	if (!p)
		return -EINVAL;

	if (!strcmp(p, "nopentium")) {
#ifdef CONFIG_X86_32
		setup_clear_cpu_cap(X86_FEATURE_PSE);
		return 0;
#else
<<<<<<< HEAD
		printk(KERN_WARNING "mem=nopentium ignored! (only supported on x86_32)\n");
=======
		pr_warn("mem=nopentium ignored! (only supported on x86_32)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
#endif
	}

	userdef = 1;
	mem_size = memparse(p, &p);
<<<<<<< HEAD
	/* don't remove all of memory when handling "mem={invalid}" param */
	if (mem_size == 0)
		return -EINVAL;
	e820_remove_range(mem_size, ULLONG_MAX - mem_size, E820_RAM, 1);
=======

	/* Don't remove all memory when getting "mem={invalid}" parameter: */
	if (mem_size == 0)
		return -EINVAL;

	e820__range_remove(mem_size, ULLONG_MAX - mem_size, E820_TYPE_RAM, 1);

#ifdef CONFIG_MEMORY_HOTPLUG
	max_mem_size = mem_size;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
early_param("mem", parse_memopt);

<<<<<<< HEAD
static int __init parse_memmap_opt(char *p)
=======
static int __init parse_memmap_one(char *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *oldp;
	u64 start_at, mem_size;

	if (!p)
		return -EINVAL;

	if (!strncmp(p, "exactmap", 8)) {
<<<<<<< HEAD
#ifdef CONFIG_CRASH_DUMP
		/*
		 * If we are doing a crash dump, we still need to know
		 * the real mem size before original memory map is
		 * reset.
		 */
		saved_max_pfn = e820_end_of_ram_pfn();
#endif
		e820.nr_map = 0;
=======
		e820_table->nr_entries = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		userdef = 1;
		return 0;
	}

	oldp = p;
	mem_size = memparse(p, &p);
	if (p == oldp)
		return -EINVAL;

	userdef = 1;
	if (*p == '@') {
		start_at = memparse(p+1, &p);
<<<<<<< HEAD
		e820_add_region(start_at, mem_size, E820_RAM);
	} else if (*p == '#') {
		start_at = memparse(p+1, &p);
		e820_add_region(start_at, mem_size, E820_ACPI);
	} else if (*p == '$') {
		start_at = memparse(p+1, &p);
		e820_add_region(start_at, mem_size, E820_RESERVED);
	} else
		e820_remove_range(mem_size, ULLONG_MAX - mem_size, E820_RAM, 1);

	return *p == '\0' ? 0 : -EINVAL;
}
early_param("memmap", parse_memmap_opt);

void __init finish_e820_parsing(void)
{
	if (userdef) {
		u32 nr = e820.nr_map;

		if (sanitize_e820_map(e820.map, ARRAY_SIZE(e820.map), &nr) < 0)
			early_panic("Invalid user supplied memory map");
		e820.nr_map = nr;

		printk(KERN_INFO "user-defined physical RAM map:\n");
		e820_print_map("user");
	}
}

static inline const char *e820_type_to_string(int e820_type)
{
	switch (e820_type) {
	case E820_RESERVED_KERN:
	case E820_RAM:	return "System RAM";
	case E820_ACPI:	return "ACPI Tables";
	case E820_NVS:	return "ACPI Non-volatile Storage";
	case E820_UNUSABLE:	return "Unusable memory";
	default:	return "reserved";
=======
		e820__range_add(start_at, mem_size, E820_TYPE_RAM);
	} else if (*p == '#') {
		start_at = memparse(p+1, &p);
		e820__range_add(start_at, mem_size, E820_TYPE_ACPI);
	} else if (*p == '$') {
		start_at = memparse(p+1, &p);
		e820__range_add(start_at, mem_size, E820_TYPE_RESERVED);
	} else if (*p == '!') {
		start_at = memparse(p+1, &p);
		e820__range_add(start_at, mem_size, E820_TYPE_PRAM);
	} else if (*p == '%') {
		enum e820_type from = 0, to = 0;

		start_at = memparse(p + 1, &p);
		if (*p == '-')
			from = simple_strtoull(p + 1, &p, 0);
		if (*p == '+')
			to = simple_strtoull(p + 1, &p, 0);
		if (*p != '\0')
			return -EINVAL;
		if (from && to)
			e820__range_update(start_at, mem_size, from, to);
		else if (to)
			e820__range_add(start_at, mem_size, to);
		else if (from)
			e820__range_remove(start_at, mem_size, from, 1);
		else
			e820__range_remove(start_at, mem_size, 0, 0);
	} else {
		e820__range_remove(mem_size, ULLONG_MAX - mem_size, E820_TYPE_RAM, 1);
	}

	return *p == '\0' ? 0 : -EINVAL;
}

static int __init parse_memmap_opt(char *str)
{
	while (str) {
		char *k = strchr(str, ',');

		if (k)
			*k++ = 0;

		parse_memmap_one(str);
		str = k;
	}

	return 0;
}
early_param("memmap", parse_memmap_opt);

/*
 * Reserve all entries from the bootloader's extensible data nodes list,
 * because if present we are going to use it later on to fetch e820
 * entries from it:
 */
void __init e820__reserve_setup_data(void)
{
	struct setup_indirect *indirect;
	struct setup_data *data;
	u64 pa_data, pa_next;
	u32 len;

	pa_data = boot_params.hdr.setup_data;
	if (!pa_data)
		return;

	while (pa_data) {
		data = early_memremap(pa_data, sizeof(*data));
		if (!data) {
			pr_warn("e820: failed to memremap setup_data entry\n");
			return;
		}

		len = sizeof(*data);
		pa_next = data->next;

		e820__range_update(pa_data, sizeof(*data)+data->len, E820_TYPE_RAM, E820_TYPE_RESERVED_KERN);

		if (data->type == SETUP_INDIRECT) {
			len += data->len;
			early_memunmap(data, sizeof(*data));
			data = early_memremap(pa_data, len);
			if (!data) {
				pr_warn("e820: failed to memremap indirect setup_data\n");
				return;
			}

			indirect = (struct setup_indirect *)data->data;

			if (indirect->type != SETUP_INDIRECT)
				e820__range_update(indirect->addr, indirect->len,
						   E820_TYPE_RAM, E820_TYPE_RESERVED_KERN);
		}

		pa_data = pa_next;
		early_memunmap(data, len);
	}

	e820__update_table(e820_table);

	pr_info("extended physical RAM map:\n");
	e820__print_table("reserve setup_data");
}

/*
 * Called after parse_early_param(), after early parameters (such as mem=)
 * have been processed, in which case we already have an E820 table filled in
 * via the parameter callback function(s), but it's not sorted and printed yet:
 */
void __init e820__finish_early_params(void)
{
	if (userdef) {
		if (e820__update_table(e820_table) < 0)
			early_panic("Invalid user supplied memory map");

		pr_info("user-defined physical RAM map:\n");
		e820__print_table("user");
	}
}

static const char *__init e820_type_to_string(struct e820_entry *entry)
{
	switch (entry->type) {
	case E820_TYPE_RESERVED_KERN:	/* Fall-through: */
	case E820_TYPE_RAM:		return "System RAM";
	case E820_TYPE_ACPI:		return "ACPI Tables";
	case E820_TYPE_NVS:		return "ACPI Non-volatile Storage";
	case E820_TYPE_UNUSABLE:	return "Unusable memory";
	case E820_TYPE_PRAM:		return "Persistent Memory (legacy)";
	case E820_TYPE_PMEM:		return "Persistent Memory";
	case E820_TYPE_RESERVED:	return "Reserved";
	case E820_TYPE_SOFT_RESERVED:	return "Soft Reserved";
	default:			return "Unknown E820 type";
	}
}

static unsigned long __init e820_type_to_iomem_type(struct e820_entry *entry)
{
	switch (entry->type) {
	case E820_TYPE_RESERVED_KERN:	/* Fall-through: */
	case E820_TYPE_RAM:		return IORESOURCE_SYSTEM_RAM;
	case E820_TYPE_ACPI:		/* Fall-through: */
	case E820_TYPE_NVS:		/* Fall-through: */
	case E820_TYPE_UNUSABLE:	/* Fall-through: */
	case E820_TYPE_PRAM:		/* Fall-through: */
	case E820_TYPE_PMEM:		/* Fall-through: */
	case E820_TYPE_RESERVED:	/* Fall-through: */
	case E820_TYPE_SOFT_RESERVED:	/* Fall-through: */
	default:			return IORESOURCE_MEM;
	}
}

static unsigned long __init e820_type_to_iores_desc(struct e820_entry *entry)
{
	switch (entry->type) {
	case E820_TYPE_ACPI:		return IORES_DESC_ACPI_TABLES;
	case E820_TYPE_NVS:		return IORES_DESC_ACPI_NV_STORAGE;
	case E820_TYPE_PMEM:		return IORES_DESC_PERSISTENT_MEMORY;
	case E820_TYPE_PRAM:		return IORES_DESC_PERSISTENT_MEMORY_LEGACY;
	case E820_TYPE_RESERVED:	return IORES_DESC_RESERVED;
	case E820_TYPE_SOFT_RESERVED:	return IORES_DESC_SOFT_RESERVED;
	case E820_TYPE_RESERVED_KERN:	/* Fall-through: */
	case E820_TYPE_RAM:		/* Fall-through: */
	case E820_TYPE_UNUSABLE:	/* Fall-through: */
	default:			return IORES_DESC_NONE;
	}
}

static bool __init do_mark_busy(enum e820_type type, struct resource *res)
{
	/* this is the legacy bios/dos rom-shadow + mmio region */
	if (res->start < (1ULL<<20))
		return true;

	/*
	 * Treat persistent memory and other special memory ranges like
	 * device memory, i.e. reserve it for exclusive use of a driver
	 */
	switch (type) {
	case E820_TYPE_RESERVED:
	case E820_TYPE_SOFT_RESERVED:
	case E820_TYPE_PRAM:
	case E820_TYPE_PMEM:
		return false;
	case E820_TYPE_RESERVED_KERN:
	case E820_TYPE_RAM:
	case E820_TYPE_ACPI:
	case E820_TYPE_NVS:
	case E820_TYPE_UNUSABLE:
	default:
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
 * Mark e820 reserved areas as busy for the resource manager.
 */
static struct resource __initdata *e820_res;
void __init e820_reserve_resources(void)
=======
 * Mark E820 reserved areas as busy for the resource manager:
 */

static struct resource __initdata *e820_res;

void __init e820__reserve_resources(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	struct resource *res;
	u64 end;

<<<<<<< HEAD
	res = alloc_bootmem(sizeof(struct resource) * e820.nr_map);
	e820_res = res;
	for (i = 0; i < e820.nr_map; i++) {
		end = e820.map[i].addr + e820.map[i].size - 1;
=======
	res = memblock_alloc(sizeof(*res) * e820_table->nr_entries,
			     SMP_CACHE_BYTES);
	if (!res)
		panic("%s: Failed to allocate %zu bytes\n", __func__,
		      sizeof(*res) * e820_table->nr_entries);
	e820_res = res;

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = e820_table->entries + i;

		end = entry->addr + entry->size - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (end != (resource_size_t)end) {
			res++;
			continue;
		}
<<<<<<< HEAD
		res->name = e820_type_to_string(e820.map[i].type);
		res->start = e820.map[i].addr;
		res->end = end;

		res->flags = IORESOURCE_MEM;

		/*
		 * don't register the region that could be conflicted with
		 * pci device BAR resource and insert them later in
		 * pcibios_resource_survey()
		 */
		if (e820.map[i].type != E820_RESERVED || res->start < (1ULL<<20)) {
=======
		res->start = entry->addr;
		res->end   = end;
		res->name  = e820_type_to_string(entry);
		res->flags = e820_type_to_iomem_type(entry);
		res->desc  = e820_type_to_iores_desc(entry);

		/*
		 * Don't register the region that could be conflicted with
		 * PCI device BAR resources and insert them later in
		 * pcibios_resource_survey():
		 */
		if (do_mark_busy(entry->type, res)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			res->flags |= IORESOURCE_BUSY;
			insert_resource(&iomem_resource, res);
		}
		res++;
	}

<<<<<<< HEAD
	for (i = 0; i < e820_saved.nr_map; i++) {
		struct e820entry *entry = &e820_saved.map[i];
		firmware_map_add_early(entry->addr,
			entry->addr + entry->size - 1,
			e820_type_to_string(entry->type));
	}
}

/* How much should we pad RAM ending depending on where it is? */
static unsigned long ram_alignment(resource_size_t pos)
=======
	/* Expose the bootloader-provided memory layout to the sysfs. */
	for (i = 0; i < e820_table_firmware->nr_entries; i++) {
		struct e820_entry *entry = e820_table_firmware->entries + i;

		firmware_map_add_early(entry->addr, entry->addr + entry->size, e820_type_to_string(entry));
	}
}

/*
 * How much should we pad the end of RAM, depending on where it is?
 */
static unsigned long __init ram_alignment(resource_size_t pos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long mb = pos >> 20;

	/* To 64kB in the first megabyte */
	if (!mb)
		return 64*1024;

	/* To 1MB in the first 16MB */
	if (mb < 16)
		return 1024*1024;

	/* To 64MB for anything above that */
	return 64*1024*1024;
}

#define MAX_RESOURCE_SIZE ((resource_size_t)-1)

<<<<<<< HEAD
void __init e820_reserve_resources_late(void)
=======
void __init e820__reserve_resources_late(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	struct resource *res;

	res = e820_res;
<<<<<<< HEAD
	for (i = 0; i < e820.nr_map; i++) {
=======
	for (i = 0; i < e820_table->nr_entries; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!res->parent && res->end)
			insert_resource_expand_to_fit(&iomem_resource, res);
		res++;
	}

	/*
<<<<<<< HEAD
	 * Try to bump up RAM regions to reasonable boundaries to
	 * avoid stolen RAM:
	 */
	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *entry = &e820.map[i];
		u64 start, end;

		if (entry->type != E820_RAM)
			continue;
=======
	 * Try to bump up RAM regions to reasonable boundaries, to
	 * avoid stolen RAM:
	 */
	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];
		u64 start, end;

		if (entry->type != E820_TYPE_RAM)
			continue;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		start = entry->addr + entry->size;
		end = round_up(start, ram_alignment(start)) - 1;
		if (end > MAX_RESOURCE_SIZE)
			end = MAX_RESOURCE_SIZE;
		if (start >= end)
			continue;
<<<<<<< HEAD
		printk(KERN_DEBUG "reserve RAM buffer: %016llx - %016llx ",
			       start, end);
		reserve_region_with_split(&iomem_resource, start, end,
					  "RAM buffer");
	}
}

char *__init default_machine_specific_memory_setup(void)
{
	char *who = "BIOS-e820";
	u32 new_nr;
=======

		printk(KERN_DEBUG "e820: reserve RAM buffer [mem %#010llx-%#010llx]\n", start, end);
		reserve_region_with_split(&iomem_resource, start, end, "RAM buffer");
	}
}

/*
 * Pass the firmware (bootloader) E820 map to the kernel and process it:
 */
char *__init e820__memory_setup_default(void)
{
	char *who = "BIOS-e820";

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Try to copy the BIOS-supplied E820-map.
	 *
	 * Otherwise fake a memory map; one section from 0k->640k,
	 * the next section from 1mb->appropriate_mem_k
	 */
<<<<<<< HEAD
	new_nr = boot_params.e820_entries;
	sanitize_e820_map(boot_params.e820_map,
			ARRAY_SIZE(boot_params.e820_map),
			&new_nr);
	boot_params.e820_entries = new_nr;
	if (append_e820_map(boot_params.e820_map, boot_params.e820_entries)
	  < 0) {
		u64 mem_size;

		/* compare results from other methods and take the greater */
		if (boot_params.alt_mem_k
		    < boot_params.screen_info.ext_mem_k) {
=======
	if (append_e820_table(boot_params.e820_table, boot_params.e820_entries) < 0) {
		u64 mem_size;

		/* Compare results from other methods and take the one that gives more RAM: */
		if (boot_params.alt_mem_k < boot_params.screen_info.ext_mem_k) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mem_size = boot_params.screen_info.ext_mem_k;
			who = "BIOS-88";
		} else {
			mem_size = boot_params.alt_mem_k;
			who = "BIOS-e801";
		}

<<<<<<< HEAD
		e820.nr_map = 0;
		e820_add_region(0, LOWMEMSIZE(), E820_RAM);
		e820_add_region(HIGH_MEMORY, mem_size << 10, E820_RAM);
	}

	/* In case someone cares... */
	return who;
}

void __init setup_memory_map(void)
{
	char *who;

	who = x86_init.resources.memory_setup();
	memcpy(&e820_saved, &e820, sizeof(struct e820map));
	printk(KERN_INFO "BIOS-provided physical RAM map:\n");
	e820_print_map(who);
}

void __init memblock_x86_fill(void)
=======
		e820_table->nr_entries = 0;
		e820__range_add(0, LOWMEMSIZE(), E820_TYPE_RAM);
		e820__range_add(HIGH_MEMORY, mem_size << 10, E820_TYPE_RAM);
	}

	/* We just appended a lot of ranges, sanitize the table: */
	e820__update_table(e820_table);

	return who;
}

/*
 * Calls e820__memory_setup_default() in essence to pick up the firmware/bootloader
 * E820 map - with an optional platform quirk available for virtual platforms
 * to override this method of boot environment processing:
 */
void __init e820__memory_setup(void)
{
	char *who;

	/* This is a firmware interface ABI - make sure we don't break it: */
	BUILD_BUG_ON(sizeof(struct boot_e820_entry) != 20);

	who = x86_init.resources.memory_setup();

	memcpy(e820_table_kexec, e820_table, sizeof(*e820_table_kexec));
	memcpy(e820_table_firmware, e820_table, sizeof(*e820_table_firmware));

	pr_info("BIOS-provided physical RAM map:\n");
	e820__print_table(who);
}

void __init e820__memblock_setup(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	u64 end;

	/*
<<<<<<< HEAD
	 * EFI may have more than 128 entries
	 * We are safe to enable resizing, beause memblock_x86_fill()
	 * is rather later for x86
	 */
	memblock_allow_resize();

	for (i = 0; i < e820.nr_map; i++) {
		struct e820entry *ei = &e820.map[i];

		end = ei->addr + ei->size;
		if (end != (resource_size_t)end)
			continue;

		if (ei->type != E820_RAM && ei->type != E820_RESERVED_KERN)
			continue;

		memblock_add(ei->addr, ei->size);
	}

	/* throw away partial pages */
=======
	 * The bootstrap memblock region count maximum is 128 entries
	 * (INIT_MEMBLOCK_REGIONS), but EFI might pass us more E820 entries
	 * than that - so allow memblock resizing.
	 *
	 * This is safe, because this call happens pretty late during x86 setup,
	 * so we know about reserved memory regions already. (This is important
	 * so that memblock resizing does no stomp over reserved areas.)
	 */
	memblock_allow_resize();

	for (i = 0; i < e820_table->nr_entries; i++) {
		struct e820_entry *entry = &e820_table->entries[i];

		end = entry->addr + entry->size;
		if (end != (resource_size_t)end)
			continue;

		if (entry->type == E820_TYPE_SOFT_RESERVED)
			memblock_reserve(entry->addr, entry->size);

		if (entry->type != E820_TYPE_RAM && entry->type != E820_TYPE_RESERVED_KERN)
			continue;

		memblock_add(entry->addr, entry->size);
	}

	/* Throw away partial pages: */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memblock_trim_memory(PAGE_SIZE);

	memblock_dump_all();
}
<<<<<<< HEAD

void __init memblock_find_dma_reserve(void)
{
#ifdef CONFIG_X86_64
	u64 nr_pages = 0, nr_free_pages = 0;
	unsigned long start_pfn, end_pfn;
	phys_addr_t start, end;
	int i;
	u64 u;

	/*
	 * need to find out used area below MAX_DMA_PFN
	 * need to use memblock to get free size in [0, MAX_DMA_PFN]
	 * at first, and assume boot_mem will not take below MAX_DMA_PFN
	 */
	for_each_mem_pfn_range(i, MAX_NUMNODES, &start_pfn, &end_pfn, NULL) {
		start_pfn = min_t(unsigned long, start_pfn, MAX_DMA_PFN);
		end_pfn = min_t(unsigned long, end_pfn, MAX_DMA_PFN);
		nr_pages += end_pfn - start_pfn;
	}

	for_each_free_mem_range(u, MAX_NUMNODES, &start, &end, NULL) {
		start_pfn = min_t(unsigned long, PFN_UP(start), MAX_DMA_PFN);
		end_pfn = min_t(unsigned long, PFN_DOWN(end), MAX_DMA_PFN);
		if (start_pfn < end_pfn)
			nr_free_pages += end_pfn - start_pfn;
	}

	set_dma_reserve(nr_pages - nr_free_pages);
#endif
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
