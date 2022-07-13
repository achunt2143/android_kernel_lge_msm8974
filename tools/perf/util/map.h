<<<<<<< HEAD
#ifndef __PERF_MAP_H
#define __PERF_MAP_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_MAP_H
#define __PERF_MAP_H

#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>
<<<<<<< HEAD
#include <stdbool.h>
#include "types.h"

enum map_type {
	MAP__FUNCTION = 0,
	MAP__VARIABLE,
};

#define MAP__NR_TYPES (MAP__VARIABLE + 1)

extern const char *map_type__name[MAP__NR_TYPES];

struct dso;
struct ip_callchain;
struct ref_reloc_sym;
struct map_groups;
struct machine;
struct perf_evsel;

struct map {
	union {
		struct rb_node	rb_node;
		struct list_head node;
	};
	u64			start;
	u64			end;
	u8 /* enum map_type */	type;
	bool			referenced;
	bool			erange_warned;
	u32			priv;
	u64			pgoff;

	/* ip -> dso rip */
	u64			(*map_ip)(struct map *, u64);
	/* dso rip -> ip */
	u64			(*unmap_ip)(struct map *, u64);

	struct dso		*dso;
	struct map_groups	*groups;
};

struct kmap {
	struct ref_reloc_sym	*ref_reloc_sym;
	struct map_groups	*kmaps;
};

struct map_groups {
	struct rb_root	 maps[MAP__NR_TYPES];
	struct list_head removed_maps[MAP__NR_TYPES];
	struct machine	 *machine;
};

/* Native host kernel uses -1 as pid index in machine */
#define	HOST_KERNEL_ID			(-1)
#define	DEFAULT_GUEST_KERNEL_ID		(0)

struct machine {
	struct rb_node	  rb_node;
	pid_t		  pid;
	u16		  id_hdr_size;
	char		  *root_dir;
	struct rb_root	  threads;
	struct list_head  dead_threads;
	struct thread	  *last_match;
	struct list_head  user_dsos;
	struct list_head  kernel_dsos;
	struct map_groups kmaps;
	struct map	  *vmlinux_maps[MAP__NR_TYPES];
};

static inline
struct map *machine__kernel_map(struct machine *self, enum map_type type)
{
	return self->vmlinux_maps[type];
}

static inline struct kmap *map__kmap(struct map *self)
{
	return (struct kmap *)(self + 1);
}

static inline u64 map__map_ip(struct map *map, u64 ip)
{
	return ip - map->start + map->pgoff;
}

static inline u64 map__unmap_ip(struct map *map, u64 ip)
{
	return ip + map->start - map->pgoff;
}

static inline u64 identity__map_ip(struct map *map __used, u64 ip)
{
	return ip;
}


/* rip/ip <-> addr suitable for passing to `objdump --start-address=` */
u64 map__rip_2objdump(struct map *map, u64 rip);
u64 map__objdump_2ip(struct map *map, u64 addr);

struct symbol;

typedef int (*symbol_filter_t)(struct map *map, struct symbol *sym);

void map__init(struct map *self, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso);
struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, char *filename,
		     enum map_type type);
void map__delete(struct map *self);
struct map *map__clone(struct map *self);
int map__overlap(struct map *l, struct map *r);
size_t map__fprintf(struct map *self, FILE *fp);
size_t map__fprintf_dsoname(struct map *map, FILE *fp);

int map__load(struct map *self, symbol_filter_t filter);
struct symbol *map__find_symbol(struct map *self,
				u64 addr, symbol_filter_t filter);
struct symbol *map__find_symbol_by_name(struct map *self, const char *name,
					symbol_filter_t filter);
void map__fixup_start(struct map *self);
void map__fixup_end(struct map *self);

void map__reloc_vmlinux(struct map *self);

size_t __map_groups__fprintf_maps(struct map_groups *mg,
				  enum map_type type, int verbose, FILE *fp);
void maps__insert(struct rb_root *maps, struct map *map);
void maps__remove(struct rb_root *maps, struct map *map);
struct map *maps__find(struct rb_root *maps, u64 addr);
void map_groups__init(struct map_groups *mg);
void map_groups__exit(struct map_groups *mg);
int map_groups__clone(struct map_groups *mg,
		      struct map_groups *parent, enum map_type type);
size_t map_groups__fprintf(struct map_groups *mg, int verbose, FILE *fp);
size_t map_groups__fprintf_maps(struct map_groups *mg, int verbose, FILE *fp);

