<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Written 2000 by Andi Kleen */
#ifndef _ASM_X86_DESC_DEFS_H
#define _ASM_X86_DESC_DEFS_H

/*
 * Segment descriptor structure definitions, usable from both x86_64 and i386
 * archs.
 */

<<<<<<< HEAD
=======
/*
 * Low-level interface mapping flags/field names to bits
 */

/* Flags for _DESC_S (non-system) descriptors */
#define _DESC_ACCESSED		0x0001
#define _DESC_DATA_WRITABLE	0x0002
#define _DESC_CODE_READABLE	0x0002
#define _DESC_DATA_EXPAND_DOWN	0x0004
#define _DESC_CODE_CONFORMING	0x0004
#define _DESC_CODE_EXECUTABLE	0x0008

/* Common flags */
#define _DESC_S			0x0010
#define _DESC_DPL(dpl)		((dpl) << 5)
#define _DESC_PRESENT		0x0080

#define _DESC_LONG_CODE		0x2000
#define _DESC_DB		0x4000
#define _DESC_GRANULARITY_4K	0x8000

/* System descriptors have a numeric "type" field instead of flags */
#define _DESC_SYSTEM(code)	(code)

/*
 * High-level interface mapping intended usage to low-level combinations
 * of flags
 */

#define _DESC_DATA		(_DESC_S | _DESC_PRESENT | _DESC_ACCESSED | \
				 _DESC_DATA_WRITABLE)
#define _DESC_CODE		(_DESC_S | _DESC_PRESENT | _DESC_ACCESSED | \
				 _DESC_CODE_READABLE | _DESC_CODE_EXECUTABLE)

#define DESC_DATA16		(_DESC_DATA)
#define DESC_CODE16		(_DESC_CODE)

#define DESC_DATA32		(_DESC_DATA | _DESC_GRANULARITY_4K | _DESC_DB)
#define DESC_DATA32_BIOS	(_DESC_DATA | _DESC_DB)

#define DESC_CODE32		(_DESC_CODE | _DESC_GRANULARITY_4K | _DESC_DB)
#define DESC_CODE32_BIOS	(_DESC_CODE | _DESC_DB)

#define DESC_TSS32		(_DESC_SYSTEM(9) | _DESC_PRESENT)

#define DESC_DATA64		(_DESC_DATA | _DESC_GRANULARITY_4K | _DESC_DB)
#define DESC_CODE64		(_DESC_CODE | _DESC_GRANULARITY_4K | _DESC_LONG_CODE)

#define DESC_USER		(_DESC_DPL(3))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASSEMBLY__

#include <linux/types.h>

<<<<<<< HEAD
/*
 * FIXME: Accessing the desc_struct through its fields is more elegant,
 * and should be the one valid thing to do. However, a lot of open code
 * still touches the a and b accessors, and doing this allow us to do it
 * incrementally. We keep the signature as a struct, rather than an union,
 * so we can get rid of it transparently in the future -- glommer
 */
/* 8 byte segment descriptor */
struct desc_struct {
	union {
		struct {
			unsigned int a;
			unsigned int b;
		};
		struct {
			u16 limit0;
			u16 base0;
			unsigned base1: 8, type: 4, s: 1, dpl: 2, p: 1;
			unsigned limit: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
		};
	};
} __attribute__((packed));

#define GDT_ENTRY_INIT(flags, base, limit) { { { \
		.a = ((limit) & 0xffff) | (((base) & 0xffff) << 16), \
		.b = (((base) & 0xff0000) >> 16) | (((flags) & 0xf0ff) << 8) | \
			((limit) & 0xf0000) | ((base) & 0xff000000), \
	} } }
=======
/* 8 byte segment descriptor */
struct desc_struct {
	u16	limit0;
	u16	base0;
	u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
	u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
} __attribute__((packed));

#define GDT_ENTRY_INIT(flags, base, limit)			\
	{							\
		.limit0		= ((limit) >>  0) & 0xFFFF,	\
		.limit1		= ((limit) >> 16) & 0x000F,	\
		.base0		= ((base)  >>  0) & 0xFFFF,	\
		.base1		= ((base)  >> 16) & 0x00FF,	\
		.base2		= ((base)  >> 24) & 0x00FF,	\
		.type		= ((flags) >>  0) & 0x000F,	\
		.s		= ((flags) >>  4) & 0x0001,	\
		.dpl		= ((flags) >>  5) & 0x0003,	\
		.p		= ((flags) >>  7) & 0x0001,	\
		.avl		= ((flags) >> 12) & 0x0001,	\
		.l		= ((flags) >> 13) & 0x0001,	\
		.d		= ((flags) >> 14) & 0x0001,	\
		.g		= ((flags) >> 15) & 0x0001,	\
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum {
	GATE_INTERRUPT = 0xE,
	GATE_TRAP = 0xF,
	GATE_CALL = 0xC,
	GATE_TASK = 0x5,
};

<<<<<<< HEAD
/* 16byte gate */
struct gate_struct64 {
	u16 offset_low;
	u16 segment;
	unsigned ist : 3, zero0 : 5, type : 5, dpl : 2, p : 1;
	u16 offset_middle;
	u32 offset_high;
	u32 zero1;
} __attribute__((packed));

#define PTR_LOW(x) ((unsigned long long)(x) & 0xFFFF)
#define PTR_MIDDLE(x) (((unsigned long long)(x) >> 16) & 0xFFFF)
#define PTR_HIGH(x) ((unsigned long long)(x) >> 32)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum {
	DESC_TSS = 0x9,
	DESC_LDT = 0x2,
	DESCTYPE_S = 0x10,	/* !system */
};

