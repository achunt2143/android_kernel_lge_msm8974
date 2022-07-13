<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_DESC_H
#define _ASM_X86_DESC_H

#include <asm/desc_defs.h>
#include <asm/ldt.h>
#include <asm/mmu.h>
<<<<<<< HEAD

#include <linux/smp.h>
=======
#include <asm/fixmap.h>
#include <asm/irq_vectors.h>
#include <asm/cpu_entry_area.h>

#include <linux/debug_locks.h>
#include <linux/smp.h>
#include <linux/percpu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void fill_ldt(struct desc_struct *desc, const struct user_desc *info)
{
	desc->limit0		= info->limit & 0x0ffff;

	desc->base0		= (info->base_addr & 0x0000ffff);
	desc->base1		= (info->base_addr & 0x00ff0000) >> 16;

	desc->type		= (info->read_exec_only ^ 1) << 1;
	desc->type	       |= info->contents << 2;
<<<<<<< HEAD
=======
	/* Set the ACCESS bit so it can be mapped RO */
	desc->type	       |= 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	desc->s			= 1;
	desc->dpl		= 0x3;
	desc->p			= info->seg_not_present ^ 1;
<<<<<<< HEAD
	desc->limit		= (info->limit & 0xf0000) >> 16;
=======
	desc->limit1		= (info->limit & 0xf0000) >> 16;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	desc->avl		= info->useable;
	desc->d			= info->seg_32bit;
	desc->g			= info->limit_in_pages;

	desc->base2		= (info->base_addr & 0xff000000) >> 24;
	/*
	 * Don't allow setting of the lm bit. It would confuse
	 * user_64bit_mode and would get overridden by sysret anyway.
	 */
	desc->l			= 0;
}

<<<<<<< HEAD
extern struct desc_ptr idt_descr;
extern gate_desc idt_table[];
extern struct desc_ptr nmi_idt_descr;
extern gate_desc nmi_idt_table[];

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct gdt_page {
	struct desc_struct gdt[GDT_ENTRIES];
} __attribute__((aligned(PAGE_SIZE)));

DECLARE_PER_CPU_PAGE_ALIGNED(struct gdt_page, gdt_page);
<<<<<<< HEAD

static inline struct desc_struct *get_cpu_gdt_table(unsigned int cpu)
=======
DECLARE_INIT_PER_CPU(gdt_page);

/* Provide the original GDT */
static inline struct desc_struct *get_cpu_gdt_rw(unsigned int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return per_cpu(gdt_page, cpu).gdt;
}

<<<<<<< HEAD
#ifdef CONFIG_X86_64
=======
/* Provide the current original GDT */
static inline struct desc_struct *get_current_gdt_rw(void)
{
	return this_cpu_ptr(&gdt_page)->gdt;
}

/* Provide the fixmap address of the remapped GDT */
static inline struct desc_struct *get_cpu_gdt_ro(int cpu)
{
	return (struct desc_struct *)&get_cpu_entry_area(cpu)->gdt;
}

/* Provide the current read-only GDT */
static inline struct desc_struct *get_current_gdt_ro(void)
{
	return get_cpu_gdt_ro(smp_processor_id());
}