typedef void (*machine__process_t)(struct machine *self, void *data);

void machines__process(struct rb_root *self, machine__process_t process, void *data);
struct machine *machines__add(struct rb_root *self, pid_t pid,
			      const char *root_dir);
struct machine *machines__find_host(struct rb_root *self);
struct machine *machines__find(struct rb_root *self, pid_t pid);
struct machine *machines__findnew(struct rb_root *self, pid_t pid);
char *machine__mmap_name(struct machine *self, char *bf, size_t size);
int machine__init(struct machine *self, const char *root_dir, pid_t pid);
void machine__exit(struct machine *self);
void machine__delete(struct machine *self);

int machine__resolve_callchain(struct machine *machine,
			       struct perf_evsel *evsel, struct thread *thread,
			       struct ip_callchain *chain,
			       struct symbol **parent);
int maps__set_kallsyms_ref_reloc_sym(struct map **maps, const char *symbol_name,
				     u64 addr);

/*
 * Default guest kernel is defined by parameter --guestkallsyms
 * and --guestmodules
 */
static inline bool machine__is_default_guest(struct machine *self)
{
	return self ? self->pid == DEFAULT_GUEST_KERNEL_ID : false;
}

static inline bool machine__is_host(struct machine *self)
{
	return self ? self->pid == HOST_KERNEL_ID : false;
}

static inline void map_groups__insert(struct map_groups *mg, struct map *map)
{
	maps__insert(&mg->maps[map->type], map);
	map->groups = mg;
}

static inline void map_groups__remove(struct map_groups *mg, struct map *map)
{
	maps__remove(&mg->maps[map->type], map);
}

static inline struct map *map_groups__find(struct map_groups *mg,
					   enum map_type type, u64 addr)
{
	return maps__find(&mg->maps[type], addr);
}

struct symbol *map_groups__find_symbol(struct map_groups *mg,
				       enum map_type type, u64 addr,
				       struct map **mapp,
				       symbol_filter_t filter);

struct symbol *map_groups__find_symbol_by_name(struct map_groups *mg,
					       enum map_type type,
					       const char *name,
					       struct map **mapp,
					       symbol_filter_t filter);


struct thread *machine__findnew_thread(struct machine *machine, pid_t pid);
void machine__remove_thread(struct machine *machine, struct thread *th);

size_t machine__fprintf(struct machine *machine, FILE *fp);

static inline
struct symbol *machine__find_kernel_symbol(struct machine *self,
					   enum map_type type, u64 addr,
					   struct map **mapp,
					   symbol_filter_t filter)
{
	return map_groups__find_symbol(&self->kmaps, type, addr, mapp, filter);
}

static inline
struct symbol *machine__find_kernel_function(struct machine *self, u64 addr,
					     struct map **mapp,
					     symbol_filter_t filter)
{
	return machine__find_kernel_symbol(self, MAP__FUNCTION, addr, mapp, filter);
}

static inline
struct symbol *map_groups__find_function_by_name(struct map_groups *mg,
						 const char *name, struct map **mapp,
						 symbol_filter_t filter)
{
	return map_groups__find_symbol_by_name(mg, MAP__FUNCTION, name, mapp, filter);
}

static inline
struct symbol *machine__find_kernel_function_by_name(struct machine *self,
						     const char *name,
						     struct map **mapp,
						     symbol_filter_t filter)
{
	return map_groups__find_function_by_name(&self->kmaps, name, mapp,
						 filter);
}

int map_groups__fixup_overlappings(struct map_groups *mg, struct map *map,
				   int verbose, FILE *fp);

struct map *map_groups__find_by_name(struct map_groups *mg,
				     enum map_type type, const char *name);
struct map *machine__new_module(struct machine *self, u64 start, const char *filename);

void map_groups__flush(struct map_groups *mg);

=======
#include <string.h>
#include <stdbool.h>
#include <linux/types.h>
#include <internal/rc_check.h>

struct dso;
struct maps;
struct machine;

enum mapping_type {
	/* map__map_ip/map__unmap_ip are given as offsets in the DSO. */
	MAPPING_TYPE__DSO,
	/* map__map_ip/map__unmap_ip are just the given ip value. */
	MAPPING_TYPE__IDENTITY,
};

DECLARE_RC_STRUCT(map) {
	u64			start;
	u64			end;
	u64			pgoff;
	u64			reloc;
	struct dso		*dso;
	refcount_t		refcnt;
	u32			prot;
	u32			flags;
	enum mapping_type	mapping_type:8;
	bool			erange_warned;
	bool			priv;
};

