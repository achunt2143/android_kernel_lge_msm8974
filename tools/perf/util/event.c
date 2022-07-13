<<<<<<< HEAD
#include <linux/types.h>
#include "event.h"
#include "debug.h"
#include "sort.h"
#include "string.h"
#include "strlist.h"
#include "thread.h"
#include "thread_map.h"
=======
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <perf/cpumap.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <uapi/linux/mman.h> /* To get things like MAP_HUGETLB even on older libc headers */
#include <linux/perf_event.h>
#include <linux/zalloc.h>
#include "cpumap.h"
#include "dso.h"
#include "event.h"
#include "debug.h"
#include "hist.h"
#include "machine.h"
#include "sort.h"
#include "string2.h"
#include "strlist.h"
#include "thread.h"
#include "thread_map.h"
#include "time-utils.h"
#include <linux/ctype.h>
#include "map.h"
#include "util/namespaces.h"
#include "symbol.h"
#include "symbol/kallsyms.h"
#include "asm/bug.h"
#include "stat.h"
#include "session.h"
#include "bpf-event.h"
#include "print_binary.h"
#include "tool.h"
#include "util.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const char *perf_event__names[] = {
	[0]					= "TOTAL",
	[PERF_RECORD_MMAP]			= "MMAP",
<<<<<<< HEAD
=======
	[PERF_RECORD_MMAP2]			= "MMAP2",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[PERF_RECORD_LOST]			= "LOST",
	[PERF_RECORD_COMM]			= "COMM",
	[PERF_RECORD_EXIT]			= "EXIT",
	[PERF_RECORD_THROTTLE]			= "THROTTLE",
	[PERF_RECORD_UNTHROTTLE]		= "UNTHROTTLE",
	[PERF_RECORD_FORK]			= "FORK",
	[PERF_RECORD_READ]			= "READ",
	[PERF_RECORD_SAMPLE]			= "SAMPLE",
<<<<<<< HEAD
=======
	[PERF_RECORD_AUX]			= "AUX",
	[PERF_RECORD_ITRACE_START]		= "ITRACE_START",
	[PERF_RECORD_LOST_SAMPLES]		= "LOST_SAMPLES",
	[PERF_RECORD_SWITCH]			= "SWITCH",
	[PERF_RECORD_SWITCH_CPU_WIDE]		= "SWITCH_CPU_WIDE",
	[PERF_RECORD_NAMESPACES]		= "NAMESPACES",
	[PERF_RECORD_KSYMBOL]			= "KSYMBOL",
	[PERF_RECORD_BPF_EVENT]			= "BPF_EVENT",
	[PERF_RECORD_CGROUP]			= "CGROUP",
	[PERF_RECORD_TEXT_POKE]			= "TEXT_POKE",
	[PERF_RECORD_AUX_OUTPUT_HW_ID]		= "AUX_OUTPUT_HW_ID",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[PERF_RECORD_HEADER_ATTR]		= "ATTR",
	[PERF_RECORD_HEADER_EVENT_TYPE]		= "EVENT_TYPE",
	[PERF_RECORD_HEADER_TRACING_DATA]	= "TRACING_DATA",
	[PERF_RECORD_HEADER_BUILD_ID]		= "BUILD_ID",
	[PERF_RECORD_FINISHED_ROUND]		= "FINISHED_ROUND",
<<<<<<< HEAD
=======
	[PERF_RECORD_ID_INDEX]			= "ID_INDEX",
	[PERF_RECORD_AUXTRACE_INFO]		= "AUXTRACE_INFO",
	[PERF_RECORD_AUXTRACE]			= "AUXTRACE",
	[PERF_RECORD_AUXTRACE_ERROR]		= "AUXTRACE_ERROR",
	[PERF_RECORD_THREAD_MAP]		= "THREAD_MAP",
	[PERF_RECORD_CPU_MAP]			= "CPU_MAP",
	[PERF_RECORD_STAT_CONFIG]		= "STAT_CONFIG",
	[PERF_RECORD_STAT]			= "STAT",
	[PERF_RECORD_STAT_ROUND]		= "STAT_ROUND",
	[PERF_RECORD_EVENT_UPDATE]		= "EVENT_UPDATE",
	[PERF_RECORD_TIME_CONV]			= "TIME_CONV",
	[PERF_RECORD_HEADER_FEATURE]		= "FEATURE",
	[PERF_RECORD_COMPRESSED]		= "COMPRESSED",
	[PERF_RECORD_FINISHED_INIT]		= "FINISHED_INIT",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const char *perf_event__name(unsigned int id)
{
	if (id >= ARRAY_SIZE(perf_event__names))
		return "INVALID";
	if (!perf_event__names[id])
		return "UNKNOWN";
	return perf_event__names[id];
}

<<<<<<< HEAD
static struct perf_sample synth_sample = {
	.pid	   = -1,
	.tid	   = -1,
	.time	   = -1,
	.stream_id = -1,
	.cpu	   = -1,
	.period	   = 1,
};

static pid_t perf_event__get_comm_tgid(pid_t pid, char *comm, size_t len)
{
	char filename[PATH_MAX];
	char bf[BUFSIZ];
	FILE *fp;
	size_t size = 0;
	pid_t tgid = -1;

	snprintf(filename, sizeof(filename), "/proc/%d/status", pid);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		pr_debug("couldn't open %s\n", filename);
		return 0;
	}

	while (!comm[0] || (tgid < 0)) {
		if (fgets(bf, sizeof(bf), fp) == NULL) {
			pr_warning("couldn't get COMM and pgid, malformed %s\n",
				   filename);
			break;
		}

		if (memcmp(bf, "Name:", 5) == 0) {
			char *name = bf + 5;
			while (*name && isspace(*name))
				++name;
			size = strlen(name) - 1;
			if (size >= len)
				size = len - 1;
			memcpy(comm, name, size);
			comm[size] = '\0';

		} else if (memcmp(bf, "Tgid:", 5) == 0) {
			char *tgids = bf + 5;
			while (*tgids && isspace(*tgids))
				++tgids;
			tgid = atoi(tgids);
		}
	}

	fclose(fp);

	return tgid;
}

static pid_t perf_event__synthesize_comm(struct perf_tool *tool,
					 union perf_event *event, pid_t pid,
					 int full,
					 perf_event__handler_t process,
					 struct machine *machine)
{
	char filename[PATH_MAX];
	size_t size;
	DIR *tasks;
	struct dirent dirent, *next;
	pid_t tgid;

	memset(&event->comm, 0, sizeof(event->comm));

	tgid = perf_event__get_comm_tgid(pid, event->comm.comm,
					 sizeof(event->comm.comm));
	if (tgid < 0)
		goto out;

