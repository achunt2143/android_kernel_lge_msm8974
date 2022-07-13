<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_MICROCODE_H
#define _ASM_X86_MICROCODE_H

struct cpu_signature {
	unsigned int sig;
	unsigned int pf;
	unsigned int rev;
};

<<<<<<< HEAD
struct device;

enum ucode_state { UCODE_ERROR, UCODE_OK, UCODE_NFOUND };

struct microcode_ops {
	enum ucode_state (*request_microcode_user) (int cpu,
				const void __user *buf, size_t size);

	enum ucode_state (*request_microcode_fw) (int cpu,
				struct device *device);

	void (*microcode_fini_cpu) (int cpu);

	/*
	 * The generic 'microcode_core' part guarantees that
	 * the callbacks below run on a target cpu when they
	 * are being called.
	 * See also the "Synchronization" section in microcode_core.c.
	 */
	int (*apply_microcode) (int cpu);
	int (*collect_cpu_info) (int cpu, struct cpu_signature *csig);
};

struct ucode_cpu_info {
	struct cpu_signature	cpu_sig;
	int			valid;
	void			*mc;
};
extern struct ucode_cpu_info ucode_cpu_info[];

#ifdef CONFIG_MICROCODE_INTEL
extern struct microcode_ops * __init init_intel_microcode(void);
#else
static inline struct microcode_ops * __init init_intel_microcode(void)
{
	return NULL;
}
#endif /* CONFIG_MICROCODE_INTEL */

#ifdef CONFIG_MICROCODE_AMD
extern struct microcode_ops * __init init_amd_microcode(void);
extern void __exit exit_amd_microcode(void);

static inline void get_ucode_data(void *to, const u8 *from, size_t n)
{
	memcpy(to, from, n);
}

#else
static inline struct microcode_ops * __init init_amd_microcode(void)
{
	return NULL;
}
static inline void __exit exit_amd_microcode(void) {}
=======
struct ucode_cpu_info {
	struct cpu_signature	cpu_sig;
	void			*mc;
};

#ifdef CONFIG_MICROCODE
void load_ucode_bsp(void);
void load_ucode_ap(void);
void microcode_bsp_resume(void);
#else
static inline void load_ucode_bsp(void)	{ }
static inline void load_ucode_ap(void) { }
static inline void microcode_bsp_resume(void) { }
#endif

extern unsigned long initrd_start_early;

#ifdef CONFIG_CPU_SUP_INTEL
/* Intel specific microcode defines. Public for IFS */
struct microcode_header_intel {
	unsigned int	hdrver;
	unsigned int	rev;
	unsigned int	date;
	unsigned int	sig;
	unsigned int	cksum;
	unsigned int	ldrver;
	unsigned int	pf;
	unsigned int	datasize;
	unsigned int	totalsize;
	unsigned int	metasize;
	unsigned int	min_req_ver;
	unsigned int	reserved;
};

struct microcode_intel {
	struct microcode_header_intel	hdr;
	unsigned int			bits[];
};

#define DEFAULT_UCODE_DATASIZE		(2000)
#define MC_HEADER_SIZE			(sizeof(struct microcode_header_intel))
#define MC_HEADER_TYPE_MICROCODE	1
#define MC_HEADER_TYPE_IFS		2

static inline int intel_microcode_get_datasize(struct microcode_header_intel *hdr)
{
	return hdr->datasize ? : DEFAULT_UCODE_DATASIZE;
}

static inline u32 intel_get_microcode_revision(void)
{
	u32 rev, dummy;

	native_wrmsrl(MSR_IA32_UCODE_REV, 0);

	/* As documented in the SDM: Do a CPUID 1 here */
	native_cpuid_eax(1);

	/* get the current revision from MSR 0x8B */
	native_rdmsr(MSR_IA32_UCODE_REV, dummy, rev);

	return rev;
}
#endif /* !CONFIG_CPU_SUP_INTEL */

bool microcode_nmi_handler(void);
void microcode_offline_nmi_handler(void);

#ifdef CONFIG_MICROCODE_LATE_LOADING
DECLARE_STATIC_KEY_FALSE(microcode_nmi_handler_enable);
static __always_inline bool microcode_nmi_handler_enabled(void)
{
	return static_branch_unlikely(&microcode_nmi_handler_enable);
}
#else
static __always_inline bool microcode_nmi_handler_enabled(void) { return false; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* _ASM_X86_MICROCODE_H */
