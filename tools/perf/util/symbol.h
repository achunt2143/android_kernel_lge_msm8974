<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __PERF_SYMBOL
#define __PERF_SYMBOL 1

#include <linux/types.h>
<<<<<<< HEAD
#include <stdbool.h>
#include <stdint.h>
#include "map.h"
#include "../perf.h"
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>

#ifdef HAVE_CPLUS_DEMANGLE
extern char *cplus_demangle(const char *, int);

static inline char *bfd_demangle(void __used *v, const char *c, int i)
{
	return cplus_demangle(c, i);
}
#else
#ifdef NO_DEMANGLE
static inline char *bfd_demangle(void __used *v, const char __used *c,
				 int __used i)
{
	return NULL;
}
#else
#include <bfd.h>
#endif
#endif

int hex2u64(const char *ptr, u64 *val);
char *strxfrchar(char *s, char from, char to);
=======
#include <linux/refcount.h>
#include <stdbool.h>
#include <stdint.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>
#include "addr_location.h"
#include "path.h"
#include "symbol_conf.h"
#include "spark.h"

#ifdef HAVE_LIBELF_SUPPORT
#include <libelf.h>
#include <gelf.h>
#endif
#include <elf.h>

struct dso;
struct map;
struct maps;
struct option;
struct build_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * libelf 0.8.x and earlier do not support ELF_C_READ_MMAP;
 * for newer versions we can use mmap to reduce memory usage:
 */
<<<<<<< HEAD
#ifdef LIBELF_NO_MMAP
# define PERF_ELF_C_READ_MMAP ELF_C_READ
#else
# define PERF_ELF_C_READ_MMAP ELF_C_READ_MMAP
#endif

#ifndef DMGL_PARAMS
#define DMGL_PARAMS      (1 << 0)       /* Include function args */
#define DMGL_ANSI        (1 << 1)       /* Include const, volatile, etc */
#endif

#define BUILD_ID_SIZE 20

/** struct symbol - symtab entry
 *
 * @ignore - resolvable but tools ignore it (e.g. idle routines)
 */
struct symbol {
	struct rb_node	rb_node;
	u64		start;
	u64		end;
	u16		namelen;
	u8		binding;
	bool		ignore;
	char		name[0];
};

void symbol__delete(struct symbol *sym);

struct strlist;

struct symbol_conf {
	unsigned short	priv_size;
	unsigned short	nr_events;
	bool		try_vmlinux_path,
			show_kernel_path,
			use_modules,
			sort_by_name,
			show_nr_samples,
			show_total_period,
			use_callchain,
			exclude_other,
			show_cpu_utilization,
			initialized,
			kptr_restrict,
			annotate_asm_raw,
			annotate_src;
	const char	*vmlinux_name,
			*kallsyms_name,
			*source_prefix,
			*field_sep;
	const char	*default_guest_vmlinux_name,
			*default_guest_kallsyms,
			*default_guest_modules;
	const char	*guestmount;
	const char	*dso_list_str,
			*comm_list_str,
			*sym_list_str,
			*col_width_list_str;
       struct strlist	*dso_list,
			*comm_list,
			*sym_list,
			*dso_from_list,
			*dso_to_list,
			*sym_from_list,
			*sym_to_list;
	const char	*symfs;
};

extern struct symbol_conf symbol_conf;
=======
#ifdef ELF_C_READ_MMAP
# define PERF_ELF_C_READ_MMAP ELF_C_READ_MMAP
#else
# define PERF_ELF_C_READ_MMAP ELF_C_READ
#endif

#ifdef HAVE_LIBELF_SUPPORT
Elf_Scn *elf_section_by_name(Elf *elf, GElf_Ehdr *ep,
			     GElf_Shdr *shp, const char *name, size_t *idx);
#endif

/**
 * A symtab entry. When allocated this may be preceded by an annotation (see
 * symbol__annotation) and/or a browser_index (see symbol__browser_index).
 */
struct symbol {
	struct rb_node	rb_node;
	/** Range of symbol [start, end). */
	u64		start;
	u64		end;
	/** Length of the string name. */
	u16		namelen;
	/** ELF symbol type as defined for st_info. E.g STT_OBJECT or STT_FUNC. */
	u8		type:4;
	/** ELF binding type as defined for st_info. E.g. STB_WEAK or STB_GLOBAL. */
	u8		binding:4;
	/** Set true for kernel symbols of idle routines. */
	u8		idle:1;
	/** Resolvable but tools ignore it (e.g. idle routines). */
	u8		ignore:1;
	/** Symbol for an inlined function. */
	u8		inlined:1;
	/** Has symbol__annotate2 been performed. */
	u8		annotate2:1;
	/** Symbol is an alias of an STT_GNU_IFUNC */
	u8		ifunc_alias:1;
	/** Architecture specific. Unused except on PPC where it holds st_other. */
	u8		arch_sym;
	/** The name of length namelen associated with the symbol. */
	char		name[];
};

