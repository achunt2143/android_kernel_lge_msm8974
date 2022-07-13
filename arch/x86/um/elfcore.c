<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/elf.h>
#include <linux/coredump.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include <asm/elf.h>


<<<<<<< HEAD
Elf32_Half elf_core_extra_phdrs(void)
=======
Elf32_Half elf_core_extra_phdrs(struct coredump_params *cprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return vsyscall_ehdr ? (((struct elfhdr *)vsyscall_ehdr)->e_phnum) : 0;
}

<<<<<<< HEAD
int elf_core_write_extra_phdrs(struct file *file, loff_t offset, size_t *size,
			       unsigned long limit)
=======
int elf_core_write_extra_phdrs(struct coredump_params *cprm, loff_t offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if ( vsyscall_ehdr ) {
		const struct elfhdr *const ehdrp =
			(struct elfhdr *) vsyscall_ehdr;
		const struct elf_phdr *const phdrp =
			(const struct elf_phdr *) (vsyscall_ehdr + ehdrp->e_phoff);
		int i;
		Elf32_Off ofs = 0;

		for (i = 0; i < ehdrp->e_phnum; ++i) {
			struct elf_phdr phdr = phdrp[i];

			if (phdr.p_type == PT_LOAD) {
				ofs = phdr.p_offset = offset;
				offset += phdr.p_filesz;
			} else {
				phdr.p_offset += ofs;
			}
			phdr.p_paddr = 0; /* match other core phdrs */
<<<<<<< HEAD
			*size += sizeof(phdr);
			if (*size > limit
			    || !dump_write(file, &phdr, sizeof(phdr)))
=======
			if (!dump_emit(cprm, &phdr, sizeof(phdr)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 0;
		}
	}
	return 1;
}

<<<<<<< HEAD
int elf_core_write_extra_data(struct file *file, size_t *size,
			      unsigned long limit)
=======
int elf_core_write_extra_data(struct coredump_params *cprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if ( vsyscall_ehdr ) {
		const struct elfhdr *const ehdrp =
			(struct elfhdr *) vsyscall_ehdr;
		const struct elf_phdr *const phdrp =
			(const struct elf_phdr *) (vsyscall_ehdr + ehdrp->e_phoff);
		int i;

		for (i = 0; i < ehdrp->e_phnum; ++i) {
			if (phdrp[i].p_type == PT_LOAD) {
				void *addr = (void *) phdrp[i].p_vaddr;
				size_t filesz = phdrp[i].p_filesz;
<<<<<<< HEAD

				*size += filesz;
				if (*size > limit
				    || !dump_write(file, addr, filesz))
=======
				if (!dump_emit(cprm, addr, filesz))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					return 0;
			}
		}
	}
	return 1;
}

<<<<<<< HEAD
size_t elf_core_extra_data_size(void)
=======
size_t elf_core_extra_data_size(struct coredump_params *cprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if ( vsyscall_ehdr ) {
		const struct elfhdr *const ehdrp =
			(struct elfhdr *)vsyscall_ehdr;
		const struct elf_phdr *const phdrp =
			(const struct elf_phdr *) (vsyscall_ehdr + ehdrp->e_phoff);
		int i;

		for (i = 0; i < ehdrp->e_phnum; ++i)
			if (phdrp[i].p_type == PT_LOAD)
				return (size_t) phdrp[i].p_filesz;
	}
	return 0;
}
