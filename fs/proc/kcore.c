<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	fs/proc/kcore.c kernel ELF core dumper
 *
 *	Modelled on fs/exec.c:aout_core_dump()
 *	Jeremy Fitzhardinge <jeremy@sw.oz.au>
 *	ELF version written by David Howells <David.Howells@nexor.co.uk>
 *	Modified and incorporated into 2.3.x by Tigran Aivazian <tigran@veritas.com>
 *	Support to dump vmalloc'd areas (ELF only), Tigran Aivazian <tigran@veritas.com>
 *	Safe accesses to vmalloc/direct-mapped discontiguous areas, Kanoj Sarcar <kanoj@sgi.com>
 */

<<<<<<< HEAD
#include <linux/mm.h>
#include <linux/proc_fs.h>
=======
#include <linux/vmcore_info.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/kcore.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/user.h>
#include <linux/capability.h>
#include <linux/elf.h>
#include <linux/elfcore.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
=======
#include <linux/printk.h>
#include <linux/memblock.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uio.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/io.h>
#include <linux/list.h>
#include <linux/ioport.h>
#include <linux/memory.h>
<<<<<<< HEAD
#include <asm/sections.h>
=======
#include <linux/sched/task.h>
#include <linux/security.h>
#include <asm/sections.h>
#include "internal.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CORE_STR "CORE"

#ifndef ELF_CORE_EFLAGS
#define ELF_CORE_EFLAGS	0
#endif

static struct proc_dir_entry *proc_root_kcore;


#ifndef kc_vaddr_to_offset
#define	kc_vaddr_to_offset(v) ((v) - PAGE_OFFSET)
#endif
#ifndef	kc_offset_to_vaddr
#define	kc_offset_to_vaddr(o) ((o) + PAGE_OFFSET)
#endif

<<<<<<< HEAD
/* An ELF note in memory */
struct memelfnote
{
	const char *name;
	int type;
	unsigned int datasz;
	void *data;
};

static LIST_HEAD(kclist_head);
static DEFINE_RWLOCK(kclist_lock);
static int kcore_need_update = 1;

void
kclist_add(struct kcore_list *new, void *addr, size_t size, int type)
=======
static LIST_HEAD(kclist_head);
static DECLARE_RWSEM(kclist_lock);
static int kcore_need_update = 1;

/*
 * Returns > 0 for RAM pages, 0 for non-RAM pages, < 0 on error
 * Same as oldmem_pfn_is_ram in vmcore
 */
static int (*mem_pfn_is_ram)(unsigned long pfn);

int __init register_mem_pfn_is_ram(int (*fn)(unsigned long pfn))
{
	if (mem_pfn_is_ram)
		return -EBUSY;
	mem_pfn_is_ram = fn;
	return 0;
}

static int pfn_is_ram(unsigned long pfn)
{
	if (mem_pfn_is_ram)
		return mem_pfn_is_ram(pfn);
	else
		return 1;
}

/* This doesn't grab kclist_lock, so it should only be used at init time. */
void __init kclist_add(struct kcore_list *new, void *addr, size_t size,
		       int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	new->addr = (unsigned long)addr;
	new->size = size;
	new->type = type;

<<<<<<< HEAD
	write_lock(&kclist_lock);
	list_add_tail(&new->list, &kclist_head);
	write_unlock(&kclist_lock);
}

static size_t get_kcore_size(int *nphdr, size_t *elf_buflen)
=======
	list_add_tail(&new->list, &kclist_head);
}

static size_t get_kcore_size(int *nphdr, size_t *phdrs_len, size_t *notes_len,
			     size_t *data_offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	size_t try, size;
	struct kcore_list *m;

	*nphdr = 1; /* PT_NOTE */
	size = 0;

	list_for_each_entry(m, &kclist_head, list) {
		try = kc_vaddr_to_offset((size_t)m->addr + m->size);
		if (try > size)
			size = try;
		*nphdr = *nphdr + 1;
	}
<<<<<<< HEAD
	*elf_buflen =	sizeof(struct elfhdr) + 
			(*nphdr + 2)*sizeof(struct elf_phdr) + 
			3 * ((sizeof(struct elf_note)) +
			     roundup(sizeof(CORE_STR), 4)) +
			roundup(sizeof(struct elf_prstatus), 4) +
			roundup(sizeof(struct elf_prpsinfo), 4) +
			roundup(sizeof(struct task_struct), 4);
	*elf_buflen = PAGE_ALIGN(*elf_buflen);
	return size + *elf_buflen;
}