void symbol__delete(struct symbol *sym);
void symbols__delete(struct rb_root_cached *symbols);

/* symbols__for_each_entry - iterate over symbols (rb_root)
 *
 * @symbols: the rb_root of symbols
 * @pos: the 'struct symbol *' to use as a loop cursor
 * @nd: the 'struct rb_node *' to use as a temporary storage
 */
#define symbols__for_each_entry(symbols, pos, nd)			\
	for (nd = rb_first_cached(symbols);					\
	     nd && (pos = rb_entry(nd, struct symbol, rb_node));	\
	     nd = rb_next(nd))

static inline size_t symbol__size(const struct symbol *sym)
{
	return sym->end - sym->start;
}

struct strlist;
struct intlist;

static inline int __symbol__join_symfs(char *bf, size_t size, const char *path)
{
	return path__join(bf, size, symbol_conf.symfs, path);
}

#define symbol__join_symfs(bf, path) __symbol__join_symfs(bf, sizeof(bf), path)

extern int vmlinux_path__nr_entries;
extern char **vmlinux_path;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void *symbol__priv(struct symbol *sym)
{
	return ((void *)sym) - symbol_conf.priv_size;
}

struct ref_reloc_sym {
	const char	*name;
	u64		addr;
	u64		unrelocated_addr;
};

<<<<<<< HEAD
struct map_symbol {
	struct map    *map;
	struct symbol *sym;
	bool	      unfolded;
	bool	      has_children;
};

struct addr_map_symbol {
	struct map    *map;
	struct symbol *sym;
	u64	      addr;
	u64	      al_addr;
};

struct branch_info {
	struct addr_map_symbol from;
	struct addr_map_symbol to;
	struct branch_flags flags;
};

struct addr_location {
	struct thread *thread;
	struct map    *map;
	struct symbol *sym;
	u64	      addr;
	char	      level;
	bool	      filtered;
	u8	      cpumode;
	s32	      cpu;
};

enum dso_kernel_type {
	DSO_TYPE_USER = 0,
	DSO_TYPE_KERNEL,
	DSO_TYPE_GUEST_KERNEL
};

struct dso {
	struct list_head node;
	struct rb_root	 symbols[MAP__NR_TYPES];
	struct rb_root	 symbol_names[MAP__NR_TYPES];
	enum dso_kernel_type	kernel;
	u8		 adjust_symbols:1;
	u8		 has_build_id:1;
	u8		 hit:1;
	u8		 annotate_warned:1;
	u8		 sname_alloc:1;
	u8		 lname_alloc:1;
	unsigned char	 symtab_type;
	u8		 sorted_by_name;
	u8		 loaded;
	u8		 build_id[BUILD_ID_SIZE];
	const char	 *short_name;
	char	 	 *long_name;
	u16		 long_name_len;
	u16		 short_name_len;
	char		 name[0];
};

struct dso *dso__new(const char *name);
void dso__delete(struct dso *dso);

int dso__name_len(const struct dso *dso);

bool dso__loaded(const struct dso *dso, enum map_type type);
bool dso__sorted_by_name(const struct dso *dso, enum map_type type);

static inline void dso__set_loaded(struct dso *dso, enum map_type type)
{
	dso->loaded |= (1 << type);
}

void dso__sort_by_name(struct dso *dso, enum map_type type);

struct dso *__dsos__findnew(struct list_head *head, const char *name);

int dso__load(struct dso *dso, struct map *map, symbol_filter_t filter);
int dso__load_vmlinux(struct dso *dso, struct map *map,
		      const char *vmlinux, symbol_filter_t filter);
int dso__load_vmlinux_path(struct dso *dso, struct map *map,
			   symbol_filter_t filter);
int dso__load_kallsyms(struct dso *dso, const char *filename, struct map *map,
		       symbol_filter_t filter);
int machine__load_kallsyms(struct machine *machine, const char *filename,
			   enum map_type type, symbol_filter_t filter);
int machine__load_vmlinux_path(struct machine *machine, enum map_type type,
			       symbol_filter_t filter);

size_t __dsos__fprintf(struct list_head *head, FILE *fp);

size_t machine__fprintf_dsos_buildid(struct machine *machine,
				     FILE *fp, bool with_hits);
size_t machines__fprintf_dsos(struct rb_root *machines, FILE *fp);
size_t machines__fprintf_dsos_buildid(struct rb_root *machines,
				      FILE *fp, bool with_hits);
