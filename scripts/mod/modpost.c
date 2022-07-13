/* Postprocess module symbol versions
 *
 * Copyright 2003       Kai Germaschewski
 * Copyright 2002-2004  Rusty Russell, IBM Corporation
 * Copyright 2006-2008  Sam Ravnborg
 * Based in part on module-init-tools/depmod.c,file2alias
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * Usage: modpost vmlinux module1.o module2.o ...
 */

#define _GNU_SOURCE
<<<<<<< HEAD
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "modpost.h"
#include "../../include/generated/autoconf.h"
#include "../../include/linux/license.h"

/* Some toolchains use a `_' prefix for all user symbols. */
#ifdef CONFIG_SYMBOL_PREFIX
#define MODULE_SYMBOL_PREFIX CONFIG_SYMBOL_PREFIX
#else
#define MODULE_SYMBOL_PREFIX ""
#endif


/* Are we using CONFIG_MODVERSIONS? */
int modversions = 0;
/* Warn about undefined symbols? (do so if we have vmlinux) */
int have_vmlinux = 0;
/* Is CONFIG_MODULE_SRCVERSION_ALL set? */
static int all_versions = 0;
/* If we are modposting external module set to 1 */
static int external_module = 0;
/* Warn about section mismatch in vmlinux if set to 1 */
static int vmlinux_section_warnings = 1;
/* Exit with an error when there is a section mismatch if set to 1 */
static int section_error_on_mismatch;
/* Only warn about unresolved symbols */
static int warn_unresolved = 0;
/* How a symbol is exported */
static int sec_mismatch_count = 0;
static int sec_mismatch_verbose = 1;

enum export {
	export_plain,      export_unused,     export_gpl,
	export_unused_gpl, export_gpl_future, export_unknown
};

#define PRINTF __attribute__ ((format (printf, 1, 2)))

PRINTF void fatal(const char *fmt, ...)
{
	va_list arglist;

	fprintf(stderr, "FATAL: ");

	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);

	exit(1);
}