/* Provide the physical address of the GDT page. */
static inline phys_addr_t get_cpu_gdt_paddr(unsigned int cpu)
{
	return per_cpu_ptr_to_phys(get_cpu_gdt_rw(cpu));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void pack_gate(gate_desc *gate, unsigned type, unsigned long func,
			     unsigned dpl, unsigned ist, unsigned seg)
{
<<<<<<< HEAD
	gate->offset_low	= PTR_LOW(func);
	gate->segment		= __KERNEL_CS;
	gate->ist		= ist;
	gate->p			= 1;
	gate->dpl		= dpl;
	gate->zero0		= 0;
	gate->zero1		= 0;
	gate->type		= type;
	gate->offset_middle	= PTR_MIDDLE(func);
	gate->offset_high	= PTR_HIGH(func);
}

#else
static inline void pack_gate(gate_desc *gate, unsigned char type,
			     unsigned long base, unsigned dpl, unsigned flags,
			     unsigned short seg)
{
	gate->a = (seg << 16) | (base & 0xffff);
	gate->b = (base & 0xffff0000) | (((0x80 | type | (dpl << 5)) & 0xff) << 8);
}

#endif
=======
	gate->offset_low	= (u16) func;
	gate->bits.p		= 1;
	gate->bits.dpl		= dpl;
	gate->bits.zero		= 0;
	gate->bits.type		= type;
	gate->offset_middle	= (u16) (func >> 16);
#ifdef CONFIG_X86_64
	gate->segment		= __KERNEL_CS;
	gate->bits.ist		= ist;
	gate->reserved		= 0;
	gate->offset_high	= (u32) (func >> 32);
#else
	gate->segment		= seg;
	gate->bits.ist		= 0;
#endif
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int desc_empty(const void *ptr)
{
	const u32 *desc = ptr;

	return !(desc[0] | desc[1]);
}

<<<<<<< HEAD
#ifdef CONFIG_PARAVIRT
=======
#ifdef CONFIG_PARAVIRT_XXL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/paravirt.h>
#else
#define load_TR_desc()				native_load_tr_desc()
#define load_gdt(dtr)				native_load_gdt(dtr)
#define load_idt(dtr)				native_load_idt(dtr)
#define load_tr(tr)				asm volatile("ltr %0"::"m" (tr))
#define load_ldt(ldt)				asm volatile("lldt %0"::"m" (ldt))

#define store_gdt(dtr)				native_store_gdt(dtr)
<<<<<<< HEAD
#define store_idt(dtr)				native_store_idt(dtr)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define store_tr(tr)				(tr = native_store_tr())

#define load_TLS(t, cpu)			native_load_tls(t, cpu)
#define set_ldt					native_set_ldt

#define write_ldt_entry(dt, entry, desc)	native_write_ldt_entry(dt, entry, desc)
#define write_gdt_entry(dt, entry, desc, type)	native_write_gdt_entry(dt, entry, desc, type)
#define write_idt_entry(dt, entry, g)		native_write_idt_entry(dt, entry, g)

static inline void paravirt_alloc_ldt(struct desc_struct *ldt, unsigned entries)
{
}

static inline void paravirt_free_ldt(struct desc_struct *ldt, unsigned entries)
{
}
<<<<<<< HEAD
#endif	/* CONFIG_PARAVIRT */
=======
#endif	/* CONFIG_PARAVIRT_XXL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define store_ldt(ldt) asm("sldt %0" : "=m"(ldt))

static inline void native_write_idt_entry(gate_desc *idt, int entry, const gate_desc *gate)
{
	memcpy(&idt[entry], gate, sizeof(*gate));
}

static inline void native_write_ldt_entry(struct desc_struct *ldt, int entry, const void *desc)
{
	memcpy(&ldt[entry], desc, 8);
}

static inline void
native_write_gdt_entry(struct desc_struct *gdt, int entry, const void *desc, int type)
{
	unsigned int size;

	switch (type) {
	case DESC_TSS:	size = sizeof(tss_desc);	break;
	case DESC_LDT:	size = sizeof(ldt_desc);	break;
	default:	size = sizeof(*gdt);		break;
	}

	memcpy(&gdt[entry], desc, size);
}

<<<<<<< HEAD
static inline void pack_descriptor(struct desc_struct *desc, unsigned long base,
				   unsigned long limit, unsigned char type,
				   unsigned char flags)
{
	desc->a = ((base & 0xffff) << 16) | (limit & 0xffff);
	desc->b = (base & 0xff000000) | ((base & 0xff0000) >> 16) |
		(limit & 0x000f0000) | ((type & 0xff) << 8) |
		((flags & 0xf) << 20);
	desc->p = 1;
}


static inline void set_tssldt_descriptor(void *d, unsigned long addr, unsigned type, unsigned size)
{
#ifdef CONFIG_X86_64
	struct ldttss_desc64 *desc = d;

	memset(desc, 0, sizeof(*desc));

	desc->limit0		= size & 0xFFFF;
	desc->base0		= PTR_LOW(addr);
	desc->base1		= PTR_MIDDLE(addr) & 0xFF;
	desc->type		= type;
	desc->p			= 1;
	desc->limit1		= (size >> 16) & 0xF;
	desc->base2		= (PTR_MIDDLE(addr) >> 8) & 0xFF;
	desc->base3		= PTR_HIGH(addr);
#else
	pack_descriptor((struct desc_struct *)d, addr, size, 0x80 | type, 0);
#endif
}

static inline void __set_tss_desc(unsigned cpu, unsigned int entry, void *addr)
{
	struct desc_struct *d = get_cpu_gdt_table(cpu);
	tss_desc tss;

	/*
	 * sizeof(unsigned long) coming from an extra "long" at the end
	 * of the iobitmap. See tss_struct definition in processor.h
	 *
	 * -1? seg base+limit should be pointing to the address of the
	 * last valid byte
	 */
	set_tssldt_descriptor(&tss, (unsigned long)addr, DESC_TSS,
			      IO_BITMAP_OFFSET + IO_BITMAP_BYTES +
			      sizeof(unsigned long) - 1);
=======
static inline void set_tssldt_descriptor(void *d, unsigned long addr,
					 unsigned type, unsigned size)
{
	struct ldttss_desc *desc = d;

	memset(desc, 0, sizeof(*desc));

	desc->limit0		= (u16) size;
	desc->base0		= (u16) addr;
	desc->base1		= (addr >> 16) & 0xFF;
	desc->type		= type;
	desc->p			= 1;
	desc->limit1		= (size >> 16) & 0xF;
	desc->base2		= (addr >> 24) & 0xFF;
#ifdef CONFIG_X86_64
	desc->base3		= (u32) (addr >> 32);
#endif
}

static inline void __set_tss_desc(unsigned cpu, unsigned int entry, struct x86_hw_tss *addr)
{
	struct desc_struct *d = get_cpu_gdt_rw(cpu);
	tss_desc tss;

	set_tssldt_descriptor(&tss, (unsigned long)addr, DESC_TSS,
			      __KERNEL_TSS_LIMIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	write_gdt_entry(d, entry, &tss, DESC_TSS);
}

#define set_tss_desc(cpu, addr) __set_tss_desc(cpu, GDT_ENTRY_TSS, addr)

static inline void native_set_ldt(const void *addr, unsigned int entries)
{
	if (likely(entries == 0))
		asm volatile("lldt %w0"::"q" (0));
	else {
		unsigned cpu = smp_processor_id();
		ldt_desc ldt;

		set_tssldt_descriptor(&ldt, (unsigned long)addr, DESC_LDT,
				      entries * LDT_ENTRY_SIZE - 1);
<<<<<<< HEAD
		write_gdt_entry(get_cpu_gdt_table(cpu), GDT_ENTRY_LDT,
=======
		write_gdt_entry(get_cpu_gdt_rw(cpu), GDT_ENTRY_LDT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				&ldt, DESC_LDT);
		asm volatile("lldt %w0"::"q" (GDT_ENTRY_LDT*8));
	}
}

<<<<<<< HEAD
static inline void native_load_tr_desc(void)
{
	asm volatile("ltr %w0"::"q" (GDT_ENTRY_TSS*8));
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void native_load_gdt(const struct desc_ptr *dtr)
{
	asm volatile("lgdt %0"::"m" (*dtr));
}

<<<<<<< HEAD
static inline void native_load_idt(const struct desc_ptr *dtr)
=======
static __always_inline void native_load_idt(const struct desc_ptr *dtr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	asm volatile("lidt %0"::"m" (*dtr));
}

static inline void native_store_gdt(struct desc_ptr *dtr)
{
	asm volatile("sgdt %0":"=m" (*dtr));
}

<<<<<<< HEAD
static inline void native_store_idt(struct desc_ptr *dtr)
=======
static inline void store_idt(struct desc_ptr *dtr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	asm volatile("sidt %0":"=m" (*dtr));
}

<<<<<<< HEAD
=======
static inline void native_gdt_invalidate(void)
{
	const struct desc_ptr invalid_gdt = {
		.address = 0,
		.size = 0
	};

	native_load_gdt(&invalid_gdt);
}

static inline void native_idt_invalidate(void)
{
	const struct desc_ptr invalid_idt = {
		.address = 0,
		.size = 0
	};

	native_load_idt(&invalid_idt);
}

/*
 * The LTR instruction marks the TSS GDT entry as busy. On 64-bit, the GDT is
 * a read-only remapping. To prevent a page fault, the GDT is switched to the
 * original writeable version when needed.
 */
#ifdef CONFIG_X86_64
static inline void native_load_tr_desc(void)
{
	struct desc_ptr gdt;
	int cpu = raw_smp_processor_id();
	bool restore = 0;
	struct desc_struct *fixmap_gdt;

	native_store_gdt(&gdt);
	fixmap_gdt = get_cpu_gdt_ro(cpu);

	/*
	 * If the current GDT is the read-only fixmap, swap to the original
	 * writeable version. Swap back at the end.
	 */
	if (gdt.address == (unsigned long)fixmap_gdt) {
		load_direct_gdt(cpu);
		restore = 1;
	}
	asm volatile("ltr %w0"::"q" (GDT_ENTRY_TSS*8));
	if (restore)
		load_fixmap_gdt(cpu);
}
#else
static inline void native_load_tr_desc(void)
{
	asm volatile("ltr %w0"::"q" (GDT_ENTRY_TSS*8));
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned long native_store_tr(void)
{
	unsigned long tr;

	asm volatile("str %0":"=r" (tr));

	return tr;
}

static inline void native_load_tls(struct thread_struct *t, unsigned int cpu)
{
<<<<<<< HEAD
	struct desc_struct *gdt = get_cpu_gdt_table(cpu);
=======
	struct desc_struct *gdt = get_cpu_gdt_rw(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int i;

	for (i = 0; i < GDT_ENTRY_TLS_ENTRIES; i++)
		gdt[GDT_ENTRY_TLS_MIN + i] = t->tls_array[i];
}

<<<<<<< HEAD
=======
DECLARE_PER_CPU(bool, __tss_limit_invalid);

static inline void force_reload_TR(void)
{
	struct desc_struct *d = get_current_gdt_rw();
	tss_desc tss;

	memcpy(&tss, &d[GDT_ENTRY_TSS], sizeof(tss_desc));

	/*
	 * LTR requires an available TSS, and the TSS is currently
	 * busy.  Make it be available so that LTR will work.
	 */
	tss.type = DESC_TSS;
	write_gdt_entry(d, GDT_ENTRY_TSS, &tss, DESC_TSS);

	load_TR_desc();
	this_cpu_write(__tss_limit_invalid, false);
}

/*
 * Call this if you need the TSS limit to be correct, which should be the case
 * if and only if you have TIF_IO_BITMAP set or you're switching to a task
 * with TIF_IO_BITMAP set.
 */
static inline void refresh_tss_limit(void)
{
	DEBUG_LOCKS_WARN_ON(preemptible());

	if (unlikely(this_cpu_read(__tss_limit_invalid)))
		force_reload_TR();
}

/*
 * If you do something evil that corrupts the cached TSS limit (I'm looking
 * at you, VMX exits), call this function.
 *
 * The optimization here is that the TSS limit only matters for Linux if the
 * IO bitmap is in use.  If the TSS limit gets forced to its minimum value,
 * everything works except that IO bitmap will be ignored and all CPL 3 IO
 * instructions will #GP, which is exactly what we want for normal tasks.
 */
static inline void invalidate_tss_limit(void)
{
	DEBUG_LOCKS_WARN_ON(preemptible());

	if (unlikely(test_thread_flag(TIF_IO_BITMAP)))
		force_reload_TR();
	else
		this_cpu_write(__tss_limit_invalid, true);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* This intentionally ignores lm, since 32-bit apps don't have that field. */
#define LDT_empty(info)					\
	((info)->base_addr		== 0	&&	\
	 (info)->limit			== 0	&&	\
	 (info)->contents		== 0	&&	\
	 (info)->read_exec_only		== 1	&&	\
	 (info)->seg_32bit		== 0	&&	\
	 (info)->limit_in_pages		== 0	&&	\
	 (info)->seg_not_present	== 1	&&	\
	 (info)->useable		== 0)

/* Lots of programs expect an all-zero user_desc to mean "no segment at all". */
static inline bool LDT_zero(const struct user_desc *info)
{
	return (info->base_addr		== 0 &&
		info->limit		== 0 &&
		info->contents		== 0 &&
		info->read_exec_only	== 0 &&
		info->seg_32bit		== 0 &&
		info->limit_in_pages	== 0 &&
		info->seg_not_present	== 0 &&
		info->useable		== 0);
}

static inline void clear_LDT(void)
{
	set_ldt(NULL, 0);
}

static inline unsigned long get_desc_base(const struct desc_struct *desc)
{
	return (unsigned)(desc->base0 | ((desc->base1) << 16) | ((desc->base2) << 24));
}

static inline void set_desc_base(struct desc_struct *desc, unsigned long base)
{
	desc->base0 = base & 0xffff;
	desc->base1 = (base >> 16) & 0xff;
	desc->base2 = (base >> 24) & 0xff;
}

static inline unsigned long get_desc_limit(const struct desc_struct *desc)
{
<<<<<<< HEAD
	return desc->limit0 | (desc->limit << 16);
=======
	return desc->limit0 | (desc->limit1 << 16);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void set_desc_limit(struct desc_struct *desc, unsigned long limit)
{
	desc->limit0 = limit & 0xffff;
<<<<<<< HEAD
	desc->limit = (limit >> 16) & 0xf;
}

#ifdef CONFIG_X86_64
static inline void set_nmi_gate(int gate, void *addr)
{
	gate_desc s;

	pack_gate(&s, GATE_INTERRUPT, (unsigned long)addr, 0, 0, __KERNEL_CS);
	write_idt_entry(nmi_idt_table, gate, &s);
}
#endif

static inline void _set_gate(int gate, unsigned type, void *addr,
			     unsigned dpl, unsigned ist, unsigned seg)
{
	gate_desc s;

	pack_gate(&s, type, (unsigned long)addr, dpl, ist, seg);
	/*
	 * does not need to be atomic because it is only done once at
	 * setup time
	 */
	write_idt_entry(idt_table, gate, &s);
}

/*
 * This needs to use 'idt_table' rather than 'idt', and
 * thus use the _nonmapped_ version of the IDT, as the
 * Pentium F0 0F bugfix can have resulted in the mapped
 * IDT being write-protected.
 */
static inline void set_intr_gate(unsigned int n, void *addr)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_INTERRUPT, addr, 0, 0, __KERNEL_CS);
}

extern int first_system_vector;
/* used_vectors is BITMAP for irq is not managed by percpu vector_irq */
extern unsigned long used_vectors[];

static inline void alloc_system_vector(int vector)
{
	if (!test_bit(vector, used_vectors)) {
		set_bit(vector, used_vectors);
		if (first_system_vector > vector)
			first_system_vector = vector;
	} else {
		BUG();
	}
}

static inline void alloc_intr_gate(unsigned int n, void *addr)
{
	alloc_system_vector(n);
	set_intr_gate(n, addr);
}

/*
 * This routine sets up an interrupt gate at directory privilege level 3.
 */
static inline void set_system_intr_gate(unsigned int n, void *addr)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_INTERRUPT, addr, 0x3, 0, __KERNEL_CS);
}

static inline void set_system_trap_gate(unsigned int n, void *addr)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_TRAP, addr, 0x3, 0, __KERNEL_CS);
}

static inline void set_trap_gate(unsigned int n, void *addr)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_TRAP, addr, 0, 0, __KERNEL_CS);
}

static inline void set_task_gate(unsigned int n, unsigned int gdt_entry)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_TASK, (void *)0, 0, 0, (gdt_entry<<3));
}

