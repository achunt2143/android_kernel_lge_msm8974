<<<<<<< HEAD
#include "symbol.h"
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"

const char *map_type__name[MAP__NR_TYPES] = {
	[MAP__FUNCTION] = "Functions",
	[MAP__VARIABLE] = "Variables",
};

static inline int is_anon_memory(const char *filename)
{
	return !strcmp(filename, "//anon") ||
	       !strcmp(filename, "/dev/zero (deleted)") ||
	       !strcmp(filename, "/anon_hugepage (deleted)");
}

static inline int is_no_dso_memory(const char *filename)
{
	return !strcmp(filename, "[stack]") ||
	       !strcmp(filename, "[vdso]")  ||
	       !strcmp(filename, "[heap]");
}

void map__init(struct map *self, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso)
{
	self->type     = type;
	self->start    = start;
	self->end      = end;
	self->pgoff    = pgoff;
	self->dso      = dso;
	self->map_ip   = map__map_ip;
	self->unmap_ip = map__unmap_ip;
	RB_CLEAR_NODE(&self->rb_node);
	self->groups   = NULL;
	self->referenced = false;
	self->erange_warned = false;
}

struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, char *filename,
		     enum map_type type)
{
	struct map *self = malloc(sizeof(*self));

	if (self != NULL) {
		char newfilename[PATH_MAX];
		struct dso *dso;
		int anon, no_dso;

		anon = is_anon_memory(filename);
		no_dso = is_no_dso_memory(filename);

		if (anon) {
			snprintf(newfilename, sizeof(newfilename), "/tmp/perf-%d.map", pid);
			filename = newfilename;
		}

		dso = __dsos__findnew(dsos__list, filename);
		if (dso == NULL)
			goto out_delete;

		map__init(self, type, start, start + len, pgoff, dso);

		if (anon || no_dso) {
			self->map_ip = self->unmap_ip = identity__map_ip;
=======
// SPDX-License-Identifier: GPL-2.0
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/string.h>
#include <linux/zalloc.h>
#include <uapi/linux/mman.h> /* To get things like MAP_HUGETLB even on older libc headers */
#include "debug.h"
#include "dso.h"
#include "map.h"
#include "namespaces.h"
#include "srcline.h"
#include "symbol.h"
#include "thread.h"
#include "vdso.h"

static inline int is_android_lib(const char *filename)
{
	return strstarts(filename, "/data/app-lib/") ||
	       strstarts(filename, "/system/lib/");
}

static inline bool replace_android_lib(const char *filename, char *newfilename)
{
	const char *libname;
	char *app_abi;
	size_t app_abi_length, new_length;
	size_t lib_length = 0;

	libname  = strrchr(filename, '/');
	if (libname)
		lib_length = strlen(libname);

	app_abi = getenv("APP_ABI");
	if (!app_abi)
		return false;

	app_abi_length = strlen(app_abi);

	if (strstarts(filename, "/data/app-lib/")) {
		char *apk_path;

		if (!app_abi_length)
			return false;

		new_length = 7 + app_abi_length + lib_length;

		apk_path = getenv("APK_PATH");
		if (apk_path) {
			new_length += strlen(apk_path) + 1;
			if (new_length > PATH_MAX)
				return false;
			snprintf(newfilename, new_length,
				 "%s/libs/%s/%s", apk_path, app_abi, libname);
		} else {
			if (new_length > PATH_MAX)
				return false;
			snprintf(newfilename, new_length,
				 "libs/%s/%s", app_abi, libname);
		}
		return true;
	}

	if (strstarts(filename, "/system/lib/")) {
		char *ndk, *app;
		const char *arch;
		int ndk_length, app_length;

		ndk = getenv("NDK_ROOT");
		app = getenv("APP_PLATFORM");

		if (!(ndk && app))
			return false;

		ndk_length = strlen(ndk);
		app_length = strlen(app);

		if (!(ndk_length && app_length && app_abi_length))
			return false;

		arch = !strncmp(app_abi, "arm", 3) ? "arm" :
		       !strncmp(app_abi, "mips", 4) ? "mips" :
		       !strncmp(app_abi, "x86", 3) ? "x86" : NULL;

		if (!arch)
			return false;

		new_length = 27 + ndk_length +
			     app_length + lib_length
			   + strlen(arch);

		if (new_length > PATH_MAX)
			return false;
		snprintf(newfilename, new_length,
			"%.*s/platforms/%.*s/arch-%s/usr/lib/%s",
			ndk_length, ndk, app_length, app, arch, libname);

		return true;
	}
	return false;
}

void map__init(struct map *map, u64 start, u64 end, u64 pgoff, struct dso *dso)
{
	map__set_start(map, start);
	map__set_end(map, end);
	map__set_pgoff(map, pgoff);
	map__set_reloc(map, 0);
	map__set_dso(map, dso__get(dso));
	map__set_mapping_type(map, MAPPING_TYPE__DSO);
	map__set_erange_warned(map, false);
	refcount_set(map__refcnt(map), 1);
}

struct map *map__new(struct machine *machine, u64 start, u64 len,
		     u64 pgoff, struct dso_id *id,
		     u32 prot, u32 flags, struct build_id *bid,
		     char *filename, struct thread *thread)
{
	struct map *result;
	RC_STRUCT(map) *map;
	struct nsinfo *nsi = NULL;
	struct nsinfo *nnsi;

	map = malloc(sizeof(*map));
	if (ADD_RC_CHK(result, map)) {
		char newfilename[PATH_MAX];
		struct dso *dso, *header_bid_dso;
		int anon, no_dso, vdso, android;

		android = is_android_lib(filename);
		anon = is_anon_memory(filename) || flags & MAP_HUGETLB;
		vdso = is_vdso_map(filename);
		no_dso = is_no_dso_memory(filename);
		map->prot = prot;
		map->flags = flags;
		nsi = nsinfo__get(thread__nsinfo(thread));

		if ((anon || no_dso) && nsi && (prot & PROT_EXEC)) {
			snprintf(newfilename, sizeof(newfilename),
				 "/tmp/perf-%d.map", nsinfo__pid(nsi));
			filename = newfilename;
		}

		if (android) {
			if (replace_android_lib(filename, newfilename))
				filename = newfilename;
		}

		if (vdso) {
			/* The vdso maps are always on the host and not the
			 * container.  Ensure that we don't use setns to look
			 * them up.
			 */
			nnsi = nsinfo__copy(nsi);
			if (nnsi) {
				nsinfo__put(nsi);
				nsinfo__clear_need_setns(nnsi);
				nsi = nnsi;
			}
			pgoff = 0;
			dso = machine__findnew_vdso(machine, thread);
		} else
			dso = machine__findnew_dso_id(machine, filename, id);

		if (dso == NULL)
			goto out_delete;

		assert(!dso->kernel);
		map__init(result, start, start + len, pgoff, dso);

		if (anon || no_dso) {
			map->mapping_type = MAPPING_TYPE__IDENTITY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * Set memory without DSO as loaded. All map__find_*
			 * functions still return NULL, and we avoid the
			 * unnecessary map__load warning.
			 */
<<<<<<< HEAD
			if (no_dso)
				dso__set_loaded(dso, self->type);
		}
	}
	return self;
out_delete:
	free(self);
	return NULL;
}

void map__delete(struct map *self)
{
	free(self);
}

void map__fixup_start(struct map *self)
{
	struct rb_root *symbols = &self->dso->symbols[self->type];
	struct rb_node *nd = rb_first(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		self->start = sym->start;
	}
}

void map__fixup_end(struct map *self)
{
	struct rb_root *symbols = &self->dso->symbols[self->type];
	struct rb_node *nd = rb_last(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		self->end = sym->end;
=======
			if (!(prot & PROT_EXEC))
				dso__set_loaded(dso);
		}
		mutex_lock(&dso->lock);
		nsinfo__put(dso->nsinfo);
		dso->nsinfo = nsi;
		mutex_unlock(&dso->lock);

		if (build_id__is_defined(bid)) {
			dso__set_build_id(dso, bid);
		} else {
			/*
			 * If the mmap event had no build ID, search for an existing dso from the
			 * build ID header by name. Otherwise only the dso loaded at the time of
			 * reading the header will have the build ID set and all future mmaps will
			 * have it missing.
			 */
			down_read(&machine->dsos.lock);
			header_bid_dso = __dsos__find(&machine->dsos, filename, false);
			up_read(&machine->dsos.lock);
			if (header_bid_dso && header_bid_dso->header_build_id) {
				dso__set_build_id(dso, &header_bid_dso->bid);
				dso->header_build_id = 1;
			}
		}
		dso__put(dso);
	}
	return result;
out_delete:
	nsinfo__put(nsi);
	RC_CHK_FREE(result);
	return NULL;
}

/*
 * Constructor variant for modules (where we know from /proc/modules where
 * they are loaded) and for vmlinux, where only after we load all the
 * symbols we'll know where it starts and ends.
 */
struct map *map__new2(u64 start, struct dso *dso)
{
	struct map *result;
	RC_STRUCT(map) *map;

	map = calloc(1, sizeof(*map) + (dso->kernel ? sizeof(struct kmap) : 0));
	if (ADD_RC_CHK(result, map)) {
		/*
		 * ->end will be filled after we load all the symbols
		 */
		map__init(result, start, 0, 0, dso);
	}

	return result;
}

bool __map__is_kernel(const struct map *map)
{
	if (!map__dso(map)->kernel)
		return false;
	return machine__kernel_map(maps__machine(map__kmaps((struct map *)map))) == map;
}

bool __map__is_extra_kernel_map(const struct map *map)
{
	struct kmap *kmap = __map__kmap((struct map *)map);

	return kmap && kmap->name[0];
}

bool __map__is_bpf_prog(const struct map *map)
{
	const char *name;
	struct dso *dso = map__dso(map);

	if (dso->binary_type == DSO_BINARY_TYPE__BPF_PROG_INFO)
		return true;

	/*
	 * If PERF_RECORD_BPF_EVENT is not included, the dso will not have
	 * type of DSO_BINARY_TYPE__BPF_PROG_INFO. In such cases, we can
	 * guess the type based on name.
	 */
	name = dso->short_name;
	return name && (strstr(name, "bpf_prog_") == name);
}

bool __map__is_bpf_image(const struct map *map)
{
	const char *name;
	struct dso *dso = map__dso(map);

	if (dso->binary_type == DSO_BINARY_TYPE__BPF_IMAGE)
		return true;

	/*
	 * If PERF_RECORD_KSYMBOL is not included, the dso will not have
	 * type of DSO_BINARY_TYPE__BPF_IMAGE. In such cases, we can
	 * guess the type based on name.
	 */
	name = dso->short_name;
	return name && is_bpf_image(name);
}

bool __map__is_ool(const struct map *map)
{
	const struct dso *dso = map__dso(map);

	return dso && dso->binary_type == DSO_BINARY_TYPE__OOL;
}

bool map__has_symbols(const struct map *map)
{
	return dso__has_symbols(map__dso(map));
}

static void map__exit(struct map *map)
{
	BUG_ON(refcount_read(map__refcnt(map)) != 0);
	dso__zput(RC_CHK_ACCESS(map)->dso);
}

void map__delete(struct map *map)
{
	map__exit(map);
	RC_CHK_FREE(map);
}

void map__put(struct map *map)
{
	if (map && refcount_dec_and_test(map__refcnt(map)))
		map__delete(map);
	else
		RC_CHK_PUT(map);
}

void map__fixup_start(struct map *map)
{
	struct dso *dso = map__dso(map);
	struct rb_root_cached *symbols = &dso->symbols;
	struct rb_node *nd = rb_first_cached(symbols);

	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);

		map__set_start(map, sym->start);
	}
}