size_t dso__fprintf_buildid(struct dso *dso, FILE *fp);
size_t dso__fprintf_symbols_by_name(struct dso *dso,
				    enum map_type type, FILE *fp);
size_t dso__fprintf(struct dso *dso, enum map_type type, FILE *fp);

enum symtab_type {
	SYMTAB__KALLSYMS = 0,
	SYMTAB__GUEST_KALLSYMS,
	SYMTAB__JAVA_JIT,
	SYMTAB__BUILD_ID_CACHE,
	SYMTAB__FEDORA_DEBUGINFO,
	SYMTAB__UBUNTU_DEBUGINFO,
	SYMTAB__BUILDID_DEBUGINFO,
	SYMTAB__SYSTEM_PATH_DSO,
	SYMTAB__GUEST_KMODULE,
	SYMTAB__SYSTEM_PATH_KMODULE,
	SYMTAB__NOT_FOUND,
};

char dso__symtab_origin(const struct dso *dso);
void dso__set_long_name(struct dso *dso, char *name);
void dso__set_build_id(struct dso *dso, void *build_id);
void dso__read_running_kernel_build_id(struct dso *dso,
				       struct machine *machine);
struct symbol *dso__find_symbol(struct dso *dso, enum map_type type,
				u64 addr);
struct symbol *dso__find_symbol_by_name(struct dso *dso, enum map_type type,
					const char *name);

int filename__read_build_id(const char *filename, void *bf, size_t size);
int sysfs__read_build_id(const char *filename, void *bf, size_t size);
bool __dsos__read_build_ids(struct list_head *head, bool with_hits);
int build_id__sprintf(const u8 *build_id, int len, char *bf);
int kallsyms__parse(const char *filename, void *arg,
		    int (*process_symbol)(void *arg, const char *name,
					  char type, u64 start, u64 end));

void machine__destroy_kernel_maps(struct machine *machine);
int __machine__create_kernel_maps(struct machine *machine, struct dso *kernel);
int machine__create_kernel_maps(struct machine *machine);

int machines__create_kernel_maps(struct rb_root *machines, pid_t pid);
int machines__create_guest_kernel_maps(struct rb_root *machines);
void machines__destroy_guest_kernel_maps(struct rb_root *machines);

int symbol__init(void);
void symbol__exit(void);
size_t symbol__fprintf_symname_offs(const struct symbol *sym,
				    const struct addr_location *al, FILE *fp);
size_t symbol__fprintf_symname(const struct symbol *sym, FILE *fp);
bool symbol_type__is_a(char symbol_type, enum map_type map_type);

size_t machine__fprintf_vmlinux_path(struct machine *machine, FILE *fp);
=======
int dso__load(struct dso *dso, struct map *map);
int dso__load_vmlinux(struct dso *dso, struct map *map,
		      const char *vmlinux, bool vmlinux_allocated);
int dso__load_vmlinux_path(struct dso *dso, struct map *map);
int __dso__load_kallsyms(struct dso *dso, const char *filename, struct map *map,
			 bool no_kcore);
int dso__load_kallsyms(struct dso *dso, const char *filename, struct map *map);

void dso__insert_symbol(struct dso *dso,
			struct symbol *sym);
void dso__delete_symbol(struct dso *dso,
			struct symbol *sym);

struct symbol *dso__find_symbol(struct dso *dso, u64 addr);
struct symbol *dso__find_symbol_nocache(struct dso *dso, u64 addr);

struct symbol *dso__next_symbol_by_name(struct dso *dso, size_t *idx);
struct symbol *dso__find_symbol_by_name(struct dso *dso, const char *name, size_t *idx);

struct symbol *dso__first_symbol(struct dso *dso);
struct symbol *dso__last_symbol(struct dso *dso);
struct symbol *dso__next_symbol(struct symbol *sym);

enum dso_type dso__type_fd(int fd);

int filename__read_build_id(const char *filename, struct build_id *id);
int sysfs__read_build_id(const char *filename, struct build_id *bid);
int modules__parse(const char *filename, void *arg,
		   int (*process_module)(void *arg, const char *name,
					 u64 start, u64 size));
int filename__read_debuglink(const char *filename, char *debuglink,
			     size_t size);
bool filename__has_section(const char *filename, const char *sec);

struct perf_env;
int symbol__init(struct perf_env *env);
void symbol__exit(void);
void symbol__elf_init(void);
int symbol__annotation_init(void);

struct symbol *symbol__new(u64 start, u64 len, u8 binding, u8 type, const char *name);
size_t __symbol__fprintf_symname_offs(const struct symbol *sym,
				      const struct addr_location *al,
				      bool unknown_as_addr,
				      bool print_offsets, FILE *fp);