	event->comm.pid = tgid;
	event->comm.header.type = PERF_RECORD_COMM;

	size = strlen(event->comm.comm) + 1;
	size = ALIGN(size, sizeof(u64));
	memset(event->comm.comm + size, 0, machine->id_hdr_size);
	event->comm.header.size = (sizeof(event->comm) -
				(sizeof(event->comm.comm) - size) +
				machine->id_hdr_size);
	if (!full) {
		event->comm.tid = pid;

		process(tool, event, &synth_sample, machine);
		goto out;
	}

	snprintf(filename, sizeof(filename), "/proc/%d/task", pid);

	tasks = opendir(filename);
	if (tasks == NULL) {
		pr_debug("couldn't open %s\n", filename);
		return 0;
	}

	while (!readdir_r(tasks, &dirent, &next) && next) {
		char *end;
		pid = strtol(dirent.d_name, &end, 10);
		if (*end)
			continue;

		/* already have tgid; jut want to update the comm */
		(void) perf_event__get_comm_tgid(pid, event->comm.comm,
					 sizeof(event->comm.comm));

		size = strlen(event->comm.comm) + 1;
		size = ALIGN(size, sizeof(u64));
		memset(event->comm.comm + size, 0, machine->id_hdr_size);
		event->comm.header.size = (sizeof(event->comm) -
					  (sizeof(event->comm.comm) - size) +
					  machine->id_hdr_size);

		event->comm.tid = pid;

		process(tool, event, &synth_sample, machine);
	}

	closedir(tasks);
out:
	return tgid;
}

static int perf_event__synthesize_mmap_events(struct perf_tool *tool,
					      union perf_event *event,
					      pid_t pid, pid_t tgid,
					      perf_event__handler_t process,
					      struct machine *machine)
{
	char filename[PATH_MAX];
	FILE *fp;

	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		/*
		 * We raced with a task exiting - just return:
		 */
		pr_debug("couldn't open %s\n", filename);
		return -1;
	}

	event->header.type = PERF_RECORD_MMAP;
	/*
	 * Just like the kernel, see __perf_event_mmap in kernel/perf_event.c
	 */
	event->header.misc = PERF_RECORD_MISC_USER;

	while (1) {
		char bf[BUFSIZ], *pbf = bf;
		int n;
		size_t size;
		if (fgets(bf, sizeof(bf), fp) == NULL)
			break;

		/* 00400000-0040c000 r-xp 00000000 fd:01 41038  /bin/cat */
		n = hex2u64(pbf, &event->mmap.start);
		if (n < 0)
			continue;
		pbf += n + 1;
		n = hex2u64(pbf, &event->mmap.len);
		if (n < 0)
			continue;
		pbf += n + 3;
		if (*pbf == 'x') { /* vm_exec */
			char anonstr[] = "//anon\n";
			char *execname = strchr(bf, '/');

			/* Catch VDSO */
			if (execname == NULL)
				execname = strstr(bf, "[vdso]");

			/* Catch anonymous mmaps */
			if ((execname == NULL) && !strstr(bf, "["))
				execname = anonstr;

			if (execname == NULL)
				continue;

			pbf += 3;
			n = hex2u64(pbf, &event->mmap.pgoff);

			size = strlen(execname);
			execname[size - 1] = '\0'; /* Remove \n */
			memcpy(event->mmap.filename, execname, size);
			size = ALIGN(size, sizeof(u64));
			event->mmap.len -= event->mmap.start;
			event->mmap.header.size = (sizeof(event->mmap) -
					        (sizeof(event->mmap.filename) - size));
			memset(event->mmap.filename + size, 0, machine->id_hdr_size);
			event->mmap.header.size += machine->id_hdr_size;
			event->mmap.pid = tgid;
			event->mmap.tid = pid;

			process(tool, event, &synth_sample, machine);
		}
	}

	fclose(fp);
	return 0;
}

int perf_event__synthesize_modules(struct perf_tool *tool,
				   perf_event__handler_t process,
				   struct machine *machine)
{
	struct rb_node *nd;
	struct map_groups *kmaps = &machine->kmaps;
	union perf_event *event = zalloc((sizeof(event->mmap) +
					  machine->id_hdr_size));
	if (event == NULL) {
		pr_debug("Not enough memory synthesizing mmap event "
			 "for kernel modules\n");
		return -1;
	}

	event->header.type = PERF_RECORD_MMAP;

	/*
	 * kernel uses 0 for user space maps, see kernel/perf_event.c
	 * __perf_event_mmap
	 */
	if (machine__is_host(machine))
		event->header.misc = PERF_RECORD_MISC_KERNEL;
	else
		event->header.misc = PERF_RECORD_MISC_GUEST_KERNEL;

	for (nd = rb_first(&kmaps->maps[MAP__FUNCTION]);
	     nd; nd = rb_next(nd)) {
		size_t size;
		struct map *pos = rb_entry(nd, struct map, rb_node);

		if (pos->dso->kernel)
			continue;

		size = ALIGN(pos->dso->long_name_len + 1, sizeof(u64));
		event->mmap.header.type = PERF_RECORD_MMAP;
		event->mmap.header.size = (sizeof(event->mmap) -
				        (sizeof(event->mmap.filename) - size));
		memset(event->mmap.filename + size, 0, machine->id_hdr_size);
		event->mmap.header.size += machine->id_hdr_size;
		event->mmap.start = pos->start;
		event->mmap.len   = pos->end - pos->start;
		event->mmap.pid   = machine->pid;

		memcpy(event->mmap.filename, pos->dso->long_name,
		       pos->dso->long_name_len + 1);
		process(tool, event, &synth_sample, machine);
	}

	free(event);
	return 0;
}

static int __event__synthesize_thread(union perf_event *comm_event,
				      union perf_event *mmap_event,
				      pid_t pid, int full,
					  perf_event__handler_t process,
				      struct perf_tool *tool,
				      struct machine *machine)
{
	pid_t tgid = perf_event__synthesize_comm(tool, comm_event, pid, full,
						 process, machine);
	if (tgid == -1)
		return -1;
	return perf_event__synthesize_mmap_events(tool, mmap_event, pid, tgid,
						  process, machine);
}

int perf_event__synthesize_thread_map(struct perf_tool *tool,
				      struct thread_map *threads,
				      perf_event__handler_t process,
				      struct machine *machine)
{
	union perf_event *comm_event, *mmap_event;
	int err = -1, thread, j;

	comm_event = malloc(sizeof(comm_event->comm) + machine->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + machine->id_hdr_size);
	if (mmap_event == NULL)
		goto out_free_comm;