struct kmap;

struct kmap *__map__kmap(struct map *map);
struct kmap *map__kmap(struct map *map);
struct maps *map__kmaps(struct map *map);

static inline struct dso *map__dso(const struct map *map)
{
	return RC_CHK_ACCESS(map)->dso;
}

static inline u64 map__start(const struct map *map)
{
	return RC_CHK_ACCESS(map)->start;
}

static inline u64 map__end(const struct map *map)
{
	return RC_CHK_ACCESS(map)->end;
}

static inline u64 map__pgoff(const struct map *map)
{
	return RC_CHK_ACCESS(map)->pgoff;
}

static inline u64 map__reloc(const struct map *map)
{
	return RC_CHK_ACCESS(map)->reloc;
}

static inline u32 map__flags(const struct map *map)
{
	return RC_CHK_ACCESS(map)->flags;
}

static inline u32 map__prot(const struct map *map)
{
	return RC_CHK_ACCESS(map)->prot;
}

static inline bool map__priv(const struct map *map)
{
	return RC_CHK_ACCESS(map)->priv;
}

static inline refcount_t *map__refcnt(struct map *map)
{
	return &RC_CHK_ACCESS(map)->refcnt;
}

static inline bool map__erange_warned(struct map *map)
{
	return RC_CHK_ACCESS(map)->erange_warned;
}

static inline size_t map__size(const struct map *map)
{
	return map__end(map) - map__start(map);
}

/* ip -> dso rip */
static inline u64 map__dso_map_ip(const struct map *map, u64 ip)
{
	return ip - map__start(map) + map__pgoff(map);
}

/* dso rip -> ip */
static inline u64 map__dso_unmap_ip(const struct map *map, u64 rip)
{
	return rip + map__start(map) - map__pgoff(map);
}

static inline u64 map__map_ip(const struct map *map, u64 ip_or_rip)
{
	if ((RC_CHK_ACCESS(map)->mapping_type) == MAPPING_TYPE__DSO)
		return map__dso_map_ip(map, ip_or_rip);
	else
		return ip_or_rip;
}

static inline u64 map__unmap_ip(const struct map *map, u64 ip_or_rip)
{
	if ((RC_CHK_ACCESS(map)->mapping_type) == MAPPING_TYPE__DSO)
		return map__dso_unmap_ip(map, ip_or_rip);
	else
		return ip_or_rip;
}

/* rip/ip <-> addr suitable for passing to `objdump --start-address=` */
u64 map__rip_2objdump(struct map *map, u64 rip);

/* objdump address -> memory address */
u64 map__objdump_2mem(struct map *map, u64 ip);

struct symbol;
struct thread;

/* map__for_each_symbol - iterate over the symbols in the given map
 *
 * @map: the 'struct map *' in which symbols are iterated
 * @pos: the 'struct symbol *' to use as a loop cursor
 * @n: the 'struct rb_node *' to use as a temporary storage
 * Note: caller must ensure map->dso is not NULL (map is loaded).
 */
#define map__for_each_symbol(map, pos, n)	\
	dso__for_each_symbol(map__dso(map), pos, n)

/* map__for_each_symbol_with_name - iterate over the symbols in the given map
 *                                  that have the given name
 *
 * @map: the 'struct map *' in which symbols are iterated
 * @sym_name: the symbol name
 * @pos: the 'struct symbol *' to use as a loop cursor
 * @idx: the cursor index in the symbol names array
 */
#define __map__for_each_symbol_by_name(map, sym_name, pos, idx)		\
	for (pos = map__find_symbol_by_name_idx(map, sym_name, &idx);	\
	     pos &&						\
	     !symbol__match_symbol_name(pos->name, sym_name,	\
					SYMBOL_TAG_INCLUDE__DEFAULT_ONLY); \
	     pos = dso__next_symbol_by_name(map__dso(map), &idx))

#define map__for_each_symbol_by_name(map, sym_name, pos, idx)	\
	__map__for_each_symbol_by_name(map, sym_name, (pos), idx)

void map__init(struct map *map,
	       u64 start, u64 end, u64 pgoff, struct dso *dso);

struct dso_id;
struct build_id;

struct map *map__new(struct machine *machine, u64 start, u64 len,
		     u64 pgoff, struct dso_id *id, u32 prot, u32 flags,
		     struct build_id *bid, char *filename, struct thread *thread);
