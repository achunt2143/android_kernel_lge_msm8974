<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This only handles 32bit MTRR on 32bit hosts. This is strictly wrong
 * because MTRRs can span up to 40 bits (36bits on most modern x86)
 */
<<<<<<< HEAD
#define DEBUG

#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/mm.h>

#include <asm/processor-flags.h>
#include <asm/cpufeature.h>
#include <asm/tlbflush.h>
#include <asm/mtrr.h>
#include <asm/msr.h>
#include <asm/pat.h>
=======

#include <linux/export.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/cc_platform.h>
#include <asm/processor-flags.h>
#include <asm/cacheinfo.h>
#include <asm/cpufeature.h>
#include <asm/hypervisor.h>
#include <asm/mshyperv.h>
#include <asm/tlbflush.h>
#include <asm/mtrr.h>
#include <asm/msr.h>
#include <asm/memtype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "mtrr.h"

struct fixed_range_block {
	int base_msr;		/* start address of an MTRR block */
	int ranges;		/* number of MTRRs in this block  */
};

static struct fixed_range_block fixed_range_blocks[] = {
	{ MSR_MTRRfix64K_00000, 1 }, /* one   64k MTRR  */
	{ MSR_MTRRfix16K_80000, 2 }, /* two   16k MTRRs */
	{ MSR_MTRRfix4K_C0000,  8 }, /* eight  4k MTRRs */
	{}
};

<<<<<<< HEAD
=======
struct cache_map {
	u64 start;
	u64 end;
	u64 flags;
	u64 type:8;
	u64 fixed:1;
};

bool mtrr_debug;

static int __init mtrr_param_setup(char *str)
{
	int rc = 0;

	if (!str)
		return -EINVAL;
	if (!strcmp(str, "debug"))
		mtrr_debug = true;
	else
		rc = -EINVAL;

	return rc;
}
early_param("mtrr", mtrr_param_setup);

/*
 * CACHE_MAP_MAX is the maximum number of memory ranges in cache_map, where
 * no 2 adjacent ranges have the same cache mode (those would be merged).
 * The number is based on the worst case:
 * - no two adjacent fixed MTRRs share the same cache mode
 * - one variable MTRR is spanning a huge area with mode WB
 * - 255 variable MTRRs with mode UC all overlap with the WB MTRR, creating 2
 *   additional ranges each (result like "ababababa...aba" with a = WB, b = UC),
 *   accounting for MTRR_MAX_VAR_RANGES * 2 - 1 range entries
 * - a TOP_MEM2 area (even with overlapping an UC MTRR can't add 2 range entries
 *   to the possible maximum, as it always starts at 4GB, thus it can't be in
 *   the middle of that MTRR, unless that MTRR starts at 0, which would remove
 *   the initial "a" from the "abababa" pattern above)
 * The map won't contain ranges with no matching MTRR (those fall back to the
 * default cache mode).
 */
#define CACHE_MAP_MAX	(MTRR_NUM_FIXED_RANGES + MTRR_MAX_VAR_RANGES * 2)

static struct cache_map init_cache_map[CACHE_MAP_MAX] __initdata;
static struct cache_map *cache_map __refdata = init_cache_map;
static unsigned int cache_map_size = CACHE_MAP_MAX;
static unsigned int cache_map_n;
static unsigned int cache_map_fixed;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long smp_changes_mask;
static int mtrr_state_set;
u64 mtrr_tom2;

struct mtrr_state_type mtrr_state;
EXPORT_SYMBOL_GPL(mtrr_state);

<<<<<<< HEAD
=======
/* Reserved bits in the high portion of the MTRRphysBaseN MSR. */
u32 phys_hi_rsvd;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * BIOS is expected to clear MtrrFixDramModEn bit, see for example
 * "BIOS and Kernel Developer's Guide for the AMD Athlon 64 and AMD
 * Opteron Processors" (26094 Rev. 3.30 February 2006), section
 * "13.2.1.2 SYSCFG Register": "The MtrrFixDramModEn bit should be set
<<<<<<< HEAD
 * to 1 during BIOS initalization of the fixed MTRRs, then cleared to
=======
 * to 1 during BIOS initialization of the fixed MTRRs, then cleared to
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * 0 for operation."
 */