size_t symbol__fprintf_symname_offs(const struct symbol *sym,
				    const struct addr_location *al, FILE *fp);
size_t __symbol__fprintf_symname(const struct symbol *sym,
				 const struct addr_location *al,
				 bool unknown_as_addr, FILE *fp);
size_t symbol__fprintf_symname(const struct symbol *sym, FILE *fp);
size_t symbol__fprintf(struct symbol *sym, FILE *fp);
bool symbol__restricted_filename(const char *filename,
				 const char *restricted_filename);
int symbol__config_symfs(const struct option *opt __maybe_unused,
			 const char *dir, int unset __maybe_unused);

struct symsrc;

#ifdef HAVE_LIBBFD_SUPPORT
int dso__load_bfd_symbols(struct dso *dso, const char *debugfile);
#endif

int dso__load_sym(struct dso *dso, struct map *map, struct symsrc *syms_ss,
		  struct symsrc *runtime_ss, int kmodule);
int dso__synthesize_plt_symbols(struct dso *dso, struct symsrc *ss);

char *dso__demangle_sym(struct dso *dso, int kmodule, const char *elf_name);

void __symbols__insert(struct rb_root_cached *symbols, struct symbol *sym,
		       bool kernel);
void symbols__insert(struct rb_root_cached *symbols, struct symbol *sym);
void symbols__fixup_duplicate(struct rb_root_cached *symbols);
void symbols__fixup_end(struct rb_root_cached *symbols, bool is_kallsyms);

typedef int (*mapfn_t)(u64 start, u64 len, u64 pgoff, void *data);
int file__read_maps(int fd, bool exe, mapfn_t mapfn, void *data,
		    bool *is_64_bit);

#define PERF_KCORE_EXTRACT "/tmp/perf-kcore-XXXXXX"

struct kcore_extract {
	char *kcore_filename;
	u64 addr;
	u64 offs;
	u64 len;
	char extract_filename[sizeof(PERF_KCORE_EXTRACT)];
	int fd;
};

int kcore_extract__create(struct kcore_extract *kce);
void kcore_extract__delete(struct kcore_extract *kce);

int kcore_copy(const char *from_dir, const char *to_dir);
int compare_proc_modules(const char *from, const char *to);

int setup_list(struct strlist **list, const char *list_str,
	       const char *list_name);
int setup_intlist(struct intlist **list, const char *list_str,
		  const char *list_name);

#ifdef HAVE_LIBELF_SUPPORT
bool elf__needs_adjust_symbols(GElf_Ehdr ehdr);
void arch__sym_update(struct symbol *s, GElf_Sym *sym);
#endif

const char *arch__normalize_symbol_name(const char *name);
#define SYMBOL_A 0
#define SYMBOL_B 1

int arch__compare_symbol_names(const char *namea, const char *nameb);
int arch__compare_symbol_names_n(const char *namea, const char *nameb,
				 unsigned int n);
int arch__choose_best_symbol(struct symbol *syma, struct symbol *symb);

enum symbol_tag_include {
	SYMBOL_TAG_INCLUDE__NONE = 0,
	SYMBOL_TAG_INCLUDE__DEFAULT_ONLY
};

int symbol__match_symbol_name(const char *namea, const char *nameb,
			      enum symbol_tag_include includes);

/* structure containing an SDT note's info */
struct sdt_note {
	char *name;			/* name of the note*/
	char *provider;			/* provider name */
	char *args;
	bool bit32;			/* whether the location is 32 bits? */
	union {				/* location, base and semaphore addrs */
		Elf64_Addr a64[3];
		Elf32_Addr a32[3];
	} addr;
	struct list_head note_list;	/* SDT notes' list */
};

int get_sdt_note_list(struct list_head *head, const char *target);
int cleanup_sdt_note_list(struct list_head *sdt_notes);
int sdt_notes__get_count(struct list_head *start);

#define SDT_PROBES_SCN ".probes"
#define SDT_BASE_SCN ".stapsdt.base"
#define SDT_NOTE_SCN  ".note.stapsdt"
#define SDT_NOTE_TYPE 3
#define SDT_NOTE_NAME "stapsdt"
#define NR_ADDR 3

enum {
	SDT_NOTE_IDX_LOC = 0,
	SDT_NOTE_IDX_BASE,
	SDT_NOTE_IDX_REFCTR,
};

struct mem_info *mem_info__new(void);
struct mem_info *mem_info__get(struct mem_info *mi);
void   mem_info__put(struct mem_info *mi);

static inline void __mem_info__zput(struct mem_info **mi)
{
	mem_info__put(*mi);
	*mi = NULL;
}

#define mem_info__zput(mi) __mem_info__zput(&mi)

int symbol__validate_sym_arguments(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __PERF_SYMBOL */