	err = 0;
	for (thread = 0; thread < threads->nr; ++thread) {
		if (__event__synthesize_thread(comm_event, mmap_event,
					       threads->map[thread], 0,
					       process, tool, machine)) {
			err = -1;
			break;
		}

		/*
		 * comm.pid is set to thread group id by
		 * perf_event__synthesize_comm
		 */
		if ((int) comm_event->comm.pid != threads->map[thread]) {
			bool need_leader = true;

			/* is thread group leader in thread_map? */
			for (j = 0; j < threads->nr; ++j) {
				if ((int) comm_event->comm.pid == threads->map[j]) {
					need_leader = false;
					break;
				}
			}

			/* if not, generate events for it */
			if (need_leader &&
			    __event__synthesize_thread(comm_event,
						      mmap_event,
						      comm_event->comm.pid, 0,
						      process, tool, machine)) {
				err = -1;
				break;
			}
		}
	}
	free(mmap_event);
out_free_comm:
	free(comm_event);
out:
	return err;
}

int perf_event__synthesize_threads(struct perf_tool *tool,
				   perf_event__handler_t process,
				   struct machine *machine)
{
	DIR *proc;
	struct dirent dirent, *next;
	union perf_event *comm_event, *mmap_event;
	int err = -1;

	comm_event = malloc(sizeof(comm_event->comm) + machine->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + machine->id_hdr_size);
	if (mmap_event == NULL)
		goto out_free_comm;

	proc = opendir("/proc");
	if (proc == NULL)
		goto out_free_mmap;

	while (!readdir_r(proc, &dirent, &next) && next) {
		char *end;
		pid_t pid = strtol(dirent.d_name, &end, 10);

		if (*end) /* only interested in proper numerical dirents */
			continue;

		__event__synthesize_thread(comm_event, mmap_event, pid, 1,
					   process, tool, machine);
	}

	closedir(proc);
	err = 0;
out_free_mmap:
	free(mmap_event);
out_free_comm:
	free(comm_event);
out:
	return err;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct process_symbol_args {
	const char *name;
	u64	   start;
};

<<<<<<< HEAD
static int find_symbol_cb(void *arg, const char *name, char type,
			  u64 start, u64 end __used)
=======
static int find_func_symbol_cb(void *arg, const char *name, char type,
			       u64 start)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct process_symbol_args *args = arg;

