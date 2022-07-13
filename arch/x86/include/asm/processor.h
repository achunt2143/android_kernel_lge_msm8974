<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_PROCESSOR_H
#define _ASM_X86_PROCESSOR_H

#include <asm/processor-flags.h>

/* Forward declaration, a strange C thing */
struct task_struct;
struct mm_struct;
<<<<<<< HEAD

#include <asm/vm86.h>
#include <asm/math_emu.h>
#include <asm/segment.h>
#include <asm/types.h>
#include <asm/sigcontext.h>
#include <asm/current.h>
#include <asm/cpufeature.h>
#include <asm/page.h>
#include <asm/pgtable_types.h>
#include <asm/percpu.h>
#include <asm/msr.h>
#include <asm/desc_defs.h>
#include <asm/nops.h>
#include <asm/special_insns.h>

#include <linux/personality.h>
#include <linux/cpumask.h>
#include <linux/cache.h>
#include <linux/threads.h>
#include <linux/math64.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/irqflags.h>
=======
struct io_bitmap;
struct vm86;

#include <asm/math_emu.h>
#include <asm/segment.h>
#include <asm/types.h>
#include <uapi/asm/sigcontext.h>
#include <asm/current.h>
#include <asm/cpufeatures.h>
#include <asm/cpuid.h>
#include <asm/page.h>
#include <asm/pgtable_types.h>
#include <asm/percpu.h>
#include <asm/desc_defs.h>
#include <asm/nops.h>
#include <asm/special_insns.h>
#include <asm/fpu/types.h>
#include <asm/unwind_hints.h>
#include <asm/vmxfeatures.h>
#include <asm/vdso/processor.h>
#include <asm/shstk.h>

#include <linux/personality.h>
#include <linux/cache.h>
#include <linux/threads.h>
#include <linux/math64.h>
#include <linux/err.h>
#include <linux/irqflags.h>
#include <linux/mem_encrypt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * We handle most unaligned accesses in hardware.  On the other hand
 * unaligned DMA can be quite expensive on some Nehalem processors.
 *
 * Based on this we disable the IP header alignment in network drivers.
 */
#define NET_IP_ALIGN	0

#define HBP_NUM 4
<<<<<<< HEAD
/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
static inline void *current_text_addr(void)
{
	void *pc;

	asm volatile("mov $1f, %0; 1:":"=r" (pc));

	return pc;
}

=======

/*
 * These alignment constraints are for performance in the vSMP case,
 * but in the task_struct case we must also meet hardware imposed
 * alignment requirements of the FPU state:
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_VSMP
# define ARCH_MIN_TASKALIGN		(1 << INTERNODE_CACHE_SHIFT)
# define ARCH_MIN_MMSTRUCT_ALIGN	(1 << INTERNODE_CACHE_SHIFT)
#else
<<<<<<< HEAD
# define ARCH_MIN_TASKALIGN		16
# define ARCH_MIN_MMSTRUCT_ALIGN	0
#endif

/*
 *  CPU type and hardware bug flags. Kept separately for each CPU.
 *  Members of this structure are referenced in head.S, so think twice
 *  before touching them. [mj]
 */

=======
# define ARCH_MIN_TASKALIGN		__alignof__(union fpregs_state)
# define ARCH_MIN_MMSTRUCT_ALIGN	0
#endif

enum tlb_infos {
	ENTRIES,
	NR_INFO
};

extern u16 __read_mostly tlb_lli_4k[NR_INFO];
extern u16 __read_mostly tlb_lli_2m[NR_INFO];
extern u16 __read_mostly tlb_lli_4m[NR_INFO];
extern u16 __read_mostly tlb_lld_4k[NR_INFO];
extern u16 __read_mostly tlb_lld_2m[NR_INFO];
extern u16 __read_mostly tlb_lld_4m[NR_INFO];
extern u16 __read_mostly tlb_lld_1g[NR_INFO];

/*
 * CPU type and hardware bug flags. Kept separately for each CPU.
 */

struct cpuinfo_topology {
	// Real APIC ID read from the local APIC
	u32			apicid;
	// The initial APIC ID provided by CPUID
	u32			initial_apicid;

	// Physical package ID
	u32			pkg_id;

	// Physical die ID on AMD, Relative on Intel
	u32			die_id;

	// Compute unit ID - AMD specific
	u32			cu_id;

	// Core ID relative to the package
	u32			core_id;

	// Logical ID mappings
	u32			logical_pkg_id;
	u32			logical_die_id;

	// AMD Node ID and Nodes per Package info
	u32			amd_node_id;