static inline void set_intr_gate_ist(int n, void *addr, unsigned ist)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_INTERRUPT, addr, 0, ist, __KERNEL_CS);
}

static inline void set_system_intr_gate_ist(int n, void *addr, unsigned ist)
{
	BUG_ON((unsigned)n > 0xFF);
	_set_gate(n, GATE_INTERRUPT, addr, 0x3, ist, __KERNEL_CS);
}
=======
	desc->limit1 = (limit >> 16) & 0xf;
}

static inline void init_idt_data(struct idt_data *data, unsigned int n,
				 const void *addr)
{
	BUG_ON(n > 0xFF);

	memset(data, 0, sizeof(*data));
	data->vector	= n;
	data->addr	= addr;
	data->segment	= __KERNEL_CS;
	data->bits.type	= GATE_INTERRUPT;
	data->bits.p	= 1;
}

static inline void idt_init_desc(gate_desc *gate, const struct idt_data *d)
{
	unsigned long addr = (unsigned long) d->addr;

	gate->offset_low	= (u16) addr;
	gate->segment		= (u16) d->segment;
	gate->bits		= d->bits;
	gate->offset_middle	= (u16) (addr >> 16);
#ifdef CONFIG_X86_64
	gate->offset_high	= (u32) (addr >> 32);
	gate->reserved		= 0;
#endif
}

extern unsigned long system_vectors[];

extern void load_current_idt(void);
extern void idt_setup_early_handler(void);
extern void idt_setup_early_traps(void);
extern void idt_setup_traps(void);
extern void idt_setup_apic_and_irq_gates(void);
extern bool idt_is_f00f_address(unsigned long address);

#ifdef CONFIG_X86_64
extern void idt_setup_early_pf(void);
#else
static inline void idt_setup_early_pf(void) { }
#endif

extern void idt_invalidate(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_DESC_H */