	/*
	 * Must be a function or at least an alias, as in PARISC64, where "_text" is
	 * an 'A' to the same address as "_stext".
	 */
<<<<<<< HEAD
	if (!(symbol_type__is_a(type, MAP__FUNCTION) ||
=======
	if (!(kallsyms__is_function(type) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	      type == 'A') || strcmp(name, args->name))
		return 0;

	args->start = start;
	return 1;
}

<<<<<<< HEAD
int perf_event__synthesize_kernel_mmap(struct perf_tool *tool,
				       perf_event__handler_t process,
				       struct machine *machine,
				       const char *symbol_name)
{
	size_t size;
	const char *filename, *mmap_name;
	char path[PATH_MAX];
	char name_buff[PATH_MAX];
	struct map *map;
	int err;
	/*
	 * We should get this from /sys/kernel/sections/.text, but till that is
	 * available use this, and after it is use this as a fallback for older
	 * kernels.
	 */
	struct process_symbol_args args = { .name = symbol_name, };
	union perf_event *event = zalloc((sizeof(event->mmap) +
					  machine->id_hdr_size));
	if (event == NULL) {
		pr_debug("Not enough memory synthesizing mmap event "
			 "for kernel modules\n");
		return -1;
	}

	mmap_name = machine__mmap_name(machine, name_buff, sizeof(name_buff));
	if (machine__is_host(machine)) {
		/*
		 * kernel uses PERF_RECORD_MISC_USER for user space maps,
		 * see kernel/perf_event.c __perf_event_mmap
		 */
		event->header.misc = PERF_RECORD_MISC_KERNEL;
		filename = "/proc/kallsyms";
	} else {
		event->header.misc = PERF_RECORD_MISC_GUEST_KERNEL;
		if (machine__is_default_guest(machine))
			filename = (char *) symbol_conf.default_guest_kallsyms;
		else {
			sprintf(path, "%s/proc/kallsyms", machine->root_dir);
			filename = path;
		}
	}

	if (kallsyms__parse(filename, &args, find_symbol_cb) <= 0)
		return -ENOENT;

	map = machine->vmlinux_maps[MAP__FUNCTION];
	size = snprintf(event->mmap.filename, sizeof(event->mmap.filename),
			"%s%s", mmap_name, symbol_name) + 1;
	size = ALIGN(size, sizeof(u64));
	event->mmap.header.type = PERF_RECORD_MMAP;
	event->mmap.header.size = (sizeof(event->mmap) -
			(sizeof(event->mmap.filename) - size) + machine->id_hdr_size);
	event->mmap.pgoff = args.start;
	event->mmap.start = map->start;
	event->mmap.len   = map->end - event->mmap.start;
	event->mmap.pid   = machine->pid;

	err = process(tool, event, &synth_sample, machine);
	free(event);

	return err;
=======
static int find_any_symbol_cb(void *arg, const char *name,
			      char type __maybe_unused, u64 start)
{
	struct process_symbol_args *args = arg;

	if (strcmp(name, args->name))
		return 0;

	args->start = start;
	return 1;
}

int kallsyms__get_function_start(const char *kallsyms_filename,
				 const char *symbol_name, u64 *addr)
{
	struct process_symbol_args args = { .name = symbol_name, };

	if (kallsyms__parse(kallsyms_filename, &args, find_func_symbol_cb) <= 0)
		return -1;

	*addr = args.start;
	return 0;
}

int kallsyms__get_symbol_start(const char *kallsyms_filename,
			       const char *symbol_name, u64 *addr)
{
	struct process_symbol_args args = { .name = symbol_name, };

	if (kallsyms__parse(kallsyms_filename, &args, find_any_symbol_cb) <= 0)
		return -1;

	*addr = args.start;
	return 0;
}

void perf_event__read_stat_config(struct perf_stat_config *config,
				  struct perf_record_stat_config *event)
{
	unsigned i;

	for (i = 0; i < event->nr; i++) {

		switch (event->data[i].tag) {
#define CASE(__term, __val)					\
		case PERF_STAT_CONFIG_TERM__##__term:		\
			config->__val = event->data[i].val;	\
			break;

		CASE(AGGR_MODE,  aggr_mode)
		CASE(SCALE,      scale)
		CASE(INTERVAL,   interval)
		CASE(AGGR_LEVEL, aggr_level)
#undef CASE
		default:
			pr_warning("unknown stat config term %" PRI_lu64 "\n",
				   event->data[i].tag);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

size_t perf_event__fprintf_comm(union perf_event *event, FILE *fp)
{
<<<<<<< HEAD
	return fprintf(fp, ": %s:%d\n", event->comm.comm, event->comm.tid);
}

int perf_event__process_comm(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine)
{
	struct thread *thread = machine__findnew_thread(machine, event->comm.tid);

	if (dump_trace)
		perf_event__fprintf_comm(event, stdout);

	if (thread == NULL || thread__set_comm(thread, event->comm.comm)) {
		dump_printf("problem processing PERF_RECORD_COMM, skipping event.\n");
		return -1;
	}

	return 0;
}

int perf_event__process_lost(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine __used)
{
	dump_printf(": id:%" PRIu64 ": lost:%" PRIu64 "\n",
		    event->lost.id, event->lost.lost);
	return 0;
}

static void perf_event__set_kernel_mmap_len(union perf_event *event,
					    struct map **maps)
{
	maps[MAP__FUNCTION]->start = event->mmap.start;
	maps[MAP__FUNCTION]->end   = event->mmap.start + event->mmap.len;
	/*
	 * Be a bit paranoid here, some perf.data file came with
	 * a zero sized synthesized MMAP event for the kernel.
	 */
	if (maps[MAP__FUNCTION]->end == 0)
		maps[MAP__FUNCTION]->end = ~0ULL;
}

static int perf_event__process_kernel_mmap(struct perf_tool *tool __used,
					   union perf_event *event,
					   struct machine *machine)
{
	struct map *map;
	char kmmap_prefix[PATH_MAX];
	enum dso_kernel_type kernel_type;
	bool is_kernel_mmap;

	machine__mmap_name(machine, kmmap_prefix, sizeof(kmmap_prefix));
	if (machine__is_host(machine))
		kernel_type = DSO_TYPE_KERNEL;
	else
		kernel_type = DSO_TYPE_GUEST_KERNEL;

	is_kernel_mmap = memcmp(event->mmap.filename,
				kmmap_prefix,
				strlen(kmmap_prefix) - 1) == 0;
	if (event->mmap.filename[0] == '/' ||
	    (!is_kernel_mmap && event->mmap.filename[0] == '[')) {

		char short_module_name[1024];
		char *name, *dot;

		if (event->mmap.filename[0] == '/') {
			name = strrchr(event->mmap.filename, '/');
			if (name == NULL)
				goto out_problem;

			++name; /* skip / */
			dot = strrchr(name, '.');
			if (dot == NULL)
				goto out_problem;
			snprintf(short_module_name, sizeof(short_module_name),
					"[%.*s]", (int)(dot - name), name);
			strxfrchar(short_module_name, '-', '_');
		} else
			strcpy(short_module_name, event->mmap.filename);

		map = machine__new_module(machine, event->mmap.start,
					  event->mmap.filename);
		if (map == NULL)
			goto out_problem;

		name = strdup(short_module_name);
		if (name == NULL)
			goto out_problem;

		map->dso->short_name = name;
		map->dso->sname_alloc = 1;
		map->end = map->start + event->mmap.len;
	} else if (is_kernel_mmap) {
		const char *symbol_name = (event->mmap.filename +
				strlen(kmmap_prefix));
		/*
		 * Should be there already, from the build-id table in
		 * the header.
		 */
		struct dso *kernel = __dsos__findnew(&machine->kernel_dsos,
						     kmmap_prefix);
		if (kernel == NULL)
			goto out_problem;

		kernel->kernel = kernel_type;
		if (__machine__create_kernel_maps(machine, kernel) < 0)
			goto out_problem;

		perf_event__set_kernel_mmap_len(event, machine->vmlinux_maps);

		/*
		 * Avoid using a zero address (kptr_restrict) for the ref reloc
		 * symbol. Effectively having zero here means that at record
		 * time /proc/sys/kernel/kptr_restrict was non zero.
		 */
		if (event->mmap.pgoff != 0) {
			maps__set_kallsyms_ref_reloc_sym(machine->vmlinux_maps,
							 symbol_name,
							 event->mmap.pgoff);
		}

		if (machine__is_default_guest(machine)) {
			/*
			 * preload dso of guest kernel and modules
			 */
			dso__load(kernel, machine->vmlinux_maps[MAP__FUNCTION],
				  NULL);
		}
	}
	return 0;
out_problem:
	return -1;
=======
	const char *s;

	if (event->header.misc & PERF_RECORD_MISC_COMM_EXEC)
		s = " exec";
	else
		s = "";

	return fprintf(fp, "%s: %s:%d/%d\n", s, event->comm.comm, event->comm.pid, event->comm.tid);
}

size_t perf_event__fprintf_namespaces(union perf_event *event, FILE *fp)
{
	size_t ret = 0;
	struct perf_ns_link_info *ns_link_info;
	u32 nr_namespaces, idx;

	ns_link_info = event->namespaces.link_info;
	nr_namespaces = event->namespaces.nr_namespaces;

	ret += fprintf(fp, " %d/%d - nr_namespaces: %u\n\t\t[",
		       event->namespaces.pid,
		       event->namespaces.tid,
		       nr_namespaces);

	for (idx = 0; idx < nr_namespaces; idx++) {
		if (idx && (idx % 4 == 0))
			ret += fprintf(fp, "\n\t\t ");

		ret  += fprintf(fp, "%u/%s: %" PRIu64 "/%#" PRIx64 "%s", idx,
				perf_ns__name(idx), (u64)ns_link_info[idx].dev,
				(u64)ns_link_info[idx].ino,
				((idx + 1) != nr_namespaces) ? ", " : "]\n");
	}

	return ret;
}

size_t perf_event__fprintf_cgroup(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " cgroup: %" PRI_lu64 " %s\n",
		       event->cgroup.id, event->cgroup.path);
}

int perf_event__process_comm(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_comm_event(machine, event, sample);
}

int perf_event__process_namespaces(struct perf_tool *tool __maybe_unused,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	return machine__process_namespaces_event(machine, event, sample);
}

int perf_event__process_cgroup(struct perf_tool *tool __maybe_unused,
			       union perf_event *event,
			       struct perf_sample *sample,
			       struct machine *machine)
{
	return machine__process_cgroup_event(machine, event, sample);
}

int perf_event__process_lost(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_lost_event(machine, event, sample);
}

int perf_event__process_aux(struct perf_tool *tool __maybe_unused,
			    union perf_event *event,
			    struct perf_sample *sample __maybe_unused,
			    struct machine *machine)
{
	return machine__process_aux_event(machine, event);
}

int perf_event__process_itrace_start(struct perf_tool *tool __maybe_unused,
				     union perf_event *event,
				     struct perf_sample *sample __maybe_unused,
				     struct machine *machine)
{
	return machine__process_itrace_start_event(machine, event);
}

int perf_event__process_aux_output_hw_id(struct perf_tool *tool __maybe_unused,
					 union perf_event *event,
					 struct perf_sample *sample __maybe_unused,
					 struct machine *machine)
{
	return machine__process_aux_output_hw_id_event(machine, event);
}

int perf_event__process_lost_samples(struct perf_tool *tool __maybe_unused,
				     union perf_event *event,
				     struct perf_sample *sample,
				     struct machine *machine)
{
	return machine__process_lost_samples_event(machine, event, sample);
}

int perf_event__process_switch(struct perf_tool *tool __maybe_unused,
			       union perf_event *event,
			       struct perf_sample *sample __maybe_unused,
			       struct machine *machine)
{
	return machine__process_switch_event(machine, event);
}

int perf_event__process_ksymbol(struct perf_tool *tool __maybe_unused,
				union perf_event *event,
				struct perf_sample *sample __maybe_unused,
				struct machine *machine)
{
	return machine__process_ksymbol(machine, event, sample);
}

int perf_event__process_bpf(struct perf_tool *tool __maybe_unused,
			    union perf_event *event,
			    struct perf_sample *sample,
			    struct machine *machine)
{
	return machine__process_bpf(machine, event, sample);
}

int perf_event__process_text_poke(struct perf_tool *tool __maybe_unused,
				  union perf_event *event,
				  struct perf_sample *sample,
				  struct machine *machine)
{
	return machine__process_text_poke(machine, event, sample);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

size_t perf_event__fprintf_mmap(union perf_event *event, FILE *fp)
{
<<<<<<< HEAD
	return fprintf(fp, " %d/%d: [%#" PRIx64 "(%#" PRIx64 ") @ %#" PRIx64 "]: %s\n",
		       event->mmap.pid, event->mmap.tid, event->mmap.start,
		       event->mmap.len, event->mmap.pgoff, event->mmap.filename);
}

int perf_event__process_mmap(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine)
{
	struct thread *thread;
	struct map *map;
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	int ret = 0;

	if (dump_trace)
		perf_event__fprintf_mmap(event, stdout);

	if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL ||
	    cpumode == PERF_RECORD_MISC_KERNEL) {
		ret = perf_event__process_kernel_mmap(tool, event, machine);
		if (ret < 0)
			goto out_problem;
		return 0;
	}

	thread = machine__findnew_thread(machine, event->mmap.pid);
	if (thread == NULL)
		goto out_problem;
	map = map__new(&machine->user_dsos, event->mmap.start,
			event->mmap.len, event->mmap.pgoff,
			event->mmap.pid, event->mmap.filename,
			MAP__FUNCTION);
	if (map == NULL)
		goto out_problem;

	thread__insert_map(thread, map);
	return 0;

out_problem:
	dump_printf("problem processing PERF_RECORD_MMAP, skipping event.\n");
	return 0;
=======
	return fprintf(fp, " %d/%d: [%#" PRI_lx64 "(%#" PRI_lx64 ") @ %#" PRI_lx64 "]: %c %s\n",
		       event->mmap.pid, event->mmap.tid, event->mmap.start,
		       event->mmap.len, event->mmap.pgoff,
		       (event->header.misc & PERF_RECORD_MISC_MMAP_DATA) ? 'r' : 'x',
		       event->mmap.filename);
}

size_t perf_event__fprintf_mmap2(union perf_event *event, FILE *fp)
{
	if (event->header.misc & PERF_RECORD_MISC_MMAP_BUILD_ID) {
		char sbuild_id[SBUILD_ID_SIZE];
		struct build_id bid;

		build_id__init(&bid, event->mmap2.build_id,
			       event->mmap2.build_id_size);
		build_id__sprintf(&bid, sbuild_id);

		return fprintf(fp, " %d/%d: [%#" PRI_lx64 "(%#" PRI_lx64 ") @ %#" PRI_lx64
				   " <%s>]: %c%c%c%c %s\n",
			       event->mmap2.pid, event->mmap2.tid, event->mmap2.start,
			       event->mmap2.len, event->mmap2.pgoff, sbuild_id,
			       (event->mmap2.prot & PROT_READ) ? 'r' : '-',
			       (event->mmap2.prot & PROT_WRITE) ? 'w' : '-',
			       (event->mmap2.prot & PROT_EXEC) ? 'x' : '-',
			       (event->mmap2.flags & MAP_SHARED) ? 's' : 'p',
			       event->mmap2.filename);
	} else {
		return fprintf(fp, " %d/%d: [%#" PRI_lx64 "(%#" PRI_lx64 ") @ %#" PRI_lx64
				   " %02x:%02x %"PRI_lu64" %"PRI_lu64"]: %c%c%c%c %s\n",
			       event->mmap2.pid, event->mmap2.tid, event->mmap2.start,
			       event->mmap2.len, event->mmap2.pgoff, event->mmap2.maj,
			       event->mmap2.min, event->mmap2.ino,
			       event->mmap2.ino_generation,
			       (event->mmap2.prot & PROT_READ) ? 'r' : '-',
			       (event->mmap2.prot & PROT_WRITE) ? 'w' : '-',
			       (event->mmap2.prot & PROT_EXEC) ? 'x' : '-',
			       (event->mmap2.flags & MAP_SHARED) ? 's' : 'p',
			       event->mmap2.filename);
	}
}

size_t perf_event__fprintf_thread_map(union perf_event *event, FILE *fp)
{
	struct perf_thread_map *threads = thread_map__new_event(&event->thread_map);
	size_t ret;

	ret = fprintf(fp, " nr: ");

	if (threads)
		ret += thread_map__fprintf(threads, fp);
	else
		ret += fprintf(fp, "failed to get threads from event\n");

	perf_thread_map__put(threads);
	return ret;
}

size_t perf_event__fprintf_cpu_map(union perf_event *event, FILE *fp)
{
	struct perf_cpu_map *cpus = cpu_map__new_data(&event->cpu_map.data);
	size_t ret;

	ret = fprintf(fp, ": ");

	if (cpus)
		ret += cpu_map__fprintf(cpus, fp);
	else
		ret += fprintf(fp, "failed to get cpumap from event\n");

	perf_cpu_map__put(cpus);
	return ret;
}

int perf_event__process_mmap(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_mmap_event(machine, event, sample);
}

int perf_event__process_mmap2(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_mmap2_event(machine, event, sample);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

size_t perf_event__fprintf_task(union perf_event *event, FILE *fp)
{
	return fprintf(fp, "(%d:%d):(%d:%d)\n",
		       event->fork.pid, event->fork.tid,
		       event->fork.ppid, event->fork.ptid);
}

<<<<<<< HEAD
int perf_event__process_task(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			      struct machine *machine)
{
	struct thread *thread = machine__findnew_thread(machine, event->fork.tid);
	struct thread *parent = machine__findnew_thread(machine, event->fork.ptid);

	if (dump_trace)
		perf_event__fprintf_task(event, stdout);

	if (event->header.type == PERF_RECORD_EXIT) {
		machine__remove_thread(machine, thread);
		return 0;
	}

	if (thread == NULL || parent == NULL ||
	    thread__fork(thread, parent) < 0) {
		dump_printf("problem processing PERF_RECORD_FORK, skipping event.\n");
		return -1;
	}

	return 0;
}

size_t perf_event__fprintf(union perf_event *event, FILE *fp)
=======
int perf_event__process_fork(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_fork_event(machine, event, sample);
}

int perf_event__process_exit(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_exit_event(machine, event, sample);
}

size_t perf_event__fprintf_aux(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " offset: %#"PRI_lx64" size: %#"PRI_lx64" flags: %#"PRI_lx64" [%s%s%s]\n",
		       event->aux.aux_offset, event->aux.aux_size,
		       event->aux.flags,
		       event->aux.flags & PERF_AUX_FLAG_TRUNCATED ? "T" : "",
		       event->aux.flags & PERF_AUX_FLAG_OVERWRITE ? "O" : "",
		       event->aux.flags & PERF_AUX_FLAG_PARTIAL   ? "P" : "");
}

size_t perf_event__fprintf_itrace_start(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " pid: %u tid: %u\n",
		       event->itrace_start.pid, event->itrace_start.tid);
}

size_t perf_event__fprintf_aux_output_hw_id(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " hw_id: %#"PRI_lx64"\n",
		       event->aux_output_hw_id.hw_id);
}

size_t perf_event__fprintf_switch(union perf_event *event, FILE *fp)
{
	bool out = event->header.misc & PERF_RECORD_MISC_SWITCH_OUT;
	const char *in_out = !out ? "IN         " :
		!(event->header.misc & PERF_RECORD_MISC_SWITCH_OUT_PREEMPT) ?
				    "OUT        " : "OUT preempt";

	if (event->header.type == PERF_RECORD_SWITCH)
		return fprintf(fp, " %s\n", in_out);

	return fprintf(fp, " %s  %s pid/tid: %5d/%-5d\n",
		       in_out, out ? "next" : "prev",
		       event->context_switch.next_prev_pid,
		       event->context_switch.next_prev_tid);
}

static size_t perf_event__fprintf_lost(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " lost %" PRI_lu64 "\n", event->lost.lost);
}

size_t perf_event__fprintf_ksymbol(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " addr %" PRI_lx64 " len %u type %u flags 0x%x name %s\n",
		       event->ksymbol.addr, event->ksymbol.len,
		       event->ksymbol.ksym_type,
		       event->ksymbol.flags, event->ksymbol.name);
}

size_t perf_event__fprintf_bpf(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " type %u, flags %u, id %u\n",
		       event->bpf.type, event->bpf.flags, event->bpf.id);
}