	// Cache level topology IDs
	u32			llc_id;
	u32			l2c_id;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct cpuinfo_x86 {
	__u8			x86;		/* CPU family */
	__u8			x86_vendor;	/* CPU vendor */
	__u8			x86_model;
<<<<<<< HEAD
	__u8			x86_mask;
#ifdef CONFIG_X86_32
	char			wp_works_ok;	/* It doesn't on 386's */

	/* Problems on some 486Dx4's and old 386's: */
	char			hlt_works_ok;
	char			hard_math;
	char			rfu;
	char			fdiv_bug;
	char			f00f_bug;
	char			coma_bug;
	char			pad0;
#else
	/* Number of 4K pages in DTLB/ITLB combined(in pages): */
	int			x86_tlbsize;
#endif
	__u8			x86_virt_bits;
	__u8			x86_phys_bits;
	/* CPUID returned core id bits: */
	__u8			x86_coreid_bits;
=======
	__u8			x86_stepping;
#ifdef CONFIG_X86_64
	/* Number of 4K pages in DTLB/ITLB combined(in pages): */
	int			x86_tlbsize;
#endif
#ifdef CONFIG_X86_VMX_FEATURE_NAMES
	__u32			vmx_capability[NVMXINTS];
#endif
	__u8			x86_virt_bits;
	__u8			x86_phys_bits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Max extended CPUID function supported: */
	__u32			extended_cpuid_level;
	/* Maximum supported CPUID level, -1=no CPUID: */
	int			cpuid_level;
<<<<<<< HEAD
	__u32			x86_capability[NCAPINTS];
	char			x86_vendor_id[16];
	char			x86_model_id[64];
	/* in KB - valid for CPUS which support this call: */
	int			x86_cache_size;
	int			x86_cache_alignment;	/* In bytes */
	int			x86_power;
	unsigned long		loops_per_jiffy;
	/* cpuid returned max cores value: */
	u16			 x86_max_cores;
	u16			apicid;
	u16			initial_apicid;
	u16			x86_clflush_size;
	/* number of cores as seen by the OS: */
	u16			booted_cores;
	/* Physical processor id: */
	u16			phys_proc_id;
	/* Core id: */
	u16			cpu_core_id;
	/* Compute unit id */
	u8			compute_unit_id;
	/* Index into per_cpu list: */
	u16			cpu_index;
	u32			microcode;
} __attribute__((__aligned__(SMP_CACHE_BYTES)));
=======
	/*
	 * Align to size of unsigned long because the x86_capability array
	 * is passed to bitops which require the alignment. Use unnamed
	 * union to enforce the array is aligned to size of unsigned long.
	 */
	union {
		__u32		x86_capability[NCAPINTS + NBUGINTS];
		unsigned long	x86_capability_alignment;
	};
	char			x86_vendor_id[16];
	char			x86_model_id[64];
	struct cpuinfo_topology	topo;
	/* in KB - valid for CPUS which support this call: */
	unsigned int		x86_cache_size;
	int			x86_cache_alignment;	/* In bytes */
	/* Cache QoS architectural values, valid only on the BSP: */
	int			x86_cache_max_rmid;	/* max index */
	int			x86_cache_occ_scale;	/* scale to bytes */
	int			x86_cache_mbm_width_offset;
	int			x86_power;
	unsigned long		loops_per_jiffy;
	/* protected processor identification number */
	u64			ppin;
	u16			x86_clflush_size;
	/* number of cores as seen by the OS: */
	u16			booted_cores;
	/* Index into per_cpu list: */
	u16			cpu_index;
	/*  Is SMT active on this core? */
	bool			smt_active;
	u32			microcode;
	/* Address space bits used by the cache internally */
	u8			x86_cache_bits;
	unsigned		initialized : 1;
} __randomize_layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define X86_VENDOR_INTEL	0
#define X86_VENDOR_CYRIX	1
#define X86_VENDOR_AMD		2
#define X86_VENDOR_UMC		3
#define X86_VENDOR_CENTAUR	5
#define X86_VENDOR_TRANSMETA	7
#define X86_VENDOR_NSC		8
<<<<<<< HEAD
#define X86_VENDOR_NUM		9
=======
#define X86_VENDOR_HYGON	9
#define X86_VENDOR_ZHAOXIN	10
#define X86_VENDOR_VORTEX	11
#define X86_VENDOR_NUM		12
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define X86_VENDOR_UNKNOWN	0xff

/*
 * capabilities of CPUs
 */
extern struct cpuinfo_x86	boot_cpu_data;
extern struct cpuinfo_x86	new_cpu_data;

<<<<<<< HEAD
extern struct tss_struct	doublefault_tss;
extern __u32			cpu_caps_cleared[NCAPINTS];
extern __u32			cpu_caps_set[NCAPINTS];

#ifdef CONFIG_SMP
DECLARE_PER_CPU_SHARED_ALIGNED(struct cpuinfo_x86, cpu_info);
#define cpu_data(cpu)		per_cpu(cpu_info, cpu)
#else
#define cpu_info		boot_cpu_data
#define cpu_data(cpu)		boot_cpu_data
#endif

extern const struct seq_operations cpuinfo_op;

static inline int hlt_works(int cpu)
{
#ifdef CONFIG_X86_32
	return cpu_data(cpu).hlt_works_ok;
#else
	return 1;
#endif
}

=======
extern __u32			cpu_caps_cleared[NCAPINTS + NBUGINTS];
extern __u32			cpu_caps_set[NCAPINTS + NBUGINTS];

DECLARE_PER_CPU_READ_MOSTLY(struct cpuinfo_x86, cpu_info);
#define cpu_data(cpu)		per_cpu(cpu_info, cpu)

extern const struct seq_operations cpuinfo_op;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define cache_line_size()	(boot_cpu_data.x86_cache_alignment)

extern void cpu_detect(struct cpuinfo_x86 *c);

<<<<<<< HEAD
extern struct pt_regs *idle_regs(struct pt_regs *);

extern void early_cpu_init(void);
extern void identify_boot_cpu(void);
extern void identify_secondary_cpu(struct cpuinfo_x86 *);
extern void print_cpu_info(struct cpuinfo_x86 *);
void print_cpu_msr(struct cpuinfo_x86 *);
extern void init_scattered_cpuid_features(struct cpuinfo_x86 *c);
extern unsigned int init_intel_cacheinfo(struct cpuinfo_x86 *c);
extern unsigned short num_cache_leaves;

extern void detect_extended_topology(struct cpuinfo_x86 *c);
extern void detect_ht(struct cpuinfo_x86 *c);

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx)
{
	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (*eax),
	      "=b" (*ebx),
	      "=c" (*ecx),
	      "=d" (*edx)
	    : "0" (*eax), "2" (*ecx)
	    : "memory");
=======
static inline unsigned long long l1tf_pfn_limit(void)
{
	return BIT_ULL(boot_cpu_data.x86_cache_bits - 1 - PAGE_SHIFT);
}

extern void early_cpu_init(void);
extern void identify_secondary_cpu(struct cpuinfo_x86 *);
extern void print_cpu_info(struct cpuinfo_x86 *);
void print_cpu_msr(struct cpuinfo_x86 *);

/*
 * Friendlier CR3 helpers.
 */
static inline unsigned long read_cr3_pa(void)
{
	return __read_cr3() & CR3_ADDR_MASK;
}

static inline unsigned long native_read_cr3_pa(void)
{
	return __native_read_cr3() & CR3_ADDR_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void load_cr3(pgd_t *pgdir)
{
<<<<<<< HEAD
	write_cr3(__pa(pgdir));
}

=======
	write_cr3(__sme_pa(pgdir));
}