<<<<<<< HEAD
/* LDT or TSS descriptor in the GDT. 16 bytes. */
struct ldttss_desc64 {
	u16 limit0;
	u16 base0;
	unsigned base1 : 8, type : 5, dpl : 2, p : 1;
	unsigned limit1 : 4, zero0 : 3, g : 1, base2 : 8;
	u32 base3;
	u32 zero1;
} __attribute__((packed));

#ifdef CONFIG_X86_64
typedef struct gate_struct64 gate_desc;
typedef struct ldttss_desc64 ldt_desc;
typedef struct ldttss_desc64 tss_desc;
#define gate_offset(g) ((g).offset_low | ((unsigned long)(g).offset_middle << 16) | ((unsigned long)(g).offset_high << 32))
#define gate_segment(g) ((g).segment)
#else
typedef struct desc_struct gate_desc;
typedef struct desc_struct ldt_desc;
typedef struct desc_struct tss_desc;
#define gate_offset(g)		(((g).b & 0xffff0000) | ((g).a & 0x0000ffff))
#define gate_segment(g)		((g).a >> 16)
=======
/* LDT or TSS descriptor in the GDT. */
struct ldttss_desc {
	u16	limit0;
	u16	base0;

	u16	base1 : 8, type : 5, dpl : 2, p : 1;
	u16	limit1 : 4, zero0 : 3, g : 1, base2 : 8;
#ifdef CONFIG_X86_64
	u32	base3;
	u32	zero1;
#endif
} __attribute__((packed));

typedef struct ldttss_desc ldt_desc;
typedef struct ldttss_desc tss_desc;

struct idt_bits {
	u16		ist	: 3,
			zero	: 5,
			type	: 5,
			dpl	: 2,
			p	: 1;
} __attribute__((packed));

struct idt_data {
	unsigned int	vector;
	unsigned int	segment;
	struct idt_bits	bits;
	const void	*addr;
};

struct gate_struct {
	u16		offset_low;
	u16		segment;
	struct idt_bits	bits;
	u16		offset_middle;
#ifdef CONFIG_X86_64
	u32		offset_high;
	u32		reserved;
#endif
} __attribute__((packed));

typedef struct gate_struct gate_desc;

#ifndef _SETUP
static inline unsigned long gate_offset(const gate_desc *g)
{
#ifdef CONFIG_X86_64
	return g->offset_low | ((unsigned long)g->offset_middle << 16) |
		((unsigned long) g->offset_high << 32);
#else
	return g->offset_low | ((unsigned long)g->offset_middle << 16);
#endif
}

static inline unsigned long gate_segment(const gate_desc *g)
{
	return g->segment;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

struct desc_ptr {
	unsigned short size;
	unsigned long address;
} __attribute__((packed)) ;

#endif /* !__ASSEMBLY__ */

<<<<<<< HEAD
=======
/* Boot IDT definitions */
#define	BOOT_IDT_ENTRIES	32

/* Access rights as returned by LAR */
#define AR_TYPE_RODATA		(0 * (1 << 9))
#define AR_TYPE_RWDATA		(1 * (1 << 9))
#define AR_TYPE_RODATA_EXPDOWN	(2 * (1 << 9))
#define AR_TYPE_RWDATA_EXPDOWN	(3 * (1 << 9))
#define AR_TYPE_XOCODE		(4 * (1 << 9))
#define AR_TYPE_XRCODE		(5 * (1 << 9))
#define AR_TYPE_XOCODE_CONF	(6 * (1 << 9))
#define AR_TYPE_XRCODE_CONF	(7 * (1 << 9))
#define AR_TYPE_MASK		(7 * (1 << 9))

#define AR_DPL0			(0 * (1 << 13))
#define AR_DPL3			(3 * (1 << 13))
#define AR_DPL_MASK		(3 * (1 << 13))

#define AR_A			(1 << 8)   /* "Accessed" */
#define AR_S			(1 << 12)  /* If clear, "System" segment */
#define AR_P			(1 << 15)  /* "Present" */
#define AR_AVL			(1 << 20)  /* "AVaiLable" (no HW effect) */
#define AR_L			(1 << 21)  /* "Long mode" for code segments */
#define AR_DB			(1 << 22)  /* D/B, effect depends on type */
#define AR_G			(1 << 23)  /* "Granularity" (limit in pages) */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_DESC_DEFS_H */