static void free_kclist_ents(struct list_head *head)
{
	struct kcore_list *tmp, *pos;

	list_for_each_entry_safe(pos, tmp, head, list) {
		list_del(&pos->list);
		kfree(pos);
	}
}
/*
 * Replace all KCORE_RAM/KCORE_VMEMMAP information with passed list.
 */
static void __kcore_update_ram(struct list_head *list)
{
	int nphdr;
	size_t size;
	struct kcore_list *tmp, *pos;
	LIST_HEAD(garbage);

	write_lock(&kclist_lock);
	if (kcore_need_update) {
		list_for_each_entry_safe(pos, tmp, &kclist_head, list) {
			if (pos->type == KCORE_RAM
				|| pos->type == KCORE_VMEMMAP)
				list_move(&pos->list, &garbage);
		}
		list_splice_tail(list, &kclist_head);
	} else
		list_splice(list, &garbage);
	kcore_need_update = 0;
	proc_root_kcore->size = get_kcore_size(&nphdr, &size);
	write_unlock(&kclist_lock);

	free_kclist_ents(&garbage);
}


=======

	*phdrs_len = *nphdr * sizeof(struct elf_phdr);
	*notes_len = (4 * sizeof(struct elf_note) +
		      3 * ALIGN(sizeof(CORE_STR), 4) +
		      VMCOREINFO_NOTE_NAME_BYTES +
		      ALIGN(sizeof(struct elf_prstatus), 4) +
		      ALIGN(sizeof(struct elf_prpsinfo), 4) +
		      ALIGN(arch_task_struct_size, 4) +
		      ALIGN(vmcoreinfo_size, 4));
	*data_offset = PAGE_ALIGN(sizeof(struct elfhdr) + *phdrs_len +
				  *notes_len);
	return *data_offset + size;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_HIGHMEM
/*
 * If no highmem, we can assume [0...max_low_pfn) continuous range of memory
 * because memory hole is not as big as !HIGHMEM case.
 * (HIGHMEM is special because part of memory is _invisible_ from the kernel.)
 */