/*
 * Note that while the legacy 'TSS' name comes from 'Task State Segment',
 * on modern x86 CPUs the TSS also holds information important to 64-bit mode,
 * unrelated to the task-switch mechanism:
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_32
/* This is the TSS defined by the hardware. */
struct x86_hw_tss {
	unsigned short		back_link, __blh;
	unsigned long		sp0;
	unsigned short		ss0, __ss0h;
	unsigned long		sp1;
<<<<<<< HEAD
	/* ss1 caches MSR_IA32_SYSENTER_CS: */
	unsigned short		ss1, __ss1h;
=======

	/*
	 * We don't use ring 1, so ss1 is a convenient scratch space in
	 * the same cacheline as sp0.  We use ss1 to cache the value in
	 * MSR_IA32_SYSENTER_CS.  When we context switch
	 * MSR_IA32_SYSENTER_CS, we first check if the new value being
	 * written matches ss1, and, if it's not, then we wrmsr the new
	 * value and update ss1.
	 *
	 * The only reason we context switch MSR_IA32_SYSENTER_CS is
	 * that we set it to zero in vm86 tasks to avoid corrupting the
	 * stack if we were to go through the sysenter path from vm86
	 * mode.
	 */
	unsigned short		ss1;	/* MSR_IA32_SYSENTER_CS */

	unsigned short		__ss1h;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		sp2;
	unsigned short		ss2, __ss2h;
	unsigned long		__cr3;
	unsigned long		ip;
	unsigned long		flags;
	unsigned long		ax;
	unsigned long		cx;
	unsigned long		dx;
	unsigned long		bx;
	unsigned long		sp;
	unsigned long		bp;
	unsigned long		si;
	unsigned long		di;
	unsigned short		es, __esh;
	unsigned short		cs, __csh;
	unsigned short		ss, __ssh;
	unsigned short		ds, __dsh;
	unsigned short		fs, __fsh;
	unsigned short		gs, __gsh;
	unsigned short		ldt, __ldth;
	unsigned short		trace;
	unsigned short		io_bitmap_base;

} __attribute__((packed));
#else
struct x86_hw_tss {
	u32			reserved1;
	u64			sp0;
	u64			sp1;
<<<<<<< HEAD
	u64			sp2;
=======

	/*
	 * Since Linux does not use ring 2, the 'sp2' slot is unused by
	 * hardware.  entry_SYSCALL_64 uses it as scratch space to stash
	 * the user RSP value.
	 */
	u64			sp2;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64			reserved2;
	u64			ist[7];
	u32			reserved3;
	u32			reserved4;
	u16			reserved5;
	u16			io_bitmap_base;

<<<<<<< HEAD
} __attribute__((packed)) ____cacheline_aligned;
=======
} __attribute__((packed));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * IO-bitmap sizes:
 */
#define IO_BITMAP_BITS			65536
<<<<<<< HEAD
#define IO_BITMAP_BYTES			(IO_BITMAP_BITS/8)
#define IO_BITMAP_LONGS			(IO_BITMAP_BYTES/sizeof(long))
#define IO_BITMAP_OFFSET		offsetof(struct tss_struct, io_bitmap)
#define INVALID_IO_BITMAP_OFFSET	0x8000

struct tss_struct {
	/*
	 * The hardware state:
	 */
	struct x86_hw_tss	x86_tss;
=======
#define IO_BITMAP_BYTES			(IO_BITMAP_BITS / BITS_PER_BYTE)
#define IO_BITMAP_LONGS			(IO_BITMAP_BYTES / sizeof(long))

#define IO_BITMAP_OFFSET_VALID_MAP				\
	(offsetof(struct tss_struct, io_bitmap.bitmap) -	\
	 offsetof(struct tss_struct, x86_tss))

#define IO_BITMAP_OFFSET_VALID_ALL				\
	(offsetof(struct tss_struct, io_bitmap.mapall) -	\
	 offsetof(struct tss_struct, x86_tss))

#ifdef CONFIG_X86_IOPL_IOPERM
/*
 * sizeof(unsigned long) coming from an extra "long" at the end of the
 * iobitmap. The limit is inclusive, i.e. the last valid byte.
 */
# define __KERNEL_TSS_LIMIT	\
	(IO_BITMAP_OFFSET_VALID_ALL + IO_BITMAP_BYTES + \
	 sizeof(unsigned long) - 1)
#else
# define __KERNEL_TSS_LIMIT	\
	(offsetof(struct tss_struct, x86_tss) + sizeof(struct x86_hw_tss) - 1)
#endif

/* Base offset outside of TSS_LIMIT so unpriviledged IO causes #GP */
#define IO_BITMAP_OFFSET_INVALID	(__KERNEL_TSS_LIMIT + 1)

struct entry_stack {
	char	stack[PAGE_SIZE];
};

struct entry_stack_page {
	struct entry_stack stack;
} __aligned(PAGE_SIZE);

/*
 * All IO bitmap related data stored in the TSS:
 */
struct x86_io_bitmap {
	/* The sequence number of the last active bitmap. */
	u64			prev_sequence;

	/*
	 * Store the dirty size of the last io bitmap offender. The next
	 * one will have to do the cleanup as the switch out to a non io
	 * bitmap user will just set x86_tss.io_bitmap_base to a value
	 * outside of the TSS limit. So for sane tasks there is no need to
	 * actually touch the io_bitmap at all.
	 */
	unsigned int		prev_max;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit.
	 */
<<<<<<< HEAD
	unsigned long		io_bitmap[IO_BITMAP_LONGS + 1];

	/*
	 * .. and then another 0x100 bytes for the emergency kernel stack:
	 */
	unsigned long		stack[64];

} ____cacheline_aligned;

DECLARE_PER_CPU_SHARED_ALIGNED(struct tss_struct, init_tss);

/*
 * Save the original ist values for checking stack pointers during debugging
 */
struct orig_ist {
	unsigned long		ist[7];
};

#define	MXCSR_DEFAULT		0x1f80

struct i387_fsave_struct {
	u32			cwd;	/* FPU Control Word		*/
	u32			swd;	/* FPU Status Word		*/
	u32			twd;	/* FPU Tag Word			*/
	u32			fip;	/* FPU IP Offset		*/
	u32			fcs;	/* FPU IP Selector		*/
	u32			foo;	/* FPU Operand Pointer Offset	*/
	u32			fos;	/* FPU Operand Pointer Selector	*/

