<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  Copyright (C) 2001 Rusty Russell.
 *  Copyright (C) 2003, 2004 Ralf Baechle (ralf@linux-mips.org)
 *  Copyright (C) 2005 Thiemo Seufer
 */

#undef DEBUG

<<<<<<< HEAD
#include <linux/moduleloader.h>
#include <linux/elf.h>
#include <linux/mm.h>
=======
#include <linux/extable.h>
#include <linux/moduleloader.h>
#include <linux/elf.h>
#include <linux/mm.h>
#include <linux/numa.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/jump_label.h>
<<<<<<< HEAD

#include <asm/pgtable.h>	/* MODULE_START */
=======
#include <asm/jump_label.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct mips_hi16 {
	struct mips_hi16 *next;
	Elf_Addr *addr;
	Elf_Addr value;
};

<<<<<<< HEAD
static struct mips_hi16 *mips_hi16_list;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static LIST_HEAD(dbe_list);
static DEFINE_SPINLOCK(dbe_lock);

#ifdef MODULE_START
void *module_alloc(unsigned long size)
{
	return __vmalloc_node_range(size, 1, MODULE_START, MODULE_END,
<<<<<<< HEAD
				GFP_KERNEL, PAGE_KERNEL, -1,
=======
				GFP_KERNEL, PAGE_KERNEL, 0, NUMA_NO_NODE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				__builtin_return_address(0));
}
#endif

<<<<<<< HEAD
static int apply_r_mips_none(struct module *me, u32 *location, Elf_Addr v)
{
	return 0;
}

static int apply_r_mips_32_rel(struct module *me, u32 *location, Elf_Addr v)
{
	*location += v;

	return 0;
}

static int apply_r_mips_32_rela(struct module *me, u32 *location, Elf_Addr v)
{
	*location = v;

	return 0;
}