<<<<<<< HEAD
static int kcore_update_ram(void)
{
	LIST_HEAD(head);
	struct kcore_list *ent;
	int ret = 0;
=======
static int kcore_ram_list(struct list_head *head)
{
	struct kcore_list *ent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ent = kmalloc(sizeof(*ent), GFP_KERNEL);
	if (!ent)
		return -ENOMEM;
	ent->addr = (unsigned long)__va(0);
	ent->size = max_low_pfn << PAGE_SHIFT;
	ent->type = KCORE_RAM;
<<<<<<< HEAD
	list_add(&ent->list, &head);
	__kcore_update_ram(&head);
	return ret;
=======
	list_add(&ent->list, head);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else /* !CONFIG_HIGHMEM */

#ifdef CONFIG_SPARSEMEM_VMEMMAP
/* calculate vmemmap's address from given system ram pfn and register it */
static int
get_sparsemem_vmemmap_info(struct kcore_list *ent, struct list_head *head)
{
	unsigned long pfn = __pa(ent->addr) >> PAGE_SHIFT;
	unsigned long nr_pages = ent->size >> PAGE_SHIFT;
	unsigned long start, end;
	struct kcore_list *vmm, *tmp;


	start = ((unsigned long)pfn_to_page(pfn)) & PAGE_MASK;
	end = ((unsigned long)pfn_to_page(pfn + nr_pages)) - 1;
<<<<<<< HEAD
	end = ALIGN(end, PAGE_SIZE);
=======
	end = PAGE_ALIGN(end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* overlap check (because we have to align page */
	list_for_each_entry(tmp, head, list) {
		if (tmp->type != KCORE_VMEMMAP)
			continue;
		if (start < tmp->addr + tmp->size)
			if (end > tmp->addr)
				end = tmp->addr;
	}
	if (start < end) {
		vmm = kmalloc(sizeof(*vmm), GFP_KERNEL);
		if (!vmm)
			return 0;
		vmm->addr = start;
		vmm->size = end - start;
		vmm->type = KCORE_VMEMMAP;
		list_add_tail(&vmm->list, head);
	}
	return 1;

}
#else
static int
get_sparsemem_vmemmap_info(struct kcore_list *ent, struct list_head *head)
{
	return 1;
}

#endif

static int
kclist_add_private(unsigned long pfn, unsigned long nr_pages, void *arg)
{
	struct list_head *head = (struct list_head *)arg;
	struct kcore_list *ent;
<<<<<<< HEAD
=======
	struct page *p;

	if (!pfn_valid(pfn))
		return 1;

	p = pfn_to_page(pfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ent = kmalloc(sizeof(*ent), GFP_KERNEL);
	if (!ent)
		return -ENOMEM;
<<<<<<< HEAD
	ent->addr = (unsigned long)__va((pfn << PAGE_SHIFT));
	ent->size = nr_pages << PAGE_SHIFT;

	/* Sanity check: Can happen in 32bit arch...maybe */
	if (ent->addr < (unsigned long) __va(0))
=======
	ent->addr = (unsigned long)page_to_virt(p);
	ent->size = nr_pages << PAGE_SHIFT;

	if (!virt_addr_valid((void *)ent->addr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto free_out;

	/* cut not-mapped area. ....from ppc-32 code. */
	if (ULONG_MAX - ent->addr < ent->size)
		ent->size = ULONG_MAX - ent->addr;

<<<<<<< HEAD
	/* cut when vmalloc() area is higher than direct-map area */
	if (VMALLOC_START > (unsigned long)__va(0)) {
		if (ent->addr > VMALLOC_START)
			goto free_out;
=======
	/*
	 * We've already checked virt_addr_valid so we know this address
	 * is a valid pointer, therefore we can check against it to determine
	 * if we need to trim
	 */
	if (VMALLOC_START > ent->addr) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (VMALLOC_START - ent->addr < ent->size)
			ent->size = VMALLOC_START - ent->addr;
	}

	ent->type = KCORE_RAM;
	list_add_tail(&ent->list, head);

	if (!get_sparsemem_vmemmap_info(ent, head)) {
		list_del(&ent->list);
		goto free_out;
	}

	return 0;
free_out:
	kfree(ent);
	return 1;
}

<<<<<<< HEAD
static int kcore_update_ram(void)
{
	int nid, ret;
	unsigned long end_pfn;
	LIST_HEAD(head);
=======
static int kcore_ram_list(struct list_head *list)
{
	int nid, ret;
	unsigned long end_pfn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Not inialized....update now */
	/* find out "max pfn" */
	end_pfn = 0;
<<<<<<< HEAD
	for_each_node_state(nid, N_HIGH_MEMORY) {
		unsigned long node_end;
		node_end  = NODE_DATA(nid)->node_start_pfn +
			NODE_DATA(nid)->node_spanned_pages;
=======
	for_each_node_state(nid, N_MEMORY) {
		unsigned long node_end;
		node_end = node_end_pfn(nid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (end_pfn < node_end)
			end_pfn = node_end;
	}
	/* scan 0 to max_pfn */
<<<<<<< HEAD
	ret = walk_system_ram_range(0, end_pfn, &head, kclist_add_private);
	if (ret) {
		free_kclist_ents(&head);
		return -ENOMEM;
	}
	__kcore_update_ram(&head);
	return ret;
}
#endif /* CONFIG_HIGHMEM */

/*****************************************************************************/
/*
 * determine size of ELF note
 */
static int notesize(struct memelfnote *en)
{
	int sz;

	sz = sizeof(struct elf_note);
	sz += roundup((strlen(en->name) + 1), 4);
	sz += roundup(en->datasz, 4);

	return sz;
} /* end notesize() */

/*****************************************************************************/
/*
 * store a note in the header buffer
 */
static char *storenote(struct memelfnote *men, char *bufp)
{
	struct elf_note en;

#define DUMP_WRITE(addr,nr) do { memcpy(bufp,addr,nr); bufp += nr; } while(0)

	en.n_namesz = strlen(men->name) + 1;
	en.n_descsz = men->datasz;
	en.n_type = men->type;

	DUMP_WRITE(&en, sizeof(en));
	DUMP_WRITE(men->name, en.n_namesz);

	/* XXX - cast from long long to long to avoid need for libgcc.a */
	bufp = (char*) roundup((unsigned long)bufp,4);
	DUMP_WRITE(men->data, men->datasz);
	bufp = (char*) roundup((unsigned long)bufp,4);

#undef DUMP_WRITE

	return bufp;
} /* end storenote() */

/*
 * store an ELF coredump header in the supplied buffer
 * nphdr is the number of elf_phdr to insert
 */
static void elf_kcore_store_hdr(char *bufp, int nphdr, int dataoff)
{
	struct elf_prstatus prstatus;	/* NT_PRSTATUS */
	struct elf_prpsinfo prpsinfo;	/* NT_PRPSINFO */
	struct elf_phdr *nhdr, *phdr;
	struct elfhdr *elf;
	struct memelfnote notes[3];
	off_t offset = 0;
	struct kcore_list *m;

	/* setup ELF header */
	elf = (struct elfhdr *) bufp;
	bufp += sizeof(struct elfhdr);
	offset += sizeof(struct elfhdr);
	memcpy(elf->e_ident, ELFMAG, SELFMAG);
	elf->e_ident[EI_CLASS]	= ELF_CLASS;
	elf->e_ident[EI_DATA]	= ELF_DATA;
	elf->e_ident[EI_VERSION]= EV_CURRENT;
	elf->e_ident[EI_OSABI] = ELF_OSABI;
	memset(elf->e_ident+EI_PAD, 0, EI_NIDENT-EI_PAD);
	elf->e_type	= ET_CORE;
	elf->e_machine	= ELF_ARCH;
	elf->e_version	= EV_CURRENT;
	elf->e_entry	= 0;
	elf->e_phoff	= sizeof(struct elfhdr);
	elf->e_shoff	= 0;
	elf->e_flags	= ELF_CORE_EFLAGS;
	elf->e_ehsize	= sizeof(struct elfhdr);
	elf->e_phentsize= sizeof(struct elf_phdr);
	elf->e_phnum	= nphdr;
	elf->e_shentsize= 0;
	elf->e_shnum	= 0;
	elf->e_shstrndx	= 0;

	/* setup ELF PT_NOTE program header */
	nhdr = (struct elf_phdr *) bufp;
	bufp += sizeof(struct elf_phdr);
	offset += sizeof(struct elf_phdr);
	nhdr->p_type	= PT_NOTE;
	nhdr->p_offset	= 0;
	nhdr->p_vaddr	= 0;
	nhdr->p_paddr	= 0;
	nhdr->p_filesz	= 0;
	nhdr->p_memsz	= 0;
	nhdr->p_flags	= 0;
	nhdr->p_align	= 0;

	/* setup ELF PT_LOAD program header for every area */
	list_for_each_entry(m, &kclist_head, list) {
		phdr = (struct elf_phdr *) bufp;
		bufp += sizeof(struct elf_phdr);
		offset += sizeof(struct elf_phdr);

		phdr->p_type	= PT_LOAD;
		phdr->p_flags	= PF_R|PF_W|PF_X;
		phdr->p_offset	= kc_vaddr_to_offset(m->addr) + dataoff;
		phdr->p_vaddr	= (size_t)m->addr;
		phdr->p_paddr	= 0;
		phdr->p_filesz	= phdr->p_memsz	= m->size;
		phdr->p_align	= PAGE_SIZE;
	}

	/*
	 * Set up the notes in similar form to SVR4 core dumps made
	 * with info from their /proc.
	 */
	nhdr->p_offset	= offset;

	/* set up the process status */
	notes[0].name = CORE_STR;
	notes[0].type = NT_PRSTATUS;
	notes[0].datasz = sizeof(struct elf_prstatus);
	notes[0].data = &prstatus;

	memset(&prstatus, 0, sizeof(struct elf_prstatus));

	nhdr->p_filesz	= notesize(&notes[0]);
	bufp = storenote(&notes[0], bufp);

	/* set up the process info */
	notes[1].name	= CORE_STR;
	notes[1].type	= NT_PRPSINFO;
	notes[1].datasz	= sizeof(struct elf_prpsinfo);
	notes[1].data	= &prpsinfo;

	memset(&prpsinfo, 0, sizeof(struct elf_prpsinfo));
	prpsinfo.pr_state	= 0;
	prpsinfo.pr_sname	= 'R';
	prpsinfo.pr_zomb	= 0;

	strcpy(prpsinfo.pr_fname, "vmlinux");
	strncpy(prpsinfo.pr_psargs, saved_command_line, ELF_PRARGSZ);

	nhdr->p_filesz	+= notesize(&notes[1]);
	bufp = storenote(&notes[1], bufp);

	/* set up the task structure */
	notes[2].name	= CORE_STR;
	notes[2].type	= NT_TASKSTRUCT;
	notes[2].datasz	= sizeof(struct task_struct);
	notes[2].data	= current;

	nhdr->p_filesz	+= notesize(&notes[2]);
	bufp = storenote(&notes[2], bufp);

} /* end elf_kcore_store_hdr() */

/*****************************************************************************/
/*
 * read from the ELF header and then kernel memory
 */
static ssize_t
read_kcore(struct file *file, char __user *buffer, size_t buflen, loff_t *fpos)
{
	ssize_t acc = 0;
	size_t size, tsz;
	size_t elf_buflen;
	int nphdr;
	unsigned long start;

	read_lock(&kclist_lock);
	size = get_kcore_size(&nphdr, &elf_buflen);

	if (buflen == 0 || *fpos >= size) {
		read_unlock(&kclist_lock);
		return 0;
	}

	/* trim buflen to not go beyond EOF */
	if (buflen > size - *fpos)
		buflen = size - *fpos;

	/* construct an ELF core header if we'll need some of it */
	if (*fpos < elf_buflen) {
		char * elf_buf;

		tsz = elf_buflen - *fpos;
		if (buflen < tsz)
			tsz = buflen;
		elf_buf = kzalloc(elf_buflen, GFP_ATOMIC);
		if (!elf_buf) {
			read_unlock(&kclist_lock);
			return -ENOMEM;
		}
		elf_kcore_store_hdr(elf_buf, nphdr, elf_buflen);
		read_unlock(&kclist_lock);
		if (copy_to_user(buffer, elf_buf + *fpos, tsz)) {
			kfree(elf_buf);
			return -EFAULT;
		}
		kfree(elf_buf);
		buflen -= tsz;
		*fpos += tsz;
		buffer += tsz;
		acc += tsz;

		/* leave now if filled buffer already */
		if (buflen == 0)
			return acc;
	} else
		read_unlock(&kclist_lock);
=======
	ret = walk_system_ram_range(0, end_pfn, list, kclist_add_private);
	if (ret)
		return -ENOMEM;
	return 0;
}
#endif /* CONFIG_HIGHMEM */

static int kcore_update_ram(void)
{
	LIST_HEAD(list);
	LIST_HEAD(garbage);
	int nphdr;
	size_t phdrs_len, notes_len, data_offset;
	struct kcore_list *tmp, *pos;
	int ret = 0;

	down_write(&kclist_lock);
	if (!xchg(&kcore_need_update, 0))
		goto out;

	ret = kcore_ram_list(&list);
	if (ret) {
		/* Couldn't get the RAM list, try again next time. */
		WRITE_ONCE(kcore_need_update, 1);
		list_splice_tail(&list, &garbage);
		goto out;
	}

	list_for_each_entry_safe(pos, tmp, &kclist_head, list) {
		if (pos->type == KCORE_RAM || pos->type == KCORE_VMEMMAP)
			list_move(&pos->list, &garbage);
	}
	list_splice_tail(&list, &kclist_head);

	proc_root_kcore->size = get_kcore_size(&nphdr, &phdrs_len, &notes_len,
					       &data_offset);

out:
	up_write(&kclist_lock);
	list_for_each_entry_safe(pos, tmp, &garbage, list) {
		list_del(&pos->list);
		kfree(pos);
	}
	return ret;
}

static void append_kcore_note(char *notes, size_t *i, const char *name,
			      unsigned int type, const void *desc,
			      size_t descsz)
{
	struct elf_note *note = (struct elf_note *)&notes[*i];

	note->n_namesz = strlen(name) + 1;
	note->n_descsz = descsz;
	note->n_type = type;
	*i += sizeof(*note);
	memcpy(&notes[*i], name, note->n_namesz);
	*i = ALIGN(*i + note->n_namesz, 4);
	memcpy(&notes[*i], desc, descsz);
	*i = ALIGN(*i + descsz, 4);
}

static ssize_t read_kcore_iter(struct kiocb *iocb, struct iov_iter *iter)
{
	struct file *file = iocb->ki_filp;
	char *buf = file->private_data;
	loff_t *fpos = &iocb->ki_pos;
	size_t phdrs_offset, notes_offset, data_offset;
	size_t page_offline_frozen = 1;
	size_t phdrs_len, notes_len;
	struct kcore_list *m;
	size_t tsz;
	int nphdr;
	unsigned long start;
	size_t buflen = iov_iter_count(iter);
	size_t orig_buflen = buflen;
	int ret = 0;

	down_read(&kclist_lock);
	/*
	 * Don't race against drivers that set PageOffline() and expect no
	 * further page access.
	 */
	page_offline_freeze();

	get_kcore_size(&nphdr, &phdrs_len, &notes_len, &data_offset);
	phdrs_offset = sizeof(struct elfhdr);
	notes_offset = phdrs_offset + phdrs_len;

	/* ELF file header. */
	if (buflen && *fpos < sizeof(struct elfhdr)) {
		struct elfhdr ehdr = {
			.e_ident = {
				[EI_MAG0] = ELFMAG0,
				[EI_MAG1] = ELFMAG1,
				[EI_MAG2] = ELFMAG2,
				[EI_MAG3] = ELFMAG3,
				[EI_CLASS] = ELF_CLASS,
				[EI_DATA] = ELF_DATA,
				[EI_VERSION] = EV_CURRENT,
				[EI_OSABI] = ELF_OSABI,
			},
			.e_type = ET_CORE,
			.e_machine = ELF_ARCH,
			.e_version = EV_CURRENT,
			.e_phoff = sizeof(struct elfhdr),
			.e_flags = ELF_CORE_EFLAGS,
			.e_ehsize = sizeof(struct elfhdr),
			.e_phentsize = sizeof(struct elf_phdr),
			.e_phnum = nphdr,
		};

		tsz = min_t(size_t, buflen, sizeof(struct elfhdr) - *fpos);
		if (copy_to_iter((char *)&ehdr + *fpos, tsz, iter) != tsz) {
			ret = -EFAULT;
			goto out;
		}

		buflen -= tsz;
		*fpos += tsz;
	}

	/* ELF program headers. */
	if (buflen && *fpos < phdrs_offset + phdrs_len) {
		struct elf_phdr *phdrs, *phdr;

		phdrs = kzalloc(phdrs_len, GFP_KERNEL);
		if (!phdrs) {
			ret = -ENOMEM;
			goto out;
		}

		phdrs[0].p_type = PT_NOTE;
		phdrs[0].p_offset = notes_offset;
		phdrs[0].p_filesz = notes_len;

		phdr = &phdrs[1];
		list_for_each_entry(m, &kclist_head, list) {
			phdr->p_type = PT_LOAD;
			phdr->p_flags = PF_R | PF_W | PF_X;
			phdr->p_offset = kc_vaddr_to_offset(m->addr) + data_offset;
			phdr->p_vaddr = (size_t)m->addr;
			if (m->type == KCORE_RAM)
				phdr->p_paddr = __pa(m->addr);
			else if (m->type == KCORE_TEXT)
				phdr->p_paddr = __pa_symbol(m->addr);
			else
				phdr->p_paddr = (elf_addr_t)-1;
			phdr->p_filesz = phdr->p_memsz = m->size;
			phdr->p_align = PAGE_SIZE;
			phdr++;
		}

		tsz = min_t(size_t, buflen, phdrs_offset + phdrs_len - *fpos);
		if (copy_to_iter((char *)phdrs + *fpos - phdrs_offset, tsz,
				 iter) != tsz) {
			kfree(phdrs);
			ret = -EFAULT;
			goto out;
		}
		kfree(phdrs);

		buflen -= tsz;
		*fpos += tsz;
	}

	/* ELF note segment. */
	if (buflen && *fpos < notes_offset + notes_len) {
		struct elf_prstatus prstatus = {};
		struct elf_prpsinfo prpsinfo = {
			.pr_sname = 'R',
			.pr_fname = "vmlinux",
		};
		char *notes;
		size_t i = 0;

		strscpy(prpsinfo.pr_psargs, saved_command_line,
			sizeof(prpsinfo.pr_psargs));

		notes = kzalloc(notes_len, GFP_KERNEL);
		if (!notes) {
			ret = -ENOMEM;
			goto out;
		}

		append_kcore_note(notes, &i, CORE_STR, NT_PRSTATUS, &prstatus,
				  sizeof(prstatus));
		append_kcore_note(notes, &i, CORE_STR, NT_PRPSINFO, &prpsinfo,
				  sizeof(prpsinfo));
		append_kcore_note(notes, &i, CORE_STR, NT_TASKSTRUCT, current,
				  arch_task_struct_size);
		/*
		 * vmcoreinfo_size is mostly constant after init time, but it
		 * can be changed by crash_save_vmcoreinfo(). Racing here with a
		 * panic on another CPU before the machine goes down is insanely
		 * unlikely, but it's better to not leave potential buffer
		 * overflows lying around, regardless.
		 */
		append_kcore_note(notes, &i, VMCOREINFO_NOTE_NAME, 0,
				  vmcoreinfo_data,
				  min(vmcoreinfo_size, notes_len - i));

		tsz = min_t(size_t, buflen, notes_offset + notes_len - *fpos);
		if (copy_to_iter(notes + *fpos - notes_offset, tsz, iter) != tsz) {
			kfree(notes);
			ret = -EFAULT;
			goto out;
		}
		kfree(notes);

		buflen -= tsz;
		*fpos += tsz;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check to see if our file offset matches with any of
	 * the addresses in the elf_phdr on our list.
	 */
<<<<<<< HEAD
	start = kc_offset_to_vaddr(*fpos - elf_buflen);
	if ((tsz = (PAGE_SIZE - (start & ~PAGE_MASK))) > buflen)
		tsz = buflen;
		
	while (buflen) {
		struct kcore_list *m;

		read_lock(&kclist_lock);
		list_for_each_entry(m, &kclist_head, list) {
			if (start >= m->addr && start < (m->addr+m->size))
				break;
		}
		read_unlock(&kclist_lock);

		if (&m->list == &kclist_head) {
			if (clear_user(buffer, tsz))
				return -EFAULT;
		} else if (is_vmalloc_or_module_addr((void *)start)) {
			char * elf_buf;

			elf_buf = kzalloc(tsz, GFP_KERNEL);
			if (!elf_buf)
				return -ENOMEM;
			vread(elf_buf, (char *)start, tsz);
			/* we have to zero-fill user buffer even if no read */
			if (copy_to_user(buffer, elf_buf, tsz)) {
				kfree(elf_buf);
				return -EFAULT;
			}
			kfree(elf_buf);
		} else {
			if (kern_addr_valid(start)) {
				unsigned long n;

				n = copy_to_user(buffer, (char *)start, tsz);
				/*
				 * We cannot distinguish between fault on source
				 * and fault on destination. When this happens
				 * we clear too and hope it will trigger the
				 * EFAULT again.
				 */
				if (n) { 
					if (clear_user(buffer + tsz - n,
								n))
						return -EFAULT;
				}
			} else {
				if (clear_user(buffer, tsz))
					return -EFAULT;
			}
		}
		buflen -= tsz;
		*fpos += tsz;
		buffer += tsz;
		acc += tsz;
=======
	start = kc_offset_to_vaddr(*fpos - data_offset);
	if ((tsz = (PAGE_SIZE - (start & ~PAGE_MASK))) > buflen)
		tsz = buflen;

	m = NULL;
	while (buflen) {
		struct page *page;
		unsigned long pfn;

		/*
		 * If this is the first iteration or the address is not within
		 * the previous entry, search for a matching entry.
		 */
		if (!m || start < m->addr || start >= m->addr + m->size) {
			struct kcore_list *iter;

			m = NULL;
			list_for_each_entry(iter, &kclist_head, list) {
				if (start >= iter->addr &&
				    start < iter->addr + iter->size) {
					m = iter;
					break;
				}
			}
		}

		if (page_offline_frozen++ % MAX_ORDER_NR_PAGES == 0) {
			page_offline_thaw();
			cond_resched();
			page_offline_freeze();
		}

		if (!m) {
			if (iov_iter_zero(tsz, iter) != tsz) {
				ret = -EFAULT;
				goto out;
			}
			goto skip;
		}

		switch (m->type) {
		case KCORE_VMALLOC:
		{
			const char *src = (char *)start;
			size_t read = 0, left = tsz;

			/*
			 * vmalloc uses spinlocks, so we optimistically try to
			 * read memory. If this fails, fault pages in and try
			 * again until we are done.
			 */
			while (true) {
				read += vread_iter(iter, src, left);
				if (read == tsz)
					break;

				src += read;
				left -= read;

				if (fault_in_iov_iter_writeable(iter, left)) {
					ret = -EFAULT;
					goto out;
				}
			}
			break;
		}
		case KCORE_USER:
			/* User page is handled prior to normal kernel page: */
			if (copy_to_iter((char *)start, tsz, iter) != tsz) {
				ret = -EFAULT;
				goto out;
			}
			break;
		case KCORE_RAM:
			pfn = __pa(start) >> PAGE_SHIFT;
			page = pfn_to_online_page(pfn);

			/*
			 * Don't read offline sections, logically offline pages
			 * (e.g., inflated in a balloon), hwpoisoned pages,
			 * and explicitly excluded physical ranges.
			 */
			if (!page || PageOffline(page) ||
			    is_page_hwpoison(page) || !pfn_is_ram(pfn) ||
			    pfn_is_unaccepted_memory(pfn)) {
				if (iov_iter_zero(tsz, iter) != tsz) {
					ret = -EFAULT;
					goto out;
				}
				break;
			}
			fallthrough;
		case KCORE_VMEMMAP:
		case KCORE_TEXT:
			/*
			 * Sadly we must use a bounce buffer here to be able to
			 * make use of copy_from_kernel_nofault(), as these
			 * memory regions might not always be mapped on all
			 * architectures.
			 */
			if (copy_from_kernel_nofault(buf, (void *)start, tsz)) {
				if (iov_iter_zero(tsz, iter) != tsz) {
					ret = -EFAULT;
					goto out;
				}
			/*
			 * We know the bounce buffer is safe to copy from, so
			 * use _copy_to_iter() directly.
			 */
			} else if (_copy_to_iter(buf, tsz, iter) != tsz) {
				ret = -EFAULT;
				goto out;
			}
			break;
		default:
			pr_warn_once("Unhandled KCORE type: %d\n", m->type);
			if (iov_iter_zero(tsz, iter) != tsz) {
				ret = -EFAULT;
				goto out;
			}
		}
skip:
		buflen -= tsz;
		*fpos += tsz;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		start += tsz;
		tsz = (buflen > PAGE_SIZE ? PAGE_SIZE : buflen);
	}

<<<<<<< HEAD
	return acc;
}


static int open_kcore(struct inode *inode, struct file *filp)
{
	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	if (kcore_need_update)
		kcore_update_ram();
	if (i_size_read(inode) != proc_root_kcore->size) {
		mutex_lock(&inode->i_mutex);
		i_size_write(inode, proc_root_kcore->size);
		mutex_unlock(&inode->i_mutex);
=======
out:
	page_offline_thaw();
	up_read(&kclist_lock);
	if (ret)
		return ret;
	return orig_buflen - buflen;
}

static int open_kcore(struct inode *inode, struct file *filp)
{
	int ret = security_locked_down(LOCKDOWN_KCORE);

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;

	if (ret)
		return ret;

	filp->private_data = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!filp->private_data)
		return -ENOMEM;

	if (kcore_need_update)
		kcore_update_ram();
	if (i_size_read(inode) != proc_root_kcore->size) {
		inode_lock(inode);
		i_size_write(inode, proc_root_kcore->size);
		inode_unlock(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD

static const struct file_operations proc_kcore_operations = {
	.read		= read_kcore,
	.open		= open_kcore,
	.llseek		= default_llseek,
};

#ifdef CONFIG_MEMORY_HOTPLUG
=======
static int release_kcore(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

static const struct proc_ops kcore_proc_ops = {
	.proc_read_iter	= read_kcore_iter,
	.proc_open	= open_kcore,
	.proc_release	= release_kcore,
	.proc_lseek	= default_llseek,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* just remember that we have to update kcore */
static int __meminit kcore_callback(struct notifier_block *self,
				    unsigned long action, void *arg)
{
	switch (action) {
	case MEM_ONLINE:
	case MEM_OFFLINE:
<<<<<<< HEAD
		write_lock(&kclist_lock);
		kcore_need_update = 1;
		write_unlock(&kclist_lock);
	}
	return NOTIFY_OK;
}
#endif
=======
		kcore_need_update = 1;
		break;
	}
	return NOTIFY_OK;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


static struct kcore_list kcore_vmalloc;

#ifdef CONFIG_ARCH_PROC_KCORE_TEXT
static struct kcore_list kcore_text;
/*
 * If defined, special segment is used for mapping kernel text instead of
 * direct-map area. We need to create special TEXT section.
 */
static void __init proc_kcore_text_init(void)
{
	kclist_add(&kcore_text, _text, _end - _text, KCORE_TEXT);
}
#else
static void __init proc_kcore_text_init(void)
{
}
#endif

#if defined(CONFIG_MODULES) && defined(MODULES_VADDR)
/*
 * MODULES_VADDR has no intersection with VMALLOC_ADDR.
 */
<<<<<<< HEAD
struct kcore_list kcore_modules;
static void __init add_modules_range(void)
{
	kclist_add(&kcore_modules, (void *)MODULES_VADDR,
			MODULES_END - MODULES_VADDR, KCORE_VMALLOC);
=======
static struct kcore_list kcore_modules;
static void __init add_modules_range(void)
{
	if (MODULES_VADDR != VMALLOC_START && MODULES_END != VMALLOC_END) {
		kclist_add(&kcore_modules, (void *)MODULES_VADDR,
			MODULES_END - MODULES_VADDR, KCORE_VMALLOC);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else
static void __init add_modules_range(void)
{
}
#endif

static int __init proc_kcore_init(void)
{
<<<<<<< HEAD
	proc_root_kcore = proc_create("kcore", S_IRUSR, NULL,
				      &proc_kcore_operations);
	if (!proc_root_kcore) {
		printk(KERN_ERR "couldn't create /proc/kcore\n");
=======
	proc_root_kcore = proc_create("kcore", S_IRUSR, NULL, &kcore_proc_ops);
	if (!proc_root_kcore) {
		pr_err("couldn't create /proc/kcore\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0; /* Always returns 0. */
	}
	/* Store text area if it's special */
	proc_kcore_text_init();
	/* Store vmalloc area */
	kclist_add(&kcore_vmalloc, (void *)VMALLOC_START,
		VMALLOC_END - VMALLOC_START, KCORE_VMALLOC);
	add_modules_range();
	/* Store direct-map area from physical memory map */
	kcore_update_ram();
<<<<<<< HEAD
	hotplug_memory_notifier(kcore_callback, 0);

	return 0;
}
module_init(proc_kcore_init);
=======
	hotplug_memory_notifier(kcore_callback, DEFAULT_CALLBACK_PRI);

	return 0;
}
fs_initcall(proc_kcore_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