	/* 8*10 bytes for each FP-reg = 80 bytes:			*/
	u32			st_space[20];

	/* Software status information [not touched by FSAVE ]:		*/
	u32			status;
};

struct i387_fxsave_struct {
	u16			cwd; /* Control Word			*/
	u16			swd; /* Status Word			*/
	u16			twd; /* Tag Word			*/
	u16			fop; /* Last Instruction Opcode		*/
	union {
		struct {
			u64	rip; /* Instruction Pointer		*/
			u64	rdp; /* Data Pointer			*/
		};
		struct {
			u32	fip; /* FPU IP Offset			*/
			u32	fcs; /* FPU IP Selector			*/
			u32	foo; /* FPU Operand Offset		*/
			u32	fos; /* FPU Operand Selector		*/
		};
	};
	u32			mxcsr;		/* MXCSR Register State */
	u32			mxcsr_mask;	/* MXCSR Mask		*/

	/* 8*16 bytes for each FP-reg = 128 bytes:			*/
	u32			st_space[32];

	/* 16*16 bytes for each XMM-reg = 256 bytes:			*/
	u32			xmm_space[64];

	u32			padding[12];

	union {
		u32		padding1[12];
		u32		sw_reserved[12];
	};

} __attribute__((aligned(16)));

struct i387_soft_struct {
	u32			cwd;
	u32			swd;
	u32			twd;
	u32			fip;
	u32			fcs;
	u32			foo;
	u32			fos;
	/* 8*10 bytes for each FP-reg = 80 bytes: */
	u32			st_space[20];
	u8			ftop;
	u8			changed;
	u8			lookahead;
	u8			no_update;
	u8			rm;
	u8			alimit;
	struct math_emu_info	*info;
	u32			entry_eip;
};

struct ymmh_struct {
	/* 16 * 16 bytes for each YMMH-reg = 256 bytes */
	u32 ymmh_space[64];
};

struct xsave_hdr_struct {
	u64 xstate_bv;
	u64 reserved1[2];
	u64 reserved2[5];
} __attribute__((packed));

struct xsave_struct {
	struct i387_fxsave_struct i387;
	struct xsave_hdr_struct xsave_hdr;
	struct ymmh_struct ymmh;
	/* new processor state extensions will go here */
} __attribute__ ((packed, aligned (64)));

union thread_xstate {
	struct i387_fsave_struct	fsave;
	struct i387_fxsave_struct	fxsave;
	struct i387_soft_struct		soft;
	struct xsave_struct		xsave;
};

struct fpu {
	unsigned int last_cpu;
	unsigned int has_fpu;
	union thread_xstate *state;
};

#ifdef CONFIG_X86_64
DECLARE_PER_CPU(struct orig_ist, orig_ist);

union irq_stack_union {
	char irq_stack[IRQ_STACK_SIZE];
=======
	unsigned long		bitmap[IO_BITMAP_LONGS + 1];

	/*
	 * Special I/O bitmap to emulate IOPL(3). All bytes zero,
	 * except the additional byte at the end.
	 */
	unsigned long		mapall[IO_BITMAP_LONGS + 1];
};

struct tss_struct {
	/*
	 * The fixed hardware portion.  This must not cross a page boundary
	 * at risk of violating the SDM's advice and potentially triggering
	 * errata.
	 */
	struct x86_hw_tss	x86_tss;

	struct x86_io_bitmap	io_bitmap;
} __aligned(PAGE_SIZE);

DECLARE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw);

/* Per CPU interrupt stacks */
struct irq_stack {
	char		stack[IRQ_STACK_SIZE];
} __aligned(IRQ_STACK_SIZE);

#ifdef CONFIG_X86_64
struct fixed_percpu_data {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * GCC hardcodes the stack canary as %gs:40.  Since the
	 * irq_stack is the object at %gs:0, we reserve the bottom
	 * 48 bytes of the irq stack for the canary.
<<<<<<< HEAD
	 */
	struct {
		char gs_base[40];
		unsigned long stack_canary;
	};
};

DECLARE_PER_CPU_FIRST(union irq_stack_union, irq_stack_union);
DECLARE_INIT_PER_CPU(irq_stack_union);

DECLARE_PER_CPU(char *, irq_stack_ptr);
DECLARE_PER_CPU(unsigned int, irq_count);
extern unsigned long kernel_eflags;
extern asmlinkage void ignore_sysret(void);
#else	/* X86_64 */
#ifdef CONFIG_CC_STACKPROTECTOR
/*
 * Make sure stack canary segment base is cached-aligned:
 *   "For Intel Atom processors, avoid non zero segment base address
 *    that is not aligned to cache line boundary at all cost."
 * (Optim Ref Manual Assembly/Compiler Coding Rule 15.)
 */
struct stack_canary {
	char __pad[20];		/* canary at %gs:20 */
	unsigned long canary;
};
DECLARE_PER_CPU_ALIGNED(struct stack_canary, stack_canary);
#endif
#endif	/* X86_64 */

extern unsigned int xstate_size;
extern void free_thread_xstate(struct task_struct *);
extern struct kmem_cache *task_xstate_cachep;
=======
	 *
	 * Once we are willing to require -mstack-protector-guard-symbol=
	 * support for x86_64 stackprotector, we can get rid of this.
	 */
	char		gs_base[40];
	unsigned long	stack_canary;
};

DECLARE_PER_CPU_FIRST(struct fixed_percpu_data, fixed_percpu_data) __visible;
DECLARE_INIT_PER_CPU(fixed_percpu_data);

static inline unsigned long cpu_kernelmode_gs_base(int cpu)
{
	return (unsigned long)per_cpu(fixed_percpu_data.gs_base, cpu);
}

extern asmlinkage void entry_SYSCALL32_ignore(void);

/* Save actual FS/GS selectors and bases to current->thread */
void current_save_fsgs(void);
#else	/* X86_64 */
#ifdef CONFIG_STACKPROTECTOR
DECLARE_PER_CPU(unsigned long, __stack_chk_guard);
#endif
#endif	/* !X86_64 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct perf_event;