struct map *map__new2(u64 start, struct dso *dso);
void map__delete(struct map *map);
struct map *map__clone(struct map *map);

static inline struct map *map__get(struct map *map)
{
	struct map *result;

	if (RC_CHK_GET(result, map))
		refcount_inc(map__refcnt(map));

	return result;
}

void map__put(struct map *map);

static inline void __map__zput(struct map **map)
{
	map__put(*map);
	*map = NULL;
}

#define map__zput(map) __map__zput(&map)

size_t map__fprintf(struct map *map, FILE *fp);
size_t map__fprintf_dsoname(struct map *map, FILE *fp);
size_t map__fprintf_dsoname_dsoff(struct map *map, bool print_off, u64 addr, FILE *fp);
char *map__srcline(struct map *map, u64 addr, struct symbol *sym);
int map__fprintf_srcline(struct map *map, u64 addr, const char *prefix,
			 FILE *fp);

int map__load(struct map *map);
struct symbol *map__find_symbol(struct map *map, u64 addr);
struct symbol *map__find_symbol_by_name(struct map *map, const char *name);
struct symbol *map__find_symbol_by_name_idx(struct map *map, const char *name, size_t *idx);
void map__fixup_start(struct map *map);
void map__fixup_end(struct map *map);

int map__set_kallsyms_ref_reloc_sym(struct map *map, const char *symbol_name,
				    u64 addr);

bool __map__is_kernel(const struct map *map);
bool __map__is_extra_kernel_map(const struct map *map);
bool __map__is_bpf_prog(const struct map *map);
bool __map__is_bpf_image(const struct map *map);
bool __map__is_ool(const struct map *map);

static inline bool __map__is_kmodule(const struct map *map)
{
	return !__map__is_kernel(map) && !__map__is_extra_kernel_map(map) &&
	       !__map__is_bpf_prog(map) && !__map__is_ool(map) &&
	       !__map__is_bpf_image(map);
}

bool map__has_symbols(const struct map *map);

bool map__contains_symbol(const struct map *map, const struct symbol *sym);

#define ENTRY_TRAMPOLINE_NAME "__entry_SYSCALL_64_trampoline"

static inline bool is_entry_trampoline(const char *name)
{
	return !strcmp(name, ENTRY_TRAMPOLINE_NAME);
}

static inline bool is_bpf_image(const char *name)
{
	return strncmp(name, "bpf_trampoline_", sizeof("bpf_trampoline_") - 1) == 0 ||
	       strncmp(name, "bpf_dispatcher_", sizeof("bpf_dispatcher_") - 1) == 0;
}

static inline int is_anon_memory(const char *filename)
{
	return !strcmp(filename, "//anon") ||
	       !strncmp(filename, "/dev/zero", sizeof("/dev/zero") - 1) ||
	       !strncmp(filename, "/anon_hugepage", sizeof("/anon_hugepage") - 1);
}

static inline int is_no_dso_memory(const char *filename)
{
	return !strncmp(filename, "[stack", 6) ||
	       !strncmp(filename, "/SYSV", 5)  ||
	       !strcmp(filename, "[heap]");
}

static inline void map__set_start(struct map *map, u64 start)
{
	RC_CHK_ACCESS(map)->start = start;
}

static inline void map__set_end(struct map *map, u64 end)
{
	RC_CHK_ACCESS(map)->end = end;
}

static inline void map__set_pgoff(struct map *map, u64 pgoff)
{
	RC_CHK_ACCESS(map)->pgoff = pgoff;
}

static inline void map__add_pgoff(struct map *map, u64 inc)
{
	RC_CHK_ACCESS(map)->pgoff += inc;
}

static inline void map__set_reloc(struct map *map, u64 reloc)
{
	RC_CHK_ACCESS(map)->reloc = reloc;
}

static inline void map__set_priv(struct map *map, int priv)
{
	RC_CHK_ACCESS(map)->priv = priv;
}

static inline void map__set_erange_warned(struct map *map, bool erange_warned)
{
	RC_CHK_ACCESS(map)->erange_warned = erange_warned;
}

static inline void map__set_dso(struct map *map, struct dso *dso)
{
	RC_CHK_ACCESS(map)->dso = dso;
}

static inline void map__set_mapping_type(struct map *map, enum mapping_type type)
{
	RC_CHK_ACCESS(map)->mapping_type = type;
}

static inline enum mapping_type map__mapping_type(struct map *map)
{
	return RC_CHK_ACCESS(map)->mapping_type;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __PERF_MAP_H */