static int text_poke_printer(enum binary_printer_ops op, unsigned int val,
			     void *extra, FILE *fp)
{
	bool old = *(bool *)extra;

	switch ((int)op) {
	case BINARY_PRINT_LINE_BEGIN:
		return fprintf(fp, "            %s bytes:", old ? "Old" : "New");
	case BINARY_PRINT_NUM_DATA:
		return fprintf(fp, " %02x", val);
	case BINARY_PRINT_LINE_END:
		return fprintf(fp, "\n");
	default:
		return 0;
	}
}

size_t perf_event__fprintf_text_poke(union perf_event *event, struct machine *machine, FILE *fp)
{
	struct perf_record_text_poke_event *tp = &event->text_poke;
	size_t ret;
	bool old;

	ret = fprintf(fp, " %" PRI_lx64 " ", tp->addr);
	if (machine) {
		struct addr_location al;

		addr_location__init(&al);
		al.map = maps__find(machine__kernel_maps(machine), tp->addr);
		if (al.map && map__load(al.map) >= 0) {
			al.addr = map__map_ip(al.map, tp->addr);
			al.sym = map__find_symbol(al.map, al.addr);
			if (al.sym)
				ret += symbol__fprintf_symname_offs(al.sym, &al, fp);
		}
		addr_location__exit(&al);
	}
	ret += fprintf(fp, " old len %u new len %u\n", tp->old_len, tp->new_len);
	old = true;
	ret += binary__fprintf(tp->bytes, tp->old_len, 16, text_poke_printer,
			       &old, fp);
	old = false;
	ret += binary__fprintf(tp->bytes + tp->old_len, tp->new_len, 16,
			       text_poke_printer, &old, fp);
	return ret;
}