PRINTF void warn(const char *fmt, ...)
{
	va_list arglist;

	fprintf(stderr, "WARNING: ");
=======
#include <elf.h>
#include <fnmatch.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include "modpost.h"
#include "../../include/linux/license.h"

static bool module_enabled;
/* Are we using CONFIG_MODVERSIONS? */
static bool modversions;
/* Is CONFIG_MODULE_SRCVERSION_ALL set? */
static bool all_versions;
/* If we are modposting external module set to 1 */
static bool external_module;
/* Only warn about unresolved symbols */
static bool warn_unresolved;

static int sec_mismatch_count;
static bool sec_mismatch_warn_only = true;
/* Trim EXPORT_SYMBOLs that are unused by in-tree modules */
static bool trim_unused_exports;

/* ignore missing files */
static bool ignore_missing_files;
/* If set to 1, only warn (instead of error) about missing ns imports */
static bool allow_missing_ns_imports;

static bool error_occurred;

static bool extra_warn;

/*
 * Cut off the warnings when there are too many. This typically occurs when
 * vmlinux is missing. ('make modules' without building vmlinux.)
 */
#define MAX_UNRESOLVED_REPORTS	10
static unsigned int nr_unresolved;

/* In kernel, this size is defined in linux/module.h;
 * here we use Elf_Addr instead of long for covering cross-compile
 */

#define MODULE_NAME_LEN (64 - sizeof(Elf_Addr))

void modpost_log(enum loglevel loglevel, const char *fmt, ...)
{
	va_list arglist;

	switch (loglevel) {
	case LOG_WARN:
		fprintf(stderr, "WARNING: ");
		break;
	case LOG_ERROR:
		fprintf(stderr, "ERROR: ");
		error_occurred = true;
		break;
	default: /* invalid loglevel, ignore */
		break;
	}

	fprintf(stderr, "modpost: ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}

<<<<<<< HEAD
PRINTF void merror(const char *fmt, ...)
{
	va_list arglist;

	fprintf(stderr, "ERROR: ");

	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}

static int is_vmlinux(const char *modname)
{
	const char *myname;

	myname = strrchr(modname, '/');
	if (myname)
		myname++;
	else
		myname = modname;

	return (strcmp(myname, "vmlinux") == 0) ||
	       (strcmp(myname, "vmlinux.o") == 0);
=======
static inline bool strends(const char *str, const char *postfix)
{
	if (strlen(str) < strlen(postfix))
		return false;

	return strcmp(str + strlen(str) - strlen(postfix), postfix) == 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void *do_nofail(void *ptr, const char *expr)
{
	if (!ptr)
<<<<<<< HEAD
		fatal("modpost: Memory allocation failure: %s.\n", expr);
=======
		fatal("Memory allocation failure: %s.\n", expr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ptr;
}

<<<<<<< HEAD
/* A list of all modules we processed */
static struct module *modules;

static struct module *find_module(char *modname)
{
	struct module *mod;

	for (mod = modules; mod; mod = mod->next)
		if (strcmp(mod->name, modname) == 0)
			break;
	return mod;
}

static struct module *new_module(char *modname)
{
	struct module *mod;
	char *p, *s;

	mod = NOFAIL(malloc(sizeof(*mod)));
	memset(mod, 0, sizeof(*mod));
	p = NOFAIL(strdup(modname));

	/* strip trailing .o */
	s = strrchr(p, '.');
	if (s != NULL)
		if (strcmp(s, ".o") == 0) {
			*s = '\0';
			mod->is_dot_o = 1;
		}

	/* add to list */
	mod->name = p;
	mod->gpl_compatible = -1;
	mod->next = modules;
	modules = mod;
=======
char *read_text_file(const char *filename)
{
	struct stat st;
	size_t nbytes;
	int fd;
	char *buf;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror(filename);
		exit(1);
	}

	if (fstat(fd, &st) < 0) {
		perror(filename);
		exit(1);
	}

	buf = NOFAIL(malloc(st.st_size + 1));

	nbytes = st.st_size;

	while (nbytes) {
		ssize_t bytes_read;

		bytes_read = read(fd, buf, nbytes);
		if (bytes_read < 0) {
			perror(filename);
			exit(1);
		}

		nbytes -= bytes_read;
	}
	buf[st.st_size] = '\0';

	close(fd);

	return buf;
}

char *get_line(char **stringp)
{
	char *orig = *stringp, *next;

	/* do not return the unwanted extra line at EOF */
	if (!orig || *orig == '\0')
		return NULL;

	/* don't use strsep here, it is not available everywhere */
	next = strchr(orig, '\n');
	if (next)
		*next++ = '\0';

	*stringp = next;

	return orig;
}

/* A list of all modules we processed */
LIST_HEAD(modules);

static struct module *find_module(const char *modname)
{
	struct module *mod;

	list_for_each_entry(mod, &modules, list) {
		if (strcmp(mod->name, modname) == 0)
			return mod;
	}
	return NULL;
}

static struct module *new_module(const char *name, size_t namelen)
{
	struct module *mod;

	mod = NOFAIL(malloc(sizeof(*mod) + namelen + 1));
	memset(mod, 0, sizeof(*mod));

	INIT_LIST_HEAD(&mod->exported_symbols);
	INIT_LIST_HEAD(&mod->unresolved_symbols);
	INIT_LIST_HEAD(&mod->missing_namespaces);
	INIT_LIST_HEAD(&mod->imported_namespaces);

	memcpy(mod->name, name, namelen);
	mod->name[namelen] = '\0';
	mod->is_vmlinux = (strcmp(mod->name, "vmlinux") == 0);

	/*
	 * Set mod->is_gpl_compatible to true by default. If MODULE_LICENSE()
	 * is missing, do not check the use for EXPORT_SYMBOL_GPL() becasue
	 * modpost will exit wiht error anyway.
	 */
	mod->is_gpl_compatible = true;

	list_add_tail(&mod->list, &modules);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return mod;
}

/* A hash of all exported symbols,
 * struct symbol is also used for lists of unresolved symbols */

#define SYMBOL_HASH_SIZE 1024

struct symbol {
	struct symbol *next;
<<<<<<< HEAD
	struct module *module;
	unsigned int crc;
	int crc_valid;
	unsigned int weak:1;
	unsigned int vmlinux:1;    /* 1 if symbol is defined in vmlinux */
	unsigned int kernel:1;     /* 1 if symbol is from kernel
				    *  (only for external modules) **/
	unsigned int preloaded:1;  /* 1 if symbol from Module.symvers */
	enum export  export;       /* Type of export */
	char name[0];
=======
	struct list_head list;	/* link to module::exported_symbols or module::unresolved_symbols */
	struct module *module;
	char *namespace;
	unsigned int crc;
	bool crc_valid;
	bool weak;
	bool is_func;
	bool is_gpl_only;	/* exported by EXPORT_SYMBOL_GPL */
	bool used;		/* there exists a user of this symbol */
	char name[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct symbol *symbolhash[SYMBOL_HASH_SIZE];

<<<<<<< HEAD
/* This is based on the hash agorithm from gdbm, via tdb */
=======
/* This is based on the hash algorithm from gdbm, via tdb */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned int tdb_hash(const char *name)
{
	unsigned value;	/* Used to compute the hash value.  */
	unsigned   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	for (value = 0x238F13AF * strlen(name), i = 0; name[i]; i++)
		value = (value + (((unsigned char *)name)[i] << (i*5 % 24)));

	return (1103515243 * value + 12345);
}

/**
 * Allocate a new symbols for use in the hash of exported symbols or
 * the list of unresolved symbols per module
 **/
<<<<<<< HEAD
static struct symbol *alloc_symbol(const char *name, unsigned int weak,
				   struct symbol *next)
=======
static struct symbol *alloc_symbol(const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct symbol *s = NOFAIL(malloc(sizeof(*s) + strlen(name) + 1));

	memset(s, 0, sizeof(*s));
	strcpy(s->name, name);
<<<<<<< HEAD
	s->weak = weak;
	s->next = next;
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return s;
}

/* For the hash of exported symbols */
<<<<<<< HEAD
static struct symbol *new_symbol(const char *name, struct module *module,
				 enum export export)
{
	unsigned int hash;
	struct symbol *new;

	hash = tdb_hash(name) % SYMBOL_HASH_SIZE;
	new = symbolhash[hash] = alloc_symbol(name, 0, symbolhash[hash]);
	new->module = module;
	new->export = export;
	return new;
}

static struct symbol *find_symbol(const char *name)
=======
static void hash_add_symbol(struct symbol *sym)
{
	unsigned int hash;

	hash = tdb_hash(sym->name) % SYMBOL_HASH_SIZE;
	sym->next = symbolhash[hash];
	symbolhash[hash] = sym;
}

static void sym_add_unresolved(const char *name, struct module *mod, bool weak)
{
	struct symbol *sym;

	sym = alloc_symbol(name);
	sym->weak = weak;

	list_add_tail(&sym->list, &mod->unresolved_symbols);
}

static struct symbol *sym_find_with_module(const char *name, struct module *mod)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct symbol *s;

	/* For our purposes, .foo matches foo.  PPC64 needs this. */
	if (name[0] == '.')
		name++;

	for (s = symbolhash[tdb_hash(name) % SYMBOL_HASH_SIZE]; s; s = s->next) {
<<<<<<< HEAD
		if (strcmp(s->name, name) == 0)
=======
		if (strcmp(s->name, name) == 0 && (!mod || s->module == mod))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return s;
	}
	return NULL;
}

<<<<<<< HEAD
static struct {
	const char *str;
	enum export export;
} export_list[] = {
	{ .str = "EXPORT_SYMBOL",            .export = export_plain },
	{ .str = "EXPORT_UNUSED_SYMBOL",     .export = export_unused },
	{ .str = "EXPORT_SYMBOL_GPL",        .export = export_gpl },
	{ .str = "EXPORT_UNUSED_SYMBOL_GPL", .export = export_unused_gpl },
	{ .str = "EXPORT_SYMBOL_GPL_FUTURE", .export = export_gpl_future },
	{ .str = "(unknown)",                .export = export_unknown },
};


static const char *export_str(enum export ex)
{
	return export_list[ex].str;
}

static enum export export_no(const char *s)
{
	int i;

	if (!s)
		return export_unknown;
	for (i = 0; export_list[i].export != export_unknown; i++) {
		if (strcmp(export_list[i].str, s) == 0)
			return export_list[i].export;
	}
	return export_unknown;
}

static const char *sec_name(struct elf_info *elf, int secindex);

#define strstarts(str, prefix) (strncmp(str, prefix, strlen(prefix)) == 0)

static enum export export_from_secname(struct elf_info *elf, unsigned int sec)
{
	const char *secname = sec_name(elf, sec);

	if (strstarts(secname, "___ksymtab+"))
		return export_plain;
	else if (strstarts(secname, "___ksymtab_unused+"))
		return export_unused;
	else if (strstarts(secname, "___ksymtab_gpl+"))
		return export_gpl;
	else if (strstarts(secname, "___ksymtab_unused_gpl+"))
		return export_unused_gpl;
	else if (strstarts(secname, "___ksymtab_gpl_future+"))
		return export_gpl_future;
	else
		return export_unknown;
}

static enum export export_from_sec(struct elf_info *elf, unsigned int sec)
{
	if (sec == elf->export_sec)
		return export_plain;
	else if (sec == elf->export_unused_sec)
		return export_unused;
	else if (sec == elf->export_gpl_sec)
		return export_gpl;
	else if (sec == elf->export_unused_gpl_sec)
		return export_unused_gpl;
	else if (sec == elf->export_gpl_future_sec)
		return export_gpl_future;
	else
		return export_unknown;
}

/**
 * Add an exported symbol - it may have already been added without a
 * CRC, in this case just update the CRC
 **/
static struct symbol *sym_add_exported(const char *name, struct module *mod,
				       enum export export)
{
	struct symbol *s = find_symbol(name);

	if (!s) {
		s = new_symbol(name, mod, export);
	} else {
		if (!s->preloaded) {
			warn("%s: '%s' exported twice. Previous export "
			     "was in %s%s\n", mod->name, name,
			     s->module->name,
			     is_vmlinux(s->module->name) ?"":".ko");
		} else {
			/* In case Modules.symvers was out of date */
			s->module = mod;
		}
	}
	s->preloaded = 0;
	s->vmlinux   = is_vmlinux(mod->name);
	s->kernel    = 0;
	s->export    = export;
	return s;
}

static void sym_update_crc(const char *name, struct module *mod,
			   unsigned int crc, enum export export)
{
	struct symbol *s = find_symbol(name);

	if (!s)
		s = new_symbol(name, mod, export);
	s->crc = crc;
	s->crc_valid = 1;
}

void *grab_file(const char *filename, unsigned long *size)
{
	struct stat st;
	void *map;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0 || fstat(fd, &st) != 0)
		return NULL;

	*size = st.st_size;
	map = mmap(NULL, *size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	close(fd);

=======
static struct symbol *find_symbol(const char *name)
{
	return sym_find_with_module(name, NULL);
}

struct namespace_list {
	struct list_head list;
	char namespace[];
};

static bool contains_namespace(struct list_head *head, const char *namespace)
{
	struct namespace_list *list;

	/*
	 * The default namespace is null string "", which is always implicitly
	 * contained.
	 */
	if (!namespace[0])
		return true;

	list_for_each_entry(list, head, list) {
		if (!strcmp(list->namespace, namespace))
			return true;
	}

	return false;
}

static void add_namespace(struct list_head *head, const char *namespace)
{
	struct namespace_list *ns_entry;

	if (!contains_namespace(head, namespace)) {
		ns_entry = NOFAIL(malloc(sizeof(*ns_entry) +
					 strlen(namespace) + 1));
		strcpy(ns_entry->namespace, namespace);
		list_add_tail(&ns_entry->list, head);
	}
}

static void *sym_get_data_by_offset(const struct elf_info *info,
				    unsigned int secindex, unsigned long offset)
{
	Elf_Shdr *sechdr = &info->sechdrs[secindex];

	return (void *)info->hdr + sechdr->sh_offset + offset;
}

void *sym_get_data(const struct elf_info *info, const Elf_Sym *sym)
{
	return sym_get_data_by_offset(info, get_secindex(info, sym),
				      sym->st_value);
}

static const char *sech_name(const struct elf_info *info, Elf_Shdr *sechdr)
{
	return sym_get_data_by_offset(info, info->secindex_strings,
				      sechdr->sh_name);
}

static const char *sec_name(const struct elf_info *info, unsigned int secindex)
{
	/*
	 * If sym->st_shndx is a special section index, there is no
	 * corresponding section header.
	 * Return "" if the index is out of range of info->sechdrs[] array.
	 */
	if (secindex >= info->num_sections)
		return "";

	return sech_name(info, &info->sechdrs[secindex]);
}

#define strstarts(str, prefix) (strncmp(str, prefix, strlen(prefix)) == 0)

static struct symbol *sym_add_exported(const char *name, struct module *mod,
				       bool gpl_only, const char *namespace)
{
	struct symbol *s = find_symbol(name);

	if (s && (!external_module || s->module->is_vmlinux || s->module == mod)) {
		error("%s: '%s' exported twice. Previous export was in %s%s\n",
		      mod->name, name, s->module->name,
		      s->module->is_vmlinux ? "" : ".ko");
	}

	s = alloc_symbol(name);
	s->module = mod;
	s->is_gpl_only = gpl_only;
	s->namespace = NOFAIL(strdup(namespace));
	list_add_tail(&s->list, &mod->exported_symbols);
	hash_add_symbol(s);

	return s;
}

static void sym_set_crc(struct symbol *sym, unsigned int crc)
{
	sym->crc = crc;
	sym->crc_valid = true;
}

static void *grab_file(const char *filename, size_t *size)
{
	struct stat st;
	void *map = MAP_FAILED;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return NULL;
	if (fstat(fd, &st))
		goto failed;

	*size = st.st_size;
	map = mmap(NULL, *size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

failed:
	close(fd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (map == MAP_FAILED)
		return NULL;
	return map;
}

<<<<<<< HEAD
/**
  * Return a copy of the next line in a mmap'ed file.
  * spaces in the beginning of the line is trimmed away.
  * Return a pointer to a static buffer.
  **/
char *get_next_line(unsigned long *pos, void *file, unsigned long size)
{
	static char line[4096];
	int skip = 1;
	size_t len = 0;
	signed char *p = (signed char *)file + *pos;
	char *s = line;

	for (; *pos < size ; (*pos)++) {
		if (skip && isspace(*p)) {
			p++;
			continue;
		}
		skip = 0;
		if (*p != '\n' && (*pos < size)) {
			len++;
			*s++ = *p++;
			if (len > 4095)
				break; /* Too long, stop */
		} else {
			/* End of string */
			*s = '\0';
			return line;
		}
	}
	/* End of buffer */
	return NULL;
}

void release_file(void *file, unsigned long size)
=======
static void release_file(void *file, size_t size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	munmap(file, size);
}

static int parse_elf(struct elf_info *info, const char *filename)
{
	unsigned int i;
	Elf_Ehdr *hdr;
	Elf_Shdr *sechdrs;
	Elf_Sym  *sym;
	const char *secstrings;
	unsigned int symtab_idx = ~0U, symtab_shndx_idx = ~0U;

	hdr = grab_file(filename, &info->size);
	if (!hdr) {
<<<<<<< HEAD
=======
		if (ignore_missing_files) {
			fprintf(stderr, "%s: %s (ignored)\n", filename,
				strerror(errno));
			return 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		perror(filename);
		exit(1);
	}
	info->hdr = hdr;
	if (info->size < sizeof(*hdr)) {
		/* file too small, assume this is an empty .o file */
		return 0;
	}
	/* Is this a valid ELF file? */
	if ((hdr->e_ident[EI_MAG0] != ELFMAG0) ||
	    (hdr->e_ident[EI_MAG1] != ELFMAG1) ||
	    (hdr->e_ident[EI_MAG2] != ELFMAG2) ||
	    (hdr->e_ident[EI_MAG3] != ELFMAG3)) {
		/* Not an ELF file - silently ignore it */
		return 0;
	}
	/* Fix endianness in ELF header */
	hdr->e_type      = TO_NATIVE(hdr->e_type);
	hdr->e_machine   = TO_NATIVE(hdr->e_machine);
	hdr->e_version   = TO_NATIVE(hdr->e_version);
	hdr->e_entry     = TO_NATIVE(hdr->e_entry);
	hdr->e_phoff     = TO_NATIVE(hdr->e_phoff);
	hdr->e_shoff     = TO_NATIVE(hdr->e_shoff);
	hdr->e_flags     = TO_NATIVE(hdr->e_flags);
	hdr->e_ehsize    = TO_NATIVE(hdr->e_ehsize);
	hdr->e_phentsize = TO_NATIVE(hdr->e_phentsize);
	hdr->e_phnum     = TO_NATIVE(hdr->e_phnum);
	hdr->e_shentsize = TO_NATIVE(hdr->e_shentsize);
	hdr->e_shnum     = TO_NATIVE(hdr->e_shnum);
	hdr->e_shstrndx  = TO_NATIVE(hdr->e_shstrndx);
	sechdrs = (void *)hdr + hdr->e_shoff;
	info->sechdrs = sechdrs;

<<<<<<< HEAD
	/* Check if file offset is correct */
	if (hdr->e_shoff > info->size) {
		fatal("section header offset=%lu in file '%s' is bigger than "
		      "filesize=%lu\n", (unsigned long)hdr->e_shoff,
		      filename, info->size);
		return 0;
	}
=======
	/* modpost only works for relocatable objects */
	if (hdr->e_type != ET_REL)
		fatal("%s: not relocatable object.", filename);

	/* Check if file offset is correct */
	if (hdr->e_shoff > info->size)
		fatal("section header offset=%lu in file '%s' is bigger than filesize=%zu\n",
		      (unsigned long)hdr->e_shoff, filename, info->size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hdr->e_shnum == SHN_UNDEF) {
		/*
		 * There are more than 64k sections,
		 * read count from .sh_size.
		 */
		info->num_sections = TO_NATIVE(sechdrs[0].sh_size);
	}
	else {
		info->num_sections = hdr->e_shnum;
	}
	if (hdr->e_shstrndx == SHN_XINDEX) {
		info->secindex_strings = TO_NATIVE(sechdrs[0].sh_link);
	}
	else {
		info->secindex_strings = hdr->e_shstrndx;
	}

	/* Fix endianness in section headers */
	for (i = 0; i < info->num_sections; i++) {
		sechdrs[i].sh_name      = TO_NATIVE(sechdrs[i].sh_name);
		sechdrs[i].sh_type      = TO_NATIVE(sechdrs[i].sh_type);
		sechdrs[i].sh_flags     = TO_NATIVE(sechdrs[i].sh_flags);
		sechdrs[i].sh_addr      = TO_NATIVE(sechdrs[i].sh_addr);
		sechdrs[i].sh_offset    = TO_NATIVE(sechdrs[i].sh_offset);
		sechdrs[i].sh_size      = TO_NATIVE(sechdrs[i].sh_size);
		sechdrs[i].sh_link      = TO_NATIVE(sechdrs[i].sh_link);
		sechdrs[i].sh_info      = TO_NATIVE(sechdrs[i].sh_info);
		sechdrs[i].sh_addralign = TO_NATIVE(sechdrs[i].sh_addralign);
		sechdrs[i].sh_entsize   = TO_NATIVE(sechdrs[i].sh_entsize);
	}
	/* Find symbol table. */
	secstrings = (void *)hdr + sechdrs[info->secindex_strings].sh_offset;
	for (i = 1; i < info->num_sections; i++) {
		const char *secname;
		int nobits = sechdrs[i].sh_type == SHT_NOBITS;

<<<<<<< HEAD
		if (!nobits && sechdrs[i].sh_offset > info->size) {
			fatal("%s is truncated. sechdrs[i].sh_offset=%lu > "
			      "sizeof(*hrd)=%zu\n", filename,
			      (unsigned long)sechdrs[i].sh_offset,
			      sizeof(*hdr));
			return 0;
		}
=======
		if (!nobits && sechdrs[i].sh_offset > info->size)
			fatal("%s is truncated. sechdrs[i].sh_offset=%lu > sizeof(*hrd)=%zu\n",
			      filename, (unsigned long)sechdrs[i].sh_offset,
			      sizeof(*hdr));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		secname = secstrings + sechdrs[i].sh_name;
		if (strcmp(secname, ".modinfo") == 0) {
			if (nobits)
				fatal("%s has NOBITS .modinfo\n", filename);
			info->modinfo = (void *)hdr + sechdrs[i].sh_offset;
			info->modinfo_len = sechdrs[i].sh_size;
<<<<<<< HEAD
		} else if (strcmp(secname, "__ksymtab") == 0)
			info->export_sec = i;
		else if (strcmp(secname, "__ksymtab_unused") == 0)
			info->export_unused_sec = i;
		else if (strcmp(secname, "__ksymtab_gpl") == 0)
			info->export_gpl_sec = i;
		else if (strcmp(secname, "__ksymtab_unused_gpl") == 0)
			info->export_unused_gpl_sec = i;
		else if (strcmp(secname, "__ksymtab_gpl_future") == 0)
			info->export_gpl_future_sec = i;
=======
		} else if (!strcmp(secname, ".export_symbol")) {
			info->export_symbol_secndx = i;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (sechdrs[i].sh_type == SHT_SYMTAB) {
			unsigned int sh_link_idx;
			symtab_idx = i;
			info->symtab_start = (void *)hdr +
			    sechdrs[i].sh_offset;
			info->symtab_stop  = (void *)hdr +
			    sechdrs[i].sh_offset + sechdrs[i].sh_size;
			sh_link_idx = sechdrs[i].sh_link;
			info->strtab       = (void *)hdr +
			    sechdrs[sh_link_idx].sh_offset;
		}

		/* 32bit section no. table? ("more than 64k sections") */
		if (sechdrs[i].sh_type == SHT_SYMTAB_SHNDX) {
			symtab_shndx_idx = i;
			info->symtab_shndx_start = (void *)hdr +
			    sechdrs[i].sh_offset;
			info->symtab_shndx_stop  = (void *)hdr +
			    sechdrs[i].sh_offset + sechdrs[i].sh_size;
		}
	}
	if (!info->symtab_start)
		fatal("%s has no symtab?\n", filename);

	/* Fix endianness in symbols */
	for (sym = info->symtab_start; sym < info->symtab_stop; sym++) {
		sym->st_shndx = TO_NATIVE(sym->st_shndx);
		sym->st_name  = TO_NATIVE(sym->st_name);
		sym->st_value = TO_NATIVE(sym->st_value);
		sym->st_size  = TO_NATIVE(sym->st_size);
	}

	if (symtab_shndx_idx != ~0U) {
		Elf32_Word *p;
		if (symtab_idx != sechdrs[symtab_shndx_idx].sh_link)
			fatal("%s: SYMTAB_SHNDX has bad sh_link: %u!=%u\n",
			      filename, sechdrs[symtab_shndx_idx].sh_link,
			      symtab_idx);
		/* Fix endianness */
		for (p = info->symtab_shndx_start; p < info->symtab_shndx_stop;
		     p++)
			*p = TO_NATIVE(*p);
	}

<<<<<<< HEAD
=======
	symsearch_init(info);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

static void parse_elf_finish(struct elf_info *info)
{
<<<<<<< HEAD
=======
	symsearch_finish(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	release_file(info->hdr, info->size);
}

static int ignore_undef_symbol(struct elf_info *info, const char *symname)
{
	/* ignore __this_module, it will be resolved shortly */
<<<<<<< HEAD
	if (strcmp(symname, MODULE_SYMBOL_PREFIX "__this_module") == 0)
=======
	if (strcmp(symname, "__this_module") == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	/* ignore global offset table */
	if (strcmp(symname, "_GLOBAL_OFFSET_TABLE_") == 0)
		return 1;
	if (info->hdr->e_machine == EM_PPC)
		/* Special register function linked on all modules during final link of .ko */
<<<<<<< HEAD
		if (strncmp(symname, "_restgpr_", sizeof("_restgpr_") - 1) == 0 ||
		    strncmp(symname, "_savegpr_", sizeof("_savegpr_") - 1) == 0 ||
		    strncmp(symname, "_rest32gpr_", sizeof("_rest32gpr_") - 1) == 0 ||
		    strncmp(symname, "_save32gpr_", sizeof("_save32gpr_") - 1) == 0 ||
		    strncmp(symname, "_restvr_", sizeof("_restvr_") - 1) == 0 ||
		    strncmp(symname, "_savevr_", sizeof("_savevr_") - 1) == 0)
			return 1;
	if (info->hdr->e_machine == EM_PPC64)
		/* Special register function linked on all modules during final link of .ko */
		if (strncmp(symname, "_restgpr0_", sizeof("_restgpr0_") - 1) == 0 ||
		    strncmp(symname, "_savegpr0_", sizeof("_savegpr0_") - 1) == 0 ||
		    strncmp(symname, "_restvr_", sizeof("_restvr_") - 1) == 0 ||
		    strncmp(symname, "_savevr_", sizeof("_savevr_") - 1) == 0)
=======
		if (strstarts(symname, "_restgpr_") ||
		    strstarts(symname, "_savegpr_") ||
		    strstarts(symname, "_rest32gpr_") ||
		    strstarts(symname, "_save32gpr_") ||
		    strstarts(symname, "_restvr_") ||
		    strstarts(symname, "_savevr_"))
			return 1;
	if (info->hdr->e_machine == EM_PPC64)
		/* Special register function linked on all modules during final link of .ko */
		if (strstarts(symname, "_restgpr0_") ||
		    strstarts(symname, "_savegpr0_") ||
		    strstarts(symname, "_restvr_") ||
		    strstarts(symname, "_savevr_") ||
		    strcmp(symname, ".TOC.") == 0)
			return 1;

	if (info->hdr->e_machine == EM_S390)
		/* Expoline thunks are linked on all kernel modules during final link of .ko */
		if (strstarts(symname, "__s390_indirect_jump_r"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;
	/* Do not ignore this symbol */
	return 0;
}

<<<<<<< HEAD
#define CRC_PFX     MODULE_SYMBOL_PREFIX "__crc_"
#define KSYMTAB_PFX MODULE_SYMBOL_PREFIX "__ksymtab_"

static void handle_modversions(struct module *mod, struct elf_info *info,
			       Elf_Sym *sym, const char *symname)
{
	unsigned int crc;
	enum export export;

	if ((!is_vmlinux(mod->name) || mod->is_dot_o) &&
	    strncmp(symname, "__ksymtab", 9) == 0)
		export = export_from_secname(info, get_secindex(info, sym));
	else
		export = export_from_sec(info, get_secindex(info, sym));

	switch (sym->st_shndx) {
	case SHN_COMMON:
		warn("\"%s\" [%s] is COMMON symbol\n", symname, mod->name);
		break;
	case SHN_ABS:
		/* CRC'd symbol */
		if (strncmp(symname, CRC_PFX, strlen(CRC_PFX)) == 0) {
			crc = (unsigned int) sym->st_value;
			sym_update_crc(symname + strlen(CRC_PFX), mod, crc,
					export);
		}
=======
static void handle_symbol(struct module *mod, struct elf_info *info,
			  const Elf_Sym *sym, const char *symname)
{
	switch (sym->st_shndx) {
	case SHN_COMMON:
		if (strstarts(symname, "__gnu_lto_")) {
			/* Should warn here, but modpost runs before the linker */
		} else
			warn("\"%s\" [%s] is COMMON symbol\n", symname, mod->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case SHN_UNDEF:
		/* undefined symbol */
		if (ELF_ST_BIND(sym->st_info) != STB_GLOBAL &&
		    ELF_ST_BIND(sym->st_info) != STB_WEAK)
			break;
		if (ignore_undef_symbol(info, symname))
			break;
<<<<<<< HEAD
/* cope with newer glibc (2.3.4 or higher) STT_ definition in elf.h */
#if defined(STT_REGISTER) || defined(STT_SPARC_REGISTER)
/* add compatibility with older glibc */
#ifndef STT_SPARC_REGISTER
#define STT_SPARC_REGISTER STT_REGISTER
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (info->hdr->e_machine == EM_SPARC ||
		    info->hdr->e_machine == EM_SPARCV9) {
			/* Ignore register directives. */
			if (ELF_ST_TYPE(sym->st_info) == STT_SPARC_REGISTER)
				break;
			if (symname[0] == '.') {
<<<<<<< HEAD
				char *munged = strdup(symname);
=======
				char *munged = NOFAIL(strdup(symname));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				munged[0] = '_';
				munged[1] = toupper(munged[1]);
				symname = munged;
			}
		}
<<<<<<< HEAD
#endif

		if (memcmp(symname, MODULE_SYMBOL_PREFIX,
			   strlen(MODULE_SYMBOL_PREFIX)) == 0) {
			mod->unres =
			  alloc_symbol(symname +
			               strlen(MODULE_SYMBOL_PREFIX),
			               ELF_ST_BIND(sym->st_info) == STB_WEAK,
			               mod->unres);
		}
		break;
	default:
		/* All exported symbols */
		if (strncmp(symname, KSYMTAB_PFX, strlen(KSYMTAB_PFX)) == 0) {
			sym_add_exported(symname + strlen(KSYMTAB_PFX), mod,
					export);
		}
		if (strcmp(symname, MODULE_SYMBOL_PREFIX "init_module") == 0)
			mod->has_init = 1;
		if (strcmp(symname, MODULE_SYMBOL_PREFIX "cleanup_module") == 0)
			mod->has_cleanup = 1;
=======

		sym_add_unresolved(symname, mod,
				   ELF_ST_BIND(sym->st_info) == STB_WEAK);
		break;
	default:
		if (strcmp(symname, "init_module") == 0)
			mod->has_init = true;
		if (strcmp(symname, "cleanup_module") == 0)
			mod->has_cleanup = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

/**
 * Parse tag=value strings from .modinfo section
 **/
static char *next_string(char *string, unsigned long *secsize)
{
	/* Skip non-zero chars */
	while (string[0]) {
		string++;
		if ((*secsize)-- <= 1)
			return NULL;
	}

	/* Skip any zero padding. */
	while (!string[0]) {
		string++;
		if ((*secsize)-- <= 1)
			return NULL;
	}
	return string;
}

<<<<<<< HEAD
static char *get_next_modinfo(void *modinfo, unsigned long modinfo_len,
			      const char *tag, char *info)
{
	char *p;
	unsigned int taglen = strlen(tag);
	unsigned long size = modinfo_len;

	if (info) {
		size -= info - (char *)modinfo;
		modinfo = next_string(info, &size);
=======
static char *get_next_modinfo(struct elf_info *info, const char *tag,
			      char *prev)
{
	char *p;
	unsigned int taglen = strlen(tag);
	char *modinfo = info->modinfo;
	unsigned long size = info->modinfo_len;

	if (prev) {
		size -= prev - modinfo;
		modinfo = next_string(prev, &size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	for (p = modinfo; p; p = next_string(p, &size)) {
		if (strncmp(p, tag, taglen) == 0 && p[taglen] == '=')
			return p + taglen + 1;
	}
	return NULL;
}

<<<<<<< HEAD
static char *get_modinfo(void *modinfo, unsigned long modinfo_len,
			 const char *tag)

{
	return get_next_modinfo(modinfo, modinfo_len, tag, NULL);
}

/**
 * Test if string s ends in string sub
 * return 0 if match
 **/
static int strrcmp(const char *s, const char *sub)
{
	int slen, sublen;

	if (!s || !sub)
		return 1;

	slen = strlen(s);
	sublen = strlen(sub);

	if ((slen == 0) || (sublen == 0))
		return 1;

	if (sublen > slen)
		return 1;

	return memcmp(s + slen - sublen, sub, sublen);
=======
static char *get_modinfo(struct elf_info *info, const char *tag)

{
	return get_next_modinfo(info, tag, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const char *sym_name(struct elf_info *elf, Elf_Sym *sym)
{
	if (sym)
		return elf->strtab + sym->st_name;
	else
		return "(unknown)";
}

<<<<<<< HEAD
static const char *sec_name(struct elf_info *elf, int secindex)
{
	Elf_Shdr *sechdrs = elf->sechdrs;
	return (void *)elf->hdr +
		elf->sechdrs[elf->secindex_strings].sh_offset +
		sechdrs[secindex].sh_name;
}

static const char *sech_name(struct elf_info *elf, Elf_Shdr *sechdr)
{
	return (void *)elf->hdr +
		elf->sechdrs[elf->secindex_strings].sh_offset +
		sechdr->sh_name;
}

/* if sym is empty or point to a string
 * like ".[0-9]+" then return 1.
 * This is the optional prefix added by ld to some sections
 */
static int number_prefix(const char *sym)
{
	if (*sym++ == '\0')
		return 1;
	if (*sym != '.')
		return 0;
	do {
		char c = *sym++;
		if (c < '0' || c > '9')
			return 0;
	} while (*sym);
	return 1;
}

/* The pattern is an array of simple patterns.
 * "foo" will match an exact string equal to "foo"
 * "*foo" will match a string that ends with "foo"
 * "foo*" will match a string that begins with "foo"
 * "foo$" will match a string equal to "foo" or "foo.1"
 *   where the '1' can be any number including several digits.
 *   The $ syntax is for sections where ld append a dot number
 *   to make section name unique.
 */
static int match(const char *sym, const char * const pat[])
{
	const char *p;
	while (*pat) {
		p = *pat++;
		const char *endp = p + strlen(p) - 1;

		/* "*foo" */
		if (*p == '*') {
			if (strrcmp(sym, p + 1) == 0)
				return 1;
		}
		/* "foo*" */
		else if (*endp == '*') {
			if (strncmp(sym, p, strlen(p) - 1) == 0)
				return 1;
		}
		/* "foo$" */
		else if (*endp == '$') {
			if (strncmp(sym, p, strlen(p) - 1) == 0) {
				if (number_prefix(sym + strlen(p) - 1))
					return 1;
			}
		}
		/* no wildcards */
		else {
			if (strcmp(p, sym) == 0)
				return 1;
		}
	}
	/* no match */
	return 0;
}

/* sections that we do not want to do full section mismatch check on */
static const char *section_white_list[] =
=======
/*
 * Check whether the 'string' argument matches one of the 'patterns',
 * an array of shell wildcard patterns (glob).
 *
 * Return true is there is a match.
 */
static bool match(const char *string, const char *const patterns[])
{
	const char *pattern;

	while ((pattern = *patterns++)) {
		if (!fnmatch(pattern, string, 0))
			return true;
	}

	return false;
}

/* useful to pass patterns to match() directly */
#define PATTERNS(...) \
	({ \
		static const char *const patterns[] = {__VA_ARGS__, NULL}; \
		patterns; \
	})

/* sections that we do not want to do full section mismatch check on */
static const char *const section_white_list[] =
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	".comment*",
	".debug*",
	".zdebug*",		/* Compressed debug sections. */
<<<<<<< HEAD
	".GCC-command-line",	/* mn10300 */
=======
	".GCC.command.line",	/* record-gcc-switches */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	".mdebug*",        /* alpha, score, mips etc. */
	".pdr",            /* alpha, score, mips etc. */
	".stab*",
	".note*",
	".got*",
	".toc*",
<<<<<<< HEAD
=======
	".xt.prop",				 /* xtensa */
	".xt.lit",         /* xtensa */
	".arcextmap*",			/* arc */
	".gnu.linkonce.arcext*",	/* arc : modules */
	".cmem*",			/* EZchip */
	".fmt_slot*",			/* EZchip */
	".gnu.lto*",
	".discard.*",
	".llvm.call-graph-profile",	/* call graph */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL
};

/*
 * This is used to find sections missing the SHF_ALLOC flag.
 * The cause of this is often a section specified in assembler
 * without "ax" / "aw".
 */
static void check_section(const char *modname, struct elf_info *elf,
<<<<<<< HEAD
                          Elf_Shdr *sechdr)
=======
			  Elf_Shdr *sechdr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *sec = sech_name(elf, sechdr);

	if (sechdr->sh_type == SHT_PROGBITS &&
	    !(sechdr->sh_flags & SHF_ALLOC) &&
	    !match(sec, section_white_list)) {
		warn("%s (%s): unexpected non-allocatable section.\n"
		     "Did you forget to use \"ax\"/\"aw\" in a .S file?\n"
		     "Note that for example <linux/init.h> contains\n"
		     "section definitions for use in .S files.\n\n",
		     modname, sec);
	}
}



#define ALL_INIT_DATA_SECTIONS \
<<<<<<< HEAD
	".init.setup$", ".init.rodata$", \
	".devinit.rodata$", ".cpuinit.rodata$", ".meminit.rodata$", \
	".init.data$", ".devinit.data$", ".cpuinit.data$", ".meminit.data$"
#define ALL_EXIT_DATA_SECTIONS \
	".exit.data$", ".devexit.data$", ".cpuexit.data$", ".memexit.data$"

#define ALL_INIT_TEXT_SECTIONS \
	".init.text$", ".devinit.text$", ".cpuinit.text$", ".meminit.text$"
#define ALL_EXIT_TEXT_SECTIONS \
	".exit.text$", ".devexit.text$", ".cpuexit.text$", ".memexit.text$"

#define ALL_XXXINIT_SECTIONS DEV_INIT_SECTIONS, CPU_INIT_SECTIONS, \
	MEM_INIT_SECTIONS
#define ALL_XXXEXIT_SECTIONS DEV_EXIT_SECTIONS, CPU_EXIT_SECTIONS, \
	MEM_EXIT_SECTIONS

#define ALL_INIT_SECTIONS INIT_SECTIONS, ALL_XXXINIT_SECTIONS
#define ALL_EXIT_SECTIONS EXIT_SECTIONS, ALL_XXXEXIT_SECTIONS

#define DATA_SECTIONS ".data$", ".data.rel$"
#define TEXT_SECTIONS ".text$"

#define INIT_SECTIONS      ".init.*"
#define DEV_INIT_SECTIONS  ".devinit.*"
#define CPU_INIT_SECTIONS  ".cpuinit.*"
#define MEM_INIT_SECTIONS  ".meminit.*"

#define EXIT_SECTIONS      ".exit.*"
#define DEV_EXIT_SECTIONS  ".devexit.*"
#define CPU_EXIT_SECTIONS  ".cpuexit.*"
#define MEM_EXIT_SECTIONS  ".memexit.*"

/* init data sections */
static const char *init_data_sections[] = { ALL_INIT_DATA_SECTIONS, NULL };

/* all init sections */
static const char *init_sections[] = { ALL_INIT_SECTIONS, NULL };

/* All init and exit sections (code + data) */
static const char *init_exit_sections[] =
	{ALL_INIT_SECTIONS, ALL_EXIT_SECTIONS, NULL };

/* data section */
static const char *data_sections[] = { DATA_SECTIONS, NULL };


/* symbols in .data that may refer to init/exit sections */
#define DEFAULT_SYMBOL_WHITE_LIST					\
	"*driver",							\
	"*_template", /* scsi uses *_template a lot */			\
	"*_timer",    /* arm uses ops structures named _timer a lot */	\
	"*_sht",      /* scsi also used *_sht to some extent */		\
	"*_ops",							\
	"*_probe",							\
	"*_probe_one",							\
	"*_console"

static const char *head_sections[] = { ".head.text*", NULL };
static const char *linker_symbols[] =
	{ "__init_begin", "_sinittext", "_einittext", NULL };

enum mismatch {
	TEXT_TO_ANY_INIT,
	DATA_TO_ANY_INIT,
	TEXT_TO_ANY_EXIT,
	DATA_TO_ANY_EXIT,
	XXXINIT_TO_SOME_INIT,
	XXXEXIT_TO_SOME_EXIT,
	ANY_INIT_TO_ANY_EXIT,
	ANY_EXIT_TO_ANY_INIT,
	EXPORT_TO_INIT_EXIT,
};

struct sectioncheck {
	const char *fromsec[20];
	const char *tosec[20];
	enum mismatch mismatch;
	const char *symbol_white_list[20];
};

const struct sectioncheck sectioncheck[] = {
=======
	".init.setup", ".init.rodata", ".meminit.rodata", \
	".init.data", ".meminit.data"

#define ALL_PCI_INIT_SECTIONS	\
	".pci_fixup_early", ".pci_fixup_header", ".pci_fixup_final", \
	".pci_fixup_enable", ".pci_fixup_resume", \
	".pci_fixup_resume_early", ".pci_fixup_suspend"

#define ALL_XXXINIT_SECTIONS ".meminit.*"

#define ALL_INIT_SECTIONS INIT_SECTIONS, ALL_XXXINIT_SECTIONS
#define ALL_EXIT_SECTIONS ".exit.*"

#define DATA_SECTIONS ".data", ".data.rel"
#define TEXT_SECTIONS ".text", ".text.*", ".sched.text", \
		".kprobes.text", ".cpuidle.text", ".noinstr.text", \
		".ltext", ".ltext.*"
#define OTHER_TEXT_SECTIONS ".ref.text", ".head.text", ".spinlock.text", \
		".fixup", ".entry.text", ".exception.text", \
		".coldtext", ".softirqentry.text"

#define INIT_SECTIONS      ".init.*"

#define ALL_TEXT_SECTIONS  ".init.text", ".meminit.text", ".exit.text", \
		TEXT_SECTIONS, OTHER_TEXT_SECTIONS

enum mismatch {
	TEXTDATA_TO_ANY_INIT_EXIT,
	XXXINIT_TO_SOME_INIT,
	ANY_INIT_TO_ANY_EXIT,
	ANY_EXIT_TO_ANY_INIT,
	EXTABLE_TO_NON_TEXT,
};

/**
 * Describe how to match sections on different criteria:
 *
 * @fromsec: Array of sections to be matched.
 *
 * @bad_tosec: Relocations applied to a section in @fromsec to a section in
 * this array is forbidden (black-list).  Can be empty.
 *
 * @good_tosec: Relocations applied to a section in @fromsec must be
 * targeting sections in this array (white-list).  Can be empty.
 *
 * @mismatch: Type of mismatch.
 */
struct sectioncheck {
	const char *fromsec[20];
	const char *bad_tosec[20];
	const char *good_tosec[20];
	enum mismatch mismatch;
};

static const struct sectioncheck sectioncheck[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Do not reference init/exit code/data from
 * normal code and data
 */
{
<<<<<<< HEAD
	.fromsec = { TEXT_SECTIONS, NULL },
	.tosec   = { ALL_INIT_SECTIONS, NULL },
	.mismatch = TEXT_TO_ANY_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
{
	.fromsec = { DATA_SECTIONS, NULL },
	.tosec   = { ALL_XXXINIT_SECTIONS, NULL },
	.mismatch = DATA_TO_ANY_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
{
	.fromsec = { DATA_SECTIONS, NULL },
	.tosec   = { INIT_SECTIONS, NULL },
	.mismatch = DATA_TO_ANY_INIT,
	.symbol_white_list = {
		"*_template", "*_timer", "*_sht", "*_ops",
		"*_probe", "*_probe_one", "*_console", NULL
	},
},
{
	.fromsec = { TEXT_SECTIONS, NULL },
	.tosec   = { ALL_EXIT_SECTIONS, NULL },
	.mismatch = TEXT_TO_ANY_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
{
	.fromsec = { DATA_SECTIONS, NULL },
	.tosec   = { ALL_EXIT_SECTIONS, NULL },
	.mismatch = DATA_TO_ANY_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference init code/data from devinit/cpuinit/meminit code/data */
{
	.fromsec = { ALL_XXXINIT_SECTIONS, NULL },
	.tosec   = { INIT_SECTIONS, NULL },
	.mismatch = XXXINIT_TO_SOME_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference cpuinit code/data from meminit code/data */
{
	.fromsec = { MEM_INIT_SECTIONS, NULL },
	.tosec   = { CPU_INIT_SECTIONS, NULL },
	.mismatch = XXXINIT_TO_SOME_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference meminit code/data from cpuinit code/data */
{
	.fromsec = { CPU_INIT_SECTIONS, NULL },
	.tosec   = { MEM_INIT_SECTIONS, NULL },
	.mismatch = XXXINIT_TO_SOME_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference exit code/data from devexit/cpuexit/memexit code/data */
{
	.fromsec = { ALL_XXXEXIT_SECTIONS, NULL },
	.tosec   = { EXIT_SECTIONS, NULL },
	.mismatch = XXXEXIT_TO_SOME_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference cpuexit code/data from memexit code/data */
{
	.fromsec = { MEM_EXIT_SECTIONS, NULL },
	.tosec   = { CPU_EXIT_SECTIONS, NULL },
	.mismatch = XXXEXIT_TO_SOME_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not reference memexit code/data from cpuexit code/data */
{
	.fromsec = { CPU_EXIT_SECTIONS, NULL },
	.tosec   = { MEM_EXIT_SECTIONS, NULL },
	.mismatch = XXXEXIT_TO_SOME_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
=======
	.fromsec = { TEXT_SECTIONS, DATA_SECTIONS, NULL },
	.bad_tosec = { ALL_INIT_SECTIONS, ALL_EXIT_SECTIONS, NULL },
	.mismatch = TEXTDATA_TO_ANY_INIT_EXIT,
},
/* Do not reference init code/data from meminit code/data */
{
	.fromsec = { ALL_XXXINIT_SECTIONS, NULL },
	.bad_tosec = { INIT_SECTIONS, NULL },
	.mismatch = XXXINIT_TO_SOME_INIT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
},
/* Do not use exit code/data from init code */
{
	.fromsec = { ALL_INIT_SECTIONS, NULL },
<<<<<<< HEAD
	.tosec   = { ALL_EXIT_SECTIONS, NULL },
	.mismatch = ANY_INIT_TO_ANY_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
=======
	.bad_tosec = { ALL_EXIT_SECTIONS, NULL },
	.mismatch = ANY_INIT_TO_ANY_EXIT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
},
/* Do not use init code/data from exit code */
{
	.fromsec = { ALL_EXIT_SECTIONS, NULL },
<<<<<<< HEAD
	.tosec   = { ALL_INIT_SECTIONS, NULL },
	.mismatch = ANY_EXIT_TO_ANY_INIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
},
/* Do not export init/exit functions or data */
{
	.fromsec = { "__ksymtab*", NULL },
	.tosec   = { INIT_SECTIONS, EXIT_SECTIONS, NULL },
	.mismatch = EXPORT_TO_INIT_EXIT,
	.symbol_white_list = { DEFAULT_SYMBOL_WHITE_LIST, NULL },
=======
	.bad_tosec = { ALL_INIT_SECTIONS, NULL },
	.mismatch = ANY_EXIT_TO_ANY_INIT,
},
{
	.fromsec = { ALL_PCI_INIT_SECTIONS, NULL },
	.bad_tosec = { INIT_SECTIONS, NULL },
	.mismatch = ANY_INIT_TO_ANY_EXIT,
},
{
	.fromsec = { "__ex_table", NULL },
	/* If you're adding any new black-listed sections in here, consider
	 * adding a special 'printer' for them in scripts/check_extable.
	 */
	.bad_tosec = { ".altinstr_replacement", NULL },
	.good_tosec = {ALL_TEXT_SECTIONS , NULL},
	.mismatch = EXTABLE_TO_NON_TEXT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
};

static const struct sectioncheck *section_mismatch(
		const char *fromsec, const char *tosec)
{
	int i;
<<<<<<< HEAD
	int elems = sizeof(sectioncheck) / sizeof(struct sectioncheck);
	const struct sectioncheck *check = &sectioncheck[0];

	for (i = 0; i < elems; i++) {
		if (match(fromsec, check->fromsec) &&
		    match(tosec, check->tosec))
			return check;
		check++;
=======

	/*
	 * The target section could be the SHT_NUL section when we're
	 * handling relocations to un-resolved symbols, trying to match it
	 * doesn't make much sense and causes build failures on parisc
	 * architectures.
	 */
	if (*tosec == '\0')
		return NULL;

	for (i = 0; i < ARRAY_SIZE(sectioncheck); i++) {
		const struct sectioncheck *check = &sectioncheck[i];

		if (match(fromsec, check->fromsec)) {
			if (check->bad_tosec[0] && match(tosec, check->bad_tosec))
				return check;
			if (check->good_tosec[0] && !match(tosec, check->good_tosec))
				return check;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return NULL;
}

/**
 * Whitelist to allow certain references to pass with no warning.
 *
 * Pattern 1:
 *   If a module parameter is declared __initdata and permissions=0
 *   then this is legal despite the warning generated.
 *   We cannot see value of permissions here, so just ignore
 *   this pattern.
 *   The pattern is identified by:
 *   tosec   = .init.data
 *   fromsec = .data*
 *   atsym   =__param*
 *
 * Pattern 1a:
 *   module_param_call() ops can refer to __init set function if permissions=0
 *   The pattern is identified by:
 *   tosec   = .init.text
 *   fromsec = .data*
 *   atsym   = __param_ops_*
 *
<<<<<<< HEAD
 * Pattern 2:
 *   Many drivers utilise a *driver container with references to
 *   add, remove, probe functions etc.
 *   These functions may often be marked __devinit and we do not want to
 *   warn here.
 *   the pattern is identified by:
 *   tosec   = init or exit section
 *   fromsec = data section
 *   atsym = *driver, *_template, *_sht, *_ops, *_probe,
 *           *probe_one, *_console, *_timer
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Pattern 3:
 *   Whitelist all references from .head.text to any init section
 *
 * Pattern 4:
 *   Some symbols belong to init section but still it is ok to reference
 *   these from non-init sections as these symbols don't have any memory
 *   allocated for them and symbol address and value are same. So even
 *   if init section is freed, its ok to reference those symbols.
 *   For ex. symbols marking the init section boundaries.
 *   This pattern is identified by
 *   refsymname = __init_begin, _sinittext, _einittext
 *
<<<<<<< HEAD
 **/
static int secref_whitelist(const struct sectioncheck *mismatch,
			    const char *fromsec, const char *fromsym,
			    const char *tosec, const char *tosym)
{
	/* Check for pattern 1 */
	if (match(tosec, init_data_sections) &&
	    match(fromsec, data_sections) &&
	    (strncmp(fromsym, "__param", strlen("__param")) == 0))
=======
 * Pattern 5:
 *   GCC may optimize static inlines when fed constant arg(s) resulting
 *   in functions like cpumask_empty() -- generating an associated symbol
 *   cpumask_empty.constprop.3 that appears in the audit.  If the const that
 *   is passed in comes from __init, like say nmi_ipi_mask, we get a
 *   meaningless section warning.  May need to add isra symbols too...
 *   This pattern is identified by
 *   tosec   = init section
 *   fromsec = text section
 *   refsymname = *.constprop.*
 *
 **/
static int secref_whitelist(const char *fromsec, const char *fromsym,
			    const char *tosec, const char *tosym)
{
	/* Check for pattern 1 */
	if (match(tosec, PATTERNS(ALL_INIT_DATA_SECTIONS)) &&
	    match(fromsec, PATTERNS(DATA_SECTIONS)) &&
	    strstarts(fromsym, "__param"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	/* Check for pattern 1a */
	if (strcmp(tosec, ".init.text") == 0 &&
<<<<<<< HEAD
	    match(fromsec, data_sections) &&
	    (strncmp(fromsym, "__param_ops_", strlen("__param_ops_")) == 0))
		return 0;

	/* Check for pattern 2 */
	if (match(tosec, init_exit_sections) &&
	    match(fromsec, data_sections) &&
	    match(fromsym, mismatch->symbol_white_list))
		return 0;

	/* Check for pattern 3 */
	if (match(fromsec, head_sections) &&
	    match(tosec, init_sections))
		return 0;

	/* Check for pattern 4 */
	if (match(tosym, linker_symbols))
=======
	    match(fromsec, PATTERNS(DATA_SECTIONS)) &&
	    strstarts(fromsym, "__param_ops_"))
		return 0;

	/* symbols in data sections that may refer to any init/exit sections */
	if (match(fromsec, PATTERNS(DATA_SECTIONS)) &&
	    match(tosec, PATTERNS(ALL_INIT_SECTIONS, ALL_EXIT_SECTIONS)) &&
	    match(fromsym, PATTERNS("*_ops", "*_probe", "*_console")))
		return 0;

	/*
	 * symbols in data sections must not refer to .exit.*, but there are
	 * quite a few offenders, so hide these unless for W=1 builds until
	 * these are fixed.
	 */
	if (!extra_warn &&
	    match(fromsec, PATTERNS(DATA_SECTIONS)) &&
	    match(tosec, PATTERNS(ALL_EXIT_SECTIONS)) &&
	    match(fromsym, PATTERNS("*driver")))
		return 0;

	/* Check for pattern 3 */
	if (strstarts(fromsec, ".head.text") &&
	    match(tosec, PATTERNS(ALL_INIT_SECTIONS)))
		return 0;

	/* Check for pattern 4 */
	if (match(tosym, PATTERNS("__init_begin", "_sinittext", "_einittext")))
		return 0;

	/* Check for pattern 5 */
	if (match(fromsec, PATTERNS(ALL_TEXT_SECTIONS)) &&
	    match(tosec, PATTERNS(ALL_INIT_SECTIONS)) &&
	    match(fromsym, PATTERNS("*.constprop.*")))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return 1;
}

<<<<<<< HEAD
/**
 * Find symbol based on relocation record info.
 * In some cases the symbol supplied is a valid symbol so
 * return refsym. If st_name != 0 we assume this is a valid symbol.
 * In other cases the symbol needs to be looked up in the symbol table
 * based on section and address.
 *  **/
static Elf_Sym *find_elf_symbol(struct elf_info *elf, Elf64_Sword addr,
				Elf_Sym *relsym)
{
	Elf_Sym *sym;
	Elf_Sym *near = NULL;
	Elf64_Sword distance = 20;
	Elf64_Sword d;
	unsigned int relsym_secindex;

	if (relsym->st_name != 0)
		return relsym;

	relsym_secindex = get_secindex(elf, relsym);
	for (sym = elf->symtab_start; sym < elf->symtab_stop; sym++) {
		if (get_secindex(elf, sym) != relsym_secindex)
			continue;
		if (ELF_ST_TYPE(sym->st_info) == STT_SECTION)
			continue;
		if (sym->st_value == addr)
			return sym;
		/* Find a symbol nearby - addr are maybe negative */
		d = sym->st_value - addr;
		if (d < 0)
			d = addr - sym->st_value;
		if (d < distance) {
			distance = d;
			near = sym;
		}
	}
	/* We need a close match */
	if (distance < 20)
		return near;
	else
		return NULL;
}

static inline int is_arm_mapping_symbol(const char *str)
{
	return str[0] == '$' && strchr("atd", str[1])
	       && (str[2] == '\0' || str[2] == '.');
}

/*
 * If there's no name there, ignore it; likewise, ignore it if it's
 * one of the magic symbols emitted used by current ARM tools.
 *
 * Otherwise if find_symbols_between() returns those symbols, they'll
 * fail the whitelist tests and cause lots of false alarms ... fixable
 * only by merging __exit and __init sections into __text, bloating
 * the kernel (which is especially evil on embedded platforms).
 */
static inline int is_valid_name(struct elf_info *elf, Elf_Sym *sym)
{
	const char *name = elf->strtab + sym->st_name;

	if (!name || !strlen(name))
		return 0;
	return !is_arm_mapping_symbol(name);
}

/*
 * Find symbols before or equal addr and after addr - in the section sec.
 * If we find two symbols with equal offset prefer one with a valid name.
 * The ELF format may have a better way to detect what type of symbol
 * it is, but this works for now.
 **/
static Elf_Sym *find_elf_symbol2(struct elf_info *elf, Elf_Addr addr,
				 const char *sec)
{
	Elf_Sym *sym;
	Elf_Sym *near = NULL;
	Elf_Addr distance = ~0;

	for (sym = elf->symtab_start; sym < elf->symtab_stop; sym++) {
		const char *symsec;

		if (is_shndx_special(sym->st_shndx))
			continue;
		symsec = sec_name(elf, get_secindex(elf, sym));
		if (strcmp(symsec, sec) != 0)
			continue;
		if (!is_valid_name(elf, sym))
			continue;
		if (sym->st_value <= addr) {
			if ((addr - sym->st_value) < distance) {
				distance = addr - sym->st_value;
				near = sym;
			} else if ((addr - sym->st_value) == distance) {
				near = sym;
			}
		}
	}
	return near;
}

/*
 * Convert a section name to the function/data attribute
 * .init.text => __init
 * .cpuinit.data => __cpudata
 * .memexitconst => __memconst
 * etc.
 *
 * The memory of returned value has been allocated on a heap. The user of this
 * method should free it after usage.
*/
static char *sec2annotation(const char *s)
{
	if (match(s, init_exit_sections)) {
		char *p = malloc(20);
		char *r = p;

		*p++ = '_';
		*p++ = '_';
		if (*s == '.')
			s++;
		while (*s && *s != '.')
			*p++ = *s++;
		*p = '\0';
		if (*s == '.')
			s++;
		if (strstr(s, "rodata") != NULL)
			strcat(p, "const ");
		else if (strstr(s, "data") != NULL)
			strcat(p, "data ");
		else
			strcat(p, " ");
		return r;
	} else {
		return strdup("");
	}
}

static int is_function(Elf_Sym *sym)
{
	if (sym)
		return ELF_ST_TYPE(sym->st_info) == STT_FUNC;
	else
		return -1;
}

static void print_section_list(const char * const list[20])
{
	const char *const *s = list;

	while (*s) {
		fprintf(stderr, "%s", *s);
		s++;
		if (*s)
			fprintf(stderr, ", ");
	}
	fprintf(stderr, "\n");
}

/*
 * Print a warning about a section mismatch.
 * Try to find symbols near it so user can find it.
 * Check whitelist before warning - it may be a false positive.
 */
static void report_sec_mismatch(const char *modname,
				const struct sectioncheck *mismatch,
                                const char *fromsec,
                                unsigned long long fromaddr,
                                const char *fromsym,
                                int from_is_func,
                                const char *tosec, const char *tosym,
                                int to_is_func)
{
	const char *from, *from_p;
	const char *to, *to_p;
	char *prl_from;
	char *prl_to;

	switch (from_is_func) {
	case 0: from = "variable"; from_p = "";   break;
	case 1: from = "function"; from_p = "()"; break;
	default: from = "(unknown reference)"; from_p = ""; break;
	}
	switch (to_is_func) {
	case 0: to = "variable"; to_p = "";   break;
	case 1: to = "function"; to_p = "()"; break;
	default: to = "(unknown reference)"; to_p = ""; break;
	}

	sec_mismatch_count++;
	if (!sec_mismatch_verbose)
		return;

	warn("%s(%s+0x%llx): Section mismatch in reference from the %s %s%s "
	     "to the %s %s:%s%s\n",
	     modname, fromsec, fromaddr, from, fromsym, from_p, to, tosec,
	     tosym, to_p);

	switch (mismatch->mismatch) {
	case TEXT_TO_ANY_INIT:
		prl_from = sec2annotation(fromsec);
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The function %s%s() references\n"
		"the %s %s%s%s.\n"
		"This is often because %s lacks a %s\n"
		"annotation or the annotation of %s is wrong.\n",
		prl_from, fromsym,
		to, prl_to, tosym, to_p,
		fromsym, prl_to, tosym);
		free(prl_from);
		free(prl_to);
		break;
	case DATA_TO_ANY_INIT: {
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The variable %s references\n"
		"the %s %s%s%s\n"
		"If the reference is valid then annotate the\n"
		"variable with __init* or __refdata (see linux/init.h) "
		"or name the variable:\n",
		fromsym, to, prl_to, tosym, to_p);
		print_section_list(mismatch->symbol_white_list);
		free(prl_to);
		break;
	}
	case TEXT_TO_ANY_EXIT:
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The function %s() references a %s in an exit section.\n"
		"Often the %s %s%s has valid usage outside the exit section\n"
		"and the fix is to remove the %sannotation of %s.\n",
		fromsym, to, to, tosym, to_p, prl_to, tosym);
		free(prl_to);
		break;
	case DATA_TO_ANY_EXIT: {
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The variable %s references\n"
		"the %s %s%s%s\n"
		"If the reference is valid then annotate the\n"
		"variable with __exit* (see linux/init.h) or "
		"name the variable:\n",
		fromsym, to, prl_to, tosym, to_p);
		print_section_list(mismatch->symbol_white_list);
		free(prl_to);
		break;
	}
	case XXXINIT_TO_SOME_INIT:
	case XXXEXIT_TO_SOME_EXIT:
		prl_from = sec2annotation(fromsec);
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The %s %s%s%s references\n"
		"a %s %s%s%s.\n"
		"If %s is only used by %s then\n"
		"annotate %s with a matching annotation.\n",
		from, prl_from, fromsym, from_p,
		to, prl_to, tosym, to_p,
		tosym, fromsym, tosym);
		free(prl_from);
		free(prl_to);
		break;
	case ANY_INIT_TO_ANY_EXIT:
		prl_from = sec2annotation(fromsec);
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The %s %s%s%s references\n"
		"a %s %s%s%s.\n"
		"This is often seen when error handling "
		"in the init function\n"
		"uses functionality in the exit path.\n"
		"The fix is often to remove the %sannotation of\n"
		"%s%s so it may be used outside an exit section.\n",
		from, prl_from, fromsym, from_p,
		to, prl_to, tosym, to_p,
		prl_to, tosym, to_p);
		free(prl_from);
		free(prl_to);
		break;
	case ANY_EXIT_TO_ANY_INIT:
		prl_from = sec2annotation(fromsec);
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The %s %s%s%s references\n"
		"a %s %s%s%s.\n"
		"This is often seen when error handling "
		"in the exit function\n"
		"uses functionality in the init path.\n"
		"The fix is often to remove the %sannotation of\n"
		"%s%s so it may be used outside an init section.\n",
		from, prl_from, fromsym, from_p,
		to, prl_to, tosym, to_p,
		prl_to, tosym, to_p);
		free(prl_from);
		free(prl_to);
		break;
	case EXPORT_TO_INIT_EXIT:
		prl_to = sec2annotation(tosec);
		fprintf(stderr,
		"The symbol %s is exported and annotated %s\n"
		"Fix this by removing the %sannotation of %s "
		"or drop the export.\n",
		tosym, prl_to, prl_to, tosym);
		free(prl_to);
		break;
	}
	fprintf(stderr, "\n");
}

static void check_section_mismatch(const char *modname, struct elf_info *elf,
                                   Elf_Rela *r, Elf_Sym *sym, const char *fromsec)
{
	const char *tosec;
	const struct sectioncheck *mismatch;

	tosec = sec_name(elf, get_secindex(elf, sym));
	mismatch = section_mismatch(fromsec, tosec);
	if (mismatch) {
		Elf_Sym *to;
		Elf_Sym *from;
		const char *tosym;
		const char *fromsym;

		from = find_elf_symbol2(elf, r->r_offset, fromsec);
		fromsym = sym_name(elf, from);
		to = find_elf_symbol(elf, r->r_addend, sym);
		tosym = sym_name(elf, to);

		/* check whitelist - we may ignore it */
		if (secref_whitelist(mismatch,
					fromsec, fromsym, tosec, tosym)) {
			report_sec_mismatch(modname, mismatch,
			   fromsec, r->r_offset, fromsym,
			   is_function(from), tosec, tosym,
			   is_function(to));
		}
	}
}

static unsigned int *reloc_location(struct elf_info *elf,
				    Elf_Shdr *sechdr, Elf_Rela *r)
{
	Elf_Shdr *sechdrs = elf->sechdrs;
	int section = sechdr->sh_info;

	return (void *)elf->hdr + sechdrs[section].sh_offset +
		r->r_offset;
}

static int addend_386_rel(struct elf_info *elf, Elf_Shdr *sechdr, Elf_Rela *r)
{
	unsigned int r_typ = ELF_R_TYPE(r->r_info);
	unsigned int *location = reloc_location(elf, sechdr, r);

	switch (r_typ) {
	case R_386_32:
		r->r_addend = TO_NATIVE(*location);
		break;
	case R_386_PC32:
		r->r_addend = TO_NATIVE(*location) + 4;
		/* For CONFIG_RELOCATABLE=y */
		if (elf->hdr->e_type == ET_EXEC)
			r->r_addend += r->r_offset;
		break;
	}
	return 0;
=======
static Elf_Sym *find_fromsym(struct elf_info *elf, Elf_Addr addr,
			     unsigned int secndx)
{
	return symsearch_find_nearest(elf, addr, secndx, false, ~0);
}

static Elf_Sym *find_tosym(struct elf_info *elf, Elf_Addr addr, Elf_Sym *sym)
{
	Elf_Sym *new_sym;

	/* If the supplied symbol has a valid name, return it */
	if (is_valid_name(elf, sym))
		return sym;

	/*
	 * Strive to find a better symbol name, but the resulting name may not
	 * match the symbol referenced in the original code.
	 */
	new_sym = symsearch_find_nearest(elf, addr, get_secindex(elf, sym),
					 true, 20);
	return new_sym ? new_sym : sym;
}

static bool is_executable_section(struct elf_info *elf, unsigned int secndx)
{
	if (secndx >= elf->num_sections)
		return false;

	return (elf->sechdrs[secndx].sh_flags & SHF_EXECINSTR) != 0;
}

static void default_mismatch_handler(const char *modname, struct elf_info *elf,
				     const struct sectioncheck* const mismatch,
				     Elf_Sym *tsym,
				     unsigned int fsecndx, const char *fromsec, Elf_Addr faddr,
				     const char *tosec, Elf_Addr taddr)
{
	Elf_Sym *from;
	const char *tosym;
	const char *fromsym;

	from = find_fromsym(elf, faddr, fsecndx);
	fromsym = sym_name(elf, from);

	tsym = find_tosym(elf, taddr, tsym);
	tosym = sym_name(elf, tsym);

	/* check whitelist - we may ignore it */
	if (!secref_whitelist(fromsec, fromsym, tosec, tosym))
		return;

	sec_mismatch_count++;

	warn("%s: section mismatch in reference: %s+0x%x (section: %s) -> %s (section: %s)\n",
	     modname, fromsym,
	     (unsigned int)(faddr - (from ? from->st_value : 0)),
	     fromsec, tosym, tosec);

	if (mismatch->mismatch == EXTABLE_TO_NON_TEXT) {
		if (match(tosec, mismatch->bad_tosec))
			fatal("The relocation at %s+0x%lx references\n"
			      "section \"%s\" which is black-listed.\n"
			      "Something is seriously wrong and should be fixed.\n"
			      "You might get more information about where this is\n"
			      "coming from by using scripts/check_extable.sh %s\n",
			      fromsec, (long)faddr, tosec, modname);
		else if (is_executable_section(elf, get_secindex(elf, tsym)))
			warn("The relocation at %s+0x%lx references\n"
			     "section \"%s\" which is not in the list of\n"
			     "authorized sections.  If you're adding a new section\n"
			     "and/or if this reference is valid, add \"%s\" to the\n"
			     "list of authorized sections to jump to on fault.\n"
			     "This can be achieved by adding \"%s\" to\n"
			     "OTHER_TEXT_SECTIONS in scripts/mod/modpost.c.\n",
			     fromsec, (long)faddr, tosec, tosec, tosec);
		else
			error("%s+0x%lx references non-executable section '%s'\n",
			      fromsec, (long)faddr, tosec);
	}
}

static void check_export_symbol(struct module *mod, struct elf_info *elf,
				Elf_Addr faddr, const char *secname,
				Elf_Sym *sym)
{
	static const char *prefix = "__export_symbol_";
	const char *label_name, *name, *data;
	Elf_Sym *label;
	struct symbol *s;
	bool is_gpl;

	label = find_fromsym(elf, faddr, elf->export_symbol_secndx);
	label_name = sym_name(elf, label);

	if (!strstarts(label_name, prefix)) {
		error("%s: .export_symbol section contains strange symbol '%s'\n",
		      mod->name, label_name);
		return;
	}

	if (ELF_ST_BIND(sym->st_info) != STB_GLOBAL &&
	    ELF_ST_BIND(sym->st_info) != STB_WEAK) {
		error("%s: local symbol '%s' was exported\n", mod->name,
		      label_name + strlen(prefix));
		return;
	}

	name = sym_name(elf, sym);
	if (strcmp(label_name + strlen(prefix), name)) {
		error("%s: .export_symbol section references '%s', but it does not seem to be an export symbol\n",
		      mod->name, name);
		return;
	}

	data = sym_get_data(elf, label);	/* license */
	if (!strcmp(data, "GPL")) {
		is_gpl = true;
	} else if (!strcmp(data, "")) {
		is_gpl = false;
	} else {
		error("%s: unknown license '%s' was specified for '%s'\n",
		      mod->name, data, name);
		return;
	}

	data += strlen(data) + 1;	/* namespace */
	s = sym_add_exported(name, mod, is_gpl, data);

	/*
	 * We need to be aware whether we are exporting a function or
	 * a data on some architectures.
	 */
	s->is_func = (ELF_ST_TYPE(sym->st_info) == STT_FUNC);

	/*
	 * For parisc64, symbols prefixed $$ from the library have the symbol type
	 * STT_LOPROC. They should be handled as functions too.
	 */
	if (elf->hdr->e_ident[EI_CLASS] == ELFCLASS64 &&
	    elf->hdr->e_machine == EM_PARISC &&
	    ELF_ST_TYPE(sym->st_info) == STT_LOPROC)
		s->is_func = true;

	if (match(secname, PATTERNS(ALL_INIT_SECTIONS)))
		warn("%s: %s: EXPORT_SYMBOL used for init symbol. Remove __init or EXPORT_SYMBOL.\n",
		     mod->name, name);
	else if (match(secname, PATTERNS(ALL_EXIT_SECTIONS)))
		warn("%s: %s: EXPORT_SYMBOL used for exit symbol. Remove __exit or EXPORT_SYMBOL.\n",
		     mod->name, name);
}

static void check_section_mismatch(struct module *mod, struct elf_info *elf,
				   Elf_Sym *sym,
				   unsigned int fsecndx, const char *fromsec,
				   Elf_Addr faddr, Elf_Addr taddr)
{
	const char *tosec = sec_name(elf, get_secindex(elf, sym));
	const struct sectioncheck *mismatch;

	if (module_enabled && elf->export_symbol_secndx == fsecndx) {
		check_export_symbol(mod, elf, faddr, tosec, sym);
		return;
	}

	mismatch = section_mismatch(fromsec, tosec);
	if (!mismatch)
		return;

	default_mismatch_handler(mod->name, elf, mismatch, sym,
				 fsecndx, fromsec, faddr,
				 tosec, taddr);
}

static Elf_Addr addend_386_rel(uint32_t *location, unsigned int r_type)
{
	switch (r_type) {
	case R_386_32:
		return TO_NATIVE(*location);
	case R_386_PC32:
		return TO_NATIVE(*location) + 4;
	}

	return (Elf_Addr)(-1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifndef R_ARM_CALL
#define R_ARM_CALL	28
#endif
#ifndef R_ARM_JUMP24
#define R_ARM_JUMP24	29
#endif

<<<<<<< HEAD
static int addend_arm_rel(struct elf_info *elf, Elf_Shdr *sechdr, Elf_Rela *r)
{
	unsigned int r_typ = ELF_R_TYPE(r->r_info);

	switch (r_typ) {
	case R_ARM_ABS32:
		/* From ARM ABI: (S + A) | T */
		r->r_addend = (int)(long)
		              (elf->symtab_start + ELF_R_SYM(r->r_info));
		break;
	case R_ARM_PC24:
	case R_ARM_CALL:
	case R_ARM_JUMP24:
		/* From ARM ABI: ((S + A) | T) - P */
		r->r_addend = (int)(long)(elf->hdr +
		              sechdr->sh_offset +
		              (r->r_offset - sechdr->sh_addr));
		break;
	default:
		return 1;
	}
	return 0;
}

static int addend_mips_rel(struct elf_info *elf, Elf_Shdr *sechdr, Elf_Rela *r)
{
	unsigned int r_typ = ELF_R_TYPE(r->r_info);
	unsigned int *location = reloc_location(elf, sechdr, r);
	unsigned int inst;

	if (r_typ == R_MIPS_HI16)
		return 1;	/* skip this */
	inst = TO_NATIVE(*location);
	switch (r_typ) {
	case R_MIPS_LO16:
		r->r_addend = inst & 0xffff;
		break;
	case R_MIPS_26:
		r->r_addend = (inst & 0x03ffffff) << 2;
		break;
	case R_MIPS_32:
		r->r_addend = inst;
		break;
	}
	return 0;
}

static void section_rela(const char *modname, struct elf_info *elf,
                         Elf_Shdr *sechdr)
{
	Elf_Sym  *sym;
	Elf_Rela *rela;
	Elf_Rela r;
	unsigned int r_sym;
	const char *fromsec;

	Elf_Rela *start = (void *)elf->hdr + sechdr->sh_offset;
	Elf_Rela *stop  = (void *)start + sechdr->sh_size;

	fromsec = sech_name(elf, sechdr);
	fromsec += strlen(".rela");
	/* if from section (name) is know good then skip it */
	if (match(fromsec, section_white_list))
		return;

	for (rela = start; rela < stop; rela++) {
		r.r_offset = TO_NATIVE(rela->r_offset);
#if KERNEL_ELFCLASS == ELFCLASS64
		if (elf->hdr->e_machine == EM_MIPS) {
			unsigned int r_typ;
			r_sym = ELF64_MIPS_R_SYM(rela->r_info);
			r_sym = TO_NATIVE(r_sym);
			r_typ = ELF64_MIPS_R_TYPE(rela->r_info);
			r.r_info = ELF64_R_INFO(r_sym, r_typ);
		} else {
			r.r_info = TO_NATIVE(rela->r_info);
			r_sym = ELF_R_SYM(r.r_info);
		}
#else
		r.r_info = TO_NATIVE(rela->r_info);
		r_sym = ELF_R_SYM(r.r_info);
#endif
		r.r_addend = TO_NATIVE(rela->r_addend);
		sym = elf->symtab_start + r_sym;
		/* Skip special sections */
		if (is_shndx_special(sym->st_shndx))
			continue;
		check_section_mismatch(modname, elf, &r, sym, fromsec);
	}
}

static void section_rel(const char *modname, struct elf_info *elf,
                        Elf_Shdr *sechdr)
{
	Elf_Sym *sym;
	Elf_Rel *rel;
	Elf_Rela r;
	unsigned int r_sym;
	const char *fromsec;

	Elf_Rel *start = (void *)elf->hdr + sechdr->sh_offset;
	Elf_Rel *stop  = (void *)start + sechdr->sh_size;

	fromsec = sech_name(elf, sechdr);
	fromsec += strlen(".rel");
	/* if from section (name) is know good then skip it */
	if (match(fromsec, section_white_list))
		return;

	for (rel = start; rel < stop; rel++) {
		r.r_offset = TO_NATIVE(rel->r_offset);
#if KERNEL_ELFCLASS == ELFCLASS64
		if (elf->hdr->e_machine == EM_MIPS) {
			unsigned int r_typ;
			r_sym = ELF64_MIPS_R_SYM(rel->r_info);
			r_sym = TO_NATIVE(r_sym);
			r_typ = ELF64_MIPS_R_TYPE(rel->r_info);
			r.r_info = ELF64_R_INFO(r_sym, r_typ);
		} else {
			r.r_info = TO_NATIVE(rel->r_info);
			r_sym = ELF_R_SYM(r.r_info);
		}
#else
		r.r_info = TO_NATIVE(rel->r_info);
		r_sym = ELF_R_SYM(r.r_info);
#endif
		r.r_addend = 0;
		switch (elf->hdr->e_machine) {
		case EM_386:
			if (addend_386_rel(elf, sechdr, &r))
				continue;
			break;
		case EM_ARM:
			if (addend_arm_rel(elf, sechdr, &r))
				continue;
			break;
		case EM_MIPS:
			if (addend_mips_rel(elf, sechdr, &r))
				continue;
			break;
		}
		sym = elf->symtab_start + r_sym;
		/* Skip special sections */
		if (is_shndx_special(sym->st_shndx))
			continue;
		check_section_mismatch(modname, elf, &r, sym, fromsec);
=======
#ifndef	R_ARM_THM_CALL
#define	R_ARM_THM_CALL		10
#endif
#ifndef	R_ARM_THM_JUMP24
#define	R_ARM_THM_JUMP24	30
#endif

#ifndef R_ARM_MOVW_ABS_NC
#define R_ARM_MOVW_ABS_NC	43
#endif

#ifndef R_ARM_MOVT_ABS
#define R_ARM_MOVT_ABS		44
#endif

#ifndef R_ARM_THM_MOVW_ABS_NC
#define R_ARM_THM_MOVW_ABS_NC	47
#endif

#ifndef R_ARM_THM_MOVT_ABS
#define R_ARM_THM_MOVT_ABS	48
#endif

#ifndef	R_ARM_THM_JUMP19
#define	R_ARM_THM_JUMP19	51
#endif

static int32_t sign_extend32(int32_t value, int index)
{
	uint8_t shift = 31 - index;

	return (int32_t)(value << shift) >> shift;
}

static Elf_Addr addend_arm_rel(void *loc, Elf_Sym *sym, unsigned int r_type)
{
	uint32_t inst, upper, lower, sign, j1, j2;
	int32_t offset;

	switch (r_type) {
	case R_ARM_ABS32:
	case R_ARM_REL32:
		inst = TO_NATIVE(*(uint32_t *)loc);
		return inst + sym->st_value;
	case R_ARM_MOVW_ABS_NC:
	case R_ARM_MOVT_ABS:
		inst = TO_NATIVE(*(uint32_t *)loc);
		offset = sign_extend32(((inst & 0xf0000) >> 4) | (inst & 0xfff),
				       15);
		return offset + sym->st_value;
	case R_ARM_PC24:
	case R_ARM_CALL:
	case R_ARM_JUMP24:
		inst = TO_NATIVE(*(uint32_t *)loc);
		offset = sign_extend32((inst & 0x00ffffff) << 2, 25);
		return offset + sym->st_value + 8;
	case R_ARM_THM_MOVW_ABS_NC:
	case R_ARM_THM_MOVT_ABS:
		upper = TO_NATIVE(*(uint16_t *)loc);
		lower = TO_NATIVE(*((uint16_t *)loc + 1));
		offset = sign_extend32(((upper & 0x000f) << 12) |
				       ((upper & 0x0400) << 1) |
				       ((lower & 0x7000) >> 4) |
				       (lower & 0x00ff),
				       15);
		return offset + sym->st_value;
	case R_ARM_THM_JUMP19:
		/*
		 * Encoding T3:
		 * S     = upper[10]
		 * imm6  = upper[5:0]
		 * J1    = lower[13]
		 * J2    = lower[11]
		 * imm11 = lower[10:0]
		 * imm32 = SignExtend(S:J2:J1:imm6:imm11:'0')
		 */
		upper = TO_NATIVE(*(uint16_t *)loc);
		lower = TO_NATIVE(*((uint16_t *)loc + 1));

		sign = (upper >> 10) & 1;
		j1 = (lower >> 13) & 1;
		j2 = (lower >> 11) & 1;
		offset = sign_extend32((sign << 20) | (j2 << 19) | (j1 << 18) |
				       ((upper & 0x03f) << 12) |
				       ((lower & 0x07ff) << 1),
				       20);
		return offset + sym->st_value + 4;
	case R_ARM_THM_CALL:
	case R_ARM_THM_JUMP24:
		/*
		 * Encoding T4:
		 * S     = upper[10]
		 * imm10 = upper[9:0]
		 * J1    = lower[13]
		 * J2    = lower[11]
		 * imm11 = lower[10:0]
		 * I1    = NOT(J1 XOR S)
		 * I2    = NOT(J2 XOR S)
		 * imm32 = SignExtend(S:I1:I2:imm10:imm11:'0')
		 */
		upper = TO_NATIVE(*(uint16_t *)loc);
		lower = TO_NATIVE(*((uint16_t *)loc + 1));

		sign = (upper >> 10) & 1;
		j1 = (lower >> 13) & 1;
		j2 = (lower >> 11) & 1;
		offset = sign_extend32((sign << 24) |
				       ((~(j1 ^ sign) & 1) << 23) |
				       ((~(j2 ^ sign) & 1) << 22) |
				       ((upper & 0x03ff) << 12) |
				       ((lower & 0x07ff) << 1),
				       24);
		return offset + sym->st_value + 4;
	}

	return (Elf_Addr)(-1);
}

static Elf_Addr addend_mips_rel(uint32_t *location, unsigned int r_type)
{
	uint32_t inst;

	inst = TO_NATIVE(*location);
	switch (r_type) {
	case R_MIPS_LO16:
		return inst & 0xffff;
	case R_MIPS_26:
		return (inst & 0x03ffffff) << 2;
	case R_MIPS_32:
		return inst;
	}
	return (Elf_Addr)(-1);
}

#ifndef EM_RISCV
#define EM_RISCV		243
#endif

#ifndef R_RISCV_SUB32
#define R_RISCV_SUB32		39
#endif

#ifndef EM_LOONGARCH
#define EM_LOONGARCH		258
#endif

#ifndef R_LARCH_SUB32
#define R_LARCH_SUB32		55
#endif

#ifndef R_LARCH_RELAX
#define R_LARCH_RELAX		100
#endif

#ifndef R_LARCH_ALIGN
#define R_LARCH_ALIGN		102
#endif

static void get_rel_type_and_sym(struct elf_info *elf, uint64_t r_info,
				 unsigned int *r_type, unsigned int *r_sym)
{
	typedef struct {
		Elf64_Word    r_sym;	/* Symbol index */
		unsigned char r_ssym;	/* Special symbol for 2nd relocation */
		unsigned char r_type3;	/* 3rd relocation type */
		unsigned char r_type2;	/* 2nd relocation type */
		unsigned char r_type;	/* 1st relocation type */
	} Elf64_Mips_R_Info;

	bool is_64bit = (elf->hdr->e_ident[EI_CLASS] == ELFCLASS64);

	if (elf->hdr->e_machine == EM_MIPS && is_64bit) {
		Elf64_Mips_R_Info *mips64_r_info = (void *)&r_info;

		*r_type = mips64_r_info->r_type;
		*r_sym = TO_NATIVE(mips64_r_info->r_sym);
		return;
	}

	if (is_64bit)
		r_info = TO_NATIVE((Elf64_Xword)r_info);
	else
		r_info = TO_NATIVE((Elf32_Word)r_info);

	*r_type = ELF_R_TYPE(r_info);
	*r_sym = ELF_R_SYM(r_info);
}

static void section_rela(struct module *mod, struct elf_info *elf,
			 unsigned int fsecndx, const char *fromsec,
			 const Elf_Rela *start, const Elf_Rela *stop)
{
	const Elf_Rela *rela;

	for (rela = start; rela < stop; rela++) {
		Elf_Sym *tsym;
		Elf_Addr taddr, r_offset;
		unsigned int r_type, r_sym;

		r_offset = TO_NATIVE(rela->r_offset);
		get_rel_type_and_sym(elf, rela->r_info, &r_type, &r_sym);

		tsym = elf->symtab_start + r_sym;
		taddr = tsym->st_value + TO_NATIVE(rela->r_addend);

		switch (elf->hdr->e_machine) {
		case EM_RISCV:
			if (!strcmp("__ex_table", fromsec) &&
			    r_type == R_RISCV_SUB32)
				continue;
			break;
		case EM_LOONGARCH:
			switch (r_type) {
			case R_LARCH_SUB32:
				if (!strcmp("__ex_table", fromsec))
					continue;
				break;
			case R_LARCH_RELAX:
			case R_LARCH_ALIGN:
				/* These relocs do not refer to symbols */
				continue;
			}
			break;
		}

		check_section_mismatch(mod, elf, tsym,
				       fsecndx, fromsec, r_offset, taddr);
	}
}

static void section_rel(struct module *mod, struct elf_info *elf,
			unsigned int fsecndx, const char *fromsec,
			const Elf_Rel *start, const Elf_Rel *stop)
{
	const Elf_Rel *rel;

	for (rel = start; rel < stop; rel++) {
		Elf_Sym *tsym;
		Elf_Addr taddr, r_offset;
		unsigned int r_type, r_sym;
		void *loc;

		r_offset = TO_NATIVE(rel->r_offset);
		get_rel_type_and_sym(elf, rel->r_info, &r_type, &r_sym);

		loc = sym_get_data_by_offset(elf, fsecndx, r_offset);
		tsym = elf->symtab_start + r_sym;

		switch (elf->hdr->e_machine) {
		case EM_386:
			taddr = addend_386_rel(loc, r_type);
			break;
		case EM_ARM:
			taddr = addend_arm_rel(loc, tsym, r_type);
			break;
		case EM_MIPS:
			taddr = addend_mips_rel(loc, r_type);
			break;
		default:
			fatal("Please add code to calculate addend for this architecture\n");
		}

		check_section_mismatch(mod, elf, tsym,
				       fsecndx, fromsec, r_offset, taddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
 * A module includes a number of sections that are discarded
 * either when loaded or when used as built-in.
 * For loaded modules all functions marked __init and all data
 * marked __initdata will be discarded when the module has been initialized.
 * Likewise for modules used built-in the sections marked __exit
 * are discarded because __exit marked function are supposed to be called
 * only when a module is unloaded which never happens for built-in modules.
 * The check_sec_ref() function traverses all relocation records
 * to find all references to a section that reference a section that will
 * be discarded and warns about it.
 **/
<<<<<<< HEAD
static void check_sec_ref(struct module *mod, const char *modname,
                          struct elf_info *elf)
{
	int i;
	Elf_Shdr *sechdrs = elf->sechdrs;

	/* Walk through all sections */
	for (i = 0; i < elf->num_sections; i++) {
		check_section(modname, elf, &elf->sechdrs[i]);
		/* We want to process only relocation sections and not .init */
		if (sechdrs[i].sh_type == SHT_RELA)
			section_rela(modname, elf, &elf->sechdrs[i]);
		else if (sechdrs[i].sh_type == SHT_REL)
			section_rel(modname, elf, &elf->sechdrs[i]);
	}
}

static void read_symbols(char *modname)
=======
static void check_sec_ref(struct module *mod, struct elf_info *elf)
{
	int i;

	/* Walk through all sections */
	for (i = 0; i < elf->num_sections; i++) {
		Elf_Shdr *sechdr = &elf->sechdrs[i];

		check_section(mod->name, elf, sechdr);
		/* We want to process only relocation sections and not .init */
		if (sechdr->sh_type == SHT_REL || sechdr->sh_type == SHT_RELA) {
			/* section to which the relocation applies */
			unsigned int secndx = sechdr->sh_info;
			const char *secname = sec_name(elf, secndx);
			const void *start, *stop;

			/* If the section is known good, skip it */
			if (match(secname, section_white_list))
				continue;

			start = sym_get_data_by_offset(elf, i, 0);
			stop = start + sechdr->sh_size;

			if (sechdr->sh_type == SHT_RELA)
				section_rela(mod, elf, secndx, secname,
					     start, stop);
			else
				section_rel(mod, elf, secndx, secname,
					    start, stop);
		}
	}
}

static char *remove_dot(char *s)
{
	size_t n = strcspn(s, ".");

	if (n && s[n]) {
		size_t m = strspn(s + n + 1, "0123456789");
		if (m && (s[n + m + 1] == '.' || s[n + m + 1] == 0))
			s[n] = 0;
	}
	return s;
}

/*
 * The CRCs are recorded in .*.cmd files in the form of:
 * #SYMVER <name> <crc>
 */
static void extract_crcs_for_object(const char *object, struct module *mod)
{
	char cmd_file[PATH_MAX];
	char *buf, *p;
	const char *base;
	int dirlen, ret;

	base = strrchr(object, '/');
	if (base) {
		base++;
		dirlen = base - object;
	} else {
		dirlen = 0;
		base = object;
	}

	ret = snprintf(cmd_file, sizeof(cmd_file), "%.*s.%s.cmd",
		       dirlen, object, base);
	if (ret >= sizeof(cmd_file)) {
		error("%s: too long path was truncated\n", cmd_file);
		return;
	}

	buf = read_text_file(cmd_file);
	p = buf;

	while ((p = strstr(p, "\n#SYMVER "))) {
		char *name;
		size_t namelen;
		unsigned int crc;
		struct symbol *sym;

		name = p + strlen("\n#SYMVER ");

		p = strchr(name, ' ');
		if (!p)
			break;

		namelen = p - name;
		p++;

		if (!isdigit(*p))
			continue;	/* skip this line */

		crc = strtoul(p, &p, 0);
		if (*p != '\n')
			continue;	/* skip this line */

		name[namelen] = '\0';

		/*
		 * sym_find_with_module() may return NULL here.
		 * It typically occurs when CONFIG_TRIM_UNUSED_KSYMS=y.
		 * Since commit e1327a127703, genksyms calculates CRCs of all
		 * symbols, including trimmed ones. Ignore orphan CRCs.
		 */
		sym = sym_find_with_module(name, mod);
		if (sym)
			sym_set_crc(sym, crc);
	}

	free(buf);
}

/*
 * The symbol versions (CRC) are recorded in the .*.cmd files.
 * Parse them to retrieve CRCs for the current module.
 */
static void mod_set_crcs(struct module *mod)
{
	char objlist[PATH_MAX];
	char *buf, *p, *obj;
	int ret;

	if (mod->is_vmlinux) {
		strcpy(objlist, ".vmlinux.objs");
	} else {
		/* objects for a module are listed in the *.mod file. */
		ret = snprintf(objlist, sizeof(objlist), "%s.mod", mod->name);
		if (ret >= sizeof(objlist)) {
			error("%s: too long path was truncated\n", objlist);
			return;
		}
	}

	buf = read_text_file(objlist);
	p = buf;

	while ((obj = strsep(&p, "\n")) && obj[0])
		extract_crcs_for_object(obj, mod);

	free(buf);
}

static void read_symbols(const char *modname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *symname;
	char *version;
	char *license;
<<<<<<< HEAD
=======
	char *namespace;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct module *mod;
	struct elf_info info = { };
	Elf_Sym *sym;

	if (!parse_elf(&info, modname))
		return;

<<<<<<< HEAD
	mod = new_module(modname);

	/* When there's no vmlinux, don't print warnings about
	 * unresolved symbols (since there'll be too many ;) */
	if (is_vmlinux(modname)) {
		have_vmlinux = 1;
		mod->skip = 1;
	}

	license = get_modinfo(info.modinfo, info.modinfo_len, "license");
	if (info.modinfo && !license && !is_vmlinux(modname))
		warn("modpost: missing MODULE_LICENSE() in %s\n"
		     "see include/linux/module.h for "
		     "more information\n", modname);
	while (license) {
		if (license_is_gpl_compatible(license))
			mod->gpl_compatible = 1;
		else {
			mod->gpl_compatible = 0;
			break;
		}
		license = get_next_modinfo(info.modinfo, info.modinfo_len,
					   "license", license);
	}

	for (sym = info.symtab_start; sym < info.symtab_stop; sym++) {
		symname = info.strtab + sym->st_name;

		handle_modversions(mod, &info, sym, symname);
		handle_moddevtable(mod, &info, sym, symname);
	}
	if (!is_vmlinux(modname) ||
	     (is_vmlinux(modname) && vmlinux_section_warnings))
		check_sec_ref(mod, modname, &info);

	version = get_modinfo(info.modinfo, info.modinfo_len, "version");
	if (version)
		maybe_frob_rcs_version(modname, version, info.modinfo,
				       version - (char *)info.hdr);
	if (version || (all_versions && !is_vmlinux(modname)))
		get_src_version(modname, mod->srcversion,
				sizeof(mod->srcversion)-1);

	parse_elf_finish(&info);

	/* Our trick to get versioning for module struct etc. - it's
	 * never passed as an argument to an exported function, so
	 * the automatic versioning doesn't pick it up, but it's really
	 * important anyhow */
	if (modversions)
		mod->unres = alloc_symbol("module_layout", 0, mod->unres);
=======
	if (!strends(modname, ".o")) {
		error("%s: filename must be suffixed with .o\n", modname);
		return;
	}

	/* strip trailing .o */
	mod = new_module(modname, strlen(modname) - strlen(".o"));

	if (!mod->is_vmlinux) {
		license = get_modinfo(&info, "license");
		if (!license)
			error("missing MODULE_LICENSE() in %s\n", modname);
		while (license) {
			if (!license_is_gpl_compatible(license)) {
				mod->is_gpl_compatible = false;
				break;
			}
			license = get_next_modinfo(&info, "license", license);
		}

		namespace = get_modinfo(&info, "import_ns");
		while (namespace) {
			add_namespace(&mod->imported_namespaces, namespace);
			namespace = get_next_modinfo(&info, "import_ns",
						     namespace);
		}
	}

	if (extra_warn && !get_modinfo(&info, "description"))
		warn("missing MODULE_DESCRIPTION() in %s\n", modname);
	for (sym = info.symtab_start; sym < info.symtab_stop; sym++) {
		symname = remove_dot(info.strtab + sym->st_name);

		handle_symbol(mod, &info, sym, symname);
		handle_moddevtable(mod, &info, sym, symname);
	}

	check_sec_ref(mod, &info);

	if (!mod->is_vmlinux) {
		version = get_modinfo(&info, "version");
		if (version || all_versions)
			get_src_version(mod->name, mod->srcversion,
					sizeof(mod->srcversion) - 1);
	}

	parse_elf_finish(&info);

	if (modversions) {
		/*
		 * Our trick to get versioning for module struct etc. - it's
		 * never passed as an argument to an exported function, so
		 * the automatic versioning doesn't pick it up, but it's really
		 * important anyhow.
		 */
		sym_add_unresolved("module_layout", mod, false);

		mod_set_crcs(mod);
	}
}

static void read_symbols_from_files(const char *filename)
{
	FILE *in = stdin;
	char fname[PATH_MAX];

	in = fopen(filename, "r");
	if (!in)
		fatal("Can't open filenames file %s: %m", filename);

	while (fgets(fname, PATH_MAX, in) != NULL) {
		if (strends(fname, "\n"))
			fname[strlen(fname)-1] = '\0';
		read_symbols(fname);
	}

	fclose(in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define SZ 500

/* We first write the generated file into memory using the
 * following helper, then compare to the file on disk and
 * only update the later if anything changed */

void __attribute__((format(printf, 2, 3))) buf_printf(struct buffer *buf,
						      const char *fmt, ...)
{
	char tmp[SZ];
	int len;
	va_list ap;

	va_start(ap, fmt);
	len = vsnprintf(tmp, SZ, fmt, ap);
	buf_write(buf, tmp, len);
	va_end(ap);
}

void buf_write(struct buffer *buf, const char *s, int len)
{
	if (buf->size - buf->pos < len) {
		buf->size += len + SZ;
<<<<<<< HEAD
		buf->p = realloc(buf->p, buf->size);
=======
		buf->p = NOFAIL(realloc(buf->p, buf->size));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	strncpy(buf->p + buf->pos, s, len);
	buf->pos += len;
}

<<<<<<< HEAD
static void check_for_gpl_usage(enum export exp, const char *m, const char *s)
{
	const char *e = is_vmlinux(m) ?"":".ko";

	switch (exp) {
	case export_gpl:
		fatal("modpost: GPL-incompatible module %s%s "
		      "uses GPL-only symbol '%s'\n", m, e, s);
		break;
	case export_unused_gpl:
		fatal("modpost: GPL-incompatible module %s%s "
		      "uses GPL-only symbol marked UNUSED '%s'\n", m, e, s);
		break;
	case export_gpl_future:
		warn("modpost: GPL-incompatible module %s%s "
		      "uses future GPL-only symbol '%s'\n", m, e, s);
		break;
	case export_plain:
	case export_unused:
	case export_unknown:
		/* ignore */
		break;
	}
}

static void check_for_unused(enum export exp, const char *m, const char *s)
{
	const char *e = is_vmlinux(m) ?"":".ko";

	switch (exp) {
	case export_unused:
	case export_unused_gpl:
		warn("modpost: module %s%s "
		      "uses symbol '%s' marked UNUSED\n", m, e, s);
		break;
	default:
		/* ignore */
		break;
	}
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void check_exports(struct module *mod)
{
	struct symbol *s, *exp;

<<<<<<< HEAD
	for (s = mod->unres; s; s = s->next) {
		const char *basename;
		exp = find_symbol(s->name);
		if (!exp || exp->module == mod)
			continue;
=======
	list_for_each_entry(s, &mod->unresolved_symbols, list) {
		const char *basename;
		exp = find_symbol(s->name);
		if (!exp) {
			if (!s->weak && nr_unresolved++ < MAX_UNRESOLVED_REPORTS)
				modpost_log(warn_unresolved ? LOG_WARN : LOG_ERROR,
					    "\"%s\" [%s.ko] undefined!\n",
					    s->name, mod->name);
			continue;
		}
		if (exp->module == mod) {
			error("\"%s\" [%s.ko] was exported without definition\n",
			      s->name, mod->name);
			continue;
		}

		exp->used = true;
		s->module = exp->module;
		s->crc_valid = exp->crc_valid;
		s->crc = exp->crc;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		basename = strrchr(mod->name, '/');
		if (basename)
			basename++;
		else
			basename = mod->name;
<<<<<<< HEAD
		if (!mod->gpl_compatible)
			check_for_gpl_usage(exp->export, basename, exp->name);
		check_for_unused(exp->export, basename, exp->name);
	}
}

=======

		if (!contains_namespace(&mod->imported_namespaces, exp->namespace)) {
			modpost_log(allow_missing_ns_imports ? LOG_WARN : LOG_ERROR,
				    "module %s uses symbol %s from namespace %s, but does not import it.\n",
				    basename, exp->name, exp->namespace);
			add_namespace(&mod->missing_namespaces, exp->namespace);
		}

		if (!mod->is_gpl_compatible && exp->is_gpl_only)
			error("GPL-incompatible module %s.ko uses GPL-only symbol '%s'\n",
			      basename, exp->name);
	}
}

static void handle_white_list_exports(const char *white_list)
{
	char *buf, *p, *name;

	buf = read_text_file(white_list);
	p = buf;

	while ((name = strsep(&p, "\n"))) {
		struct symbol *sym = find_symbol(name);

		if (sym)
			sym->used = true;
	}

	free(buf);
}

static void check_modname_len(struct module *mod)
{
	const char *mod_name;

	mod_name = strrchr(mod->name, '/');
	if (mod_name == NULL)
		mod_name = mod->name;
	else
		mod_name++;
	if (strlen(mod_name) >= MODULE_NAME_LEN)
		error("module name is too long [%s.ko]\n", mod->name);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * Header for the generated file
 **/
static void add_header(struct buffer *b, struct module *mod)
{
	buf_printf(b, "#include <linux/module.h>\n");
<<<<<<< HEAD
	buf_printf(b, "#include <linux/vermagic.h>\n");
	buf_printf(b, "#include <linux/compiler.h>\n");
	buf_printf(b, "\n");
	buf_printf(b, "MODULE_INFO(vermagic, VERMAGIC_STRING);\n");
	buf_printf(b, "\n");
	buf_printf(b, "struct module __this_module\n");
	buf_printf(b, "__attribute__((section(\".gnu.linkonce.this_module\"))) = {\n");
	buf_printf(b, " .name = KBUILD_MODNAME,\n");
	if (mod->has_init)
		buf_printf(b, " .init = init_module,\n");
	if (mod->has_cleanup)
		buf_printf(b, "#ifdef CONFIG_MODULE_UNLOAD\n"
			      " .exit = cleanup_module,\n"
			      "#endif\n");
	buf_printf(b, " .arch = MODULE_ARCH_INIT,\n");
	buf_printf(b, "};\n");
}

static void add_intree_flag(struct buffer *b, int is_intree)
{
	if (is_intree)
		buf_printf(b, "\nMODULE_INFO(intree, \"Y\");\n");
}

static void add_staging_flag(struct buffer *b, const char *name)
{
	static const char *staging_dir = "drivers/staging";

	if (strncmp(staging_dir, name, strlen(staging_dir)) == 0)
		buf_printf(b, "\nMODULE_INFO(staging, \"Y\");\n");
=======
	/*
	 * Include build-salt.h after module.h in order to
	 * inherit the definitions.
	 */
	buf_printf(b, "#define INCLUDE_VERMAGIC\n");
	buf_printf(b, "#include <linux/build-salt.h>\n");
	buf_printf(b, "#include <linux/elfnote-lto.h>\n");
	buf_printf(b, "#include <linux/export-internal.h>\n");
	buf_printf(b, "#include <linux/vermagic.h>\n");
	buf_printf(b, "#include <linux/compiler.h>\n");
	buf_printf(b, "\n");
	buf_printf(b, "#ifdef CONFIG_UNWINDER_ORC\n");
	buf_printf(b, "#include <asm/orc_header.h>\n");
	buf_printf(b, "ORC_HEADER;\n");
	buf_printf(b, "#endif\n");
	buf_printf(b, "\n");
	buf_printf(b, "BUILD_SALT;\n");
	buf_printf(b, "BUILD_LTO_INFO;\n");
	buf_printf(b, "\n");
	buf_printf(b, "MODULE_INFO(vermagic, VERMAGIC_STRING);\n");
	buf_printf(b, "MODULE_INFO(name, KBUILD_MODNAME);\n");
	buf_printf(b, "\n");
	buf_printf(b, "__visible struct module __this_module\n");
	buf_printf(b, "__section(\".gnu.linkonce.this_module\") = {\n");
	buf_printf(b, "\t.name = KBUILD_MODNAME,\n");
	if (mod->has_init)
		buf_printf(b, "\t.init = init_module,\n");
	if (mod->has_cleanup)
		buf_printf(b, "#ifdef CONFIG_MODULE_UNLOAD\n"
			      "\t.exit = cleanup_module,\n"
			      "#endif\n");
	buf_printf(b, "\t.arch = MODULE_ARCH_INIT,\n");
	buf_printf(b, "};\n");

	if (!external_module)
		buf_printf(b, "\nMODULE_INFO(intree, \"Y\");\n");

	buf_printf(b,
		   "\n"
		   "#ifdef CONFIG_MITIGATION_RETPOLINE\n"
		   "MODULE_INFO(retpoline, \"Y\");\n"
		   "#endif\n");

	if (strstarts(mod->name, "drivers/staging"))
		buf_printf(b, "\nMODULE_INFO(staging, \"Y\");\n");

	if (strstarts(mod->name, "tools/testing"))
		buf_printf(b, "\nMODULE_INFO(test, \"Y\");\n");
}

static void add_exported_symbols(struct buffer *buf, struct module *mod)
{
	struct symbol *sym;

	/* generate struct for exported symbols */
	buf_printf(buf, "\n");
	list_for_each_entry(sym, &mod->exported_symbols, list) {
		if (trim_unused_exports && !sym->used)
			continue;

		buf_printf(buf, "KSYMTAB_%s(%s, \"%s\", \"%s\");\n",
			   sym->is_func ? "FUNC" : "DATA", sym->name,
			   sym->is_gpl_only ? "_gpl" : "", sym->namespace);
	}

	if (!modversions)
		return;

	/* record CRCs for exported symbols */
	buf_printf(buf, "\n");
	list_for_each_entry(sym, &mod->exported_symbols, list) {
		if (trim_unused_exports && !sym->used)
			continue;

		if (!sym->crc_valid)
			warn("EXPORT symbol \"%s\" [%s%s] version generation failed, symbol will not be versioned.\n"
			     "Is \"%s\" prototyped in <asm/asm-prototypes.h>?\n",
			     sym->name, mod->name, mod->is_vmlinux ? "" : ".ko",
			     sym->name);

		buf_printf(buf, "SYMBOL_CRC(%s, 0x%08x, \"%s\");\n",
			   sym->name, sym->crc, sym->is_gpl_only ? "_gpl" : "");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * Record CRCs for unresolved symbols
 **/
<<<<<<< HEAD
static int add_versions(struct buffer *b, struct module *mod)
{
	struct symbol *s, *exp;
	int err = 0;

	for (s = mod->unres; s; s = s->next) {
		exp = find_symbol(s->name);
		if (!exp || exp->module == mod) {
			if (have_vmlinux && !s->weak) {
				if (warn_unresolved) {
					warn("\"%s\" [%s.ko] undefined!\n",
					     s->name, mod->name);
				} else {
					merror("\"%s\" [%s.ko] undefined!\n",
					          s->name, mod->name);
					err = 1;
				}
			}
			continue;
		}
		s->module = exp->module;
		s->crc_valid = exp->crc_valid;
		s->crc = exp->crc;
	}

	if (!modversions)
		return err;

	buf_printf(b, "\n");
	buf_printf(b, "static const struct modversion_info ____versions[]\n");
	buf_printf(b, "__used\n");
	buf_printf(b, "__attribute__((section(\"__versions\"))) = {\n");

	for (s = mod->unres; s; s = s->next) {
=======
static void add_versions(struct buffer *b, struct module *mod)
{
	struct symbol *s;

	if (!modversions)
		return;

	buf_printf(b, "\n");
	buf_printf(b, "static const struct modversion_info ____versions[]\n");
	buf_printf(b, "__used __section(\"__versions\") = {\n");

	list_for_each_entry(s, &mod->unresolved_symbols, list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!s->module)
			continue;
		if (!s->crc_valid) {
			warn("\"%s\" [%s.ko] has no CRC!\n",
				s->name, mod->name);
			continue;
		}
<<<<<<< HEAD
		buf_printf(b, "\t{ %#8x, \"%s\" },\n", s->crc, s->name);
	}

	buf_printf(b, "};\n");

	return err;
}

static void add_depends(struct buffer *b, struct module *mod,
			struct module *modules)
{
	struct symbol *s;
	struct module *m;
	int first = 1;

	for (m = modules; m; m = m->next)
		m->seen = is_vmlinux(m->name);

	buf_printf(b, "\n");
	buf_printf(b, "static const char __module_depends[]\n");
	buf_printf(b, "__used\n");
	buf_printf(b, "__attribute__((section(\".modinfo\"))) =\n");
	buf_printf(b, "\"depends=");
	for (s = mod->unres; s; s = s->next) {
=======
		if (strlen(s->name) >= MODULE_NAME_LEN) {
			error("too long symbol \"%s\" [%s.ko]\n",
			      s->name, mod->name);
			break;
		}
		buf_printf(b, "\t{ %#8x, \"%s\" },\n",
			   s->crc, s->name);
	}

	buf_printf(b, "};\n");
}

static void add_depends(struct buffer *b, struct module *mod)
{
	struct symbol *s;
	int first = 1;

	/* Clear ->seen flag of modules that own symbols needed by this. */
	list_for_each_entry(s, &mod->unresolved_symbols, list) {
		if (s->module)
			s->module->seen = s->module->is_vmlinux;
	}

	buf_printf(b, "\n");
	buf_printf(b, "MODULE_INFO(depends, \"");
	list_for_each_entry(s, &mod->unresolved_symbols, list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const char *p;
		if (!s->module)
			continue;

		if (s->module->seen)
			continue;

<<<<<<< HEAD
		s->module->seen = 1;
=======
		s->module->seen = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		p = strrchr(s->module->name, '/');
		if (p)
			p++;
		else
			p = s->module->name;
		buf_printf(b, "%s%s", first ? "" : ",", p);
		first = 0;
	}
<<<<<<< HEAD
	buf_printf(b, "\";\n");
=======
	buf_printf(b, "\");\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void add_srcversion(struct buffer *b, struct module *mod)
{
	if (mod->srcversion[0]) {
		buf_printf(b, "\n");
		buf_printf(b, "MODULE_INFO(srcversion, \"%s\");\n",
			   mod->srcversion);
	}
}

<<<<<<< HEAD
=======
static void write_buf(struct buffer *b, const char *fname)
{
	FILE *file;

	if (error_occurred)
		return;

	file = fopen(fname, "w");
	if (!file) {
		perror(fname);
		exit(1);
	}
	if (fwrite(b->p, 1, b->pos, file) != b->pos) {
		perror(fname);
		exit(1);
	}
	if (fclose(file) != 0) {
		perror(fname);
		exit(1);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void write_if_changed(struct buffer *b, const char *fname)
{
	char *tmp;
	FILE *file;
	struct stat st;

	file = fopen(fname, "r");
	if (!file)
		goto write;

	if (fstat(fileno(file), &st) < 0)
		goto close_write;

	if (st.st_size != b->pos)
		goto close_write;

	tmp = NOFAIL(malloc(b->pos));
	if (fread(tmp, 1, b->pos, file) != b->pos)
		goto free_write;

	if (memcmp(tmp, b->p, b->pos) != 0)
		goto free_write;

	free(tmp);
	fclose(file);
	return;

 free_write:
	free(tmp);
 close_write:
	fclose(file);
 write:
<<<<<<< HEAD
	file = fopen(fname, "w");
	if (!file) {
		perror(fname);
		exit(1);
	}
	if (fwrite(b->p, 1, b->pos, file) != b->pos) {
		perror(fname);
		exit(1);
	}
	fclose(file);
}

/* parse Module.symvers file. line format:
 * 0x12345678<tab>symbol<tab>module[[<tab>export]<tab>something]
 **/
static void read_dump(const char *fname, unsigned int kernel)
{
	unsigned long size, pos = 0;
	void *file = grab_file(fname, &size);
	char *line;

	if (!file)
		/* No symbol versions, silently ignore */
		return;

	while ((line = get_next_line(&pos, file, size))) {
		char *symname, *modname, *d, *export, *end;
		unsigned int crc;
		struct module *mod;
		struct symbol *s;
=======
	write_buf(b, fname);
}

static void write_vmlinux_export_c_file(struct module *mod)
{
	struct buffer buf = { };

	buf_printf(&buf,
		   "#include <linux/export-internal.h>\n");

	add_exported_symbols(&buf, mod);
	write_if_changed(&buf, ".vmlinux.export.c");
	free(buf.p);
}

/* do sanity checks, and generate *.mod.c file */
static void write_mod_c_file(struct module *mod)
{
	struct buffer buf = { };
	char fname[PATH_MAX];
	int ret;

	add_header(&buf, mod);
	add_exported_symbols(&buf, mod);
	add_versions(&buf, mod);
	add_depends(&buf, mod);
	add_moddevtable(&buf, mod);
	add_srcversion(&buf, mod);

	ret = snprintf(fname, sizeof(fname), "%s.mod.c", mod->name);
	if (ret >= sizeof(fname)) {
		error("%s: too long path was truncated\n", fname);
		goto free;
	}

	write_if_changed(&buf, fname);

free:
	free(buf.p);
}

/* parse Module.symvers file. line format:
 * 0x12345678<tab>symbol<tab>module<tab>export<tab>namespace
 **/
static void read_dump(const char *fname)
{
	char *buf, *pos, *line;

	buf = read_text_file(fname);
	if (!buf)
		/* No symbol versions, silently ignore */
		return;

	pos = buf;

	while ((line = get_line(&pos))) {
		char *symname, *namespace, *modname, *d, *export;
		unsigned int crc;
		struct module *mod;
		struct symbol *s;
		bool gpl_only;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!(symname = strchr(line, '\t')))
			goto fail;
		*symname++ = '\0';
		if (!(modname = strchr(symname, '\t')))
			goto fail;
		*modname++ = '\0';
<<<<<<< HEAD
		if ((export = strchr(modname, '\t')) != NULL)
			*export++ = '\0';
		if (export && ((end = strchr(export, '\t')) != NULL))
			*end = '\0';
		crc = strtoul(line, &d, 16);
		if (*symname == '\0' || *modname == '\0' || *d != '\0')
			goto fail;
		mod = find_module(modname);
		if (!mod) {
			if (is_vmlinux(modname))
				have_vmlinux = 1;
			mod = new_module(modname);
			mod->skip = 1;
		}
		s = sym_add_exported(symname, mod, export_no(export));
		s->kernel    = kernel;
		s->preloaded = 1;
		sym_update_crc(symname, mod, crc, export_no(export));
	}
	return;
fail:
	fatal("parse error in symbol dump file\n");
}

/* For normal builds always dump all symbols.
 * For external modules only dump symbols
 * that are not read from kernel Module.symvers.
 **/
static int dump_sym(struct symbol *sym)
{
	if (!external_module)
		return 1;
	if (sym->vmlinux || sym->kernel)
		return 0;
	return 1;
}

static void write_dump(const char *fname)
{
	struct buffer buf = { };
	struct symbol *symbol;
	int n;

	for (n = 0; n < SYMBOL_HASH_SIZE ; n++) {
		symbol = symbolhash[n];
		while (symbol) {
			if (dump_sym(symbol))
				buf_printf(&buf, "0x%08x\t%s\t%s\t%s\n",
					symbol->crc, symbol->name,
					symbol->module->name,
					export_str(symbol->export));
			symbol = symbol->next;
		}
	}
	write_if_changed(&buf, fname);
}

struct ext_sym_list {
	struct ext_sym_list *next;
=======
		if (!(export = strchr(modname, '\t')))
			goto fail;
		*export++ = '\0';
		if (!(namespace = strchr(export, '\t')))
			goto fail;
		*namespace++ = '\0';

		crc = strtoul(line, &d, 16);
		if (*symname == '\0' || *modname == '\0' || *d != '\0')
			goto fail;

		if (!strcmp(export, "EXPORT_SYMBOL_GPL")) {
			gpl_only = true;
		} else if (!strcmp(export, "EXPORT_SYMBOL")) {
			gpl_only = false;
		} else {
			error("%s: unknown license %s. skip", symname, export);
			continue;
		}

		mod = find_module(modname);
		if (!mod) {
			mod = new_module(modname, strlen(modname));
			mod->from_dump = true;
		}
		s = sym_add_exported(symname, mod, gpl_only, namespace);
		sym_set_crc(s, crc);
	}
	free(buf);
	return;
fail:
	free(buf);
	fatal("parse error in symbol dump file\n");
}

static void write_dump(const char *fname)
{
	struct buffer buf = { };
	struct module *mod;
	struct symbol *sym;

	list_for_each_entry(mod, &modules, list) {
		if (mod->from_dump)
			continue;
		list_for_each_entry(sym, &mod->exported_symbols, list) {
			if (trim_unused_exports && !sym->used)
				continue;

			buf_printf(&buf, "0x%08x\t%s\t%s\tEXPORT_SYMBOL%s\t%s\n",
				   sym->crc, sym->name, mod->name,
				   sym->is_gpl_only ? "_GPL" : "",
				   sym->namespace);
		}
	}
	write_buf(&buf, fname);
	free(buf.p);
}

static void write_namespace_deps_files(const char *fname)
{
	struct module *mod;
	struct namespace_list *ns;
	struct buffer ns_deps_buf = {};

	list_for_each_entry(mod, &modules, list) {

		if (mod->from_dump || list_empty(&mod->missing_namespaces))
			continue;

		buf_printf(&ns_deps_buf, "%s.ko:", mod->name);

		list_for_each_entry(ns, &mod->missing_namespaces, list)
			buf_printf(&ns_deps_buf, " %s", ns->namespace);

		buf_printf(&ns_deps_buf, "\n");
	}

	write_if_changed(&ns_deps_buf, fname);
	free(ns_deps_buf.p);
}

struct dump_list {
	struct list_head list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *file;
};

int main(int argc, char **argv)
{
	struct module *mod;
<<<<<<< HEAD
	struct buffer buf = { };
	char *kernel_read = NULL, *module_read = NULL;
	char *dump_write = NULL;
	int opt;
	int err;
	struct ext_sym_list *extsym_iter;
	struct ext_sym_list *extsym_start = NULL;

	while ((opt = getopt(argc, argv, "i:I:e:cmsSo:awM:K:E")) != -1) {
		switch (opt) {
		case 'i':
			kernel_read = optarg;
			break;
		case 'I':
			module_read = optarg;
			external_module = 1;
			break;
		case 'c':
			cross_build = 1;
			break;
		case 'e':
			external_module = 1;
			extsym_iter =
			   NOFAIL(malloc(sizeof(*extsym_iter)));
			extsym_iter->next = extsym_start;
			extsym_iter->file = optarg;
			extsym_start = extsym_iter;
			break;
		case 'm':
			modversions = 1;
=======
	char *missing_namespace_deps = NULL;
	char *unused_exports_white_list = NULL;
	char *dump_write = NULL, *files_source = NULL;
	int opt;
	LIST_HEAD(dump_lists);
	struct dump_list *dl, *dl2;

	while ((opt = getopt(argc, argv, "ei:MmnT:to:au:WwENd:")) != -1) {
		switch (opt) {
		case 'e':
			external_module = true;
			break;
		case 'i':
			dl = NOFAIL(malloc(sizeof(*dl)));
			dl->file = optarg;
			list_add_tail(&dl->list, &dump_lists);
			break;
		case 'M':
			module_enabled = true;
			break;
		case 'm':
			modversions = true;
			break;
		case 'n':
			ignore_missing_files = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 'o':
			dump_write = optarg;
			break;
		case 'a':
<<<<<<< HEAD
			all_versions = 1;
			break;
		case 's':
			vmlinux_section_warnings = 0;
			break;
		case 'S':
			sec_mismatch_verbose = 0;
			break;
		case 'w':
			warn_unresolved = 1;
			break;
		case 'E':
			section_error_on_mismatch = 1;
=======
			all_versions = true;
			break;
		case 'T':
			files_source = optarg;
			break;
		case 't':
			trim_unused_exports = true;
			break;
		case 'u':
			unused_exports_white_list = optarg;
			break;
		case 'W':
			extra_warn = true;
			break;
		case 'w':
			warn_unresolved = true;
			break;
		case 'E':
			sec_mismatch_warn_only = false;
			break;
		case 'N':
			allow_missing_ns_imports = true;
			break;
		case 'd':
			missing_namespace_deps = optarg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			exit(1);
		}
	}

<<<<<<< HEAD
	if (kernel_read)
		read_dump(kernel_read, 1);
	if (module_read)
		read_dump(module_read, 0);
	while (extsym_start) {
		read_dump(extsym_start->file, 0);
		extsym_iter = extsym_start->next;
		free(extsym_start);
		extsym_start = extsym_iter;
=======
	list_for_each_entry_safe(dl, dl2, &dump_lists, list) {
		read_dump(dl->file);
		list_del(&dl->list);
		free(dl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	while (optind < argc)
		read_symbols(argv[optind++]);

<<<<<<< HEAD
	for (mod = modules; mod; mod = mod->next) {
		if (mod->skip)
			continue;
		check_exports(mod);
	}

	err = 0;

	for (mod = modules; mod; mod = mod->next) {
		char fname[strlen(mod->name) + 10];

		if (mod->skip)
			continue;

		buf.pos = 0;

		add_header(&buf, mod);
		add_intree_flag(&buf, !external_module);
		add_staging_flag(&buf, mod->name);
		err |= add_versions(&buf, mod);
		add_depends(&buf, mod, modules);
		add_moddevtable(&buf, mod);
		add_srcversion(&buf, mod);

		sprintf(fname, "%s.mod.c", mod->name);
		write_if_changed(&buf, fname);
	}

	if (dump_write)
		write_dump(dump_write);

	if (sec_mismatch_count && !sec_mismatch_verbose) {
		merror(
		"modpost: Found %d section mismatch(es).\n"
		"To see full details build your kernel with:\n"
		"'make CONFIG_DEBUG_SECTION_MISMATCH=y'\n",
		sec_mismatch_count);

	}

	if (sec_mismatch_count && section_error_on_mismatch) {
		err |= 1;
		printf(
		"To build the kernel despite the mismatches, "
		"build with:\n'make CONFIG_NO_ERROR_ON_MISMATCH=y'\n"
		"(NOTE: This is not recommended)\n");
	}

	return err;
=======
	if (files_source)
		read_symbols_from_files(files_source);

	list_for_each_entry(mod, &modules, list) {
		if (mod->from_dump || mod->is_vmlinux)
			continue;

		check_modname_len(mod);
		check_exports(mod);
	}

	if (unused_exports_white_list)
		handle_white_list_exports(unused_exports_white_list);

	list_for_each_entry(mod, &modules, list) {
		if (mod->from_dump)
			continue;

		if (mod->is_vmlinux)
			write_vmlinux_export_c_file(mod);
		else
			write_mod_c_file(mod);
	}

	if (missing_namespace_deps)
		write_namespace_deps_files(missing_namespace_deps);

	if (dump_write)
		write_dump(dump_write);
	if (sec_mismatch_count && !sec_mismatch_warn_only)
		error("Section mismatches detected.\n"
		      "Set CONFIG_SECTION_MISMATCH_WARN_ONLY=y to allow them.\n");

	if (nr_unresolved > MAX_UNRESOLVED_REPORTS)
		warn("suppressed %u unresolved symbol warnings because there were too many)\n",
		     nr_unresolved - MAX_UNRESOLVED_REPORTS);

	return error_occurred ? 1 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