struct thread_struct {
	/* Cached TLS descriptors: */
	struct desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];
<<<<<<< HEAD
	unsigned long		sp0;
=======
#ifdef CONFIG_X86_32
	unsigned long		sp0;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		sp;
#ifdef CONFIG_X86_32
	unsigned long		sysenter_cs;
#else
<<<<<<< HEAD
	unsigned long		usersp;	/* Copy from PDA */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned short		es;
	unsigned short		ds;
	unsigned short		fsindex;
	unsigned short		gsindex;
#endif
<<<<<<< HEAD
#ifdef CONFIG_X86_32
	unsigned long		ip;
#endif
#ifdef CONFIG_X86_64
	unsigned long		fs;
#endif
	unsigned long		gs;
	/* Save middle states of ptrace breakpoints */
	struct perf_event	*ptrace_bps[HBP_NUM];
	/* Debug status used for traps, single steps, etc... */
	unsigned long           debugreg6;
=======

#ifdef CONFIG_X86_64
	unsigned long		fsbase;
	unsigned long		gsbase;
#else
	/*
	 * XXX: this could presumably be unsigned short.  Alternatively,
	 * 32-bit kernels could be taught to use fsindex instead.
	 */
	unsigned long fs;
	unsigned long gs;
#endif

	/* Save middle states of ptrace breakpoints */
	struct perf_event	*ptrace_bps[HBP_NUM];
	/* Debug status used for traps, single steps, etc... */
	unsigned long           virtual_dr6;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Keep track of the exact dr7 value set by the user */
	unsigned long           ptrace_dr7;
	/* Fault info: */
	unsigned long		cr2;
	unsigned long		trap_nr;
	unsigned long		error_code;
<<<<<<< HEAD
	/* floating point and extended processor state */
	struct fpu		fpu;
#ifdef CONFIG_X86_32
	/* Virtual 86 mode info */
	struct vm86_struct __user *vm86_info;
	unsigned long		screen_bitmap;
	unsigned long		v86flags;
	unsigned long		v86mask;
	unsigned long		saved_sp0;
	unsigned int		saved_fs;
	unsigned int		saved_gs;
#endif
	/* IO permissions: */
	unsigned long		*io_bitmap_ptr;
	unsigned long		iopl;
	/* Max allowed port in the bitmap, in bytes: */
	unsigned		io_bitmap_max;
};

/*
 * Set IOPL bits in EFLAGS from given mask
 */
static inline void native_set_iopl_mask(unsigned mask)
{
#ifdef CONFIG_X86_32
	unsigned int reg;

	asm volatile ("pushfl;"
		      "popl %0;"
		      "andl %1, %0;"
		      "orl %2, %0;"
		      "pushl %0;"
		      "popfl"
		      : "=&r" (reg)
		      : "i" (~X86_EFLAGS_IOPL), "r" (mask));
#endif
}

static inline void
native_load_sp0(struct tss_struct *tss, struct thread_struct *thread)
{
	tss->x86_tss.sp0 = thread->sp0;
#ifdef CONFIG_X86_32
	/* Only happens when SEP is enabled, no need to test "SEP"arately: */
	if (unlikely(tss->x86_tss.ss1 != thread->sysenter_cs)) {
		tss->x86_tss.ss1 = thread->sysenter_cs;
		wrmsr(MSR_IA32_SYSENTER_CS, thread->sysenter_cs, 0);
	}
#endif
}

static inline void native_swapgs(void)
=======
#ifdef CONFIG_VM86
	/* Virtual 86 mode info */
	struct vm86		*vm86;
#endif
	/* IO permissions: */
	struct io_bitmap	*io_bitmap;

	/*
	 * IOPL. Privilege level dependent I/O permission which is
	 * emulated via the I/O bitmap to prevent user space from disabling
	 * interrupts.
	 */
	unsigned long		iopl_emul;

	unsigned int		iopl_warn:1;

	/*
	 * Protection Keys Register for Userspace.  Loaded immediately on
	 * context switch. Store it in thread_struct to avoid a lookup in
	 * the tasks's FPU xstate buffer. This value is only valid when a
	 * task is scheduled out. For 'current' the authoritative source of
	 * PKRU is the hardware itself.
	 */
	u32			pkru;

#ifdef CONFIG_X86_USER_SHADOW_STACK
	unsigned long		features;
	unsigned long		features_locked;

	struct thread_shstk	shstk;
#endif

	/* Floating point and extended processor state */
	struct fpu		fpu;
	/*
	 * WARNING: 'fpu' is dynamically-sized.  It *MUST* be at
	 * the end.
	 */
};

extern void fpu_thread_struct_whitelist(unsigned long *offset, unsigned long *size);

static inline void arch_thread_struct_whitelist(unsigned long *offset,
						unsigned long *size)
{
	fpu_thread_struct_whitelist(offset, size);
}

static inline void
native_load_sp0(unsigned long sp0)
{
	this_cpu_write(cpu_tss_rw.x86_tss.sp0, sp0);
}

static __always_inline void native_swapgs(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#ifdef CONFIG_X86_64
	asm volatile("swapgs" ::: "memory");
#endif
}

<<<<<<< HEAD
#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#else
#define __cpuid			native_cpuid
#define paravirt_enabled()	0

static inline void load_sp0(struct tss_struct *tss,
			    struct thread_struct *thread)
{
	native_load_sp0(tss, thread);
}

#define set_iopl_mask native_set_iopl_mask
#endif /* CONFIG_PARAVIRT */

/*
 * Save the cr4 feature set we're using (ie
 * Pentium 4MB enable and PPro Global page
 * enable), so that any CPU's that boot up
 * after us can get the correct flags.
 */
extern unsigned long		mmu_cr4_features;

static inline void set_in_cr4(unsigned long mask)
{
	unsigned long cr4;

	mmu_cr4_features |= mask;
	cr4 = read_cr4();
	cr4 |= mask;
	write_cr4(cr4);
}

static inline void clear_in_cr4(unsigned long mask)
{
	unsigned long cr4;

	mmu_cr4_features &= ~mask;
	cr4 = read_cr4();
	cr4 &= ~mask;
	write_cr4(cr4);
}