size_t perf_event__fprintf(union perf_event *event, struct machine *machine, FILE *fp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	size_t ret = fprintf(fp, "PERF_RECORD_%s",
			     perf_event__name(event->header.type));

	switch (event->header.type) {
	case PERF_RECORD_COMM:
		ret += perf_event__fprintf_comm(event, fp);
		break;
	case PERF_RECORD_FORK:
	case PERF_RECORD_EXIT:
		ret += perf_event__fprintf_task(event, fp);
		break;
	case PERF_RECORD_MMAP:
		ret += perf_event__fprintf_mmap(event, fp);
		break;
<<<<<<< HEAD
=======
	case PERF_RECORD_NAMESPACES:
		ret += perf_event__fprintf_namespaces(event, fp);
		break;
	case PERF_RECORD_CGROUP:
		ret += perf_event__fprintf_cgroup(event, fp);
		break;
	case PERF_RECORD_MMAP2:
		ret += perf_event__fprintf_mmap2(event, fp);
		break;
	case PERF_RECORD_AUX:
		ret += perf_event__fprintf_aux(event, fp);
		break;
	case PERF_RECORD_ITRACE_START:
		ret += perf_event__fprintf_itrace_start(event, fp);
		break;
	case PERF_RECORD_SWITCH:
	case PERF_RECORD_SWITCH_CPU_WIDE:
		ret += perf_event__fprintf_switch(event, fp);
		break;
	case PERF_RECORD_LOST:
		ret += perf_event__fprintf_lost(event, fp);
		break;
	case PERF_RECORD_KSYMBOL:
		ret += perf_event__fprintf_ksymbol(event, fp);
		break;
	case PERF_RECORD_BPF_EVENT:
		ret += perf_event__fprintf_bpf(event, fp);
		break;
	case PERF_RECORD_TEXT_POKE:
		ret += perf_event__fprintf_text_poke(event, machine, fp);
		break;
	case PERF_RECORD_AUX_OUTPUT_HW_ID:
		ret += perf_event__fprintf_aux_output_hw_id(event, fp);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		ret += fprintf(fp, "\n");
	}

	return ret;
}