static inline void k8_check_syscfg_dram_mod_en(void)
{
	u32 lo, hi;

	if (!((boot_cpu_data.x86_vendor == X86_VENDOR_AMD) &&
	      (boot_cpu_data.x86 >= 0x0f)))
		return;

<<<<<<< HEAD
	rdmsr(MSR_K8_SYSCFG, lo, hi);
	if (lo & K8_MTRRFIXRANGE_DRAM_MODIFY) {
		printk(KERN_ERR FW_WARN "MTRR: CPU %u: SYSCFG[MtrrFixDramModEn]"
		       " not cleared by BIOS, clearing this bit\n",
		       smp_processor_id());
		lo &= ~K8_MTRRFIXRANGE_DRAM_MODIFY;
		mtrr_wrmsr(MSR_K8_SYSCFG, lo, hi);
=======
	if (cc_platform_has(CC_ATTR_HOST_SEV_SNP))
		return;

	rdmsr(MSR_AMD64_SYSCFG, lo, hi);
	if (lo & K8_MTRRFIXRANGE_DRAM_MODIFY) {
		pr_err(FW_WARN "MTRR: CPU %u: SYSCFG[MtrrFixDramModEn]"
		       " not cleared by BIOS, clearing this bit\n",
		       smp_processor_id());
		lo &= ~K8_MTRRFIXRANGE_DRAM_MODIFY;
		mtrr_wrmsr(MSR_AMD64_SYSCFG, lo, hi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* Get the size of contiguous MTRR range */
static u64 get_mtrr_size(u64 mask)
{
	u64 size;

<<<<<<< HEAD
	mask >>= PAGE_SHIFT;
	mask |= size_or_mask;
	size = -mask;
	size <<= PAGE_SHIFT;
	return size;
}

/*
 * Check and return the effective type for MTRR-MTRR type overlap.
 * Returns 1 if the effective type is UNCACHEABLE, else returns 0
 */
static int check_type_overlap(u8 *prev, u8 *curr)
{
	if (*prev == MTRR_TYPE_UNCACHABLE || *curr == MTRR_TYPE_UNCACHABLE) {
		*prev = MTRR_TYPE_UNCACHABLE;
		*curr = MTRR_TYPE_UNCACHABLE;
		return 1;
	}

	if ((*prev == MTRR_TYPE_WRBACK && *curr == MTRR_TYPE_WRTHROUGH) ||
	    (*prev == MTRR_TYPE_WRTHROUGH && *curr == MTRR_TYPE_WRBACK)) {
		*prev = MTRR_TYPE_WRTHROUGH;
		*curr = MTRR_TYPE_WRTHROUGH;
	}

	if (*prev != *curr) {
		*prev = MTRR_TYPE_UNCACHABLE;
		*curr = MTRR_TYPE_UNCACHABLE;
		return 1;
	}

	return 0;
}

/*
 * Error/Semi-error returns:
 * 0xFF - when MTRR is not enabled
 * *repeat == 1 implies [start:end] spanned across MTRR range and type returned
 *		corresponds only to [start:*partial_end].
 *		Caller has to lookup again for [*partial_end:end].
 */
static u8 __mtrr_type_lookup(u64 start, u64 end, u64 *partial_end, int *repeat)
{
	int i;
	u64 base, mask;
	u8 prev_match, curr_match;

	*repeat = 0;
	if (!mtrr_state_set)
		return 0xFF;

	if (!mtrr_state.enabled)
		return 0xFF;

	/* Make end inclusive end, instead of exclusive */
	end--;

	/* Look in fixed ranges. Just return the type as per start */
	if (mtrr_state.have_fixed && (start < 0x100000)) {
		int idx;

		if (start < 0x80000) {
			idx = 0;
			idx += (start >> 16);
			return mtrr_state.fixed_ranges[idx];
		} else if (start < 0xC0000) {
			idx = 1 * 8;
			idx += ((start - 0x80000) >> 14);
			return mtrr_state.fixed_ranges[idx];
		} else if (start < 0x1000000) {
			idx = 3 * 8;
			idx += ((start - 0xC0000) >> 12);
			return mtrr_state.fixed_ranges[idx];
		}
	}

	/*
	 * Look in variable ranges
	 * Look of multiple ranges matching this address and pick type
	 * as per MTRR precedence
	 */
	if (!(mtrr_state.enabled & 2))
		return mtrr_state.def_type;

	prev_match = 0xFF;
	for (i = 0; i < num_var_ranges; ++i) {
		unsigned short start_state, end_state;

		if (!(mtrr_state.var_ranges[i].mask_lo & (1 << 11)))
			continue;

		base = (((u64)mtrr_state.var_ranges[i].base_hi) << 32) +
		       (mtrr_state.var_ranges[i].base_lo & PAGE_MASK);
		mask = (((u64)mtrr_state.var_ranges[i].mask_hi) << 32) +
		       (mtrr_state.var_ranges[i].mask_lo & PAGE_MASK);

		start_state = ((start & mask) == (base & mask));
		end_state = ((end & mask) == (base & mask));

		if (start_state != end_state) {
			/*
			 * We have start:end spanning across an MTRR.
			 * We split the region into
			 * either
			 * (start:mtrr_end) (mtrr_end:end)
			 * or
			 * (start:mtrr_start) (mtrr_start:end)
			 * depending on kind of overlap.
			 * Return the type for first region and a pointer to
			 * the start of second region so that caller will
			 * lookup again on the second region.
			 * Note: This way we handle multiple overlaps as well.
			 */
			if (start_state)
				*partial_end = base + get_mtrr_size(mask);
			else
				*partial_end = base;

			if (unlikely(*partial_end <= start)) {
				WARN_ON(1);
				*partial_end = start + PAGE_SIZE;
			}

			end = *partial_end - 1; /* end is inclusive */
			*repeat = 1;
		}

		if ((start & mask) != (base & mask))
			continue;

		curr_match = mtrr_state.var_ranges[i].base_lo & 0xff;
		if (prev_match == 0xFF) {
			prev_match = curr_match;
			continue;
		}

		if (check_type_overlap(&prev_match, &curr_match))
			return curr_match;
	}

	if (mtrr_tom2) {
		if (start >= (1ULL<<32) && (end < mtrr_tom2))
			return MTRR_TYPE_WRBACK;
	}

	if (prev_match != 0xFF)
		return prev_match;

	return mtrr_state.def_type;
}

/*
 * Returns the effective MTRR type for the region
 * Error return:
 * 0xFF - when MTRR is not enabled
 */
u8 mtrr_type_lookup(u64 start, u64 end)
{
	u8 type, prev_type;
	int repeat;
	u64 partial_end;

	type = __mtrr_type_lookup(start, end, &partial_end, &repeat);

	/*
	 * Common path is with repeat = 0.
	 * However, we can have cases where [start:end] spans across some
	 * MTRR range. Do repeated lookups for that case here.
	 */
	while (repeat) {
		prev_type = type;
		start = partial_end;
		type = __mtrr_type_lookup(start, end, &partial_end, &repeat);

		if (check_type_overlap(&prev_type, &type))
			return type;
	}

=======
	mask |= (u64)phys_hi_rsvd << 32;
	size = -mask;

	return size;
}

static u8 get_var_mtrr_state(unsigned int reg, u64 *start, u64 *size)
{
	struct mtrr_var_range *mtrr = mtrr_state.var_ranges + reg;

	if (!(mtrr->mask_lo & MTRR_PHYSMASK_V))
		return MTRR_TYPE_INVALID;

	*start = (((u64)mtrr->base_hi) << 32) + (mtrr->base_lo & PAGE_MASK);
	*size = get_mtrr_size((((u64)mtrr->mask_hi) << 32) +
			      (mtrr->mask_lo & PAGE_MASK));

	return mtrr->base_lo & MTRR_PHYSBASE_TYPE;
}

static u8 get_effective_type(u8 type1, u8 type2)
{
	if (type1 == MTRR_TYPE_UNCACHABLE || type2 == MTRR_TYPE_UNCACHABLE)
		return MTRR_TYPE_UNCACHABLE;

	if ((type1 == MTRR_TYPE_WRBACK && type2 == MTRR_TYPE_WRTHROUGH) ||
	    (type1 == MTRR_TYPE_WRTHROUGH && type2 == MTRR_TYPE_WRBACK))
		return MTRR_TYPE_WRTHROUGH;

	if (type1 != type2)
		return MTRR_TYPE_UNCACHABLE;

	return type1;
}

static void rm_map_entry_at(int idx)
{
	cache_map_n--;
	if (cache_map_n > idx) {
		memmove(cache_map + idx, cache_map + idx + 1,
			sizeof(*cache_map) * (cache_map_n - idx));
	}
}

/*
 * Add an entry into cache_map at a specific index.  Merges adjacent entries if
 * appropriate.  Return the number of merges for correcting the scan index
 * (this is needed as merging will reduce the number of entries, which will
 * result in skipping entries in future iterations if the scan index isn't
 * corrected).
 * Note that the corrected index can never go below -1 (resulting in being 0 in
 * the next scan iteration), as "2" is returned only if the current index is
 * larger than zero.
 */
static int add_map_entry_at(u64 start, u64 end, u8 type, int idx)
{
	bool merge_prev = false, merge_next = false;

	if (start >= end)
		return 0;

	if (idx > 0) {
		struct cache_map *prev = cache_map + idx - 1;

		if (!prev->fixed && start == prev->end && type == prev->type)
			merge_prev = true;
	}

	if (idx < cache_map_n) {
		struct cache_map *next = cache_map + idx;

		if (!next->fixed && end == next->start && type == next->type)
			merge_next = true;
	}

	if (merge_prev && merge_next) {
		cache_map[idx - 1].end = cache_map[idx].end;
		rm_map_entry_at(idx);
		return 2;
	}
	if (merge_prev) {
		cache_map[idx - 1].end = end;
		return 1;
	}
	if (merge_next) {
		cache_map[idx].start = start;
		return 1;
	}

	/* Sanity check: the array should NEVER be too small! */
	if (cache_map_n == cache_map_size) {
		WARN(1, "MTRR cache mode memory map exhausted!\n");
		cache_map_n = cache_map_fixed;
		return 0;
	}

	if (cache_map_n > idx) {
		memmove(cache_map + idx + 1, cache_map + idx,
			sizeof(*cache_map) * (cache_map_n - idx));
	}

	cache_map[idx].start = start;
	cache_map[idx].end = end;
	cache_map[idx].type = type;
	cache_map[idx].fixed = 0;
	cache_map_n++;

	return 0;
}

/* Clear a part of an entry. Return 1 if start of entry is still valid. */
static int clr_map_range_at(u64 start, u64 end, int idx)
{
	int ret = start != cache_map[idx].start;
	u64 tmp;

	if (start == cache_map[idx].start && end == cache_map[idx].end) {
		rm_map_entry_at(idx);
	} else if (start == cache_map[idx].start) {
		cache_map[idx].start = end;
	} else if (end == cache_map[idx].end) {
		cache_map[idx].end = start;
	} else {
		tmp = cache_map[idx].end;
		cache_map[idx].end = start;
		add_map_entry_at(end, tmp, cache_map[idx].type, idx + 1);
	}

	return ret;
}

/*
 * Add MTRR to the map.  The current map is scanned and each part of the MTRR
 * either overlapping with an existing entry or with a hole in the map is
 * handled separately.
 */
static void add_map_entry(u64 start, u64 end, u8 type)
{
	u8 new_type, old_type;
	u64 tmp;
	int i;

	for (i = 0; i < cache_map_n && start < end; i++) {
		if (start >= cache_map[i].end)
			continue;

		if (start < cache_map[i].start) {
			/* Region start has no overlap. */
			tmp = min(end, cache_map[i].start);
			i -= add_map_entry_at(start, tmp,  type, i);
			start = tmp;
			continue;
		}

		new_type = get_effective_type(type, cache_map[i].type);
		old_type = cache_map[i].type;

		if (cache_map[i].fixed || new_type == old_type) {
			/* Cut off start of new entry. */
			start = cache_map[i].end;
			continue;
		}

		/* Handle only overlapping part of region. */
		tmp = min(end, cache_map[i].end);
		i += clr_map_range_at(start, tmp, i);
		i -= add_map_entry_at(start, tmp, new_type, i);
		start = tmp;
	}

	/* Add rest of region after last map entry (rest might be empty). */
	add_map_entry_at(start, end, type, i);
}

/* Add variable MTRRs to cache map. */
static void map_add_var(void)
{
	u64 start, size;
	unsigned int i;
	u8 type;

	/*
	 * Add AMD TOP_MEM2 area.  Can't be added in mtrr_build_map(), as it
	 * needs to be added again when rebuilding the map due to potentially
	 * having moved as a result of variable MTRRs for memory below 4GB.
	 */
	if (mtrr_tom2) {
		add_map_entry(BIT_ULL(32), mtrr_tom2, MTRR_TYPE_WRBACK);
		cache_map[cache_map_n - 1].fixed = 1;
	}

	for (i = 0; i < num_var_ranges; i++) {
		type = get_var_mtrr_state(i, &start, &size);
		if (type != MTRR_TYPE_INVALID)
			add_map_entry(start, start + size, type);
	}
}

/*
 * Rebuild map by replacing variable entries.  Needs to be called when MTRR
 * registers are being changed after boot, as such changes could include
 * removals of registers, which are complicated to handle without rebuild of
 * the map.
 */
void generic_rebuild_map(void)
{
	if (mtrr_if != &generic_mtrr_ops)
		return;

	cache_map_n = cache_map_fixed;

	map_add_var();
}

static unsigned int __init get_cache_map_size(void)
{
	return cache_map_fixed + 2 * num_var_ranges + (mtrr_tom2 != 0);
}

/* Build the cache_map containing the cache modes per memory range. */
void __init mtrr_build_map(void)
{
	u64 start, end, size;
	unsigned int i;
	u8 type;

	/* Add fixed MTRRs, optimize for adjacent entries with same type. */
	if (mtrr_state.enabled & MTRR_STATE_MTRR_FIXED_ENABLED) {
		/*
		 * Start with 64k size fixed entries, preset 1st one (hence the
		 * loop below is starting with index 1).
		 */
		start = 0;
		end = size = 0x10000;
		type = mtrr_state.fixed_ranges[0];

		for (i = 1; i < MTRR_NUM_FIXED_RANGES; i++) {
			/* 8 64k entries, then 16 16k ones, rest 4k. */
			if (i == 8 || i == 24)
				size >>= 2;

			if (mtrr_state.fixed_ranges[i] != type) {
				add_map_entry(start, end, type);
				start = end;
				type = mtrr_state.fixed_ranges[i];
			}
			end += size;
		}
		add_map_entry(start, end, type);
	}

	/* Mark fixed, they take precedence. */
	for (i = 0; i < cache_map_n; i++)
		cache_map[i].fixed = 1;
	cache_map_fixed = cache_map_n;

	map_add_var();

	pr_info("MTRR map: %u entries (%u fixed + %u variable; max %u), built from %u variable MTRRs\n",
		cache_map_n, cache_map_fixed, cache_map_n - cache_map_fixed,
		get_cache_map_size(), num_var_ranges + (mtrr_tom2 != 0));

	if (mtrr_debug) {
		for (i = 0; i < cache_map_n; i++) {
			pr_info("%3u: %016llx-%016llx %s\n", i,
				cache_map[i].start, cache_map[i].end - 1,
				mtrr_attrib_to_str(cache_map[i].type));
		}
	}
}

/* Copy the cache_map from __initdata memory to dynamically allocated one. */
void __init mtrr_copy_map(void)
{
	unsigned int new_size = get_cache_map_size();

	if (!mtrr_state.enabled || !new_size) {
		cache_map = NULL;
		return;
	}

	mutex_lock(&mtrr_mutex);

	cache_map = kcalloc(new_size, sizeof(*cache_map), GFP_KERNEL);
	if (cache_map) {
		memmove(cache_map, init_cache_map,
			cache_map_n * sizeof(*cache_map));
		cache_map_size = new_size;
	} else {
		mtrr_state.enabled = 0;
		pr_err("MTRRs disabled due to allocation failure for lookup map.\n");
	}

	mutex_unlock(&mtrr_mutex);
}

/**
 * mtrr_overwrite_state - set static MTRR state
 *
 * Used to set MTRR state via different means (e.g. with data obtained from
 * a hypervisor).
 * Is allowed only for special cases when running virtualized. Must be called
 * from the x86_init.hyper.init_platform() hook.  It can be called only once.
 * The MTRR state can't be changed afterwards.  To ensure that, X86_FEATURE_MTRR
 * is cleared.
 *
 * @var: MTRR variable range array to use
 * @num_var: length of the @var array
 * @def_type: default caching type
 */
void mtrr_overwrite_state(struct mtrr_var_range *var, unsigned int num_var,
			  mtrr_type def_type)
{
	unsigned int i;

	/* Only allowed to be called once before mtrr_bp_init(). */
	if (WARN_ON_ONCE(mtrr_state_set))
		return;

	/* Only allowed when running virtualized. */
	if (!cpu_feature_enabled(X86_FEATURE_HYPERVISOR))
		return;

	/*
	 * Only allowed for special virtualization cases:
	 * - when running as Hyper-V, SEV-SNP guest using vTOM
	 * - when running as Xen PV guest
	 * - when running as SEV-SNP or TDX guest to avoid unnecessary
	 *   VMM communication/Virtualization exceptions (#VC, #VE)
	 */
	if (!cc_platform_has(CC_ATTR_GUEST_SEV_SNP) &&
	    !hv_is_isolation_supported() &&
	    !cpu_feature_enabled(X86_FEATURE_XENPV) &&
	    !cpu_feature_enabled(X86_FEATURE_TDX_GUEST))
		return;

	/* Disable MTRR in order to disable MTRR modifications. */
	setup_clear_cpu_cap(X86_FEATURE_MTRR);

	if (var) {
		if (num_var > MTRR_MAX_VAR_RANGES) {
			pr_warn("Trying to overwrite MTRR state with %u variable entries\n",
				num_var);
			num_var = MTRR_MAX_VAR_RANGES;
		}
		for (i = 0; i < num_var; i++)
			mtrr_state.var_ranges[i] = var[i];
		num_var_ranges = num_var;
	}

	mtrr_state.def_type = def_type;
	mtrr_state.enabled |= MTRR_STATE_MTRR_ENABLED;

	mtrr_state_set = 1;
}

static u8 type_merge(u8 type, u8 new_type, u8 *uniform)
{
	u8 effective_type;

	if (type == MTRR_TYPE_INVALID)
		return new_type;

	effective_type = get_effective_type(type, new_type);
	if (type != effective_type)
		*uniform = 0;

	return effective_type;
}

/**
 * mtrr_type_lookup - look up memory type in MTRR
 *
 * @start: Begin of the physical address range
 * @end: End of the physical address range
 * @uniform: output argument:
 *  - 1: the returned MTRR type is valid for the whole region
 *  - 0: otherwise
 *
 * Return Values:
 * MTRR_TYPE_(type)  - The effective MTRR type for the region
 * MTRR_TYPE_INVALID - MTRR is disabled
 */
u8 mtrr_type_lookup(u64 start, u64 end, u8 *uniform)
{
	u8 type = MTRR_TYPE_INVALID;
	unsigned int i;

	if (!mtrr_state_set) {
		/* Uniformity is unknown. */
		*uniform = 0;
		return MTRR_TYPE_UNCACHABLE;
	}

	*uniform = 1;

	if (!(mtrr_state.enabled & MTRR_STATE_MTRR_ENABLED))
		return MTRR_TYPE_UNCACHABLE;

	for (i = 0; i < cache_map_n && start < end; i++) {
		/* Region after current map entry? -> continue with next one. */
		if (start >= cache_map[i].end)
			continue;

		/* Start of region not covered by current map entry? */
		if (start < cache_map[i].start) {
			/* At least some part of region has default type. */
			type = type_merge(type, mtrr_state.def_type, uniform);
			/* End of region not covered, too? -> lookup done. */
			if (end <= cache_map[i].start)
				return type;
		}

		/* At least part of region covered by map entry. */
		type = type_merge(type, cache_map[i].type, uniform);

		start = cache_map[i].end;
	}

	/* End of region past last entry in map? -> use default type. */
	if (start < end)
		type = type_merge(type, mtrr_state.def_type, uniform);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return type;
}

/* Get the MSR pair relating to a var range */
static void
get_mtrr_var_range(unsigned int index, struct mtrr_var_range *vr)
{
	rdmsr(MTRRphysBase_MSR(index), vr->base_lo, vr->base_hi);
	rdmsr(MTRRphysMask_MSR(index), vr->mask_lo, vr->mask_hi);
}

/* Fill the MSR pair relating to a var range */
void fill_mtrr_var_range(unsigned int index,
		u32 base_lo, u32 base_hi, u32 mask_lo, u32 mask_hi)
{
	struct mtrr_var_range *vr;

	vr = mtrr_state.var_ranges;

	vr[index].base_lo = base_lo;
	vr[index].base_hi = base_hi;
	vr[index].mask_lo = mask_lo;
	vr[index].mask_hi = mask_hi;
}

static void get_fixed_ranges(mtrr_type *frs)
{
	unsigned int *p = (unsigned int *)frs;
	int i;

	k8_check_syscfg_dram_mod_en();

	rdmsr(MSR_MTRRfix64K_00000, p[0], p[1]);

	for (i = 0; i < 2; i++)
		rdmsr(MSR_MTRRfix16K_80000 + i, p[2 + i * 2], p[3 + i * 2]);
	for (i = 0; i < 8; i++)
		rdmsr(MSR_MTRRfix4K_C0000 + i, p[6 + i * 2], p[7 + i * 2]);
}

void mtrr_save_fixed_ranges(void *info)
{
<<<<<<< HEAD
	if (cpu_has_mtrr)
=======
	if (boot_cpu_has(X86_FEATURE_MTRR))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		get_fixed_ranges(mtrr_state.fixed_ranges);
}

static unsigned __initdata last_fixed_start;
static unsigned __initdata last_fixed_end;
static mtrr_type __initdata last_fixed_type;

static void __init print_fixed_last(void)
{
	if (!last_fixed_end)
		return;

<<<<<<< HEAD
	pr_debug("  %05X-%05X %s\n", last_fixed_start,
		 last_fixed_end - 1, mtrr_attrib_to_str(last_fixed_type));
=======
	pr_info("  %05X-%05X %s\n", last_fixed_start,
		last_fixed_end - 1, mtrr_attrib_to_str(last_fixed_type));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	last_fixed_end = 0;
}

static void __init update_fixed_last(unsigned base, unsigned end,
				     mtrr_type type)
{
	last_fixed_start = base;
	last_fixed_end = end;
	last_fixed_type = type;
}

static void __init
print_fixed(unsigned base, unsigned step, const mtrr_type *types)
{
	unsigned i;

	for (i = 0; i < 8; ++i, ++types, base += step) {
		if (last_fixed_end == 0) {
			update_fixed_last(base, base + step, *types);
			continue;
		}
		if (last_fixed_end == base && last_fixed_type == *types) {
			last_fixed_end = base + step;
			continue;
		}
		/* new segments: gap or different type */
		print_fixed_last();
		update_fixed_last(base, base + step, *types);
	}
}

<<<<<<< HEAD
static void prepare_set(void);
static void post_set(void);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __init print_mtrr_state(void)
{
	unsigned int i;
	int high_width;

<<<<<<< HEAD
	pr_debug("MTRR default type: %s\n",
		 mtrr_attrib_to_str(mtrr_state.def_type));
	if (mtrr_state.have_fixed) {
		pr_debug("MTRR fixed ranges %sabled:\n",
			 mtrr_state.enabled & 1 ? "en" : "dis");
=======
	pr_info("MTRR default type: %s\n",
		mtrr_attrib_to_str(mtrr_state.def_type));
	if (mtrr_state.have_fixed) {
		pr_info("MTRR fixed ranges %sabled:\n",
			((mtrr_state.enabled & MTRR_STATE_MTRR_ENABLED) &&
			 (mtrr_state.enabled & MTRR_STATE_MTRR_FIXED_ENABLED)) ?
			 "en" : "dis");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		print_fixed(0x00000, 0x10000, mtrr_state.fixed_ranges + 0);
		for (i = 0; i < 2; ++i)
			print_fixed(0x80000 + i * 0x20000, 0x04000,
				    mtrr_state.fixed_ranges + (i + 1) * 8);
		for (i = 0; i < 8; ++i)
			print_fixed(0xC0000 + i * 0x08000, 0x01000,
				    mtrr_state.fixed_ranges + (i + 3) * 8);

		/* tail */
		print_fixed_last();
	}
<<<<<<< HEAD
	pr_debug("MTRR variable ranges %sabled:\n",
		 mtrr_state.enabled & 2 ? "en" : "dis");
	if (size_or_mask & 0xffffffffUL)
		high_width = ffs(size_or_mask & 0xffffffffUL) - 1;
	else
		high_width = ffs(size_or_mask>>32) + 32 - 1;
	high_width = (high_width - (32 - PAGE_SHIFT) + 3) / 4;

	for (i = 0; i < num_var_ranges; ++i) {
		if (mtrr_state.var_ranges[i].mask_lo & (1 << 11))
			pr_debug("  %u base %0*X%05X000 mask %0*X%05X000 %s\n",
				 i,
				 high_width,
				 mtrr_state.var_ranges[i].base_hi,
				 mtrr_state.var_ranges[i].base_lo >> 12,
				 high_width,
				 mtrr_state.var_ranges[i].mask_hi,
				 mtrr_state.var_ranges[i].mask_lo >> 12,
				 mtrr_attrib_to_str(mtrr_state.var_ranges[i].base_lo & 0xff));
		else
			pr_debug("  %u disabled\n", i);
	}
	if (mtrr_tom2)
		pr_debug("TOM2: %016llx aka %lldM\n", mtrr_tom2, mtrr_tom2>>20);
}

/* Grab all of the MTRR state for this CPU into *state */
void __init get_mtrr_state(void)
{
	struct mtrr_var_range *vrs;
	unsigned long flags;
=======
	pr_info("MTRR variable ranges %sabled:\n",
		mtrr_state.enabled & MTRR_STATE_MTRR_ENABLED ? "en" : "dis");
	high_width = (boot_cpu_data.x86_phys_bits - (32 - PAGE_SHIFT) + 3) / 4;

	for (i = 0; i < num_var_ranges; ++i) {
		if (mtrr_state.var_ranges[i].mask_lo & MTRR_PHYSMASK_V)
			pr_info("  %u base %0*X%05X000 mask %0*X%05X000 %s\n",
				i,
				high_width,
				mtrr_state.var_ranges[i].base_hi,
				mtrr_state.var_ranges[i].base_lo >> 12,
				high_width,
				mtrr_state.var_ranges[i].mask_hi,
				mtrr_state.var_ranges[i].mask_lo >> 12,
				mtrr_attrib_to_str(mtrr_state.var_ranges[i].base_lo &
						    MTRR_PHYSBASE_TYPE));
		else
			pr_info("  %u disabled\n", i);
	}
	if (mtrr_tom2)
		pr_info("TOM2: %016llx aka %lldM\n", mtrr_tom2, mtrr_tom2>>20);
}

/* Grab all of the MTRR state for this CPU into *state */
bool __init get_mtrr_state(void)
{
	struct mtrr_var_range *vrs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned lo, dummy;
	unsigned int i;

	vrs = mtrr_state.var_ranges;

	rdmsr(MSR_MTRRcap, lo, dummy);
<<<<<<< HEAD
	mtrr_state.have_fixed = (lo >> 8) & 1;
=======
	mtrr_state.have_fixed = lo & MTRR_CAP_FIX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < num_var_ranges; i++)
		get_mtrr_var_range(i, &vrs[i]);
	if (mtrr_state.have_fixed)
		get_fixed_ranges(mtrr_state.fixed_ranges);

	rdmsr(MSR_MTRRdefType, lo, dummy);
<<<<<<< HEAD
	mtrr_state.def_type = (lo & 0xff);
	mtrr_state.enabled = (lo & 0xc00) >> 10;
=======
	mtrr_state.def_type = lo & MTRR_DEF_TYPE_TYPE;
	mtrr_state.enabled = (lo & MTRR_DEF_TYPE_ENABLE) >> MTRR_STATE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (amd_special_default_mtrr()) {
		unsigned low, high;

		/* TOP_MEM2 */
		rdmsr(MSR_K8_TOP_MEM2, low, high);
		mtrr_tom2 = high;
		mtrr_tom2 <<= 32;
		mtrr_tom2 |= low;
		mtrr_tom2 &= 0xffffff800000ULL;
	}

<<<<<<< HEAD
	print_mtrr_state();

	mtrr_state_set = 1;

	/* PAT setup for BP. We need to go through sync steps here */
	local_irq_save(flags);
	prepare_set();

	pat_init();

	post_set();
	local_irq_restore(flags);
=======
	if (mtrr_debug)
		print_mtrr_state();

	mtrr_state_set = 1;

	return !!(mtrr_state.enabled & MTRR_STATE_MTRR_ENABLED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Some BIOS's are messed up and don't set all MTRRs the same! */
void __init mtrr_state_warn(void)
{
	unsigned long mask = smp_changes_mask;

	if (!mask)
		return;
	if (mask & MTRR_CHANGE_MASK_FIXED)
<<<<<<< HEAD
		pr_warning("mtrr: your CPUs had inconsistent fixed MTRR settings\n");
	if (mask & MTRR_CHANGE_MASK_VARIABLE)
		pr_warning("mtrr: your CPUs had inconsistent variable MTRR settings\n");
	if (mask & MTRR_CHANGE_MASK_DEFTYPE)
		pr_warning("mtrr: your CPUs had inconsistent MTRRdefType settings\n");

	printk(KERN_INFO "mtrr: probably your BIOS does not setup all CPUs.\n");
	printk(KERN_INFO "mtrr: corrected configuration.\n");
=======
		pr_warn("mtrr: your CPUs had inconsistent fixed MTRR settings\n");
	if (mask & MTRR_CHANGE_MASK_VARIABLE)
		pr_warn("mtrr: your CPUs had inconsistent variable MTRR settings\n");
	if (mask & MTRR_CHANGE_MASK_DEFTYPE)
		pr_warn("mtrr: your CPUs had inconsistent MTRRdefType settings\n");

	pr_info("mtrr: probably your BIOS does not setup all CPUs.\n");
	pr_info("mtrr: corrected configuration.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Doesn't attempt to pass an error out to MTRR users
 * because it's quite complicated in some cases and probably not
 * worth it because the best error handling is to ignore it.
 */
void mtrr_wrmsr(unsigned msr, unsigned a, unsigned b)
{
	if (wrmsr_safe(msr, a, b) < 0) {
<<<<<<< HEAD
		printk(KERN_ERR
			"MTRR: CPU %u: Writing MSR %x to %x:%x failed\n",
=======
		pr_err("MTRR: CPU %u: Writing MSR %x to %x:%x failed\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			smp_processor_id(), msr, a, b);
	}
}

/**
 * set_fixed_range - checks & updates a fixed-range MTRR if it
 *		     differs from the value it should have
 * @msr: MSR address of the MTTR which should be checked and updated
 * @changed: pointer which indicates whether the MTRR needed to be changed
 * @msrwords: pointer to the MSR values which the MSR should have
 */
static void set_fixed_range(int msr, bool *changed, unsigned int *msrwords)
{
	unsigned lo, hi;

	rdmsr(msr, lo, hi);

	if (lo != msrwords[0] || hi != msrwords[1]) {
		mtrr_wrmsr(msr, msrwords[0], msrwords[1]);
		*changed = true;
	}
}

/**
 * generic_get_free_region - Get a free MTRR.
 * @base: The starting (base) address of the region.
 * @size: The size (in bytes) of the region.
 * @replace_reg: mtrr index to be replaced; set to invalid value if none.
 *
 * Returns: The index of the region on success, else negative on error.
 */
int
generic_get_free_region(unsigned long base, unsigned long size, int replace_reg)
{
	unsigned long lbase, lsize;
	mtrr_type ltype;
	int i, max;

	max = num_var_ranges;
	if (replace_reg >= 0 && replace_reg < max)
		return replace_reg;

	for (i = 0; i < max; ++i) {
		mtrr_if->get(i, &lbase, &lsize, &ltype);
		if (lsize == 0)
			return i;
	}

	return -ENOSPC;
}

static void generic_get_mtrr(unsigned int reg, unsigned long *base,
			     unsigned long *size, mtrr_type *type)
{
<<<<<<< HEAD
	unsigned int mask_lo, mask_hi, base_lo, base_hi;
	unsigned int tmp, hi;
=======
	u32 mask_lo, mask_hi, base_lo, base_hi;
	unsigned int hi;
	u64 tmp, mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * get_mtrr doesn't need to update mtrr_state, also it could be called
	 * from any cpu, so try to print it out directly.
	 */
	get_cpu();

	rdmsr(MTRRphysMask_MSR(reg), mask_lo, mask_hi);

<<<<<<< HEAD
	if ((mask_lo & 0x800) == 0) {
=======
	if (!(mask_lo & MTRR_PHYSMASK_V)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*  Invalid (i.e. free) range */
		*base = 0;
		*size = 0;
		*type = 0;
		goto out_put_cpu;
	}

	rdmsr(MTRRphysBase_MSR(reg), base_lo, base_hi);

	/* Work out the shifted address mask: */
<<<<<<< HEAD
	tmp = mask_hi << (32 - PAGE_SHIFT) | mask_lo >> PAGE_SHIFT;
	mask_lo = size_or_mask | tmp;

	/* Expand tmp with high bits to all 1s: */
	hi = fls(tmp);
	if (hi > 0) {
		tmp |= ~((1<<(hi - 1)) - 1);

		if (tmp != mask_lo) {
			printk(KERN_WARNING "mtrr: your BIOS has configured an incorrect mask, fixing it.\n");
			add_taint(TAINT_FIRMWARE_WORKAROUND);
			mask_lo = tmp;
=======
	tmp = (u64)mask_hi << 32 | (mask_lo & PAGE_MASK);
	mask = (u64)phys_hi_rsvd << 32 | tmp;

	/* Expand tmp with high bits to all 1s: */
	hi = fls64(tmp);
	if (hi > 0) {
		tmp |= ~((1ULL<<(hi - 1)) - 1);

		if (tmp != mask) {
			pr_warn("mtrr: your BIOS has configured an incorrect mask, fixing it.\n");
			add_taint(TAINT_FIRMWARE_WORKAROUND, LOCKDEP_STILL_OK);
			mask = tmp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
	 * This works correctly if size is a power of two, i.e. a
	 * contiguous range:
	 */
<<<<<<< HEAD
	*size = -mask_lo;
	*base = base_hi << (32 - PAGE_SHIFT) | base_lo >> PAGE_SHIFT;
	*type = base_lo & 0xff;
=======
	*size = -mask >> PAGE_SHIFT;
	*base = (u64)base_hi << (32 - PAGE_SHIFT) | base_lo >> PAGE_SHIFT;
	*type = base_lo & MTRR_PHYSBASE_TYPE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_put_cpu:
	put_cpu();
}

/**
 * set_fixed_ranges - checks & updates the fixed-range MTRRs if they
 *		      differ from the saved set
 * @frs: pointer to fixed-range MTRR values, saved by get_fixed_ranges()
 */
static int set_fixed_ranges(mtrr_type *frs)
{
	unsigned long long *saved = (unsigned long long *)frs;
	bool changed = false;
	int block = -1, range;

	k8_check_syscfg_dram_mod_en();

	while (fixed_range_blocks[++block].ranges) {
		for (range = 0; range < fixed_range_blocks[block].ranges; range++)
			set_fixed_range(fixed_range_blocks[block].base_msr + range,
					&changed, (unsigned int *)saved++);
	}

	return changed;
}

/*
 * Set the MSR pair relating to a var range.
 * Returns true if changes are made.
 */
static bool set_mtrr_var_ranges(unsigned int index, struct mtrr_var_range *vr)
{
	unsigned int lo, hi;
	bool changed = false;

	rdmsr(MTRRphysBase_MSR(index), lo, hi);
<<<<<<< HEAD
	if ((vr->base_lo & 0xfffff0ffUL) != (lo & 0xfffff0ffUL)
	    || (vr->base_hi & (size_and_mask >> (32 - PAGE_SHIFT))) !=
		(hi & (size_and_mask >> (32 - PAGE_SHIFT)))) {
=======
	if ((vr->base_lo & ~MTRR_PHYSBASE_RSVD) != (lo & ~MTRR_PHYSBASE_RSVD)
	    || (vr->base_hi & ~phys_hi_rsvd) != (hi & ~phys_hi_rsvd)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		mtrr_wrmsr(MTRRphysBase_MSR(index), vr->base_lo, vr->base_hi);
		changed = true;
	}

	rdmsr(MTRRphysMask_MSR(index), lo, hi);

<<<<<<< HEAD
	if ((vr->mask_lo & 0xfffff800UL) != (lo & 0xfffff800UL)
	    || (vr->mask_hi & (size_and_mask >> (32 - PAGE_SHIFT))) !=
		(hi & (size_and_mask >> (32 - PAGE_SHIFT)))) {
=======
	if ((vr->mask_lo & ~MTRR_PHYSMASK_RSVD) != (lo & ~MTRR_PHYSMASK_RSVD)
	    || (vr->mask_hi & ~phys_hi_rsvd) != (hi & ~phys_hi_rsvd)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mtrr_wrmsr(MTRRphysMask_MSR(index), vr->mask_lo, vr->mask_hi);
		changed = true;
	}
	return changed;
}

static u32 deftype_lo, deftype_hi;

/**
 * set_mtrr_state - Set the MTRR state for this CPU.
 *
<<<<<<< HEAD
 * NOTE: The CPU must already be in a safe state for MTRR changes.
=======
 * NOTE: The CPU must already be in a safe state for MTRR changes, including
 *       measures that only a single CPU can be active in set_mtrr_state() in
 *       order to not be subject to races for usage of deftype_lo. This is
 *       accomplished by taking cache_disable_lock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * RETURNS: 0 if no changes made, else a mask indicating what was changed.
 */
static unsigned long set_mtrr_state(void)
{
	unsigned long change_mask = 0;
	unsigned int i;

	for (i = 0; i < num_var_ranges; i++) {
		if (set_mtrr_var_ranges(i, &mtrr_state.var_ranges[i]))
			change_mask |= MTRR_CHANGE_MASK_VARIABLE;
	}

	if (mtrr_state.have_fixed && set_fixed_ranges(mtrr_state.fixed_ranges))
		change_mask |= MTRR_CHANGE_MASK_FIXED;

	/*
	 * Set_mtrr_restore restores the old value of MTRRdefType,
	 * so to set it we fiddle with the saved value:
	 */
<<<<<<< HEAD
	if ((deftype_lo & 0xff) != mtrr_state.def_type
	    || ((deftype_lo & 0xc00) >> 10) != mtrr_state.enabled) {

		deftype_lo = (deftype_lo & ~0xcff) | mtrr_state.def_type |
			     (mtrr_state.enabled << 10);
=======
	if ((deftype_lo & MTRR_DEF_TYPE_TYPE) != mtrr_state.def_type ||
	    ((deftype_lo & MTRR_DEF_TYPE_ENABLE) >> MTRR_STATE_SHIFT) != mtrr_state.enabled) {

		deftype_lo = (deftype_lo & MTRR_DEF_TYPE_DISABLE) |
			     mtrr_state.def_type |
			     (mtrr_state.enabled << MTRR_STATE_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		change_mask |= MTRR_CHANGE_MASK_DEFTYPE;
	}

	return change_mask;
}

<<<<<<< HEAD

static unsigned long cr4;
static DEFINE_RAW_SPINLOCK(set_atomicity_lock);

/*
 * Since we are disabling the cache don't allow any interrupts,
 * they would run extremely slow and would only increase the pain.
 *
 * The caller must ensure that local interrupts are disabled and
 * are reenabled after post_set() has been called.
 */
static void prepare_set(void) __acquires(set_atomicity_lock)
{
	unsigned long cr0;

	/*
	 * Note that this is not ideal
	 * since the cache is only flushed/disabled for this CPU while the
	 * MTRRs are changed, but changing this requires more invasive
	 * changes to the way the kernel boots
	 */

	raw_spin_lock(&set_atomicity_lock);

	/* Enter the no-fill (CD=1, NW=0) cache mode and flush caches. */
	cr0 = read_cr0() | X86_CR0_CD;
	write_cr0(cr0);
	wbinvd();

	/* Save value of CR4 and clear Page Global Enable (bit 7) */
	if (cpu_has_pge) {
		cr4 = read_cr4();
		write_cr4(cr4 & ~X86_CR4_PGE);
	}

	/* Flush all TLBs via a mov %cr3, %reg; mov %reg, %cr3 */
	__flush_tlb();

=======
void mtrr_disable(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Save MTRR state */
	rdmsr(MSR_MTRRdefType, deftype_lo, deftype_hi);

	/* Disable MTRRs, and set the default type to uncached */
<<<<<<< HEAD
	mtrr_wrmsr(MSR_MTRRdefType, deftype_lo & ~0xcff, deftype_hi);
	wbinvd();
}

static void post_set(void) __releases(set_atomicity_lock)
{
	/* Flush TLBs (no need to flush caches - they are disabled) */
	__flush_tlb();

	/* Intel (P6) standard MTRRs */
	mtrr_wrmsr(MSR_MTRRdefType, deftype_lo, deftype_hi);

	/* Enable caches */
	write_cr0(read_cr0() & 0xbfffffff);

	/* Restore value of CR4 */
	if (cpu_has_pge)
		write_cr4(cr4);
	raw_spin_unlock(&set_atomicity_lock);
}

static void generic_set_all(void)
{
	unsigned long mask, count;
	unsigned long flags;

	local_irq_save(flags);
	prepare_set();
=======
	mtrr_wrmsr(MSR_MTRRdefType, deftype_lo & MTRR_DEF_TYPE_DISABLE, deftype_hi);
}

void mtrr_enable(void)
{
	/* Intel (P6) standard MTRRs */
	mtrr_wrmsr(MSR_MTRRdefType, deftype_lo, deftype_hi);
}

void mtrr_generic_set_state(void)
{
	unsigned long mask, count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Actually set the state */
	mask = set_mtrr_state();

<<<<<<< HEAD
	/* also set PAT */
	pat_init();

	post_set();
	local_irq_restore(flags);

	/* Use the atomic bitops to update the global mask */
	for (count = 0; count < sizeof mask * 8; ++count) {
=======
	/* Use the atomic bitops to update the global mask */
	for (count = 0; count < sizeof(mask) * 8; ++count) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (mask & 0x01)
			set_bit(count, &smp_changes_mask);
		mask >>= 1;
	}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * generic_set_mtrr - set variable MTRR register on the local CPU.
 *
 * @reg: The register to set.
 * @base: The base address of the region.
 * @size: The size of the region. If this is 0 the region is disabled.
 * @type: The type of the region.
 *
 * Returns nothing.
 */
static void generic_set_mtrr(unsigned int reg, unsigned long base,
			     unsigned long size, mtrr_type type)
{
	unsigned long flags;
	struct mtrr_var_range *vr;

	vr = &mtrr_state.var_ranges[reg];

	local_irq_save(flags);
<<<<<<< HEAD
	prepare_set();
=======
	cache_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (size == 0) {
		/*
		 * The invalid bit is kept in the mask, so we simply
		 * clear the relevant mask register to disable a range.
		 */
		mtrr_wrmsr(MTRRphysMask_MSR(reg), 0, 0);
		memset(vr, 0, sizeof(struct mtrr_var_range));
	} else {
		vr->base_lo = base << PAGE_SHIFT | type;
<<<<<<< HEAD
		vr->base_hi = (base & size_and_mask) >> (32 - PAGE_SHIFT);
		vr->mask_lo = -size << PAGE_SHIFT | 0x800;
		vr->mask_hi = (-size & size_and_mask) >> (32 - PAGE_SHIFT);
=======
		vr->base_hi = (base >> (32 - PAGE_SHIFT)) & ~phys_hi_rsvd;
		vr->mask_lo = -size << PAGE_SHIFT | MTRR_PHYSMASK_V;
		vr->mask_hi = (-size >> (32 - PAGE_SHIFT)) & ~phys_hi_rsvd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		mtrr_wrmsr(MTRRphysBase_MSR(reg), vr->base_lo, vr->base_hi);
		mtrr_wrmsr(MTRRphysMask_MSR(reg), vr->mask_lo, vr->mask_hi);
	}

<<<<<<< HEAD
	post_set();
=======
	cache_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	local_irq_restore(flags);
}

int generic_validate_add_page(unsigned long base, unsigned long size,
			      unsigned int type)
{
	unsigned long lbase, last;

	/*
	 * For Intel PPro stepping <= 7
	 * must be 4 MiB aligned and not touch 0x70000000 -> 0x7003FFFF
	 */
<<<<<<< HEAD
	if (is_cpu(INTEL) && boot_cpu_data.x86 == 6 &&
	    boot_cpu_data.x86_model == 1 &&
	    boot_cpu_data.x86_mask <= 7) {
		if (base & ((1 << (22 - PAGE_SHIFT)) - 1)) {
			pr_warning("mtrr: base(0x%lx000) is not 4 MiB aligned\n", base);
=======
	if (mtrr_if == &generic_mtrr_ops && boot_cpu_data.x86 == 6 &&
	    boot_cpu_data.x86_model == 1 &&
	    boot_cpu_data.x86_stepping <= 7) {
		if (base & ((1 << (22 - PAGE_SHIFT)) - 1)) {
			pr_warn("mtrr: base(0x%lx000) is not 4 MiB aligned\n", base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		if (!(base + size < 0x70000 || base > 0x7003F) &&
		    (type == MTRR_TYPE_WRCOMB
		     || type == MTRR_TYPE_WRBACK)) {
<<<<<<< HEAD
			pr_warning("mtrr: writable mtrr between 0x70000000 and 0x7003FFFF may hang the CPU.\n");
=======
			pr_warn("mtrr: writable mtrr between 0x70000000 and 0x7003FFFF may hang the CPU.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
	}

	/*
	 * Check upper bits of base and last are equal and lower bits are 0
	 * for base and 1 for last
	 */
	last = base + size - 1;
	for (lbase = base; !(lbase & 1) && (last & 1);
	     lbase = lbase >> 1, last = last >> 1)
		;
	if (lbase != last) {
<<<<<<< HEAD
		pr_warning("mtrr: base(0x%lx000) is not aligned on a size(0x%lx000) boundary\n", base, size);
=======
		pr_warn("mtrr: base(0x%lx000) is not aligned on a size(0x%lx000) boundary\n", base, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}
	return 0;
}

static int generic_have_wrcomb(void)
{
	unsigned long config, dummy;
	rdmsr(MSR_MTRRcap, config, dummy);
<<<<<<< HEAD
	return config & (1 << 10);
=======
	return config & MTRR_CAP_WC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int positive_have_wrcomb(void)
{
	return 1;
}

/*
 * Generic structure...
 */
const struct mtrr_ops generic_mtrr_ops = {
<<<<<<< HEAD
	.use_intel_if		= 1,
	.set_all		= generic_set_all,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get			= generic_get_mtrr,
	.get_free_region	= generic_get_free_region,
	.set			= generic_set_mtrr,
	.validate_add_page	= generic_validate_add_page,
	.have_wrcomb		= generic_have_wrcomb,
};