typedef struct {
	unsigned long		seg;
} mm_segment_t;


/*
 * create a kernel thread without removing it from tasklists
 */
extern int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);

/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *);

/* Prepare to copy thread state - unlazy all lazy state */
extern void prepare_to_copy(struct task_struct *tsk);

unsigned long get_wchan(struct task_struct *p);

/*
 * Generic CPUID function
 * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
 * resulting in stale register contents being returned.
 */
static inline void cpuid(unsigned int op,
			 unsigned int *eax, unsigned int *ebx,
			 unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = 0;
	__cpuid(eax, ebx, ecx, edx);
}

/* Some CPUID calls want 'count' to be placed in ecx */
static inline void cpuid_count(unsigned int op, int count,
			       unsigned int *eax, unsigned int *ebx,
			       unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = count;
	__cpuid(eax, ebx, ecx, edx);
}

/*
 * CPUID functions returning a single datum
 */
static inline unsigned int cpuid_eax(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return eax;
}

static inline unsigned int cpuid_ebx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return ebx;
}

static inline unsigned int cpuid_ecx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return ecx;
}

static inline unsigned int cpuid_edx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return edx;
}

/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static inline void rep_nop(void)
{
	asm volatile("rep; nop" ::: "memory");
}

static inline void cpu_relax(void)
{
	rep_nop();
}

/* Stop speculative execution and prefetching of modified code. */
static inline void sync_core(void)
{
	int tmp;

#if defined(CONFIG_M386) || defined(CONFIG_M486)
	if (boot_cpu_data.x86 < 5)
		/* There is no speculative execution.
		 * jmp is a barrier to prefetching. */
		asm volatile("jmp 1f\n1:\n" ::: "memory");
	else
#endif
		/* cpuid is a barrier to speculative execution.
		 * Prefetched instructions are automatically
		 * invalidated when modified. */
		asm volatile("cpuid" : "=a" (tmp) : "0" (1)
			     : "ebx", "ecx", "edx", "memory");
}

static inline void __monitor(const void *eax, unsigned long ecx,
			     unsigned long edx)
{
	/* "monitor %eax, %ecx, %edx;" */
	asm volatile(".byte 0x0f, 0x01, 0xc8;"
		     :: "a" (eax), "c" (ecx), "d"(edx));
}

static inline void __mwait(unsigned long eax, unsigned long ecx)
{
	/* "mwait %eax, %ecx;" */
	asm volatile(".byte 0x0f, 0x01, 0xc9;"
		     :: "a" (eax), "c" (ecx));
}

static inline void __sti_mwait(unsigned long eax, unsigned long ecx)
{
	trace_hardirqs_on();
	/* "mwait %eax, %ecx;" */
	asm volatile("sti; .byte 0x0f, 0x01, 0xc9;"
		     :: "a" (eax), "c" (ecx));
}

extern void select_idle_routine(const struct cpuinfo_x86 *c);
extern void init_amd_e400_c1e_mask(void);

extern unsigned long		boot_option_idle_override;
extern bool			amd_e400_c1e_detected;

enum idle_boot_override {IDLE_NO_OVERRIDE=0, IDLE_HALT, IDLE_NOMWAIT,
			 IDLE_POLL, IDLE_FORCE_MWAIT};

extern void enable_sep_cpu(void);
extern int sysenter_setup(void);

extern void early_trap_init(void);
=======
static __always_inline unsigned long current_top_of_stack(void)
{
	/*
	 *  We can't read directly from tss.sp0: sp0 on x86_32 is special in
	 *  and around vm86 mode and sp0 on x86_64 is special because of the
	 *  entry trampoline.
	 */
	if (IS_ENABLED(CONFIG_USE_X86_SEG_SUPPORT))
		return this_cpu_read_const(const_pcpu_hot.top_of_stack);

	return this_cpu_read_stable(pcpu_hot.top_of_stack);
}

static __always_inline bool on_thread_stack(void)
{
	return (unsigned long)(current_top_of_stack() -
			       current_stack_pointer) < THREAD_SIZE;
}

#ifdef CONFIG_PARAVIRT_XXL
#include <asm/paravirt.h>
#else

static inline void load_sp0(unsigned long sp0)
{
	native_load_sp0(sp0);
}

#endif /* CONFIG_PARAVIRT_XXL */

unsigned long __get_wchan(struct task_struct *p);

extern void select_idle_routine(void);
extern void amd_e400_c1e_apic_setup(void);

extern unsigned long		boot_option_idle_override;

enum idle_boot_override {IDLE_NO_OVERRIDE=0, IDLE_HALT, IDLE_NOMWAIT,
			 IDLE_POLL};

extern void enable_sep_cpu(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Defined in head.S */
extern struct desc_ptr		early_gdt_descr;

<<<<<<< HEAD
extern void cpu_set_gdt(int);
extern void switch_to_new_gdt(int);
extern void load_percpu_segment(int);
extern void cpu_init(void);

static inline unsigned long get_debugctlmsr(void)
{
	unsigned long debugctlmsr = 0;

#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return 0;
#endif
	rdmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);

	return debugctlmsr;
}

static inline void update_debugctlmsr(unsigned long debugctlmsr)
{
#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return;
#endif
	wrmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);
}

/*
 * from system description table in BIOS. Mostly for MCA use, but
 * others may find it useful:
 */
extern unsigned int		machine_id;
extern unsigned int		machine_submodel_id;
extern unsigned int		BIOS_revision;
=======
extern void switch_gdt_and_percpu_base(int);
extern void load_direct_gdt(int);
extern void load_fixmap_gdt(int);
extern void cpu_init(void);
extern void cpu_init_exception_handling(void);
extern void cr4_init(void);

extern void set_task_blockstep(struct task_struct *task, bool on);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Boot loader type from the setup header: */
extern int			bootloader_type;
extern int			bootloader_version;

extern char			ignore_fpu_irq;

#define HAVE_ARCH_PICK_MMAP_LAYOUT 1
#define ARCH_HAS_PREFETCHW
<<<<<<< HEAD
#define ARCH_HAS_SPINLOCK_PREFETCH

