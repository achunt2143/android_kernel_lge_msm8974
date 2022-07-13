<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * local MTRR defines.
 */

#include <linux/types.h>
#include <linux/stddef.h>

#define MTRR_CHANGE_MASK_FIXED     0x01
#define MTRR_CHANGE_MASK_VARIABLE  0x02
#define MTRR_CHANGE_MASK_DEFTYPE   0x04

<<<<<<< HEAD
extern unsigned int mtrr_usage_table[MTRR_MAX_VAR_RANGES];

struct mtrr_ops {
	u32	vendor;
	u32	use_intel_if;
	void	(*set)(unsigned int reg, unsigned long base,
		       unsigned long size, mtrr_type type);
	void	(*set_all)(void);

=======
extern bool mtrr_debug;
#define Dprintk(x...) do { if (mtrr_debug) pr_info(x); } while (0)

extern unsigned int mtrr_usage_table[MTRR_MAX_VAR_RANGES];

struct mtrr_ops {
	u32	var_regs;
	void	(*set)(unsigned int reg, unsigned long base,
		       unsigned long size, mtrr_type type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void	(*get)(unsigned int reg, unsigned long *base,
		       unsigned long *size, mtrr_type *type);
	int	(*get_free_region)(unsigned long base, unsigned long size,
				   int replace_reg);
	int	(*validate_add_page)(unsigned long base, unsigned long size,
				     unsigned int type);
	int	(*have_wrcomb)(void);
};

extern int generic_get_free_region(unsigned long base, unsigned long size,
				   int replace_reg);
extern int generic_validate_add_page(unsigned long base, unsigned long size,
				     unsigned int type);

extern const struct mtrr_ops generic_mtrr_ops;

extern int positive_have_wrcomb(void);

/* library functions for processor-specific routines */
struct set_mtrr_context {
	unsigned long	flags;
	unsigned long	cr4val;
	u32		deftype_lo;
	u32		deftype_hi;
	u32		ccr3;
};

void set_mtrr_done(struct set_mtrr_context *ctxt);
void set_mtrr_cache_disable(struct set_mtrr_context *ctxt);
void set_mtrr_prepare_save(struct set_mtrr_context *ctxt);

void fill_mtrr_var_range(unsigned int index,
		u32 base_lo, u32 base_hi, u32 mask_lo, u32 mask_hi);
<<<<<<< HEAD
void get_mtrr_state(void);

extern void set_mtrr_ops(const struct mtrr_ops *ops);

extern u64 size_or_mask, size_and_mask;
extern const struct mtrr_ops *mtrr_if;

#define is_cpu(vnd)	(mtrr_if && mtrr_if->vendor == X86_VENDOR_##vnd)
#define use_intel()	(mtrr_if && mtrr_if->use_intel_if == 1)
=======
bool get_mtrr_state(void);

extern const struct mtrr_ops *mtrr_if;
extern struct mutex mtrr_mutex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern unsigned int num_var_ranges;
extern u64 mtrr_tom2;
extern struct mtrr_state_type mtrr_state;
<<<<<<< HEAD
=======
extern u32 phys_hi_rsvd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void mtrr_state_warn(void);
const char *mtrr_attrib_to_str(int x);
void mtrr_wrmsr(unsigned, unsigned, unsigned);
<<<<<<< HEAD

/* CPU specific mtrr init functions */
int amd_init_mtrr(void);
int cyrix_init_mtrr(void);
int centaur_init_mtrr(void);

extern int changed_by_mtrr_cleanup;
extern int mtrr_cleanup(unsigned address_bits);
=======
#ifdef CONFIG_X86_32
void mtrr_set_if(void);
void mtrr_register_syscore(void);
#else
static inline void mtrr_set_if(void) { }
static inline void mtrr_register_syscore(void) { }
#endif
void mtrr_build_map(void);
void mtrr_copy_map(void);

/* CPU specific mtrr_ops vectors. */
extern const struct mtrr_ops amd_mtrr_ops;
extern const struct mtrr_ops cyrix_mtrr_ops;
extern const struct mtrr_ops centaur_mtrr_ops;

extern int changed_by_mtrr_cleanup;
extern int mtrr_cleanup(void);

/*
 * Must be used by code which uses mtrr_if to call platform-specific
 * MTRR manipulation functions.
 */
static inline bool mtrr_enabled(void)
{
	return !!mtrr_if;
}
void generic_rebuild_map(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