void map__fixup_end(struct map *map)
{
	struct dso *dso = map__dso(map);
	struct rb_root_cached *symbols = &dso->symbols;
	struct rb_node *nd = rb_last(&symbols->rb_root);

	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		map__set_end(map, sym->end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

#define DSO__DELETED "(deleted)"

<<<<<<< HEAD
int map__load(struct map *self, symbol_filter_t filter)
{
	const char *name = self->dso->long_name;
	int nr;

	if (dso__loaded(self->dso, self->type))
		return 0;

	nr = dso__load(self->dso, self, filter);
	if (nr < 0) {
		if (self->dso->has_build_id) {
			char sbuild_id[BUILD_ID_SIZE * 2 + 1];

			build_id__sprintf(self->dso->build_id,
					  sizeof(self->dso->build_id),
					  sbuild_id);
			pr_warning("%s with build id %s not found",
				   name, sbuild_id);
		} else
			pr_warning("Failed to open %s", name);

		pr_warning(", continuing without symbols\n");
		return -1;
	} else if (nr == 0) {
=======
int map__load(struct map *map)
{
	struct dso *dso = map__dso(map);
	const char *name = dso->long_name;
	int nr;

	if (dso__loaded(dso))
		return 0;

	nr = dso__load(dso, map);
	if (nr < 0) {
		if (dso->has_build_id) {
			char sbuild_id[SBUILD_ID_SIZE];

			build_id__sprintf(&dso->bid, sbuild_id);
			pr_debug("%s with build id %s not found", name, sbuild_id);
		} else
			pr_debug("Failed to open %s", name);

		pr_debug(", continuing without symbols\n");
		return -1;
	} else if (nr == 0) {
#ifdef HAVE_LIBELF_SUPPORT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const size_t len = strlen(name);
		const size_t real_len = len - sizeof(DSO__DELETED);

		if (len > sizeof(DSO__DELETED) &&
		    strcmp(name + real_len + 1, DSO__DELETED) == 0) {
<<<<<<< HEAD
			pr_warning("%.*s was updated (is prelink enabled?). "
				"Restart the long running apps that use it!\n",
				   (int)real_len, name);
		} else {
			pr_warning("no symbols found in %s, maybe install "
				   "a debug package?\n", name);
		}

		return -1;
	}
	/*
	 * Only applies to the kernel, as its symtabs aren't relative like the
	 * module ones.
	 */
	if (self->dso->kernel)
		map__reloc_vmlinux(self);
=======
			pr_debug("%.*s was updated (is prelink enabled?). "
				"Restart the long running apps that use it!\n",
				   (int)real_len, name);
		} else {
			pr_debug("no symbols found in %s, maybe install a debug package?\n", name);
		}
#endif
		return -1;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
struct symbol *map__find_symbol(struct map *self, u64 addr,
				symbol_filter_t filter)
{
	if (map__load(self, filter) < 0)
		return NULL;

	return dso__find_symbol(self->dso, self->type, addr);
}

struct symbol *map__find_symbol_by_name(struct map *self, const char *name,
					symbol_filter_t filter)
{
	if (map__load(self, filter) < 0)
		return NULL;

	if (!dso__sorted_by_name(self->dso, self->type))
		dso__sort_by_name(self->dso, self->type);

	return dso__find_symbol_by_name(self->dso, self->type, name);
}

struct map *map__clone(struct map *self)
{
	struct map *map = malloc(sizeof(*self));

	if (!map)
		return NULL;

	memcpy(map, self, sizeof(*self));

	return map;
}

int map__overlap(struct map *l, struct map *r)
{
	if (l->start > r->start) {
		struct map *t = l;
		l = r;
		r = t;
	}

	if (l->end > r->start)
		return 1;

	return 0;
}

size_t map__fprintf(struct map *self, FILE *fp)
{
	return fprintf(fp, " %" PRIx64 "-%" PRIx64 " %" PRIx64 " %s\n",
		       self->start, self->end, self->pgoff, self->dso->name);
}

size_t map__fprintf_dsoname(struct map *map, FILE *fp)
{
	const char *dsoname;

	if (map && map->dso && (map->dso->name || map->dso->long_name)) {
		if (symbol_conf.show_kernel_path && map->dso->long_name)
			dsoname = map->dso->long_name;
		else if (map->dso->name)
			dsoname = map->dso->name;
	} else
		dsoname = "[unknown]";
=======
struct symbol *map__find_symbol(struct map *map, u64 addr)
{
	if (map__load(map) < 0)
		return NULL;

	return dso__find_symbol(map__dso(map), addr);
}

struct symbol *map__find_symbol_by_name_idx(struct map *map, const char *name, size_t *idx)
{
	struct dso *dso;

	if (map__load(map) < 0)
		return NULL;

	dso = map__dso(map);
	dso__sort_by_name(dso);

	return dso__find_symbol_by_name(dso, name, idx);
}

struct symbol *map__find_symbol_by_name(struct map *map, const char *name)
{
	size_t idx;

	return map__find_symbol_by_name_idx(map, name, &idx);
}

struct map *map__clone(struct map *from)
{
	struct map *result;
	RC_STRUCT(map) *map;
	size_t size = sizeof(RC_STRUCT(map));
	struct dso *dso = map__dso(from);

	if (dso && dso->kernel)
		size += sizeof(struct kmap);

	map = memdup(RC_CHK_ACCESS(from), size);
	if (ADD_RC_CHK(result, map)) {
		refcount_set(&map->refcnt, 1);
		map->dso = dso__get(dso);
	}

	return result;
}

size_t map__fprintf(struct map *map, FILE *fp)
{
	const struct dso *dso = map__dso(map);

	return fprintf(fp, " %" PRIx64 "-%" PRIx64 " %" PRIx64 " %s\n",
		       map__start(map), map__end(map), map__pgoff(map), dso->name);
}

static bool prefer_dso_long_name(const struct dso *dso, bool print_off)
{
	return dso->long_name &&
	       (symbol_conf.show_kernel_path ||
		(print_off && (dso->name[0] == '[' || dso__is_kcore(dso))));
}

static size_t __map__fprintf_dsoname(struct map *map, bool print_off, FILE *fp)
{
	char buf[symbol_conf.pad_output_len_dso + 1];
	const char *dsoname = "[unknown]";
	const struct dso *dso = map ? map__dso(map) : NULL;

	if (dso) {
		if (prefer_dso_long_name(dso, print_off))
			dsoname = dso->long_name;
		else
			dsoname = dso->name;
	}

	if (symbol_conf.pad_output_len_dso) {
		scnprintf_pad(buf, symbol_conf.pad_output_len_dso, "%s", dsoname);
		dsoname = buf;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return fprintf(fp, "%s", dsoname);
}

<<<<<<< HEAD
/*
 * objdump wants/reports absolute IPs for ET_EXEC, and RIPs for ET_DYN.
 * map->dso->adjust_symbols==1 for ET_EXEC-like cases.
 */
u64 map__rip_2objdump(struct map *map, u64 rip)
{
	u64 addr = map->dso->adjust_symbols ?
			map->unmap_ip(map, rip) :	/* RIP -> IP */
			rip;
	return addr;
}

u64 map__objdump_2ip(struct map *map, u64 addr)
{
	u64 ip = map->dso->adjust_symbols ?
			addr :
			map->unmap_ip(map, addr);	/* RIP -> IP */
	return ip;
}

void map_groups__init(struct map_groups *mg)
{
	int i;
	for (i = 0; i < MAP__NR_TYPES; ++i) {
		mg->maps[i] = RB_ROOT;
		INIT_LIST_HEAD(&mg->removed_maps[i]);
	}
	mg->machine = NULL;
}

static void maps__delete(struct rb_root *maps)
{
	struct rb_node *next = rb_first(maps);

	while (next) {
		struct map *pos = rb_entry(next, struct map, rb_node);

		next = rb_next(&pos->rb_node);
		rb_erase(&pos->rb_node, maps);
		map__delete(pos);
	}
}

static void maps__delete_removed(struct list_head *maps)
{
	struct map *pos, *n;

	list_for_each_entry_safe(pos, n, maps, node) {
		list_del(&pos->node);
		map__delete(pos);
	}
}

void map_groups__exit(struct map_groups *mg)
{
	int i;

	for (i = 0; i < MAP__NR_TYPES; ++i) {
		maps__delete(&mg->maps[i]);
		maps__delete_removed(&mg->removed_maps[i]);
	}
}

void map_groups__flush(struct map_groups *mg)
{
	int type;

	for (type = 0; type < MAP__NR_TYPES; type++) {
		struct rb_root *root = &mg->maps[type];
		struct rb_node *next = rb_first(root);

		while (next) {
			struct map *pos = rb_entry(next, struct map, rb_node);
			next = rb_next(&pos->rb_node);
			rb_erase(&pos->rb_node, root);
			/*
			 * We may have references to this map, for
			 * instance in some hist_entry instances, so
			 * just move them to a separate list.
			 */
			list_add_tail(&pos->node, &mg->removed_maps[pos->type]);
		}
	}
}

struct symbol *map_groups__find_symbol(struct map_groups *mg,
				       enum map_type type, u64 addr,
				       struct map **mapp,
				       symbol_filter_t filter)
{
	struct map *map = map_groups__find(mg, type, addr);

	if (map != NULL) {
		if (mapp != NULL)
			*mapp = map;
		return map__find_symbol(map, map->map_ip(map, addr), filter);
	}

	return NULL;
}

struct symbol *map_groups__find_symbol_by_name(struct map_groups *mg,
					       enum map_type type,
					       const char *name,
					       struct map **mapp,
					       symbol_filter_t filter)
{
	struct rb_node *nd;

	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
		struct map *pos = rb_entry(nd, struct map, rb_node);
		struct symbol *sym = map__find_symbol_by_name(pos, name, filter);

		if (sym == NULL)
			continue;
		if (mapp != NULL)
			*mapp = pos;
		return sym;
	}

	return NULL;
}

size_t __map_groups__fprintf_maps(struct map_groups *mg,
				  enum map_type type, int verbose, FILE *fp)
{
	size_t printed = fprintf(fp, "%s:\n", map_type__name[type]);
	struct rb_node *nd;

	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
		struct map *pos = rb_entry(nd, struct map, rb_node);
		printed += fprintf(fp, "Map:");
		printed += map__fprintf(pos, fp);
		if (verbose > 2) {
			printed += dso__fprintf(pos->dso, type, fp);
			printed += fprintf(fp, "--\n");
		}
	}

	return printed;
}

size_t map_groups__fprintf_maps(struct map_groups *mg, int verbose, FILE *fp)
{
	size_t printed = 0, i;
	for (i = 0; i < MAP__NR_TYPES; ++i)
		printed += __map_groups__fprintf_maps(mg, i, verbose, fp);
	return printed;
}

static size_t __map_groups__fprintf_removed_maps(struct map_groups *mg,
						 enum map_type type,
						 int verbose, FILE *fp)
{
	struct map *pos;
	size_t printed = 0;

	list_for_each_entry(pos, &mg->removed_maps[type], node) {
		printed += fprintf(fp, "Map:");
		printed += map__fprintf(pos, fp);
		if (verbose > 1) {
			printed += dso__fprintf(pos->dso, type, fp);
			printed += fprintf(fp, "--\n");
		}
	}
	return printed;
}

static size_t map_groups__fprintf_removed_maps(struct map_groups *mg,
					       int verbose, FILE *fp)
{
	size_t printed = 0, i;
	for (i = 0; i < MAP__NR_TYPES; ++i)
		printed += __map_groups__fprintf_removed_maps(mg, i, verbose, fp);
	return printed;
}

size_t map_groups__fprintf(struct map_groups *mg, int verbose, FILE *fp)
{
	size_t printed = map_groups__fprintf_maps(mg, verbose, fp);
	printed += fprintf(fp, "Removed maps:\n");
	return printed + map_groups__fprintf_removed_maps(mg, verbose, fp);
}

int map_groups__fixup_overlappings(struct map_groups *mg, struct map *map,
				   int verbose, FILE *fp)
{
	struct rb_root *root = &mg->maps[map->type];
	struct rb_node *next = rb_first(root);
	int err = 0;

	while (next) {
		struct map *pos = rb_entry(next, struct map, rb_node);
		next = rb_next(&pos->rb_node);

		if (!map__overlap(pos, map))
			continue;

		if (verbose >= 2) {
			fputs("overlapping maps:\n", fp);
			map__fprintf(map, fp);
			map__fprintf(pos, fp);
		}

		rb_erase(&pos->rb_node, root);
		/*
		 * Now check if we need to create new maps for areas not
		 * overlapped by the new map:
		 */
		if (map->start > pos->start) {
			struct map *before = map__clone(pos);

			if (before == NULL) {
				err = -ENOMEM;
				goto move_map;
			}

			before->end = map->start - 1;
			map_groups__insert(mg, before);
			if (verbose >= 2)
				map__fprintf(before, fp);
		}

		if (map->end < pos->end) {
			struct map *after = map__clone(pos);

			if (after == NULL) {
				err = -ENOMEM;
				goto move_map;
			}

			after->start = map->end + 1;
			map_groups__insert(mg, after);
			if (verbose >= 2)
				map__fprintf(after, fp);
		}
move_map:
		/*
		 * If we have references, just move them to a separate list.
		 */
		if (pos->referenced)
			list_add_tail(&pos->node, &mg->removed_maps[map->type]);
		else
			map__delete(pos);

		if (err)
			return err;
	}

	return 0;
}

/*
 * XXX This should not really _copy_ te maps, but refcount them.
 */
int map_groups__clone(struct map_groups *mg,
		      struct map_groups *parent, enum map_type type)
{
	struct rb_node *nd;
	for (nd = rb_first(&parent->maps[type]); nd; nd = rb_next(nd)) {
		struct map *map = rb_entry(nd, struct map, rb_node);
		struct map *new = map__clone(map);
		if (new == NULL)
			return -ENOMEM;
		map_groups__insert(mg, new);
	}
	return 0;
}

static u64 map__reloc_map_ip(struct map *map, u64 ip)
{
	return ip + (s64)map->pgoff;
}

static u64 map__reloc_unmap_ip(struct map *map, u64 ip)
{
	return ip - (s64)map->pgoff;
}

void map__reloc_vmlinux(struct map *self)
{
	struct kmap *kmap = map__kmap(self);
	s64 reloc;

	if (!kmap->ref_reloc_sym || !kmap->ref_reloc_sym->unrelocated_addr)
		return;

	reloc = (kmap->ref_reloc_sym->unrelocated_addr -
		 kmap->ref_reloc_sym->addr);

	if (!reloc)
		return;

	self->map_ip   = map__reloc_map_ip;
	self->unmap_ip = map__reloc_unmap_ip;
	self->pgoff    = reloc;
}

void maps__insert(struct rb_root *maps, struct map *map)
{
	struct rb_node **p = &maps->rb_node;
	struct rb_node *parent = NULL;
	const u64 ip = map->start;
	struct map *m;

	while (*p != NULL) {
		parent = *p;
		m = rb_entry(parent, struct map, rb_node);
		if (ip < m->start)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&map->rb_node, parent, p);
	rb_insert_color(&map->rb_node, maps);
}

void maps__remove(struct rb_root *self, struct map *map)
{
	rb_erase(&map->rb_node, self);
}

struct map *maps__find(struct rb_root *maps, u64 ip)
{
	struct rb_node **p = &maps->rb_node;
	struct rb_node *parent = NULL;
	struct map *m;

	while (*p != NULL) {
		parent = *p;
		m = rb_entry(parent, struct map, rb_node);
		if (ip < m->start)
			p = &(*p)->rb_left;
		else if (ip > m->end)
			p = &(*p)->rb_right;
		else
			return m;
	}

	return NULL;
}

int machine__init(struct machine *self, const char *root_dir, pid_t pid)
{
	map_groups__init(&self->kmaps);
	RB_CLEAR_NODE(&self->rb_node);
	INIT_LIST_HEAD(&self->user_dsos);
	INIT_LIST_HEAD(&self->kernel_dsos);

	self->threads = RB_ROOT;
	INIT_LIST_HEAD(&self->dead_threads);
	self->last_match = NULL;

	self->kmaps.machine = self;
	self->pid	    = pid;
	self->root_dir      = strdup(root_dir);
	return self->root_dir == NULL ? -ENOMEM : 0;
}

static void dsos__delete(struct list_head *self)
{
	struct dso *pos, *n;

	list_for_each_entry_safe(pos, n, self, node) {
		list_del(&pos->node);
		dso__delete(pos);
	}
}

void machine__exit(struct machine *self)
{
	map_groups__exit(&self->kmaps);
	dsos__delete(&self->user_dsos);
	dsos__delete(&self->kernel_dsos);
	free(self->root_dir);
	self->root_dir = NULL;
}

void machine__delete(struct machine *self)
{
	machine__exit(self);
	free(self);
}

struct machine *machines__add(struct rb_root *self, pid_t pid,
			      const char *root_dir)
{
	struct rb_node **p = &self->rb_node;
	struct rb_node *parent = NULL;
	struct machine *pos, *machine = malloc(sizeof(*machine));

	if (!machine)
		return NULL;

	if (machine__init(machine, root_dir, pid) != 0) {
		free(machine);
		return NULL;
	}

	while (*p != NULL) {
		parent = *p;
		pos = rb_entry(parent, struct machine, rb_node);
		if (pid < pos->pid)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&machine->rb_node, parent, p);
	rb_insert_color(&machine->rb_node, self);

	return machine;
}

struct machine *machines__find(struct rb_root *self, pid_t pid)
{
	struct rb_node **p = &self->rb_node;
	struct rb_node *parent = NULL;
	struct machine *machine;
	struct machine *default_machine = NULL;

	while (*p != NULL) {
		parent = *p;
		machine = rb_entry(parent, struct machine, rb_node);
		if (pid < machine->pid)
			p = &(*p)->rb_left;
		else if (pid > machine->pid)
			p = &(*p)->rb_right;
		else
			return machine;
		if (!machine->pid)
			default_machine = machine;
	}

	return default_machine;
}

struct machine *machines__findnew(struct rb_root *self, pid_t pid)
{
	char path[PATH_MAX];
	const char *root_dir;
	struct machine *machine = machines__find(self, pid);

	if (!machine || machine->pid != pid) {
		if (pid == HOST_KERNEL_ID || pid == DEFAULT_GUEST_KERNEL_ID)
			root_dir = "";
		else {
			if (!symbol_conf.guestmount)
				goto out;
			sprintf(path, "%s/%d", symbol_conf.guestmount, pid);
			if (access(path, R_OK)) {
				pr_err("Can't access file %s\n", path);
				goto out;
			}
			root_dir = path;
		}
		machine = machines__add(self, pid, root_dir);
	}

out:
	return machine;
}

void machines__process(struct rb_root *self, machine__process_t process, void *data)
{
	struct rb_node *nd;

	for (nd = rb_first(self); nd; nd = rb_next(nd)) {
		struct machine *pos = rb_entry(nd, struct machine, rb_node);
		process(pos, data);
	}
}

char *machine__mmap_name(struct machine *self, char *bf, size_t size)
{
	if (machine__is_host(self))
		snprintf(bf, size, "[%s]", "kernel.kallsyms");
	else if (machine__is_default_guest(self))
		snprintf(bf, size, "[%s]", "guest.kernel.kallsyms");
	else
		snprintf(bf, size, "[%s.%d]", "guest.kernel.kallsyms", self->pid);

	return bf;
=======
size_t map__fprintf_dsoname(struct map *map, FILE *fp)
{
	return __map__fprintf_dsoname(map, false, fp);
}

size_t map__fprintf_dsoname_dsoff(struct map *map, bool print_off, u64 addr, FILE *fp)
{
	const struct dso *dso = map ? map__dso(map) : NULL;
	int printed = 0;

	if (print_off && (!dso || !dso__is_object_file(dso)))
		print_off = false;
	printed += fprintf(fp, " (");
	printed += __map__fprintf_dsoname(map, print_off, fp);
	if (print_off)
		printed += fprintf(fp, "+0x%" PRIx64, addr);
	printed += fprintf(fp, ")");

	return printed;
}

char *map__srcline(struct map *map, u64 addr, struct symbol *sym)
{
	if (map == NULL)
		return SRCLINE_UNKNOWN;

	return get_srcline(map__dso(map), map__rip_2objdump(map, addr), sym, true, true, addr);
}

int map__fprintf_srcline(struct map *map, u64 addr, const char *prefix,
			 FILE *fp)
{
	const struct dso *dso = map ? map__dso(map) : NULL;
	int ret = 0;

	if (dso) {
		char *srcline = map__srcline(map, addr, NULL);
		if (srcline != SRCLINE_UNKNOWN)
			ret = fprintf(fp, "%s%s", prefix, srcline);
		zfree_srcline(&srcline);
	}
	return ret;
}

void srccode_state_free(struct srccode_state *state)
{
	zfree(&state->srcfile);
	state->line = 0;
}

/**
 * map__rip_2objdump - convert symbol start address to objdump address.
 * @map: memory map
 * @rip: symbol start address
 *
 * objdump wants/reports absolute IPs for ET_EXEC, and RIPs for ET_DYN.
 * map->dso->adjust_symbols==1 for ET_EXEC-like cases except ET_REL which is
 * relative to section start.
 *
 * Return: Address suitable for passing to "objdump --start-address="
 */
u64 map__rip_2objdump(struct map *map, u64 rip)
{
	struct kmap *kmap = __map__kmap(map);
	const struct dso *dso = map__dso(map);

	/*
	 * vmlinux does not have program headers for PTI entry trampolines and
	 * kcore may not either. However the trampoline object code is on the
	 * main kernel map, so just use that instead.
	 */
	if (kmap && is_entry_trampoline(kmap->name) && kmap->kmaps) {
		struct machine *machine = maps__machine(kmap->kmaps);

		if (machine) {
			struct map *kernel_map = machine__kernel_map(machine);

			if (kernel_map)
				map = kernel_map;
		}
	}

	if (!dso->adjust_symbols)
		return rip;

	if (dso->rel)
		return rip - map__pgoff(map);

	if (dso->kernel == DSO_SPACE__USER)
		return rip + dso->text_offset;

	return map__unmap_ip(map, rip) - map__reloc(map);
}

/**
 * map__objdump_2mem - convert objdump address to a memory address.
 * @map: memory map
 * @ip: objdump address
 *
 * Closely related to map__rip_2objdump(), this function takes an address from
 * objdump and converts it to a memory address.  Note this assumes that @map
 * contains the address.  To be sure the result is valid, check it forwards
 * e.g. map__rip_2objdump(map__map_ip(map, map__objdump_2mem(map, ip))) == ip
 *
 * Return: Memory address.
 */
u64 map__objdump_2mem(struct map *map, u64 ip)
{
	const struct dso *dso = map__dso(map);

	if (!dso->adjust_symbols)
		return map__unmap_ip(map, ip);

	if (dso->rel)
		return map__unmap_ip(map, ip + map__pgoff(map));

	if (dso->kernel == DSO_SPACE__USER)
		return map__unmap_ip(map, ip - dso->text_offset);

	return ip + map__reloc(map);
}

bool map__contains_symbol(const struct map *map, const struct symbol *sym)
{
	u64 ip = map__unmap_ip(map, sym->start);

	return ip >= map__start(map) && ip < map__end(map);
}

struct kmap *__map__kmap(struct map *map)
{
	const struct dso *dso = map__dso(map);

	if (!dso || !dso->kernel)
		return NULL;
	return (struct kmap *)(&RC_CHK_ACCESS(map)[1]);
}

struct kmap *map__kmap(struct map *map)
{
	struct kmap *kmap = __map__kmap(map);

	if (!kmap)
		pr_err("Internal error: map__kmap with a non-kernel map\n");
	return kmap;
}

struct maps *map__kmaps(struct map *map)
{
	struct kmap *kmap = map__kmap(map);

	if (!kmap || !kmap->kmaps) {
		pr_err("Internal error: map__kmaps with a non-kernel map\n");
		return NULL;
	}
	return kmap->kmaps;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