#ifdef CONFIG_X86_32
# define BASE_PREFETCH		ASM_NOP4
# define ARCH_HAS_PREFETCH
#else
# define BASE_PREFETCH		"prefetcht0 (%1)"
=======

#ifdef CONFIG_X86_32
# define BASE_PREFETCH		""
# define ARCH_HAS_PREFETCH
#else
# define BASE_PREFETCH		"prefetcht0 %P1"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * Prefetch instructions for Pentium III (+) and AMD Athlon (+)
 *
 * It's not worth to care about 3dnow prefetches for the K6
 * because they are microcoded there and very slow.
 */
static inline void prefetch(const void *x)
{
<<<<<<< HEAD
	alternative_input(BASE_PREFETCH,
			  "prefetchnta (%1)",
			  X86_FEATURE_XMM,
			  "r" (x));
=======
	alternative_input(BASE_PREFETCH, "prefetchnta %P1",
			  X86_FEATURE_XMM,
			  "m" (*(const char *)x));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * 3dnow prefetch to get an exclusive cache line.
 * Useful for spinlocks to avoid one state transition in the
 * cache coherency protocol:
 */
<<<<<<< HEAD
static inline void prefetchw(const void *x)
{
	alternative_input(BASE_PREFETCH,
			  "prefetchw (%1)",
			  X86_FEATURE_3DNOW,
			  "r" (x));
}

static inline void spin_lock_prefetch(const void *x)
{
	prefetchw(x);
}

#ifdef CONFIG_X86_32
/*
 * User space process size: 3GB (default).
 */
#define TASK_SIZE		PAGE_OFFSET
#define TASK_SIZE_MAX		TASK_SIZE
#define STACK_TOP		TASK_SIZE
#define STACK_TOP_MAX		STACK_TOP

#define INIT_THREAD  {							  \
	.sp0			= sizeof(init_stack) + (long)&init_stack, \
	.vm86_info		= NULL,					  \
	.sysenter_cs		= __KERNEL_CS,				  \
	.io_bitmap_ptr		= NULL,					  \
}

/*
 * Note that the .io_bitmap member must be extra-big. This is because
 * the CPU will access an additional byte beyond the end of the IO
 * permission bitmap. The extra byte must be all 1 bits, and must
 * be within the limit.
 */
#define INIT_TSS  {							  \
	.x86_tss = {							  \
		.sp0		= sizeof(init_stack) + (long)&init_stack, \
		.ss0		= __KERNEL_DS,				  \
		.ss1		= __KERNEL_CS,				  \
		.io_bitmap_base	= INVALID_IO_BITMAP_OFFSET,		  \
	 },								  \
	.io_bitmap		= { [0 ... IO_BITMAP_LONGS] = ~0 },	  \
}

extern unsigned long thread_saved_pc(struct task_struct *tsk);

#define THREAD_SIZE_LONGS      (THREAD_SIZE/sizeof(unsigned long))
#define KSTK_TOP(info)                                                 \
({                                                                     \
       unsigned long *__ptr = (unsigned long *)(info);                 \
       (unsigned long)(&__ptr[THREAD_SIZE_LONGS]);                     \
})

/*
 * The below -8 is to reserve 8 bytes on top of the ring0 stack.
 * This is necessary to guarantee that the entire "struct pt_regs"
 * is accessible even if the CPU haven't stored the SS/ESP registers
 * on the stack (interrupt gate does not save these registers
 * when switching to the same priv ring).
 * Therefore beware: accessing the ss/esp fields of the
 * "struct pt_regs" is possible, but they may contain the
 * completely wrong values.
 */
#define task_pt_regs(task)                                             \
({                                                                     \
       struct pt_regs *__regs__;                                       \
       __regs__ = (struct pt_regs *)(KSTK_TOP(task_stack_page(task))-8); \
       __regs__ - 1;                                                   \
})

#define KSTK_ESP(task)		(task_pt_regs(task)->sp)

#else
/*
 * User space process size. 47bits minus one guard page.
 */
#define TASK_SIZE_MAX	((1UL << 47) - PAGE_SIZE)

/* This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define IA32_PAGE_OFFSET	((current->personality & ADDR_LIMIT_3GB) ? \
					0xc0000000 : 0xFFFFe000)

#define TASK_SIZE		(test_thread_flag(TIF_ADDR32) ? \
					IA32_PAGE_OFFSET : TASK_SIZE_MAX)
#define TASK_SIZE_OF(child)	((test_tsk_thread_flag(child, TIF_ADDR32)) ? \
					IA32_PAGE_OFFSET : TASK_SIZE_MAX)

#define STACK_TOP		TASK_SIZE
#define STACK_TOP_MAX		TASK_SIZE_MAX

#define INIT_THREAD  { \
	.sp0 = (unsigned long)&init_stack + sizeof(init_stack) \
}

#define INIT_TSS  { \
	.x86_tss.sp0 = (unsigned long)&init_stack + sizeof(init_stack) \
}

/*
 * Return saved PC of a blocked thread.
 * What is this good for? it will be always the scheduler or ret_from_fork.
 */
#define thread_saved_pc(t)	(*(unsigned long *)((t)->thread.sp - 8))

#define task_pt_regs(tsk)	((struct pt_regs *)(tsk)->thread.sp0 - 1)
extern unsigned long KSTK_ESP(struct task_struct *task);

/*
 * User space RSP while inside the SYSCALL fast path
 */
DECLARE_PER_CPU(unsigned long, old_rsp);

=======
static __always_inline void prefetchw(const void *x)
{
	alternative_input(BASE_PREFETCH, "prefetchw %P1",
			  X86_FEATURE_3DNOWPREFETCH,
			  "m" (*(const char *)x));
}

#define TOP_OF_INIT_STACK ((unsigned long)&init_stack + sizeof(init_stack) - \
			   TOP_OF_KERNEL_STACK_PADDING)

#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))

#define task_pt_regs(task) \
({									\
	unsigned long __ptr = (unsigned long)task_stack_page(task);	\
	__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;		\
	((struct pt_regs *)__ptr) - 1;					\
})

#ifdef CONFIG_X86_32
#define INIT_THREAD  {							  \
	.sp0			= TOP_OF_INIT_STACK,			  \
	.sysenter_cs		= __KERNEL_CS,				  \
}