static int apply_r_mips_26_rel(struct module *me, u32 *location, Elf_Addr v)
{
	if (v % 4) {
		pr_err("module %s: dangerous R_MIPS_26 REL relocation\n",
=======
static void apply_r_mips_32(u32 *location, u32 base, Elf_Addr v)
{
	*location = base + v;
}

static int apply_r_mips_26(struct module *me, u32 *location, u32 base,
			   Elf_Addr v)
{
	if (v % 4) {
		pr_err("module %s: dangerous R_MIPS_26 relocation\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       me->name);
		return -ENOEXEC;
	}

	if ((v & 0xf0000000) != (((unsigned long)location + 4) & 0xf0000000)) {
<<<<<<< HEAD
		printk(KERN_ERR
		       "module %s: relocation overflow\n",
=======
		pr_err("module %s: relocation overflow\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       me->name);
		return -ENOEXEC;
	}

	*location = (*location & ~0x03ffffff) |
<<<<<<< HEAD
	            ((*location + (v >> 2)) & 0x03ffffff);
=======
		    ((base + (v >> 2)) & 0x03ffffff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int apply_r_mips_26_rela(struct module *me, u32 *location, Elf_Addr v)
{
	if (v % 4) {
		pr_err("module %s: dangerous R_MIPS_26 RELArelocation\n",
		       me->name);
		return -ENOEXEC;
	}

	if ((v & 0xf0000000) != (((unsigned long)location + 4) & 0xf0000000)) {
		printk(KERN_ERR
		       "module %s: relocation overflow\n",
		       me->name);
		return -ENOEXEC;
	}

	*location = (*location & ~0x03ffffff) | ((v >> 2) & 0x03ffffff);

	return 0;
}

static int apply_r_mips_hi16_rel(struct module *me, u32 *location, Elf_Addr v)
{
	struct mips_hi16 *n;

=======
static int apply_r_mips_hi16(struct module *me, u32 *location, Elf_Addr v,
			     bool rela)
{
	struct mips_hi16 *n;

	if (rela) {
		*location = (*location & 0xffff0000) |
			    ((((long long) v + 0x8000LL) >> 16) & 0xffff);
		return 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We cannot relocate this one now because we don't know the value of
	 * the carry we need to add.  Save the information, and let LO16 do the
	 * actual relocation.
	 */
	n = kmalloc(sizeof *n, GFP_KERNEL);
	if (!n)
		return -ENOMEM;

	n->addr = (Elf_Addr *)location;
	n->value = v;
<<<<<<< HEAD
	n->next = mips_hi16_list;
	mips_hi16_list = n;
=======
	n->next = me->arch.r_mips_hi16_list;
	me->arch.r_mips_hi16_list = n;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int apply_r_mips_hi16_rela(struct module *me, u32 *location, Elf_Addr v)
{
	*location = (*location & 0xffff0000) |
	            ((((long long) v + 0x8000LL) >> 16) & 0xffff);

	return 0;
}

static int apply_r_mips_lo16_rel(struct module *me, u32 *location, Elf_Addr v)
{
	unsigned long insnlo = *location;
	Elf_Addr val, vallo;

	/* Sign extend the addend we extract from the lo insn.  */
	vallo = ((insnlo & 0xffff) ^ 0x8000) - 0x8000;

	if (mips_hi16_list != NULL) {
		struct mips_hi16 *l;

		l = mips_hi16_list;
=======
static void free_relocation_chain(struct mips_hi16 *l)
{
	struct mips_hi16 *next;

	while (l) {
		next = l->next;
		kfree(l);
		l = next;
	}
}

static int apply_r_mips_lo16(struct module *me, u32 *location,
			     u32 base, Elf_Addr v, bool rela)
{
	unsigned long insnlo = base;
	struct mips_hi16 *l;
	Elf_Addr val, vallo;

	if (rela) {
		*location = (*location & 0xffff0000) | (v & 0xffff);
		return 0;
	}

	/* Sign extend the addend we extract from the lo insn.	*/
	vallo = ((insnlo & 0xffff) ^ 0x8000) - 0x8000;

	if (me->arch.r_mips_hi16_list != NULL) {
		l = me->arch.r_mips_hi16_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (l != NULL) {
			struct mips_hi16 *next;
			unsigned long insn;

			/*
			 * The value for the HI16 had best be the same.
			 */
			if (v != l->value)
				goto out_danger;

			/*
			 * Do the HI16 relocation.  Note that we actually don't
			 * need to know anything about the LO16 itself, except
			 * where to find the low 16 bits of the addend needed
			 * by the LO16.
			 */
			insn = *l->addr;
			val = ((insn & 0xffff) << 16) + vallo;
			val += v;

			/*
			 * Account for the sign extension that will happen in
			 * the low bits.
			 */
			val = ((val >> 16) + ((val & 0x8000) != 0)) & 0xffff;

			insn = (insn & ~0xffff) | val;
			*l->addr = insn;

			next = l->next;
			kfree(l);
			l = next;
		}

<<<<<<< HEAD
		mips_hi16_list = NULL;
	}

	/*
	 * Ok, we're done with the HI16 relocs.  Now deal with the LO16.
=======
		me->arch.r_mips_hi16_list = NULL;
	}

	/*
	 * Ok, we're done with the HI16 relocs.	 Now deal with the LO16.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	val = v + vallo;
	insnlo = (insnlo & ~0xffff) | (val & 0xffff);
	*location = insnlo;

	return 0;

out_danger:
<<<<<<< HEAD
	pr_err("module %s: dangerous R_MIPS_LO16 REL relocation\n", me->name);
=======
	free_relocation_chain(l);
	me->arch.r_mips_hi16_list = NULL;

	pr_err("module %s: dangerous R_MIPS_LO16 relocation\n", me->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return -ENOEXEC;
}

<<<<<<< HEAD
static int apply_r_mips_lo16_rela(struct module *me, u32 *location, Elf_Addr v)
{
	*location = (*location & 0xffff0000) | (v & 0xffff);
=======
static int apply_r_mips_pc(struct module *me, u32 *location, u32 base,
			   Elf_Addr v, unsigned int bits)
{
	unsigned long mask = GENMASK(bits - 1, 0);
	unsigned long se_bits;
	long offset;

	if (v % 4) {
		pr_err("module %s: dangerous R_MIPS_PC%u relocation\n",
		       me->name, bits);
		return -ENOEXEC;
	}

	/* retrieve & sign extend implicit addend if any */
	offset = base & mask;
	offset |= (offset & BIT(bits - 1)) ? ~mask : 0;

	offset += ((long)v - (long)location) >> 2;

	/* check the sign bit onwards are identical - ie. we didn't overflow */
	se_bits = (offset & BIT(bits - 1)) ? ~0ul : 0;
	if ((offset & ~mask) != (se_bits & ~mask)) {
		pr_err("module %s: relocation overflow\n", me->name);
		return -ENOEXEC;
	}

	*location = (*location & ~mask) | (offset & mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int apply_r_mips_64_rela(struct module *me, u32 *location, Elf_Addr v)
{
=======
static int apply_r_mips_pc16(struct module *me, u32 *location, u32 base,
			     Elf_Addr v)
{
	return apply_r_mips_pc(me, location, base, v, 16);
}

static int apply_r_mips_pc21(struct module *me, u32 *location, u32 base,
			     Elf_Addr v)
{
	return apply_r_mips_pc(me, location, base, v, 21);
}

static int apply_r_mips_pc26(struct module *me, u32 *location, u32 base,
			     Elf_Addr v)
{
	return apply_r_mips_pc(me, location, base, v, 26);
}

static int apply_r_mips_64(u32 *location, Elf_Addr v, bool rela)
{
	if (WARN_ON(!rela))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*(Elf_Addr *)location = v;

	return 0;
}

<<<<<<< HEAD
static int apply_r_mips_higher_rela(struct module *me, u32 *location,
				    Elf_Addr v)
{
	*location = (*location & 0xffff0000) |
	            ((((long long) v + 0x80008000LL) >> 32) & 0xffff);
=======
static int apply_r_mips_higher(u32 *location, Elf_Addr v, bool rela)
{
	if (WARN_ON(!rela))
		return -EINVAL;

	*location = (*location & 0xffff0000) |
		    ((((long long)v + 0x80008000LL) >> 32) & 0xffff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int apply_r_mips_highest_rela(struct module *me, u32 *location,
				     Elf_Addr v)
{
	*location = (*location & 0xffff0000) |
	            ((((long long) v + 0x800080008000LL) >> 48) & 0xffff);
=======
static int apply_r_mips_highest(u32 *location, Elf_Addr v, bool rela)
{
	if (WARN_ON(!rela))
		return -EINVAL;

	*location = (*location & 0xffff0000) |
		    ((((long long)v + 0x800080008000LL) >> 48) & 0xffff);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int (*reloc_handlers_rel[]) (struct module *me, u32 *location,
				Elf_Addr v) = {
	[R_MIPS_NONE]		= apply_r_mips_none,
	[R_MIPS_32]		= apply_r_mips_32_rel,
	[R_MIPS_26]		= apply_r_mips_26_rel,
	[R_MIPS_HI16]		= apply_r_mips_hi16_rel,
	[R_MIPS_LO16]		= apply_r_mips_lo16_rel
};

static int (*reloc_handlers_rela[]) (struct module *me, u32 *location,
				Elf_Addr v) = {
	[R_MIPS_NONE]		= apply_r_mips_none,
	[R_MIPS_32]		= apply_r_mips_32_rela,
	[R_MIPS_26]		= apply_r_mips_26_rela,
	[R_MIPS_HI16]		= apply_r_mips_hi16_rela,
	[R_MIPS_LO16]		= apply_r_mips_lo16_rela,
	[R_MIPS_64]		= apply_r_mips_64_rela,
	[R_MIPS_HIGHER]		= apply_r_mips_higher_rela,
	[R_MIPS_HIGHEST]	= apply_r_mips_highest_rela
};
=======
/**
 * reloc_handler() - Apply a particular relocation to a module
 * @type: type of the relocation to apply
 * @me: the module to apply the reloc to
 * @location: the address at which the reloc is to be applied
 * @base: the existing value at location for REL-style; 0 for RELA-style
 * @v: the value of the reloc, with addend for RELA-style
 * @rela: indication of is this a RELA (true) or REL (false) relocation
 *
 * Each implemented relocation function applies a particular type of
 * relocation to the module @me. Relocs that may be found in either REL or RELA
 * variants can be handled by making use of the @base & @v parameters which are
 * set to values which abstract the difference away from the particular reloc
 * implementations.
 *
 * Return: 0 upon success, else -ERRNO
 */
static int reloc_handler(u32 type, struct module *me, u32 *location, u32 base,
			 Elf_Addr v, bool rela)
{
	switch (type) {
	case R_MIPS_NONE:
		break;
	case R_MIPS_32:
		apply_r_mips_32(location, base, v);
		break;
	case R_MIPS_26:
		return apply_r_mips_26(me, location, base, v);
	case R_MIPS_HI16:
		return apply_r_mips_hi16(me, location, v, rela);
	case R_MIPS_LO16:
		return apply_r_mips_lo16(me, location, base, v, rela);
	case R_MIPS_PC16:
		return apply_r_mips_pc16(me, location, base, v);
	case R_MIPS_PC21_S2:
		return apply_r_mips_pc21(me, location, base, v);
	case R_MIPS_PC26_S2:
		return apply_r_mips_pc26(me, location, base, v);
	case R_MIPS_64:
		return apply_r_mips_64(location, v, rela);
	case R_MIPS_HIGHER:
		return apply_r_mips_higher(location, v, rela);
	case R_MIPS_HIGHEST:
		return apply_r_mips_highest(location, v, rela);
	default:
		pr_err("%s: Unknown relocation type %u\n", me->name, type);
		return -EINVAL;
	}

	return 0;
}

static int __apply_relocate(Elf_Shdr *sechdrs, const char *strtab,
			    unsigned int symindex, unsigned int relsec,
			    struct module *me, bool rela)
{
	union {
		Elf_Mips_Rel *rel;
		Elf_Mips_Rela *rela;
	} r;
	Elf_Sym *sym;
	u32 *location, base;
	unsigned int i, type;
	Elf_Addr v;
	int err = 0;
	size_t reloc_sz;

	pr_debug("Applying relocate section %u to %u\n", relsec,
	       sechdrs[relsec].sh_info);

	r.rel = (void *)sechdrs[relsec].sh_addr;
	reloc_sz = rela ? sizeof(*r.rela) : sizeof(*r.rel);
	me->arch.r_mips_hi16_list = NULL;
	for (i = 0; i < sechdrs[relsec].sh_size / reloc_sz; i++) {
		/* This is where to make the change */
		location = (void *)sechdrs[sechdrs[relsec].sh_info].sh_addr
			+ r.rel->r_offset;
		/* This is the symbol it is referring to */
		sym = (Elf_Sym *)sechdrs[symindex].sh_addr
			+ ELF_MIPS_R_SYM(*r.rel);
		if (sym->st_value >= -MAX_ERRNO) {
			/* Ignore unresolved weak symbol */
			if (ELF_ST_BIND(sym->st_info) == STB_WEAK)
				continue;
			pr_warn("%s: Unknown symbol %s\n",
				me->name, strtab + sym->st_name);
			err = -ENOENT;
			goto out;
		}

		type = ELF_MIPS_R_TYPE(*r.rel);

		if (rela) {
			v = sym->st_value + r.rela->r_addend;
			base = 0;
			r.rela = &r.rela[1];
		} else {
			v = sym->st_value;
			base = *location;
			r.rel = &r.rel[1];
		}

		err = reloc_handler(type, me, location, base, v, rela);
		if (err)
			goto out;
	}

out:
	/*
	 * Normally the hi16 list should be deallocated at this point. A
	 * malformed binary however could contain a series of R_MIPS_HI16
	 * relocations not followed by a R_MIPS_LO16 relocation, or if we hit
	 * an error processing a reloc we might have gotten here before
	 * reaching the R_MIPS_LO16. In either case, free up the list and
	 * return an error.
	 */
	if (me->arch.r_mips_hi16_list) {
		free_relocation_chain(me->arch.r_mips_hi16_list);
		me->arch.r_mips_hi16_list = NULL;
		err = err ?: -ENOEXEC;
	}

	return err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int apply_relocate(Elf_Shdr *sechdrs, const char *strtab,
		   unsigned int symindex, unsigned int relsec,
		   struct module *me)
{
<<<<<<< HEAD
	Elf_Mips_Rel *rel = (void *) sechdrs[relsec].sh_addr;
	Elf_Sym *sym;
	u32 *location;
	unsigned int i;
	Elf_Addr v;
	int res;

	pr_debug("Applying relocate section %u to %u\n", relsec,
	       sechdrs[relsec].sh_info);

	for (i = 0; i < sechdrs[relsec].sh_size / sizeof(*rel); i++) {
		/* This is where to make the change */
		location = (void *)sechdrs[sechdrs[relsec].sh_info].sh_addr
			+ rel[i].r_offset;
		/* This is the symbol it is referring to */
		sym = (Elf_Sym *)sechdrs[symindex].sh_addr
			+ ELF_MIPS_R_SYM(rel[i]);
		if (IS_ERR_VALUE(sym->st_value)) {
			/* Ignore unresolved weak symbol */
			if (ELF_ST_BIND(sym->st_info) == STB_WEAK)
				continue;
			printk(KERN_WARNING "%s: Unknown symbol %s\n",
			       me->name, strtab + sym->st_name);
			return -ENOENT;
		}

		v = sym->st_value;

		res = reloc_handlers_rel[ELF_MIPS_R_TYPE(rel[i])](me, location, v);
		if (res)
			return res;
	}

	return 0;
}

=======
	return __apply_relocate(sechdrs, strtab, symindex, relsec, me, false);
}

#ifdef CONFIG_MODULES_USE_ELF_RELA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int apply_relocate_add(Elf_Shdr *sechdrs, const char *strtab,
		       unsigned int symindex, unsigned int relsec,
		       struct module *me)
{
<<<<<<< HEAD
	Elf_Mips_Rela *rel = (void *) sechdrs[relsec].sh_addr;
	Elf_Sym *sym;
	u32 *location;
	unsigned int i;
	Elf_Addr v;
	int res;

	pr_debug("Applying relocate section %u to %u\n", relsec,
	       sechdrs[relsec].sh_info);

	for (i = 0; i < sechdrs[relsec].sh_size / sizeof(*rel); i++) {
		/* This is where to make the change */
		location = (void *)sechdrs[sechdrs[relsec].sh_info].sh_addr
			+ rel[i].r_offset;
		/* This is the symbol it is referring to */
		sym = (Elf_Sym *)sechdrs[symindex].sh_addr
			+ ELF_MIPS_R_SYM(rel[i]);
		if (IS_ERR_VALUE(sym->st_value)) {
			/* Ignore unresolved weak symbol */
			if (ELF_ST_BIND(sym->st_info) == STB_WEAK)
				continue;
			printk(KERN_WARNING "%s: Unknown symbol %s\n",
			       me->name, strtab + sym->st_name);
			return -ENOENT;
		}

		v = sym->st_value + rel[i].r_addend;

		res = reloc_handlers_rela[ELF_MIPS_R_TYPE(rel[i])](me, location, v);
		if (res)
			return res;
	}

	return 0;
}
=======
	return __apply_relocate(sechdrs, strtab, symindex, relsec, me, true);
}
#endif /* CONFIG_MODULES_USE_ELF_RELA */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Given an address, look for it in the module exception tables. */
const struct exception_table_entry *search_module_dbetables(unsigned long addr)
{
	unsigned long flags;
	const struct exception_table_entry *e = NULL;
	struct mod_arch_specific *dbe;

	spin_lock_irqsave(&dbe_lock, flags);
	list_for_each_entry(dbe, &dbe_list, dbe_list) {
<<<<<<< HEAD
		e = search_extable(dbe->dbe_start, dbe->dbe_end - 1, addr);
=======
		e = search_extable(dbe->dbe_start,
				   dbe->dbe_end - dbe->dbe_start, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (e)
			break;
	}
	spin_unlock_irqrestore(&dbe_lock, flags);

	/* Now, if we found one, we are running inside it now, hence
<<<<<<< HEAD
           we cannot unload the module, hence no refcnt needed. */
=======
	   we cannot unload the module, hence no refcnt needed. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return e;
}

/* Put in dbe list if necessary. */
int module_finalize(const Elf_Ehdr *hdr,
		    const Elf_Shdr *sechdrs,
		    struct module *me)
{
	const Elf_Shdr *s;
	char *secstrings = (void *)hdr + sechdrs[hdr->e_shstrndx].sh_offset;

<<<<<<< HEAD
	/* Make jump label nops. */
	jump_label_apply_nops(me);
=======
	if (IS_ENABLED(CONFIG_JUMP_LABEL))
		jump_label_apply_nops(me);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	INIT_LIST_HEAD(&me->arch.dbe_list);
	for (s = sechdrs; s < sechdrs + hdr->e_shnum; s++) {
		if (strcmp("__dbe_table", secstrings + s->sh_name) != 0)
			continue;
		me->arch.dbe_start = (void *)s->sh_addr;
		me->arch.dbe_end = (void *)s->sh_addr + s->sh_size;
		spin_lock_irq(&dbe_lock);
		list_add(&me->arch.dbe_list, &dbe_list);
		spin_unlock_irq(&dbe_lock);
	}
	return 0;
}

void module_arch_cleanup(struct module *mod)
{
	spin_lock_irq(&dbe_lock);
	list_del(&mod->arch.dbe_list);
	spin_unlock_irq(&dbe_lock);
}