<<<<<<< HEAD
int perf_event__process(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine)
{
	switch (event->header.type) {
	case PERF_RECORD_COMM:
		perf_event__process_comm(tool, event, sample, machine);
		break;
	case PERF_RECORD_MMAP:
		perf_event__process_mmap(tool, event, sample, machine);
		break;
	case PERF_RECORD_FORK:
	case PERF_RECORD_EXIT:
		perf_event__process_task(tool, event, sample, machine);
		break;
	case PERF_RECORD_LOST:
		perf_event__process_lost(tool, event, sample, machine);
	default:
		break;
	}

	return 0;
}

void thread__find_addr_map(struct thread *self,
			   struct machine *machine, u8 cpumode,
			   enum map_type type, u64 addr,
			   struct addr_location *al)
{
	struct map_groups *mg = &self->mg;

	al->thread = self;
	al->addr = addr;
	al->cpumode = cpumode;
	al->filtered = false;

	if (machine == NULL) {
		al->map = NULL;
		return;
	}

	if (cpumode == PERF_RECORD_MISC_KERNEL && perf_host) {
		al->level = 'k';
		mg = &machine->kmaps;
=======
int perf_event__process(struct perf_tool *tool __maybe_unused,
			union perf_event *event,
			struct perf_sample *sample,
			struct machine *machine)
{
	return machine__process_event(machine, event, sample);
}

struct map *thread__find_map(struct thread *thread, u8 cpumode, u64 addr,
			     struct addr_location *al)
{
	struct maps *maps = thread__maps(thread);
	struct machine *machine = maps__machine(maps);
	bool load_map = false;

	maps__zput(al->maps);
	map__zput(al->map);
	thread__zput(al->thread);
	al->thread = thread__get(thread);

	al->addr = addr;
	al->cpumode = cpumode;
	al->filtered = 0;

	if (machine == NULL)
		return NULL;

	if (cpumode == PERF_RECORD_MISC_KERNEL && perf_host) {
		al->level = 'k';
		maps = machine__kernel_maps(machine);
		load_map = !symbol_conf.lazy_load_kernel_maps;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (cpumode == PERF_RECORD_MISC_USER && perf_host) {
		al->level = '.';
	} else if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL && perf_guest) {
		al->level = 'g';
<<<<<<< HEAD
		mg = &machine->kmaps;
	} else {
		/*
		 * 'u' means guest os user space.
		 * TODO: We don't support guest user space. Might support late.
		 */
		if (cpumode == PERF_RECORD_MISC_GUEST_USER && perf_guest)
			al->level = 'u';
		else
			al->level = 'H';
		al->map = NULL;
=======
		maps = machine__kernel_maps(machine);
		load_map = !symbol_conf.lazy_load_kernel_maps;
	} else if (cpumode == PERF_RECORD_MISC_GUEST_USER && perf_guest) {
		al->level = 'u';
	} else {
		al->level = 'H';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if ((cpumode == PERF_RECORD_MISC_GUEST_USER ||
			cpumode == PERF_RECORD_MISC_GUEST_KERNEL) &&
			!perf_guest)
<<<<<<< HEAD
			al->filtered = true;
		if ((cpumode == PERF_RECORD_MISC_USER ||
			cpumode == PERF_RECORD_MISC_KERNEL) &&
			!perf_host)
			al->filtered = true;

		return;
	}
try_again:
	al->map = map_groups__find(mg, type, al->addr);
	if (al->map == NULL) {
		/*
		 * If this is outside of all known maps, and is a negative
		 * address, try to look it up in the kernel dso, as it might be
		 * a vsyscall or vdso (which executes in user-mode).
		 *
		 * XXX This is nasty, we should have a symbol list in the
		 * "[vdso]" dso, but for now lets use the old trick of looking
		 * in the whole kernel symbol list.
		 */
		if ((long long)al->addr < 0 &&
		    cpumode == PERF_RECORD_MISC_USER &&
		    machine && mg != &machine->kmaps) {
			mg = &machine->kmaps;
			goto try_again;
		}
	} else
		al->addr = al->map->map_ip(al->map, al->addr);
}

void thread__find_addr_location(struct thread *thread, struct machine *machine,
				u8 cpumode, enum map_type type, u64 addr,
				struct addr_location *al,
				symbol_filter_t filter)
{
	thread__find_addr_map(thread, machine, cpumode, type, addr, al);
	if (al->map != NULL)
		al->sym = map__find_symbol(al->map, al->addr, filter);
	else
		al->sym = NULL;
}

int perf_event__preprocess_sample(const union perf_event *event,
				  struct machine *machine,
				  struct addr_location *al,
				  struct perf_sample *sample,
				  symbol_filter_t filter)
{
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	struct thread *thread = machine__findnew_thread(machine, event->ip.pid);

	if (thread == NULL)
		return -1;

	if (symbol_conf.comm_list &&
	    !strlist__has_entry(symbol_conf.comm_list, thread->comm))
		goto out_filtered;

	dump_printf(" ... thread: %s:%d\n", thread->comm, thread->pid);
	/*
	 * Have we already created the kernel maps for this machine?
	 *
	 * This should have happened earlier, when we processed the kernel MMAP
	 * events, but for older perf.data files there was no such thing, so do
	 * it now.
	 */
	if (cpumode == PERF_RECORD_MISC_KERNEL &&
	    machine->vmlinux_maps[MAP__FUNCTION] == NULL)
		machine__create_kernel_maps(machine);

	thread__find_addr_map(thread, machine, cpumode, MAP__FUNCTION,
			      event->ip.ip, al);
	dump_printf(" ...... dso: %s\n",
		    al->map ? al->map->dso->long_name :
			al->level == 'H' ? "[hypervisor]" : "<not found>");
	al->sym = NULL;
	al->cpu = sample->cpu;

	if (al->map) {
		struct dso *dso = al->map->dso;

=======
			al->filtered |= (1 << HIST_FILTER__GUEST);
		if ((cpumode == PERF_RECORD_MISC_USER ||
			cpumode == PERF_RECORD_MISC_KERNEL) &&
			!perf_host)
			al->filtered |= (1 << HIST_FILTER__HOST);

		return NULL;
	}
	al->maps = maps__get(maps);
	al->map = maps__find(maps, al->addr);
	if (al->map != NULL) {
		/*
		 * Kernel maps might be changed when loading symbols so loading
		 * must be done prior to using kernel maps.
		 */
		if (load_map)
			map__load(al->map);
		al->addr = map__map_ip(al->map, al->addr);
	}

	return al->map;
}

/*
 * For branch stacks or branch samples, the sample cpumode might not be correct
 * because it applies only to the sample 'ip' and not necessary to 'addr' or
 * branch stack addresses. If possible, use a fallback to deal with those cases.
 */
struct map *thread__find_map_fb(struct thread *thread, u8 cpumode, u64 addr,
				struct addr_location *al)
{
	struct map *map = thread__find_map(thread, cpumode, addr, al);
	struct machine *machine = maps__machine(thread__maps(thread));
	u8 addr_cpumode = machine__addr_cpumode(machine, cpumode, addr);

	if (map || addr_cpumode == cpumode)
		return map;

	return thread__find_map(thread, addr_cpumode, addr, al);
}

struct symbol *thread__find_symbol(struct thread *thread, u8 cpumode,
				   u64 addr, struct addr_location *al)
{
	al->sym = NULL;
	if (thread__find_map(thread, cpumode, addr, al))
		al->sym = map__find_symbol(al->map, al->addr);
	return al->sym;
}

struct symbol *thread__find_symbol_fb(struct thread *thread, u8 cpumode,
				      u64 addr, struct addr_location *al)
{
	al->sym = NULL;
	if (thread__find_map_fb(thread, cpumode, addr, al))
		al->sym = map__find_symbol(al->map, al->addr);
	return al->sym;
}

static bool check_address_range(struct intlist *addr_list, int addr_range,
				unsigned long addr)
{
	struct int_node *pos;

	intlist__for_each_entry(pos, addr_list) {
		if (addr >= pos->i && addr < pos->i + addr_range)
			return true;
	}

	return false;
}

/*
 * Callers need to drop the reference to al->thread, obtained in
 * machine__findnew_thread()
 */
int machine__resolve(struct machine *machine, struct addr_location *al,
		     struct perf_sample *sample)
{
	struct thread *thread;
	struct dso *dso;

	if (symbol_conf.guest_code && !machine__is_host(machine))
		thread = machine__findnew_guest_code(machine, sample->pid);
	else
		thread = machine__findnew_thread(machine, sample->pid, sample->tid);
	if (thread == NULL)
		return -1;

	dump_printf(" ... thread: %s:%d\n", thread__comm_str(thread), thread__tid(thread));
	thread__find_map(thread, sample->cpumode, sample->ip, al);
	dso = al->map ? map__dso(al->map) : NULL;
	dump_printf(" ...... dso: %s\n",
		dso
		? dso->long_name
		: (al->level == 'H' ? "[hypervisor]" : "<not found>"));

	if (thread__is_filtered(thread))
		al->filtered |= (1 << HIST_FILTER__THREAD);

	thread__put(thread);
	thread = NULL;

	al->sym = NULL;
	al->cpu = sample->cpu;
	al->socket = -1;
	al->srcline = NULL;

	if (al->cpu >= 0) {
		struct perf_env *env = machine->env;

		if (env && env->cpu)
			al->socket = env->cpu[al->cpu].socket_id;
	}

	if (al->map) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (symbol_conf.dso_list &&
		    (!dso || !(strlist__has_entry(symbol_conf.dso_list,
						  dso->short_name) ||
			       (dso->short_name != dso->long_name &&
				strlist__has_entry(symbol_conf.dso_list,
<<<<<<< HEAD
						   dso->long_name)))))
			goto out_filtered;

		al->sym = map__find_symbol(al->map, al->addr, filter);
	}

	if (symbol_conf.sym_list && al->sym &&
	    !strlist__has_entry(symbol_conf.sym_list, al->sym->name))
		goto out_filtered;

	return 0;

out_filtered:
	al->filtered = true;
	return 0;
}
=======
						   dso->long_name))))) {
			al->filtered |= (1 << HIST_FILTER__DSO);
		}

		al->sym = map__find_symbol(al->map, al->addr);
	} else if (symbol_conf.dso_list) {
		al->filtered |= (1 << HIST_FILTER__DSO);
	}

	if (symbol_conf.sym_list) {
		int ret = 0;
		char al_addr_str[32];
		size_t sz = sizeof(al_addr_str);

		if (al->sym) {
			ret = strlist__has_entry(symbol_conf.sym_list,
						al->sym->name);
		}
		if (!ret && al->sym) {
			snprintf(al_addr_str, sz, "0x%"PRIx64,
				 map__unmap_ip(al->map, al->sym->start));
			ret = strlist__has_entry(symbol_conf.sym_list,
						al_addr_str);
		}
		if (!ret && symbol_conf.addr_list && al->map) {
			unsigned long addr = map__unmap_ip(al->map, al->addr);

			ret = intlist__has_entry(symbol_conf.addr_list, addr);
			if (!ret && symbol_conf.addr_range) {
				ret = check_address_range(symbol_conf.addr_list,
							  symbol_conf.addr_range,
							  addr);
			}
		}

		if (!ret)
			al->filtered |= (1 << HIST_FILTER__SYMBOL);
	}

	return 0;
}

bool is_bts_event(struct perf_event_attr *attr)
{
	return attr->type == PERF_TYPE_HARDWARE &&
	       (attr->config & PERF_COUNT_HW_BRANCH_INSTRUCTIONS) &&
	       attr->sample_period == 1;
}

bool sample_addr_correlates_sym(struct perf_event_attr *attr)
{
	if (attr->type == PERF_TYPE_SOFTWARE &&
	    (attr->config == PERF_COUNT_SW_PAGE_FAULTS ||
	     attr->config == PERF_COUNT_SW_PAGE_FAULTS_MIN ||
	     attr->config == PERF_COUNT_SW_PAGE_FAULTS_MAJ))
		return true;

	if (is_bts_event(attr))
		return true;

	return false;
}

void thread__resolve(struct thread *thread, struct addr_location *al,
		     struct perf_sample *sample)
{
	thread__find_map_fb(thread, sample->cpumode, sample->addr, al);

	al->cpu = sample->cpu;
	al->sym = NULL;

	if (al->map)
		al->sym = map__find_symbol(al->map, al->addr);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