#define KSTK_ESP(task)		(task_pt_regs(task)->sp)

#else
extern unsigned long __end_init_task[];

#define INIT_THREAD {							\
	.sp	= (unsigned long)&__end_init_task -			\
		  TOP_OF_KERNEL_STACK_PADDING -				\
		  sizeof(struct pt_regs),				\
}

extern unsigned long KSTK_ESP(struct task_struct *task);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_X86_64 */

extern void start_thread(struct pt_regs *regs, unsigned long new_ip,
					       unsigned long new_sp);

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
<<<<<<< HEAD
#define TASK_UNMAPPED_BASE	(PAGE_ALIGN(TASK_SIZE / 3))
=======
#define __TASK_UNMAPPED_BASE(task_size)	(PAGE_ALIGN(task_size / 3))
#define TASK_UNMAPPED_BASE		__TASK_UNMAPPED_BASE(TASK_SIZE_LOW)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define KSTK_EIP(task)		(task_pt_regs(task)->ip)

/* Get/set a process' ability to use the timestamp counter instruction */
#define GET_TSC_CTL(adr)	get_tsc_mode((adr))
#define SET_TSC_CTL(val)	set_tsc_mode((val))

extern int get_tsc_mode(unsigned long adr);
extern int set_tsc_mode(unsigned int val);

<<<<<<< HEAD
extern int amd_get_nb_id(int cpu);

struct aperfmperf {
	u64 aperf, mperf;
};

static inline void get_aperfmperf(struct aperfmperf *am)
{
	WARN_ON_ONCE(!boot_cpu_has(X86_FEATURE_APERFMPERF));

	rdmsrl(MSR_IA32_APERF, am->aperf);
	rdmsrl(MSR_IA32_MPERF, am->mperf);
}

#define APERFMPERF_SHIFT 10

static inline
unsigned long calc_aperfmperf_ratio(struct aperfmperf *old,
				    struct aperfmperf *new)
{
	u64 aperf = new->aperf - old->aperf;
	u64 mperf = new->mperf - old->mperf;
	unsigned long ratio = aperf;

	mperf >>= APERFMPERF_SHIFT;
	if (mperf)
		ratio = div64_u64(aperf, mperf);

	return ratio;
}

/*
 * AMD errata checking
 */
#ifdef CONFIG_CPU_SUP_AMD
extern const int amd_erratum_383[];
extern const int amd_erratum_400[];
extern bool cpu_has_amd_erratum(const int *);

#define AMD_LEGACY_ERRATUM(...)		{ -1, __VA_ARGS__, 0 }
#define AMD_OSVW_ERRATUM(osvw_id, ...)	{ osvw_id, __VA_ARGS__, 0 }
#define AMD_MODEL_RANGE(f, m_start, s_start, m_end, s_end) \
	((f << 24) | (m_start << 16) | (s_start << 12) | (m_end << 4) | (s_end))
#define AMD_MODEL_RANGE_FAMILY(range)	(((range) >> 24) & 0xff)
#define AMD_MODEL_RANGE_START(range)	(((range) >> 12) & 0xfff)
#define AMD_MODEL_RANGE_END(range)	((range) & 0xfff)

#else
#define cpu_has_amd_erratum(x)	(false)
#endif /* CONFIG_CPU_SUP_AMD */

void cpu_idle_wait(void);

extern unsigned long arch_align_stack(unsigned long sp);
extern void free_init_pages(char *what, unsigned long begin, unsigned long end);

void default_idle(void);
bool set_pm_idle_to_default(void);

void stop_this_cpu(void *dummy);
=======
DECLARE_PER_CPU(u64, msr_misc_features_shadow);

static inline u32 per_cpu_llc_id(unsigned int cpu)
{
	return per_cpu(cpu_info.topo.llc_id, cpu);
}

static inline u32 per_cpu_l2c_id(unsigned int cpu)
{
	return per_cpu(cpu_info.topo.l2c_id, cpu);
}

#ifdef CONFIG_CPU_SUP_AMD
extern u32 amd_get_highest_perf(void);
extern void amd_clear_divider(void);
extern void amd_check_microcode(void);
#else
static inline u32 amd_get_highest_perf(void)		{ return 0; }
static inline void amd_clear_divider(void)		{ }
static inline void amd_check_microcode(void)		{ }
#endif

extern unsigned long arch_align_stack(unsigned long sp);
void free_init_pages(const char *what, unsigned long begin, unsigned long end);
extern void free_kernel_image_pages(const char *what, void *begin, void *end);

void default_idle(void);
#ifdef	CONFIG_XEN
bool xen_set_default_idle(void);
#else
#define xen_set_default_idle 0
#endif

void __noreturn stop_this_cpu(void *dummy);
void microcode_check(struct cpuinfo_x86 *prev_info);
void store_cpu_caps(struct cpuinfo_x86 *info);

enum l1tf_mitigations {
	L1TF_MITIGATION_OFF,
	L1TF_MITIGATION_FLUSH_NOWARN,
	L1TF_MITIGATION_FLUSH,
	L1TF_MITIGATION_FLUSH_NOSMT,
	L1TF_MITIGATION_FULL,
	L1TF_MITIGATION_FULL_FORCE
};

extern enum l1tf_mitigations l1tf_mitigation;

enum mds_mitigations {
	MDS_MITIGATION_OFF,
	MDS_MITIGATION_FULL,
	MDS_MITIGATION_VMWERV,
};

extern bool gds_ucode_mitigated(void);

/*
 * Make previous memory operations globally visible before
 * a WRMSR.
 *
 * MFENCE makes writes visible, but only affects load/store
 * instructions.  WRMSR is unfortunately not a load/store
 * instruction and is unaffected by MFENCE.  The LFENCE ensures
 * that the WRMSR is not reordered.
 *
 * Most WRMSRs are full serializing instructions themselves and
 * do not require this barrier.  This is only required for the
 * IA32_TSC_DEADLINE and X2APIC MSRs.
 */
static inline void weak_wrmsr_fence(void)
{
	alternative("mfence; lfence", "", ALT_NOT(X86_FEATURE_APIC_MSRS_FENCE));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_PROCESSOR_H */
